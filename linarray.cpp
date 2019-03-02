//linarray.cpp
//���������� ������ TLinearArray - �������� �������� ������� (LINEAR ARRAY)
//Initial date: October,1,1998. Final date:
//Copyright (c) JohnSoft 1998. All rights reserved. Borland C++ 5.01

#include "linarray.h"

//******************************************************************************
//���������� ���������� ����������, ������� ������������ ��� ���������� �������-
//������ �������� �������� ������� �� ���������� ����� ������������ ����.
//��������� �������������� �������� ������� ����� ������������� ��� �� �����
//������, ��� � �� ���������� (��������, � ������ ���� ������������� ����������
//����� ���������� �� ������ � ��������� ���� � ���������� ��������� �������
//������).
//******************************************************************************
string Buffer;//������� ���������� ������ ��������� �� ��������� ������ fin
string Line; //������� ������ ������ ��������� �� �������� ��������� ������ fin
ulong CurRow = 0;//���������� ����� ������� ������ Buffer

//�������-����������� ����������� �� ������ � �������. ���� ����� ���������
//������ ��������� �� ������� ��������� ��������� �������������.
const uint sz_CommentSep = 3;
const char* CommentSep[3] = {"//","#","rem"};

//������� ������������ ���������� (����������) ��������. ������������ ��� �����-
//����� �������� ������. ��� ������ ���� ��������-������������ ���������������
//�������, ���������� �� �� �������� ��������. � ����� ���������, ����� ��������
//�������� �������, �� �������� ��� �������� ������ (�������) �������� ���������
//���������� �����������  �������� ������������ �����������.
const uint sz_AssignSep = 2;
const char* AssignSep[2] = {":=","="};

const string EngABC = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const string Engabc = "abcdefghijklmnopqrstuvwxyz";
const string RusABC = "�����Ũ��������������������������";
const string Rusabc = "��������������������������������";
const string Digits = "0123456789";
const string Sign = "-+";
const string RealNumber= "-+0123456789.eE";   //Sign+Digits+".eE";

//������ ������������� �������� �������� �������
const uint sz_LAAPerform = 7;
const char* LAAPerformPattern[7] = {
 "   LINEAR ARRAY PROPERTIES:",
 "1. CONFIGURATION: LEFT = < value >; TOGETHER = < value >; RIGHT = < value >;",
 "2. UNIFORMITY: < Uniform || Nonuniform >;",
 "3. EQUIDISTANCE: <Equidistant || Unequidistant>; LV = <value>;",
 "4. LEFTDISTLAW: <law name>; PARAMETERS: <value1,value2,...,valueN>;",
 "5. RIGHTDISTLAW: <law name>; PARAMETERS: <value1,value2,...,valueN>;",
 "6. DISPOSITION: <Equator || Meridian>;"
};

//������ ��������� ������� �������� �������� �������
const uint sz_LAAHeader = 3;
const char* LAAHeaderPattern[3] = { "LINEAR ARRAY PROPERTIES",
 "LINEAR ANTENNA ARRAY PROPERTIES", "�������� �������� �������� �������"};

//������ ������������ �������� <CONFIGURATION> �������� �������
const uint sz_LAAConfig = 3;
const char* LAAConfigPattern[3] = {"CONFIGURATION","������������", "CONFIG"};

//������ ������������ �������� <LEFT> �������� ������� - ���������� ���������
//�������� �������, ������������� ����� �� ��������� (��������) �������� ��
const uint sz_LAALeft = 4;
const char* LAALeftPattern[4] = {"TO THE LEFT","FROM THE LEFT","LEFT","�����"};

//������ ������������ �������� <TOGETHER> �������� ������� - ���������� ���������
//�������� �������, ������������� ������ � �������� (�������) ��������� ��
const uint sz_LAATogether = 2;
const char* LAATogetherPattern[2] = {"TOGETHER","������"};

//������ ������������ �������� <RIGHT> �������� ������� - ���������� ���������
//�������� �������, ������������� ������ �� ��������� (��������) �������� ��
const uint sz_LAARight = 4;
const char* LAARightPattern[4] = {"TO THE RIGHT","FROM THE RIGHT","RIGHT",
                                  "������"};

//������ ������������ �������� <UNIFORMITY> �������� �������
const uint sz_LAAUniform = 3;
const char* LAAUniformPattern[3] = {"UNIFORMITY","HOMOGENEITY","������������"};

const uint sz_TbUniform = 4;
const STR_BOOL TbUniform[4] = { {"NONUNIFORM",false},{"UNIFORM",true},
 {"������������",false},{"����������",true} };

//������ ������������ �������� <EQUIDISTANCE> �������� �������
const uint sz_LAAEquidist = 2;
const char* LAAEquidistPattern[2] = {"EQUIDISTANCE","����������������"};

const uint sz_TbEquidist = 6;
const STR_BOOL TbEquidist[6] = { {"UNEQUIDISTANT",false},{"EQUIDISTANT",true},
 {"����������������",false},{"��������������",true}, {"UNEQUAL",false},
 {"EQUAL",true} };

//������ ������������ �������������� ��, ������������ ������������� ����������
//����� ���������� �������������� ��
const uint sz_LAARelDist = 2;
const char* LAARelDistPattern[2] = { {"LV"}, {"RELATIVE DISTANCE"}};

//������ ������������ �������� <LEFTDISTLAW> - �����, ������������ �������������
//���������� ����� ����� ��������� ���������� ��, �������������� ����� ���������
//�������� ��
const uint sz_LAALfLaw = 2;
const char* LAALfLawPattern[2] = {"LEFTDISTLAW","LEFTLAW"};

//������ ������������ �������� <RIGHTDISTLAW> - �����, ������������ �������������
//���������� ����� ����� ��������� ���������� ��, �������������� ������ ���������
//�������� ��
const uint sz_LAARtLaw = 2;
const char* LAARtLawPattern[2] = {"RIGHTDISTLAW","RIGHTLAW"};

//������ ������������ �������� <PARAMETERS> - ���������� ������
const uint sz_LawParam = 3;
const char* LawParamPattern[3] = {"PARAMETERS", "PARAM", "���������"};

//������ ������������ �������� <DISPOSITION> �������� ������� - ������������ ��-
//������ �������� ������� ������������ �������������� ��� �������������� �������-
//��� �����
const uint sz_LAADisp = 2;
const char* LAADispPattern[2] = {"DISPOSITION","������������"};

const uint sz_TbDispos = 6;
const STR_BOOL TbDispos[6] = { {"MERIDIAN",false},{"EQUATOR",true},
 {"VERTICAL",false},{"HORIZONTAL",true}, {"�� ���������",false},
 {"�� �����������",true} };

//���������� ������� �������, ��������������� ��� ����������� �������������
//���������� ����� ��������� ���������� ��, �������������� ����� � ������ �� ��-
//������� �������� ��, � ������ ���������������� ��.
//1. ������������ ����������� ������ F(x) = A;
uint sz_ConstLawName = 3;
const char* ConstLawName[3] = {"CONST_LAW","Constant law","���������� �����"};
//2. ������������ ��������� ������ F(x) = A*x+B
uint sz_LinearLawName = 3;
const char* LinearLawName[3] = {"LINEAR_LAW","Linear law","�������� �����"};
//3. ������������ ������������� ������ F(x) = A*x^2+B*x+C
uint sz_SquareLawName = 3;
const char* SquareLawName[3] = {"SQUARE_LAW","Square law","������������ �����"};
//4. ������������ ����������������� ������ F(x) = A*exp(B*x)
uint sz_ExpLawName = 3;
const char* ExpLawName[3] = {"EXP_LAW","Exponential law",
                             "���������������� �����"};
//5. ������������ ���������������� ������ F(x) = A/x
uint sz_HyperbolLawName = 3;
const char* HyperbolLawName[3] = {"HYPERBOL_LAW","Hyperbolic law",
                                  "��������������� �����"};
//6. ������������ ������ ����������� ����� �� �������� F(x) = SQRT(A*x+B)
uint sz_SqrtLawName = 3;
const char* SqrtLawName[3] = {"SQRT_LAW","Square root","���������� ������"};
//7. ������������ ������ ������������ ��������� F(x) = LN(x)
uint sz_LnLawName = 3;
const char* LnLawName[3] = {"LN_LAW","Natural logarithm","����������� ��������"};
//8. ������������ ������ ����������� ��������� F(x) = LOG10(x)
uint sz_Log10LawName = 3;
const char* Log10LawName[3] = {"LOG10_LAW", "Decimal logarithm",
                               "���������� ��������"};
//9. ������������ ������ ��������� �� ��������� A F(x) = LOGA(x)
uint sz_LogALawName = 3;
const char* LogALawName[3] = {"LOGA_LAW", "Log on base A",
                              "�������� �� ��������� �"};
//10. ������������ ��������������� ������  F(x) = RAND[A..B]
uint sz_RandABLawName = 3;
const char* RandABLawName[3] = {"RANDAB_LAW", "Random law [A..B]",
                                "�������������� ����� [A..B]"};
//11. ������������ ���������� ������
uint sz_TableLawName = 3;
const char* TableLawName[3] = {"TABLE_LAW","Table law","��������� �����"};

//������� �������, ������������ ������������� ���������� ����� ����� ���������
//���������� �������� �������
const uint sz_DistLawTable = 11;
const INFO_LAW DistLawTable[11] = {
 {ConstLawName,sz_ConstLawName,CONST_LAW,ConstLaw,2,2,0,"F(x)=A",DETLAW},
 {LinearLawName,sz_LinearLawName,LINEAR_LAW,LinearLaw,3,3,0,"F(x)=A*x+B",DETLAW},
 {SquareLawName,sz_SquareLawName,SQUARE_LAW,SquareLaw,4,4,0,"F(x)=A*x^2+B*x+C",
  DETLAW},
 {ExpLawName,sz_ExpLawName,EXP_LAW,ExpLaw,3,3,0,"F(x)=A*exp(B*x)",DETLAW},
 {HyperbolLawName,sz_HyperbolLawName,HYPERBOL_LAW,HyperbolLaw,2,2,0,"F(x)=A/x",
  DETLAW},
 {SqrtLawName,sz_SqrtLawName,SQRT_LAW,SqrtLaw,3,3,0,"F(x)=SQRT(A*x+B)",DETLAW},
 {LnLawName,sz_LnLawName,LN_LAW,LnLaw,1,1,0,"F(x)=LN(x)",DETLAW},
 {Log10LawName,sz_Log10LawName,LOG10_LAW,Log10Law,1,1,0,"F(x)=LOG10(x)",DETLAW},
 {LogALawName,sz_LogALawName,LOGA_LAW,LogALaw,2,2,0,"F(x) = LOGA(x)",DETLAW},
 {RandABLawName,sz_RandABLawName,RANDAB_LAW,RandABLaw,3,2,1,"F(x)=RAND[A..B]",
  CASUALLAW},
 {TableLawName,sz_TableLawName,TABLE_LAW,ZeroLaw,0,0,1,"Table law",DETLAW}
 };

//������� �������, ������������ ������ ��������� ��������� � ���� �����������
//���� � ������� �������� �������
//1. ������������ ��������������� ������  F(x) = RAND[0..1]
uint sz_RandLawName = 3;
const char* RandLawName[3] = {"RAND_LAW", "Random law", "�������������� �����"};
//2. ������������ ��������������� ������  F(x) = RAND[0..2PI]
uint sz_Rand2PiLawName = 3;
const char* Rand2PiLawName[3] = {"RAND2PI_LAW", "Random law [0..2PI]",
                                 "�������������� ����� [0..2PI]"};
//3. ������������ ���������� ������ P���� F(x) = RELAY_LAW
uint sz_RelayLawName = 3;
const char* RelayLawName[3] = {"RELAY_LAW", "Relay law", "����� �����"};

//4. ������������ ������������������ ������  F(x) = 1
uint sz_Det1LawName = 1;
const char* Det1LawName[1] = {"DET1_LAW"};

//5. ������������ ������������������ ������  F(x) = 0
uint sz_Det0LawName = 1;
const char* Det0LawName[1] = {"DET0_LAW"};

const uint sz_NoiseLawTable = 6;
const INFO_LAW NoiseLawTable[6] = {
 {RelayLawName,sz_RelayLawName,RELAY_LAW,RelayLaw,1,0,0,
  "F(x)=sqrt(-2*ln(RAND[0..1])", CASUALLAW},
 {Rand2PiLawName,sz_Rand2PiLawName,RAND2PI_LAW,Rand2PiLaw,1,0,0,
  "F(x)=RAND[0..2PI]", CASUALLAW},
 {RandLawName,sz_RandLawName,RAND_LAW,RandLaw,1,0,0,"F(x)=RAND[0..1]",
  CASUALLAW},
 {RandABLawName,sz_RandABLawName,RANDAB_LAW,RandABLaw,3,2,1,"F(x)=RAND[A..B]",
  CASUALLAW},
 {Det1LawName,sz_Det1LawName,DET1_LAW,Det1Law,1,0,0,"F(x) = 1",DETLAW},
 {Det0LawName,sz_Det0LawName,DET0_LAW,Det0Law,1,0,0,"F(x) = 0",DETLAW}
 };

//������ ������������ ������, ������������ � �������� �������
//1. ������������ ������������� ���������
uint sz_HWDipoleName = 3;
const char* HWDipoleName[3] = {"HW_DIPOLE", "Half-wave dipole",
                               "������������ ��������"};
//2. ������������ ������������ ����
uint sz_HWSlotAntName = 3;
const char* HWSlotAntName[3] = {"HWSLOT_ANT", "Half-wave slot antenna",
                                "������������ ����"};
//3. ������������ �������������� �������
uint sz_ParabAntName = 3;
const char* ParabAntName[3] = {"PARAB_ANT", "Parabolic antenna",
                               "�������������� �������"};
//������������ ���������� �������������� ������� � E- � H-����������
uint sz_ParabAntEH = 1;
const char* ParabAntEH[1] = {"La - Parabolic antenna relative aperture"};
//�������������� ��������� � ��������, ����������� ��������� �������������� ��-
//���� �� �����
uint sz_UnParam = 1;
const char* UnParam[1] = {"The additional parameters are not required"};

//������� ������ �� ��������� �������� �������, ������������ ��� ���������
//������������� �� �������� ��� ���������� �� � �����
const uint sz_AntTable = 3;
const ANTENNA_INFO AntTable[3] = {
 //1. ������������ ��������
 {HWDipoleName,sz_HWDipoleName,HW_DIPOLE,1,0,UnParam,sz_UnParam,1,0,UnParam,
  sz_UnParam},
 //2. ������������ ����
 {HWSlotAntName,sz_HWSlotAntName,HWSLOT_ANT,1,0,UnParam,sz_UnParam,1,0,UnParam,
  sz_UnParam},
 //3. �������������� �������
 {ParabAntName,sz_ParabAntName,PARAB_ANT,2,1,ParabAntEH,sz_ParabAntEH,2,1,
  ParabAntEH,sz_ParabAntEH}
};
//������ ������������� �������� �������� �������� �������
const uint sz_AntPerform = 9;
const char* AntPerform[9] = {
 "   MASTER (SLAVE) ELEMENT of LINEAR ARRAY PROPERTIES:",
 "1. USAGE: < ON || OFF >;",
 "2. AMPLRELFACTOR: <value>;",
 "3. ANTENNA: <antenna name>;",
 "4. E_DIRECT: PARAMETERS: <value1,value2,...,valueN>;",
 "5. H_DIRECT: PARAMETERS: <value1,value2,...,valueN>;",
 "6. E_LOCATION: <Equator || Meridian>;",
 "7. NOISEAMPL: <law name>; PARAMETERS: <value1,value2,...,valueN>;",
 "8. NOISEPHASE: <law name>; PARAMETERS: <value1,value2,...,valueN>;"
};
//������ ��������� ������� �������� �������� �������� �������
const uint sz_MasterHeader = 3;
const char* MasterHeader[3] = { "MASTER ELEMENT of LAA PROPERTIES",
 "MASTER ELEMENT of LINEAR ARRAY PROPERTIES",
 "�������� ��������� �������� �������� �������� �������"};

const uint sz_SlaveHeader = 3;
const char* SlaveHeader[3] = { "SLAVE ELEMENT of LAA PROPERTIES",
 "SLAVE ELEMENT of LINEAR ARRAY PROPERTIES",
 "�������� ���������������� �������� �������� �������� �������"};

//1. ������ ������������ �������� <USAGE> �������� �������� �������
const uint sz_LAAElemUsage = 3;
const char* LAAElemUsage[3] = {"USAGE","�������������","�������������"};

const uint sz_TbUsage = 4;
const STR_BOOL TbUsage[4] = { {"OFF",false},{"ON",true},
 {"�� ������������",false},{"������������",true} };

//2. ������ ������������ �������� <AMPLRELFACTOR> �������� �������� �������
const uint sz_LAAElemARF = 3;
const char* LAAElemARF[3] = {"AMPLRELFACTOR","ARF","����������������"};

//������ ������������ ���-�� �������� ��, ������������ ������������� �����������
//�������� �������� �� - amplify relative factor (ARF)
const uint sz_NameARF = 2;
const char* NameARF[2] = { {"ARF"}, {"N"}};

//3. ������ ������������ �������� <ANTENNA> �������� ��
const uint sz_Antenna = 3;
const char* AntennaPattern[3] = {"ANTENNA","�������","�������"};

//4. ������ ������������ �������� <E_DIRECT> �������� ��
const uint sz_EDirect = 3;
const char* EDirectPattern[3] = {"E_DIRECT","E-PLANE","E-���������"};

//5. ������ ������������ �������� <H_DIRECT> �������� ��
const uint sz_HDirect = 3;
const char* HDirectPattern[3] = {"H_DIRECT","H-PLANE","H-���������"};

//6. ������ ������������ �������� <E_LOCATION> �������� �� - ������������ �����-
//   �� ������������ �������������� ��� �������������� ���������� �����
const uint sz_LAAElemELocation = 2;
const char* LAAElemELocation[2] = {"E_LOCATION","������������ E-���������"};

//6. ������ ������������ �������� <NOISEAMPL> �������� �� - ����� ���������
//   ��������� ����������� ���� ��������� ������
const uint sz_LAAElemNoiseAmpl = 2;
const char* LAAElemNoiseAmpl[2] = {"NOISEAMPL","��������� ����"};

//7. ������ ������������ �������� <NOISEPHASE> �������� �� - ����� ���������
//   ���� ����������� ���� ��������� ������
const uint sz_LAAElemNoisePhase = 2;
const char* LAAElemNoisePhase[2] = {"NOISEPHASE","���� ����"};

//==========================================================================
// I. �������� ������������� ������-������� ������ TLinearArray
//    PUBLIC MEMBER-FUNCTION OF TLinearArray CLASS
//==========================================================================
//*****************************************************************************
//������: I.1; public member-function of class TElArray
//������������ �������: string& Properties(string& s, bool headfoot = ON)
//����������:
//������� ��������� �������������� ������� TLinearArray � ���� ������. ������
//�������� �������� �� ����������� �������� '\n'. �������������� �������� �����-
//��� ������� ��������� (������):
// LINEAR ARRAY PROPERTIES
// 1. CONFIGURATION: LEFT = <value>; TOGETHER = <value>; RIGHT = <value>;
// 2. UNIFORMITY: < Uniform || Nonuniform >;
// 3. EQUIDISTANCE: <Equidistant || Unequidistant>;
// 4. LEFTDISTLAW: <law name>; PARAMETERS: <value1,value2,...,valueN>;
// 5. RIGHTDISTLAW: <law name>; PARAMETERS: <value1,value2,...,valueN>;",
// 6. DISPOSITION: <Equator || Meridian>;
//-----------------------------------------------------------------------------
//������������ ���������:
//1. string& s - ������ �� ��������� ������
//2. bool headfoot - ��������� ������������ ��������� � ������� ������
//                   ������������� �������� ��
//-----------------------------------------------------------------------------
//������������ ��������: string& s - ������ �� ���������� �������
//*****************************************************************************
string& TLinearArray::Properties(string& s, bool headfoot)
{char buffer[80];
 ostrstream text (buffer, sizeof(buffer));
 string t;
 s.resize(0);
 if (headfoot == ON)
 {text << "**************  Performance of Linear Antenna Array   **************"
       << endl << ends;
  s.append(text.str());
  text.seekp(0);
 }
 //������������ ��
 s.append("1. CONFIGURATION: ");
 text << "TOTAL = " << Count;
 text << "; LEFT = " << Left << "; TOGETHER = " << Together << "; RIGHT = "
      << Right << ";" << endl << ends;
 s.append(text.str());
 text.seekp(0);
 //������������ ��
 s.append("2. UNIFORMITY:    ");
 if (Uniformity == UNIFORM) s.append("Uniform\n");
 else s.append("Nonuniform\n");

 s.append("3. EQUIDISTANCE:  ");
 if (Equidistance == EQUAL) s.append("Equidistant\n");
 else s.append("Unequidistant\n");

 s.append("4. LEFTDISTLAW:   ");
 if (LeftLawType == CONST_LAW) //���������� ����� ��� �������������� �������
  text << "CONST_LAW; " << "LV = " << LeftLawArg->List[1] << endl << ends;
 else if (LeftLawType == TABLE_LAW) //��������� �����
  text << "TABLE_LAW;" << endl << ends;
 else //����� ������
 {//����� ������������ ������ ���������������� LeftLawType � DistLawTable
  uint index = 0;
  for (uint i = 0; i < sz_DistLawTable; i++)
   if(DistLawTable[i].ident == LeftLawType) { index = i; break; }
  text << DistLawTable[index].pattern[0] << ";" << endl << ends;
 }
 s.append(text.str());
 text.seekp(0);

 s.append("5. RIGHTDISTLAW:  ");
 if (RightLawType == CONST_LAW) //���������� ����� ��� �������������� �������
  text << "CONST_LAW; " << "LV = " << RightLawArg->List[1] << endl << ends;
 else if (RightLawType == TABLE_LAW) //��������� �����
  text << "TABLE_LAW;" << endl << ends;
 else //����� ������
 {//����� ������������ ������ ���������������� LeftLawType � DistLawTable
  uint index = 0;
  for (uint i = 0; i < sz_DistLawTable; i++)
   if(DistLawTable[i].ident == RightLawType) { index = i; break; }
  text << DistLawTable[index].pattern[0] << ";" << endl << ends;
 }
 s.append(text.str());
 text.seekp(0);

 s.append("6. DISPOSITION:   ");
 if (Disposition == EQUATOR) s.append("Equator (Horizontal)\n");
 else s.append("Meridian (Vertical)\n");

 if (headfoot == ON)
 {text << "********************************************************************"
       << endl << ends;
  s.append(text.str());
 }
return s;
}
//******************************************************************************
//������: I.2; public member-function of class TElArray
//������������ �������:
// string& LAAElemProperties(string& s, ulong index, bool headfoot = ON)
//����������:
//������� ��������� �������������� ��������� �������� TElArray ������� TLinear-
//Array � ���� ������ � ���������� ������� index. ������ �������� �������� ��
//����������� �������� '\n'.
//------------------------------------------------------------------------------
//������������ ���������:
//1. string& s - ������ �� ��������� ������
//2. ulong index - ������ �������� �� � ������� data ��� �������� �����������
//   ��������������
//3. bool headfoot - ��������� ������������ ��������� � ������� ������ �������-
//   ������� �������� �������� ��
//------------------------------------------------------------------------------
//������������ ��������: string& s - ������ �� ���������� �������
//******************************************************************************
string& TLinearArray::LAAElemProperties(string& s,ulong index,bool headfoot)
{char buffer[80];
 ostrstream text (buffer, sizeof(buffer));
 s.resize(0);
 if (index >= Count)
 {text << "Element of LAA with number <" << index << "> does not exist !"
       << endl << ends;
  s.append(text.str());
  text.seekp(0);
 }
 else data[index].GetPerformance(s,headfoot);
return s;
}
//*****************************************************************************
//������: I.3; public member-function of class TLinearArray
//������������ �������: void CalcRelDistance()
//����������:
//������� ������������ ������������� ���������� ����� ��������� ���������� ��, �
//����� ����� �������� � ���������������� ���������� ��.
//-----------------------------------------------------------------------------
//������������ ���������: ���
//-----------------------------------------------------------------------------
//������������ ��������: ���
//*****************************************************************************
void TLinearArray::CalcRelDistance()
{double L;   //���������� ����� ��������������� � �������� ��������� ��
 double LV;  //���������� ����� ��������� ���������������� ����������
 uint index;
 bool type;  //��������� ��� ����������������� �����
 uint border;
 //****************************************************************************
 //1. ���������� ������������� ���������� ����� ���������� ��, ��������������
 //   ����� �� ���������
 //****************************************************************************
 if (Left != 0) //������� �������� ��, ������������� ����� �� ���������.
 {//����� ������� ������ ���������������� LeftLawType � DistLawTable
  for (uint i = 0; i < sz_DistLawTable; i++)
   if(DistLawTable[i].ident == LeftLawType) { index = i; break; }
  type = DistLawTable[index].type;
  //�����������������, ����������� �����
  if ((type == DETLAW) && (LeftLawType != TABLE_LAW))
  {//���������� ������� ������� ������������ ��������� �� � ������� data
   border = Left - 1;
   data[Left].SetLv(-1.0*LeftLawArg->List[0]);
   data[Left].SetL(-1.0*LeftLawArg->List[0]);
   L = LeftLawArg->List[0];
   if (border != 0) //����� ��������� �� ���� ������� ��
   {for (uint i = border; i > 0; i--)
    {LV = LeftLaw(LeftLawArg); L += LV;
     data[i].SetLv(-1.0*LV); data[i].SetL(-1.0*L);
     LeftLawArg->List[0] = LV;
    }
   }
  }
  //��������� �����
  else if (LeftLawType == TABLE_LAW)
  {uint j = 1; L = 0;
   for (uint i = Left; i > 0; i--)
   {LV = LeftLawArg->List[j++];
    data[i].SetLv(-1.0*LV); L += LV; data[i].SetL(-1.0*L);
   }
  }
  //��������� �����
  else if (type == CASUALLAW)
  {L = 0;
   for (uint i = Left; i > 0; i--)
   {LV = LeftLaw(LeftLawArg);
    data[i].SetLv(-1.0*LV); L += LV; data[i].SetL(-1.0*L);
   }
  }
 }
 //****************************************************************************
 //2. ���������� ������������� ���������� ����� ���������� ��, ��������������
 //   ������ �� ���������
 //****************************************************************************
 if (Right != 0) //������� �������� ��, ������������� ������ �� ���������.
 {//����� ������� ������ ���������������� RightLawType � DistLawTable
  for (uint i = 0; i < sz_DistLawTable; i++)
   if(DistLawTable[i].ident == RightLawType) { index = i; break; }
  type = DistLawTable[index].type;
  //�����������������, ����������� �����
  if ((type == DETLAW) && (RightLawType != TABLE_LAW))
  {//���������� ������ ������� ������������ ��������� �� � ������� data
   border = Left + Together + 1;
   data[border].SetLv(RightLawArg->List[0]);
   data[border].SetL(RightLawArg->List[0]);
   L = RightLawArg->List[0];
   if (border != Count-1) //������ ��������� �� ���� ������� ��
   {for (uint i = border+1; i < Count; i++)
    {LV = RightLaw(RightLawArg); L += LV;
     data[i].SetLv(LV); data[i].SetL(L);
     RightLawArg->List[0] = LV;
    }
   }
  }
  //��������� �����
  else if (RightLawType == TABLE_LAW)
  {uint j = 1; L = 0;
   border = Left + Together + 1;
   for (uint i = border; i < Count; i++)
   {LV = RightLawArg->List[j++];
    data[i].SetLv(LV); L += LV; data[i].SetL(L);
   }
  }
  //��������� �����
  else if (type == CASUALLAW)
  {border = Left + Together + 1; L = 0;
   for (uint i = border; i < Count; i++)
   {LV = RightLaw(RightLawArg);
    data[i].SetLv(LV); L += LV; data[i].SetL(L);
   }
  }
 }
}
//*****************************************************************************
//������: I.4; public member-function of class TLinearArray
//������������ �������:
//  void SetAngle(double AzimuthAngle, double ElevateAngle, bool Deg = DEG)
//����������:
//������� ������������� ���� ��������� �������, ��������������� E-��������� �
//H-���������, ��� ���� ��������� �� � ����������� �� �� ������������ � �����-
//�������.
//���� E-��������� ������� ��������� � �������������� ���������� �����, �� ��
//����� ��������������� ElevateAngle, � H-��������� - AzimuthAngle.
//���� E-��������� ������� ��������� � �������������� ���������� �����, �� ��
//����� ��������������� AzimuthAngle, � H-��������� - ElevateAngle.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. double AzimuthAngle - ���� ������� ��������� �������
//2. double ElevateAngle - ���� ����� ��������� �������
//3. bool Deg - ��������� ��������� ����� �� ������� � ����� (� �������� ��� �
//   � ��������)
//-----------------------------------------------------------------------------
//������������ ��������: ���
//*****************************************************************************
void TLinearArray::SetAngle(double AzimuthAngle, double ElevateAngle, bool Deg)
{
 //���� ��������� ����� ����� � ������� ��������� ������� ��� ���� ��������� ��,
 //������� ������������ � �������� (� usage == ON)
 for (ulong i = 0; i < Count; i++)
  if (data[i].GetUsage() == ON) data[i].SetAngle(AzimuthAngle,ElevateAngle,Deg);
}
//*****************************************************************************
//������: I.5; public; TLinearArray class member-function
//������������ �������: void SetRandNumber();
//����������:
//������� �������� ������� �������� ��������� �������� � ��� ��� ����������
//������������� ���� �� ������ �������� ������� ��� ���� ��������� ��, �������
//������������ ��� �����������.
//��������� �������� �������� (���) ��������� � ������ ���������� ���������
//������� ��� �������� ������ 0. ������ ������� ��������� ��������� ��������
//�������� ��������� � ���������� ������, ������������ ��������� ���� ��� �����-
//���� ����, ��� ����������� ���������� �������� ���������� ��� ���������
//���� � ���� � ��� �� ������ ������� t. ������� ��������� � ��������� ��������
//������� �� � ��������� ������ ��������� �������� ��������� (����), ���
//�������� ������������. ��������� �������������� ���� �������� �������:
//1. NoiseAmpl  - ARGLIST* NoiseAmplArg class Antenna
//2. NoisePhase - ARGLIST* NoisePhaseArg class Antenna
//-----------------------------------------------------------------------------
//������������ ���������: ���
//-----------------------------------------------------------------------------
//������������ ��������: ���
//*****************************************************************************
void TLinearArray::SetRandNumber()
{//���� ��������� ��������� �������� ��������� � ���� ���� ��� ���� ��������� ��,
 //������� ������������ ��� �����������
 for (ulong i = 0; i < Count; i++)
  if (data[i].GetUsage() == ON) data[i].SetRandNumber();
}
//*****************************************************************************
//������: I.6; public member-function of class TLinearArray
//������������ �������:
//rvector AntPatterns(bool EarthPlane, double ScanAngle = 0,
//        double InitAngle = -90.0, double EndAngle = 90.0, double Step = 0.1,
//        bool Deg = DEG);
//����������:
//������� ���������� ��������� �������������� �������� ������� � ��������������
//(�� ���� �����) ��� � �������������� (�� �������) ��������� � ������� ��
//InitAngle �� EndAngle. ���� ������������� ������������ ������� � ��������
//��������� �������� �������� �������.
//������� �������������� �������������� ������� � �������������� ��� �����������-
//��� ��������� ����� ���������� � ����������� �� �������� EarthPlane.
//��� ��� �� ����� ������������� ��� � ��������������, ��� � ������������ ����-
//�����, �� �� ��������� �������������� ����� �������� �� ���� � ����� ���������
//(�������������� ��� ��������������) ����� ��� �����������. ���� ������������
//�� ����� ��������� � EarthPlane, �� �� ��������� �������������� �� ������ ����-
//��� ������� ������, ����������� � ��������� �� ���������� �� �����������������
//��������, � ����� ��������� �������������� ��������� �� � �� �������������
//����������� ��������, � ������ ������������ ��. ���� ������������ �� ����� ��-
//����� �� EarthPlane, �� ������� ������ ����� ���������� ����� ����� ���� � ��
//��������� �������������� �� ������ ������� ������ ��������� ��������������
//��������� �� � �� ������������� ������������ �������� � ������ ������������ ��.
//������ ������� ��������� ������������� ��������� �������������� �� �� �������:
// G(q) = 10*log10{|A(q)|^2/N^2} [Db] - � ���������,
// 1. q - ���� ����� ���������� ������� � �������� � ��� �������, ����������
//    ����� �������� ������� ��.
// 2. N - ����� ���������� ��������� ��.
// 3. A(q) - ��������� �������������� �� � ����������� �� �������� �������
// ��������� �������������� �� A(q) �����������:
// A(q) = SUM{ARF[i]*F(q)[i]*EXP(j(i-1)Ph[i]) by i = 1 to N},
// 1. i - i-�� ������� ��;
// 2. ARF[i] - ������������� �����-� �������� i-�� �������� ��
// 3. F(q)[i] - �������� ������������� ��������� �������������� i-�� �������� ��
//    � ����������� ��������� �������
// 4. Ph[i] - ������� ����� (�����) � i-�� �������� �� �� ��������� � ��������.
// Ph[i] = 2*PI*(L[i])*sin(q), L[i] - ������������� ���������� ����� i-�� � ��-
// ������ ���������� ��.
//��� �������� �������������� �������������� �� � �������� ������� ��������� �
//������ Data. ��������� �������� ������ � ������� Data:
//Data[0] - InitAngle; Data[1] - EndAngle; Data[2] - Step; Data[3]...Data[n] -
//������.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. bool EathPlane - ��������� �����, ������������ ����� ������� ��������������
//   �������. (MERIDIAN; EQUATOR))
//2. double ScanAngle (0 deg) - ����������� �������� ��������
//3. double InitAngle (-90 deg) - ��������� ����, ���������� ��������� ���������-
//   ����� ������� (antenna patterns)
//4. double EndAngle (+90 deg) - �������� ����, ���������� ��������� ���������-
//   ����� �������.
//5. double Step - ���������� ����
//6. bool Deg (DEG) - ������������� ���� (� �������� - DEG ��� �������� - RAD)
//-----------------------------------------------------------------------------
//������������ ��������:
// rvector Data - ��������� �������������� �������
//*****************************************************************************
rvector TLinearArray::AntPatterns(bool EarthPlane, double ScanAngle,
        double InitAngle, double EndAngle, double Step, bool Deg)
{ulong Total; //���������� ������, ������� ���������� ���������� � ������ data

 if (EndAngle < InitAngle)
 { double Value = InitAngle;
   InitAngle = EndAngle;
   EndAngle = Value;
 }
 //���������� ���� ����� ���� ������ ������������� ���������
 Step = fabs(Step);
 if (Step == 0.0) Step = (Deg == DEG) ? 0.1 : 0.1*DEG_RAD;
 //���������� ���������� ������, ������� ����� ��������� � ������ data
 Total = ceil((EndAngle-InitAngle)/Step);
 if (((EndAngle-InitAngle)/Step) != 0.0) Total += 1;
 //������� ������ Data ��� �������� �������� �������������� LAA
 rvector Data(Total+3,COL,0.0);
 //������� � ������ ������ ����������� ��������� �������������� �������
 //� ��������
 if (Deg == RAD) //������� �� ������ � �������
 { InitAngle = InitAngle*RAD_DEG;
   EndAngle = EndAngle*RAD_DEG;
   Step = Step*RAD_DEG;
 }
 Data[0] = InitAngle; Data[1] = EndAngle; Data[2] = Step;
 double Angle = InitAngle;
 double Phi; //������� ����� � i-�� �������� ��
 double Ampl;
 complex Sum;
 complex Init(0,0);
 complex C_ONE(1,0);

 //������ ������������� �������������� ������� �������� �������, �������� ���-
 //�������� �������� ��������
 complex* W = new complex[Count];
 if (Deg == DEG) //������� �� �������� � �������
  ScanAngle *= DEG_RAD;
 if (ScanAngle == 0.0)
   for (ulong i = 0; i < Count; i++) W[i] = C_ONE;
  else
   for (ulong i = 0; i < Count; i++)
    {Phi = -1.0 * GetL(i) * sin(ScanAngle) * TWO_PI;
     W[i] = polar(1, Phi);
    }

 //����������� ��������� �������������� �� � �������� ������� ������
 for (ulong index = 3; index < Data.Size(); index++)
 {Sum = Init;
  //������ ���� Angle � ����������� �� EarthPlane, � ������� �����������
  //��������� �������������� ��
  if (EarthPlane == EQUATOR) SetAngle(Angle,0,Deg);
  else SetAngle(0,Angle,Deg);
  //���� ������� ��������� �������������� �� � ����������� �� ���� Angle
  for (ulong i = 0; i < Count; i++)
  { //������� �� ������������ ��� ��������
    if (data[i].GetUsage() == ON)
    {//��������� ������� ����� � i-�� �������� ��
     //��������� ������������ �� ��������� � EarthPlane
     if (EarthPlane == Disposition)
     {Phi = Angle*DEG_RAD;
      Phi = data[i].GetL()*sin(Phi)*TWO_PI;
     }
     //��������� ������������ �� � EarthPlane �� ���������, ������� ������ �����
     //���������� �� �����������
     else Phi = 0.0;
     //��������� �������� �������������� ��� i-�� �������� ��
     Ampl = data[i].GetN()*data[i].CalcAntPatterns(EarthPlane);
     Sum += polar(Ampl,Phi)*W[i];
    }
  } //����� ����� �� i - ��������� ��
  Data[index] = norm(Sum);
  Angle = Angle+Step; //��������� ����� �������� ����
  if (fabs(Angle) <= 1.0e-6) Angle = 0.0;
 } //����� ����� �� index - ������� ����������� ��������� �������������� ��
 delete[] W;
 //����� ������������� ��������
 double Max = Data[3];
 for (ulong index = 4; index < Data.Size(); index++)
  if (Max < Data[index]) Max = Data[index];
 //���������� ������������� ��������� �������������� ����������� �������������-
 //���� �������
 for (ulong index = 3; index < Data.Size(); index++)
  Data[index] = 10*log10(Data[index]/Max);
return Data;
}

//****************************************************************************
// I. �������� ������������� � ������������ ������ TLinearArray
//****************************************************************************

//*****************************************************************************
//������: I.1; public; constructor by default
//������������ �������: TLinearArray()
//����������: ����������� �� ��������� ������ TLinearArray.
//������� ������������� ��� �������� ������ TLinearArray �� ���������. ��� ���
//����� TLinearArray �������� ����������� ������ TElArray, �� ����� �������
//������������ ���������� ����������� �� ��������� ������ TElArray. � ���� ����-
//�� ������������ ��������� ������� ������� DistanceLaw, ������������ ����������-
//��� ���������� ����� ����� ��������� ���������� �������� �������.
//-----------------------------------------------------------------------------
//������������ ���������: ���
//-----------------------------------------------------------------------------
//������������ ��������: ���
//*****************************************************************************
TLinearArray::TLinearArray() 
{ //��������� ������������� ������������� �������� ��
  InitPerformance();
}
//*****************************************************************************
//������: II.2; public; constructor with argument
//������������ �������: TLinearArray(const char* filename)
//����������: ����������� � ���������� ������ TLinearArray.
//������� ������������� ��� �������� ������ TLinearArray �� ������ ����������� �
//��������� ����� filename. ���� ���� ���������� �����������. ��� ����������
//������ �� ����� ����� �������������� �������� ��� ������������. � ������ ���-
//��� ��� �������������� ��������� ����� ���������� ������� ������������ �����-
//��������� ���������� ������ ������������, ������, ��������� ���������� ������-
//�������� � ����� filename.err. ���������� �������� ����� � ������� �� ��������
//�� �� ������ ����� ���������� - ".err".
//-----------------------------------------------------------------------------
//������������ ���������: const char* filename - ��� �������� �����
//-----------------------------------------------------------------------------
//������������ ��������: ���
//*****************************************************************************
TLinearArray::TLinearArray(const char* filename)
{bool lSuccess;
 //��������� ������������� ������������� �������� ��
 InitPerformance();
 try
 { //��������� ���� � ���������������� �������� �������� �������
   lSuccess = OpenDataFile(filename);
   if (!lSuccess) throw FOPEN_ERROR;
   //���������� ������������� �������� �� �� ���������� ����� � �������������
   //������-������ ������� ������ TLinearArray � ���������� ��������� ������
   lSuccess = GetDataFromFile();
   if (!lSuccess) throw FREAD_ERROR;
   if (lSuccess == true)
   {fin.close();
    ferr.close();
    remove(ferrname.c_str());//�������� ���������� ����� ����������� ������
    #if defined(CONSOLE)
     clrscr();
     cout << "The input data on LAA components from file <" << fname
          << ">" << "\nare read out successfully." << endl;
     cout << "Press any key ...";
     getch();
    #endif
   }
 }
 //��������� �������������� ��������
 catch (int error_type)
 {if (error_type == FOPEN_ERROR) //�� ������� ������� ���� ������
  {
   #if defined(CONSOLE)
    cout << endl;
    cout << "ERROR: CAN'T OPEN DATA FILE <" << uppercase << fname  //to_upper(fname)
         << "> !!!" << endl;
    cout << "The LINEAR ARRAY design on file data is impossible." << endl;
    cout << "The program will be terminated." << endl;
    cout << "Press any key ..."; getch();
   #endif
  }
  else if (error_type == FREAD_ERROR) //������ ���������� ������
  {
   #if defined(CONSOLE)
    cout << endl;
    cout << "ERROR: DETECT ERROR AT READING DATA FROM FILE <" << uppercase << fname //to_upper(fname)
         << "> !!!" << endl;
    cout << "The LINEAR ARRAY design on file data is impossible." << endl;
    cout << "Information about a probable error is in the file <"
         << uppercase << ferrname << ">." << endl;
    cout << "The program will be terminated." << endl;
    cout << "Press any key ..."; getch();
   #endif
  }
  //�������� ������� TLinearArray
  #if defined(CONSOLE)
   cout << "\nDestroy object of TLinearArray class..." << endl;
  #endif
  //�������� ���� ������������� �������� ��
  ResetPerformance();
 } //���������� ����� ��������� �������������� ��������

}
//*****************************************************************************
//������: I.3; public; TLinearArray class destructor
//������������ �������: ~TLinearArray()
//����������: ���������� ������ TLinearArray.
//������� ������������� ��� ���������� ������. ��� ������ ������� ��������� ���-
//���� �������, ������������ ������������� ���������� ����� ����� ����������
//�������� �� DistanceLaw, ������ ���������� �������, ������ ��������� ��,
//������������ �������� �������� �������.
//-----------------------------------------------------------------------------
//������������ ���������: ���
//-----------------------------------------------------------------------------
//������������ ��������: ���
//*****************************************************************************
TLinearArray::~TLinearArray()
{
 #if defined(CONSOLE)
   cout << "\nDestroy object of TLinearArray class..." << endl;
 #endif
 //�������� ���� ������������� �������� ��
 ResetPerformance();
}

//===================================================================
// II. �������� �������� ������-������� ������ TLinearArray
//     PRIVATE MEMBER-FUNCTION OF TLinearArray CLASS
//===================================================================

//=============================================================
//III. �������� ���������� ������-������� ������ TLinearArray
//     PROTECTED MEMBER-FUNCTION OF TLinearArray CLASS
//=============================================================
//*****************************************************************************
//������: III.1., protected, class member-function
//������������ �������: virtual void InitPerformance();
//����������:
//������� ������������ ��������� ������������� ������������� �������� ��������
//������� ��� �� ���������������. �� ��������� ��������������� ��������� �����-
//��������� Linear Array: ����������, ��������������, ����������� �� �����������.
//��������� ���������� ��������� �� ����� ����. ������ ������������ ��������� -
//�������. ���� ���������� �� ������ ������������� NULL.
//-----------------------------------------------------------------------------
//������������ ���������: ���
//-----------------------------------------------------------------------------
//������������ ��������: ���
//*****************************************************************************
void TLinearArray::InitPerformance()
{ //��������� ������������� ������������� ������� ������
  Uniformity = UNIFORM; //����������
  Equidistance = EQUIDISTANT; //��������������
  Left = 0; Together = 0; Right = 0; //��������� ������������ �������� �������
  Disposition = EQUATOR; //�������������� ������������
  //������������� ������� � �� ����������, ������������ ������������� ����������
  //����� ����� ��������� ���������� �������� �������� �������
  LeftLawType = ZERO_LAW;
  LeftLaw = ZeroLaw;
  LeftLawArg = NULL;
  RightLawType = ZERO_LAW;
  RightLaw = ZeroLaw;
  RightLawArg = NULL;
  Count = 0L;
  data = NULL;
}

//*****************************************************************************
//������: III.2., protected, class member-function
//������������ �������: virtual void ResetPerformance();
//����������:
//������� ������������ ����� ���� ������������� �������� �������� �������. ����-
//��� ����� �������������� ��� �������� ������� Linear Array, � ����� � ������
//���������� ���� ������������� �������� �������. ��������������� ���������
//�������������� Linear Array: ����������, ��������������, ����������� �� �����������.
//��������� ���������� ��������� �� ����� ����. ������ ������������ ��������� -
//�������. ���� ���������� �� ������ ������������� NULL. ������������� ��� ����-
//�������� ������ ���������� ��� �������� ��������� �������� ������� � ���������
//������� ���������� ��������� ������������ ���� �����.
//-----------------------------------------------------------------------------
//������������ ���������: ���
//-----------------------------------------------------------------------------
//������������ ��������: ���
//*****************************************************************************
void TLinearArray::ResetPerformance()
{ //����� ����� ������������� ������������� ������� ������
  Uniformity = UNIFORM; //����������
  Equidistance = EQUIDISTANT; //��������������
  Left = 0; Together = 0; Right = 0; //��������� ������������ �������� �������
  Disposition = EQUATOR; //�������������� ������������
  //������������� ������� � �� ����������, ������������ ������������� ����������
  //����� ����� ��������� ���������� �������� �������� �������
  LeftLawType = ZERO_LAW;
  LeftLaw = ZeroLaw;
  if (LeftLawArg != NULL) LeftLawArg = DeleteArgList(LeftLawArg);
  RightLawType = ZERO_LAW;
  RightLaw = ZeroLaw;
  if (RightLawArg != NULL) RightLawArg = DeleteArgList(RightLawArg);
  Count = 0L;
  //�������� ��������� �������� �������� �������
  if (data != NULL) { delete[] data; data = NULL; }
}

//*****************************************************************************
//�������� �������, ��������������� ��� ���������� ������������� �������� ��
//�� ���������� ����� ������������ ���� ��� �� ���������������
//*****************************************************************************

//*****************************************************************************
//������: III.3., protected, class member-function
//������������ �������: OpenDataFile(const char* filename)
//����������:
//������� ��������� ������� ���� � ������� �� �������� ��, ������� ���� ��������-
//��� ��������� ������, ������� ����� �������� � ����� ������. ���� �����������
//������ ferrname ����� ����� ��� ������� � ������� ������ fname � �����������
//".err". �� ���� ������� ���������� ".err" � ����� fname �����������. �������
//���� ����������� � ������ ������. ���� �������� ��� ����� �� ������� ��� ����
//� ����� ������ �� ����������, �� � ���������� ������ ������������ ������ �����
//��� �������� �����. ��� ��������� ������� �������� ����� fname ��� ���� ����
//����� ����� ���������� ".err", ������� ���������� false.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. const char* filename - ������������ ����� � �������� ������� �� ��
//-----------------------------------------------------------------------------
//������������ ��������: bool lSuccess
// true - ����� � ������� ������
// false - ������ � ������� ������� �� �������
//*****************************************************************************
bool TLinearArray::OpenDataFile(const char* filename)
{ bool lSuccess = true;
  char c;
  uint Y, CurY;
  //************************************************************************
  //��� I.
  //��������� ��������� ���� � ������� �� EXTERNAL SIGNAL � ������ ������
  //************************************************************************
  fname = filename;
  if (fname.length() == 0) //��� �������� ����� � ������� �� ������
  {lSuccess = false;
   #if defined(CONSOLE)
    Y = wherey();
    cout << "\nERROR: filename is absent." << endl;
    cout << "Enter new filename? [Y/N]: ";
    cin >> c;
    //������� ������
    CurY = wherey(); gotoxy(1,Y);
    for(uint i = Y; i <= CurY; i++) {clreol(); cout << endl;}
    gotoxy(1,Y);
    if ((c == 'Y') || (c == 'y')) lSuccess = OpenFileFromConsole();
    if (!lSuccess) return lSuccess;
   #endif
  }
  //��������� ������� ����
  else
  { fin.open(fname.c_str(),ios::in);
    if (!fin)
    { lSuccess = false;
     #if defined(CONSOLE)
      Y = wherey();
      cout << "\nERROR: unable to open <" << fname << ">" << endl;
      cout << "Try again ? [Y/N]: "; cin >> c;
      //������� ������
      CurY = wherey();  gotoxy(1,Y);
      for(uint i = Y; i <= CurY; i++) {clreol(); cout << endl;}
      gotoxy(1,Y);
      if ((c == 'Y') || (c == 'y')) lSuccess = OpenFileFromConsole();
      if (!lSuccess) return lSuccess;
     #endif
    }
    else //���� � ������ filename ������ � ������ ������
    {
     #if defined(CONSOLE)
      //������� ������
      CurY = wherey();  gotoxy(1,Y);
      for(uint i = Y; i <= CurY; i++) {clreol(); cout << endl;}
      gotoxy(1,Y);
     #endif
    }
  }
  //**************************************************************************
  //��� II. ��������� ���� ����������� ������, ������� ����� ���� ����������
  //        ��� ���������� ������ �� �������� ��������� ������ fin - *.err
  //**************************************************************************
  //���������� ����� �������� ����� ��� ����������
  string s;
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
      cout << "Input data file <" << fname << "> contains the inadmissiblis "
           << "extension - .err." << endl;
     #endif
     return lSuccess;
    }
   //������������ ����� ����� ����������� ������ ferrname � ������ �����������
   //� ������ �������� ����� ������ fname � ����������� ".err"
   pos = fname.find(".");
   ferrname = fname.substr(0,pos)+".err";
  }
  s.resize(70);
  for (uint i = 0; i < s.length(); i++) s[i] = '=';
  ferr.open(ferrname.c_str(),ios::out);
  ferr << s << endl;
  string t("���� ����������� ������, ��������� ��� ���������� ������ �� �����");
  pos = (s.length()-t.length())/2;
  for (uint i = 0; i < s.length(); i++) s[i] = ' ';
  s.insert(pos,t);
  ferr << s << endl;
  s.resize(70);
  for (uint i = 0; i < s.length(); i++) s[i] = ' ';
  s.insert((s.length()-fname.length())/2,fname);//to_upper(fname)
  ferr << s << endl;
  s.resize(70);
  for (uint i = 0; i < s.length(); i++) s[i] = '=';
  ferr << s << endl;

 return lSuccess;
}
//*****************************************************************************
//������: III.4., protected, class member-function
//������������ �������: bool OpenFileFromConsole()
//����������:
//������� ������������� ��� �������� �������� ��������� ������ � ���������
//������ � ������ ��� ������. ��� ����� �������� � ���������� ���������� ������
//TLinearArray - fname, ������� �������� ����� ���������� ����� ���������� ����-
//������ ������ TLinearArray - fin.
//-----------------------------------------------------------------------------
//������������ ���������: ���
//-----------------------------------------------------------------------------
//������������ ��������: bool lSuccess
// true - ��������� ���� ������� ������� � ������ ������
// false - ��������� ���� �� ������� ������� � ������ ������
//*****************************************************************************
bool TLinearArray::OpenFileFromConsole()
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
//������: III.5., protected, class member-function
//������������ �������:
// bool GetLineWithoutComment(ifstream fin, string& buffer, const string& ABC,
//      const char** comment = NULL, const uint sz_comment = 0)
//����������:
//������� ������������� ��� ������ ��������� ������ �� ��������� ������ fin �
//������ �� � ��������� ������ buffer. ����� ������� ������ �������������� ���-
//����� �� ��� ������������, ������� ���������� �� ��������� ������ ��������� -
//�������������, ������������� � ������� comment. ������ ������ ����� ������
//������������� �������� - '\n'. ������, ������� �� �������� �������, ���������-
//��� � ��������� ������� ABC, ������������. ��� ���������� ����� �����
//������� ���������� �������� false, ��� �������� �������, ��� ������� �����
//� ����������������� ������������ � ����� ��������������� � ������� �������
//fin ������ ���.
//���������� ����������, ������������ ��������: 1) Buffer 2) CurRow
//-----------------------------------------------------------------------------
//������������ ���������:
//1. ifstream fin - ������� �������� �����
//2. string& line - ������ �� ������, � ������� ����� ������� ������ �� �����,
//                 ��� ������������, ���� � ��� ������� ������� �� �������� ABC
//3. string& ABC - ������� ��������, ������� ������ �������������� � ���������
//                 �� ����� ������ ��� �� ������ � ������ line
//4. const char** comment - ������ � ���������-������������� ����������� �� ������
//5. const uint sz_comment - ���������� ����� � ������� comment
//-----------------------------------------------------------------------------
//������������ ��������: bool lSuccess
// true -  ������� ������, ������� ����� ������� �� ����������� ��������, � ���
//         �������� � buffer
// false - ��������� ����� �����, ����� � ������������ ����� ������ ������ ���
//*****************************************************************************
bool TLinearArray::GetLineWithoutComment(ifstream& fin, string& line,
     const string& ABC, const char** comment, const uint sz_comment)
{ bool lSuccess = true;
  bool lFind = false;
  uint pos;
  //������ ������ �� �����
  while ((!fin.eof()) && (!lFind))
  { getline(fin,Buffer,'\n'); //���������� ������ �� �����
    CurRow++; //������� ����� ����� � �����
   if (Buffer.length() != 0) //����� �������� �������
   {//����� ����������� � ������
    line = Buffer;
    if (comment != NULL)
    { //���� ������ ��������-������������ ������ �� �����������
      for (uint i = 0; i < sz_comment; i++)
      { pos = line.find(comment[i]);
        //�������-����������� ����������� �� ������ �������
        //����������� �����������
        if (pos != string::npos) {line = line.substr(0,pos); break;}
      }
    }
    //����������� ������ line �� ����������� � ��� ���������� ��������
    pos = line.find_first_of(ABC);
    //���������� ������� �������, ��������� ���� ���������� ������
    if (pos != string::npos)
    {lFind = true; lSuccess = true;
     //�������� ��������� � �������� �������� �� ������ line, ������� �� ������-
     //�� ��������� �������� ABC.
     line = line.substr(pos); //�������� ��������� ��-ABC ��������
     //����� � ������ line ���������� ������� �� ����������� �������� ABC
     pos = line.find_last_of(ABC);
     //�������� �������� ��-��� ��������
     if (pos != string::npos) line = line.substr(0,pos+1);
     else line = line.substr(0,1);
    }
   }
  } //����� ����� ���������� ����� �� ��������� ������ fin

  //������ � ��������� ����������� ����� �� �������
  //��������� ����� �����
  if (lFind == false) {lSuccess = false; line.resize(0);}
return lSuccess;
}
//****************************************************************************
//������: III.6., protected, class member-function
//������������ �������:
//       bool SearchKeyString(const string& s, const char** pattern,
//                            const uint sz_pattern, pKEYSTRPOS info = NULL)
//����������:
//������� ������������� ��� ������ �������� ������ (��������� �����) ���������
//�������� ������� ���������� � ������� �������� - pattern � ������ s. ���� ���-
//������� ����� �������, ������� ���������� true, � ��������� ������ - false.
//� ������ �������� ������ � ������� ���������� ��������� �� ��������� KEYSTRPOS,
//��� ����� ��������� ������ �������� ������ � ������� pattern, � �������
//��������� ����������, �� ������ � ����� ������� � ������ s.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. const string& s - ������ �� ������, � ������� ����� ������������� �����
//                     �������� ������
//2. const char** pattern - ������ �������� ��������� ��������� �������� �����
//3. const uint sz_pattern - ���������� ����� � ������� pattern
//4. pKEYSTRPOS - ��������� �� ��������� � ����������� � �������������� ��������
//   ������ � ������ s � ������� pattern ��� ������� �� ���������� � ����� ��
//   �������� �������� ������� pattern. ������ �������� �������� ��������������,
//   �� ��������� - NULL.
//-----------------------------------------------------------------------------
//������������ ��������: bool lSuccess
// true -  �������� ������ � ������ s �������
// false - �������� ������ � ������ s �� �������
//*****************************************************************************
bool TLinearArray::SearchKeyString(const string& s, const char** pattern,
                   const uint sz_pattern, pKEYSTRPOS info)
{ bool lSuccess = false;
  uint lpos;
  //��������� ������������� ���������
  if (info != NULL) {info->index = info->lpos = info->rpos = string::npos;}
  //����� �������� ������
  for (uint i = 0; i < sz_pattern; i++)
  { lpos = s.find(pattern[i]);
    //�������� ������ �������
    if (lpos != string::npos)
    {lSuccess = true;
     //��������� ���������� �� �������� ������
     if (info != NULL)
     {info->index = i;
      info->lpos = lpos; info->rpos = lpos + strlen(pattern[i]) - 1;
      break;
     }
    }
  }
return lSuccess;
}
//****************************************************************************************
//������: III.7., protected, class member-function
//������������ �������:
// void Display(ofstream& out, const char** s, const char* title = NULL, const uint sz_s,
//              const char* left = NULL , const char* right = NULL, bool num = true);
//����������: ������� ������� � ����� ��������� ������. ������ ������ ������� ���������� �
//����� ������.
//----------------------------------------------------------------------------------------
//������������ ���������:
//1. ofstream& out - ������ �� �����, � ������� ����� ���������� s;
//2. const char** s - ������ �� ������ �����;
//3. const uint sz_s - ���������� ����� � ������� s;
//4. const char* title - ��������� ��������� ����������;
//5. const char* left - ������ �������������� ��������� ��������� ������ �� ������� s;
//6. const char* right - ������ ����������� ��������� ��������� ������ �� ������� s;
//7. bool num - ����� ��������� ��������� ����� (num = true).
//----------------------------------------------------------------------------------------
//������������ ��������: ���
//****************************************************************************************
void TLinearArray::Display(ofstream& out, const char** s, const uint sz_s,
     const char* title, const char* left, const char* right, bool num)
{uint width;
 if (left == NULL) left = "< ";
 if (right == NULL) right = " >";

 if (num == true) //���������� ������
 { if (sz_s < 10) width = 1;
   else if ((sz_s >= 10)   && (sz_s < 100)) width = 2;
   else if ((sz_s >= 100)  && (sz_s < 1000)) width = 3;
   else if ((sz_s >= 1000) && (sz_s < 10000)) width = 4;
   else width = 5;
 }
 //����������� ��������� ������� �����
 out << endl;
 if (title != NULL) {out << title << endl;}
 //���� ������ ����� � �����
 //��������� ������ ������� ����� s ���������� � ����� ������
 for(uint i = 0; i < sz_s; i++)
 { if (num) {out << setw(width) << (i+1) << ". ";}
   out << left << s[i] << right << endl;
 }
}
//****************************************************************************
//������: III.8., protected, class member-function
//������������ �������:
//bool FindKeyStrInFile(const string& TitleKeyStr, const char** KeyStr,
//     const uint sz_KeyStr, string& Line, const pKEYSTRPOS info, const string& ABC,
//     const char** Comment, const uint sz_Comment, ifstream& fin, ofstream& ferr,
//     const char** Perform, const uint sz_Perform);
//����������:
//������� ������������� ��� ������ �������� ������ (��������� �����) ���������
//�������� ������� ���������� � ������� �������� - KeyStr � �������� ������ fin.
//���� ���������� ����� �������, ������� ���������� true, � ��������� ������ -
//false. � ������ �������� ������ ��������� �� ��������� pKEYSTRPOS info, �����
//��������� ������ �������� ������ � ������� KeyStr, � ������� ��������� �����-
//�����, �� ������ � ����� ������� � ������ Line. ��� ������ ����� ������� �����-
//������ ������ �� ������ �����, ������� �������� ������� �������� ABC, ��������
//�� ������������ �����������, ������� ���������� �� ������ ��������� �� �������
//����� Comment. � ������ �������, � �������� ������ ferr �������������� �����-
//����� � �� ������� � � ����� �������� ������ ��� ���� �������. ����� ����, ��
//���������� �� ������� Perform � �������� ����� ferr ���������� ���������� �
//��������� ������ (������ ������), ������� ���������� ���� ������� �� �����.
//���������. ���� ��������� �� ����� ������ ��� ������������ ����� ��������� ����
//�� ���� ������ �� �������� ABC � � ���� ������ �� �������� ������� ��������
//������, �� ������� ��������� ���� ������ �� ��������� false.
//-----------------------------------------------------------------------------
//������������ ���������:
//01. const string& TitleKeyStr - ������ �� ������, � ������� ������������ ����-
//    ��� �������� ������ � �������� ������ fin
//02. const char** KeyStr - ������ �������� ��������� ��������� �������� �����
//03. const uint sz_KeyStr - ���������� ����� � ������� KeyStr
//04. string& Line - ������ �� ������, � ������� ����� ���������� ������ �� fin �
//    ������ �������� ������, �������� �����������.
//05. pKEYSTRPOS - ��������� �� ��������� � ����������� � �������������� ��������
//    ������ � ������ Line � ������� KeyStr ��� ������� �� ���������� � ����� ��
//    �������� �������� ������� KeyStr.
//06. const string& ABC - ������ �� ����������� ������� ��������. ���� ������ ��
//    ��������� ������ �� ����� ��������� ���� �� ������ ������� �� ABC ����� �
//    ��� ������� �� �����.
//07. const char** Comment - ������ � ���������� ���������-������������� �������-
//    ��� �� �����������.
//08. const uint sz_Comment - ���������� ����� � ������� Comment
//09. ifstream& fin - ������� �������� �����
//10. ofstream& ferr - �������� ����� � ��������
//11. const char** Perform - ������ �����, ���������� ������ ���������, �������
//    ���������� ������� �� �����
//12. const uint sz_Perform - ���������� ����� � ������� Perform
//-----------------------------------------------------------------------------
//������������ ��������: bool lSuccess
// true -  �������� ������ � �������� ������ fin �������
// false - �������� ������ � �������� ������ fin �� �������
//*****************************************************************************
bool TLinearArray::FindKeyStrInFile(const string& TitleKeyStr,const char** KeyStr,
     const uint sz_KeyStr,string& Line,const pKEYSTRPOS info,const string& ABC,
     const char** Comment,const uint sz_Comment,ifstream& fin,ofstream& ferr,
     const char** Perform,const uint sz_Perform)
{ bool lSuccess;
  string title;
  //����� ��������� ����� TitleKeyStr
  lSuccess = GetLineWithoutComment(fin, Line, ABC, Comment, sz_Comment);
  //������ ���������� ������� ABC ��� ������������ �� �������.
  //��������� ����� �����, ��������� ��������� �� ������
  if (lSuccess == false)
  {ferr << "ERROR:  End of file is reached. It was not possible to receive\n";
   ferr << "        a <" << TitleKeyStr << "> of Antenna Array.";
   title = "REASON: The following keywords were expected:";
   Display(ferr,KeyStr,sz_KeyStr,title.c_str(),"< "," >;");
   ferr << endl;
   ferr << "=============== The block diagram of Performances ==============";
   Display(ferr,Perform,sz_Perform,"","","",false);
   ferr << "================================================================";
   ferr << endl;
   return lSuccess; //����������� ������ �������
  }
  lSuccess = SearchKeyString(Line, KeyStr, sz_KeyStr, info);
  //�������� ����� �� �������, ��������� �� ����� ������ Line ��������
  //������������ ����������. ��������� ��������� �� ������
  if (lSuccess == false)
  {ferr << "ERROR:  The keyword <" << TitleKeyStr << "> is not found." << endl;
   ferr << "REASON: The following line of file contains an incorrect data.\n";
   ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
   ferr << "BUFFER: <" << Buffer << ">";
   title = "The following keywords were expected:";
   Display(ferr,KeyStr,sz_KeyStr,title.c_str(),"< "," >;");
   ferr << endl;
   ferr << "=============== The block diagram of Performances ==============";
   Display(ferr,Perform,sz_Perform,"","","",false);
   ferr << "================================================================";
   ferr << endl;
  }
return lSuccess;
}
//****************************************************************************
//������: III.9., protected, class member-function
//������������ �������:
//bool StrToBool(bool& value, const string& line, const pKEYSTRPOS info,
//              const pSTR_BOOL TableStrBool, const uint sz_TableStrBool,
//              const string& title,const char** Perform,const uint sz_Perform);
//����������:
//������ ������: <KEY STRING;>
//<KEY STRING> = <str1> || <str2> || <str3> ... || <strN>
//������� ������������� ��� ���������� �������� ������ ��������� ��������, ����-
//��� ���������� � ������� TableStrBool � ������ Line � �������������� �� � ��-
//�������� �������� �� ������� ������������ TableStrBool. ������ ������ ��������
//����. � ������ ������, ������� ���������� �������� false � ���������� �� ������ ������-
//�������� � �������� ������ ������ ferr. ������� ��������� ������������� �����-
//��� ������ � ������ Line, � ����� ��������� �������������� ������, ��������� �
//������������ �� ������� ������ ������������ ����. � ����� ������� ���������
//������� ������� ';', ������� � ������ ������ ��������, ����� �������� � ����-
//�����. ��� �������� ������ � �������������� � ���������� ���������� value ��-
//������� ��������� �������������� ������ � ���������� ��������.
//���������� ���������� ������������ ��������:
// 1. string Buffer; 2. string Line; 3. CurRow
//-----------------------------------------------------------------------------
//������������ ���������:
//1. bool& value - ������ �� ���������� ����������, ������� ����� ��������
//   ��������� ������ ������� � ������ �� ��������� ����������.
//2. const string& title - ������ �� ������, � ������� ������������ �������
//   �������� ������ � �������� ������ fin
//3. string& line - ������ �� ������, � ������� �������������� ����� ��������
//   ������.
//4. pKEYSTRPOS info - ��������� �� ��������� � ����������� � ��������������
//   ���������� �������� ������ � ������ line
//5. const pSTR_BOOL TableStrBool - ������� ������������ ��������� ���������
//   �������� ����� �������������� � ���� ���������� ��������
//6. const uint sz_TableStrBool - ���������� ����� � ������� TableStrBool
//7. const char** Perform - ������ �����, ���������� ������ ���������, �������
//   ���������� ������� �� �����
//8. const uint sz_Perform - ���������� ����� � ������� Perform
//-----------------------------------------------------------------------------
//������������ ��������: bool lSuccess
// true -  ������ ������������� � ���������� �������� �������
// false - ������������� ������ � ���������� �������� �� �������
//*****************************************************************************
bool TLinearArray::StrToBool(bool& value, const string& line,
     const pKEYSTRPOS info, const pSTR_BOOL TableStrBool,
     const uint sz_TableStrBool, const string& title, const char** Perform,
     const uint sz_Perform)
{bool lSuccess;
 uint rpos;
 //����� �������-����������� ';' ����� ��������� ��������, ������� ��������
 //�����������, � ������ Line
 rpos = line.find_first_of(';');
 if (rpos == string::npos) //������
 {lSuccess = false;
  ferr << "ERROR:  " << title << " to receive it was not possible." << endl;
  ferr << "REASON: Complete token <;> was expected for this data string" << endl;
  ferr << "Line N " << CurRow << ": <" << line << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << endl;
  ferr << "============== The block diagram of Performances ===============";
  Display(ferr,Perform,sz_Perform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return lSuccess;
 }
 //��������� �� Line ��������� s1 � ����������� �������� ';'
 uint lpos = info->rpos+1;
 string s1 = line.substr(lpos,rpos-lpos+1);
 //s1.set_case_sensitive(0); //��������� ����������-�������������� �����
 //����� �������� ����� �� ������� TableStrBool � ������ s1
 lSuccess = false;
 uint index = 0;
 for (uint i = 0; i < sz_TableStrBool; i++)
 { lpos = s1.find(TableStrBool[i].s);
   if (lpos != string::npos) //�������� ������ � ������� �������
   {lSuccess = true;
    index = i;
    value = TableStrBool[i].value;
    rpos = lpos+strlen(TableStrBool[i].s)-1;
    break;
   }
 }
 if (lSuccess == false) //������: �������� ������ �� �������
 {ferr << "ERROR:  " << title << " to receive it was not possible." << endl;
  ferr << "REASON: Key string to find it was not possible, the logic value of\n";
  ferr << "        performance is not defined." << endl;
  ferr << "Line N " << CurRow << ": <" << s1 << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << "The following key strings were expected: " << endl;
  for (uint i = 0; i < sz_TableStrBool; i++)
  {
   ferr << setw(2) << (i+1) << ". <" << TableStrBool[i].s;
   if ((sz_TableStrBool-i) > 1) ferr << ">;" << endl;
   else ferr << ">." << endl;//��������� ������
  }
  ferr << endl;
  ferr << "============== The block diagram of Performances ===============";
  Display(ferr,Perform,sz_Perform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return lSuccess;
 }
 //������ �� ����������� ����������� �������� � ������. ����� ������� ������� �
 //��������� ������ �������� ���� �� ������
 //����� ����������� �������� ����� �� ��������� �������� ������
 string s2 = s1.substr(0,lpos);
 lpos = s2.find_first_not_of(" \t");
 if (lpos != string::npos) lSuccess = false;//������: ����������� ������� �������
 //����� ����������� �������� ������ �� ��������� �������� ������
 if (lSuccess != false)
 { s2 = s1.substr(rpos+1);
   lpos = s2.find_first_not_of(" \t;");
   if (lpos != string::npos) lSuccess = false;//������: ����������� ������� �������
 }
 if (lSuccess == false) //��������� ��������� �� ������
 {ferr << "ERROR:  " << title << " to receive it was not possible." << endl;
  ferr << "REASON: The given line contains superfluous tokens, except a\n";
  ferr << "        value of performance - <" << TableStrBool[index].s << ">."
       << endl;
  ferr << "Line N " << CurRow << ": <" << line << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << endl;
  ferr << "============== The block diagram of Performances ===============";
  Display(ferr,Perform,sz_Perform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
 }
return lSuccess;
}
//****************************************************************************************
//������: III.10., protected, class member-function
//������������ �������:
//bool GetNextStrPosAfterSep(const string& line, int& lpos, const char* sep=NULL)
//����������:
//������� ������������� ��� ����������� ������� ������� ���������� �� ��������-
//������������, ������� ���������� � ������� sep (�� ��������� - ';'). ����� ��-
//����� � ������ line ����� �������. ���� ������-����������� �� ������, �������
//���������� false, � lpos = NPOS. ���� ������-����������� ������, �� �� ��������
//��������� �������� � ������, �� lpos = NPOS.
//----------------------------------------------------------------------------------------
//������������ ���������:
//1. const string& line - ������ �� ������, � ������� �������������� �����������
//   ������� �������, ���������� �� ��������-������������.
//2. int lpos - ��������� ������� ������� � ������ line, ���������� �� ��������-
//   ������������ �� ������� sep.
//3. const char* sep - ���������� �������, ����������� �� �������-�����������
//----------------------------------------------------------------------------------------
//������������ ��������: bool lSuccess
//true - ������-����������� ������ � ������
//false - ������-����������� � ������ �� ������
//****************************************************************************************
bool TLinearArray::GetNextStrPosAfterSep(const string& line, int& lpos, const char* sep)
{ bool lSuccess = false;
  if (sep == NULL) sep = ";";
  //���������� �������������� �������-����������� � ������ line
  lpos = line.find_first_of(sep);
  //������-����������� � ������ �����������
  if (lpos == string::npos) return lSuccess;
  //������-����������� ������. ����������� ��� �� ��, ��� �� ��������� � ������
  lSuccess = true;
  if (lpos == (int) line.length()-1) //������-����������� ��������� � ������
   lpos = string::npos;
  else lpos++;
return lSuccess;
}
//****************************************************************************
//������: III.11., protected, class member-function
//������������ �������:
// bool StrToDouble(double& value, const string& line, const char** pattern,
//                  const uint sz_pattern, const string& title,
//                  const char** perform,const uint sz_perform);
//����������:
//������ ������: <KEY STRING <ASSIGNMENT OPERATOR> <value>;>
//<KEY STRING> = <str1> || <str2> || ... <strN>
//<ASSIGNMENT OPERATOR> = <=> || <:=> ...
//<value> - �������� ���� double
//<value> = [SIGN]DIGIT1_k[.]DIGITk+1_n[e||E[SIGN]DIGIT1_3]
//<SIGN> = <+> || <->; <DIGIT> = <0123456789>
//������� ������������� ��� ���������� �������� ������ ��������� ��������, ����-
//��� ���������� � ���������� ������� pattern � ������ line � ��������������
//����������� ������������� ������������� �����, ������� ������ ��������� �����
//��������� ������������, � �������� ���� double. ������ ������ �������� ����.
//� ������ ������, ������� ���������� �������� false � ���������� �� ������ ��-
//������������ � �������� ������ ������ ferr. ������� ��������� �������������
//�������� ������ � ������ Line, � ����� ��������� �������������� ������, ���-
//������ � ������������ �� ������� ������ ������������ ����. � ����� �������
//��������� ������� ������� ';', ������� � ������ ������ ��������, ����� �����-
//��� � ���������, ��������� ������������, � ����� �������� �������� � ������,
//������� ���������� ��� ������������� ������������� ����� � ���������� ����.
//��� �������� ������ � �������������� ����� � ���������� ���� � ���������� ����
//double  value ��������� ��������� �������������� ������ � ������������ ��������.
//���������� ���������� ������������ ��������:
// 1. string Buffer; 2. string Line; 3. CurRow
//-----------------------------------------------------------------------------
//������������ ���������:
//1. double& value - ������ �� ����������, ������� ����� ��������
//   ��������� ������ ������� � ������ �� ��������� ����������.
//2. const string& line - ������ �� ������, � ������� �������������� �����
//   �������� ������.
//3. const char** pattern - ������ ����� ��������� ��������� �������� �����
//4. const uint sz_pattern - ���������� ����� � ������� pattern
//5. const string& title - ������ �� ������ � ������������� ������� ��������
//   ������ � �������� ������ fin
//6. const char** perform - ������ �����, ���������� ������ ���������, �������
//   ���������� ������� �� �����
//7. const uint sz_perform - ���������� ����� � ������� Perform
//-----------------------------------------------------------------------------
//������������ ��������: bool lSuccess
// true -  � ������ ������� � ������� ������� ������������ �����
// false - ����� ��� ������� �� ������ ������������ ����� �� �������
//*****************************************************************************
bool TLinearArray::StrToDouble(double& value, const string& line,
     const char** pattern, const uint sz_pattern, const string& title,
     const char** perform, const uint sz_perform)
{bool lSuccess;
 int rpos;
 int lpos;
 //����� �������-����������� ';' ����� ��������� ��������, ������� ��������
 //�����������, � ������ Line
 rpos = line.find_first_of(';');
 if (rpos == string::npos) //������
 {lSuccess = false;
  ferr << "ERROR:  " << title << " to receive it was not possible." << endl;
  ferr << "REASON: Complete token <;> was expected for this data string" << endl;
  ferr << "Line N " << CurRow << ": <" << line << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << endl;
  ferr << "============== The block diagram of Performances ===============";
  Display(ferr,perform,sz_perform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return lSuccess;
 }
 //��������� �� line ��������� s1 � ����������� �������� ';'
 string s1 = line.substr(0,rpos+1);
 //s1.set_case_sensitive(0); //��������� ����������-�������������� �����
 //����� �������� ����� �� ������� ����� pattern � ������ s1
 KEYSTRPOS keystr_info;
 lSuccess = SearchKeyString(s1, pattern, sz_pattern, &keystr_info);

 if (lSuccess == false) //������: �������� ������ �� �������
 {ferr << "ERROR:  " << title << " to receive it was not possible." << endl;
  ferr << "REASON: Key string to find it was not possible, the logic value of\n";
  ferr << "        performance is not defined." << endl;
  ferr << "Line N " << CurRow << ": <" << s1 << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">";
  s1 = "The following key strings were expected:";
  Display(ferr, pattern, sz_pattern, s1.c_str());
  ferr << endl;
  ferr << "============== The block diagram of Performances ===============";
  Display(ferr,perform,sz_perform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return lSuccess;
 }
 //����� ������� ������������ �� �������-������� AssignSep � ������ s1. �������
 //������� ������� �����������: <=>, <:=> � �.�.
 KEYSTRPOS assign_info;
 lSuccess = SearchKeyString(s1, AssignSep, sz_AssignSep, &assign_info);
 if (lSuccess == false) //������ ������� ������������ ����� �� �������
 {ferr << "ERROR: " << title << " to receive it was not possible."
       << endl;
  ferr << "REASON: Assignment operator was expected for a key string <"
       << pattern[assign_info.index] << ">" << endl;
  ferr << "Line N " << CurRow << ": <" << s1 << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">";
  s1 = "The following ASSIGN OPERATORs were expected:";
  Display(ferr,AssignSep,sz_AssignSep,s1.c_str(),"< "," >;");
  ferr << endl;
  ferr << "======== The block diagram of Linear Array Performances ========";
  Display(ferr,LAAPerformPattern,sz_LAAPerform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return lSuccess;
 }
 //����� ����������� �������� ����� ������� s1 � ���������� ������������. �����
 //������� ������� � ��������� ������ �������� ���� �� ������
 //����� ����������� �������� ����� �� ��������� �������� ������
 string s2 = s1.substr(0,keystr_info.lpos);
 lpos = s2.find_first_not_of(" \t");
  if (lpos != string::npos) lSuccess = false;//������: ����������� ������� �������
 //����� ����������� �������� ������ �� ��������� �������� ������ �� ���������
 //������������
 if (lSuccess != false)
 { s2 = s1.substr(keystr_info.rpos+1,assign_info.lpos-keystr_info.rpos-1);
   lpos = s2.find_first_not_of(" \t");
   if (lpos != string::npos) lSuccess = false;//������: ����������� ������� �������
 }
 if (lSuccess == false) //��������� ��������� �� ������
 {ferr << "ERROR:  " << title << " to receive it was not possible." << endl;
  ferr << "REASON: To the left of assignment operator a PROHIBITED TOKENS are";
  ferr << " detected." << endl;
  ferr << "Line N " << CurRow << ": <" << s1 << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << "The following allowed tokens were expected: <[SPACE],[TAB]>." << endl;
  ferr << endl;
  ferr << "============== The block diagram of Performances ===============";
  Display(ferr,perform,sz_perform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return lSuccess;
 }
 //����� ������������� �����, ������� ������ ��������� ����� ����� ������������.
 s1 = s1.substr(assign_info.rpos+1);
 lpos = s1.find_first_of(RealNumber);

 if (lpos == string::npos) //������: ����� ��������� ������������ ������� �� �����
 {lSuccess = false;
  ferr << "ERROR: " << title << " to receive it was not possible." << endl;
  ferr << "REASON: After assignment operator the non-digit token(s) are detected\n";
  ferr << "        for a key string <" << pattern[keystr_info.index] << ">"
       << endl;
  ferr << "Line N " << CurRow << ": <" << s1 << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << "The following digit tokens were expected: <" << RealNumber << ">.";
  ferr << endl << endl;
  ferr << "======== The block diagram of Linear Array Performances ========";
  Display(ferr,perform,sz_perform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return lSuccess;
 }
 //����� ������ ������� ������������� �����
 rpos = s1.find_first_not_of(RealNumber.c_str(),lpos);
 rpos--;
 //����� ����������� �������� ����� ���������� ������������ � ';'. �����
 //������� ������� � ��������� ������ �������� ���� �� ������
 //����� ����������� �������� ����� �� ���������� ��������� ��������
 s2 = s1.substr(0,lpos);
 int pos = s2.find_first_not_of(" \t");
 if (pos != string::npos) lSuccess = false;//������: ����������� ������� �������
 //����� ����������� �������� ������ �� ���������� ��������� �������� �� ';'
 if (lSuccess != false)
 { s2 = s1.substr(rpos+1);
   pos = s2.find_first_not_of(" \t;");
   if (pos != string::npos) lSuccess = false;//������: ����������� ������� �������
 }
 if (lSuccess == false) //��������� ��������� �� ������
 {ferr << "ERROR:  " << title << " to receive it was not possible." << endl;
  ferr << "REASON: To the right of assignment operator a PROHIBITED TOKENS are";
  ferr << " detected." << endl;
  ferr << "Line N " << CurRow << ": <" << s1 << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << "The following allowed tokens were expected: <[SPACE],[TAB]>." << endl;
  ferr << endl;
  ferr << "============== The block diagram of Performances ===============";
  Display(ferr,perform,sz_perform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return lSuccess;
 }
 //�������������� ����� �� ���������� ������������� � ������������ ���� double
 s1 = s1.substr(lpos,rpos-lpos+1); //���������� ����� � ���� ������
 char* endptr = NULL;
 value = strtod(s1.c_str(),&endptr);
 if ((value == +HUGE_VAL) || (value == -HUGE_VAL)) //������
 {lSuccess = false;
  ferr << "ERROR:  " << title << " to receive it was not possible." << endl;
  ferr << "REASON: Transformation of character representation of number in a\n";
  ferr << "floating-point number is impossible. Check a format of number."<<endl;
  ferr << "Line N " << CurRow << ": <" << s1 << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << endl;
  ferr << "============== The block diagram of Performances ===============";
  Display(ferr,perform,sz_perform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return lSuccess;
 }
 lSuccess = true;
 return lSuccess;
}
//****************************************************************************
//������: III.12., protected, class member-function
//������������ �������:
// bool GetLawFromStr(const INFO_LAW* Table, const uint sz_Table, uint& index,
//      const string& title, const char** perform, const uint sz_perform);
//����������:
//������ ������: <KEY STRING;>
//<KEY STRING> = <str1> || <str2> || ... <strN>
//������� ������������� ��� ������ �������� ������ - ������������ ������, ����-
//��� �������� � ������� Table � ������ Line � � ������ �������� ������ �������-
//��� ���������� index ������� ������, � ������� ���������� ���������� �� ������-
//���� ������������ ������. ������ ������ �������� ����.
//� ������ ������, ������� ���������� �������� false � ���������� �� ������ ��-
//������������ � �������� ������ ������ ferr. ������� ��������� �������������
//�������� ������ � ������ Line, � ����� ��������� �������������� ������, ���-
//������ � ������������ �� ������� ������ ������������ ����. � ����� �������
//��������� ������� ������� ';', ������� � ������ ������ ��������, ����� �����-
//��� � ���������.
//���������� ���������� ������������ ��������:
// 1. string Buffer; 2. string Line; 3. CurRow
//-----------------------------------------------------------------------------
//������������ ���������:
//1. const INFO_LAW* Table - ��������� �� ������� � ����������� � ���������
//   ������������� �������, ������� ����� ��������� ������ Line
//2. const uint sz_Table - ������ ������� Table
//3. uint& index - ������ �� ����������, � ������� ����� ��������� ������
//   ������ � ������� Table, � ������ �������� ������ ��� � ������ Line
//4. const string& title - ������ �� ������ � ������������� ������� ��������
//   ������ � �������� ������ fin
//5. const char** perform - ������ �����, ���������� ������ ���������, �������
//   ���������� ������� �� �����
//6. const uint sz_perform - ���������� ����� � ������� Perform
//-----------------------------------------------------------------------------
//������������ ��������: bool lSuccess
// true -  � ������ Line ������������ ������ �������
// false - � ������ Line ����������� ������������ ������ ����� �� �������
//*****************************************************************************
bool TLinearArray::GetLawFromStr(const INFO_LAW* Table, const uint sz_Table,
     uint& index,const string& title,const char** perform,const uint sz_perform)
{bool lSuccess;
 uint rpos;
 uint lpos;
 //����� �������-����������� ';' ����� ��������� ��������, ������� ��������
 //�����������, � ������ Line
 rpos = Line.find_first_of(';');
 if (rpos == string::npos) //������
 {lSuccess = false;
  ferr << "ERROR:  " << title << " to receive it was not possible." << endl;
  ferr << "REASON: Complete token <;> was expected for this data string" << endl;
  ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << endl;
  ferr << "============== The block diagram of Performances ===============";
  Display(ferr,perform,sz_perform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return lSuccess;
 }
 //��������� �� line ��������� s1 � ����������� �������� ';'
 string s1 = Line.substr(1,rpos+1);
 //s1.set_case_sensitive(0); //��������� ����������-�������������� �����
 //����� ����������� ������������ ������ �� ������� ������� Table � ������ s1
 KEYSTRPOS keystr_info;
 for (uint i = 0; i < sz_Table; i++)
 {lSuccess=SearchKeyString(s1,Table[i].pattern,Table[i].sz_pattern,&keystr_info);
  if (lSuccess) {index = i; break;}
 }
 if (lSuccess == false) //������: ������������ ������ �� �������
 {ferr << "ERROR:  " << title << " to receive it was not possible." << endl;
  ferr << "REASON: Law name to find it was not possible." << endl;
  ferr << "Line N " << CurRow << ": <" << s1 << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">";
  s1 = "The following LAW NAMEs were expected:";
  string s2;
  char str[20];
  for (uint i = 0; i < sz_Table; i++)
  {itoa(i+1,str,10); s2 = str; s2 =  s2 + ". " + s1;
   Display(ferr, Table[i].pattern, Table[i].sz_pattern, s2.c_str());
  }
  ferr << endl;
  ferr << "============== The block diagram of Performances ===============";
  Display(ferr,perform,sz_perform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return lSuccess;
 }
//����� ����������� �������� ����� � ������ �� ������������ ������. �����
//������� ������� � ��������� ������ �������� ���� �� ������.
//����� ����������� �������� ����� �� ������������ ������
 string s2 = s1.substr(0,keystr_info.lpos);
 lpos = s2.find_first_not_of(" \t");
  if (lpos != string::npos) //������: ����������� ������� �������
  {s2 = "To the left of "; lSuccess = false;}
//����� ����������� �������� ������ �� ������������ ������ �� ';'
 if (lSuccess != false)
 { s2 = s1.substr(keystr_info.rpos+1);
   lpos = s2.find_first_not_of(" \t;");
   if (lpos != string::npos) //������: ����������� ������� �������
   {s2 = "To the right of "; lSuccess = false;}
 }
 if (lSuccess == false) //��������� ��������� �� ������
 {ferr << "ERROR:  " << title << " to receive it was not possible." << endl;
  ferr << "REASON: " << s2 << "law name a PROHIBITED TOKENS are";
  ferr << " detected." << endl;
  ferr << "Line N " << CurRow << ": <" << s1 << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << "The following allowed tokens were expected: <[SPACE],[TAB]>." << endl;
  ferr << endl;
  ferr << "============== The block diagram of Performances ===============";
  Display(ferr,perform,sz_perform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return lSuccess;
 }
 //������������ ������ ����������
 if (rpos != Line.length()-1) Line = Line.substr(rpos+1);
 else Line = "";
return lSuccess;
}
//****************************************************************************
//������: III.13., protected, class member-function
//������������ �������:
//ARGLIST* GetLawParamFromFile(const INFO_LAW* Table, const uint index,
//         const string& title, const char** perform, const uint sz_perform);
//����������:
//������ ������: <PARAMETERS: ARG1; ARG2; ... ARGn;>
//<ARGn> = [IDENT <=>] <value>; IDENT - ������������� ��������� (������������)
//<IDENT> = LETTER+[LETTER||DIGIT||OTHER]+[LETTER||DIGIT||OTHER]+ ...]
//<OTHER> = <[]-_> for example: lv[-1_0]
//<value> - �������� ��������
//������� ������������� ��� ���������� ���������� ������ �� ��������� ������ fin.
//���������� �� ���������� ����������� ���������� � ������� ������ ARGLIST arg,
//� ������� ��� ����� �������������, ���������� � ������� ������� Table. ������
//index ��������� �� ����� � Table �� �������� ��������� �����, ��� ���������
//����� ����������� �� �����. ��������� ����� ������������� �� ���������� ����-
//��� �����. ������ ������ �������� ����. ���������, ������� �� ����������� ��
//������ ������ ������� ��������� �� ��������� ������� ��� �������� ���������
//����� - PARAMETERS. ���� ����� �������� ���������, �� � ������ ��������� �����-
//������ ����������� ����� ��������, ������� ������ ���� ������� �����.
//��������� ������ � ����� ������ �������� ������������� �������. ����� ������
//��������� ��������� ����� ���� ������ ��� ������������� � ����������� �������-
//��� ������������ ���� <=>.
//� ������ ������, ������� ���������� �������� LawArg == NULL � ���������� ��
//������ �������������� � �������� ������ ������ ferr. ������� ��������� ������-
//������� �������� ������ � ������ Line, � ����� ��������� �������������� ������,
//��������� � ������������ �� ������� ������ ������������ ����. � ����� �������
//��������� ������� ������� ';', ������� � ������ ������ ��������, ����� �����-
//��� � ���������.
//���������� ���������� ������������ ��������:
// 1. string Buffer; 2. string Line; 3. CurRow
//-----------------------------------------------------------------------------
//������������ ���������:
//1. const INFO_LAW* Table - ��������� �� ������� � ����������� � ���������
//   ������������� �������, ������� ����� ��������� ������ Line
//2. const uint index - ������ ������ � ������� Table ��� �������� ����� �����-
//   ������ ��������� �� �����
//3. const string& title - ������ �� ������ � ������������� ������� ��������
//   ������ � �������� ������ fin
//4. const char** perform - ������ �����, ���������� ������ ���������, �������
//   ���������� ������� �� �����
//5. const uint sz_perform - ���������� ����� � ������� Perform
//-----------------------------------------------------------------------------
//������������ ��������: ARGLIST* LawArg - ������ ��������� �� ����� ����������
//������ �� ������� ������� Table ���������� ��� �������� index
//� ������ ���������� ���������� ���������� LawArg == NULL
//*****************************************************************************
ARGLIST* TLinearArray::GetLawParamFromFile(const INFO_LAW* Table,
         const uint index, const string& title, const char** perform,
         const uint sz_perform)
{ARGLIST* LawArg = NULL;
 //�������� ������������ ����� ����� ������ ����������, ����������� �����������
 //���������� � ������� ��������, � ������� ����� ������������� � ARGLIST arg
 //��������� ���������
 if ((Table[index].total_arg != 0) && (Table[index].read_arg != 0))
 if ((Table[index].total_arg < Table[index].read_arg) ||
     (Table[index].total_arg < (Table[index].read_arg+Table[index].offset)))
 //������: �������������� ����� ��������� ���������� ����������� ������
 {ferr << "ERROR:  LAW PARAMETERS for " << title << " to receive it was not "
       << "possible." << endl;
  ferr << "REASON: " << "Detect mismatch between total parameters, amount of\n";
  ferr << "        reading parameters and initial position." << endl;
  ferr << "Law name: <" << Table[index].pattern[1] << ">;" << endl;
  ferr << "Total_Arg = " << Table[index].total_arg << endl;
  ferr << "Read_Arg  = " << Table[index].read_arg << endl;
  ferr << "Init_Pos  = " << Table[index].offset << endl;
  ferr << "TIPS: Total_Arg >= Read_Arg + Init_Pos." << endl;
  ferr << endl;
  ferr << "============== The block diagram of Performances ===============";
  Display(ferr,perform,sz_perform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return LawArg;
 }
 //*************************************************************************
 //1. �������� ������ ARGLIST, � ������� ����� ���������� ��������� ������
 //*************************************************************************
 if (Table[index].ident != TABLE_LAW)
  LawArg = CreateArgList(Table[index].total_arg);
 //*************************************************************************
 //2. �������� �� ������������� ���������� ���������� �� �����.
 //*************************************************************************
 if ((Table[index].ident != TABLE_LAW) && (Table[index].read_arg == 0))
  return LawArg;
 //******************************************************
 //3. ����� ��������� ����� <PARAMETERS>
 //******************************************************
 bool lSuccess;
 string name = "PARAMETERS";
 KEYSTRPOS info;
 lSuccess = SearchKeyString(Line, LawParamPattern, sz_LawParam, &info);
 if (lSuccess == false) //�������� ����� �� �������
 {LawArg = DeleteArgList(LawArg);
  ferr << "ERROR:  " << name << " for " << title << " to receive it was not "
       << "possible." << endl;
  ferr << "REASON: KEY STRING to find it was not possible." << endl;
  ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">";
  name =  "The following KEY STRINGs were expected: ";
  Display(ferr, LawParamPattern, sz_LawParam, name.c_str());
  ferr << endl;
  ferr << "============== The block diagram of Performances ===============";
  Display(ferr,perform,sz_perform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return LawArg;
 }
 //***********************************************************************
 //4. ����� ������� ����������� ����� <PARAMETERS> � ����������� ������
 //***********************************************************************
 uint rpos = Line.find_first_of(":",info.rpos);
 if (rpos == string::npos) //������ �������-����������� <:=> �� �������
 {LawArg = DeleteArgList(LawArg);
  ferr << "ERROR:  " << name << " for " << title << " to receive it was not "
       << "possible." << endl;
  ferr << "REASON: After <" << name << "> the token-separator should follow.\n";
  ferr << "The following token were expected: <:>." << endl;
  ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << endl;
  ferr << "============== The block diagram of Performances ===============";
  Display(ferr, perform, sz_perform, "", "", "", false);
  ferr << "================================================================";
  ferr << endl;
  return LawArg;
 }
//����� ����������� �������� ����� � ������ �� ��������� �����. ����� ��������
//������� � ��������� ������ �������� ���� �� ������.
//����� ����������� �������� ����� �� ��������� �����.
 string s1 = Line.substr(0,info.lpos);
 uint lpos = s1.find_first_not_of(" \t");
  if (lpos != string::npos) //������: ����������� ������� �������
  {s1 = "To the left of "; lSuccess = false;}
//����� ����������� �������� ������ �� ������������ ������ �� ':'
 if (lSuccess != false)
 { s1 = Line.substr(info.rpos+1,rpos-info.rpos-1);
   lpos = s1.find_first_not_of(" \t");
   if (lpos != string::npos) //������: ����������� ������� �������
   {s1 = "To the right of "; lSuccess = false;}
 }
 if (lSuccess == false) //��������� ��������� �� ������
 {ferr << "ERROR:  " << name << " for " << title << " to receive it was not "
       << "possible." << endl;
  ferr << "REASON: " << s1 << " <" << name << "> a PROHIBITED TOKENS are";
  ferr << " detected." << endl;
  ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << "The following allowed tokens were expected: <[SPACE],[TAB]>.";
  name =  "The only following KEY STRINGs were expected: ";
  Display(ferr, LawParamPattern, sz_LawParam, name.c_str());
  ferr << endl;
  ferr << "============== The block diagram of Performances ===============";
  Display(ferr,perform,sz_perform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  LawArg = DeleteArgList(LawArg);
  return LawArg;
 }
 //�������� ����� <PARAMETERS> ����������
 if (rpos != Line.length()-1) Line = Line.substr(rpos+1);
 else Line = "";
//***************************************************************************
//5. ���������������� ���������� ���������� ������� ������ �� ����� �
//   ��������� �� � ������ LawArg
//***************************************************************************
//���������� �������� �� ����� ���������. ��� ���������� ������ ������ ��������
//��������� �� ���-�� ����������, ������� ��������� ������� �� �����
 ulong ReadArg = 0L;  //���������� ��������� ����������
 ulong TotalArg = 0L;//����� ����� ���������� ������� ��������� �������
 double Value = 0.0;
 name = "PARAMETERs for "; name = name + title;
 //��������� ����� ��� �������� ���������� ����������� �� ����� ����������
 if ((Table[index].ident == TABLE_LAW) && (Table[index].total_arg == 0))
 {long value = 0L;
  lSuccess = GetLongFromFile(value,TotalArg,ReadArg,name,perform,sz_perform);
  if (lSuccess == false) //������ ��� ���������� ����� �� �����
  {LawArg = DeleteArgList(LawArg); return LawArg;}
  if (value < 0) TotalArg = value*(-1L);
  else TotalArg = value;
  LawArg = CreateArgList(TotalArg+1);
 }
 //��������� ����� � ��������� ���������� ����������� �� ����� ����������
 else if ((Table[index].ident == TABLE_LAW) && (Table[index].total_arg != 0))
 {LawArg = CreateArgList(Table[index].total_arg);
  TotalArg = Table[index].total_arg;
 }
 else {TotalArg = Table[index].read_arg;}
 //���� ���������� ���������� ������
 for (ulong i = 0; i < TotalArg; i++)
 {lSuccess = GetDoubleFromFile(Value,TotalArg,ReadArg,name,perform,sz_perform);
  //������ ��� ���������� ������
  if (lSuccess == false) {LawArg = DeleteArgList(LawArg); return LawArg;}
  //��������� �������� ������ �� ��������� ������ �������
  ReadArg++;
  LawArg->List[Table[index].offset+i] = Value;
 }
return LawArg;
}
//****************************************************************************
//������: III.14., protected, class member-function
//������������ �������:
// bool GetDoubleFromFile(double& value, const ulong total, const ulong read,
//      const string& title, const char** perform, const uint sz_perform);
//����������:
//������ ������: [<IDENTIFICATOR <ASSIGNMENT OPERATOR>] <value>;>
//<IDENTIFICATOR> = LETTER+[LETTER||DIGIT||OTHER]+[LETTER||DIGIT||OTHER]+ ...]
//<OTHER> = <[]-_> for example: lv[-1_0]
//<ASSIGNMENT OPERATOR> = <=> || <:=> ...
//<value> - �������� ���� double
//<value> = [SIGN]DIGIT1_k[.]DIGITk+1_n[e||E[SIGN]DIGIT1_3]
//<SIGN> = <+> || <->; <DIGIT> = <0123456789>
//������� ������������� ��� ��������� � ������ Line, ����������� �� ���������
//������ fin, ����������� ������������� ������������� ����� � �������������� ���
//� ������������ ����� ���� double. ���������� ������ ������ �������� ����. ��
//������� �����, ��� ������������� ����� ����� �������������� �������������, ��-
//����� �������� ���������� ������������ ���������, ����� �� ��������������� ���-
//��� ��������� �������� ������������ � ����� - ���������� ������������� �����.
//� ������ ������, ������� ���������� �������� false � ���������� �� ������ ��-
//������������ � �������� ������ ������ ferr. ������� ��������� �������������
//����� � �������� ������ fin, � ����� ��������� �������������� ������, ������-
//��� � ������������ �� ������� ������ ������������ ����. � ����� �������
//��������� ������� ������� ';', ������� � ������ ������ ��������, ����� �����-
//��� � ���������, ��������� ������������, � ����� �������� �������� � ������,
//������� ���������� ��� ������������� ������������� ����� � ���������� ����.
//��� �������� ������ � �������������� ����� �� ����������� ���� � ����������
//value ���� double ��������� ��������� �������������� ���� ������.
//��������� ���������� ������������ �������� ������ ��� ������������ ���������
//� ������ ������������� ������ ���������� ������������� ����� �� �����.
//���������� ���������� ������������ ��������:
// 1. string Buffer; 2. string Line; 3. CurRow; 4. ifstream fin; ofstream ferr;
//-----------------------------------------------------------------------------
//������������ ���������:
//1. double& value - ������ �� ����������, ������� ����� ��������
//   ��������� ������ ������� � ������ �� ��������� ����������.
//2. const ulong total - ����� ���������� ������������ �����, ������� ����������
//   ������� �� ��������� ������ fin
//3. const ulong read - ����� ��������� �� ��������� ������ ������������ �����
//4. const string& title - ������ �� ������ � ������������� ���� ��������� ���
//   �������� ����������� ������������ ����� �� ��������� ������ fin
//5. const char** perform - ������ �����, ���������� ������ ���������, �������
//   ���������� ������� �� �����
//6. const uint sz_perform - ���������� ����� � ������� perform
//-----------------------------------------------------------------------------
//������������ ��������: bool lSuccess
// true -  ��������� ������������ ����� ������� �� ����� �������
// false - �������� ������ ��� ���������� ������������� ����� �� �����
//*****************************************************************************
bool TLinearArray::GetDoubleFromFile(double& value, const ulong total,
     const ulong read, const string& title, const char** perform,
     const uint sz_perform)
{bool lSuccess = true;
 uint rpos;
 uint lpos;
 string ABC(EngABC); //EngABC + Engabc + RusABC + Rusabc + RealNumber + ';';
 ABC.append(Engabc); ABC.append(RusABC); ABC.append(Rusabc);
 ABC.append(RealNumber); ABC.append(1,';');
 //���� ������ �������-����������� ';' ����� �������, ������� ��������
 //�����������, � ������ Line
 do
 {rpos = Line.find_first_of(';');
  if (rpos == string::npos) //������ ';' � ������� ������ Line ����� �� ������
  {//������ �� ����������� ������ �������� � ������, ����� �������� � ���������
   //������ �������� ���� �� ������
   lpos = Line.find_first_not_of(" \t");
   if (lpos != string::npos) //������!
   {lSuccess = false; value = +HUGE_VAL;
    ferr << "ERROR:  " << title << " to receive it was not possible." << endl;
    ferr << "REASON: Complete token <;> was expected for this data string or\n";
    ferr << "        at this line there are superfluous tokens." << endl;
    ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
    ferr << "BUFFER: <" << Buffer << ">" << endl;
    ferr << "Total parameters: " << total << endl;
    ferr << "Amount of obtained parameters: " << read << endl;
    ferr << endl;
    ferr << "============== The block diagram of Performances ===============";
    Display(ferr,perform,sz_perform,"","","",false);
    ferr << "================================================================";
    ferr << endl;
    return lSuccess;
   }
   //��������� ����� ������ �� ��������� ������ fin
   lSuccess = GetLineWithoutComment(fin, Line, ABC, CommentSep, sz_CommentSep);
   if (lSuccess == false) //������! ��������� ����� �����
   {value = +HUGE_VAL;
    ferr << "ERROR:  " << title << " to receive it was not possible." << endl;
    ferr << "REASON: End of the file is reached. It was't possible to receive\n";
    ferr << "        from the file the next real number." << endl;
    ferr << "Total parameters: " << total << endl;
    ferr << "Amount of obtained parameters: " << read << endl;
    ferr << endl;
    ferr << "============== The block diagram of Performances ===============";
    Display(ferr,perform,sz_perform,"","","",false);
    ferr << "================================================================";
    ferr << endl;
    return lSuccess;
   }
  }
 } while(rpos == string::npos); //����� ����� ������ ������� ';'
 //��������� ��������� �� ������� ';' ������������ �� Line � ��������� �����
 //������ Line, �������� �������, ����������� ����� rpos
 string s1 = Line.substr(0,rpos+1);
 string s2 = s1;
 if (rpos != Line.length()-1) Line = Line.substr(rpos+1);
 else Line = "";
 //������������ ����� ������� ������������� ������� � ������ s1. ����� ������
 //����������� ����� �������������� � ���� ������.
 rpos = s1.find_first_not_of(" \t");
 //����������� ��������� ������ �� ������ ����������� ��� �������� ��������
 ABC = EngABC + Engabc + RusABC + Rusabc;
 lpos = ABC.find(s1[rpos]);
 //��������� ������ �������� ������. ������� ������������� ���������
 if (lpos != string::npos)
 {//���� ��������� ������ ��������������
  s1 = s1.substr(rpos);
  ABC = EngABC + Engabc + RusABC + Rusabc + Digits + Sign + "_[],";
  //������ ���������� ������� �������������� � s1
  rpos = s1.find_first_not_of(ABC);
  //������������ ����� ��������� ������������, ������� ������ ��������� �����
  //�������������� ���������, � ������ s1
  KEYSTRPOS info;
  lSuccess = SearchKeyString(s1, AssignSep, sz_AssignSep, &info);
  if (lSuccess == false)//������! ������ ������������ �� ������
  {value = +HUGE_VAL;
   ferr << "ERROR:  " << title << " to receive it was not possible." << endl;
   ferr << "REASON: ASSIGNMENT OPERATOR after the parameter identifier <"
        << s1.substr(0,rpos) << ">" << endl;
   ferr << "        was expected." << endl;
   ferr << "Line N " << CurRow << ": <" << s1 << ">" << endl;
   ferr << "BUFFER: <" << Buffer << ">" << endl;
   ferr << "Total parameters: " << total << endl;
   ferr << "Amount of obtained parameters: " << read;
   s1 = "The following ASSIGNMENT OPERATORs were expected:";
   Display(ferr,AssignSep,sz_AssignSep,s1.c_str(),"< "," >;");
   ferr << endl;
   ferr << "============== The block diagram of Performances ===============";
   Display(ferr,perform,sz_perform,"","","",false);
   ferr << "================================================================";
   ferr << endl;
   return lSuccess;
  }
  //����� ������ �������� ����� ��������������� � ���������� ������������
  //����� �������� ������� � ��������� ������ �������� �� ������ ����
  s2 = s1.substr(rpos,info.lpos-rpos);
  lpos = s2.find_first_not_of(" \t");
  if (lpos != string::npos) //������! ������� ������ �������
  {lSuccess = false; value = +HUGE_VAL;
   ferr << "ERROR:  " << title << " to receive it was not possible." << endl;
   ferr << "REASON: Between identifier <" << s1.substr(0,rpos) << ">"
        << " and ASSIGNMENT OPERATOR\nsuperfluous tokens are detected." << endl;
   ferr << "Line N " << CurRow << ": <" << s1 << ">" << endl;
   ferr << "BUFFER: <" << Buffer << ">" << endl;
   ferr << "Total parameters: " << total << endl;
   ferr << "Amount of obtained parameters: " << read << endl;
   ferr << endl;
   ferr << "============== The block diagram of Performances ===============";
   Display(ferr,perform,sz_perform,"","","",false);
   ferr << "================================================================";
   ferr << endl;
   return lSuccess;
  }
  //��������� ���������, ����������� ������ ������� ������������ �� s1
  s1 = s1.substr(info.rpos+1);
 }//������������� ��������� � �������� ������������ ������� �������
 //����������� ������ s1 �� ����������� � ��� �������� ��������
 rpos = s1.find_first_not_of(" \t");
 ABC = Sign + Digits + ".";
 lpos = ABC.find(s1[rpos]);
 //��������� ������ �������� �������� ��������. ������� �������� ���������
 if (lpos != string::npos)
 {//���� ��������� �������� ������ �������� ���������
  s1 = s1.substr(rpos);
  ABC = RealNumber;
  //������ ���������� ��������� ������� �������� ��������� � s1
  rpos = s1.find_first_not_of(ABC);
  //����������� �������� ������� � ������������ �����
  s2 = s1.substr(0,rpos);
  char* endptr;
  value = strtod(s2.c_str(), &endptr);
  //������! ������� ������� ��� ��������� �����
  if ((value == +HUGE_VAL) || (value == -HUGE_VAL))
  {lSuccess = false;
   ferr << "ERROR:  " << title << " to receive it was not possible." << endl;
   ferr << "REASON: The real number <" << s2 << "> oversteps the bounds of\n"
        << "        an admissible range." << endl;
   ferr << "Line N " << CurRow << ": <" << s1 << ">" << endl;
   ferr << "BUFFER: <" << Buffer << ">" << endl;
   ferr << "Total parameters: " << total << endl;
   ferr << "Amount of obtained parameters: " << read << endl;
   ferr << endl;
   return lSuccess;
  }
  //������ �� ����������� ������ �������� ����� �����
  s2 = s1.substr(rpos);
  lpos = s2.find_first_not_of(" \t;");
  if (lpos != string::npos) //������! ������� ������ �������
  {lSuccess = false;
   ferr << "ERROR:  " << title << " to receive it was not possible." << endl;
   ferr << "REASON: After number <" << value << "> superfluous numerals "
        << "are found." << endl;
   ferr << "Line N " << CurRow << ": <" << s1 << ">" << endl;
   ferr << "BUFFER: <" << Buffer << ">" << endl;
   ferr << "Total parameters: " << total << endl;
   ferr << "Amount of obtained parameters: " << read << endl;
   ferr << endl;
   ferr << "============== The block diagram of Performances ===============";
   Display(ferr,perform,sz_perform,"","","",false);
   ferr << "================================================================";
   ferr << endl;
   value = +HUGE_VAL;
   return lSuccess;
  }
 }
 else //������! ������ �� �������� ��������
 {lSuccess = false; value = +HUGE_VAL;
  ferr << "ERROR:  " << title << " to receive it was not possible." << endl;
  ferr << "REASON: Token <" << s1[rpos] << "> is found which is not a DIGITAL "
       << "NUMERAL." << endl;
  ferr << "Line N " << CurRow << ": <" << s1 << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << "Total parameters: " << total << endl;
  ferr << "Amount of obtained parameters: " << read << endl;
  ferr << endl;
  ferr << "============== The block diagram of Performances ===============";
  Display(ferr,perform,sz_perform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
 }
return lSuccess;
}
//****************************************************************************
//������: III.15., protected, class member-function
//������������ �������:
// bool GetLongFromFile(long& value, const ulong total, const ulong read,
//      const string& title, const char** perform, const uint sz_perform);
//����������:
//������ ������: [<IDENTIFICATOR <ASSIGNMENT OPERATOR>] <value>;>
//<IDENTIFICATOR> = LETTER+[LETTER||DIGIT||OTHER]+[LETTER||DIGIT||OTHER]+ ...]
//<OTHER> = <[]-_> for example: lv[-1_0]
//<ASSIGNMENT OPERATOR> = <=> || <:=> ...
//<value> - �������� ���� long
//<value> = [SIGN]DIGIT1_k
//<SIGN> = <+> || <->; <DIGIT> = <0123456789>
//������� ������������� ��� ��������� � ������ Line, ����������� �� ���������
//������ fin, ����������� ������������� ������ ����� � �������������� ���
//� ������� ����� ����� ���� long. ���������� ������ ������ �������� ����. ��
//������� �����, ��� ������  ����� ����� �������������� �������������, �������
//�������� ���������� ������������ ���������, ����� �� ��������������� ������
//��������� �������� ������������ � ����� - ���������� ������������� �����.
//� ������ ������, ������� ���������� �������� false � ���������� �� ������ ��-
//������������ � �������� ������ ������ ferr. ������� ��������� �������������
//����� � �������� ������ fin, � ����� ��������� �������������� ������, ������-
//��� � ������������ �� ������� ������ ������������ ����. � ����� �������
//��������� ������� ������� ';', ������� � ������ ������ ��������, ����� �����-
//��� � ���������, ��������� ������������, � ����� �������� �������� � ������,
//������� ���������� ��� ������������� �������� ������ ����� � ���������� ����.
//��� �������� ������ � �������������� ����� �� ����������� ���� � ����������
//value ���� long ��������� ��������� �������������� ���� ������.
//��������� ���������� ������������ �������� ������ ��� ������������ ���������
//� ������ ������������� ������ ���������� ������������� ����� �� �����.
//���������� ���������� ������������ ��������:
// 1. string Buffer; 2. string Line; 3. CurRow; 4. ifstream fin; ofstream ferr;
//-----------------------------------------------------------------------------
//������������ ���������:
//1. long& value - ������ �� ����������, ������� ����� ��������
//   ��������� ������ ������� � ������ �� ��������� ����������.
//2. const ulong total - ����� ���������� ������������ �����, ������� ����������
//   ������� �� ��������� ������ fin
//3. const ulong read - ����� ��������� �� ��������� ������ ������������ �����
//4. const string& title - ������ �� ������ � ������������� ���� ��������� ���
//   �������� ����������� ������������ ����� �� ��������� ������ fin
//5. const char** perform - ������ �����, ���������� ������ ���������, �������
//   ���������� ������� �� �����
//6. const uint sz_perform - ���������� ����� � ������� perform
//-----------------------------------------------------------------------------
//������������ ��������: bool lSuccess
// true -  ��������� ������������ ����� ������� �� ����� �������
// false - �������� ������ ��� ���������� ������������� ����� �� �����
//*****************************************************************************
bool TLinearArray::GetLongFromFile(long& value, const ulong total,
     const ulong read, const string& title, const char** perform,
     const uint sz_perform)
{bool lSuccess = true;
 uint rpos;
 uint lpos;
 string ABC(EngABC); //EngABC + Engabc + RusABC + Rusabc + Digits + Sign + ';';
 ABC.append(Engabc); ABC.append(RusABC); ABC.append(Rusabc);
 ABC.append(Digits); ABC.append(Sign); ABC.append(1,';');
 //���� ������ �������-����������� ';' ����� �������, ������� ��������
 //�����������, � ������ Line
 do
 {rpos = Line.find_first_of(';');
  if (rpos == string::npos) //������ ';' � ������� ������ Line ����� �� ������
  {//������ �� ����������� ������ �������� � ������, ����� �������� � ���������
   //������ �������� ���� �� ������
   lpos = Line.find_first_not_of(" \t");
   if (lpos != string::npos) //������!
   {lSuccess = false; value = 0L;
    ferr << "ERROR:  " << title << " to receive it was not possible." << endl;
    ferr << "REASON: Complete token <;> was expected for this data string or\n";
    ferr << "        at this line there are superfluous tokens." << endl;
    ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
    ferr << "BUFFER: <" << Buffer << ">" << endl;
    ferr << "Total parameters: " << total << endl;
    ferr << "Amount of obtained parameters: " << read << endl;
    ferr << endl;
    ferr << "============== The block diagram of Performances ===============";
    Display(ferr,perform,sz_perform,"","","",false);
    ferr << "================================================================";
    ferr << endl;
    return lSuccess;
   }
   //��������� ����� ������ �� ��������� ������ fin
   lSuccess = GetLineWithoutComment(fin, Line, ABC, CommentSep, sz_CommentSep);
   if (lSuccess == false) //������! ��������� ����� �����
   {value = 0L;
    ferr << "ERROR:  " << title << " to receive it was not possible." << endl;
    ferr << "REASON: End of the file is reached. It was't possible to receive\n";
    ferr << "        from the file the next real number." << endl;
    ferr << "Total parameters: " << total << endl;
    ferr << "Amount of obtained parameters: " << read << endl;
    ferr << endl;
    ferr << "============== The block diagram of Performances ===============";
    Display(ferr,perform,sz_perform,"","","",false);
    ferr << "================================================================";
    ferr << endl;
    return lSuccess;
   }
  }
 } while(rpos == string::npos); //����� ����� ������ ������� ';'
 //��������� ��������� �� ������� ';' ������������ �� Line � ��������� �����
 //������ Line, �������� �������, ����������� ����� rpos
 string s1 = Line.substr(0,rpos+1);
 string s2 = s1;
 if (rpos != Line.length()-1) Line = Line.substr(rpos+1);
 else Line = "";
 //������������ ����� ������� ������������� ������� � ������ s1. ����� ������
 //����������� ����� �������������� � ���� ������.
 rpos = s1.find_first_not_of(" \t");
 //����������� ��������� ������ �� ������ ����������� ��� �������� ��������
 ABC = EngABC + Engabc + RusABC + Rusabc;
 lpos = ABC.find(s1[rpos]);
 //��������� ������ �������� ������. ������� ������������� ���������
 if (lpos != string::npos)
 {//���� ��������� ������ ��������������
  s1 = s1.substr(rpos);
  ABC = EngABC + Engabc + RusABC + Rusabc + Digits + Sign + "_[],";
  //������ ���������� ������� �������������� � s1
  rpos = s1.find_first_not_of(ABC);
  //������������ ����� ��������� ������������, ������� ������ ��������� �����
  //�������������� ���������, � ������ s1
  KEYSTRPOS info;
  lSuccess = SearchKeyString(s1, AssignSep, sz_AssignSep, &info);
  if (lSuccess == false)//������! ������ ������������ �� ������
  {value = 0L;
   ferr << "ERROR:  " << title << " to receive it was not possible." << endl;
   ferr << "REASON: ASSIGNMENT OPERATOR after the parameter identifier <"
        << s1.substr(0,rpos) << ">" << endl;
   ferr << "        was expected." << endl;
   ferr << "Line N " << CurRow << ": <" << s1 << ">" << endl;
   ferr << "BUFFER: <" << Buffer << ">" << endl;
   ferr << "Total parameters: " << total << endl;
   ferr << "Amount of obtained parameters: " << read;
   s1 = "The following ASSIGNMENT OPERATORs were expected:";
   Display(ferr,AssignSep,sz_AssignSep,s1.c_str(),"< "," >;");
   ferr << endl;
   ferr << "============== The block diagram of Performances ===============";
   Display(ferr,perform,sz_perform,"","","",false);
   ferr << "================================================================";
   ferr << endl;
   return lSuccess;
  }
  //����� ������ �������� ����� ��������������� � ���������� ������������
  //����� �������� ������� � ��������� ������ �������� �� ������ ����
  s2 = s1.substr(rpos,info.lpos-rpos);
  lpos = s2.find_first_not_of(" \t");
  if (lpos != string::npos) //������! ������� ������ �������
  {lSuccess = false; value = 0L;
   ferr << "ERROR:  " << title << " to receive it was not possible." << endl;
   ferr << "REASON: Between identifier <" << s1.substr(0,rpos) << ">"
        << " and ASSIGNMENT OPERATOR\nsuperfluous tokens are detected." << endl;
   ferr << "Line N " << CurRow << ": <" << s1 << ">" << endl;
   ferr << "BUFFER: <" << Buffer << ">" << endl;
   ferr << "Total parameters: " << total << endl;
   ferr << "Amount of obtained parameters: " << read << endl;
   ferr << endl;
   ferr << "============== The block diagram of Performances ===============";
   Display(ferr,perform,sz_perform,"","","",false);
   ferr << "================================================================";
   ferr << endl;
   return lSuccess;
  }
  //��������� ���������, ����������� ������ ������� ������������ �� s1
  s1 = s1.substr(info.rpos+1);
 }//������������� ��������� � �������� ������������ ������� �������
 //����������� ������ s1 �� ����������� � ��� �������� ��������
 rpos = s1.find_first_not_of(" \t");
 ABC = Sign + Digits;
 lpos = ABC.find(s1[rpos]);
 //��������� ������ �������� �������� ��������. ������� �������� ���������
 if (lpos != string::npos)
 {//���� ��������� �������� ������ �������� ���������
  s1 = s1.substr(rpos);
  //������ ���������� ��������� ������� �������� ��������� � s1
  rpos = s1.find_first_not_of(ABC);
  //����������� �������� ������� � ������� ����� �����
  s2 = s1.substr(0,rpos);
  char* endptr;
  value = strtol(s2.c_str(), &endptr, 10);
  //������ �� ����������� ������ �������� ����� �����
  s2 = s1.substr(rpos);
  lpos = s2.find_first_not_of(" \t;");
  if (lpos != string::npos) //������! ������� ������ �������
  {lSuccess = false;
   ferr << "ERROR:  " << title << " to receive it was not possible." << endl;
   ferr << "REASON: After number <" << value << "> superfluous numerals "
        << "are found." << endl;
   ferr << "Line N " << CurRow << ": <" << s1 << ">" << endl;
   ferr << "BUFFER: <" << Buffer << ">" << endl;
   ferr << "Total parameters: " << total << endl;
   ferr << "Amount of obtained parameters: " << read << endl;
   ferr << endl;
   ferr << "============== The block diagram of Performances ===============";
   Display(ferr,perform,sz_perform,"","","",false);
   ferr << "================================================================";
   ferr << endl;
   value = 0L;
   return lSuccess;
  }
 }
 else //������! ������ �� �������� ��������
 {lSuccess = false; value = 0L;
  ferr << "ERROR:  " << title << " to receive it was not possible." << endl;
  ferr << "REASON: Token <" << s1[rpos] << "> is found which is not a DIGITAL "
       << "NUMERAL." << endl;
  ferr << "Line N " << CurRow << ": <" << s1 << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << "Total parameters: " << total << endl;
  ferr << "Amount of obtained parameters: " << read << endl;
  ferr << endl;
  ferr << "============== The block diagram of Performances ===============";
  Display(ferr,perform,sz_perform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
 }
return lSuccess;
}
//****************************************************************************
//������: III.16., protected, class member-function
//������������ �������: bool GetDataFromFile()
//����������:
//������� ������������� ��� ������������� ������������� �������� �������� �����-
//�� �� ������, ����������� � ��������� ����� ������������ ����. ������� � ����
//����� ������ ���������� ������ ���� ������ �������:
//1. GetLAAPerformFromFile() 2. GetElemArrayPerformFromFile(TElArray& ElArray)
//������ ������� ������������� ��� ������������� ������������� ������������
//������������� �������� �������� �������. ������ ������� �������������� ����-
//����� ��������� ��������� �������� �������. ������� ������������� ������ ����-
//��� ������� �� ������������� �������� ��: ������������ � ������������. ����
//�������� �� �������� ����������, �� ������ ������� ���������� ����� ��� ����:
//��� ������������� ������������� ��������� �������� �� � ��� ������������� ��-
//����������� ���� ��������� ���������� ��������� ��. ���� ������� ������������,
//�� ���������� ������� ������� ������������ ������������� ��, ������� � �������-
//�� ���������� ����� ���������� ��������������� ��������� �������.
//������ �� ��������������� �������� �� ����������� �� �������� ��������� ������
//fin. � ���� ���������� ������ �������������� �� �������������� � �������������
//������ �� ������������ ����������, ������� �������� ����. ��� ����������� ���-
//��� ������� ��������� ���� ������, ������ ��������� � ���������������� �����-
//����� ������ ������� ��������������� � �������� �������� ������ ������ ferr c
//���������������� �������������.
//���������� ���������� ������������ ��������:
// 1. string Buffer; 2. string Line; 3. CurRow
//-----------------------------------------------------------------------------
//������������ ���������: ���
//-----------------------------------------------------------------------------
//������������ ��������: bool lSuccess
// true -  �������� �������� �� �� ����� fname ������� �������
// false - �������� �������� �� �� ����� fname ������� �� �������
//*****************************************************************************
bool TLinearArray::GetDataFromFile()
{ bool lSuccess;

 //��������� ������������� ���������� ����������
 CurRow = 0; //Buffer.skip_whitespace(0);
 //Line.skip_whitespace(0); Line.set_case_sensitive(0);
 #if defined(CONSOLE)
  cout << endl;
  cout << "Reading data on Linear Array Properties from <"
       << uppercase << fname << "> ...";
 #endif

 //���������� ������������� �������� ������ �������� ��
 lSuccess = GetLAAPerformFromFile();
 #if defined(CONSOLE)
  gotoxy(1,wherey()); clreol(); gotoxy(1,wherey()); //������� ������
 #endif
 //� ���� ���������� ������ ���������� ������ - ���������� ������
 if (lSuccess == false) return lSuccess;
 //��������� ������ ��� �������� ���������� �� ��������� ��
 //�������� ������� �������� TElArray � ������� ������������ �� ���������
 Count = Left + Together + Right + 1;
 data = new TElArray[Count];

 //���������� ������������� �� ��������� �������� �������
 //1. ��������� ������������� ��������� �������� ��
 TElArray ElArray;
 lSuccess = GetElemArrayPerformFromFile(ElArray, MASTER);
 if (lSuccess) { ElArray.SetNum(0L); data[0] = ElArray; }
 else return lSuccess;

 //2. ��������� ������������� �� ��������������� ��������� ��
 //   ��� ���������� �������� �������� ������� ����������� ������ ���� ��������-
 //   ������� �������� �������
 bool flag = true;
 for (ulong i = 1; i <= (Count-1); i++)
 { if (flag) lSuccess = GetElemArrayPerformFromFile(ElArray, SLAVE);
   flag = (Uniformity == UNIFORM) ? false : true;
   if (lSuccess) //���������� ������ ��������� �������
   {//��������� ����������� ������� ������������ ��������������� ��������� ��
    if (i <= Left) ElArray.SetNum((Left-i+1)*(-1L));
    else if ((i > Left) && (i <= (Left+Together))) ElArray.SetNum(0L);
    else ElArray.SetNum(i-Left-Together);
    data[i] = ElArray;
   }
   else //������ ��� ���������� ������������� i-�� �������� ��
   {

    return lSuccess;
   }
 }
 //���������� �� ����� ��������� �������. ��������� ������������� ����������
 //����� ���������� ��
 CalcRelDistance();
return lSuccess;
}
//****************************************************************************
//������: III.17., protected, class member-function
//������������ �������: bool GetLAAPerformFromFile()
//����������:
//������� ������������� ��� ������������� ������������� ������������ ������ ��-
//������ �������� �������. � ����� ��������������� ��������� ���������:
//1. CONFIGURATION: LEFT <uint>; TOGETHER <uint>; RIGHT <uint> - ������������
//   �������� �������, ���������� ��������� ������������� �����, ������ � ������
//   � �������� ��������� �������� �������. ������ ������� �������� �������
//   ������������ �� ���� �������. ������������ ���-�� ��������� � ������ �� 3
//   ������� 65535. ����������� ���������� ��������� � �����-���� �� 3 �������
//   ��� ����������, ��� � ������������. ������������ ����������� ������� � ���,
//   ��� ��������� ���������� ��������� ����� � ������ ������ ���� ������ 1, ���
//   � ��������� ������ �������� ������� ����������� � �������.
//2. UNIFORMITY: <uniform || nonuniform> - ������������ �������� ��. ��� �������-
//   ������ �� ����������, ��� ��� ��������������� �������� �� ����� ����������
//   �������� �������������� - ��������� � ������ ���� ������, ���������� �������
//   ������������� � ������������, � ����������� �������� ����������������.
//   �������� ������� �� ����� ����� ������ ��� �������. � ������ ������������
//   �� ��������� ��� ��������� ��������� �������� - ��������� ���� ������, ���-
//   ������ ������������ � ������������, ��������� ��������� ������ ������ ����
//   � �.�. ���������� �������� ������� ����� ���������� ���������������.
//3. EQUIDISTANCE: <equidistant || unequidistant>, LV = <double> - �����������-
//   ����� ��. ��� ����������������� �� ���������� ���������� (������) ������-
//   ������� ���������� ����� ����� ��������� ���������� ��. ���������� ���
//   �������� �������� �������� ����� ������������� ���������� - ��������� ���-
//   ������������ ���������� ����� ��������� ���������� � ����� �����, �������
//   ����������� �������� ��������. ���� ������� �������� ��������������, �� ��-
//   ��� � ���� �� ������ ����������� LV = <double> - ������������� ���� ��������
//   ������� (������������� ���������� ����� ��������� ���������� ��), �������
//   ����� ��������� ��� ���� ��� �������� ��������� �������.
//4. LEFTDISTLAW: <law name> PARAMETERS <double_1,...double_N>
//5. RIGHTDISTLAW: <law name> PARAMETERS <double_1,...double_N> - ������, ����-
//   ��� ���������� ������������� ���������� ����� ��������� ���������� ��, ���-
//   �����������, ��������������, ����� � ������ �� ��������� ��������, � ������
//   ���� ������� �� �������� ��������������. � ��������� ������, ��� ������
//   ������������. �� ������� ���������� ������������, �������� ���������� ����-
//   ������� ���������� ����� ���������� ������ �������� ��������� �� �����������
//   ������������� ���������� ����� ������ �� ����� �������� ��, ������� �����
//   (������) ��������� ��������. �������� �������� ���������� ������ ���������
//   ����� ����� ��������� ����� PARAMETERS, ����� ������� ��������� ������������-
//   ��� ����� <law name>, �� ���������� ������������ ���������� �������. ����-
//   ����� ������ ����� ������������� �� ���������� �������, ���������� ��������
//   ��������� ������ ����� �������������� ��������������� ���������, �������
//   ����������� ������ ���������� � �����, ����� ���� ������ ��������� �������-
//   ������� ������ (separator - "= :") � ����� ������������ ��� ����� �����.
//   ������� ���������� ����� ���� ������ ������������� ��������� (>0). ���� ��-
//   ��� ������������ (��������� �����) ����������� ������������� ����������
//   ����� ��������� ����������, �� ����� ������������ ������ <TABLE_LAW>, ���-
//   ����� ������� � ����� ������, � ����������� ����� ������� ������ ���� ����-
//   ������� ��� ������������� ���������� ����� ��������� ����������, �������
//   ������������� ����� � ������ ��������� ��������. �������� �������������
//   ������ � �������� ��������� � ������ �� �����������, ��� ��� �� ����������-
//   ��� ���������� ���� � ������ ����� ����. ��� ������������� ���������� ����-
//   �� ���� �������������� (>0.0) ����������, � ��������� ������ ������������
//   ������. � ������ ������������� ������ ������� ������������� ����������, ���-
//   ���� ��������, ����� ����� �������������� ���������, ������� ������ ����-
//   ������ � �����, ����� ��������� � ���������� ������ ���� ����������� ����-
//   ���������� ���� - "= :". ���� ����� �� �������� ������������, �� ����������
//   ������������� ���������� �� �������� ����� ���������� ���������� �� ��������-
//   ������ �� ��������� �����. ������� ���������� ����� ������ ��������� �� �
//   �������� ��������� ����� ������� ���������� ��� ������� LEFTDISTLAW (RIGHT-
//   DISTLAW) ��� ����������� ���������� (����� LV) ����� ������ � ������ ������-
//   ��� (LV2-1). ����� LV2-1 ����� ������� ���������� ��� ��� �� ������� ���
//   ���������� LV3-2 � ��� ����� �� �������: LV[i+1,i] = FUNC(LV[i,i-1]).
//6. DISPOSITION: <equator || meridian> - ������������� ������������ �������� ��
//   ������������ ���������� �����. EQUATOR - ������������ �� �����������, ���-
//   ������ �������������� ����� �������� �� ����� �������� � ����������� �� ��-
//   �� �� ������� �� �������� �������. MERIDIAN - ������������ �� ���������,
//   ��������� �������������� �� ����� �������� � ����������� �� ���� ����� ��-
//   ������� �������. ��� ���������� �������������� ���������� ������� ���������
//   �������������� ��. ���� �� ����������� �����������, �� �������� ����������-
//   ��� ���� ��������� �������� ����� ���������� � ����� ��������� ��, � �����-
//   �������� ���� ��������� �������� ����� ���������� � ������ ��������� ��.
//� �������� ���������� ������������� ������������� �������������� � ����������-
//��� �������� ������������ �����. � ������ ������ ������� ��������� ���� ������
//�������������� � ������������ ��������� ������ � ����������� ������������� �
//����� ferrname. � �������� ��������� ������ ������� ���������� ������� �����-
//��� ����� fin, ��������������� � ������ ����� fname.
//
//���������� ���������� ������������ ��������:
// 1. string Buffer; 2. string Line; 3. CurRow
//-----------------------------------------------------------------------------
//������������ ���������: ���
//-----------------------------------------------------------------------------
//������������ ��������: bool lSuccess
// true -  �������� �������� �� �� ����� fname ������� �������
// false - �������� �������� �� �� ����� fname ������� �� �������
//*****************************************************************************
bool TLinearArray::GetLAAPerformFromFile()
{ bool lSuccess;
  string ABC(EngABC); //���������� ����� ��������
  ABC.append(RusABC);
  string title;
  //******************************************************
  //1. ����� ������-��������� ������������� �������� ��
  //******************************************************
  //ABC = EngABC + RusABC;
  lSuccess = GetLineWithoutComment(fin, Line, ABC, CommentSep, sz_CommentSep);
  //������ ���������� ������� ABC ��� ������������ �� �������.
  //��������� ����� �����, ��������� ��������� �� ������
  if (lSuccess == false)
  {ferr << "ERROR:  End of file is reached. It was not possible to receive\n";
   ferr << "        a TITLE of Linear Array performances.";
   title = "REASON: The following names of Linear Array TITLE were expected:";
   Display(ferr,LAAHeaderPattern,sz_LAAHeader,title.c_str(),"< "," >;");
   ferr << endl;
   ferr << "======== The block diagram of Linear Array Performances ========";
   Display(ferr,LAAPerformPattern,sz_LAAPerform,"","","",false);
   ferr << "================================================================";
   ferr << endl;
   return lSuccess; //����������� ������ �������
  }
  lSuccess = SearchKeyString(Line, LAAHeaderPattern, sz_LAAHeader);
  //������-��������� �� �������, ��������� �� ����� ������ Line ��������
  //������������ ����������. ��������� ��������� �� ������
  if (lSuccess == false)
  {ferr << "ERROR:  The TITLE of Linear Array not found." << endl;
   ferr << "REASON: The following line of file contains an incorrect data.\n";
   ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
   ferr << "BUFFER: <" << Buffer << ">";
   title = "The following names of Linear Array TITLE were expected:";
   Display(ferr,LAAHeaderPattern,sz_LAAHeader,title.c_str(),"< "," >;");
   ferr << endl;
   ferr << "======== The block diagram of Linear Array Performances ========";
   Display(ferr,LAAPerformPattern,sz_LAAPerform,"","","",false);
   ferr << "================================================================";
   ferr << endl;
   return lSuccess;
  }
  //******************************************************************
  //2. ����������� ������������ �������� �� - CONFIGURATION PROPERTY
  //******************************************************************
  uint left = 0, together = 0, right = 0;
  lSuccess = GetConfigurationLAA(left,together,right);
  if (lSuccess == false) return lSuccess;
  Left = left; Together = together; Right = right;

  //******************************************************************
  //3. ����������� ������������ �������� �� - UNIFORMITY PROPERTY
  //******************************************************************
  bool uniform = NONUNIFORM;
  lSuccess = GetUniformLAA(uniform);
  if (lSuccess == false) return lSuccess;
  Uniformity = uniform;

  //*********************************************************************
  //4. ����������� ���������������� �������� �� - EQUIDISTANCE PROPERTY
  //*********************************************************************
  bool equal = UNEQUIDISTANT;
  double lv = 0.0;
  lSuccess = GetEquidistLAA(equal, lv);
  if (lSuccess == false) return lSuccess;
  Equidistance = equal;
  //�������� ������� - �������������� - F(i+1,i) = lv, ��� ���� �������������
  //���������� ����� ���������� ��.
  if (equal == EQUIDISTANT)
  { ARGLIST* LawArg = CreateArgList(2);
    LawArg->List[0] = lv; LawArg->List[1] = lv;
    if (left > 0) //� �� ������� �������� ������������� ����� �� ���������
    { LeftLawType = CONST_LAW;
      LeftLaw = ConstLaw;
      LeftLawArg = CopyArgList(LawArg, LeftLawArg);
    }
    if (right > 0) //� �� ������� �������� ������������� ������ �� ���������
    { RightLawType = CONST_LAW;
      RightLaw = ConstLaw;
      RightLawArg = CopyArgList(LawArg, RightLawArg);
    }
  }
  //****************************************************************************
  //5. ����������� ������, ������������� ������������� ���������� ����� ������-
  //   ���� ��, �������������� ����� �� ��������� �������� - LEFTDISTLAW. ������
  //   ����� ���������, ���� �� �������� ���������������� � ���������� ���������
  //   ��, ������������� ����� �� ��������� ������� �� ����.
  //****************************************************************************
  if ((equal == UNEQUIDISTANT) && (left > 0))
  { lSuccess = GetLeftDistLawLAA();
    if (lSuccess == false) return lSuccess;
  }
  //****************************************************************************
  //6. ����������� ������, ������������� ������������� ���������� ����� ������-
  //   ���� ��, �������������� ������ �� ��������� �������� - LEFTDISTLAW. ������
  //   ����� ���������, ���� �� �������� ���������������� � ���������� ���������
  //   ��, ������������� ������ �� ��������� ������� �� ����.
  //****************************************************************************
  if ((equal == UNEQUIDISTANT) && (right > 0))
  { lSuccess = GetRightDistLawLAA();
    if (lSuccess == false) return lSuccess;
  }
  //***************************************************************************
  //7. ����������� ������������ �������� �� ������������ ������ �����������
  //***************************************************************************
  bool disp = EQUATOR; //�� ������������ - ������������ ����
  lSuccess = GetDisposLAA(disp);
  if (lSuccess) Disposition = disp;

return lSuccess;
}
//****************************************************************************
//������: III.18., protected, class member-function
//������������ �������:
//  bool GetConfigurationLAA(uint& left, uint& together, uint& right)
//����������:
//������� ������������� ��� ��������� �������� � ������������ �������� ��, �����-
//����� � ������� �������� �� ������� �������� ������ fin. ������� ������ ����-
//���� �������� ����� <CONFIGURATION> � ���������� ���������� ��������� ��, ��-
//����� ������������� ������, ����� � ������ � �������� ���������. ���-�� ������-
//��� �� � ��������� ������� �� ������ ��������� 65535 ���������. � ������ ���-
//���������� ������ ��������������� ��� �������������� ��������� ��������������
//�� ����������� � ����� ferrname, ��������� � �������� ������� ������ ferr, �
//������� ��������� ���� ������ � ���������� �������� false.
//���������� ���������� ������������ ��������:
// 1. string Buffer; 2. string Line; 3. CurRow
//-----------------------------------------------------------------------------
//������������ ���������:
//1. uint& left - ���������� ��������������� ��������� ��, ������������� �����
//   ��������� �������� ��
//2. uint& together - ���������� ��������������� ��������� ��, �������������
//   ������ � �������� ��������� ��
//3. uint& right - ���������� ��������������� ��������� ��, ������������� ������
//   ��������� �������� ��
//-----------------------------------------------------------------------------
//������������ ��������: bool lSuccess
// true -  ������������ �������� �� �� ����� fname ������� �������
// false - ������������ �������� �� �� ����� fname ������� �� �������
//*****************************************************************************
bool TLinearArray::GetConfigurationLAA(uint& left, uint& together, uint& right)
{ bool lSuccess;
  string ABC(EngABC); //���������� ����� ��������
  ABC.append(RusABC);
  ABC.append(Digits);
  ABC.append(1,';');
  string title;
  //******************************************************
  //1. ����� ��������� ����� <CONFIGURATION>
  //******************************************************
  //ABC = EngABC + RusABC + Digits + ';';

  lSuccess = GetLineWithoutComment(fin, Line, ABC, CommentSep, sz_CommentSep);
  //������ ���������� ������� ABC ��� ������������ �� �������.
  //��������� ����� �����, ��������� ��������� �� ������
  if (lSuccess == false)
  {ferr << "ERROR:  End of file is reached. It was not possible to receive\n";
   ferr << "        a CONFIGURATION of Linear Antenna Array.";
   title = "REASON: The following keywords were expected:";
   Display(ferr,LAAConfigPattern,sz_LAAConfig,title.c_str(),"< "," >;");
   ferr << endl;
   ferr << "======== The block diagram of Linear Array Performances ========";
   Display(ferr,LAAPerformPattern,sz_LAAPerform,"","","",false);
   ferr << "================================================================";
   ferr << endl;
   return lSuccess; //����������� ������ �������
  }
  lSuccess = SearchKeyString(Line, LAAConfigPattern, sz_LAAConfig);
  //�������� ����� �� �������, ��������� �� ����� ������ Line ��������
  //������������ ����������. ��������� ��������� �� ������
  if (lSuccess == false)
  {ferr << "ERROR:  The keyword <CONFIGURATION> is not found." << endl;
   ferr << "REASON: The following line of file contains an incorrect data.\n";
   ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
   ferr << "BUFFER: <" << Buffer << ">";
   title = "The following keywords were expected:";
   Display(ferr,LAAConfigPattern,sz_LAAConfig,title.c_str(),"< "," >;");
   ferr << endl;
   ferr << "======== The block diagram of Linear Array Performances ========";
   Display(ferr,LAAPerformPattern,sz_LAAPerform,"","","",false);
   ferr << "================================================================";
   ferr << endl;
  }
  //�������� ����� <CONFIGURATION> �������. ������������ ����� � ���� �� ������
  //�������� ���� <LEFT> ��� <RIGHT>. ���� �� ��� ������ ����������� ����������-
  //���� � ������ Line
  bool lLeft;
  bool lRight;
  string OldLine;
  KEYSTRPOS info;
  ulong count = 0;
  lSuccess = SearchKeyString(Line, LAALeftPattern, sz_LAALeft, &info);
  lLeft = lSuccess;
  if (lSuccess == false) left = 0; //�������� ����� <LEFT> �� �������
  else //�������� ����� <LEFT> �������, ���������� ���-�� ��������� �����
  {OldLine = Line;
   Line = Line.substr(info.lpos);
   info.rpos -= info.lpos; info.lpos = 0;
   lSuccess = GetCountElArray(count, &info, LAALeftPattern);
   if (lSuccess == false) return lSuccess; //������
   //����� ��������� �������� ������� � ����� �� ���� ������� �� ������ > 65535
   if (count > 65535) //������
   {lSuccess = false;
    ferr << "ERROR:  Linear Array CONFIGURATION to receive it was not possible."
         << endl;
    ferr << "REASON: Total of the elements located to the left of a basic\n"
            "        element of Antenna Array should not exceed 65535." << endl;
    ferr << "Total of the LEFT LOCATED ELEMENTS is <" << count << ">" << endl;
    ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
    ferr << "BUFFER: <" << Buffer << ">" << endl;
    ferr << endl;
    ferr << "======== The block diagram of Linear Array Performances ========";
    Display(ferr,LAAPerformPattern,sz_LAAPerform,"","","",false);
    ferr << "================================================================";
    ferr << endl;
    return lSuccess;
   }
    left = count;
    Line = OldLine; //��������������� �������������� ���������� ������ Line
  }
  //����� ��������� ����� <RIGHT>
  lSuccess = SearchKeyString(Line, LAARightPattern, sz_LAARight, &info);
  lRight = lSuccess;
  if (lSuccess == false) right = 0; //�������� ����� <RIGHT> �� �������
  else //�������� ����� <RIGHT> �������, ���������� ���-�� ��������� ������
  {OldLine = Line;
   Line = Line.substr(info.lpos);
   info.rpos -= info.lpos; info.lpos = 0;
   lSuccess = GetCountElArray(count, &info, LAARightPattern);
   if (lSuccess == false) return lSuccess; //������
   //����� ��������� �������� ������� � ����� �� ���� ������� �� ������ > 65535
   if (count > 65535) //������
   {lSuccess = false;
    ferr << "ERROR:  Linear Array CONFIGURATION to receive it was not possible."
         << endl;
    ferr << "REASON: Total of the elements located to the right of a basic\n"
            "        element of Antenna Array should not exceed 65535." << endl;
    ferr << "Total of the RIGHT LOCATED ELEMENTS is <" << count << ">" << endl;
    ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
    ferr << "BUFFER: <" << Buffer << ">" << endl;
    ferr << endl;
    ferr << "======== The block diagram of Linear Array Performances ========";
    Display(ferr,LAAPerformPattern,sz_LAAPerform,"","","",false);
    ferr << "================================================================";
    ferr << endl;
    return lSuccess;
   }
   right = count;
   Line = OldLine; //��������������� �������������� ���������� ������ Line
  }
  //����������� ���-�� ����� ����� ��������� ��, ������������� ����� � ������
  //������: �������� ��, ������������� ����� � ������ �� ��������� �����������
  if ((left+right) == 0)
  {lSuccess = false;
   ferr << "ERROR:  Linear Array CONFIGURATION to receive it was not possible."
        << endl;
   ferr << "REASON: Total of the Linear Array Elements located at the left and\n"
        << "        to the right of a basic element, is equal to zero." << endl;
   ferr << "TIPS:   The following line of file contains an incorrect data.\n";
   ferr << "        The Linear Array Elements located at the left or to the\n"
        << "        right of a basic element, it is necessary should be."<<endl;
   ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
   ferr << "BUFFER: <" << Buffer << ">" << endl;
   if ((lLeft == false) || (lRight == false))
   {ferr << "The following keywords were expected:";
    if (!lLeft) Display(ferr,LAALeftPattern,sz_LAALeft,NULL,"< "," >;");
    if (!Right) Display(ferr,LAARightPattern,sz_LAARight,NULL,"< "," >;");
   }
   ferr << endl;
   ferr << "======== The block diagram of Linear Array Performances ========";
   Display(ferr,LAAPerformPattern,sz_LAAPerform,"","","",false);
   ferr << "================================================================";
   ferr << endl;
   return lSuccess;
  }
  //����� ��������� ����� <TOGETHER>
  lSuccess = SearchKeyString(Line, LAATogetherPattern, sz_LAATogether, &info);
  if (lSuccess == false) together = 0; //�������� ����� <TOGETHER> �� �������
  else //�������� ����� <TOGETHER> �������, ���������� ���-�� ��������� ��, ���-
       //���������� ������ � �������� ��������� ��.
  {OldLine = Line;
   Line = Line.substr(info.lpos);
   info.rpos -= info.lpos; info.lpos = 0;
   lSuccess = GetCountElArray(count, &info, LAATogetherPattern);
   if (lSuccess == false) return lSuccess; //������
   //����� ��������� �������� ������� � ����� �� ���� ������� �� ������ > 65535
   if (count > 65535) //������
   {lSuccess = false;
    ferr << "ERROR:  Linear Array CONFIGURATION to receive it was not possible."
         << endl;
    ferr << "REASON: Total of the elements located together with a basic\n"
            "        element of Antenna Array should not exceed 65535." << endl;
    ferr << "Total of the TOGETHER LOCATED ELEMENTS is <" << count << ">" << endl;
    ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
    ferr << "BUFFER: <" << Buffer << ">" << endl;
    ferr << endl;
    ferr << "======== The block diagram of Linear Array Performances ========";
    Display(ferr,LAAPerformPattern,sz_LAAPerform,"","","",false);
    ferr << "================================================================";
    ferr << endl;
    return lSuccess;
   }
   together = count;
   Line = OldLine; //��������������� �������������� ���������� ������ Line
  }
return lSuccess;
}
//****************************************************************************
//������: III.19., protected, class member-function
//������������ �������:
// bool GetCountElArray(ulong& count,const pKEYSTRPOS info,const char** pattern)
//����������:
//������ ������: <KEY STRING> = <value> <;>
// value - ����� ������������� ����� ���� ulong
// <KEY STRING> = <LEFT> || <RIGHT> || <TOGETHER>
//������� ������������� ��� ����������� ���������� ��������� �������� ��, �����-
//����� � ������� ������ �������� � ������ Line. ������ ������ �������� ����. �
//������ ������, ������� ���������� �������� false � ���������� �� ������ ������-
//�������� � �������� ������ ������ ferr.
//���������� ���������� ������������ ��������:
// 1. string Buffer; 2. string Line; 3. CurRow
//-----------------------------------------------------------------------------
//������������ ���������:
//1. uint& count - ���������� ��������������� ��������� ��, ������������� �����
//   (������ ��� ������) ��������� �������� ��
//2. const pKEYSTRPOS info - ���������� �� �������������� �������� ������
//3. const char** pattern - ������ �����, ��������������� � �������� �������
//-----------------------------------------------------------------------------
//������������ ��������: bool lSuccess
// true -  ������������ �������� �� �� ����� fname ������� �������
// false - ������������ �������� �� �� ����� fname ������� �� �������
//*****************************************************************************
bool TLinearArray::GetCountElArray(ulong& count, const pKEYSTRPOS info,
                                   const char** pattern)
{bool lSuccess;
 uint rpos;
 count = 0;
 //����� �������-����������� ';' ����� ��������� ��������, ������� ��������
 //�����������, � ������ Line
 rpos = Line.find_first_of(';');
 if (rpos == string::npos) //������
 {lSuccess = false;
  ferr << "ERROR:  Linear Array CONFIGURATION to receive it was not possible."
        << endl;
  ferr << "REASON: Complete token <;> was expected for a key string <"
       << pattern[info->index] << ">" << endl;
  ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << endl;
  ferr << "======== The block diagram of Linear Array Performances ========";
  Display(ferr,LAAPerformPattern,sz_LAAPerform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return lSuccess;
 }
 //��������� �� Line ��������� s1 � ����������� �������� ';'
 uint lpos = info->rpos+1;
 string title;
 string s1 = Line.substr(lpos,rpos-lpos+1);
 string s = Line.substr(info->lpos,rpos+1);
 count = 0;
 //����� ������� ������������ �� �������-������� AssignSep � ������ s1. �������
 //������� ������� �����������: <=>, <:=> � �.�.
 KEYSTRPOS InfoSep;
 lSuccess = SearchKeyString(s1, AssignSep, sz_AssignSep, &InfoSep);
 if (lSuccess == false) //������ ������� ������������ ����� �� �������
 {ferr << "ERROR:  Linear Array CONFIGURATION to receive it was not possible."
       << endl;
  ferr << "REASON: Assignment operator was expected for a key string <"
       << pattern[info->index] << ">" << endl;
  ferr << "Line N " << CurRow << ": <" << s << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">";
  title = "The following ASSIGN OPERATORs were expected:";
  Display(ferr,AssignSep,sz_AssignSep,title.c_str(),"< "," >;");
  ferr << endl;
  ferr << "======== The block diagram of Linear Array Performances ========";
  Display(ferr,LAAPerformPattern,sz_LAAPerform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return lSuccess;
 }
 //����� ����������� � ������������� �������� ����� ������ <��������� ���������>
 //� �������� ������������ '=' ��� ':='
 //������������ ������������ �������� ��������
 string ABC = " \t";
 string s2 = s1.substr(0,InfoSep.lpos);
 lpos = s2.find_first_not_of(ABC);
 if (lpos != string::npos) //������! ����������� ������� �������
 {lSuccess = false;
  ferr << "ERROR:  Linear Array CONFIGURATION to receive it was not possible."
       << endl;
  ferr << "REASON: Between a  key string <" << pattern[info->index] << "> and\n";
  ferr << "        assignment operator the PROHIBITED TOKENS are detected.\n";
  ferr << "Line N " << CurRow << ": <" << s << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << "The following allowed tokens were expected: <[SPACE],[TAB]>." << endl;
  ferr << endl;
  ferr << "======== The block diagram of Linear Array Performances ========";
  Display(ferr,LAAPerformPattern,sz_LAAPerform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return lSuccess;
 }
 //����� ����������� � ������������� �������� ����� ������ <ASSIGNMENT OPERATOR>
 //� ����������� �������� ';'
 //������������ ������������ �������� ��������
 ABC = Digits + " \t;";
 s2 = s1.substr(InfoSep.rpos+1);
 lpos = s2.find_first_not_of(ABC);
 if (lpos != string::npos) //������! ����������� ������� �������
 {lSuccess = false;
  ferr << "ERROR:  Linear Array CONFIGURATION to receive it was not possible."
       << endl;
  ferr << "REASON: Between a assignment operator and completing token ';'\n";
  ferr << "        the PROHIBITED TOKENS are detected.\n";
  ferr << "Line N " << CurRow << ": <" << s << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << "The following allowed tokens were expected: <[SPACE],[TAB],"
       << Digits << ">." << endl;
  ferr << endl;
  ferr << "======== The block diagram of Linear Array Performances ========";
  Display(ferr,LAAPerformPattern,sz_LAAPerform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return lSuccess;
 }
 //������ ������������ ������. ����� ������ �������������� �����, ������� ������
 //��������� ����� ����� ������������. ����� ������ � ������ ������������ �����-
 //������ ������ �������.
 lpos = InfoSep.rpos+1;
 s1 = s1.substr(lpos);
 lpos = s1.find_first_not_of(" \t");
 //����������� ������ ��������� ����� ��������� ������������ ������. �� ������
 //���� ������ ������
 rpos = Digits.find(s1[lpos]);
 if (rpos == string::npos) //������: ����� ��������� ������������ ������� �� �����
 {lSuccess = false;
  ferr << "ERROR:  Linear Array CONFIGURATION to receive it was not possible."
       << endl;
  ferr << "REASON: After assignment operator the non-digit token(s) are detected\n";
  ferr << "        for a key string <" << pattern[info->index] << ">" << endl;
  ferr << "Line N " << CurRow << ": <" << s << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << "The following digits were expected: <" << Digits << ">." << endl;
  ferr << endl;
  ferr << "======== The block diagram of Linear Array Performances ========";
  Display(ferr,LAAPerformPattern,sz_LAAPerform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return lSuccess;
 }
 //������ ������ (�� ����������� ��������) �������� ������. �������� ������
 //���������� ������ �������� ������� � ����������� �� � ����������� ����� �����.
 s1 = s1.substr(lpos);
 rpos = s1.find_first_not_of(Digits);
 s1 = s1.substr(0,rpos);
 char *endptr;
 count = strtoul(s1.c_str(),&endptr,10);

return lSuccess = true;
}
//****************************************************************************
//������: III.20., protected, class member-function
//������������ �������: bool GetUniformLAA(bool& uniform);
//����������:
//������ ������: <UNIFORMITY>: <UNIFORM || NONUNIFORM> <;>
//������� ������������� ��� ��������� �������� �� ������������ �������� ��,
//���������� � ������� �������� �� ������� �������� ������ fin. ������� ������
//�������� �������� ����� <UNIFORMITY> � ���������� ������ �������� �� ��������
//������ �� ������� ������������ �������� ���� � ������� �������� .
//� ������ ������������� ������ ��������������� ��� �������������� ���������
//�������������� �� ����������� � ����� ferrname, ��������� � �������� �������
//������ ferr, � ������� ��������� ���� ������ � ���������� �������� false.
//���������� ���������� ������������ ��������:
// 1. string Buffer; 2. string Line; 3. CurRow
//-----------------------------------------------------------------------------
//������������ ���������:
//1. bool& uniform - ������ �� ��������� ������������ ��������� �������� �������
//-----------------------------------------------------------------------------
//������������ ��������: bool lSuccess
// true -  �������� ������������ �� ����� fname ������� ����������
// false - �������� ������������ �� ����� fname �� ������� ����������
//*****************************************************************************
bool TLinearArray::GetUniformLAA(bool& uniform)
{bool lSuccess;
 string ABC(EngABC); //���������� ����� ��������
 ABC.append(RusABC);
 ABC.append(Digits);
 ABC.append(":;");
 string Title = "UNIFORMITY";
 KEYSTRPOS info;
 //******************************************************
 //1. ����� ��������� ����� <UNIFORMITY>
 //******************************************************
 lSuccess = FindKeyStrInFile(Title, LAAUniformPattern, sz_LAAUniform, Line,
            &info, ABC, CommentSep, sz_CommentSep, fin, ferr, LAAPerformPattern,
            sz_LAAPerform);
 if (lSuccess == false) return lSuccess;//�������� ����� �� �������
 //*****************************************************************
 //2. ����� ������� ����������� ����� <UNIFORMITY> � �� ���������
 //*****************************************************************
 uint rpos = Line.find_first_of(":=",info.rpos);
 if (rpos == string::npos) //������ �������-����������� <:=> �� �������
 {lSuccess = false;
  ferr << "ERROR:  Linear Array UNIFORMITY to receive it was not possible."
       << endl;
  ferr << "REASON: After <UNIFORMITY> the token-separator should follow.\n";
  ferr << "The following tokens were expected: <:> or <=>." << endl;
  ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << endl;
  ferr << "======== The block diagram of Linear Array Performances ========";
  Display(ferr,LAAPerformPattern,sz_LAAPerform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return lSuccess;
 }
 info.rpos = rpos;
 //*******************************************************
 //3. ����������� �������� �������� <UNIFORMITY>
 //*******************************************************
 Title = "Linear Array "+Title;
 lSuccess = StrToBool(uniform, Line, &info, TbUniform, sz_TbUniform, Title,
            LAAPerformPattern, sz_LAAPerform);
return lSuccess;
}
//****************************************************************************
//������: III.21., protected, class member-function
//������������ �������: bool GetEquidistLAA(bool& equal, double& lv);
//����������:
//������ ������: <EQUIDISTANCE>: <EQUIDISTANT || UNEQUIDISTANT;>
//[<LV> <=> <doublevalue>;]
//������� ������������� ��� ��������� �������� �� ���������������� �������� ��,
//���������� � ������� �������� �� ������� �������� ������ fin. ������� ������
//�������� �������� ����� <EQUIDISTANCE> � ���������� ������ �������� �� ��������
//������ �� ������� ������������ �������� ���� � ������� �������� .
//���� ������� ��������������, �� ����� ��������� ����� (ex. <EQUIDISTANT>)
//����������� ������ ��������� �������� �������������� ���������� ����� �������-
//�� ���������� �������� �������. ������ ������ �� �������� ����� �����������
//���������� �������������� ���������������� �������� ������� �������� ����.
//� ������ ������������� ������ ��������������� ��� �������������� ���������
//�������������� �� ����������� � ����� ferrname, ��������� � �������� �������
//������ ferr, � ������� ��������� ���� ������ � ���������� �������� false.
//���������� ���������� ������������ ��������:
// 1. string Buffer; 2. string Line; 3. CurRow
//-----------------------------------------------------------------------------
//������������ ���������:
//1. bool& equal - ������ �� ������� ���������������� ��������� �������� �������
//2. double& lv - ������ �� ���������� � ������������� ����������� ����� �����-
//   ���� ���������� ��. ���� �� ��������������� - lv = 0; � ������ ��������������
//   �� lv ������ ���� ������������� ��������� (lv > 0).
//-----------------------------------------------------------------------------
//������������ ��������: bool lSuccess
// true -  �������� ���������������� �� ����� fname ������� ����������
// false - �������� ���������������� �� ����� fname �� ������� ����������
//*****************************************************************************
bool TLinearArray::GetEquidistLAA(bool& equal, double& lv)
{bool lSuccess;
 string ABC(EngABC); //EngABC + RusABC + Digits + ":;"; //���������� ����� ��������
 ABC.append(RusABC); ABC.append(Digits); ABC.append(":;");
 string Title = "EQUIDISTANCE";
 KEYSTRPOS info;
 //******************************************************
 //1. ����� ��������� ����� <EQUIDISTANCE>
 //******************************************************
 lSuccess = FindKeyStrInFile(Title, LAAEquidistPattern, sz_LAAEquidist, Line,
            &info, ABC, CommentSep, sz_CommentSep, fin, ferr, LAAPerformPattern,
            sz_LAAPerform);
 if (lSuccess == false) return lSuccess;//�������� ����� �� �������
 //*****************************************************************
 //2. ����� ������� ����������� ����� <EQUIDISTANCE> � �� ���������
 //*****************************************************************
 int rpos = Line.find_first_of(":=",info.rpos);
 if (rpos == string::npos) //������ �������-����������� <:=> �� �������
 {lSuccess = false;
  ferr << "ERROR:  Linear Array EQUIDISTANCE to receive it was not possible."
       << endl;
  ferr << "REASON: After <EQUIDISTANCE> the token-separator should follow.\n";
  ferr << "The following tokens were expected: <:> or <=>." << endl;
  ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << endl;
  ferr << "======== The block diagram of Linear Array Performances ========";
  Display(ferr,LAAPerformPattern,sz_LAAPerform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return lSuccess;
 }
 info.rpos = rpos;
 //*******************************************************
 //3. ����������� �������� �������� <EQUIDISTANCE>
 //*******************************************************
 Title = "Linear Array "+Title;
 lSuccess = StrToBool(equal, Line, &info, TbEquidist, sz_TbEquidist, Title,
            LAAPerformPattern, sz_LAAPerform);
 if (lSuccess == false) return lSuccess;
 //*************************************************************
 //4. ����������� �������������� ���������� ����� ����������
 //   �������������� ��
 //*************************************************************
 if (equal == EQUIDISTANT)
 {//���������� ��������� ������� ���������� �� ������ ��������-������������ ';'
  GetNextStrPosAfterSep(Line, rpos);
  if (rpos == string::npos) //������! � ������ Line ����������� ���������� �� LV
  {lSuccess = false;
   ferr << "ERROR:  Linear Array EQUIDISTANCE to receive it was not possible."
        << endl;
   ferr << "REASON: There is no information about a relative distance between\n";
   ferr << "        the elements by an Equidistant Antenna Array." << endl;
   ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
   ferr << "BUFFER: <" << Buffer << ">" << endl;
   ferr << endl;
   ferr << "======== The block diagram of Linear Array Performances ========";
   Display(ferr,LAAPerformPattern,sz_LAAPerform,"","","",false);
   ferr << "================================================================";
   ferr << endl;
   return lSuccess;
  }
  //���������� ������������� ���������� ����� ���������� �������������� ��
  Line = Line.substr(rpos);
  lv = 0.0;
  lSuccess = StrToDouble(lv, Line, LAARelDistPattern, sz_LAARelDist, Title,
                         LAAPerformPattern, sz_LAAPerform);
  if (lSuccess == false) return lSuccess;
  //������ ����������� �������� lv (lv > 0.0)
  if (lv <= 0.0) //������
  {lSuccess = false;
   ferr << "ERROR:  Linear Array EQUIDISTANCE to receive it was not possible."
        << endl;
   ferr << "REASON: The relative distance (LV) between the elements by" << endl;
   ferr << "        an Equidistant Antenna Array is NONPOSITIVE NUMBER !!!"<<endl;
   ferr << "LV = " << lv << ";" << endl;
   ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
   ferr << "BUFFER: <" << Buffer << ">" << endl;
   ferr << endl;
   ferr << "======== The block diagram of Linear Array Performances ========";
   Display(ferr,LAAPerformPattern,sz_LAAPerform,"","","",false);
   ferr << "================================================================";
   ferr << endl;
   return lSuccess;
  }
 }
return lSuccess;
}
//****************************************************************************
//������: III.22., protected, class member-function
//������������ �������: bool GetLeftDistLawLAA();
//����������:
//������ ������: <LEFTDISTLAW>: <Law name;> [<PARAMETERS: ARG1;ARG2;...ARGn;>]
//<Law name> - ������������ ������;
//<PARAMETERS:> - ������ ���������� ������;
//<ARGn> = [LETTER+[LETTER || DIGIT || OTHER] <=> ] double value <;>
//<LETTER> = EngABC + RusABC; DIGIT = Digits; OTHER = <[],->
//double value - ������������ ����� ���� double
//������� ������������� ��� ��������� �������� � ������ �� �������� �����������
//�������� �������� ��, ������������� ����� �� ��������� �������� �������� ��,
//���������� � ������� �������� �� ������� �������� ������ fin, � ������ ������-
//���������� �������. ������� ������ �������� �������� ����� <LEFTDISTLAW>,
//���������� ��� ������ �� �������� ������ �� ������� DistLawTable �, ���������
//���������� � ������ �� ���� �������, ������� ����������� ���������� ����������,
//������� ������ ����� ����������.
//������ ������ �� �������� ����� ����������� ���������� ������ �������� ����.
//� ������ ������������� ������ ��������������� ��� �������������� ���������
//�������������� �� ����������� � ����� ferrname, ��������� � �������� �������
//������ ferr, � ������� ��������� ���� ������ � ���������� �������� false.
//���������� ���������� ������������ ��������:
// 1. string Buffer; 2. string Line; 3. CurRow; 4. DistLawTable
//-----------------------------------------------------------------------------
//������������ ���������: ���
//-----------------------------------------------------------------------------
//������������ ��������: bool lSuccess
// true -  LEFTDISTLAW �� ����� fname ������� ����������
// false - LEFTDISTLAW �� ����� fname �� ������� ����������
//*****************************************************************************
bool TLinearArray::GetLeftDistLawLAA()
{bool lSuccess;
 string ABC(EngABC); //= EngABC + RusABC + Digits + ":;"; //���������� ����� ��������
 ABC.append(RusABC); ABC.append(Digits); ABC.append(":;");
 string Title = "LEFTDISTLAW";
 KEYSTRPOS info;
 //******************************************************
 //1. ����� ��������� ����� <LEFTDISTLAW>
 //******************************************************
 lSuccess = FindKeyStrInFile(Title, LAALfLawPattern, sz_LAALfLaw, Line,
            &info, ABC, CommentSep, sz_CommentSep, fin, ferr, LAAPerformPattern,
            sz_LAAPerform);
 if (lSuccess == false) return lSuccess;//�������� ����� �� �������
 //********************************************************************
 //2. ����� ������� ����������� ����� <LEFTDISTLAW> � �� ���������
 //********************************************************************
 uint rpos = Line.find_first_of(":",info.rpos);
 if (rpos == string::npos) //������ �������-����������� <:=> �� �������
 {lSuccess = false;
  ferr << "ERROR:  Linear Array <LEFTDISTLAW> to receive it was not possible."
       << endl;
  ferr << "REASON: After <LEFTDISTLAW> the token-separator should follow.\n";
  ferr << "The following tokens were expected: <:> or <=>." << endl;
  ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << endl;
  ferr << "======== The block diagram of Linear Array Performances ========";
  Display(ferr,LAAPerformPattern,sz_LAAPerform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return lSuccess;
 }
 Line = Line.substr(rpos);
 //****************************************************************************
 //3. ����������� ���� ������ ��������� ������������ ��������� ��, �����������
 //   ����� �� ��������� ��������
 //****************************************************************************
 uint Index = 0; //������ ������������ ������ � ������� DistLawTable
 lSuccess = GetLawFromStr(DistLawTable, sz_DistLawTable, Index, Title,
                          LAAPerformPattern, sz_LAAPerform);
 if (lSuccess == false) return lSuccess;
 //��� ������ ������������ ��������� �� ������ �������
 LeftLawType = DistLawTable[Index].ident;
 LeftLaw = DistLawTable[Index].law;
 //****************************************************************************
 //4. ��������� ���������� ��� ������ ��������� ������������ ��������� ��,
 //   ����������� ����� �� ��������� ��������
 //****************************************************************************
 LeftLawArg = GetLawParamFromFile(DistLawTable, Index, Title, LAAPerformPattern,
                                  sz_LAAPerform);
 if (LeftLawArg == NULL) {lSuccess = false; return lSuccess;}
 //�������� LEFTDISTLAW ������ ���� ������������� ���������
 if (LeftLawType != TABLE_LAW) LeftLawArg->List[0] = fabs(LeftLawArg->List[0]);
 else //��� ���������� ������ ��� ������������� ���������� �������������
  for (ulong i = 0; i < LeftLawArg->Size; i++)
   LeftLawArg->List[i] = fabs(LeftLawArg->List[i]);
return lSuccess;
}
//****************************************************************************
//������: III.23., protected, class member-function
//������������ �������: bool GetRightDistLawLAA();
//����������:
//������ ������: <RIGHTDISTLAW>: <Law name;> [<PARAMETERS: ARG1;ARG2;...ARGn;>]
//<Law name> - ������������ ������;
//<PARAMETERS:> - ������ ���������� ������;
//<ARGn> = [LETTER+[LETTER || DIGIT || OTHER] <=> ] double value <;>
//<LETTER> = EngABC + RusABC; DIGIT = Digits; OTHER = <[],->
//double value - ������������ ����� ���� double
//������� ������������� ��� ��������� �������� � ������ �� �������� �����������
//�������� �������� ��, ������������� ������ �� ��������� �������� �������� ��,
//���������� � ������� �������� �� ������� �������� ������ fin, � ������ ������-
//���������� �������. ������� ������ �������� �������� ����� <RIGHTDISTLAW>,
//���������� ��� ������ �� �������� ������ �� ������� DistLawTable �, ���������
//���������� � ������ �� ���� �������, ������� ����������� ���������� ����������,
//������� ������ ����� ����������.
//������ ������ �� �������� ����� ����������� ���������� ������ �������� ����.
//� ������ ������������� ������ ��������������� ��� �������������� ���������
//�������������� �� ����������� � ����� ferrname, ��������� � �������� �������
//������ ferr, � ������� ��������� ���� ������ � ���������� �������� false.
//���������� ���������� ������������ ��������:
// 1. string Buffer; 2. string Line; 3. CurRow; 4. DistLawTable
//-----------------------------------------------------------------------------
//������������ ���������: ���
//-----------------------------------------------------------------------------
//������������ ��������: bool lSuccess
// true -  RIGHTDISTLAW �� ����� fname ������� ����������
// false - RIGHTDISTLAW �� ����� fname �� ������� ����������
//*****************************************************************************
bool TLinearArray::GetRightDistLawLAA()
{bool lSuccess;

 string ABC(EngABC); //= EngABC + RusABC + Digits + ":;"; //���������� ����� ��������
 ABC.append(RusABC); ABC.append(Digits); ABC.append(":;");

 string Title = "RIGHTDISTLAW";
 KEYSTRPOS info;
 //******************************************************
 //1. ����� ��������� ����� <RIGHTDISTLAW>
 //******************************************************
 lSuccess = FindKeyStrInFile(Title, LAARtLawPattern, sz_LAARtLaw, Line,
            &info, ABC, CommentSep, sz_CommentSep, fin, ferr, LAAPerformPattern,
            sz_LAAPerform);
 if (lSuccess == false) return lSuccess;//�������� ����� �� �������
 //********************************************************************
 //2. ����� ������� ����������� ����� <RIGHTDISTLAW> � �� ���������
 //********************************************************************
 uint rpos = Line.find_first_of(":",info.rpos);
 if (rpos == string::npos) //������ �������-����������� <:=> �� �������
 {lSuccess = false;
  ferr << "ERROR:  Linear Array <RIGHTDISTLAW> to receive it was not possible."
       << endl;
  ferr << "REASON: After <RIGHTDISTLAW> the token-separator should follow.\n";
  ferr << "The following tokens were expected: <:> or <=>." << endl;
  ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << endl;
  ferr << "======== The block diagram of Linear Array Performances ========";
  Display(ferr,LAAPerformPattern,sz_LAAPerform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return lSuccess;
 }
 Line = Line.substr(rpos);
 //****************************************************************************
 //3. ����������� ���� ������ ��������� ������������ ��������� ��, �����������
 //   ������ �� ��������� ��������
 //****************************************************************************
 uint Index = 0; //������ ������������ ������ � ������� DistLawTable
 lSuccess = GetLawFromStr(DistLawTable, sz_DistLawTable, Index, Title,
                          LAAPerformPattern, sz_LAAPerform);
 if (lSuccess == false) return lSuccess;
 //��� ������ ������������ ��������� �� ������ �������
 RightLawType = DistLawTable[Index].ident;
 RightLaw = DistLawTable[Index].law;
 //****************************************************************************
 //4. ��������� ���������� ��� ������ ��������� ������������ ��������� ��,
 //   ����������� ������ �� ��������� ��������
 //****************************************************************************
 RightLawArg = GetLawParamFromFile(DistLawTable,Index,Title,LAAPerformPattern,
                                   sz_LAAPerform);
 if (RightLawArg == NULL) {lSuccess = false; return lSuccess;}
 //�������� RIGHTDISTLAW ������ ���� ������������� ���������
 if (RightLawType != TABLE_LAW)
  RightLawArg->List[0] = fabs(RightLawArg->List[0]);
 else //��� ���������� ������ ��� ������������� ���������� �������������
  for (ulong i = 0; i < RightLawArg->Size; i++)
   RightLawArg->List[i] = fabs(RightLawArg->List[i]);
return lSuccess;
}
//****************************************************************************
//������: III.24., protected, class member-function
//������������ �������: bool GetDisposLAA(bool& disp);
//����������:
//������ ������: <DISPOSITION>: <EQUATOR || MERIDIAN> <;>
//������� ������������� ��� ��������� �������� � ������������ �������� �� ����-
//�������� ������ �����������, ���������� � ������� �������� �� ������� ��������
//������ fin. ������� ������ �������� �������� ����� <DISPOSITION> � ����������
//������ �������� �� �������� ������ �� ������� ������������ �������� ���� �
//������� ��������.
//�������� �� ����� ������������� � ���� ���������� � �������������� ���������,
//�.�. ������������� ����������� ����� ��� � �������������� ���������, �.�. ���-
//�������� ����������� �����. � ������ ������, ������� ������ � ��������� �� ��-
//��� ������������� �� ���� ������������� ���� ��������� �������. �� ������ ���-
//��� - �� ���� ���� ����� ��������� �������, ������������ �������� ��������. 
//� ������ ������������� ������ ��������������� ��� �������������� ���������
//�������������� �� ����������� � ����� ferrname, ��������� � �������� �������
//������ ferr, � ������� ��������� ���� ������ � ���������� �������� false.
//���������� ���������� ������������ ��������:
// 1. string Buffer; 2. string Line; 3. CurRow
//-----------------------------------------------------------------------------
//������������ ���������:
//1. bool& disp - ������ �� ��������� ������������ ������������ �����������
//   ����� �������� �������
//-----------------------------------------------------------------------------
//������������ ��������: bool lSuccess
// true -  �������� DISPOSITION �� ����� fname ������� ����������
// false - �������� DISPOSITION �� ����� fname �� ������� ����������
//*****************************************************************************
bool TLinearArray::GetDisposLAA(bool& disp)
{bool lSuccess;
 string ABC(EngABC); //= EngABC + RusABC + Digits + ":;"; //���������� ����� ��������
 ABC.append(RusABC); ABC.append(Digits); ABC.append(":;");

 string Title = "DISPOSITION";
 KEYSTRPOS info;
 //******************************************************
 //1. ����� ��������� ����� <DISPOSITION>
 //******************************************************
 lSuccess = FindKeyStrInFile(Title, LAADispPattern, sz_LAADisp, Line,
            &info, ABC, CommentSep, sz_CommentSep, fin, ferr, LAAPerformPattern,
            sz_LAAPerform);
 if (lSuccess == false) return lSuccess;//�������� ����� �� �������
 //*****************************************************************
 //2. ����� ������� ����������� ����� <DISPOSITION> � �� ���������
 //*****************************************************************
 uint rpos = Line.find_first_of(":=",info.rpos);
 if (rpos == string::npos) //������ �������-����������� <:=> �� �������
 {lSuccess = false;
  ferr << "ERROR:  Linear Array DISPOSITION to receive it was not possible."
       << endl;
  ferr << "REASON: After <DISPOSITION> the token-separator should follow.\n";
  ferr << "The following tokens were expected: <:> or <=>." << endl;
  ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << endl;
  ferr << "======== The block diagram of Linear Array Performances ========";
  Display(ferr,LAAPerformPattern,sz_LAAPerform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return lSuccess;
 }
 info.rpos = rpos;
 //*******************************************************
 //3. ����������� �������� �������� <DISPOSITION>
 //*******************************************************
 Title = "Linear Array "+Title;
 lSuccess = StrToBool(disp, Line, &info, TbDispos, sz_TbDispos, Title,
            LAAPerformPattern, sz_LAAPerform);
return lSuccess;
}
//****************************************************************************
//������: III.25., protected, class member-function
//������������ �������:
// bool GetElemArrayPerformFromFile(TElArray& ElArray, bool status = SLAVE)
//����������:
//������� ������������� ��� ������������� ������������� ������������ ���������
//�������� �������� �������. � ����� ��������������� ��������� ���������:
//1. STATUS: <MASTER || SLAVE> - �������� ��� ��������������� ������� ��. � ��
//   ������ �������������� ���� �������� �������. ��� ������� ������������ ����-
//   ���� ������� ������ ������� ������� � ��������� ��. ���� status = MASTER,
//   �� ��������� ���������� ��������� ��������.
//2. USAGE: <ON || OFF> - ������������� �������� �� � ��������.
//3. RELAMPLFACTOR: <0 < value <= 1> - ������������� ����������� �������� ������-
//   �� ��. �� ���������� ������� ������ ������� ������������ ������ ������
//   ������� � ������������ ���. ������������, ���� �� - ������������, �.�. �
//   ���������� �� RelAmplFactor = n = 1 ��� ���� ��������� �������.
//4. ANTENNA: <antenna name> - ������������ ������� ������� �������� ��. �� ���-
//   ��������� ������� ������������ �� �������� - �������������� ��������������
//   � E- � H-���������� ����� ����������� ������� ������������.
//5. E_DIRECT: PARAMETERS: <arg1, arg2,...argN> - ��������� ������� ���������-
//   ����� � E-��������� (�� �������������).
//6. H_DIRECT: PARAMETERS: <arg1, arg2,...argN> - ��������� ������� ���������-
//   ����� � H-��������� (�� �������������).
//   ������� ������������� 5 � 6 ������� �� ���� �������, ������� ��������
//   ������ �������������� �������������� ������� � E- � H-����������. �������-
//   ������� �������������� � ���� ������� ����������� ���������, ��� �������
//   �������������� ������� ���������� �� ������ �������� ������������. � �����
//   ��������, ��������, ��������� ������������ ���� � ������������ ��������.
//7. E_LOCATION: <EQUATOR || MERIDIAN> - ������������ E-��������� ������� ����-
//   �������� ����������� �����.
//8. NOISEAMPL: <Law name;> PARAMETERS: <arg1,arg2,...,argN> - ����� ���������
//   ��������� ���� �������� ��.
//9. NOISEPHASE: <Law name;> PARAMETERS: <arg1,arg2,...,argN> - ����� ���������
//   ���� ���� �������� ��.
//� �������� ���������� ������������� ������������� �������������� � ����������-
//��� �������� ������������ ������. � ������ ������ ������� ��������� ���� ����-
//�� �������������� � ������������ ��������� ������ � ����������� �������������
//� ����� ferrname. � �������� ��������� ������ ������� ���������� ������� ���-
//����� ����� fin, ��������������� � ������ ����� fname.
//���������� ���������� ������������ ��������:
// 1. string Buffer; 2. string Line; 3. CurRow
//-----------------------------------------------------------------------------
//������������ ���������:
//1. TElArray& ElArray - ������ �� ������ ElArray. � ������ ������ ���������
//   �������������� �������� �� �������� � ������� �������� � �����.
//2. bool status - ��������� �� ������ ������������ �������� ��:
//   ��������� (MASTER) ��� ���������������� (SLAVE).
//-----------------------------------------------------------------------------
//������������ ��������: bool lSuccess
// true -  �������� �������� �� �� ����� fname ������� �������
// false - �������� �������� �� �� ����� fname ������� �� �������
//*****************************************************************************
bool TLinearArray::GetElemArrayPerformFromFile(TElArray& ElArray, bool status)
{bool lSuccess;
 string ABC; //���������� ����� ��������
 string title;
 KEYSTRPOS info;
 //******************************************************
 //1. ����� ������-��������� ������������� �������� ��
 //******************************************************
 //ABC = EngABC + RusABC + Digits;
 ABC.append(EngABC); ABC.append(RusABC); ABC.append(Digits);
 
 if (status)
 {title = "MASTER ELEMENT of LAA";
  lSuccess = FindKeyStrInFile(title, MasterHeader, sz_MasterHeader, Line,
             &info, ABC, CommentSep, sz_CommentSep, fin, ferr, AntPerform,
             sz_AntPerform);
 }
 else
 {title = "SLAVE ELEMENT of LAA";
  lSuccess = FindKeyStrInFile(title, SlaveHeader, sz_SlaveHeader, Line, &info,
             ABC, CommentSep, sz_CommentSep, fin, ferr, AntPerform,
             sz_AntPerform);
 }
 if (lSuccess == false) return lSuccess;
 //************************************************************************
 //1. ����������� ������� �������� �� �� ��������� ������ fin
 //   STATUS: MASTER || SLAVE
 //************************************************************************
 if (status == MASTER) ElArray.SetStatus(MASTER);
 else ElArray.SetStatus(SLAVE);
 //**************************************************************************
 //2. ����������� ������������� �������� �� ��� �������� �� ��������� ������
 //   fin : USAGE: ON || OFF
 //**************************************************************************
 lSuccess = GetElArrayUsage(ElArray);
 if (lSuccess == false) return lSuccess;
 //*****************************************************************************
 //3. ����������� �������������� ������������ �������� �������� �� �� ���������
 //   ������ fin : AMPLRELFACTOR: <value>,  0 < value <= 1
 //*****************************************************************************
 lSuccess = GetElArrayARF(ElArray);
 if (lSuccess == false) return lSuccess;
 //������, ���������� �� ����� �������� �������-�� �����-�� �������� �������
 if ((ElArray.GetN() <= 0.0) || (ElArray.GetN() > 1.0))
 {lSuccess = false;
  ferr << "ERROR: Property <AMPLRELFACTOR> of Element LAA to receive it was not"
       << " possible."  << endl;
  ferr << "REASON: The value of a antenna relative amplification factor\n"
       << "        to overstep the limits of admissible value." << endl;
  ferr << "Admissible value of factor: 0 < N <= 1." << endl;
  ferr << "Obtained value of factor: " << ElArray.GetN() << "." << endl;
  ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << endl;
  ferr << "==== The block diagram of Linear Array Element Performances ====";
  Display(ferr,AntPerform,sz_AntPerform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return lSuccess;
 }
 //************************************************************************
 //4. ����������� ���� ������� �� �� ������������ �� ��������� ������ fin
 //   �� ���� ������� ������� ���������� ��������� �������������� ������� �
 //   ��������� � ������������� ����������, � ����� ��������� ������� ����
 //   �������� ��������������, ���� ��� ����������
 //************************************************************************
 lSuccess = GetAntennaType(ElArray);
 if (lSuccess == false) return lSuccess;
 //*****************************************************************************
 //5. ����������� ������������ �������� �� ������������ ����������� ����� ��
 //   ��������� ������ fin : E_LOCATION: EQUATOR || MERIDIAN
 //*****************************************************************************
 lSuccess = GetAntELocate(ElArray);
 if (lSuccess == false) return lSuccess;
 //*****************************************************************************
 //6. ����������� ������� � �� ���������� ��������� ��������� � ���� ���� �� ��-
 //   ���� ��������� ������ �� ��������� ������ fin
 //*****************************************************************************
 lSuccess = GetAntNoiseLaw(ElArray);
return lSuccess;
}
//****************************************************************************
//������: III.29., protected, class member-function
//������������ �������: bool GetAntennaType(TElArray& ElArray)
//����������:
//������ ������: <ANTENNA>: <Antenna name;>
//<Antenna name> - ������������ �������;
//������� ������������� ��� ��������� �������� � ������������ �������, �������
//������ � ������ �������� �������� �������. � ������ ������� ������������ �����
//��������� �����  <ANTENNA>, ����� ����� ��������� ������ ��������� ����������
//��� �������. ���������� �� ������ ������ � �� ��������������� �������� � ���-
//���� AntTable. ����� ��������� ������ ������������ ������������ ������� � ����-
//�� ����� ���������� ������ �� AntTable � ������ �������� �� ElArray �� ����
//������� ��������� �������� � ���� �������, ������� �������������� ������� �
//E- � H-����������, �� ���������������, � ����� ����������� ��� ������ �������-
//��� ������� ���� ARGLIST.
//������ ������ �� �������� ����� ����������� ���������� ������ �������� ����.
//� ������ ������������� ������ ��������������� ��� �������������� ���������
//�������������� �� ����������� � ����� ferrname, ��������� � �������� �������
//������ ferr, � ������� ��������� ���� ������ � ���������� �������� false.
//���������� ���������� ������������ ��������:
// 1. string Buffer; 2. string Line; 3. CurRow; 4. AntTable 5. AntPerform
//-----------------------------------------------------------------------------
//������������ ���������:
//1. TElArray& ElArray - ������ �� ������ ������ TElArray, � ������� ����� ����-
//   ������ ���������� �� �������� ��������� �������� ��, ����������� �� �����
//2. uint& index - ������ �� ����������, ������� ����� ��������� ������ �������-
//   ��� ������� � ������� AntTable, � ������ �������� ������.
//-----------------------------------------------------------------------------
//������������ ��������: bool lSuccess
// true -  ��� ������� �� ����� fname ������� ����������
// false - ��� ������� �� ����� fname �� ������� ����������
//*****************************************************************************
bool TLinearArray::GetAntennaType(TElArray& ElArray)
{bool lSuccess;
 string ABC(EngABC); //= EngABC + RusABC + Digits + ":;"; //���������� ����� ��������
 ABC.append(RusABC); ABC.append(Digits); ABC.append(":;");

 string Title = "ANTENNA";
 KEYSTRPOS info;
 //******************************************************
 //1. ����� ��������� ����� <ANTENNA>
 //******************************************************
 lSuccess = FindKeyStrInFile(Title, AntennaPattern, sz_Antenna, Line,
            &info, ABC, CommentSep, sz_CommentSep, fin, ferr, AntPerform,
            sz_AntPerform);
 if (lSuccess == false) return lSuccess;//�������� ����� �� �������
 //********************************************************************
 //2. ����� ������� ����������� ����� <ANTENNA> � �� ���������
 //********************************************************************
 uint rpos = Line.find_first_of(":",info.rpos);
 if (rpos == string::npos) //������ �������-����������� <:=> �� �������
 {lSuccess = false;
  ferr << "ERROR: Property <ANTENNA> of Element LAA to receive it was not possible."
       << endl;
  ferr << "REASON: After <ANTENNA> the token-separator should follow.\n";
  ferr << "The following token were expected: <:>." << endl;
  ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << endl;
  ferr << "==== The block diagram of Linear Array Element Performances ====";
  Display(ferr,AntPerform,sz_AntPerform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return lSuccess;
 }
 Line = Line.substr(rpos);
 //****************************************************************************
 //3. ����������� ���� ������� �������� �� �� �� ������������ �� ������ �����
 //****************************************************************************

 uint Index = 0; //������ ������������ ������� � ������� AntTable
 lSuccess = GetAntNameFromStr(AntTable, sz_AntTable, Index, Title);
 //��� ������� �������� �� ���������� �� �������
 if (lSuccess == false) return lSuccess;

 //****************************************************************************
 //4. ������������ ������ ���������� ��� ��������� �������������� ������� �
 //   E-���������
 //****************************************************************************
 ARGLIST* EArg = GetDirectParamFromFile(Index,E_PLANE);
 if (EArg == NULL) {lSuccess = false; return lSuccess;}

 //****************************************************************************
 //5. ������������ ������ ���������� ��� ��������� �������������� ������� �
 //   H-���������
 //****************************************************************************
 ARGLIST* HArg = GetDirectParamFromFile(Index,H_PLANE);
 if (HArg == NULL) {lSuccess = false; return lSuccess;}

 //������������� ��� �������, ������� �������� �������������� ������� �
 //E- � �-���������� � �� ���������
 ElArray.SetPerformance(AntTable[Index].type,MERIDIAN,EArg,HArg);

return lSuccess;
}
//****************************************************************************
//������: III.30., protected, class member-function
//������������ �������:
// bool GetAntNameFromStr(const ANTENNA_INFO* Table, const uint sz_Table,
//      uint& index, const string& title);
//����������:
//������ ������: <Antenna name;>
//<Antenna name> = <str1> || <str2> || ... <strN>
//������� ������������� ��� ������ �������� ������ - ������������ �������, ����-
//��� �������� � ������� Table � ������ Line � � ������ �������� ������ �������-
//��� ���������� index ������� ������, � ������� ���������� ���������� �� ������-
//���� ������������ ������. ������ ������ �������� ����.
//� ������ ������, ������� ���������� �������� false � ���������� �� ������ ��-
//������������ � �������� ������ ������ ferr. ������� ��������� �������������
//�������� ������ � ������ Line, � ����� ��������� �������������� ������, ���-
//������ � ������������ �� ������� ������ ������������ ����. � ����� �������
//��������� ������� ������� ';', ������� � ������ ������ ��������, ����� �����-
//��� � ���������.
//���������� ���������� ������������ ��������:
//1. string Buffer; 2. string Line; 3. CurRow; 4. AntPerform; 
//-----------------------------------------------------------------------------
//������������ ���������:
//1. const ANTENNA_INFO* Table - ��������� �� ������� � ����������� � ���������
//   ������������� �������, ������� ����� ��������� ������ Line
//2. const uint sz_Table - ������ ������� Table
//3. uint& index - ������ �� ����������, � ������� ����� ��������� ������
//   ������ � ������� Table, � ������ �������� ������ ��� � ������ Line
//4. const string& title - ������ �� ������ � ������������� ������� ��������
//   ������ � �������� ������ fin
//-----------------------------------------------------------------------------
//������������ ��������: bool lSuccess
// true -  � ������ Line ������������ ������� �������
// false - � ������ Line ����������� ������������ ������� ����� �� �������
//*****************************************************************************
bool TLinearArray::GetAntNameFromStr(const ANTENNA_INFO* Table,
     const uint sz_Table, uint& index, const string& title)
{bool lSuccess;
 uint rpos;
 uint lpos;
 //����� �������-����������� ';' ����� ��������� ��������, ������� ��������
 //�����������, � ������ Line
 rpos = Line.find_first_of(';');
 if (rpos == string::npos) //������
 {lSuccess = false;
  ferr << "ERROR:  " << title << " to receive it was not possible." << endl;
  ferr << "REASON: Complete token <;> was expected for this data string" << endl;
  ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << endl;
  ferr << "============== The block diagram of Performances ===============";
  Display(ferr,AntPerform,sz_AntPerform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return lSuccess;
 }
 //��������� �� line ��������� s1 � ����������� �������� ';'
 string s1 = Line.substr(1,rpos+1);
 //s1.set_case_sensitive(0); //��������� ����������-�������������� �����
 //����� ����������� ������������ ������ �� ������� ������� Table � ������ s1
 KEYSTRPOS keystr_info;
 for (uint i = 0; i < sz_Table; i++)
 {lSuccess=SearchKeyString(s1,Table[i].name,Table[i].sz_name,&keystr_info);
  if (lSuccess) {index = i; break;}
 }
 if (lSuccess == false) //������: ������������ ������ �� �������
 {ferr << "ERROR:  " << title << " to receive it was not possible." << endl;
  ferr << "REASON: Law name to find it was not possible." << endl;
  ferr << "Line N " << CurRow << ": <" << s1 << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">";
  s1 = "The following ANTENNA NAMEs were expected:";
  string s2;
  char str[20];
  for (uint i = 0; i < sz_Table; i++)
  {itoa(i+1,str,10); s2 = str; s2 =  s2 + ". " + s1;
   Display(ferr, Table[i].name, Table[i].sz_name, s2.c_str());
  }
  ferr << endl;
  ferr << "============== The block diagram of Performances ===============";
  Display(ferr,AntPerform,sz_AntPerform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return lSuccess;
 }
//����� ����������� �������� ����� � ������ �� ������������ ������. �����
//������� ������� � ��������� ������ �������� ���� �� ������.
//����� ����������� �������� ����� �� ������������ ������
 string s2 = s1.substr(0,keystr_info.lpos);
 lpos = s2.find_first_not_of(" \t");
  if (lpos != string::npos) //������: ����������� ������� �������
  {s2 = "To the left of "; lSuccess = false;}
//����� ����������� �������� ������ �� ������������ ������ �� ';'
 if (lSuccess != false)
 { s2 = s1.substr(keystr_info.rpos+1);
   lpos = s2.find_first_not_of(" \t;");
   if (lpos != string::npos) //������: ����������� ������� �������
   {s2 = "To the right of "; lSuccess = false;}
 }
 if (lSuccess == false) //��������� ��������� �� ������
 {ferr << "ERROR:  " << title << " to receive it was not possible." << endl;
  ferr << "REASON: " << s2 << "law name a PROHIBITED TOKENS are";
  ferr << " detected." << endl;
  ferr << "Line N " << CurRow << ": <" << s1 << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << "The following allowed tokens were expected: <[SPACE],[TAB]>." << endl;
  ferr << endl;
  ferr << "============== The block diagram of Performances ===============";
  Display(ferr,AntPerform,sz_AntPerform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return lSuccess;
 }
 //������������ ������ ����������
 if (rpos != Line.length()-1) Line = Line.substr(rpos+1);
 else Line = "";
return lSuccess;
}
//****************************************************************************
//������: III.31., protected, class member-function
//������������ �������:
// ARGLIST* GetDirectParamFromFile(const uint index, bool plane);
//����������:
//������ ������: <KEY STRING: ARG1; ARG2; ... ARGn;>
//<KEY STRING> = <E_DIRECT> || <H_DIRECT>
//<ARGn> = [IDENT <=>] <value>; IDENT - ������������� ��������� (������������)
//<IDENT> = LETTER+[LETTER||DIGIT||OTHER]+[LETTER||DIGIT||OTHER]+ ...]
//<OTHER> = <[]-_> for example: lv[-1_0]
//<value> - �������� ��������
//������� ������������� ��� �������� ������ ���������� ��������� ��������������
//������� � E- ��� H-���������, ��� ������������� �������� ������ �� ���������
//������ fin.
//���������� �� ���������� ����������� ���������� � ������� ������ ARGLIST arg,
//� ������� ��� ����� �������������, ���������� � ������� AntTable. ������
//index ��������� �� ����� � AntTable �� �������� ��������� ���������� � �������
//������������ ������ ���������� ARGLIST � ���������� ����������, ������� ����-
//������ ������� �� ����� ��� ���������� ������ �������, �������� ���������
//�������������� ������� ��������� ����. ���������� plane ���������� ��� �����
//�� ���������� ����� ����������� ������ ����������. ��������� ����� �������-
//������ �� ���������� ������� �����. ������ ������ �������� ����. ���������,
//������� �� ����������� �� ������ ������ ������� ��������� �� ��������� �������
//��� �������� ��������� �����. ��������� �������� �������������� � ����� ������
//�������� ������������� �������. ����� ������ ��������� ��������� ����� ����
//������ ��� ������������� � ����������� ���������� ������������ ���� <=>.
//� ������ ������, ������� ���������� �������� LawArg == NULL � ���������� ��
//������ �������������� � �������� ������ ������ ferr. ������� ��������� ������-
//������� �������� ������ � ������ Line, � ����� ��������� �������������� ������,
//��������� � ������������ �� ������� ������ ������������ ����. � ����� �������
//��������� ������� ������� ';', ������� � ������ ������ ��������, ����� �����-
//��� � ���������.
//���������� ���������� ������������ ��������:
//1. string Buffer; 2. string Line; 3. CurRow; 4. AntTable; 5. EDirectPattern;
//6. HDirectPattern; 7. AntPerform
//-----------------------------------------------------------------------------
//������������ ���������:
//1. const uint index - ������ ������� � ������� AntTable, ��� ������� �����
//   ������ ������ ���������� ������ �������������� ������� � E- ��� H-���������
//2. bool plane - ���������� �������� ������ ���������� ��� E- ��� H-���������.
//-----------------------------------------------------------------------------
//������������ ��������: ARGLIST* LawArg - ������ ��������� �� ����� ����������
//������ ��������� �������������� �������  ��� ������������� ��� ��������� ����-
//����� �� ������� AntTable ������������� � ��� ��� �������� index
//� ������ ���������� ���������� ���������� LawArg == NULL
//*****************************************************************************
ARGLIST* TLinearArray::GetDirectParamFromFile(const uint index, bool plane)
{ARGLIST* LawArg = NULL;
 string title;
 uint total_arg;
 uint read_arg;
 if (plane == E_PLANE)
 {title = "E_DIRECT PARAMETERS for Antenna";
  total_arg = AntTable[index].E_total;
  read_arg = AntTable[index].E_read;
 }
 else
 {title = "H_DIRECT PARAMETERS for Antenna";
  total_arg = AntTable[index].H_total;
  read_arg = AntTable[index].H_read;
 }
 //�������� ������������ ����� ����� ������ ���������� � ����������� �����������
 //����������
 if ((total_arg == 0) || (total_arg <= read_arg))
 //������: �������������� ����� ��������� ���������� ����������� ������
 {ferr << "ERROR:  " << title << " to receive it was not possible." << endl;
  ferr << "REASON: " << "Detect mismatch between total parameters and amount of\n";
  ferr << "        reading parameters." << endl;
  ferr << "Antenna name: <" << AntTable[index].name[1] << ">;" << endl;
  ferr << "Total_Arg = " << total_arg << endl;
  ferr << "Read_Arg  = " << read_arg << endl;
  ferr << "TIPS: Total_Arg > Read_Arg && Total_Arg > 0" << endl;
  ferr << endl;
  ferr << "============== The block diagram of Performances ===============";
  Display(ferr,AntPerform,sz_AntPerform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return LawArg;
 }
 //*************************************************************************
 //1. �������� ������ ARGLIST, � ������� ����� ���������� ��������� ������
 //*************************************************************************
 LawArg = CreateArgList(total_arg);
 //*************************************************************************
 //2. �������� �� ������������� ���������� ���������� �� �����.
 //*************************************************************************
 if (read_arg == 0)  return LawArg;
 //******************************************************
 //3. ����� ��������� ����� <KEYSTRING>
 //******************************************************
 bool lSuccess;
 string ABC(EngABC); //= EngABC + RusABC + Digits + ":;"; //���������� ����� ��������
 ABC.append(RusABC); ABC.append(Digits); ABC.append(":;");

 KEYSTRPOS info;

 if (plane == E_PLANE)
  lSuccess = FindKeyStrInFile(title, EDirectPattern, sz_EDirect, Line,
             &info, ABC, CommentSep, sz_CommentSep, fin, ferr, AntPerform,
             sz_AntPerform);
 else
  lSuccess = FindKeyStrInFile(title, HDirectPattern, sz_HDirect, Line,
             &info, ABC, CommentSep, sz_CommentSep, fin, ferr, AntPerform,
             sz_AntPerform);
 //�������� ����� �� �������
 if (lSuccess == false) {LawArg = DeleteArgList(LawArg); return LawArg;}

 //********************************************************************
 //2. ����� ������� ����������� ����� <KEYSTRING> � �����������
 //********************************************************************
 uint rpos = Line.find_first_of(":",info.rpos);
 if (rpos == string::npos) //������ �������-����������� <:=> �� �������
 {LawArg = DeleteArgList(LawArg);
  ferr << "ERROR:  " << title << " to receive it was not possible." << endl;
  ferr << "REASON: After <KEYSTRING> the token-separator should follow.\n";
  ferr << "The following token were expected: <:>." << endl;
  ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << endl;
  ferr << "==== The block diagram of Linear Array Element Performances ====";
  Display(ferr,AntPerform,sz_AntPerform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return LawArg;
 }
 //�������� ����� <KEYSTRING> ����������
 if (rpos != Line.length()-1) Line = Line.substr(rpos+1);
 else Line = "";
//***************************************************************************
//5. ���������������� ���������� ���������� ������� ������ �� ����� �
//   ��������� �� � ������ LawArg
//***************************************************************************
 ulong ReadArg = 0L;  //���������� ��������� ����������
 double Value = 0.0;
 //���� ���������� ���������� ������
 for (ulong i = 0; i < read_arg; i++)
 {lSuccess = GetDoubleFromFile(Value,total_arg,ReadArg,title,AntPerform,
             sz_AntPerform);
  //������ ��� ���������� ������
  if (lSuccess == false) {LawArg = DeleteArgList(LawArg); break;}
  //��������� �������� ������ �� ��������� ������ �������
  ReadArg++;
  LawArg->List[i+1] = Value;
 }
 if (LawArg == NULL) //������ ��� ���������� ���������� �� �����
 {ferr << "****************************************************************\n";
  ferr << "The following parameters were expected:";
  if (plane == E_PLANE)
   Display(ferr,AntTable[index].EParam,AntTable[index].sz_EParam);
  else
   Display(ferr,AntTable[index].HParam,AntTable[index].sz_HParam);
  ferr << "****************************************************************\n";
 }
return LawArg;
}
//****************************************************************************
//������: III.32., protected, class member-function
//������������ �������: bool GetElArrayUsage(TElArray& ElArray)
//����������:
//������ ������: <USAGE>: <ON || OFF> <;>
//������� ������������� ��� ��������� �������� �� ������������� �������� �����-
//��� �� ��� ��������, ���������� � ������� �������� �� ������� �������� ������
//fin. ������� ������ �������� �������� ����� <USAGE> � ���������� ������
//�������� �� �������� ������ �� ������� ������������ �������� ���� � �������
//��������.
//� ������ ������������� ������ ��������������� ��� �������������� ���������
//�������������� �� ����������� � ����� ferrname, ��������� � �������� �������
//������ ferr, � ������� ��������� ���� ������ � ���������� �������� false.
//���������� ���������� ������������ ��������:
// 1. string Buffer; 2. string Line; 3. CurRow; 4. AntPerform; 5. TbUsage;
// 6. LAAElemUsage
//-----------------------------------------------------------------------------
//������������ ���������:
//1. TElArray& ElArray - ������ �� ������ �������� �������� �������
//-----------------------------------------------------------------------------
//������������ ��������: bool lSuccess
// true -  �������� USAGE �� ����� fname ������� ����������
// false - �������� USAGE �� ����� fname �� ������� ����������
//*****************************************************************************
bool TLinearArray::GetElArrayUsage(TElArray& ElArray)
{bool lSuccess;
 string ABC(EngABC); //= EngABC + RusABC + Digits + ":;"; //���������� ����� ��������
 ABC.append(RusABC); ABC.append(Digits); ABC.append(":;");

 string Title = "USAGE";
 KEYSTRPOS info;
 //******************************************************
 //1. ����� ��������� ����� <USAGE>
 //******************************************************
 lSuccess = FindKeyStrInFile(Title, LAAElemUsage, sz_LAAElemUsage, Line,
            &info, ABC, CommentSep, sz_CommentSep, fin, ferr, AntPerform,
            sz_AntPerform);
 if (lSuccess == false) return lSuccess;//�������� ����� �� �������
 //*****************************************************************
 //2. ����� ������� ����������� ����� <USAGE> � �� ���������
 //*****************************************************************
 uint rpos = Line.find_first_of(":=",info.rpos);
 if (rpos == string::npos) //������ �������-����������� <:=> �� �������
 {lSuccess = false;
  ferr << "ERROR:  USAGE of LAA Element to receive it was not possible." << endl;
  ferr << "REASON: After <USAGE> the token-separator should follow.\n";
  ferr << "The following tokens were expected: <:> or <=>." << endl;
  ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << endl;
  ferr << "=========  The block diagram of LAA Element Performances  =========";
  Display(ferr,AntPerform,sz_AntPerform,"","","",false);
  ferr << "===================================================================";
  ferr << endl;
  return lSuccess;
 }
 info.rpos = rpos;
 //*******************************************************
 //3. ����������� �������� �������� <USAGE>
 //*******************************************************
 bool usage = OFF;
 Title = Title + " of Antenna Array Element";
 lSuccess = StrToBool(usage, Line, &info, TbUsage, sz_TbUsage, Title,
            AntPerform, sz_AntPerform);
 if (lSuccess) ElArray.SetUsage(usage);
return lSuccess;
}
//****************************************************************************
//������: III.33., protected, class member-function
//������������ �������: bool GetElArrayARF(TElArray& ElArray)
//����������:
//������ ������: <AMPLRELFACTOR>: [<Ident> <=> ] <value>;  0 < value <= 1
//������� ������������� ��� ��������� �������� �� ������������� ������������ ���-
//����� �������� ��, ���������� � ������� �������� �� ������� �������� ������
//fin. ������� ������ �������� �������� ����� <AMPLRELFACTOR> � ���������� ���-
//��� �������� ������������, ������� ������ ���� ������������� ������������ ���-
//��� �� ������������� 1. ������ ������ �� �������� ����� ����������� ����������
//������ �������������� �������� �������� ������� �������� ����.
//� ������ ������������� ������ ��������������� ��� �������������� ���������
//�������������� �� ����������� � ����� ferrname, ��������� � �������� �������
//������ ferr, � ������� ��������� ���� ������ � ���������� �������� false.
//���������� ���������� ������������ ��������:
// 1. string Buffer; 2. string Line; 3. CurRow; 4. AntPerform; 5. LAAElemARF
//-----------------------------------------------------------------------------
//������������ ���������:
//1. TElArray& ElArray - ������ �� ������ �������� �������� �������
//-----------------------------------------------------------------------------
//������������ ��������: bool lSuccess
//true -  �������� �������� �� AMPLRELFACTOR �� ����� fname ������� ����������
//false - �������� �������� �� AMPLRELFACTOR �� ����� fname �� ������� ����������
//*****************************************************************************
bool TLinearArray::GetElArrayARF(TElArray& ElArray)
{bool lSuccess;
 string ABC(EngABC); //= EngABC + RusABC + Digits + ":;"; //���������� ����� ��������
 ABC.append(RusABC); ABC.append(Digits); ABC.append(":;");

 string Title = "AMPLRELFACTOR";
 KEYSTRPOS info;
 //******************************************************
 //1. ����� ��������� ����� <AMPLRELFACTOR>
 //******************************************************
 lSuccess = FindKeyStrInFile(Title, LAAElemARF, sz_LAAElemARF, Line,
            &info, ABC, CommentSep, sz_CommentSep, fin, ferr, AntPerform,
            sz_AntPerform);
 if (lSuccess == false) return lSuccess;//�������� ����� �� �������
 //*****************************************************************
 //2. ����� ������� ����������� ����� <AMPLRELFACTOR> � �� ���������
 //*****************************************************************
 uint rpos = Line.find_first_of(":=",info.rpos);
 if (rpos == string::npos) //������ �������-����������� <:=> �� �������
 {lSuccess = false;
  ferr << "ERROR:  AMPLRELFACTOR of Element LAA  to receive it was not possible."
       << endl;
  ferr << "REASON: After <AMPLRELFACTOR> the token-separator should follow.\n";
  ferr << "The following tokens were expected: <:> or <=>." << endl;
  ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << endl;
  ferr << "==== The block diagram of Antenna Array Element Performances ====";
  Display(ferr,AntPerform,sz_AntPerform,"","","",false);
  ferr << "=================================================================";
  ferr << endl;
  return lSuccess;
 }
 info.rpos = rpos;
 //**************************************************************************
 //3. ����������� �������� ������������ �������������� �������� �������� ��
 //**************************************************************************
 if (rpos == string::npos) //������! � ������ Line ����������� ���������� �� LV
 {lSuccess = false;
  ferr << "ERROR:  AMPLRELFACTOR of LAA Element to receive it was not possible."
       << endl;
  ferr << "REASON: There is no information about a amplify relative factor\n";
  ferr << "        the element of Antenna Array." << endl;
  ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << endl;
  ferr << "========  The block diagram of LAA Element Performances  ========";
  Display(ferr,AntPerform,sz_AntPerform,"","","",false);
  ferr << "=================================================================";
  ferr << endl;
  return lSuccess;
 }
  //���������� ������������� ���������� ����� ���������� �������������� ��
  Line = Line.substr(rpos+1);
  double factor = 0.0;
  lSuccess = StrToDouble(factor, Line, NameARF, sz_NameARF, Title, AntPerform,
             sz_AntPerform);
  if (lSuccess == false) return lSuccess;
  ElArray.SetN(factor);
return lSuccess;
}
//****************************************************************************
//������: III.34., protected, class member-function
//������������ �������: bool GetAntELocate(TElArray& ElArray);
//����������:
//������ ������: <E_LOCATION>: <EQUATOR || MERIDIAN> <;>
//������� ������������� ��� ��������� �������� � ������������ ������� ��������
//�� ������������ ������ �����������, ���������� � ������� �������� �� �������
//�������� ������ fin. ������� ������ �������� �������� ����� <E_LOCATION> �
//���������� ������ �������� �� �������� ������ �� ������� ������������ �������� ���� �
//������� ��������.
//������� �� ����� ������������� � ���� ���������� � �������������� ���������,
//�.�. ������������� ����������� ����� ��� � �������������� ���������, �.�. ���-
//�������� ����������� �����.
//� ������ ������������� ������ ��������������� ��� �������������� ���������
//�������������� �� ����������� � ����� ferrname, ��������� � �������� �������
//������ ferr, � ������� ��������� ���� ������ � ���������� �������� false.
//���������� ���������� ������������ ��������:
// 1. string Buffer; 2. string Line; 3. CurRow; 4. AntPerform;
// 5. LAAElemELocation
//-----------------------------------------------------------------------------
//������������ ���������:
//1. TElArray& ElArray - ������ �� ������ �������� �������� �������
//-----------------------------------------------------------------------------
//������������ ��������: bool lSuccess
// true -  �������� E_LOCATION �� ����� fname ������� ����������
// false - �������� E_LOCATION �� ����� fname �� ������� ����������
//*****************************************************************************
bool TLinearArray::GetAntELocate(TElArray& ElArray)
{bool lSuccess;
 string ABC(EngABC); //= EngABC + RusABC + Digits + ":;"; //���������� ����� ��������
 ABC.append(RusABC); ABC.append(Digits); ABC.append(":;");

 string Title = "E_LOCATION";
 KEYSTRPOS info;
 //******************************************************
 //1. ����� ��������� ����� <E_LOCATION>
 //******************************************************
 lSuccess = FindKeyStrInFile(Title, LAAElemELocation, sz_LAAElemELocation, Line,
            &info, ABC, CommentSep, sz_CommentSep, fin, ferr, AntPerform,
            sz_AntPerform);
 if (lSuccess == false) return lSuccess;//�������� ����� �� �������
 //*****************************************************************
 //2. ����� ������� ����������� ����� <E_LOCATION> � �� ���������
 //*****************************************************************
 uint rpos = Line.find_first_of(":=",info.rpos);
 if (rpos == string::npos) //������ �������-����������� <:=> �� �������
 {lSuccess = false;
  ferr << "ERROR:  E_LOCATION of LAA Element to receive it was not possible."
       << endl;
  ferr << "REASON: After <E_LOCATION> the token-separator should follow.\n";
  ferr << "The following tokens were expected: <:> or <=>." << endl;
  ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << endl;
  ferr << "========  The block diagram of LAA Element Performances  ========";
  Display(ferr,AntPerform,sz_AntPerform,"","","",false);
  ferr << "=================================================================";
  ferr << endl;
  return lSuccess;
 }
 info.rpos = rpos;
 //*******************************************************
 //3. ����������� �������� �������� <E_LOCATION>
 //*******************************************************
 bool disp = MERIDIAN;
 Title = Title +" LAA Element";
 lSuccess = StrToBool(disp, Line, &info, TbDispos, sz_TbDispos, Title,
            AntPerform, sz_AntPerform);
 if (lSuccess) ElArray.SetELocation(disp);
return lSuccess;
}
//****************************************************************************
//������: III.35., protected, class member-function
//������������ �������: bool GetAntNoiseLaw(TElArray& ElArray)
//����������:
//������ ������: <NOISEAMPL>: <Law name;> [<PARAMETERS: ARG1;ARG2;...ARGn;>]
//������ ������: <NOISEPHASE>: <Law name;> [<PARAMETERS: ARG1;ARG2;...ARGn;>]
//<Law name> - ������������ ������;
//<PARAMETERS:> - ������ ���������� ������;
//<ARGn> = [LETTER+[LETTER || DIGIT || OTHER] <=> ] double value <;>
//<LETTER> = EngABC + RusABC; DIGIT = Digits; OTHER = <[],->
//double value - ������������ ����� ���� double
//������� ������������� ��� ��������� �������� � �������, �� ������� ����������
//��������� � ���� ����������� ���� �� ������ ��������� ������, ���������� � ��-
//����� �������� �� ������� �������� ������ fin. ������� ������ �������� �����-
//��� ����� <NOISEAMPL> ��� <NOISEPHASE>, ���������� ��� ������ �� ��������
//������ �� ������� NoiseLawTable �, ��������� ���������� � ������ �� ���� ���-
//����, ������� ����������� ���������� ����������, ������� ������ ����� ������-
//����. ������ ������ �� �������� ����� ����������� ���������� ������� ��������
//����.
//� ������ ������������� ������ ��������������� ��� �������������� ���������
//�������������� �� ����������� � ����� ferrname, ��������� � �������� �������
//������ ferr, � ������� ��������� ���� ������ � ���������� �������� false.
//���������� ���������� ������������ ��������:
// 1. string Buffer; 2. string Line; 3. CurRow; 4. NoiseLawTable;
// 5. LAAElemNoiseAmpl; 6. LAAElemNoisePhase
//-----------------------------------------------------------------------------
//������������ ���������:
//1. TElArray& ElArray - ������ �� ������ �������� �������� �������
//-----------------------------------------------------------------------------
//������������ ��������: bool lSuccess
// true -  NOISEAMPL � NOISEPHASE �� ����� fname ������� ����������
// false - NOISEAMPL � NOISEPHASE �� ����� fname �� ������� ����������
//*****************************************************************************
bool TLinearArray::GetAntNoiseLaw(TElArray& ElArray)
{bool lSuccess;
 string ABC(EngABC); //= EngABC + RusABC + Digits + ":;"; //���������� ����� ��������
 ABC.append(RusABC); ABC.append(Digits); ABC.append(":;");

 string Title = "NOISEAMPL";
 KEYSTRPOS info;
 //******************************************************
 //1. ����� ��������� ����� <NOISEAMPL>
 //******************************************************
 lSuccess = FindKeyStrInFile(Title, LAAElemNoiseAmpl, sz_LAAElemNoiseAmpl, Line,
            &info, ABC, CommentSep, sz_CommentSep, fin, ferr, AntPerform,
            sz_AntPerform);
 if (lSuccess == false) return lSuccess;//�������� ����� �� �������
 //********************************************************************
 //2. ����� ������� ����������� ����� <NOISEAMPL> � �� ���������
 //********************************************************************
 uint rpos = Line.find_first_of(":",info.rpos);
 if (rpos == string::npos) //������ �������-����������� <:=> �� �������
 {lSuccess = false;
  ferr << "ERROR:  <NOISEAMPL> of LAA Element to receive it was not possible."
       << endl;
  ferr << "REASON: After <NOISEAMPL> the token-separator should follow.\n";
  ferr << "The following tokens were expected: <:>." << endl;
  ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << endl;
  ferr << "========  The block diagram of LAA Element Performances  ========";
  Display(ferr,AntPerform,sz_AntPerform,"","","",false);
  ferr << "=================================================================";
  ferr << endl;
  return lSuccess;
 }
 Line = Line.substr(rpos);
 //****************************************************************************
 //3. ����������� ���� ������ ��������� ��������� ����������� ����
 //****************************************************************************
 uint Index = 0; //������ ������������ ������ � ������� NoiseLawTable
 lSuccess = GetLawFromStr(NoiseLawTable, sz_NoiseLawTable, Index, Title,
                          AntPerform, sz_AntPerform);
 if (lSuccess == false) return lSuccess;
 //��� ������ ��������� ����
 uint AmplType = NoiseLawTable[Index].ident;
 //****************************************************************************
 //4. ��������� ���������� ��� ������ ��������� ��������� ����
 //****************************************************************************
 ARGLIST* AmplArg = GetLawParamFromFile(NoiseLawTable,Index,Title,AntPerform,
                                   sz_AntPerform);
 if (AmplArg == NULL) {lSuccess = false; return lSuccess;}

 //******************************************************
 //1. ����� ��������� ����� <NOISEPHASE>
 //******************************************************
 Title = "NOISEPHASE";
 lSuccess = FindKeyStrInFile(Title,LAAElemNoisePhase,sz_LAAElemNoisePhase,Line,
            &info, ABC, CommentSep, sz_CommentSep, fin, ferr, AntPerform,
            sz_AntPerform);
 if (lSuccess == false) return lSuccess;//�������� ����� �� �������
 //********************************************************************
 //2. ����� ������� ����������� ����� <NOISEPHASE> � �� ���������
 //********************************************************************
 rpos = Line.find_first_of(":",info.rpos);
 if (rpos == string::npos) //������ �������-����������� <:=> �� �������
 {lSuccess = false;
  ferr << "ERROR:  <NOISEPHASE> of LAA Element to receive it was not possible."
       << endl;
  ferr << "REASON: After <NOISEPHASE> the token-separator should follow.\n";
  ferr << "The following tokens were expected: <:>." << endl;
  ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << endl;
  ferr << "========  The block diagram of LAA Element Performances  ========";
  Display(ferr,AntPerform,sz_AntPerform,"","","",false);
  ferr << "=================================================================";
  ferr << endl;
  return lSuccess;
 }
 Line = Line.substr(rpos);
 //****************************************************************************
 //3. ����������� ���� ������ ��������� ���� ����������� ����
 //****************************************************************************
 Index = 0; //������ ������������ ������ � ������� NoiseLawTable
 lSuccess = GetLawFromStr(NoiseLawTable, sz_NoiseLawTable, Index, Title,
                          AntPerform, sz_AntPerform);
 if (lSuccess == false) return lSuccess;
 //��� ������ ���� ����
 uint PhaseType = NoiseLawTable[Index].ident;
 //****************************************************************************
 //4. ��������� ���������� ��� ������ ��������� ���� ����
 //****************************************************************************
 ARGLIST* PhaseArg = GetLawParamFromFile(NoiseLawTable,Index,Title,AntPerform,
                                         sz_AntPerform);
 if (PhaseArg == NULL) {lSuccess = false; return lSuccess;}
 //������ � �� ��������� ���������� �������
 ElArray.SetNoisePerform(AmplType, PhaseType, AmplArg, PhaseArg);

return lSuccess;
}

//----------------------- The end of Linarray.cpp -----------------------------
