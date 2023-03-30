#pragma once

#include "OperatorToken.hpp"

template <typename T>
class DivisionOperator : public OperatorToken<T, true> {
 public:
  DivisionOperator(const std::string& k_token)
      : OperatorToken<T, true>(k_token, kStr, kPriority) {}

 private:
  inline static const int kPriority = 5;
  inline static const std::string kStr = "/";
  OperandToken<T>* CalculateNoCheck(OperandToken<T>* lhs,
                                    OperandToken<T>* rhs) const override {
    return new OperandToken<T>(lhs->GetValue() / rhs->GetValue());
  }
};
