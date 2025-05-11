#pragma once

#include <numeric>
#include <cmath>

template <class T>
T operator*(std::array<T, 3> lhs, std::array<T, 3> rhs) {
    return std::inner_product(std::begin(lhs), std::end(lhs), std::begin(rhs), T(0));
}

template <class T>
std::array<T, 3> operator*(std::array<T, 3> lhs, T rhs) {
    return {lhs[0] * rhs, lhs[1] * rhs, lhs[2] * rhs};
}

template <class T>
std::array<T, 3> operator-(std::array<T, 3> lhs, std::array<T, 3> rhs) {
    return {lhs[0] - rhs[0], lhs[1] - rhs[1], lhs[2] - rhs[2]};
}

template <class T>
std::array<T, 3> operator+(std::array<T, 3> lhs, std::array<T, 3> rhs) {
    return {lhs[0] + rhs[0], lhs[1] + rhs[1], lhs[2] + rhs[2]};
}


template <class T>
std::ostream& operator<<(std::ostream& out, std::array<T, 3> a) {
    return out << '(' << a[0] << ',' << a[1] << ',' << a[2] << ')';
}

namespace mysph {

template <class T>
auto abs(T x) {
    return std::sqrt(x * x);
}

}