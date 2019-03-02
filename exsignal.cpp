//exsignal.cpp
//���������� ������ ������� ������ -- ExtSignal
//Initial date: August,18,1998. Final date:
//Copyright (c) JohnSoft 1998. All rights reserved

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
//
//*****************************************************************************

#include "exsignal.h"

//���������� ������� ��������� ������� ��������� ������������� ����
//Azimuth Angle � ���� ����� Elevate Angle ��������� ������������ ��������
//������� ������������ ������� � �������� ������� ��������� ������ ��������
//�������, � ����� ��������� � ���� (�������������� � ������������ ������������)
//���� ����������� ������ ���������� �������� �������.
LAW* TableAngleLaw = NULL;
uint TableAngleLawCount = 0;

//���������� ������� ��������� ����������������� ������� ��������� ���������
//������������ �������� �������
LAW* TableAmplLaw = NULL;
uint TableAmplLawCount = 0;

//���������� ������� ��������� ��������� ������� ��������� ����������� � �������
//�������������� ���������� �������� �������
LAW* TableCasualLaw = NULL;
uint TableCasualLawCount = 0;

//���������� ������� ������������ ������������� ��������� �������� �������
const char* SignalPropertyName[] =
   //0    1. Type        2. Usage         3. Azimuth Angle   4. Elevate Angle
{"Noname","SIGNAL TYPE", "SIGNAL USAGE,", "AZIMUTH ANGLE",   "ELEVATE ANGLE",
  //5. Polar. Angle    6. Phase of vert. comp. polar.
 "POLARIZATION ANGLE", "PHASE OF VERTCOMP POLARIZATION",
 //7. Phase of hor. polar.        8. Ampitude   9. Phase  10. Casual amplitude
 "PHASE OF HORCOMP POLARIZATION", "AMPLITUDE",  "PHASE",  "CASUAL AMPLITUDE",
 //11. Casual phase
 "CASUAL PHASE" };


//*****************************************************************************
// �������� ������� �������� ������ � ���������� �������� ���������
// ������������� ������������ �������� ������� �� �������
//*****************************************************************************

//��������� �������
//*****************************************************************************
//������: II.1., private, class member-function
//������������ �������: uint AccessibleLaw(const LAW* Table, uint Count)
//����������: ������� ����� ��������� � ������ ������� � ������� ������� Table
//������� ���������� ����� ��������� ��� ������ �������. ������ ����� � �������
//����� ���� turn, ������� ���������� ����������� ������ ��� ������ ��� ������-
//�������. ������ ����� � turn = ON ����������� ���-�� ��������� ��� ������ ��-
//���, ����� ������� �� ����� ��������� Count.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. LAW* Table - ������� �������
//2. uint Count - ����� ���������� ������� � �������
//-----------------------------------------------------------------------------
//������������ ��������:
//uint Total - ���-�� ��������� ��� ������ ������� � ������� Table
//*****************************************************************************
uint ExtSignal::AccessibleLaw(const LAW* Table, uint Count)
{ uint Total = 0;
  for (uint i = 0; i < Count; i++)
   if (Table[i].turn == ON) Total++;
return Total;
}
//*****************************************************************************
//������: II.2., private, class member-function
//������������ �������: bool SetLaw(LAW* Table, uint Count, uint LawType)
//����������: ���������� ������ ������ LawType � ������� ������� Table
//������� ������������� ���� turn = ON ��� ������ � ��������������� LawType.
//���� ������ ���� LawType � ������� ���������� �� �����, �� ������� ����������
//false, ����� - true.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. LAW* Table - ������� �������
//2. uint Count - ����� ���������� ������� � �������
//3. uint LawType - ������������� ������ ����� �������� ����� ��������
//-----------------------------------------------------------------------------
//������������ ��������:
//bool Success - true - ����� ������ � ������� � ����� ��� ��������
//               false - ����� �� ������ � �������
//*****************************************************************************
bool ExtSignal::SetLaw(LAW* Table, uint Count, uint LawType)
{ bool lSuccess = false;
  for (uint i = 0; i < Count; i++)
   if (Table[i].type == LawType)
   { Table[i].turn = ON;
     lSuccess = true;
     break; //����� �� �����
   }
return lSuccess;
}
//*****************************************************************************
//������: II.3., private, class member-function
//������������ �������: bool ResetLaw(LAW* Table, uint Count, uint LawType)
//����������: ���������� ������ ������ LawType � ������� ������� Table
//������� ������������� ���� turn = OFF ��� ������ � ��������������� LawType.
//���� ������ ���� LawType � ������� ���������� �� �����, �� ������� ����������
//false, ����� - true.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. LAW* Table - ������� �������
//2. uint Count - ����� ���������� ������� � �������
//3. uint LawType - ������������� ������ ����� �������� ����� ��������
//-----------------------------------------------------------------------------
//������������ ��������:
//bool Success - true - ����� ������ � ������� � ����� ��� ��������
//               false - ����� �� ������ � �������
//*****************************************************************************
bool ExtSignal::ResetLaw(LAW* Table, uint Count, uint LawType)
{ bool lSuccess = false;
  for (uint i = 0; i < Count; i++)
   if (Table[i].type == LawType)
   { Table[i].turn = OFF;
     lSuccess = true;
     break; //����� �� �����
   }
return lSuccess;
}
//*****************************************************************************
//������: II.4., private, class member-function
//������������ �������: bool SetAllLaw(LAW* Table, uint Count)
//����������: ���������� ������ ���� ������� � ������� ������� Table
//������� ������������� ���� turn = ON ��� ���� �������.
//���� ������� ����� (Count == 0), �� ������� ���������� false, ����� - true.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. LAW* Table - ������� �������
//2. uint Count - ����� ���������� ������� � �������
//-----------------------------------------------------------------------------
//������������ ��������:
//bool Success - true - ����� ���� ������� ��������
//               false - ������� ������� �� �������� (������� �����)
//*****************************************************************************
bool ExtSignal::SetAllLaw(LAW* Table, uint Count)
{ bool lSuccess = true;
 try
 {
  if (Count == 0) throw EMPTY_TABLE;
  for (uint i = 0; i < Count; i++) Table[i].turn = ON;
 }
 catch (int error_type)
 {
  if (error_type == EMPTY_TABLE)
  { lSuccess = false;
    #if defined(CONSOLE)
      cout << "\nLAWS TABLE IS EMPTY !!!" << endl;
      getch();
    #endif
    exit(-1);
  }
 }
return lSuccess;
}
//*****************************************************************************
//������: II.5., private, class member-function
//������������ �������: bool ResetAllLaw(LAW* Table, uint Count)
//����������: ���������� ������ ���� ������� � ������� ������� Table
//������� ������������� ���� turn = OFF ��� ���� �������.
//���� ������� ����� (Count == 0), �� ������� ���������� false, ����� - true.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. LAW* Table - ������� �������
//2. uint Count - ����� ���������� ������� � �������
//-----------------------------------------------------------------------------
//������������ ��������:
//bool Success - true - ����� ���� ������� ��������
//               false - ������� ������� �� �������� (������� �����)
//*****************************************************************************
bool ExtSignal::ResetAllLaw(LAW* Table, uint Count)
{ bool lSuccess = true;
 try
 {
  if (Count == 0) throw EMPTY_TABLE;
  for (uint i = 0; i < Count; i++) Table[i].turn = OFF;
 }
 catch (int error_type)
 {
  if (error_type == EMPTY_TABLE)
  { lSuccess = false;
    #if defined(CONSOLE)
      cout << "\nLAWS TABLE IS EMPTY !!!" << endl;
      getch();
    #endif
    exit(-1);
  }
 }
return lSuccess;
}
//*****************************************************************************
//������: II.6., private, class member-function
//������������ �������: uint GetIndexLaw(LAW* Table, uint Count, uint Select)
//����������:
//������� ���������� ������ �� �������� ���������� � ������� �������, ���������
//� ������������� �����. ������ ���� ������� ���������� � ��� ������, ���� �
//������� ������� ����������� � ������ ������.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. LAW* Table - ������� �������
//2. uint Count - ���������� ������� � �������
//3. uint Select - ��������� ������� � �������
//-----------------------------------------------------------------------------
//������������ ��������:
//uint Index - ������ ���������� ������������� ������ � ������� �������
//*****************************************************************************
uint ExtSignal::GetIndexLaw(const LAW* Table, uint Count, uint Select)
{uint Index = 0;
 try
 {
  if (Count == 0) throw EMPTY_TABLE; //������� �� �������� �������

  for (uint i = 0; i < Count; i++)
   if (Table[i].turn == ON)
    if (Select == Index) {Index = i; break;}
    else Index++;
 }
 catch (int error_type)
 {
  if (error_type == EMPTY_TABLE)
  {
    #if defined(CONSOLE)
      cout << "\nLAWS TABLE IS EMPTY !!!" << endl;
      getch();
    #endif
    exit(-1);
  }
 }
return Index;
}
//*****************************************************************************
//������: II.7., private, class member-function
//������������ �������:
// uint GetIndexLawByType(LAW* Table, uint Count, uint Type)
//����������:
//������� ���������� ������ ������������ ������ � ������� ������� �� ��� ����.
//������� ������������ ��� ����������� ������� ���������� �������, ����������
//������ �� ����������� �������� ������� (�������� ������ + ������) �� ����� �
//�� ���������� � ����������� ������ � ��������� COMPONENT.
//���� ����� ������ ���� �� ���������� � ������� �������� ������� Table, ��
//��������� ��������� ���� ������.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. LAW* Table - ������� �������
//2. uint Count - ���������� ������� � �������
//3. uint Type - ������������� ������, ������� ��������� ����� � �������
//-----------------------------------------------------------------------------
//������������ ��������:
//uint Index - ������ ������ � ������� �������, ���������������� Type
//*****************************************************************************
uint ExtSignal::GetIndexLawByType(const LAW* Table, uint Count, uint Type)
{uint Index = 0;
 bool lSuccess = false;
 try
 {
  if (Count == 0) throw EMPTY_TABLE; //������� �� �������� �������
  if (Table == NULL) throw EMPTY_TABLE; //������� �� �������� �������

  for (uint i = 0; i < Count; i++)
   if (Table[i].type == Type) //����� � �����. Type ������ � ������� �������
   {Index = i; lSuccess = true;  break;}

  if (lSuccess == false) throw ABSENT_LAW; //����� ���� Type �� ������
 }
 catch (int error_type)
 {
  if (error_type == EMPTY_TABLE)
  {
   #if defined(CONSOLE)
    cout << "\nLAWS TABLE IS EMPTY !!!" << endl;
    getch();
   #endif
    exit(-1);
  }
  else if (error_type == ABSENT_LAW)
  {
   #if defined(CONSOLE)
    cout << "\nTHE LAW IS ABSENT IN LAWS TABLE !!!" << endl;
    cout << "Can't find law with the specific identifier " << Type << endl;
    getch();
   #endif
    exit(-1);
  }
 }
return Index;
}

//*****************************************************************************
//������: II.7., private, class member-function
//������������ �������: void CreateTableOfAngleLaw()
//����������:
//�������� ������� ������� ��������� �� ������� ������������� ����, ����
//����� ���������, � ����� ��������� � ���� (�������������� � ������������
//������������) ���� ����������� ������ ���������� �������� �������.
//��� ������ ������� ���������� ���������� ��������� TableAngleLaw �
//TableAngleLawCount
//������ ������� (�� �������, ����� ���������� - 5):
//1. ConstLaw; 2. LinearLaw; 3. SquareLaw; 4. CosLaw; 5. SinLaw; 6. ExpLaw
//-----------------------------------------------------------------------------
//������������ ���������: ���
//-----------------------------------------------------------------------------
//������������ ��������:  ���
//*****************************************************************************
void ExtSignal::CreateTableOfAngleLaw()
{
 TableAngleLawCount = 6;
 TableAngleLaw = new LAW[6];
 //������ ����� - F(t) = A
 TableAngleLaw[0].turn = ON;
 TableAngleLaw[0].law = ConstLaw;
 TableAngleLaw[0].type = CONST_LAW;
 TableAngleLaw[0].inputfactor = InitFactors;
 TableAngleLaw[0].mathlawfactor = GetMathNotateConstLaw;
 TableAngleLaw[0].M.DimFunc = RADIAN;
 TableAngleLaw[0].M.Size = 2;
 TableAngleLaw[0].M.Factor = new FACTOR[2];
 //t
 TableAngleLaw[0].M.Factor[0].SourceDim = SECOND;
 TableAngleLaw[0].M.Factor[0].TargetDim = SECOND;
 TableAngleLaw[0].M.Factor[0].Name = new char[2];
 strcpy(TableAngleLaw[0].M.Factor[0].Name,"t");
 TableAngleLaw[0].M.Factor[0].Convert = Copy;
 TableAngleLaw[0].M.Factor[0].Invert = Copy;
 TableAngleLaw[0].M.Factor[0].PreBlock = AlwaysTrue;
 TableAngleLaw[0].M.Factor[0].PostBlock = AlwaysTrue;
 //A
 TableAngleLaw[0].M.Factor[1].SourceDim = DEGREE;
 TableAngleLaw[0].M.Factor[1].TargetDim = RADIAN;
 TableAngleLaw[0].M.Factor[1].Name = new char[9];
 strcpy(TableAngleLaw[0].M.Factor[1].Name,"Factor A");
 TableAngleLaw[0].M.Factor[1].Convert = DegToRad;
 TableAngleLaw[0].M.Factor[1].Invert = RadToDeg;
 TableAngleLaw[0].M.Factor[1].PreBlock = AlwaysTrue;
 TableAngleLaw[0].M.Factor[1].PostBlock = AlwaysTrue;

 //������ ����� - �������� ����� F(t) = A*t+B
 TableAngleLaw[1].turn = ON;
 TableAngleLaw[1].law = LinearLaw;
 TableAngleLaw[1].type = LINEAR_LAW;
 TableAngleLaw[1].inputfactor = InitFactors;
 TableAngleLaw[1].mathlawfactor = GetMathNotateLinearLaw;
 TableAngleLaw[1].M.DimFunc = RADIAN;
 TableAngleLaw[1].M.Size = 3;
 TableAngleLaw[1].M.Factor = new FACTOR[3];

 //t {SECOND,SECOND,"t",Copy,Copy,AlwaysTrue,AlwaysTrue}
 TableAngleLaw[1].M.Factor[0].SourceDim = SECOND;
 TableAngleLaw[1].M.Factor[0].TargetDim = SECOND;
 TableAngleLaw[1].M.Factor[0].Name = new char[2];
 strcpy(TableAngleLaw[1].M.Factor[0].Name,"t");
 TableAngleLaw[1].M.Factor[0].Convert = Copy;
 TableAngleLaw[1].M.Factor[0].Invert = Copy;
 TableAngleLaw[1].M.Factor[0].PreBlock = AlwaysTrue;
 TableAngleLaw[1].M.Factor[0].PostBlock = AlwaysTrue;

 //A {DEG_SEC,RAD_SEC,"Factor A",DegToRad,RadToDeg,AlwaysTrue,AlwaysTrue}
 TableAngleLaw[1].M.Factor[1].SourceDim = DEG_SEC;
 TableAngleLaw[1].M.Factor[1].TargetDim = RAD_SEC;
 TableAngleLaw[1].M.Factor[1].Name = new char[9];
 strcpy(TableAngleLaw[1].M.Factor[1].Name,"Factor A");
 TableAngleLaw[1].M.Factor[1].Convert = DegToRad;
 TableAngleLaw[1].M.Factor[1].Invert = RadToDeg;
 TableAngleLaw[1].M.Factor[1].PreBlock = AlwaysTrue;
 TableAngleLaw[1].M.Factor[1].PostBlock = AlwaysTrue;

 //B {DEGREE,RADIAN,"Factor B",DegToRad,RadToDeg,AlwaysTrue,AlwaysTrue}
 TableAngleLaw[1].M.Factor[2].SourceDim = DEGREE;
 TableAngleLaw[1].M.Factor[2].TargetDim = RADIAN;
 TableAngleLaw[1].M.Factor[2].Name = new char[9];
 strcpy(TableAngleLaw[1].M.Factor[2].Name,"Factor B");
 TableAngleLaw[1].M.Factor[2].Convert = DegToRad;
 TableAngleLaw[1].M.Factor[2].Invert = RadToDeg;
 TableAngleLaw[1].M.Factor[2].PreBlock = AlwaysTrue;
 TableAngleLaw[1].M.Factor[2].PostBlock = AlwaysTrue;

 //������ ����� - ������������ ����� F(t) = A*t^2+B*t+C
 TableAngleLaw[2].turn = ON;
 TableAngleLaw[2].law = SquareLaw;
 TableAngleLaw[2].type = SQUARE_LAW;
 TableAngleLaw[2].inputfactor = InitFactors;
 TableAngleLaw[2].mathlawfactor = GetMathNotateSquareLaw;
 TableAngleLaw[2].M.DimFunc = RADIAN;
 TableAngleLaw[2].M.Size = 4;
 TableAngleLaw[2].M.Factor = new FACTOR[4];

 //t {SECOND,SECOND,"t",Copy,Copy,AlwaysTrue,AlwaysTrue}
 TableAngleLaw[2].M.Factor[0].SourceDim = SECOND;
 TableAngleLaw[2].M.Factor[0].TargetDim = SECOND;
 TableAngleLaw[2].M.Factor[0].Name = new char[2];
 strcpy(TableAngleLaw[2].M.Factor[0].Name,"t");
 TableAngleLaw[2].M.Factor[0].Convert = Copy;
 TableAngleLaw[2].M.Factor[0].Invert = Copy;
 TableAngleLaw[2].M.Factor[0].PreBlock = AlwaysTrue;
 TableAngleLaw[2].M.Factor[0].PostBlock = AlwaysTrue;

 //A {DEG_SEC2,RAD_SEC2,"Factor A",DegToRad,RadToDeg,AlwaysTrue,AlwaysTrue}
 TableAngleLaw[2].M.Factor[1].SourceDim = DEG_SEC2;
 TableAngleLaw[2].M.Factor[1].TargetDim = RAD_SEC2;
 TableAngleLaw[2].M.Factor[1].Name = new char[9];
 strcpy(TableAngleLaw[2].M.Factor[1].Name,"Factor A");
 TableAngleLaw[2].M.Factor[1].Convert = DegToRad;
 TableAngleLaw[2].M.Factor[1].Invert = RadToDeg;
 TableAngleLaw[2].M.Factor[1].PreBlock = AlwaysTrue;
 TableAngleLaw[2].M.Factor[1].PostBlock = AlwaysTrue;

 //B {DEG_SEC,RAD_SEC,"Factor B",DegToRad,RadToDeg,AlwaysTrue,AlwaysTrue}
 TableAngleLaw[2].M.Factor[2].SourceDim = DEG_SEC;
 TableAngleLaw[2].M.Factor[2].TargetDim = RAD_SEC;
 TableAngleLaw[2].M.Factor[2].Name = new char[9];
 strcpy(TableAngleLaw[2].M.Factor[2].Name,"Factor B");
 TableAngleLaw[2].M.Factor[2].Convert = DegToRad;
 TableAngleLaw[2].M.Factor[2].Invert = RadToDeg;
 TableAngleLaw[2].M.Factor[2].PreBlock = AlwaysTrue;
 TableAngleLaw[2].M.Factor[2].PostBlock = AlwaysTrue;

 //C {DEGREE,RADIAN,"Factor C",DegToRad,RadToDeg,AlwaysTrue,AlwaysTrue}
 TableAngleLaw[2].M.Factor[3].SourceDim = DEGREE;
 TableAngleLaw[2].M.Factor[3].TargetDim = RADIAN;
 TableAngleLaw[2].M.Factor[3].Name = new char[9];
 strcpy(TableAngleLaw[2].M.Factor[3].Name,"Factor C");
 TableAngleLaw[2].M.Factor[3].Convert = DegToRad;
 TableAngleLaw[2].M.Factor[3].Invert = RadToDeg;
 TableAngleLaw[2].M.Factor[3].PreBlock = AlwaysTrue;
 TableAngleLaw[2].M.Factor[3].PostBlock = AlwaysTrue;

 //��������� ����� - ���������������� ����� F(t) = A*COS(B*t+C)
 TableAngleLaw[3].turn = ON;
 TableAngleLaw[3].law = CosLaw;
 TableAngleLaw[3].type = COS_LAW;
 TableAngleLaw[3].inputfactor = InitFactors;
 TableAngleLaw[3].mathlawfactor = GetMathNotateCosLaw;
 TableAngleLaw[3].M.DimFunc = RADIAN;
 TableAngleLaw[3].M.Size = 4;
 TableAngleLaw[3].M.Factor = new FACTOR[4];

 //t {SECOND,SECOND,"t",Copy,Copy,AlwaysTrue,AlwaysTrue}
 TableAngleLaw[3].M.Factor[0].SourceDim = SECOND;
 TableAngleLaw[3].M.Factor[0].TargetDim = SECOND;
 TableAngleLaw[3].M.Factor[0].Name = new char[2];
 strcpy(TableAngleLaw[3].M.Factor[0].Name,"t");
 TableAngleLaw[3].M.Factor[0].Convert = Copy;
 TableAngleLaw[3].M.Factor[0].Invert = Copy;
 TableAngleLaw[3].M.Factor[0].PreBlock = AlwaysTrue;
 TableAngleLaw[3].M.Factor[0].PostBlock = AlwaysTrue;

 //A {DEGREE,RADIAN,"Amplitude A",DegToRad,RadToDeg,AlwaysTrue,AlwaysTrue}
 TableAngleLaw[3].M.Factor[1].SourceDim = DEGREE;
 TableAngleLaw[3].M.Factor[1].TargetDim = RADIAN;
 TableAngleLaw[3].M.Factor[1].Name = new char[12];
 strcpy(TableAngleLaw[3].M.Factor[1].Name,"Amplitude A");
 TableAngleLaw[3].M.Factor[1].Convert = DegToRad;
 TableAngleLaw[3].M.Factor[1].Invert = RadToDeg;
 TableAngleLaw[3].M.Factor[1].PreBlock = AlwaysTrue;
 TableAngleLaw[3].M.Factor[1].PostBlock = AlwaysTrue;

 //B {HZ,PULSAT,"Frequence B",FreqToPulsatance,PulsatanceToFreq,AlwaysTrue,
 //   AlwaysTrue},
 TableAngleLaw[3].M.Factor[2].SourceDim = HZ;
 TableAngleLaw[3].M.Factor[2].TargetDim = PULSAT;
 TableAngleLaw[3].M.Factor[2].Name = new char[12];
 strcpy(TableAngleLaw[3].M.Factor[2].Name,"Frequence B");
 TableAngleLaw[3].M.Factor[2].Convert = FreqToPulsatance;
 TableAngleLaw[3].M.Factor[2].Invert = PulsatanceToFreq;
 TableAngleLaw[3].M.Factor[2].PreBlock = AlwaysTrue;
 TableAngleLaw[3].M.Factor[2].PostBlock = AlwaysTrue;

 //C {DEGREE,RADIAN,"Epoch Angle C",DegToRad,RadToDeg,AlwaysTrue,AlwaysTrue}
 TableAngleLaw[3].M.Factor[3].SourceDim = DEGREE;
 TableAngleLaw[3].M.Factor[3].TargetDim = RADIAN;
 TableAngleLaw[3].M.Factor[3].Name = new char[14];
 strcpy(TableAngleLaw[3].M.Factor[3].Name,"Epoch Angle C");
 TableAngleLaw[3].M.Factor[3].Convert = DegToRad;
 TableAngleLaw[3].M.Factor[3].Invert = RadToDeg;
 TableAngleLaw[3].M.Factor[3].PreBlock = AlwaysTrue;
 TableAngleLaw[3].M.Factor[3].PostBlock = AlwaysTrue;

 //����� ����� - �������������� ����� F(t) = A*SIN(B*t+C)
 TableAngleLaw[4].turn = ON;
 TableAngleLaw[4].law = SinLaw;
 TableAngleLaw[4].type = SIN_LAW;
 TableAngleLaw[4].inputfactor = InitFactors;
 TableAngleLaw[4].mathlawfactor = GetMathNotateSinLaw;
 TableAngleLaw[4].M.DimFunc = RADIAN;
 TableAngleLaw[4].M.Size = 4;
 TableAngleLaw[4].M.Factor = new FACTOR[4];

 //t {SECOND,SECOND,"t",Copy,Copy,AlwaysTrue,AlwaysTrue}
 TableAngleLaw[4].M.Factor[0].SourceDim = SECOND;
 TableAngleLaw[4].M.Factor[0].TargetDim = SECOND;
 TableAngleLaw[4].M.Factor[0].Name = new char[2];
 strcpy(TableAngleLaw[4].M.Factor[0].Name,"t");
 TableAngleLaw[4].M.Factor[0].Convert = Copy;
 TableAngleLaw[4].M.Factor[0].Invert = Copy;
 TableAngleLaw[4].M.Factor[0].PreBlock = AlwaysTrue;
 TableAngleLaw[4].M.Factor[0].PostBlock = AlwaysTrue;

 //A {DEGREE,RADIAN,"Amplitude A",DegToRad,RadToDeg,AlwaysTrue,AlwaysTrue}
 TableAngleLaw[4].M.Factor[1].SourceDim = DEGREE;
 TableAngleLaw[4].M.Factor[1].TargetDim = RADIAN;
 TableAngleLaw[4].M.Factor[1].Name = new char[12];
 strcpy(TableAngleLaw[4].M.Factor[1].Name,"Amplitude A");
 TableAngleLaw[4].M.Factor[1].Convert = DegToRad;
 TableAngleLaw[4].M.Factor[1].Invert = RadToDeg;
 TableAngleLaw[4].M.Factor[1].PreBlock = AlwaysTrue;
 TableAngleLaw[4].M.Factor[1].PostBlock = AlwaysTrue;

 //B {HZ,PULSAT,"Frequence B",FreqToPulsatance,PulsatanceToFreq,AlwaysTrue,
 //   AlwaysTrue},
 TableAngleLaw[4].M.Factor[2].SourceDim = HZ;
 TableAngleLaw[4].M.Factor[2].TargetDim = PULSAT;
 TableAngleLaw[4].M.Factor[2].Name = new char[12];
 strcpy(TableAngleLaw[4].M.Factor[2].Name,"Frequence B");
 TableAngleLaw[4].M.Factor[2].Convert = FreqToPulsatance;
 TableAngleLaw[4].M.Factor[2].Invert = PulsatanceToFreq;
 TableAngleLaw[4].M.Factor[2].PreBlock = AlwaysTrue;
 TableAngleLaw[4].M.Factor[2].PostBlock = AlwaysTrue;

 //C {DEGREE,RADIAN,"Epoch Angle C",DegToRad,RadToDeg,AlwaysTrue,AlwaysTrue}
 TableAngleLaw[4].M.Factor[3].SourceDim = DEGREE;
 TableAngleLaw[4].M.Factor[3].TargetDim = RADIAN;
 TableAngleLaw[4].M.Factor[3].Name = new char[14];
 strcpy(TableAngleLaw[4].M.Factor[3].Name,"Epoch Angle C");
 TableAngleLaw[4].M.Factor[3].Convert = DegToRad;
 TableAngleLaw[4].M.Factor[3].Invert = RadToDeg;
 TableAngleLaw[4].M.Factor[3].PreBlock = AlwaysTrue;
 TableAngleLaw[4].M.Factor[3].PostBlock = AlwaysTrue;

 //������ ����� - ���������������� ����� F(t) = A*EXP(B*t)
 TableAngleLaw[5].turn = ON;
 TableAngleLaw[5].law = ExpLaw;
 TableAngleLaw[5].type = EXP_LAW;
 TableAngleLaw[5].inputfactor = InitFactors;
 TableAngleLaw[5].mathlawfactor = GetMathNotateExpLaw;
 TableAngleLaw[5].M.DimFunc = RADIAN;
 TableAngleLaw[5].M.Size = 3;
 TableAngleLaw[5].M.Factor = new FACTOR[3];

 //t {SECOND,SECOND,"t",Copy,Copy,AlwaysTrue,AlwaysTrue}
 TableAngleLaw[5].M.Factor[0].SourceDim = SECOND;
 TableAngleLaw[5].M.Factor[0].TargetDim = SECOND;
 TableAngleLaw[5].M.Factor[0].Name = new char[2];
 strcpy(TableAngleLaw[5].M.Factor[0].Name,"t");
 TableAngleLaw[5].M.Factor[0].Convert = Copy;
 TableAngleLaw[5].M.Factor[0].Invert = Copy;
 TableAngleLaw[5].M.Factor[0].PreBlock = AlwaysTrue;
 TableAngleLaw[5].M.Factor[0].PostBlock = AlwaysTrue;

 //A {DEG_SEC,RAD_SEC,"Factor A",DegToRad,RadToDeg,AlwaysTrue,AlwaysTrue}
 TableAngleLaw[5].M.Factor[1].SourceDim = DEGREE;
 TableAngleLaw[5].M.Factor[1].TargetDim = RADIAN;
 TableAngleLaw[5].M.Factor[1].Name = new char[9];
 strcpy(TableAngleLaw[5].M.Factor[1].Name,"Factor A");
 TableAngleLaw[5].M.Factor[1].Convert = DegToRad;
 TableAngleLaw[5].M.Factor[1].Invert = RadToDeg;
 TableAngleLaw[5].M.Factor[1].PreBlock = AlwaysTrue;
 TableAngleLaw[5].M.Factor[1].PostBlock = AlwaysTrue;

 //B {DEGREE,RADIAN,"Factor B",DegToRad,RadToDeg,AlwaysTrue,AlwaysTrue}
 TableAngleLaw[5].M.Factor[2].SourceDim = DEG_SEC;
 TableAngleLaw[5].M.Factor[2].TargetDim = RAD_SEC;
 TableAngleLaw[5].M.Factor[2].Name = new char[9];
 strcpy(TableAngleLaw[5].M.Factor[2].Name,"Factor B");
 TableAngleLaw[5].M.Factor[2].Convert = DegToRad;
 TableAngleLaw[5].M.Factor[2].Invert = RadToDeg;
 TableAngleLaw[5].M.Factor[2].PreBlock = AlwaysTrue;
 TableAngleLaw[5].M.Factor[2].PostBlock = AlwaysTrue;

}
//*****************************************************************************
//������: II.8., private, class member-function
//������������ �������: void CreateTableOfAmplLaw()
//����������:
//�������� ������� ����������������� ������� ��������� �� �������
//��� ������ ������� ���������� ���������� ��������� TableAmplLaw �
//TableAmplLawCount
//������ ������� (�� �������, ����� ���������� - 4):
//1. ConstLaw; 2. CosLaw; 3. SinLaw; 4. SquarePulseLaw
//-----------------------------------------------------------------------------
//������������ ���������: ���
//-----------------------------------------------------------------------------
//������������ ��������:  ���
//*****************************************************************************
void ExtSignal::CreateTableOfAmplLaw()
{
 TableAmplLawCount = 4;
 TableAmplLaw = new LAW[4];

 //****************************
 //������ ����� - F(t) = A
 //****************************
 // {ConstLaw, CONST_LAW, InitFactors, GetMathNotateConstLaw, RADIAN, 2}
 TableAmplLaw[0].turn = ON;
 TableAmplLaw[0].law = ConstLaw;
 TableAmplLaw[0].type = CONST_LAW;
 TableAmplLaw[0].inputfactor = InitFactors;
 TableAmplLaw[0].mathlawfactor = GetMathNotateConstLaw;
 TableAmplLaw[0].M.DimFunc = NONDIM;
 TableAmplLaw[0].M.Size = 2;
 TableAmplLaw[0].M.Factor = new FACTOR[2];

 //t {SECOND,SECOND,"t",Copy,Copy,AlwaysTrue,AlwaysTrue}
 TableAmplLaw[0].M.Factor[0].SourceDim = SECOND;
 TableAmplLaw[0].M.Factor[0].TargetDim = SECOND;
 TableAmplLaw[0].M.Factor[0].Name = new char[2];
 strcpy(TableAmplLaw[0].M.Factor[0].Name,"t");
 TableAmplLaw[0].M.Factor[0].Convert = Copy;
 TableAmplLaw[0].M.Factor[0].Invert = Copy;
 TableAmplLaw[0].M.Factor[0].PreBlock = AlwaysTrue;
 TableAmplLaw[0].M.Factor[0].PostBlock = AlwaysTrue;

 //A  {NONDIM,NONDIM,"Amplitude A",Copy,Copy,AlwaysTrue,AlwaysTrue}
 TableAmplLaw[0].M.Factor[1].SourceDim = NONDIM;
 TableAmplLaw[0].M.Factor[1].TargetDim = NONDIM;
 TableAmplLaw[0].M.Factor[1].Name = new char[12];
 strcpy(TableAmplLaw[0].M.Factor[1].Name,"Amplitude A");
 TableAmplLaw[0].M.Factor[1].Convert = Copy;
 TableAmplLaw[0].M.Factor[1].Invert = Copy;
 TableAmplLaw[0].M.Factor[1].PreBlock = AlwaysTrue;
 TableAmplLaw[0].M.Factor[1].PostBlock = AlwaysTrue;

 //***********************************
 //������ ����� - F(t) = A*COS(B*t+C)
 //**********************************
 //{CosLaw,COS_LAW, InitFactors, GetMathNotateCosLaw, RADIAN, 4}
 TableAmplLaw[1].turn = ON;
 TableAmplLaw[1].law = CosLaw;
 TableAmplLaw[1].type = COS_LAW;
 TableAmplLaw[1].inputfactor = InitFactors;
 TableAmplLaw[1].mathlawfactor = GetMathNotateCosLaw;
 TableAmplLaw[1].M.DimFunc = NONDIM;
 TableAmplLaw[1].M.Size = 4;
 TableAmplLaw[1].M.Factor = new FACTOR[4];

 //t  {SECOND,SECOND,"t",Copy,Copy,AlwaysTrue,AlwaysTrue}
 TableAmplLaw[1].M.Factor[0].SourceDim = SECOND;
 TableAmplLaw[1].M.Factor[0].TargetDim = SECOND;
 TableAmplLaw[1].M.Factor[0].Name = new char[2];
 strcpy(TableAmplLaw[1].M.Factor[0].Name,"t");
 TableAmplLaw[1].M.Factor[0].Convert = Copy;
 TableAmplLaw[1].M.Factor[0].Invert = Copy;
 TableAmplLaw[1].M.Factor[0].PreBlock = AlwaysTrue;
 TableAmplLaw[1].M.Factor[0].PostBlock = AlwaysTrue;

 //A {NONDIM,NONDIM,"Amplitude A",DegToRad,RadToDeg,AlwaysTrue,AlwaysTrue}
 TableAmplLaw[1].M.Factor[1].SourceDim = NONDIM;
 TableAmplLaw[1].M.Factor[1].TargetDim = NONDIM;
 TableAmplLaw[1].M.Factor[1].Name = new char[12];
 strcpy(TableAmplLaw[1].M.Factor[1].Name,"Amplitude A");
 TableAmplLaw[1].M.Factor[1].Convert = Copy;
 TableAmplLaw[1].M.Factor[1].Invert = Copy;
 TableAmplLaw[1].M.Factor[1].PreBlock = AlwaysTrue;
 TableAmplLaw[1].M.Factor[1].PostBlock = AlwaysTrue;

 //B {HZ,PULSAT,"Frequence B",FreqToPulsatance,PulsatanceToFreq,AlwaysTrue,
 //   AlwaysTrue}
 TableAmplLaw[1].M.Factor[2].SourceDim = HZ;
 TableAmplLaw[1].M.Factor[2].TargetDim = PULSAT;
 TableAmplLaw[1].M.Factor[2].Name = new char[12];
 strcpy(TableAmplLaw[1].M.Factor[2].Name,"Frequence B");
 TableAmplLaw[1].M.Factor[2].Convert = FreqToPulsatance;
 TableAmplLaw[1].M.Factor[2].Invert = PulsatanceToFreq;
 TableAmplLaw[1].M.Factor[2].PreBlock = AlwaysTrue;
 TableAmplLaw[1].M.Factor[2].PostBlock = AlwaysTrue;

 //C {DEGREE,RADIAN,"Epoch Angle C",DegToRad,RadToDeg,AlwaysTrue,AlwaysTrue}
 TableAmplLaw[1].M.Factor[3].SourceDim = DEGREE;
 TableAmplLaw[1].M.Factor[3].TargetDim = RADIAN;
 TableAmplLaw[1].M.Factor[3].Name = new char[14];
 strcpy(TableAmplLaw[1].M.Factor[3].Name,"Epoch Angle C");
 TableAmplLaw[1].M.Factor[3].Convert = DegToRad;
 TableAmplLaw[1].M.Factor[3].Invert = RadToDeg;
 TableAmplLaw[1].M.Factor[3].PreBlock = AlwaysTrue;
 TableAmplLaw[1].M.Factor[3].PostBlock = AlwaysTrue;

 //***********************************
 //������ ����� - F(t) = A*SIN(B*t+C)
 //**********************************
 //{SinLaw,SIN_LAW, InitFactors, GetMathNotateSinLaw, RADIAN, 4}
 TableAmplLaw[2].turn = ON;
 TableAmplLaw[2].law = SinLaw;
 TableAmplLaw[2].type = SIN_LAW;
 TableAmplLaw[2].inputfactor = InitFactors;
 TableAmplLaw[2].mathlawfactor = GetMathNotateSinLaw;
 TableAmplLaw[2].M.DimFunc = NONDIM;
 TableAmplLaw[2].M.Size = 4;
 TableAmplLaw[2].M.Factor = new FACTOR[4];

 //t  {SECOND,SECOND,"t",Copy,Copy,AlwaysTrue,AlwaysTrue}
 TableAmplLaw[2].M.Factor[0].SourceDim = SECOND;
 TableAmplLaw[2].M.Factor[0].TargetDim = SECOND;
 TableAmplLaw[2].M.Factor[0].Name = new char[2];
 strcpy(TableAmplLaw[2].M.Factor[0].Name,"t");
 TableAmplLaw[2].M.Factor[0].Convert = Copy;
 TableAmplLaw[2].M.Factor[0].Invert = Copy;
 TableAmplLaw[2].M.Factor[0].PreBlock = AlwaysTrue;
 TableAmplLaw[2].M.Factor[0].PostBlock = AlwaysTrue;

 //A {NONDIM,NONDIM,"Amplitude A",DegToRad,RadToDeg,AlwaysTrue,AlwaysTrue}
 TableAmplLaw[2].M.Factor[1].SourceDim = NONDIM;
 TableAmplLaw[2].M.Factor[1].TargetDim = NONDIM;
 TableAmplLaw[2].M.Factor[1].Name = new char[12];
 strcpy(TableAmplLaw[2].M.Factor[1].Name,"Amplitude A");
 TableAmplLaw[2].M.Factor[1].Convert = Copy;
 TableAmplLaw[2].M.Factor[1].Invert = Copy;
 TableAmplLaw[2].M.Factor[1].PreBlock = AlwaysTrue;
 TableAmplLaw[2].M.Factor[1].PostBlock = AlwaysTrue;

 //B {HZ,PULSAT,"Frequence B",FreqToPulsatance,PulsatanceToFreq,AlwaysTrue,
 //   AlwaysTrue}
 TableAmplLaw[2].M.Factor[2].SourceDim = HZ;
 TableAmplLaw[2].M.Factor[2].TargetDim = PULSAT;
 TableAmplLaw[2].M.Factor[2].Name = new char[12];
 strcpy(TableAmplLaw[2].M.Factor[2].Name,"Frequence B");
 TableAmplLaw[2].M.Factor[2].Convert = FreqToPulsatance;
 TableAmplLaw[2].M.Factor[2].Invert = PulsatanceToFreq;
 TableAmplLaw[2].M.Factor[2].PreBlock = AlwaysTrue;
 TableAmplLaw[2].M.Factor[2].PostBlock = AlwaysTrue;

 //C {DEGREE,RADIAN,"Epoch Angle C",DegToRad,RadToDeg,AlwaysTrue,AlwaysTrue}
 TableAmplLaw[2].M.Factor[3].SourceDim = DEGREE;
 TableAmplLaw[2].M.Factor[3].TargetDim = RADIAN;
 TableAmplLaw[2].M.Factor[3].Name = new char[14];
 strcpy(TableAmplLaw[2].M.Factor[3].Name,"Epoch Angle C");
 TableAmplLaw[2].M.Factor[3].Convert = DegToRad;
 TableAmplLaw[2].M.Factor[3].Invert = RadToDeg;
 TableAmplLaw[2].M.Factor[3].PreBlock = AlwaysTrue;
 TableAmplLaw[2].M.Factor[3].PostBlock = AlwaysTrue;

 //**********************************************
 //��������� ����� - F(t) = ������������� �������
 //**********************************************
 //{SquarePulseLaw, SQUAREPULSE_LAW, InitFactors, GetMathNotateSquarePulseLaw,
 // NONDIM, 8}
 TableAmplLaw[3].turn = ON;
 TableAmplLaw[3].law = SquarePulseLaw;
 TableAmplLaw[3].type = SQUAREPULSE_LAW;
 TableAmplLaw[3].inputfactor = InitFactors;
 TableAmplLaw[3].mathlawfactor = GetMathNotateSquarePulseLaw;
 TableAmplLaw[3].M.DimFunc = NONDIM;
 TableAmplLaw[3].M.Size = 8;
 TableAmplLaw[3].M.Factor = new FACTOR[8];

 //t  {SECOND,SECOND,"t",Copy,Copy,AlwaysTrue,AlwaysTrue}
 TableAmplLaw[3].M.Factor[0].SourceDim = SECOND;
 TableAmplLaw[3].M.Factor[0].TargetDim = SECOND;
 TableAmplLaw[3].M.Factor[0].Name = new char[2];
 strcpy(TableAmplLaw[3].M.Factor[0].Name,"t");
 TableAmplLaw[3].M.Factor[0].Convert = Copy;
 TableAmplLaw[3].M.Factor[0].Invert = Copy;
 TableAmplLaw[3].M.Factor[0].PreBlock = AlwaysTrue;
 TableAmplLaw[3].M.Factor[0].PostBlock = AlwaysTrue;

 //t0 {NONDIM,NONDIM,"t0",Copy,Copy,AlwaysTrue,AlwaysTrue}
 TableAmplLaw[3].M.Factor[1].SourceDim = NONDIM;
 TableAmplLaw[3].M.Factor[1].TargetDim = NONDIM;
 TableAmplLaw[3].M.Factor[1].Name = new char[3];
 strcpy(TableAmplLaw[3].M.Factor[1].Name,"t0");
 TableAmplLaw[3].M.Factor[1].Convert = Copy;
 TableAmplLaw[3].M.Factor[1].Invert = Copy;
 TableAmplLaw[3].M.Factor[1].PreBlock = AlwaysTrue;
 TableAmplLaw[3].M.Factor[1].PostBlock = AlwaysTrue;

 //timp {NONDIM,NONDIM,"timp",Copy,Copy,AlwaysTrue,AlwaysTrue}
 TableAmplLaw[3].M.Factor[2].SourceDim = NONDIM;
 TableAmplLaw[3].M.Factor[2].TargetDim = NONDIM;
 TableAmplLaw[3].M.Factor[2].Name = new char[5];
 strcpy(TableAmplLaw[3].M.Factor[2].Name,"timp");
 TableAmplLaw[3].M.Factor[2].Convert = Copy;
 TableAmplLaw[3].M.Factor[2].Invert = Copy;
 TableAmplLaw[3].M.Factor[2].PreBlock = AlwaysTrue;
 TableAmplLaw[3].M.Factor[2].PostBlock = AlwaysTrue;

 //Timp {NONDIM,NONDIM,"Timp",Copy,Copy,AlwaysTrue,AlwaysTrue}
 TableAmplLaw[3].M.Factor[3].SourceDim = NONDIM;
 TableAmplLaw[3].M.Factor[3].TargetDim = NONDIM;
 TableAmplLaw[3].M.Factor[3].Name = new char[5];
 strcpy(TableAmplLaw[3].M.Factor[3].Name,"Timp");
 TableAmplLaw[3].M.Factor[3].Convert = Copy;
 TableAmplLaw[3].M.Factor[3].Invert = Copy;
 TableAmplLaw[3].M.Factor[3].PreBlock = AlwaysTrue;
 TableAmplLaw[3].M.Factor[3].PostBlock = AlwaysTrue;

 //Umax {NONDIM,NONDIM,"Umax",Copy,Copy,AlwaysTrue,AlwaysTrue}
 TableAmplLaw[3].M.Factor[4].SourceDim = NONDIM;
 TableAmplLaw[3].M.Factor[4].TargetDim = NONDIM;
 TableAmplLaw[3].M.Factor[4].Name = new char[5];
 strcpy(TableAmplLaw[3].M.Factor[4].Name,"Umax");
 TableAmplLaw[3].M.Factor[4].Convert = Copy;
 TableAmplLaw[3].M.Factor[4].Invert = Copy;
 TableAmplLaw[3].M.Factor[4].PreBlock = AlwaysTrue;
 TableAmplLaw[3].M.Factor[4].PostBlock = AlwaysTrue;

 //Umin {NONDIM,NONDIM,"Umin",Copy,Copy,AlwaysTrue,AlwaysTrue}
 TableAmplLaw[3].M.Factor[5].SourceDim = NONDIM;
 TableAmplLaw[3].M.Factor[5].TargetDim = NONDIM;
 TableAmplLaw[3].M.Factor[5].Name = new char[5];
 strcpy(TableAmplLaw[3].M.Factor[5].Name,"Umin");
 TableAmplLaw[3].M.Factor[5].Convert = Copy;
 TableAmplLaw[3].M.Factor[5].Invert = Copy;
 TableAmplLaw[3].M.Factor[5].PreBlock = AlwaysTrue;
 TableAmplLaw[3].M.Factor[5].PostBlock = AlwaysTrue;

 //Flag {NONDIM,NONDIM,"Flag",Copy,Copy,AlwaysTrue,AlwaysTrue}
 TableAmplLaw[3].M.Factor[6].SourceDim = NONDIM;
 TableAmplLaw[3].M.Factor[6].TargetDim = NONDIM;
 TableAmplLaw[3].M.Factor[6].Name = new char[5];
 strcpy(TableAmplLaw[3].M.Factor[6].Name,"Flag");
 TableAmplLaw[3].M.Factor[6].Convert = Copy;
 TableAmplLaw[3].M.Factor[6].Invert = Copy;
 TableAmplLaw[3].M.Factor[6].PreBlock = AlwaysTrue;
 TableAmplLaw[3].M.Factor[6].PostBlock = AlwaysTrue;

 //Count {NONDIM,NONDIM,"Count",Copy,Copy,AlwaysTrue,AlwaysTrue}
 TableAmplLaw[3].M.Factor[7].SourceDim = NONDIM;
 TableAmplLaw[3].M.Factor[7].TargetDim = NONDIM;
 TableAmplLaw[3].M.Factor[7].Name = new char[6];
 strcpy(TableAmplLaw[3].M.Factor[7].Name,"Count");
 TableAmplLaw[3].M.Factor[7].Convert = Copy;
 TableAmplLaw[3].M.Factor[7].Invert = Copy;
 TableAmplLaw[3].M.Factor[7].PreBlock = AlwaysTrue;
 TableAmplLaw[3].M.Factor[7].PostBlock = AlwaysTrue;

}
//*****************************************************************************
//������: II.9., private, class member-function
//������������ �������: void CreateTableOfCasualLaw()
//����������:
//�������� ������� ��������� ������� ��������� ������� �� �������
//��� ������ ������� ���������� ���������� ��������� TableCasualLaw �
//TableCasualLawCount
//������ ������� (�� �������, ����� ���������� - 6):
//1. RandLaw; 2. Rand2PiLaw; 3. RandABLaw; 4. RelayLaw; 5. Det1Law; 6. Det0Law
//-----------------------------------------------------------------------------
//������������ ���������: ���
//-----------------------------------------------------------------------------
//������������ ��������:  ���
//*****************************************************************************
void ExtSignal::CreateTableOfCasualLaw()
{
 TableCasualLawCount = 6;
 TableCasualLaw = new LAW[6];

 //********************************************************
 //������ ����� - �������������� ����� � ��������� [0..1]
 //��������� ������� � �������� �����������.
 //********************************************************
 // {RandLaw, RAND_LAW, InitFactors, GetMathNotateRandLaw, NONDIM, 0}
 TableCasualLaw[0].turn = ON;
 TableCasualLaw[0].law = RandLaw;
 TableCasualLaw[0].type = RAND_LAW;
 TableCasualLaw[0].inputfactor = InitFactors;
 TableCasualLaw[0].mathlawfactor = GetMathNotateRandLaw;
 TableCasualLaw[0].M.DimFunc = NONDIM;
 TableCasualLaw[0].M.Size = 1;
 TableCasualLaw[0].M.Factor = new FACTOR[1];

 //RandValue
 TableCasualLaw[0].M.Factor[0].SourceDim = NONDIM;
 TableCasualLaw[0].M.Factor[0].TargetDim = NONDIM;
 TableCasualLaw[0].M.Factor[0].Name = new char[5];
 strcpy(TableCasualLaw[0].M.Factor[0].Name,"Rand");
 TableCasualLaw[0].M.Factor[0].Convert = Copy;
 TableCasualLaw[0].M.Factor[0].Invert = Copy;
 TableCasualLaw[0].M.Factor[0].PreBlock = AlwaysTrue;
 TableCasualLaw[0].M.Factor[0].PostBlock = AlwaysTrue;

 //*************************************************************
 //������ ����� - �������������� ����� � ��������� [0..2PI]
 //*************************************************************
 //{Ran2PiLaw,RAND2PI_LAW, InitFactors, GetMathNotateRand2PiLaw, NONDIM, 0}
 TableCasualLaw[1].turn = ON;
 TableCasualLaw[1].law = Rand2PiLaw;
 TableCasualLaw[1].type = RAND2PI_LAW;
 TableCasualLaw[1].inputfactor = InitFactors;
 TableCasualLaw[1].mathlawfactor = GetMathNotateRand2PiLaw;
 TableCasualLaw[1].M.DimFunc = NONDIM;
 TableCasualLaw[1].M.Size = 1;
 TableCasualLaw[1].M.Factor = new FACTOR[1];

 //RandValue
 TableCasualLaw[1].M.Factor[0].SourceDim = NONDIM;
 TableCasualLaw[1].M.Factor[0].TargetDim = NONDIM;
 TableCasualLaw[1].M.Factor[0].Name = new char[5];
 strcpy(TableCasualLaw[1].M.Factor[0].Name,"Rand");
 TableCasualLaw[1].M.Factor[0].Convert = Copy;
 TableCasualLaw[1].M.Factor[0].Invert = Copy;
 TableCasualLaw[1].M.Factor[0].PreBlock = AlwaysTrue;
 TableCasualLaw[1].M.Factor[0].PostBlock = AlwaysTrue;

 //*************************************************************
 //������ ����� - �������������� ����� � ��������� [A..B]
 //*************************************************************
 //{RanABLaw,RANDAB_LAW, InitFactors, GetMathNotateRandABLaw, NONDIM, 2}
 TableCasualLaw[2].turn = ON;
 TableCasualLaw[2].law = RandABLaw;
 TableCasualLaw[2].type = RANDAB_LAW;
 TableCasualLaw[2].inputfactor = InitFactors;
 TableCasualLaw[2].mathlawfactor = GetMathNotateRandABLaw;
 TableCasualLaw[2].M.DimFunc = NONDIM;
 TableCasualLaw[2].M.Size = 3;
 TableCasualLaw[2].M.Factor = new FACTOR[3];

 //RandValue
 TableCasualLaw[2].M.Factor[0].SourceDim = NONDIM;
 TableCasualLaw[2].M.Factor[0].TargetDim = NONDIM;
 TableCasualLaw[2].M.Factor[0].Name = new char[5];
 strcpy(TableCasualLaw[2].M.Factor[0].Name,"Rand");
 TableCasualLaw[2].M.Factor[0].Convert = Copy;
 TableCasualLaw[2].M.Factor[0].Invert = Copy;
 TableCasualLaw[2].M.Factor[0].PreBlock = AlwaysTrue;
 TableCasualLaw[2].M.Factor[0].PostBlock = AlwaysTrue;

 //A  {NONDIM,NONDIM,"Lower border A",Copy,Copy,AlwaysTrue,AlwaysTrue}
 TableCasualLaw[2].M.Factor[1].SourceDim = NONDIM;
 TableCasualLaw[2].M.Factor[1].TargetDim = NONDIM;
 TableCasualLaw[2].M.Factor[1].Name = new char[15];
 strcpy(TableCasualLaw[2].M.Factor[1].Name,"Lower border A");
 TableCasualLaw[2].M.Factor[1].Convert = Copy;
 TableCasualLaw[2].M.Factor[1].Invert = Copy;
 TableCasualLaw[2].M.Factor[1].PreBlock = AlwaysTrue;
 TableCasualLaw[2].M.Factor[1].PostBlock = AlwaysTrue;

 //B  {NONDIM,NONDIM,"Upper border B",Copy,Copy,AlwaysTrue,AlwaysTrue}
 TableCasualLaw[2].M.Factor[2].SourceDim = NONDIM;
 TableCasualLaw[2].M.Factor[2].TargetDim = NONDIM;
 TableCasualLaw[2].M.Factor[2].Name = new char[15];
 strcpy(TableCasualLaw[2].M.Factor[2].Name,"Upper border B");
 TableCasualLaw[2].M.Factor[2].Convert = Copy;
 TableCasualLaw[2].M.Factor[2].Invert = Copy;
 TableCasualLaw[2].M.Factor[2].PreBlock = AlwaysTrue;
 TableCasualLaw[2].M.Factor[2].PostBlock = AlwaysTrue;

 //*************************************************************
 //��������� ����� - ��������� ����� ����� [0..+INFINITE]
 //*************************************************************
 //{RelayLaw,RELAY_LAW, InitFactors, GetMathNotateRandLaw, NONDIM, 0}
 TableCasualLaw[3].turn = ON;
 TableCasualLaw[3].law = RelayLaw;
 TableCasualLaw[3].type = RELAY_LAW;
 TableCasualLaw[3].inputfactor = InitFactors;
 TableCasualLaw[3].mathlawfactor = GetMathNotateRelayLaw;
 TableCasualLaw[3].M.DimFunc = NONDIM;
 TableCasualLaw[3].M.Size = 1;
 TableCasualLaw[3].M.Factor = new FACTOR[1];

 //RandValue
 TableCasualLaw[3].M.Factor[0].SourceDim = NONDIM;
 TableCasualLaw[3].M.Factor[0].TargetDim = NONDIM;
 TableCasualLaw[3].M.Factor[0].Name = new char[5];
 strcpy(TableCasualLaw[3].M.Factor[0].Name,"Rand");
 TableCasualLaw[3].M.Factor[0].Convert = Copy;
 TableCasualLaw[3].M.Factor[0].Invert = Copy;
 TableCasualLaw[3].M.Factor[0].PreBlock = AlwaysTrue;
 TableCasualLaw[3].M.Factor[0].PostBlock = AlwaysTrue;

 //*************************************************************
 //����� ����� - ����� ���������� �� �� ������������
 //*************************************************************
 //{Det1Law, DET1_LAW, InitFactors, GetMathNotateDet1Law, NONDIM, 0}
 TableCasualLaw[4].turn = ON;
 TableCasualLaw[4].law = Det1Law;
 TableCasualLaw[4].type = DET1_LAW;
 TableCasualLaw[4].inputfactor = InitFactors;
 TableCasualLaw[4].mathlawfactor = GetMathNotateDet1Law;
 TableCasualLaw[4].M.DimFunc = NONDIM;
 TableCasualLaw[4].M.Size = 1;
 TableCasualLaw[4].M.Factor = new FACTOR[1];

 //RandValue
 TableCasualLaw[4].M.Factor[0].SourceDim = NONDIM;
 TableCasualLaw[4].M.Factor[0].TargetDim = NONDIM;
 TableCasualLaw[4].M.Factor[0].Name = new char[5];
 strcpy(TableCasualLaw[4].M.Factor[0].Name,"Rand");
 TableCasualLaw[4].M.Factor[0].Convert = Copy;
 TableCasualLaw[4].M.Factor[0].Invert = Copy;
 TableCasualLaw[4].M.Factor[0].PreBlock = AlwaysTrue;
 TableCasualLaw[4].M.Factor[0].PostBlock = AlwaysTrue;

 //*************************************************************
 //������ ����� - ����� ���������� �� �� �����
 //*************************************************************
 //{Det0Law, DET0_LAW, InitFactors, GetMathNotateDet0Law, NONDIM, 0}
 TableCasualLaw[5].turn = ON;
 TableCasualLaw[5].law = Det0Law;
 TableCasualLaw[5].type = DET0_LAW;
 TableCasualLaw[5].inputfactor = InitFactors;
 TableCasualLaw[5].mathlawfactor = GetMathNotateDet0Law;
 TableCasualLaw[5].M.DimFunc = NONDIM;
 TableCasualLaw[5].M.Size = 1;
 TableCasualLaw[5].M.Factor = new FACTOR[1];

 //RandValue
 TableCasualLaw[5].M.Factor[0].SourceDim = NONDIM;
 TableCasualLaw[5].M.Factor[0].TargetDim = NONDIM;
 TableCasualLaw[5].M.Factor[0].Name = new char[5];
 strcpy(TableCasualLaw[5].M.Factor[0].Name,"Rand");
 TableCasualLaw[5].M.Factor[0].Convert = Copy;
 TableCasualLaw[5].M.Factor[0].Invert = Copy;
 TableCasualLaw[5].M.Factor[0].PreBlock = AlwaysTrue;
 TableCasualLaw[5].M.Factor[0].PostBlock = AlwaysTrue;

}
//*****************************************************************************
//������: II.9., private, class member-function
//������������ �������: void DeleteTable(LAW* Table, uint Size)
//����������:
//�������� ������� ������� ��������� ������� �� �������.
//��� ������ ������� ���������� ���������� ��������� Law* Table � ������ �������
//Size
//-----------------------------------------------------------------------------
//������������ ���������:
//1. LAW* Table - ��������� �� ��������� �������
//2. uint Size - ������ ��������� �������
//-----------------------------------------------------------------------------
//������������ ��������:  ���
//*****************************************************************************
void ExtSignal::DeleteTable(LAW* Table, uint& Size)
{
 uint Param;
 if ((Table == NULL) || (Size == 0)) return;
 for (uint i = 0; i < Size; i++)
 {
  Param = Table[i].M.Size;
  for (uint j = 0; j < Param; j++)
   if (Table[i].M.Factor[j].Name != NULL) delete Table[i].M.Factor[j].Name;
  delete[] Table[i].M.Factor;
 }
 delete[] Table;
 Size = 0; 
}

//=============================================================================
//                    �������� ������� - ������ ������ ExtSignal
//=============================================================================

//**********************************************
// I. �������� ������� - ����� ������ ExtSignal
//    PRIVATE MEMBER - FUNCTION
//**********************************************

//*****************************************************************************
//������: I.1., private, class member-function
//������������ �������: bool CreateArray(const uint size)
//����������:
//������� ������������� ��� ��������� ������ ��� ���������� ��������� ��������
//�������. ����������� ������ �������� �������� ����������� ������� ����
//COMPONENT �������� size. � ������ ������ ������� ��� ������������� ���������
//������ ��������� ������� ���������� �������������� �������� ������ �������� ��
//������ ������������� ������ � �� ���� ��������� ��������� ���� ������.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. const uint size - ���������� ����������� �� ������� �������
//-----------------------------------------------------------------------------
//������������ ��������: bool
//1. true - ������ ������ ��������� �������
//2. false - ��������� ������ ������ �������� �� �������
//*****************************************************************************
bool ExtSignal::CreateArray(const uint size)
{
 bool lSuccess = true;
 try //������������� ������ ��� �������� �������
  {   //� �������������� �� �������������
  	data = new COMPONENT[size];
   for(uint i = 0; i < Count; i++)
   { data[i].AzAngleArg = NULL;
     data[i].ElevAngleArg = NULL;
     data[i].PolAngleArg = NULL;
     data[i].AmplArg = NULL;
     data[i].AmplCasualLawArg = NULL;
     data[i].VertPolPhaseArg = NULL;
     data[i].HorPolPhaseArg = NULL;
     data[i].PhaseArg = NULL;
     data[i].PhaseCasualLawArg = NULL;
   }
  }
  // ENTER THIS BLOCK ONLY IF xalloc IS THROWN.
  // YOU COULD REQUEST OTHER ACTIONS BEFORE TERMINATING
  catch (xalloc x)
  {
    lSuccess = false;
    #if defined(CONSOLE)
      cout << "\nMEMORY ALLOCATION ERROR !!!" << endl;
      cout << "Could not allocate all components external signal in memory."
           << endl;
      cout << "Memory request is " << x.requested() << " byte." << endl;
      getch();
    #endif
    exit(-1);
  }
return lSuccess;
}
//*****************************************************************************
//������: I.2., private, class member-function
//������������ �������: void ResetProperty(uint index)
//����������:
//������� ������������� ��� ������������� ���� ������������� ���������� ��������
//������� � ���������� ������� index. ��� �������� ������ 0 ������� data ������
//�������� �������� ������������ �������� �������.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. const uint index - ���������� ����� ���������� �������� ������� � �������
//                      data
//-----------------------------------------------------------------------------
//������������ ��������: ���
//*****************************************************************************
void ExtSignal::ResetProperty(uint index)
{
  //����� �������������� ������������
  data[index].Type = (index == 0) ? SIGNAL : JAMMER;
  data[index].Usage = OFF;

  //������������ ���� ��������� �������
  data[index].AzAngleLawType = ZERO_LAW;
  if (data[index].AzAngleArg != NULL)
   data[index].AzAngleArg = DeleteArgList(data[index].AzAngleArg);
  data[index].AzimuthAngle = ZeroLaw;

  //���� ����� ��������� �������
  data[index].ElevAngleLawType = ZERO_LAW;
  if (data[index].ElevAngleArg != NULL)
   data[index].ElevAngleArg = DeleteArgList(data[index].ElevAngleArg);
  data[index].ElevateAngle = ZeroLaw;

  //���� ����������� �������
  data[index].PolAngleLawType = ZERO_LAW;
  if (data[index].PolAngleArg != NULL)
   data[index].PolAngleArg = DeleteArgList(data[index].PolAngleArg);
  data[index].PolAngle = ZeroLaw;

  //����������� �������������� ������������ �������
  //����������������� ����� ����������� ��������� ������� �� �������
  data[index].AmplLawType = ZERO_LAW;
  if (data[index].AmplArg != NULL)
   data[index].AmplArg = DeleteArgList(data[index].AmplArg);
  data[index].AmplLaw = ZeroLaw;

  //��������� ����� ��������� ��������� �� �������
  data[index].AmplCasualLawType = ZERO_LAW;
  if (data[index].AmplCasualLawArg != NULL)
   data[index].AmplCasualLawArg = DeleteArgList(data[index].AmplCasualLawArg);
  data[index].AmplCasualLaw = ZeroLaw;

  //������� �������������� ������������ �������
  //������� ������� ������� (�������������� ��������)
  data[index].CarPulsatance  = 0.0; //(in rad/sec)

  //������� ����� ��������� ������������ ������������ ���������� ��������
  //������� �� �������
  data[index].VertPolPhaseLawType = ZERO_LAW;
  if (data[index].VertPolPhaseArg != NULL)
   data[index].VertPolPhaseArg = DeleteArgList(data[index].VertPolPhaseArg);
  data[index].VertPolPhase = ZeroLaw;

  //������� ����� ��������� �������������� ������������ ���������� ��������
  //������� �� �������
  data[index].HorPolPhaseLawType = ZERO_LAW;
  if (data[index].HorPolPhaseArg != NULL)
   data[index].HorPolPhaseArg = DeleteArgList(data[index].HorPolPhaseArg);
  data[index].HorPolPhase = ZeroLaw;


  //����������������� ������� ����� ��������� ������������ ������� �� �������
  data[index].PhaseLawType = ZERO_LAW;
  if (data[index].PhaseArg != NULL)
   data[index].PhaseArg = DeleteArgList(data[index].PhaseArg);
  data[index].PhaseLaw = ZeroLaw;

  //��������� ������� ����� ��������� ������������ ������� �� �������
  data[index].PhaseCasualLawType = ZERO_LAW;
  if (data[index].PhaseCasualLawArg != NULL)
   data[index].PhaseCasualLawArg = DeleteArgList(data[index].PhaseCasualLawArg);
  data[index].PhaseCasualLaw = ZeroLaw;
}
//*****************************************************************************
//������: I.2., private, class member-function
//������������ �������: void ConsoleDataEntry(uint index)
//����������:
//������� ������������� ��� ����� ������ �� ���� ��������������� ����������
//�������� ������� � ���������� ������� index. ��� �������� ������ 0 �������
//data ������ �������� �������� ������������ �������� �������.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. const uint index - ���������� ����� ���������� �������� ������� � �������
// data
//-----------------------------------------------------------------------------
//������������ ��������: ���
//*****************************************************************************
void ExtSignal::ConsoleDataEntry(uint index)
{ uint X = 1;
  uint Y = wherey();
  uint CurX, CurY;
  //������������� �� ���������
  data[index].Type = (index == 0) ? SIGNAL : JAMMER;
  data[index].Usage = ON;
  //���������� ���������� �� �������� ���������� �������� �������
  cout << "INPUT DATA by: ";
  if (index == 0) cout << "DESIRED SIGNAL ";
  else cout << "JAMMER N " << index;
  cout << " PROPERTY: ";
  CurX = wherex();

  //���� ���������� �� ������������� ���� ��������� �������
  cout << "1. Azimuth Angle of Signal Source" << endl;
  AzimuthAngleInPut(index,ENG);

  //���� ���������� �� ���� ����� ��������� �������
  gotoxy(CurX,Y);
  clreol();
  cout << "2. Elevate Angle of Signal Source" << endl;
  ElevateAngleInPut(index,ENG);

  //*************************************************************************
  //����������� ������������ �������
  //�������� � ���� ��������������� (I) � ���������������� (II) ����������
  //1. ��������������� ��������� ������� - PolAngle
  //2. ���������������� ��������� ������� - AmplLaw
  //3. ��������� ����� ��������� ��������� �� ������� - AmplCasualLaw
  //1-3 - �������� ��������� (������-������) ����������� ������������ �������
  //*************************************************************************

  //���� ���������� �� ���� ����������� �������
  gotoxy(CurX,Y);
  clreol();
  cout << "3. Signal Polarization Amplitude" << endl;
  ResetLaw(TableAngleLaw,TableAngleLawCount,SQUARE_LAW);
  PolAngleInPut(index,ENG);
  SetAllLaw(TableAngleLaw,TableAngleLawCount);

  //���� ���������� �� ������ ��������� ��������� �������
  gotoxy(CurX,Y);
  clreol();
  cout << "4. Signal Space Amplitude" << endl;
  AmplLawInPut(index,ENG);

  //����� ���������� ������ ��������� ��������� �������
  gotoxy(CurX,Y);
  clreol();
  cout << "5. Amplitude Casual Law" << endl;
  ResetLaw(TableCasualLaw,TableCasualLawCount,DET0_LAW);
  AmplCasualLawInPut(index,ENG);
  SetLaw(TableCasualLaw,TableCasualLawCount,DET0_LAW);

  //****************************************************************************
  //������� ������������ �������
  //�������� � ���� ��������������� (I) � ���������������� (II) ����������
  //1. ���� ������������ ������������ ����������� ������� - VertPolPhaseLaw
  //2. ���� �������������� ������������ ����������� ������� - HorPolPhaseLaw
  //3. ���������������� ���� ������� - PhaseLaw
  //4. ��������� ����� ��������� ���� �� ������� - PhaseCasualLaw
  //5. ������� ������� ������� (�� ����������� � ��������) - Carrier Pulsatance
  //1-4 - ���������� ���� (����) ����������� ��������� �������
  //****************************************************************************

  //����� ������ ��������� ���� ������������ ������������ ����������� �������
  gotoxy(CurX,Y);
  clreol();
  cout << "6. Vert. Polarization Phase of Signal" << endl;
  ResetLaw(TableAngleLaw,TableAngleLawCount,SQUARE_LAW);
  VertPhasePolInPut(index,ENG);
  SetLaw(TableAngleLaw,TableAngleLawCount,SQUARE_LAW);

  //����� ������ ��������� ���� �������������� ������������ ����������� �������
  gotoxy(CurX,Y);
  clreol();
  cout << "7. Hor. Polarization Phase of Signal" << endl;
  ResetLaw(TableAngleLaw,TableAngleLawCount,SQUARE_LAW);
  HorPhasePolInPut(index,ENG);
  SetLaw(TableAngleLaw,TableAngleLawCount,SQUARE_LAW);

  //����� ������ ��������� ���������������� ���� �������
  gotoxy(CurX,Y);
  clreol();
  cout << "8. Space Phase of Signal" << endl;
  ResetLaw(TableAngleLaw,TableAngleLawCount,SQUARE_LAW);
  PhaseLawInPut(index,ENG);
  SetLaw(TableAngleLaw,TableAngleLawCount,SQUARE_LAW);

  //����� ���������� ������ ��������� ���� �������
  gotoxy(CurX,Y);
  clreol();
  cout << "9. Phase Casual Law" << endl;
  ResetLaw(TableCasualLaw,TableCasualLawCount,DET1_LAW);
  PhaseCasualLawInPut(index,ENG);
  SetLaw(TableCasualLaw,TableCasualLawCount,DET1_LAW);

  //������� ������
  CurY = wherey();
  gotoxy(X,Y);
  for(uint i = Y; i <= CurY; i++) {clreol(); cout << endl;}
  gotoxy(X,Y);

}

//*****************************************************************************
//������: I.3., private, class member-function
//������������ �������: void AzimuthAngleInPut(uint index,uint language);
//����������:
//������� ������������� ��� ����� ������ �� ������������ ���� ���������� ��������
//������� � ���������� ������� index. ��� �������� ������ 0 ������� data ������
//�������� �������� ������������ �������� �������.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. const uint index - ���������� ����� ���������� �������� ������� � �������
//data
//2. uint language - ���� ����������
//-----------------------------------------------------------------------------
//������������ ��������: ���
//*****************************************************************************
void ExtSignal::AzimuthAngleInPut(uint index,uint language)
{ uint X = 1;
  uint Y = wherey();
  uint CurX,CurY;
  uint Select;
  char c;
  char* str;
  bool lBreak;
  //����� ������ ������� �� ������� ����� ���������� �� ������� ���� �� �������
  //��������� ������������ �������� �������
  uint Count = AccessibleLaw(TableAngleLaw,TableAngleLawCount);
  uint Index;
  gotoxy(X,Y);
 do
  {
    cout << "*------------------------------------------------------------*\n";
    cout << "*            SELECT AZIMUTH ANGLE MODIFICATION LAW           *\n";
    cout << "*------------------------------------------------------------*\n";
    for(uint i = 0; i<Count; i++)
    { Index = GetIndexLaw(TableAngleLaw, TableAngleLawCount, i);
      cout << (i+1) << ". ";
      cout << GetLawName(TableAngleLaw[Index].type,language);
      cout << "\t: " << GetLawNotate(TableAngleLaw[Index].type) << endl;
    }
    cout << "*------------------------------------------------------------*\n";
   do
    { do
       { CurX = wherex(); CurY = wherey();
         cout << "Enter number [1 - " << Count << "]: ";
         cin >> Select;
         gotoxy(CurX,CurY); clreol();
       } while((Select > Count) || (Select < 1));
      Select--;
      Select = GetIndexLaw(TableAngleLaw, TableAngleLawCount, Select);
      cout << "Have You selected <";
      cout << GetLawName(TableAngleLaw[Select].type,language);
      cout << ": " << GetLawNotate(TableAngleLaw[Select].type) << "> ? [Y/N]: ";
      cin >> c;
      if((c == 'Y') || (c == 'y')) lBreak = true;
      else lBreak = false;
      gotoxy(CurX,CurY); clreol();
    } while(!lBreak);

    data[index].AzAngleLawType = TableAngleLaw[Select].type;
    data[index].AzimuthAngle = TableAngleLaw[Select].law;

    //���� ���������� ������ ��������� ������������� ���� ������������
    //�������� �������
    const char* dimstr = GetMeasureName(TableAngleLaw[Select].M.DimFunc);
    gotoxy(X,Y);
    for(uint i = Y; i<=CurY; i++) { clreol(); cout << endl; }
    gotoxy(X,Y);
    cout << "AZIMUTH ANGLE MODIFICATION LAW: "
         << GetLawNotate(TableAngleLaw[Select].type) << " [" << dimstr
         << "]" << endl;

    //���������� ����������� ������, ��������� � ���������� �������, ���������-
    //������ ��������� ������������� ���� ��������� ������� �� �������

    data[index].AzAngleArg = TableAngleLaw[Select].inputfactor(TableAngleLaw[Select].M);

    gotoxy(X,Y);
    clreol();
    str = TableAngleLaw[Select].mathlawfactor(data[index].AzAngleArg);
    cout << "AZIMUTH ANGLE LAW: " << str << " [" << dimstr << "]" << endl;
    delete[] str;
    cout << "Have You accept this law? [Y/N]: ";
    cin >> c;
    CurY = wherey();
    if((c == 'Y') || (c == 'y')) lBreak = true;
    else lBreak = false;
    gotoxy(X,Y);
    for(uint i = Y; i<=CurY; i++) { clreol(); cout << endl; }
    gotoxy(X,Y);
  } while(!lBreak);
}
//*****************************************************************************
//������: I.4., private, class member-function
//������������ �������: ElevateAngleInPut(uint index,uint language);
//����������:
//������� ������������� ��� ����� ������ �� ���� ����� ���������� ��������
//������� � ���������� ������� index. ��� �������� ������ 0 ������� data ������
//�������� �������� ������������ �������� �������.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. const uint index - ���������� ����� ���������� �������� ������� � �������
//   data
//2. uint language - ���� ����������
//-----------------------------------------------------------------------------
//������������ ��������: ���
//*****************************************************************************
void ExtSignal::ElevateAngleInPut(uint index,uint language)
{ uint X = 1;
  uint Y = wherey();
  uint CurX,CurY;
  uint Select;
  char c;
  char* str;
  bool lBreak;
  //����� ������ ������� �� ������� ����� ���������� �� ������� ���� �����
  //��������� ������������ �������� �������
  uint Count = AccessibleLaw(TableAngleLaw,TableAngleLawCount);
  uint Index;
  gotoxy(X,Y);
 do
  {
    cout << "*------------------------------------------------------------*\n";
    cout << "*            SELECT ELEVATE ANGLE MODIFICATION LAW           *\n";
    cout << "*------------------------------------------------------------*\n";
    for(uint i = 0; i<Count; i++)
    { Index = GetIndexLaw(TableAngleLaw, TableAngleLawCount, i);
      cout << (i+1) << ". ";
      cout << GetLawName(TableAngleLaw[Index].type,language);
      cout << "\t: " << GetLawNotate(TableAngleLaw[Index].type) << endl;
    }
    cout << "*------------------------------------------------------------*\n";
   do
    { do
       { CurX = wherex(); CurY = wherey();
         cout << "Enter number [1 - " << Count << "]: ";
         cin >> Select;
         gotoxy(CurX,CurY); clreol();
       } while((Select > Count) || (Select < 1));
      Select--;
      Select = GetIndexLaw(TableAngleLaw, TableAngleLawCount, Select);
      cout << "Have You selected <";
      cout << GetLawName(TableAngleLaw[Select].type,language);
      cout << ": " << GetLawNotate(TableAngleLaw[Select].type) << "> ? [Y/N]: ";
      cin >> c;
      if((c == 'Y') || (c == 'y')) lBreak = true;
      else lBreak = false;
      gotoxy(CurX,CurY); clreol();
    } while(!lBreak);

    data[index].ElevAngleLawType = TableAngleLaw[Select].type;
    data[index].ElevateAngle = TableAngleLaw[Select].law;

    //������������� ���������� ������ ��������� ���� ����� ������������
    //�������� �������
    const char* dimstr = GetMeasureName(TableAngleLaw[Select].M.DimFunc);
    gotoxy(X,Y);
    for(uint i = Y; i<=CurY; i++) { clreol(); cout << endl; }
    gotoxy(X,Y);
    cout << "ELEVATE ANGLE MODIFICATION LAW: "
         << GetLawNotate(TableAngleLaw[Select].type) << " [" << dimstr
         << "]" << endl;

    //���������� ����������� ������, ��������� � ���������� �������, ���������-
    //������ ��������� ���� ����� ��������� ������� �� �������

    data[index].ElevAngleArg = TableAngleLaw[Select].inputfactor(TableAngleLaw[Select].M);

    gotoxy(X,Y);
    clreol();
    str = TableAngleLaw[Select].mathlawfactor(data[index].ElevAngleArg);
    cout << "ELEVATE ANGLE LAW: " << str << " [" << dimstr << "]" << endl;
    delete[] str;
    cout << "Have You accept this law? [Y/N]: ";
    cin >> c;
    CurY = wherey();
    if((c == 'Y') || (c == 'y')) lBreak = true;
    else lBreak = false;
    gotoxy(X,Y);
    for(uint i = Y; i<=CurY; i++) { clreol(); cout << endl; }
    gotoxy(X,Y);
  } while(!lBreak);
}
//*****************************************************************************
//������: I.5., private, class member-function
//������������ �������: void PolAngleInPut(uint index, uint language)
//����������:
//������� ������������� ��� ����� ������ �� ��������� ����������� ����������
//�������� ������� � ���������� ������� index.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. const uint index - ���������� ����� ���������� �������� ������� � �������
//data
//2. uint language - ���� ����������
//-----------------------------------------------------------------------------
//������������ ��������: ���
//*****************************************************************************
void ExtSignal::PolAngleInPut(uint index,uint language)
{ uint X = 1;
  uint Y = wherey();
  uint CurX,CurY;
  uint Select;
  char c;
  char* str;
  bool lBreak;
  //����� ������ ������� �� ������� ����� ���������� �� ������� ���������
  //����������� ������������ �������� �������
  uint Count = AccessibleLaw(TableAngleLaw,TableAngleLawCount);
  uint Index;
  gotoxy(X,Y);
 do
  {
    cout << "*------------------------------------------------------------*\n";
    cout << "*       SELECT POLARIZATION AMPLITUDE MODIFICATION LAW       *\n";
    cout << "*------------------------------------------------------------*\n";
    for(uint i = 0; i<Count; i++)
    { Index = GetIndexLaw(TableAngleLaw, TableAngleLawCount, i);
      cout << (i+1) << ". ";
      cout << GetLawName(TableAngleLaw[Index].type,language);
      cout << "\t: " << GetLawNotate(TableAngleLaw[Index].type) << endl;
    }
    cout << "*------------------------------------------------------------*\n";
   do
    { do
       { CurX = wherex(); CurY = wherey();
         cout << "Enter number [1 - " << Count << "]: ";
         cin >> Select;
         gotoxy(CurX,CurY); clreol();
       } while((Select > Count) || (Select < 1));
      Select--;
      Select = GetIndexLaw(TableAngleLaw, TableAngleLawCount, Select);
      cout << "Have You selected <";
      cout << GetLawName(TableAngleLaw[Select].type,language);
      cout << ": " << GetLawNotate(TableAngleLaw[Select].type) << "> ? [Y/N]: ";
      cin >> c;
      if((c == 'Y') || (c == 'y')) lBreak = true;
      else lBreak = false;
      gotoxy(CurX,CurY); clreol();
    } while(!lBreak);

    data[index].PolAngleLawType = TableAngleLaw[Select].type;
    data[index].PolAngle = TableAngleLaw[Select].law;

    //������������� ���������� ������ ��������� ��������� �����������
    //������������ �������� �������
    const char* dimstr = GetMeasureName(TableAngleLaw[Select].M.DimFunc);
    gotoxy(X,Y);
    for(uint i = Y; i<=CurY; i++) { clreol(); cout << endl; }
    gotoxy(X,Y);
    cout << "LAW of POLARIZATION AMPLITUDE: "
         << GetLawNotate(TableAngleLaw[Select].type) << " [" << dimstr
         << "]" << endl;

    //���������� ����������� ������, ��������� � ���������� �������, ���������-
    //������ ��������� ��������� ����������� ������� �� �������

    data[index].PolAngleArg = TableAngleLaw[Select].inputfactor(TableAngleLaw[Select].M);

    gotoxy(X,Y);
    clreol();
    str = TableAngleLaw[Select].mathlawfactor(data[index].PolAngleArg);
    cout << "LAW of AMPLITUDE POLARIZATION: " << str << " [" << dimstr << "]"
         << endl;
    delete[] str;
    cout << "Have You accept this law? [Y/N]: ";
    cin >> c;
    CurY = wherey();
    if((c == 'Y') || (c == 'y')) lBreak = true;
    else lBreak = false;
    gotoxy(X,Y);
    for(uint i = Y; i<=CurY; i++) { clreol(); cout << endl; }
    gotoxy(X,Y);
  } while(!lBreak);
}

//*****************************************************************************
//������: I.6., private, class member-function
//������������ �������: void AmplLawInPut(uint index,uint language)
//����������:
//������� ������������� ��� ������ ������ ��������� ��������� ���������� ��������
//������� � ���������� ������� index. ��� �������� ������ 0 ������� data ������
//�������� �������� ������������ �������� �������.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. const uint index - ���������� ����� ���������� �������� ������� � �������
//2. uint language - ���� ������ �������������� ���������
//-----------------------------------------------------------------------------
//������������ ��������: ���
//*****************************************************************************
void ExtSignal::AmplLawInPut(uint index,uint language)
{ uint X = 1;
  uint Y = wherey();
  uint CurX,CurY;
  uint Select;
  char c;
  char* str;
  bool lBreak;
  //����� ������ ����������������� ������� �� ������� ����� ���������� ��
  //������� ��������� ������������ �������� �������
  uint Count = AccessibleLaw(TableAmplLaw,TableAmplLawCount);
  uint Index;
  gotoxy(X,Y);
 do
  {
    cout << "*------------------------------------------------------------*\n";
    cout << "*          SELECT AMPLITUDE MODIFICATION DETERMINE LAW       *\n";
    cout << "*------------------------------------------------------------*\n";
    for(uint i = 0; i<Count; i++)
    { Index = GetIndexLaw(TableAmplLaw, TableAmplLawCount, i);
      cout << (i+1) << ". ";
      cout << GetLawName(TableAmplLaw[Index].type,language);
      cout << "\t: " << GetLawNotate(TableAmplLaw[Index].type) << endl;
    }
    cout << "*------------------------------------------------------------*\n";
   do
    { do
       { CurX = wherex(); CurY = wherey();
         cout << "Enter number [1 - " << Count << "]: ";
         cin >> Select;
         gotoxy(CurX,CurY); clreol();
       } while((Select > Count) || (Select < 1));
      Select--;
      Select = GetIndexLaw(TableAmplLaw, TableAmplLawCount, Select);
      cout << "Have You selected <";
      cout << GetLawName(TableAmplLaw[Select].type,language);
      cout << ": " << GetLawNotate(TableAmplLaw[Select].type) << "> ? [Y/N]: ";
      cin >> c;
      if((c == 'Y') || (c == 'y')) lBreak = true;
      else lBreak = false;
      gotoxy(CurX,CurY); clreol();
    } while(!lBreak);

    data[index].AmplLawType = TableAmplLaw[Select].type;
    data[index].AmplLaw = TableAmplLaw[Select].law;

    //������������� ���������� ������ ��������� ��������� ������������
    //�������� �������
    const char* dimstr = GetMeasureName(TableAmplLaw[Select].M.DimFunc);
    gotoxy(X,Y);
    for(uint i = Y; i<=CurY; i++) { clreol(); cout << endl; }
    gotoxy(X,Y);
    cout << "AMPLITUDE DETERMINE LAW: "
         << GetLawNotate(TableAmplLaw[Select].type) << " [" << dimstr
         << "]" << endl;

    //���������� ����������� ������, ��������� � ���������� �������, ���������-
    //������ ��������� ��������� ��������� ������� �� �������

    data[index].AmplArg = TableAmplLaw[Select].inputfactor(TableAmplLaw[Select].M);

    gotoxy(X,Y);
    clreol();
    str = TableAmplLaw[Select].mathlawfactor(data[index].AmplArg);
    cout << "AMPLITUDE DETERMINE LAW: " << str << " [" << dimstr << "]" << endl;
    delete[] str;
    cout << "Have You accept this law? [Y/N]: ";
    cin >> c;
    CurY = wherey();
    if((c == 'Y') || (c == 'y')) lBreak = true;
    else lBreak = false;
    gotoxy(X,Y);
    for(uint i = Y; i<=CurY; i++) { clreol(); cout << endl; }
    gotoxy(X,Y);
  } while(!lBreak);
}
//*****************************************************************************
//������: I.6., private, class member-function
//������������ �������: void AmplCasualLawInPut(uint index,uint language)
//����������:
//������� ������������� ��� ������ ���������� ������ ��������� ���������
//���������� �������� ������� � ���������� ������� index. �������� ����������
//������� ��������� �������� �� ��������� �������. � ���� ������ ����������
//������� ����� ���� - "Casuality is absent: F(t) = 1"
//-----------------------------------------------------------------------------
//������������ ���������:
//1. const uint index - ���������� ����� ���������� �������� ������� � �������
//2. uint language - ���� ������ �������������� ���������
//-----------------------------------------------------------------------------
//������������ ��������: ���
//*****************************************************************************
void ExtSignal::AmplCasualLawInPut(uint index,uint language)
{ uint X = 1;
  uint Y = wherey();
  uint CurX,CurY;
  uint Select;
  char c;
  char* str;
  bool lBreak;
  //����� ������ ��������� ������� �� ������� ����� ���������� ��
  //������� ��������� ������������ �������� �������
  uint Count = AccessibleLaw(TableCasualLaw,TableCasualLawCount);
  uint Index;
  gotoxy(X,Y);
 do
  {
    cout << "*------------------------------------------------------------*\n";
    cout << "*          SELECT AMPLITUDE MODIFICATION CASUAL LAW          *\n";
    cout << "*------------------------------------------------------------*\n";
    for(uint i = 0; i<Count; i++)
    { Index = GetIndexLaw(TableCasualLaw, TableCasualLawCount, i);
      cout << (i+1) << ". ";
      cout << GetLawName(TableCasualLaw[Index].type,language);
      cout << "\t: " << GetLawNotate(TableCasualLaw[Index].type) << endl;
    }
    cout << "*------------------------------------------------------------*\n";
   do
    { do
       { CurX = wherex(); CurY = wherey();
         cout << "Enter number [1 - " << Count << "]: ";
         cin >> Select;
         gotoxy(CurX,CurY); clreol();
       } while((Select > Count) || (Select < 1));
      Select--;
      Select = GetIndexLaw(TableCasualLaw, TableCasualLawCount, Select);
      cout << "Have You selected <";
      cout << GetLawName(TableCasualLaw[Select].type,language);
      cout << ": " << GetLawNotate(TableCasualLaw[Select].type) << "> ? [Y/N]: ";
      cin >> c;
      if((c == 'Y') || (c == 'y')) lBreak = true;
      else lBreak = false;
      gotoxy(CurX,CurY); clreol();
    } while(!lBreak);

    data[index].AmplCasualLawType = TableCasualLaw[Select].type;
    data[index].AmplCasualLaw = TableCasualLaw[Select].law;

    //������������� ���������� ���������� ������ ��������� ���������
    //�������� �������
    gotoxy(X,Y);
    for(uint i = Y; i<=CurY; i++) { clreol(); cout << endl; }
    gotoxy(X,Y);
    cout << "AMPLITUDE CASUAL LAW: "
         << GetLawNotate(TableCasualLaw[Select].type) << endl;

    //���������� ��������� (��� �������������) ���������� ������, ������������-
    //��� ��������� ��������� ������� �� �������
    data[index].AmplCasualLawArg =
    TableCasualLaw[Select].inputfactor(TableCasualLaw[Select].M);

    gotoxy(X,Y);
    clreol();
    str = TableCasualLaw[Select].mathlawfactor(data[index].AmplCasualLawArg);
    cout << "AMPLITUDE CASUAL LAW: " << str << endl;
    delete[] str;
    cout << "Have You accept this law? [Y/N]: ";
    cin >> c;
    CurY = wherey();
    if((c == 'Y') || (c == 'y')) lBreak = true;
    else lBreak = false;
    gotoxy(X,Y);
    for(uint i = Y; i<=CurY; i++) { clreol(); cout << endl; }
    gotoxy(X,Y);
  } while(!lBreak);
}
//*****************************************************************************
//������: I.7., private, class member-function
//������������ �������: void VertPhasePolInPut(uint index,uint language)
//����������:
//������� ������������� ��� ����� ������ �� ���� ������������ ������������
//����������� ���������� �������� ������� � ���������� ������� index.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. const uint index - ���������� ����� ���������� �������� ������� � �������
//data
//2. uint language - ���� ����������
//-----------------------------------------------------------------------------
//������������ ��������: ���
//*****************************************************************************
void ExtSignal::VertPhasePolInPut(uint index,uint language)
{ uint X = 1;
  uint Y = wherey();
  uint CurX,CurY;
  uint Select;
  char c;
  char* str;
  bool lBreak;
  //����� ������ ������� �� ������� ����� ���������� �� ������� ���� ������������
  //������������ ����������� ���������� �������� �������
  uint Count = AccessibleLaw(TableAngleLaw,TableAngleLawCount);
  uint Index;
  gotoxy(X,Y);
 do
  {
    cout << "*------------------------------------------------------------*\n";
    cout << "*   SELECT MODIFICATION LAW OF VERTICAL POLARIZATION PHASE   *\n";
    cout << "*------------------------------------------------------------*\n";
    for(uint i = 0; i<Count; i++)
    { Index = GetIndexLaw(TableAngleLaw, TableAngleLawCount, i);
      cout << (i+1) << ". ";
      cout << GetLawName(TableAngleLaw[Index].type,language);
      cout << "\t: " << GetLawNotate(TableAngleLaw[Index].type) << endl;
    }
    cout << "*------------------------------------------------------------*\n";
   do
    { do
       { CurX = wherex(); CurY = wherey();
         cout << "Enter number [1 - " << Count << "]: ";
         cin >> Select;
         gotoxy(CurX,CurY); clreol();
       } while((Select > Count) || (Select < 1));
      Select--;
      Select = GetIndexLaw(TableAngleLaw, TableAngleLawCount, Select);
      cout << "Have You selected <";
      cout << GetLawName(TableAngleLaw[Select].type,language);
      cout << ": " << GetLawNotate(TableAngleLaw[Select].type) << "> ? [Y/N]: ";
      cin >> c;
      if((c == 'Y') || (c == 'y')) lBreak = true;
      else lBreak = false;
      gotoxy(CurX,CurY); clreol();
    } while(!lBreak);

    data[index].VertPolPhaseLawType = TableAngleLaw[Select].type;
    data[index].VertPolPhase = TableAngleLaw[Select].law;

    //���� ���������� ������ ��������� ���� ������������ ������������ �����������
    //�������� �������
    const char* dimstr = GetMeasureName(TableAngleLaw[Select].M.DimFunc);
    gotoxy(X,Y);
    for(uint i = Y; i<=CurY; i++) { clreol(); cout << endl; }
    gotoxy(X,Y);
    cout << "VERTICAL POLARIZATION PHASE LAW: "
         << GetLawNotate(TableAngleLaw[Select].type) << " [" << dimstr
         << "]" << endl;

    //���������� ����������� ������, ��������� � ���������� �������, ���������-
    //������ ��������� ���� ������������ ������������ ����������� �������
    //�� �������

    data[index].VertPolPhaseArg =
    TableAngleLaw[Select].inputfactor(TableAngleLaw[Select].M);

    gotoxy(X,Y);
    clreol();
    str = TableAngleLaw[Select].mathlawfactor(data[index].VertPolPhaseArg);
    cout << "VERT. POLAR. PHASE LAW: " << str << " [" << dimstr << "]" << endl;
    delete[] str;
    cout << "Have You accept this law? [Y/N]: ";
    cin >> c;
    CurY = wherey();
    if((c == 'Y') || (c == 'y')) lBreak = true;
    else lBreak = false;
    gotoxy(X,Y);
    for(uint i = Y; i<=CurY; i++) { clreol(); cout << endl; }
    gotoxy(X,Y);
  } while(!lBreak);
}
//*****************************************************************************
//������: I.8., private, class member-function
//������������ �������: void HorPhasePolInPut(uint index,uint language)
//����������:
//������� ������������� ��� ����� ������ �� ���� �������������� ������������
//����������� ���������� �������� ������� � ���������� ������� index.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. const uint index - ���������� ����� ���������� �������� ������� � �������
//data
//2. uint language - ���� ����������
//-----------------------------------------------------------------------------
//������������ ��������: ���
//*****************************************************************************
void ExtSignal::HorPhasePolInPut(uint index,uint language)
{ uint X = 1;
  uint Y = wherey();
  uint CurX,CurY;
  uint Select;
  char c;
  char* str;
  bool lBreak;
  //����� ������ ������� �� ������� ����� ���������� �� ������� ���� �����������-
  //��� ������������ ����������� ���������� �������� �������
  uint Count = AccessibleLaw(TableAngleLaw,TableAngleLawCount);
  uint Index;
  gotoxy(X,Y);
 do
  {
    cout << "*------------------------------------------------------------*\n";
    cout << "* SELECT MODIFICATION LAW OF HORIZONTAL POLARIZATION PHASE   *\n";
    cout << "*------------------------------------------------------------*\n";
    for(uint i = 0; i<Count; i++)
    { Index = GetIndexLaw(TableAngleLaw, TableAngleLawCount, i);
      cout << (i+1) << ". ";
      cout << GetLawName(TableAngleLaw[Index].type,language);
      cout << "\t: " << GetLawNotate(TableAngleLaw[Index].type) << endl;
    }
    cout << "*------------------------------------------------------------*\n";
   do
    { do
       { CurX = wherex(); CurY = wherey();
         cout << "Enter number [1 - " << Count << "]: ";
         cin >> Select;
         gotoxy(CurX,CurY); clreol();
       } while((Select > Count) || (Select < 1));
      Select--;
      Select = GetIndexLaw(TableAngleLaw, TableAngleLawCount, Select);
      cout << "Have You selected <";
      cout << GetLawName(TableAngleLaw[Select].type,language);
      cout << ": " << GetLawNotate(TableAngleLaw[Select].type) << "> ? [Y/N]: ";
      cin >> c;
      if((c == 'Y') || (c == 'y')) lBreak = true;
      else lBreak = false;
      gotoxy(CurX,CurY); clreol();
    } while(!lBreak);

    data[index].HorPolPhaseLawType = TableAngleLaw[Select].type;
    data[index].HorPolPhase = TableAngleLaw[Select].law;

    //���� ���������� ������ ��������� ���� �������������� ������������
    //����������� �������� �������
    const char* dimstr = GetMeasureName(TableAngleLaw[Select].M.DimFunc);
    gotoxy(X,Y);
    for(uint i = Y; i<=CurY; i++) { clreol(); cout << endl; }
    gotoxy(X,Y);
    cout << "HORIZONTAL POLARIZATION PHASE LAW: "
         << GetLawNotate(TableAngleLaw[Select].type) << " [" << dimstr
         << "]" << endl;

    //���������� ����������� ������, ��������� � ���������� �������, ���������-
    //������ ��������� ���� �������������� ������������ ����������� �������
    //�� �������

    data[index].HorPolPhaseArg =
    TableAngleLaw[Select].inputfactor(TableAngleLaw[Select].M);

    gotoxy(X,Y);
    clreol();
    str = TableAngleLaw[Select].mathlawfactor(data[index].HorPolPhaseArg);
    cout << "HORIZ. POLAR. PHASE LAW: " << str << " [" << dimstr << "]" << endl;
    delete[] str;
    cout << "Have You accept this law? [Y/N]: ";
    cin >> c;
    CurY = wherey();
    if((c == 'Y') || (c == 'y')) lBreak = true;
    else lBreak = false;
    gotoxy(X,Y);
    for(uint i = Y; i<=CurY; i++) { clreol(); cout << endl; }
    gotoxy(X,Y);
  } while(!lBreak);
}
//*****************************************************************************
//������: I.9., private, class member-function
//������������ �������: void PhaseLawInPut(uint index,uint language)
//����������:
//������� ������������� ��� ����� ������ �� ���������������� ����
//���������� �������� ������� � ���������� ������� index.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. const uint index - ���������� ����� ���������� �������� ������� � �������
//data
//2. uint language - ���� ����������
//-----------------------------------------------------------------------------
//������������ ��������: ���
//*****************************************************************************
void ExtSignal::PhaseLawInPut(uint index,uint language)
{ uint X = 1;
  uint Y = wherey();
  uint CurX,CurY;
  uint Select;
  char c;
  char* str;
  bool lBreak;
  //����� ������ ������� �� ������� ����� ���������� �� ������� ����������������
  //���� ������������ �������� �������
  uint Count = AccessibleLaw(TableAngleLaw,TableAngleLawCount);
  uint Index;
  gotoxy(X,Y);
 do
  {
    cout << "*------------------------------------------------------------*\n";
    cout << "*             SELECT MODIFICATION LAW OF SPACE PHASE         *\n";
    cout << "*------------------------------------------------------------*\n";
    for(uint i = 0; i<Count; i++)
    { Index = GetIndexLaw(TableAngleLaw, TableAngleLawCount, i);
      cout << (i+1) << ". ";
      cout << GetLawName(TableAngleLaw[Index].type,language);
      cout << "\t: " << GetLawNotate(TableAngleLaw[Index].type) << endl;
    }
    cout << "*------------------------------------------------------------*\n";
   do
    { do
       { CurX = wherex(); CurY = wherey();
         cout << "Enter number [1 - " << Count << "]: ";
         cin >> Select;
         gotoxy(CurX,CurY); clreol();
       } while((Select > Count) || (Select < 1));
      Select--;
      Select = GetIndexLaw(TableAngleLaw, TableAngleLawCount, Select);
      cout << "Have You selected <";
      cout << GetLawName(TableAngleLaw[Select].type,language);
      cout << ": " << GetLawNotate(TableAngleLaw[Select].type) << "> ? [Y/N]: ";
      cin >> c;
      if((c == 'Y') || (c == 'y')) lBreak = true;
      else lBreak = false;
      gotoxy(CurX,CurY); clreol();
    } while(!lBreak);

    data[index].PhaseLawType = TableAngleLaw[Select].type;
    data[index].PhaseLaw = TableAngleLaw[Select].law;

    //���� ���������� ������ ��������� ���������������� ����  ������������
    //�������� �������
    const char* dimstr = GetMeasureName(TableAngleLaw[Select].M.DimFunc);
    gotoxy(X,Y);
    for(uint i = Y; i<=CurY; i++) { clreol(); cout << endl; }
    gotoxy(X,Y);
    cout << "SPACE PHASE LAW: "
         << GetLawNotate(TableAngleLaw[Select].type) << " [" << dimstr
         << "]" << endl;

    //���������� ����������� ������, ��������� � ���������� �������, ���������-
    //������ ��������� ���������������� ���� ������� �� �������

    data[index].PhaseArg =
    TableAngleLaw[Select].inputfactor(TableAngleLaw[Select].M);

    gotoxy(X,Y);
    clreol();
    str = TableAngleLaw[Select].mathlawfactor(data[index].PhaseArg);
    cout << "SPACE PHASE LAW: " << str << " [" << dimstr << "]" << endl;
    delete[] str;
    cout << "Have You accept this law? [Y/N]: ";
    cin >> c;
    CurY = wherey();
    if((c == 'Y') || (c == 'y')) lBreak = true;
    else lBreak = false;
    gotoxy(X,Y);
    for(uint i = Y; i<=CurY; i++) { clreol(); cout << endl; }
    gotoxy(X,Y);
  } while(!lBreak);
}
//*****************************************************************************
//������: I.10., private, class member-function
//������������ �������: void PhaseCasualLawInPut(uint index,uint language)
//����������:
//������� ������������� ��� ������ ���������� ������ ��������� ����
//���������� �������� ������� � ���������� ������� index. �������� ����������
//������� ��������� �������� �� ���� �������. � ���� ������ ����������
//������� ����� ���� - "UnCasual law: F(t) = 0"
//-----------------------------------------------------------------------------
//������������ ���������:
//1. const uint index - ���������� ����� ���������� �������� ������� � �������
//2. uint language - ���� ������ �������������� ���������
//-----------------------------------------------------------------------------
//������������ ��������: ���
//*****************************************************************************
void ExtSignal::PhaseCasualLawInPut(uint index,uint language)
{ uint X = 1;
  uint Y = wherey();
  uint CurX,CurY;
  uint Select;
  char c;
  char* str;
  bool lBreak;
  //����� ������ ��������� ������� �� ������� ����� ���������� ��
  //������� ���� ������������ �������� �������
  uint Count = AccessibleLaw(TableCasualLaw,TableCasualLawCount);
  uint Index;
  gotoxy(X,Y);
 do
  {
    cout << "*------------------------------------------------------------*\n";
    cout << "*            SELECT PHASE MODIFICATION CASUAL LAW            *\n";
    cout << "*------------------------------------------------------------*\n";
    for(uint i = 0; i<Count; i++)
    { Index = GetIndexLaw(TableCasualLaw, TableCasualLawCount, i);
      cout << (i+1) << ". ";
      cout << GetLawName(TableCasualLaw[Index].type,language);
      cout << "\t: " << GetLawNotate(TableCasualLaw[Index].type) << endl;
    }
    cout << "*------------------------------------------------------------*\n";
   do
    { do
       { CurX = wherex(); CurY = wherey();
         cout << "Enter number [1 - " << Count << "]: ";
         cin >> Select;
         gotoxy(CurX,CurY); clreol();
       } while((Select > Count) || (Select < 1));
      Select--;
      Select = GetIndexLaw(TableCasualLaw, TableCasualLawCount, Select);
      cout << "Have You selected <";
      cout << GetLawName(TableCasualLaw[Select].type,language);
      cout << ": " << GetLawNotate(TableCasualLaw[Select].type) << "> ? [Y/N]: ";
      cin >> c;
      if((c == 'Y') || (c == 'y')) lBreak = true;
      else lBreak = false;
      gotoxy(CurX,CurY); clreol();
    } while(!lBreak);

    data[index].PhaseCasualLawType = TableCasualLaw[Select].type;
    data[index].PhaseCasualLaw = TableCasualLaw[Select].law;

    //������������� ���������� ���������� ������ ��������� ���������
    //�������� �������
    gotoxy(X,Y);
    for(uint i = Y; i<=CurY; i++) { clreol(); cout << endl; }
    gotoxy(X,Y);
    cout << "PHASE CASUAL LAW: "
         << GetLawNotate(TableCasualLaw[Select].type) << endl;

    //���������� ��������� (��� �������������) ���������� ������, ������������-
    //��� ��������� ���� ������� �� �������
    data[index].PhaseCasualLawArg =
    TableCasualLaw[Select].inputfactor(TableCasualLaw[Select].M);

    gotoxy(X,Y);
    clreol();
    str = TableCasualLaw[Select].mathlawfactor(data[index].PhaseCasualLawArg);
    cout << "PHASE CASUAL LAW: " << str << endl;
    delete[] str;
    cout << "Have You accept this law? [Y/N]: ";
    cin >> c;
    CurY = wherey();
    if((c == 'Y') || (c == 'y')) lBreak = true;
    else lBreak = false;
    gotoxy(X,Y);
    for(uint i = Y; i<=CurY; i++) { clreol(); cout << endl; }
    gotoxy(X,Y);
  } while(!lBreak);
}
//*****************************************************************************
//������: I.11., private, class member-function
//������������ �������: bool ConsoleDataOutPut(uint index, bool dim = TDIM,
//                           bool confirm = false)
//����������:
//������� ������������� ��� ������ ������ �� ���� ��������������� ����������
//�������� ������� � ���������� ������� index. ��� �������� ������ 0 �������
//data ������ �������� �������� ������������ �������� �������.
//������� ����� ����������� � ������������ ��� �������� (����������) � �������-
//���� �������. ������ ��������������, ���� confirm = true (����� �� ��������� -
//��� ������� confirm == false).
//��������� ������� ����� ������������ � ���� ������������:
//dim == TDIM � �������� ����������� (�� ���������), ��� ������� ���������� ��-
//�������� ������� ��������� ������� ��������� ������� �� �������
//dim == SDIM � �������� �����������, ��� ������� ��������� ������� ��������
//������������� (� ���������� ��� �� ����� � �������).
//-----------------------------------------------------------------------------
//������������ ���������:
//1. const uint index - ���������� ����� ���������� �������� ������� � �������
//                      ��������� �������� ������� data
//2. bool dim - �����������, � ������� ������������ ��������� ������� ���������
//              ������� ��������� �������� �������
//3. bool confirm - ����� ������������� ���������� ������������� �������
//-----------------------------------------------------------------------------
//������������ ��������: bool confirm - ������������� ���������� ������
//*****************************************************************************
bool ExtSignal::ConsoleDataOutPut(uint index, bool dim, bool confirm)
{ uint X = 1;
  uint Y = wherey();
  uint Select, CurY;
  char c;
  const char* dimstr;
  char* str;
  LAW* Table = TableAngleLaw;
  uint Count = TableAngleLawCount;

  cout << "*******************************************************************\n";
  cout << "*            Performances  of  signal  component                  *\n";
  cout << "*******************************************************************\n";
  //����� ����� ������������� ���������� �������
  cout << " I. TOTAL PROPERTIES" << endl;

  cout << "  1. Type: ";
  if (index == 0) cout << "DESIRED SIGNAL " << endl;
  else cout << "JAMMER N " << index << endl;

  cout << "  2. Usage: ";
  if (data[index].Usage == ON) cout << "YES" << endl;
  else cout << "NO" << endl;

  cout << "  3. Azimuth Angle: ";
  Select = GetIndexLawByType(Table, Count, data[index].AzAngleLawType);
  dimstr = GetMeasureName(TableAngleLaw[Select].M.DimFunc);
  if (dim != TDIM)
   InvertAllLawParam(TableAngleLaw+Select, data[index].AzAngleArg);
  str = TableAngleLaw[Select].mathlawfactor(data[index].AzAngleArg);
  cout << str << " [" << dimstr << "]" << endl;

  if (dim != TDIM)
   ConvertAllLawParam(TableAngleLaw+Select, data[index].AzAngleArg);
  delete str;

  cout << "  4. Elevate Angle: ";
  Select = GetIndexLawByType(Table, Count, data[index].ElevAngleLawType);
  dimstr = GetMeasureName(TableAngleLaw[Select].M.DimFunc);
  if (dim != TDIM)
   InvertAllLawParam(TableAngleLaw+Select, data[index].ElevAngleArg);
  str = TableAngleLaw[Select].mathlawfactor(data[index].ElevAngleArg);
  cout << str << " [" << dimstr << "]" << endl;
  if (dim != TDIM)
   ConvertAllLawParam(TableAngleLaw+Select, data[index].ElevAngleArg);
  delete str;

  //����� ��������������� ������� ���������� �������
  cout << " II. POLARIZING PROPERTIES" << endl;

  cout << "  5. Polarization Angle: ";
  Select = GetIndexLawByType(Table, Count, data[index].PolAngleLawType);
  dimstr = GetMeasureName(TableAngleLaw[Select].M.DimFunc);
  if (dim != TDIM)
   InvertAllLawParam(TableAngleLaw+Select, data[index].PolAngleArg);
  str = TableAngleLaw[Select].mathlawfactor(data[index].PolAngleArg);
  cout << str << " [" << dimstr << "]" << endl;
  if (dim != TDIM)
   ConvertAllLawParam(TableAngleLaw+Select, data[index].PolAngleArg);
  delete str;

  cout << "  6. Phase of Vert. Comp. Polar.: ";
  Select = GetIndexLawByType(Table, Count, data[index].VertPolPhaseLawType);
  dimstr = GetMeasureName(TableAngleLaw[Select].M.DimFunc);
  if (dim != TDIM)
   InvertAllLawParam(TableAngleLaw+Select, data[index].VertPolPhaseArg);
  str = TableAngleLaw[Select].mathlawfactor(data[index].VertPolPhaseArg);
  cout << str << " [" << dimstr << "]" << endl;
  if (dim != TDIM)
   ConvertAllLawParam(TableAngleLaw+Select, data[index].VertPolPhaseArg);
  delete str;

  cout << "  7. Phase of Hor. Comp. Polar.: ";
  Select = GetIndexLawByType(Table, Count, data[index].HorPolPhaseLawType);
  dimstr = GetMeasureName(TableAngleLaw[Select].M.DimFunc);
  if (dim != TDIM)
   InvertAllLawParam(TableAngleLaw+Select, data[index].HorPolPhaseArg);
  str = TableAngleLaw[Select].mathlawfactor(data[index].HorPolPhaseArg);
  cout << str << " [" << dimstr << "]" << endl;
  if (dim != TDIM)
   ConvertAllLawParam(TableAngleLaw+Select, data[index].HorPolPhaseArg);
  delete str;

  //����� ����������������� ������� ���������� �������
  cout << " III. SPACE PROPERTIES" << endl;

  cout << "  8. Space Amplitude: ";
  Table = TableAmplLaw; Count = TableAmplLawCount;
  Select = GetIndexLawByType(Table, Count, data[index].AmplLawType);
  dimstr = GetMeasureName(TableAmplLaw[Select].M.DimFunc);
  if (dim != TDIM)
   InvertAllLawParam(TableAmplLaw+Select, data[index].AmplArg);
  str = TableAmplLaw[Select].mathlawfactor(data[index].AmplArg);
  cout << str << " [" << dimstr << "]" << endl;
  if (dim != TDIM)
   ConvertAllLawParam(TableAmplLaw+Select, data[index].AmplArg);
  delete str;

  cout << "  9. Space Phase: ";
  Table = TableAngleLaw; Count = TableAngleLawCount;
  Select = GetIndexLawByType(Table, Count, data[index].PhaseLawType);
  dimstr = GetMeasureName(TableAngleLaw[Select].M.DimFunc);
  if (dim != TDIM)
   InvertAllLawParam(TableAngleLaw+Select, data[index].PhaseArg);
  str = TableAngleLaw[Select].mathlawfactor(data[index].PhaseArg);
  cout << str << " [" << dimstr << "]" << endl;
  if (dim != TDIM)
   ConvertAllLawParam(TableAngleLaw+Select, data[index].PhaseArg);
  delete str;

  //����� ��������� ������������ ��������� � ���� ���������� �������
  cout << " IV. CASUAL COMPONENTS AMPLITUDE AND PHASE" << endl;

  cout << "  10. Casual Amplitude: ";
  Table = TableCasualLaw; Count = TableCasualLawCount;
  Select = GetIndexLawByType(Table, Count, data[index].AmplCasualLawType);
  str = TableCasualLaw[Select].mathlawfactor(data[index].AmplCasualLawArg);
  cout << str << endl;
  delete str;

  cout << "  11. Casual Phase: ";
  Select = GetIndexLawByType(Table, Count, data[index].PhaseCasualLawType);
  str = TableCasualLaw[Select].mathlawfactor(data[index].PhaseCasualLawArg);
  cout << str << endl;
  cout << "*******************************************************************\n";
  delete str;
  if (confirm == false) //��� �������������
  { cout << "Press any key for continue..." << endl;
    getch();
    confirm = true;
  }
  else //����� �������������
  {
   cout << "Have You accept this performances signal component? [Y/N]: ";
   cin >> c;
   if ((c == 'Y') || (c == 'y')) confirm = true;
   else confirm = false;
  }

  //������� ������
  CurY = wherey();
  gotoxy(X,Y);
  for(uint i = Y; i <= CurY; i++) {clreol(); cout << endl;}
  gotoxy(X,Y);

 return confirm;
}
//*****************************************************************************
//������: I.12., private, class member-function
//������������ �������: ARGLIST* InvertAllLawParam(const LAW* Law, ARGLIST* Arg)
//����������:
//������� ������������� ��� �������������� ���� ���������� ������ (��� ���������)
//�� �������� ����������� (TDIM) � �������� ����������� (SDIM). ��� ��������
//������������ ����������� ����������� � ������� ������������ ������ ���������
//������� ��������� ������� ��������� ������� (����. � DEG). ��� �������� ���-
//��������� ���������� ����������� ���������� ��� ������� �������������� �������
//(����. RAD). ��� ����� ������ ���������� �������������� �������������� ��
//�������� ����������� � ��������. ���� �������� ������� ������������� ������
//�������, ������� ��������� ���������� ��������� ������� � ��������� ���������-
//����.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. const LAW* Law - ����� ��������� �������� ���������� �������� �������
//2. ARGLIST* Arg - ������ ���������� ������
//-----------------------------------------------------------------------------
//������������ ��������: ARGLIST* Arg - ��������������� ������ ���������� ������
//�� �������� ����������� � ��������
//*****************************************************************************
ARGLIST* ExtSignal::InvertAllLawParam(const LAW* Law, ARGLIST* Arg)
{
 if ((Arg != NULL) && (Arg->Size > 1))
  for (uint i = 1; i < Arg->Size; i++)
   Arg->List[i] = Law->M.Factor[i].Invert(Arg->List[i]);

 return Arg;
}
//*****************************************************************************
//������: I.13., private, class member-function
//������������ �������: ARGLIST* ConvertAllLawParam(const LAW* Law, ARGLIST* Arg)
//����������:
//������� ������������� ��� �������������� ���� ���������� ������ (��� ���������)
//�� �������� ����������� (TDIM) � �������� ����������� (SDIM). ��� ��������
//������������ ����������� ����������� � ������� ������������ ������ ���������
//������� ��������� ������� ��������� ������� (����. � DEG). ��� �������� ���-
//��������� ���������� ����������� ���������� ��� ������� �������������� �������
//(����. RAD). ��� ����� ������ ���������� �������������� �������������� ��
//�������� ����������� � ��������. ���� �������� ������� ������������� ������
//�������, ������� ��������� ���������� ��������� ������� � ��������� ���������-
//����.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. const LAW* Law - ����� ��������� �������� ���������� �������� �������
//2. ARGLIST* Arg - ������ ���������� ������
//-----------------------------------------------------------------------------
//������������ ��������: ARGLIST* Arg - ��������������� ������ ���������� ������
//�� �������� ����������� � ��������
//*****************************************************************************
ARGLIST* ExtSignal::ConvertAllLawParam(const LAW* Law, ARGLIST* Arg)
{
 if ((Arg != NULL) && (Arg->Size > 1))
  for (uint i = 1; i < Arg->Size; i++)
   Arg->List[i] = Law->M.Factor[i].Convert(Arg->List[i]);

 return Arg;
}

//*****************************************************************************
//������: I.14., private, class member-function
//������������ �������: bool FileDataEntry(const char* filename)
//����������:
//������� ������������� ��� ����� ������ �� ���� ��������������� ���������
//�������� ������� �� ���������� �����.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. string filename - ������������ ����� � �������� ������� �� �������� �������
//-----------------------------------------------------------------------------
//������������ ��������: bool lSuccess
// true - ������ ������� �� ����� �������
// false - ������ ������� �� ����� �� �������
//*****************************************************************************
bool ExtSignal::FileDataEntry(const char* filename)
{ bool lSuccess = true;
  ulong CurRow = 0; //������� ����� ������ �������� ����� filename
  uint Y = wherey();
  uint CurY;
  string s;
  char c;
  fname.append(filename);
  //************************************************************************
  //��� I.
  //��������� ��������� ���� � ������� �� EXTERNAL SIGNAL � ������ ������
  //************************************************************************
  if (fname.length() == 0) //��� �������� ����� � ������� �� ������
  { lSuccess = false;
    cout << "\nERROR: filename is absent." << endl;
    cout << "Enter new filename? [Y/N]: ";
    cin >> c;

    //������� ������
    CurY = wherey();
    gotoxy(1,Y);
    for(uint i = Y; i <= CurY; i++) {clreol(); cout << endl;}
    gotoxy(1,Y);

    if ((c == 'Y') || (c == 'y')) lSuccess = GetInPutFilename();
    if (!lSuccess) return lSuccess;
  }
  //��������� ������� ����
  else
  { fin.open(fname.c_str(),ios::in);
    if (!fin)
    { lSuccess = false;
      cout << "\nERROR: unable to open <" << fname << ">" << endl;
      cout << "Try again ? [Y/N]: ";
      cin >> c;
      //������� ������
      CurY = wherey();  gotoxy(1,Y);
      for(uint i = Y; i <= CurY; i++) {clreol(); cout << endl;}
      gotoxy(1,Y);

      if ((c == 'Y') || (c == 'y')) lSuccess = GetInPutFilename();
      if (!lSuccess) return lSuccess;
    }
    else //���� � ������ filename ������ � ������ ������
    { cout << "You open file <" << fname << ">" << endl;
      //������� ������
      CurY = wherey();  gotoxy(1,Y);
      for(uint i = Y; i <= CurY; i++) {clreol(); cout << endl;}
      gotoxy(1,Y);
    }
  }
  //**************************************************************************
  //��� II. ��������� ���� ����������� ������, ������� ����� ���� ����������
  //        ��� ���������� ������ �� �������� ��������� ������ fin - signal.err
  //**************************************************************************
  //���������� ����� �������� ����� ��� ����������
  uint pos = fname.find(".");
  //������� ���� �� ����� ����������
  if (pos == string::npos) ferrname = fname+".err";
  else //���� ����� ����������
  { s = fname.substr(pos);
    pos = s.find(".err");
    if (pos != string::npos) //������� ���� ����� ������������ ���������� ".err"
    {lSuccess = false;
     #if defined(CONSOLE)
      cout << "\nERROR IN INPUT DATA FILE NAME !!!" << endl;
      cout << "Input data file <" << fname << "> contains the inadmissibles "
           << "extension - .err." << endl;
     #endif
     return lSuccess;
    }
   //������������ ����� ����� ����������� ������ ferrname � ������ �����������
   //� ������ �������� ����� ������ fname � ����������� ".err"
   pos = fname.find(".");
   ferrname = fname.substr(0,pos)+".err";
  }
  ferr.open(ferrname.c_str(),ios::out);
  s.assign(67,' ');
  ferr<<"===================================================================" << endl;
  ferr<<"*���� ����������� ������, ��������� ��� ���������� ������ �� �����*" << endl;
  s.insert((s.length()-fname.length())/2,fname);//to_upper(fname)
  ferr << s << endl;
  ferr<<"===================================================================" << endl;
  //***************************************************************************
  //��� III. ���������� ��������� �������� �������:
  //         DESIRED SIGNAL (1) + N (JAMMERS)
  //�� ������� ����� ������ �������������� ������, ���������� ����� ����� ����
  //JAMMER = 2. ��� ������ �� ������ ���� ������������������ ��������� '//'
  //***************************************************************************
  //��������� �� ����� fname ���������� �����, �������������� �� ������� �������
  if (GetNoiseCountFromFile(CurRow))
  { //���������� ������������ �������� ������� � ������ ��������� �������
    Count += 1;
    //������������� ������ ��� ���������� �������� �������
    CreateArray(Count);
    //��������� ��������� ���� ������������� �������� �������
    for (uint i = 0; i < Count; i++) ResetProperty(i);

    if (TableAngleLawCount == 0) CreateTableOfAngleLaw();
    if (TableAmplLawCount == 0) CreateTableOfAmplLaw();
    if (TableCasualLawCount == 0) CreateTableOfCasualLaw();
  }
  //�������� ������ �� ����� fname �� ����� ����� �� �������
  else
  { lSuccess = false;
    cout << "\nERROR: Error in file <" << uppercase << fname << ">" << endl;
    cout << "To receive data on noises amount it was not possible" << endl;
    cout << "Press any key...";
    getch();
    return lSuccess;
  }
  //***************************************************************************
  // ��� IV. ��������� �� ����� fname ������ �� ������� ���������� ��������
  //         �������. ������ ����������� ������ ������ ���� �������� ������.
  //***************************************************************************
  uint i;
  for (i = 0; i < Count; i++)
  {
   clrscr();
   cout << "Reading data from file about ";
   if (i == 0) cout << "< DESIRED SIGNAL > ..." << endl;
   else cout << "< JAMMER_" << i << " > ..." << endl;
   lSuccess = GetCompSignalFromFile(i, CurRow);
   if (lSuccess == false) //������ ������ ������ � ���������� �������
   { cout << "\nERROR: Error in file <" << uppercase << fname << ">" << endl;
     cout << "Error at deriving of signal component performance from the file."
          << endl;
     cout << "The information about an error is placed in an error log file "
          << "< SIGNAL.ERR > ." << endl;
     cout << "Press any key...";
     getch();
     clrscr();
     break;
   }
  }

 clrscr();
return lSuccess;
}
//*****************************************************************************
//������: I.15., private, class member-function
//������������ �������: bool GetInPutFilename()
//����������:
//������� ������������� ��� �������� �������� ��������� ������ � ������
//��������� ������ � ������ ��� ������. ��� ����� �������� � ���������� �������-
//��� ������ ExtSignal - fname, ������� �������� ����� ���������� ����� �������-
//��� ���������� ������ ExtSignal - fin.
//-----------------------------------------------------------------------------
//������������ ���������: ���
//-----------------------------------------------------------------------------
//������������ ��������: bool lSuccess
// true - ��������� ���� ������� ������� � ������ ������
// false - ��������� ���� ������� �� ������� � ������ ������
//*****************************************************************************
bool ExtSignal::GetInPutFilename()
{ bool lSuccess = true;
  bool lBreak = false;
  uint Y = wherey();
  uint CurY;
  char c;
  do
  { cout << "Enter filename: ";
    cin >> fname; //���� ����� �����
    //��������� ���� � ������ s � ������ ������ ��� ��������� ����
    fin.open(fname.c_str(),ios::in);
    //�������� �� ��������
    if (!fin) //���� ������� �� �������
    { cout << "\nERROR: unable to open file <" << fname << ">" << endl;
      cout << "Enter new filename ? [Y/N]: ";
      cin >> c;
      //���������� ������� ������� ����
      if ((c != 'Y') && (c != 'y')) {lBreak = true; lSuccess = false;}
    }
    //���� ������, ������� �� �����
    else
    { lBreak = true; lSuccess = true;
      cout << "You open file <" << uppercase << fname << ">" << endl;
      cout << "Press any key... ";
      getch();
    }
    //������� ������
    CurY = wherey(); gotoxy(1,Y);
    for(uint i = Y; i <= CurY; i++) {clreol(); cout << endl;}
    gotoxy(1,Y);
  } while (!lBreak);
return lSuccess;
}
//*****************************************************************************
//������: I.16., private, class member-function
//������������ �������: bool GetNoiseCountFromFile(ulong& CurRow)
//����������:
//������� ������������� ��� ����������� ����� �����, ������� ������������ ��
//������� ������� �� ������ �� �������� ����� fname. ������, � ������� ������
//������������� ��� ���������� ������ ��������� �������� ����� <JAMMER>, �����
//�������� ������ ��������� �����, ����������� �� ���������� �����. ��������,
//< JAMMER = 4 >. ������ ������ �� ������ ���� ���������������� � ������� �����-
//��� "//". ���� ������ ���������� ����� �� �������, �� ������� ���������� false
//-----------------------------------------------------------------------------
//������������ ���������:
//1. ulong& CurRow - ����� ������� ������ ����� fname
//-----------------------------------------------------------------------------
//������������ ��������: bool lSuccess
// true - ���������� ����� � External Signal ����������
// false - ���������� ����� � External Signal ���������� �� �������
//*****************************************************************************
bool ExtSignal::GetNoiseCountFromFile(ulong& CurRow)
{ bool lSuccess = true;
  bool lFind = false;
  string s;
  ulong value;
  uint pos;
  uint endpos;
  //s.set_case_sensitive(0);
  //���� ������������ ����� �������� ��������� ������ fin
  //����� ��������� ����� <JAMMER>
  while ((fin.eof() == 0) && (!lFind))
  { getline(fin,s,'\n');
    CurRow++;
    pos = s.find("//");
    if ( pos != 0 )
    { pos = (pos != string::npos) ? pos-1 : s.length();
      s = s.substr(0,pos);
      pos = s.find("JAMMER");
      if (pos != string::npos) lFind = true; //�������� ����� �������
    }
  }
  if (lFind == false) //�������� ����� �� �������
  { lSuccess = false;
    ferr << endl;
    ferr << "ERROR: Noises amount to determine it was not possible." << endl;
    ferr << "REASON: The key word < JAMMER > not found in file <" << fname << ">"
         << endl;
    return lSuccess;
  }
  //����������� ����� �����
  pos = s.find_first_of("123456789",pos+5);
  if (pos != string::npos) //����� �������
  { endpos = s.find_first_not_of("0123456789",pos+1);
    s = s.substr(pos,endpos);
    char *endptr;
    value = strtol(s.c_str(),&endptr,10);
    if (value < 65534) //
     Count = value;
    else //����� ������� ����� �����
    { lSuccess = false;
      ferr << endl;
      ferr << "ERROR: Very large number of jammers = " << value << endl;
      ferr << "TIP: Total number admissible range of jammers = [0...65534]"
           << endl;
      return lSuccess;
    }
  }
  else //����� ����� ���������� �� �������
  { lSuccess = false;
    ferr << endl;
    ferr << "ERROR: Jammers amount to determine it was not possible." << endl;
    ferr << "Line N " << CurRow << " <" << s << ">" << endl;
    ferr << "REASON: In this line total number of jammers not found"  << endl;
    return lSuccess;
  }
return lSuccess;
}
//*****************************************************************************
//������: I.17., private, class member-function
//������������ �������: bool GetCompSignalFromFile(uint index, ulong& CurRow)
//����������:
//������� ������������� ��� ��������� ������ �� ��������� ������� ����������
//������� � ���������� ������� index �� �������� ����� fname. ������ ����������
//������� ��������������� 11 ����������������. ������ ���������� (index = 0)
//������ ��������� �������� ��������, ��������� ���������� - ������. ������
//�������������� ������������ ������� ����������� �� ������������. ����
//�������������� �� �������� ����������, �� ���������� ������ ������������ � ��-
//����� ��������� �� ������ � ����� signal.err.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. uint index - ���������� ����� ���������� �������� �������
//2. ulong& CurRow - ����� ������� ������ ����� fname
//-----------------------------------------------------------------------------
//������������ ��������: bool lSuccess
// true - �������������� ������������ ������� ������� ���������
// false - �������������� ������������ ������� ������� �� ���������
//*****************************************************************************
bool ExtSignal::GetCompSignalFromFile(uint index, ulong& CurRow)
{ bool lSuccess = true;
  bool lFind;
  ulong num;
  uint pos;
  string s;
  string digit = "0123456789";
  uint Property;

  //s.skip_whitespace(0);
  //���� ���������� ������������� ������� �� �������� �����
  for (Property = 1; Property <= 11; Property++)
  {
   lFind = false;
   num = 0;
   //����� ������, ������� ���������� � ������ ���������������� ������ �������-
   //������� �������, ������� � ������ ������ ������������
   while ((fin.eof() == 0) && (!lFind))
   { getline(fin,s,'\n');
     CurRow++;
     pos = s.find("//");
     if (pos != 0)
     { pos = (pos != string::npos) ? pos : s.length();
       s = s.substr(0,pos);
       //����� ������� ������������� ������� � ������
       pos = s.find_first_not_of(" \t");
       if (pos != string::npos) //������ �������� �� ������� ������
       { s = s.substr(pos);
         //������ �������� �� ������� ������ � ������ ������ ���� ������
         pos = digit.find(s[0]);
         if (pos != string::npos) //���� ������ �������� ������
         {
          char *endptr;
          num = strtol(s.c_str(),&endptr,10);
          //���������� ����� �������������� ��������� � ���������
          if (num == Property)
          {//����� ������� ':' - ������ ������ �� ���������� �������� ����������
           //������� � ���������� ������� num
           pos = s.find(":");
           if (pos == string::npos) //������! ������ ':' �� ������
           {lSuccess = false;
            ferr << "ERROR: Incorrect format of signal component performance.\n";
            ferr << "Signal Component: " << index << endl;
            ferr << "Signal Property: " << SignalPropertyName[Property] << endl;
            ferr << "Line N " << CurRow << ": <" << s << ">" << endl;
            ferr << "REASON: The token ':' is not found - token of a\n";
            ferr << "        beginning of a signal component property." << endl;
            return lSuccess;
           }
           //������� ��������� ������� � ������ �������������
           if (pos != s.length()-1) pos = s.find_first_not_of(" \t",pos+1);
           else pos = string::npos;

           if (pos == string::npos) s = "ERROR";
           else s = s.substr(pos);
           //������� �������� ������� � ������ �������������
           pos = s.find_last_not_of(" \t");
           s = s.substr(0,pos+1);
           //���������� ��������� �������������� ���������� �������, �������
           //������������� �������� � ������� Property
           lSuccess = GetPropertySignalFromFile(s,index,Property,CurRow);
           //������ ��� ���������� ���������� �������� ������� �� �����
           if (lSuccess == false) return lSuccess;
           lFind = true;
          }
          else //���������� ����� �������������� �� ��������� � ���������
          { lSuccess = false;
            ferr << "ERROR: The serial number signal property does't coincide "
                 << "expected number." << endl;
            ferr << "Signal Component: " << index << endl;
            ferr << "Signal Property: " << SignalPropertyName[Property] << endl;
            ferr << "Serial number: " << num << " Expected number: " << Property
                 << endl;
            ferr << "Line N " << CurRow << ": <" << s << ">" << endl;
            return lSuccess;
          }
         }
         else //������: ���� ������ �� �����
         { lSuccess = false;
           ferr << "ERROR: Incorrect format of signal component performance.\n";
           ferr << "Signal Component: " << index << endl;
           ferr << "Signal Property: " << SignalPropertyName[Property] << endl;
           ferr << "Line N " << CurRow << ": <" << s << ">" << endl;
           ferr << "REASON: The line with signal property should begin "
                << "with figure" << endl;
           return lSuccess;
         }
       }

     }
   }//����� ����� while c��������� ����� �� �����

   //��������� ����� �����, ������� �� ��� �������������� ���������� �������
   //��� ������� �� ��� ���������� �������
   if (lFind == false)
   { lSuccess = false;
    if ((num != 11) && (num != 0))
    {ferr << "ERROR: End of file is reached. It was not possible to receive\n";
     ferr << "       all performances of a signal component." << endl;
     ferr << "Signal Component: " << index << endl;
     ferr << "Signal Property: " << SignalPropertyName[Property] << endl;
     ferr << "Line N " << CurRow << ": <" << s << ">" << endl;
    }
    else if (num == 0) //������� �� ��� ���������� �������
    {ferr << "ERROR: End of file is reached. It was not possible to receive\n";
     ferr << "       all signal components." << endl;
     ferr << "Total Signal Components: " << Count << endl;
     ferr << "Getted Signal Components: " << index << endl;
    }
     return lSuccess;
   }
  }//����� ����� for ���������� ������������� ������� - Property
return lSuccess;
}
//*****************************************************************************
//������: I.18., private, class member-function
//������������ �������: bool GetPropertySignalFromFile(string& s,
//                      uint CompSignal, uint PropSignal, ulong& CurRow)
//����������:
//������� ������������� ��� ����������� ���������� �������� � ���������� �������
//Property ���������� �������� �������. ������ ������������ ������� ���������-
//������ 11 ����������. ��������� �������������� ���������� ������� ���������:
//1.  TYPE: [DESIRED SIGNAL || JAMMER]
//2.  USAGE: [YES || NO]
//3.  AZIMUTH ANGLE: {NameLAW, [Param_1],...[Param_N]}
//4.  ELEVATE ANGLE: {NameLAW, [Param_1],...[Param_N]}
//5.  POLARIZATION ANGLE: {NameLAW, [Param_1],...[Param_N]}
//6.  PHASE OF VERT. COMP. POLAR.: {NameLAW, [Param_1],...[Param_N]}
//7.  PHASE OF HOR. COMP. POLAR.: {NameLAW, [Param_1],...[Param_N]}
//8.  AMPLITUDE: {NameLAW, [Param_1],...[Param_N]}
//9.  PHASE: {NameLAW, [Param_1],...[Param_N]}
//10. CASUAL AMPLITUDE: {NameLAW, [Param_1],...[Param_N]}
//11. CASUAL PHASE: {NameLAW, [Param_1],...[Param_N]}
//���� ��� ����������� ���������� �������� ���������� ������� ��������������
//������, �� ������ ������� ����������� � ������������� ����������� � ���������-
//���� ���������������� ��������� � ����� "signal.err"
//-----------------------------------------------------------------------------
//������������ ���������:
//1. string& s - ������ � ��������� ��������� ���������� �������
//2. uint CompSignal - ���������� ����� ���������� �������� �������
//3. uint PropSignal - ���������� ����� �������� ���������� �������� �������
//4. ulong& CurRow - ����� ������� ������ ����� fname
//-----------------------------------------------------------------------------
//������������ ��������: bool lSuccess
// true - ��������� �������������� ������������ ������� ������� ���������
// false - ��������� �������������� ������������ ������� ������� �� ���������
//*****************************************************************************
bool ExtSignal::GetPropertySignalFromFile(string& s, uint CompSignal,
                                          uint PropSignal, ulong& CurRow)
{ bool lSuccess = true;
  uint pos;

  //s.set_case_sensitive(0);
  //***********************************************
  //1. ����������� ���� ���������� ������� - TYPE
  //***********************************************
  if (PropSignal == 1)
  { if (CompSignal == 0) //�������� ������
    { pos = s.find("DESIRED SIGNAL");
      if (pos == string::npos) pos = s.find("SIGNAL");
      //������! ������������ ������������� ���������� �������
      if (pos == string::npos)
      {lSuccess = false;
       ferr << "ERROR: The incorrect identifier of a signal component." << endl;
       ferr << "Signal Component: " << CompSignal << endl;
       ferr << "Signal Property: " << SignalPropertyName[PropSignal] << endl;
       ferr << "Line N " << CurRow << ": <" << s << ">" << endl;
       ferr << "REASON: Expected <DESIRED SIGNAL> or <SIGNAL> for the first\n";
       ferr << "        component external signal" << endl;
       return lSuccess;
      }
      data[CompSignal].Type = SIGNAL;
    }
    else if(CompSignal != 0) //������
    { pos = s.find("JAMMER");
      if (pos == string::npos) //������! ������������ ������������� ���������� �������
      {lSuccess = false;
       ferr << "ERROR: The incorrect identifier of a signal component." << endl;
       ferr << "Signal Component: " << CompSignal << endl;
       ferr << "Signal Property: " << SignalPropertyName[PropSignal] << endl;
       ferr << "Line N " << CurRow << ": <" << s << ">" << endl;
       ferr << "REASON: Expected <JAMMER> for this\n";
       ferr << "        component external signal" << endl;
       return lSuccess;
      }
      data[CompSignal].Type = JAMMER;
    }
  }
  //*********************************************************
  //2. ������������� ���������� ������� � �������� - USAGE
  //*********************************************************
  else if (PropSignal == 2)
  { pos = s.find("YES");
    if (pos == string::npos)
    { pos = s.find("NO");
      //���� ������������� ��� �� ������������� ���������� ������� �����������
      if (pos == string::npos) //������!
      { lSuccess = false;
        ferr << "ERROR: Can't find indicator of use of a signal component\n";
        ferr << "       at calculation." << endl;
        ferr << "Signal Component: " << CompSignal << endl;
        ferr << "Signal Property: " << SignalPropertyName[PropSignal] << endl;
        ferr << "Line N " << CurRow << ": <" << s << ">" << endl;
        ferr << "REASON: Expected <YES> or <NO> for this ";
        ferr << "component external signal" << endl;
        return lSuccess;
      }
      else //��������� ������� �� ����� �������������� � ��������
       data[CompSignal].Usage = OFF;
    }
    else //��������� ������� ����� �������������� � ��������
     data[CompSignal].Usage = ON;
  }
  //****************************************************************
  //3. ����������� ������� ��������� � �� ���������� ��������� �
  //   ���� ���������� �������� ������� (3 - 11 �������� �������)
  //****************************************************************
  else
  { string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    //���������� ������ ������������ ������ � ������ �������������� �������.
    //������ ������ ������ ���� ������ ����. ��������, �.�. ������������ �������
    //������ ������ ���������� � �����. ������ ������� �� ���������.
    pos = s.find_first_not_of(" \t\n");
    if (pos != string::npos) //C����� �� ������
     pos = alphabet.find_first_of(s[pos]); //������ ���� �����
    //������! ������ ������ ���� ������ ������������ ������ �� �����
    if (pos == string::npos) //��������� ������
    { lSuccess = false;
      ferr << "ERROR: Incorrect format of signal component performance.\n";
      ferr << "Signal Component: " << CompSignal << endl;
      ferr << "Signal Property: " << SignalPropertyName[PropSignal] << endl;
      ferr << "Line N " << CurRow << ": <" << s << ">" << endl;
      ferr << "REASON: Expected modification law name of "
           << SignalPropertyName[PropSignal] << "." << endl;
      ferr << "The law name should begin with token <" << alphabet << ">" << endl;
      return lSuccess;
    }
    //����� ��������� �� ����� ������������ ������ � ������� ������������ �������
    //LawName[] � ����� ��������� �������������� ������ type �� ��� ������������
    //� ������������� ���������������� �������� ���������� �������. ���� �����-
    //������� ������ ����� �� �������, �� ������������ ������.
    bool detlaw;
    uint LawType;
    string abbr;

    pos = s.find_first_not_of(" \t\n");
    s = s.substr(pos);
    abbr = s;
    pos = abbr.find_first_of(" ,;\t\n",pos);
    //�������� ������������ ������ �� ������ ������� ���������� �������
    abbr = abbr.substr(0,pos);
    detlaw = (PropSignal < 10) ? DETLAW : CASUALLAW;
    //����� �������������� ������ �� ��� ������������ � ������� LawName
    LawType = GetTypeLawByAbbrLaw(abbr,detlaw);
    if (LawType == UNEXIST) //������! ����� ������������ ������ ���
    { lSuccess = false;
      ferr << "ERROR: Incorrect law name abbreviate of signal component " <<
              "performance." << endl;
      ferr << "Signal Component: " << CompSignal << endl;
      ferr << "Signal Property: " << SignalPropertyName[PropSignal] << endl;
      ferr << "Line N " << CurRow << ": <" << abbr << ">" << endl;
      ferr << "The list of possible abbreviations of ";
      if (detlaw == DETLAW) ferr << "determine laws:" << endl;
      else ferr << "causal laws:" << endl;
      s = GetAbbrLaw(s, detlaw);
      uint i = 1;
      bool lBreak = false;
      do
      { pos = s.find_first_of(";");
        if (pos == string::npos) lBreak = true;
        ferr << setw(2) << i++ << ". " << s.substr(0,pos) << endl;
        if (pos == s.length()-1) lBreak = true;
        else s = s.substr(++pos);
      } while (!lBreak);
      return lSuccess;
    }
 //������������� ������ ��������� ��������� �������� ���������� �������
 //���������. ����� ������ � ������� ������� TableAngleLaw, TableAmplLaw �
 //TableCasualLaw � ����������� �� �������������� ���������� ������� �� LawType
   uint Index;
   LAW* PropLaw;
   ARGLIST* Arg = NULL;
   if (pos != string::npos) s = s.substr(pos);
   else s = " ";
   //����� � TableAngleLaw
   if ((PropSignal < 10) && (PropSignal != 8))
    {Index = GetIndexLawByType(TableAngleLaw,TableAngleLawCount,LawType);
     PropLaw = (TableAngleLaw+Index);
      switch (PropSignal)
      { case 3 : //������������ ����
         data[CompSignal].AzAngleLawType = TableAngleLaw[Index].type;
         data[CompSignal].AzimuthAngle = TableAngleLaw[Index].law;
         //�������� ������ ��� ���������� ���������� ������
         data[CompSignal].AzAngleArg = CreateArgList(PropLaw->M.Size);
         Arg = data[CompSignal].AzAngleArg;
         //����������� ���������� ������ ��������� ���� �� ������� �������
         lSuccess = GetArgLawFromFile(s,PropLaw,Arg,CompSignal,PropSignal,CurRow);
         if (lSuccess == false) return lSuccess;
         break;
        case 4 : //���� ����� ��������� �������
         data[CompSignal].ElevAngleLawType = TableAngleLaw[Index].type;
         data[CompSignal].ElevateAngle = TableAngleLaw[Index].law;
         //�������� ������ ��� ���������� ���������� ������
         data[CompSignal].ElevAngleArg = CreateArgList(PropLaw->M.Size);
         Arg = data[CompSignal].ElevAngleArg;
         //����������� ���������� ������ ��������� ���� ����� �������
         lSuccess = GetArgLawFromFile(s,PropLaw,Arg,CompSignal,PropSignal,CurRow);
         if (lSuccess == false) return lSuccess;
         break;
        case 5 : //���� ���������������� ��������� �������
         data[CompSignal].PolAngleLawType = TableAngleLaw[Index].type;
         data[CompSignal].PolAngle = TableAngleLaw[Index].law;
         //�������� ������ ��� ���������� ���������� ������
         data[CompSignal].PolAngleArg = CreateArgList(PropLaw->M.Size);
         Arg = data[CompSignal].PolAngleArg;
         //����������� ���������� ������ ��������� ���� ���������������� ���-�
         lSuccess = GetArgLawFromFile(s,PropLaw,Arg,CompSignal,PropSignal,CurRow);
         if (lSuccess == false) return lSuccess;
         break;
        case 6 : //���� ������������ ������������ ����������� �������
         data[CompSignal].VertPolPhaseLawType = TableAngleLaw[Index].type;
         data[CompSignal].VertPolPhase = TableAngleLaw[Index].law;
         //�������� ������ ��� ���������� ���������� ������
         data[CompSignal].VertPolPhaseArg = CreateArgList(PropLaw->M.Size);
         Arg = data[CompSignal].VertPolPhaseArg;
         //����������� ���������� ������ ��������� ���� ������������ ���������-
         //��� ����������� �������
         lSuccess = GetArgLawFromFile(s,PropLaw,Arg,CompSignal,PropSignal,CurRow);
         if (lSuccess == false) return lSuccess;
         break;
        case 7 : //���� �������������� ������������ ����������� �������
         data[CompSignal].HorPolPhaseLawType = TableAngleLaw[Index].type;
         data[CompSignal].HorPolPhase = TableAngleLaw[Index].law;
         //�������� ������ ��� ���������� ���������� ������
         data[CompSignal].HorPolPhaseArg = CreateArgList(PropLaw->M.Size);
         Arg = data[CompSignal].HorPolPhaseArg;
         //����������� ���������� ������ ��������� ���� �������������� ���������-
         //��� ����������� �������
         lSuccess = GetArgLawFromFile(s,PropLaw,Arg,CompSignal,PropSignal,CurRow);
         if (lSuccess == false) return lSuccess;
         break;
        case 9 : //���������������� ���� �������
         data[CompSignal].PhaseLawType = TableAngleLaw[Index].type;
         data[CompSignal].PhaseLaw = TableAngleLaw[Index].law;
         //�������� ������ ��� ���������� ���������� ������
         data[CompSignal].PhaseArg = CreateArgList(PropLaw->M.Size);
         Arg = data[CompSignal].PhaseArg;
         //����������� ���������� ������ ��������� ���������������� ���� �������
         lSuccess = GetArgLawFromFile(s,PropLaw,Arg,CompSignal,PropSignal,CurRow);
         if (lSuccess == false) return lSuccess;
         break;
      }
   }
   else if (PropSignal == 8) //��������� �������
   { Index = GetIndexLawByType(TableAmplLaw,TableAmplLawCount,LawType);
     PropLaw = (TableAmplLaw+Index);
     data[CompSignal].AmplLawType = TableAmplLaw[Index].type;
     data[CompSignal].AmplLaw = TableAmplLaw[Index].law;
     //�������� ������ ��� ���������� ���������� ������
     data[CompSignal].AmplArg = CreateArgList(PropLaw->M.Size);
     Arg = data[CompSignal].AmplArg;
     //����������� ���������� ������ ��������� ��������� �������
     lSuccess = GetArgLawFromFile(s,PropLaw,Arg,CompSignal,PropSignal,CurRow);
     if (lSuccess == false) return lSuccess;
   }
   else if (PropSignal > 9)
   { Index = GetIndexLawByType(TableCasualLaw,TableCasualLawCount,LawType);
     PropLaw = (TableCasualLaw+Index);
     switch (PropSignal)
     { case 10 : //��������� ����� ��������� ��������� �������
        data[CompSignal].AmplCasualLawType = TableCasualLaw[Index].type;
        data[CompSignal].AmplCasualLaw = TableCasualLaw[Index].law;
        //�������� ������ ��� ���������� ���������� ������
        data[CompSignal].AmplCasualLawArg = CreateArgList(PropLaw->M.Size);
        Arg = data[CompSignal].AmplCasualLawArg;
        //����������� ���������� ������ ��������� ��������� ��������� �������
        lSuccess = GetArgLawFromFile(s,PropLaw,Arg,CompSignal,PropSignal,CurRow);
        if (lSuccess == false) return lSuccess;
        break;
       case 11 : //��������� ����� ��������� ���� �������
        data[CompSignal].PhaseCasualLawType = TableCasualLaw[Index].type;
        data[CompSignal].PhaseCasualLaw = TableCasualLaw[Index].law;
        //�������� ������ ��� ���������� ���������� ������
        data[CompSignal].PhaseCasualLawArg = CreateArgList(PropLaw->M.Size);
        Arg = data[CompSignal].PhaseCasualLawArg;
        //����������� ���������� ������ ��������� ��������� ���� �������
        lSuccess = GetArgLawFromFile(s,PropLaw,Arg,CompSignal,PropSignal,CurRow);
        if (lSuccess == false) return lSuccess;
        break;
     }
   }
  }
return lSuccess;
}
//*****************************************************************************
//������: I.19., private, class member-function
//������������ �������:
//bool GetArgLawFromFile(string& s, LAW* PropLaw, ARGLIST* Arg, uint CompSignal,
//                       uint PropSignal, ulong& CurRow);
//����������:
//������� ������������� ��� ��������� ���������� ������ ��������� ��������������
//���������� �������� �������. ��� ��������� ����� ���� ������� ���� double.
//��������� ����� ������������� �� ���������� �������. ��� ���� ����������� ���-
//������ ����������� ����� ���������� ����������: <Space,;Tab>, ������ �������
//� ������� ��� ������������� ��������� �� ����������� (���� ������ ��� �� �����-
//����������� ��������� //). � ������ ������������� ������� ������� �����, �����-
//����� ��������� ������ ��� �������������� �� ����� - ���������� ����������,
//������������ ������ � ������� ��������� ������ � ���� signal.err. ���� �����
//���������� ��������� ����������� �����, �� ��� ������������. ���� ������ ����-
//����� ���������� ��������� �� ������ �������, �� ��� �������� � ������ �������-
//��� ���������� �������� ���������� �������. ����������� ��������� �������������
//������������� � �����������, ������� ���������� � �������� �������� � ��������
//�������� �������� ������� � �������� ��� �������, ���������� � �������� ����-
//��� (��������, ������� �� DEG to RAD, �� Hz to RAD/SEC � �.�.). ������� ������-
//��� ��������� ��������� � ������ ���������� ������� ��������������� ����������-
//��� ��������� �������� �������. ������ ���������� ���� ARGLIST ��������� ���-
//��� ��������. ��� ���� ����������������� ������� ������ ���������� � ARGLIST
//���������� �� ������� �������� ������, �.�. ������ � ����� ������ ������ ���-
//����� �������� ������� - ���������� ��������. ��� ��������� ������� ����� ���-
//��� ����� ������������� ������, �.�. ��� �� ������� �� ��������� (��������,t).
//-----------------------------------------------------------------------------
//������������ ���������:
//1. string& s - ������ � ����������� ������ ��������� �������� �������
//2. LAW* PropLaw - ����� ��������� �������� PropSignal ���������� �������
//3. ARGLIST* Arg - ������ ���������� ������ PropLaw
//4. uint CompSignal - ���������� ����� ���������� �������� �������
//5. uint PropSignal - ���������� ����� �������� ���������� �������� �������
//6. ulong& CurRow - ����� ������� ������ ����� fname
//-----------------------------------------------------------------------------
//������������ ��������: bool lSuccess
// true  - ��������� ������ ��������� �������� ������������ ������� �������
//         ���������
// false - ��������� ������ ��������� �������� ������������ ������� �������
//         �� ���������
//*****************************************************************************
bool ExtSignal::GetArgLawFromFile(string& s, LAW* PropLaw, ARGLIST* Arg,
                uint CompSignal, uint PropSignal, ulong& CurRow)
{ bool lSuccess = true;
  uint Count = PropLaw->M.Size; //���������� ���������� ������

  //������������� ����� ��� ����������: ��������� ��������� ��� ���� �������
  if ((PropSignal > 9) && (Count == 1)) return lSuccess;

  //�������� ������ ����������
  //Arg = CreateArgList(Count);
  //����������������� ����� ��� ����������, ������ � ���������� (����., ZERO_LAW)
  if ((Count == 1) && (PropSignal < 10)) return lSuccess;

  //����������� ������������ ���������� ���������� ������ � ���������� �������,
  //� �������� ��� ������ ���������� � ������ ARGLIST, � ����������� �� ����
  //������: ������������������ ��� ��������������.
  uint offset = 1;
  Count  = Count-1;
  uint GetParam = 0; //���������� ��������� ����������
  uint pos;
  string separator = " ,;\t";
  string realnum = "-+.0123456789Ee";
  string sep_rnum(" ,;\t-+.0123456789Ee");
  bool lFind;
  //���������� ���������� ������ ��������� �������������� ���������� �������
  //�� ����� � ��������� �������

  while (GetParam != Count)
  {//�������� ������ ������, �� ����c������ � ��������-������������
   pos = s.find_first_not_of(separator);
   //������ �������� �� ����������� ����� � ������ ������ �� �������
   if (pos == string::npos)
   { //��������� ����� ������ �� �����, ��������� �����������
     lFind = false;
     while ((fin.eof() == 0) && (!lFind))
     { getline(fin,s,'\n');
       CurRow++;
       pos = s.find("//");
       if (pos != 0)
       { pos = (pos != string::npos) ? pos : s.length();
         s = s.substr(0,pos);
         //����� ������� ������� � ������, �� ������������ � ������������
         pos = s.find_first_not_of(separator);
         if (pos != string::npos) //������ �������� �� ����������� ������
          lFind = true;
       }
     }//����� ����� ���������� ����� �� �����
      //��������� ����� �����, ������� �� ��� ��������� ������ ���������
      //���������� �������
      if (lFind == false)
      { lSuccess = false;
       ferr << "ERROR: End of file is reached. It was not possible to receive\n";
       ferr << "       all parameters of law modification of a signal component."
            << endl;
       ferr << "Signal Component: " << CompSignal << endl;
       ferr << "Signal Property: " << SignalPropertyName[PropSignal] << endl;
       ferr << "Line N " << CurRow << ": <" << s << ">" << endl;
       ferr << "Abbreviate Law: " << GetAbbrLawByType(PropLaw->type) << endl;
       ferr << "Total Law Parameters: " << Count << " without argument" <<endl;
       ferr << "Getted Law Parameters: " << GetParam << endl;
       DeleteArgList(Arg);
       return lSuccess;
      }
   }
   //������ �������� �� �������-����������� ������
   //����������� ������ �� ���������� ������ ������������� ������������� �����
   //������ ������������ ������ � ������ ���������� ������
   if (s.find_first_not_of(sep_rnum) != string::npos) //realnum+separator
   { lSuccess = false;
     ferr << "ERROR: Incorrect token in law parameters line of signal component"
          <<" performance." << endl;
     ferr << "Signal Component: " << CompSignal << endl;
     ferr << "Signal Property: " << SignalPropertyName[PropSignal] << endl;
     ferr << "Abbreviate Law: " << GetAbbrLawByType(PropLaw->type) << endl;
     ferr << "Total Law Parameters: " << Count << " without argument" <<endl;
     ferr << "Getted Law Parameters: " << GetParam << endl;
     ferr << "Line N " << CurRow << ": <" << s << ">" << endl;
     ferr << "EXPECTED TOKENS: " << endl;
     ferr << "SEPAROTORS: " << "<" <<separator << ">" << endl;
     ferr << "REALNUMBER TOKENS: " << "<" <<realnum << ">" << endl;
    DeleteArgList(Arg);
    return lSuccess;
   }
   s = s.substr(pos);
   double value;
   char *endptr;
   //����������� �������� ������ �� ����������� ������������� � ������������
   //���� double
   value = strtod(s.c_str(), &endptr);
   //����������� ���������� ��������
   if ((value == +HUGE_VAL) || (value == -HUGE_VAL) || (endptr == 0))
   {//������������ �����
    lSuccess = false;
    ferr << "ERROR: Incorrect token in law parameters line of signal component"
         <<" performance." << endl;
    ferr << "Signal Component: " << CompSignal << endl;
    ferr << "Signal Property: " << SignalPropertyName[PropSignal] << endl;
    ferr << "Abbreviate Law: " << GetAbbrLawByType(PropLaw->type) << endl;
    ferr << "Total Law Parameters: " << Count << " without argument" <<endl;
    ferr << "Getted Law Parameters: " << GetParam << endl;
    ferr << "Line N " << CurRow << ": <" << s << ">" << endl;
    ferr << "EXPECTED TOKENS: " << endl;
    ferr << "SEPARATORS: " << "<" << separator << ">" << endl;
    ferr << "REALNUMBER TOKENS: " << "<" <<realnum << ">" << endl;
    DeleteArgList(Arg);
    return lSuccess;
   }
   //����������� ���������� �������� �� �������� ����������� � ��������
   value = PropLaw->M.Factor[offset+GetParam].Convert(value);
   //������� �������� ������ � ������ ����������
   Arg->List[offset+GetParam] = value;
   //�������� c�������� ��������
   GetParam++;
   pos = s.find_first_of(separator);
   if (pos == string::npos) s = " ";
   else s = s.substr(pos);
  }//����� ����� ���������� ���������� ������
return lSuccess;
}
//----------------------------------------------------------------------------

//*****************************************************************************
// II. ������������� ������� - ����� ������ ExtSignal
//    PUBLIC MEMBER - FUNCTION ExtSignal CLASS
//*****************************************************************************

//*****************************************************************************
//������: II.1; public; constructor by default
//������������ �������: ExtSignal()
//����������: ����������� �� ��������� ������ ExtSignal.
//������� ������������� ��� �������� ������ ExtSignal �� ���������. � ������ ��-
//���� �������� ������� ������ �������������� ���������� ���� ������������� ����
//��������� �������� �������
//-----------------------------------------------------------------------------
//������������ ���������: ���
//-----------------------------------------------------------------------------
//������������ ��������: ���
//*****************************************************************************
ExtSignal::ExtSignal()
{
 data = NULL; Count = 0; fname.resize(0);
 //�������� ������� ������ � ������ ����������� �����
 #if defined(CONSOLE)
  clrscr(); //������� ������ �������
  cout << "*******************************************************************\n";
  cout << "*           Designing of an EXTERNAL SIGNAL by default.           *\n";
  cout << "* All performances component of external signal are necessary for *\n";
  cout << "* entering from the keyboard.                                     *\n";
  cout << "*******************************************************************\n";
  cout << "* EXTERNAL SIGNAL is always ONE useful signal and N of parasites. *\n";
  cout << "* The number of parasites N can be in a range from 0 up to 65535. *\n";
  cout << "*******************************************************************\n";
  cout << endl;
  cout << "Enter total of parasites N: ";
  cin >> Count;
  //���������� ������������ �������� ������� � ������ ��������� �������
  Count += 1;
  //������������� ������ ��� ���������� �������� �������
  CreateArray(Count);
  //��������� ��������� ���� ������������� �������� �������
  for (uint i = 0; i < Count; i++) ResetProperty(i);

  //�������� ������� ��������� ������� ��������� ������������� ���� � ����
  //����� ���������� �������� �������, � ����� ����������� � ������� (�������-
  //������� � ������������) ������������ ���� ����������� �������.
  if (TableAngleLawCount == 0) CreateTableOfAngleLaw();

  //�������� ������� ����������������� ������� ��������� �� ������� �����������
  //������������ ���������� �������� �������
  if (TableAmplLawCount == 0) CreateTableOfAmplLaw();

  //�������� ������� ��������� ������� ��������� ����������� � ������� �������-
  //������� ���������� �������� �������
  if (TableCasualLawCount == 0) CreateTableOfCasualLaw();

  clrscr(); gotoxy(1,1);
  cout << "********************************************************************"
       << endl;
  cout << " EXTERNAL SIGNAL STRUCTURE: Desired Signal - 1, Interference - "
       << (Count-1) << endl;
  cout << "********************************************************************"
       << endl;
  //���� ������� ������ ���������� �������� ������� � ����������
  for (uint i = 0; i < Count; i++)
   do ConsoleDataEntry(i);
   //����� ������������� ���������� � ��������������
   while(!ConsoleDataOutPut(i,SDIM,true));
  clrscr();
 #endif
}
//*****************************************************************************
//������: II.2; public; constructor with argument
//������������ �������: ExtSignal(const char* filename)
//����������: ����������� � ���������� ������ ExtSignal.
//������� ������������� ��� �������� ������ ExtSignal �� ������ ����������� �
//��������� ����� filename. ���� ���� ���������� �����������. ��� ����������
//������ �� ����� ����� �������������� �������� ��� ������������. � ������ ���-
//��� ��� �������������� ��������� ����� ���������� ������� ������������ �����-
//��������� ���������� ������ ������������, ������, ��������� ���������� ������-
//�������� � ����� signal.err
//-----------------------------------------------------------------------------
//������������ ���������: const char* filename - ��� �������� �����
//-----------------------------------------------------------------------------
//������������ ��������: ���
//*****************************************************************************
ExtSignal::ExtSignal(const char* filename)
{bool lSuccess;

 Count = 0; data = NULL;
 fname.resize(0);
 lSuccess = FileDataEntry(filename);
 if (lSuccess == true)
 {fin.close();
  //�������� ���������� ����� ����������� ������
  ferr.close();
  remove(ferrname.c_str());
  #if defined(CONSOLE)
   clrscr();
   cout << "The input data on EXTERNAL SIGNAL components from file <" << fname
        << ">" << "\nare read out successfully." << endl;
   cout << "Total noise: " << (Count-1) << endl;
   cout << "Press any key ...";
   getch();
  #endif
 }
 else if (lSuccess == false) //������ ��� ���������� ������ �� �����
 {
  #if defined(CONSOLE)
   cout << endl;
   cout << "The error is detected at reading of datas from the file." << endl;
   cout << "The EXTERNAL SIGNAL design on file data is impossible." << endl;
   cout << "The program will be terminated." << endl;
   cout << "Press any key ...";
   getch();
  #endif

  #if defined(CONSOLE)
   cout << "\nDestroy object of EXTERNAL SIGNAL class..." << endl;
   cout << "Delete law tables." << endl;
  #endif
  fname.resize(0);
  DeleteTable(TableAngleLaw, TableAngleLawCount);
  DeleteTable(TableAmplLaw, TableAmplLawCount);
  DeleteTable(TableCasualLaw, TableCasualLawCount);

  #if defined(CONSOLE)
   cout << "Delete " << Count << " components EXTERNAL SIGNAL." << endl;
  #endif
  for (uint i = 0; i < Count; i++) ResetProperty(i);
  delete[] data;
  Count = 0;

  exit(-1);
 }
}

//*****************************************************************************
//������: II.3; public; class destructor
//������������ �������: ~ExtSignal()
//����������: ���������� ������ ExtSignal.
//������� ������������� ��� ���������� ������ ExtSignal. ������� ������� �� ����-
//�� ������� ������� ��������� ������������� ��������� �������� ������� � ������
//(� ���� ������� �������� COMPONENT) c����������� �������� �������.
//������ ��������� ������ �������:
//1. TableAngleLaw 2. TableAmplLaw 3. TableCasualLaw
//-----------------------------------------------------------------------------
//������������ ���������: ���
//-----------------------------------------------------------------------------
//������������ ��������: ���
//*****************************************************************************
ExtSignal::~ExtSignal()
{
 //�������� ������ ������� �� ������
 #if defined(CONSOLE)
  cout << "\nDestroy object of EXTERNAL SIGNAL class..." << endl;
  cout << "Delete law tables." << endl;
 #endif
  fname.resize(0);
  DeleteTable(TableAngleLaw, TableAngleLawCount);
  DeleteTable(TableAmplLaw, TableAmplLawCount);
  DeleteTable(TableCasualLaw, TableCasualLawCount);

 #if defined(CONSOLE)
  cout << "Delete " << Count << " components EXTERNAL SIGNAL." << endl;
 #endif
  for (uint i = 0; i < Count; i++) ResetProperty(i);
  delete[] data;
  Count = 0;

 #if defined(CONSOLE)
  cout << "EXTERNAL SIGNAL destroyed." << endl;
 #endif
}


//******************************************************************************
//III. ���������� ���������� �������� ������� � ��� ���������
//�������� ������-������� ������ ExtSignal �� ���������� ���������� ������������
//�������� �������: ���������� �������� �������� � ���, ����������� ���������� �
//������ � ��� ����� ��������������� ������� ��������� �������� �������, ������-
//��� ��������� �������, ���������� �������, ��������� ������ � �.�.
//******************************************************************************

//*****************************************************************************
//������: III.1; public; ExtSignal class member-function
//������������ �������: void SetTime(double time)
//����������:
//������� ������������� ������� ��������� ����� ��� ���������� �������������
//��������� �������� �������, ������� ���������� �� ������� ��������� �� �������.
//�������� ���������� ������� ��������� � ������ ���������� ���� ������� ���
//�������� ������ 0. �������������� ��������� �������� ������� ��������� �� t:
//1. AzimuthAngle - ARGLIST* AzAngleArg
//2. ElevateAngle - ARGLIST* ElevAngleArg
//3. PolAngle     - ARGLIST* PolAngleArg
//4. AmplLaw      - ARGLIST* AmplArg
//5. VertPolPhase - ARGLIST* VertPolPhaseArg
//6. HorPolPhase  - ARGLIST* HorPolPhaseArg
//7. PhaseLaw     - ARGLIST* PhaseArg
//-----------------------------------------------------------------------------
//������������ ���������:
//1. double time - ������� ��������� �����
//-----------------------------------------------------------------------------
//������������ ��������: ���
//*****************************************************************************
void ExtSignal::SetTime(double time)
{
 for (uint i = 0; i < Count; i++)
 {
  data[i].AzAngleArg->List[0] = time;
  data[i].ElevAngleArg->List[0] = time;
  data[i].PolAngleArg->List[0] = time;
  data[i].AmplArg->List[0] = time;
  data[i].VertPolPhaseArg->List[0] = time;
  data[i].HorPolPhaseArg->List[0] = time;
  data[i].PhaseArg->List[0] = time;
 }
}
//*****************************************************************************
//������: III.2; public; ExtSignal class member-function
//������������ �������: void SetRandNumber();
//����������:
//������� �������� ������� �������� ��������� �������� � ��� ��� ����������
//������������� ���� ��������� �������� �������.
//��������� �������� �������� (���) ��������� � ������ ���������� ���������
//������� ��� �������� ������ 0. ������ ������� ��������� ��������� ��������
//�������� ��������� � ���������� ������, ������������ ��������� ���� ��� �����-
//���� ���������� �������� �������, ��� ����������� ���������� �������� �������-
//��� ��� ��������� ��������� ������� � ���� � ��� �� ������ ������� t. �����
//������� ����� ���������, ��� ��� ��������� � ���� ���������� �������� �������
//��������������� �� �������������� � ������������ ��������������� ������������.
//�������������, ���������� ��������� ���������� ��������� �������� ��������� �
//���� ��� ���� ������������ ���������� �������. ������� ��������� � ���������
//�������� ������� �� � ��������� ������ ��������� �������� ��������� (����), ���
//�������� ������������. ��������� �������������� ��������� �������� �������:
//1. AmplCasualLaw  - ARGLIST* AmplCasualLawArg
//2. PhaseCasualLaw - ARGLIST* PhaseCasualLawArg
//-----------------------------------------------------------------------------
//������������ ���������: ���
//-----------------------------------------------------------------------------
//������������ ��������: ���
//*****************************************************************************
void ExtSignal::SetRandNumber()
{
 for (uint i = 0; i < Count; i++)
 {
  data[i].AmplCasualLawArg->List[0]  = data[i].AmplCasualLaw(data[i].AmplCasualLawArg);
  data[i].PhaseCasualLawArg->List[0] = data[i].PhaseCasualLaw(data[i].PhaseCasualLawArg);
 }
}
//*****************************************************************************
//������: III.3; public; ExtSignal class member-function
//������������ �������: uint ActiveJammers() const
//����������: ������� ���������� ����������� �������� ����� � Usage == ON
//-----------------------------------------------------------------------------
//������������ ���������: ���
//-----------------------------------------------------------------------------
//������������ ��������: uint amount - ���-�� ����������� �������� �����
//*****************************************************************************
uint ExtSignal::ActiveJammers() const
{uint amount = Jammers();
 if (amount == 0) return amount;
 for (uint i = 1; i < Count; i++) {if (data[i].Usage == OFF) amount--;}
return amount;
};
//*****************************************************************************
//������: III.4; public; ExtSignal class member-function
//������������ �������:
// double AmplSignalComp(uint Number, uint PolarCompType = NONPOLAR) const
//����������:
//������� ���������� �������� ���������� ��������� ���������� �������� �������
//� ���������� ������� Number. Number == 0 - ������������� ��������� �������,
// 0 < Number < Count - ������ � ������� Number. ���� ������ � �������� �������
//Number �� ���������� � ������� data, �� ������������ ������� �������� �������-
//��. ���������� PolarCompType ��������� ��������� ��� �� ��������� ������������-
//��� �������� ���������� �������. ��� PolarCompType = V_POLAR ������������
//��������� �������, ��������������� ������������ ������������ ����������������
//��������� �������, ��� PolarCompType = H_POLAR - �������������� ������������
//���������������� ��������� �������, ��� PolarCompType = NONPOLAR (�������� ��
//���������) - ��������������� �������� ������� �� �����������.
//������� ��������� ���� Usage �� ������������� ������������� ���������� �������
//� ���������� ������� Number � ��������. ���� Usage == OFF (���������� � �����-
//��� �� ������������), �� ������� ���������� ampl = 0.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. uint Number - ���������� ����� ���������� External Signal � ������� data
//2. uint PolarCompType - ��� ������������ ����������� �������.
//-----------------------------------------------------------------------------
//������������ ��������: double ampl - ���������� �������� ��������� ����������
//�������� ������� � ���������� ������� Number
//*****************************************************************************
double ExtSignal::AmplSignalComp(uint Number, uint PolarCompType) const
{ double ampl = 0.0;
  //���������� �������� ������� � ����� ������� �� ����������
  if ((Count == 0) || (Number >= Count)) return ampl;
  //�������� �� ������������� ���������� ������� � ������� Number � ��������
  //��������� �������� ������� � �������� �� ������ ��������������
  if (data[Number].Usage == OFF) return ampl;

  ampl = data[Number].AmplLaw(data[Number].AmplArg) *
         data[Number].AmplCasualLawArg->List[0];
  //��������������� �������� ������� �� �����������
  if (PolarCompType == NONPOLAR) return ampl;
  //��������������� �������� ������� �����������
  //������������ ������������ ����������� ���������� �������
  if (PolarCompType == V_POLAR)
   ampl *= sin(data[Number].PolAngle(data[Number].PolAngleArg));
  //�������������� ������������ ����������� ���������� �������
  if (PolarCompType == H_POLAR)
   ampl *= cos(data[Number].PolAngle(data[Number].PolAngleArg));
 return ampl;
}
//*****************************************************************************
//������: III.5; public; ExtSignal class member-function
//������������ �������:
// double PhaseSignalComp(uint Number, uint PolarCompType = NONPOLAR) const
//����������:
//������� ���������� �������� ���������� ���� ���������� �������� �������
//� ���������� ������� Number. Number == 0 - ������������� ��������� �������,
// 0 < Number < Count - ������ � ������� Number. ���� ������ � �������� �������
//Number �� ���������� � ������� data, �� ������������ ������� �������� ����.
//���������� PolarCompType ��������� ��������� ��� �� ��������� ���������������
//�������� ���������� �������. ��� PolarCompType = V_POLAR ������������
//���� �������, ��������������� ������������ ������������ ����������������
//��������� �������, ��� PolarCompType = H_POLAR - �������������� ������������
//���������������� ��������� �������, ��� PolarCompType = NONPOLAR (�������� ��
//���������) - ��������������� �������� ������� �� �����������.
//������� ��������� ���� Usage �� ������������� ������������� ���������� �������
//� ���������� ������� Number � ��������. ���� Usage == OFF (���������� � �����-
//��� �� ������������), �� ������� ���������� phase = 0.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. uint Number - ���������� ����� ���������� External Signal � ������� data
//2. uint PolarCompType - ��� ������������ ����������� �������.
//-----------------------------------------------------------------------------
//������������ ��������: double phase - ���������� �������� ���� ����������
//�������� ������� � ���������� ������� Number (� ��������)
//*****************************************************************************
double ExtSignal::PhaseSignalComp(uint Number, uint PolarCompType) const
{ double phase = 0.0;
  //���������� �������� ������� � ����� ������� �� ����������
  if ((Count == 0) || (Number >= Count)) return phase;
  //�������� �� ������������� ���������� ������� � ������� Number � ��������
  //��������� �������� ������� � �������� �� ������ ��������������
  if (data[Number].Usage == OFF) return phase;

  phase = data[Number].PhaseLaw(data[Number].PhaseArg) +
          data[Number].PhaseCasualLawArg->List[0];

  //��������������� �������� ������� �� �����������
  if (PolarCompType == NONPOLAR) return phase;
  //��������������� �������� ������� �����������
  //������������ ������������ ����������� ���������� �������
  if (PolarCompType == V_POLAR)
   phase += data[Number].VertPolPhase(data[Number].VertPolPhaseArg);
  //�������������� ������������ ����������� ���������� �������
  if (PolarCompType == H_POLAR)
   phase += data[Number].HorPolPhase(data[Number].HorPolPhaseArg);

 return phase;
}
//*****************************************************************************
//������: III.6; public; ExtSignal class member-function
//������������ �������:
// double DetAmpl(uint Number, uint PolarCompType = NONPOLAR) const
//����������:
//������� ���������� �������� ���������� ��������� ���������� �������� �������
//� ���������� ������� Number ��� ��������� ������������. Number == 0 - ���������-
//���� ��������� �������, 0 < Number < Count - ������ � ������� Number. ���� ��-
//���� � �������� ������� Number �� ���������� � ������� data, �� ������������
//������� �������� ���������. ���������� PolarCompType ��������� ��������� ���
//�� ��������� ��������������� �������� ���������� �������.
//��� PolarCompType = V_POLAR ������������ ��������� �������, ���������������
//������������ ������������ ���������������� ��������� �������, ���
//PolarCompType = H_POLAR - �������������� ������������ ���������������� ������-
//��� �������, ��� PolarCompType = NONPOLAR (�������� �� ���������) - ��������-
//������� �������� ������� �� �����������.
//������� ��������� ���� Usage �� ������������� ������������� ���������� �������
//� ���������� ������� Number � ��������. ���� Usage == OFF (���������� � �����-
//��� �� ������������), �� ������� ���������� ampl = 0.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. uint Number - ���������� ����� ���������� External Signal � ������� data
//2. uint PolarCompType - ��� ������������ ����������� �������.
//-----------------------------------------------------------------------------
//������������ ��������: double ampl - ���������� �������� ��������� ����������
//�������� ������� � ���������� ������� Number ��� ��������� ������������.
//*****************************************************************************
double ExtSignal::DetAmpl(uint Number, uint PolarCompType) const
{ double ampl = 0.0;
  //���������� �������� ������� � ����� ������� �� ����������
  if ((Count == 0) || (Number >= Count)) return ampl;
  //�������� �� ������������� ���������� ������� � ������� Number � ��������
  //��������� �������� ������� � �������� �� ������ ��������������
  if (data[Number].Usage == OFF) return ampl;

  ampl = data[Number].AmplLaw(data[Number].AmplArg);

  //��������������� �������� ������� �� �����������
  if (PolarCompType == NONPOLAR) return ampl;
  //��������������� �������� ������� �����������
  //������������ ������������ ����������� ���������� �������
  if (PolarCompType == V_POLAR)
   ampl *= sin(data[Number].PolAngle(data[Number].PolAngleArg));
  //�������������� ������������ ����������� ���������� �������
  if (PolarCompType == H_POLAR)
   ampl *= cos(data[Number].PolAngle(data[Number].PolAngleArg));
 return ampl;
}
//*****************************************************************************
//������: III.7; public; ExtSignal class member-function
//������������ �������:
// double DetPhase(uint Number, uint PolarCompType = NONPOLAR) const
//����������:
//������� ���������� �������� ���������� ���� ���������� �������� �������
//� ���������� ������� Number ��� ��������� ������������ ����. Number == 0 -
//������������� ��������� �������, 0 < Number < Count - ������ � ������� Number.
//���� ������ � �������� ������� Number �� ���������� � ������� data, �� ������-
//������ ������� �������� ����.
//���������� PolarCompType ��������� ��������� ��� �� ��������� ���������������
//�������� ���������� �������. ��� PolarCompType = V_POLAR ������������
//���� �������, ��������������� ������������ ������������ ����������������
//��������� �������, ��� PolarCompType = H_POLAR - �������������� ������������
//���������������� ��������� �������, ��� PolarCompType = NONPOLAR (�������� ��
//���������) - ��������������� �������� ������� �� �����������.
//������� ��������� ���� Usage �� ������������� ������������� ���������� �������
//� ���������� ������� Number � ��������. ���� Usage == OFF (���������� � �����-
//��� �� ������������), �� ������� ���������� phase = 0.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. uint Number - ���������� ����� ���������� External Signal � ������� data
//2. uint PolarCompType - ��� ������������ ����������� �������.
//-----------------------------------------------------------------------------
//������������ ��������: double phase - ���������� �������� ���� ����������
//�������� ������� � ���������� ������� Number (� ��������) ��� ���������
//������������ ����.
//*****************************************************************************
double ExtSignal::DetPhase(uint Number, uint PolarCompType) const
{ double phase = 0.0;
  //���������� �������� ������� � ����� ������� �� ����������
  if ((Count == 0) || (Number >= Count)) return phase;
  //�������� �� ������������� ���������� ������� � ������� Number � ��������
  //��������� �������� ������� � �������� �� ������ ��������������
  if (data[Number].Usage == OFF) return phase;

  phase = data[Number].PhaseLaw(data[Number].PhaseArg);

  //��������������� �������� ������� �� �����������
  if (PolarCompType == NONPOLAR) return phase;
  //��������������� �������� ������� �����������
  //������������ ������������ ����������� ���������� �������
  if (PolarCompType == V_POLAR)
   phase += data[Number].VertPolPhase(data[Number].VertPolPhaseArg);
  //�������������� ������������ ����������� ���������� �������
  if (PolarCompType == H_POLAR)
   phase += data[Number].HorPolPhase(data[Number].HorPolPhaseArg);

 return phase;
}
//*****************************************************************************
//������: III.8; public; ExtSignal class member-function
//������������ �������: complex CalcSumUJammer(uint PolarCompType)
//����������:
//������� ��������� ��������� ����������� ���������� ���� ����� �������� �������.
//���������� PolarCompType ��������� ��������� ��� �� ��������� ���������������
//�������� ���������� �������. ��� PolarCompType = V_POLAR ������������
//���� �������, ��������������� ������������ ������������ ����������������
//��������� �������, ��� PolarCompType = H_POLAR - �������������� ������������
//���������������� ��������� �������, ��� PolarCompType = NONPOLAR (�������� ��
//���������) - ��������������� �������� ������� �� �����������.
//������� ��������� ���� Usage �� ������������� ������������� ���������� �������
//� ���������� ������� Number � ��������. ���� Usage == OFF (���������� � �����-
//��� �� ������������), �� ������ ������ � �������� �� �����������.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. uint PolarCompType - ��� ������������ ����������� �������.
//-----------------------------------------------------------------------------
//������������ ��������: complex UNoise - ���������� �������� ����������
//������������ ���������� ���� ����� �������� �������
//*****************************************************************************
complex ExtSignal::CalcSumUJammer(uint PolarCompType)
{ complex UNoise(0.0,0.0);
  //������� ������ ����� �� ��������
  if (Jammers() == 0) return UNoise;

  for (uint i = 1; i < Count; i++)
   //�������� �� ������������� ������ � ������� i � ��������
   if (data[i].Usage == ON)
    UNoise += polar(AmplSignalComp(i,PolarCompType),
                    PhaseSignalComp(i, PolarCompType));
 return UNoise;
}
//*****************************************************************************
//������: III.9; public; ExtSignal class member-function
//������������ �������: complex CalcSumPJammer(uint PolarCompType)
//����������:
//������� ��������� ��������� ����������� �������� ���� ����� �������� �������.
//���������� PolarCompType ��������� ��������� ��� �� ��������� ���������������
//�������� ���������� �������. ��� PolarCompType = V_POLAR ������������
//���� �������, ��������������� ������������ ������������ ����������������
//��������� �������, ��� PolarCompType = H_POLAR - �������������� ������������
//���������������� ��������� �������, ��� PolarCompType = NONPOLAR (�������� ��
//���������) - ��������������� �������� ������� �� �����������.
//������� ��������� ���� Usage �� ������������� ������������� ���������� �������
//� ���������� ������� Number � ��������. ���� Usage == OFF (���������� � �����-
//��� �� ������������), �� ������ ������ � �������� �� �����������.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. uint PolarCompType - ��� ������������ ����������� �������.
//-----------------------------------------------------------------------------
//������������ ��������: complex UNoise - ���������� �������� ����������
//������������ ���������� ���� ����� �������� �������
//*****************************************************************************
double ExtSignal::CalcSumPJammer(uint PolarCompType)
{ double PNoise = 0.0;
  //������� ������ ����� �� ��������
  if (Jammers() == 0) return PNoise;

  for (uint i = 1; i < Count; i++)
   //�������� �� ������������� ������ � ������� i � ��������
   if (data[i].Usage == ON)
    PNoise += norm(polar(AmplSignalComp(i,PolarCompType),
                         PhaseSignalComp(i, PolarCompType)));
 return PNoise;
}
//*****************************************************************************
//������: III.10; public; ExtSignal class member-function
//������������ �������: double SignalJammerRatio()
//����������:
//������� ��������� ��������� ������-������ (� ���������) �� ���������� �����-
//���� ��������� ��������� ������� � ���������� ������ (jammer)
//-----------------------------------------------------------------------------
//������������ ���������: ���
//-----------------------------------------------------------------------------
//������������ ��������: double SJR - ��������� ������-������ � �� (Db)
//*****************************************************************************
double ExtSignal::SignalJammerRatio()
{ double SJR;
  double SumPj = CalcSumPJammer();
  double Ps = CalcPSignal();
  //��������� �������� ������ ����� ����
  if (SumPj == 0.0) return 100.0;
  //�������� ������� ����� ����
  if (Ps == 0.0) return -100.0;
  //������ ��������� ������-������
  SJR = 10*log10(Ps/SumPj);
 return SJR;
}
