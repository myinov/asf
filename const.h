//----------------------------------------------------------------------------------------
//const.h
//Объявление констант
//Initial date: May,10,2005. Final date:
//Copyright (c) JohnSoft 2005. All rights reserved. C++ Builder 6
//----------------------------------------------------------------------------------------

#ifndef CONST_H
#define CONST_H

#include <string>
#include <vector>
#include <list>
#include <deque>
#include <set>
#include <map>
#include <algorithm>
#include <iostream>
#include <strstream>

#include <vcl.h>
#include <SysUtils.hpp>
#include <Graphics.hpp>
#include <typeinfo.h>

#include <iostream.h>
#include <iomanip.h>  // Required for parameterized manipulators.

#include "complex.h"   //THIS ALREADY INCLUDES iostream.h

#include <except.h>
#include <strstrea.h>
#include <fstream.h>

#include <conio.h>
#include "stddef.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <float.h>

using namespace std;
//----------------------------------------------------------------------------------------
//Объявление констант
//Перевод из градусов в радианы
#define DegRad  0.0174532925199432957692369076848861
//Перевод из радиан в градусы
#define RadDeg  57.2957795130823208767981548141052

#define TwoPI   6.28318530717958647692528676655901  //TWO_PI
#define OnePI   3.1415926535897932384626433832795   //PI
#define HalfPI  1.57079632679489661923132169163975  //PI/2
#define SqrtTwo 1.4142135623730950488016887242097   //Square Root of TWO

#define DEG     true//Градусная мера угла
#define RAD     false//Радианная мера угла

#define ON      true//Включено / используется
#define OFF     false//Выключено / не используется

//Комплексный нуль
#define C_ZERO  (complex(0,0))
//Комплексная единица
#define C_ONE   (complex(1,0))

//Пустая строка
#define STR_NULL (string(""))

#define LEFT  false
#define RIGHT true

//Переопределение типов
typedef unsigned long ulong;
typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned __int64 uint64;

typedef std::set<double> DblSet;
typedef std::set<long> LongSet;
typedef std::set<string> StringSet;
typedef std::map<ulong, ulong> ULongMap;
typedef std::map<ulong, string> UStringMap;

//Определение монотонно-возрастающей и монотонно-убывающей функции на интервале
typedef enum {IncFunc = 1, DecFunc = 2} enIncDecFunc;

//Определение типа процесса: детерминированный или случайный
typedef enum {enDetProcess = 1, enRandProcess = 2} enProcess;

//****************************************************************************************
//Идентификаторы типов схемы исключения приема сигналов с заданных направлений
//в дополнительных каналах пространственного адаптивного фильтра
//****************************************************************************************
#define MultiStageUnit  1 //Многоступенчатая схема
#define SingleStageUnit 2 //Одноступенчатая схема

class TComplexVector;
typedef TComplexVector cvector;

class TRealVector;
typedef TRealVector rvector;

class TIntVector;
typedef TIntVector ivector;

#endif //Конец объявления заголовочного файла const.h
