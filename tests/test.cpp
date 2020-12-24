// Copyright 2020 ivan <ikhonyak@gmail.com>
#include <cache.hpp>
#include <gtest/gtest.h>

TEST(Example, EmptyTest) {
  Cache experiments;
  experiments.GenerateSizes();
  experiments.StraightExperiment();
  experiments.BackExperiment();
  std::cout << experiments;
  EXPECT_TRUE(true);
}

