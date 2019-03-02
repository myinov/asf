#include "const.h"

#if !defined(__PREDICATE_HPP)
#define __PREDICATE_HPP
//**********************************************************************************
// ОБЪЯВЛЕНИЕ ШАБЛОННЫХ КЛАССОВ для работы с векторами
// 1. template <class TArg, class TVector> class TArgVectorPredicate
// 2. template <class TArg, class TVector> class TAlwaysTrue
// 3. template <class TArg, class TVector> class TBelongClosedInterval
// 4. template <class TArg, class TVector> class TClosedIntervalWithDiscontPoints
// 5. template <class TArg, class TVector> class TGreaterOrEqual
//**********************************************************************************
//----------------------------------------------------------------------------------
//Объявление шаблонного класса TPredicate, который возвращает тип bool и принимает
//два аргумента - один типа ссылка на TArg, второй ссылка на вектор TVector.
//В качестве второго параметра следует использовать ссылки на классы ivector,
//rvector, cvector.
//----------------------------------------------------------------------------------
template <class TArg, class TVector>
class TArgVectorPredicate
{protected:
  TVector v_param;//Вектор параметров предиката
  bool lReady;//"Работоспособность" объекта
 public:
  TArgVectorPredicate() {v_param = TVector(); SetReady();};
  TArgVectorPredicate(const TVector& param) {v_param = param; SetReady();}
  virtual bool operator() (const TArg& Value) const = 0;
  virtual string& GetPerform(string& s) const = 0;
  void operator =(const TVector& V) {v_param = V; SetReady();};
  bool IsReady() const {return lReady;};
 protected:
  void SetReady() {lReady = (v_param.Size() == 0) ? false : true;};
};

template <class TArg, class TVector>
class TAlwaysTrue: public TArgVectorPredicate<TArg, TVector>
{public:
  TAlwaysTrue() : TArgVectorPredicate<TArg, TVector>() {SetReady();};
  TAlwaysTrue(const TVector& param) :
  TArgVectorPredicate<TArg, TVector>(param) {SetReady();};
  bool operator() (const TArg& Value) const {return true;}
  string& GetPerform(string& s) const {return s = "(-INF..+INF)";}
 protected:
  void SetReady() {lReady = true;};
};
//-----------------------------------------------------------------------------------
//Шаблонный класс определения принадлежности значения типа TArg закрытому интервалу,
//задаваемому двумя параметрами left & right, которые передаются через TVector:
//Param[0] = left; Param[1] = right.
//-----------------------------------------------------------------------------------
template <class TArg, class TVector>
class TBelongClosedInterval: public TArgVectorPredicate<TArg, TVector>
{public:
  //Конструкторы класса
  TBelongClosedInterval() : TArgVectorPredicate<TArg, TVector>() {SetReady();};
  TBelongClosedInterval(const TVector& param) :
  TArgVectorPredicate<TArg, TVector>(param) {SetReady();};
  TBelongClosedInterval(const TArg& bottom, const TArg& upper)
  {v_param.Resize(2); v_param[0] = bottom; v_param[1] = upper; SetReady();};

  //Перегрузка оператора вызова функции
  bool operator() (const TArg& Value) const
  {bool result = false;
   //Проверка на соответствие параметров, передавемых через TVector
   if (!IsReady()) return result;
   TArg left = v_param[0]; TArg right = v_param[1];
   //Проверка на принадлежность закрытому интервалу [left..right] значения Value
   if ((Value >= left) && (Value <= right)) result = true;
   return result;
  }
  void operator =(const TVector& V) {v_param = V; SetReady();};
  string& GetPerform(string& s) const
  {s.resize(0);
   if (v_param.Size() != 2) return s = "Closed Interval Bounds Indefinite.";
   if (v_param[1] < v_param[0])
   {s = "Closed Interval Bounds is Wrong.\n";
    s.append("Left Bound Value > Right Bound Value.\n");
   }
   char buffer[80]; ostrstream text (buffer, sizeof(buffer));
   text << "[" << v_param[0] << ".." << v_param[1] << "]" << ends;
   s.append(text.str()); text.seekp(0);
   return s;
  }
 protected:
  void SetReady()
  {ulong size = v_param.Size();
   lReady = ((size != 2) || (v_param[1] < v_param[0])) ? false : true;
  }
};
//-----------------------------------------------------------------------------------
//Шаблонный класс определения принадлежности значения типа TArg закрытому интервалу,
//который содержит точки не принадлежащие данному интервалу - точки разрыва. Вектор
//параметров TVector имеет следующую структуру:
//v_param[0] = left bound; v_param[1] = right bound; v_param[2]...v_param[N-1] -
//точки разрыва, которые должны находиться внутри интервала; N - размерность вектора
//параметров класса TClosedIntervalWithDiscontPoints. Класс может не содержать точек
//разрыва. В этом случае он аналогичен шаблонному классу TBelongClosedInterval.
//-----------------------------------------------------------------------------------
template <class TArg, class TVector>
class TClosedIntervalWithDiscontPoints: public TArgVectorPredicate<TArg, TVector>
{protected:
  TVector discont_points;
 public:
  TClosedIntervalWithDiscontPoints() : TArgVectorPredicate<TArg, TVector>()
  {SetReady(); Set();};
  TClosedIntervalWithDiscontPoints(const TVector& param) :
  TArgVectorPredicate<TArg, TVector>(param) {SetReady(); Set();};
  bool operator() (const TArg& Value) const
  {bool result = false;
   //Проверка на соответствие параметров, передавемых через TVector
   if (!IsReady()) return result;
   TArg left = v_param[0]; TArg right = v_param[1];
   //Проверка на принадлежность закрытому интервалу [left..right] значения Value
   if ((Value >= left) && (Value <= right)) result = true;
   else return false;
   //Поиск на совпадение Value с точками внутри интервала, которые ему не
   //принадлежат
   result = (discont_points.Find(Value)) ? false : true;
   return result;
  }
  void operator =(const TVector& V) {v_param = V; SetReady(); Set();};
  string& GetPerform(string& s) const
  {s.resize(0);
   if (v_param.Size() < 2) return s = "Closed Interval Bounds Indefinite.";
   if (v_param[1] < v_param[0])
   {s = "Closed Interval Bounds is Wrong.\n";
    s.append("Left Bound Value > Right Bound Value.\n");
   }
   char buffer[80]; ostrstream text (buffer, sizeof(buffer));
   text << "[" << v_param[0] << ".." << v_param[1] << "] " << ends;
   s.append(text.str()); text.seekp(0);
   text << "Discontinuity Points = " << discont_points.Size() << ";" << ends;
   s.append(text.str()); text.seekp(0);
   return s;
  }
 protected:
  void SetReady()
  {ulong size = v_param.Size();
   lReady = ((size < 2) || (v_param[1] < v_param[0])) ? false : true;
  }
  void Set();//Установка параметров класса
};//Завершение описания класса TClosedIntervalWithDiscontPoints
//-----------------------------------------------------------------------------------
//Реализация членов-функций класса TClosedIntervalWithDiscontPoints
//-----------------------------------------------------------------------------------
template <class TArg, class TVector>
void TClosedIntervalWithDiscontPoints<TArg, TVector>::Set()
{discont_points = TVector();
 if ((!IsReady()) || (v_param.Size() < 3)) return;
 ulong count = 0L;
 //Подсчет точек разрыва интервала, которые ему не принадлежат
 TArg left = v_param[0]; TArg right = v_param[1]; TArg value;
 for (ulong i = 2; i < v_param.Size(); i++)
 {value = v_param[i]; if ((value >= left) && (value <= right)) count++;}
 if (count == 0L) return;
 discont_points.Resize(count);
 //Занесение значений точек разрыва интервала из v_param в discont_points
 ulong j = 0L;
 for (ulong i = 2; i < v_param.Size(); i++)
 {value = v_param[i];
  if ((value >= left) && (value <= right)) //Точка разрыва из v_param принадлежит интервалу
  {discont_points[j++] = value;
   if (j == discont_points.Size()) break; //Проверка на заполнение вектора discont_points
  }
 }
 v_param.Resize(2,true); //Модификация массива v_param
}//Завершение функции Set()
//-----------------------------------------------------------------------------------
//Шаблонный класс TGreaterOrEqual определяет принадлежность значения типа TArg интервалу:
//[LowerBound..+INF) задаваемому одним параметром LowerBound, передаваемый через TVector:
//Param[0] = LowerBound.
//-----------------------------------------------------------------------------------
template <class TArg, class TVector>
class TGreaterOrEqual: public TArgVectorPredicate<TArg, TVector>
{public:
  TGreaterOrEqual() : TArgVectorPredicate<TArg, TVector>() {SetReady();};
  TGreaterOrEqual(const TVector& param) :
  TArgVectorPredicate<TArg, TVector>(param) {SetReady();};
  TGreaterOrEqual(const TArg& LowerBound)
  {v_param.Resize(1); v_param[0] = LowerBound; SetReady();};

  //Перегрузка оператора вызова функции
  bool operator() (const TArg& Value) const
  {bool result = false;
   //Проверка на соответствие параметров, передавемых через TVector
   if (!IsReady()) return result;
   TArg LowerBound = v_param[0];
   //Проверка на принадлежность интервалу [LowerBound..+INF] значения Value
   if (Value >= LowerBound) result = true;
   return result;
  }
  void operator =(const TVector& V) {v_param = V; SetReady();};
  string& GetPerform(string& s) const
  {s.resize(0);
   if (v_param.Size() != 1) return s = "Interval [LowerBound..+INF) is undefined.";
   char buffer[40]; ostrstream text (buffer, sizeof(buffer));
   text << "[" << v_param[0] << "..+INF)" << ends;
   s.append(text.str()); text.seekp(0);
   return s;
  }
 protected:
  void SetReady() {lReady = (v_param.Size() != 1) ? false : true;}
};

#endif //ЗАВЕРШЕНИЕ ОБЪЯВЛЕНИЯ ЗАГОЛОВОЧНОГО ФАЙЛА "PREDICATE.HPP"

 