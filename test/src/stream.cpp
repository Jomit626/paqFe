#include <cstdio>

#include <paqFe/StreamFile.hpp>

int main() {
  paqFe::FineGridOStream<8, 1024> os;
  os.open("/tmp/stream.test");

  // write some thing
  for(int i=0;i<=0xFF;i++) {
    for(int j=0;j<8;j++) {
      os.write_byte(i, j);
    }
  }

  // write half
  for(int i=0;i<=0xFF;i++) {
    for(int j=0;j<4;j++) {
      os.write_byte(i, j);
    }
  }

  // try to fill it
  
  for(int i=0;i<4096;i++) {
    for(int j=0;j<8;j++) {
      os.write_byte(i, j);
    }
  }
  

  os.close();
  
  paqFe::FineGridIStream<8, 1024> is;
  is.open("/tmp/stream.test");

  for(int i=0;i<=0xFF;i++) {
    for(int j=0;j<8;j++) {
      assert(is.read_byte(j) == i);
    }
  }

  for(int i=0;i<=0xFF;i++) {
    for(int j=0;j<4;j++) {
      assert(is.read_byte(j) == i);
    }
  }

  for(int i=0;i<4096;i++) {
    for(int j=0;j<8;j++) {
      assert(is.read_byte(j) == (i & 0xFF));
    }
  }

  is.close();
  
  return 0;
}