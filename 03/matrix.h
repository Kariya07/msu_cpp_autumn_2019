#include <iostream>
#include <vector>
#include <cassert>

class Matrix {
private:
    size_t rows;
    size_t columns;
    std::vector<std::vector<int>> matrix;

    class Row {
    public:
        std::vector<int> *row;
        size_t n_column;

        Row(std::vector<int> *r, size_t n) {
            row = r;
            n_column = n;
        }

        int &operator[](const int &index2) {
            if (index2 > n_column || index2 <= 0) {
                throw std::out_of_range("Error []: invalid index2");
            }
            return (*row).at(index2);
        }
    };

public:
    Matrix(size_t row, size_t column) {
        rows = row;
        columns = column;
        matrix.resize(rows);
        for (int i = 0; i < rows; i++) {
            matrix[i].resize(columns);
        }
    }

    size_t getRows() {
        return rows;
    }

    size_t getColumns() {
        return columns;
    }

    bool operator==(const Matrix &other_matrix) const;

    bool operator!=(const Matrix &other_matrix) const;

    const Matrix &operator*=(const int &c);

    Row operator[](const int &index1) {
        //rows numbering: 1, 2, 3, ...
        if (index1 > rows || index1 <= 0) {
            throw std::out_of_range("Error []: invalid index1");
        }
        return Row(&((*this).matrix[index1]), columns);
    }
};

bool Matrix::operator==(const Matrix &other_matrix) const {
    if (rows != other_matrix.rows || columns != other_matrix.columns) {
        throw std::out_of_range("Error ==: incomparable matrices");
    }
    for (int i = 0; i < rows; i++) {
        if (matrix[i] != other_matrix.matrix[i]) {
            return false;
        }
    }
    return true;
}

bool Matrix::operator!=(const Matrix &other_matrix) const {
    if (rows != other_matrix.rows || columns != other_matrix.columns) {
        throw std::out_of_range("Error !=: incomparable matrices");
    }
    for (int i = 0; i < rows; i++) {
        if (matrix[i] != other_matrix.matrix[i]) {
            return true;
        }
    }
    return false;
}

const Matrix &Matrix::operator*=(const int &c) {
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < columns; j++) {
            matrix[i][j] *= c;
        }
    }
    return *this;
}

