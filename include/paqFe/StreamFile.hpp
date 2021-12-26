#pragma once

#include "types.hpp"

#include "Header.hpp"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>

namespace paqFe::internal {

template<size_t N>
class FilesOStream {
  static_assert(N <= 9 && N >= 1);

  FILE* h = nullptr;
  FILE* f[N] = {nullptr};

  size_t write_file(const void *src, size_t size, FILE* f) {
    return fwrite(src, 1, size, f);
  };

public:
  FilesOStream() {

  }

  int open(const char* pathname) {
    int len = strlen(pathname);
    char* pathname_subfix = (char*) malloc(sizeof(char) * (len + 8));
    strcpy(pathname_subfix, pathname);

    pathname_subfix[len] = '.';
    pathname_subfix[len + 2] = '\0';

    int idx = len + 1;
    for(int i=0;i<N;i++) {
      pathname_subfix[idx] = '0' + i;
      f[i] = fopen(pathname_subfix, "wb");
    }

    h = fopen(pathname, "wb");  // header

    delete(pathname_subfix);
    return h != NULL;
  }

  int write_header(Header *header) {
    fseek(h, 0, SEEK_SET);
    int n = write_file(header, sizeof(Header), h);

    return n == sizeof(Header);
  }

  void write_byte(uint8_t byte, int n) {
    write_file(&byte, 1, f[n]);
  }

  int close() {

    fclose(h);
    for(int i=0;i<N;i++)
      fclose(f[i]);

    return 0;

  }
};


template<size_t N>
class FilesIStream {
  static_assert(N <= 9 && N >= 1);

  FILE* h = nullptr;
  FILE* f[N] = {nullptr};

  size_t read_file(void* dst, size_t size, FILE* f) {
    return fread(dst, 1, size, f);
  };

public:
  FilesIStream() {

  }

  int open(const char* pathname) {
    int len = strlen(pathname);
    char* pathname_subfix = (char*) malloc(sizeof(char) * (len + 8));
    strcpy(pathname_subfix, pathname);

    pathname_subfix[len] = '.';
    pathname_subfix[len + 2] = '\0';

    int idx = len + 1;
    for(int i=0;i<N;i++) {
      pathname_subfix[idx] = '0' + i;
      f[i] = fopen(pathname_subfix, "rb");
    }

    h = fopen(pathname, "rb");  // header

    delete(pathname_subfix);
    return h != NULL;
  }

  int read_header(Header* header) {
    fseek(h, 0, SEEK_SET);
    int n = read_file(header, sizeof(Header), h);

    return n == sizeof(Header);
  }

  uint8_t read_byte(int n) {
    uint8_t byte;
    read_file(&byte, 1, f[n]);
    return byte;
  }

  int close() {
    fclose(h);
    for(int i=0;i<N;i++)
      fclose(f[i]);

    return 0;
    
  }
};

template<int N>
union Stream {
  FilesIStream<N> in;
  FilesOStream<N> out;

  Stream() { };
};

}