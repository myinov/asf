//linarray.h
//���������� ������ TLinearArray - �������� �������� ������� (LINEAR ARRAY)
//Initial date: October,1,1998. Final date:
//Copyright (c) JohnSoft 1998. All rights reserved. Borland C++ 5.01
//������������ ���� linarray.h ��������� ����� TLinearArray, ������� ���������
//�������� �������� �������� �������� ������� (linear antenna array). ������
//����� �������� ����������� ������ TElArray, ������� ��������� �������� ������
//�������� �������� �������. �������� �������� ������� ������������ �� ���� ��-
//���������� (������) ������, ������� ����������� � ���� �����.

#if !defined (__LINARRAY_H) //�������������� ���������� ���������� �����
#define __LINARRAY_H

#include "elarray.h" //���������� ������� �������� ��� ������ TLinearArray
                     //������ TElArray
#define CONSOLE 1    //� ��������� ������������ ���������� ����-����� �� �����

//���������� ���������, ���������� ������ �� �������, ������� �������� ��������-
//���� ��� ���������� ������ �� ��� �� ���������� �����.
typedef struct
{ const char** name; //������ ��������� ������������ �������
  uint sz_name; //���-�� ������������ ������� � ������� name
  uint type; //������������� ���� �������

  //�������������� �������������� ������� � E-���������
  uint E_total;//������ ������ ����������
  uint E_read;//���-�� ����������, ������� ���������� ������� �� �����
  const char** EParam; //������ ������������ �������� ���������� ��� ������� ELaw
  uint sz_EParam; //���-�� ������������ � EParam

  //�������������� �������������� ������� � H-���������
  uint H_total;//������ ������ ����������
  uint H_read;//���-�� ����������, ������� ���������� ������� �� �����
  const char** HParam; //������ ������������ �������� ���������� ��� ������� HLaw
  uint sz_HParam; //���-�� ������������ � HParam

} ANTENNA_INFO;

typedef const ANTENNA_INFO* pANT;//��������� �� ��������� ANTENNA_INFO

//���������� ���������, ���������� ���������� �� ������
//� ������ ARGLIST->List[0] - ������ �������� ������� �������� ������� law,
//����� ������� ��������� ������ (�������). �������� �������� ���������� �, ���
//�������, ��� ������ ���������� �� ����� �������������. ��������� � ����� ���-
//��� �������� ����������� ���������� �� ����� �������������, �� ����������� ��-
//��� ����������, ������� ����� ���������� �� �������, ��� ��������, ��������,
//����������� �������. ��� ������� �������� �� - LEFTDISTLAW � RIGHTDISTLAW �
//ARGLIST->List[0] ����� ��������� ���������� ����� [i+1,i] ���������� �������
//�� ������ �������� �� ������ law ����� ����� ��������� ���������� ����� [i+2,i+1]
//���������� ��. �������������� ���������� ����� �������� � ������ �� ���������
//�������� ��, ������������� ����� (������) �� ����, ������ ���� ������ � ������
//���������� ������ � ����� ������ ���� ���������� �������� ����������.
typedef struct
{ const char** pattern; //������ ��������� ������������ ������
  uint  sz_pattern; //���-�� ������������ ������ � ������� pattern
  uint ident; //������������� ������
  double (*law)(const ARGLIST* arg); //��������� �� ������� ��������������� � �������
  uint total_arg;//����� ����� ���������� � ����������, ����������� ��� �������
                 //������� law
  uint read_arg; //���-�� ���������� �������, ������� �.�. ������� (��������,
                 //�� ��������� ������)
  uint offset;   //��������� ������� � ������ ���������� ARGLIST � ������� ���-
                 //����� ������ ��������� ����������
  const char* math_notate; //�������������� ������ ������
  bool type; //��������� ��� ����������������� �����
} INFO_LAW;

typedef const INFO_LAW* pINFO_LAW;//��������� �� ��������� INFO_LAW

//���������� ���������, ������� �������� ���������� �� ��������� �������� ������
//� ������ (����� � ������ ������� �������� ������) � �� ������� � ������� �����
//���� const char** Pattern. ������������ �������� SearchKeyString � �����������
//��� ��������������� ������� ������.
typedef struct
{ uint index; //������ �������� ������ � ������� �����
  uint lpos;  //����� ������� �������� ������ � �������� ������
  uint rpos;  //������ ������� �������� ������ � �������� ������
} KEYSTRPOS;

typedef KEYSTRPOS* pKEYSTRPOS; //��������� �� ��������� KEYSTRPOS

//���������� ���������, � ������� ������ �har* �������� � ������������ ������
//��������. ������������ ��� ��������� ���������� �������� �� ���������� �������
typedef struct
{ char* s; //��������� �� ���������� ������
  bool value; //���������� �������� ������ ������
} STR_BOOL;

typedef const STR_BOOL* pSTR_BOOL; //��������� �� ��������� STR_BOOL

//���������� ������� ������������ ���������� ����� ������� ���������
typedef struct
{ uint size; //������ �������
  STR_BOOL* table; //��������� �� ��������� STR_BOOL
} TABLEStrBool;

//�������� ������������ �������� ������� - UNIFORMITY PROPERTY
const bool UNIFORM    = true;  //���������� �������� �������
const bool NONUNIFORM = false; //������������ �������� �������

//�������� ���������������� �������� ������� - EQUIDISTANCE PROPERTY
const bool EQUIDISTANT   = true; //�������������� �������� �������
const bool UNEQUIDISTANT = false;//���������������� �������� �������
const bool EQUAL   = true;       //�������������� �������� �������
const bool UNEQUAL = false;      //���������������� �������� �������

//���������� ��������, ��������� � �������� ��� ��������������� �������� �������
//�� ������, ����������� � ��������� ����� ������������ ����.
const int FOPEN_ERROR = -1; //������ �������� ����� ������
const int FREAD_ERROR = -2; //������ ��� ���������� ������ �� �����

//****************************************************************************
// ���������� ������ - TLinearArray - �������� �������� �������
//****************************************************************************
class TLinearArray {
//-------------------------------------------------------------
// ���������� ���������� ������-������ ������ TLinearArray
//-------------------------------------------------------------
protected:
 //Linear array property - �������� �������� �������� �������
 bool Uniformity;   //������������ �������� �������� �������
 bool Equidistance; //���������������� �������� �������� �������

 //Configuration linear array - ������������ �������� �������� �������
 uint Left; //���-�� ��������� ��, ������������� ����� �� ��������� �������� ��
 uint Together; //���-�� ��������� ��, ������������� ������ � �������� ���������
 uint Right;//���-�� ��������� ��, ������������� ������ �� ��������� �������� ��

 //Disposition of an linear array of a rather terrestrial surface.
 //������������ �������� �������� ������� ������������ ������ �����������
 bool Disposition; //EQUATOR or MERIDIAN plane

 //The disposition laws of the antenna elements from each other.
 //������, ������������ ������������� ����������, ����� ����� ��������� ������-
 //���� �������� �������� �������. ����� ���� ������ ������������ ��� ��������.

 //���������� ������ ������������ ��������� �� ������������ ���� �����, �������
 //����������� ����� ��������� �������� (���� ����� �������� �������)
 uint LeftLawType;
 double (*LeftLaw)(const ARGLIST* Arg);
 ARGLIST* LeftLawArg;
 //���������� ������ ������������ ��������� �� ������������ ���� �����, �������
 //����������� ������ ��������� �������� (���� ����� �������� �������)
 uint RightLawType;
 double (*RightLaw)(const ARGLIST* Arg);
 ARGLIST* RightLawArg;

 ulong Count; //���������� ��������� ��
 TElArray* data; //��������� �� ������ � ���������� ��.

 //��� ����� � ��������� ������� �� Linear Array
 string fname;
 //��� ����� ����������� ��������� ������ � fname
 string ferrname;
 //���������� �������� ��������� ������ � ������� �� TLinearArray
 ifstream fin;
 //���������� ��������� ��������� ������ ����������� ��������� ������ � fin
 ofstream ferr;

//------------------------------------------------------------------------------
// I. ���������� ������������� ������-������� ������ TLinearArray
//------------------------------------------------------------------------------
public:
 //��������� ������������� �������� �� � ���� ���������� ������� string
 virtual string& Properties(string& s, bool headfoot = ON);
 //��������� �������������� �������� �������� �� � ���� ���������� ������� string
 string& LAAElemProperties(string& s, ulong index, bool headfoot = ON);
 //����������� ���������� ��������� � ��
 ulong GetTotal() const {return Count;};
 uint GetLeft() const {return Left;};
 uint GetTogether() const {return Together;};
 uint GetRight() const {return Right;};
 //����������� ������������ �������� �������� ������� ������������ ������
 //�����������
 bool GetDisp() const {return Disposition;};
 //����������� �������������� ���������� ����� �������� � ��������������� ���-
 //������� �� � ���������� ������� i.
 double GetL(ulong i) const
  {if ((i > Count) || (i == 0)) return 0.0; else return data[i].GetL();};
 //����������� ����������� ������������� � �������� �������� �� � ����������
 //������� i.
 bool GetUsage(ulong i) const
  {if (i > Count) return OFF; else return data[i].GetUsage();};
 //����������� �������������� ������������ �������� �������� �� � ����������
 //������� i.
 double GetN(ulong i) const
  {if (i > Count) return 0.0; else return data[i].GetN();};
 //���������� �������� ������������� ��������� �������������� i-�� �������� ��
 //� ����������� ��������� �������
 double CalcAntPatterns(ulong i, bool EarthPlane) const
  {if (i > Count) return 0.0; else return data[i].CalcAntPatterns(EarthPlane);};

 //������ ������������� ���������� ����� ��������� ���������� ��, � ����� �����
 //�������� � ���������������� ���������� ��
 void CalcRelDistance();
 //��������� ����� ��������� �������, ��������������� E-��������� � H-���������
 //��� ���� ��������� ��, ������������ � ��������
 void SetAngle(double AzimuthAngle, double ElevateAngle, bool Deg = DEG);
 //��������� ����� ��������� �������, ��������������� E-��������� � H-���������
 //��� i-�� �������� ��
 void SetAngle(ulong i, double AzimuthAngle, double ElevateAngle, bool Deg = DEG)
 { if (i < Count) data[i].SetAngle(AzimuthAngle,ElevateAngle,Deg);};
 //��������� ��������� �������� ��������� � ���� ��� ���� �� ������ ���������
 //�������� �������, ������������ � ��������
 void SetRandNumber();
 //���������� ��������� �������������� �������� ������� � �������� ������� ����-
 //���������� ��� �������������� ��������� �����
 rvector AntPatterns(bool EarthPlane, double ScanAngle = 0,
         double InitAngle = -90, double EndAngle = 90, double Step = 0.1,
         bool Deg = DEG);

//*******************************************************************
// ���������� ������������� � ����������� ������ TLinearArray
//*******************************************************************
 //���������� ������������ �� ���������
 TLinearArray();
 //���������� ������������ � �����������
 //�������� ������� ������ TLinearArray �� ������ �� ����� filename
 TLinearArray(const char* filename);
 //���������� ����������� ������
 virtual ~TLinearArray();

//-------------------------------------------------------------------
// II. ���������� �������� ������-������� ������ TLinearArray
//-------------------------------------------------------------------
private:

//-------------------------------------------------------------------
// III. ���������� ���������� ������-������� ������ TLinearArray
//-------------------------------------------------------------------
protected:
 //��������� ������������� ������������� Linear Array
 virtual void InitPerformance();
 //����� ���� ������������� Linear Array
 virtual void ResetPerformance();

 //*****************************************************************************
 //���������� �������, ��������������� ��� ���������� ������������� �������� ��
 //��� �� ��������������� �� ���������� ����� ������������ ����
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
 // ���������� ������� ����������� ������������� �������� �������� ������� ��
 // �������� ��������� ������ fin
 //*****************************************************************************
 bool GetLAAPerformFromFile();
 //1. ����������� ������������ �������� �������
 bool GetConfigurationLAA(uint& left, uint& together, uint& right);
 bool GetCountElArray(ulong& count, const pKEYSTRPOS info, const char** pattern);
 //2. ����������� �������� ������������ �������� �������
 bool GetUniformLAA(bool& uniform);
 //3. ����������� �������� ���������������� �������� �������
 bool GetEquidistLAA(bool& equal, double& lv);
 //4. ����������� ������ �� �������� ������������� ���� ������������ �����
 //   �������� ��, ������������� ����� �� ��������� ��������� ��������
 bool GetLeftDistLawLAA();
 //5. ����������� ������ �� �������� ������������� ���� ������������ �����
 //   �������� ��, ������������� ������ �� ��������� ��������� ��������
 bool GetRightDistLawLAA();
 //6. ����������� ������������ �������� �� ������������ ������ �����������
 bool GetDisposLAA(bool& disp);

 //����� ������� ���������� ���� ������������� �� � �� ��������� �� �����
 bool GetDataFromFile();
 //****************************************************************************
 // ���������� ������� ����������� ������������� ��������� �������� ��������
 // ������� �� �������� ��������� ������ fin
 //****************************************************************************
 bool GetElemArrayPerformFromFile(TElArray& ElArray, bool status = SLAVE);
 //1. ����������� ������������� �������� �� ��� ��������
 bool GetElArrayUsage(TElArray& ElArray);
 //2. ����������� �������������� �����-�� �������� �������� ��
 //   Relative Factor of Amplify - ARF
 bool GetElArrayARF(TElArray& ElArray);
 //3. ����������� ���� �������� �������� ������� - ���� �������
 bool GetAntennaType(TElArray& ElArray);
 bool GetAntNameFromStr(const ANTENNA_INFO* Table, const uint sz_Table,
      uint& index, const string& title);
 ARGLIST* GetDirectParamFromFile(const uint index, bool plane);
 //4. ����������� ������������ E-��������� ������� ������������ �����
 bool GetAntELocate(TElArray& ElArray);
 //5. ����������� ������ ����������� ���� �������� �������� �������
 bool GetAntNoiseLaw(TElArray& ElArray);

};//���������� ���������� ������ TLinearArray

#endif //���������� ���������� ������������� ����� "LINARRAY.H"

//---------------------- The end of file "linarray.h" --------------------------