#include <cmath>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
template <size_t N, size_t M, typename T = int64_t>
class Matrix {
 private:
  std::vector<std::vector<T>> matr_;

 public:
  Matrix();
  Matrix(std::vector<std::vector<T>> matrix);
  Matrix(T elem);
  Matrix<N, M, T> operator+(const Matrix<N, M, T>& matr) const;
  Matrix<N, M, T> operator-(const Matrix<N, M, T>& matr) const;
  Matrix<N, M, T>& operator+=(const Matrix<N, M, T>& matr);
  Matrix<N, M, T>& operator-=(const Matrix<N, M, T>& matr);
  Matrix<N, M, T> operator*(T num) const;
  template <size_t P>
  Matrix<N, P, T> operator*(const Matrix<M, P, T>& matr);
  const T& operator()(size_t fir, size_t sec) const;
  T& operator()(size_t fir, size_t sec);
  template <size_t K, size_t L>
  bool operator==(const Matrix<K, L, T>& matr);
  bool operator==(const std::vector<std::vector<T>>& matr);
  Matrix<M, N, T> Transposed();
  T Trace();
};
template <size_t N, size_t M, typename T>
Matrix<N, M, T>::Matrix() {
  matr_.resize(N, std::vector<T>(M));
}
template <size_t N, size_t M, typename T>
Matrix<N, M, T>::Matrix(std::vector<std::vector<T>> matr) {
  matr_.resize(N, std::vector<T>(M));
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < M; ++j) {
      matr_[i][j] = matr[i][j];
    }
  }
}

template <size_t N, size_t M, typename T>
Matrix<N, M, T>::Matrix(T elem) {
  matr_.resize(N, std::vector<T>(M, elem));
}

template <size_t N, size_t M, typename T>
Matrix<N, M, T>& Matrix<N, M, T>::operator+=(const Matrix<N, M, T>& matr) {
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < M; ++j) {
      matr_[i][j] += matr(i, j);
    }
  }
  return *this;
}
template <size_t N, size_t M, typename T>

Matrix<N, M, T>& Matrix<N, M, T>::operator-=(const Matrix<N, M, T>& matr) {
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < M; ++j) {
      matr_[i][j] -= matr(i, j);
    }
  }
  return *this;
}
template <size_t N, size_t M, typename T>
Matrix<N, M, T> Matrix<N, M, T>::operator*(T num) const {
  Matrix<N, M, T> res(*this);
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < M; ++j) {
      res.matr_[i][j] *= num;
    }
  }
  return res;
}
template <size_t N, size_t M, typename T>
template <size_t L>
Matrix<N, L, T> Matrix<N, M, T>::operator*(const Matrix<M, L, T>& matr) {
  Matrix<N, L, T> result;
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < L; ++j) {
      for (size_t k = 0; k < M; ++k) {
        result(i, j) += matr_[i][k] * matr(k, j);
      }
    }
  }
  return result;
}
template <size_t N, size_t M, typename T>

bool Matrix<N, M, T>::operator==(const std::vector<std::vector<T>>& matr) {
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < M; ++j) {
      if (matr_[i][j] != matr[i][j]) {
        return false;
      }
    }
  }
  return true;
}
template <size_t N, size_t M, typename T>

template <size_t F, size_t H>
bool Matrix<N, M, T>::operator==(const Matrix<F, H, T>& matr) {
  if (N != F or M != H) {
    return false;
  }
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; i < M; ++j) {
      if (matr_[i][j] != matr[i][j]) {
        return false;
      }
    }
  }
  return true;
}
template <size_t N, size_t M, typename T>

Matrix<N, M, T> Matrix<N, M, T>::operator+(const Matrix& matr) const {
  Matrix res(*this);
  res += matr;
  return res;
}
template <size_t N, size_t M, typename T>

Matrix<N, M, T> Matrix<N, M, T>::operator-(const Matrix<N, M, T>& matr) const {
  Matrix res(*this);
  res -= matr;
  return res;
}
template <size_t N, size_t M, typename T>

const T& Matrix<N, M, T>::operator()(size_t fir, size_t sec) const {
  return matr_[fir][sec];
}
template <size_t N, size_t M, typename T>

T& Matrix<N, M, T>::operator()(size_t fir, size_t sec) {
  return matr_[fir][sec];
}
template <size_t N, size_t M, typename T>

Matrix<M, N, T> Matrix<N, M, T>::Transposed() {
  Matrix<M, N, T> result;
  for (size_t i = 0; i < M; ++i) {
    for (size_t j = 0; j < N; ++j) {
      result(i, j) = matr_[j][i];
    }
  }
  return result;
}
template <size_t N, size_t M, typename T>

T Matrix<N, M, T>::Trace() {
  T res = 0;
  for (size_t i = 0; i < M; ++i) {
    res += matr_[i][i];
  }
  return res;
}
