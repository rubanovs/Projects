#pragma once

#include "OperatorToken.hpp"

template <typename T>
class UnaryMinusOperator : public OperatorToken<T, false> {
 public:
  UnaryMinusOperator(const std::string& k_token)
      : OperatorToken<T, false>(k_token, kStr, kPriority) {}

 private:
  inline static const int kPriority = 10;
  inline static const std::string kStr = "-";
  OperandToken<T>* CalculateNoCheck(OperandToken<T>* operand) const {
    return new OperandToken<T>((-operand->GetValue()));
  }
};

