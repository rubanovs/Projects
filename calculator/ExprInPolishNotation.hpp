#pragma once

#include <algorithm>
#include <stack>
#include <string>
#include <vector>

#include "AbstractToken.hpp"
#include "BracketToken.hpp"
#include "DivisionToken.hpp"
#include "MinusToken.hpp"
#include "MultiplicationToken.hpp"
#include "OperandToken.hpp"
#include "OperatorToken.hpp"
#include "PlusToken.hpp"
#include "UnaryMinusOperator.hpp"
#include "UnaryPlusToken.hpp"

template <typename T>
class ExprInPolishNotation {
 public:
  ExprInPolishNotation(const std::string& expression) {
    try {
      PartitionIntoTokens(expression);
      Converting();
    } catch (const InvalidExpr& exception) {
      for (auto* token : tokens_) {
        delete token;
      }
      throw exception;
    }
  }
  ~ExprInPolishNotation() {
    for (auto* token : tokens_) {
      delete token;
    }
  }

  const std::vector<AbstractToken*>& GetTokens() const { return tokens_; }

 private:
  std::vector<AbstractToken*> tokens_;

  static const int kStart = 0;
  static const int kOp = 1;
  static const int kCl = 2;
  static const int kBin = 3;
  static const int kUn = 4;
  static const int kState = 5;
  void PartitionIntoTokens(const std::string& expression) {
    const char kSkipSimbol = ' ';
    std::string operand;
    int current = kStart;

    for (char tok : expression) {
      if (tok == kSkipSimbol) {
        continue;
      }
      switch (current) {
        case kStart:
          current = HandleStartState(tok, operand);
          break;
        case kOp:
          current = WorkingWithOpenBracket(tok, operand);
          break;
        case kCl:
          current = WorkingWithCloseBracketState(tok);
          break;
        case kBin:
          current = WorkingWithBinaryOperatorState(tok, operand);
          break;
        case kUn:
          current = WorkingUnaryOperatorState(tok, operand);
          break;
        case kState:
          current = WorkingOperandState(tok, operand);
          break;
        default:
          throw std::logic_error("Unkown state");
      }
    }

    if (!operand.empty()) {
      tokens_.push_back(new OperandToken<T>(operand));
    }
  }
  void CycleOne(std::vector<AbstractToken*>& postfix,
                std::stack<AbstractToken*>& operators) {
    for (auto it = tokens_.rbegin(); it != tokens_.rend(); it++) {
      AbstractToken* token = *it;

      if (ProcessingOfOperand(token, postfix)) {
        continue;
      }

      if (ProcessingOfOperator(token, postfix, operators)) {
        continue;
      }

      if (ProcessingOfBracket(token, postfix, operators)) {
        continue;
      }

      throw std::logic_error("Unexpected token");
    }
  }
  void CycleTwo(std::vector<AbstractToken*>& postfix,
                std::stack<AbstractToken*>& operators) {
    while (!operators.empty()) {
      auto* token = operators.top();
      if (IsCloseBracket(token)) {
        throw InvalidExpr();
      }
      postfix.push_back(token);
      operators.pop();
    }
  }
  void CycleThree(std::vector<AbstractToken*>& postfix) {
    for (auto it = postfix.rbegin(); it != postfix.rend(); it++) {
      tokens_.push_back(*it);
    }
  }
  void Converting() {
    std::vector<AbstractToken*> postfix;
    std::stack<AbstractToken*> operators;
    CycleOne(postfix, operators);
    CycleTwo(postfix, operators);
    DeleteBrackets();
    tokens_.clear();
    CycleThree(postfix);
  }
  void DeleteBrackets() {
    for (auto* token : tokens_) {
      if (dynamic_cast<BracketToken*>(token) != nullptr) {
        delete token;
      }
    }
  }
  int WorkingWithOpenBracket(char tok, std::string& operand) {
    std::string string_token(1, tok);

    auto* unary_token = ParsingOfUnaryOperator(string_token);
    if (unary_token != nullptr) {
      tokens_.push_back(unary_token);
      return kUn;
    }

    auto* bracket_token = ParsingOfBracket(string_token);
    if (bracket_token != nullptr) {
      tokens_.push_back(bracket_token);
      if (!bracket_token->IsOpening()) {
        throw InvalidExpr();
      }
      return kOp;
    }

    operand.push_back(tok);
    return kState;
  }

  int HandleStartState(char tok, std::string& operand) {
    std::string string_token(1, tok);

    auto* unary_token = ParsingOfUnaryOperator(string_token);
    if (unary_token != nullptr) {
      tokens_.push_back(unary_token);
      return kUn;
    }

    auto* bracket_token = ParsingOfBracket(string_token);
    if (bracket_token != nullptr) {
      tokens_.push_back(bracket_token);
      if (!bracket_token->IsOpening()) {
        throw InvalidExpr();
      }
      return kOp;
    }

    operand.push_back(tok);
    return kState;
  }

  int WorkingWithCloseBracketState(char tok) {
    std::string string_token(1, tok);

    auto* binary_token = ParsingOfBinaryOperator(string_token);
    if (binary_token != nullptr) {
      tokens_.push_back(binary_token);
      return kBin;
    }

    auto* unary_token = ParsingOfBracket(string_token);
    if (unary_token != nullptr) {
      tokens_.push_back(unary_token);
      if (unary_token->IsOpening()) {
        throw InvalidExpr();
      }
      return kCl;
    }

    throw InvalidExpr();
  }

  int WorkingWithBinaryOperatorState(char tok, std::string& operand) {
    std::string string_token(1, tok);

    auto* unary_token = ParsingOfUnaryOperator(string_token);
    if (unary_token != nullptr) {
      tokens_.push_back(unary_token);
      return kUn;
    }

    auto* bracket_token = ParsingOfBracket(string_token);
    if (bracket_token != nullptr) {
      tokens_.push_back(bracket_token);
      if (bracket_token->IsOpening()) {
        return kOp;
      }
      throw InvalidExpr();
    }

    operand.push_back(tok);
    return kState;
  }

  int WorkingUnaryOperatorState(char tok, std::string& operand) {
    std::string string_token(1, tok);

    auto* bracket_token = ParsingOfBracket(string_token);
    if (bracket_token != nullptr) {
      tokens_.push_back(bracket_token);
      if (bracket_token->IsOpening()) {
        return kOp;
      }
      throw InvalidExpr();
    }

    operand.push_back(tok);
    return kState;
  }

  int WorkingOperandState(char tok, std::string& operand) {
    std::string string_token(1, tok);

    auto* bracket_token = ParsingOfBracket(string_token);
    if (bracket_token != nullptr) {
      tokens_.push_back(bracket_token);
      if (bracket_token->IsOpening()) {
        throw InvalidExpr();
      }
      tokens_.push_back(new OperandToken<T>(operand));
      operand.clear();
      return kCl;
    }
    auto* binary_token = ParsingOfBinaryOperator(string_token);
    if (binary_token != nullptr) {
      tokens_.push_back(new OperandToken<T>(operand));
      operand.clear();
      tokens_.push_back(binary_token);
      return kBin;
    }

    operand.push_back(tok);
    return kState;
  }

  AbstractToken* ParsingOfUnaryOperator(const std::string& string_token) {
    try {
      return new UnaryPlusOperator<T>(string_token);
    } catch (InvalidExpr& expression) {
    }

    try {
      return new UnaryMinusOperator<T>(string_token);
    } catch (InvalidExpr& expression) {
      return nullptr;
    }
  }

  AbstractToken* ParsingOfBinaryOperator(const std::string& string_token) {
    try {
      return new PlusOperator<T>(string_token);
    } catch (InvalidExpr& expression) {
    }

    try {
      return new MultiplicationOperator<T>(string_token);
    } catch (InvalidExpr& expression) {
    }

    try {
      return new DivisionOperator<T>(string_token);
    } catch (InvalidExpr& expression) {
    }

    try {
      return new MinusOperator<T>(string_token);
    } catch (InvalidExpr& expression) {
      return nullptr;
    }
  }

  BracketToken* ParsingOfBracket(const std::string& string_token) {
    try {
      return new BracketToken(string_token);
    } catch (InvalidExpr& expression) {
      return nullptr;
    }
  }

  int GetPriority(AbstractToken* token) {
    auto binary_token = dynamic_cast<OperatorToken<T, true>*>(token);
    if (binary_token != nullptr) {
      return binary_token->GetPriority();
    }
    auto unary_token = dynamic_cast<OperatorToken<T, false>*>(token);
    if (unary_token != nullptr) {
      return unary_token->GetPriority();
    }
    throw std::logic_error("Invalid token");
  }
  bool IsOperation(AbstractToken* token) {
    auto binary_token = dynamic_cast<OperatorToken<T, true>*>(token);
    auto unary_token = dynamic_cast<OperatorToken<T, false>*>(token);
    return binary_token != nullptr || unary_token != nullptr;
  }

  bool IsOpenBracket(AbstractToken* token) {
    auto* bracket_token = dynamic_cast<BracketToken*>(token);
    return bracket_token != nullptr && bracket_token->IsOpening();
  }

  bool IsCloseBracket(AbstractToken* token) {
    auto* bracket_token = dynamic_cast<BracketToken*>(token);
    return bracket_token != nullptr && !bracket_token->IsOpening();
  }

  bool ProcessingOfOperand(AbstractToken* token,
                           std::vector<AbstractToken*>& postfix) {
    if (dynamic_cast<OperandToken<T>*>(token)) {
      postfix.push_back(token);
      return true;
    }
    return false;
  }

  bool ProcessingOfOperator(AbstractToken* token,
                            std::vector<AbstractToken*>& postfix,
                            std::stack<AbstractToken*>& operators) {
    if (IsOperation(token)) {
      while (!operators.empty() && IsOperation(operators.top()) &&
             GetPriority(token) <= GetPriority(operators.top())) {
        postfix.push_back(operators.top());
        operators.pop();
      }
      operators.push(token);
      return true;
      ;
    }
    return false;
  }

  bool ProcessingOfBracket(AbstractToken* token,
                           std::vector<AbstractToken*>& postfix,
                           std::stack<AbstractToken*>& operators) {
    if (IsCloseBracket(token)) {
      operators.push(token);
      return true;
    }

    if (IsOpenBracket(token)) {
      while (!operators.empty() && !IsCloseBracket(operators.top())) {
        postfix.push_back(operators.top());
        operators.pop();
      }
      if (operators.empty()) {
        throw InvalidExpr();
      }
      operators.pop();
      return true;
    }

    return false;
  }
};
