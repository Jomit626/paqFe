#pragma once

#include <cstdio>
#include <cassert>

#include "types.hpp"
#include "Header.hpp"
#include "Coder.hpp"
#include "Predictor.hpp"

namespace paqFe {

template<typename ... Models>
class Engine {
  Predictor<Models...> predictor;
};

template<typename ... Models>
class CompressEngine {
  const OpMode m;
  FILE *f = NULL;
  Header header;

  size_t n_byte_processed = 0;
  Coder coder;
  
  Predictor<Models...> predictor;
  
  uint8_t buf[32];
  size_t buf_size = 0;
  Prob prob_next = ProbEven;
  uint32_t decode_pos;

public:
  CompressEngine(const char* pathname, OpMode m)
    : m(m) {
    
    if(m == OpMode::Comresss) {
      open_as_output(pathname);
    } else if (m == OpMode::Decompress) {
      open_as_input(pathname);

      decode_pos = get_byte();
      decode_pos = (decode_pos << 8) | get_byte();
      decode_pos = (decode_pos << 8) | get_byte();
      decode_pos = (decode_pos << 8) | get_byte();
    }

    coder.init();
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
    assert(m == OpMode::Comresss);

    Prob p;

    for(int i=0;i<n;i++) {
      uint8_t byte = src[i];
      for(int j=7;j>=0;j--) {
        uint8_t bit = (byte >> j) & 0x1;
        predictor.predict(bit, &p);
        buf_size += coder.encode(bit, p, &buf[buf_size]);
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
    if(m == OpMode::Comresss) {
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

};

typedef CompressEngine<MatchModel<8192,8192,32>> Comressor;

}