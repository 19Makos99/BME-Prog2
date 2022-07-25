#ifndef SUFFIX_ARRAY_H
#define SUFFIX_ARRAY_H

#include "memtrace.h"

#include <iostream>

class SuffixArray {
    std::string szoveg;
    std::string* suffix;
    void rendez();
    void suffixGeneralas();
    std::string leghosszabb_kozos_substr(const std::string&, size_t) const;
public:
    struct iterator {
        std::string* first;
        std::string* last;
    public:
        iterator(std::string* = nullptr, std::string* = nullptr);
        iterator& operator++();
        iterator operator++(int);
        bool operator==(const iterator&) const;
        bool operator!=(const iterator&) const;
        const std::string& operator*() const;
        const std::string* operator->() const;
    };

    SuffixArray(const char*);
    SuffixArray(const std::string& str);
    SuffixArray(const SuffixArray&);
    const char* c_str() const;
    iterator keres(const char) const;
    iterator keres(const char*) const;
    iterator keres(const std::string&) const;
    iterator end() const;
    SuffixArray operator+(const SuffixArray&) const;
    SuffixArray& operator=(const SuffixArray&);
    SuffixArray& operator+=(const SuffixArray&);
    std::string operator&(const SuffixArray&) const;
    ~SuffixArray();
};

#endif // !SUFFIX_ARRAY_H