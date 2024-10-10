// Randoms.cpp
#include "randoms.hpp"

namespace globalConfigsForRandoms {
std::random_device randomDevice;
std::mt19937 gen(randomDevice());
std::bernoulli_distribution distro(0.5);
}

bool getRandBool() {
  return globalConfigsForRandoms::distro(globalConfigsForRandoms::gen);
}