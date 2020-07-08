#ifndef UNIFORM_H_
#define UNIFORM_H_

#include <cinttypes>
#include <random>
#include <tuple>

namespace Randoms {

class Uniform {
 public:
  Uniform() : rd(), gen_for_uniform(rd()){}
  // get_uniform works independently from other get functions in this class.
  std::tuple<uint64_t, double*> get_uniform(double left, double right, uint64_t count) {
    std::uniform_real_distribution<> distrib(left, right);
    double* res = new double[count];
    for (uint64_t n = 0; n < count; n++) res[n] = distrib(gen_for_uniform);
    return {count, res};
  }
  std::random_device rd;
  std::mt19937_64 gen_for_uniform;
};

}

#endif