#include <stdlib.h>
#include <exception>
#include <iostream>
#include <typeinfo>
#include "friend-exceptions.h"
#include "friend-input.h"
#include "friend-output.h"

void onTerminate() noexcept {
  if (auto cur_ex = std::current_exception()) {
    try {
      std::rethrow_exception(cur_ex);
    } catch (const Friend::runtime_error& e) {
      std::cout << "Unhandled Friend exception thrown at " << e.what()
                << std::endl;
    }
  } else {
    std::cout << "Program terminated abnormally without exception."
              << std::endl;
  }

  exit(1);
}

int main(int argc, char** argv) {
  std::set_terminate(&onTerminate);

  Friend::Input* input = new Friend::Input(4949);
  delete input;

  return 0;
}
