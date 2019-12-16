#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <thread>
#include <queue>
#include <iterator>

const size_t chunk_size = 1024 * 1024;

void merge_files(int num_of_files, const std::vector<std::string> &names) {
    typedef std::istream_iterator<uint64_t> It;
    typedef std::pair<It, It> Range;
    auto less_first = [](const Range &lhs, const Range &rhs) {
        return *lhs.first > *rhs.first;
    };
    std::priority_queue<Range, std::vector<Range>, decltype(less_first)> heap(less_first);
    std::fstream file[num_of_files];
    while (num_of_files > 0) {
        file[num_of_files - 1].open(names[num_of_files - 1]);
        heap.emplace(It(file[num_of_files - 1]), It());
        num_of_files--;
    }
    std::ofstream out;
    const std::string newFileName = "sorted.txt";
    out.open(newFileName);
    if (!out) {
        throw (std::runtime_error(newFileName));
    }
    while (!heap.empty()) {
        auto range = heap.top();
        heap.pop();
        out << *range.first << ' ';
        *range.first++;
        if (range.first != range.second)
            heap.emplace(range);
    }
    out.close();
}

void BigFileSort(const std::string &FileName) {
    std::ifstream file;
    file.open(FileName);
    if (!file) {
        throw (std::runtime_error(FileName));
    }

    size_t num_of_files = 0;
    auto *buffer = new uint64_t[chunk_size]; //8byte*1024*1024 = 8kb*1024 = 8 mb
    std::vector<std::string> names;
    while (file) {
        file.read(reinterpret_cast<char *>(buffer), chunk_size);
        size_t read_size = file.gcount() / sizeof(uint64_t);
        size_t middle = read_size / 2;
        names.push_back(std::to_string(num_of_files) + ".txt");
        names.push_back(std::to_string(num_of_files + 1) + ".txt");
        if (read_size > 0) {
            std::thread thread1([buffer, middle]() {
                                    std::sort(buffer, buffer + middle);
                                }
            );
            std::thread thread2([buffer, middle, read_size]() {
                                    std::sort(buffer + middle, buffer + read_size);
                                }
            );
            thread1.join();
            thread2.join();
            std::ofstream out;
            out.open(names[num_of_files]);
            if (!out) {
                delete[] buffer;
                throw (std::runtime_error(names[num_of_files]));
            }
            for (size_t i = 0; i < read_size / 2; i++) {
                out << buffer[i] << ' ';
            }
            out.close();
            out.open(names[num_of_files + 1]);
            if (!out) {
                delete[] buffer;
                throw (std::runtime_error(names[num_of_files + 1]));
            }
            for (size_t i = middle; i < read_size; i++) {
                out << buffer[i] << ' ';
            }
            out.close();
            num_of_files += 2;
        }
    }
    file.close();
    delete[] buffer;
    merge_files(num_of_files, names);
}

int main() {
    const std::string FileName = "test.txt";
    std::ofstream newfile(FileName);
    if (!newfile) {
        std::cerr << "Can't open file: " << FileName << '\n';
        return 1;
    }
    const size_t exprNum = 1000;
    for (size_t i = 0; i < exprNum; i++) {
        uint64_t num = std::rand() % 1000;
        newfile.write((char *) &num, sizeof(uint64_t));
    }
    newfile.close();
    try {
        BigFileSort(FileName);
    } catch (const std::runtime_error &err) {
        std::cerr << "Can't open file: " << err.what() << '\n';
        return 1;
    }
    return 0;
}
