//Image.h
//Copyright (c) JohnSoft 2002. All rights reserved. Borland C++ 5.02.
//Initial date: January,20,2002. Final date:
//Объявление классов по обработке дискретных изображений
#if !defined(__IMAGE_H)
#define __IMAGE_H

//Заголовочные файлы
#include "mvector.h"

const bool N_WHITE = true;//Белый шум
const bool N_COLOR = false;//Небелый шум


//***********************************************************************************
//Предварительное объявление имен классов
//***********************************************************************************
//Объявление имен классов по работе с изображениями
class TImpulseResponse;//Абстрактный оператор импульсного отклика системы
//Оператор импульсного отклика с единичной функцией ядра свертки
class TIR_UnitFunction;
//Оператор импульсного отклика с гауссовой функцией ядра свертки
class TIR_GaussFunction;
//Оператор импульсного отклика с функцией окружности ядра свертки
class TIR_RoundFunction;
//Оператор импульсного отклика с функцией эллипса ядра свертки
class TIR_EllipseFunction;

//Статистические параметры случайных процессов, возмущающих оператор реставрации W,
//при его конструировании методом инверсного синтеза - обращение оператора импульсного
//отклика системы
class TDisturbance;

//-----------------------------------------------------------------------------------
//Идентификаторы функций ядра двумерной свертки
//Аббревиатура: CK_2D_NameFunction
//1) CK - Convolution Kernel (ядро свертки);
//2) 2D - TwoDimensional (свертка двумерная);
//3) NameFunction - конкретное (уникальное) наименование функции ядра свертки
//-----------------------------------------------------------------------------------
const uint CK_2D_UNIT  = 1;//Единичная функция ядра двумерной свертки (x)
const uint CK_2D_GAUSS = 2;//Гауссова функция ядра двумерной свертки  (x)
const uint CK_2D_ROUND = 3;//Круговая функция ядра двумерной свертки  (?)
const uint CK_2D_ELLIPSE = 4;//Эллипcная функция ядра двумерной свертки (?)


//Редактирование свойств оператора двумерной свертки:
//1) Размерность входной выборки HeightX & WidthX (опционально по lEditIn)
//2) Размерность выходной выборки HeightY & WidthY
//3) Выбор функции ядра свертки (Convolution Kernel) и редактирование ее параметров
//Функция при редактировании изменяет свойства оператора H
void Edit_2DConvolutionOperator(TImpulseResponse** pH, bool lEditIn = true);
void Edit_2DConvolutionKernel(TImpulseResponse** pH);
bool Modify_2DConvKernel(TImpulseResponse** pH, const uint type);

//class TGaussDistortionOperator;//Гауссовский оператор искажения
//typedef TGaussDistortionOperator TGaussDistortion;

typedef enum {SLEEP = 1, OPTIMAL, FULL} HOLDING;

using namespace std;
//****************************************************************************************
//               Объявление базового абстрактного класса TImpulseResponse
//      ОПЕРАТОР ИМПУЛЬСНОГО ОТКЛИКА СИСТЕМЫ ОБРАБОТКИ ДИСКРЕТНЫХ ДВУМЕРНЫХ СИГНАЛОВ
//  ФОРМИРОВАНИЕ ОТСЧЕТОВ ДЛЯ ПОСТРОЕНИЯ МАТРИЦЫ ИМПУЛЬСНОГО ОТКЛИКА СИСТЕМЫ ДИСКРЕТНОЙ
//                     ОБРАБОТКИ ДВУМЕРНЫХ ИЗОБРАЖЕНИЙ (СИГНАЛОВ)
//Базовый класс TImpulseResponse используется для формирования операторов искажения H и
//восстановления в виде обратной матрицы invH к матрице искажения H. Класс позволяет рас-
//считывать коэффициенты матрицы импульсного отклика для трех типов систем обработки диск-
//ретных изображений: НЕДООПРЕДЕЛЕННОЙ СИСТЕМЫ, ПЕРЕОПРЕДЕЛЕННОЙ СИСТЕМЫ и ОПРЕДЕЛЕННОЙ
//СИСТЕМЫ.
//Пусть X(M,N) - входное изображение с размерами по вертикали M, по горизонтали - N. Общее
//количество отсчетов P входного изображения X: P = M*N. Пусть Y(K,L) - выходное изображение
//размерностью по вертикали - K, по горизонтали - L. Общее количество отсчетов Q выходного
//изображения: Q = K*L.
//НЕДООПРЕДЕЛЕННАЯ СИСТЕМА: P > Q, количество входных отсчетов больше числа выходных.
//ОПРЕДЕЛЕННАЯ СИСТЕМА: P == Q, количество входных и выходных отсчетов совпадают, при этом
//геометрические размеры входного и выходного изображения могут не совпадать.
//ПЕРЕОПРЕДЕЛЕННАЯ СИСТЕМА: P < Q, количество входных отсчетов меньше числа выходных.
//Для преобразования входного изображения X(M,N) в выходное изображение Y(K,L) необходимо
//сформировать матрицу импульсного отклика H(Q,P) и выполнить следующую операцию:
// y(Q,1) = H(Q,P) * x(P,1), где y(Q,1) и x(P,1) - векторы-столбцы выходного Y и входного
//изображения X соответственно: X(M,N) -> x(P,1) -> H(Q,P) -> y(Q,1) -> Y(K,L)
//Класс рассчитывает значения целочисленных коэффициенты h[i,j] матрицы H, в соответствии
//с которыми формируются коэффициенты матрицы импульсного отклика для двумерной системы.
//Так как матрица импульсного отклика строится для двумерных дискретных сигналов, то элемент
//h[i,j] будут представлять два значения: для i-координаты и для j-координаты. Для входного
//X(3,3) и выходного Y(3,3) изображений значения для h[i,j] могут быть представлены следующей
//таблицей (создается матрица H(9,9)):
//   |           J=0          |           J=1            |           J=2             |
//   | m=0    m=1     m=2     | m=0    m=1     m=2       | m=0    m=1     m=2        |
//---+-------------------------------------------------------------------------------+--
//k=0| h(0,0) h(0,-1) h(0,-2) | h(0,0)  h(0,-1) h(0,-2)  | h(-2,0) h(-2,-1) h(-2,-2) |
//k=1| h(0,1) h(0,0)  h(0,-1) | h(-1,1) h(-1,0) h(-1,-1) | h(-2,1) h(-2,0)  h(-2,-1) |I=0
//k=2| h(0,2) h(0,1)  h(0,0)  | h(-1,2) h(-1,1) h(-1,0)  | h(-2,2) h(-2,0)  h(-2,0)  |
//---|-------------------------------------------------------------------------------|--
//k=0| h(1,0) h(1,-1) h(1,-2) | h(0,0)  h(0,-1) h(0,-2)  | h(-1,0) h(-1,-1) h(-1,-2) |
//k=1| h(1,1) h(1,0)  h(1,-1) | h(0,1)  h(0,0)  h(0,-1)  | h(-1,1) h(-1,0)  h(-1,-1) |I=1
//k=2| h(1,2) h(1,1)  h(1,0)  | h(0,2)  h(0,1)  h(0,0)   | h(-1,2) h(-1,1)  h(-1,0)  |
//---|-------------------------------------------------------------------------------|--
//k=0| h(2,0) h(2,-1) h(2,-2) | h(1,0)  h(1,-1) h(1,-2)  | h(0,0)  h(0,-1)  h(0,-2)  |
//k=1| h(2,1) h(2,0)  h(2,-1) | h(1,1)  h(1,0)  h(1,-1)  | h(0,1)  h(0,0)   h(0,-1)  |I=2
//k=2| h(2,2) h(2,1)  h(2,0)  | h(2,2)  h(1,1)  h(1,0)   | h(0,2)  h(0,1)   h(0,0)   |
//---|-------------------------------------------------------------------------------|--
//Значения i и j h[i,j]-го элемента H вычисляются по формуле: i=I-J; j=k-m.
//Коэффициенты k,I и m,J изменяются в тех же пределах, что и индексы строк и столбцов
//в матрице выходного Y и входного X изображений соответственно. Коэффициенты k,m периоди-
//чески изменяются внутри каждого блока матрицы H от 0 до K и M соответственно.
//Коэффициенты I,J изменяются от 0 до L и N при переходе от блока к блоку матрицы H по вер-
//тикали и горизонтали соответственно. Количество блоков матрицы H равно ее размерности,
//таким образом, индексы I и J однозначно определяют каждый блок матрицы в двумерной системе
//координат. Коэффициенты k и m однозначно определяют каждый элемент блока (I,J) матрицы H.
//Значения индексов i,j h[i,j]-го элемента матрицы H, используются для вычисления значений
//каждого элемента матрицы импульсного отклика.
//Класс является АБСТРАКТНЫМ.
//****************************************************************************************
class TImpulseResponse {
//----------------------------------------------------------------------------------------
// Объявление закрытых членов-данных класса TImpulseResponse
//----------------------------------------------------------------------------------------
private:
 ulong _HeightX; //Количество отсчетов входного изображения по высоте (кол-во строк)
 ulong _WidthX;  //Количество отсчетов входного изображения по ширине (кол-во столбцов)
 ulong _HeightY; //Количество отсчетов выходного изображения по высоте (кол-во строк)
 ulong _WidthY;  //Количество отсчетов выходного изображения по ширине (кол-во столбцов)
 ulong _OriginHX;//Начало отсчета по высоте для входного изображения [0.._HeightX)
 ulong _OriginWX;//Начало отсчета по ширине для входного ихображения [0.._WidthX)
 ulong _OriginHY;//Начало отсчета по высоте для выходного изображения [0.._HeightY)
 ulong _OriginWY;//Начало отсчета по ширине для выходного ихображения [0.._WidthY)

//----------------------------------------------------------------------------------------
// ОБЪЯВЛЕНИЕ КОНСТРУКТОРОВ и ДЕСТРУКТОРА КЛАССА TImpulseResponse
//----------------------------------------------------------------------------------------
public:
 //Конструктор по умолчанию
 TImpulseResponse() {Set(0L,0L,0L,0L); SetDefOrigins();};
 //Конструкторы с аргументами
 TImpulseResponse(ulong hX, ulong wX, ulong hY, ulong wY)
 {Set(hX,wX,hY,wY); SetDefOrigins();};
 TImpulseResponse(ulong hX, ulong wX, ulong hY, ulong wY, ulong hX0, ulong wX0,
                  ulong hY0, ulong wY0)
 {Set(hX,wX,hY,wY); SetOrigins(hX0,wX0,hY0,wY0);};

 TImpulseResponse(ulong hX, ulong wX) {Set(hX,wX,hX,wX); SetDefOrigins();};

 TImpulseResponse(ulong hX, ulong wX, ulong hY, ulong wY, const rvector& Args)
 {Set(hX,wX,hY,wY); SetDefOrigins();};
 TImpulseResponse(ulong hX, ulong wX, ulong hY, ulong wY, ulong hX0, ulong wX0,
                  ulong hY0, ulong wY0, const rvector& Args)
 {Set(hX,wX,hY,wY); SetOrigins(hX0,wX0,hY0,wY0);};

 TImpulseResponse(ulong hX, ulong wX, const rvector& Args)
 {Set(hX,wX,hX,wX); SetDefOrigins();};
 //Объявление конструктора копирования
 TImpulseResponse(const TImpulseResponse &Operator) {*this = Operator;};
 //Объявление деструктора класса
 virtual ~TImpulseResponse() {Reset();};

//---------------------------------------------------------------------------------------
// Объявление общедоступных членов-функций класса TImpulseResponse
//---------------------------------------------------------------------------------------
 //Получение свойств класса
 bool IsReady() const
 {return (_HeightX && _WidthX && _HeightY && _WidthY) ? true : false;};
 ulong GetInHeight() const {return _HeightX;};
 ulong GetInWidth() const {return _WidthX;};
 ulong GetInSize() const {return _HeightX*_WidthX;};
 ulong GetInSize(ulong& hX, ulong& wX) const
 {hX = _HeightX; wX = _WidthX; return _HeightX*_WidthX;};
 ulong GetOutHeight() const {return _HeightY;};
 ulong GetOutWidth() const {return _WidthY;};
 ulong GetOutSize() const {return _HeightY*_WidthY;};
 ulong GetOutSize(ulong& hY, ulong& wY) const
 {hY = _HeightY; wY = _WidthY; return _HeightY*_WidthY;};
 void GetInOutSizes(ulong& hX, ulong& wX, ulong& hY, ulong& wY) const
 {hX = _HeightX; wX = _WidthX; hY = _HeightY; wY = _WidthY;};
 //Координаты начала отсчетов для входного и выходного изображений
 ulong GetInH0() const {return _OriginHX;};
 ulong GetInW0() const {return _OriginWX;};
 void GetInOrigin(ulong& hX0, ulong& wX0 ) const {hX0 = _OriginHX; wX0 = _OriginWX;};
 ulong GetOutH0() const {return _OriginHY;};
 ulong GetOutW0() const {return _OriginWY;};
 void GetOutOrigin(ulong& hY0, ulong& wY0 ) const {hY0 = _OriginHY; wY0 = _OriginWY;};

 virtual rvector& GetConvKernelArgs(rvector& Param) const = 0;//#1
 virtual uint GetTypeConvKernel() const = 0;//#2
 //----------------------------------------------------------------------------------
 // Функции строчного представления свойств класса
 //----------------------------------------------------------------------------------
 //Формирование отсчетов матрицы импульсного отклика H по двум координатам
 bool GetHSamples(imatrix& H_1, imatrix& H_2) const;
 //Формирование отсчетов матрицы импульсного отклика H по двум координатам
 bool GetTrHSamples(imatrix& trH_1, imatrix& trH_2) const;
 //Тип функции импульсного отклика системы (класса)
 virtual string& GetType(string& s) const = 0;//#3
 //Формирование размеров входного и выходного изображения в виде строки
 string& GetHSamplesSize(string& s, uint left_indent = 0) const;
 string& GetInSize(string& s) const;
 string& GetOutSize(string& s) const;
 //Формирование сведений о начале координат для входного и выходного сигналов
 string& GetInOrigin(string& s) const;
 string& GetOutOrigin(string& s) const;
 string& GetOrigins(string& s, uint left_indent = 0) const;
 //Формирование сведений о размере и начале координат для входного и выходного сигналов
 string& GetInSettings(string& s) const;
 string& GetOutSettings(string& s) const;
 //Формирование сведений о функции ядра свертки
 virtual string& GetConvKernel(string& s, uint left_indent = 0) const = 0;//#4
 virtual string& BriefConvKernel(string& s) const = 0;//#5
 //Формирование отсчетов матрицы импульсного отклика по двум координатам в виде строки
 string& GetHSamples(string& s, const string& delim = string(1,' '),
                     uint left_indent = 0) const;//string(' ')
  //Формирование отсчетов транспонированной матрицы импульсного отклика по двум
  //координатам в виде строки
 string& GetTrHSamples(string& s, const string& delim = string(1,' '),
                       uint left_indent = 0) const;//string(' ')
 //Формирование всех свойств класса в виде строки
 string& Info(string& s, string& head = string(), uint left_indent = 0) const;
 string& BriefInfo(string& s, string& head = string(), uint left_indent = 0) const;

 string& Properties(string& s, string& head = string(), uint left_indent = 0) const
 {return BriefInfo(s,head,left_indent);};


 //OVERLOAD THE STREAM INSERTION OPERATOR
 //Перегрузка операции потокового вывода (<<) свойств класса
 friend ostream& operator <<(ostream& out, const TImpulseResponse& Samples)
 {string s; return out << Samples.BriefInfo(s);};
 //---------------------------------------------------------------------------------------
 // Объявление функций консольного редактирования свойств класса
 //---------------------------------------------------------------------------------------
 void EditInSampleSize(uint left_indent = 0);//Редактирование размеров входной выборки
 void EditOutSampleSize(uint left_indent = 0);//Редактирование размеров выходной выборки
 //Редактирование значений начала координат для входной и выходной выборок
 void EditInOrigin(uint left_indent = 0);
 void EditOutOrigin(uint left_indent = 0);
 //Редактирование параметров функции ядра свертки
 virtual void EditConvKernel(uint left_indent = 0) = 0;//#6
 //Редактирование всех свойств объекта класса
 void Edit(bool lEditInSample = true);
 //---------------------------------------------------------------------------------------
 //Установка свойств класса
 bool Set(ulong hX, ulong wX, ulong hY, ulong wY)
 {_HeightX = hX; _WidthX = wX; _HeightY = hY; _WidthY = wY;
  SetDefOrigins(); return IsReady();};
 bool Set(ulong hX, ulong wX, ulong hY, ulong wY, ulong hX0, ulong wX0, ulong hY0, ulong wY0)
 {Set(hX,wX,hY,wY); SetOrigins(hX0,wX0,hY0,wY0); return IsReady();};

 bool Set(ulong hX, ulong wX) {SetDefOrigins(); return Set(hX,wX,hX,wX);};
 bool SetInSize(ulong hX, ulong wX)
 {_HeightX = hX; _WidthX = wX; SetInOrigin(0,0); return IsReady();};
 bool SetOutSize(ulong hY, ulong wY)
 {_HeightY = hY; _WidthY = wY; SetOutOrigin(0,0); return IsReady();};
 //Установка координат начала отсчета для входной и выходной выборок
 void SetDefOrigins() {_OriginHX = _OriginWX = _OriginHY = _OriginWY = 0L;};
 void SetInOrigin(ulong hX0, ulong wX0)
 {_OriginHX = (hX0 < GetInHeight()) ? hX0 : 0L;
  _OriginWX = (wX0 < GetInWidth())  ? wX0 : 0L;
 }
 void SetOutOrigin(ulong hY0, ulong wY0)
 {_OriginHY = (hY0 < GetOutHeight()) ? hY0 : 0L;
  _OriginWY = (wY0 < GetOutWidth())  ? wY0 : 0L;
 }
 void SetOrigins(ulong hX0, ulong wX0, ulong hY0, ulong wY0)
 {SetInOrigin(hX0,wX0); SetOutOrigin(hY0,wY0);};

 virtual bool SetConvKernelArgs(const rvector& Param) = 0;//#7
 virtual bool SetDefConvKernelArgs() = 0;//#8
 virtual bool ResetConvKernelArgs() = 0;//#9
 bool Reset() {Set(0L,0L,0L,0L); SetDefOrigins(); return IsReady();};
 //---------------------------------------------------------------------------------------
 //ПЕРЕГРУЗКА ОПЕРАТОРОВ для класса TImpulseResponse
 //Перегрузка оператора присваивания (=)
 TImpulseResponse* operator =(const TImpulseResponse& H)
 {if (this == &H) return this;
  rvector Args;
  Set(H.GetInHeight(),H.GetInWidth(),H.GetOutHeight(),H.GetOutWidth(),H.GetInH0(),
      H.GetInW0(),H.GetOutH0(),H.GetOutW0());
  SetConvKernelArgs(H.GetConvKernelArgs(Args));
  return this;
 };
 //Перегрузка оператора равенства (==)
 bool operator ==(const TImpulseResponse& H) const
 {return ((GetInHeight() == H.GetInHeight()) && (GetInWidth() == H.GetInWidth()) &&
         (GetOutHeight() == H.GetOutHeight()) && (GetOutWidth() == H.GetOutWidth()))
         ? true : false;
 };
 //Объявление перегруженного оператора вызова функции для построения выходного изображения
 //по заданному входному и функции импульсного отклика системы с заданным ядром
 bool operator() (const rmatrix& X, rmatrix& Y, ulong hY = 0L, ulong wY = 0L);
 bool operator() (const rmatrix& X, rvector& Y, ulong hY = 0L, ulong wY = 0L);

 bool TFunc(const rmatrix& X, rmatrix& Y, ulong hY = 0L, ulong wY = 0L)
 {return (*this)(X,Y,hY,wY);};
 bool TFunc(const rmatrix& X, rvector& Y, ulong hY = 0L, ulong wY = 0L)
 {return (*this)(X,Y,hY,wY);};
 //Формирование матрицы импульсного отклика в матрице H
 bool ImpulseResponse(rmatrix& H) const;
 bool ImpulseResponse(rmatrix& H, double& normH) const;
 bool GetH(rmatrix& H) const {return ImpulseResponse(H);};
 //Частотная характеристика оператора H
 bool FrequencyResponse(rmatrix& H, ulong HCount = 0L, ulong VCount = 0L) const;
 //Формирование транспонированной матрицы импульсного отклика в матрице trH
 bool TrImpulseResponse(rmatrix& trH) const;
 bool GetTrH(rmatrix& trH) const {return TrImpulseResponse(trH);};
 //Формирование оператора обращения матрицы импульсного отклика (оператора двумерной
 //свертки). Оператор обращения двумерной свертки с учетом соотношений размеров входной
 //выборки N и выходной выборки M рассчитывается следующим образом:
 // a) inv(H) - complete system M == N  [inv(*) - оператор обращения]
 // b) inv(trH x H) - overdetermined system M > N
 // c) inv(H x trH) - undetermined system M < N
 bool GetInverseOperator(rmatrix& invH) const;
 bool GetInverseOperator(rmatrix& invH, double& normH) const;
 bool GetInverseOperator(rmatrix& invH, double& normH, double& normA) const;
 //Симметричность оператора обращения матрицы импульсного отклика:
 //Матрица оператора обращения матрицы импульсного отклика симметрична:
 //1. Для ПЕРЕОПРЕДЕЛЕННОЙ и НЕДООПРЕДЕЛЕННОЙ систем всегда, т.к. произведения матриц
 //   trH x H и H x trH - симметрические матрицы и в случае существования обратных
 //   матриц - inv(trH x H) и inv(H x trH) - они также будут симметрическими;
 //2. Для ПОЛНОЙ системы, inv(H) будет симметрической, если H - будет симметрической и
 //   невырожденной матрицей. Симметричность H определяется функцией ядра свертки.
 bool IsInvOpSymmetric() const
 {if (!IsReady()) return false;
  return (GetOutSize() != GetInSize()) ? true : IsConvKernelSymmetric();
 };
 //Формирование оператора реконструкции (инверсного синтеза) искаженного изображения
 //с учетом соотношений размеров входной выборки N и выходной выборки M:
 // a) inv(H) - complete system M == N
 // b) inv(trH x H) x trH - overdetermined system M > N
 // c) trH x inv(H x trH) - undetermined system M < N
 bool GetRecoveryOperator(rmatrix& W) const;
 bool GetRecoveryOperator(rmatrix& W, rmatrix& InvA, double& normH, double& normA) const;

 //---------------------------------------------------------------------------------------
 // Объявление дружественных функций
 //---------------------------------------------------------------------------------------
 //Искажение изображения X подаваемого системой с функцией импульсного отклика H
 //Выходное изображение формируется в матрице Y. Размерности входной выборки X должны
 //быть согласованы с входными размерами H
 friend bool StrictConvolution(const TImpulseResponse& H, const rmatrix& X, rmatrix& Y);
 friend bool StrictConvolution(const TImpulseResponse* H, const rmatrix& X, rmatrix& Y);
 friend bool StrictConvolution(const TImpulseResponse* H, const rmatrix& X, rvector& Y);
 friend bool AdaptConvolution(TImpulseResponse& H, const rmatrix& X, rmatrix& Y);
 //Реставрация искаженного изображения Y методом инверсного синтеза. Реставрированное
 //изображение формируется в матрице EstX.
 friend bool StrictInvRecovery(const TImpulseResponse& H, const rmatrix& Y, rmatrix& EstX);
 friend bool StrictInvRecovery(const TImpulseResponse* H, const rmatrix& Y, rmatrix& EstX);
 friend bool StrictInvRecovery(const TImpulseResponse* H, const rvector& Y, rvector& EstX);
 friend bool AdaptInvRecovery(TImpulseResponse& H, const rmatrix& Y, rmatrix& EstX);

//----------------------------------------------------------------------------------------
//II. Объявление защищенных членов-функций класса TImpulseResponse
//----------------------------------------------------------------------------------------
protected:
 //Ядро свертки (Convolution kernel)
 virtual double ConvolutionKernel(long i, long j) const = 0;//#10
 //Симметричность функции ядра свертки
 virtual bool IsConvKernelSymmetric() const = 0;//#11
 //Построение двумерной свертки для матрицы входного изображения X в матрице Y
 bool Convolute(const rmatrix& X, rmatrix& Y) const;
 //Построение двумерной свертки для матрицы входного изображения X в векторе Y
 bool Convolute(const rmatrix& X, rvector& Y) const;
 //Построение двумерной свертки для вектора входного изображения X в векторе Y
 bool Convolute(const rvector& X, rvector& Y) const;
 //Реставрация искаженного изображения Y методом инверсного синтеза
 bool InvRecovery(const rmatrix& Y, rmatrix& EstX) const;
 bool InvRecovery(const rvector& Y, rvector& EstX) const;
};//Завершение объявления класса TImpulseResponse
//-------------------- Завершение класса TImpulseResponse ---------------------------

//***********************************************************************************
//            Объявление класса TIR_UnitFunction
//   ОПЕРАТОР ИМПУЛЬСНОГО ОТКЛИКА СИСТЕМЫ ОБРАБОТКИ ДИСКРЕТНЫХ ДВУМЕРНЫХ СИГНАЛОВ
//Класс является производным от абстрактного класса TImpulseResponse. В качестве
//ядра свертки используется ЕДИНИЧНАЯ функция (идеальный оператор):
// (i^2 + j^2) <  1 : 1.0
// (i^2 + j^2) >= 1 : 0.0
//***********************************************************************************
class TIR_UnitFunction : public TImpulseResponse {
//-----------------------------------------------------------------------------------
// Объявление закрытых членов-данных класса TIR_UnitFunction
//-----------------------------------------------------------------------------------
//private:
//-----------------------------------------------------------------------------------
// ОБЪЯВЛЕНИЕ КОНСТРУКТОРОВ и ДЕСТРУКТОРА КЛАССА TIR_UnitFunction
//-----------------------------------------------------------------------------------
public:
 //Конструктор по умолчанию
 TIR_UnitFunction() : TImpulseResponse() {SetDefConvKernelArgs();};
 //Конструкторы с аргументами
 TIR_UnitFunction(ulong hX, ulong wX, ulong hY, ulong wY) :
 TImpulseResponse(hX, wX, hY, wY) {SetDefConvKernelArgs();};

 TIR_UnitFunction(ulong hX, ulong wX) : TImpulseResponse(hX, wX) {SetDefConvKernelArgs();};


 TIR_UnitFunction(ulong hX, ulong wX, ulong hY, ulong wY, const rvector& Args) :
 TImpulseResponse(hX, wX, hY, wY, Args) {SetConvKernelArgs(Args);};

 TIR_UnitFunction(ulong hX, ulong wX, const rvector& Args) :
 TImpulseResponse(hX, wX, Args) {SetConvKernelArgs(Args);};
 //Объявление конструктора копирования
 TIR_UnitFunction(const TIR_UnitFunction &Operator) {*this = Operator;};
 //Объявление деструктора класса
 virtual ~TIR_UnitFunction() {ResetConvKernelArgs();};

//-----------------------------------------------------------------------------------
// Объявление общедоступных членов-функций класса TIR_UnitFunction
//-----------------------------------------------------------------------------------
 //Получение параметров функции ядра свертки
 virtual rvector& GetConvKernelArgs(rvector& Param) const
 {Param.Set(1L,ROW,1.0); return Param;};//#1
 virtual uint GetTypeConvKernel() const {return CK_2D_UNIT;};//#2
 //----------------------------------------------------------------------------------
 // Функции строчного представления свойств класса
 //----------------------------------------------------------------------------------
 //Тип оператора импульсного отклика системы (класса)
 virtual string& GetType(string& s) const
 {s.assign("2D-ImpulseResponse with UnitFunction ConvKernel"); return s;};//#3
 //Формирование сведений о функции ядра свертки
 virtual string& GetConvKernel(string& s, uint left_indent = 0) const;//#4
 virtual string& BriefConvKernel(string& s) const {return s.assign("Unit()");};//#5
 //----------------------------------------------------------------------------------
 // Объявление функций консольного редактирования свойств класса
 //----------------------------------------------------------------------------------
 //Редактирование параметров функции ядра свертки
 virtual void EditConvKernel(uint left_indent = 0);//#6
 //----------------------------------------------------------------------------------
 //Установка свойств класса
 //----------------------------------------------------------------------------------
 virtual bool SetConvKernelArgs(const rvector& Param) {return true;};//#7
 virtual bool SetDefConvKernelArgs() {return true;};//#8
 virtual bool ResetConvKernelArgs() {return true;}; //#9

 //----------------------------------------------------------------------------------
 //ПЕРЕГРУЗКА ОПЕРАТОРОВ для класса TIR_UnitFunction
 //----------------------------------------------------------------------------------
 //Перегрузка оператора присваивания (=)
 TIR_UnitFunction* operator =(const TIR_UnitFunction& H)
 {if (this == &H) return this;
  rvector Args;
  Set(H.GetInHeight(),H.GetInWidth(),H.GetOutHeight(),H.GetOutWidth());
  SetConvKernelArgs(H.GetConvKernelArgs(Args));
  return this;
 };
//-----------------------------------------------------------------------------------
//II. Объявление защищенных членов-функций класса TIR_UnitFunction
//-----------------------------------------------------------------------------------
protected:
 //Ядро свертки (Convolution kernel)
 virtual double ConvolutionKernel(long i, long j) const
 {return ((i == 0L) && (j == 0L)) ? 1.0 : 0.0;};//#10
 //Симметричность функции ядра свертки
 virtual bool IsConvKernelSymmetric() const {return true;};//#11
};//Завершение объявления класса TIR_UnitFunction
//-------------------- Завершение класса TIR_UnitFunction ---------------------------

//***********************************************************************************
//            Объявление класса TIR_GaussFunction
//   ОПЕРАТОР ИМПУЛЬСНОГО ОТКЛИКА СИСТЕМЫ ОБРАБОТКИ ДИСКРЕТНЫХ ДВУМЕРНЫХ СИГНАЛОВ
//Класс является производным от абстрактного класса TImpulseResponse. В качестве
//ядра свертки используется ГАУССОВА функция:
//                H(i,j) = EXP(-[t1*Hi(i,j)^2 + t2*Hj(i,j)^2]),
//t1, t2 - параметры функции Гаусса (0 <= t1, t2 < +INF);
//Hi(i,j) - значение первого отсчета двумерной функции Гаусса для (i,j) элемента
//          матрицы импульсного отклика H
//Hj(i,j) - значение второго отсчета двумерной функции Гаусса для (i,j) элемента
//          матрицы импульсного отклика H
//***********************************************************************************
class TIR_GaussFunction : public TImpulseResponse {
//-----------------------------------------------------------------------------------
// Объявление закрытых членов-данных класса TIR_GaussFunction
//-----------------------------------------------------------------------------------
private:
 double _t1;//Параметр гауссовой функции искажения по строкам  (i)
 double _t2;//Параметр гауссовой функции искажения по столбцам (j)
//-----------------------------------------------------------------------------------
// ОБЪЯВЛЕНИЕ КОНСТРУКТОРОВ и ДЕСТРУКТОРА КЛАССА TIR_GaussFunction
//-----------------------------------------------------------------------------------
public:
 //Конструктор по умолчанию
 TIR_GaussFunction() : TImpulseResponse() {SetDefConvKernelArgs();};
 //Конструкторы с аргументами
 TIR_GaussFunction(ulong hX, ulong wX, ulong hY, ulong wY) :
 TImpulseResponse(hX, wX, hY, wY) {SetDefConvKernelArgs();};

 TIR_GaussFunction(ulong hX, ulong wX) : TImpulseResponse(hX, wX) {SetDefConvKernelArgs();};

 TIR_GaussFunction(ulong hX, ulong wX, ulong hY, ulong wY, const rvector& Args) :
 TImpulseResponse(hX, wX, hY, wY, Args) {SetConvKernelArgs(Args);};

 TIR_GaussFunction(ulong hX, ulong wX, const rvector& Args) :
 TImpulseResponse(hX, wX, Args) {SetConvKernelArgs(Args);};
 //Объявление конструктора копирования
 TIR_GaussFunction(const TIR_GaussFunction &Operator) {*this = Operator;};
 //Объявление деструктора класса
 virtual ~TIR_GaussFunction() {ResetConvKernelArgs();};

//-----------------------------------------------------------------------------------
// Объявление общедоступных членов-функций класса TIR_GaussFunction
//-----------------------------------------------------------------------------------
 //Получение параметров функции ядра свертки
 virtual rvector& GetConvKernelArgs(rvector& Param) const
 {Param.Set(2L,ROW,false); Param.Put(0L,_t1); Param.Put(1L,_t2); return Param;};//#1
 virtual uint GetTypeConvKernel() const {return CK_2D_GAUSS;};//#2
 //----------------------------------------------------------------------------------
 // Функции строчного представления свойств класса
 //----------------------------------------------------------------------------------
 //Тип оператора импульсного отклика системы (класса)
 virtual string& GetType(string& s) const
 {s.assign("2D-ImpulseResponse with GaussFunction ConvKernel"); return s;};//#3
 //Формирование сведений о функции ядра свертки
 virtual string& GetConvKernel(string& s, uint left_indent = 0) const;//#4
 virtual string& BriefConvKernel(string& s) const;//#5
 //----------------------------------------------------------------------------------
 // Объявление функций консольного редактирования свойств класса
 //----------------------------------------------------------------------------------
 //Редактирование параметров функции ядра свертки
 virtual void EditConvKernel(uint left_indent = 0);//#6
 //----------------------------------------------------------------------------------
 //Установка свойств класса
 //----------------------------------------------------------------------------------
 //Установка параметров гауссовой функции по данным из вектора типа rvector:
 //_t1 = Param[0]; _t2 = Param[1]
 virtual bool SetConvKernelArgs(const rvector& Param) //#7
 {if (Param.Size() != 2) return false;
  SetConvKernelArgs(Param.Get(0L),Param.Get(1L));
  return true;
 };
 virtual bool SetDefConvKernelArgs() {_t1=1.0; _t2=1.0; return true;};//#8
 virtual bool ResetConvKernelArgs() {_t1 = _t2 = 0.0; return true;}; //#9
 void SetConvKernelArgs(double t1, double t2) {SetT1(t1); SetT2(t2);};
 //----------------------------------------------------------------------------------
 //ПЕРЕГРУЗКА ОПЕРАТОРОВ для класса TIR_GaussFunction
 //----------------------------------------------------------------------------------
 //Перегрузка оператора присваивания (=)
 TIR_GaussFunction* operator =(const TIR_GaussFunction& H)
 {if (this == &H) return this;
  rvector Args;
  Set(H.GetInHeight(),H.GetInWidth(),H.GetOutHeight(),H.GetOutWidth());
  SetConvKernelArgs(H.GetConvKernelArgs(Args));
  return this;
 };
//-----------------------------------------------------------------------------------
//II. Объявление защищенных членов-функций класса TIR_GaussFunction
//-----------------------------------------------------------------------------------
protected:
 //Ядро свертки (Convolution kernel)
 virtual double ConvolutionKernel(long i, long j) const
 {return ((i == 0L) && (j == 0L)) ? 1.0 : exp(-(_t1*i*i + _t2*j*j));};//#10
 //Симметричность функции ядра свертки
 virtual bool IsConvKernelSymmetric() const {return true;};//#11
 //Установка параметров функции ядра двумерной свертки
 void SetT1(double t1) {_t1 = fabs(t1);};
 void SetT2(double t2) {_t2 = fabs(t2);};
};//Завершение объявления класса TIR_GaussFunction
//-------------------- Завершение класса TIR_GaussFunction --------------------------

//***********************************************************************************
//            Объявление класса TIR_RoundFunction
//   ОПЕРАТОР ИМПУЛЬСНОГО ОТКЛИКА СИСТЕМЫ ОБРАБОТКИ ДИСКРЕТНЫХ ДВУМЕРНЫХ СИГНАЛОВ
//Класс является производным от абстрактного класса TImpulseResponse. В качестве
//ядра свертки используется функция ОКРУЖНОСТИ:
//          | 1/(pi*R^2), Hi(i,j)^2 + Hj(i,j)^2 <= R^2;
// H(i,j) = |
//          | 0, Hi(i,j)^2 + Hj(i,j)^2 > R^2,
//R - радиус окружности;
//Hi(i,j) - значение первого отсчета двумерной функции Гаусса для (i,j) элемента
//          матрицы импульсного отклика H
//Hj(i,j) - значение второго отсчета двумерной функции Гаусса для (i,j) элемента
//          матрицы импульсного отклика H
//***********************************************************************************
class TIR_RoundFunction : public TImpulseResponse {
//-----------------------------------------------------------------------------------
// Объявление закрытых членов-данных класса TIR_RoundFunction
//-----------------------------------------------------------------------------------
private:
 double _R;//Радиус окружности (пятна)
 double _Brightness;// 1/(PI*R^2) PI = 3.14159265359
 double _SqR;// _R^2
//-----------------------------------------------------------------------------------
// ОБЪЯВЛЕНИЕ КОНСТРУКТОРОВ и ДЕСТРУКТОРА КЛАССА TIR_RoundFunction
//-----------------------------------------------------------------------------------
public:
 //Конструктор по умолчанию
 TIR_RoundFunction() : TImpulseResponse() {SetDefConvKernelArgs();};
 //Конструкторы с аргументами
 TIR_RoundFunction(ulong hX, ulong wX, ulong hY, ulong wY) :
 TImpulseResponse(hX, wX, hY, wY) {SetDefConvKernelArgs();};

 TIR_RoundFunction(ulong hX, ulong wX) : TImpulseResponse(hX, wX) {SetDefConvKernelArgs();};

 TIR_RoundFunction(ulong hX, ulong wX, ulong hY, ulong wY, const rvector& Args) :
 TImpulseResponse(hX, wX, hY, wY, Args) {SetConvKernelArgs(Args);};

 TIR_RoundFunction(ulong hX, ulong wX, const rvector& Args) :
 TImpulseResponse(hX, wX, Args) {SetConvKernelArgs(Args);};
 //Объявление конструктора копирования
 TIR_RoundFunction(const TIR_RoundFunction &Operator) {*this = Operator;};
 //Объявление деструктора класса
 virtual ~TIR_RoundFunction() {ResetConvKernelArgs();};

//-----------------------------------------------------------------------------------
// Объявление общедоступных членов-функций класса TIR_RoundFunction
//-----------------------------------------------------------------------------------
 //Получение параметров функции ядра свертки
 virtual rvector& GetConvKernelArgs(rvector& Param) const
 {Param.Set(1L,ROW,false); Param.Put(0L,_R); return Param;};//#1
 virtual uint GetTypeConvKernel() const {return CK_2D_ROUND;};//#2
 //----------------------------------------------------------------------------------
 // Функции строчного представления свойств класса
 //----------------------------------------------------------------------------------
 //Тип оператора импульсного отклика системы (класса)
 virtual string& GetType(string& s) const
 {s.assign("2D-ImpulseResponse with RoundFunction ConvKernel"); return s;};//#3
 //Формирование сведений о функции ядра свертки
 virtual string& GetConvKernel(string& s, uint left_indent = 0) const;//#4
 virtual string& BriefConvKernel(string& s) const;//#5
 //----------------------------------------------------------------------------------
 // Объявление функций консольного редактирования свойств класса
 //----------------------------------------------------------------------------------
 //Редактирование параметров функции ядра свертки
 virtual void EditConvKernel(uint left_indent = 0);//#6
 //----------------------------------------------------------------------------------
 //Установка свойств класса
 //----------------------------------------------------------------------------------
 //Установка параметров функции окружности по данным из вектора типа rvector: _R = Param[0]
 virtual bool SetConvKernelArgs(const rvector& Param) //#7
 {if (Param.Size() != 1) return false;
  SetConvKernelArgs(Param.Get(0L));
  return true;
 };
 virtual bool SetDefConvKernelArgs() {SetR(1.0); return true;};//#8
 virtual bool ResetConvKernelArgs()
 {_R = 0.0; _SqR = 0.0; _Brightness = 0.0; return true;}; //#9
 void SetConvKernelArgs(double radius) {SetR(radius);};
 //----------------------------------------------------------------------------------
 //ПЕРЕГРУЗКА ОПЕРАТОРОВ для класса TIR_RoundFunction
 //----------------------------------------------------------------------------------
 //Перегрузка оператора присваивания (=)
 TIR_RoundFunction* operator =(const TIR_RoundFunction& H)
 {if (this == &H) return this;
  rvector Args;
  Set(H.GetInHeight(),H.GetInWidth(),H.GetOutHeight(),H.GetOutWidth());
  SetConvKernelArgs(H.GetConvKernelArgs(Args));
  return this;
 };
//-----------------------------------------------------------------------------------
//II. Объявление защищенных членов-функций класса TIR_RoundFunction
//-----------------------------------------------------------------------------------
protected:
 //Ядро свертки (Convolution kernel)
 virtual double ConvolutionKernel(long i, long j) const
 {return ((i*i + j*j) <= _SqR) ? _Brightness : 0.0;};//#10
 //Симметричность функции ядра свертки
 virtual bool IsConvKernelSymmetric() const {return true;};//#11
 //Установка параметров функции ядра двумерной свертки
 void SetR(double radius)
 {_R = fabs(radius);
  if (_R == 0.0) _R = 1.0;
  _SqR = _R*_R; _Brightness = 1.0/(3.14159265359*_SqR);
 };
};//Завершение объявления класса TIR_RoundFunction
//-------------------- Завершение класса TIR_RoundFunction --------------------------

//***********************************************************************************
//            Объявление класса TIR_EllipseFunction
//   ОПЕРАТОР ИМПУЛЬСНОГО ОТКЛИКА СИСТЕМЫ ОБРАБОТКИ ДИСКРЕТНЫХ ДВУМЕРНЫХ СИГНАЛОВ
//Класс является производным от абстрактного класса TImpulseResponse. В качестве
//ядра свертки используется функция ЭЛЛИПС:
//          | с, Hi(i,j)^2/a^2 + Hj(i,j)^2/b^2 <= 1;
// H(i,j) = |
//          | 0, Hi(i,j)^2/a^2 + Hj(i,j)^2/b^2 > 1,
//a - большая ось эллипса;
//b - малая ось эллипса;
//с - уровень яркости (brightness) внутри эллипса;
//Hi(i,j) - значение первого отсчета двумерной функции Гаусса для (i,j) элемента
//          матрицы импульсного отклика H;
//Hj(i,j) - значение второго отсчета двумерной функции Гаусса для (i,j) элемента
//          матрицы импульсного отклика H.
//***********************************************************************************
class TIR_EllipseFunction : public TImpulseResponse {
//-----------------------------------------------------------------------------------
// Объявление закрытых членов-данных класса TIR_EllipseFunction
//-----------------------------------------------------------------------------------
private:
 double _A;//longer axes (большая полуось эллипса, по ширине) - ось OX
 double _B;//shorter axes (малая полуось эллипса, по высоте) - ось OY
 double _Brightness;//уровень яркости внутри эллипса
 double _SqA;// _A^2
 double _SqB;// _B^2
//-----------------------------------------------------------------------------------
// ОБЪЯВЛЕНИЕ КОНСТРУКТОРОВ и ДЕСТРУКТОРА КЛАССА TIR_EllipseFunction
//-----------------------------------------------------------------------------------
public:
 //Конструктор по умолчанию
 TIR_EllipseFunction() : TImpulseResponse() {SetDefConvKernelArgs();};
 //Конструкторы с аргументами
 TIR_EllipseFunction(ulong hX, ulong wX, ulong hY, ulong wY) :
 TImpulseResponse(hX, wX, hY, wY) {SetDefConvKernelArgs();};

 TIR_EllipseFunction(ulong hX, ulong wX) :
 TImpulseResponse(hX, wX) {SetDefConvKernelArgs();};

 TIR_EllipseFunction(ulong hX, ulong wX, ulong hY, ulong wY, const rvector& Args) :
 TImpulseResponse(hX, wX, hY, wY, Args) {SetConvKernelArgs(Args);};

 TIR_EllipseFunction(ulong hX, ulong wX, const rvector& Args) :
 TImpulseResponse(hX, wX, Args) {SetConvKernelArgs(Args);};
 //Объявление конструктора копирования
 TIR_EllipseFunction(const TIR_EllipseFunction &Operator) {*this = Operator;};
 //Объявление деструктора класса
 virtual ~TIR_EllipseFunction() {ResetConvKernelArgs();};

//-----------------------------------------------------------------------------------
// Объявление общедоступных членов-функций класса TIR_EllipseFunction
//-----------------------------------------------------------------------------------
 //Получение параметров функции ядра свертки
 virtual rvector& GetConvKernelArgs(rvector& Param) const //#1
 {Param.Set(3L,ROW,false);
  Param.Put(0L,_A); Param.Put(1L,_B); Param.Put(2L,_Brightness);
  return Param;
 };
 virtual uint GetTypeConvKernel() const {return CK_2D_ELLIPSE;};//#2
 //----------------------------------------------------------------------------------
 // Функции строчного представления свойств класса
 //----------------------------------------------------------------------------------
 //Тип оператора импульсного отклика системы (класса)
 virtual string& GetType(string& s) const
 {s.assign("2D-ImpulseResponse with EllipseFunction ConvKernel"); return s;};//#3
 //Формирование сведений о функции ядра свертки
 virtual string& GetConvKernel(string& s, uint left_indent = 0) const;//#4
 virtual string& BriefConvKernel(string& s) const;//#5
 //----------------------------------------------------------------------------------
 // Объявление функций консольного редактирования свойств класса
 //----------------------------------------------------------------------------------
 //Редактирование параметров функции ядра свертки
 virtual void EditConvKernel(uint left_indent = 0);//#6
 //----------------------------------------------------------------------------------
 //Установка свойств класса
 //----------------------------------------------------------------------------------
 //Установка параметров функции окружности по данным из вектора типа rvector:
 //_A = Param[0]; _B = Param[1]; _Brightnrss = Param[2];
 virtual bool SetConvKernelArgs(const rvector& Param) //#7
 {if (Param.Size() != 3) return false;
  SetConvKernelArgs(Param.Get(0L),Param.Get(1L),Param.Get(2L));
  return true;
 };
 virtual bool SetDefConvKernelArgs()
 {SetA(1.0); SetB(1.0); SetBrightness(1.0); return true;};//#8
 virtual bool ResetConvKernelArgs() //#9
 {_A = 0.0; _SqA = 0.0; _B = 0.0; _SqB = 0.0; _Brightness = 0.0; return true;};
 void SetConvKernelArgs(double a, double b, double brightness)
 {SetA(a); SetB(b); SetBrightness(brightness);};
 //----------------------------------------------------------------------------------
 //ПЕРЕГРУЗКА ОПЕРАТОРОВ для класса TIR_EllipseFunction
 //----------------------------------------------------------------------------------
 //Перегрузка оператора присваивания (=)
 TIR_EllipseFunction* operator =(const TIR_EllipseFunction& H)
 {if (this == &H) return this;
  rvector Args;
  Set(H.GetInHeight(),H.GetInWidth(),H.GetOutHeight(),H.GetOutWidth());
  SetConvKernelArgs(H.GetConvKernelArgs(Args));
  return this;
 };
//-----------------------------------------------------------------------------------
//II. Объявление защищенных членов-функций класса TIR_EllipseFunction
//-----------------------------------------------------------------------------------
protected:
 //Ядро свертки (Convolution kernel)
 virtual double ConvolutionKernel(long i, long j) const
 {return (((i*i)/_SqA + (j*j)/_SqB) <= 1) ? _Brightness : 0.0;};//#10
 //Симметричность функции ядра свертки
 virtual bool IsConvKernelSymmetric() const {return true;};//#11
 //Установка параметров функции ядра двумерной свертки
 void SetA(double a) {_A = (a == 0.0) ? 1.0 : fabs(a); _SqA = _A*_A;};
 void SetB(double b) {_B = (b == 0.0) ? 1.0 : fabs(b); _SqB = _B*_B;};
 void SetBrightness(double c) {_Brightness = fabs(c);};
};//Завершение объявления класса TIR_EllipseFunction
//------------------- Завершение класса TIR_EllipseFunction -------------------------

//***********************************************************************************
//                 Объявление класса TDisturbance
//Класс описывает статистические свойства внутрисистемных случайных процессов и внут-
//реннего шума, которые имеют место в системах реставрации входного сигнала (изображе-
//ния) по наблюдаемому (выходному) сигналу и известному оператору импульсного отклика
//методом инверсного синтеза. Предполагается, что при построении оператора реставрации
//методом инверсного синтеза (обращение оператора импульсного отклика прямыми методами)
//данный оператор реставрации W возмущается тремя независимыми случайными процессами
//с математическими ожиданиями равными нулю и заданными уровнями дисперсии. Перечислим
//эти источники возмущения:
//1. Внутрисистемная ошибка оценки оператора импульсного отклика системы H.
//   Характеризуется уровнем дисперсии - DispH (DispH >= 0). Случайный процесс приво-
//   дящий к ошибке в оценке оператора H может быть БЕЛЫМ или НЕБЕЛЫМ ШУМОМ.
//   Принадлежность к виду шума определяется - lNoiseH (true - белый шум).
//2. Внутрисистемная ошибка при обращении оператора H (для случев ПЕРЕОПРЕДЕЛЕННОЙ и
//   НЕДООПРЕДЕЛЕННОЙ СИСТЕМ). Характеризуется уровнем дисперсии - DispInvH
//   (DispInvH >= 0). Случайный процесс приводящий к ошибке при обращении оператора H
//   может быть БЕЛЫМ или НЕБЕЛЫМ ШУМОМ. Принадлежность к виду шума определяется -
//   lNoiseInvH (true - белый шум).
//3. Внутренний шум системы (всегда БЕЛЫЙ). Характеризуется уровнем внутреннего шума
//   дисперсией - DispNoise (DispNoise >= 0).
//Класс используется для оценки влияния внутрисистемных ошибок и внутреннего шума на
//качество реставрации входного (исходного) сигнала (изображения) методом инверсного
//синтеза при возмущении оператора реставрации. Возмущение оператора реставрации W
//задается вышеперечисленными случайными процессами.
//Функции класса: хранение, редактирование и передача статистических свойств возмущаю-
//щих процессов для оценки их влияния на качество реставрации.
//***********************************************************************************
class TDisturbance {
//-----------------------------------------------------------------------------------
// Объявление закрытых членов-данных класса TDisturbance
//-----------------------------------------------------------------------------------
private:
 //1. Статистические свойства внутрисистемной ошибки оценки оператора H
 double _DispH;//дисперсия
 bool lNoiseH;//вид шума [белый (true) или небелый (false)]
 //2. Статистические свойства внутрисистемной ошибки при обращении оператора H
 double _DispInvH;//дисперсия
 bool lNoiseInvH;//вид шума [белый (true) или небелый (false)]
 //3. Статистические свойства внутреннего шума
 double _DispNoise;//дисперсия внутреннего шума

//-----------------------------------------------------------------------------------
// ОБЪЯВЛЕНИЕ КОНСТРУКТОРОВ и ДЕСТРУКТОРА КЛАССА TDisturbance
//-----------------------------------------------------------------------------------
public:
 //Конструктор по умолчанию
 TDisturbance() {Set(0.0,false,0.0,false,0.0);};
 //Конструкторы с аргументами
 TDisturbance(double DispH, bool NoiseH, double DispInvH, bool NoiseInvH, double DispNoise)
 {Set(DispH, NoiseH, DispInvH, NoiseInvH, DispNoise);};
 //Объявление деструктора класса
 ~TDisturbance() {Set(0.0,false,0.0,false,0.0);};

//-----------------------------------------------------------------------------------
// Объявление общедоступных членов-функций класса TDisturbance
//-----------------------------------------------------------------------------------
 //Получение свойств класса
 double GetDispH() const {return _DispH;};
 bool GetNoiseH() const {return lNoiseH;};
 double GetDispInvH() const {return _DispInvH;};
 bool GetNoiseInvH() const {return lNoiseInvH;};
 double GetDispNoise() const {return _DispNoise;};

 //----------------------------------------------------------------------------------
 // Функции строчного представления свойств класса TDisturbance
 //----------------------------------------------------------------------------------
 //Статистические свойства внутрисистемной ошибки оценки оператора H
 string& RandProcH(string& s, uint left_indent = 0) const;
 string& BriefRandProcH(string& s) const;
 //Статистические свойства внутрисистемной ошибки при обращении оператора H
 string& RandProcInvH(string& s, uint left_indent = 0) const;
 string& BriefRandProcInvH(string& s) const;
 //Статистические свойства внутреннего шума
 string& RandProcNoise(string& s, uint left_indent = 0) const;
 string& BriefRandProcNoise(string& s) const;

 //Формирование всех свойств класса в виде строки
 string& Info(string& s, bool lInvH = true, string& head = string(),
              uint left_indent = 0) const;
 string& BriefInfo(string& s, bool lInvH = true, bool lNoise = true) const;

 string& Properties(string& s, bool lInvH = true, string& head = string(),
         uint left_indent = 0) const {return Info(s, lInvH, head, left_indent);};

 //OVERLOAD THE STREAM INSERTION OPERATOR
 //Перегрузка операции потокового вывода (<<) свойств класса
 friend ostream& operator <<(ostream& out, const TDisturbance& Disturb)
 {string s; return out << Disturb.Info(s);};
 //----------------------------------------------------------------------------------
 // Объявление функций консольного редактирования свойств класса
 //----------------------------------------------------------------------------------
 //Редактирование cтатистических свойств внутрисистемной ошибки оценки оператора H
 void EditRandProcH(uint left_indent = 0);
 //Редактирование статистических свойств внутрисистемной ошибки при обращении оператора H
 void EditRandProcInvH(uint left_indent = 0);
 //Редактирование статистических свойств внутреннего шума
 void EditRandProcNoise(uint left_indent = 0);
 //Редактирование всех свойств объекта класса
 void Edit();
 //----------------------------------------------------------------------------------
 //Установка свойств класса
 //----------------------------------------------------------------------------------
 //Установка cтатистических свойств внутрисистемной ошибки оценки оператора H
 double SetDispH(double Disp) {_DispH = fabs(Disp); return GetDispH();};
 bool SetNoiseH(bool Noise) {lNoiseH = Noise; return GetNoiseH();};
 void SetRandProcH(double Disp, bool Noise) {SetDispH(Disp); SetNoiseH(Noise);};

 //Установка статистических свойств внутрисистемной ошибки при обращении оператора H
 double SetDispInvH(double Disp) {_DispInvH = fabs(Disp); return GetDispInvH();};
 bool SetNoiseInvH(bool Noise) {lNoiseInvH = Noise; return GetNoiseInvH();};
 void SetRandProcInvH(double Disp, bool Noise) {SetDispInvH(Disp); SetNoiseInvH(Noise);};

 //Установка статистических свойств внутреннего шума
 double SetDispNoise(double Disp) {_DispNoise = fabs(Disp); return GetDispNoise();};
 void SetRandProcNoise(double Disp) {SetDispNoise(Disp);};

 //Установка всех свойств класса
 void Set(double DispH, bool NoiseH, double DispInvH, bool NoiseInvH, double DispNoise)
 {SetRandProcH(DispH,NoiseH);
  SetRandProcInvH(DispInvH,NoiseInvH);
  SetRandProcNoise(DispNoise);
 };

//-----------------------------------------------------------------------------------
//II. Объявление защищенных членов-функций класса TDisturbance
//-----------------------------------------------------------------------------------

};//Завершение объявления класса TDisturbance
//---------------------- Завершение класса TDisturbance -----------------------------

//***********************************************************************************
//                 Объявление класса TRandomProcess
//Класс описывает статистические свойства случайного процесса с математическим ожида-
//нием равным нулю и различными уровнями дисперсии (Disp >= 0). Случайный процесс мо-
//жет быть отнесен к БЕЛОМУ или НЕБЕЛОМУ (ЦВЕТНОМУ) шуму (lNoise).
//Класс используется для оценки влияния внутрисистемных ошибок и внутреннего шума на
//качество реставрации входного (исходного) сигнала (изображения) методом инверсного
//синтеза при возмущении оператора реставрации. Возмущение оператора реставрации W
//задается случайным(и) процессами с мат. ожиданием равным нулю.
//Класс позволяет хранить множество различных значений дисперсий для случайного воз-
//мущающего процесса, что позволяет за одну сессию моделирования просчитать несколько
//вариантов с существенно меньшими временными затратами, чем если бы проводилось моде-
//лирование для одного значения дисперсии за одну сессию.
//Функции класса: хранение, редактирование и передача статистических свойств процесса
//для оценки его влияния на качество реставрации.
//***********************************************************************************
class TRandomProcess {
//-----------------------------------------------------------------------------------
// Объявление закрытых членов-данных класса TRandomProcess
//-----------------------------------------------------------------------------------
private:
 rvector DispSet;//Список дисперсий (dispersion) случайного процесса
 bool lNoise;//Вид шума [белый - N_WHITE(true) или небелый - N_COLOR(false)]
//-----------------------------------------------------------------------------------
// ОБЪЯВЛЕНИЕ КОНСТРУКТОРОВ и ДЕСТРУКТОРА КЛАССА TRandomProcess
//-----------------------------------------------------------------------------------
public:
 //Конструктор по умолчанию
 TRandomProcess() {Set();};
 //Конструкторы с аргументами
 TRandomProcess(const rvector& Disp, bool Noise) {Set(Disp, Noise);};
 //Объявление деструктора класса
 ~TRandomProcess() {Reset();};
//-----------------------------------------------------------------------------------
// Объявление общедоступных членов-функций класса TRandomProcess
//-----------------------------------------------------------------------------------
 //Получение свойств класса
 ulong DispCount() const {return DispSet.Size();};
 double GetDisp(ulong i) const {return (i >= DispCount()) ? 0 : DispSet.Get(i);};
 rvector& GetDisp(rvector& Disp) const {Disp = DispSet; return Disp;};
 bool GetNoise() const {return lNoise;};
 bool IsWhiteNoise() const {return GetNoise();};//lNoise == N_WHITE == true
 //Индикатор наличия в списке дисперсий значения отличного от нуля
 bool IsNonZeroDisp() const
 {return (DispCount() > 1) ? true : (GetDisp(0) != 0.0) ? true : false;}
 //----------------------------------------------------------------------------------
 // Функции строчного представления свойств класса TRandomProcess
 //----------------------------------------------------------------------------------
 //Формирование символьного представления значений дисперсий в строке длиной maxlen
 string& Disp(string& s, uint maxlen = 0, uint prec = 6) const;
 //Формирование наименования типа случайного процесса (WHITE || COLOR)
 string& Noise(string& s, bool brief = true) const;
 //Формирование символьного представления значения i-ой дисперсии
 string& Value(string& s, ulong i, uint precision = 6) const;
 //Формирование строкового представления значения i-ой дисперсии и типа шума
 string& ValueInfo(string& s, ulong i, bool brief = true, uint precision = 6) const;
 //Формирование свойств класса в виде строки
 string& Info(string& s, uint maxlen = 0, uint precision = 6) const;
 //OVERLOAD THE STREAM INSERTION OPERATOR
 //Перегрузка операции потокового вывода (<<) свойств класса
 friend ostream& operator <<(ostream& out, const TRandomProcess& Random)
 {string s; return out << Random.Info(s);};
 //----------------------------------------------------------------------------------
 // Объявление функций консольного редактирования свойств класса
 //----------------------------------------------------------------------------------
 //Редактирование значений дисперсии
 void EditDisp(uint left_indent = 0);
 //Редактирование вида шума
 void EditNoise(uint left_indent = 0);
 //Редактирование всех свойств объекта класса
 void Edit(string& head = string(), bool lDispEdit = true, bool lNoiseEdit = true);
 //----------------------------------------------------------------------------------
 //Установка свойств класса
 //----------------------------------------------------------------------------------
 //Установка значений дисперсии случайного процесса
 ulong SetDisp(const rvector& Disp);
 //Установка вида случайного процесса
 bool SetNoise(bool Noise) {return lNoise = Noise;};
 //Установка значений дисперсии и вида случайного процесса
 ulong Set() {DispSet.Set(1,ROW,0.0); lNoise = N_WHITE; return DispCount();};
 ulong Set(const rvector& Disp, bool Noise) {SetNoise(Noise); return SetDisp(Disp);};

//-----------------------------------------------------------------------------------
//II. Объявление защищенных членов-функций класса TRandomProcess
//-----------------------------------------------------------------------------------
protected:
 void Reset() {DispSet.Resize(0L); lNoise = N_WHITE;};

};//Завершение объявления класса TRandomProcess
//---------------------- Завершение класса TRandomProcess ---------------------------

//***********************************************************************************
//                 Объявление класса TRecoveryDisturbance
//Класс описывает статистические свойства внутрисистемных случайных процессов и внут-
//реннего шума, которые имеют место в системах реставрации входного сигнала (изображе-
//ния) по наблюдаемому (выходному) сигналу и известному оператору импульсного отклика
//методом инверсного синтеза. Предполагается, что при построении оператора реставрации
//методом инверсного синтеза (обращение оператора импульсного отклика прямыми методами)
//данный оператор реставрации W возмущается тремя независимыми случайными процессами
//с математическими ожиданиями равными нулю и заданными уровнями дисперсии. Каждый
//случайный процесс описывается классом TRandomProcess, который позволяет задавать
//список уровней дисперсии для каждого случайного процесса и классифицировать случай-
//ный процесс как БЕЛЫЙ (WHITE) или НЕБЕЛЫЙ (COLOR) шум. Источники возмущения:
//1. Внутрисистемная ошибка оценки оператора ErrH импульсного отклика системы H.
//2. Внутрисистемная ошибка ErrInvH при обращении оператора H (для случев ПЕРЕОПРЕДЕ-
//   ЛЕННОЙ и НЕДООПРЕДЕЛЕННОЙ СИСТЕМ).
//3. Внутренний шум ErrNoise системы (всегда БЕЛЫЙ).
//Класс используется для оценки влияния внутрисистемных ошибок и внутреннего шума на
//качество реставрации входного (исходного) сигнала (изображения) методом инверсного
//синтеза при возмущении оператора реставрации. Возмущение оператора реставрации W
//задается вышеперечисленными случайными процессами.
//Функции класса: хранение, редактирование и передача статистических свойств возмущаю-
//щих процессов для оценки их влияния на качество реставрации.
//***********************************************************************************
class TRecoveryDisturbance {
//-----------------------------------------------------------------------------------
// Объявление закрытых членов-данных класса TRecoveryDisturbance
//-----------------------------------------------------------------------------------
private:
 //1. Статистические свойства внутрисистемной ошибки оценки оператора H
 TRandomProcess _ErrH;
 //2. Статистические свойства внутрисистемной ошибки при обращении оператора H
 TRandomProcess _ErrInvH;
 //3. Статистические свойства внутреннего шума
 TRandomProcess _Noise;
//-----------------------------------------------------------------------------------
// ОБЪЯВЛЕНИЕ КОНСТРУКТОРОВ и ДЕСТРУКТОРА КЛАССА TRecoveryDisturbance
//-----------------------------------------------------------------------------------
public:
 //Конструктор по умолчанию
 TRecoveryDisturbance() {Set();};
 //Конструкторы с аргументами
 TRecoveryDisturbance(const rvector& DispH, bool NoiseH, const rvector& DispInvH,
                      bool NoiseInvH, const rvector& DispNoise)
                      {Set(DispH, NoiseH, DispInvH, NoiseInvH, DispNoise);};
 //Объявление деструктора класса
 ~TRecoveryDisturbance() {};

//-----------------------------------------------------------------------------------
// Объявление общедоступных членов-функций класса TRecoveryDisturbance
//-----------------------------------------------------------------------------------
 //Получение свойств класса
 ulong DispErrH() const {return _ErrH.DispCount();};
 ulong DispErrInvH() const {return _ErrInvH.DispCount();};
 ulong DispNoise() const {return _Noise.DispCount();};

 double DispErrH(ulong i) const {return _ErrH.GetDisp(i);};
 double DispErrInvH(ulong i) const {return _ErrInvH.GetDisp(i);};
 double DispNoise(ulong i) const {return _Noise.GetDisp(i);};

 bool NoiseErrH() const {return _ErrH.GetNoise();};
 bool NoiseErrInvH() const {return _ErrInvH.GetNoise();};
 bool Noise() const {return _Noise.GetNoise();};

 bool IsWhiteErrH() const {return _ErrH.IsWhiteNoise();};
 bool IsWhiteErrInvH() const {return _ErrInvH.IsWhiteNoise();};
 bool IsWhiteNoise() const {return _Noise.IsWhiteNoise();};

 bool IsNonZeroErrH() const {return _ErrH.IsNonZeroDisp();};
 bool IsNonZeroErrInvH() const {return _ErrInvH.IsNonZeroDisp();};
 bool IsNonZeroNoise() const {return _Noise.IsNonZeroDisp();};

 //----------------------------------------------------------------------------------
 // Функции строчного представления свойств класса TRecoveryDisturbance
 //----------------------------------------------------------------------------------
 //Статистические свойства внутрисистемной ошибки оценки оператора H
 string& DispErrH(string& s, uint maxlen = 0, uint precision = 6) const
         {return _ErrH.Disp(s,maxlen,precision);};
 string& NoiseErrH(string& s, bool brief = true) const
         {return _ErrH.Noise(s,brief);};
 string& InfoErrH(string& s, uint maxlen = 0, uint precision = 6) const
         {return _ErrH.Info(s,maxlen,precision);};

 //Статистические свойства внутрисистемной ошибки при обращении оператора H
 string& DispErrInvH(string& s, uint maxlen = 0, uint precision = 6) const
         {return _ErrInvH.Disp(s,maxlen,precision);};
 string& NoiseErrInvH(string& s, bool brief = true) const
         {return _ErrInvH.Noise(s,brief);};
 string& InfoInvErrH(string& s, uint maxlen = 0, uint precision = 6) const
         {return _ErrInvH.Info(s,maxlen,precision);};

 //Статистические свойства внутреннего шума
 string& DispNoise(string& s, uint maxlen = 0, uint precision = 6) const
         {return _Noise.Disp(s,maxlen,precision);};
 string& Noise(string& s, bool brief = true) const
         {return _Noise.Noise(s,brief);};
 string& InfoNoise(string& s, uint maxlen = 0, uint precision = 6) const
         {return _Noise.Info(s,maxlen,precision);};

 //Формирование всех свойств класса в виде строки
 string& Info(string& s, bool lErrH = true, bool lErrInvH = true, bool lNoise = true,
              uint maxlen = 0, uint precision = 6) const;
 //OVERLOAD THE STREAM INSERTION OPERATOR
 //Перегрузка операции потокового вывода (<<) свойств класса
 friend ostream& operator <<(ostream& out, const TRecoveryDisturbance& Disturb)
 {string s; return out << Disturb.Info(s);};

 //----------------------------------------------------------------------------------
 // Объявление функций консольного редактирования свойств класса
 //----------------------------------------------------------------------------------
 //Редактирование всех свойств объекта класса
 void Edit(bool lErrH = true, bool lErrInvH = true, bool lNoise = true);
 //----------------------------------------------------------------------------------
 //Установка свойств класса
 //----------------------------------------------------------------------------------
 //Установка cтатистических свойств внутрисистемной ошибки оценки оператора H
 ulong SetErrH(const rvector& Disp, bool Noise) {return _ErrH.Set(Disp,Noise);};

 //Установка статистических свойств внутрисистемной ошибки при обращении оператора H
 ulong SetErrInvH(const rvector& Disp, bool Noise) {return _ErrInvH.Set(Disp,Noise);};

 //Установка статистических свойств внутреннего шума
 ulong SetNoise(const rvector& Disp) {return _Noise.Set(Disp,N_WHITE);};

 //Установка всех свойств класса
 void Set(const rvector& DispH, bool NoiseH, const rvector& DispInvH, bool NoiseInvH,
          const rvector& DispNoise)
 {SetErrH(DispH,NoiseH);
  SetErrInvH(DispInvH,NoiseInvH);
  SetNoise(DispNoise);
 };
 void Set() {_ErrH.Set(); _ErrInvH.Set(); _Noise.Set();};

//-----------------------------------------------------------------------------------
//II. Объявление защищенных членов-функций класса TRecoveryDisturbance
//-----------------------------------------------------------------------------------


};//Завершение объявления класса TRecoveryDisturbance
//------------------ Завершение класса TRecoveryDisturbance -------------------------


//***********************************************************************************
// Объявление класса TGaussDistortionOperator - ГАУССОВ ОПЕРАТОР ИСКАЖЕНИЯ
//Класс формирует гауссовский оператор искажения в виде квадратной матрицы H и
//оператор восстановления в виде обратной матрицы invH к матрице искажения H.
//Класс предназначен для аналитического моделирования деформации дискретных
//изображений при возмущении оператора восстановления. При размерности матрицы
//идеального изображения X(K x M), размерность искажающего оператора H(N x N),
//где N = K*M. Каждый элемент матрицы H формируется по следующему закону:
//                H(i,j) = EXP(-[t1*(k-i)^2 + t2*(m-j)^2]),
//t1, t2 - параметры гауссового оператора искажения (0 <= t1, t2 < +INF);
//k, m - индексы матрицы X (0 <= k < K; 0 <= m < M).
//Матрица H - симметрическая, положительно определенная с единичной главной
//диагональю. Матрица invH - обратная матрица к матрице H, используется для
//восстановления искаженного изображения.
//H(i-k,j-m) - массив импульсного отклика изображающей системы, вносящей нерезкость.
//         АЛГОРИТМ  ПОСТРОЕНИЯ  ГАУССОВОГО ОПЕРАТОРА ИСКАЖЕНИЯ H
//Чтобы исказить входное изображение, представленное, например, матрицей A(3x3),
//необходимо создать матрицу H размерностью (9x9) следующей структуры:
//   |           J=0          |           J=1            |           J=2             |
//   | m=0    m=1     m=2     | m=0    m=1     m=2       | m=0    m=1     m=2        |
//---+-------------------------------------------------------------------------------+--
//k=0| h(0,0) h(0,-1) h(0,-2) | h(0,0)  h(0,-1) h(0,-2)  | h(-2,0) h(-2,-1) h(-2,-2) |
//k=1| h(0,1) h(0,0)  h(0,-1) | h(-1,1) h(-1,0) h(-1,-1) | h(-2,1) h(-2,0)  h(-2,-1) |I=0
//k=2| h(0,2) h(0,1)  h(0,0)  | h(-1,2) h(-1,1) h(-1,0)  | h(-2,2) h(-2,0)  h(-2,0)  |
//---|-------------------------------------------------------------------------------|--
//k=0| h(1,0) h(1,-1) h(1,-2) | h(0,0)  h(0,-1) h(0,-2)  | h(-1,0) h(-1,-1) h(-1,-2) |
//k=1| h(1,1) h(1,0)  h(1,-1) | h(0,1)  h(0,0)  h(0,-1)  | h(-1,1) h(-1,0)  h(-1,-1) |I=1
//k=2| h(1,2) h(1,1)  h(1,0)  | h(0,2)  h(0,1)  h(0,0)   | h(-1,2) h(-1,1)  h(-1,0)  |
//---|-------------------------------------------------------------------------------|--
//k=0| h(2,0) h(2,-1) h(2,-2) | h(1,0)  h(1,-1) h(1,-2)  | h(0,0)  h(0,-1)  h(0,-2)  |
//k=1| h(2,1) h(2,0)  h(2,-1) | h(1,1)  h(1,0)  h(1,-1)  | h(0,1)  h(0,0)   h(0,-1)  |I=2
//k=2| h(2,2) h(2,1)  h(2,0)  | h(2,2)  h(1,1)  h(1,0)   | h(0,2)  h(0,1)   h(0,0)   |
//---|-------------------------------------------------------------------------------|--
//Значения i и j h(i,j)-го элемента H вычисляются по формуле: i=I-J; j=k-m.
//Коэффициенты k,I и m,J изменяются в тех же пределах, что и индексы строк и столбцов
//в матрице исходного изображения A. Коэффициенты k,m периодически изменяются внутри
//каждого блока матрицы H от 0 до K и M соответственно. Коэффициенты I,J изменяются
//от 0 до K и M при переходе от блока к блоку матрицы H по вертикали и горизонтали
//соответственно. Количество блоков матрицы H равно ее размерности, таким образом,
//индексы I и J однозначно определяют каждый блок матрицы в двухмерной системе координат.
//Коэффициенты k и m однозначно определяют каждый элемент блока (I,J) матрицы H.
//Значения индексов i,j h(i,j)-го элемента матрицы H, используются для вычисления значения
//каждого элемента оператора искажения по вышеупомянутой формуле:
// H(i,j) = EXP(-[t1*(I-J)^2 + t2*(k-m)^2]).
//    ОПЕРАЦИЯ ИСКАЖЕНИЯ ИСХОДНОГО (ВХОДНОГО) ИЗОБРАЖЕНИЯ ПО НОРМАЛЬНОМУ ЗАКОНУ
//Для реализации операции искажения изображения, представленного в виде прямоугольной
//матрицы A(K x M) необходимо выполнить следующие шаги:
//1) Построить гауссов оператор искажения H размерности N = K*M c параметрами t1, t2
//   по вышеописанному методу.
//2) Преобразовать матрицу A в вектор-строку (вектор-столбец) X размером N=K*M.
//   Например, для A(5x3) в вектор-строку X(15):
//    | 1 1 1 |         i=0  |  i=1 | i=2 |  i=3 |  i=4
//    | 1 0 0 |       -----------------------------------
//A = | 1 0 0 | --> X |1 1 1  1 0 0  1 0 0  1 0 0  1 1 1|
//    | 1 0 0 |       -----------------------------------
//    | 1 1 1 |
//
//3) Вектор искаженного изображения Y получается операцией умножения справа оператора
//   искажения H на исходное изображение X: Y = X*H.
//4) Для получения матрицы искаженного изображения B необходимо преобразовать вектор-строку
//   искаженного изображения Y в матрицу с размерностью аналогичной матрице исходного
//   изображения A.
//     РЕСТАВРАЦИЯ ИСКАЖЕННОГО ИЗОБРАЖЕНИЯ ПО ИЗВЕСТНОМУ ОПЕРАТОРУ ИСКАЖЕНИЯ H
//1) Вычисляем обратную матрицу invH к матрице оператора искажения H.
//2) Преобразуем матрицу искаженного изображения B в вектор-строку (вектор-столбец) Y
//   по вышеописанной в пункте 2 методике.
//3) Восстанавливаем исходный вектор изображения X операцией: X = Y*invH.
//4) Преобразуем вектор-строку реставрированного изображения X в матрицу исходного
//   изображения A размерности матрицы искаженного изображения B.
//-----------------------------------------------------------------------------------
//ЗАМЕЧАНИЯ:
//1. Вычисление обратного оператора искажения (оператора восстановления) является
//факультативной функцией для данного класса. С целью экономии памяти и времени работы
//программы пользователь может сам определять необходимость вычисления invH как при
//начальном формировании оператора искажения H, так и при последующих его модификациях
//в ходе моделирования.
//2. При поступлении на вход объекта матрицы дискретного изображения класс автоматически
//формирует оператор искажения H (восстановления invH) согласно размерности входной
//матрицы для выполнения операции искажения (восстановления) над ней. Пользователь
//самостоятельно обязан определить какую операцию (искажения или восстановления) над
//входной матрицей изображения он хочет осуществить, т.е. определиться является ли
//матрица исходным или искаженным изображением.
//3. При конструировании объекта TGaussDistortionOperator по умолчанию создаются
//матрицы H и invH нулевого размера, параметры t1 и t2 равны 0,
//режим хранения Mode = SLEEP.
//4. После обработки событий, связанных с формированием матриц H и invH, дальнейшее
//их существование в полном объеме зависит от режима работы объекта, который определяется
//значением Mode. Режим Mode задается пользователем и влияет на скорость обработки
//потока событий, связанных с матрицами H и invH, а также на объем памяти, который
//объект класса TGaussDistortionOperator занимает в памяти.
//        ОПИСАНИЕ ЗАКРЫТЫХ ЧЛЕНОВ-ДАННЫХ КЛАССА TGaussDistortionOperator
//1. t1, t2 - параметры гауссового оператора искажения : t1, t2 >= 0;
//2. H - квадратная симметрическая положительно-определенная матрица оператора искажения;
//3. invH - матрица оператора восстановления (матрица обратная матрице H);
//4. Height, Width - высота и ширина матрицы изображения для которого строятся
//   матрицы операторов искажения и восстановления - H и invH. При Height==Width==0
//   операторы не созданы (объект класса не активен);
//5. Mode - режим хранения матриц операторов искажения и восстановления:
//   SLEEP - матрицы H и invH нулевого размера, создаются только в момент обработки
//   событий, связанных с ними, после обработки события они удаляются из памяти.
//   Преимущества: объект занимает минимально возможный размер в памяти.
//   Недостатки: необходимость создания операторов искажения и восстановления каждый
//   раз при возникновении связанного с ними события, может увеличиться время работы,
//   если требуется исказить (восстановить) последовательность изображений одинакового
//   размера при фиксированных параметрах t1, t2.
//   OPTIMAL - всегда существует матрица H, матрица invH - нулевого размера и создается
//   только при появлении события, связанного с ней, после обработки события матрица
//   invH удаляется из памяти.
//   Преимущества: повышает быстродействие обработки событий, связанных с искажением
//   последовательности изображений одного размера при фиксированных параметрах t1, t2
//   по сравнению с режимом SLEEP.
//   Недостатки: объект занимает в памяти больше места, чем в режиме SLEEP, за счет
//   постоянного хранения в памяти матрицы H.
//   FULL - матрицы H и invH хранятся в полном объеме все время. Размерность матриц
//   меняется синхронно в зависимости от размерности изображения, которое необходимо
//   обработать. После обработки события, связанного с созданием либо матрицы H либо
//   матрицы invH, они хранятся в памяти в полном размере.
//   Преимущества: повышает быстродействие обработки событий, связанных с искажением
//   и одновременным восстановлением последовательности изображений одного размера при
//   фиксированных параметрах t1, t2 по сравнению с режимом OPTIMAL.
//   Недостатки: объект занимает максимально возможный размер в памяти; снижает
//   быстродействие обработки событий, связанных только с искажением последовательности
//   изображений разной размерности, т.к. каждый раз требуется создавать и матрицу invH
//   хотя в ее использовании нет необходимости.
//
//***********************************************************************************

//--------------------- The end of file "image.h" ---------------------------
#endif //ЗАВЕРШЕНИЕ ОБЪЯВЛЕНИЯ ЗАГОЛОВОЧНОГО ФАЙЛА "IMAGE.H"