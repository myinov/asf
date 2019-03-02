//mathfunc.h
//���������� �������������� �������, ����������� ������ ��������� �� �������
//����������� � ������� ������������ ��������.
//Copyright (c) JohnSoft 1998. All rights reserved.
//Initial date: August,22,1998. Final date:
//������������ ���� mathfunc.h ��������� ��������� �������������� �������, ����-
//��� ��������� ��������� ������ ��������� ����������� � ������� ������������
//�������� �� �������. ��� ������� ���������� �������� ���� double, �������
//����� ���������� ��� �������� ���� ��� ��������� ������� � ���������� ������
//������� t. ��� ������� � �������� ���������� ��������� ������ ���������� ����
//double. ��� ���� ������� � ������ �������� ������� ���������� �������� �������
//F(t) - t (�����). ���������� ������������ ���������� (����������� �������) � ��
//������������� ������� �� ���� �������, ������� ��������� �������� ���������
//���� ��� ��������� ������� �� �������, � ����� ���������� � ����� ������� ���-
//�����.


#if !defined(__MATHFUNC_H)
#define __MATHFUNC_H

#include <except.h>   //������������ ���� �� ��������� �������������� ��������

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
//#include <complex.h>
#include "matrix.h"  //������������ ���� ������ � ���������
#include "mvector.h" //������������ ���� ������ � ���������
#include "mathlib.h" //������������ ����� � ����������� ��������� �����
#include "const.h"

//#include <cstring.h>
#include <string>
#include <strstrea.h>

#include <iomanip.h>
#include <iostream.h>
#include <fstream.h>

#define CONSOLE 1     //� ��������� ������������ ���������� ����-�����

#if defined(CONSOLE)
 #include <conio.h>
#endif

typedef unsigned int  uint;
typedef unsigned long ulong;

typedef double (*DDFunc)(double);
typedef bool (*BDVAFunc)(double,...);

//const int RUS = 1; //������� ����
//const int ENG = 2; //���������� ����

const bool SHORTNAME = true;  //����������� ����� ������
const bool LONGNAME  = false; //������ ����� ������

const bool DETLAW    = true;  //����������������� �����
const bool CASUALLAW = false; //������������� �����

const bool TDIM = true;  //�������� ����������� ��������� ������
const bool SDIM = false; //�������� ����������� ��������� ������

//const bool ON  = true;
//const bool OFF = false;

//**************************************************************************
//���������� ��������� LAWNAME
//��������� �������� ������������ ������ �� ������� � ���������� ������,
//�������������� ����� ������ ������.
typedef struct
{
  uint type;      //������������� ������
  bool detlaw;    //���� ������������ ����������������� ��� ������������� �����
  char *rus_name; //������������� ������������ ������
  char *eng_name; //������������ ������������ ������
  char *math_notation; //�������������� ����� ������ ������
  char *abbreviate; //����������� ������������ ������ �� ���������� �����
} LAWNAME;

//**************************************************************************
//���������� ��������� FACTOR
//��������� FACTOR ������������� ����������� ��������� � ���������� �������,
//�� ���������� ������������, ������� �������� �������� ���������� �������
//��� ������������� ��� ����������� ������ � ����, ������������ �����
//**************************************************************************
typedef struct
{
  uint SourceDim; //����������� �������� �������� ��������� (���������) �������
  uint TargetDim; //����������� �������� �������� ��������� (���������) �������
  char* Name;     //������������ ��������� (���������)
  //������� �������� �� SourceDim � TargetDim
  double (*Convert)(double value);
  //������� �������� �� TargetDim � SourceDim
  double (*Invert)(double value);
  //������� ����������� (����������� ����� � �������������� ���������)
  bool (*PreBlock)(double value,...);
  //������� ����������� (����������� ������ �� �������������� ���������)
  bool (*PostBlock)(double value,...);
} FACTOR;

//**************************************************************************
//���������� ��������� MEASURE
//��������� MEASURE ������������� ����������� �������, �� ��������� �
//�������� ���������� (�������������) ������� (DIMENSIONALITY)
//**********************************************************************
typedef struct
{
 uint DimFunc; //����������� �������
 uint Size; //���������� ���������� � ���������� ��������������� ������ �-���
 FACTOR* Factor; //������ ������������� �� ��������� � �������� �������
} MEASURE;

//*****************************************************************************
//���������� ��������� MEASURENAME
//��������� �������� ������ � ����������� ������������ ����������� �������� ��
//������� � ���������� ������
//*****************************************************************************
typedef struct
{
  uint  Type;    //��� ���������� ��������
  char* Rus;     //������ ������������ �� ������� �����
  char* RusShort;//����������� ������������ �� ������� �����
  char* Eng;     //������ ������������ �� ���������� �����
  char* EngShort;//����������� ������������ �� ���������� �����
} MEASURENAME;

//���������� ��������� ��������������� ������������ ���������� �������
const uint NONDIM   = 0; //������������ ��������
const uint SECOND   = 1; //�������
const uint RADIAN   = 2; //��������� ����
const uint DEGREE   = 3; //��������� ����
const uint VOLT     = 4; //���������� � �������
const uint HZ       = 5; //������� � ������
const uint PULSAT   = 6; //������� �������
const uint DEG_SEC  = 7; //�������� � ����/���
const uint DEG_SEC2 = 8; //��������� � ����/���^2
const uint RAD_SEC  = 9; //�������� � ���/���
const uint RAD_SEC2 = 10;//��������� � ���/���^2

//****************************************************************************
// ���������� ��������� ArgList - ������ ���������� ������� ���������� �����
// ��������� ������ ��� ������������� ����������� ���������� ����������
// ���� double
//****************************************************************************
typedef struct
{
 ulong Size; //���������� ���������� � ������
 double *List; //������ ����������
} ARGLIST;

//����������� ��������� �� ������ ����������
typedef ARGLIST* PARGLIST;

//****************************************************************************
// ���������� ��������� V_ArgList - ������ ������ ���������� ������� ����������
// �����
// ��������� ������ ��� ������������� ������� (����������� �������) ������, � ��-
// ����� �������� �������� ���� double. ����� ��������� ����� ������� ���������
// ���������� ������� � ��������� ����������� ����������. ������ ���������� ���
// ������ ���������� �� ��������� ARGLIST - ARGLIST**, ��� ��������� ������������
// ������� ��� ������ �� ���������� ARGLIST.
//****************************************************************************
typedef struct
{
 ulong Size; //����� �������
 ARGLIST** Vector; //������, ���������� ������ �������� ARGLIST
} V_ARGLIST;

//����������� ��������� �� ������ ������ �������� ARGLIST
typedef V_ARGLIST* pV_ARGLIST;


//*************************************************************************
//���������� ��������� ������ ��������� ������������ ��������� ���
//���� ���������� �������� �������
//*************************************************************************
typedef struct
{
  bool turn; //����������� ������ ��� ��� ������ �������������
  double (*law)(const ARGLIST *Arg);
  uint type; //������������� ������
  ARGLIST* (*inputfactor)(const MEASURE& M);
  //�������������� ����� ������ ������ c ��������� ��������������
  char* (*mathlawfactor)(const ARGLIST* Arg);
  MEASURE M; //�������������� ���������� � ���������� �������
} LAW;

//*****************************************************************************
// ���������� ��������� ��������, ����������� ��� ������ �������
//*****************************************************************************
//���������� ��������� TWO_PI = 2 * PI
#define TWO_PI 6.283185307178;
//���������� ��������� DEG_RAD = PI / 180 ��� �������� �� ��������� � ���������
//���� ����
#define DEG_RAD 0.01745329251994;
//���������� ��������� RAD_DEG = 180 / PI ��� �������� �� ��������� � ���������
//���� ����
#define RAD_DEG 57.2957795131;
//���������� ��������� EPSILON - �������� ������������ �������������� �����.
//������������ � ��� ������, ����� ������� � �������� ����� �� ����������
#define EPSILON 0.0000001

// ������ ��������� ������
const int ARG_MISMATCH = -1;//�������������� ���������� ���������� ����������
const int EMPTY_TABLE  = -2;//������� �� �������� �� ����� ������
const int ABSENT_LAW   = -3;//����� � ������� ������� �����������
const int IMPOS_VALUE  = -4;//������������ �������� ����������

// ���������� ��������, ������������ � ������ �������
//����������������� �������
const uint UNEXIST      =  0;//�� ������������������ �����
const uint ZERO_LAW     =  1;//������� �� ������� �����
const uint CONST_LAW    =  2;//���������� �� ������� �����
const uint LINEAR_LAW   =  3;//�������� �� ������� �����
const uint SQUARE_LAW   =  4;//������������ �� ������� �����
const uint EXP_LAW      =  5;//���������������� �� ������� �����
const uint COS_LAW      =  6;//���������������� �� ������� �����
const uint SIN_LAW      =  7;//C������������� �� ������� �����

const uint HYPERBOL_LAW =  8;//��������������� �����
const uint LN_LAW       =  9;//����� ������������ ���������
const uint LOG10_LAW    = 10;//����� ����������� ���������
const uint LOGA_LAW     = 11;//����� ��������� �� ��������� A
const uint SQRT_LAW     = 12;//����� ����������� ����� �� ��������� ��������
//��������� �������
const uint RAND_LAW     = 13;//����������� ������������� [0..1]
const uint RAND2PI_LAW  = 14;//����������� ������������� [0..2PI]
const uint RANDAB_LAW   = 15;//����������� ������������� [A..B]
const uint RELAY_LAW    = 16;//������������� �� �����
//������� ����������� ��������� ����������
const uint DET1_LAW  = 17; //����������������� ����� ���������
const uint DET0_LAW  = 18; //����������������� ����� ����
//��������� �������
const uint TABLE_LAW = 19; //���������������� �������
//������������� �������
const uint SQUAREPULSE_LAW = 20; //������� �������������� ��������
//*****************************************************************************
// ���������� �������, ����������� ��������� ������ ��������� �������
//*****************************************************************************
//I. ����������������� �������
//������� �� ������� �������  F(t) = 0;
double ZeroLaw(const ARGLIST* Arg); //1. ZERO_LAW

//���������� ����� �� �������. ������� ���� F(t) = const;
double ConstLaw(const ARGLIST* Arg); //2. CONST_LAW

//�������� ����� �� �������. ������� ���� F(t) = At + B;
double LinearLaw(const ARGLIST* Arg); //3. LINEAR_LAW

//������������ ����� �� �������. ������� ���� F(t) = A^2*t + B*t + C
double SquareLaw(const ARGLIST* Arg); //4. SQUARE_LAW

//���������������� ����� �� �������. ������� F(t) = A*EXP(B*t)
double ExpLaw(const ARGLIST* Arg);//5. EXP_LAW

//���������������� ����� �� �������. ������� F(t) = A*cos(B*t+C);
double CosLaw(const ARGLIST* Arg); //6. COS_LAW

//�������������� ����� �� �������. ������� F(t) = A*sin(B*t+C);
double SinLaw(const ARGLIST* Arg); //7. SIN_LAW

//��������������� �����, �������� ������������������. ������� F(t) = A/t;
double HyperbolLaw(const ARGLIST* Arg); //8. HYPERBOL_LAW

//����� ������������ ���������. ������� F(t) = LN(t);
double LnLaw(const ARGLIST* Arg); //9. LN_LAW

//����� ����������� ���������. ������� F(t) = LOG10(t);
double Log10Law(const ARGLIST* Arg); //10. LOG10_LAW

//����� ��������� �� ��������� A. ������� F(t) = LOGA(t);
double LogALaw(const ARGLIST* Arg); //11. LOGA_LAW

//����� ����������� ����� �� ��������� ��������. ������� F(t) = SQRT(A*t+B);
double SqrtLaw(const ARGLIST* Arg); //12. SQRT_LAW

//����� ��������� �������������� �������� �� �������.
double SquarePulseLaw(const ARGLIST* Arg); //20. SQUAREPULSE_LAW
//� �.�. ...

//II. ��������� ������
//������� ��������������� ��������� �������� � ��������� �� 0 �� 1.
double RandLaw(const ARGLIST* Arg);    //13. RAND_LAW

//�������������� ��������� ����������� ��� ������� ������������ �� ������� �
//��������� �� 0 �� 2*PI
double Rand2PiLaw(const ARGLIST* Arg); //14. RAND2PI_LAW

//������� ��������������� ��������� �������� � ��������� �� a �� b.
//a - ������ ������� ���������; b - ������� ������� ���������
double RandABLaw(const ARGLIST* Arg); //15. RANDAB_LAW

//������� ��������� �������� �� ���������� ������ �����
//sqrt(-2*ln(RandLaw())
double RelayLaw(const ARGLIST* Arg);  //16. RELAY_LAW

//������� ����������� ��������� ����������

//����������� ��� ������������ �������, ����� ������� ����� ���� ��������� ���-
//������� � �� ���������� ��������� �� ������������
double Det1Law(const ARGLIST* Arg);   //17. DET1_LAW

//����������� ��� �������� �������, ����� ������� ����� ���� ��������� ���-
//������� � �� ���������� ��������� �� ������������
double Det0Law(const ARGLIST* Arg);   //18. DET0_LAW

//***************************************************************************
// ���������� ������� ���������������� ������ ����������� ���������� ���
// ������ ������, ����������� ������ ��������� �������
// ��� ������� � ���������� ������ ���������� ��������� �� ��������� ARGLIST ��
// ������� �������������.
//***************************************************************************
ARGLIST* CreateArgList(const ulong size);
ARGLIST* DeleteArgList(ARGLIST *Arg);
ARGLIST* CopyArgList(const ARGLIST *Source, ARGLIST *Target);

V_ARGLIST* CreateVectorArgList(const ulong VectorSize, const ulong ListSize = 0);
V_ARGLIST* DeleteVectorArgList(V_ARGLIST *Vector);

//���������� ���� ���� ���������� ������� ���� double � ������ �������������
//(���������� �����) ���� ���������� �������
ARGLIST* InitFactors(const MEASURE& M);
//���� ������ ��������� ���� double
double GetDoubleValue(double value, const FACTOR& F);

//���������� ������� ����- � ������������ �������� (�������������) �������
//���� double
bool AlwaysTrue(double value,...);
bool AlwaysFalse(double value,...);

//���������� ������� �������������� ������ �� ����� ����������� � ������
//���� double
double Copy(double value);   //��� ���������
double DegToRad(double value); //�� �������� � �������
double RadToDeg(double value); //�� ������ � �������
double FreqToPulsatance(double value); //�� �������� � ������� �������
double PulsatanceToFreq(double value); //�� ������� � �������� �������


//******************************************************************************
// ���������� �������, ����������� �������������� ������ ������ ���������
// �������� �� ������� � ����������� �����������
// ��� ������� � ���������� ������ ���������� ��������� �� ������ � �����������-
// ��� ������� ������ ��������� �������� �� ������� � ��������� ��������������.
// ������� ��������� � �������� ��������� ������ ���������� ������ ���������
// ������ ��������.
//******************************************************************************
//1. ����������������� �������
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

//2. ��������� �������
char* GetMathNotateRandLaw(const ARGLIST *Arg);     //13. RAND_LAW
char* GetMathNotateRand2PiLaw(const ARGLIST *Arg);  //14. RAND2PI_LAW
char* GetMathNotateRandABLaw(const ARGLIST *Arg);   //15. RANDAB_LAW
char* GetMathNotateRelayLaw(const ARGLIST *Arg);    //16. RELAY_LAW
char* GetMathNotateDet1Law(const ARGLIST *Arg);     //17. DET1_LAW
char* GetMathNotateDet0Law(const ARGLIST *Arg);     //18. DET0_LAW
//3. ��������� �����
char* GetMathNotateTableLaw(const ARGLIST *Arg);    //19. TABLIC_LAW

//���������� �������
char* GetMathNotateSquarePulseLaw(const ARGLIST *Arg); //20. SQUAREPULSE_LAW

//******************************************************************
// ���������� �������, ���������� � ��������� LawName � MeasureName
//******************************************************************
const char* GetLawName(uint type, uint language = ENG);
const char* GetLawNotate(uint type);
const char* GetMeasureName(uint type, uint language = ENG,
                           bool format = SHORTNAME);
uint GetTypeLawByAbbrLaw(string& abbr, bool detlaw);
string& GetAbbrLaw(string& s, bool detlaw);
const char* GetAbbrLawByType(uint type);

#endif //���������� ���������� ������������� ����� "MATHFUNC.H"
