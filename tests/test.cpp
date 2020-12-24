// Copyright 2020 Your Name <your_email>
#include <cache.hpp>
#include <gtest/gtest.h>

TEST(Example, EmptyTest) {
  Cache experiments;
  experiments.GenerateSizes();
  experiments.StraightExperiment();
  std::cout << experiments;
  EXPECT_TRUE(true);
}

