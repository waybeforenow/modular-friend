#ifdef LADDER_USE_FULL_DIFFERENTIAL
#include "include/full-differential.h"
#elif defined LADDER_USE_RECURSIVE
#include "include/recursive.h"
#else
#include "include/canonical.h"
#endif
#include <fstream>
#include <iostream>

template <typename T, size_t size = sizeof(T)>
void rbyteorder(void* bytes) {
  size_t n = size, j = 0;
  while (j < (size / 2)) {
    char tmp = ((char*)bytes)[--n];
    ((char*)bytes)[n] = ((char*)bytes)[j];
    ((char*)bytes)[j++] = tmp;
  }
}

int main(int argc, char** argv) {
  using namespace Friend::Effects::LadderFilter;
  using sample_fmt = float;
  sample_fmt current, processed;

  if (argc < 2) {
    std::cerr << "please pass a filename" << std::endl;
    return 1;
  }

  auto filter = new Filter<sample_fmt>(1500, 0.8f);

  std::ifstream infile(argv[1], std::ios::in | std::ios::binary);
  if (infile.fail()) {
    std::cerr << "error opening " << argv[1] << std::endl;
    return 1;
  }

  while (!infile.read((char*)&current, sizeof(sample_fmt)).eof()) {
    processed = filter->tick(current);
    std::cout.write((char*)&processed, sizeof(sample_fmt));
  }

  infile.close();

  delete filter;

  return 0;
}
