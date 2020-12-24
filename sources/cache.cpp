// Copyright 2020 ivan <ikhonyak@gmail.com>

#include <cache.hpp>
//1/2 * cache_sizes['1'] < 2^x < 2^(x+1) < ... < 2^(x+n) < 3/2 * cache_sizes['max']
//128kB<1MB<2MB<4MB<8MB<12MB
//vector<int> cache = {0.125, 1, 2, 4, 8, 12};

#include <random>

void Cache::GenerateSizes(){
  double n = 1;
  sizes.push_back(min/2);
  while (n<max*1.5){
    sizes.push_back(n);
    n = n*2;
  }
  sizes.push_back(max*1.5);
}

void Cache::Warming(int* arrayToWarm, size_t size) {
  [[maybe_unused]]int k;
  for (size_t i = 0; i < size; ++i) {
    k = arrayToWarm[i];
  }
}

int* Cache::GenerateArray(size_t bufferSize) {
  int * generatedArray = new int [bufferSize];

  for (size_t i = 0; i < bufferSize; ++i) {
    generatedArray[i] = rand()%100;
  }

  return generatedArray;
}

 //прямой эксперимент
void Cache::StraightExperiment() {
  std::vector<double> time;
  for (const double& size : sizes) {
    size_t bufferSize = ( size * 1024 * 1024 ) / 4;
    int* array = GenerateArray(bufferSize);

    Warming(array, bufferSize);

    [[maybe_unused]]int k;
    auto start = std::chrono::system_clock::now();
    for (size_t i = 0; i < bufferSize * 1000; ++i) { // EXPERIMENT
      k = array[i % 1000];
    }
    auto end = std::chrono::system_clock::now();
    time.push_back(static_cast<double>(std::chrono::nanoseconds(end - start).count()));
    delete[] array;  //нада нет?
  }
  data.emplace_back("straight", time);
}

//обратный
void Cache::BackExperiment() {
  std::vector<double> time;
  for (const double& size : sizes) {
    size_t bufferSize = ( size * 1024 * 1024 ) / 4;
    int* array = GenerateArray(bufferSize);

    Warming(array, bufferSize);

    [[maybe_unused]]int k;
    auto start = std::chrono::system_clock::now();
    for (size_t i = bufferSize; i > 0 * 1000; --i) { // EXPERIMENT
      k = array[i % 1000];
    }
    auto end = std::chrono::system_clock::now();
    time.push_back(static_cast<double>(std::chrono::nanoseconds(end - start).count()));
  }
  data.emplace_back("back", time);
}






















std::ostream& operator<<(std::ostream& os, const Cache& experiments) {
  for (size_t i = 0; i < experiments.data.size(); ++i) {
    os << R"(Invistigations: )" << std::endl <<
       R"(     travel_variant: )" << experiments.data[i].travelOrder << std::endl <<
       R"(     experiments: )" << std::endl;
    for (size_t j = 0; j < experiments.sizes.size(); ++j) {
      os << R"(           -experiment:
            number: )" << j+1 << std::endl <<
         R"(             input_data:
                buffer_size: )" << experiments.sizes[j] << " mb" << std::endl <<
         R"(             results:
                duration: )" << experiments.data[i].experimentTime[j] << " nanoseconds" << std::endl << std::endl;
    }
  }
  return os;
}
