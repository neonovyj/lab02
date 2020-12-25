// Copyright 2020 ivan <ikhonyak@gmail.com>
#include <cache.hpp>
#include <gtest/gtest.h>

TEST(Example, EmptyTest) {
  Cache experiments;
  experiments.CacheSizes();
  experiments.StraightExperiment();
  experiments.BackExperiment();
  experiments.RandomExperiment();
  std::cout << experiments;
}
