#include <iostream>
#include <sstream>
#include <cassert>
#include <vector>

enum State {
    Str, Arg
};

template<class T>
void process(std::vector<std::string> &form_args, const T &val) {
    std::stringstream stream;
    stream << val;
    form_args.push_back(stream.str());
}

template<class T, class... Args>
void process(std::vector<std::string> &form_args, const T &val, const Args &... args) {
    std::stringstream stream;
    stream << val;
    form_args.push_back(stream.str());
    process(form_args, args...);
}

template<class... Args>
std::string format(const std::string &form, const Args &... args) {
    std::string res;
    std::vector<std::string> form_args;
    process(form_args, args...);

    State state = Str;
    int i = 0;
    std::string number;
    size_t idx = 0;
    while (i < form.size()) {
        switch (state) {
            case Str:
                if (form[i] == '}') {
                    throw std::runtime_error("not opened {}");
                }
                if (form[i] == '{') {
                    state = Arg;
                    number.clear();
                } else {
                    res += form[i];
                }
                i++;
                break;
            case Arg:
                if (form[i] == '}') {
                    idx = stoul(number);
                    if (idx >= form_args.size()) {
                        throw std::runtime_error("number of argument out of range");
                    }
                    res += form_args[idx];
                    state = Str;
                    i++;
                    break;
                }
                if (form[i] < '0' || form[i] > '9') {
                    throw std::runtime_error("expected number in {}");
                } else {
                    number += form[i];
                }
                i++;
                break;
        }
    }
    if (state == Arg) {
        throw std::runtime_error("not closed {}");
    }
    return res;
}

int main() {
    try {
        auto text = format("{1}+{1} = {0}", 2, "one");
        assert(text == "one+one = 2");
        //text = format("{0a}", 1);
        text = format("{0} {2} {1}", 2, 3);
        //text = format("}", 1);
        //text = format("{0", "err");
    }
    catch (std::runtime_error &err) {
        std::cout << err.what() << std::endl;
        return 0;
    }
    return 0;
}
