//mathfunc.cpp
//���������� ������� ��������� ������������ ��������� � ����
//������� �� �������
//Initial date: August,22,1998. Final date:
//Copyright (c) JohnSoft 1998. All rights reserved

#include "mathfunc.h"
const double DegRad = 0.01745329251994;//DEG_RAD
const double RadDeg = 57.2957795131;//RAD_DEG
const double TwoPI = 6.283185307178;//TWO_PI
//������ ������������ ������� � �������������� ���� ��� ������
const LAWNAME LawName[] = {
 {UNEXIST,DETLAW,"��� �� �������","Name not found","Math. notate not found",
  "UNEXIST_LAW"},                                                            //0
 //����������������� ������
 {ZERO_LAW,DETLAW,"������� �����","Zero law","F(t) = 0","ZERO_LAW"},        //1
 {CONST_LAW,DETLAW,"���������� �����","Constant law","F(t) = A",
  "CONST_LAW"},                                                             //2
 {LINEAR_LAW,DETLAW,"�������� �����","Linear law","F(t) = A*t + B",
  "LINEAR_LAW"},                                                            //3
 {SQUARE_LAW,DETLAW,"������������ �����","Square law","F(t) = A*t^2+B*t+C",
  "SQUARE_LAW"},                                                            //4
 {EXP_LAW,DETLAW,"���������������� �����","Exponential law","F(t) = A*EXP[B*t]",
  "EXP_LAW"},                                                               //5
 {COS_LAW,DETLAW,"���������������� �����","Cosine law","F(t) = A*COS(B*t+C)",
  "COS_LAW"},                                                               //6
 {SIN_LAW,DETLAW,"�������������� �����","Sine law","F(t) = A*SIN(B*t+C)",
  "SIN_LAW"},                                                               //7
 {HYPERBOL_LAW,DETLAW,"��������������� �����","Hyperbolic law","F(t) = A/t",
  "HYPERBOL_LAW"},                                                          //8
 {LN_LAW,DETLAW,"����� LN(x) ","Natural logarithm law","F(t) = LN(t)",
  "LN_LAW"},                                                                //9
 {LOG10_LAW,DETLAW,"����� LOG10(x) ","Decimal logarithm law","F(t) = LOG10(t)",
  "LOG10_LAW"},                                                             //10
 {LOGA_LAW,DETLAW,"����� LOGa(x)","LOGa(x) law","F(t) = LOGA(t)",
  "LOGA_LAW"},                                                              //11
 {SQRT_LAW,DETLAW,"����� ����������� �����","Square root law","F(t) = Sqrt(A*t+B)",
  "SQRT_LAW"},                                                              //12
 //��������� ������
 {RAND_LAW,CASUALLAW,"�������������� ����� [0..1]","RANDOM LAW","[0..1]",
  "RAND_LAW"},                                                              //13
 {RAND2PI_LAW,CASUALLAW,"�������������� ����� [0..2PI]","RANDOM LAW",
  "[0..2PI]","RAND2PI_LAW"},                                                //14
 {RANDAB_LAW,CASUALLAW,"�������������� ����� [A..B]","RANDOM LAW ","[A..B]",
  "RANDAB_LAW"},                                                            //15
 {RELAY_LAW,CASUALLAW,"��������� ����� �����","RELAY LAW","[0..+INFINITE]",
  "RELAY_LAW"},                                                             //16
 //��� ��������� - � ������ ��������� �������
 {DET1_LAW,CASUALLAW,"����������� �����","UNCASUAL LAW"," 1 ","DET1_LAW"},  //17
 //��� ���� - � ������ �������� �������
 {DET0_LAW,CASUALLAW,"����������� �����","UNCASUAL LAW"," 0 ","DET0_LAW"},  //18
 //��������� �����
 {TABLE_LAW,DETLAW,"��������� �����","TABLE LAW","  ","TABLE_LAW"},         //19
 //����� �������������� ��������
 {SQUAREPULSE_LAW,DETLAW,"������������� �������","SQUARE PULSE","  ",
  "SQUAREPULSE_LAW"}                                                        //20
};

//������ ������������ ���������� �������
const  MEASURENAME MeasureName[] = {
{NONDIM,"������������","","NON-DIMENSIONAL",""}, //0
{SECOND,"�������","���","SECOND","SEC"},         //1
{RADIAN,"������","���","RADIAN","RAD"},          //2
{DEGREE,"������","����","DEGREE","DEG"},         //3
{VOLT,  "�����","�","VOLT","V"},                 //4
{HZ,"�������","��","HERTZ","HZ"},                //5
{PULSAT,"������� �������","���/�","PULSATANCE","RAD/SEC"},                //6
{DEG_SEC,"������� ��������","����/�","ANGULAR VELOCITY","DEG/SEC"},       //7
{DEG_SEC2,"������� ���������","����/�^2","ANG. ACCELERATION","DEG/SEC^2"},//8
{RAD_SEC,"������� ��������","���/�","ANGULAR VELOCITY","RAD/SEC"},       //9
{RAD_SEC2,"������� ���������","���/�^2","ANG. ACCELERATION","RAD/SEC^2"} //10
};

//������������� ���������� ��������� �����
TRandomGenerator RNG(T_16,0);

//*****************************************************************************
//������:
//������������ �������:
//����������:
//������� ������������� ��� ...
//
//-----------------------------------------------------------------------------
//������������ ���������:
//1. -
//2. -
//-----------------------------------------------------------------------------
//������������ ��������:
//*****************************************************************************

//*************************************
//I. ����������������� �������
//************************************
//*****************************************************************************
//������: I.1
//������������ �������: double ZeroLaw(const ARGLIST* Arg)
//����������: F(t) = 0 - ������� ������� �� �������
//������� ���������� ����� ��������� �������� �� ������� ��� ������� ������
//�������� ��������� ����� ����. ������ ������� �� ������� �� ���������� �������
//��� ���������� � ������ Arg.
//��������� ������ Arg ��� ���������� ������ ZEROLAW()
//���������� ����������: 1
//1. Arg.List[0] - ������� (���������) ����� t (�������� ������� F(t) - t)
//-----------------------------------------------------------------------------
//������������ ���������:
//1. struct ARGLIST* Arg - ������ ���������� ������ ��������� ZeroLaw
//-----------------------------------------------------------------------------
//������������ ��������: double
// ������ ���������� ����.
//*****************************************************************************
double ZeroLaw(const ARGLIST* Arg)
{
 try
  {
   if (Arg == NULL) //����������� ���������
    throw ARG_MISMATCH;
   if (Arg->Size != 1) //��� ZeroLaw ������ Arg ������ ����� ���� ��������
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
//������: I.2
//������������ �������: double ConstLaw(const ARGLIST* Arg)
//����������: F(t) = const - ���������� ������� �� �������
//������� ���������� ����� ��������� �������� �� ������� ��� ������� ������
//�������� ��������� �� �������. ��� ������ ������� ����������� �� �������� ���
//��������� ����� ������ Arg. ��� ���� ������������ ������ ������ �������� -
//Arg.List[1].
//��������� ������ Arg ��� ���������� ������ CONSTLAW()
//���������� ����������: 2
//1. Arg.List[0] - ������� (���������) ����� t (�������� ������� F(t) - t)
//2. Arg.List[1] - const (�������� ���� double). F(t) = 5.0 => arg[1] = 5.0
//-----------------------------------------------------------------------------
//������������ ���������:
//1. struct ARGLIST* Arg - ������ ���������� ������ ��������� ConstLaw
//-----------------------------------------------------------------------------
//������������ ��������: double Arg.List[1]
//*****************************************************************************
double ConstLaw(const ARGLIST* Arg)
{
 try
  {
   if (Arg == NULL) //����������� ���������
    throw ARG_MISMATCH;
   if (Arg->Size != 2) //������ ���� �������� ����� ��� ���������
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
//������: I.3
//������������ �������: double LinearLaw(const ARGLIST* Arg)
//����������: F(t) = A*t+B - �������� ������� �� �������
//������� ��������� �������� ����� ��������� �������� �� �������. ��� ������
// ������� ����������� �� �������� ��� ��������� ����� ������ Arg.
//��������� ������ Arg ��� ���������� ������ LINEARLAW()
//���������� ����������: 3
//1. Arg.List[0] - t - ������� ����� (��������� �����)
//2. Arg.List[1] - A
//3. Arg.List[2] - B
//-----------------------------------------------------------------------------
//������������ ���������:
//1. struct ARGLIST* Arg - ������ ���������� ������ ��������� LinearLaw
//-----------------------------------------------------------------------------
//������������ ��������: double value = A*t+B
//*****************************************************************************
double LinearLaw(const ARGLIST* Arg)
{
 try
  {
   if (Arg == NULL) //����������� ���������
    throw ARG_MISMATCH;
   if (Arg->Size != 3) //������ ���� �������� ����� ��� ���������
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
//������: I.4
//������������ �������: double SquareLaw(const ARGLIST* Arg)
//����������: F(t) = A*t^2+B*t+C - ������������ ������� �� �������
//������� ��������� ������������ ����� ��������� �������� �� �������. ��� ������
// ������� ����������� �� �������� 4 ��������� ����� ��������� Arg.
//��������� ������ Arg ��� ���������� ������ SQUARELAW()
//���������� ����������: 4
//1. Arg.List[0] - double t - ������� ����� (��������� �����)
//2. Arg.List[1] - double A
//3. Arg.List[2] - double B
//4. Arg.List[3] - double C
//-----------------------------------------------------------------------------
//������������ ���������:
//1. struct ARGLIST* Arg - ������ ���������� ������ ��������� SquareLaw
//-----------------------------------------------------------------------------
//������������ ��������: double value = A*t^2+B*t+C
//*****************************************************************************
double SquareLaw(const ARGLIST* Arg)
{
 double t;
 try
  {
   if (Arg == NULL) //����������� ���������
    throw ARG_MISMATCH;
   if (Arg->Size != 4) //������ ���� �������� ����� 4 ���������
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
//������: I.5
//������������ �������: double ExpLaw(const ARGLIST* Arg)
//����������: F(t) = A*EXP(B*t)
//������� ��������� ���������������� ����� ��������� �������� �� �������. ���
//������ ������� ����������� �� �������� 3 ��������� ����� ��������� Arg.
//��������� ������ Arg ��� ���������� ������ EXPLAW()
//���������� ����������: 3
//1. Arg.List[0] - double t - ������� ����� (��������� �����)
//2. Arg.List[1] - double A
//3. Arg.List[2] - double B
//-----------------------------------------------------------------------------
//������������ ���������:
//1. struct ARGLIST* Arg - ������ ���������� ������ ��������� ExpLaw
//-----------------------------------------------------------------------------
//������������ ��������: double value = A*EXP(B*t)
//*****************************************************************************
double ExpLaw(const ARGLIST* Arg)
{
 double t;
 try
  {
   if (Arg == NULL) //����������� ���������
    throw ARG_MISMATCH;
   if (Arg->Size != 3) //������ ���� �������� ����� 3 ���������
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
//������: I.6
//������������ �������: double CosLaw(const ARGLIST* Arg)
//����������: F(t) = A*COS(B*t+C) - ���������������� ������� �� �������
//������� ��������� ���������������� ����� ��������� �������� �� �������. ���
//������ ������� ����������� �� �������� 4 ��������� ����� ��������� Arg.
//��������� ������ Arg ��� ���������� ������ COSLAW()
//���������� ����������: 4
//1. Arg.List[0] - double t - ������� ����� (��������� �����)
//2. Arg.List[1] - double A - ���������
//3. Arg.List[2] - double B - ������� ������� B = 2*PI*f
//4. Arg.List[3] - double C - ��������� ����
//-----------------------------------------------------------------------------
//������������ ���������:
//1. struct ARGLIST* Arg - ������ ���������� ������ ��������� CosLaw
//-----------------------------------------------------------------------------
//������������ ��������: double value = A*COS(B*t+C)
//*****************************************************************************
double CosLaw(const ARGLIST* Arg)
{
 double t;
 try
  {
   if (Arg == NULL) //����������� ���������
    throw ARG_MISMATCH;
   if (Arg->Size != 4) //������ ���� �������� ����� 4 ���������
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
//������: I.7
//������������ �������: double SinLaw(const ARGLIST* Arg)
//����������: F(t) = A*SIN(B*t+C) - �������������� ������� �� �������
//������� ��������� �������������� ����� ��������� �������� �� �������. ���
//������ ������� ����������� �� �������� 4 ��������� ����� ��������� Arg.
//��������� ������ Arg ��� ���������� ������ SINLAW()
//���������� ����������: 4
//1. Arg.List[0] - double t - ������� ����� (��������� �����)
//2. Arg.List[1] - double A - ���������
//3. Arg.List[2] - double B - ������� ������� B = 2*PI*f
//4. Arg.List[3] - double C - ��������� ����
//-----------------------------------------------------------------------------
//������������ ���������:
//1. struct ARGLIST* Arg - ������ ���������� ������ ��������� SinLaw
//-----------------------------------------------------------------------------
//������������ ��������: double value = A*SIN(B*t+C)
//*****************************************************************************
double SinLaw(const ARGLIST* Arg)
{
 double t;
 try
  {
   if (Arg == NULL) //����������� ���������
    throw ARG_MISMATCH;
   if (Arg->Size != 4) //������ ���� �������� ����� 4 ���������
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
//������: I.8
//������������ �������: double HyperbolLaw(const ARGLIST* Arg)
//����������: F(t) = A/t - ��������������� ������� �� �������
//������� ��������� ��������������� ����� ��������� �������� �� �������. ���
//������ ������� ����������� �� �������� 2 ��������� ����� ��������� Arg.
//��������� ������ Arg ��� ���������� ������ HyperbolLaw()
//���������� ����������: 2
//1. Arg.List[0] - double t - ������� ����� (��������� �����)
//2. Arg.List[1] - double A - ���������
//-----------------------------------------------------------------------------
//������������ ���������:
//1. struct ARGLIST* Arg - ������ ���������� ������ ��������� HyperbolLaw
//-----------------------------------------------------------------------------
//������������ ��������: double value = A/t
//*****************************************************************************
double HyperbolLaw(const ARGLIST* Arg)
{
 double t;
 try
  {
   if (Arg == NULL) //����������� ���������
    throw ARG_MISMATCH;
   if (Arg->Size != 2) //������ ���� �������� ����� 2 ���������
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
//������: I.9
//������������ �������: double LnLaw(const ARGLIST* Arg)
//����������: F(t) = Ln(t) - ������� ������������ ��������� �� �������
//������� ��������� ��������������� ����� ��������� �������� �� �������. ���
//������ ������� ����������� �� �������� 1 �������� ����� ��������� Arg.
//��������� ������ Arg ��� ���������� ������ LnLaw()
//���������� ����������: 1
//1. Arg.List[0] - double t - ������� ����� (��������� �����)
//-----------------------------------------------------------------------------
//������������ ���������:
//1. struct ARGLIST* Arg - ������ ���������� ������ ��������� LnLaw
//-----------------------------------------------------------------------------
//������������ ��������: double value = Ln(t)
//*****************************************************************************
double LnLaw(const ARGLIST* Arg)
{
 double t;
 try
  {
   if (Arg == NULL) //����������� ���������
    throw ARG_MISMATCH;
   if (Arg->Size != 1) //������ ���� ������� ����� 1 ��������
    throw ARG_MISMATCH;
   if (Arg->List[0] < 0.0) //������������� ��������
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
   if (error_type == IMPOS_VALUE) //������������ �������� ��������� �������
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
//������: I.10
//������������ �������: double Log10Law(const ARGLIST* Arg)
//����������: F(t) = Log10(t) - ������� ����������� ��������� �� �������
//������� ��������� ��������������� ����� ��������� �������� �� �������. ���
//������ ������� ����������� �� �������� 1 �������� ����� ��������� Arg.
//��������� ������ Arg ��� ���������� ������ Log10Law()
//���������� ����������: 1
//1. Arg.List[0] - double t - ������� ����� (��������� �����)
//-----------------------------------------------------------------------------
//������������ ���������:
//1. struct ARGLIST* Arg - ������ ���������� ������ ��������� Log10Law
//-----------------------------------------------------------------------------
//������������ ��������: double value = Log10(t)
//*****************************************************************************
double Log10Law(const ARGLIST* Arg)
{
 double t;
 try
  {
   if (Arg == NULL) //����������� ���������
    throw ARG_MISMATCH;
   if (Arg->Size != 1) //������ ���� ������� ����� 1 ��������
    throw ARG_MISMATCH;
   if (Arg->List[0] < 0.0) //������������� ��������
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
   if (error_type == IMPOS_VALUE) //������������ �������� ��������� �������
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
//������: I.11
//������������ �������: double LogALaw(const ARGLIST* Arg)
//����������: F(t) = Log10(t) - ������� ��������� �� ��������� A �� �������
//������� ��������� ��������������� ����� ��������� �������� �� �������. ���
//������ ������� ����������� �� �������� 2 ��������� ����� ��������� Arg.
//��������� ������ Arg ��� ���������� ������ LogALaw()
//���������� ����������: 1
//1. Arg.List[0] - double t - ������� ����� (��������� �����)
//2. Arg.List[1] - double A - ��������� ���������
//-----------------------------------------------------------------------------
//������������ ���������:
//1. struct ARGLIST* Arg - ������ ���������� ������ ��������� Log10Law
//-----------------------------------------------------------------------------
//������������ ��������: double value = LogA(t)
//*****************************************************************************
double LogALaw(const ARGLIST* Arg)
{
 double t;
 try
  {
   if (Arg == NULL) //����������� ���������
    throw ARG_MISMATCH;
   if (Arg->Size != 2) //������ ���� �������� ����� 2 ���������
    throw ARG_MISMATCH;
   if (Arg->List[0] < 0.0) //������������� ��������
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
   if (error_type == IMPOS_VALUE) //������������ �������� ��������� �������
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
//������: I.12
//������������ �������: double SgrtLaw(const ARGLIST* Arg)
//����������: F(t) = Sgrt(A*t+B) - ������� ����������� ����� �� �������
//������� ��������� ����� ����������� ����� �� ��������� ��������. ��� ������
//������� ����������� �� �������� 3 ��������� ����� ��������� Arg.
//��������� ������ Arg ��� ���������� ������ SqrtLaw()
//���������� ����������: 3
//1. Arg.List[0] - double t - ������� ����� (��������� �����)
//2. Arg.List[1] - double A - ���������
//3. Arg.List[2] - double B - ���������
//-----------------------------------------------------------------------------
//������������ ���������:
//1. struct ARGLIST* Arg - ������ ���������� ������ ��������� SqrtLaw
//-----------------------------------------------------------------------------
//������������ ��������: double value = Sqrt(t)
//*****************************************************************************
double SqrtLaw(const ARGLIST* Arg)
{
 double t;
 try
  {
   if (Arg == NULL) //����������� ���������
    throw ARG_MISMATCH;
   if (Arg->Size != 3) //������ ���� �������� ����� 3 ���������
    throw ARG_MISMATCH;
   //���������� ������������ ���������
   t = Arg->List[0]*Arg->List[1]+Arg->List[2];
   if (t < 0.0) //������������� ��������
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
   if (error_type == IMPOS_VALUE) //������������ �������� ��������� �������
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
//������: I.13
//������������ �������: double SquarePulseLaw(const ARGLIST* Arg)
//����������:
//������� ��������� ����� ��������� �������������� �������� �� �������. ���
//������ ������� ����������� �� �������� 8 ���������� ����� ��������� Arg.
//��������� ������ Arg ��� ���������� ������ SquarePulseLaw()
//���������� ����������: 8
//1. Arg.List[0] - T - ������� ����� (��������� �����)
//2. Arg.List[1] - t0 - ������ ���������� ��������� � ������� ������� ��������-
//   ����� - ��������� (t0 >= 0)
//3. Arg.List[2] - timp - ������������ �������� - ��������� (timp > 0)
//4. Arg.List[3] - Timp - ������ ���������� ��������� - ��������� (Timp > timp)
//5. Arg.List[4] - Umax - ������� ������� ��� ���������� �������� - ���������
//6. Arg.List[5] - Umin - ������� ������� ��� ���������� �������� - ���������
//���������� ���������
//7. Arg.List[6] - flag - ��������� ����������� ������� - ����������
//   ��������� ��������: -1.0 - ������� �����������; 0.0 - ��������� ��������
//   ��������� ������� �������� � ������� �������������; +1.0 - ������� ����.
//8. Arg.List[7] - count - ������� ������� - ����������. ������� ������� ������-
//   ���� ������� ������� �� �������� 1) ��������� ��������, 2) ���������� ��-
//   ������, 3) ��������� ������� ��������. ������� ������� ���������� ���������
//   �� ������� (-1.0) ������ ��� ��� ��������� � �������.
//��������� ��������� ������� ����������: flag = 0.0, count = 0.0. ������ ����-
//��� ��������� ����� �� ����������. ��� ��� count ���������� ���������, �� �
//��������� ��������� �������������� �������� ����� ����� �������� ������������-
//�� ���������� (��������� �����, ��� �������, ������������� ��������).
//-----------------------------------------------------------------------------
//������������ ���������:
//1. struct ARGLIST* Arg - ������ ���������� ������ ��������� SqrtLaw
//-----------------------------------------------------------------------------
//������������ ��������: double value - ������� �������������� �������
//*****************************************************************************
double SquarePulseLaw(const ARGLIST* Arg)
{
 try
  {
   if (Arg == NULL) //����������� ���������
    throw ARG_MISMATCH;
   if (Arg->Size != 8) //������ ���� �������� ����� 8 ����������
    throw ARG_MISMATCH;
   //******************************************************************
   //���������� ������ �������������� �������� � ������ ������� T
   //******************************************************************
   //��������� ��������� �������� ������� ���������� ������� �������������
   if (Arg->List[0] == 0) {Arg->List[6] = 0; Arg->List[7] = 0;}

   if (Arg->List[6] == 0) //��������� �������� ������� ��������
   {if (Arg->List[1] == 0) //����� �������� t0 ����� ����
    { Arg->List[6] = 1; Arg->List[7] = Arg->List[2]; return Arg->List[4];}
    else //����� �������� t0 ������� �� ����
    {if (Arg->List[7] == 0) //������ ���������
     { Arg->List[7] = Arg->List[1]; return Arg->List[5];} //�������� ���: Umin
     else //����������� ���������
     { if (--Arg->List[7] > 0) return Arg->List[5]; //�������� ���: Umin
       //������ ������� ��������
       else
       {Arg->List[6] = 1; Arg->List[7] = Arg->List[2]; return Arg->List[4];}
     }
    }
   }
   else if (Arg->List[6] == 1) //������� ������������
   { if (--Arg->List[7] > 0) return Arg->List[4]; //������� ����
     else //�������� ���
     {Arg->List[6] = -1; Arg->List[7] = fabs(Arg->List[3] - Arg->List[2]);
      return Arg->List[5];
     }
   }
   else if (Arg->List[6] == -1) //�������� ���
   { if (--Arg->List[7] > 0) return Arg->List[5]; //�������� ���
     else //������� ��������
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
// II. ��������� �������
//*******************************************************************
//*****************************************************************************
//������: II.1
//������������ �������: double RandLaw(const ARGLIST* Arg)
//����������: �������������� ��������� ����� [0..1]
//������� ���������� ���������� �������������� �� �� ��������� [0..1].
//� ������ ���������� Arg ������� �� ���������
//��������� ������ Arg ��� ���������� ������ RandLaw()
//���������� ����������: 0
//-----------------------------------------------------------------------------
//������������ ���������:
//1. struct ARGLIST* Arg - ������ ���������� RandLaw
//-----------------------------------------------------------------------------
//������������ ��������: double - �� �������������� �������������
//*****************************************************************************
double RandLaw(const ARGLIST* Arg)
{
 //double value = (double) rand()/RAND_MAX;
 double value = RNG.Random();
 if (Arg != NULL) return value;
return  value;
}
//*****************************************************************************
//������: II.2
//������������ �������: double Rand2PiLaw(const ARGLIST* Arg)
//����������: �������������� ��������� ����� [0..2Pi]
//������� ���������� ���������� �������������� �� �� ��������� [0..2Pi].
//� ������ ���������� Arg ������� �� ���������
//��������� ������ Arg ��� ���������� ������ Rand2PiLaw()
//���������� ����������: 0
//-----------------------------------------------------------------------------
//������������ ���������:
//1. struct ARGLIST* Arg - ������ ���������� Rand2PiLaw
//-----------------------------------------------------------------------------
//������������ ��������: double - �� ���������� �������������� [0..2PI]
//*****************************************************************************
double Rand2PiLaw(const ARGLIST* Arg)
{
 return RandLaw(Arg)*TwoPI;
}
//*****************************************************************************
//������: II.3
//������������ �������: double RandABLaw(const ARGLIST* Arg)
//����������: �������������� ��������� ����� [A..B]
//(B-RandLaw()*(B-A))
//������� ���������� ���������� �������������� �� �� ��������� [A..B].
//��������� ������ Arg ��� ���������� ������ RandABLaw()
//���������� ����������: 2
// Arg.List[0] - A (������ ������� ��������� �������������)
// Arg.List[1] - B (������� ������� ��������� �������������)
//-----------------------------------------------------------------------------
//������������ ���������:
//1. struct ARGLIST* Arg - ������ ���������� RandABLaw
//-----------------------------------------------------------------------------
//������������ ��������: double - �� ���������� �������������� [A..B]
//*****************************************************************************
double RandABLaw(const ARGLIST* Arg)
{
 double value;
 try
   {
    if (Arg == NULL) //����������� ���������
     throw ARG_MISMATCH;
    if (Arg->Size != 3) //������ ���� �������� ����� 2 ���������
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
//������: II.4
//������������ �������: double RelayLaw(const ARGLIST* Arg)
//����������: ��������� ����� ����� [0..1]
//sqrt(-2*ln(RandLaw())
//������� ���������� �� �������������� �� ����� �� ��������� [0..1]. � ����
//��������� ������� ����� ��������� �������� - [+INFINITE..0]
//� ������ ���������� Arg ������� �� ���������
//��������� ������ Arg ��� ���������� ������ RelayLaw()
//���������� ����������: 0
//-----------------------------------------------------------------------------
//������������ ���������:
//1. struct ARGLIST* Arg - ������ ���������� RelayLaw
//-----------------------------------------------------------------------------
//������������ ��������: double - �� �������������� �� ��������� [0..1]
//*****************************************************************************
double RelayLaw(const ARGLIST* Arg)
{ double value;
  //���������� �������������� ��������� �������� �� ������ ���� ����� ����
  do { value = RandLaw(Arg); } while (value == 0.0);
  //��������� �� �������������� �� ������ �����
  value = sqrt(-2.0*log(value));
 return value;
}
//*****************************************************************************
//������: II.5
//������������ �������: double Det1Law(const ARGLIST* Arg)
//����������: ��������� ��������� �������� �� ������������
//������� ���������� 1. ����������� ��� ���������� �� ������������ ���������
//��������, ������� ������������� � ������� ����������.
//��������� ������ Arg ��� ���������� ������ Det1Law()
//���������� ����������: 0
//-----------------------------------------------------------------------------
//������������ ���������:
//1. struct ARGLIST* Arg - ������ ���������� Det1Law()
//-----------------------------------------------------------------------------
//������������ ��������: double - 1.0
//*****************************************************************************
double Det1Law(const ARGLIST* Arg)
{
 if (Arg == NULL) return 1.0;
 return 1.0;
}
//*****************************************************************************
//������: II.5
//������������ �������: double Det0Law(const ARGLIST* Arg)
//����������: ��������� ��������� �������� �� ������������
//������� ���������� 0. ����������� ��� ���������� �� ������������ ���������
//��������, ������� ������������ � ������� ����������.
//��������� ������ Arg ��� ���������� ������ Det0Law()
//���������� ����������: 0
//-----------------------------------------------------------------------------
//������������ ���������:
//1. struct ARGLIST* Arg - ������ ���������� Det0Law()
//-----------------------------------------------------------------------------
//������������ ��������: double - 0.0
//*****************************************************************************
double Det0Law(const ARGLIST* Arg)
{
 if (Arg == NULL) return 0.0;
 return 0.0;
}

//***********************************************************************
// III. �������� ������� ���������������� ������ ���������� Arg
//***********************************************************************

//*****************************************************************************
//������: III.1
//������������ �������: ARGLIST* CreateArgList(const ulong size)
//����������:
//������� ������������� ��� �������� ��������� ARGLIST ��� �������� ������
//����������. ��������� ARGLIST ������������ ����� ������������ ����������
//������ ARGLIST.List � ���������� ARGLIST.Size - ���������� ��������� �������.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. ulong size - ������ �������
//-----------------------------------------------------------------------------
//������������ ��������: ARGLIST *ptr - ��������� �� ��������� ���������
//*****************************************************************************
ARGLIST* CreateArgList(const ulong size)
{ ARGLIST *ptr = NULL;
  try //������������� ������ ��� �������� ������� � �����������
  {   //� �������������� �� �������������
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
//������: III.2
//������������ �������: ARGLIST* DeleteArgList(ARGLIST *Arg)
//����������:
//������� ������� ��������� ARGLIST �������� ������ ����������. ���������
//ARGLIST ������������ ����� ������������ ���������� ������ ARGLIST.List �
//���������� ARGLIST.Size - ���������� ��������� �������.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. ARGLIST *Arg - ��������� ���������� ������ ����������
//-----------------------------------------------------------------------------
//������������ ��������: ������� ���������
//*****************************************************************************
ARGLIST* DeleteArgList(ARGLIST *Arg)
{if (Arg != NULL)
 {if (Arg->List != NULL) {delete[] Arg->List; Arg->List = NULL; Arg->Size = 0L;}
  delete Arg; Arg = NULL;
 }
return Arg;
}
//*****************************************************************************
//������: III.3
//������������ �������:
// ARGLIST* CopyArgList(const ARGLIST *Source, ARGLIST* Target)
//����������:
//������� �������� ��������� ARGLIST �������� ������ ���������� �� ���������
//Source � �������� Target. ��� ����������� ���������� Target ������������.
//ARGLIST ������������ ����� ������������ ���������� ������ ARGLIST.List �
//���������� ARGLIST.Size - ���������� ��������� �������.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. const ARGLIST *Source - ��������� ��������� ������ ����������
//2. ARGLIST *Target - ��������� ������ ����������, � ������� ����� �����������
//   ���������� ������ Source
//-----------------------------------------------------------------------------
//������������ ��������: ARGLIST* Target - ��������� ������-���������
//*****************************************************************************
ARGLIST* CopyArgList(const ARGLIST* Source, ARGLIST* Target)
{
 if (Target != NULL) Target = DeleteArgList(Target);
 //�������� ������ - ������
 if (Source == NULL) return Target;
 //������� ������ Target ����������� ������� � Source
 Target = CreateArgList(Source->Size);
 //�������� ���������
 for (ulong i = 0; i < Target->Size; i++) Target->List[i] = Source->List[i];

return Target;
}
//*****************************************************************************
//������: III.4
//������������ �������:
//V_ARGLIST* CreateVectorArgList(const ulong VectorSize, const ulong ListSize=0)
//����������:
//������� ������������� ��� �������� ��������� V_ARGLIST ��� �������� ������
//�������� ARGLIST. ��������� V_ARGLIST ������������ ����� ������������ �������-
//��� ������ ���������� �� ARGLIST � ���������� V_ARGLIST.Size - ����������
//�������� ARGLIST, ���������� � ������� V_ARGLIST. ������� ������� ������ ����-
//������ ���� double ����������� �������, ������������� ���������� ListSize. ��-
//�������� ����������� �������� ���� ARGLIST ������������ ���������� VectorSize.
//������� ���������� ��������� �� ������ �������� ARGLIST - pV_ARGLIST.
//���� ListSize = 0 (�������� �� ���������), �� ��������� ARGLIST �� ���������.
//� ���� ������ ������ ���������� �� ���������, ���������������� ���������� NULL.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. const ulong VectorSize - ������ ������� V_ARGLIST;
//2. const ulong ListSize - ������ ��������� ARGLIST;
//-----------------------------------------------------------------------------
//������������ ��������: V_ARGLIST *ptr - ��������� �� ��������� ���������
//*****************************************************************************
V_ARGLIST* CreateVectorArgList(const ulong VectorSize, const ulong ListSize)
{ V_ARGLIST *ptr = NULL;
  try //������������� ������ ��� �������� ������� V_ARGLIST, ��� ������ �������-
  {   //�� ARGLIST, �������� � ������ ������� � �������������� �� �������������
   ptr = new V_ARGLIST;
   ptr->Size = VectorSize;
  	ptr->Vector = new ARGLIST*[VectorSize];
   //��������� ������������� ���������� �� ��������� ARGLIST
   for( ulong i=0; i<VectorSize; i++) ptr->Vector[i] = NULL;
   //������������� ������ ��� �������� ������ ���� double � ������ ���������
   //ARGLIST, �������� � ������� (�������) �������� ARGLIST.
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
//������: III.5
//������������ �������: V_ARGLIST* DeleteVectorArgList(V_ARGLIST *Vector)
//����������:
//������� ������� ��������� V_ARGLIST �������� �������� ���� ARGLIST. ���������
//V_ARGLIST ������������ ����� ������������ ���������� ������ ���������� ��
//��������� V_ARGLIST.Vector � ���������� V_ARGLIST.Size - ���������� ���������
//������� (�������).
//-----------------------------------------------------------------------------
//������������ ���������:
//1. V_ARGLIST *Vector - ��������� ���������� ������� �������� ARGLIST.
//-----------------------------------------------------------------------------
//������������ ��������:
// V_ARGLIST *Vector - ������� ���������
//*****************************************************************************
V_ARGLIST* DeleteVectorArgList(V_ARGLIST *Vector)
{if (Vector != NULL)
 {//�������� ��������� ������� - �������� ARGLIST
  if (Vector->Vector != NULL)
   for (ulong i = 0; i < Vector->Size; i++)
    Vector->Vector[i] = DeleteArgList(Vector->Vector[i]);
  //�������� ������ �������
  delete[] Vector->Vector; Vector->Vector = NULL; Vector->Size = 0L;
  //�������� ��������� �� ��������� V_ARGLIST
  delete Vector; Vector = NULL;
 }
return Vector;
}

//*****************************************************************************
//������: III.6
//������������ �������: ARGLIST* InitFactors(const MEASURE& M);
//����������:
//������������� ������� �������� � ��������� ������������� ���������� �������.
//������� ������������� ��� �������� ������ ���������� ARGLIST, ��� ����������-
//��� � ����� ���������� ������ � ��� ���������� �������. �������������� ����-
//����� � ���������� (�������������) ���������� ������� ������������ ����������
//MEASURE. ������������� ���������� �� ������� ��������� - Arg->List[1], �.�.
// Arg->List[0] �������� �������� �������, ������� ����� ��� ����� ����������
//��� ����������� � ��������� ��� ������� � ����������� ������� �� ��������
//��������������. ��� ������� ��������� ������� Arg->List[0] �������� ��������-
//��� �������� ��������� ��������.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. const MEASURE& M  - �������������� ���������� � ��������� �������
//-----------------------------------------------------------------------------
//������������ ��������: ARGLIST* Arg - ������ ���������� ��� ������ �������
//*****************************************************************************
ARGLIST* InitFactors(const MEASURE& M)
{ ARGLIST* Arg = CreateArgList(M.Size);

  //������ �������� �������, ��������� ����������� (F(t)=0), ��������� �������
  //��� ���������� - RandLaw(),Rand2PILaw(),RelayLaw(), etc.
  if (M.Size == 1) return Arg;

 //��������� ������������� ���� ���������� �������
 for (ulong i = 1; i < M.Size; i++)
  Arg->List[i] = GetDoubleValue(Arg->List[i],M.Factor[i]);

return Arg;
}

//*****************************************************************************
//������: III.7
//������������ �������: double GetDoubleValue(double value, const FACTOR& F)
//����������:
//������������� ������� ������������� ��� ����� �������� ���� double. �������
//������������ ����- � ������������ ����������� ����� � ������ �� ��������������
//����������, ����������� �� ������������� �������� �������� �� ����� �����������
//� ������ (��������, �� DEG � RAD). �������������� �� ������������� ����������
//���������� ����� ��������� ���� FACTOR.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. double value - �������� ������������� ����������
//2. const FACTOR* F - ��������� �� ��������� � ���������������� �������������
//   �������� ���� double
//-----------------------------------------------------------------------------
//������������ ��������:
//double value - ����������������� �������� ���� double
//*****************************************************************************
double GetDoubleValue(double value, const FACTOR& F)
{ uint Y = wherey();
  double convert;
  char c;
  bool lBreak;

  //������������ ����������� �������������� ����������
  if (!(F.PreBlock)) //������ �� ��������������
   return value;     //����� �� ��������������
 do
 { do
    { cout << "Input " << F.Name << " [" << GetMeasureName(F.SourceDim)
           << "] : ";
      cin >> value;
      gotoxy(1,Y); clreol();
      //������������� ����� �
      cout << "< " << F.Name << " = " << value << " ["
           << GetMeasureName(F.SourceDim) << "]";
      //�������������� �������� ����������� SourceDim � �������� TargetDim
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
  } while(!(F.PostBlock)); //������������ �������� ��������

 //�������������� �������� ����������� SourceDim � �������� TargetDim
 if (F.SourceDim != F.TargetDim) value = F.Convert(value);

return value;
}

//*****************************************************************************
//������: III.8 ����- � ������������ ������������� ������� ���� double
//������������ �������: bool AlwaysTrue(double value);
//����������:
//������������� ������� ����- � ������������ ������������� �������� ���� double.
//������ ���������� true
//-----------------------------------------------------------------------------
//������������ ���������:
//1. double value - �������� �������������� ���������
//-----------------------------------------------------------------------------
//������������ ��������: bool - ��������� ��������
//*****************************************************************************
bool AlwaysTrue(double value,...)
{
 //��� �������������� �������������� ��� ���������� !!!
 if (value > 0.0) return true;
 else return true;
}
//*****************************************************************************
//������: III.9 ����- � ������������ ������������� ������� ���� double
//������������ �������: bool AlwaysFalse(double value);
//����������:
//������������� ������� ����- � ������������ ������������� �������� ���� double.
//������ ���������� false
//-----------------------------------------------------------------------------
//������������ ���������:
//1. double value - �������� �������������� ���������
//-----------------------------------------------------------------------------
//������������ ��������: bool - ��������� ��������
//*****************************************************************************
bool AlwaysFalse(double value)
{
 //��� �������������� �������������� ��� ���������� !!!
 if (value > 0.0) return false;
 else return false;
}

//*****************************************************************************
//������: III.10 �������������� �� ����� ����������� � ������
//������������ �������: double DegToRad(double value)
//����������:
//������� ������������� ��� �������� ����������� �������� �� ��������� ����
//� ���������.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. double value - �������� �������� � ��������
//-----------------------------------------------------------------------------
//������������ ��������:
//double value - �������� �������� � ��������
//*****************************************************************************
double DegToRad(double value)
{
 return value*DegRad;
}

//*****************************************************************************
//������: III.11 �������������� �� ����� ����������� � ������
//������������ �������: double RadToDeg(double value)
//����������:
//������� ������������� ��� �������� ����������� �������� �� ��������� ����
//� ���������.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. double value - �������� �������� � ��������
//-----------------------------------------------------------------------------
//������������ ��������:
//double value - �������� �������� � ��������
//*****************************************************************************
double RadToDeg(double value)
{
 return value*RadDeg;
}
//*****************************************************************************
//������: III.12 �������������� �� ����� ����������� � ������
//������������ �������: double FreqToPulsatance(double value)
//����������:
//������� ������������� ��� �������� �������� ������� � ������� �������.
//(Hz -> RAD/SEC)
//-----------------------------------------------------------------------------
//������������ ���������:
//1. double value - �������� ������� � ������ (�������� �������)
//-----------------------------------------------------------------------------
//������������ ��������:
//double value - �������� ������� � RAD/SEC (������� �������)
//*****************************************************************************
double FreqToPulsatance(double value)
{
 return value*TwoPI;
}
//*****************************************************************************
//������: III.13 �������������� �� ����� ����������� � ������
//������������ �������: double PulsatanceToFreq(double value)
//����������:
//������� ������������� ��� �������� ������� ������� � �������� �������.
//(RAD/SEC -> Hz)
//-----------------------------------------------------------------------------
//������������ ���������:
//1. double value - �������� ������� � RAD/SEC (������� �������)
//-----------------------------------------------------------------------------
//������������ ��������:
//double value - �������� ������� � ������ (�������� �������)
//*****************************************************************************
double PulsatanceToFreq(double value)
{
 return value/TwoPI;
}
//*****************************************************************************
//������: III.14 �������������� �� ����� ����������� � ������
//������������ �������: double Copy(double value)
//����������:
//������� ���������� �������� ����� �� �����������, ��� � ���������� ��������
//-----------------------------------------------------------------------------
//������������ ���������:
//1. double value - �������� ��������
//-----------------------------------------------------------------------------
//������������ ��������:
//double value - �������� ��������
//*****************************************************************************
double Copy(double value)
{
 return value;
}


//**************************************************************************
// IV. ���������� �������, ����������� �������������� ������ ������ ���������
//     �������� �� ������� � ����������� �����������
//**************************************************************************
//*****************************************************************************
//������: IV.1
//������������ �������: char* GetMathNotateZeroLaw(const ARGLIST *Arg)
//����������:
//������� ��������� �������������� ������ ����� ��������� �������� �� �������
//F(t) = 0 � ���� ������.
//��������� ������ Arg ��� ���������� ������ �������
//���������� ����������: 1
//1. Arg.List[0] - ������� (���������) ����� t
//-----------------------------------------------------------------------------
//������������ ���������:
//1. ARGLIST *Arg - ������ ���������� ������ ZeroLaw()
//-----------------------------------------------------------------------------
//������������ ��������:
// char *s - ����� ��������� ZeroLaw � ���� ���������� ������
//*****************************************************************************
char* GetMathNotateZeroLaw(const ARGLIST *Arg)
{
 char* s = new char[80];//������ � �������������� ������� ������
 ostrstream text(s, 80);
 try
  {
  	if (Arg == NULL)  //������ ���� ������ ����������
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
//������: IV.2
//������������ �������: char* GetMathNotateConstLaw(const ARGLIST *Arg)
//����������:
//������� ��������� �������������� ������ ����� ��������� �������� �� �������
//F(t) = A � ���� ������.
//��������� C����� Arg ��� ���������� ������ �������
//���������� ����������: 2
//1. Arg.List[0] - ������� (���������) ����� t (�������� ������� F(t) - t)
//2. Arg.List[1] - const (�������� ���� double). F(t) = 5.0 => arg[1] = 5.0
//-----------------------------------------------------------------------------
//������������ ���������:
//1. ARGLIST *Arg - ������ ���������� ������ ��������� ConstLaw
//-----------------------------------------------------------------------------
//������������ ��������:
// char *s - ����� ��������� ConstLaw � ���� ���������� ������
//*****************************************************************************
char* GetMathNotateConstLaw(const ARGLIST* Arg)
{
 char* s = new char[80];//������ � �������������� ������� ������
 ostrstream text(s, 80);
 try
  {
  	if (Arg == NULL)  //��������� �� ��������
    throw ARG_MISMATCH;
   if (Arg->Size != 2) //������ ���� �������� ����� ��� ���������
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
//������: IV.3
//������������ �������: char* GetMathNotateLinearLaw(const ARGLIST* Arg)
//����������:
//������� ��������� �������������� ������ ����� ��������� �������� �� �������
//F(t) = A*t + B � ���� ������.
//��������� ������ Arg ��� ���������� ������ �������
//���������� ����������: 3
//1. Arg.List[0] - t - ������� ����� (��������� �����)
//2. Arg.List[1] - A
//3. Arg.List[2] - B
//-----------------------------------------------------------------------------
//������������ ���������:
//1. ARGLIST *Arg - ������ ���������� ������ ��������� LinearLaw
//-----------------------------------------------------------------------------
//������������ ��������:
// char *s - ����� ��������� LinearLaw � ���� ���������� ������
//*****************************************************************************
char* GetMathNotateLinearLaw(const ARGLIST* Arg)
{
 char* s = new char[80];//������ � �������������� ������� ������
 ostrstream text(s, 80);
 try
  {
  	if (Arg == NULL)  //��������� �� ��������
    throw ARG_MISMATCH;
   if (Arg->Size != 3) //������ ���� �������� ����� ��� ���������
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
//������: IV.4
//������������ �������: char* GetMathNotateSquareLaw(const ARGLIST* Arg)
//����������:
//������� ��������� �������������� ������ ����� ��������� �������� �� �������
//F(t) = A*t^2 + B*t + C � ���� ������.
//��������� ������� Arg ��� ���������� ������ �������
//���������� ����������: 4
//1. Arg.List[0] - double t - ������� ����� (��������� �����)
//2. Arg.List[1] - double A
//3. Arg.List[2] - double B
//4. Arg.List[3] - double C
//-----------------------------------------------------------------------------
//������������ ���������:
//1. ARGLIST* Arg - ������ ���������� ������ ��������� SquareLaw
//-----------------------------------------------------------------------------
//������������ ��������:
// char *s - ����� ��������� SquareLaw � ���� ���������� ������
//*****************************************************************************
char* GetMathNotateSquareLaw(const ARGLIST* Arg)
{
 char* s = new char[80];//������ � �������������� ������� ������
 ostrstream text(s, 80);
 try
  {
  	if (Arg == NULL)  //��������� �� ��������
    throw ARG_MISMATCH;
   if (Arg->Size != 4) //������ ���� �������� ����� 4 ���������
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
//������: IV.5
//������������ �������: char* GetMathNotateExpLaw(const ARGLIST *Arg)
//����������:
//������� ��������� �������������� ������ ����� ��������� �������� �� �������
//F(t) = A*EXP(B*t) � ���� ������.
//��������� ������ Arg ��� ���������� ������ �������
//���������� ����������: 3
//1. Arg.List[0] - double t - ������� ����� (��������� �����)
//2. Arg.List[1] - double A
//3. Arg.List[2] - double B
//-----------------------------------------------------------------------------
//������������ ���������:
//1. ARGLIST *Arg - ������ ���������� ������ ��������� ExpLaw
//-----------------------------------------------------------------------------
//������������ ��������:
// char *s - ����� ��������� ExpLaw � ���� ���������� ������
//*****************************************************************************
char* GetMathNotateExpLaw(const ARGLIST *Arg)
{
 char* s = new char[80];//������ � �������������� ������� ������
 ostrstream text(s, 80);
 try
  {
  	if (Arg == NULL)  //��������� �� ��������
    throw ARG_MISMATCH;
   if (Arg->Size != 3) //������ ���� �������� ����� 3 ���������
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
//������: IV.6
//������������ �������: char* GetMathNotateCosLaw(const ARGLIST *Arg)
//����������:
//������� ��������� �������������� ������ ����� ��������� �������� �� �������
//F(t) = A*COS(B*t+C) � ���� ������.
//��������� ������ Arg ��� ���������� ������ �������
//���������� ����������: 4
//1. Arg.List[0] - double t - ������� ����� (��������� �����)
//2. Arg.List[1] - double A - ���������
//3. Arg.List[2] - double B - ������� ������� B = 2*PI*f
//4. Arg.List[3] - double C - ��������� ����
//-----------------------------------------------------------------------------
//������������ ���������:
//1. ARGLIST *Arg - ������ ���������� ������ ��������� CosLaw
//-----------------------------------------------------------------------------
//������������ ��������:
// char *s - ����� ��������� CosLaw � ���� ���������� ������
//*****************************************************************************
char* GetMathNotateCosLaw(const ARGLIST *Arg)
{
 char* s = new char[80];//������ � �������������� ������� ������
 ostrstream text(s, 80);
 try
  {
  	if (Arg == NULL)  //��������� �� ��������
    throw ARG_MISMATCH;
   if (Arg->Size != 4) //������ ���� �������� ����� 4 ���������
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
//������: IV.7
//������������ �������: char* GetMathNotateSinLaw(const ARGLIST *Arg)
//����������:
//������� ��������� �������������� ������ ����� ��������� �������� �� �������
//F(t) = A*SIN(B*t+C) � ���� ������.
//��������� ������ *Arg ��� ���������� ������ �������
//���������� ����������: 4
//1. Arg.List[0] - double t - ������� ����� (��������� �����)
//2. Arg.List[1] - double A - ���������
//3. Arg.List[2] - double B - ������� ������� B = 2*PI*f
//4. Arg.List[3] - double C - ��������� ����
//-----------------------------------------------------------------------------
//������������ ���������:
//1. ARGLIST* Arg - ������ ���������� ������ ��������� SinLaw
//-----------------------------------------------------------------------------
//������������ ��������:
// char *s - ����� ��������� SinLaw � ���� ���������� ������
//*****************************************************************************
char* GetMathNotateSinLaw(const ARGLIST* Arg)
{
 char* s = new char[80];//������ � �������������� ������� ������
 ostrstream text(s, 80);
 try
  {
  	if (Arg == NULL)  //��������� �� ��������
    throw ARG_MISMATCH;
   if (Arg->Size != 4) //������ ���� �������� ����� 4 ���������
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
//������: IV.8
//������������ �������: char* GetMathNotateHyperbolLaw(const ARGLIST *Arg)
//����������:
//������� ��������� �������������� ������ ����� ��������� �������� �� �������
//F(t) = A/t � ���� ������.
//��������� ������ *Arg ��� ���������� ������ �������
//���������� ����������: 2
//1. Arg.List[0] - double t - ������� ����� (��������� �����)
//2. Arg.List[1] - double A - ���������
//-----------------------------------------------------------------------------
//������������ ���������:
//1. ARGLIST* Arg - ������ ���������� ������ ��������� HyperbolLaw
//-----------------------------------------------------------------------------
//������������ ��������:
// char *s - ����� ��������� HyperbolLaw � ���� ���������� ������
//*****************************************************************************
char* GetMathNotateHyperbolLaw(const ARGLIST* Arg)
{
 char* s = new char[80];//������ � �������������� ������� ������
 ostrstream text(s, 80);
 try
  {
   if (Arg->Size != 2) //������ ���� �������� ����� 2 ���������
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
//������: IV.9
//������������ �������: char* GetMathNotateLnLaw(const ARGLIST *Arg)
//����������:
//������� ��������� �������������� ������ ����� ��������� �������� �� �������
//F(t) = LN(t) � ���� ������.
//��������� ������ *Arg ��� ���������� ������ �������
//���������� ����������: 1
//-----------------------------------------------------------------------------
//������������ ���������:
//1. ARGLIST* Arg - ������ ���������� ������ ��������� LnLaw
//-----------------------------------------------------------------------------
//������������ ��������:
// char *s - ����� ��������� LnLaw � ���� ���������� ������
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
//������: IV.10
//������������ �������: char* GetMathNotateLog10Law(const ARGLIST *Arg)
//����������:
//������� ��������� �������������� ������ ����� ��������� �������� �� �������
//F(t) = LOG10(t) � ���� ������.
//��������� ������ *Arg ��� ���������� ������ �������
//���������� ����������: 1
//-----------------------------------------------------------------------------
//������������ ���������:
//1. ARGLIST* Arg - ������ ���������� ������ ��������� Log10Law
//-----------------------------------------------------------------------------
//������������ ��������:
// char *s - ����� ��������� Log10Law � ���� ���������� ������
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
//������: IV.11
//������������ �������: char* GetMathNotateLogALaw(const ARGLIST *Arg)
//����������:
//������� ��������� �������������� ������ ����� ��������� �������� �� �������
//F(t) = LogA(t) � ���� ������.
//��������� ������ *Arg ��� ���������� ������ �������
//���������� ����������: 2
//1. Arg.List[0] - double t - ������� ����� (��������� �����)
//2. Arg.List[1] - double A - ��������� ���������
//-----------------------------------------------------------------------------
//������������ ���������:
//1. ARGLIST* Arg - ������ ���������� ������ ��������� LogALaw
//-----------------------------------------------------------------------------
//������������ ��������:
// char *s - ����� ��������� LogALaw � ���� ���������� ������
//*****************************************************************************
char* GetMathNotateLogALaw(const ARGLIST* Arg)
{
 char* s = new char[80];//������ � �������������� ������� ������
 ostrstream text(s, 80);
 try
  {
   if (Arg->Size != 2) //������ ���� �������� ����� 2 ���������
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
//������: IV.12
//������������ �������: char* GetMathNotateSqrtLaw(const ARGLIST *Arg)
//����������:
//������� ��������� �������������� ������ ����� ��������� �������� �� �������
//F(t) = Sqrt(A*t+B) � ���� ������.
//��������� ������ *Arg ��� ���������� ������ �������
//���������� ����������: 3
//1. Arg.List[0] - double t - ������� ����� (��������� �����)
//2. Arg.List[1] - double A - ���������
//3. Arg.List[2] - double B - ���������
//-----------------------------------------------------------------------------
//������������ ���������:
//1. ARGLIST* Arg - ������ ���������� ������ ��������� SqrtLaw
//-----------------------------------------------------------------------------
//������������ ��������:
// char *s - ����� ��������� SqrtLaw � ���� ���������� ������
//*****************************************************************************
char* GetMathNotateSqrtLaw(const ARGLIST* Arg)
{
 char* s = new char[80];//������ � �������������� ������� ������
 ostrstream text(s, 80);
 try
  {
   if (Arg->Size != 3) //������ ���� �������� ����� 3 ���������
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
//������: IV.13
//������������ �������: char* GetMathNotateRandLaw(const ARGLIST *Arg)
//����������:
//������� ��������� ������ ��� ������������� ���������� ������:
//RandLaw
//��������� ������ *Arg ��� ���������� ������ �������
//���������� ����������: 1
//-----------------------------------------------------------------------------
//������������ ���������:
//1. ARGLIST* Arg - ������ ���������� ������ ��������� RandLaw
//-----------------------------------------------------------------------------
//������������ ��������:
// char *s - ����� ��������� RandLaw � ���� ���������� ������
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
//������: IV.14
//������������ �������: char* GetMathNotateRand2PiLaw(const ARGLIST *Arg)
//����������:
//������� ��������� ������ ��� ������������� ���������� ������:
//Rand2PiLaw
//��������� ������ *Arg ��� ���������� ������ �������
//���������� ����������: 1
//-----------------------------------------------------------------------------
//������������ ���������:
//1. ARGLIST* Arg - ������ ���������� ������ ��������� Rand2PiLaw
//-----------------------------------------------------------------------------
//������������ ��������:
// char *s - ����� ��������� Rand2PiLaw � ���� ���������� ������
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
//������: IV.15
//������������ �������: char* GetMathNotateRandABLaw(const ARGLIST *Arg)
//����������:
//������� ��������� ������ � ����������� ��������� ���������� �������������� ��
//� ������������ ��������� [A..B]
//��������� ������ *Arg ��� ���������� ������ �������
//���������� ����������: 3
//1. Arg.List[0] - double x - �������� ��������� �������� � ������ ������� t
//1. Arg.List[1] - double A - ������ ������� ���������
//2. Arg.List[2] - double B - ������� ������� ���������
//-----------------------------------------------------------------------------
//������������ ���������:
//1. ARGLIST* Arg - ������ ���������� ������ ��������� RandABLaw
//-----------------------------------------------------------------------------
//������������ ��������:
// char *s - ��������� ��������� RandABLaw � ���� ���������� ������
//*****************************************************************************
char* GetMathNotateRandABLaw(const ARGLIST *Arg)
{
 char* s = new char[80];//������ � �������������� ������� ������
 ostrstream text(s, 80);
 double value;
 try
  {
  	if (Arg == NULL)  //��������� �� ��������
    throw ARG_MISMATCH;
   if (Arg->Size != 3) //������ ���� �������� ����� 3 ���������
    throw ARG_MISMATCH;
   //������������ A � B, ���� A > B
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
return s; //�������������� ������� � ��������� [A..B]
}
//*****************************************************************************
//������: IV.16
//������������ �������: char* GetMathNotateRelayLaw(const ARGLIST *Arg)
//����������:
//������� ��������� ������ ��� ������������� ���������� ������:
//RelayLaw
//��������� ������ *Arg ��� ���������� ������ �������
//���������� ����������: 1
//-----------------------------------------------------------------------------
//������������ ���������:
//1. ARGLIST* Arg - ������ ���������� ������ ��������� RelayLaw
//-----------------------------------------------------------------------------
//������������ ��������:
// char *s - ����� ��������� RelayLaw � ���� ���������� ������
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
//������: IV.17
//������������ �������: char* GetMathNotateDet1Law(const ARGLIST *Arg)
//����������:
//������� ��������� ������ ��� ������������� ������, ������������ ������� ��,
//������� ������������� � ������� ����������.
//��������� ������ *Arg ��� ���������� ������ �������
//���������� ����������: 1
//-----------------------------------------------------------------------------
//������������ ���������:
//1. ARGLIST* Arg - ������ ���������� ������ ��������� RandLaw
//-----------------------------------------------------------------------------
//������������ ��������:
// char *s - ����� ��������� Det1Law � ���� ���������� ������
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
//������: IV.18
//������������ �������: char* GetMathNotateDet0Law(const ARGLIST *Arg)
//����������:
//������� ��������� ������ ��� ������������� ������, ������������ ������� ��,
//������� ������������ � ������� ����������.
//��������� ������ *Arg ��� ���������� ������ �������
//���������� ����������: 1
//-----------------------------------------------------------------------------
//������������ ���������:
//1. ARGLIST* Arg - ������ ���������� ������ ��������� RandLaw
//-----------------------------------------------------------------------------
//������������ ��������:
// char *s - ����� ��������� Det0Law � ���� ���������� ������
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
//������: IV.19
//������������ �������: char* GetMathNotateTableLaw(const ARGLIST *Arg)
//����������:
//������� ��������� ������ ���������� ����� ��������� �������� �� �������
//����� ������� ������ ��� ������� ������� ������� (�������� ���������)
//������������ �������� �������. ������ ���������� ���������� �������� �������.
//��������� ������ *Arg ��� ���������� ������ �������
//���������� ����������: from 1 to N
//-----------------------------------------------------------------------------
//������������ ���������:
//1. ARGLIST* Arg - ������ ���������� ������ ��������� TableLaw
//-----------------------------------------------------------------------------
//������������ ��������:
// char *s - ����� ��������� TableLaw � ���� ���������� ������
//*****************************************************************************
char* GetMathNotateTableLaw(const ARGLIST *Arg)
{
 char* s = new char[45];
 ostrstream text(s, 45);
 text << "TABLE LAW: [1..N], N = " << Arg->Size << ends;
 return s;
}
//*****************************************************************************
//������: IV.20
//������������ �������: char* GetMathNotateSquarePulseLaw(const ARGLIST *Arg)
//����������:
//������� ��������� ������ ������ ��������� �������������� �������� �� �������
//������� ��� ������� ������� ������� (�������� ���������) ���������� ���������-
//��� �������� ������ ������� - Umax ��� Umin. ������ ���������� ��������� ���-
//����������� ��������.
//��������� ������ *Arg ��� ���������� ������ �������
//���������� ����������: 8.
//1. Arg.List[0] - T - ������� ����� (��������� �����)
//2. Arg.List[1] - t0 - ������ ���������� ��������� � ������� ������� ��������-
//   ����� - ��������� (t0 >= 0)
//3. Arg.List[2] - timp - ������������ �������� - ��������� (timp > 0)
//4. Arg.List[3] - Timp - ������ ���������� ��������� - ��������� (Timp > timp)
//5. Arg.List[4] - Umax - ������� ������� ��� ���������� �������� - ���������
//6. Arg.List[5] - Umin - ������� ������� ��� ���������� �������� - ���������
//���������� ���������
//7. Arg.List[6] - flag - ��������� ����������� ������� - ����������
//   ��������� ��������: -1.0 - ������� �����������; 0.0 - ��������� ��������
//   ��������� ������� �������� � ������� �������������; +1.0 - ������� ����.
//8. Arg.List[7] - count - ������� ������� - ����������. ������� ������� ������-
//   ���� ������� ������� �� �������� 1) ��������� ��������, 2) ���������� ��-
//   ������, 3) ��������� ������� ��������. ������� ������� ���������� ���������
//   �� ������� (-1.0) ������ ��� ��� ��������� � �������.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. ARGLIST* Arg - ������ ���������� ������ ��������� SquarePulseLaw
//-----------------------------------------------------------------------------
//������������ ��������:
// char *s - ����� ��������� SquarePulseLaw � ���� ���������� ������
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
// V. �������� �������, ���������� � ��������� LawName � MeasureName
//******************************************************************
//*****************************************************************************
//������: V.1
//������������ �������: const char* GetLawName(uint type, uint language = ENG)
//����������:
//������� ���������� ������������ ������ �� ������� LawName �� ��� ���� type.
//�������� ��������� ������������ �� ������� � ���������� ������ � �����������
//�� �������� ���������� language
//-----------------------------------------------------------------------------
//������������ ���������:
//1. uint type - ������������� ������ ��������� ��������
//2. uint language - ���� ��������� ������������ ������ (ENG - �� ���������)
//-----------------------------------------------------------------------------
//������������ ��������:
//const char* ptr - ������������ ������ �� ��� ����
//*****************************************************************************
const char* GetLawName(uint type, uint language)
{ uint Count;
  const char* ptr;
  if (language != RUS) ptr = LawName[UNEXIST].eng_name;
  else ptr = LawName[UNEXIST].rus_name;
  //���������� ������ ������� LawName
  Count = sizeof(LawName)/sizeof(LawName[0]);
  //����� ������������ ������, ���������������� type
  for (uint i = 1; i < Count; i++)
  { if (LawName[i].type == type) //������ �������
    {
     if (language != RUS) ptr = LawName[i].eng_name;
     else ptr = LawName[i].rus_name;
     break; //��������������� ����� �� �����
    }
  }
return ptr;
}
//*****************************************************************************
//������: V.2
//������������ �������: const char* GetLawNotate(uint type)
//����������:
//�������  ���������� �������������� ������ ������ �� ������� LawName �� ���
//���� type.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. uint type - ������������� ������ ��������� ��������
//-----------------------------------------------------------------------------
//������������ ��������:
//const char* ptr - �������������� ������ ������
//*****************************************************************************
const char* GetLawNotate(uint type)
{ uint Count;
  const char* ptr;
  ptr = LawName[UNEXIST].math_notation;
  //���������� ������ ������� LawName
  Count = sizeof(LawName)/sizeof(LawName[0]);
  //����� ������������ ������, ���������������� type
  for (uint i = 1; i < Count; i++)
  { if (LawName[i].type == type) //������ �������
    {ptr = LawName[i].math_notation;
     break; //��������������� ����� �� �����
    }
  }
return ptr;
}
//*****************************************************************************
//������: V.3
//������������ �������:
//const char* GetMeasureName(uint type, uint language = ENG, bool format = SHORT)
//����������:
//������� ���������� ������������ ����������� ��������  ��� ��� ������������ ��
//������� MeasureName �� ��� ���� type.
//�������� ��������� ������������ �� ������� � ���������� ������ � �����������
//�� �������� ���������� language, � ����� ������ ��� ����������� ������������
//����������� �������� � ����������� �� �������� format: SHORT - ������������;
//LONG - ������ ������������
//-----------------------------------------------------------------------------
//������������ ���������:
//1. uint type - ������������� ����������� ��������
//2. uint language - ���� ��������� ������������ ������ (ENG - �� ���������)
//3. bool format - ����� ������ ����������� (������ ��� �����������) (SHORT - ��
//���������).
//-----------------------------------------------------------------------------
//������������ ��������:
//const char* ptr - ������������ ����������� �������� �� �� ����
//*****************************************************************************
const char* GetMeasureName(uint type, uint language, bool format)
{ uint Count;
  const char* ptr;
  if (language != RUS) ptr = MeasureName[NONDIM].EngShort;
  else ptr = MeasureName[NONDIM].RusShort;
  //���������� ������ ������� MeasureName
  Count = sizeof(MeasureName)/sizeof(MeasureName[0]);
  //����� ������������ ������, ���������������� type
  for (uint i = 1; i < Count; i++)
  { if (MeasureName[i].Type == type) //������ �������
    {
     if (language != RUS)
      if (format != LONGNAME) ptr = MeasureName[i].EngShort;
      else ptr = MeasureName[i].Eng;
     else
      if (format != LONGNAME) ptr = MeasureName[i].RusShort;
      else ptr = MeasureName[i].Rus;
     break; //��������������� ����� �� �����
    }
  }
return ptr;
}
//*****************************************************************************
//������: V.4
//������������ �������: uint GetTypeLawByAbbrLaw(string abbr, bool detlaw)
//����������:
//������� ����������  ������������� ������ �� ��� ������������ �� ������� �����-
//������� ������� LawName. ����� ������� �� ���� abbreviate. ���� ������������
//������ �� ������� ������������ type == UNEXIST. ����� �������������� ���� ���-
//�� ����������������� �������, ���� ����� ������������� ������� � �����������
//�� �������� ����� detlaw.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. string abbr - ������������ ������������ ������
//2. bool detlaw - ���� ������������ ����������������� ��� ������������� �����
//-----------------------------------------------------------------------------
//������������ ��������:
//uint type - ������������� ������
//*****************************************************************************
uint GetTypeLawByAbbrLaw(string& abbr, bool detlaw)
{ uint type = UNEXIST;
  uint Count = sizeof(LawName)/sizeof(LawName[0]);
  //����� �������������� ������ �� ��� ������������
  for (uint i = 1; i < Count; i++)
  {
   if (LawName[i].detlaw == detlaw)
    if (abbr.compare(LawName[i].abbreviate) == 0) //������������ �������
    { type = LawName[i].type;
      break; //����� �� ����� ������
    }
  }
return type;
}
//*****************************************************************************
//������: V.5
//������������ �������: string& GetAbbrLaw(string& s, bool detlaw)
//����������:
//������� ��������� ������ string s c �������������� ������� ���������� � ������-
//�������, ������� �������� � ������� LawName. ������ ������������ ���������� ��
//������ �������� ';'. � ���� ������ �������������� ������������ ������ �����-
//������ ���� �����������������, ���� ������������� ������� � ����������� ��
//�������� detlaw.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. string& s  - ������ � ������� ����� ����������� ������ ����������� �������
//2. bool detlaw - ���� ������������ ��������� � ������ ������������������ ���
//                 �������������� ������
//-----------------------------------------------------------------------------
//������������ ��������:
//string& s  - ������ ����������� �������
//*****************************************************************************
string& GetAbbrLaw(string& s, bool detlaw)
{
  uint Count = sizeof(LawName)/sizeof(LawName[0]);
  uint pos;
  s.resize(0); //�������� ����������� ������
  //��������� ������ ����������� ������� � ������ ������ detlaw
  for (uint i = 1; i < Count; i++)
   if (LawName[i].detlaw == detlaw)
   { s.append(LawName[i].abbreviate);
     s.append("\t\t");
     s.append(LawName[i].math_notation);
     s.append(";");
   }
   pos = s.find_last_of(";");
   if (pos == string::npos) s = "LIST OF ABBREVIATE LAWS IS EMPTY!";
   else s = s.substr(0,pos); //�������� ���������� ';'
return s;
}
//*****************************************************************************
//������: V.6
//������������ �������: const char* GetAbbrLawByType(uint type)
//����������:
//������� ���������� ������������ ������ �� �� ����, ����������� ����� � �������
//������������ ������� LawName
//-----------------------------------------------------------------------------
//������������ ���������:
//1. uint type - ������������� ������ �� �������� �������������� �����
//-----------------------------------------------------------------------------
//������������ ��������:
// s  - ������������ ������
//*****************************************************************************
const char* GetAbbrLawByType(uint type)
{ const char* s = LawName[0].abbreviate;
  uint Count = sizeof(LawName)/sizeof(LawName[0]);

  for (uint i = 1; i < Count; i++)
   //������������� ������ ������
   if (LawName[i].type == type) {s = LawName[i].abbreviate; break; }

return s;
}

