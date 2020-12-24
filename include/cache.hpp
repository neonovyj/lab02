// Copyright 2020 ivan <ikhonyak@gmail.com>

#ifndef INCLUDE_HEADER_HPP_
#define INCLUDE_HEADER_HPP_

#define NATURALS unsigned int

#include <vector>
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <chrono>
#include <algorithm>
#include <functional>

using std::vector;
using std::cout;
using std::cin;
using std::string;
using std::stringstream;
using std::endl;
using std::ostream;

struct Data;

class Cache {
  struct ExperimentData
  {
    std::string travelOrder;
    std::vector<double> experimentTime;
    ExperimentData(std::string order,const std::vector<double>& time) // const link - ?
        : travelOrder(std::move(order)), experimentTime(time) {} // std::move()- ?
    // initialisation list - ?
  };

 public:
  void GenerateSizes();
  void warming(int* arr, int& h, const double& buffer);
  void StraightExperiment();
  void WarmArray(int * arrayToWarm, size_t size);
  int* GenerateArray(size_t bufferSize);

  friend std::ostream& operator<<(std::ostream& os, const Cache& experiments);
 private:
  // 0.125 MB << 1 MB << 2 MB << 4 MB << 8 MB << 12 MB
  std::vector<double> sizes;
  std::vector<double> caches {0.125, 2, 8};
  std::vector<ExperimentData> data;

  const double min = 0.256;
  const double max = 8;
  const int step = 16;
};



#endif // INCLUDE_HEADER_HPP_


