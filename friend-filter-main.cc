#include <stdlib.h>
#include <csignal>
#include <exception>
#include <functional>
#include <iostream>
#include <thread>
#include <typeinfo>
#include "friend-defaults.h"
#include "friend-exceptions.h"
#include "friend-filter-input.h"
#include "friend-filter-output.h"
#include "friend-safe-queue.h"

#ifndef SIGUSR1
#define SIGUSR1 10
#endif
#ifndef SIGUSR2
#define SIGUSR2 12
#endif

/*
 * default exception handler - prints filename/line number and terminates the
 * program
 *
 */
void on_terminate() noexcept {
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

/*
 * allows the cutoff and resonance to be modified in real time by sending
 * SIGUSR1 (+100Hz) and SIGUSR2 (-100Hz) to the running process
 *
 */
float* cutoff = new float(1000);
float* resonance = new float(0.6f);

#define FRIEND__CUTOFF_STEP 100.f
#define FRIEND__CUTOFF_UPPER_BOUND 22050
void cutoff_mod(int sig) {
  if (sig == SIGUSR1) {
    *cutoff += FRIEND__CUTOFF_STEP;
    if (*cutoff < 0) {
      *cutoff = 0;
    }
  } else if (sig == SIGUSR2) {
    *cutoff -= FRIEND__CUTOFF_STEP;
    if (*cutoff > FRIEND__CUTOFF_UPPER_BOUND) {
      *cutoff = FRIEND__CUTOFF_UPPER_BOUND;
    }
  }
}

// run the ladder filter - process incoming samples and play them back
int main(int argc, char** argv) {
  std::set_terminate(&on_terminate);

  auto queue = new Friend::SafeQueue<FRIEND__PCM_TYPE>;
  auto input = new Friend::Input(queue, cutoff, resonance);
  auto output = new Friend::Output(queue);

  std::thread input_thread(std::bind(&Friend::Input::MainLoop, input));
  std::thread output_thread(std::bind(&Friend::Output::MainLoop, output));

  std::signal(SIGUSR1, cutoff_mod);
  std::signal(SIGUSR2, cutoff_mod);

  input_thread.join();
  output_thread.join();

  delete input;
  delete output;
  delete queue;

  return 0;
}
