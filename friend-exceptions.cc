#include "friend-exceptions.h"
#include <exception>

namespace Friend {

runtime_error::runtime_error(const char* file, const size_t line)
    : what_str(new std::string(file)) {
  what_str->push_back(':');
  what_str->append(std::to_string(line));
}

const char* runtime_error::what() const noexcept { return what_str->c_str(); }

}  // namespace Friend
