#pragma once

#include <cstdio>
#include <cassert>

#include "models/Orders.hpp"
#include "types.hpp"
#include "StreamFile.hpp"
#include "Header.hpp"
#include "Coder.hpp"
#include "Predictor.hpp"

namespace paqFe::internal {

template<typename ... Models>
class CompressEngine {
protected:
  const OpMode m;
  FILE *f = NULL;
  Header header;

  size_t n_byte_processed = 0;
  Coder coder;
  
  Predictor<1, Models...> predictor;
  
  uint8_t buf[32];
  size_t buf_size = 0;
  Prob prob_next = ProbEven;
  uint32_t decode_pos;

public:
  CompressEngine(const char* pathname, OpMode m)
    : m(m) {
    
    if(m == OpMode::Compress) {
      open_as_output(pathname);
    } else if (m == OpMode::Decompress) {
      open_as_input(pathname);

      decode_pos = get_byte();
      decode_pos = (decode_pos << 8) | get_byte();
      decode_pos = (decode_pos << 8) | get_byte();
      decode_pos = (decode_pos << 8) | get_byte();
    }
  };

  size_t size() {
    return header.origin_size.dw;
  };

  size_t read(uint8_t *dst, size_t n) {
    assert(m == OpMode::Decompress);

    size_t n_byte_left = size() - n_byte_processed;
    n = n > n_byte_left ? n_byte_left : n;

    uint8_t byte;
    for(int i=0;i<n;i++) {
      byte = 0;    
      for(int j=0;j<8;j++) {
        size_t n = coder.decode(decode_pos, prob_next, &byte);

        while(n--) {
          decode_pos = (decode_pos << 8) | get_byte();
        }

        uint8_t bit = byte & 0x1;
        predictor.predict(bit, &prob_next);
      }

      dst[i] = byte;
    }
    n_byte_processed += n;
    return n;
  };

  size_t write(const uint8_t *src, size_t n) {
    assert(m == OpMode::Compress);

    for(int i=0;i<n;i++) {
      uint8_t byte = src[i];
      for(int j=7;j>=0;j--) {
        uint8_t bit = (byte >> j) & 0x1;
        buf_size += coder.encode(bit, prob_next, &buf[buf_size]);
        predictor.predict(bit, &prob_next);
      }

      if(buf_size > 16) {
        buf_size -= write_file(buf, buf_size);
      }
    }

    buf_size -= write_file(buf, buf_size);

    n_byte_processed += n;
    return n;
  };

  int close() {
    if(m == OpMode::Compress) {
      header.origin_size.dw = n_byte_processed;

      write_tail();
      write_header();
    }

    return fclose(f);
  }

private:
  int open_as_input(const char* pathname) {
    f = fopen(pathname, "rb");
    if(!f)
      return 0;
    
    read_file(&header, sizeof(Header));
    return 1;
  }

  int open_as_output(const char* pathname) {
    f = fopen(pathname, "wb");
    if(!f)
      return 0;
    
    write_file(&header, sizeof(Header));
    return 1;
  }


  size_t read_file(void *src, size_t size) {
    return fread(src, 1, size, f);
  };

  size_t write_file(const void *dst, size_t size) {
    return fwrite(dst, 1, size, f);
  };

  uint8_t get_byte() {
    uint8_t buf;
    if(read_file(&buf, 1))
        return buf;
      else 
        return 0xFF;
  }


  int write_header() {
#ifdef LITTLE_ENDIAN

    size_t pos = ftell(f);
    fseek(f, 0, SEEK_SET);
    int n = write_file(&header, sizeof(Header));

    fseek(f, pos, SEEK_SET);
    return n == sizeof(Header);
#else

#error

#endif
  }

  int read_header() {
    size_t pos = ftell(f);
    fseek(f, 0, SEEK_SET);
    int n = read_file(&header, sizeof(Header));

    fseek(f, pos, SEEK_SET);
    return n == sizeof(Header);
  }

  int write_tail() {
    uint32_t tail = coder.get_tail();

    uint8_t data[4] = {
      (uint8_t)((tail >> 24) & 0xFF),
      (uint8_t)((tail >> 16) & 0xFF),
      (uint8_t)((tail >> 8) & 0xFF),
      (uint8_t)((tail >> 0) & 0xFF),
    };

    return write_file(data, 4);
  }
public:

};


template<int N, typename ... Models>
class CompressEngineNw {
protected:
  const OpMode m;
  
  Header header;  
  size_t n_byte_processed = 0;

  int coder_duty = 0;
  Coder coder[N];
  Prob prob_next[N] = { ProbEven };
  uint32_t decode_pos[N];

  //Predictor<N, Models...> predictor;
  PredictorWithOutMixer<Models...> predictor;

  Stream<N> stream;

public:
  CompressEngineNw(const char* pathname, OpMode m) 
    : m(m) {
    
    if(m == OpMode::Compress) {
      stream.out.open(pathname);

    } else if (m == OpMode::Decompress) {
      stream.in.open(pathname);

      stream.in.read_header(&header);

      for(int i=0;i<N;i++) {
        uint32_t pos = stream.in.read_byte(i);
        pos = (pos << 8) | stream.in.read_byte(i);
        pos = (pos << 8) | stream.in.read_byte(i);
        pos = (pos << 8) | stream.in.read_byte(i);

        decode_pos[i] = pos;
      }
    }
  };

  size_t size() {
    return header.origin_size.dw;
  };

  size_t read(uint8_t *dst, size_t n) {
    assert(m == OpMode::Decompress);

    size_t n_byte_left = size() - n_byte_processed;
    n = n > n_byte_left ? n_byte_left : n;

    for(int i=0;i<n;i++) {
      uint8_t byte = 0;
      for(int j=7;j>=0;j--) {
        uint32_t pos = decode_pos[coder_duty];

        if(coder[coder_duty].decode(pos, prob_next[coder_duty], &byte))
          decode_pos[coder_duty] = (pos << 8) | stream.in.read_byte(coder_duty);

        coder_duty = (coder_duty + 1) % N;
        uint8_t bit = byte & 0x1;
        predictor.predict(bit, &prob_next[coder_duty]);
      }

      dst[i] = byte;
    }

    n_byte_processed += n;
    return n;
  };

  size_t write(const uint8_t *src, size_t n) {
    assert(m == OpMode::Compress);

    uint8_t out_byte;
    Prob p[8];
    for(int i=0;i<n;i++) {
      uint8_t byte = src[i];
      predictor.predict_byte(byte, p);

      for(int j=7;j>=0;j--) {
        uint8_t bit = (byte >> j) & 0x1;

        if(coder[coder_duty].encode(bit, p[7 - j], &out_byte))
          stream.out.write_byte(out_byte, coder_duty);
        
        coder_duty = (coder_duty + 1) % N;
      }
    }

    n_byte_processed += n;
    return n;
  };

  int close() {
    if(m == OpMode::Compress) {
      header.origin_size.dw = n_byte_processed;

      write_tail();

      stream.out.write_header(&header);
      return stream.out.close();
    } else {
      return stream.in.close();
    }
  }

protected:
  void write_tail() {
    for(int i=0;i<N;i++) {
      uint32_t tail = coder[i].get_tail();

      stream.out.write_byte((tail >> 24) & 0xFF, i);
      stream.out.write_byte((tail >> 16) & 0xFF, i);
      stream.out.write_byte((tail >> 8) & 0xFF, i);
      stream.out.write_byte((tail >> 0) & 0xFF, i);
    }
  }
public:
  size_t read(char *dst, size_t n) {
    return read((uint8_t*)(dst), n);
  }
  size_t write(const char *src, size_t n) {
    return write((const uint8_t*)(src), n);
  }
};


typedef CompressEngineNw<8, Order1> Comressor;

}