#include <iostream>
#include <sstream>
#include <cassert>

enum class Error {
    NoError,
    CorruptedArchive
};

struct Data {
    uint64_t a;
    bool b;
    uint64_t c;

    template<class Serializer>
    Error serialize(Serializer &serializer) {
        return serializer(a, b, c);
    }
};

class Serializer {
    static constexpr char Separator = ' ';
public:
    explicit Serializer(std::ostream &out) : out_(out) {}

    template<class T>
    Error save(T &object) {
        return object.serialize(*this);
    }

    template<class... ArgsT>
    Error operator()(ArgsT... args) {
        return process(args...);
    }

private:
    std::ostream &out_;

    template<class T>
    Error process(T &val) {
        return save_val(val);
    }

    template<class T, class... Args>
    Error process(T val, Args... args) {
        if (save_val(val) == Error::CorruptedArchive) {
            return Error::CorruptedArchive;
        } else {
            return process(args...);
        }
    }

    template<class T>
    Error save_val(T) {
        return Error::CorruptedArchive;
    }

    Error save_val(bool val) {
        if (val) {
            out_ << "true";
            out_ << Separator;
        } else {
            out_ << "false";
            out_ << Separator;
        }
        return Error::NoError;
    }

    Error save_val(uint64_t val) {
        out_ << val;
        out_ << Separator;
        return Error::NoError;
    }
};

class Deserializer {
public:
    explicit Deserializer(std::istream &in) : in_(in) {}

    template<class T>
    Error load(T &object) {
        return object.serialize(*this);
    }

    template<class... ArgsT>
    Error operator()(ArgsT &... args) {
        return process(args...);
    }

private:
    std::istream &in_;

    template<class T>
    Error process(T &val) {
        return load_val(val);
    }

    template<class T, class... Args>
    Error process(T &val, Args &... args) {
        if (load_val(val) == Error::CorruptedArchive) {
            return Error::CorruptedArchive;
        } else {
            return process(args...);
        }
    }

    template<class T>
    Error load_val(T) {
        return Error::CorruptedArchive;
    }

    Error load_val(bool &val) {
        std::string text;
        in_ >> text;
        if (text == " ") {
            return Error::NoError;
        }
        if (text == "true")
            val = true;
        else if (text == "false")
            val = false;
        else
            return Error::CorruptedArchive;

        return Error::NoError;
    }

    Error load_val(uint64_t &val) {
        in_ >> val;
        return Error::NoError;
    }
};

int main() {
    Data x{1, false, 17};

    std::stringstream stream;
    std::stringstream stream1;

    Serializer serializer(stream);
    serializer.save(x);
    stream1 << "err_str" << " " << std::endl;

    Serializer serializer1(stream1);
    serializer1.save(x);

    Data y{0, false, 0};
    serializer1.save(y);

    Deserializer deserializer1(stream1);
    const Error err1 = deserializer1.load(y);

    assert(err1 == Error::CorruptedArchive);

    Deserializer deserializer(stream);
    const Error err = deserializer.load(y);

    assert(err == Error::NoError);
    assert(x.a == y.a);
    assert(x.b == y.b);
    assert(x.c == y.c);

    return 0;
}
