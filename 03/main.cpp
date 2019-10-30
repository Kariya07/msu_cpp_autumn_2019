#include "matrix.h"

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