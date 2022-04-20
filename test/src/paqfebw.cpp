#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

#include <paqFe/paqFe.hpp>

using namespace paqFe;

int main(int argc, char** argv) {
  int opt;
  size_t block_size = 64 * 1024;

  OpMode mode = OpMode::Compress;
  int verobse = 0;
  size_t origin_size = 0;
  const char *input_pathname = NULL, *output_pathname = NULL;
  while((opt = getopt(argc, argv, "vcxi:o:b:")) != -1) {
    switch (opt) {
    case 'v':
      verobse = 1;
      break;
    case 'c':
      mode = OpMode::Compress;
      break;
    case 'x':
      mode = OpMode::Decompress;
      break;  
    case 'i':
      input_pathname = optarg;
      break;
    case 'o':
      output_pathname = optarg;
      break;
    case 'b':
      block_size = atol(optarg) * 1024;
      break;
    default:
      break;
    }
  }

  if(verobse)
    printf(
      "mode: %s\n"
      "input file:%s\n"
      "output file:%s\n"
      "block size:%ld KB\n"
      "mem usage:%.2lf MB\n"
      ,
      mode == OpMode::Compress ? "compress" : "decompress",
      input_pathname == NULL ? "null" : input_pathname,
      output_pathname == NULL ? "null" : output_pathname,
      block_size / 1024,
      (double)sizeof(paqFeFile::Engine) / 1024 / 1024
    );

  if(!input_pathname || !output_pathname)
    return 1;


  if(mode == OpMode::Compress) {
    char* output_pathname_blocked = (char*)(malloc(strlen(output_pathname) + 512));
    FILE* fin = fopen(input_pathname, "rb");
    uint8_t *block = (uint8_t*)malloc(block_size);
    ssize_t n;
    int block_id = 0;
    while((n = fread(block, 1, block_size, fin))) {
      sprintf(output_pathname_blocked, "%s.%d", output_pathname, block_id);
      paqFeFile *fout = new paqFeFile(output_pathname_blocked, OpMode::Compress);

      fout->write(block, n);
      fout->close();

      delete fout;
      block_id ++;
    }

    fclose(fin);
    if(verobse) {
      printf("%d blocks total\n", block_id);
    }
  } else {
    printf("no decompress");
    
  }

  return 0;
}