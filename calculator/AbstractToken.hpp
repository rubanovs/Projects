#pragma once

#include <string>

class AbstractToken {
 public:
  AbstractToken(const std::string& string) : token_(string) {}
  virtual ~AbstractToken() = default;
  const std::string& GetStringToken() const { return token_; }

 private:
  std::string token_;
};
