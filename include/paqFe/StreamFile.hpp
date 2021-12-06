#pragma once

#include "types.hpp"

#include "Header.hpp"
#include <cstdint>
#include <cstdio>
#include <cassert>

namespace paqFe {

template<size_t N, size_t BufSize = 1024>
class FineGridOStream {

  using DataPack = uint8_t[N];
  static_assert(isPow2(BufSize));
  static constexpr int BufMask = BufSize - 1;

  DataPack buf[BufSize];
  size_t buf_start = 0;
  size_t buf_len[N] = {0};

  FILE* f = nullptr;

  size_t write_file(const void *src, size_t size) {
    return fwrite(src, 1, size, f);
  };

  size_t write_buf2file() {
    size_t avail = BufSize;
    for(int i=0;i<N;i++) {  // avail = min(data_len)
      if(avail > buf_len[i])
        avail = buf_len[i];
    }

    if(avail) {
      for(int i=0;i<avail;i++) {
        write_file(&buf[(buf_start + i) & BufMask], sizeof(DataPack));
      }
      
      buf_start = (buf_start + avail) & BufMask;
      for(int i=0;i<N;i++) {
        buf_len[i] -= avail;
      }
    }

    return avail;
  } 

  void flush_buf2file() {
    int n = 0;
    for(int i=0;i<N;i++) {  // n = max(data_len)
      if(n < buf_len[i])
        n = buf_len[i];
    }

    for(int i=0;i<N;i++) {  // aligment, fill empty with 0xFF
      int len = buf_len[i];
      buf_len[i] = 0; // empty it
      while(len < n) {
        buf[(buf_start + len) & BufMask][i] = 0xFF;
        len ++;
      }
    }

    for(int i=0;i<n;i++) {
      write_file(&buf[(buf_start + i) & BufMask], sizeof(DataPack));
    }
  }

  public:
  FineGridOStream() {

  }

  int open(const char* pathname) {
    f = fopen(pathname, "wb");

    fseek(f, sizeof(Header), SEEK_SET);
    return f != NULL;
  }

  int write_header(Header *h) {

    size_t pos = ftell(f);
    fseek(f, 0, SEEK_SET);
    int n = write_file(h, sizeof(Header));

    fseek(f, pos, SEEK_SET);
    return n == sizeof(Header);
  }

  void write_byte(uint8_t byte, int n) {
    int len = buf_len[n];
    if(len >= BufSize) {
      if(write_buf2file()) {
        len = buf_len[n];
      } else {
        assert(("unexpected buf overflow.", 0));
      }
    }
    
    buf[(buf_start + len) & BufMask][n] = byte;
    buf_len[n] = len + 1;
  }

  int close() {
    flush_buf2file();
    return fclose(f);
  }
};


template<size_t N, size_t BufSize = 1024>
class FineGridIStream {
  using DataPack = uint8_t[N];
  static_assert(isPow2(BufSize));
  static constexpr int BufMask = BufSize - 1;

  DataPack buf[BufSize];
  size_t buf_end = 0;
  size_t buf_len[N] = {0};

  FILE* f = nullptr;
  bool eof = false;

  size_t read_file(void* dst, size_t size) {
    return fread(dst, 1, size, f);
  };

  int read_file2buf() {
    int max = 0;
    for(int i=0;i<N;i++){
      if(max < buf_len[i])
        max = buf_len[i];
    }
    int avail = BufSize - max;

    if(avail && !eof) {
      for(int i=0;i<avail;i++) {
        if(read_file(&buf[(buf_end + i) & BufMask], sizeof(DataPack)) != sizeof(DataPack)) {
          eof = true;
          avail = i;
          break;
        }
      }

      for(int i=0;i<N;i++) {
        buf_len[i] += avail;
      }

      buf_end = (buf_end + avail) & BufMask;
    }


    return avail;
  }

public:
  FineGridIStream() {

  }

  int open(const char* pathname) {
    f = fopen(pathname, "rb");

    fseek(f, sizeof(Header), SEEK_SET);
    return f != NULL;
  }

  int read_header(Header* h) {
    size_t pos = ftell(f);
    fseek(f, 0, SEEK_SET);
    int n = read_file(h, sizeof(Header));

    fseek(f, pos, SEEK_SET);
    return n == sizeof(Header);
  }

  uint8_t read_byte(int n) {
    size_t len = buf_len[n];
    if(len) {
      buf_len[n] = len - 1;
      return buf[(buf_end - len) & BufMask][n];

    } else if(!len && eof) {
      return 0xFF;

    } else {
      if(read_file2buf()) {
        len = buf_len[n];

        buf_len[n] = len - 1;
        return buf[(buf_end - len) & BufMask][n];

      } else {
        return 0xFF;
      }
    }
  }

  int close() {
    return fclose(f);
  }
};

}