// Randoms.hpp
#ifndef RANDOMS_HPP
#define RANDOMS_HPP

#include <random>

namespace globalConfigsForRandoms {
extern std::random_device randomDevice;
extern std::mt19937 gen;
extern std::bernoulli_distribution distro;
}  // namespace globalConfigsForRandoms

bool getRandBool50();

#endif  // RANDOMS_HPP
