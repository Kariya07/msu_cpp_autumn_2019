#include <iostream>
#include <cassert>
#include <limits>
#include "bigint.h"

void BigInt:: expand_buf() {
    int *exp_digit = new int[2 * buf_size];
    for (long i = 0; i < num_size; i++) {
        exp_digit[i] = digit[i];
    }
    delete[] digit;
    digit = exp_digit;
    buf_size *= 2;
}

void BigInt:: check_zeros() {
    long ind = num_size - 1;
    while (digit[ind] == 0 && ind >= 0) {
        num_size--;
        ind--;
    }
}

BigInt::BigInt() {
    buf_size = 1;
    num_size = 1;
    sign_flag = false;
    digit = new int[buf_size];
    digit[0] = 0;
}

BigInt:: ~BigInt() {
    delete[] digit;
}

BigInt::BigInt(const BigInt &big_int) {
    buf_size = big_int.buf_size;
    digit = new int[buf_size];
    num_size = big_int.num_size;
    for (long i = 0; i < num_size; i++) {
        digit[i] = big_int.digit[i];
    }
    sign_flag = big_int.sign_flag;
}

BigInt &BigInt::operator=(const BigInt &big_int) {
    if (this != &big_int) {
        delete[] digit;
        buf_size = big_int.buf_size;
        digit = new int[buf_size];
        num_size = big_int.num_size;
        for (long i = 0; i < num_size; i++) {
            digit[i] = big_int.digit[i];
        }
        sign_flag = big_int.sign_flag;
    }
    return *this;
}

BigInt::BigInt(const int &number) {
    int number_copy = number;
    if (number >= 0) {
        sign_flag = false;
    } else {
        number_copy = -number;
        sign_flag = true;
    }
    buf_size = sizeof(int);
    num_size = 0;
    digit = new int[buf_size];
    if (number == 0) {
        num_size = 1;
        digit[0] = 0;
    }
    while (number_copy != 0) {
        if (num_size == buf_size) {
            expand_buf();
        }
        digit[num_size] = number_copy % 10;
        number_copy /= 10;
        num_size++;
    }
}

BigInt::BigInt(const long &number) {
    long number_copy = number;
    if (number >= 0) {
        sign_flag = false;
    } else {
        number_copy = -number;
        sign_flag = true;
    }
    buf_size = 10;
    num_size = 0;
    digit = new int[buf_size];
    if (number == 0) {
        num_size = 1;
        digit[0] = 0;
    }
    while (number_copy != 0) {
        if (num_size == buf_size) {
            expand_buf();
        }
        digit[num_size] = number_copy % 10;
        number_copy /= 10;
        num_size++;
    }
}

std::ostream &operator<<(std::ostream &stream, const BigInt &big_int) {
    if (big_int.sign_flag) {
        stream << "-";
    }
    long ind = big_int.num_size - 1;
    while (ind >= 0) {
        stream << big_int.digit[ind];
        ind--;
    }
    return stream;
}

BigInt BigInt::operator-() const {
    BigInt neg_int = *this;
    if (neg_int != 0) {
        neg_int.sign_flag = !(this->sign_flag);
    }
    return neg_int;
}

bool operator==(const BigInt &a, const BigInt &b) {
    if (a.sign_flag != b.sign_flag) {
        return false;
    }
    if (a.num_size != b.num_size) {
        return false;
    }
    long ind = a.num_size - 1;
    while (ind >= 0) {
        if (a.digit[ind] != b.digit[ind]) {
            return false;
        }
        ind--;
    }
    return true;
}

bool operator<(const BigInt &a, const BigInt &b) {
    if (a == b) {
        return false;
    }
    if (a.sign_flag != b.sign_flag) {
        return a.sign_flag > b.sign_flag;
    }
    if (a.num_size != b.num_size) {
        return a.num_size < b.num_size;
    }
    long ind = a.num_size - 1;
    while (ind >= 0) {
        if (a.digit[ind] != b.digit[ind]) {
            if (a.sign_flag) {
                return a.digit[ind] > b.digit[ind];
            } else {
                return a.digit[ind] < b.digit[ind];
            }
        }
        ind--;
    }
    return false;
}

bool BigInt::operator!=(const BigInt &a) {
    return !(*this == a);
}

bool BigInt::operator<=(const BigInt &a) {
    return (*this < a || *this == a);
}

bool BigInt::operator>(const BigInt &a) {
    return !(*this <= a);
}

bool BigInt::operator>=(const BigInt &a) {
    return !(*this < a);
}

BigInt operator+(const BigInt &a, const BigInt &b) {
    if (a.sign_flag) {
        if (b.sign_flag) {
            return -(-a + (-b));
        } else {
            return b - (-a);
        }
    } else {
        if (b.sign_flag) {
            return a - (-b);
        }
    }
    int mind = 0;
    int local_sum = 0;
    long ind = 0;
    BigInt sum = 0;
    if (a.num_size > b.num_size) {
        sum = a;
    } else {
        sum = b;
    }
    while (ind < sum.num_size) {
        if (ind >= a.num_size) {
            local_sum = 0 + b.digit[ind] + mind;
        } else {
            if (ind >= b.num_size) {
                local_sum = a.digit[ind] + 0 + mind;
            } else {
                local_sum = a.digit[ind] + b.digit[ind] + mind;
            }
        }
        sum.digit[ind] = local_sum % 10;
        mind = local_sum / 10;
        ind++;
    }
    if (mind != 0) {
        if (sum.num_size == sum.buf_size) {
            sum.expand_buf();
        }
        sum.digit[ind] = mind;
        sum.num_size++;
    }
    return sum;
}

BigInt operator-(const BigInt &a, const BigInt &b) {
    if (b.sign_flag) {
        return a + (-b);
    } else {
        if (a.sign_flag) {
            return -((-a) + b);
        } else {
            if (a < b) {
                return -(b - a);
            }
        }
    }
    int mind = 0;
    int local_sub = 0;
    long ind = 0;
    BigInt sub = a;
    if (a == b) {
        sub = 0;
        return sub;
    }
    while (ind < a.num_size) {
        if (ind >= b.num_size) {
            if (a.digit[ind] - mind < 0) {
                local_sub = 10 + a.digit[ind] - mind;
                mind = 1;
            } else {
                local_sub = a.digit[ind] - mind;
                mind = 0;
            }
        } else {
            if ((a.digit[ind] - mind) < b.digit[ind]) {
                local_sub = 10 + a.digit[ind] - mind - b.digit[ind];
                mind = 1;
            } else {
                local_sub = a.digit[ind] - mind - b.digit[ind];
                mind = 0;
            }
        }
        if (local_sub == 0 && (ind == a.num_size - 1)) {
            sub.num_size--;
            break;
        }
        sub.digit[ind] = local_sub;
        ind++;
    }
    if (mind != 0) {
        throw std::runtime_error("Sub error");
    }
    sub.check_zeros();
    return sub;
}
