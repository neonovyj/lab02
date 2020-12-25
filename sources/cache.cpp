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
  [[maybe_unused]]int k; //в двойных скобках, чтобы не было никаких предупреждений
  for (size_t i = 0; i < size; ++i) { //проходимся по всему массиву, мы не знаем размер массива, тк его тут нет поэтому внизу передаем его в функцию, а тут получаем наш size
    k = arrayToWarm[i]; //обращаемся к массиву по его размеру, прогреваем
  }
}

int* Cache::GenerateArray(size_t bufferSize) { //получаем размер массива, который нужно сгенирировать, а выводим указатель на int, то есть сам динамический массив
  int * generatedArray = new int [bufferSize];  //создаем массив

  for (size_t i = 0; i < bufferSize; ++i) { //заполняем массив
    generatedArray[i] = rand()%100; //ограничиваем значение функции сотней
  }

  return generatedArray; //возвращаем массив
}

 //прямой эксперимент
void Cache::StraightExperiment() {
  std::vector<double> time;//записываем время
  for (const double& size : sizes) { //перебираем массив sizes и записываем все в size
    size_t bufferSize = ( size * 1024 * 1024 ) / 4; //записываем наши проходы. size_t чтобы все поместилось. Берем size, переводим в байты
    int* array = GenerateArray(bufferSize); //инициализирует array с помощью функции GenerateArray

    Warming(array, bufferSize); //прогреваем, передав наш массив. Мы не знаем размер массива, тк его тут нет поэтому bufferSize передаем его в функцию

    [[maybe_unused]]int k;
    auto start = std::chrono::system_clock::now();
    for (size_t i = 0; i < bufferSize * 1000; i+=step) { // EXPERIMENT на эксперименте нам нужно пронгнать его тыщу раз, поэтому умножаем//step??
      k = array[i % 1000]; //записываем все значения массива
    }
    auto end = std::chrono::system_clock::now();
    time.push_back(static_cast<double>(std::chrono::nanoseconds((end - start) / 1000).count())); //записываем время в массив, который выше
    //time.push_back(static_cast<double>(std::chrono::nanoseconds(end - start).count()));
    delete[] array;  //нада нет?
  }
  data.emplace_back("straight", time); //передаем все элементы для конструирования структуры (string порядок и время )
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
    for (size_t i = bufferSize * 1000; i > 0 ; i-=step) { // EXPERIMENT
      k = array[i % 1000];
    }
    auto end = std::chrono::system_clock::now();

   time.push_back(static_cast<double>(std::chrono::nanoseconds((end - start) / 1000).count()));
    delete[] array;
  }
  data.emplace_back("back", time);
}

void Cache::RandomExperiment() {
  std::vector<double> time;
  for (const double& size : sizes) {
    size_t bufferSize = ( size * 1024 * 1024 ) / 4;
    int* array = GenerateArray(bufferSize);

    std::vector<unsigned>::iterator start, end;

    std::vector<unsigned> arr;

    for (unsigned i = 0; i < bufferSize; i += step) arr.emplace_back(i);
    start = arr.begin();
    end = arr.end();
    shuffle(start, end, std::mt19937(std::random_device()()));

    Warming(array, bufferSize);

    auto startTime = std::chrono::high_resolution_clock::now();
    [[maybe_unused]]int k;
    for (size_t i = 0; i < bufferSize * 1000; i+=step) {
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
