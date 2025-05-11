#pragma once

#include <array>
#include <numeric>
#include <vector>

namespace mysph {

template<typename T>
using vec3 = std::array<T, 3>;

template <typename T>
struct Particle {
    Particle() = default;
    Particle(vec3<T> r, vec3<T> v, T m, T rho): r(r), v(v), m(m), rho(rho) {}
    Particle(vec3<T> r): r(r) {}
    vec3<T> r = {T(0), T(0), T(0)}, v = {T(0), T(0), T(0)}, vstar = {T(0), T(0), T(0)}, Fv = {T(0), T(0), T(0)}, F = {T(0), T(0), T(0)};
    T m = T(0), rho = T(0), p = T(0); 
};

}