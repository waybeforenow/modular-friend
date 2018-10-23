/*
 * Process a 32-bit PCM encoded RAW (headerless) audio file using a compile-time
 * configurable low-pass ladder filter algorithm.
 *
 */

#ifdef LADDER_USE_FULL_DIFFERENTIAL
#include "include/full-differential.h"
#elif defined LADDER_USE_RECURSIVE
#include "include/recursive.h"
#else
#include "include/canonical.h"
#endif
#include <fstream>
#include <iostream>

int main(int argc, char** argv) {
  using namespace Friend::Effects::LadderFilter;
  using sample_fmt = float;
  sample_fmt current, processed;

  if (argc < 2) {
    std::cerr << "please pass a filename" << std::endl;
    return 1;
  }

  auto fc = new float(500);
  auto res = new float(0.6f);
  auto filter = new Filter<sample_fmt>(fc, res, 44100);

  std::ifstream infile(argv[1], std::ios::in | std::ios::binary);
  if (infile.fail()) {
    std::cerr << "error opening " << argv[1] << std::endl;
    return 1;
  }

  // transform and write the raw samples to stdout
  while (!infile.read((char*)&current, sizeof(sample_fmt)).eof()) {
    processed = filter->tick(current);
    std::cout.write((char*)&processed, sizeof(sample_fmt));
  }

  infile.close();

  delete filter;
  delete fc;
  delete res;

  return 0;
}
