#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/socket.h>
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

  struct sockaddr_in bind_address;
  bind_address.sin_family = AF_INET;
  bind_address.sin_port = htons(4949);
  bind_address.sin_addr.s_addr = inet_addr("127.0.0.1");

  Friend::Output* output = new Friend::Output(bind_address);
  output->MainLoop();
  delete output;

  return 0;
}
