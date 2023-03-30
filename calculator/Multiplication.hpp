#pragma once

#include "OperatorToken.hpp"

template <typename T>
class MultiplicationOperator : public OperatorToken<T, true> {
 public:
  MultiplicationOperator(const std::string& k_token)
      : OperatorToken<T, true>(k_token, kStr, kPriority) {}

 private:
  OperandToken<T>* CalculateNoCheck(OperandToken<T>* lhs,
                                    OperandToken<T>* rhs) const {
    return new OperandToken<T>(lhs->GetValue() * rhs->GetValue());
  }
  inline static const int kPriority = 5;
  inline static const std::string kStr = "*";
};
