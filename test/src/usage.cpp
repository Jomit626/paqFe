#include <iostream>
#include <paqFe/paqFe.hpp>

const char data[] = "Hello, World!";
const size_t data_size = sizeof(data);

char buf[data_size] = {0};

int main() {
  
  paqFe::paqFeFile fout("some.paqfe", paqFe::OpMode::Compress);
  fout.write(data, data_size);
  fout.close();

  paqFe::paqFeFile fin("some.paqfe", paqFe::OpMode::Decompress);
  fin.read(buf, data_size);
  fin.close();

  std::cout << buf << std::endl;
}
