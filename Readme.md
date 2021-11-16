# paqFe

`paqFe` is a header-only C++ compression library using a PAQ-varient algorithm adapted for text compression and FPGA implementations. It has its [RTL model]() written with `Chisel`.  

## Overviwe

- **High Compression Ratio**

- **Slow Decompress Speed**

## Usage

``` C++
#include <iostream>
#include <paqFe/paqFe.hpp>

const char data[] = "Hello, World!";
const size_t data_size = sizeof(data);

char buf[data_size] = {0};

int main() {
  
  paqFe::paqFeFile fout("some.paqfe", paqFe::Mode::Compress);
  fout.write(data, data_size);
  fout.close();

  paqFe::paqFeFile fin("some.paqfe", paqFe::Mode::Decompress);
  fin.read(buf, data_size);
  fin.close();

  std::cout << buf << std::endl;
}
```

## Benchmark

|file|gzip|lpaq1|paq8px|paqFe|
|--|--|--|--|--|

## License

The `paqFe` C++ library is licensed under the GNU General Public License v3.0. See LICENSE for more information.

## FQAs

- Why the name paqFe?

I do remeber I named it for some reason, but I forgot it.

- 

## Links

- [LPAQ1]()
- [PAQ8PX]()