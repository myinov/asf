//matrix.h
//Объявление классов TComplexMatrix & TRealMatrix
//Copyright (c) JohnSoft 1999. All rights reserved.
//Initial date: February,13,1999; Final date:
//Файл содержит классы, который осуществляет хранение матриц, обращение к ее компонен-
//там и основные операции, которые могут выполняться над матрицами с комплексными и
//действительными числами. Каждый компонент комплексной матрицы представлены числом
//типа complex (16 байт) - реальная часть число типа double (8 байт) и мнимая часть
//число типа double (8 байт). Каждый компонент действительной матрицы представлен чис-
//лом типа double. Допустимый диапазон, представления элементов матрицы находится в
//пределах от 1.7x10-308 до 1.7x10+308, максимальное число значащих цифр компоненты -
//19 цифр.
//Максимальная допускаемая размерность матриц 4,294,967,296 строк на 4,294,967,296
//столбцов.
//С помощью классов TComplexMatrix & TRealMatrix, кроме матриц, можно представлять
//комплексные и действительные вектор-столбец и вектор-строку.
//За индекс первой строки и первого столбца матрицы принят 0 (ноль).


#if !defined(__MATRIX_H)
#define __MATRIX_H

#include "const.h"
#pragma hdrstop

#pragma warn -inl      //IGNORE not expanded inline WARNINGS.

class TComplexMatrix;
typedef TComplexMatrix cmatrix;

class TRealMatrix;
typedef TRealMatrix rmatrix;

class TIntMatrix;
typedef TIntMatrix imatrix;

#define RUS 1 //русский язык
#define ENG 2 //английский язык

//const bool LEFT  = false;
//const bool RIGHT = true;

#define SQUARE  true  //Квадратная матрица
#define RECTANG false //Прямоугольная матрица


#if !defined(__MVECTOR_H)
 #include "mvector.h"
#endif //__MVECTOR_H


#if !defined(__MATHLIB_H)
 #include <mathlib.h>
#endif //__MATHLIB.H


//***********************************************************************************
//Объявление структуры наименования типа матрицы - MATRIX_NAME
//***********************************************************************************
typedef struct name
{ uint type;      //тип матрицы (вектора)
  char *rus_name; //русскоязычное наименование матрицы (вектора)
  char *eng_name; //англоязычное наименование матрицы (вектора)
} MATRIX_NAME;

//***********************************************************************************
//Объявление структуры характеристик матрицы - PF_MATRIX
//***********************************************************************************
typedef struct info
{ bool type; //тип матрицы (квадратная или прямоугольная)
  uint subtype; //подтип матрицы (расширенный тип матрицы)
  ulong rows;//количество строк матрицы
  ulong cols;//количество столбцов матрицы
} PF_MATRIX;

typedef PF_MATRIX* pPF_MATRIX; //Указатель на структуру PF_MATRIX

//СПИСОК ОШИБОК, ВОЗНИКАЮЩИХ ПРИ РАБОТЕ С МАТРИЦАМИ
const int M_SIZE_ERROR     = -1; //Несоответствие размерности матрицы
const int M_TYPE_ERROR     = -2; //Несоответствие типа матрицы
const int M_ZERO_ERROR     = -3; //Обращение к матрице нулевого размера
const int M_ACCESS_ERROR   = -4; //Обращение к несуществующим элементам матрицы
const int M_DIVIDE_BY_ZERO = -5; //Попытка деления на ноль
const int M_SIZE_MISMATCH  = -6; //Несоответствие размерностей 2-х матриц
const int M_TYPE_MISMATCH  = -7; //Несоответствие типов 2-х матриц
const int M_UNKNOWN_TYPE   = -8; //Неизвестный тип матрицы
const int M_DEGENERACY     = -9; //Вырожденная матрица (det A = 0)

//-----------------------------------------------------------------------------------
//        ОБЪЯВЛЕНИЕ КОНСТАНТ, СТРУКТУР и КЛАССА TRealMatrix & TIntMatrix
//-----------------------------------------------------------------------------------
//КЛАССИФИКАЦИЯ ДЕЙСТВИТЕЛЬНЫХ МАТРИЦ
//Возможные значения принимаемые переменной Type класса rmatrix
const uint RM_ZEROSIZE    = 0; //Матрица нулевого размера - 0 x 0
const uint RM_ZERO        = 1;  //Матрица с нулевыми элементами

const uint RM_NUMBER      = 2; //Матрица размером 1 x 1

const uint RV_ROW         = 3;//Действительная вектор-строка
const uint RV_COLUMN      = 4;//Действительный вектор-столбец

const uint RM_RECTANGULAR = 5; //Прямоугольная матрица
const uint RM_RECTANG     = 5; //Прямоугольная матрица
const uint RM_RECT        = 5; //Прямоугольная матрица

const uint RM_SQUARE      = 6; //Квадратная матрица
const uint RM_SQ          = 6; //Квадратная матрица

//**************** Подкласс квадратных матриц ****************************
//1. КВАДРАТНЫЕ ДИАГОНАЛЬНЫЕ МАТРИЦЫ
const uint RM_DIAGONAL    = 7; //Диагональная матрица


//****************** Подкласс диагональных матриц ************************
//1.1 Скалярная матрица (a[i][j] = c, для всех i = j)
//1.2 Единичная матрица (a[i][j] = 1, для всех i = j)
//1.3 Ленточная матрица (a[i][j] = 0, если |i-j| < N, N - число диагоналей)
const uint RM_SCALAR      = 8; //Скалярная матрица
const uint RM_UNIT        = 9; //Единичная матрица
const uint RM_BAND        = 10; //Ленточная матрица (N-диагональная, N > 1)

//2. КВАДРАТНЫЕ ТРЕУГОЛЬНЫЕ МАТРИЦЫ
//2.1. Верхняя треугольная (a[i][j] = 0, если i > j)
//2.2. Нижняя треугольная  (a[i][j] = 0, если i < j)
const uint RM_UPPERTRIANG = 11; //Верхняя треугольная матрица
const uint RM_UTRIANGULAR = 11; //Верхняя треугольная матрица
const uint RM_UTRIANG     = 11; //Верхняя треугольная матрица
const uint RM_U           = 11; //Верхняя треугольная матрица
const uint RM_LOWERTRIANG = 12; //Нижняя треугольная матрица
const uint RM_LTRIANGULAR = 12; //Нижняя треугольная матрица
const uint RM_LTRIANG     = 12; //Нижняя треугольная матрица
const uint RM_L           = 12; //Нижняя треугольная матрица

//3. КВАДРАТНЫЕ СИММЕТРИЧЕСКИЕ И КОСОСИММЕТРИЧЕСКИЕ МАТРИЦЫ
//3.1. Симметрическая матрица (a[i][j] = a[j][i])
//3.2. Кососимметрическая матрица (a[i][j] = -a[j][i])
//3.3. Знакопеременная матрица (a[i][j] = -a[j][i], a[i][i] = 0)
const uint RM_SYMMETRICAL     = 13; //Симметрическая матрица
const uint RM_SYMMETR         = 13; //Симметрическая матрица
const uint RM_SKEWSYMMETRIC   = 14; //Кососимметрическая матрица
const uint RM_SKEWSYMMETR     = 14; //Кососимметрическая матрица
const uint RM_ALTERNATINGSIGN = 15; //Знакопеременная матрица
const uint RM_ALTSIGN         = 15; //Знакопеременная матрица


//***********************************************************************************
//          Объявление класса - МАТРИЦА ДЕЙСТВИТЕЛЬНЫХ ЧИСЕЛ - TRealMatrix
//TRealMatrix создает матрицу с элементами типа double необходимой размерности,
//выполняет основные математические операции над вещественными матрицами и ее элемен-
//тами, а также обеспечивает ряд других сервисных функций.
//***********************************************************************************
class TRealMatrix {
//Объявление дружественных классов

//-----------------------------------------------------------------------------------
//Объявление защищенных членов-данных класса
//-----------------------------------------------------------------------------------
protected:
 bool type;//тип вещественной матрицы (квадратная или прямоугольная)
 ulong m;  //число строк матрицы/число столбцов для квадратных матриц
 ulong n;  //число столбцов матрицы/число диагоналей для ленточной матрицы
 double **data; //Указатель на двумерный массив хранения компонент матрицы
//-----------------------------------------------------------------------------------
//I. Объявление общедоступных членов-функций класса TRealMatrix
//-----------------------------------------------------------------------------------
public:
 //**********************************************************************************
 //     ОБЪЯВЛЕНИЕ КОНСТРУКТОРОВ И ДЕСТРУКТОРА КЛАССА TRealMatrix
 //**********************************************************************************
 TRealMatrix(); //Объявление конструктора по умолчанию
 //----------------------------------------------------------------------------------
 //Объявление конструкторов с аргументами
 //----------------------------------------------------------------------------------
 //Конструирование матриц по свойствам структуры типа PF_MATRIX
 TRealMatrix(const PF_MATRIX& Info);
 //Конструирование прямоугольных (m != n) или квадратных матриц (m == n)
 TRealMatrix(ulong row, ulong col, double value);
 TRealMatrix(ulong row, ulong col, bool init);
 //Конструирование квадратных матриц (m == n)
 TRealMatrix(ulong size, double value);
 TRealMatrix(ulong size, bool init);
 TRealMatrix(ulong size, uint sq_type, ulong diag = 3L);
 //Объявление конструктора копирования
 TRealMatrix(const TRealMatrix& R);
 //Объявление деструктора класса
 ~TRealMatrix();
 //----------------------------------------------------------------------------------
 //ОБЪЯВЛЕНИЕ ФУНКЦИЙ ОБЩЕГО НАЗНАЧЕНИЯ КЛАССА TRealMatrix
 //----------------------------------------------------------------------------------
 //**********************************************************************************
 //                         ПОЛУЧЕНИЕ СВОЙСТВ МАТРИЦЫ
 //**********************************************************************************
 //----------------------------------------------------------------------------------
 //1. Определение типа и подтипа матрицы
 bool Type() const {return type;};
 bool GetType() const {return type;};
 //Проверка матрицы на квадратную
 bool IsSquare() const {return (type == SQUARE) ? true : false;};
 //Проверка матрицы на прямоугольную
 bool IsRectang() const {return (type == RECTANG) ? true : false;};
 bool IsZeroSize() const;       //Проверка на матрицу нулевого размера
 bool IsZero() const;           //Проверка на матрицу с нулевыми элементами
 bool IsSymmetric() const;      //Проверка матрицы на симметричность
 bool IsSkewSymmetric() const;  //Проверка матрицы на кососимметричность
 bool IsAltSign() const;        //Проверка матрицы на знакопеременность
 bool IsUpperTriang() const;    //Проверка на верхнюю треугольную матрицу
 bool IsLowerTriang() const;    //Проверка на нижнюю треугольную матрицу
 bool IsDiagonal() const;       //Проверка на диагональную матрицу
 bool IsScalar() const;         //Проверка на скалярную матрицу
 bool IsUnit() const;           //Проверка на единичную матрицу
 bool IsBand(ulong& diags) const;//Проверка на ленточную матрицу
 uint SubType() const; //Определение подтипа матрицы
 uint SubType(ulong& n) const; //Определение подтипа матрицы
 //----------------------------------------------------------------------------------
 //2. Определение размерности матрицы
 //Определение количества строк матрицы
 inline ulong GetRows() const {return m;};
 inline ulong Rows() const {return m;};
 //Определение количества столбцов матрицы
 inline ulong GetCols() const {return (type == SQUARE) ? m : n;};
 inline ulong Cols() const {return (type == SQUARE) ? m : n;};
 //----------------------------------------------------------------------------------
 //3. Получение свойств матрицы
 void Properties(ulong& rows, ulong& cols, bool& type) const
 {rows = GetRows(); cols = GetCols(); type = GetType();};
 void Properties(ulong& rows, ulong& cols, bool& type, uint& subtype) const;
 void Properties(PF_MATRIX& Info) const;
 string& Properties(string& s) const;//Свойства матрицы в символьном виде
 //Получение наименования типа матрицы (квадратная или прямоугольная)
 string& GetType(string& s, uint language = ENG) const;
 //Получение наименования подтипа матрицы
 string& GetSubType(string& s, uint language = ENG) const;
 //Размещение элементов матрицы в символьной строке
 string& GetData(string& s, uint precision = 6, string& delim = string(1,' ')) const;
 //Определение максимальной символьной длины элементов матрицы и максимальной
 //ширины каждого столбца матрицы.
 uint WidthCols(ivector& Width, uint Precision) const;

 //**********************************************************************************
 //                    УСТАНОВКА СВОЙСТВ МАТРИЦЫ
 //**********************************************************************************
 bool Set(ulong rows, ulong cols, double value);
 bool Set(ulong rows, ulong cols, bool init);
 bool Set(ulong size, double value) {return Set(size, size, value);};
 bool Set(ulong size, bool init) {return Set(size, size, init);};
 bool Set(ulong size, uint sq_type, ulong diags = 3L);
 bool Set(const PF_MATRIX& Info);
 //**********************************************************************************
 // ИНИЦИАЛИЗАЦИЯ ЭЛЕМЕНТОВ МАТРИЦЫ СЛУЧАЙНЫМИ ЗНАЧЕНИЯМИ
 //**********************************************************************************
 bool Random(double a = 0.0, double b = 1.0);//По равновероятному закону [a..b]
 bool Rand2PI();//По равновероятному закону [0..2PI]
 bool Gauss(double E = 0.0, double D = 1.0);//По нормальному закону
 bool Relay();//По закону Рэлея
 bool Exp(double t = 1.0);//По экспоненциальному закону

 //Консольное редактирование (ввод) элементов матрицы
 //void Edit(bool confirm = true, const char* name = "A");
 void Edit(const string& name, bool confirm = true);

 //**********************************************************************************
 // ФУНКЦИИ ДОСТУПА К ЭЛЕМЕНТАМ МАТРИЦЫ
 //**********************************************************************************
 //Получение значения элемента матрицы
 inline double Get(ulong row, ulong col) const {return data[row][col];};
 //Присвоение значения элементу матрицы
 inline void Put(ulong row, ulong col, double value) {data[row][col] = value;};

 //Получение строки матрицы с заданным индексом от 0 до Rows()-1
 rvector Row(ulong index) const;
 bool Row(rvector& V, ulong index) const;
 //Получение столбца матрицы с заданным индексом от 0 до Cols()-1
 rvector Col(ulong index) const;
 bool Col(rvector& V, ulong index) const;

 //Перестановка двух строк матрицы
 bool ExchangeRows(ulong i, ulong j);
 //Перестановка строк матрицы согласно вектору-перестановок
 bool ExchangeRows(const ivector& permutation);
 //Перестановка двух столбцов матрицы
 bool ExchangeCols(ulong i, ulong j);
 //Перестановка столбцов матрицы согласно вектору-перестановок
 bool ExchangeCols(const ivector& permutation);
 //----------------------------------------------------------------------------------
 //Перестановка элементов матрицы в заданном интервале столбцов (строк)
 //в обратном порядке по столбцам (по строкам)
 //----------------------------------------------------------------------------------
 rmatrix ReverseByCol(ulong left = 0L, ulong right = MAX_SIZE) const;
 rmatrix ReverseByRow(ulong left = 0L, ulong right = MAX_SIZE) const;
 bool ReverseCols();
 bool ReverseRows();
 //Поворот элементов матрицы на 90 градусов по или против часовой стрелки
 bool Rotate(rmatrix& A, bool clockwise = true) const;
 //----------------------------------------------------------------------------------
 //Формирование подматриц исходной матрицы
 //----------------------------------------------------------------------------------
 //Формирование квадратной подматрицы A(0 1 ... k-1 k; 0 1 ... k-1 g), где g > k
 rmatrix U_SubMatrix(ulong k, ulong g) const;
 //Формирование квадратной подматрицы A(0 1 ... k-1 g; 0 1 ... k-1 k), где g > k
 rmatrix L_SubMatrix(ulong g, ulong k) const;
 //Формирование квадратной подматрицы A(0 1 ... k-1 k; 0 1 ... k-1 k)
 rmatrix P_SubMatrix(ulong k) const;

 //Формирование подматрицы из исходной матрицы по индексам столбцов, указанным в
 //векторе ivector
 rmatrix SubMatrixByCols(const ivector& IndexCols, bool Check = false) const;
 //Формирование подматрицы из исходной матрицы по индексам строк, указанным в
 //векторе ivector
 rmatrix SubMatrixByRows(const ivector& IndexRows, bool Check = false) const;
 //Формирование подматрицы из исходной матрицы по индексам строк и столбцов,
 //размещенных в векторах IndexRows & IndexCols
 rmatrix SubMatrix(const ivector& IndexRows, const ivector& IndexCols,
                   bool Check = false) const;
 //----------------------------------------------------------------------------------
 // Присваивание исходной матрице матрицы, вектора-строки, вектора-столбца
 //----------------------------------------------------------------------------------
 rmatrix AssignRow(const rvector& V, ulong row, ulong pos = 0L) const;
 rmatrix AssignRow(const rvector& V, ulong left, ulong right, ulong pos) const;
 rmatrix AssignRow(const rvector& V, const ivector& rows, ulong pos = 0L) const;

 rmatrix AssignCol(const rvector& V, ulong col, ulong pos = 0L) const;
 rmatrix AssignCol(const rvector& V, ulong left = 0L, ulong right = MAX_SIZE,
                   ulong pos = 0L) const;
 rmatrix AssignCol(const rvector& V, const ivector& cols, ulong pos = 0L) const;

 rmatrix Assign(const rmatrix& M, ulong row = 0L, ulong col = 0L) const;
 //----------------------------------------------------------------------------------
 //Умножение матрицы на элементарную матрицу и элементарные матрицы специального вида
 //(elementary matrix)
 //ПРАВИЛА УМНОЖЕНИЯ:
 //1. Элементарная матрица может умножаться на исходную матрицу СПРАВА
 //2. Элементарная матрица может умножаться на исходную матрицу СЛЕВА
 //3. Элементарная матрица всегда КВАДРАТНАЯ
 //4. Размерности матрицы и элементарной матрицы должны быть РАВНЫ и отличны от 0
 //ЭЛЕМЕНТАРНАЯ МАТРИЦА - матрица E = I + B, где I - единичная матрица, B - матрица
 //ранга единица, т.е. матрица с единичной главной диагональю и одним внедиагональным
 //элементом отличным от нуля.
 //       |1        |
 //       |  1      |
 // Ekl = |    1    |, E(k,l) != 0, k=4,l=2.
 //       |  x   1  |
 //       |        1|
 //----------------------------------------------------------------------------------
 //Элементарные матрицы специального вида. К ним относятся:
 //1. Диагональная элементарная  матрица Dk (DiagonalEM). Диагональная матрица с
 //   элементами Dk(i,i)=1 для всех i != k и Dk(k,k)!=0.
 //      |1        |
 //      |  1      |
 // Dk = |    x    |, k = 3.
 //      |      1  |
 //      |        1|
 //2. Нижняя столбцовая элементарная матрица Lkc (LowerColumnEM). Внедиагональные
 //   ненулевые элементы расположены только в k-м столбце под диагональю.
 //       |1        |
 //       |  1      |
 // Lkc = |    1    |, k = 3.
 //       |    x 1  |
 //       |    x   1|
 //3. Верхняя столбцовая элементарная матрица Ukc (UpperColumnEM). Внедиагональные
 //   ненулевые элементы расположены только в k-м столбце над диагональю.
 //       |1   x    |
 //       |  1 x    |
 // Ukc = |    1    |, k = 3.
 //       |      1  |
 //       |        1|
 //4. Полная столбцовая элементарная матрица Tkc (CompleteColumnEM). Внедиагональные
 //   ненулевые элементы расположены только в k-м столбце.
 //       |1   x    |
 //       |  1 x    |
 // Tkc = |    1    |, k = 3.
 //       |    x 1  |
 //       |    x   1|
 //5. Левая строчная элементарная матрица Lkr (LeftRowEM). Внедиагональные ненулевые
 //   элементы расположены только в k-й строке левее диагонали.
 //       |1        |
 //       |  1      |
 // Lkr = |x x 1    |, k = 3.
 //       |      1  |
 //       |        1|
 //6. Правая строчная элементарная матрица Ukr (RightRowEM). Внедиагональные
 //   ненулевые элементы расположены только в k-й строке справа от диагонали.
 //       |1        |
 //       |  1      |
 // Ukr = |    1 x x|, k = 3.
 //       |      1  |
 //       |        1|
 //7. Полная строчная элементарная матрица Tkr (CompleteRowEM). Внедиагональные
 //   ненулевые элементы расположены только в k-й строке.
 //       |1        |
 //       |  1      |
 // Tkr = |x x 1 x x|, k = 3.
 //       |      1  |
 //       |        1|
 //При умножении вектора на элементарную, специальную элементарную или группу специаль-
 //ных элементарных матриц используется в качестве элементарной обыкновенная квадрат-
 //ная матрица. Квадратная матрица позволяет размещать группу элементарных специальных
 //матриц. Поэтому можно реализовать выполнение операций умножения вектора на несколько
 //элементарных матриц, элементы которых можно хранить в одной квадратной матрице.
 //Например, квадратная матрица размерности N, позволяет хранить одновременно N
 //DiagonalEM, N LowerColumnEM и N Upper-ColumnEM.
 //----------------------------------------------------------------------------------

 //Умножение на диагональную элементарную матрицу Dk
 //k < Q.Rows(), Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool DiagonalEM(const rmatrix& Q, bool Disp, ulong k);
 //Умножение на диагональные элементарные матрицы Dk, k=[first..last]
 //first < last < Q.Rows(), Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool DiagonalEM(const rmatrix& Q, bool Disp, ulong first, ulong last);
 //Умножение на все диагональные элементарные матрицы Dk, k=[0..Q.Rows()-1]
 //Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool DiagonalEM(const rmatrix& Q, bool Disp);

 //Умножение на нижнюю столбцовую элементарную матрицу Lkc
 //k < Q.Rows(), Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool LowerColumnEM(const rmatrix& Q, bool Disp, ulong k);
 //Умножение на нижние столбцовые элементарные матрицы Lkc, k=[first..last]
 //first < last < Q.Rows(), Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool LowerColumnEM(const rmatrix& Q, bool Disp, ulong first, ulong last);
 //Умножение на все нижние столбцовые элементарные матрицы Lkc, k=[0..Q.Rows()-1]
 //Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool LowerColumnEM(const rmatrix& Q, bool Disp);

 //Умножение на верхнюю столбцовую элементарную матрицу Ukc
 //k < Q.Rows(), Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool UpperColumnEM(const rmatrix& Q, bool Disp, ulong k);
 //Умножение на верхние столбцовые элементарные матрицы Ukc, k=[first..last]
 //first < last < Q.Rows(), Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool UpperColumnEM(const rmatrix& Q, bool Disp, ulong first, ulong last);
 //Умножение на все верхние столбцовые элементарные матрицы Ukc, k=[0..Q.Rows()-1]
 //Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool UpperColumnEM(const rmatrix& Q, bool Disp);

 //Умножение на полную столбцовую элементарную матрицу Tkc
 //k < Q.Rows(), Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool CompleteColumnEM(const rmatrix& Q, bool Disp, ulong k);
 //Умножение на полные столбцовые элементарные матрицы Tkc, k=[first..last]
 //first < last < Q.Rows(), Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool CompleteColumnEM(const rmatrix& Q, bool Disp, ulong first, ulong last);
 //Умножение на все полные столбцовые элементарные матрицы Tkc, k=[0..Q.Rows()-1]
 //Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool CompleteColumnEM(const rmatrix& Q, bool Disp);

 //Умножение на левую строчную элементарную матрицу Lkr
 //k < Q.Rows(), Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool LeftRowEM(const rmatrix& Q, bool Disp, ulong k);
 //Умножение на левые строчные элементарные матрицы Lkr, k=[first..last]
 //first < last < Q.Rows(), Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool LeftRowEM(const rmatrix& Q, bool Disp, ulong first, ulong last);
 //Умножение на все левые строчные элементарные матрицы Lkr, k=[0..Q.Rows()-1]
 //Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool LeftRowEM(const rmatrix& Q, bool Disp);

 //Умножение на правую строчную элементарную матрицу Ukr
 //k < Q.Rows(), Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool RightRowEM(const rmatrix& Q, bool Disp, ulong k);
 //Умножение на правые строчные элементарные матрицы Ukr, k=[first..last]
 //first < last < Q.Rows(), Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool RightRowEM(const rmatrix& Q, bool Disp, ulong first, ulong last);
 //Умножение на все правые строчные элементарные матрицы Ukr, k=[0..Q.Rows()-1]
 //Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool RightRowEM(const rmatrix& Q, bool Disp);

 //Умножение на полную строчную элементарную матрицу Tkr
 //k < Q.Rows(), Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool CompleteRowEM(const rmatrix& Q, bool Disp, ulong k);
 //Умножение на полные строчные элементарные матрицы Tkr, k=[first..last]
 //first < last < Q.Rows(), Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool CompleteRowEM(const rmatrix& Q, bool Disp, ulong first, ulong last);
 //Умножение на все полные строчные элементарные матрицы Tkr, k=[0..Q.Rows()-1]
 //Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool CompleteRowEM(const rmatrix& Q, bool Disp);

 //Умножение на обыкновенную элементарную матрицу Bij
 //Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 //row < Rows() && col < Cols()
 bool OrdinaryEM(const rmatrix& Q, bool Disp, ulong row, ulong col);

//-----------------------------------------------------------------------------------
//              ОБЪЯВЛЕНИЕ ПЕРЕГРУЖЕННЫХ ОПЕРАТОРОВ
//-----------------------------------------------------------------------------------
 //1. OVERLOAD THE CALL FUNCTION OPERATOR
 //Объявление перегруженного оператора вызова функции для доступа к компоненте
 //матрицы (двумерного массива). Используется в программах пользователя.
 double& operator() (ulong i, ulong j);
 //Объявление перегруженного оператора вызова функции для доступа к компоненте
 //матрицы (двумерного массива). Эта версия вызывается в операторе присваивания
 //и в конструкторе копирования.
 //This version is called in ASSIGNMENT OPERATOR and COPY the CONSTRUCTOR
 const double& operator() (ulong i, ulong j) const;
 //2. OVERLOAD THE ASSIGNMENT OPERATOR
 //Объявление перегруженного оператора присваивания operator=
 bool operator =(const TRealMatrix& R);
 //3. Объявление перегруженного оператора operator= преобразования комплексной
 //   матрицы в действительную матрицу (РАЗЛОЖЕНИЕ КОМПЛЕКСНОЙ МАТРИЦЫ)
 bool operator =(const TComplexMatrix& C);
 //4. Объявление перегруженного оператора operator= преобразования целочисленной мат-
 //   рицы в действительную матрицу
 bool operator =(const TIntMatrix& A);
 //5. OVERLOAD THE ASSIGNMENT OPERATOR
 //Объявление перегруженного оператора присваивания operator = компонентам
 //вещественной матрицы действительного значения
 bool operator =(const double value);
 //6. OVERLOAD THE COMPARISON OPERATOR (==)
 //Объявление перегруженного оператора сравнения двух матриц operator ==
 bool operator ==(const TRealMatrix& A) const;
 //7. OVERLOAD THE STREAM INSERTION OPERATOR
 //Перегрузка операции потокового вывода (<<) для отображения элементов матрицы
 friend ostream& operator <<(ostream& out, const TRealMatrix& R);

 //**********************************************************************************
 //             Операции Линейной Алгебры над Действительными Матрицами
 //   Все операции линейной алгебры производятся с копией исходной матрицы.
 //**********************************************************************************
 //Перестановка элементов квадратной матрицы относительно дополнительной диагонали
 bool Reverse();
 //Операция транспонирования вещественной матрицы
 rmatrix Transpose() const;
 bool Transpose(rmatrix& A) const;
 //Перегрузка унарного оператора (!) для получения транспонированной матрицы
 rmatrix operator !() const;
 //Вычисление следа квадратной матрицы
 double Trace() const;
 double Spur() const {return Trace();};
 //Определение максимального и минимального значений элементов матрицы
 bool MaxMin(double& Max, double& Min) const;
 //----------------------------------------------------------------------------------
 //Определение норм (мер) вещественной матрицы
 double NFirstNorm() const;
 double FirstNorm() const;
 double SecondNorm() const;
 double ThirdNorm() const;
 double PNorm(uint p = 2) const;
 double Norm(uint type = EUCLIDEAN_NORM, uint p = 2) const;
 //Вычисление нормы строки матрицы с заданным индексом от 0 до Rows()-1
 double NormRow(ulong row, uint type = EUCLIDEAN_NORM, uint p = 2) const;
 //Вычисление нормы столбца матрицы с заданным индексом от 0 до Cols()-1
 double NormCol(ulong col, uint type = EUCLIDEAN_NORM, uint p = 2) const;
 //Вычисление норм всех строк матрицы
 bool NormAllRows(rvector& V, uint type = EUCLIDEAN_NORM, uint p = 2) const;
 //Вычисление норм всех столбцов матрицы
 bool NormAllCols(rvector& V, uint type = EUCLIDEAN_NORM, uint p = 2) const;
 //----------------------------------------------------------------------------------
 //Нормирование элементов матрицы
 bool Normalize(rmatrix& A, uint NormType, uint p = 0) const;
 bool Normalize(uint NormType, uint p = 0);
 bool NormalizeToDB(rmatrix& A, bool Power = true, double MindB = -60) const;
 bool NormalizeToDB(bool Power = true, double MindB = -60);
 //Нормирование элементов матрицы по строкам
 bool NormalizeRows(rmatrix& A, uint NormType, uint p = 0) const;
 bool NormalizeRows(uint NormType, uint p = 0);
 bool NormalizeRowsToDB(rmatrix& A, bool Power = true, double MindB = -60) const;
 bool NormalizeRowsToDB(bool Power = true, double MindB = -60);
 //Нормирование элементов матрицы по столбцам
 bool NormalizeCols(rmatrix& A, uint NormType, uint p = 0) const;
 bool NormalizeCols(uint NormType, uint p = 0);
 bool NormalizeColsToDB(rmatrix& A, bool Power = true, double MindB = -60) const;
 bool NormalizeColsToDB(bool Power = true, double MindB = -60);

 //Увеличение матрицы на действительное число value
 //B = const + A, B(i,j) = B(i,j) + const
 friend rmatrix operator +(const double value, const rmatrix& A);
 //B = A + const, B(i,j) = A(i,j) + const
 friend rmatrix operator +(const rmatrix& A, const double value);

 //Уменьшение матрицы на действительное число value
 //B = A - const, B(i,j) = A(i,j) - const
 friend rmatrix operator -(const rmatrix& A, const double value);

 //Умножение матрицы на действительное число value
 //B = const * A, B(i,j) = A(i,j) * const
 friend rmatrix operator *(const double value, const rmatrix& A);
 //B = A * const, B(i,j) = A(i,j) * const
 friend rmatrix operator *(const rmatrix& A, const double value);

 //Деление матрицы на действительное число value
 //B = A / const, B(i,j) = A(i,j) / const
 rmatrix operator /(const double value) const;

 //Сложение двух вещественных матриц
 //С = A + B, C(i,j) = A(i,j) + B(i,j);
 rmatrix operator +(const rmatrix& A) const;

 //Вычитание двух вещественных матриц
 //С = A - B, C(i,j) = A(i,j) - B(i,j);
 rmatrix operator -(const rmatrix& A) const;

 //Умножение двух вещественных матриц
 rmatrix operator *(const rmatrix& A) const;

 //Умножение матрицы на ее транспонированную матрицу справа B = A*trA
 bool MultByTransposedMatrixOnRight(rmatrix& A) const;

 //Умножение матрицы на ее транспонированную матрицу слева B = trA*A
 bool MultByTransposedMatrixOnLeft(rmatrix& A) const;

 //Возведение матрицы в квадрат: A^2 = A*A с возможностью учета ее симметричности
 //Только для квадратных матриц
 bool Squaring(rmatrix& A, bool lSymmetrical = false) const;

 //Поэлементное умножение столбцов (строк) вещественной матрицы на вещественный
 //вектор одинакового с матрицей размера по строкам (столбцам)
 rmatrix MultByCols(const rvector& V) const;
 rmatrix MultByCols(const rvector& V, ulong index) const;
 rmatrix MultByCols(const rvector& V, ulong left, ulong right) const;
 rmatrix MultByCols(const rvector& V, const ivector& index, bool check = false) const;

 rmatrix MultByRows(const rvector& V) const;
 rmatrix MultByRows(const rvector& V, ulong index) const;
 rmatrix MultByRows(const rvector& V, ulong left, ulong right) const;
 rmatrix MultByRows(const rvector& V, const ivector& index, bool check = false) const;

 //Сложение столбцов (строк) вещественной матрицы с вещественным вектором
 //одинакового с матрицей размера по строкам (столбцам)
 rmatrix AddByCols(const rvector& V) const;
 rmatrix AddByCols(const rvector& V, ulong index) const;
 rmatrix AddByCols(const rvector& V, ulong left, ulong right) const;
 rmatrix AddByCols(const rvector& V, const ivector& index, bool check = false) const;

 rmatrix AddByRows(const rvector& V) const;
 rmatrix AddByRows(const rvector& V, ulong index) const;
 rmatrix AddByRows(const rvector& V, ulong left, ulong right) const;
 rmatrix AddByRows(const rvector& V, const ivector& index, bool check = false) const;

 //Суммирование столбцов (строк) вещественной матрицы
 rvector SumCols(ulong left = 0L, ulong right = MAX_SIZE) const;
 rvector SumCols(const ivector& index) const;

 rvector SumRows(ulong left = 0L, ulong right = MAX_SIZE) const;
 rvector SumRows(const ivector& index) const;

 //----------------------------------------------------------------------------------
 //Расчет миноров исходной матрицы
 //----------------------------------------------------------------------------------
 //Расчет минора вида M(0 1 ... k-1 k; 0 1 ... k-1 g), где g > k
 double U_Minor(ulong k, ulong g) const;
 //Расчет минора вида M(0 1 ... k-1 g; 0 1 ... k-1 k), где g > k
 double L_Minor(ulong g, ulong k) const;
 //Расчет минора вида M(0 1 ... k-1 k; 0 1 ... k-1 k) - главный минор
 double P_Minor(ulong k) const;
 //Расчет всех миноров вида M(0 1 ... k-1 k; 0 1 ... k-1 g), где g > k
 // 0 < g < min(Cols(),Rows()), 0 <= k < g
 rmatrix U_AllMinors() const;
 //Расчет минора вида M(0 1 ... k-1 g; 0 1 ... k-1 k), где g > k
 // 0 < g < min(Cols(),Rows()), 0 <= k < g
 rmatrix L_AllMinors() const;
 //Расчет минора вида M(0 1 ... k-1 k; 0 1 ... k-1 k) - главный минор
 // 0 <= k < min(Cols(),Rows())
 rmatrix P_AllMinors() const;
 rvector& P_AllMinors(rvector& P) const;
 //Расчет всех миноров матрицы
 rmatrix AllMinors(bool L = true, bool P = true, bool U = true) const;

 //----------------------------------------------------------------------------------
 // Разложение действительных квадратных матриц на нижнюю, верхнюю и диагональную
 // матрицы: A = LDU
 // В алгоритме используется вычисление всех миноров исходной матрицы для расчета
 // коэффициентов нижней L, верхней U и диагональной D матриц. С вычислительной
 // точки зрения этот алгоритм нельзя считать эффективным для разложения матрицы.
 //----------------------------------------------------------------------------------
 bool LDU_Gauss(rmatrix& LDU) const;
 bool LDU_Gauss(rmatrix& L, rmatrix& D, rmatrix& U) const;

 //----------------------------------------------------------------------------------
 //Расчет определителей действительных квадратных матриц и их разложение на
 //нижнюю и верхнюю треугольную матрицы: A = LU
 //----------------------------------------------------------------------------------
 //Накопление произведений вида Ak*Bk и суммирование их с начальным значением
 friend double InnerProduct(ulong l, ulong s, ulong u, double init,
                            const rmatrix& A, bool Avar, ulong Aconst,
                            const rmatrix& B, bool Bvar, ulong Bconst);

 //Алгоритм Краута (модификация компактной схемы Гаусса с частичным выбором
 //главного элемента по столбцу): плотная, несимметрическая квадратная матрица
 bool LU_UnSymmetric(double& Det, rmatrix& LU, ivector& ExChange,
                     double* Base = NULL, long* Power = NULL) const;
 bool LU_UnSymmetric(rmatrix& LU, ivector& ExChange, double Eps = 1e-16) const;
 double DET_UnSymmetric(double* Base = NULL, long* Power = NULL) const;

 //----------------------------------------------------------------------------------
 // Решение действительных систем линейных уравнений
 //----------------------------------------------------------------------------------
 //Решение системы уравнений AX = B, где A = LU, B - матрица правых частей
 //Матрица A - несимметрическая матрица. Решение проводится по компактной схеме
 //Гаусса (алгоритм Краута).
 rmatrix LE_UnSymSolution(const rmatrix& B, const ivector& ExChange) const;
 friend rmatrix LE_UnSymSolution(const rmatrix& A, const rmatrix& B);

 //----------------------------------------------------------------------------------
 // Обращение матриц
 //----------------------------------------------------------------------------------
 //Обращение несимметрической квадратной матрицы n x n методом разложения ее на треугольные
 //матрицы L & U c помощью алгоритма Краута и решения системы линейных уравнений с единич-
 //ной матрицей размерности n x n в правой части системы
 rmatrix INV_Krauth(double Eps = 1e-16) const;
 bool INV_Krauth(rmatrix& invA, double Eps = 1e-16) const;
 //----------------------------------------------------------------------------------
 //Расчет собственных значений и собственных векторов действительных
 //квадратных матриц
 //----------------------------------------------------------------------------------
 //Алгоритм Якоби: плотная, симметрическая матрица
 uint EVV_Jacobi(TRealVector& D, TRealMatrix& V, double EPS = 1e-19) const;
 int EV_Jacobi(TRealVector& D, double EPS = 1e-19, bool lSort = true) const;
//-----------------------------------------------------------------------------------
//    Треугольное разложение положительно определенных симметрических матриц
//                     (разложение по схеме Холецкого)
// Если A - симметрическая  положительно определенная матрица, то существует действи-
// тельная невырожденная нижняя треугольная матрица L, такая что   L*tr(L) = A.  (1)
// Более того, если диагональные элементы матрицы L выбраны положительными, то разло-
// жение единственно. Элементы матрицы L можно определять по строкам или по столбцам,
// приравнивая соответствующие элементы матриц в выражении (1). Если матрицу вычисляют
// по строкам, то для элементов i-ой строки справедливы следующие соотношения:
//  j
// SUM L[ik]*L[jk] = A[ij] или                        (2)
// k=1
//                  j-1
//          A[ij] - SUM L[ik]*L[jk]
//                  k=1
// L[ij] = ------------------------ , j = 1,...,i-1;  (2)
//                  L[jj]
//
//  i
// SUM L[ik]*L[ik] = A[ii], или                       (3)
// k=1
//
//                   i-1          ^1/2
// L[ii] = { A[ii] - SUM L[ik]^2 }                    (3)
//                   k=1
// Для реализации метода необходимо N действий извлечения квадратного корня и прибли-
// зительно (N^3)/6 умножений, N - размерность квадратной матрицы.
//-----------------------------------------------------------------------------------
//Разложение по схеме Холецкого вида A = LU, U = tr(L) с вычислением определителя
 bool DET_Cholesky(rmatrix& LU, double& Det) const;

//Решение системы уравнений AX = B, где A = LU, B - матрица правых частей
//Матрица A - симметрическая матрица положительно определенная матрица. Решение
//проводится по схеме Холецкого.
 friend bool LE_Cholesky(const rmatrix& A, const rmatrix& B, rmatrix& X);

//Обращение симметрической положительно определенной матрицы
 bool INV_Cholesky(rmatrix& invA) const;

//-----------------------------------------------------------------------------------
//         ОБЪЯВЛЕНИЕ ДРУЖЕСТВЕННЫХ ФУНКЦИЙ КЛАССУ TRealMatrix
//-----------------------------------------------------------------------------------
//Проверка на равенство размерностей матриц A и B
 friend bool IsEqualSize(const rmatrix& A, const rmatrix& B)
 {return ((A.Rows() == B.Rows()) && (A.Cols() == B.Cols())) ? true : false;};
//Определение максимальной символьной длины элементов матрицы и максимальной
//ширины каждого столбца матрицы.
 friend uint* MaxStrLen(const TRealMatrix& R, uint precision);
//-----------------------------------------------------------------------------------
//Построение корреляционной матрицы
//-----------------------------------------------------------------------------------
 friend bool CorrelationMatrix(const rvector& V, rmatrix& R);
//-----------------------------------------------------------------------------------
//Преобразование матрицы в вектор и вектора в матрицу
//-----------------------------------------------------------------------------------
 friend bool MatrixToVectorByCols(const rmatrix& A, rvector& V);
 friend bool MatrixToVectorByRows(const rmatrix& A, rvector& V);
 friend bool VectorToMatrixByCols(const rvector& V, rmatrix& A, ulong rows, ulong cols);
 friend bool VectorToMatrixByRows(const rvector& V, rmatrix& A, ulong rows, ulong cols);
//-----------------------------------------------------------------------------------
//Реализация основных матричных операций
//1) Умножение матрицы на матрицу C = A*B;
//2) Умножение матрицы на число   B = A*c || c*A;
//3) Сложение двух матриц C = A+B;
//
//Данные функции следует использовать при работе с матрицами больших размеров.
//Скорость их выполнения выше и требования к размеру памяти меньше, чем в аналогичных
//функциях с перегрузкой операторов. В функциях с перегрузкой операторов результаты
//вычислений передаются по значению, что приводит к уменьшению общей скорости расчета
//и к большим затратам памяти на передачу результата вычисления (матриц, векторов)
//другим функциям.
//-----------------------------------------------------------------------------------
//Умножение матриц C = A*B (общий алгоритм перемножения двух матриц)
friend bool Multiply(rmatrix& C, const rmatrix& A, const rmatrix& B);
//Умножение вида A = A * B
friend bool Multiply(rmatrix& A, const rmatrix& B);
//Умножение вида D = A * B * C
friend bool Multiply(rmatrix& D, const rmatrix& A, const rmatrix& B, const rmatrix& C);
//Умножение элементов матрицы A на константу c: B = A*c = c*A
friend bool Multiply(rmatrix& B, const rmatrix& A, double c);
//Умножение элементов матрицы A на константу c: A = A*c = c*A
friend bool Multiply(rmatrix& A, double c);
//Сложение двух матриц C = A+B = B+A
friend bool Add(rmatrix& C, const rmatrix& A, const rmatrix& B);
//Сложение двух матриц A = A+B
friend bool Add(rmatrix& A, const rmatrix& B);
//Увеличение каждого элемента матрицы A на величину value
friend bool Add(rmatrix& A, double value);
//Умножение матрицы A на вектор X: Y = A*X
friend bool Multiply(rvector& Y, const rmatrix& A, const rvector& X);
//Квадратическая форма B = H*A*trH
friend bool QuadraticForm(rmatrix& B, const rmatrix& H, const rmatrix& A);
//Квадратическая форма вида Q(m,m) = H(m,n)*Rxx(n,n)*trH(n,m), Rxx = X*trX
//X - вектор-столбец, trX - вектор-строка, H - матрица, trH - транспонированная H
friend bool QuadraticForm(rmatrix& Q, const rmatrix& H, const rvector& X);
//Прямое двумерное преобразование Фурье дискретного сигнала X
friend bool DirectFourierTransform(rmatrix& FT_Y, const rmatrix& X, long hX0 = 0L,
            long wX0 = 0L, ulong VCount = 0L, ulong HCount = 0L);
//Логарифмирование всех элементов матрицы A по основанию 10
friend bool Log10(rmatrix& A);
friend bool Log10(rmatrix& A, double Min);
//Логарифмирование всех элементов матрицы A по основанию 10 с последующим умножением
//на множитель factor и ограничением по минимуму до значения Min. Для значение матрицы
//равных или меньших нуля устанавливается значение Min.
friend long Log10(rmatrix& B, const rmatrix& A, double factor = 10, double Min = -60);
//Округление всех значений элементов матрицы A до величины r_val с допуском eps
friend ulong Round(rmatrix& A, const double r_val, const double eps = 1e-10);
//Возведение всех элементов матрицы A в квадрат
friend bool Square(rmatrix& A);
//Возведение всех элементов матрицы A в квадрат и присвоение их матрице B
friend bool Square(rmatrix& B, const rmatrix& A);
//Извлечение квадратного корня из всех элементов матрицы A
friend bool Sqrt(rmatrix& A);
//Вычисление абсолютных величин всех элементов матрицы A
friend bool Abs(rmatrix& A);
//Вычисление диагональных элементов произведения трех матриц вида:
// diag(Q(n,n)) = trH(n,m)*Rxx(m,m)*H(m,n),
//H - матрица и trH - матрица, транспонированная к H, Rxx - квадратная матрица
friend bool CalcDiagElems(rvector& V, const rmatrix& H, const rmatrix& R, bool sq_root);
//Выполнение операции усреднения элементов матрицы B с последующим суммированием с матри-
//цей A: A = A + B/n
friend bool Average(rmatrix& A, const rmatrix& B, ulong n);
//Выполнение операции непрерывного усреднения без задания общего числа усреднений
//На каждом k-ом шаге усреднения получаем матрицу AvX усредненную за k шагов (k >= 0)
friend bool InfiniteAverage(rmatrix& AvX, const rmatrix& Xk, ulong k);

//Табличное форматирование данных матрицы A с формированием заголовка таблицы по столбцам
//в строке sTbl типа string
friend string& MakeTable(string& sTbl, const UStringMap& HeadColl, const rmatrix& A,
       uint precision = 6, string& delim = string(1,' '));

//-----------------------------------------------------------------------------------
//II. Объявление закрытых членов-функций класса TRealMatrix
//-----------------------------------------------------------------------------------
private:
 void Initial();//Инициализация членов-данных класса при его конструировании
 //Создание матрицы определенного размера в оперативной памяти без инициализации
 bool Create();
 void Delete();//Удаление элементов матрицы из оперативной памяти

//------------------------------------------------------------------------------
//III. Объявление защищенных членов-функций класса TRealMatrix
//------------------------------------------------------------------------------
protected:
 //*****************************************************************************
 //Объявление функций консольного ввода элементов матриц различного типа
 //*****************************************************************************
 //Ввод элементов прямоугольной и квадратной матриц, вектора-строки,
 //вектора-столбца действительных чисел
 void InPutRealMatrix(const string& name, bool confirm = true);
 //Ввод элементов диагональной матрицы
 void InPutDiagonal(const string& name, bool confirm = true);
 //Ввод элементов скалярной матрицы
 void InPutScalar(const string& name, bool confirm = true);
 //Ввод элементов ленточной матрицы
 void InPutBand(ulong diags, const string& name, bool confirm = true);
 //Ввод элементов верхней треугольной матрицы
 void InPutUTriang(const string& name, bool confirm = true);
 //Ввод элементов нижней треугольной матрицы
 void InPutLTriang(const string& name, bool confirm = true);
 //Ввод элементов симметрической матрицы
 void InPutSymmetric(const string& name, bool confirm = true);
 //Ввод элементов кососимметрической матрицы
 void InPutSkewSymmetric(const string& name, bool confirm = true);
 //Ввод элементов знакопеременной матрицы
 void InPutAltSign(const string& name, bool confirm = true);
 //**********************************************************************************
 //Объявление функций инициализации элементов матриц различного типа
 //**********************************************************************************
 void InitZeroMatrix();
 void InitMatrix();
 void InitDiagonal();
 void InitScalar();
 void InitUnit();
 void InitBand(ulong diags);
 void InitUTriang();
 void InitLTriang();
 void InitSymmetric();
 void InitSkewSymmetric();
 void InitAltSign();
};
/****** КОНЕЦ ОБЪЯВЛЕНИЯ КЛАССА - ДЕЙСТВИТЕЛЬНАЯ МАТРИЦА - class TRealMatrix *******/

//-----------------------------------------------------------------------------------
//          ОБЪЯВЛЕНИЕ КОНСТАНТ, СТРУКТУР и КЛАССА TComplexMatrix
//-----------------------------------------------------------------------------------
//КЛАССИФИКАЦИЯ КОМПЛЕКСНЫХ МАТРИЦ
//Возможные значения принимаемые type - членом-данных класса cmatrix
const uint CM_ZEROSIZE    = 0;//Комплексная матрица нулевой размерности
const uint CM_ZERO        = 1;//Комплексная матрица с нулевыми элементами
const uint CM_NUMBER      = 2;//Комплексная матрица размером 1 x 1
const uint CV_ROW         = 3;//Комплексный вектор-строка
const uint CV_COLUMN      = 4;//Комплексный вектор-столбец
const uint CM_RECTANG     = 5;//Прямоугольная комплексная матрица
const uint CM_RECTANGULAR = 5;//Прямоугольная комплексная матрица
const uint CM_RECT        = 5;//Прямоугольная комплексная матрица

//КВАДРАТНЫЕ КОМПЛЕКСНЫЕ МАТРИЦЫ
const uint CM_SQUARE = 6;//Квадратная матрица
const uint CM_SQ     = 6;//Квадратная матрица
/******************** Подкласс квадратных матриц ******************************/
//1. КВАДРАТНЫЕ ДИАГОНАЛЬНЫЕ МАТРИЦЫ
//Диагональная матрица: a[i][j] = 0, для всех i != j
const uint CM_DIAGONAL = 7;//Диагональная матрица

/********************* Подкласс диагональных матриц ***************************/
//1.1 Скалярная матрица (a[i][j] = c, для всех i = j)
//1.2 Единичная матрица (a[i][j] = 1, для всех i = j)
//1.3 Ленточная матрица (a[i][j] = 0, если |i-j| < N, N - число диагоналей)
const uint CM_SCALAR = 8;//Скалярная матрица
const uint CM_UNIT   = 9;//Единичная матрица
const uint CM_BAND   = 10;//Ленточная матрица (N-диагональная, N > 1)

//2. КВАДРАТНЫЕ ТРЕУГОЛЬНЫЕ МАТРИЦЫ
//2.1. Верхняя треугольная (a[i][j] = 0, если i > j)
//2.2. Нижняя треугольная  (a[i][j] = 0, если i < j)
const uint CM_UPPERTRIANG = 11;//Верхняя треугольная матрица
const uint CM_UTRIANGULAR = 11;//Верхняя треугольная матрица
const uint CM_UTRIANG     = 11;//Верхняя треугольная матрица
const uint CM_U           = 11;//Верхняя треугольная матрица
const uint CM_LOWERTRIANG = 12;//Нижняя треугольная матрица
const uint CM_LTRIANGULAR = 12;//Нижняя треугольная матрица
const uint CM_LTRIANG     = 12;//Нижняя треугольная матрица
const uint CM_L           = 12;//Нижняя треугольная матрица

//3. КВАДРАТНЫЕ СИММЕТРИЧЕСКИЕ И КОСОСИММЕТРИЧЕСКИЕ МАТРИЦЫ
//3.1. Симметрическая матрица (a[i][j] = a[j][i])
//3.2. Кососимметрическая матрица (a[i][j] = -a[j][i])
//3.3. Знакопеременная матрица (a[i][j] = -a[j][i], a[i][i] = 0)
const uint CM_SYMMETRICAL     = 13;//Симметрическая матрица
const uint CM_SYMMETR         = 13;//Симметрическая матрица
const uint CM_SKEWSYMMETRIC   = 14;//Кососимметрическая матрица
const uint CM_SKEWSYMMETR     = 14;//Кососимметрическая матрица
const uint CM_SKEWSYM         = 14;//Кососимметрическая матрица
const uint CM_ALTERNATINGSIGN = 15;//Знакопеременная матрица
const uint CM_ALTSIGN         = 15;//Знакопеременная матрица

//4. ЭРМИТОВЫ МАТРИЦЫ
//Эрмитова матрица: a[i][i] = real_number; a[i][j] = conj(a[j][i]).
const uint CM_HERMITIAN = 16;//Эрмитовая матрица

//******************************************************************************
//Объявление класса - МАТРИЦА КОМПЛЕКСНЫХ ЧИСЕЛ - TComplexMatrix
//TComplexMatrix осуществляет создание комплексной матрицы, хранение ее элементов,
//выполнение основных математических операций над комплексными матрицами и ее
//элементами, а также обеспечивает ряд других сервисных функций.
//******************************************************************************
class TComplexMatrix {
//Объявление дружественных классов

//------------------------------------------------------------------------------
//Объявление защищенных членов-данных класса
//------------------------------------------------------------------------------
protected:
 bool type;   //тип комплексной матрицы (квадратная или прямоугольная)
 ulong m;     //число строк матрицы
 ulong n;     //число столбцов матрицы
 complex **data;//Указатель на двумерный массив хранения компонент матрицы
//------------------------------------------------------------------------------
//I. Объявление общедоступных членов-функций класса TComplexMatrix
//------------------------------------------------------------------------------
public:
 //-----------------------------------------------------------------------------
 //    ОБЪЯВЛЕНИЕ КОНСТРУКТОРОВ И ДЕСТРУКТОРА КЛАССА TComplexMatrix
 //-----------------------------------------------------------------------------
 //Объявление конструктора по умолчанию
 TComplexMatrix();
 //Объявление конструкторов с аргументами
 //Конструирование прямоугольных (m != n) или квадратных (m == n) матриц
 TComplexMatrix(ulong row, ulong col, complex value);
 TComplexMatrix(ulong row, ulong col, bool init);
 TComplexMatrix(const PF_MATRIX& Info);
 //Конструирование квадратных матриц
 TComplexMatrix(ulong size, complex value);
 TComplexMatrix(ulong size, bool init);
 TComplexMatrix(ulong size, uint sq_type, ulong diag = 3);
 //Объявление конструктора копирования
 TComplexMatrix(const TComplexMatrix& A);
 //Объявление деструктора класса
 ~TComplexMatrix();
 //-----------------------------------------------------------------------------
 // ПОЛУЧЕНИЕ СВОЙСТВ МАТРИЦЫ
 //-----------------------------------------------------------------------------
 //1. Определение типа и подтипа матрицы
 bool Type() const {return type;};
 bool GetType() const {return type;};
 bool IsSquare() const          //Проверка матрицы на квадратную
 {return (type == SQUARE) ? true : false;};
 bool IsRectang() const         //Проверка матрицы на прямоугольную
 {return (type == RECTANG) ? true : false;};
 bool IsZeroSize() const;       //Проверка на матрицу нулевого размера
 bool IsZero() const;           //Проверка на матрицу с нулевыми элементами
 bool IsSymmetric() const;      //Проверка матрицы на симметричность
 bool IsSkewSymmetric() const;  //Проверка матрицы на кососимметричность
 bool IsAltSign() const;        //Проверка матрицы на знакопеременность
 bool IsUpperTriang() const;    //Проверка на верхнюю треугольную матрицу
 bool IsLowerTriang() const;    //Проверка на нижнюю треугольную матрицу
 bool IsDiagonal() const;       //Проверка на диагональную матрицу
 bool IsScalar() const;         //Проверка на скалярную матрицу
 bool IsUnit() const;           //Проверка на единичную матрицу
 bool IsBand(ulong& diags) const;//Проверка на ленточную матрицу
 bool IsHermitian() const;//Проверка матрицы на эрмитовую

 uint SubType() const; //Определение подтипа матрицы
 uint SubType(ulong& n) const; //Определение подтипа матрицы
 //-----------------------------------------------------------------------------
 //2. Определение размерности матрицы
 //Определение количества строк матрицы
 ulong GetRows() const {return m;};
 ulong Rows() const {return m;};
 //Определение количества столбцов матрицы
 ulong GetCols() const {return (type == SQUARE) ? m : n;};
 ulong Cols() const {return (type == SQUARE) ? m : n;};
 //-----------------------------------------------------------------------------
 //3. Получение свойств матрицы
 void Properties(ulong& rows, ulong& cols, bool& type) const
 {rows = GetRows(); cols = GetCols(); type = GetType();};
 void Properties(ulong& rows, ulong& cols, bool& type, uint& subtype) const;
 void Properties(PF_MATRIX& Info) const;
 string& Properties(string& s) const;//Свойства матрицы в символьном виде
 //Получение наименования типа матрицы (квадратная или прямоугольная)
 string& GetType(string& s, uint language = ENG) const;
 //Получение наименования подтипа матрицы
 string& GetSubType(string& s, uint language = ENG) const;
 //*****************************************************************************
 //                    УСТАНОВКА СВОЙСТВ МАТРИЦЫ
 //*****************************************************************************
 bool Set(ulong rows, ulong cols, complex value);
 bool Set(ulong rows, ulong cols, bool init);
 bool Set(ulong size, complex value) {return Set(size, size, value);};
 bool Set(ulong size, bool init) {return Set(size, size, init);};
 bool Set(ulong size, uint sq_type, ulong diags = 3L);
 bool Set(const PF_MATRIX& Info);
 //*****************************************************************************
 // ИНИЦИАЛИЗАЦИЯ ЭЛЕМЕНТОВ МАТРИЦЫ СЛУЧАЙНЫМИ ЗНАЧЕНИЯМИ
 //*****************************************************************************
 bool Random(double a = 0.0, double b = 1.0);//По равновероятному закону [a..b]
 bool Rand2PI();//По равновероятному закону [0..2PI]
 bool Gauss(double E = 0.0, double D = 1.0);//По нормальному закону
 bool Relay();//По закону Рэлея
 bool Exp(double t = 1.0);//По экспоненциальному закону

 //Консольное редактирование (ввод) элементов матрицы
 void Edit(const string& name, bool confirm = true);

 //*****************************************************************************
 // ФУНКЦИИ ДОСТУПА К ЭЛЕМЕНТАМ МАТРИЦЫ
 //*****************************************************************************
 //Получение значения элемента матрицы
 inline complex Get(ulong row, ulong col) const {return data[row][col];};
 //Присвоение значения элементу матрицы
 inline void Put(ulong row, ulong col, complex value) {data[row][col] = value;};
 inline void Put(ulong row, ulong col, double value) {data[row][col] = complex(value,0);};

 //Получение строки матрицы с заданным индексом от 0 до Rows()-1
 cvector Row(ulong index) const;
 bool Row(cvector& V, ulong index) const;
 //Получение столбца матрицы с заданным индексом от 0 до Cols()-1
 cvector Col(ulong index) const;
 bool Col(cvector& V, ulong index) const;

 //Перестановка двух строк матрицы
 bool ExchangeRows(ulong i, ulong j);
 //Перестановка строк матрицы согласно вектору-перестановок
 bool ExchangeRows(const ivector& permutation);
 //Перестановка двух столбцов матрицы
 bool ExchangeCols(ulong i, ulong j);
 //Перестановка столбцов матрицы согласно вектору-перестановок
 bool ExchangeCols(const ivector& permutation);
 //-----------------------------------------------------------------------------
 //Перестановка элементов матрицы в заданном интервале столбцов (строк)
 //в обратном порядке по столбцам (по строкам)
 //-----------------------------------------------------------------------------
 cmatrix ReverseByCol(ulong left = 0L, ulong right = MAX_SIZE) const;
 cmatrix ReverseByRow(ulong left = 0L, ulong right = MAX_SIZE) const;
 bool ReverseCols();
 bool ReverseRows();
 //-----------------------------------------------------------------------------
 //Формирование подматриц исходной матрицы
 //-----------------------------------------------------------------------------
 //Формирование квадратной подматрицы A(0 1 ... k-1 k; 0 1 ... k-1 g), где g > k
 cmatrix U_SubMatrix(ulong k, ulong g) const;
 //Формирование квадратной подматрицы A(0 1 ... k-1 g; 0 1 ... k-1 k), где g > k
 cmatrix L_SubMatrix(ulong g, ulong k) const;
 //Формирование квадратной подматрицы A(0 1 ... k-1 k; 0 1 ... k-1 k)
 cmatrix P_SubMatrix(ulong k) const;

 //Формирование подматрицы из исходной матрицы по индексам столбцов, указанным в
 //векторе ivector
 cmatrix SubMatrixByCols(const ivector& IndexCols, bool Check = false) const;
 //Формирование подматрицы из исходной матрицы по индексам строк, указанным в
 //векторе ivector
 cmatrix SubMatrixByRows(const ivector& IndexRows, bool Check = false) const;
 //Формирование подматрицы из исходной матрицы по индексам строк и столбцов,
 //размещенных в векторах IndexRows & IndexCols
 cmatrix SubMatrix(const ivector& IndexRows, const ivector& IndexCols,
                   bool Check = false) const;
 //-----------------------------------------------------------------------------
 // Присваивание исходной матрице матрицы, вектора-строки, вектора-столбца
 //-----------------------------------------------------------------------------
 cmatrix AssignRow(const cvector& V, ulong row, ulong pos = 0L) const;
 cmatrix AssignRow(const cvector& V, ulong left, ulong right, ulong pos) const;
 cmatrix AssignRow(const cvector& V, const ivector& rows, ulong pos = 0L) const;

 cmatrix AssignCol(const cvector& V, ulong col, ulong pos = 0L) const;
 cmatrix AssignCol(const cvector& V, ulong left = 0L, ulong right = MAX_SIZE,
                   ulong pos = 0L) const;
 cmatrix AssignCol(const cvector& V, const ivector& cols, ulong pos = 0L) const;

 cmatrix Assign(const cmatrix& M, ulong row = 0L, ulong col = 0L) const;

 //-----------------------------------------------------------------------------
 //Умножение матрицы на элементарную матрицу и элементарные матрицы специального
 //                              вида (elementary matrix)
 //ПРАВИЛА УМНОЖЕНИЯ:
 //1. Элементарная матрица может умножаться на исходную матрицу СПРАВА
 //2. Элементарная матрица может умножаться на исходную матрицу СЛЕВА
 //3. Элементарная матрица всегда КВАДРАТНАЯ
 //4. Размерности матрицы и элементарной матрицы должны быть РАВНЫ и отличны от 0
 //ЭЛЕМЕНТАРНАЯ МАТРИЦА - матрица E = I + B, где I - единичная матрица, B - мат-
 //рица ранга единица, т.е. матрица с единичной главной диагональю и одним вне-
 //диагональным элементом отличным от нуля.
 //       |1        |
 //       |  1      |
 // Ekl = |    1    |, E(k,l) != 0, k=4,l=2.
 //       |  x   1  |
 //       |        1|
 //-----------------------------------------------------------------------------
 //Элементарные матрицы специального вида. К ним относятся:
 //1. Диагональная элементарная  матрица Dk (DiagonalEM). Диагональная матрица с
 //   элементами Dk(i,i)=1 для всех i != k и Dk(k,k)!=0.
 //      |1        |
 //      |  1      |
 // Dk = |    x    |, k = 3.
 //      |      1  |
 //      |        1|
 //2. Нижняя столбцовая элементарная матрица Lkc (LowerColumnEM).  Внедиагональ-
 //   ные ненулевые элементы расположены только в k-м столбце под диагональю.
 //       |1        |
 //       |  1      |
 // Lkc = |    1    |, k = 3.
 //       |    x 1  |
 //       |    x   1|
 //3. Верхняя столбцовая элементарная матрица Ukc (UpperColumnEM). Внедиагональ-
 //   ные ненулевые элементы расположены только в k-м столбце над диагональю.
 //       |1   x    |
 //       |  1 x    |
 // Ukc = |    1    |, k = 3.
 //       |      1  |
 //       |        1|
 //4. Полная столбцовая элементарная матрица Tkc (CompleteColumnEM). Внедиагональ-
 //   ные ненулевые элементы расположены только в k-м столбце.
 //       |1   x    |
 //       |  1 x    |
 // Tkc = |    1    |, k = 3.
 //       |    x 1  |
 //       |    x   1|
 //5. Левая строчная элементарная матрица Lkr (LeftRowEM). Внедиагональные нену-
 //   левые элементы расположены только в k-й строке левее диагонали.
 //       |1        |
 //       |  1      |
 // Lkr = |x x 1    |, k = 3.
 //       |      1  |
 //       |        1|
 //6. Правая строчная элементарная матрица Ukr (RightRowEM). Внедиагональные
 //   ненулевые элементы расположены только в k-й строке справа от диагонали.
 //       |1        |
 //       |  1      |
 // Ukr = |    1 x x|, k = 3.
 //       |      1  |
 //       |        1|
 //7. Полная строчная элементарная матрица Tkr (CompleteRowEM). Внедиагональные
 //   ненулевые элементы расположены только в k-й строке.
 //       |1        |
 //       |  1      |
 // Tkr = |x x 1 x x|, k = 3.
 //       |      1  |
 //       |        1|
 //При  умножении  вектора на  элементарную, специальную элементарную или группу
 //специальных  элементарных матриц используется в качестве элементарной обыкно-
 //венная квадратная матрица. Квадратная матрица позволяет размещать группу эле-
 //ментарных специальных матриц. Поэтому  можно  реализовать выполнение операций
 //умножения  вектора  на несколько  элементарных матриц, элементы которых можно
 //хранить в одной квадратной  матрице. Например, квадратная матрица размерности
 //N, позволяет  хранить  одновременно  N DiagonalEM, N LowerColumnEM и N Upper-
 //ColumnEM.
 //-----------------------------------------------------------------------------

 //Умножение на диагональную элементарную матрицу Dk
 //k < Q.Rows(), Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool DiagonalEM(const cmatrix& Q, bool Disp, ulong k);
 //Умножение на диагональные элементарные матрицы Dk, k=[first..last]
 //first < last < Q.Rows(), Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool DiagonalEM(const cmatrix& Q, bool Disp, ulong first, ulong last);
 //Умножение на все диагональные элементарные матрицы Dk, k=[0..Q.Rows()-1]
 //Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool DiagonalEM(const cmatrix& Q, bool Disp);

 //Умножение на нижнюю столбцовую элементарную матрицу Lkc
 //k < Q.Rows(), Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool LowerColumnEM(const cmatrix& Q, bool Disp, ulong k);
 //Умножение на нижние столбцовые элементарные матрицы Lkc, k=[first..last]
 //first < last < Q.Rows(), Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool LowerColumnEM(const cmatrix& Q, bool Disp, ulong first, ulong last);
 //Умножение на все нижние столбцовые элементарные матрицы Lkc, k=[0..Q.Rows()-1]
 //Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool LowerColumnEM(const cmatrix& Q, bool Disp);

 //Умножение на верхнюю столбцовую элементарную матрицу Ukc
 //k < Q.Rows(), Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool UpperColumnEM(const cmatrix& Q, bool Disp, ulong k);
 //Умножение на верхние столбцовые элементарные матрицы Ukc, k=[first..last]
 //first < last < Q.Rows(), Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool UpperColumnEM(const cmatrix& Q, bool Disp, ulong first, ulong last);
 //Умножение на все верхние столбцовые элементарные матрицы Ukc, k=[0..Q.Rows()-1]
 //Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool UpperColumnEM(const cmatrix& Q, bool Disp);

 //Умножение на полную столбцовую элементарную матрицу Tkc
 //k < Q.Rows(), Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool CompleteColumnEM(const cmatrix& Q, bool Disp, ulong k);
 //Умножение на полные столбцовые элементарные матрицы Tkc, k=[first..last]
 //first < last < Q.Rows(), Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool CompleteColumnEM(const cmatrix& Q, bool Disp, ulong first, ulong last);
 //Умножение на все полные столбцовые элементарные матрицы Tkc, k=[0..Q.Rows()-1]
 //Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool CompleteColumnEM(const cmatrix& Q, bool Disp);

 //Умножение на левую строчную элементарную матрицу Lkr
 //k < Q.Rows(), Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool LeftRowEM(const cmatrix& Q, bool Disp, ulong k);
 //Умножение на левые строчные элементарные матрицы Lkr, k=[first..last]
 //first < last < Q.Rows(), Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool LeftRowEM(const cmatrix& Q, bool Disp, ulong first, ulong last);
 //Умножение на все левые строчные элементарные матрицы Lkr, k=[0..Q.Rows()-1]
 //Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool LeftRowEM(const cmatrix& Q, bool Disp);

 //Умножение на правую строчную элементарную матрицу Ukr
 //k < Q.Rows(), Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool RightRowEM(const cmatrix& Q, bool Disp, ulong k);
 //Умножение на правые строчные элементарные матрицы Ukr, k=[first..last]
 //first < last < Q.Rows(), Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool RightRowEM(const cmatrix& Q, bool Disp, ulong first, ulong last);
 //Умножение на все правые строчные элементарные матрицы Ukr, k=[0..Q.Rows()-1]
 //Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool RightRowEM(const cmatrix& Q, bool Disp);

 //Умножение на полную строчную элементарную матрицу Tkr
 //k < Q.Rows(), Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool CompleteRowEM(const cmatrix& Q, bool Disp, ulong k);
 //Умножение на полные строчные элементарные матрицы Tkr, k=[first..last]
 //first < last < Q.Rows(), Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool CompleteRowEM(const cmatrix& Q, bool Disp, ulong first, ulong last);
 //Умножение на все полные строчные элементарные матрицы Tkr, k=[0..Q.Rows()-1]
 //Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool CompleteRowEM(const cmatrix& Q, bool Disp);

 //Умножение на обыкновенную элементарную матрицу Bij
 //Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 //row < Rows() && col < Cols()
 bool OrdinaryEM(const cmatrix& Q, bool Disp, ulong row, ulong col);

//------------------------------------------------------------------------------
//              ОБЪЯВЛЕНИЕ ПЕРЕГРУЖЕННЫХ ОПЕРАТОРОВ
//------------------------------------------------------------------------------
 //1. OVERLOAD THE CALL FUNCTION OPERATOR
 //Объявление перегруженного оператора вызова функции для доступа к компоненте
 //матрицы (двумерного массива). Используется в программах пользователя.
 complex& operator() (ulong i, ulong j);
 //Объявление перегруженного оператора вызова функции для доступа к компоненте
 //матрицы (двумерного массива). Эта версия вызывается в операторе присваивания
 //и в конструкторе копирования.
 //This version is called in ASSIGNMENT OPERATOR and COPY the CONSTRUCTOR
 const complex& operator() (ulong i, ulong j) const;
 //2. OVERLOAD THE ASSIGNMENT OPERATOR
 //Объявление перегруженного оператора присвоения operator =
 bool operator =(const TComplexMatrix& C);
 //3. OVERLOAD THE ASSIGNMENT OPERATOR
 //Объявление перегруженного оператора присваивания operator = компонентам
 //комплексной матрицы действительного значения
 bool operator =(const double value);
 //4. OVERLOAD THE ASSIGNMENT OPERATOR
 //Объявление перегруженного оператора присваивания operator = всем компонентам
 //комплексной матрицы комплексного значения
 bool operator =(const complex value);
 //5. OVERLOAD THE COMPARISON OPERATOR (==)
 //Объявление перегруженного оператора сравнения двух матриц operator ==
 bool operator ==(const TComplexMatrix& A) const;
 //6. OVERLOAD THE STREAM INSERTION OPERATOR
 //Перегрузка операции потокового вывода (<<) для отображения элементов матрицы
 friend ostream& operator <<(ostream& out, const TComplexMatrix& C);

 //*****************************************************************************
 //             Операции Линейной Алгебры над Комплексными Матрицами
 //   Все операции линейной алгебры производятся с копией исходной матрицы.
 //*****************************************************************************
 //Перестановка элементов квадратной матрицы относительно дополнительной диагонали
 bool Reverse();

 //Операция транспонирования и комплексного сопряжения комплексной матрицы
 cmatrix Transpose(bool conjugate = true) const;

 //Перегрузка унарного оператора (!) для получения транспонированной матрицы
 cmatrix operator !() const;

 //Операция сопряжения комплексной матрицы
 cmatrix Conj() const;

 //Перегрузка унарного оператора (~) для получения сопряженной матрицы
 cmatrix operator ~() const;

 //Вычисление следа квадратной комплексной матрицы
 complex Trace() const;
 complex Spur() const {return Trace();};

 //-----------------------------------------------------------------------------
 //Определение норм (мер) комплексной матрицы
 double NFirstNorm() const;
 double FirstNorm() const;
 double SecondNorm() const;
 double ThirdNorm() const;
 double PNorm(uint p = 2) const;
 double Norm(uint type = EUCLIDEAN_NORM, uint p = 2) const;
 //Вычисление нормы строки матрицы с заданным индексом от 0 до Rows()-1
 double NormRow(ulong index, uint type = EUCLIDEAN_NORM, uint p = 2) const;
 //Вычисление нормы столбца матрицы с заданным индексом от 0 до Cols()-1
 double NormCol(ulong index, uint type = EUCLIDEAN_NORM, uint p = 2) const;
 //Вычисление норм всех строк матрицы
 rvector NormAllRows(uint type = EUCLIDEAN_NORM, uint p = 2) const;
 bool NormAllRows(rvector& V, uint type = EUCLIDEAN_NORM, uint p = 2) const;
 //Вычисление норм всех столбцов матрицы
 rvector NormAllCols(uint type = EUCLIDEAN_NORM, uint p = 2) const;
 bool NormAllCols(rvector& V, uint type = EUCLIDEAN_NORM, uint p = 2) const;
 //-----------------------------------------------------------------------------
 //Нормирование элементов матрицы
 bool Normalize(cmatrix& A, uint NormType, uint p = 0) const;
 bool Normalize(uint NormType, uint p = 0);
 bool NormalizeToDB(rmatrix& A, double MindB = -60) const;
 //Нормирование элементов матрицы по строкам
 bool NormalizeRows(cmatrix& A, uint NormType, uint p = 0) const;
 bool NormalizeRows(uint NormType, uint p = 0);
 bool NormalizeRowsToDB(rmatrix& A, double MindB = -60) const;
 //Нормирование элементов матрицы по столбцам
 bool NormalizeCols(cmatrix& A, uint NormType, uint p = 0) const;
 bool NormalizeCols(uint NormType, uint p = 0);
 bool NormalizeColsToDB(rmatrix& A, double MindB = -60) const;

 //Увеличение матрицы на действительное число value
 //B = const + A, B(i,j) = B(i,j) + const
 friend cmatrix operator +(const double value, const cmatrix& A);
 //B = A + const, B(i,j) = A(i,j) + const
 friend cmatrix operator +(const cmatrix& A, const double value);

 //Увеличение матрицы на комплексное число value
 //B = const + A, B(i,j) = B(i,j) + const
 friend cmatrix operator +(const complex value, const cmatrix& A);
 //B = A + const, B(i,j) = A(i,j) + const
 friend cmatrix operator +(const cmatrix& A, const complex value);

 //Уменьшение матрицы на действительное число value
 //B = A - const, B(i,j) = A(i,j) - const
 friend cmatrix operator -(const cmatrix& A, const double value);

 //Уменьшение матрицы на комплексное число value
 //B = A - const, B(i,j) = A(i,j) - const
 friend cmatrix operator -(const cmatrix& A, const complex value);

 //Умножение матрицы на действительное число value
 //B = const * A, B(i,j) = A(i,j) * const
 friend cmatrix operator *(const double value, const cmatrix& A);
 //B = A * const, B(i,j) = A(i,j) * const
 friend cmatrix operator *(const cmatrix& A, const double value);

 //Умножение матрицы на комплексное число value
 //B = const * A, B(i,j) = A(i,j) * const
 friend cmatrix operator *(const complex value, const cmatrix& A);
 //B = A * const, B(i,j) = A(i,j) * const
 friend cmatrix operator *(const cmatrix& A, const complex value);

 //Деление матрицы на действительное число value
 //B = A / const, B(i,j) = A(i,j) / const
 cmatrix operator /(const double value) const;

 //Деление матрицы на комплексное число value
 //B = A / const, B(i,j) = A(i,j) / const
 cmatrix operator /(const complex value) const;

 //Сложение двух комплексных матриц
 //С = A + B, C(i,j) = A(i,j) + B(i,j);
 cmatrix operator +(const cmatrix& A) const;

 //Сложение комплексной и вещественной матриц слева и справа
 friend cmatrix operator +(const rmatrix& A, const cmatrix& B);
 friend cmatrix operator +(const cmatrix& A, const rmatrix& B);

 //Вычитание двух комплексных матриц
 //С = A - B, C(i,j) = A(i,j) - B(i,j);
 cmatrix operator -(const cmatrix& A) const;

 //Вычитание из комплексной матрицы вещественной матрицы
 //С = A - B, C(i,j) = A(i,j) - B(i,j);
 cmatrix operator -(const rmatrix& A) const;

 //Умножение двух комплексных матриц
 cmatrix operator *(const cmatrix& A) const;

 //Умножение вещественной матрицы на комплексную матрицу слева и справа
 friend cmatrix operator *(const rmatrix& A, const cmatrix& B);
 friend cmatrix operator *(const cmatrix& A, const rmatrix& B);

 //Поэлементное умножение столбцов (строк) комплексной матрицы на комплексный
 //вектор одинакового с матрицей размера по столбцам (строкам)
 cmatrix MultByCols(const cvector& V) const;
 cmatrix MultByCols(const cvector& V, ulong index) const;
 cmatrix MultByCols(const cvector& V, ulong left, ulong right) const;
 cmatrix MultByCols(const cvector& V, const ivector& index, bool check = false) const;

 cmatrix MultByRows(const cvector& V) const;
 cmatrix MultByRows(const cvector& V, ulong index) const;
 cmatrix MultByRows(const cvector& V, ulong left, ulong right) const;
 cmatrix MultByRows(const cvector& V, const ivector& index, bool check = false) const;

 //Сложение столбцов (строк) комплексной матрицы с комплексным вектором
 //одинакового с матрицей размера по строкам (столбцам)
 cmatrix AddByCols(const cvector& V) const;
 cmatrix AddByCols(const cvector& V, ulong index) const;
 cmatrix AddByCols(const cvector& V, ulong left, ulong right) const;
 cmatrix AddByCols(const cvector& V, const ivector& index, bool check = false) const;

 cmatrix AddByRows(const cvector& V) const;
 cmatrix AddByRows(const cvector& V, ulong index) const;
 cmatrix AddByRows(const cvector& V, ulong left, ulong right) const;
 cmatrix AddByRows(const cvector& V, const ivector& index, bool check = false) const;

 //Суммирование столбцов (строк) комплексной матрицы
 cvector SumCols(ulong left = 0L, ulong right = MAX_SIZE) const;
 cvector SumCols(const ivector& index) const;

 cvector SumRows(ulong left = 0L, ulong right = MAX_SIZE) const;
 cvector SumRows(const ivector& index) const;

 //-----------------------------------------------------------------------------
 //Расчет миноров исходной матрицы
 //-----------------------------------------------------------------------------
 //Расчет минора вида M(0 1 ... k-1 k; 0 1 ... k-1 g), где g > k
 complex U_Minor(ulong k, ulong g) const;
 //Расчет минора вида M(0 1 ... k-1 g; 0 1 ... k-1 k), где g > k
 complex L_Minor(ulong g, ulong k) const;
 //Расчет минора вида M(0 1 ... k-1 k; 0 1 ... k-1 k) - главный минор
 complex P_Minor(ulong k) const;
 //Расчет всех миноров вида M(0 1 ... k-1 k; 0 1 ... k-1 g), где g > k
 // 0 < g < min(Cols(),Rows()), 0 <= k < g
 cmatrix U_AllMinors() const;
 //Расчет минора вида M(0 1 ... k-1 g; 0 1 ... k-1 k), где g > k
 // 0 < g < min(Cols(),Rows()), 0 <= k < g
 cmatrix L_AllMinors() const;
 //Расчет минора вида M(0 1 ... k-1 k; 0 1 ... k-1 k) - главный минор
 // 0 <= k < min(Cols(),Rows())
 cmatrix P_AllMinors() const;
 cvector& P_AllMinors(cvector& P) const;
 //Расчет всех миноров матрицы
 cmatrix AllMinors(bool L = true, bool P = true, bool U = true) const;

 //-----------------------------------------------------------------------------
 // Разложение комплексных квадратных матриц на нижнюю, верхнюю и диагональную
 // матрицы: A = LDU
 // В алгоритме используется вычисление всех миноров исходной матрицы для расчета
 // коэффициентов нижней L, верхней U и диагональной D матриц. С вычислительной
 // точки зрения этот алгоритм нельзя считать эффективным для разложения матрицы.
 //-----------------------------------------------------------------------------
 bool LDU_Gauss(cmatrix& LDU) const;
 bool LDU_Gauss(cmatrix& L, cmatrix& D, cmatrix& U) const;

 //-----------------------------------------------------------------------------
 //Расчет определителей комплексных квадратных матриц и их разложение на
 //нижнюю и верхнюю треугольную матрицы: A = LU
 //-----------------------------------------------------------------------------
 //Накопление произведений вида Ak*Bk и суммирование их с начальным значением
 friend complex InnerProduct(ulong l, ulong s, ulong u, complex init,
                             const cmatrix& A, bool Avar, ulong Aconst,
                             const cmatrix& B, bool Bvar, ulong Bconst);

 //Алгоритм Краута (модификация компактной схемы Гаусса с частичным выбором
 //главного элемента по столбцу): плотная, несимметрическая квадратная матрица
 bool LU_UnSymmetric(complex& Det, cmatrix& LU, ivector& ExChange, double* Real = NULL,
                     double* Imag = NULL, long* Power = NULL) const;
 complex DET_UnSymmetric(double* Real = NULL, double* Imag = NULL, long* Power = NULL) const;

 //-----------------------------------------------------------------------------
 // Решение комплексных систем линейных уравнений
 //-----------------------------------------------------------------------------
 //Решение системы уравнений AX = B, где A = LU, B - матрица правых частей
 //Матрица A - несимметрическая матрица. Решение проводится по компактной схеме
 //Гаусса (алгоритм Краута).
 cmatrix LE_UnSymSolution(const cmatrix& B, const ivector& ExChange) const;
 friend cmatrix LE_UnSymSolution(const cmatrix& A, const cmatrix& B);

 //-----------------------------------------------------------------------------
 // Обращение матриц
 //-----------------------------------------------------------------------------
 //Обращение несимметрической квадратной матрицы n x n методом разложения ее на
 //треугольные матрицы L & U c помощью алгоритма Краута и решения системы линей-
 //ных уравнений с единичной матрицей размерности n x n в правой части системы
 cmatrix INV_Krauth() const;

 //-----------------------------------------------------------------------------
 //Расчет собственных значений и собственных векторов комплексных матриц
 //-----------------------------------------------------------------------------
 //Алгоритм Якоби: плотная, эрмитовая матрица
 uint EVV_Jacobi(TRealVector& D, TRealMatrix& V, double EPS = 1e-19);
 int EV_Jacobi(TRealVector& D, double EPS = 1e-19, bool lExcludePairVals = true) const;
 //--------------------------------------------------------------------------------
 //Решение полной проблемы собственных значений для комплексных матриц общего вида
 //--------------------------------------------------------------------------------
 //Приведение к форме Хессенберга исходной комплексной матрицы. Реальные и мнимые части
 //матрицы Хессенберга H будут хранится в вещественных матрицах Ar и Ai
 bool HessenbergForm(rmatrix& Ar, rmatrix& Ai, ivector& permut) const;
 //LR-алгоритм: решение полной проблемы собственных значений для комплексной квадратной
 //матрицы с определением ненормированных собственных векторов исходной матрицы
 bool EVV_LR_algorithm(cvector& W, cmatrix& V, double eps = 1e-19) const;
 //Метод Якоби с понижением нормы для комплексных матриц общего вида
 bool EVV_NormReducingJacobi(cvector& W, cmatrix& V, double eps = 1e-19) const;

 //-----------------------------------------------------------------------------
 //Построение корреляционной матрицы
 //-----------------------------------------------------------------------------
 friend bool CorrelationMatrix(const cvector& V, cmatrix& R);
 friend bool CorrelationMatrix(const cmatrix& A, cmatrix& R);

//------------------------------------------------------------------------------
//         ОБЪЯВЛЕНИЕ ДРУЖЕСТВЕННЫХ ФУНКЦИЙ КЛАССУ TComplexMatrix
//------------------------------------------------------------------------------
 //Проверка на равенство размерностей матриц A и B
 friend bool IsEqualSize(const cmatrix& A, const cmatrix& B)
 {return ((A.Rows() == B.Rows()) && (A.Cols() == B.Cols())) ? true : false;};
 //Определение максимальной символьной длины элементов матрицы и максимальной
 //ширины каждого столбца матрицы.
 friend uint* MaxStrLen(const TComplexMatrix& C, uint precision);
 //Вычисление квадратической формы c_val = trW*A*W,
 //Вектор trW комплексно-сопряженный вектору W
 friend bool QuadraticForm(complex& c_val, const cmatrix& A, const cvector& W);
 //Вычисление произведения вида: с_val = L*A*R, где L и R - векторы, согласованные по
 //размеру с матрицей A (A - квадратная матрица)
 friend bool Multiply(complex& c_val, const cvector& L, const cmatrix& A, const cvector& R);
 //Матричное преобразование вида: B(m,m) = trC(m,n)*A(n,n)*C(n,m), trC - транспонированная
 //комплексно-сопряженная матрица к матрице C, A - квадратная матрица
 friend bool Transform_trCAC(cmatrix& B, const cmatrix& A, const cmatrix& C);
 //Вычисление следа матричного преобразования: B(m,m) = trC(m,n)*A(n,n)*C(n,m),
 //trC - транспонированная комплексно-сопряженная матрица к C, A - квадратная матрица
 friend bool Trace_trCAC(double& Trace, const cmatrix& A, const cmatrix& C);

 //Представление матрицы C = Ar + i*Ai, размещением реальных и мнимых частей матрицы C
 //в вещественных матрицах Ar и Ai.
 friend bool ComplexToReal(const cmatrix& C, rmatrix& Ar, rmatrix& Ai);
 //Создание комплексной матрицы C из двух вещественных матриц Ar и Ai, которые содержат
 //вещественные (Ar) и мнимые (Ai) части формируемой матрицы C
 friend bool RealToComplex(cmatrix& C, const rmatrix& Ar, const rmatrix& Ai);

 //Выполнение операции усреднения элементов матрицы B с последующим суммированием с матри-
 //цей A: A = A + B/n, n - число усреднений
 friend bool Average(cmatrix& A, const cmatrix& B, ulong n);
 //Выполнение операции непрерывного усреднения без задания общего числа усреднений
 //На каждом k-ом шаге усреднения получаем матрицу AvX усредненную за k шагов (k >= 0)
 friend bool InfiniteAverage(cmatrix& AvX, const cmatrix& Xk, ulong k);

 //Операция комплексного сопряжения элементов матрицы C
 friend bool Conj(cmatrix& C);
 //Выполнение комплексного сопряжения элементов матрицы B с присвоением результата A
 //A = Conj(B), элементы матрицы B не изменяются.
 friend bool Conj(cmatrix& A, const cmatrix& B);

//------------------------------------------------------------------------------
//II. Объявление закрытых членов-функций класса TComplexMatrix
//------------------------------------------------------------------------------
private:
 void Initial();//Инициализация членов-данных класса при его конструировании
 //Создание матрицы определенного размера в оперативной памяти без инициализации
 bool Create();
 void Delete();//Удаление матрицы из оперативной памяти

//------------------------------------------------------------------------------
//III. Объявление защищенных членов-функций класса TComplexMatrix
//------------------------------------------------------------------------------
protected:
 //*****************************************************************************
 // Объявление функций консольного ввода матриц различного типа
 //*****************************************************************************
 //Ввод элементов комплексной матрицы (прямоугольной или квадратной)
 void InPutComplexMatrix(const string& name, bool confirm = true);
 //Ввод элементов эрмитовой матрицы
 void InPutHermitianMatrix(const string& name, bool confirm = true);
 //Ввод элементов диагональной матрицы
 void InPutDiagonal(const string& name, bool confirm = true);
 //Ввод элементов скалярной матрицы
 void InPutScalar(const string& name, bool confirm = true);
 //Ввод элементов ленточной матрицы
 void InPutBand(ulong diags, const string& name, bool confirm = true);
 //Ввод элементов верхней треугольной матрицы
 void InPutUTriang(const string& name, bool confirm = true);
 //Ввод элементов нижней треугольной матрицы
 void InPutLTriang(const string& name, bool confirm = true);
 //Ввод элементов симметрической матрицы
 void InPutSymmetric(const string& name, bool confirm = true);
 //Ввод элементов кососимметрической матрицы
 void InPutSkewSymmetric(const string& name, bool confirm = true);
 //Ввод элементов знакопеременной матрицы
 void InPutAltSign(const string& name, bool confirm = true);
 //*****************************************************************************
 //Объявление функций инициализации элементов матриц различного типа
 //*****************************************************************************
 void InitZeroMatrix();
 void InitMatrix();
 void InitDiagonal();
 void InitScalar();
 void InitUnit();
 void InitBand(ulong diags);
 void InitUTriang();
 void InitLTriang();
 void InitSymmetric();
 void InitSkewSymmetric();
 void InitAltSign();
 void InitHermitian();
};
/**** КОНЕЦ ОБЪЯВЛЕНИЯ КЛАССА - КОМПЛЕКСНАЯ МАТРИЦА - class TComplexMatrix ****/

//***********************************************************************************
//                Объявление класса - МАТРИЦА ЦЕЛЫХ ЧИСЕЛ - TIntMatrix
//TIntMatrix создает двумерную матрицу целых чисел типа long необходимой размерности,
//выполняет основные операции линейной алгебры над такими матрицами и ее элементами,
//а также обеспечивает ряд других сервисных функций.
//***********************************************************************************
class TIntMatrix {
//Объявление дружественных классов

//-----------------------------------------------------------------------------------
//Объявление защищенных членов-данных класса
//-----------------------------------------------------------------------------------
protected:
 bool type;//тип матрицы (квадратная или прямоугольная)
 ulong m;  //число строк матрицы/число столбцов для квадратных матриц
 ulong n;  //число столбцов матрицы/число диагоналей для ленточной матрицы
 long **data; //Указатель на двумерный массив хранения компонент матрицы
//-----------------------------------------------------------------------------------
//I. Объявление общедоступных членов-функций класса TIntMatrix
//-----------------------------------------------------------------------------------
public:
 //**********************************************************************************
 //     ОБЪЯВЛЕНИЕ КОНСТРУКТОРОВ И ДЕСТРУКТОРА КЛАССА TIntMatrix
 //**********************************************************************************
 TIntMatrix() {Initial();}; //Объявление конструктора по умолчанию
 //----------------------------------------------------------------------------------
 //Объявление конструкторов с аргументами
 //----------------------------------------------------------------------------------
 //Конструирование прямоугольных (m != n) или квадратных матриц (m == n)
 TIntMatrix(ulong row, ulong col, long value) {Initial(); Set(row,col,value);};
 TIntMatrix(ulong row, ulong col, bool init)  {Initial(); Set(row,col,init);};
 //Конструирование квадратных матриц (m == n)
 TIntMatrix(ulong size, long value) {Initial(); Set(size,value);};
 TIntMatrix(ulong size, bool init)  {Initial(); Set(size,init);};
 TIntMatrix(ulong size, uint sq_type, ulong diag = 3L)
 {Initial();Set(size, sq_type, diag);};
 //Объявление конструктора копирования
 TIntMatrix(const TIntMatrix& A) {Initial(); *this = A;};
 //Объявление деструктора класса
 ~TIntMatrix() {Delete();};
 //----------------------------------------------------------------------------------
 //ОБЪЯВЛЕНИЕ ФУНКЦИЙ ОБЩЕГО НАЗНАЧЕНИЯ КЛАССА TIntMatrix
 //----------------------------------------------------------------------------------
 //**********************************************************************************
 //                         ПОЛУЧЕНИЕ СВОЙСТВ МАТРИЦЫ
 //**********************************************************************************
 //----------------------------------------------------------------------------------
 //1. Определение типа и подтипа матрицы
 bool Type() const {return type;};
 bool GetType() const {return type;};
 //Проверка матрицы на квадратную
 bool IsSquare() const {return (type == SQUARE) ? true : false;};
 //Проверка матрицы на прямоугольную
 bool IsRectang() const {return (type == RECTANG) ? true : false;};
 //Проверка на матрицу нулевого размера
 bool IsZeroSize() const {return ((Rows() == 0L) || (Cols() == 0L)) ? true : false;};
 bool IsZero() const;           //Проверка на матрицу с нулевыми элементами
 bool IsSymmetric() const;      //Проверка матрицы на симметричность
 bool IsSkewSymmetric() const;  //Проверка матрицы на кососимметричность
 bool IsAltSign() const;        //Проверка матрицы на знакопеременность
 bool IsUpperTriang() const;    //Проверка на верхнюю треугольную матрицу
 bool IsLowerTriang() const;    //Проверка на нижнюю треугольную матрицу
 bool IsDiagonal() const;       //Проверка на диагональную матрицу
 bool IsScalar() const;         //Проверка на скалярную матрицу
 bool IsUnit() const;           //Проверка на единичную матрицу
 bool IsBand(ulong& diags) const;//Проверка на ленточную матрицу
 uint SubType() const; //Определение подтипа матрицы
 uint SubType(ulong& n) const; //Определение подтипа матрицы
 //----------------------------------------------------------------------------------
 //2. Определение размерности матрицы
 //Определение количества строк матрицы
 ulong Rows() const {return m;};
 //Определение количества столбцов матрицы
 ulong Cols() const {return (type == SQUARE) ? m : n;};
 //----------------------------------------------------------------------------------
 //3. Получение свойств матрицы
 void Properties(ulong& rows, ulong& cols, bool& type) const
 {rows = Rows(); cols = Cols(); type = GetType();};
 void Properties(ulong& rows, ulong& cols, bool& type, uint& subtype) const;
 void Properties(PF_MATRIX& Info) const;
 string& Properties(string& s) const;//Свойства матрицы в символьном виде
 //Получение наименования типа матрицы (квадратная или прямоугольная)
 string& GetType(string& s, uint language = ENG) const;
 //Получение наименования подтипа матрицы
 string& GetSubType(string& s, uint language = ENG) const;
 //Размещение элементов матрицы в символьной строке
 string& GetData(string& s, string& delim) const;
 //Определение максимальной символьной длины элементов матрицы и максимальной
 //ширины каждого столбца матрицы.
 uint WidthCols(ivector& Width) const;

 //**********************************************************************************
 //                    УСТАНОВКА СВОЙСТВ МАТРИЦЫ
 //**********************************************************************************
 bool Set(ulong rows, ulong cols, long value);
 bool Set(ulong rows, ulong cols, bool init);
 bool Set(ulong size, long value) {return Set(size, size, value);};
 bool Set(ulong size, bool init) {return Set(size, size, init);};
 bool Set(ulong size, uint sq_type, ulong diags = 3L);
 bool Set(const PF_MATRIX& Info);
 //**********************************************************************************
 // ИНИЦИАЛИЗАЦИЯ ЭЛЕМЕНТОВ МАТРИЦЫ СЛУЧАЙНЫМИ ЗНАЧЕНИЯМИ
 //**********************************************************************************
 bool Rand();//По равновероятному закону
 bool Rand(long A, long B);//По равновероятному закону в интервале
 //Консольное редактирование (ввод) элементов матрицы
 //void Edit(bool confirm = true, const char* name = "A");

 //**********************************************************************************
 // ФУНКЦИИ ДОСТУПА К ЭЛЕМЕНТАМ МАТРИЦЫ
 //**********************************************************************************
 //Получение значения элемента матрицы
 double Get(ulong row, ulong col) const {return data[row][col];};
 //Присвоение значения элементу матрицы
 void Put(ulong row, ulong col, double value) {data[row][col] = value;};

 //Получение строки матрицы с заданным индексом от 0 до Rows()-1
 ivector Row(ulong index) const;
 bool Row(ivector& V, ulong index) const;
 //Получение столбца матрицы с заданным индексом от 0 до Cols()-1
 ivector Col(ulong index) const;
 bool Col(ivector& V, ulong index) const;

 //Перестановка двух строк матрицы
 bool ExchangeRows(ulong i, ulong j);
 //Перестановка строк матрицы согласно вектору-перестановок
 bool ExchangeRows(const ivector& permutation);
 //Перестановка двух столбцов матрицы
 bool ExchangeCols(ulong i, ulong j);
 //Перестановка столбцов матрицы согласно вектору-перестановок
 bool ExchangeCols(const ivector& permutation);
 //----------------------------------------------------------------------------------
 //Перестановка элементов матрицы в заданном интервале столбцов (строк)
 //в обратном порядке по столбцам (по строкам)
 //----------------------------------------------------------------------------------
 imatrix& ReverseByCol(imatrix& A, ulong left = 0L, ulong right = MAX_SIZE) const;
 imatrix& ReverseByRow(imatrix& A, ulong left = 0L, ulong right = MAX_SIZE) const;
 bool ReverseCols();
 bool ReverseRows();
 //----------------------------------------------------------------------------------
 //Формирование подматриц исходной матрицы
 //----------------------------------------------------------------------------------
 //Формирование квадратной подматрицы A(0 1 ... k-1 k; 0 1 ... k-1 g), где g > k
 imatrix& U_SubMatrix(imatrix& SubMatrix, ulong k, ulong g) const;
 //Формирование квадратной подматрицы A(0 1 ... k-1 g; 0 1 ... k-1 k), где g > k
 imatrix& L_SubMatrix(imatrix& SubMatrix, ulong g, ulong k) const;
 //Формирование квадратной подматрицы A(0 1 ... k-1 k; 0 1 ... k-1 k)
 imatrix& P_SubMatrix(imatrix& SubMatrix, ulong k) const;

 //Формирование подматрицы из исходной матрицы по индексам столбцов, указанным в
 //векторе ivector
 imatrix& SubMatrixByCols(imatrix& SubMatrix, const ivector& IndexCols,
                          bool Check=false) const;
 //Формирование подматрицы из исходной матрицы по индексам строк, указанным в
 //векторе ivector
 imatrix& SubMatrixByRows(imatrix& SubMatrix, const ivector& IndexRows,
                          bool Check=false) const;
 //Формирование подматрицы из исходной матрицы по индексам строк и столбцов,
 //размещенных в векторах IndexRows & IndexCols
 imatrix& SubMatrix(imatrix& SubMatrix,  const ivector& IndexRows,
                    const ivector& IndexCols, bool Check = false) const;
 //----------------------------------------------------------------------------------
 // Присваивание исходной матрице матрицы, вектора-строки, вектора-столбца
 //----------------------------------------------------------------------------------
 imatrix& AssignRow(imatrix& A, const ivector& V, ulong row, ulong pos = 0L) const;
 imatrix& AssignRow(imatrix& A, const ivector& V, ulong left, ulong right, ulong pos) const;
 imatrix& AssignRow(imatrix& A, const ivector& V, const ivector& rows, ulong pos = 0L) const;

 imatrix& AssignCol(imatrix& A, const ivector& V, ulong col, ulong pos = 0L) const;
 imatrix& AssignCol(imatrix& A, const ivector& V, ulong left = 0L, ulong right = MAX_SIZE,
                    ulong pos = 0L) const;
 imatrix& AssignCol(imatrix& A, const ivector& V, const ivector& cols, ulong pos = 0L) const;

 imatrix& Assign(imatrix& A, const imatrix& B, ulong row = 0L, ulong col = 0L) const;

 //----------------------------------------------------------------------------------
 //              ОБЪЯВЛЕНИЕ ПЕРЕГРУЖЕННЫХ ОПЕРАТОРОВ
 //----------------------------------------------------------------------------------
 //1. OVERLOAD THE CALL FUNCTION OPERATOR
 //Объявление перегруженного оператора вызова функции для доступа к компоненте
 //матрицы (двумерного массива). Используется в программах пользователя.
 long& operator() (ulong i, ulong j);
 //Объявление перегруженного оператора вызова функции для доступа к компоненте
 //матрицы (двумерного массива). Эта версия вызывается в операторе присваивания
 //и в конструкторе копирования.
 //This version is called in ASSIGNMENT OPERATOR and COPY the CONSTRUCTOR
 const long& operator() (ulong i, ulong j) const;
 //2. OVERLOAD THE ASSIGNMENT OPERATOR
 //Объявление перегруженного оператора присваивания operator=
 bool operator =(const TIntMatrix& A);
 //3. Объявление перегруженного оператора operator= извлечение из строки символов
 //   целых чисел и и присвоение их целочисленной матрице
 bool operator =(const string& s);
 //4. OVERLOAD THE ASSIGNMENT OPERATOR
 //Объявление перегруженного оператора присваивания operator = компонентам
 //вещественной матрицы действительного значения
 bool operator =(const long value);
 //5. OVERLOAD THE COMPARISON OPERATOR (==)
 //Объявление перегруженного оператора сравнения двух матриц operator ==
 bool operator ==(const TIntMatrix& A) const;
 //6. OVERLOAD THE STREAM INSERTION OPERATOR
 //Перегрузка операции потокового вывода (<<) для отображения элементов матрицы
 friend ostream& operator <<(ostream& out, const TIntMatrix& A);

 //**********************************************************************************
 //             Операции Линейной Алгебры над Целочисленными Матрицами
 //   Все операции линейной алгебры производятся с копией исходной матрицы.
 //**********************************************************************************
 //Перестановка элементов квадратной матрицы относительно дополнительной диагонали
 bool Reverse();
 imatrix& Reverse(imatrix& A) const;
 //Операция транспонирования матрицы
 bool Transpose();
 imatrix& Transpose(imatrix& A) const;
 //Перегрузка унарного оператора (!) для получения транспонированной матрицы
 imatrix& operator !() const;
 //Вычисление следа квадратной матрицы
 double Trace() const;
 double Spur() const {return Trace();};
 //----------------------------------------------------------------------------------
 //Определение норм (мер) матрицы
 double NFirstNorm() const;
 double FirstNorm() const;
 double SecondNorm() const;
 double ThirdNorm() const;
 double PNorm(uint p = 2) const;
 double Norm(uint type = EUCLIDEAN_NORM, uint p = 2) const;

 //Увеличение матрицы на целое число value
 //B = const + A, B(i,j) = B(i,j) + const
 friend imatrix operator +(const long value, const imatrix& A);
 //B = A + const, B(i,j) = A(i,j) + const
 friend imatrix operator +(const imatrix& A, const long value);

 //Уменьшение матрицы на целое число value
 //B = A - const, B(i,j) = A(i,j) - const
 friend imatrix operator -(const imatrix& A, const long value);

 //Умножение матрицы на целое число value
 //B = const * A, B(i,j) = A(i,j) * const
 friend imatrix operator *(const long value, const imatrix& A);
 //B = A * const, B(i,j) = A(i,j) * const
 friend imatrix operator *(const imatrix& A, const long value);

 //Деление матрицы на целое число value
 //B = A / const, B(i,j) = A(i,j) / const
 rmatrix operator /(const long value) const;

 //Сложение двух целочисленных матриц
 //С = A + B, C(i,j) = A(i,j) + B(i,j);
 imatrix operator +(const imatrix& A) const;

 //Вычитание двух целочисленных матриц
 //С = A - B, C(i,j) = A(i,j) - B(i,j);
 imatrix operator -(const imatrix& A) const;

 //Умножение двух целочисленных матриц
 imatrix operator *(const imatrix& A) const;

 //Умножение целочисленного вектора матрицу слева и справа
 //Умножение на матрицу слева
 friend ivector operator *(const ivector& V, const imatrix& A);
 //Умножение на матрицу справа
 friend ivector operator *(const imatrix& A, const ivector& V);

//-----------------------------------------------------------------------------------
//         ОБЪЯВЛЕНИЕ ДРУЖЕСТВЕННЫХ ФУНКЦИЙ КЛАССУ TIntMatrix
//-----------------------------------------------------------------------------------
//Табличное форматирование данных матрицы A с формированием заголовка таблицы по столбцам
//в строке sTbl типа string
friend string& MakeTable(string& sTbl, const UStringMap& HeadColl, const imatrix& A,
       string& delim = string(1,' '));

//-----------------------------------------------------------------------------------
//II. Объявление закрытых членов-функций класса TIntMatrix
//-----------------------------------------------------------------------------------
private:
 void Initial();//Инициализация членов-данных класса при его конструировании
 //Создание матрицы определенного размера в оперативной памяти без инициализации
 bool Create();
 void Delete();//Удаление элементов матрицы из оперативной памяти

//-----------------------------------------------------------------------------------
//III. Объявление защищенных членов-функций класса TIntMatrix
//-----------------------------------------------------------------------------------
protected:
 //**********************************************************************************
 //Объявление функций инициализации элементов матриц различного типа
 //**********************************************************************************
 void InitZeroMatrix();
 void InitMatrix();
 void InitDiagonal();
 void InitScalar();
 void InitUnit();
 void InitBand(ulong diags);
 void InitUTriang();
 void InitLTriang();
 void InitSymmetric();
 void InitSkewSymmetric();
 void InitAltSign();
};
/******* КОНЕЦ ОБЪЯВЛЕНИЯ КЛАССА - ЦЕЛОЧИСЛЕННАЯ МАТРИЦА - class TIntMatrix ********/


//***********************************************************************************
//Объявление структуры соответствия символьной строке значения беззнакового целого -
//STR_UINT
//***********************************************************************************
typedef struct
{ uint value; //Сигнатура (значение) символьной строки
  char* str;  //Символьная строка
} STR_UINT;

typedef STR_UINT* pSTR_UINT; //Указатель на структуру STR_UINT

//***********************************************************************************
//Объявление структуры таблицы соответствий символьных строк значениям беззнаковых
//целых чисел - TB_STR_UINT
//***********************************************************************************
typedef struct
{ ulong size;    //Количество записей в таблице
  pSTR_UINT rec; //Таблица соответствий символьных строк положительным числам
} TB_STR_UINT;

typedef TB_STR_UINT* pTB_STR_UINT; //Указатель на структуру TB_STR_UINT

//***********************************************************************************
// Объявление сервисных функций
//***********************************************************************************
//Вычисление модуля комплексного числа Xr + i*Xi по формуле:
//cabs(X) = sqrt(Xr*Xr + Xi*Xi)
double cabs(double Xr, double Xi);
//Деление двух комплексных чисел
//Z = X/Y: Zr + i*Zi = (Xr + i*Xi)/(Yr + i*Yi)
complex cdiv(double Xr, double Xi, double Yr, double Yi);
//Извлечение квадратного корня из комплексного числа
//Y = sqrt(X): Yr + i*Yi = sqrt(Xr + i*Xi)
complex csqrt(double Xr, double Xi);

//Функция замены в строке s символов из символьного множества charset на строку str
ulong ReplaceCharSet(string& s, const string& charset, const string& str);
//Функция преобразования строки в число типа complex
bool StrToComplex(complex& z, const string& s, string& ends);
//Функция редактирования (ввода) комплексного числа
complex EditComplex(bool confirm = true);
//Функция преобразования строки в число типа double
bool StrToDouble(double& z, const string& s, string& ends);
//Функция редактирования (ввода) вещественного числа
double EditDouble(bool confirm = true);
//Функция преобразования строки в число типа long
bool StrToLong(long& z, const string& s, string& ends);
//Функция редактирования (ввода) длинного целого числа
long EditLong(bool confirm = true);
//Функция ввода характеристик действительной матрицы
void InPutPF_RMATRIX(PF_MATRIX& Info, const char* Name = NULL);
//Функция ввода характеристик комплексной матрицы
void InPutPF_CMATRIX(PF_MATRIX& Info, const char* Name = NULL);
//Консольная функция выбора записи из таблицы типа TB_STR_UINT по содержимому
//символьной строки
uint ConsoleChoice(const TB_STR_UINT& Table, uint Default,
                   const char* Header = NULL, uint Offset = 0);
//Получение символьной строки из таблицы типа TB_STR_UINT по полю value
const char* GetName(const TB_STR_UINT& Table, uint Value);
void ClrScr(uint x, uint y);

#endif //ЗАВЕРШЕНИЕ ОБЪЯВЛЕНИЯ ЗАГОЛОВОЧНОГО ФАЙЛА "MATRIX.H"