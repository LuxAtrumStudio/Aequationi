#include <pessum.h>
#include <vector>
#include "../../aequatio.hpp"
#include "../symbol.hpp"
#include "../types.hpp"
#include "../vector/vector.hpp"
#include "matrix.hpp"

aequatio::Matrix::Matrix() {
  rows = 0;
  cols = 0;
}

aequatio::Matrix::Matrix(int m, int n) {
  rows = m;
  cols = n;
  mat_terms = std::vector<std::vector<Symbol>>(
      rows, std::vector<Symbol>(cols, Symbol()));
}

aequatio::Matrix::Matrix(int m, int n, Symbol val){
  rows = m;
  cols = n;
  mat_terms = std::vector<std::vector<Symbol>>(rows, std::vector<Symbol>(cols,val));
}

aequatio::Matrix::Matrix(std::vector<std::vector<Symbol>> elements) {
  rows = elements.size();
  cols = elements[0].size();
  mat_terms = elements;
}

aequatio::Matrix::~Matrix() {
  mat_terms.clear();
  rows = 0;
  cols = 0;
}

int aequatio::Matrix::Type() { return (SY_MATRIX); }

std::string aequatio::Matrix::String() {
  std::string mat_str = "[";
  for (int i = 0; i < rows; i++) {
    mat_str += "[";
    for (int j = 0; j < cols; j++) {
      mat_str += mat_terms[i][j].String();
      if (j != cols - 1) {
        mat_str += ",";
      }
    }
    mat_str += "]";
    if (i != rows - 1) {
      mat_str += ",";
    }
  }
  mat_str += "]";
  return (mat_str);
}

bool aequatio::operator==(const Matrix& a, const Matrix& b) {
  bool same = true;
  if (a.rows != b.rows || a.cols != b.cols) {
    same = false;
  }
  for (int i = 0; i < a.rows && same == true; i++) {
    for (int j = 0; j < a.cols && same == true; j++) {
      if (a.mat_terms[i][j] != b.mat_terms[i][j]) {
        same = false;
      }
    }
  }
  return (same);
}

bool aequatio::operator<(const Matrix& a, const Matrix& b) {
  bool less = true;
  for (int i = 0; i < a.rows && i < b.rows && less == true; i++) {
    for (int j = 0; j < a.cols && j < b.cols && less == true; j++) {
      if (a.mat_terms[i][j] > b.mat_terms[i][j]) {
        less = false;
      }
    }
  }
  return (less);
}

bool aequatio::operator!=(const Matrix& a, const Matrix& b) {
  return (!(a == b));
}

bool aequatio::operator>(const Matrix& a, const Matrix& b) { return (b > a); }

bool aequatio::operator<=(const Matrix& a, const Matrix& b) {
  return (!(b < a));
}

bool aequatio::operator>=(const Matrix& a, const Matrix& b) {
  return (!(a < b));
}

aequatio::Matrix aequatio::operator+(const Matrix& a, const Matrix& b) {
  Matrix out(std::min(a.rows, b.rows), std::min(a.cols, b.cols));
  for (int i = 0; i < out.rows; i++) {
    for (int j = 0; j < out.cols; j++) {
      out.mat_terms[i][j] = a.mat_terms[i][j] + b.mat_terms[i][j];
    }
  }
  return (out);
}

aequatio::Matrix aequatio::operator-(const Matrix& a, const Matrix& b) {
  Matrix out(std::min(a.rows, b.rows), std::min(a.cols, b.cols));
  for (int i = 0; i < out.rows; i++) {
    for (int j = 0; j < out.cols; j++) {
      out.mat_terms[i][j] = a.mat_terms[i][j] - b.mat_terms[i][j];
    }
  }
  return (out);
}

aequatio::Matrix aequatio::operator*(const Matrix& a, const Matrix& b) {
  Matrix out(std::min(a.rows, b.rows), std::min(a.cols, b.cols));
  for (int i = 0; i < out.rows; i++) {
    for (int j = 0; j < out.cols; j++) {
      out.mat_terms[i][j] = a.mat_terms[i][j] * b.mat_terms[i][j];
    }
  }
  return (out);
}

aequatio::Matrix aequatio::operator/(const Matrix& a, const Matrix& b) {
  Matrix out(std::min(a.rows, b.rows), std::min(a.cols, b.cols));
  for (int i = 0; i < out.rows; i++) {
    for (int j = 0; j < out.cols; j++) {
      out.mat_terms[i][j] = a.mat_terms[i][j] / b.mat_terms[i][j];
    }
  }
  return (out);
}

aequatio::Matrix aequatio::operator+(const Matrix& a, const Vector& b) {
  Matrix out = a;
  if (a.rows == b.length) {
    for (int i = 0; i < a.cols; i++) {
      for (int j = 0; j < a.rows; j++) {
        out.mat_terms[j][i] = a.mat_terms[j][i] + b.vec_terms[j];
      }
    }
  } else if (a.cols == b.length) {
    for (int i = 0; i < a.rows; i++) {
      for (int j = 0; j < a.cols; j++) {
        out.mat_terms[i][j] = a.mat_terms[i][j] + b.vec_terms[j];
      }
    }
  } else {
    pessum::Log(pessum::WARNING,
                "Matrix plus a vector must have the same number of terms in at "
                "least one dim not %ix%i and %i",
                "Matrix/Operator+(Matrix,Vector)", a.rows, a.cols, b.length);
  }
  return (out);
}

aequatio::Matrix aequatio::operator-(const Matrix& a, const Vector& b) {
  Matrix out = a;
  if (a.rows == b.length) {
    for (int i = 0; i < a.cols; i++) {
      for (int j = 0; j < a.rows; j++) {
        out.mat_terms[j][i] = a.mat_terms[j][i] - b.vec_terms[j];
      }
    }
  } else if (a.cols == b.length) {
    for (int i = 0; i < a.rows; i++) {
      for (int j = 0; j < a.cols; j++) {
        out.mat_terms[i][j] = a.mat_terms[i][j] - b.vec_terms[j];
      }
    }
  } else {
    pessum::Log(pessum::WARNING,
                "Matrix sub a vector must have the same number of terms in at"
                "least one dim not %ix%i and %i",
                "Matrix/Operator-(Matrix,Vector)", a.rows, a.cols, b.length);
  }
  return (out);
}

aequatio::Matrix aequatio::operator*(const Matrix& a, const Vector& b) {
  Matrix out = a;
  if (a.rows == b.length) {
    for (int i = 0; i < a.cols; i++) {
      for (int j = 0; j < a.rows; j++) {
        out.mat_terms[j][i] = a.mat_terms[j][i] * b.vec_terms[j];
      }
    }
  } else if (a.cols == b.length) {
    for (int i = 0; i < a.rows; i++) {
      for (int j = 0; j < a.cols; j++) {
        out.mat_terms[i][j] = a.mat_terms[i][j] * b.vec_terms[j];
      }
    }
  } else {
    pessum::Log(pessum::WARNING,
                "Matrix multiplied by a vector must have the same number of "
                "terms in at least one dim not %ix%i and %i",
                "Matrix/Operator*(Matrix,Vector)", a.rows, a.cols, b.length);
  }
  return (out);
}

aequatio::Matrix aequatio::operator/(const Matrix& a, const Vector& b) {
  Matrix out = a;
  if (a.rows == b.length) {
    for (int i = 0; i < a.cols; i++) {
      for (int j = 0; j < a.rows; j++) {
        out.mat_terms[j][i] = a.mat_terms[j][i] / b.vec_terms[j];
      }
    }
  } else if (a.cols == b.length) {
    for (int i = 0; i < a.rows; i++) {
      for (int j = 0; j < a.cols; j++) {
        out.mat_terms[i][j] = a.mat_terms[i][j] / b.vec_terms[j];
      }
    }
  } else {
    pessum::Log(
        pessum::WARNING,
        "Matrix divided by a vector must have the same number of terms in at "
        "least one dim not %ix%i and %i",
        "Matrix/Operator/(Matrix,Vector)", a.rows, a.cols, b.length);
  }
  return (out);
}

aequatio::Matrix aequatio::operator+(const Matrix& a, const Symbol& b) {
  Matrix out(a.rows, a.cols);
  for (int i = 0; i < out.rows; i++) {
    for (int j = 0; j < out.cols; j++) {
      out.mat_terms[i][j] = a.mat_terms[i][j] + b;
    }
  }
  return (out);
}

aequatio::Matrix aequatio::operator-(const Matrix& a, const Symbol& b) {
  Matrix out(a.rows, a.cols);
  for (int i = 0; i < out.rows; i++) {
    for (int j = 0; j < out.cols; j++) {
      out.mat_terms[i][j] = a.mat_terms[i][j] - b;
    }
  }
  return (out);
}

aequatio::Matrix aequatio::operator*(const Matrix& a, const Symbol& b) {
  Matrix out(a.rows, a.cols);
  for (int i = 0; i < out.rows; i++) {
    for (int j = 0; j < out.cols; j++) {
      out.mat_terms[i][j] = a.mat_terms[i][j] * b;
    }
  }
  return (out);
}

aequatio::Matrix aequatio::operator/(const Matrix& a, const Symbol& b) {
  Matrix out(a.rows, a.cols);
  for (int i = 0; i < out.rows; i++) {
    for (int j = 0; j < out.cols; j++) {
      out.mat_terms[i][j] = a.mat_terms[i][j] / b;
    }
  }
  return (out);
}
