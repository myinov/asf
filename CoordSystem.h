//-----------------------------------------------------------------------------------
//CoordSystem.h
//Copyright (c) JohnSoft 2003. All rights reserved. C++ Builder 6.
//Initial date: April,12,2002. Final date:
//Объявление классов представления точек на плоскости и в пространстве различными
//система координат:
//1. Полярные координаты (Polar coordinates)
//2. Декартовы прямоугольные координаты (rectangular Cartesian coordinates)


#ifndef CoordSystemH
#define CoordSystemH

//Заголовочные файлы
#include "const.h"
#pragma hdrstop

//****************************************************************************************
// Положение точки на плоскости в декартовых прямоугольных координатах:
// xyOrigin - начало координат
// xyQuadOne, xyQuadTwo, xyQuadThree, xyQuadFour - I - IV квадрант
// xyPosOX, xyNegOX, xyPosOY, xyNegOY - положительная или отрицательная полуоси OX, OY
//****************************************************************************************
typedef enum {xyOrigin=0, xyQuadOne, xyQuadTwo, xyQuadThree, xyQuadFour,
              xyPosOX, xyNegOX, xyPosOY, xyNegOY} XYPos;

//----------------------------------------------------------------------------------------
//Объявление функций общего назначения
//----------------------------------------------------------------------------------------
//Попытка округления вещественного числа до ближайшего целого с точностью eps
double RoundToInt(const double value, double eps = 1e-10);

//****************************************************************************************
//Предварительное объявление имен классов
//****************************************************************************************
//Класс представления точки в ПОЛЯРНОЙ СИСТЕМЕ КООРДИНАТ на плоскости
class TPolarCoord;
typedef TPolarCoord TRandSignal;
//Класс представления точки в ПРЯМОУГОЛЬНОЙ ДЕКАРТОВОЙ системе координат
class TXYCoord;


//****************************************************************************************
//                          Объявление класса TPolarCoord
//          ПОЛЯРНАЯ СИСТЕМА ПРЕДСТАВЛЕНИЯ КООРДИНАТ ТОЧКИ НА ПЛОСКОСТИ
//Полярными координатами точки P называют РАДИУС-ВЕКТОР r - расстояние от точки P до
//заданной точки O (ПОЛЮСА) и ПОЛЯРНЫЙ УГОЛ phi - угол между прямой OP и заданной прямой,
//проходящей при отсчете от полярной оси против часовой стрелки и отрицательным при отсчете
//в обратную сторону.
//Радиус-вектор r является НЕОТРИЦАТЕЛЬНОЙ вещественной величиной (r >= 0) и представлен в
//классе типом double. Если в качестве радиуса-вектора задается отрицательная величина, то
//берется МОДУЛЬ данной величины, которая присваивается r.
//Полярный угол phi представляется в классе РАДИАННОЙ мерой от [0..2PI) типом double.
//При задании полярного угла отрицательным он автоматически преобразуется в положительный.
//Любое значение угла, которое может быть задано как в градусах, так и в радианах приводится
//к интервалу [0..2PI).
//****************************************************************************************
class TPolarCoord {
//-----------------------------------------------------------------------------------
// Объявление закрытых членов-данных класса TPolarCoord
//-----------------------------------------------------------------------------------
private:
 double r; //Радиус-вектор, модуль (r >= 0)
 double phi;//Полярный угол (0 <= phi < 2*PI) в РАДИАНАХ
//-----------------------------------------------------------------------------------
// ОБЪЯВЛЕНИЕ КОНСТРУКТОРОВ и ДЕСТРУКТОРА КЛАССА TPolarCoord
//-----------------------------------------------------------------------------------
public:
 //Конструктор по умолчанию
 TPolarCoord() {Set(0.0,0.0,RAD);};
 //Конструкторы с аргументами
 TPolarCoord(double Radius, double Angle, bool AngleMeasure)
 {Set(Radius,Angle,AngleMeasure);};
 //Объявление конструкторов копирования
 TPolarCoord(const TPolarCoord &Point) {*this = Point;};
 TPolarCoord(const TXYCoord &Point) {*this = Point;};
 //Объявление деструктора
 ~TPolarCoord() {Set(0.0,0.0,RAD);};
 //----------------------------------------------------------------------------------
 // Объявление общедоступных членов-функций класса TPolarCoord
 //----------------------------------------------------------------------------------
 //Получение свойств класса
 double AngleOfRad() const {return phi;};
 double AngleOfDeg() const {return phi*RadDeg;};
 double Angle(bool AngleMeasure) const
 {return (AngleMeasure == DEG) ? AngleOfDeg() : AngleOfRad();};
 double Phase(bool AngleMeasure) const
 {return (AngleMeasure == DEG) ? AngleOfDeg() : AngleOfRad();};

 double DirectionOfRad() const {return phi;};
 double DirectionOfDeg() const {return phi*RadDeg;};
 double Direction(bool AngleMeasure) const
 {return (AngleMeasure == DEG) ? phi*RadDeg : phi;};
 double Radius() const {return r;};
 double Magnitude() const {return r;};
 double Amplitude() const {return r;};

 //Преобразование полярных координат точки в декартовы координаты
 double GetX() const {return r*cos(phi);};
 double GetY() const {return r*sin(phi);};
 void CartesianCoord(double& x, double& y) const {x = GetX(); y = GetY();};
 void XYCoord(double& x, double& y) const {x = GetX(); y = GetY();};

 //----------------------------------------------------------------------------------
 //Установка свойств класса
 double SetAngleOfRad(double Angle);
 double SetAngleOfDeg(double Angle) {Angle*=DegRad; return SetAngleOfRad(Angle);};
 double SetAngle(double Angle, bool AngleMeasure)
 {if (AngleMeasure == DEG) Angle*=DegRad; return SetAngleOfRad(Angle);}
 double SetPhase(double Phase, bool Measure)
 {if (Measure == DEG) Phase*=DegRad; return SetAngleOfRad(Phase);}

 double SetDirectionOfRad(double Angle) {return SetAngleOfRad(Angle);};
 double SetDirectionOfDeg(double Angle) {return SetAngleOfDeg(Angle);};
 double SetDirection(double Angle, bool AngleMeasure)
 {return SetAngle(Angle,AngleMeasure);};
 double SetRadius(double Radius) {return r = fabs(Radius);};
 double SetMagnitude(double Radius) {return SetRadius(Radius);};
 double SetAmplitude(double Ampl) {return SetRadius(Ampl);};
 //Установка радиуса-вектора и полярного угла
 void Set(double Radius, double Angle, bool AngleMeasure)
 {SetRadius(Radius); SetAngle(Angle,AngleMeasure);};
 //----------------------------------------------------------------------------------
 //ПЕРЕГРУЗКА ОПЕРАТОРОВ для класса TPolarCoord
 //Перегрузка оператора присваивания (=)
 TPolarCoord* operator =(const TPolarCoord& P)
 {if (this == &P) return this;
  Set(P.Radius(),P.AngleOfRad(),RAD);
  return this;
 };
 //Перегрузка оператора присваивания (=)  TPolarCoord = TXYCoord
 TPolarCoord* operator =(const TXYCoord& P);

 //Перегрузка оператора равенства (==)
 bool operator ==(const TPolarCoord& P) const
 {return ((Radius() == P.Radius()) && (Angle(RAD) == P.Angle(RAD))) ? true : false;};

}; //Завершение объявления класса TPolarCoord

//***********************************************************************************
//                          Объявление класса TXYCoord
//     ПРЯМОУГОЛЬНАЯ ДЕКАРТОВА СИСТЕМА ПРЕДСТАВЛЕНИЯ КООРДИНАТ ТОЧКИ НА ПЛОСКОСТИ
//Декартовыми прямоугольными координатами точки P называют взятые с определенным зна-
//ком расстояния (выраженные в определенном масштабе) этой точки от двух взаимно пер-
//пендикулярных прямых - осей координат. Точка пересечения осей О называется началом
//координат. Горизонтальная ось - ось АБСЦИСС (ось OX), вертикальная ось - ось ОРДИНАТ
//(ось OY). На этих осях устанавливается положительное направление: на оси OX - вправо,
//на оси OY - вверх. Координаты точки P(x,y) считаются положительными или отрицатель-
//ными в зависимости от полуоси, на которую попадает проекция точки P. Координаты x и
//y называются соответственно АБСЦИССОЙ и ОРДИНАТОЙ точки P.
//В классе TXYCoord координаты точки P(x,y) представляются типом double.
//***********************************************************************************
class TXYCoord {
//-----------------------------------------------------------------------------------
// Объявление закрытых членов-данных класса TXYCoord
//-----------------------------------------------------------------------------------
private:
 double x;//значение абсциссы точки
 double y;//значение ординаты точки
//-----------------------------------------------------------------------------------
// ОБЪЯВЛЕНИЕ КОНСТРУКТОРОВ и ДЕСТРУКТОРА КЛАССА TXYCoord
//-----------------------------------------------------------------------------------
public:
 //Конструктор по умолчанию
 TXYCoord() {Set(0.0,0.0);};
 //Конструкторы с аргументами
 TXYCoord(double X, double Y) {Set(X,Y);};
 //Объявление конструкторов копирования
 TXYCoord(const TXYCoord &P) {*this = P;};
 TXYCoord(const TPolarCoord &P) {*this = P;};
 //Объявление деструктора
 ~TXYCoord() {Set(0.0,0.0);};
 //----------------------------------------------------------------------------------
 // Объявление общедоступных членов-функций класса TXYCoord
 //----------------------------------------------------------------------------------
 //Получение свойств класса
 double GetX() const {return x;};
 double GetY() const {return y;};
 void GetXY(double &X, double &Y) const {X = x; Y = y;};
 //Определение местоположения точки P(x,y) на плоскости
 XYPos Position(double eps) const;
 //Точка совпадает с началом координат
 bool IsOrigin(double eps = 1e-10) const {return (IsOX() && IsOY()) ? true : false;};
 //Местоположение точки на оси OX
 bool IsOX(double eps = 1e-10) const {return (fabs(GetY()) < eps) ? true : false;};
 //Местоположение точки на оси OY
 bool IsOY(double eps = 1e-10) const {return (fabs(GetX()) < eps) ? true : false;};

 //Преобразование декартовых прямоугольных координат в полярные координаты
 //Расстояние от точки P(x,y) до начала координат
 double GetRadius() const {return sqrt(x*x+y*y);};
 double GetPhi(bool AngleMeasure, double eps = 1e-10) const;
 void PolarCoord(double& r, double& phi, bool AngleMeasure, double eps = 1e-10) const
 {r = GetRadius(); phi = GetPhi(AngleMeasure,eps);};

 //----------------------------------------------------------------------------------
 //Установка свойств класса
 double SetX(double X) {return x = X;};
 double SetY(double Y) {return y = Y;};
 void Set(double X, double Y) {x = X; y = Y;};
 void SetXY(double X, double Y) {Set(X,Y);};
 //----------------------------------------------------------------------------------
 //ПЕРЕГРУЗКА ОПЕРАТОРОВ для класса TXYCoord
 //----------------------------------------------------------------------------------
 //Перегрузка оператора присваивания (=)
 TXYCoord* operator =(const TXYCoord& P)
 {if (this == &P) return this;
  Set(P.GetX(),P.GetY());
  return this;
 };
 //Перегрузка оператора присваивания (=)  TXYCoord = TPolarCoord
 TXYCoord* operator =(const TPolarCoord& P);
 //Перегрузка оператора равенства (==)
 bool operator ==(const TXYCoord& P) const
 {return ((GetX() == P.GetX()) && (GetY() == P.GetY())) ? true : false;};

 //Вращение точки P(x,y) вокруг начала координат O(0,0) на заданный угол
 TXYCoord* Rotation(double Phi, bool AngleMeasure);
 bool Rotation(TXYCoord& NP, double Phi, bool AngleMeasure) const;
 //Вращение точки P(x,y) относительно заданной точки T(x0,y0) на заданный угол
 TXYCoord* Rotation(double Phi, bool AngleMeasure, const TXYCoord& T);
 bool Rotation(TXYCoord& NP, double Phi, bool AngleMeasure, const TXYCoord& T) const;

 //----------------------------------------------------------------------------------
 //         ОБЪЯВЛЕНИЕ ДРУЖЕСТВЕННЫХ ФУНКЦИЙ КЛАССУ TXYCoord
 //----------------------------------------------------------------------------------
 //Вращение точки P(x,y) вокруг начала координат O(0,0) на заданный угол
 friend TXYCoord* Rotation(TXYCoord& P, double Phi, bool AngleMeasure)
 {return P.Rotation(Phi,AngleMeasure);};
 friend bool Rotation(TXYCoord& NP, const TXYCoord& P, double Phi, bool AngleMeasure)
 {return P.Rotation(NP,Phi,AngleMeasure);};
 //Вращение точки P(x,y) относительно заданной точки T(x0,y0) на заданный угол
 friend TXYCoord* Rotation(TXYCoord& P, double Phi, bool AngleMeasure, const TXYCoord& T)
 {return P.Rotation(Phi,AngleMeasure,T);};
 friend bool Rotation(TXYCoord& NP, const TXYCoord& P, double Phi, bool AngleMeasure,
                      const TXYCoord& T)
 {return P.Rotation(NP,Phi,AngleMeasure,T);};

}; //Завершение объявления класса TXYCoord

#endif
