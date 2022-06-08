#pragma once

#include <cstdio>
#include <cassert>

#include "types.hpp"
#include "StreamFile.hpp"
#include "Header.hpp"
#include "Coder.hpp"
#include "Predictor.hpp"
#include "models/CRC.hpp"

namespace paqFe::internal {

template<int N, typename Predictor>
class CompressEngineNw {
protected:
  const OpMode m;
  
  Header header;  
  size_t n_byte_processed = 0;
  CRC<32> crc;
  uint32_t check_sum = 0;

  int coder_duty = 0;
  Coder coder[N];
  Prob prob_next[N] = { ProbEven };
  uint32_t decode_pos[N];

  Predictor predictor;
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

  ssize_t read(uint8_t *dst, size_t n) {
    assert(m == OpMode::Decompress);

    ssize_t n_byte_left = size() - n_byte_processed;
    n = n > n_byte_left ? n_byte_left : n;

    for(size_t i=0;i<n;i++) {
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

    for(size_t i=0;i<n;i++) {
      check_sum = crc.next(check_sum, dst[i]);
    }

    n_byte_processed += n;

    if(n_byte_processed == size()) {
      if(check_sum != header.crc32_check_sum.dw) {
        n = -n;
      }
    }

    return n;
  };

  ssize_t write(const uint8_t *src, size_t n) {
    assert(m == OpMode::Compress);

    uint8_t out_byte;
    Prob p[8];
    for(size_t i=0;i<n;i++) {
      uint8_t byte = src[i];
      predictor.predict_byte(byte, p);

      for(int j=7;j>=0;j--) {
        uint8_t bit = (byte >> j) & 0x1;

        if(coder[coder_duty].encode(bit, p[7 - j], &out_byte))
          stream.out.write_byte(out_byte, coder_duty);
        
        coder_duty = (coder_duty + 1) % N;
      }
    }

    for(size_t i=0;i<n;i++) {
      check_sum = crc.next(check_sum, src[i]);
    }

    n_byte_processed += n;
    return n;
  };

  int close() {
    if(m == OpMode::Compress) {
      header.origin_size.dw = n_byte_processed;
      header.crc32_check_sum.dw = check_sum;

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

}