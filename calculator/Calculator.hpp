#pragma once

#include <stack>

#include "ExprInPolishNotation.hpp"

template <typename T>
class Calculator {
 public:
  static T CalculateExpr(const std::string& expression) {
    ExprInPolishNotation<T> parser(expression);
    auto& tokens = parser.GetTokens();
    std::stack<AbstractToken*> stack_tokens;
    for (auto it = tokens.rbegin(); it != tokens.rend(); it++) {
      stack_tokens.push(*it);
    }
    auto result_operand = CalculateExpr(stack_tokens);
    auto result_value = result_operand->GetValue();
    delete result_operand;
    return result_value;
  }

 private:
  static OperandToken<T>* CalculateExpr(std::stack<AbstractToken*>& tokens) {
    auto* token = tokens.top();
    tokens.pop();
    auto operand_token = dynamic_cast<OperandToken<T>*>(token);
    if (operand_token != nullptr) {
      return new OperandToken<T>(operand_token->GetValue());
    }
    auto unary_token = dynamic_cast<OperatorToken<T, false>*>(token);
    static_assert(static_cast<bool>(unary_token->IsBinaryOperator));
    if (unary_token != nullptr) {
      operand_token = CalculateExpr(tokens);
      auto result = unary_token->Calculate(operand_token);
      delete operand_token;
      return result;
    }
    auto binary_token = dynamic_cast<OperatorToken<T, true>*>(token);
    static_assert(!static_cast<bool>(binary_token->IsBinaryOperator));
    auto first = CalculateExpr(tokens);
    auto second = CalculateExpr(tokens);
    auto result = binary_token->Calculate(first, second);
    delete first;
    delete second;
    return result;
  }
};
