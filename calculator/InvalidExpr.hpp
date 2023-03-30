#pragma once

#include <stdexcept>

struct InvalidExpr : public std::exception {
  [[nodiscard]] const char* what() const noexcept override {
    return "Invalid expression!";
  }
};
