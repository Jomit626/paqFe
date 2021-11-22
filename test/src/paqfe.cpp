#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

#include <paqFe/paqFe.hpp>

using namespace paqFe;

int main(int argc, char** argv) {
  int opt;
  OpMode mode = OpMode::Comresss;
  int verobse = 0;
  size_t origin_size = 0;
  const char *input_pathname = NULL, *output_pathname = NULL;
  while((opt = getopt(argc, argv, "vcxi:o:")) != -1) {
    switch (opt) {
    case 'v':
      verobse = 1;
      break;
    case 'c':
      mode = OpMode::Comresss;
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
    default:
      break;
    }
  }

  if(verobse)
    printf(
      "mode: %s\n"
      "input file:%s\n"
      "output file:%s\n"
      "mem usage:%d bytes\n"
      ,
      mode == OpMode::Comresss ? "compress" : "decompress",
      input_pathname == NULL ? "null" : input_pathname,
      output_pathname == NULL ? "null" : output_pathname,
      sizeof(Comressor)
    );

  if(mode == OpMode::Comresss) {
    Comressor *c = new Comressor(output_pathname, OpMode::Comresss);
    FILE* fin = fopen(input_pathname, "rb");
    uint8_t buf[1024];
    size_t n;
    while((n = fread(buf, 1, 1024, fin))) {
      c->write(buf, n);
    }

    fclose(fin);
    c->close();

    delete c;
  } else {
    Comressor *c = new Comressor(input_pathname, OpMode::Decompress);
    printf("origin size:%d\n", c->size());
    FILE* fout = fopen(output_pathname, "wb");
    uint8_t buf[1024];
    sizeof(*c);
    size_t n;
    while((n = c->read(buf, 1024))) {
      fwrite(buf, 1, n, fout);
    }

    fclose(fout);
    c->close();

    delete c;
  }

  return 0;
}