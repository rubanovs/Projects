#pragma once

#include "AbstractToken.hpp"
#include "InvalidExpr.hpp"
#include "OperandToken.hpp"

template <typename T, bool IsBinaryOperator>
class OperatorToken : public AbstractToken {
 private:
  int priority_ = 0;

 public:
  int GetPriority() const { return priority_; }
  bool IsBinary() const { return IsBinaryOperator; }
  OperandToken<T>* Calculate(OperandToken<T>* lhs, OperandToken<T>* rhs) const {
    static_assert(IsBinaryOperator);
    return CalculateNoCheck(lhs, rhs);
  }
  OperandToken<T>* Calculate(OperandToken<T>* operand) const {
    static_assert(!IsBinaryOperator);
    return CalculateNoCheck(operand);
  }

 protected:
  OperatorToken(const std::string& k_token, const std::string& string_operator,
                int priority)
      : AbstractToken(k_token), priority_(priority) {
    if (k_token != string_operator) {
      throw InvalidExpr();
    }
  }

  virtual OperandToken<T>* CalculateNoCheck(
      [[maybe_unused]] OperandToken<T>* lhs,
      [[maybe_unused]] OperandToken<T>* rhs) const {
    return nullptr;
  }

  virtual OperandToken<T>* CalculateNoCheck(
      [[maybe_unused]] OperandToken<T>* operand) const {
    return nullptr;
  }
};

