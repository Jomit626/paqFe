# paqFe

`paqFe` is a header-only C++ compression library using a PAQ-varient algorithm adapted for text compression and FPGA implementations. It has its [RTL model](https://github.com/Jomit626/paqFe-RTL) written in `Chisel`.  

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
  
  paqFe::paqFeFile fout("some.paqfe", paqFe::OpMode::Compress);
  fout.write(data, data_size);
  fout.close();

  paqFe::paqFeFile fin("some.paqfe", paqFe::OpMode::Decompress);
  fin.read(buf, data_size);
  fin.close();

  std::cout << buf << std::endl;
}
```

## Benchmark

|file|gzip|lpaq1|paq8px|paqFe|
|--|--|--|--|--|
| bib     | 31.36%  | 20.80%  | 17.64% | 21.75% |
| book1   | 40.62%  | 26.34%  | 23.86% | 28.37% |
| book2   | 33.74%  | 21.43%  | 18.68% | 23.12% |
| geo     | 66.80%  | 49.82%  | 41.49% | 46.77% |
| news    | 38.28%  | 25.77%  | 22.07% | 27.90% |
| obj1    | 47.96%  | 43.68%  | 32.89% | 42.86% |
| obj2    | 32.85%  | 24.51%  | 16.61% | 23.91% |
| paper1  | 34.86%  | 26.17%  | 23.31% | 27.68% |
| paper2  | 36.08%  | 26.00%  | 23.81% | 27.62% |
| paper3  | 38.83%  | 29.29%  | 26.64% | 31.01% |
| paper4  | 41.60%  | 33.25%  | 29.97% | 35.20% |
| paper5  | 41.72%  | 34.65%  | 30.94% | 36.53% |
| paper6  | 34.65%  | 26.95%  | 23.60% | 28.51% |
| pic     | 10.20%  | 8.50%   | 4.84%  | 8.64%  |
| progc   | 33.46%  | 26.25%  | 22.47% | 27.31% |
| progl   | 22.55%  | 16.81%  | 13.33% | 17.33% |
| progp   | 22.64%  | 17.02%  | 12.99% | 17.58% |
| trans   | 20.12%  | 14.31%  | 11.71% | 14.63% |
| enwik8  |         | 24.77%  |        | 24.74% |

## License

The `paqFe` C++ library is licensed under the GNU General Public License v3.0. See LICENSE for more information.

## FQAs

- Why the name paqFe?

I do remeber I named it for some reason, but I forgot it.

## Links

- [LPAQ1](http://www.mattmahoney.net/dc/#lpaq)
- [PAQ8PX](https://github.com/hxim/paq8px)
