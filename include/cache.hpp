// Copyright 2020 ivan <ikhonyak@gmail.com>

#ifndef INCLUDE_HEADER_HPP_
#define INCLUDE_HEADER_HPP_

#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <chrono>
#include <functional>

using std::cout;
using std::cin;
using std::string;
using std::stringstream;
using std::ostream;



class Cache {
  struct ExperimentData
  {
    std::string travelOrder;
    std::vector<double> experimentTime;
    ExperimentData(std::string order,const std::vector<double>& time)
        : travelOrder(std::move(order)), experimentTime(time) {}
  };

 public:
  void CacheSizes();
  void Warming(int * arrayToWarm, size_t size);
  void StraightExperiment();
  void BackExperiment();
  void RandomExperiment();
  int* GenerateArray(size_t bufferSize);

  friend std::ostream& operator<<(std::ostream& os, const Cache& experiments);
 private:
  std::vector<ExperimentData> data;
  std::vector<double> sizes;
  const std::vector<double> cache = {0.256, 1, 8};
  double degree = 1;
  const int step = 16;
  const int quantity = 1000;
};

#endif // INCLUDE_HEADER_HPP_
