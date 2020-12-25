// Copyright 2020 ivan <ikhonyak@gmail.com>
#include <cache.hpp>
#include <gtest/gtest.h>

TEST(Example, EmptyTest) {
  Cache experiments; //берем наш класс и пользуемся экспериментами
  experiments.CacheSizes();//генирируем размеры в зависимости от размеров кэша
  experiments.StraightExperiment(); //вызываем наши функции
  experiments.BackExperiment();
  experiments.RandomExperiment();
  std::cout << experiments; //вызов оператора
}
