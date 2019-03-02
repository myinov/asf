//mathfunc.h
//Объявление математических функций, описывающих законы изменения во времени
//амплитудных и фазовых составляющих сигналов.
//Copyright (c) JohnSoft 1998. All rights reserved.
//Initial date: August,22,1998. Final date:
//Заголовочный файл mathfunc.h объявляет некоторые математические функции, кото-
//рые позволяют описывать законы изменения амплитудных и фазовых составляющих
//сигналов во времени. Все функции возвращают значения типа double, которое
//можно трактовать как значение фазы или амплитуды сигнала в мгновенный момент
//времени t. Все функции в качестве аргументов принимают массив параметров типа
//double. Для всех функций в первом элементе массива передается аргумент функции
//F(t) - t (время). Количество передаваемых параметров (размерность массива) и их
//интерпретация зависят от типа функции, которая описывает величину изменения
//фазы или амплитуды сигнала во времени, и могут изменяться в очень широких пре-
//делах.


#if !defined(__MATHFUNC_H)
#define __MATHFUNC_H

#include <except.h>   //Заголовочный файл по обработке исключительных ситуаций

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
//#include <complex.h>
#include "matrix.h"  //Заголовочеый файл работы с матрицами
#include "mvector.h" //Заголовочеый файл работы с векторами
#include "mathlib.h" //Заголовочный файлс с генератором случайных чисел
#include "const.h"

//#include <cstring.h>
#include <string>
#include <strstrea.h>

#include <iomanip.h>
#include <iostream.h>
#include <fstream.h>

#define CONSOLE 1     //В программе используется консольный ввод-вывод

#if defined(CONSOLE)
 #include <conio.h>
#endif

typedef unsigned int  uint;
typedef unsigned long ulong;

typedef double (*DDFunc)(double);
typedef bool (*BDVAFunc)(double,...);

//const int RUS = 1; //русский язык
//const int ENG = 2; //английский язык

const bool SHORTNAME = true;  //сокращенная форма записи
const bool LONGNAME  = false; //полная форма записи

const bool DETLAW    = true;  //детерминированный закон
const bool CASUALLAW = false; //вероятностный закон

const bool TDIM = true;  //заданная размерность параметра закона
const bool SDIM = false; //исходная размерность параметра закона

//const bool ON  = true;
//const bool OFF = false;

//**************************************************************************
//ОБЪЯВЛЕНИЕ СТРУКТУРЫ LAWNAME
//Структура содержит наименование закона на русском и английском языках,
//математическую форму записи закона.
typedef struct
{
  uint type;      //идентификатор закона
  bool detlaw;    //флаг определяющий детерминированный или вероятностный закон
  char *rus_name; //русскоязычное наименование закона
  char *eng_name; //англоязычное наименование закона
  char *math_notation; //математическая форма записи закона
  char *abbreviate; //сокращенное наименование закона на английском языке
} LAWNAME;

//**************************************************************************
//ОБЪЯВЛЕНИЕ СТРУКТУРЫ FACTOR
//Структура FACTOR характеризует размерность аргумента и параметров функции,
//их физические наименования, правила перевода вводимых аргументов функции
//при необходимости для последующей работы с ними, корректность ввода
//**************************************************************************
typedef struct
{
  uint SourceDim; //Размерность вводимой величины параметра (аргумента) функции
  uint TargetDim; //Размерность хранимой величины параметра (аргумента) функции
  char* Name;     //Наименование параметра (аргумента)
  //Функция перевода из SourceDim в TargetDim
  double (*Convert)(double value);
  //Функция перевода из TargetDim в SourceDim
  double (*Invert)(double value);
  //Функция предусловия (возможность входа в редактирование параметра)
  bool (*PreBlock)(double value,...);
  //Функция постусловия (возможность выхода из редактирования параметра)
  bool (*PostBlock)(double value,...);
} FACTOR;

//**************************************************************************
//ОБЪЯВЛЕНИЕ СТРУКТУРЫ MEASURE
//Структура MEASURE характеризует размерности функции, ее аргумента и
//вводимых параметров (коэффициентов) функции (DIMENSIONALITY)
//**********************************************************************
typedef struct
{
 uint DimFunc; //Размерность функции
 uint Size; //Количество параметров с аргументом соответствующих данной ф-ции
 FACTOR* Factor; //Массив характеристик на параметры и аргумент функции
} MEASURE;

//*****************************************************************************
//ОБЪЯВЛЕНИЕ СТРУКТУРЫ MEASURENAME
//Структура содержит полные и сокращенные наименования размерности величины на
//русском и английском языках
//*****************************************************************************
typedef struct
{
  uint  Type;    //Тип физической величины
  char* Rus;     //Полное наименование на русском языке
  char* RusShort;//Сокращенное наименование на русском языке
  char* Eng;     //Полное наименование на английском языке
  char* EngShort;//Сокращенное наименование на английском языке
} MEASURENAME;

//Объявление некоторых идентификаторов размерностей физических величин
const uint NONDIM   = 0; //Безразмерная величина
const uint SECOND   = 1; //Секунда
const uint RADIAN   = 2; //Радианная мера
const uint DEGREE   = 3; //Градусная мера
const uint VOLT     = 4; //Напряжение в вольтах
const uint HZ       = 5; //Частота в герцах
const uint PULSAT   = 6; //Угловая частота
const uint DEG_SEC  = 7; //Скорость в град/сек
const uint DEG_SEC2 = 8; //Ускорение в град/сек^2
const uint RAD_SEC  = 9; //Скорость в рад/сек
const uint RAD_SEC2 = 10;//Ускорение в рад/сек^2

//****************************************************************************
// ОБЪЯВЛЕНИЕ СТРУКТУРЫ ArgList - СПИСОК ПАРАМЕТРОВ ФУНКЦИИ ПЕРЕМЕННОЙ ДЛИНЫ
// Структура служит для представления переменного количества аргументов
// типа double
//****************************************************************************
typedef struct
{
 ulong Size; //Количество параметров в списке
 double *List; //Список параметров
} ARGLIST;

//Определение указателя на список аргументов
typedef ARGLIST* PARGLIST;

//****************************************************************************
// ОБЪЯВЛЕНИЕ СТРУКТУРЫ V_ArgList - ВЕКТОР СПИСКА ПАРАМЕТРОВ ФУНКЦИИ ПЕРЕМЕННОЙ
// ДЛИНЫ
// Структура служит для представления вектора (одномерного массива) списка, в ко-
// тором хранятся значения типа double. Такая структура может хранить аргументы
// нескольких функций с различным количеством аргументов. Вектор реализован как
// массив указателей на структуру ARGLIST - ARGLIST**, что позволяет использовать
// функции для работы со структурой ARGLIST.
//****************************************************************************
typedef struct
{
 ulong Size; //Длина вектора
 ARGLIST** Vector; //Вектор, содержащий список структур ARGLIST
} V_ARGLIST;

//Определение указателя на вектор списка структур ARGLIST
typedef V_ARGLIST* pV_ARGLIST;


//*************************************************************************
//Объявление структуры закона изменения составляющих амплитуды или
//фазы компоненты внешнего сигнала
//*************************************************************************
typedef struct
{
  bool turn; //доступность закона для его выбора пользователем
  double (*law)(const ARGLIST *Arg);
  uint type; //идентификатор закона
  ARGLIST* (*inputfactor)(const MEASURE& M);
  //математическая форма записи закона c реальными коэффициентами
  char* (*mathlawfactor)(const ARGLIST* Arg);
  MEASURE M; //Характеристики параметров и аргументов функции
} LAW;

//*****************************************************************************
// Объявление некоторых констант, необходимых для работы функций
//*****************************************************************************
//Объявление константы TWO_PI = 2 * PI
#define TWO_PI 6.283185307178;
//Объявление константы DEG_RAD = PI / 180 для перевода из градусной в радианную
//меру угла
#define DEG_RAD 0.01745329251994;
//Объявление константы RAD_DEG = 180 / PI для перевода из радианной в градусную
//меру угла
#define RAD_DEG 57.2957795131;
//Объявление константы EPSILON - смещение относительно рассчитываемой точки.
//Используется в том случае, когда функция в заданной точке не определена
#define EPSILON 0.0000001

// СПИСОК ВОЗМОЖНЫХ ОШИБОК
const int ARG_MISMATCH = -1;//Несоответствие количества переданных параметров
const int EMPTY_TABLE  = -2;//Таблица не содержит ни одной записи
const int ABSENT_LAW   = -3;//Закон в таблице законов отсутствует
const int IMPOS_VALUE  = -4;//Недопустимое значение переменной

// ОБЪЯВЛЕНИЕ КОНСТАНТ, СООТНЕСЕННЫХ С ТИПАМИ ФУНКЦИЙ
//Детерминированные функции
const uint UNEXIST      =  0;//Не зарегистрированный закон
const uint ZERO_LAW     =  1;//Нулевой во времени закон
const uint CONST_LAW    =  2;//Постоянный во времени закон
const uint LINEAR_LAW   =  3;//Линейный во времени закон
const uint SQUARE_LAW   =  4;//Квадратичный во времени закон
const uint EXP_LAW      =  5;//Экспоненциальный во времени закон
const uint COS_LAW      =  6;//Косинусоидальный во времени закон
const uint SIN_LAW      =  7;//Cинусоидальный во времени закон

const uint HYPERBOL_LAW =  8;//Гиперболический закон
const uint LN_LAW       =  9;//Закон натурального логарифма
const uint LOG10_LAW    = 10;//Закон десятичного логарифма
const uint LOGA_LAW     = 11;//Закон логарифма по основанию A
const uint SQRT_LAW     = 12;//Закон квадратного корня из линейного двучлена
//Случайные функции
const uint RAND_LAW     = 13;//Равномерное распределение [0..1]
const uint RAND2PI_LAW  = 14;//Равномерное распределение [0..2PI]
const uint RANDAB_LAW   = 15;//Равномерное распределение [A..B]
const uint RELAY_LAW    = 16;//Распределение по Релею
//Функции исключающие случайную компоненту
const uint DET1_LAW  = 17; //Детерминированный закон амплитуды
const uint DET0_LAW  = 18; //Детерминированный закон фазы
//Табличная функция
const uint TABLE_LAW = 19; //Табличнозаданная функция
//Прямоугольный импульс
const uint SQUAREPULSE_LAW = 20; //Функция прямоугольного импульса
//*****************************************************************************
// ОБЪЯВЛЕНИЕ ФУНКЦИЙ, ОПИСЫВАЮЩИХ НЕКОТОРЫЕ ЗАКОНЫ ИЗМЕНЕНИЯ ВЕЛИЧИН
//*****************************************************************************
//I. ДЕТЕРМИНИРОВАННЫЕ ФУНКЦИИ
//Нулевая во времени функция  F(t) = 0;
double ZeroLaw(const ARGLIST* Arg); //1. ZERO_LAW

//Постоянный закон во времени. Функция вида F(t) = const;
double ConstLaw(const ARGLIST* Arg); //2. CONST_LAW

//Линейный закон во времени. Функция вида F(t) = At + B;
double LinearLaw(const ARGLIST* Arg); //3. LINEAR_LAW

//Квадратичный закон во времени. Функция вида F(t) = A^2*t + B*t + C
double SquareLaw(const ARGLIST* Arg); //4. SQUARE_LAW

//Экспоненциальный закон во времени. Функция F(t) = A*EXP(B*t)
double ExpLaw(const ARGLIST* Arg);//5. EXP_LAW

//Косинусоидальный закон во времени. Функция F(t) = A*cos(B*t+C);
double CosLaw(const ARGLIST* Arg); //6. COS_LAW

//Синусоидальный закон во времени. Функция F(t) = A*sin(B*t+C);
double SinLaw(const ARGLIST* Arg); //7. SIN_LAW

//Гиперболический закон, обратная пропорциональность. Функция F(t) = A/t;
double HyperbolLaw(const ARGLIST* Arg); //8. HYPERBOL_LAW

//Закон натурального логарифма. Функция F(t) = LN(t);
double LnLaw(const ARGLIST* Arg); //9. LN_LAW

//Закон десятичного логарифма. Функция F(t) = LOG10(t);
double Log10Law(const ARGLIST* Arg); //10. LOG10_LAW

//Закон логарифма по основанию A. Функция F(t) = LOGA(t);
double LogALaw(const ARGLIST* Arg); //11. LOGA_LAW

//Закон квадратного корня из линейного двучлена. Функция F(t) = SQRT(A*t+B);
double SqrtLaw(const ARGLIST* Arg); //12. SQRT_LAW

//Закон изменения прямоугольного импульса во времени.
double SquarePulseLaw(const ARGLIST* Arg); //20. SQUAREPULSE_LAW
//и т.д. ...

//II. СЛУЧАЙНЫЕ ЗАКОНЫ
//Функция равновероятного изменения величины в интервале от 0 до 1.
double RandLaw(const ARGLIST* Arg);    //13. RAND_LAW

//Равновероятное изменение амплитудной или фазовой составляющей во времени в
//диапазоне от 0 до 2*PI
double Rand2PiLaw(const ARGLIST* Arg); //14. RAND2PI_LAW

//Функция равновероятного изменения величины в интервале от a до b.
//a - нижняя граница интервала; b - верхняя граница интервала
double RandABLaw(const ARGLIST* Arg); //15. RANDAB_LAW

//Функция изменения величины по случайному закону Релея
//sqrt(-2*ln(RandLaw())
double RelayLaw(const ARGLIST* Arg);  //16. RELAY_LAW

//Функции исключающие случайную компоненту

//Применяется при перемножении величин, среди которых может быть случайная ком-
//понента и ее необходимо исключить из рассмотрения
double Det1Law(const ARGLIST* Arg);   //17. DET1_LAW

//Применяется при сложении величин, среди которых может быть случайная ком-
//понента и ее необходимо исключить из рассмотрения
double Det0Law(const ARGLIST* Arg);   //18. DET0_LAW

//***************************************************************************
// ОБЪЯВЛЕНИЕ ФУНКЦИЙ ПОДГОТАВЛИВАЮЩИХ СПИСОК НЕОБХОДИМЫХ ПАРАМЕТРОВ ДЛЯ
// РАБОТЫ ФУНЦИЙ, ОПИСЫВАЮЩИХ ЗАКОНЫ ИЗМЕНЕНИЯ ВЕЛИЧИН
// Все функции в результате работы возвращают указатель на структуру ARGLIST со
// списком коэффициентов.
//***************************************************************************
ARGLIST* CreateArgList(const ulong size);
ARGLIST* DeleteArgList(ARGLIST *Arg);
ARGLIST* CopyArgList(const ARGLIST *Source, ARGLIST *Target);

V_ARGLIST* CreateVectorArgList(const ulong VectorSize, const ulong ListSize = 0);
V_ARGLIST* DeleteVectorArgList(V_ARGLIST *Vector);

//Консольный ввод всех параметров функции типа double в режиме инициализации
//(начального ввода) всех параметров функции
ARGLIST* InitFactors(const MEASURE& M);
//Ввод одного параметра типа double
double GetDoubleValue(double value, const FACTOR& F);

//Объявление функций пред- и постпроверки вводимых (редактируемых) величин
//типа double
bool AlwaysTrue(double value,...);
bool AlwaysFalse(double value,...);

//Объявление функций преобразования данных из одной размерности в другую
//типа double
double Copy(double value);   //Без изменений
double DegToRad(double value); //Из градусов в радианы
double RadToDeg(double value); //Из радиан в градусы
double FreqToPulsatance(double value); //Из линейной в угловую частоту
double PulsatanceToFreq(double value); //Из угловой в линейную частоту


//******************************************************************************
// ОБЪЯВЛЕНИЕ ФУНКЦИЙ, ФОРМИРУЮЩИХ МАТЕМАТИЧЕСКУЮ ЗАПИСЬ ЗАКОНА ИЗМЕНЕНИЯ
// ВЕЛИЧИНЫ ОТ ВРЕМЕНИ С КОНКРЕТНЫМИ ПАРАМЕТРАМИ
// Все функции в результате работы возвращают указатель на строку с математичес-
// кой записью закона изменения величины во времени с реальными коэффициентами.
// Функции принимают в качестве аргумента список параметров закона изменения
// данной величины.
//******************************************************************************
//1. Детерминированные функции
char* GetMathNotateZeroLaw(const ARGLIST* Arg);     //1.  for ZERO_LAW
char* GetMathNotateConstLaw(const ARGLIST* Arg);    //2.  for CONST_LAW
char* GetMathNotateLinearLaw(const ARGLIST* Arg);   //3.  for LINEAR_LAW
char* GetMathNotateSquareLaw(const ARGLIST* Arg);   //4.  for SQUARE_LAW
char* GetMathNotateExpLaw(const ARGLIST* Arg);      //5.  for EXP_LAW
char* GetMathNotateCosLaw(const ARGLIST* Arg);      //6.  for COS_LAW
char* GetMathNotateSinLaw(const ARGLIST* Arg);      //7.  for SIN_LAW

char* GetMathNotateHyperbolLaw(const ARGLIST* Arg); //8.  for HYPERBOL_LAW
char* GetMathNotateLnLaw(const ARGLIST* Arg);       //9.  for LN_LAW
char* GetMathNotateLog10Law(const ARGLIST* Arg);    //10. for LOG10_LAW
char* GetMathNotateLogALaw(const ARGLIST* Arg);     //11. for LOGA_LAW
char* GetMathNotateSqrtLaw(const ARGLIST* Arg);     //12. for SQRT_LAW

//2. Случайные функции
char* GetMathNotateRandLaw(const ARGLIST *Arg);     //13. RAND_LAW
char* GetMathNotateRand2PiLaw(const ARGLIST *Arg);  //14. RAND2PI_LAW
char* GetMathNotateRandABLaw(const ARGLIST *Arg);   //15. RANDAB_LAW
char* GetMathNotateRelayLaw(const ARGLIST *Arg);    //16. RELAY_LAW
char* GetMathNotateDet1Law(const ARGLIST *Arg);     //17. DET1_LAW
char* GetMathNotateDet0Law(const ARGLIST *Arg);     //18. DET0_LAW
//3. Табличный закон
char* GetMathNotateTableLaw(const ARGLIST *Arg);    //19. TABLIC_LAW

//Импульсные функции
char* GetMathNotateSquarePulseLaw(const ARGLIST *Arg); //20. SQUAREPULSE_LAW

//******************************************************************
// ОБЪЯВЛЕНИЯ ФУНКЦИЙ, РАБОТАЮЩИХ С МАССИВАМИ LawName и MeasureName
//******************************************************************
const char* GetLawName(uint type, uint language = ENG);
const char* GetLawNotate(uint type);
const char* GetMeasureName(uint type, uint language = ENG,
                           bool format = SHORTNAME);
uint GetTypeLawByAbbrLaw(string& abbr, bool detlaw);
string& GetAbbrLaw(string& s, bool detlaw);
const char* GetAbbrLawByType(uint type);

#endif //ЗАВЕРШЕНИЕ ОБЪЯВЛЕНИЯ ЗАГОЛОВОЧНОГО ФАЙЛА "MATHFUNC.H"
