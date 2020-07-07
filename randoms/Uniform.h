#ifndef RANDOM_NUMBER_GENERATOR_H_
#define RANDOM_NUMBER_GENERATOR_H_

#include <cinttypes>
#include <random>
#include <tuple>

namespace Randoms {

class Normal {
 public:
  Normal() : rd(), gen_for_normal(rd()) {}

  std::tuple<uint64_t, double*> get_normal(double mean, double stddev, uint64_t count) {
    std::normal_distribution<> distrib{mean, stddev};
    double* res = new double[count];
    for (uint64_t n = 0; n < count; n++) res[n] = distrib(gen_for_normal);
    return {count, res};
  }

  std::random_device rd;
  std::mt19937_64 gen_for_normal;
};

}

#endif