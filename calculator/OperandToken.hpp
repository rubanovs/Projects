#pragma once

#include <sstream>
#include <string>

#include "AbstractToken.hpp"
#include "InvalidExpr.hpp"

template <typename T>
class OperandToken : public AbstractToken {
 public:
  OperandToken(const std::string& token) : AbstractToken(token) {
    std::stringstream stream(token);
    stream >> operand_;
    if (stream.fail() || !stream.eof()) {
      throw InvalidExpr();
    }
  }
  OperandToken(T value)
      : AbstractToken(std::to_string(value)), operand_(value) {}
  const T& GetValue() const { return operand_; }

 private:
  T operand_ = T();
};
