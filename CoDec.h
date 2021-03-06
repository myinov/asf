//----------------------------------------------------------------------------------------
//CoDec.h
//Copyright (c) JohnSoft 2006. All rights reserved. Builder C++ 6.0
//Initial date: September,12,2006. Final date:
//���������� �������, ����������� �������������� ��������������� �������� ��������.
#ifndef CoDecH
#define CoDecH

#include "const.h"
#pragma hdrstop

#include "parsing.h"

//****************************************************************************************
//��������������� ���������� ���� ������� � �������
//****************************************************************************************
//���������� ��������������� ��������������� �������� ������� � ������������ ������������
//�� ��������� � �������� ������� �������� � ��������
class TFuncGen_DSClipper;
//����������� (����������) ������ �� TFuncGen_DSClipper
class TFuncGen_DSC_LinLaw;
class TFuncGen_DSC_DecLinLaw;
class TFuncGen_DSC_SqrtLaw;
class TFuncGen_DSC_ExpLaw;
class TFuncGen_DSC_DecExpLaw;
class TFuncGen_DSC_SinLaw;
class TFuncGen_DSC_CosLaw;
class TFuncGen_DSC_HyperbolicLaw;


//****************************************************************************************
//���������� ������������ (��������) ������ TFuncGen_DSClipper
//Copyright (c) JohnSoft 2006. All rights reserved.
//Initial date: September,12,2006; Final date:
//����� ��������� ��� �������������� �������������� �������������� �������� �������� �
//������������ �� ������������ � ����������� ��������� ������� � �������� ��������:
//1. ������ ��������������:   P(U) = k*F(U) + c (���������, �����������)
//2. �������� ��������������: U(P) = InvF((P-c)/k) (�����������, �������������)
//������� ������� # 1 ��������� ��������������� ������ ������� � �������� �������� ��
//������ F(U), ����� �������, ��� ��� ��������� ������� ������� � ��������� [Umin..Umax]
//�������� ����� ���������� � �������� �� [Pmin..Pmax]. ������������ k � c � �����������
//�� ���� ������� ����������� �������������� ��������� �������:
//1) ������� ����������� - ���������-������������ (� ����������� U ������������� P)
//   k = (Pmax - Pmin)/(F(Umax) - F(Umin)), c = Pmax - k*F(Umax);
//1) ������� ����������� - ���������-��������� (� ����������� U ����������� P)
//   k = (Pmax - Pmin)/(F(Umin) - F(Umax)), c = Pmin - k*F(Umax).
//�������� Pmax, Pmin ���������� ������ (������������ � �����������) �� ��������, Umin �
//Umax ������ ����������� � ������������ ������ �� ������� �����������. ������ �����������
//��������� �����������: Pmax > Pmin >= 0, Umax > Umin >= 0.
//��������� ������������ �� ������ ������� U (���������) � ����������� �� ���� �������
//����������� �������� ��������� �������:
//1) ������� ����������� - ���������-������������ (� ����������� U ������������� P):
//   a) ��� ���� U < Umin => P = Pmin; b) ��� ���� U > Umax => P = Pmax.
//2) ������� ����������� - ���������-��������� (� ����������� U ����������� P):
//   a) ��� ���� U < Umin => P = Pmax; b) ��� ���� U > Umax => P = Pmin.
//��������� ������������ �� �������� (�����������) � ����������� �� ���� ������� ��������-
//����� �������� ��������� �������:
//1) ������� ������������� - ���������-������������ (� ����������� P ������������� U):
//   a) ��� ���� P < Pmin => U = Umin; b) ��� ���� P > Pmax => U = Umax.
//2) ������� ������������� - ���������-��������� (� ����������� P ����������� U):
//   a) ��� ���� P < Pmin => U = Umax; b) ��� ���� P > Pmax => U = Umin.
//������� # 2 ����� ������� ��������������� �������� �������� ������� � �������� ��������
//������ ������� �� ������ InvF(U), ��� InvF(U) - �������� F(U) �������. � ���� ������ ���
//��������� ������� ��������� � ��������� [Pmin..Pmax] ������� ����������������� �������
//����� ���������� � �������� �� Umin �� Umax.
//�������������� ��������������� F(x) � InvF(x) ����������� �������� ������������ �� �������
//������������ ������ TFuncGen_DSClipper.
//****************************************************************************************
class TFuncGen_DSClipper {
//-----------------------------------------------------------------------------------
// ���������� ���������� ������-������ ������ TFuncGen_DSClipper
//-----------------------------------------------------------------------------------
protected:
 double _Umin;//����������� ��������� ������� �������
 double _Umax;//������������ ��������� ������� �������
 double _Pmin;//�����������  ������� �������� �������
 double _Pmax;//������������ ������� �������� �������
 //������������ ��������������� ��������������� P(U) = k*F(U) + c
 double k;//
 double c;//
 //��� ������� �����������-������������� - ��������� ������������ ��� ���������
 enIncDecFunc _FuncType;

//-----------------------------------------------------------------------------------
// I. ���������� ������������� ������-������� ������ TFuncGen_DSClipper
//-----------------------------------------------------------------------------------
public:
//***********************************************************************************
// ���������� ������������� � ����������� ������ TFuncGen_DSClipper
//***********************************************************************************
 //����������� �� ���������
 TFuncGen_DSClipper() {_SetDefault();};
 //����������� � �����������
 TFuncGen_DSClipper(const char* str) {_SetDefault(); _SetThresholds(str);};
 TFuncGen_DSClipper(double Umin, double Umax, double Pmin, double Pmax)
 {_SetDefault(); _SetThresholds(Umin,Umax,Pmin,Pmax);};

 //���������� ������������ �����������
 //TFuncGen_DSClipper(const TFuncGen_DSClipper& Obj) {*this = Obj;};

 //���������� ����������� ������
 virtual ~TFuncGen_DSClipper() {_Reset();};
//---------------------------------------------------------------------------------------
//���������� ������������� ������-������� ������ TFuncGen_DSClipper
//---------------------------------------------------------------------------------------
 //��������� ������������� ��������� �������� ����������
 void GetThreshold_U(double& Umin, double& Umax) const {Umin = _Umin; Umax = _Umax;};
 //��������� ������������� ��������� �������� ��������
 void GetThreshold_P(double& Pmin, double& Pmax) const {Pmin = _Pmin; Pmax = _Pmax;};
 double GetPmin() const {return _Pmin;};
 double GetPmax() const {return _Pmax;};
 //������������ ������������� ��������� �������� ���������� � ������ s
 string& GetThreshold_U(string& s) const;
 //������������ ������������� ��������� �������� �������� � ������ s
 string& GetThreshold_P(string& s) const;
 //������������ ������������� ��������������� ��������������� � ������ s
 string& GetFactors(string& s) const;
 //��������� ������� ��������������� ��������������� � ��������� ����
 string& Info(string& s) const;

 //��������� ��������� �������� ���������� �� ���������
 void SetDefault_U() {_Umin = 0; _Umax = 255.0; _CalcFactors();};
 //��������� ��������� �������� �������� �� ���������
 void SetDefault_P() {_Pmin = 10; _Pmax = 100.0; _CalcFactors();};
 //��������� ��������� �������� �� ���������� � �������� �� ���������
 void SetDefault() {_SetDefault(); _CalcFactors();};

 //��������� ��������� �������� ����������
 void SetThreshold_U(double Umin, double Umax);
 //��������� ��������� �������� ��������
 void SetThreshold_P(double Pmin, double Pmax);
 //��������� ��������� �������� ���������� � ��������
 void SetThresholds(double Umin, double Umax, double Pmin, double Pmax)
 {_SetThresholds(Umin,Umax,Pmin,Pmax); _CalcFactors();};

 //������ �� ������ s ��������� �������� ���������� � �������� � �� ���������
 void SetThresholds(const char* s) {_SetThresholds(s); _CalcFactors();};
 void SetThresholds(const string& s) {return SetThresholds(s.c_str());};

 //������ ��������������: P(U) = k*F(U) + c
 double Encode(double U) const;
 //�������� ��������������: U(P) = InvF((P-c)/k)
 double Decode(double P) const;

 //���������� �������������� ������� �������
 virtual void Edit();

 //***************************************************************************************
 // ������������� ������� ������ TFuncGen_DSClipper
 // 1. ������� ����� - ������ ����������
 //***************************************************************************************
 //���������� ��������� ������ (<<) ��� ������ ���������� ������
 friend ostream& operator <<(ostream& out, const TFuncGen_DSClipper& Obj)
 {string s; return out << Obj.Info(s);};

//---------------------------------------------------------------------------------------
//���������� ���������� ������-������� ������ TFuncGen_DSClipper
//---------------------------------------------------------------------------------------
protected:
 //��������� ��������� ����������
 void _Reset() {_Umin = _Umax = _Pmin = _Pmax = k = c = 0.0;};
 //��������� ��������� �������� �� ���������� � �������� �� ���������
 void _SetDefault() {_Umin = 0; _Umax = 255.0; _Pmin = 10; _Pmax = 100;};
 //��������� ��������� �������� ���������� � ��������
 void _SetThresholds(double Umin, double Umax, double Pmin, double Pmax);
 //������ �� ������ s ��������� �������� ���������� � �������� � �� ���������
 void _SetThresholds(const char* s);

 //��������� ������������� ���� ������� �����������
 void _SetIncFunc() {_FuncType = IncFunc;};
 //��������� ���������� ���� ������� �����������
 void _SetDecFunc() {_FuncType = DecFunc;};

 //����� ������ (�������������) ������� F(U)
 virtual double Func(double InU) const = 0;
 //����� �������� (���������������) ������� InvF(P)
 virtual double InvFunc(double InP) const = 0;
 //������ ������������� k & c ��������������� ���������������
 virtual void _CalcFactors();
 //��� ������ (�������������) ������� F(U) � ��������� ����
 virtual string& Func(string& s) const = 0;
 //��� �������� (���������������) ������� InvF(P) � ��������� ����
 virtual string& InvFunc(string& s) const = 0;

 //���������� �������������� ������� ���������� � ���������: Umin, Umax, Pmin � Pmax
 virtual void Edit_Thresholds();

};//���������� ���������� ������ TFuncGen_DSClipper


//****************************************************************************************
//���������� ������������ ������ TFuncGen_DSC_LinLaw �� ������������ (��������) ������
//TFuncGen_DSClipper
//Copyright (c) JohnSoft 2006. All rights reserved.
//Initial date: September,13,2006; Final date:
//����� ��������� ��� �������� ������ TFuncGen_DSClipper � ��������� ��� ��������������
//�������������� �������������� �������� �������� � ������������ �� ��������� ������:
//1. ������ ��������������:   P(U) = k*F(U) + c, ��� F(x) = x;
//2. �������� ��������������: U(P) = InvF((P-c)/k),  InvF(x) = x.
//****************************************************************************************
class TFuncGen_DSC_LinLaw : public TFuncGen_DSClipper {

//-----------------------------------------------------------------------------------
// I. ���������� ������������� ������-������� ������ TFuncGen_DSC_LinLaw
//-----------------------------------------------------------------------------------
public:
//***********************************************************************************
// ���������� ������������� � ����������� ������ TFuncGen_DSC_LinLaw
//***********************************************************************************
 //����������� �� ���������
 TFuncGen_DSC_LinLaw() : TFuncGen_DSClipper() {_SetIncFunc(); _CalcFactors();};

 //����������� � �����������
 TFuncGen_DSC_LinLaw(const char* str) : TFuncGen_DSClipper(str)
 {_SetIncFunc(); _CalcFactors();};

 TFuncGen_DSC_LinLaw(double Umin, double Umax, double Pmin, double Pmax) :
 TFuncGen_DSClipper(Umin,Umax,Pmin,Pmax) {_SetIncFunc(); _CalcFactors();};

 //���������� ����������� ������
 virtual ~TFuncGen_DSC_LinLaw() {};
//---------------------------------------------------------------------------------------
//���������� ������������� ������-������� ������ TFuncGen_DSC_LinLaw
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
//���������� ���������� ������-������� ������ TFuncGen_DSC_LinLaw
//---------------------------------------------------------------------------------------
protected:
 //����� ������ (�������������) ������� F(U)
 virtual double Func(double InU) const {return InU;};
 //virtual double Func(double U) const {return U;};
 //����� �������� (���������������) ������� InvF(P)
 virtual double InvFunc(double P) const {return P;};
 //��� ������ (�������������) ������� F(U) � ��������� ����
 virtual string& Func(string& s) const {s.assign("F(x) = x"); return s;};
 //��� �������� (���������������) ������� InvF(P) � ��������� ����
 virtual string& InvFunc(string& s) const {s.assign("InvF(x) = x"); return s;};
};//���������� ���������� ������ TFuncGen_DSC_LinLaw


//****************************************************************************************
//���������� ������������ ������ TFuncGen_DSC_DecLinLaw �� ������������ (��������) ������
//TFuncGen_DSClipper
//Copyright (c) JohnSoft 2006. All rights reserved.
//Initial date: September,13,2006; Final date:
//����� ��������� ��� �������� ������ TFuncGen_DSClipper � ��������� ��� ��������������
//�������������� �������������� �������� �������� � ������������ �� ���������� ���������
//������:
//1. ������ ��������������:   P(U) = k*F(U) + c, ��� F(x) = x;
//2. �������� ��������������: U(P) = InvF((P-c)/k),  InvF(x) = x.
//�������� �����������-������������� ���������� �� ���������� ��������� ������, ��� ����-
//���� ���������� �������� ������� ��� ���������� ��� ��������� ��� ����������� � ����������
//������ ��������� ������� ��� ���������� ��� �������� ��� �������������.
//****************************************************************************************
class TFuncGen_DSC_DecLinLaw : public TFuncGen_DSClipper {

//-----------------------------------------------------------------------------------
// I. ���������� ������������� ������-������� ������ TFuncGen_DSC_DecLinLaw
//-----------------------------------------------------------------------------------
public:
//***********************************************************************************
// ���������� ������������� � ����������� ������ TFuncGen_DSC_DecLinLaw
//***********************************************************************************
 //����������� �� ���������
 TFuncGen_DSC_DecLinLaw() : TFuncGen_DSClipper() {_SetDecFunc(); _CalcFactors();};

 //����������� � �����������
 TFuncGen_DSC_DecLinLaw(const char* str) : TFuncGen_DSClipper(str)
 {_SetDecFunc(); _CalcFactors();};

 TFuncGen_DSC_DecLinLaw(double Umin, double Umax, double Pmin, double Pmax) :
 TFuncGen_DSClipper(Umin,Umax,Pmin,Pmax) {_SetDecFunc(); _CalcFactors();};

 //���������� ����������� ������
 virtual ~TFuncGen_DSC_DecLinLaw() {};
//---------------------------------------------------------------------------------------
//���������� ������������� ������-������� ������ TFuncGen_DSC_DecLinLaw
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
//���������� ���������� ������-������� ������ TFuncGen_DSC_DecLinLaw
//---------------------------------------------------------------------------------------
protected:
 //����� ������ (�������������) ������� F(U)
 virtual double Func(double InU) const {return InU;};
 //virtual double Func(double U) const {return U;};
 //����� �������� (���������������) ������� InvF(P)
 virtual double InvFunc(double P) const {return P;};
 //��� ������ (�������������) ������� F(U) � ��������� ����
 virtual string& Func(string& s) const {s.assign("F(x) = x [DecFunc]"); return s;};
 //��� �������� (���������������) ������� InvF(P) � ��������� ����
 virtual string& InvFunc(string& s) const {s.assign("InvF(x) = x [DecFunc]"); return s;};
};//���������� ���������� ������ TFuncGen_DSC_DecLinLaw


//****************************************************************************************
//���������� ������������ ������ TFuncGen_DSC_SqrtLaw �� ������������ (��������) ������
//TFuncGen_DSClipper
//Copyright (c) JohnSoft 2006. All rights reserved.
//Initial date: September,13,2006; Final date:
//����� ��������� ��� �������� ������ TFuncGen_DSClipper � ��������� ��� ��������������
//�������������� �������������� �������� �������� � ������������ �� ����������������������
//������:
//1. ������ ��������������:   P(U) = k*F(U) + c, ��� F(x) = Sqrt(x);
//2. �������� ��������������: U(P) = InvF((P-c)/k),  InvF(x) = x^2.
//****************************************************************************************
class TFuncGen_DSC_SqrtLaw : public TFuncGen_DSClipper {

//-----------------------------------------------------------------------------------
// I. ���������� ������������� ������-������� ������ TFuncGen_DSC_SqrtLaw
//-----------------------------------------------------------------------------------
public:
//***********************************************************************************
// ���������� ������������� � ����������� ������ TFuncGen_DSC_SqrtLaw
//***********************************************************************************
 //����������� �� ���������
 TFuncGen_DSC_SqrtLaw() : TFuncGen_DSClipper() {_SetIncFunc(); _CalcFactors();};
 //����������� � �����������
 TFuncGen_DSC_SqrtLaw(const char* str) : TFuncGen_DSClipper(str)
 {_SetIncFunc(); _CalcFactors();};

 TFuncGen_DSC_SqrtLaw(double Umin, double Umax, double Pmin, double Pmax) :
 TFuncGen_DSClipper(Umin,Umax,Pmin,Pmax) {_SetIncFunc(); _CalcFactors();};

 //���������� ����������� ������
 virtual ~TFuncGen_DSC_SqrtLaw() {};
//---------------------------------------------------------------------------------------
//���������� ������������� ������-������� ������ TFuncGen_DSC_SqrtLaw
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
//���������� ���������� ������-������� ������ TFuncGen_DSC_SqrtLaw
//---------------------------------------------------------------------------------------
protected:
 //����� ������ (�������������) ������� F(U)
 virtual double Func(double U) const {return sqrt(U);};
 //����� �������� (���������������) ������� InvF(P)
 virtual double InvFunc(double P) const {return P*P;};
 //��� ������ (�������������) ������� F(U) � ��������� ����
 virtual string& Func(string& s) const {s.assign("F(x) = Sqrt(x)"); return s;};
 //��� �������� (���������������) ������� InvF(P) � ��������� ����
 virtual string& InvFunc(string& s) const {s.assign("InvF(x) = x^2"); return s;};
};//���������� ���������� ������ TFuncGen_DSC_SqrtLaw

//****************************************************************************************
//���������� ������������ ������ TFuncGen_DSC_ExpLaw �� ������������ (��������) ������
//TFuncGen_DSClipper
//Copyright (c) JohnSoft 2006. All rights reserved.
//Initial date: September,13,2006; Final date:
//����� ��������� ��� �������� ������ TFuncGen_DSClipper � ��������� ��� ��������������
//�������������� �������������� �������� �������� � ������������ �� �����������������
//������:
//1. ������ ��������������:   P(U) = k*F(U) + c, ��� F(x) = Exp(x);
//2. �������� ��������������: U(P) = InvF((P-c)/k),  InvF(x) = Ln(x).
//****************************************************************************************
class TFuncGen_DSC_ExpLaw : public TFuncGen_DSClipper {

//-----------------------------------------------------------------------------------
// I. ���������� ������������� ������-������� ������ TFuncGen_DSC_ExpLaw
//-----------------------------------------------------------------------------------
public:
//***********************************************************************************
// ���������� ������������� � ����������� ������ TFuncGen_DSC_ExpLaw
//***********************************************************************************
 //����������� �� ���������
 TFuncGen_DSC_ExpLaw() : TFuncGen_DSClipper() {_SetIncFunc(); _CalcFactors();};
 //����������� � �����������
 TFuncGen_DSC_ExpLaw(const char* str) : TFuncGen_DSClipper(str)
 {_SetIncFunc(); _CalcFactors();};

 TFuncGen_DSC_ExpLaw(double Umin, double Umax, double Pmin, double Pmax) :
 TFuncGen_DSClipper(Umin,Umax,Pmin,Pmax) {_SetIncFunc(); _CalcFactors();};

 //���������� ����������� ������
 virtual ~TFuncGen_DSC_ExpLaw() {};
//---------------------------------------------------------------------------------------
//���������� ������������� ������-������� ������ TFuncGen_DSC_ExpLaw
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
//���������� ���������� ������-������� ������ TFuncGen_DSC_ExpLaw
//---------------------------------------------------------------------------------------
protected:
 //����� ������ (�������������) ������� F(U)
 virtual double Func(double U) const {return exp(U);};
 //����� �������� (���������������) ������� InvF(P)
 virtual double InvFunc(double P) const {return (P <= 0) ? this->_Umin : log(P);};
 //��� ������ (�������������) ������� F(U) � ��������� ����
 virtual string& Func(string& s) const {s.assign("F(x) = Exp(x)"); return s;};
 //��� �������� (���������������) ������� InvF(P) � ��������� ����
 virtual string& InvFunc(string& s) const {s.assign("InvF(x) = Ln(x)"); return s;};
};//���������� ���������� ������ TFuncGen_DSC_ExpLaw

//****************************************************************************************
//���������� ������������ ������ TFuncGen_DSC_DecExpLaw �� ������������ (��������) ������
//TFuncGen_DSClipper
//Copyright (c) JohnSoft 2006. All rights reserved.
//Initial date: September,13,2006; Final date:
//����� ��������� ��� �������� ������ TFuncGen_DSClipper � ��������� ��� ��������������
//�������������� �������������� �������� �������� � ������������ �� ���������� ��������-
//��������� ������:
//1. ������ ��������������:   P(U) = k*F(U) + c, ��� F(x) = Exp(-x);
//2. �������� ��������������: U(P) = InvF((P-c)/k),  InvF(x) = -Ln(x) = Ln(1/x).
//�������� ����������� ���������� ��������� �� ����������������� ������ ��������, ���
//������������� �������� ������ �������� ��� ����������� ��������� ������� ��� ����������
//��������� ��� ����������� �������� ��� �������������.
//****************************************************************************************
class TFuncGen_DSC_DecExpLaw : public TFuncGen_DSClipper {

//-----------------------------------------------------------------------------------
// I. ���������� ������������� ������-������� ������ TFuncGen_DSC_DecExpLaw
//-----------------------------------------------------------------------------------
public:
//***********************************************************************************
// ���������� ������������� � ����������� ������ TFuncGen_DSC_DecExpLaw
//***********************************************************************************
 //����������� �� ���������
 TFuncGen_DSC_DecExpLaw() : TFuncGen_DSClipper() {_SetDecFunc(); _CalcFactors();};
 //����������� � �����������
 TFuncGen_DSC_DecExpLaw(const char* str) : TFuncGen_DSClipper(str)
 {_SetDecFunc(); _CalcFactors();};

 TFuncGen_DSC_DecExpLaw(double Umin, double Umax, double Pmin, double Pmax) :
 TFuncGen_DSClipper(Umin,Umax,Pmin,Pmax) {_SetDecFunc(); _CalcFactors();};

 //���������� ����������� ������
 virtual ~TFuncGen_DSC_DecExpLaw() {};
//---------------------------------------------------------------------------------------
//���������� ������������� ������-������� ������ TFuncGen_DSC_DecExpLaw
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
//���������� ���������� ������-������� ������ TFuncGen_DSC_DecExpLaw
//---------------------------------------------------------------------------------------
protected:
 //����� ������ (�������������) ������� F(U)
 virtual double Func(double U) const {return exp(-U);};
 //����� �������� (���������������) ������� InvF(P)
 virtual double InvFunc(double P) const {return (P <= 0) ? this->_Umin : log(P);};
 //��� ������ (�������������) ������� F(U) � ��������� ����
 virtual string& Func(string& s) const {s.assign("F(x) = Exp(-x)"); return s;};
 //��� �������� (���������������) ������� InvF(P) � ��������� ����
 virtual string& InvFunc(string& s) const {s.assign("InvF(x) = Ln(1/x)"); return s;};
};//���������� ���������� ������ TFuncGen_DSC_DecExpLaw


//****************************************************************************************
//���������� ������������ ������ TFuncGen_DSC_SinLaw �� ������������ (��������) ������
//TFuncGen_DSClipper
//Copyright (c) JohnSoft 2006. All rights reserved.
//Initial date: September,13,2006; Final date:
//����� ��������� ��� �������� ������ TFuncGen_DSClipper � ��������� ��� ��������������
//�������������� �������������� �������� �������� � ������������ �� ��������������� ������
//F(x) = Sin(x), � D(x) = [0..Pi/2], E(F(x) = [0..1]. � ���� ������ F(x) ��������� ���:
//F(x) = Sin[a*(X - Xmin)], ��� a = 0.5*Pi/(Xmax-Xmin).
//����� �������, ������������ k � c ����������� �� ��������: k = Pmax-Pmin, c = Pmin.
//1. ������ ��������������: P(U) = k*F(U) + c = (Pmax-Pmin)*Sin(a*(U-Umin)) + Pmin, ���
//   a = 0.5Pi/(Umax-Umin);
//2. �������� ��������������: U(P) = InvF((P-c)/k) = (1/a)*ArcSin((P-c)/k) + Umin.
//****************************************************************************************
class TFuncGen_DSC_SinLaw : public TFuncGen_DSClipper {
//-----------------------------------------------------------------------------------
// ���������� ���������� ������-������ ������ TFuncGen_DSClipper_SinLaw
//-----------------------------------------------------------------------------------
protected:
 double a;//a = 0.5Pi/(Umax-Umin)
//-----------------------------------------------------------------------------------
// I. ���������� ������������� ������-������� ������ TFuncGen_DSC_SinLaw
//-----------------------------------------------------------------------------------
public:
//***********************************************************************************
// ���������� ������������� � ����������� ������ TFuncGen_DSC_SinLaw
//***********************************************************************************
 //����������� �� ���������
 TFuncGen_DSC_SinLaw() : TFuncGen_DSClipper() {_SetIncFunc(); _CalcFactors();};
 //����������� � �����������
 TFuncGen_DSC_SinLaw(const char* str) : TFuncGen_DSClipper(str)
 {_SetIncFunc(); _CalcFactors();};

 TFuncGen_DSC_SinLaw(double Umin, double Umax, double Pmin, double Pmax) :
 TFuncGen_DSClipper(Umin,Umax,Pmin,Pmax) {_SetIncFunc(); _CalcFactors();};

 //���������� ����������� ������
 virtual ~TFuncGen_DSC_SinLaw() {};
//---------------------------------------------------------------------------------------
//���������� ������������� ������-������� ������ TFuncGen_DSC_SinLaw
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
//���������� ���������� ������-������� ������ TFuncGen_DSC_SinLaw
//---------------------------------------------------------------------------------------
protected:
 //������ ������������� k, c � a ��������������� ��������������� �� ������ ������
 virtual void _CalcFactors() {k = _Pmax-_Pmin; c = _Pmin; a = HalfPI/(_Umax-_Umin);};
 //����� ������ (�������������) ������� F(U)
 virtual double Func(double U) const {return sin(a*(U - _Umin));};
 //����� �������� (���������������) ������� InvF(P)
 virtual double InvFunc(double P) const {return asin(P)/a + _Umin;};
 //��� ������ (�������������) ������� F(U) � ��������� ����
 virtual string& Func(string& s) const
 {string t; _GetFactorA(t); s.assign("F(x) = Sin(a*[x-Umin]), "); s.append(t); return s;};
 //��� �������� (���������������) ������� InvF(P) � ��������� ����
 virtual string& InvFunc(string& s) const
 {string t; _GetFactorA(t);
  s.assign("InvF(x) = (1/a)*ArcSin(x)+Umin, "); s.append(t); return s;
 };
 //������������ ������������ <a = 0.5Pi/(Umax-Umin)> � ������ s. ������ ������:
 //< a = 0.5Pi/(Umax-Umin) = <d_value> >
 string& _GetFactorA(string& s) const;

};//���������� ���������� ������ TFuncGen_DSC_SinLaw


//****************************************************************************************
//���������� ������������ ������ TFuncGen_DSC_CosLaw �� ������������ (��������) ������
//TFuncGen_DSClipper
//Copyright (c) JohnSoft 2006. All rights reserved.
//Initial date: September,13,2006; Final date:
//����� ��������� ��� �������� ������ TFuncGen_DSClipper � ��������� ��� ��������������
//�������������� �������������� �������� �������� � ������������ �� ����������������� ������
//F(x) = Cos(x), � D(x) = [0..Pi/2], E(F(x) = [0..1]. � ���� ������ F(x) ��������� ���:
//F(x) = Cos[a*(X - Xmin)], ��� a = 0.5*Pi/(Xmax-Xmin).
//������� ����������� � ������������� ����������� ��� ���������-��������� �������, �.�.
//� ������������ ��������� ����������� �������� � � ������������ �������� �����������
//���������.
//����� �������, ������������ k � c ����������� �� ��������: k = Pmax-Pmin, c = Pmin.
//1. ������ ��������������: P(U) = k*F(U) + c = (Pmax-Pmin)*Cos(a*(U-Umin)) + Pmin, ���
//   a = 0.5Pi/(Umax-Umin);
//2. �������� ��������������: U(P) = InvF((P-c)/k) = (1/a)*ArcCos((P-c)/k) + Umin.
//****************************************************************************************
class TFuncGen_DSC_CosLaw : public TFuncGen_DSClipper {
//-----------------------------------------------------------------------------------
// ���������� ���������� ������-������ ������ TFuncGen_DSClipper_CosLaw
//-----------------------------------------------------------------------------------
protected:
 double a;//a = 0.5Pi/(Umax-Umin)
//-----------------------------------------------------------------------------------
// I. ���������� ������������� ������-������� ������ TFuncGen_DSC_CosLaw
//-----------------------------------------------------------------------------------
public:
//***********************************************************************************
// ���������� ������������� � ����������� ������ TFuncGen_DSC_CosLaw
//***********************************************************************************
 //����������� �� ���������
 TFuncGen_DSC_CosLaw() : TFuncGen_DSClipper() {_SetDecFunc(); _CalcFactors();};
 //����������� � �����������
 TFuncGen_DSC_CosLaw(const char* str) : TFuncGen_DSClipper(str)
 {_SetDecFunc(); _CalcFactors();};

 TFuncGen_DSC_CosLaw(double Umin, double Umax, double Pmin, double Pmax) :
 TFuncGen_DSClipper(Umin,Umax,Pmin,Pmax) {_SetDecFunc(); _CalcFactors();};

 //���������� ����������� ������
 virtual ~TFuncGen_DSC_CosLaw() {};
//---------------------------------------------------------------------------------------
//���������� ������������� ������-������� ������ TFuncGen_DSC_CosLaw
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
//���������� ���������� ������-������� ������ TFuncGen_DSC_CosLaw
//---------------------------------------------------------------------------------------
protected:
 //������ ������������� k, c � a ��������������� ��������������� �� ������ ��������
 virtual void _CalcFactors() {k = _Pmax-_Pmin; c = _Pmin; a = HalfPI/(_Umax-_Umin);};
 //����� ������ (�������������) ������� F(U)
 virtual double Func(double U) const {return cos(a*(U - _Umin));};
 //����� �������� (���������������) ������� InvF(P)
 virtual double InvFunc(double P) const {return acos(P)/a + _Umin;};
 //��� ������ (�������������) ������� F(U) � ��������� ����
 virtual string& Func(string& s) const
 {string t; _GetFactorA(t); s.assign("F(x) = Cos(a*[x-Umin]), "); s.append(t); return s;};
 //��� �������� (���������������) ������� InvF(P) � ��������� ����
 virtual string& InvFunc(string& s) const
 {string t; _GetFactorA(t);
  s.assign("InvF(x) = (1/a)*ArcCos(x)+Umin, "); s.append(t); return s;
 };
 //������������ ������������ <a = 0.5Pi/(Umax-Umin)> � ������ s. ������ ������:
 //< a = 0.5Pi/(Umax-Umin) = <d_value> >
 string& _GetFactorA(string& s) const;

};//���������� ���������� ������ TFuncGen_DSC_CosLaw


//****************************************************************************************
//���������� ������������ ������ TFuncGen_DSC_HyperbolicLaw �� ������������ (��������)
//������ TFuncGen_DSClipper
//Copyright (c) JohnSoft 2006. All rights reserved.
//Initial date: September,13,2006; Final date:
//����� ��������� ��� �������� ������ TFuncGen_DSClipper � ��������� ��� ��������������
//�������������� �������������� �������� �������� � ������������ �� ���������������� ������:
//1. ������ ��������������:   P(U) = k*F(U) + c, ��� F(x) = 1/(x+1);
//2. �������� ��������������: U(P) = InvF((P-c)/k),  InvF(x) = 1/x - 1 = k/(P-c) - 1.
//****************************************************************************************
class TFuncGen_DSC_HyperbolicLaw : public TFuncGen_DSClipper {

//-----------------------------------------------------------------------------------
// I. ���������� ������������� ������-������� ������ TFuncGen_DSC_HyperbolicLaw
//-----------------------------------------------------------------------------------
public:
//***********************************************************************************
// ���������� ������������� � ����������� ������ TFuncGen_DSC_HyperbolicLaw
//***********************************************************************************
 //����������� �� ���������
 TFuncGen_DSC_HyperbolicLaw() : TFuncGen_DSClipper() {_SetDecFunc(); _CalcFactors();};

 //����������� � �����������
 TFuncGen_DSC_HyperbolicLaw(const char* str) : TFuncGen_DSClipper(str)
 {_SetDecFunc(); _CalcFactors();};

 TFuncGen_DSC_HyperbolicLaw(double Umin, double Umax, double Pmin, double Pmax) :
 TFuncGen_DSClipper(Umin,Umax,Pmin,Pmax) {_SetDecFunc(); _CalcFactors();};

 //���������� ����������� ������
 virtual ~TFuncGen_DSC_HyperbolicLaw() {};
//---------------------------------------------------------------------------------------
//���������� ������������� ������-������� ������ TFuncGen_DSC_HyperbolicLaw
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
//���������� ���������� ������-������� ������ TFuncGen_DSC_HyperbolicLaw
//---------------------------------------------------------------------------------------
protected:
 //����� ������ (�������������) ������� F(U)
 virtual double Func(double InU) const {return 1/(InU+1.0);};
 //����� �������� (���������������) ������� InvF(P)
 virtual double InvFunc(double P) const {return 1/P-1.0;};
 //��� ������ (�������������) ������� F(U) � ��������� ����
 virtual string& Func(string& s) const {s.assign("F(x) = 1/(x+1)"); return s;};
 //��� �������� (���������������) ������� InvF(P) � ��������� ����
 virtual string& InvFunc(string& s) const {s.assign("InvF(x) = 1/x - 1"); return s;};
};//���������� ���������� ������ TFuncGen_DSC_HyperbolicLaw


//--------------------------- The end of file "CoDec.h" ----------------------------------
#endif //���������� ���������� ������������� ����� "CoDec.h"
