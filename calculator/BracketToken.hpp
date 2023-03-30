#pragma once
#include "AbstractToken.hpp"
#include "InvalidExpr.hpp"

class BracketToken : public AbstractToken {
 public:
  BracketToken(const std::string& token) : AbstractToken(token) {
    if (token != kOpen && token != kClose) {
      throw InvalidExpr();
    }
    is_opening_ = token == kOpen;
  }
  bool IsOpening() const { return is_opening_; }

 private:
  bool is_opening_ = false;
  inline static const std::string kOpen = "(";
  inline static const std::string kClose = ")";
};
