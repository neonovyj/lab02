// Copyright 2020 ivan <ikhonyak@gmail.com>
#include <cache.hpp>
#include <random> //128kB<1MB<2MB<4MB<8MB<12MB
//пишем void к чему принадлежит функция и ее имя
void Cache::CacheSizes(){ //генирирует вектор
  sizes.push_back(min/2); //берем l1 и делим пополам
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
//НИЖЕ принимем int, возврщаем указатель на int, тип выводимого значения- указатель на int, указатель на первый элемент, наш динамический массив
int* Cache::GenerateArray(size_t bufferSize) { //получаем размер массива, который нужно сгенирировать, а выводим указатель на int, то есть сам динамический массив
  int * generatedArray = new int [bufferSize];  //создаем массив, new- выделяем память
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
    [[maybe_unused]]int k; //в двойных скобках, чтобы не было никаких предупреждений
    auto start = std::chrono::system_clock::now();
    for (size_t i = 0; i < bufferSize * quantity; i+=step) { // EXPERIMENT на эксперименте нам нужно прогнать его тыщу раз, поэтому умножаем/
      k = array[i % 1000]; //записываем все значения массива
    }
    auto end = std::chrono::system_clock::now();
    time.push_back(static_cast<double>(std::chrono::nanoseconds((end - start) / quantity).count())); //записываем время в массив, который выше
    delete[] array;  //чтобы не было утечки памяти, выше используем new
  }
  data.emplace_back("straight", time); //передаем все элементы для конструирования структуры (string порядок и время )
}
//обратный
void Cache::BackExperiment() {
  std::vector<double> time; //принимаем время в вектор double time
  for (const double& size : sizes) { //при каждой итерации цикла Создаем переменную типа size_t(unsignet int)bufferSize
    size_t bufferSize = ( size * 1024 * 1024 ) / 4; // которая хранит в себе размер предполагаемого массива
    int* array = GenerateArray(bufferSize);//генирируем массив с помощью функции GenerateArray размера bufferSize
    Warming(array, bufferSize);
    [[maybe_unused]]int k; //в двойных скобках, чтобы не было никаких предупреждений
    auto start = std::chrono::system_clock::now();
    for (size_t i = bufferSize * quantity; i > 0 ; i-=step) { //проводим эксперимент, в котором замеряем время
      k = array[i % 1000];
    }
    auto end = std::chrono::system_clock::now();
    time.push_back(static_cast<double>(std::chrono::nanoseconds((end - start) / quantity).count()));//время передаем
    delete[] array;
  }
  data.emplace_back("back", time); //выводим все в вектор ExperimentData (47 строка hpp)
}

void Cache::RandomExperiment() {
  std::vector<double> time;
  for (const double& size : sizes) {
    size_t bufferSize = ( size * 1024 * 1024 ) / 4;
    int* array = GenerateArray(bufferSize);
    std::vector<size_t>::iterator start, end; //структура данных, которая используется для обращения к определенному элементу в контейнерах
    std::vector<size_t> arr;  //size_t - беззнаковый целый тип данных
    for (size_t i = 0; i < bufferSize; i += step) arr.emplace_back(i);
    start = arr.begin();
    end = arr.end();
    shuffle(start, end, std::mt19937(std::random_device()())); // shuffle- перемешивает элементы последовательности в случайном порядке.
    //генератор псевдослучайных чисел, //Формирует случайную последовательность с помощью внешнего устройства.
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

std::ostream& operator<<(std::ostream& os, const Cache& experiments) { //ostream для записи в поток
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
