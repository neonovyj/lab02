// Copyright 2020 ivan <ikhonyak@gmail.com>
#include <cache.hpp>
#include <random>

void Cache::CacheSizes(){
  sizes.push_back(cache[0]*0.5);
  while (degree < cache[2] * 1.5){
    sizes.push_back(degree);
    degree = degree*2;
  }
  sizes.push_back(cache[2]*1.5);
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

void Cache::StraightExperiment() {
  std::vector<double> time;
  for (const double& size : sizes) {
    size_t bufferSize = ( size * 1024 * 1024 ) / 4;
    int* array = GenerateArray(bufferSize);
    Warming(array, bufferSize);
    [[maybe_unused]]int k;
    auto start = std::chrono::system_clock::now();
    for (size_t i = 0; i < bufferSize * quantity; i+=step) {
      k = array[i % 1000];
    }
    auto end = std::chrono::system_clock::now();
    time.push_back(static_cast<double>(std::chrono::nanoseconds((end - start) / quantity).count()));
    delete[] array;
  }
  data.emplace_back("straight", time);
}

void Cache::BackExperiment() {
  std::vector<double> time;
  for (const double& size : sizes) {
    size_t bufferSize = ( size * 1024 * 1024 ) / 4;
    int* array = GenerateArray(bufferSize);
    Warming(array, bufferSize);
    [[maybe_unused]]int k;
    auto start = std::chrono::system_clock::now();
    for (size_t i = bufferSize * quantity; i > 0 ; i-=step) {
      k = array[i % 1000];
    }
    auto end = std::chrono::system_clock::now();
    time.push_back(static_cast<double>(std::chrono::nanoseconds((end - start) / quantity).count()));
    delete[] array;
  }
  data.emplace_back("back", time);
}

void Cache::RandomExperiment() {
  std::vector<double> time;
  for (const double& size : sizes) {
    size_t bufferSize = ( size * 1024 * 1024 ) / 4;
    int* array = GenerateArray(bufferSize);
    std::vector<size_t>::iterator start, end;
    std::vector<size_t> arr;
    for (size_t i = 0; i < bufferSize; i += step) arr.emplace_back(i);
    start = arr.begin();
    end = arr.end();
    shuffle(start, end, std::mt19937(std::random_device()()));

    Warming(array, bufferSize);
    auto startTime = std::chrono::high_resolution_clock::now();
    [[maybe_unused]]int k;
    for (size_t i = 0; i < bufferSize * quantity; i+=step) {
      k = array[i % 1000];
    }
    auto endTime = std::chrono::high_resolution_clock::now();
    time.push_back(static_cast<double>(std::chrono::nanoseconds((endTime - startTime) / 1000).count()));
    delete[] array;
  }
  data.emplace_back("Random", time);
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
