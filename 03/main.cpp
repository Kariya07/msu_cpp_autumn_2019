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
        std::vector<int> &row;
        size_t n_column;

        Row(std::vector<int> &r, size_t n) : row(r) {
            n_column = n;
        }

        int &operator[](const size_t &index2) {
            if (index2 > n_column || index2 <= 0) {
                throw std::out_of_range("Error []: invalid index2");
            }
            return row.at(index2 - 1);
        }
    };

public:
    Matrix(size_t row, size_t column) {
        rows = row;
        columns = column;
        matrix.resize(rows);
        for (size_t i = 0; i < rows; i++) {
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

    Matrix &operator*=(const int &c);

    Row operator[](const int &index1) {
        //rows numbering: 1, 2, 3, ...
        if (index1 > rows || index1 <= 0) {
            throw std::out_of_range("Error []: invalid index1");
        }
        return Row((*this).matrix[index1 - 1], columns);
    }
};

bool Matrix::operator==(const Matrix &other_matrix) const {
    if (rows != other_matrix.rows || columns != other_matrix.columns) {
        throw std::out_of_range("Error ==: incomparable matrices");
    }
    for (size_t i = 0; i < rows; i++) {
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
    for (size_t i = 0; i < rows; i++) {
        if (matrix[i] != other_matrix.matrix[i]) {
            return true;
        }
    }
    return false;
}

Matrix &Matrix::operator*=(const int &c) {
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < columns; j++) {
            matrix[i][j] *= c;
        }
    }
    return *this;
}

int main() {
    const size_t rows = 5;
    const size_t cols = 3;
    try {
        Matrix m(rows, cols);
        assert(m.getRows() == 5);
        assert(m.getColumns() == 3);

        m[1][2] = 5;
        assert(m[1][2] == 5);

        double x = m[4][1];
        assert(x == 0);

        m *= 3;
        assert(m[1][2] == 15);

        //int p1 = m[5][3]; //OK
        //int p2 = m[6][2]; //Error []: invalid index1
        //int p3 = m[2][6]; //Error []: invalid index2
        Matrix m1(5, 4);
        Matrix m2(5, 4);
        if (m1 != m2) {
            std::cout << "!= incorrect" << std::endl;
        }
        //m1, m - incomparable
        if (m1 == m) {
            std::cout << "== incorrect" << std::endl;
        }
    }
    catch (const std::out_of_range &err_str) {
        std::cout << err_str.what() << std::endl;
        return 0;
    }
    catch (const std::bad_alloc &err_str) {
        std::cout << err_str.what() << std::endl;
        return 0;
    }
    return 0;
}
