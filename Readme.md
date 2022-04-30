# paqFe

`paqFe` is a header-only C++ compression library using a PAQ-varient algorithm adapted for text compression and FPGA implementations. It has its [RTL model](https://github.com/Jomit626/paqFe-RTL) written in `Chisel`.  

## Overview

- **High Compression Ratio**

- **Slow Compression/Decompression Speed**

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

|file[^1]|gzip[^2]|lpaq1[^3]|paq8px[^4]|paqFe|
|--|--|--|--|--|
| bib     | 31.36%  | 20.80%  | 17.64% | 22.85% |
| book1   | 40.62%  | 26.34%  | 23.86% | 28.13% |
| book2   | 33.74%  | 21.43%  | 18.68% | 23.27% |
| geo     | 66.80%  | 49.82%  | 41.49% | 49.84% |
| news    | 38.28%  | 25.77%  | 22.07% | 28.63% |
| obj1    | 47.96%  | 43.68%  | 32.89% | 45.35% |
| obj2    | 32.85%  | 24.51%  | 16.61% | 26.66% |
| paper1  | 34.86%  | 26.17%  | 23.31% | 27.80% |
| paper2  | 36.08%  | 26.00%  | 23.81% | 27.48% |
| paper3  | 38.83%  | 29.29%  | 26.64% | 30.71% |
| paper4  | 41.60%  | 33.25%  | 29.97% | 35.14% |
| paper5  | 41.72%  | 34.65%  | 30.94% | 36.18% |
| paper6  | 34.65%  | 26.95%  | 23.60% | 28.72% |
| pic     | 10.20%  | 8.50%   | 4.84%  | 9.32%  |
| progc   | 33.46%  | 26.25%  | 22.47% | 28.07% |
| progl   | 22.55%  | 16.81%  | 13.33% | 19.36% |
| progp   | 22.64%  | 17.02%  | 12.99% | 19.92% |
| trans   | 20.12%  | 14.31%  | 11.71% | 17.49% |
| enwik8  | 36.44%  | 23.05%  | 16.53% | 24.98% |

Speed of Compressing or Decompression

|Program[^5]|gzip|lpaq1|paq8px|paqFe|paqFe-RTL[^6]|
|-|-|-|-|-|-|
|Speed|17.43 MB/s|1088.09 KB/s|7.39KB/s|990.83 KB/s|73.84 MB/s|

## License

The `paqFe` C++ library is licensed under the GNU General Public License v3.0. See LICENSE for more information.

## FQAs

- Why the name paqFe?

I do remeber I named it for some reason, but I forgot it.

## Links

- [LPAQ1](http://www.mattmahoney.net/dc/#lpaq)
- [PAQ8PX](https://github.com/hxim/paq8px)


[^1]: The first 18 data are from the Calgary corpus.

[^2]: Gzip 1.9 with option `--best`

[^3]: [LPAQ1](http://www.mattmahoney.net/dc/lpaq1.zip) with N=1

[^4]: [PAQ8PX](https://github.com/hxim/paq8px) with `-8`

[^5]: Test with Ryzen 5 Pro 4650G@4100MHz, DDR4 16GB@3200MHz.

[^6]: Using Xilinx Virtex UltraScale+ FPGA VCU118 Evaluation Kit
