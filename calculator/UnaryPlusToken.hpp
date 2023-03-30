#pragma once

#include "OperatorToken.hpp"

template <typename T>
class UnaryPlusOperator : public OperatorToken<T, false> {
 public:
  UnaryPlusOperator(const std::string kToken)
      : OperatorToken<T, false>(kToken, kStr, kPriority) {}

 private:
  inline static const int kPriority = 10;
  inline static const std::string kStr = "+";
  OperandToken<T>* CalculateNoCheck(OperandToken<T>* operand) const {
    return new OperandToken<T>(+operand->GetValue());
  }
};
