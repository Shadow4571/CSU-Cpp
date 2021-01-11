#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stack>

template<typename T>
bool is_substack(std::stack<T>& source, std::stack<T>& sample) {
    if (source.empty() && !sample.empty() || !source.empty() && sample.empty())
        return false;

    bool find = false;
    std::stack<T> copy = std::stack<T>(sample);

    while (!source.empty() && !copy.empty()) {
        if (source.top() == copy.top()) {
            find = !find ? true : find;
            copy.pop();
        } else if (find) {
            if(source.size() < sample.size())
                return false;

            copy = std::stack<T>(sample);
        }

        source.pop();
    }

    return copy.empty();
}

template<typename T>
bool is_subsparsestack(std::stack<T>& source, std::stack<T>& sample) {
    if (source.empty() && !sample.empty() || !source.empty() && sample.empty())
        return false;

    while (!source.empty() && !sample.empty()) {
        if (source.top() == sample.top())
            sample.pop();

        source.pop();
    }

    return sample.empty();
}

void test_unit(std::stack<int> source, std::stack<int> sample, std::string sourceData, std::string sampleData) {
    std::stack<int> source_copy = std::stack<int>(source);
    std::stack<int> sample_copy = std::stack<int>(sample);

    std::cout << std::endl << "Source stack: " << sourceData << std::endl << "Sample stack: " << sampleData;
    std::cout << std::endl << std::boolalpha << "Is substack: " << is_substack(source, sample);
    std::cout << std::endl << std::boolalpha << "Is substack parse: " << is_subsparsestack(source_copy, sample_copy) << std::endl;
}

std::stack<int> stackFromString(std::string data) {
    std::stack<int> result = std::stack<int>();
    std::stringstream ss(data);
    std::string item;

    while (std::getline(ss, item, ' '))
        result.push(std::stoi(item));

    return result;
}

void test_from_file(std::filesystem::directory_entry file) {
    std::ifstream fileStream = std::ifstream(file.path(), std::ios_base::in);
    std::string sourceData, sampleData;
    std::getline(fileStream, sourceData);
    std::getline(fileStream, sampleData);

    test_unit(stackFromString(sourceData), stackFromString(sampleData), sourceData, sampleData);
    fileStream.close();
}

void run_tests(std::string tests_directory) {
    try {
        for (const std::filesystem::directory_entry file : std::filesystem::directory_iterator(tests_directory))
            test_from_file(file);
    } catch (std::filesystem::filesystem_error exp) {
        std::cout << exp.what() << std::endl;
    }
}

int main() {
    run_tests("tests");

    return 0;
}
