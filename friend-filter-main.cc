#include <stdlib.h>
#include <exception>
#include <iostream>
#include <thread>
#include <typeinfo>
#include "friend-exceptions.h"
#include "friend-filter-input.h"
#include "friend-filter-output.h"
#include "friend-safe-queue.h"

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

  auto queue = new Friend::SafeQueue<float>;
  Friend::Output* input = new Friend::Input(queue, 1500, 0.6f);
  Friend::Output* output = new Friend::Output(queue);

  std::thread input_thread(input->MainLoop());
  std::thread output_thread(output->MainLoop());

  input_thread.join();
  output_thread.join();

  delete input;
  delete output;

  return 0;
}
