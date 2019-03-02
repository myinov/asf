//mathfunc.cpp
//Реализация законов изменения составляющих амплитуды и фазы
//сигнала во времени
//Initial date: August,22,1998. Final date:
//Copyright (c) JohnSoft 1998. All rights reserved

#include "mathfunc.h"
const double DegRad = 0.01745329251994;//DEG_RAD
const double RadDeg = 57.2957795131;//RAD_DEG
const double TwoPI = 6.283185307178;//TWO_PI
//Массив наименований законов и математических форм его записи
const LAWNAME LawName[] = {
 {UNEXIST,DETLAW,"Имя не найдено","Name not found","Math. notate not found",
  "UNEXIST_LAW"},                                                            //0
 //Детерминированные законы
 {ZERO_LAW,DETLAW,"Нулевой закон","Zero law","F(t) = 0","ZERO_LAW"},        //1
 {CONST_LAW,DETLAW,"Постоянный закон","Constant law","F(t) = A",
  "CONST_LAW"},                                                             //2
 {LINEAR_LAW,DETLAW,"Линейный закон","Linear law","F(t) = A*t + B",
  "LINEAR_LAW"},                                                            //3
 {SQUARE_LAW,DETLAW,"Квадратичный закон","Square law","F(t) = A*t^2+B*t+C",
  "SQUARE_LAW"},                                                            //4
 {EXP_LAW,DETLAW,"Экспоненциальный закон","Exponential law","F(t) = A*EXP[B*t]",
  "EXP_LAW"},                                                               //5
 {COS_LAW,DETLAW,"Косинусоидальный закон","Cosine law","F(t) = A*COS(B*t+C)",
  "COS_LAW"},                                                               //6
 {SIN_LAW,DETLAW,"Синусоидальный закон","Sine law","F(t) = A*SIN(B*t+C)",
  "SIN_LAW"},                                                               //7
 {HYPERBOL_LAW,DETLAW,"Гиперболический закон","Hyperbolic law","F(t) = A/t",
  "HYPERBOL_LAW"},                                                          //8
 {LN_LAW,DETLAW,"Закон LN(x) ","Natural logarithm law","F(t) = LN(t)",
  "LN_LAW"},                                                                //9
 {LOG10_LAW,DETLAW,"Закон LOG10(x) ","Decimal logarithm law","F(t) = LOG10(t)",
  "LOG10_LAW"},                                                             //10
 {LOGA_LAW,DETLAW,"Закон LOGa(x)","LOGa(x) law","F(t) = LOGA(t)",
  "LOGA_LAW"},                                                              //11
 {SQRT_LAW,DETLAW,"Закон квадратного корня","Square root law","F(t) = Sqrt(A*t+B)",
  "SQRT_LAW"},                                                              //12
 //Случайные законы
 {RAND_LAW,CASUALLAW,"Равновероятный закон [0..1]","RANDOM LAW","[0..1]",
  "RAND_LAW"},                                                              //13
 {RAND2PI_LAW,CASUALLAW,"Равновероятный закон [0..2PI]","RANDOM LAW",
  "[0..2PI]","RAND2PI_LAW"},                                                //14
 {RANDAB_LAW,CASUALLAW,"Равновероятный закон [A..B]","RANDOM LAW ","[A..B]",
  "RANDAB_LAW"},                                                            //15
 {RELAY_LAW,CASUALLAW,"Случайный закон РЕЛЕЯ","RELAY LAW","[0..+INFINITE]",
  "RELAY_LAW"},                                                             //16
 //Для амплитуды - в случае умножения величин
 {DET1_LAW,CASUALLAW,"Неслучайный закон","UNCASUAL LAW"," 1 ","DET1_LAW"},  //17
 //Для фазы - в случае сложения величин
 {DET0_LAW,CASUALLAW,"Неслучайный закон","UNCASUAL LAW"," 0 ","DET0_LAW"},  //18
 //Табличный закон
 {TABLE_LAW,DETLAW,"Табличный закон","TABLE LAW","  ","TABLE_LAW"},         //19
 //Закон прямоугольного импульса
 {SQUAREPULSE_LAW,DETLAW,"Прямоугольный импульс","SQUARE PULSE","  ",
  "SQUAREPULSE_LAW"}                                                        //20
};

//Массив наименований физических величин
const  MEASURENAME MeasureName[] = {
{NONDIM,"БЕЗРАЗМЕРНАЯ","","NON-DIMENSIONAL",""}, //0
{SECOND,"СЕКУНДА","СЕК","SECOND","SEC"},         //1
{RADIAN,"РАДИАН","РАД","RADIAN","RAD"},          //2
{DEGREE,"ГРАДУС","ГРАД","DEGREE","DEG"},         //3
{VOLT,  "ВОЛЬТ","В","VOLT","V"},                 //4
{HZ,"ЧАСТОТА","ГЦ","HERTZ","HZ"},                //5
{PULSAT,"УГЛОВАЯ ЧАСТОТА","РАД/С","PULSATANCE","RAD/SEC"},                //6
{DEG_SEC,"УГЛОВАЯ СКОРОСТЬ","ГРАД/С","ANGULAR VELOCITY","DEG/SEC"},       //7
{DEG_SEC2,"УГЛОВОЕ УСКОРЕНИЕ","ГРАД/С^2","ANG. ACCELERATION","DEG/SEC^2"},//8
{RAD_SEC,"УГЛОВАЯ СКОРОСТЬ","РАД/С","ANGULAR VELOCITY","RAD/SEC"},       //9
{RAD_SEC2,"УГЛОВОЕ УСКОРЕНИЕ","РАД/С^2","ANG. ACCELERATION","RAD/SEC^2"} //10
};

//Инициализация генератора случайных чисел
TRandomGenerator RNG(T_16,0);

//*****************************************************************************
//СТАТУС:
//НАИМЕНОВАНИЕ ФУНКЦИИ:
//НАЗНАЧЕНИЕ:
//Функция предназначена для ...
//
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. -
//2. -
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
//*****************************************************************************

//*************************************
//I. ДЕТЕРМИНИРОВАННЫЕ ФУНКЦИИ
//************************************
//*****************************************************************************
//СТАТУС: I.1
//НАИМЕНОВАНИЕ ФУНКЦИИ: double ZeroLaw(const ARGLIST* Arg)
//НАЗНАЧЕНИЕ: F(t) = 0 - нулевая функция от времени
//Функция моделирует закон изменения величины от времени при котором данная
//величина постоянно равна нулю. Работа функции не зависит от количества передан
//ных параметров в списке Arg.
//СТРУКТУРА СПИСКА Arg ДЛЯ НОРМАЛЬНОЙ РАБОТЫ ZEROLAW()
//Количество параметров: 1
//1. Arg.List[0] - текущее (модельное) время t (аргумент функции F(t) - t)
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. struct ARGLIST* Arg - список параметров закона изменения ZeroLaw
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: double
// Всегда возвращает ноль.
//*****************************************************************************
double ZeroLaw(const ARGLIST* Arg)
{
 try
  {
   if (Arg == NULL) //Отсутствуют параметры
    throw ARG_MISMATCH;
   if (Arg->Size != 1) //Для ZeroLaw список Arg должен иметь один аргумент
    throw ARG_MISMATCH;
  }
 catch (int error_type)
  {
   if (error_type == ARG_MISMATCH)
   {
    #if defined(CONSOLE)
      cout << "\nMISMATCH ARGUMENT ERROR !!!" << endl;
      cout << "One argument should be passed even to function ZeroLaw().\n";
      cout << "Mathematical notation: F(t) = 0, t - time." << endl;
      getch();
    #endif
    exit(-1);
   }
  }
return 0.0;
}
//*****************************************************************************
//СТАТУС: I.2
//НАИМЕНОВАНИЕ ФУНКЦИИ: double ConstLaw(const ARGLIST* Arg)
//НАЗНАЧЕНИЕ: F(t) = const - постоянная функция от времени
//Функция моделирует закон изменения величины от времени при котором данная
//величина постоянна во времени. Для работы функции неообходимо ей передать два
//параметра через список Arg. При этом используется только второй аргумент -
//Arg.List[1].
//СТРУКТУРА СПИСКА Arg ДЛЯ НОРМАЛЬНОЙ РАБОТЫ CONSTLAW()
//Количество параметров: 2
//1. Arg.List[0] - текущее (модельное) время t (аргумент функции F(t) - t)
//2. Arg.List[1] - const (величина типа double). F(t) = 5.0 => arg[1] = 5.0
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. struct ARGLIST* Arg - список параметров закона изменения ConstLaw
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: double Arg.List[1]
//*****************************************************************************
double ConstLaw(const ARGLIST* Arg)
{
 try
  {
   if (Arg == NULL) //Отсутствуют параметры
    throw ARG_MISMATCH;
   if (Arg->Size != 2) //Должно быть передано точно два параметра
    throw ARG_MISMATCH;
  }
 catch (int error_type)
  {
   if (error_type == ARG_MISMATCH)
   {
    #if defined(CONSOLE)
      cout << "\nMISMATCH ARGUMENT ERROR !!!" << endl;
      cout << "Function ConstLaw() should accept two parameters: t & const.\n";
      cout << "Mathematical notation: F(t) = const, t - time." << endl;
      getch();
    #endif
    exit(-1);
   }
  }
return Arg->List[1];
}
//*****************************************************************************
//СТАТУС: I.3
//НАИМЕНОВАНИЕ ФУНКЦИИ: double LinearLaw(const ARGLIST* Arg)
//НАЗНАЧЕНИЕ: F(t) = A*t+B - линейная функция от времени
//Функция описывает линейный закон изменения величины во времени. Для работы
// функции неообходимо ей передать три параметра через список Arg.
//СТРУКТУРА СПИСКА Arg ДЛЯ НОРМАЛЬНОЙ РАБОТЫ LINEARLAW()
//Количество параметров: 3
//1. Arg.List[0] - t - текущее время (модельное время)
//2. Arg.List[1] - A
//3. Arg.List[2] - B
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. struct ARGLIST* Arg - список параметров закона изменения LinearLaw
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: double value = A*t+B
//*****************************************************************************
double LinearLaw(const ARGLIST* Arg)
{
 try
  {
   if (Arg == NULL) //Отсутствуют параметры
    throw ARG_MISMATCH;
   if (Arg->Size != 3) //Должно быть передано точно три параметра
    throw ARG_MISMATCH;
  }
 catch (int error_type)
  {
   if (error_type == ARG_MISMATCH)
   {
    #if defined(CONSOLE)
      cout << "\nMISMATCH ARGUMENT ERROR !!!" << endl;
      cout << "Function LinearLaw() should accept three parameters: t,A,B.\n";
      cout << "Mathematical notation: F(t) = A*t + B." << endl;
      getch();
    #endif
    exit(-1);
   }
  }
return Arg->List[1]*Arg->List[0]+Arg->List[2]; //F(t) = A*t + B
}
//*****************************************************************************
//СТАТУС: I.4
//НАИМЕНОВАНИЕ ФУНКЦИИ: double SquareLaw(const ARGLIST* Arg)
//НАЗНАЧЕНИЕ: F(t) = A*t^2+B*t+C - квадратичная функция от времени
//Функция описывает квадратичный закон изменения величины во времени. Для работы
// функции неообходимо ей передать 4 параметра через структуру Arg.
//СТРУКТУРА СПИСКА Arg ДЛЯ НОРМАЛЬНОЙ РАБОТЫ SQUARELAW()
//Количество параметров: 4
//1. Arg.List[0] - double t - текущее время (модельное время)
//2. Arg.List[1] - double A
//3. Arg.List[2] - double B
//4. Arg.List[3] - double C
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. struct ARGLIST* Arg - список параметров закона изменения SquareLaw
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: double value = A*t^2+B*t+C
//*****************************************************************************
double SquareLaw(const ARGLIST* Arg)
{
 double t;
 try
  {
   if (Arg == NULL) //Отсутствуют параметры
    throw ARG_MISMATCH;
   if (Arg->Size != 4) //Должно быть передано точно 4 параметра
    throw ARG_MISMATCH;
   t = Arg->List[0];
  }
 catch (int error_type)
  {
   if (error_type == ARG_MISMATCH)
   {
    #if defined(CONSOLE)
      cout << "\nMISMATCH ARGUMENT ERROR !!!" << endl;
      cout << "Function SquareLaw() should accept four parameters: t,A,B,C.\n";
      cout << "Mathematical notation: F(t) = A*t^2 + B*t + C." << endl;
      getch();
    #endif
    exit(-1);
   }
  }
return Arg->List[1]*t*t+Arg->List[2]*t+Arg->List[3]; //F(t) = A*t^2 + B*t + C
}
//*****************************************************************************
//СТАТУС: I.5
//НАИМЕНОВАНИЕ ФУНКЦИИ: double ExpLaw(const ARGLIST* Arg)
//НАЗНАЧЕНИЕ: F(t) = A*EXP(B*t)
//Функция описывает экспоненциальный закон изменения величины во времени. Для
//работы функции неообходимо ей передать 3 параметра через структуру Arg.
//СТРУКТУРА СПИСКА Arg ДЛЯ НОРМАЛЬНОЙ РАБОТЫ EXPLAW()
//Количество параметров: 3
//1. Arg.List[0] - double t - текущее время (модельное время)
//2. Arg.List[1] - double A
//3. Arg.List[2] - double B
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. struct ARGLIST* Arg - список параметров закона изменения ExpLaw
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: double value = A*EXP(B*t)
//*****************************************************************************
double ExpLaw(const ARGLIST* Arg)
{
 double t;
 try
  {
   if (Arg == NULL) //Отсутствуют параметры
    throw ARG_MISMATCH;
   if (Arg->Size != 3) //Должно быть передано точно 3 параметра
    throw ARG_MISMATCH;
   t = Arg->List[0];
  }
 catch (int error_type)
  {
   if (error_type == ARG_MISMATCH)
   {
    #if defined(CONSOLE)
      cout << "\nMISMATCH ARGUMENT ERROR !!!" << endl;
      cout << "Function ExpLaw() should accept three parameters: t,A,B.\n";
      cout << "Mathematical notation: F(t) = A*EXP(B*t)." << endl;
      getch();
    #endif
    exit(-1);
   }
  }
return Arg->List[1]*exp(Arg->List[2]*t); //F(t) = A*EXP(B*t)
}
//*****************************************************************************
//СТАТУС: I.6
//НАИМЕНОВАНИЕ ФУНКЦИИ: double CosLaw(const ARGLIST* Arg)
//НАЗНАЧЕНИЕ: F(t) = A*COS(B*t+C) - косинусоидальная функция от времени
//Функция описывает косинусоидальный закон изменения величины во времени. Для
//работы функции неообходимо ей передать 4 параметра через структуру Arg.
//СТРУКТУРА СПИСКА Arg ДЛЯ НОРМАЛЬНОЙ РАБОТЫ COSLAW()
//Количество параметров: 4
//1. Arg.List[0] - double t - текущее время (модельное время)
//2. Arg.List[1] - double A - амплитуда
//3. Arg.List[2] - double B - угловая частота B = 2*PI*f
//4. Arg.List[3] - double C - начальная фаза
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. struct ARGLIST* Arg - список параметров закона изменения CosLaw
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: double value = A*COS(B*t+C)
//*****************************************************************************
double CosLaw(const ARGLIST* Arg)
{
 double t;
 try
  {
   if (Arg == NULL) //Отсутствуют параметры
    throw ARG_MISMATCH;
   if (Arg->Size != 4) //Должно быть передано точно 4 параметра
    throw ARG_MISMATCH;
   t = Arg->List[0];
  }
 catch (int error_type)
  {
   if (error_type == ARG_MISMATCH)
   {
    #if defined(CONSOLE)
      cout << "\nMISMATCH ARGUMENT ERROR !!!" << endl;
      cout << "Function CosLaw() should accept four parameters: t,A,B,C.\n";
      cout << "Mathematical notation: F(t) = A*COS(B*t + C)." << endl;
      getch();
    #endif
    exit(-1);
   }
  }
return Arg->List[1]*cos(Arg->List[2]*t+Arg->List[3]); //F(t) = A*COS(B*t+C)
}
//*****************************************************************************
//СТАТУС: I.7
//НАИМЕНОВАНИЕ ФУНКЦИИ: double SinLaw(const ARGLIST* Arg)
//НАЗНАЧЕНИЕ: F(t) = A*SIN(B*t+C) - синусоидальная функция от времени
//Функция описывает синусоидальный закон изменения величины во времени. Для
//работы функции неообходимо ей передать 4 параметра через структуру Arg.
//СТРУКТУРА СПИСКА Arg ДЛЯ НОРМАЛЬНОЙ РАБОТЫ SINLAW()
//Количество параметров: 4
//1. Arg.List[0] - double t - текущее время (модельное время)
//2. Arg.List[1] - double A - амплитуда
//3. Arg.List[2] - double B - угловая частота B = 2*PI*f
//4. Arg.List[3] - double C - начальная фаза
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. struct ARGLIST* Arg - список параметров закона изменения SinLaw
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: double value = A*SIN(B*t+C)
//*****************************************************************************
double SinLaw(const ARGLIST* Arg)
{
 double t;
 try
  {
   if (Arg == NULL) //Отсутствуют параметры
    throw ARG_MISMATCH;
   if (Arg->Size != 4) //Должно быть передано точно 4 параметра
    throw ARG_MISMATCH;
   t = Arg->List[0];
  }
 catch (int error_type)
  {
   if (error_type == ARG_MISMATCH)
   {
    #if defined(CONSOLE)
      cout << "\nMISMATCH ARGUMENT ERROR !!!" << endl;
      cout << "Function SinLaw() should accept four parameters: t,A,B,C.\n";
      cout << "Mathematical notation: F(t) = A*SIN(B*t + C)." << endl;
      getch();
    #endif
    exit(-1);
   }
  }
return Arg->List[1]*sin(Arg->List[2]*t+Arg->List[3]); //F(t) = A*SIN(B*t+C)
}
//*****************************************************************************
//СТАТУС: I.8
//НАИМЕНОВАНИЕ ФУНКЦИИ: double HyperbolLaw(const ARGLIST* Arg)
//НАЗНАЧЕНИЕ: F(t) = A/t - гиперболическая функция от времени
//Функция описывает гиперболический закон изменения величины во времени. Для
//работы функции неообходимо ей передать 2 параметра через структуру Arg.
//СТРУКТУРА СПИСКА Arg ДЛЯ НОРМАЛЬНОЙ РАБОТЫ HyperbolLaw()
//Количество параметров: 2
//1. Arg.List[0] - double t - текущее время (модельное время)
//2. Arg.List[1] - double A - константа
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. struct ARGLIST* Arg - список параметров закона изменения HyperbolLaw
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: double value = A/t
//*****************************************************************************
double HyperbolLaw(const ARGLIST* Arg)
{
 double t;
 try
  {
   if (Arg == NULL) //Отсутствуют параметры
    throw ARG_MISMATCH;
   if (Arg->Size != 2) //Должно быть передано точно 2 параметра
    throw ARG_MISMATCH;
   t = Arg->List[0]; t = (t == 0.0) ? t+EPSILON : t;
  }
 catch (int error_type)
  {
   if (error_type == ARG_MISMATCH)
   {
    #if defined(CONSOLE)
      cout << "\nMISMATCH ARGUMENT ERROR !!!" << endl;
      cout << "Function HyperbolLaw() should accept two parameters: t,A.\n";
      cout << "Mathematical notation: F(t) = A/t." << endl;
      getch();
    #endif
    exit(-1);
   }
  }
return Arg->List[1]/t; //F(t) = A/t
}
//*****************************************************************************
//СТАТУС: I.9
//НАИМЕНОВАНИЕ ФУНКЦИИ: double LnLaw(const ARGLIST* Arg)
//НАЗНАЧЕНИЕ: F(t) = Ln(t) - функция натурального логарифма от времени
//Функция описывает логарифмический закон изменения величины во времени. Для
//работы функции неообходимо ей передать 1 параметр через структуру Arg.
//СТРУКТУРА СПИСКА Arg ДЛЯ НОРМАЛЬНОЙ РАБОТЫ LnLaw()
//Количество параметров: 1
//1. Arg.List[0] - double t - текущее время (модельное время)
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. struct ARGLIST* Arg - список параметров закона изменения LnLaw
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: double value = Ln(t)
//*****************************************************************************
double LnLaw(const ARGLIST* Arg)
{
 double t;
 try
  {
   if (Arg == NULL) //Отсутствуют параметры
    throw ARG_MISMATCH;
   if (Arg->Size != 1) //Должен быть передан точно 1 параметр
    throw ARG_MISMATCH;
   if (Arg->List[0] < 0.0) //Отрицательное значение
    throw IMPOS_VALUE;
   t = Arg->List[0]; t = (t == 0.0) ? t+EPSILON : t;
  }
 catch (int error_type)
  {
   if (error_type == ARG_MISMATCH)
   {
    #if defined(CONSOLE)
      cout << "\nMISMATCH ARGUMENT ERROR !!!" << endl;
      cout << "Function LnLaw() should accept one parameter: t.\n";
      cout << "Mathematical notation: F(t) = Ln(t)." << endl;
      getch();
    #endif
    exit(-1);
   }
   if (error_type == IMPOS_VALUE) //Недопустимое значение аргумента функции
   {
    #if defined(CONSOLE)
      cout << "\nIMPOSSIBLE VALUE FOR FUNCTION LN(X) !!!" << endl;
      cout << "Incorrect value = " << Arg->List[0] << endl;
      cout << "Mathematical notation: F(t) = Ln(t), t > 0" << endl;
      getch();
    #endif
    exit(-1);
   }
  }
return log(t); //F(t) = ln(t)
}
//*****************************************************************************
//СТАТУС: I.10
//НАИМЕНОВАНИЕ ФУНКЦИИ: double Log10Law(const ARGLIST* Arg)
//НАЗНАЧЕНИЕ: F(t) = Log10(t) - функция десятичного логарифма от времени
//Функция описывает логарифмический закон изменения величины во времени. Для
//работы функции неообходимо ей передать 1 параметр через структуру Arg.
//СТРУКТУРА СПИСКА Arg ДЛЯ НОРМАЛЬНОЙ РАБОТЫ Log10Law()
//Количество параметров: 1
//1. Arg.List[0] - double t - текущее время (модельное время)
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. struct ARGLIST* Arg - список параметров закона изменения Log10Law
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: double value = Log10(t)
//*****************************************************************************
double Log10Law(const ARGLIST* Arg)
{
 double t;
 try
  {
   if (Arg == NULL) //Отсутствуют параметры
    throw ARG_MISMATCH;
   if (Arg->Size != 1) //Должен быть передан точно 1 параметр
    throw ARG_MISMATCH;
   if (Arg->List[0] < 0.0) //Отрицательное значение
    throw IMPOS_VALUE;
   t = Arg->List[0]; t = (t == 0.0) ? t+EPSILON : t;
  }
 catch (int error_type)
  {
   if (error_type == ARG_MISMATCH)
   {
    #if defined(CONSOLE)
      cout << "\nMISMATCH ARGUMENT ERROR !!!" << endl;
      cout << "Function Log10Law() should accept one parameter: t.\n";
      cout << "Mathematical notation: F(t) = Log10(t)." << endl;
      getch();
    #endif
    exit(-1);
   }
   if (error_type == IMPOS_VALUE) //Недопустимое значение аргумента функции
   {
    #if defined(CONSOLE)
      cout << "\nIMPOSSIBLE VALUE FOR FUNCTION Log10(X) !!!" << endl;
      cout << "Incorrect value = " << Arg->List[0] << endl;
      cout << "Mathematical notation: F(t) = Log10(t), t > 0" << endl;
      getch();
    #endif
    exit(-1);
   }
  }
return log10(t); //F(t) = log10(t)
}
//*****************************************************************************
//СТАТУС: I.11
//НАИМЕНОВАНИЕ ФУНКЦИИ: double LogALaw(const ARGLIST* Arg)
//НАЗНАЧЕНИЕ: F(t) = Log10(t) - функция логарифма по основанию A от времени
//Функция описывает логарифмический закон изменения величины во времени. Для
//работы функции неообходимо ей передать 2 параметра через структуру Arg.
//СТРУКТУРА СПИСКА Arg ДЛЯ НОРМАЛЬНОЙ РАБОТЫ LogALaw()
//Количество параметров: 1
//1. Arg.List[0] - double t - текущее время (модельное время)
//2. Arg.List[1] - double A - основание логарифма
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. struct ARGLIST* Arg - список параметров закона изменения Log10Law
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: double value = LogA(t)
//*****************************************************************************
double LogALaw(const ARGLIST* Arg)
{
 double t;
 try
  {
   if (Arg == NULL) //Отсутствуют параметры
    throw ARG_MISMATCH;
   if (Arg->Size != 2) //Должно быть передано точно 2 параметра
    throw ARG_MISMATCH;
   if (Arg->List[0] < 0.0) //Отрицательное значение
    throw IMPOS_VALUE;
   t = Arg->List[0]; t = (t == 0.0) ? t+EPSILON : t;
  }
 catch (int error_type)
  {
   if (error_type == ARG_MISMATCH)
   {
    #if defined(CONSOLE)
      cout << "\nMISMATCH ARGUMENT ERROR !!!" << endl;
      cout << "Function LogALaw() should accept two parameters: t, A." << endl;
      cout << "A - logarithmic base" << endl;
      cout << "Mathematical notation: F(t) = LogA(t)." << endl;
      getch();
    #endif
    exit(-1);
   }
   if (error_type == IMPOS_VALUE) //Недопустимое значение аргумента функции
   {
    #if defined(CONSOLE)
      cout << "\nIMPOSSIBLE VALUE FOR FUNCTION LogA(X) !!!" << endl;
      cout << "Incorrect value = " << Arg->List[0] << endl;
      cout << "Mathematical notation: F(t) = LogA(t), t > 0" << endl;
      getch();
    #endif
    exit(-1);
   }
  }
return log(t)/log(Arg->List[1]); //F(t) = logA(t)
}
//*****************************************************************************
//СТАТУС: I.12
//НАИМЕНОВАНИЕ ФУНКЦИИ: double SgrtLaw(const ARGLIST* Arg)
//НАЗНАЧЕНИЕ: F(t) = Sgrt(A*t+B) - функция квадратного корня от времени
//Функция описывает закон квадратного корня из линейного двучлена. Для работы
//функции неообходимо ей передать 3 параметра через структуру Arg.
//СТРУКТУРА СПИСКА Arg ДЛЯ НОРМАЛЬНОЙ РАБОТЫ SqrtLaw()
//Количество параметров: 3
//1. Arg.List[0] - double t - текущее время (модельное время)
//2. Arg.List[1] - double A - константа
//3. Arg.List[2] - double B - константа
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. struct ARGLIST* Arg - список параметров закона изменения SqrtLaw
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: double value = Sqrt(t)
//*****************************************************************************
double SqrtLaw(const ARGLIST* Arg)
{
 double t;
 try
  {
   if (Arg == NULL) //Отсутствуют параметры
    throw ARG_MISMATCH;
   if (Arg->Size != 3) //Должно быть передано точно 3 параметра
    throw ARG_MISMATCH;
   //Вычисление подкоренного выражения
   t = Arg->List[0]*Arg->List[1]+Arg->List[2];
   if (t < 0.0) //Отрицательное значение
    throw IMPOS_VALUE;
  }
 catch (int error_type)
  {
   if (error_type == ARG_MISMATCH)
   {
    #if defined(CONSOLE)
      cout << "\nMISMATCH ARGUMENT ERROR !!!" << endl;
      cout << "Function SqrtLaw() should accept three parameters: t,A,B." << endl;
      cout << "Mathematical notation: F(t) = SQRT(A*t+B)." << endl;
      getch();
    #endif
    exit(-1);
   }
   if (error_type == IMPOS_VALUE) //Недопустимое значение аргумента функции
   {
    #if defined(CONSOLE)
      cout << "\nIMPOSSIBLE VALUE FOR FUNCTION SQRT(A*t+B) !!!" << endl;
      cout << "Incorrect value = " << t << ",value = (A*t+B) => 0" << endl;
      cout << "t = " << Arg->List[0] << ", A = " << Arg->List[1] << " ,B = "
           << Arg->List[2] << endl;
      cout << "Mathematical notation: F(t) = SQRT(A*t+B)" << endl;
      getch();
    #endif
    exit(-1);
   }
  }
return sqrt(t); //F(t) = SQRT(A*t+B)
}
//*****************************************************************************
//СТАТУС: I.13
//НАИМЕНОВАНИЕ ФУНКЦИИ: double SquarePulseLaw(const ARGLIST* Arg)
//НАЗНАЧЕНИЕ:
//Функция описывает закон изменения прямоугольного импульса во времени. Для
//работы функции неообходимо ей передать 8 параметров через структуру Arg.
//СТРУКТУРА СПИСКА Arg ДЛЯ НОРМАЛЬНОЙ РАБОТЫ SquarePulseLaw()
//Количество параметров: 8
//1. Arg.List[0] - T - текущее время (модельное время)
//2. Arg.List[1] - t0 - начало следования импульсов с момента времени моделиро-
//   вания - константа (t0 >= 0)
//3. Arg.List[2] - timp - длительность импульса - константа (timp > 0)
//4. Arg.List[3] - Timp - период следования импульсов - константа (Timp > timp)
//5. Arg.List[4] - Umax - уровень сигнала при следовании импульса - константа
//6. Arg.List[5] - Umin - уровень сигнала при отсутствии импульса - константа
//Изменяемые параметры
//7. Arg.List[6] - flag - состояние импульсного сигнала - переменная
//   Возможные значения: -1.0 - импульс отсутствует; 0.0 - состояние ожидания
//   появления первого импульса с момента моделирования; +1.0 - импульс есть.
//8. Arg.List[7] - count - счетчик времени - переменная. Счетчик времени показы-
//   вает остаток времени до моментов 1) появления импульса, 2) отсутствия им-
//   пульса, 3) появления первого импульса. Счетчик времени изменяется дискретно
//   на единицу (-1.0) каждый раз при обращении к функции.
//Начальное состояние рабочих параметров: flag = 0.0, count = 0.0. Данная функ-
//ция модельное время не использует. Так как count изменяется дискретно, то и
//временные параметры прямоугольного импульса имеет смысл задавать целочисленны-
//ми величинами (модельное время, как правило, целочисленное значение).
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. struct ARGLIST* Arg - список параметров закона изменения SqrtLaw
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: double value - уровень прямоугольного сигнала
//*****************************************************************************
double SquarePulseLaw(const ARGLIST* Arg)
{
 try
  {
   if (Arg == NULL) //Отсутствуют параметры
    throw ARG_MISMATCH;
   if (Arg->Size != 8) //Должно быть передано точно 8 параметров
    throw ARG_MISMATCH;
   //******************************************************************
   //Вычисление уровня прямоугольного импульса в момент времени T
   //******************************************************************
   //Установка начальных значений рабочих параметров вначале моделирования
   if (Arg->List[0] == 0) {Arg->List[6] = 0; Arg->List[7] = 0;}

   if (Arg->List[6] == 0) //Состояние ожидания первого импульса
   {if (Arg->List[1] == 0) //Время ожидания t0 равно нулю
    { Arg->List[6] = 1; Arg->List[7] = Arg->List[2]; return Arg->List[4];}
    else //Время ожидания t0 отлично от нуля
    {if (Arg->List[7] == 0) //Первое вхождение
     { Arg->List[7] = Arg->List[1]; return Arg->List[5];} //Импульса нет: Umin
     else //Последующие вхождения
     { if (--Arg->List[7] > 0) return Arg->List[5]; //Импульса нет: Umin
       //Первый импульс появился
       else
       {Arg->List[6] = 1; Arg->List[7] = Arg->List[2]; return Arg->List[4];}
     }
    }
   }
   else if (Arg->List[6] == 1) //Импульс присутствует
   { if (--Arg->List[7] > 0) return Arg->List[4]; //Импульс есть
     else //Импульса нет
     {Arg->List[6] = -1; Arg->List[7] = fabs(Arg->List[3] - Arg->List[2]);
      return Arg->List[5];
     }
   }
   else if (Arg->List[6] == -1) //Импульса нет
   { if (--Arg->List[7] > 0) return Arg->List[5]; //Импульса нет
     else //Импульс появился
     {Arg->List[6] = 1; Arg->List[7] = Arg->List[2]; return Arg->List[4];}
   }

  }
 catch (int error_type)
  {
   if (error_type == ARG_MISMATCH)
   {
    #if defined(CONSOLE)
      cout << "\nMISMATCH ARGUMENT ERROR !!!" << endl;
      cout << "Function SqrtLaw() should accept three parameters: t,A,B." << endl;
      cout << "Mathematical notation: F(t) = SQRT(A*t+B)." << endl;
      getch();
    #endif
    exit(-1);
   }
  }
return Arg->List[5];
}

//***********************************************************************
// II. СЛУЧАЙНЫЕ ФУНКЦИИ
//*******************************************************************
//*****************************************************************************
//СТАТУС: II.1
//НАИМЕНОВАНИЕ ФУНКЦИИ: double RandLaw(const ARGLIST* Arg)
//НАЗНАЧЕНИЕ: Равновероятный случайный закон [0..1]
//Функция возвращает равномерно распределенную СВ на интервале [0..1].
//В списке параметров Arg функция не нуждается
//СТРУКТУРА СПИСКА Arg ДЛЯ НОРМАЛЬНОЙ РАБОТЫ RandLaw()
//Количество параметров: 0
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. struct ARGLIST* Arg - список параметров RandLaw
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: double - СВ распределенная равновероятно
//*****************************************************************************
double RandLaw(const ARGLIST* Arg)
{
 //double value = (double) rand()/RAND_MAX;
 double value = RNG.Random();
 if (Arg != NULL) return value;
return  value;
}
//*****************************************************************************
//СТАТУС: II.2
//НАИМЕНОВАНИЕ ФУНКЦИИ: double Rand2PiLaw(const ARGLIST* Arg)
//НАЗНАЧЕНИЕ: Равновероятный случайный закон [0..2Pi]
//Функция возвращает равномерно распределенную СВ на интервале [0..2Pi].
//В списке параметров Arg функция не нуждается
//СТРУКТУРА СПИСКА Arg ДЛЯ НОРМАЛЬНОЙ РАБОТЫ Rand2PiLaw()
//Количество параметров: 0
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. struct ARGLIST* Arg - список параметров Rand2PiLaw
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: double - СВ равномерно распределенная [0..2PI]
//*****************************************************************************
double Rand2PiLaw(const ARGLIST* Arg)
{
 return RandLaw(Arg)*TwoPI;
}
//*****************************************************************************
//СТАТУС: II.3
//НАИМЕНОВАНИЕ ФУНКЦИИ: double RandABLaw(const ARGLIST* Arg)
//НАЗНАЧЕНИЕ: Равновероятный случайный закон [A..B]
//(B-RandLaw()*(B-A))
//Функция возвращает равномерно распределенную СВ на интервале [A..B].
//СТРУКТУРА СПИСКА Arg ДЛЯ НОРМАЛЬНОЙ РАБОТЫ RandABLaw()
//Количество параметров: 2
// Arg.List[0] - A (нижняя граница интервала распределения)
// Arg.List[1] - B (верхняя граница интервала распределения)
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. struct ARGLIST* Arg - список параметров RandABLaw
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: double - СВ равномерно распределенная [A..B]
//*****************************************************************************
double RandABLaw(const ARGLIST* Arg)
{
 double value;
 try
   {
    if (Arg == NULL) //Отсутствуют параметры
     throw ARG_MISMATCH;
    if (Arg->Size != 3) //Должно быть передано точно 2 параметра
     throw ARG_MISMATCH;
    value = Arg->List[1];
    if (value > Arg->List[2])
    { value = Arg->List[2];
      Arg->List[2] = Arg->List[1];
      Arg->List[1] = value;
    }
    value = Arg->List[2] - (Arg->List[2]-Arg->List[1])*RandLaw(Arg);
   }
 catch (int error_type)
  {
   if (error_type == ARG_MISMATCH)
   {
    #if defined(CONSOLE)
      cout << "\nMISMATCH ARGUMENT ERROR !!!" << endl;
      cout << "Function RandABLaw() should accept two parameters: A,B.\n";
      cout << "A - interval lower border; B - interval upper border" << endl;
      getch();
    #endif
    exit(-1);
   }
  }
return value;
}
//*****************************************************************************
//СТАТУС: II.4
//НАИМЕНОВАНИЕ ФУНКЦИИ: double RelayLaw(const ARGLIST* Arg)
//НАЗНАЧЕНИЕ: Случайный закон Релея [0..1]
//sqrt(-2*ln(RandLaw())
//Функция возвращает СВ распределенную по РЕЛЕЮ на интервале [0..1]. В этом
//интервале функция может принимать значения - [+INFINITE..0]
//В списке параметров Arg функция не нуждается
//СТРУКТУРА СПИСКА Arg ДЛЯ НОРМАЛЬНОЙ РАБОТЫ RelayLaw()
//Количество параметров: 0
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. struct ARGLIST* Arg - список параметров RelayLaw
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: double - СВ распределенная на интервале [0..1]
//*****************************************************************************
double RelayLaw(const ARGLIST* Arg)
{ double value;
  //Равномерно распределенная случайная величина не должна быть равна нулю
  do { value = RandLaw(Arg); } while (value == 0.0);
  //Вычисляем СВ распределенную по закону РЕЛЕЯ
  value = sqrt(-2.0*log(value));
 return value;
}
//*****************************************************************************
//СТАТУС: II.5
//НАИМЕНОВАНИЕ ФУНКЦИИ: double Det1Law(const ARGLIST* Arg)
//НАЗНАЧЕНИЕ: ИСКЛЮЧАЕТ СЛУЧАЙНУЮ ВЕЛИЧИНУ ИЗ РАССМОТРЕНИЯ
//Функция возвращает 1. Применяется для исключения из рассмотрения случайной
//величины, которая перемножается с другими величинами.
//СТРУКТУРА СПИСКА Arg ДЛЯ НОРМАЛЬНОЙ РАБОТЫ Det1Law()
//Количество параметров: 0
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. struct ARGLIST* Arg - список параметров Det1Law()
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: double - 1.0
//*****************************************************************************
double Det1Law(const ARGLIST* Arg)
{
 if (Arg == NULL) return 1.0;
 return 1.0;
}
//*****************************************************************************
//СТАТУС: II.5
//НАИМЕНОВАНИЕ ФУНКЦИИ: double Det0Law(const ARGLIST* Arg)
//НАЗНАЧЕНИЕ: ИСКЛЮЧАЕТ СЛУЧАЙНУЮ ВЕЛИЧИНУ ИЗ РАССМОТРЕНИЯ
//Функция возвращает 0. Применяется для исключения из рассмотрения случайной
//величины, которая складывается с другими величинами.
//СТРУКТУРА СПИСКА Arg ДЛЯ НОРМАЛЬНОЙ РАБОТЫ Det0Law()
//Количество параметров: 0
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. struct ARGLIST* Arg - список параметров Det0Law()
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: double - 0.0
//*****************************************************************************
double Det0Law(const ARGLIST* Arg)
{
 if (Arg == NULL) return 0.0;
 return 0.0;
}

//***********************************************************************
// III. ОПИСАНИЕ ФУНКЦИЙ ПОДГОТАВЛИВАЮЩИХ СПИСОК ПАРАМЕТРОВ Arg
//***********************************************************************

//*****************************************************************************
//СТАТУС: III.1
//НАИМЕНОВАНИЕ ФУНКЦИИ: ARGLIST* CreateArgList(const ulong size)
//НАЗНАЧЕНИЕ:
//Функция предназначена для создания структуры ARGLIST для хранения списка
//аргументов. Структура ARGLIST представляет собой динамический одномерный
//массив ARGLIST.List и переменную ARGLIST.Size - количество элементов массива.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. ulong size - размер массива
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: ARGLIST *ptr - указатель на созданную структуру
//*****************************************************************************
ARGLIST* CreateArgList(const ulong size)
{ ARGLIST *ptr = NULL;
  try //Распределение памяти под элементы массива с аргументами
  {   //и первоначальная их инициализация
   ptr = new ARGLIST;
   ptr->Size = size;
  	ptr->List = new double[size];
   for( ulong i=0; i<size; i++) ptr->List[i] = 0.0;
   return ptr;
  }
  // ENTER THIS BLOCK ONLY IF xalloc IS THROWN.
  // YOU COULD REQUEST OTHER ACTIONS BEFORE TERMINATING
  catch (xalloc x)
  {
    #if defined(CONSOLE)
      cout << "\nMEMORY ALLOCATION ERROR !!!" << endl;
      cout << "Could not allocate all parameters law in memory."
           << endl;
      cout << "Memory request is " << x.requested() << " byte." << endl;
      getch();
    #endif
    exit(-1);
  }
return ptr;
}
//*****************************************************************************
//СТАТУС: III.2
//НАИМЕНОВАНИЕ ФУНКЦИИ: ARGLIST* DeleteArgList(ARGLIST *Arg)
//НАЗНАЧЕНИЕ:
//Функция удаляет структуру ARGLIST хранения списка аргументов. Структура
//ARGLIST представляет собой динамический одномерный массив ARGLIST.List и
//переменную ARGLIST.Size - количество элементов массива.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. ARGLIST *Arg - указатель удаляемого списка аргументов
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: нулевой указатель
//*****************************************************************************
ARGLIST* DeleteArgList(ARGLIST *Arg)
{if (Arg != NULL)
 {if (Arg->List != NULL) {delete[] Arg->List; Arg->List = NULL; Arg->Size = 0L;}
  delete Arg; Arg = NULL;
 }
return Arg;
}
//*****************************************************************************
//СТАТУС: III.3
//НАИМЕНОВАНИЕ ФУНКЦИИ:
// ARGLIST* CopyArgList(const ARGLIST *Source, ARGLIST* Target)
//НАЗНАЧЕНИЕ:
//Функция копирует структуру ARGLIST хранения списка аргументов из источника
//Source в приемник Target. При копировании содержимое Target уничтожается.
//ARGLIST представляет собой динамический одномерный массив ARGLIST.List и
//переменную ARGLIST.Size - количество элементов массива.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. const ARGLIST *Source - указатель исходного списка аргументов
//2. ARGLIST *Target - указатель списка аргументов, в который будет скопировано
//   содержимое списка Source
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: ARGLIST* Target - указатель списка-дубликата
//*****************************************************************************
ARGLIST* CopyArgList(const ARGLIST* Source, ARGLIST* Target)
{
 if (Target != NULL) Target = DeleteArgList(Target);
 //Исходный список - пустой
 if (Source == NULL) return Target;
 //Создаем список Target одинакового размера с Source
 Target = CreateArgList(Source->Size);
 //Копируем параметры
 for (ulong i = 0; i < Target->Size; i++) Target->List[i] = Source->List[i];

return Target;
}
//*****************************************************************************
//СТАТУС: III.4
//НАИМЕНОВАНИЕ ФУНКЦИИ:
//V_ARGLIST* CreateVectorArgList(const ulong VectorSize, const ulong ListSize=0)
//НАЗНАЧЕНИЕ:
//Функция предназначена для создания структуры V_ARGLIST для хранения списка
//структур ARGLIST. Структура V_ARGLIST представляет собой динамический одномер-
//ный массив указателей на ARGLIST и переменную V_ARGLIST.Size - количество
//структур ARGLIST, хранящихся в векторе V_ARGLIST. Функция создает списки аргу-
//ментов типа double ОДИНАКОВОГО РАЗМЕРА, определяемого переменной ListSize. Ко-
//личество создаваемых структур типа ARGLIST определяется переменной VectorSize.
//Функция возвращает указатель на вектор структур ARGLIST - pV_ARGLIST.
//Если ListSize = 0 (значение по умолчанию), то структуры ARGLIST не создаются.
//В этом случае массив указателей на структуры, инициализируется значениями NULL.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. const ulong VectorSize - размер вектора V_ARGLIST;
//2. const ulong ListSize - размер структуры ARGLIST;
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: V_ARGLIST *ptr - указатель на созданную структуру
//*****************************************************************************
V_ARGLIST* CreateVectorArgList(const ulong VectorSize, const ulong ListSize)
{ V_ARGLIST *ptr = NULL;
  try //Распределение памяти под элементы вектора V_ARGLIST, под каждую структу-
  {   //ру ARGLIST, хранимую в данном векторе и первоначальная их инициализация
   ptr = new V_ARGLIST;
   ptr->Size = VectorSize;
  	ptr->Vector = new ARGLIST*[VectorSize];
   //Начальная инициализация указателей на структуры ARGLIST
   for( ulong i=0; i<VectorSize; i++) ptr->Vector[i] = NULL;
   //Распределение памяти под хранение данных типа double в каждой структуре
   //ARGLIST, хранимой в векторе (массиве) структур ARGLIST.
   if (ListSize != 0)
    for (ulong i = 0; i < VectorSize; i++)
     ptr->Vector[i] = CreateArgList(ListSize);
   return ptr;
  }
  // ENTER THIS BLOCK ONLY IF xalloc IS THROWN.
  // YOU COULD REQUEST OTHER ACTIONS BEFORE TERMINATING
  catch (xalloc x)
  {
    #if defined(CONSOLE)
      cout << "\nMEMORY ALLOCATION ERROR !!!" << endl;
      cout << "Could not allocate all structures of ARGLIST in memory."
           << endl;
      cout << "Memory request is " << x.requested() << " byte." << endl;
      getch();
    #endif
    exit(-1);
  }
return ptr;
}
//*****************************************************************************
//СТАТУС: III.5
//НАИМЕНОВАНИЕ ФУНКЦИИ: V_ARGLIST* DeleteVectorArgList(V_ARGLIST *Vector)
//НАЗНАЧЕНИЕ:
//Функция удаляет структуру V_ARGLIST хранения структур типа ARGLIST. Структура
//V_ARGLIST представляет собой динамический одномерный массив указателей на
//структуры V_ARGLIST.Vector и переменную V_ARGLIST.Size - количество элементов
//вектора (массива).
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. V_ARGLIST *Vector - указатель удаляемого вектора структур ARGLIST.
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
// V_ARGLIST *Vector - нулевой указатель
//*****************************************************************************
V_ARGLIST* DeleteVectorArgList(V_ARGLIST *Vector)
{if (Vector != NULL)
 {//Удаление элементов вектора - структур ARGLIST
  if (Vector->Vector != NULL)
   for (ulong i = 0; i < Vector->Size; i++)
    Vector->Vector[i] = DeleteArgList(Vector->Vector[i]);
  //Удаление самого вектора
  delete[] Vector->Vector; Vector->Vector = NULL; Vector->Size = 0L;
  //Удаление указателя на структуру V_ARGLIST
  delete Vector; Vector = NULL;
 }
return Vector;
}

//*****************************************************************************
//СТАТУС: III.6
//НАИМЕНОВАНИЕ ФУНКЦИИ: ARGLIST* InitFactors(const MEASURE& M);
//НАЗНАЧЕНИЕ:
//Универсальная функция создания и начальной инициализации параметров функции.
//Функция предназначена для создания списка параметров ARGLIST, его инициализа-
//ции с целью дальнейшей работы с ним конкретной функции. Характеристики аргу-
//мента и параметров (коэффициентов) конкретной функции определяются структурой
//MEASURE. Инициализация начинается со второго параметра - Arg->List[1], т.к.
// Arg->List[0] содержит аргумент функции, которая будет все время изменяться
//при вычислениях и начальное его задание в большинстве случаев не является
//целесообразным. Для функций случайных величин Arg->List[0] содержит вычислен-
//ное значение случайной величины.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. const MEASURE& M  - характеристика параметров и аргумента функции
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: ARGLIST* Arg - список параметров для работы функции
//*****************************************************************************
ARGLIST* InitFactors(const MEASURE& M)
{ ARGLIST* Arg = CreateArgList(M.Size);

  //Только аргумент функции, параметры отсутствуют (F(t)=0), случайные функции
  //без параметров - RandLaw(),Rand2PILaw(),RelayLaw(), etc.
  if (M.Size == 1) return Arg;

 //Начальная инициализация всех параметров функции
 for (ulong i = 1; i < M.Size; i++)
  Arg->List[i] = GetDoubleValue(Arg->List[i],M.Factor[i]);

return Arg;
}

//*****************************************************************************
//СТАТУС: III.7
//НАИМЕНОВАНИЕ ФУНКЦИИ: double GetDoubleValue(double value, const FACTOR& F)
//НАЗНАЧЕНИЕ:
//Универсальная функция предназначена для ввода значения типа double. Функция
//осуществляет пред- и постпроверку возможности входа и выхода из редактирования
//переменной, преобразует по необходимости вводимую величину из одной размерности
//в другую (например, из DEG в RAD). Характеристики на редактируемую переменную
//передаются через структуру типа FACTOR.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. double value - значение редактируемой переменной
//2. const FACTOR* F - указатель на структуру с характеристиками редактируемой
//   величины типа double
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
//double value - отредактированное значение типа double
//*****************************************************************************
double GetDoubleValue(double value, const FACTOR& F)
{ uint Y = wherey();
  double convert;
  char c;
  bool lBreak;

  //Предпроверка возможности редактирования переменной
  if (!(F.PreBlock)) //Запрет на редактирование
   return value;     //Выход из редактирования
 do
 { do
    { cout << "Input " << F.Name << " [" << GetMeasureName(F.SourceDim)
           << "] : ";
      cin >> value;
      gotoxy(1,Y); clreol();
      //Подтверждение ввода и
      cout << "< " << F.Name << " = " << value << " ["
           << GetMeasureName(F.SourceDim) << "]";
      //Преобразование вводимой размерности SourceDim в заданную TargetDim
      if (F.SourceDim != F.TargetDim)
      { convert = F.Convert(value);
        cout << " | " << convert << " [" << GetMeasureName(F.TargetDim) << "]";
      }
      cout << " > ? [Y/N]: ";
      cin >> c;
      if((c == 'Y') || (c == 'y')) lBreak = true;
      else lBreak = false;
      gotoxy(1,Y); clreol();
    } while(!lBreak);
  } while(!(F.PostBlock)); //Постпроверка вводимой величины

 //Преобразование вводимой размерности SourceDim в заданную TargetDim
 if (F.SourceDim != F.TargetDim) value = F.Convert(value);

return value;
}

//*****************************************************************************
//СТАТУС: III.8 ПРЕД- и ПОСТПРОВЕРКА редактируемых величин типа double
//НАИМЕНОВАНИЕ ФУНКЦИИ: bool AlwaysTrue(double value);
//НАЗНАЧЕНИЕ:
//Универсальная функция пред- и постпроверки редактируемой величины типа double.
//Всегда возвращает true
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. double value - значение редактируемого параметра
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: bool - результат проверки
//*****************************************************************************
bool AlwaysTrue(double value,...)
{
 //Для предотвращения предупреждений при компиляции !!!
 if (value > 0.0) return true;
 else return true;
}
//*****************************************************************************
//СТАТУС: III.9 ПРЕД- и ПОСТПРОВЕРКА редактируемых величин типа double
//НАИМЕНОВАНИЕ ФУНКЦИИ: bool AlwaysFalse(double value);
//НАЗНАЧЕНИЕ:
//Универсальная функция пред- и постпроверки редактируемой величины типа double.
//Всегда возвращает false
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. double value - значение редактируемого параметра
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: bool - результат проверки
//*****************************************************************************
bool AlwaysFalse(double value)
{
 //Для предотвращения предупреждений при компиляции !!!
 if (value > 0.0) return false;
 else return false;
}

//*****************************************************************************
//СТАТУС: III.10 ПРЕОБРАЗОВАНИЕ ИЗ ОДНОЙ РАЗМЕРНОСТИ В ДРУГУЮ
//НАИМЕНОВАНИЕ ФУНКЦИИ: double DegToRad(double value)
//НАЗНАЧЕНИЕ:
//Функция предназначена для перевода размерности величины из градусной меры
//в радианную.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. double value - значение величины в градусах
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
//double value - значение величины в радианах
//*****************************************************************************
double DegToRad(double value)
{
 return value*DegRad;
}

//*****************************************************************************
//СТАТУС: III.11 ПРЕОБРАЗОВАНИЕ ИЗ ОДНОЙ РАЗМЕРНОСТИ В ДРУГУЮ
//НАИМЕНОВАНИЕ ФУНКЦИИ: double RadToDeg(double value)
//НАЗНАЧЕНИЕ:
//Функция предназначена для перевода размерности величины из радианной меры
//в градусную.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. double value - значение величины в радианах
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
//double value - значение величины в градусах
//*****************************************************************************
double RadToDeg(double value)
{
 return value*RadDeg;
}
//*****************************************************************************
//СТАТУС: III.12 ПРЕОБРАЗОВАНИЕ ИЗ ОДНОЙ РАЗМЕРНОСТИ В ДРУГУЮ
//НАИМЕНОВАНИЕ ФУНКЦИИ: double FreqToPulsatance(double value)
//НАЗНАЧЕНИЕ:
//Функция предназначена для перевода линейной частоты в угловую частоту.
//(Hz -> RAD/SEC)
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. double value - значение частоты в герцах (линейная частота)
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
//double value - значение частоты в RAD/SEC (угловая частота)
//*****************************************************************************
double FreqToPulsatance(double value)
{
 return value*TwoPI;
}
//*****************************************************************************
//СТАТУС: III.13 ПРЕОБРАЗОВАНИЕ ИЗ ОДНОЙ РАЗМЕРНОСТИ В ДРУГУЮ
//НАИМЕНОВАНИЕ ФУНКЦИИ: double PulsatanceToFreq(double value)
//НАЗНАЧЕНИЕ:
//Функция предназначена для перевода угловой частоты в линейную частоту.
//(RAD/SEC -> Hz)
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. double value - значение частоты в RAD/SEC (угловая частота)
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
//double value - значение частоты в герцах (линейная частота)
//*****************************************************************************
double PulsatanceToFreq(double value)
{
 return value/TwoPI;
}
//*****************************************************************************
//СТАТУС: III.14 ПРЕОБРАЗОВАНИЕ ИЗ ОДНОЙ РАЗМЕРНОСТИ В ДРУГУЮ
//НАИМЕНОВАНИЕ ФУНКЦИИ: double Copy(double value)
//НАЗНАЧЕНИЕ:
//Функция возвращает величину такой же размерности, как и переданная величина
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. double value - исходное значение
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
//double value - заданное значение
//*****************************************************************************
double Copy(double value)
{
 return value;
}


//**************************************************************************
// IV. ОБЪЯВЛЕНИЕ ФУНКЦИЙ, ФОРМИРУЮЩИХ МАТЕМАТИЧЕСКУЮ ЗАПИСЬ ЗАКОНА ИЗМЕНЕНИЯ
//     ВЕЛИЧИНЫ ОТ ВРЕМЕНИ С КОНКРЕТНЫМИ ПАРАМЕТРАМИ
//**************************************************************************
//*****************************************************************************
//СТАТУС: IV.1
//НАИМЕНОВАНИЕ ФУНКЦИИ: char* GetMathNotateZeroLaw(const ARGLIST *Arg)
//НАЗНАЧЕНИЕ:
//Функция формирует математическую запись закон изменения величины от времени
//F(t) = 0 в виде строки.
//СТРУКТУРА СПИСКА Arg ДЛЯ НОРМАЛЬНОЙ РАБОТЫ ФУНКЦИИ
//Количество параметров: 1
//1. Arg.List[0] - текущее (модельное) время t
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. ARGLIST *Arg - список параметров закона ZeroLaw()
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
// char *s - закон изменения ZeroLaw в виде символьной строки
//*****************************************************************************
char* GetMathNotateZeroLaw(const ARGLIST *Arg)
{
 char* s = new char[80];//Строка с математической записью закона
 ostrstream text(s, 80);
 try
  {
  	if (Arg == NULL)  //Должен быть список аргументов
    throw ARG_MISMATCH;
   if (Arg->Size != 1)
    throw ARG_MISMATCH;
   text << "F(t) = 0" << ends;
  }
 catch (int error_type)
  {
   if (error_type == ARG_MISMATCH)
   {
    #if defined(CONSOLE)
      cout << "\nMISMATCH ARGUMENT ERROR !!!" << endl;
      cout << "One argument should be passed to GetMathNotateZeroLaw().\n";
      cout << "Mathematical notation: F(t) = 0, t - time." << endl;
      getch();
    #endif
    exit(-1);
   }
  }
return s;
}
//*****************************************************************************
//СТАТУС: IV.2
//НАИМЕНОВАНИЕ ФУНКЦИИ: char* GetMathNotateConstLaw(const ARGLIST *Arg)
//НАЗНАЧЕНИЕ:
//Функция формирует математическую запись закон изменения величины от времени
//F(t) = A в виде строки.
//СТРУКТУРА CПИСКА Arg ДЛЯ НОРМАЛЬНОЙ РАБОТЫ ФУНКЦИИ
//Количество параметров: 2
//1. Arg.List[0] - текущее (модельное) время t (аргумент функции F(t) - t)
//2. Arg.List[1] - const (величина типа double). F(t) = 5.0 => arg[1] = 5.0
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. ARGLIST *Arg - список параметров закона изменения ConstLaw
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
// char *s - закон изменения ConstLaw в виде символьной строки
//*****************************************************************************
char* GetMathNotateConstLaw(const ARGLIST* Arg)
{
 char* s = new char[80];//Строка с математической записью закона
 ostrstream text(s, 80);
 try
  {
  	if (Arg == NULL)  //Аргументы не переданы
    throw ARG_MISMATCH;
   if (Arg->Size != 2) //Должно быть передано точно два параметра
    throw ARG_MISMATCH;
   text << "F(t) = " << Arg->List[1] << ends;
  }
 catch (int error_type)
  {
   if (error_type == ARG_MISMATCH)
   {
    #if defined(CONSOLE)
      cout << "\nMISMATCH ARGUMENT ERROR !!!" << endl;
      cout << "GetMathNotateConstLaw() should accept two parameters: t & A.\n";
      cout << "Mathematical notation: F(t) = A, t - time." << endl;
      getch();
    #endif
    exit(-1);
   }
  }
return s;
}
//*****************************************************************************
//СТАТУС: IV.3
//НАИМЕНОВАНИЕ ФУНКЦИИ: char* GetMathNotateLinearLaw(const ARGLIST* Arg)
//НАЗНАЧЕНИЕ:
//Функция формирует математическую запись закон изменения величины от времени
//F(t) = A*t + B в виде строки.
//СТРУКТУРА СПИСКА Arg ДЛЯ НОРМАЛЬНОЙ РАБОТЫ ФУНКЦИИ
//Количество параметров: 3
//1. Arg.List[0] - t - текущее время (модельное время)
//2. Arg.List[1] - A
//3. Arg.List[2] - B
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. ARGLIST *Arg - список параметров закона изменения LinearLaw
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
// char *s - закон изменения LinearLaw в виде символьной строки
//*****************************************************************************
char* GetMathNotateLinearLaw(const ARGLIST* Arg)
{
 char* s = new char[80];//Строка с математической записью закона
 ostrstream text(s, 80);
 try
  {
  	if (Arg == NULL)  //Аргументы не переданы
    throw ARG_MISMATCH;
   if (Arg->Size != 3) //Должно быть передано точно три параметра
    throw ARG_MISMATCH;
   text << "F(t) = " << Arg->List[1] << "*t";
   if(Arg->List[2]>=0) text << " + ";
   text << Arg->List[2] << ends;
  }
 catch (int error_type)
  {
   if (error_type == ARG_MISMATCH)
   {
    #if defined(CONSOLE)
      cout << "\nMISMATCH ARGUMENT ERROR !!!" << endl;
      cout << "GetMathNotateLinearLaw() should accept three parameters: t,A,B.\n";
      cout << "Mathematical notation: F(t) = A*t + B." << endl;
      getch();
    #endif
     exit(-1);
   }
  }
return s; //F(t) = A*t + B
}
//*****************************************************************************
//СТАТУС: IV.4
//НАИМЕНОВАНИЕ ФУНКЦИИ: char* GetMathNotateSquareLaw(const ARGLIST* Arg)
//НАЗНАЧЕНИЕ:
//Функция формирует математическую запись закон изменения величины от времени
//F(t) = A*t^2 + B*t + C в виде строки.
//СТРУКТУРА СПИСКАА Arg ДЛЯ НОРМАЛЬНОЙ РАБОТЫ ФУНКЦИИ
//Количество параметров: 4
//1. Arg.List[0] - double t - текущее время (модельное время)
//2. Arg.List[1] - double A
//3. Arg.List[2] - double B
//4. Arg.List[3] - double C
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. ARGLIST* Arg - список параметров закона изменения SquareLaw
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
// char *s - закон изменения SquareLaw в виде символьной строки
//*****************************************************************************
char* GetMathNotateSquareLaw(const ARGLIST* Arg)
{
 char* s = new char[80];//Строка с математической записью закона
 ostrstream text(s, 80);
 try
  {
  	if (Arg == NULL)  //Аргументы не переданы
    throw ARG_MISMATCH;
   if (Arg->Size != 4) //Должно быть передано точно 4 параметра
    throw ARG_MISMATCH;
   text << "F(t) = " << Arg->List[1] << "*t^2";
   if(Arg->List[2]>=0) text << "+";
   text << Arg->List[2] << "*t";
   if(Arg->List[3]>=0)  text << "+";
   text << Arg->List[3] << ends;
  }
 catch (int error_type)
  {
   if (error_type == ARG_MISMATCH)
   {
    #if defined(CONSOLE)
      cout << "\nMISMATCH ARGUMENT ERROR !!!" << endl;
      cout << "GetMathNotateSquareLaw() should accept four parameters: t,A,B,C.\n";
      cout << "Mathematical notation: F(t) = A*t^2 + B*t + C." << endl;
      getch();
    #endif
    exit(-1);
   }
  }
return s; //F(t) = A*t^2 + B*t + C
}
//*****************************************************************************
//СТАТУС: IV.5
//НАИМЕНОВАНИЕ ФУНКЦИИ: char* GetMathNotateExpLaw(const ARGLIST *Arg)
//НАЗНАЧЕНИЕ:
//Функция формирует математическую запись закон изменения величины от времени
//F(t) = A*EXP(B*t) в виде строки.
//СТРУКТУРА СПИСКА Arg ДЛЯ НОРМАЛЬНОЙ РАБОТЫ ФУНКЦИИ
//Количество параметров: 3
//1. Arg.List[0] - double t - текущее время (модельное время)
//2. Arg.List[1] - double A
//3. Arg.List[2] - double B
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. ARGLIST *Arg - список параметров закона изменения ExpLaw
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
// char *s - закон изменения ExpLaw в виде символьной строки
//*****************************************************************************
char* GetMathNotateExpLaw(const ARGLIST *Arg)
{
 char* s = new char[80];//Строка с математической записью закона
 ostrstream text(s, 80);
 try
  {
  	if (Arg == NULL)  //Аргументы не переданы
    throw ARG_MISMATCH;
   if (Arg->Size != 3) //Должно быть передано точно 3 параметра
    throw ARG_MISMATCH;
   text << "F(t) = " << Arg->List[1] << "*EXP(" << Arg->List[2] << "*t)" << ends;
  }
 catch (int error_type)
  {
   if (error_type == ARG_MISMATCH)
   {
    #if defined(CONSOLE)
      cout << "\nMISMATCH ARGUMENT ERROR !!!" << endl;
      cout << "GetMathNotateExpLaw() should accept three parameters: t,A,B.\n";
      cout << "Mathematical notation: F(t) = A*EXP(B*t)." << endl;
      getch();
    #endif
    exit(-1);
   }
  }
return s; //F(t) = A*EXP(B*t)
}
//*****************************************************************************
//СТАТУС: IV.6
//НАИМЕНОВАНИЕ ФУНКЦИИ: char* GetMathNotateCosLaw(const ARGLIST *Arg)
//НАЗНАЧЕНИЕ:
//Функция формирует математическую запись закон изменения величины от времени
//F(t) = A*COS(B*t+C) в виде строки.
//СТРУКТУРА СПИСКА Arg ДЛЯ НОРМАЛЬНОЙ РАБОТЫ ФУНКЦИИ
//Количество параметров: 4
//1. Arg.List[0] - double t - текущее время (модельное время)
//2. Arg.List[1] - double A - амплитуда
//3. Arg.List[2] - double B - угловая частота B = 2*PI*f
//4. Arg.List[3] - double C - начальная фаза
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. ARGLIST *Arg - список параметров закона изменения CosLaw
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
// char *s - закон изменения CosLaw в виде символьной строки
//*****************************************************************************
char* GetMathNotateCosLaw(const ARGLIST *Arg)
{
 char* s = new char[80];//Строка с математической записью закона
 ostrstream text(s, 80);
 try
  {
  	if (Arg == NULL)  //Аргументы не переданы
    throw ARG_MISMATCH;
   if (Arg->Size != 4) //Должно быть передано точно 4 параметра
    throw ARG_MISMATCH;
   text << "F(t) = " << Arg->List[1] << "*COS(" << Arg->List[2] << "*t";
   if (Arg->List[3]>=0) text << "+";
   text << Arg->List[3] << ")" << ends;
  }
 catch (int error_type)
  {
   if (error_type == ARG_MISMATCH)
   {
    #if defined(CONSOLE)
      cout << "\nMISMATCH ARGUMENT ERROR !!!" << endl;
      cout << "GetMathNotateCosLaw() should accept four parameters: t,A,B,C.\n";
      cout << "Mathematical notation: F(t) = A*COS(B*t + C)." << endl;
      getch();
    #endif
    exit(-1);
   }
  }
return s; //F(t) = A*COS(B*t+C)
}
//*****************************************************************************
//СТАТУС: IV.7
//НАИМЕНОВАНИЕ ФУНКЦИИ: char* GetMathNotateSinLaw(const ARGLIST *Arg)
//НАЗНАЧЕНИЕ:
//Функция формирует математическую запись закон изменения величины от времени
//F(t) = A*SIN(B*t+C) в виде строки.
//СТРУКТУРА СПИСКА *Arg ДЛЯ НОРМАЛЬНОЙ РАБОТЫ ФУНКЦИИ
//Количество параметров: 4
//1. Arg.List[0] - double t - текущее время (модельное время)
//2. Arg.List[1] - double A - амплитуда
//3. Arg.List[2] - double B - угловая частота B = 2*PI*f
//4. Arg.List[3] - double C - начальная фаза
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. ARGLIST* Arg - список параметров закона изменения SinLaw
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
// char *s - закон изменения SinLaw в виде символьной строки
//*****************************************************************************
char* GetMathNotateSinLaw(const ARGLIST* Arg)
{
 char* s = new char[80];//Строка с математической записью закона
 ostrstream text(s, 80);
 try
  {
  	if (Arg == NULL)  //Аргументы не переданы
    throw ARG_MISMATCH;
   if (Arg->Size != 4) //Должно быть передано точно 4 параметра
    throw ARG_MISMATCH;
   text << "F(t) = " << Arg->List[1] << "*SIN(" << Arg->List[2] << "*t";
   if (Arg->List[3]>=0) text << "+";
   text << Arg->List[3] << ")" << ends;
  }
 catch (int error_type)
  {
   if (error_type == ARG_MISMATCH)
   {
    #if defined(CONSOLE)
      cout << "\nMISMATCH ARGUMENT ERROR !!!" << endl;
      cout << "GetMathNotateSinLaw() should accept four parameters: t,A,B,C.\n";
      cout << "Mathematical notation: F(t) = A*SIN(B*t + C)." << endl;
      getch();
    #endif
    exit(-1);
   }
  }
return s; //F(t) = A*SIN(B*t+C)
}
//*****************************************************************************
//СТАТУС: IV.8
//НАИМЕНОВАНИЕ ФУНКЦИИ: char* GetMathNotateHyperbolLaw(const ARGLIST *Arg)
//НАЗНАЧЕНИЕ:
//Функция формирует математическую запись закон изменения величины от времени
//F(t) = A/t в виде строки.
//СТРУКТУРА СПИСКА *Arg ДЛЯ НОРМАЛЬНОЙ РАБОТЫ ФУНКЦИИ
//Количество параметров: 2
//1. Arg.List[0] - double t - текущее время (модельное время)
//2. Arg.List[1] - double A - константа
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. ARGLIST* Arg - список параметров закона изменения HyperbolLaw
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
// char *s - закон изменения HyperbolLaw в виде символьной строки
//*****************************************************************************
char* GetMathNotateHyperbolLaw(const ARGLIST* Arg)
{
 char* s = new char[80];//Строка с математической записью закона
 ostrstream text(s, 80);
 try
  {
   if (Arg->Size != 2) //Должно быть передано точно 2 параметра
    throw ARG_MISMATCH;
   text << "F(t) = " << Arg->List[1] << "/t" << ends;
  }
 catch (int error_type)
  {
   if (error_type == ARG_MISMATCH)
   {
    #if defined(CONSOLE)
      cout << "\nMISMATCH ARGUMENT ERROR !!!" << endl;
      cout << "GetMathNotateHyperbolLaw() should accept two parameters: t,A.\n";
      cout << "Mathematical notation: F(t) = A/t." << endl;
      getch();
    #endif
    exit(-1);
   }
  }
return s; //F(t) = A/t
}
//*****************************************************************************
//СТАТУС: IV.9
//НАИМЕНОВАНИЕ ФУНКЦИИ: char* GetMathNotateLnLaw(const ARGLIST *Arg)
//НАЗНАЧЕНИЕ:
//Функция формирует математическую запись закон изменения величины от времени
//F(t) = LN(t) в виде строки.
//СТРУКТУРА СПИСКА *Arg ДЛЯ НОРМАЛЬНОЙ РАБОТЫ ФУНКЦИИ
//Количество параметров: 1
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. ARGLIST* Arg - список параметров закона изменения LnLaw
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
// char *s - закон изменения LnLaw в виде символьной строки
//*****************************************************************************
char* GetMathNotateLnLaw(const ARGLIST *Arg)
{
 char* s = new char[25];
 ostrstream text(s, 25);
 text << "LN(t)" << ends;
 if (Arg != NULL) return s;
 return s; //F(t) = LN(t)
}
//*****************************************************************************
//СТАТУС: IV.10
//НАИМЕНОВАНИЕ ФУНКЦИИ: char* GetMathNotateLog10Law(const ARGLIST *Arg)
//НАЗНАЧЕНИЕ:
//Функция формирует математическую запись закон изменения величины от времени
//F(t) = LOG10(t) в виде строки.
//СТРУКТУРА СПИСКА *Arg ДЛЯ НОРМАЛЬНОЙ РАБОТЫ ФУНКЦИИ
//Количество параметров: 1
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. ARGLIST* Arg - список параметров закона изменения Log10Law
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
// char *s - закон изменения Log10Law в виде символьной строки
//*****************************************************************************
char* GetMathNotateLog10Law(const ARGLIST *Arg)
{
 char* s = new char[25];
 ostrstream text(s, 25);
 text << "LOG10(t)" << ends;
 if (Arg != NULL) return s;
 return s; //F(t) = LOG10(t)
}
//*****************************************************************************
//СТАТУС: IV.11
//НАИМЕНОВАНИЕ ФУНКЦИИ: char* GetMathNotateLogALaw(const ARGLIST *Arg)
//НАЗНАЧЕНИЕ:
//Функция формирует математическую запись закон изменения величины от времени
//F(t) = LogA(t) в виде строки.
//СТРУКТУРА СПИСКА *Arg ДЛЯ НОРМАЛЬНОЙ РАБОТЫ ФУНКЦИИ
//Количество параметров: 2
//1. Arg.List[0] - double t - текущее время (модельное время)
//2. Arg.List[1] - double A - основание логарифма
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. ARGLIST* Arg - список параметров закона изменения LogALaw
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
// char *s - закон изменения LogALaw в виде символьной строки
//*****************************************************************************
char* GetMathNotateLogALaw(const ARGLIST* Arg)
{
 char* s = new char[80];//Строка с математической записью закона
 ostrstream text(s, 80);
 try
  {
   if (Arg->Size != 2) //Должно быть передано точно 2 параметра
    throw ARG_MISMATCH;
   text << "F(t) = Log" << Arg->List[1] << "(t)" << ends;
  }
 catch (int error_type)
  {
   if (error_type == ARG_MISMATCH)
   {
    #if defined(CONSOLE)
      cout << "\nMISMATCH ARGUMENT ERROR !!!" << endl;
      cout << "GetMathNotateLogALaw() should accept two parameters: t,A.\n";
      cout << "A - logarithmic base" << endl;
      cout << "Mathematical notation: F(t) = logA(t)." << endl;
      getch();
    #endif
    exit(-1);
   }
  }
return s; //F(t) = logA(t)
}
//*****************************************************************************
//СТАТУС: IV.12
//НАИМЕНОВАНИЕ ФУНКЦИИ: char* GetMathNotateSqrtLaw(const ARGLIST *Arg)
//НАЗНАЧЕНИЕ:
//Функция формирует математическую запись закон изменения величины от времени
//F(t) = Sqrt(A*t+B) в виде строки.
//СТРУКТУРА СПИСКА *Arg ДЛЯ НОРМАЛЬНОЙ РАБОТЫ ФУНКЦИИ
//Количество параметров: 3
//1. Arg.List[0] - double t - текущее время (модельное время)
//2. Arg.List[1] - double A - константа
//3. Arg.List[2] - double B - константа
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. ARGLIST* Arg - список параметров закона изменения SqrtLaw
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
// char *s - закон изменения SqrtLaw в виде символьной строки
//*****************************************************************************
char* GetMathNotateSqrtLaw(const ARGLIST* Arg)
{
 char* s = new char[80];//Строка с математической записью закона
 ostrstream text(s, 80);
 try
  {
   if (Arg->Size != 3) //Должно быть передано точно 3 параметра
    throw ARG_MISMATCH;
   text << "F(t) = SQRT(" << Arg->List[1] << "*t + " << Arg->List[2]
        << ")" << ends;
  }
 catch (int error_type)
  {
   if (error_type == ARG_MISMATCH)
   {
    #if defined(CONSOLE)
      cout << "\nMISMATCH ARGUMENT ERROR !!!" << endl;
      cout << "GetMathNotateSqrtLaw() should accept three parameters: t,A,B.\n";
      cout << "Mathematical notation: F(t) = SQRT(A*t+B)." << endl;
      getch();
    #endif
    exit(-1);
   }
  }
return s; //F(t) = SQRT(A*t+B)
}
//*****************************************************************************
//СТАТУС: IV.13
//НАИМЕНОВАНИЕ ФУНКЦИИ: char* GetMathNotateRandLaw(const ARGLIST *Arg)
//НАЗНАЧЕНИЕ:
//Функция формирует строку для представления случайного закона:
//RandLaw
//СТРУКТУРА СПИСКА *Arg ДЛЯ НОРМАЛЬНОЙ РАБОТЫ ФУНКЦИИ
//Количество параметров: 1
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. ARGLIST* Arg - список параметров закона изменения RandLaw
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
// char *s - закон изменения RandLaw в виде символьной строки
//*****************************************************************************
char* GetMathNotateRandLaw(const ARGLIST *Arg)
{
 char* s = new char[25];
 ostrstream text(s, 25);
 text << "RANDOM LAW: [0..1]" << ends;
 if (Arg != NULL) return s;
 return s;
}
//*****************************************************************************
//СТАТУС: IV.14
//НАИМЕНОВАНИЕ ФУНКЦИИ: char* GetMathNotateRand2PiLaw(const ARGLIST *Arg)
//НАЗНАЧЕНИЕ:
//Функция формирует строку для представления случайного закона:
//Rand2PiLaw
//СТРУКТУРА СПИСКА *Arg ДЛЯ НОРМАЛЬНОЙ РАБОТЫ ФУНКЦИИ
//Количество параметров: 1
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. ARGLIST* Arg - список параметров закона изменения Rand2PiLaw
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
// char *s - закон изменения Rand2PiLaw в виде символьной строки
//*****************************************************************************
char* GetMathNotateRand2PiLaw(const ARGLIST *Arg)
{
 char* s = new char[25];
 ostrstream text(s, 25);
 text << "RANDOM LAW: [0..2*PI]" << ends;
 if (Arg != NULL) return s;
 return s;
}
//*****************************************************************************
//СТАТУС: IV.15
//НАИМЕНОВАНИЕ ФУНКЦИИ: char* GetMathNotateRandABLaw(const ARGLIST *Arg)
//НАЗНАЧЕНИЕ:
//Функция формирует строку с интервалами изменения равномерно распределенной СВ
//в произвольном интервале [A..B]
//СТРУКТУРА СПИСКА *Arg ДЛЯ НОРМАЛЬНОЙ РАБОТЫ ФУНКЦИИ
//Количество параметров: 3
//1. Arg.List[0] - double x - значение случайной величины в момент времени t
//1. Arg.List[1] - double A - нижняя граница интервала
//2. Arg.List[2] - double B - верхняя граница интервала
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. ARGLIST* Arg - список параметров закона изменения RandABLaw
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
// char *s - интервалы изменения RandABLaw в виде символьной строки
//*****************************************************************************
char* GetMathNotateRandABLaw(const ARGLIST *Arg)
{
 char* s = new char[80];//Строка с математической записью закона
 ostrstream text(s, 80);
 double value;
 try
  {
  	if (Arg == NULL)  //Аргументы не переданы
    throw ARG_MISMATCH;
   if (Arg->Size != 3) //Должно быть передано точно 3 параметра
    throw ARG_MISMATCH;
   //Перестановка A с B, если A > B
   value = Arg->List[1];
   if (value > Arg->List[2])
   { value = Arg->List[2];
     Arg->List[2] = Arg->List[1];
     Arg->List[1] = value;
   }
   text << "RANDOM LAW: [";
   text << Arg->List[1] << ".." << Arg->List[2] << "]" << ends;
  }
 catch (int error_type)
  {
   if (error_type == ARG_MISMATCH)
   {
    #if defined(CONSOLE)
      cout << "\nMISMATCH ARGUMENT ERROR !!!" << endl;
      cout << "GetMathNotateRandABLaw() should accept two parameters: A,B.\n";
      cout << "A - lower border; B - upper border." << endl;
      getch();
    #endif
    exit(-1);
   }
  }
return s; //Равновероятная функция в интервале [A..B]
}
//*****************************************************************************
//СТАТУС: IV.16
//НАИМЕНОВАНИЕ ФУНКЦИИ: char* GetMathNotateRelayLaw(const ARGLIST *Arg)
//НАЗНАЧЕНИЕ:
//Функция формирует строку для представления случайного закона:
//RelayLaw
//СТРУКТУРА СПИСКА *Arg ДЛЯ НОРМАЛЬНОЙ РАБОТЫ ФУНКЦИИ
//Количество параметров: 1
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. ARGLIST* Arg - список параметров закона изменения RelayLaw
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
// char *s - закон изменения RelayLaw в виде символьной строки
//*****************************************************************************
char* GetMathNotateRelayLaw(const ARGLIST *Arg)
{
 char* s = new char[30];
 ostrstream text(s, 30);
 text << "RELAY LAW: [0..+INFINITE]" << ends;
 if (Arg != NULL) return s;
 return s;
}

//*****************************************************************************
//СТАТУС: IV.17
//НАИМЕНОВАНИЕ ФУНКЦИИ: char* GetMathNotateDet1Law(const ARGLIST *Arg)
//НАЗНАЧЕНИЕ:
//Функция формирует строку для представления закона, исключающего влияние СВ,
//которая перемножается с другими величинами.
//СТРУКТУРА СПИСКА *Arg ДЛЯ НОРМАЛЬНОЙ РАБОТЫ ФУНКЦИИ
//Количество параметров: 1
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. ARGLIST* Arg - список параметров закона изменения RandLaw
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
// char *s - закон изменения Det1Law в виде символьной строки
//*****************************************************************************
char* GetMathNotateDet1Law(const ARGLIST *Arg)
{
 char* s = new char[25];
 ostrstream text(s, 25);
 text << "UnCasual law: F(t)=1" << ends;

 if (Arg != NULL) return s;
 return s;
}
//*****************************************************************************
//СТАТУС: IV.18
//НАИМЕНОВАНИЕ ФУНКЦИИ: char* GetMathNotateDet0Law(const ARGLIST *Arg)
//НАЗНАЧЕНИЕ:
//Функция формирует строку для представления закона, исключающего влияние СВ,
//которая складывается с другими величинами.
//СТРУКТУРА СПИСКА *Arg ДЛЯ НОРМАЛЬНОЙ РАБОТЫ ФУНКЦИИ
//Количество параметров: 1
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. ARGLIST* Arg - список параметров закона изменения RandLaw
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
// char *s - закон изменения Det0Law в виде символьной строки
//*****************************************************************************
char* GetMathNotateDet0Law(const ARGLIST *Arg)
{
 char* s = new char[25];
 ostrstream text(s, 25);
 text << "UnCasual law: F(t)=0" << ends;

 if (Arg != NULL) return s;
 return s;
}
//*****************************************************************************
//СТАТУС: IV.19
//НАИМЕНОВАНИЕ ФУНКЦИИ: char* GetMathNotateTableLaw(const ARGLIST *Arg)
//НАЗНАЧЕНИЕ:
//Функция формирует строку табличного закон изменения величины от времени
//Такая функция хранит для каждого момента времени (значения аргумента)
//определенное значение функции. Строка отображает количество хранимых величин.
//СТРУКТУРА СПИСКА *Arg ДЛЯ НОРМАЛЬНОЙ РАБОТЫ ФУНКЦИИ
//Количество параметров: from 1 to N
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. ARGLIST* Arg - список параметров закона изменения TableLaw
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
// char *s - закон изменения TableLaw в виде символьной строки
//*****************************************************************************
char* GetMathNotateTableLaw(const ARGLIST *Arg)
{
 char* s = new char[45];
 ostrstream text(s, 45);
 text << "TABLE LAW: [1..N], N = " << Arg->Size << ends;
 return s;
}
//*****************************************************************************
//СТАТУС: IV.20
//НАИМЕНОВАНИЕ ФУНКЦИИ: char* GetMathNotateSquarePulseLaw(const ARGLIST *Arg)
//НАЗНАЧЕНИЕ:
//Функция формирует строку закона изменения прямоугольного импульса от времени
//Функция для каждого момента времени (значения аргумента) возвращает определен-
//ное значение уровня сигнала - Umax или Umin. Строка отображает параметры пря-
//моугольного импульса.
//СТРУКТУРА СПИСКА *Arg ДЛЯ НОРМАЛЬНОЙ РАБОТЫ ФУНКЦИИ
//Количество параметров: 8.
//1. Arg.List[0] - T - текущее время (модельное время)
//2. Arg.List[1] - t0 - начало следования импульсов с момента времени моделиро-
//   вания - константа (t0 >= 0)
//3. Arg.List[2] - timp - длительность импульса - константа (timp > 0)
//4. Arg.List[3] - Timp - период следования импульсов - константа (Timp > timp)
//5. Arg.List[4] - Umax - уровень сигнала при следовании импульса - константа
//6. Arg.List[5] - Umin - уровень сигнала при отсутствии импульса - константа
//Изменяемые параметры
//7. Arg.List[6] - flag - состояние импульсного сигнала - переменная
//   Возможные значения: -1.0 - импульс отсутствует; 0.0 - состояние ожидания
//   появления первого импульса с момента моделирования; +1.0 - импульс есть.
//8. Arg.List[7] - count - счетчик времени - переменная. Счетчик времени показы-
//   вает остаток времени до моментов 1) появления импульса, 2) отсутствия им-
//   пульса, 3) появления первого импульса. Счетчик времени изменяется дискретно
//   на единицу (-1.0) каждый раз при обращении к функции.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. ARGLIST* Arg - список параметров закона изменения SquarePulseLaw
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
// char *s - закон изменения SquarePulseLaw в виде символьной строки
//*****************************************************************************
char* GetMathNotateSquarePulseLaw(const ARGLIST *Arg)
{
 char* s = new char[80];
 ostrstream text(s, 80);
 text << "SQUARE PULSE: t0 = " << Arg->List[1] << "; timp = " << Arg->List[2]
      << "; Timp = " << Arg->List[3] << "; Umax = " << Arg->List[4]
      << "; Umin = " << Arg->List[5] << ends;
 return s;
}

//******************************************************************
// V. ОПИСАНИЕ ФУНКЦИЙ, РАБОТАЮЩИХ С МАССИВАМИ LawName и MeasureName
//******************************************************************
//*****************************************************************************
//СТАТУС: V.1
//НАИМЕНОВАНИЕ ФУНКЦИИ: const char* GetLawName(uint type, uint language = ENG)
//НАЗНАЧЕНИЕ:
//Функция возвращает наименование закона из массива LawName по его типу type.
//Возможно получение наименования на русском и английском языках в зависимости
//от значения переменной language
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. uint type - идентификатор закона изменения величины
//2. uint language - язык получения наименования закона (ENG - по умолчанию)
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
//const char* ptr - наименование закона по его типу
//*****************************************************************************
const char* GetLawName(uint type, uint language)
{ uint Count;
  const char* ptr;
  if (language != RUS) ptr = LawName[UNEXIST].eng_name;
  else ptr = LawName[UNEXIST].rus_name;
  //Определяем размер массива LawName
  Count = sizeof(LawName)/sizeof(LawName[0]);
  //Поиск наименования закона, соответствующего type
  for (uint i = 1; i < Count; i++)
  { if (LawName[i].type == type) //Строка найдена
    {
     if (language != RUS) ptr = LawName[i].eng_name;
     else ptr = LawName[i].rus_name;
     break; //Преждевременный выход из цикла
    }
  }
return ptr;
}
//*****************************************************************************
//СТАТУС: V.2
//НАИМЕНОВАНИЕ ФУНКЦИИ: const char* GetLawNotate(uint type)
//НАЗНАЧЕНИЕ:
//Функция  возвращает математическую запись закона из массива LawName по его
//типу type.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. uint type - идентификатор закона изменения величины
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
//const char* ptr - математическая запись закона
//*****************************************************************************
const char* GetLawNotate(uint type)
{ uint Count;
  const char* ptr;
  ptr = LawName[UNEXIST].math_notation;
  //Определяем размер массива LawName
  Count = sizeof(LawName)/sizeof(LawName[0]);
  //Поиск наименования закона, соответствующего type
  for (uint i = 1; i < Count; i++)
  { if (LawName[i].type == type) //Строка найдена
    {ptr = LawName[i].math_notation;
     break; //Преждевременный выход из цикла
    }
  }
return ptr;
}
//*****************************************************************************
//СТАТУС: V.3
//НАИМЕНОВАНИЕ ФУНКЦИИ:
//const char* GetMeasureName(uint type, uint language = ENG, bool format = SHORT)
//НАЗНАЧЕНИЕ:
//Функция возвращает наименование размерности величины  или его аббревиатуру из
//массива MeasureName по его типу type.
//Возможно получение наименования на русском и английском языках в зависимости
//от значения переменной language, а также полное или сокращенное наименование
//размерности величины в зависимости от значения format: SHORT - аббревиатура;
//LONG - полное наименование
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. uint type - идентификатор размерности величины
//2. uint language - язык получения наименования закона (ENG - по умолчанию)
//3. bool format - форма записи размерности (полная или сокращенная) (SHORT - по
//умолчанию).
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
//const char* ptr - наименование размерности величины по ее типу
//*****************************************************************************
const char* GetMeasureName(uint type, uint language, bool format)
{ uint Count;
  const char* ptr;
  if (language != RUS) ptr = MeasureName[NONDIM].EngShort;
  else ptr = MeasureName[NONDIM].RusShort;
  //Определяем размер массива MeasureName
  Count = sizeof(MeasureName)/sizeof(MeasureName[0]);
  //Поиск наименования закона, соответствующего type
  for (uint i = 1; i < Count; i++)
  { if (MeasureName[i].Type == type) //Строка найдена
    {
     if (language != RUS)
      if (format != LONGNAME) ptr = MeasureName[i].EngShort;
      else ptr = MeasureName[i].Eng;
     else
      if (format != LONGNAME) ptr = MeasureName[i].RusShort;
      else ptr = MeasureName[i].Rus;
     break; //Преждевременный выход из цикла
    }
  }
return ptr;
}
//*****************************************************************************
//СТАТУС: V.4
//НАИМЕНОВАНИЕ ФУНКЦИИ: uint GetTypeLawByAbbrLaw(string abbr, bool detlaw)
//НАЗНАЧЕНИЕ:
//Функция возвращает  идентификатор закона по его аббревиатуре из массива наиме-
//нований законов LawName. Поиск ведется по полю abbreviate. Если аббревиатура
//закона не найдена возвращается type == UNEXIST. Поиск осуществляется либо сре-
//ди детерминированных законов, либо среди вероятностных законов в зависимости
//от значения флага detlaw.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. string abbr - аббревиатура наименования закона
//2. bool detlaw - флаг определяющий детерминированный или вероятностный закон
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
//uint type - идентификатор закона
//*****************************************************************************
uint GetTypeLawByAbbrLaw(string& abbr, bool detlaw)
{ uint type = UNEXIST;
  uint Count = sizeof(LawName)/sizeof(LawName[0]);
  //Поиск идентификатора закона по его аббревиатуре
  for (uint i = 1; i < Count; i++)
  {
   if (LawName[i].detlaw == detlaw)
    if (abbr.compare(LawName[i].abbreviate) == 0) //Аббревиатура найдена
    { type = LawName[i].type;
      break; //Выход из цикла поиска
    }
  }
return type;
}
//*****************************************************************************
//СТАТУС: V.5
//НАИМЕНОВАНИЕ ФУНКЦИИ: string& GetAbbrLaw(string& s, bool detlaw)
//НАЗНАЧЕНИЕ:
//Функция формирует строку string s c аббревиатурами законов доступными к исполь-
//зованию, которые хранятся в массиве LawName. Каждая аббревиатура отделяется от
//другой символом ';'. В ходе работы осуществляется формирование списка аббре-
//виатур либо детерминированных, либо вероятностных законов в зависимости от
//значения detlaw.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. string& s  - строка в которой будет сформирован список аббревиатур законов
//2. bool detlaw - флаг определяющий включение в список детерминированного или
//                 вероятностного закона
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
//string& s  - список аббревиатур законов
//*****************************************************************************
string& GetAbbrLaw(string& s, bool detlaw)
{
  uint Count = sizeof(LawName)/sizeof(LawName[0]);
  uint pos;
  s.resize(0); //Удаление содержимого строки
  //Формируем список аббревиатур законов с флагом равным detlaw
  for (uint i = 1; i < Count; i++)
   if (LawName[i].detlaw == detlaw)
   { s.append(LawName[i].abbreviate);
     s.append("\t\t");
     s.append(LawName[i].math_notation);
     s.append(";");
   }
   pos = s.find_last_of(";");
   if (pos == string::npos) s = "LIST OF ABBREVIATE LAWS IS EMPTY!";
   else s = s.substr(0,pos); //Удаление последнего ';'
return s;
}
//*****************************************************************************
//СТАТУС: V.6
//НАИМЕНОВАНИЕ ФУНКЦИИ: const char* GetAbbrLawByType(uint type)
//НАЗНАЧЕНИЕ:
//Функция возвращает аббревиатуру закона по ее типу, осуществляя поиск в таблице
//наименований законов LawName
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. uint type - идентификатор закона по которому осуществляется поиск
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
// s  - аббревиатура закона
//*****************************************************************************
const char* GetAbbrLawByType(uint type)
{ const char* s = LawName[0].abbreviate;
  uint Count = sizeof(LawName)/sizeof(LawName[0]);

  for (uint i = 1; i < Count; i++)
   //Идентификатор закона найден
   if (LawName[i].type == type) {s = LawName[i].abbreviate; break; }

return s;
}

