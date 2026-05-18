#pragma once
#include <iostream>

// RAII helper: auto-appends '\n' and flushes when the statement ends.
struct _LogLine {
    std::ostream& os;
    explicit _LogLine(std::ostream& s) : os(s) {}
    ~_LogLine() { os << '\n'; os.flush(); }
    template<typename T>
    _LogLine& operator<<(const T& v) { os << v; return *this; }
};

// LOG  — stdout, auto newline + flush
// LOGE — stderr, auto newline + flush
#define LOG  _LogLine(std::cout) << "[" << __FILE__ << ":" << __LINE__ << "] "
#define LOGE _LogLine(std::cerr) << "[ERR][" << __FILE__ << ":" << __LINE__ << "] "
