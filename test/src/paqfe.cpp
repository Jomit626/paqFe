#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

#include <paqFe/paqFe.hpp>

using namespace paqFe;

int main(int argc, char **argv)
{
  int opt;
  OpMode mode = OpMode::Compress;
  int verobse = 0;
  size_t origin_size = 0;
  const char *input_pathname = NULL, *output_pathname = NULL;
  while ((opt = getopt(argc, argv, "vcxi:o:")) != -1)
  {
    switch (opt)
    {
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
    default:
      break;
    }
  }

  if (verobse)
    printf(
        "mode: %s\n"
        "input file:%s\n"
        "output file:%s\n"
        "mem usage:%.2lf MB\n",
        mode == OpMode::Compress ? "compress" : "decompress",
        input_pathname == NULL ? "null" : input_pathname,
        output_pathname == NULL ? "null" : output_pathname,
        (double)sizeof(paqFeFile::Engine) / 1024 / 1024);

  if (!input_pathname || !output_pathname)
    return 1;

  if (mode == OpMode::Compress)
  {
    paqFeFile *fout = new paqFeFile(output_pathname, OpMode::Compress);
    FILE *fin = fopen(input_pathname, "rb");
    uint8_t buf[1024];
    ssize_t n;
    while ((n = fread(buf, 1, 1024, fin)))
    {
      fout->write(buf, n);
    }

    fclose(fin);
    fout->close();

    delete fout;
  }
  else
  {
    paqFeFile *fin = new paqFeFile(input_pathname, OpMode::Decompress);
    if (verobse)
      printf("origin size:%ld\n", fin->size());
    FILE *fout = fopen(output_pathname, "wb");
    uint8_t buf[1024];

    ssize_t n;
    while ((n = fin->read(buf, 1024)))
    {
      if (n < 0)
      {
        fprintf(stderr, "Warning: CRC error!\n");
        break;
      }
      fwrite(buf, 1, n, fout);
    }

    fclose(fout);
    fin->close();

    delete fin;
  }

  return 0;
}