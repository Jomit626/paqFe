#include <cstdio>
#include <cerrno>
#include <cstdlib>
#include <unistd.h>

void generate_db(FILE* fin, FILE* fout);

int main(int argc, char** argv) {
  int opt;
  const char *input_pathname = NULL, *output_pathname = NULL;
  while((opt = getopt(argc, argv, "vcxi:o:")) != -1) {
    switch (opt) {
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

  if(!input_pathname || !output_pathname)
    return 1;

  FILE *fin = nullptr, *fout = nullptr;

  fin = fopen(input_pathname, "rb");
  fout = fopen(output_pathname, "w");

  if(!fin || !fout)
    return 1;

  generate_db(fin, fout);

  return 0;
}