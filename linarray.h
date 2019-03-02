//linarray.h
//Объявление класса TLinearArray - ЛИНЕЙНАЯ АНТЕННАЯ РЕШЕТКА (LINEAR ARRAY)
//Initial date: October,1,1998. Final date:
//Copyright (c) JohnSoft 1998. All rights reserved. Borland C++ 5.01
//Заголовочный файл linarray.h объявляет класс TLinearArray, который описывает
//основные свойства линейной антенной решетки (linear antenna array). Данный
//класс является ПРОИЗВОДНЫМ КЛАССА TElArray, который описывает свойства одного
//элемента антенной решетки. Линейная антенная решетка представляет из себя со-
//вокупность (массив) антенн, которые расположены в одну линию.

#if !defined (__LINARRAY_H) //Предотвращение повторного объявления файла
#define __LINARRAY_H

#include "elarray.h" //Объявление свойств базового для класса TLinearArray
                     //класса TElArray
#define CONSOLE 1    //В программе используется консольный ввод-вывод на экран

//Объявление структуры, содержащую данные по антенне, которые являются необходи-
//мыми для считывания данных по ней из текстового файла.
typedef struct
{ const char** name; //массив возможных наименований антенны
  uint sz_name; //кол-во наименований антенны в массиве name
  uint type; //идентификатор типа антенны

  //Характеристика направленности антенны в E-плоскости
  uint E_total;//размер списка параметров
  uint E_read;//кол-во параметров, которые необходимо считать из файла
  const char** EParam; //массив наименований вводимых параметров для функции ELaw
  uint sz_EParam; //кол-во наименований в EParam

  //Характеристика направленности антенны в H-плоскости
  uint H_total;//размер списка параметров
  uint H_read;//кол-во параметров, которые необходимо считать из файла
  const char** HParam; //массив наименований вводимых параметров для функции HLaw
  uint sz_HParam; //кол-во наименований в HParam

} ANTENNA_INFO;

typedef const ANTENNA_INFO* pANT;//указатель на структуру ANTENNA_INFO

//Объявление структуры, содержащую информацию по закону
//В списке ARGLIST->List[0] - всегда хранится текущий аргумент функции law,
//далее следуют параметры закона (функции). Аргумент величина переменная и, как
//правило, она должна изменяться во время моделирования. Параметры в общем слу-
//чае являются неизменными величинами во время моделирования, за исключением ра-
//чих параметров, которые могут изменяться во времени, для описания, например,
//импульсного сигнала. Для функций линейной АР - LEFTDISTLAW и RIGHTDISTLAW в
//ARGLIST->List[0] будет храниться расстояние между [i+1,i] элементами решетки
//на основе которого по закону law можно будет вычислить расстояние между [i+2,i+1]
//элементами АР. Первоначальное расстояние между основным и первым от основного
//элемента АР, расположенным слева (справа) от него, должно быть задано в списке
//параметров закона и будет играть роль начального значения переменной.
typedef struct
{ const char** pattern; //массив возможных наименований закона
  uint  sz_pattern; //кол-во наименований закона в массиве pattern
  uint ident; //идентификатор закона
  double (*law)(const ARGLIST* arg); //указатель на функцию ассоциированную с законом
  uint total_arg;//общее число параметров с аргументов, необходимых для задания
                 //функции law
  uint read_arg; //кол-во параметров функции, которые д.б. считаны (например,
                 //из файлового потока)
  uint offset;   //начальная позиция в списке аргументов ARGLIST с которой нач-
                 //нется запись считанных аргументов
  const char* math_notate; //Математическая запись закона
  bool type; //Случайный или детерминированный закон
} INFO_LAW;

typedef const INFO_LAW* pINFO_LAW;//указатель на структуру INFO_LAW

//Объявление структуры, которая содержит информацию по положению ключевое строки
//в строке (левая и правая граница ключевой строки) и ее индексе в массиве строк
//типа const char** Pattern. Используется функцией SearchKeyString и применяется
//для синтаксического анализа данных.
typedef struct
{ uint index; //индекс ключевой строки в массиве строк
  uint lpos;  //левая граница ключевой строки в заданной строке
  uint rpos;  //правая граница ключевой строки в заданной строке
} KEYSTRPOS;

typedef KEYSTRPOS* pKEYSTRPOS; //Указатель на структуру KEYSTRPOS

//Объявление структуры, в которой строке сhar* ставится в соответствие булево
//значение. Используется для получения логических значений по символьным строкам
typedef struct
{ char* s; //указатель на символьную строку
  bool value; //логическое значение данной строки
} STR_BOOL;

typedef const STR_BOOL* pSTR_BOOL; //Указатель на структуру STR_BOOL

//Объявление таблицы соответствий символьных строк булевым значениям
typedef struct
{ uint size; //размер таблицы
  STR_BOOL* table; //указатель на структуру STR_BOOL
} TABLEStrBool;

//Свойство однородности антенной решетки - UNIFORMITY PROPERTY
const bool UNIFORM    = true;  //Однородная антенная решетка
const bool NONUNIFORM = false; //Неоднородная антенная решетка

//Свойство эквидистантности антенной решетки - EQUIDISTANCE PROPERTY
const bool EQUIDISTANT   = true; //Эквидистантная антенная решетка
const bool UNEQUIDISTANT = false;//Неэквидистантная антенная решетка
const bool EQUAL   = true;       //Эквидистантная антенная решетка
const bool UNEQUAL = false;      //Неэквидистантная антенная решетка

//Объявление констант, связанных с ошибками при конструировании антенной решетки
//по данным, находящимся в текстовом файле специального вида.
const int FOPEN_ERROR = -1; //Ошибка открытия файла данных
const int FREAD_ERROR = -2; //Ошибка при считывании данных из файла

//****************************************************************************
// ОБЪЯВЛЕНИЕ КЛАССА - TLinearArray - ЛИНЕЙНАЯ АНТЕННАЯ РЕШЕТКА
//****************************************************************************
class TLinearArray {
//-------------------------------------------------------------
// ОБЪЯВЛЕНИЕ ЗАЩИЩЕННЫХ ЧЛЕНОВ-ДАННЫХ КЛАССА TLinearArray
//-------------------------------------------------------------
protected:
 //Linear array property - Свойства линейной антенной решетки
 bool Uniformity;   //Однородность линейной антенной решетки
 bool Equidistance; //Эквидистантность линейной антенной решетки

 //Configuration linear array - Конфигурация линейной антенной решетки
 uint Left; //Кол-во элементов АР, расположенных слева от основного элемента АР
 uint Together; //Кол-во элементов АР, расположенных вместе с основным элементом
 uint Right;//Кол-во элементов АР, расположенных справа от основного элемента АР

 //Disposition of an linear array of a rather terrestrial surface.
 //Расположение линейной антенной решетки относительно земной поверхности
 bool Disposition; //EQUATOR or MERIDIAN plane

 //The disposition laws of the antenna elements from each other.
 //Законы, определяющие относительные расстояния, между двумя соседними элемен-
 //тами линейной антенной решетки. Могут быть заданы аналитически или таблично.

 //Объявление закона расположения элементов АР относительно друг друга, которые
 //расположены левее основного элемента (если такие элементы имеются)
 uint LeftLawType;
 double (*LeftLaw)(const ARGLIST* Arg);
 ARGLIST* LeftLawArg;
 //Объявление закона расположения элементов АР относительно друг друга, которые
 //расположены правее основного элемента (если такие элементы имеются)
 uint RightLawType;
 double (*RightLaw)(const ARGLIST* Arg);
 ARGLIST* RightLawArg;

 ulong Count; //Количество элементов АР
 TElArray* data; //Указатель на массив с элементами АР.

 //Имя файла с исходными данными по Linear Array
 string fname;
 //Имя файла регистрации возможных ошибок в fname
 string ferrname;
 //Объявление входного файлового потока с данными по TLinearArray
 ifstream fin;
 //Объявление выходного файлового потока регистрации возможных ошибок в fin
 ofstream ferr;

//------------------------------------------------------------------------------
// I. ОБЪЯВЛЕНИЕ ОБЩЕДОСТУПНЫХ ЧЛЕНОВ-ФУНКЦИЙ КЛАССА TLinearArray
//------------------------------------------------------------------------------
public:
 //Получение характеристик линейной АР в виде строкового объекта string
 virtual string& Properties(string& s, bool headfoot = ON);
 //Получение характеристики элемента линейной АР в виде строкового объекта string
 string& LAAElemProperties(string& s, ulong index, bool headfoot = ON);
 //Определение количества элементов в АР
 ulong GetTotal() const {return Count;};
 uint GetLeft() const {return Left;};
 uint GetTogether() const {return Together;};
 uint GetRight() const {return Right;};
 //Определение расположения линейной антенной решетки относительно земной
 //поверхности
 bool GetDisp() const {return Disposition;};
 //Определение относительного расстояния между основным и вспомогательным эле-
 //ментами АР с порядковым номером i.
 double GetL(ulong i) const
  {if ((i > Count) || (i == 0)) return 0.0; else return data[i].GetL();};
 //Определение возможности использования в расчетах элемента АР с порядковым
 //номером i.
 bool GetUsage(ulong i) const
  {if (i > Count) return OFF; else return data[i].GetUsage();};
 //Определение относительного коэффициента усиления элемента АР с порядковым
 //номером i.
 double GetN(ulong i) const
  {if (i > Count) return 0.0; else return data[i].GetN();};
 //Вычисление значения нормированной диаграммы направленности i-го элемента АР
 //в направлении источника сигнала
 double CalcAntPatterns(ulong i, bool EarthPlane) const
  {if (i > Count) return 0.0; else return data[i].CalcAntPatterns(EarthPlane);};

 //Расчет относительных расстояний между соседними элементами АР, а также между
 //основным и вспомогательными элементами АР
 void CalcRelDistance();
 //Установка углов источника сигнала, соответствующих E-плоскости и H-плоскости
 //для всех элементов АР, используемых в расчетах
 void SetAngle(double AzimuthAngle, double ElevateAngle, bool Deg = DEG);
 //Установка углов источника сигнала, соответствующих E-плоскости и H-плоскости
 //для i-го элемента АР
 void SetAngle(ulong i, double AzimuthAngle, double ElevateAngle, bool Deg = DEG)
 { if (i < Count) data[i].SetAngle(AzimuthAngle,ElevateAngle,Deg);};
 //Получение случайных значений амплитуды и фазы для шума на выходе элементов
 //антенной решетки, используемых в расчетах
 void SetRandNumber();
 //Вычисление диаграммы направленности линейной решетки в заданном секторе эква-
 //ториальной или меридиональной плоскости Земли
 rvector AntPatterns(bool EarthPlane, double ScanAngle = 0,
         double InitAngle = -90, double EndAngle = 90, double Step = 0.1,
         bool Deg = DEG);

//*******************************************************************
// ОБЪЯВЛЕНИЕ КОНСТРУКТОРОВ И ДЕСТРУКТОРА КЛАССА TLinearArray
//*******************************************************************
 //Объявление конструктора по умолчанию
 TLinearArray();
 //Объявление конструктора с аргументами
 //Создание объекта класса TLinearArray по данным из файла filename
 TLinearArray(const char* filename);
 //Объявление деструктора класса
 virtual ~TLinearArray();

//-------------------------------------------------------------------
// II. ОБЪЯВЛЕНИЕ ЗАКРЫТЫХ ЧЛЕНОВ-ФУНКЦИЙ КЛАССА TLinearArray
//-------------------------------------------------------------------
private:

//-------------------------------------------------------------------
// III. ОБЪЯВЛЕНИЕ ЗАЩИЩЕННЫХ ЧЛЕНОВ-ФУНКЦИЙ КЛАССА TLinearArray
//-------------------------------------------------------------------
protected:
 //Начальная инициализация характеристик Linear Array
 virtual void InitPerformance();
 //Сброс всех характеристик Linear Array
 virtual void ResetPerformance();

 //*****************************************************************************
 //Объявление функций, предназначенных для считывания характеристик линейной АР
 //при ее конструировании из текстового файла специального вида
 //*****************************************************************************
 bool OpenDataFile(const char* filename);
 bool OpenFileFromConsole();
 bool GetLineWithoutComment(ifstream& fin, string& line, const string& ABC,
      const char** comment = NULL, const uint sz_comment = 0);
 bool SearchKeyString(const string& s, const char** pattern,
      const uint sz_pattern, pKEYSTRPOS = NULL);
 void Display(ofstream& out, const char** s, const uint sz_s, const char* title = NULL,
              const char* left = NULL, const char* right = NULL, bool num = true);
 bool FindKeyStrInFile(const string& TitleKeyStr, const char** KeyStr,
      const uint sz_KeyStr, string& Line, const pKEYSTRPOS info,
      const string& ABC, const char** Comment, const uint sz_Comment,
      ifstream& fin,ofstream& ferr,const char** Perform, const uint sz_Perform);
 bool StrToBool(bool& value, const string& line, const pKEYSTRPOS info,
      const pSTR_BOOL TableStrBool, const uint sz_TableStrBool,
      const string& title,const char** Perform,const uint sz_Perform);
 bool GetNextStrPosAfterSep(const string& line, int& lpos, const char* sep = NULL);
 bool StrToDouble(double& value, const string& line, const char** pattern,
      const uint sz_pattern, const string& title, const char** perform,
      const uint sz_perform);
 bool GetLawFromStr(const INFO_LAW* Table, const uint sz_Table, uint& index,
      const string& title, const char** perform, const uint sz_perform);
 ARGLIST* GetLawParamFromFile(const INFO_LAW* Table, const uint index,
          const string& title, const char** perform, const uint sz_perform);
 bool GetDoubleFromFile(double& value, const ulong total, const ulong read,
      const string& title, const char** perform, const uint sz_perform);
 bool GetLongFromFile(long& value, const ulong total, const ulong read,
      const string& title, const char** perform, const uint sz_perform);

 //*****************************************************************************
 // Объявление Функций определения характеристик линейной антенной решетки из
 // входного файлового потока fin
 //*****************************************************************************
 bool GetLAAPerformFromFile();
 //1. Определение конфигурации антенной решетки
 bool GetConfigurationLAA(uint& left, uint& together, uint& right);
 bool GetCountElArray(ulong& count, const pKEYSTRPOS info, const char** pattern);
 //2. Определение свойства однородности антенной решетки
 bool GetUniformLAA(bool& uniform);
 //3. Определение свойства эквидистантности антенной решетки
 bool GetEquidistLAA(bool& equal, double& lv);
 //4. Определение закона по которому располагаются друг относительно друга
 //   элементы АР, расположенные слева от основного антенного элемента
 bool GetLeftDistLawLAA();
 //5. Определение закона по которому располагаются друг относительно друга
 //   элементы АР, расположенные справа от основного антенного элемента
 bool GetRightDistLawLAA();
 //6. Определение расположения линейной АР относительно земной поверхности
 bool GetDisposLAA(bool& disp);

 //Общая функция считывания всех характеристик АР и ее элементов из файла
 bool GetDataFromFile();
 //****************************************************************************
 // Объявление функций определения характеристик элементов линейной антенной
 // решетки из входного файлового потока fin
 //****************************************************************************
 bool GetElemArrayPerformFromFile(TElArray& ElArray, bool status = SLAVE);
 //1. Определение использования элемента АР при расчетах
 bool GetElArrayUsage(TElArray& ElArray);
 //2. Определение относительного коэфф-та усиления элемента АР
 //   Relative Factor of Amplify - ARF
 bool GetElArrayARF(TElArray& ElArray);
 //3. Определение типа элемента антенной решетки - типа антенны
 bool GetAntennaType(TElArray& ElArray);
 bool GetAntNameFromStr(const ANTENNA_INFO* Table, const uint sz_Table,
      uint& index, const string& title);
 ARGLIST* GetDirectParamFromFile(const uint index, bool plane);
 //4. Определение расположения E-плоскости антенны относительно Земли
 bool GetAntELocate(TElArray& ElArray);
 //5. Определение закона внутреннего шума элемента антенной решетки
 bool GetAntNoiseLaw(TElArray& ElArray);

};//Завершение объявления класса TLinearArray

#endif //ЗАВЕРШЕНИЕ ОБЪЯВЛЕНИЯ ЗАГОЛОВОЧНОГО ФАЙЛА "LINARRAY.H"

//---------------------- The end of file "linarray.h" --------------------------