/*
 * Convenience class for throwing exceptions via FRIEND__THROWEXCEPTION or
 * FRIEND__THROWEXCEPTIONWITHTEXT (see `friend-defaults.h`).
 *
 */

#ifndef __FRIEND_EXCEPTIONS_H
#define __FRIEND_EXCEPTIONS_H

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

#endif  // __FRIEND_EXCEPTIONS_H
