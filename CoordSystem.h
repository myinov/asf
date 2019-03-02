//-----------------------------------------------------------------------------------
//CoordSystem.h
//Copyright (c) JohnSoft 2003. All rights reserved. C++ Builder 6.
//Initial date: April,12,2002. Final date:
//���������� ������� ������������� ����� �� ��������� � � ������������ ����������
//������� ���������:
//1. �������� ���������� (Polar coordinates)
//2. ��������� ������������� ���������� (rectangular Cartesian coordinates)


#ifndef CoordSystemH
#define CoordSystemH

//������������ �����
#include "const.h"
#pragma hdrstop

//****************************************************************************************
// ��������� ����� �� ��������� � ���������� ������������� �����������:
// xyOrigin - ������ ���������
// xyQuadOne, xyQuadTwo, xyQuadThree, xyQuadFour - I - IV ��������
// xyPosOX, xyNegOX, xyPosOY, xyNegOY - ������������� ��� ������������� ������� OX, OY
//****************************************************************************************
typedef enum {xyOrigin=0, xyQuadOne, xyQuadTwo, xyQuadThree, xyQuadFour,
              xyPosOX, xyNegOX, xyPosOY, xyNegOY} XYPos;

//----------------------------------------------------------------------------------------
//���������� ������� ������ ����������
//----------------------------------------------------------------------------------------
//������� ���������� ������������� ����� �� ���������� ������ � ��������� eps
double RoundToInt(const double value, double eps = 1e-10);

//****************************************************************************************
//��������������� ���������� ���� �������
//****************************************************************************************
//����� ������������� ����� � �������� ������� ��������� �� ���������
class TPolarCoord;
typedef TPolarCoord TRandSignal;
//����� ������������� ����� � ������������� ���������� ������� ���������
class TXYCoord;


//****************************************************************************************
//                          ���������� ������ TPolarCoord
//          �������� ������� ������������� ��������� ����� �� ���������
//��������� ������������ ����� P �������� ������-������ r - ���������� �� ����� P ��
//�������� ����� O (������) � �������� ���� phi - ���� ����� ������ OP � �������� ������,
//���������� ��� ������� �� �������� ��� ������ ������� ������� � ������������� ��� �������
//� �������� �������.
//������-������ r �������� ��������������� ������������ ��������� (r >= 0) � ����������� �
//������ ����� double. ���� � �������� �������-������� �������� ������������� ��������, ��
//������� ������ ������ ��������, ������� ������������� r.
//�������� ���� phi �������������� � ������ ��������� ����� �� [0..2PI) ����� double.
//��� ������� ��������� ���� ������������� �� ������������� ������������� � �������������.
//����� �������� ����, ������� ����� ���� ������ ��� � ��������, ��� � � �������� ����������
//� ��������� [0..2PI).
//****************************************************************************************
class TPolarCoord {
//-----------------------------------------------------------------------------------
// ���������� �������� ������-������ ������ TPolarCoord
//-----------------------------------------------------------------------------------
private:
 double r; //������-������, ������ (r >= 0)
 double phi;//�������� ���� (0 <= phi < 2*PI) � ��������
//-----------------------------------------------------------------------------------
// ���������� ������������� � ����������� ������ TPolarCoord
//-----------------------------------------------------------------------------------
public:
 //����������� �� ���������
 TPolarCoord() {Set(0.0,0.0,RAD);};
 //������������ � �����������
 TPolarCoord(double Radius, double Angle, bool AngleMeasure)
 {Set(Radius,Angle,AngleMeasure);};
 //���������� ������������� �����������
 TPolarCoord(const TPolarCoord &Point) {*this = Point;};
 TPolarCoord(const TXYCoord &Point) {*this = Point;};
 //���������� �����������
 ~TPolarCoord() {Set(0.0,0.0,RAD);};
 //----------------------------------------------------------------------------------
 // ���������� ������������� ������-������� ������ TPolarCoord
 //----------------------------------------------------------------------------------
 //��������� ������� ������
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

 //�������������� �������� ��������� ����� � ��������� ����������
 double GetX() const {return r*cos(phi);};
 double GetY() const {return r*sin(phi);};
 void CartesianCoord(double& x, double& y) const {x = GetX(); y = GetY();};
 void XYCoord(double& x, double& y) const {x = GetX(); y = GetY();};

 //----------------------------------------------------------------------------------
 //��������� ������� ������
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
 //��������� �������-������� � ��������� ����
 void Set(double Radius, double Angle, bool AngleMeasure)
 {SetRadius(Radius); SetAngle(Angle,AngleMeasure);};
 //----------------------------------------------------------------------------------
 //���������� ���������� ��� ������ TPolarCoord
 //���������� ��������� ������������ (=)
 TPolarCoord* operator =(const TPolarCoord& P)
 {if (this == &P) return this;
  Set(P.Radius(),P.AngleOfRad(),RAD);
  return this;
 };
 //���������� ��������� ������������ (=)  TPolarCoord = TXYCoord
 TPolarCoord* operator =(const TXYCoord& P);

 //���������� ��������� ��������� (==)
 bool operator ==(const TPolarCoord& P) const
 {return ((Radius() == P.Radius()) && (Angle(RAD) == P.Angle(RAD))) ? true : false;};

}; //���������� ���������� ������ TPolarCoord

//***********************************************************************************
//                          ���������� ������ TXYCoord
//     ������������� ��������� ������� ������������� ��������� ����� �� ���������
//����������� �������������� ������������ ����� P �������� ������ � ������������ ���-
//��� ���������� (���������� � ������������ ��������) ���� ����� �� ���� ������� ���-
//������������� ������ - ���� ���������. ����� ����������� ���� � ���������� �������
//���������. �������������� ��� - ��� ������� (��� OX), ������������ ��� - ��� �������
//(��� OY). �� ���� ���� ��������������� ������������� �����������: �� ��� OX - ������,
//�� ��� OY - �����. ���������� ����� P(x,y) ��������� �������������� ��� ����������-
//���� � ����������� �� �������, �� ������� �������� �������� ����� P. ���������� x �
//y ���������� �������������� ��������� � ��������� ����� P.
//� ������ TXYCoord ���������� ����� P(x,y) �������������� ����� double.
//***********************************************************************************
class TXYCoord {
//-----------------------------------------------------------------------------------
// ���������� �������� ������-������ ������ TXYCoord
//-----------------------------------------------------------------------------------
private:
 double x;//�������� �������� �����
 double y;//�������� �������� �����
//-----------------------------------------------------------------------------------
// ���������� ������������� � ����������� ������ TXYCoord
//-----------------------------------------------------------------------------------
public:
 //����������� �� ���������
 TXYCoord() {Set(0.0,0.0);};
 //������������ � �����������
 TXYCoord(double X, double Y) {Set(X,Y);};
 //���������� ������������� �����������
 TXYCoord(const TXYCoord &P) {*this = P;};
 TXYCoord(const TPolarCoord &P) {*this = P;};
 //���������� �����������
 ~TXYCoord() {Set(0.0,0.0);};
 //----------------------------------------------------------------------------------
 // ���������� ������������� ������-������� ������ TXYCoord
 //----------------------------------------------------------------------------------
 //��������� ������� ������
 double GetX() const {return x;};
 double GetY() const {return y;};
 void GetXY(double &X, double &Y) const {X = x; Y = y;};
 //����������� �������������� ����� P(x,y) �� ���������
 XYPos Position(double eps) const;
 //����� ��������� � ������� ���������
 bool IsOrigin(double eps = 1e-10) const {return (IsOX() && IsOY()) ? true : false;};
 //�������������� ����� �� ��� OX
 bool IsOX(double eps = 1e-10) const {return (fabs(GetY()) < eps) ? true : false;};
 //�������������� ����� �� ��� OY
 bool IsOY(double eps = 1e-10) const {return (fabs(GetX()) < eps) ? true : false;};

 //�������������� ���������� ������������� ��������� � �������� ����������
 //���������� �� ����� P(x,y) �� ������ ���������
 double GetRadius() const {return sqrt(x*x+y*y);};
 double GetPhi(bool AngleMeasure, double eps = 1e-10) const;
 void PolarCoord(double& r, double& phi, bool AngleMeasure, double eps = 1e-10) const
 {r = GetRadius(); phi = GetPhi(AngleMeasure,eps);};

 //----------------------------------------------------------------------------------
 //��������� ������� ������
 double SetX(double X) {return x = X;};
 double SetY(double Y) {return y = Y;};
 void Set(double X, double Y) {x = X; y = Y;};
 void SetXY(double X, double Y) {Set(X,Y);};
 //----------------------------------------------------------------------------------
 //���������� ���������� ��� ������ TXYCoord
 //----------------------------------------------------------------------------------
 //���������� ��������� ������������ (=)
 TXYCoord* operator =(const TXYCoord& P)
 {if (this == &P) return this;
  Set(P.GetX(),P.GetY());
  return this;
 };
 //���������� ��������� ������������ (=)  TXYCoord = TPolarCoord
 TXYCoord* operator =(const TPolarCoord& P);
 //���������� ��������� ��������� (==)
 bool operator ==(const TXYCoord& P) const
 {return ((GetX() == P.GetX()) && (GetY() == P.GetY())) ? true : false;};

 //�������� ����� P(x,y) ������ ������ ��������� O(0,0) �� �������� ����
 TXYCoord* Rotation(double Phi, bool AngleMeasure);
 bool Rotation(TXYCoord& NP, double Phi, bool AngleMeasure) const;
 //�������� ����� P(x,y) ������������ �������� ����� T(x0,y0) �� �������� ����
 TXYCoord* Rotation(double Phi, bool AngleMeasure, const TXYCoord& T);
 bool Rotation(TXYCoord& NP, double Phi, bool AngleMeasure, const TXYCoord& T) const;

 //----------------------------------------------------------------------------------
 //         ���������� ������������� ������� ������ TXYCoord
 //----------------------------------------------------------------------------------
 //�������� ����� P(x,y) ������ ������ ��������� O(0,0) �� �������� ����
 friend TXYCoord* Rotation(TXYCoord& P, double Phi, bool AngleMeasure)
 {return P.Rotation(Phi,AngleMeasure);};
 friend bool Rotation(TXYCoord& NP, const TXYCoord& P, double Phi, bool AngleMeasure)
 {return P.Rotation(NP,Phi,AngleMeasure);};
 //�������� ����� P(x,y) ������������ �������� ����� T(x0,y0) �� �������� ����
 friend TXYCoord* Rotation(TXYCoord& P, double Phi, bool AngleMeasure, const TXYCoord& T)
 {return P.Rotation(Phi,AngleMeasure,T);};
 friend bool Rotation(TXYCoord& NP, const TXYCoord& P, double Phi, bool AngleMeasure,
                      const TXYCoord& T)
 {return P.Rotation(NP,Phi,AngleMeasure,T);};

}; //���������� ���������� ������ TXYCoord

#endif
