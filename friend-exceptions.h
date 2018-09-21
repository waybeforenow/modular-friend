#ifndef _FRIEND_EXCEPTIONS_H
#define _FRIEND_EXCEPTIONS_H

#include <exception>
#include <string>

namespace Friend {

class runtime_error : public std::exception {
 private:
  std::string* what_str;

 public:
  runtime_error(const char* file, const std::size_t line);
  runtime_error(const char* what, const char* file, const std::size_t line);
  ~runtime_error() { delete what_str; }
  const char* what() const noexcept;
};

}  // namespace Friend

#endif  // _FRIEND_EXCEPTIONS_H
