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



class Cache { //объявляем класс Cache
  struct ExperimentData //структура, в которой хранятся данные после отработки экспериментов
  {
    std::string travelOrder; //порядок обхода
    std::vector<double> experimentTime;
    ExperimentData(std::string order,const std::vector<double>& time) //конструктор, принимающий два значения. Константная ссылка
        : travelOrder(std::move(order)), experimentTime(time) {} //move() —  функция, конвертирует передаваемый аргумент в r-value.
  };//l-value — значение, которое имеет свой собственный адрес в памяти.
  //r-value — значение, которое не имеет постоянного адреса в памяти.
 public:
  void CacheSizes(); //генирирует вектор
  void Warming(int * arrayToWarm, size_t size);
  void StraightExperiment();
  void BackExperiment();
  void RandomExperiment();
  int* GenerateArray(size_t bufferSize); //генирирует наш массив, заполняет числами

  friend std::ostream& operator<<(std::ostream& os, const Cache& experiments); //friend позволяет объявить operator как глобальную функцию
 private:                             //ostream для записи в поток
  std::vector<double> sizes;
  std::vector<ExperimentData> data; //вектор вложенных структур, вектор, который будем выводить
  const double min = 0.256;         //структура, в которой хранятся данные после отработки экспериментов
  const double max = 8;
  const int step = 16;
  double n = 1;
  const int quantity = 1000;
};  // 0.128 MB << 1 MB << 2 MB << 4 MB << 8 MB << 12 MB

#endif // INCLUDE_HEADER_HPP_
