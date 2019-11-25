class BigInt {
    bool sign_flag;
    int *digit;
    long buf_size;
    long num_size;
    void expand_buf();

public:
    BigInt();

    ~BigInt();

    BigInt(const BigInt &big_int);

    BigInt &operator=(const BigInt &big_int);

    BigInt(const int &number);

    BigInt(const long &number);

    friend std::ostream &operator<<(std::ostream &stream, const BigInt &big_int);

    BigInt operator-() const;

    friend bool operator==(const BigInt &a, const BigInt &b);

    bool operator!=(const BigInt &a);

    friend bool operator<(const BigInt &a, const BigInt &b);

    bool operator<=(const BigInt &a);

    bool operator>(const BigInt &a);

    bool operator>=(const BigInt &a);

    friend BigInt operator+(const BigInt &a, const BigInt &b);

    friend BigInt operator-(const BigInt &a, const BigInt &b);

    void check_zeros();
};
