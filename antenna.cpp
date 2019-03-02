//antenna.cpp
//���������� ������ ������� -- Antenna
//Initial date: September,15,1998. Final date: September,16,1998.
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

#include "antenna.h"
//�������������� ������ �� ������������� � ��������������� �������������� ������
const ANT_INFO AntennInfo[] = {
 {UNEXIST,UNEXIST,UNEXIST,"������� �� �������","Antenna not found","UNEXIST" },
 {HW_DIPOLE,FE_HWDIPOLE,FH_HWDIPOLE,"������������ ��������",       //1
  "Half-wave dipole", "HW_DIPOLE" },
 {HWSLOT_ANT, FH_HWDIPOLE, FE_HWDIPOLE, "������������ ����",       //2
  "Half-wave slot antenna", "HWSLOT_ANT" },
 {PARAB_ANT, FEH_PARABANT, FEH_PARABANT, "�������������� �������", //3
  "Parabolic antenna", "PARAB_ANT" }
};

//���������� ������� �������, ����������� �������������� �������������� ������
//��������� ���� � E- � H-����������.
LAW* AntDirect = NULL;
uint AntDirectCount = 0;

//���������� ������� ��������� ��������� ������� ��������� ����������� � �������
//�������������� ���� �� ������ �������
LAW* TableNoiseLaw = NULL;
uint TableNoiseLawCount = 0;

const double DegRad = 0.01745329251994;
const double RadDeg = 57.2957795131;

//===========================================================
//I. �������� ������������� ������-������� ������ ANTENNA
//   PUBLIC MEMBER-FUNCTION OF CLASS
//===========================================================

//*****************************************************************************
//������: I.1; public; constructor by default
//������������ �������: Antenna()
//����������: ����������� �� ��������� ������ Antenna.
//������� ������������� ��� �������� ������ Antenna �� ���������. ��������������
//������� �� ���������:
//1. Type = HW_DIPOLE (�������� ������������ ��������)
//2. E_Location = MERIDIAN (��������������� � �������������� ��������� - �� �������)
//3. E_DirectType = FE_HWDIPOLE;
//4. H_DirectType = FH_HWDIPOLE;
//5. NoiseAmplLawType = RELAY_LAW
//6. NoisePhaseLawType = RAND2PI_LAW
//�� ��������� ������������ ����������� (�����) ��� �� ������ �������.
//-----------------------------------------------------------------------------
//������������ ���������: ���
//-----------------------------------------------------------------------------
//������������ ��������: ���
//*****************************************************************************
Antenna::Antenna()
{
 //������������� ���������� First
 if (TableNoiseLawCount == 0) First = true;
 else First = false;

 //�������� ������� ��������� ������� ��������� ���� �� ������ �������
 if (TableNoiseLawCount == 0) CreateTableOfNoiseLaw();

 //�������� ������� ������������� �������������� ������ � E- � H-����������
 if (AntDirectCount == 0) CreateTableOfAntDirect();

 //������������� ������������� �������
 Type = HW_DIPOLE; //�������� ������������ ��������
 //(��������������� � �������������� ��������� - �� �������),�.�. ���������
 //��������� ������� ����� ��������� � �������������� ���������� �����.
 E_Location = MERIDIAN;

 E_DirectType = FE_HWDIPOLE; //��� �������������� �������������� � E-���������
 E_Direct = E_HWDipoleFunc;  //�������������� �������������� � E-���������
 E_DirectArg = CreateArgList(1);//��������� ���-�� �������������� � E-���������

 H_DirectType = FH_HWDIPOLE; //��� �������������� �������������� � H-���������
 H_Direct = H_HWDipoleFunc;  //�������������� �������������� � H-���������
 H_DirectArg = CreateArgList(1);//��������� ���-�� �������������� � H-���������

 NoiseAmplLawType = RELAY_LAW; //��� ������ ��������� ��������� ����
 NoiseAmpl = RelayLaw; //����� ��������� ��������� ����
 NoiseAmplArg = CreateArgList(1); //��������� ������ ��������� ��������� ����

 NoisePhaseLawType = RAND2PI_LAW; //��� ������ ��������� ��������� ����
 NoisePhase = Rand2PiLaw; //����� ��������� ��������� ����
 NoisePhaseArg = CreateArgList(1); //��������� ������ ��������� ��������� ����
}
//*****************************************************************************
//������: I.2; public; constructor with arguments
//������������ �������: Antenna(uint AntType, bool EPlLocate = MERIDIAN,
//                      ARGLIST* E_Arg = NULL, ARGLIST* H_Arg = NULL)
//����������: ����������� ������ Antenna c �����������.
//������� ������������� ��� �������� ������ Antenna. ����������� ��������� ���-
//������ ��������� �������:
//1. AntType - ��� �������
//2. EPlLocate - ���������������� E-��������� ������� � �������������� ��� ����-
//���������� ���������� ����� (�� ��������� - EPlLocate = MERIDIAN)
//3. E_Arg - ������ ���������� ������� ��������� �������������� � E-���������
//4. H_Arg - ������ ���������� ������� ��������� �������������� � H-���������
//�� ��������� E_Arg � H_Arg ����� NULL.
//��������� ������� ��������������� �� ���������:
//1. NoiseAmplLawType = RELAY_LAW
//2. NoisePhaseLawType = RAND2PI_LAW
//�� ��������� ������������ ����������� (�����) ��� �� ������ �������.
//-----------------------------------------------------------------------------
//������������ ���������: ��. ����
//-----------------------------------------------------------------------------
//������������ ��������: ���
//*****************************************************************************
Antenna::Antenna(uint AntType, bool EPlLocate, ARGLIST* E_Arg, ARGLIST* H_Arg)
{
 //������������� ���������� First
 if (TableNoiseLawCount == 0) First = true;
 else First = false;

 //�������� ������� ��������� ������� ��������� ���� �� ������ �������
 if (TableNoiseLawCount == 0) CreateTableOfNoiseLaw();
 //�������� ������� ������������� �������������� ������ � E- � H-����������
 if (AntDirectCount == 0) CreateTableOfAntDirect();

 //��������� ������������� ������� �������������� ������� � E- � H-���������
 E_Direct = E_HWDipoleFunc;
 H_Direct = H_HWDipoleFunc;
 E_DirectArg = NULL;
 H_DirectArg = NULL;

 //������������� �������������� �������
 SetPerformance(AntType, EPlLocate, E_Arg, H_Arg);

 NoiseAmplLawType = RELAY_LAW; //��� ������ ��������� ��������� ����
 NoiseAmpl = RelayLaw; //����� ��������� ��������� ����
 NoiseAmplArg = CreateArgList(1); //��������� ������ ��������� ��������� ����

 NoisePhaseLawType = RAND2PI_LAW; //��� ������ ��������� ��������� ����
 NoisePhase = Rand2PiLaw; //����� ��������� ��������� ����
 NoisePhaseArg = CreateArgList(1); //��������� ������ ��������� ��������� ����
}
//*****************************************************************************
//������: I.3; public; constructor with arguments
//������������ �������:
// Antenna(uint AntType, bool EPlLocate, uint AmplLaw, uint PhaseLaw,
//         ARGLIST* E_Arg = NULL, ARGLIST* H_Arg = NULL, ARGLIST* AmplArg = NULL,
//         ARGLIST* PhaseArg = NULL);
//����������: ����������� ������ Antenna c �����������.
//������� ������������� ��� �������� ������ Antenna. ������ ����������� ��������-
//������ ��� �����-������ ������ ANTENNA ��� ���������.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. AntType - ��� �������
//2. EPlLocate - ���������������� E-��������� ������� � �������������� ��� ����-
//   ���������� ���������� ����� (�� ��������� - EPlLocate = MERIDIAN)
//3. uint AmplLaw - ������������� ������ ��������� ��������� ���� �������
//4. PhaseLaw - ������������� ������ ��������� ���� ���� �������
//5. E_Arg - ������ ���������� ������� ��������� �������������� � E-���������
//6. H_Arg - ������ ���������� ������� ��������� �������������� � H-���������
//7. AmplArg - ��������� ������ ��������� ��������� (���� ��� ���������)
//8. PhaseArg - ��������� ������ ��������� ���� (���� ��� ���������
//-----------------------------------------------------------------------------
//������������ ��������: ���
//*****************************************************************************
Antenna::Antenna(uint AntType, bool EPlLocate, uint AmplLaw, uint PhaseLaw,
         ARGLIST* E_Arg, ARGLIST* H_Arg, ARGLIST* AmplArg, ARGLIST* PhaseArg)
{
 //������������� ���������� First
 if (TableNoiseLawCount == 0) First = true;
 else First = false;

 //�������� ������� ��������� ������� ��������� ���� �� ������ �������
 if (TableNoiseLawCount == 0) CreateTableOfNoiseLaw();
 //�������� ������� ������������� �������������� ������ � E- � H-����������
 if (AntDirectCount == 0) CreateTableOfAntDirect();

 //��������� ������������� ������� �������������� ������� � E- � H-���������
 E_Direct = E_HWDipoleFunc;
 H_Direct = H_HWDipoleFunc;
 E_DirectArg = NULL;
 H_DirectArg = NULL;

 //��������� ������������� ��������� ������� ��������� ��������� � ����
 //�� ������ �������� �������
 NoiseAmplArg = NULL; //��������� ������ ��������� ��������� ����
 NoisePhaseArg = NULL; //��������� ������ ��������� ���� ����
 NoiseAmpl = Det0Law;
 NoisePhase = Det0Law;

 //������������� �������������� �������
 SetPerformance(AntType, EPlLocate, E_Arg, H_Arg);

 //������������� ������ ��������� ��������� � ���� ���� �� ������ �������
 SetNoisePerform(AmplLaw, PhaseLaw, AmplArg, PhaseArg);
}
//*****************************************************************************
//������: I.4; public; overload assignment operator
//������������ �������: Antenna& operator =(const Antenna &A)
//����������: ������� ���������� ��������� ������������.
//������� ������������� ��� ������������ ������� ������ Antenna ������� �������
//����� ������ (Ant1 = Ant2).
//-----------------------------------------------------------------------------
//������������ ���������:
//1. const Antenna &A - ������ �� ���������� ������ ������ Antenna
//-----------------------------------------------------------------------------
//������������ ��������: Antenna& - ������ �� ���������� ������.
//*****************************************************************************
Antenna& Antenna::operator =(const Antenna &A)
{
  //�������������� ������� ������������ ������� ������ ����.
  if (this == &A) return *this;

  //����������� ������������� �������
  Type = A.Type; //������������� ���� �������
  //������������ E-��������� ������� ������������ �������������� ��� �����������-
  //��� ��������� �����
  E_Location = A.E_Location;

  E_DirectType = A.E_DirectType; //��� ��������� �������������� � E-���������
  E_Direct = A.E_Direct; //��������� �������������� � E-���������
  //��������� ��������� �������������� � E-���������
  E_DirectArg = CopyArgList(A.E_DirectArg, E_DirectArg);

  H_DirectType = A.H_DirectType; //��� ��������� �������������� � H-���������
  H_Direct = A.H_Direct; //��������� �������������� � H-���������
  //��������� ��������� �������������� � H-���������
  H_DirectArg = CopyArgList(A.H_DirectArg, H_DirectArg);

  NoiseAmplLawType = A.NoiseAmplLawType; //��� ������ ��������� ��������� ����
  NoiseAmpl = A.NoiseAmpl; //����� ��������� ��������� ����
  //��������� ������ ��������� ��������� ����
  NoiseAmplArg = CopyArgList(A.NoiseAmplArg, NoiseAmplArg);

  NoisePhaseLawType = A.NoisePhaseLawType; //��� ������ ��������� ��������� ����
  NoisePhase = A.NoisePhase; //����� ��������� ��������� ����
  //��������� ������ ��������� ��������� ����
  NoisePhaseArg = CopyArgList(A.NoisePhaseArg, NoisePhaseArg);
  //��������� ������� �������� �������
  First = false;
  return *this;
}
//*****************************************************************************
//������: I.5; public; copy constructor of object class Antenna
//������������ �������: Antenna(const Antenna& A)
//����������: ����������� ����������� �������� ������ Antenna.
//������� ������������� ��� �������� ����� ������� ������ Antenna. ������ �����-
//������ ������� ���������� �����, � ���������, �� ��������� ��������� �� ������
//���������� ������� ��������� ��������� � ���� ���� �� ������ �������.
//����������� ����� ������������ � ��������� ���������:
//1. ����� ������ ������ Antenna ������������ ��� ������������� ������ �������
//   ����� �� ������. (ex. Antenna Ant2 = Ant1)
//2. ����� ������ ������ Antenna ���������� ������� ���������� ���� ������ ��
//   ��������.
//3. ����� ������� ���������� ������ ������ Antenna (�� �� ������ ��� ���������
//   �� ������).
//-----------------------------------------------------------------------------
//������������ ���������:
//1. const Antenna &A - ������ �� ���������� ������ ������ Antenna
//-----------------------------------------------------------------------------
//������������ ��������: ���
//*****************************************************************************
Antenna::Antenna(const Antenna& A)
{
 //�������������� ��������� ��������� �������� ����������
 E_DirectArg = NULL; //��������� ��������� �������������� � E-���������
 H_DirectArg = NULL; //��������� ��������� �������������� � H-���������
 E_Direct = E_HWDipoleFunc;
 H_Direct = H_HWDipoleFunc;

 NoiseAmplArg = NULL; //��������� ������ ��������� ��������� ����
 NoisePhaseArg = NULL; //��������� ������ ��������� ���� ����
 NoiseAmpl = Det0Law;
 NoisePhase = Det0Law;

 *this = A; //�������� ������������� �������� ������������
}

//*****************************************************************************
//������: I.6; public; Antenna class destructor
//������������ �������: ~Antenna()
//����������: ���������� ������ Antenna.
//������� ������������� ��� ���������� ������ Antenna. ��� ������ �������
//��������� ������� ��������� ������� ��������� ���� � ��������� ���� �� ������
//������� TableNoiseLaw, ������ ���������� ������� ��������� ��������� � ����
//����.
//-----------------------------------------------------------------------------
//������������ ���������: ���
//-----------------------------------------------------------------------------
//������������ ��������: ���
//*****************************************************************************
Antenna::~Antenna()
{
 //#if defined(CONSOLE)
 //  uint Index = GetIndexByAntType(Type);
 //  cout << "\nDestroy object of ANTENNA class..." << endl;
 //  cout << "Antenna type: " << GetAntNameByIndex(Index) << endl;
 //#endif
 //�������� ������� ������� �� ������
 if ((TableNoiseLawCount != 0) && (First == true))
 {
  //#if defined(CONSOLE)
  // cout << "Delete Law Noise Table." << endl;
  // cout << "Delete Antennas Patterns Table." << endl;
  //#endif
  DeleteTable(TableNoiseLaw, TableNoiseLawCount);
  DeleteTable(AntDirect, AntDirectCount);
 }

 if (E_DirectArg != NULL) DeleteArgList(E_DirectArg);
 if (H_DirectArg != NULL) DeleteArgList(H_DirectArg);

 if (NoiseAmplArg != NULL) DeleteArgList(NoiseAmplArg);
 if (NoisePhaseArg != NULL) DeleteArgList(NoisePhaseArg);
}
//*****************************************************************************
//������: I.7; public member-function of class Antenna
//������������ �������: string& GetPerformance(string& s, bool headfoot = ON)
//����������:
//������� ��������� �������������� ������� Antenna � ���� ������. ������ ��������
//������� ����������� �������� '\n'. �������������� ������� (������):
//1. Antenna Type = Half-wave dipole
//2. E-plane is oriented on a MERIDIAN plane of the Earth
//3. Noise Amplitude Law = RELAY_LAW
//4. Noise Phase Law = RAND2PI_LAW
//-----------------------------------------------------------------------------
//������������ ���������:
//1. string& s - ������ �� ��������� ������
//2. bool headfoot - ��������� ������������ ��������� � ������� ������
//                   ������������� �������
//-----------------------------------------------------------------------------
//������������ ��������: string& s - ������ �� ���������� �������
//*****************************************************************************
string& Antenna::GetPerformance(string& s, bool headfoot)
{uint Select;
 uint Index = GetIndexByAntType(Type);
 char buffer[80];
 ostrstream text (buffer, sizeof(buffer));

 s.resize(0);
 if (headfoot == ON)
 { text << "*******************  Antenna Performance  *******************"
        << endl << ends;
   s.append(text.str());
   text.seekp(0);
 }
 s.append("1. Antenna Type: ");
 text << GetAntNameByIndex(Index) << endl << ends;
 s.append(text.str());
 text.seekp(0);

 s.append("2. E-plane is oriented on a ");
 if (E_Location == MERIDIAN) text << "MERIDIONAL ";
 else text << "EQUATORIAL ";
 text << "plane of the Earth." << endl << ends;
 s.append(text.str());
 text.seekp(0);

 s.append("3. Noise Amplitude Law = ");
 //�������� ������ ���������� ������ ��������� ��������� ���� � ������� �������
 Select = GetIndexLawByType(TableNoiseLaw,TableNoiseLawCount,NoiseAmplLawType);
 text << TableNoiseLaw[Select].mathlawfactor(NoiseAmplArg) << endl << ends;
 s.append(text.str());
 text.seekp(0);

 s.append("4. Noise Phase Law = ");
 //�������� ������ ���������� ������ ��������� ���� ���� � ������� �������
 Select = GetIndexLawByType(TableNoiseLaw,TableNoiseLawCount,NoisePhaseLawType);
 text << TableNoiseLaw[Select].mathlawfactor(NoisePhaseArg) << endl << ends;
 s.append(text.str());
 text.seekp(0);

 if (headfoot == ON)
 { text << "*************************************************************"
        << endl << ends;
   s.append(text.str());
 }
return s;
}
//*****************************************************************************
//������: I.8; public member-function of class Antenna
//������������ �������:
// void SetPerformance(uint AntType, bool EPlLocate = MERIDIAN,
//                     ARGLIST* E_Arg = NULL,  ARGLIST* H_Arg = NULL)
//����������:
//������� ������������� �������������� �������.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. AntType - ��� �������
//2. EPlLocate - ���������������� E-��������� ������� � �������������� ��� ����-
//���������� ���������� ����� (�� ��������� - EPlLocate = MERIDIAN)
//3. E_Arg - ������ ���������� ������� ��������� �������������� � E-���������
//4. H_Arg - ������ ���������� ������� ��������� �������������� � H-���������
//�� ��������� E_Arg � H_Arg ����� NULL.
//-----------------------------------------------------------------------------
//������������ ��������: ���
//*****************************************************************************
void Antenna::SetPerformance(uint AntType, bool EPlLocate, ARGLIST* E_Arg,
                             ARGLIST* H_Arg)
{uint Index;
 try
 { //���������� ������ ���������� ���������� � ���� ������� AntType � �������
   //AntennInfo
   Index = GetIndexByAntType(AntType);
   //���������� �� ������� ���� AntType �����������, ������������ ��������������
   //��������
   if (Index == 0) throw ANTENNA_ABSENT;

   //������������� ������������� �������
   Type = AntType;  //������������� ���� �������
   //���������� E-��������� ������������ �������������� ��� ��������������
   //���������� �����
   E_Location = EPlLocate;

   //�������� ����������
   if (E_DirectArg != NULL) DeleteArgList(E_DirectArg);
   if (H_DirectArg != NULL) DeleteArgList(H_DirectArg);

   //������������� ������� ��������� �������������� ������� � E-���������
   uint IdEHDirect = GetIdEDirByIndex(Index);
   E_DirectArg = SetLaw(AntDirect, AntDirectCount, IdEHDirect, E_DirectType,
                        E_Direct, E_Arg);

   //������������� ������� ��������� �������������� ������� � H-���������
   IdEHDirect = GetIdHDirByIndex(Index);
   H_DirectArg = SetLaw(AntDirect, AntDirectCount, IdEHDirect, H_DirectType,
                        H_Direct, H_Arg);
 }
 catch (int error_type)
 {
   if (error_type == ANTENNA_ABSENT) //���������� �� ������� �����������
   {
    #if defined(CONSOLE)
     cout << "\nANTENNA IS ABSENT IN AntennInfo TABLE !!!" << endl;
     cout << "Error in constructor with arguments Antenna" << endl;
     cout << "Can't find antenna with the specific identifier " << AntType
          << endl;
     getch();
    #endif
     exit(-1);
   }
 }
}
//*****************************************************************************
//������: I.9; public member-function of class Antenna
//������������ �������: void SetNoisePerform(uint AmplLaw, uint PhaseLaw,
//                      ARGLIST* AmplArg = NULL,  ARGLIST* PhaseArg = NULL);
//����������:
//������� ������������� ��������� ������ ��������� ��������� � ���� ���� �� ��-
//���� �������� �������.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. uint AmplLaw - ������������� ������ ��������� ��������� ���� �������
//2. PhaseLaw - ������������� ������ ��������� ���� ���� �������
//3. AmplArg - ��������� ������ ��������� ��������� (���� ��� ���������)
//4. PhaseArg - ��������� ������ ��������� ���� (���� ��� ���������
//-----------------------------------------------------------------------------
//������������ ��������: ���
//*****************************************************************************
void Antenna::SetNoisePerform(uint AmplLaw, uint PhaseLaw, ARGLIST* AmplArg,
              ARGLIST* PhaseArg)
{
  if (NoiseAmplArg != NULL) DeleteArgList(NoiseAmplArg);
  if (NoisePhaseArg != NULL) DeleteArgList(NoisePhaseArg);


  //������������� ����� ��������� ��������� ���� �� ������ �������
  NoiseAmplArg = SetLaw(TableNoiseLaw, TableNoiseLawCount, AmplLaw,
                 NoiseAmplLawType, NoiseAmpl, AmplArg);
  //������������� ����� ��������� ���� ���� �� ������ �������
  NoisePhaseArg = SetLaw(TableNoiseLaw, TableNoiseLawCount, PhaseLaw,
                  NoisePhaseLawType, NoisePhase, PhaseArg);
}
//*****************************************************************************
//������: I.10; public member-function of class Antenna
//������������ �������: complex CalcUNoise()
//����������: Calculate instantaneous value of voltage noise on antenna output
//������� ��������� ���������� �������� ������ ���� �� ������ ������� �� ������-
//��� ������� ��������� ��������� � ���� ����.
//-----------------------------------------------------------------------------
//������������ ���������: ���
//-----------------------------------------------------------------------------
//������������ ��������: complex U  - ����������� �������� ������ ����
//*****************************************************************************
complex Antenna::CalcUNoise()
{
 return polar(GetAmplNoise(),GetPhaseNoise());
}
//*****************************************************************************
//������: I.11; public member-function of class Antenna
//������������ �������: double CalcAntPatterns(bool EathPlane)
//����������:
//������� ��������� �������������� �������������� ������� � �������������� (��
//���� �����) ��� � �������������� (�� �������) ��������� � ����������� ��
//������, ������������� ��� ����� Angle ������������ � ������� �������� �������.
//������� �������������� �������������� ������� � �������������� ��� �����������-
//��� ��������� ����� ���������� � ����������� �� �������� EarthPlane.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. bool EathPlane - ��������� �����, ������������ ����� ������� ��������������
//   �������. (MERIDIAN; EQUATOR))
//-----------------------------------------------------------------------------
//������������ ��������: double value - �������� ���-�� �������������� �������
//*****************************************************************************
double Antenna::CalcAntPatterns(bool EathPlane)
{double value;
 //�������� ������� �������������� �������������� �������, ���������������
 //EarthPlane
 //E-��������� ������� ��������� � EarthPlane
 if (EathPlane == E_Location) value = E_Direct(E_DirectArg);
 else value = H_Direct(H_DirectArg);

 return value;
}
//*****************************************************************************
//������: I.12; public member-function of class Antenna
//������������ �������:
//ARGLIST* AntPatterns(bool EathPlane, double InitAngle = -90.0,
//                     double EndAngle = 90.0, double Step = 0.1, bool Deg = DEG);
//����������:
//������� ���������� ��������� �������������� ������� � �������������� (��
//���� �����) ��� � �������������� (�� �������) ��������� � ������� �� InitAngle
//�� EndAngle. ���� ������������� ������������ ������� � �������� �������.
//������� �������������� �������������� ������� � �������������� ��� �����������-
//��� ��������� ����� ���������� � ����������� �� �������� EarthPlane.
//��� �������� �������������� �������������� ������� � �������� ������� �������-
//�� � ������ ARGLIST. ��������� �������� ������ � ������ ARGLIST:
//List[0] - InitAngle; List[1] - EndAngle; List[2] - Step; List[3]...List[n] -
//������.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. bool EathPlane - ��������� �����, ������������ ����� ������� ��������������
//   �������. (MERIDIAN; EQUATOR))
//2. double InitAngle (-90 deg) - ��������� ����, ���������� ��������� ���������-
//   ����� ������� (antenna patterns)
//3. double EndAngle (+90 deg) - �������� ����, ���������� ��������� ���������-
//   ����� �������.
//4. double Step - ���������� ����
//5. bool Deg (DEG) - ������������� ���� (� �������� - DEG ��� �������� - RAD)
//-----------------------------------------------------------------------------
//������������ ��������:
// ARGLIST* data - ��������� �������������� �������
//*****************************************************************************
ARGLIST* Antenna::AntPatterns(bool EathPlane, double InitAngle, double EndAngle,
                  double Step, bool Deg)
{ulong Count; //���������� ������, ������� ���������� ���������� � ������ data

 if (EndAngle < InitAngle)
 { double Value = InitAngle;
   InitAngle = EndAngle;
   EndAngle = Value;
 }
 //���������� ���� ����� ���� ������ ������������� ���������
 Step = fabs(Step);
 if (Step == 0.0) Step = (Deg == DEG) ? 0.1 : 0.1*DegRad;
 //���������� ���������� ������, ������� ����� ��������� � ������ data
 Count = ceil((EndAngle-InitAngle)/Step)+1;
 //������� ������ data ��� �������� ������
 ARGLIST* data = CreateArgList(Count+3);
 //������� � ������ ������ ����������� ��������� �������������� �������
 //� ��������
 if (Deg == RAD) //������� �� ������ � �������
 { InitAngle = InitAngle*RadDeg;
   EndAngle = EndAngle*RadDeg;
   Step = Step*RAD_DEG;
 }
 data->List[0] = InitAngle; data->List[1] = EndAngle; data->List[2] = Step;
 double Angle = InitAngle;
 //����������� ��������� �������������� ������� � �������� ������� ������
 for (ulong index = 3; index < data->Size; index++)
 { SetAngle(Angle,Angle,Deg);
   data->List[index] = CalcAntPatterns(EathPlane);
   Angle = Angle+Step;
 }
return data;
}
//*****************************************************************************
//������: I.13; public member-function of class Antenna
//������������ �������:
//  void SetAngle(double AzimuthAngle, double ElevateAngle, bool Deg = DEG)
//����������:
//������� ������������� ���� ��������� �������, ��������������� E-��������� �
//H-��������� ������� � ����������� �� �� ������������ � ������������.
//���� E-��������� ������� ��������� � �������������� ���������� �����, �� ��
//����� ��������������� ElevateAngle, � H-��������� - AzimuthAngle.
//���� E-��������� ������� ��������� � �������������� ���������� �����, �� ��
//����� ��������������� AzimuthAngle, � H-��������� - ElevateAngle.
//-----------------------------------------------------------------------------
//������������ ���������:
//-----------------------------------------------------------------------------
//������������ ��������: ���
//*****************************************************************************
void Antenna::SetAngle(double AzimuthAngle, double ElevateAngle, bool Deg)
{
 if (Deg == DEG) //���� ������ � ��������
 { AzimuthAngle = AzimuthAngle*DegRad;
   ElevateAngle = ElevateAngle*DegRad;
 }
 //E-��������� ������� ��������� � �������������� ���������� �����
 if (E_Location == MERIDIAN)
 { E_DirectArg->List[0] = ElevateAngle;
   H_DirectArg->List[0] = AzimuthAngle;
 }
 //E-��������� ������� ��������� � �������������� ���������� �����
 else
 { E_DirectArg->List[0] = AzimuthAngle;
   H_DirectArg->List[0] = ElevateAngle;
 }
}
//*****************************************************************************
//������: I.14; public; Antenna class member-function
//������������ �������: void SetRandNumber();
//����������:
//������� �������� ������� �������� ��������� �������� � ��� ��� ����������
//������������� ���� �� ������ �������� �������.
//��������� �������� �������� (���) ��������� � ������ ���������� ���������
//������� ��� �������� ������ 0. ������ ������� ��������� ��������� ��������
//�������� ��������� � ���������� ������, ������������ ��������� ���� ��� �����-
//���� ����, ��� ����������� ���������� �������� ���������� ��� ���������
//���� � ���� � ��� �� ������ ������� t. ������� ��������� � ��������� ��������
//������� �� � ��������� ������ ��������� �������� ��������� (����), ���
//�������� ������������. ��������� �������������� ���� �������� �������:
//1. NoiseAmpl  - ARGLIST* NoiseAmplArg
//2. NoisePhase - ARGLIST* NoisePhaseArg
//-----------------------------------------------------------------------------
//������������ ���������: ���
//-----------------------------------------------------------------------------
//������������ ��������: ���
//*****************************************************************************
void Antenna::SetRandNumber()
{
  NoiseAmplArg->List[0]  = NoiseAmpl(NoiseAmplArg);
  NoisePhaseArg->List[0] = NoisePhase(NoisePhaseArg);
}


//======================================================
//II. �������� �������� ������-������� ������ ANTENNA
//    PRIVATE MEMBER-FUNCTION OF CLASS
//======================================================

//*****************************************************************************
//������: II.1, private, class member-function
//������������ �������: void CreateTableOfAntDirect()
//����������:
//�������� ������� �������, ����������� �������������� �������������� �������
//��������� ���� � E- � H-����������.
//��� ������ ������� ���������� ���������� ��������� AntDirect � AntDirectCount
//������ ������� (�� �������, ����� ���������� - 3):
//1. E_HWDipoleFunc; 2. H_HWDipoleFunc; 3. EH_ParabAntFunc;
//-----------------------------------------------------------------------------
//������������ ���������: ���
//-----------------------------------------------------------------------------
//������������ ��������:  ���
//*****************************************************************************
void Antenna::CreateTableOfAntDirect()
{
  AntDirectCount = 3;
  AntDirect = new LAW[3];

 //***************************************************************************
 //������ ������� - �������������� �������������� ������������� ��������� �
 //E-��������� (��������� ������������ ���� � H-���������)
 //��������� ������� �����������. �������� - ���� (� ��������)
 //***************************************************************************
 AntDirect[0].turn = ON;
 AntDirect[0].law = E_HWDipoleFunc;
 AntDirect[0].type = FE_HWDIPOLE;
 AntDirect[0].inputfactor = InitFactors;
 AntDirect[0].mathlawfactor = MathNotateE_HWDipole;
 AntDirect[0].M.DimFunc = NONDIM;
 AntDirect[0].M.Size = 1;
 AntDirect[0].M.Factor = new FACTOR[1];

 //Angle - Meridional or Equatorial
 AntDirect[0].M.Factor[0].SourceDim = DEGREE;
 AntDirect[0].M.Factor[0].TargetDim = RADIAN;
 AntDirect[0].M.Factor[0].Name = new char[6];
 strcpy(AntDirect[0].M.Factor[0].Name,"Angle");
 AntDirect[0].M.Factor[0].Convert = DegToRad;
 AntDirect[0].M.Factor[0].Invert = RadToDeg;
 AntDirect[0].M.Factor[0].PreBlock = AlwaysTrue;
 AntDirect[0].M.Factor[0].PostBlock = AlwaysTrue;

 //***************************************************************************
 //������ ������� - �������������� �������������� ������������� ��������� �
 //H-��������� (��������� ������������ ���� � E-���������)
 //��������� ������� �����������. �������� - ���� (� ��������)
 //***************************************************************************
 AntDirect[1].turn = ON;
 AntDirect[1].law = H_HWDipoleFunc;
 AntDirect[1].type = FH_HWDIPOLE;
 AntDirect[1].inputfactor = InitFactors;
 AntDirect[1].mathlawfactor = MathNotateH_HWDipole;
 AntDirect[1].M.DimFunc = NONDIM;
 AntDirect[1].M.Size = 1;
 AntDirect[1].M.Factor = new FACTOR[1];

 //Angle - Meridional or Equatorial
 AntDirect[1].M.Factor[0].SourceDim = DEGREE;
 AntDirect[1].M.Factor[0].TargetDim = RADIAN;
 AntDirect[1].M.Factor[0].Name = new char[6];
 strcpy(AntDirect[1].M.Factor[0].Name,"Angle");
 AntDirect[1].M.Factor[0].Convert = DegToRad;
 AntDirect[1].M.Factor[0].Invert = RadToDeg;
 AntDirect[1].M.Factor[0].PreBlock = AlwaysTrue;
 AntDirect[1].M.Factor[0].PostBlock = AlwaysTrue;

 //***************************************************************************
 //������ ������� - �������������� �������������� �������������� ������� �
 //E- � � H-����������
 //�������� - ���� (� ��������). ��������� �������: 1)La - ������������� �������
 //�������������� ������� (D/l, l-����� �����, D-������� ��������������� �������
 //� E- ��� H-���������).
 //***************************************************************************
 AntDirect[2].turn = ON;
 AntDirect[2].law = EH_ParabAntFunc;
 AntDirect[2].type = FEH_PARABANT;
 AntDirect[2].inputfactor = InitFactors;
 AntDirect[2].mathlawfactor = MathNotateEH_ParabAnt;
 AntDirect[2].M.DimFunc = NONDIM;
 AntDirect[2].M.Size = 2;
 AntDirect[2].M.Factor = new FACTOR[2];

 //Angle - Meridional or Equatorial
 AntDirect[2].M.Factor[0].SourceDim = DEGREE;
 AntDirect[2].M.Factor[0].TargetDim = RADIAN;
 AntDirect[2].M.Factor[0].Name = new char[6];
 strcpy(AntDirect[2].M.Factor[0].Name,"Angle");
 AntDirect[2].M.Factor[0].Convert = DegToRad;
 AntDirect[2].M.Factor[0].Invert = RadToDeg;
 AntDirect[2].M.Factor[0].PreBlock = AlwaysTrue;
 AntDirect[2].M.Factor[0].PostBlock = AlwaysTrue;

 //La - Relative aerial aperture
 AntDirect[2].M.Factor[1].SourceDim = NONDIM;
 AntDirect[2].M.Factor[1].TargetDim = NONDIM;
 AntDirect[2].M.Factor[1].Name = new char[18];
 strcpy(AntDirect[2].M.Factor[1].Name,"Relative Aperture");
 AntDirect[2].M.Factor[1].Convert = Copy;
 AntDirect[2].M.Factor[1].Invert = Copy;
 AntDirect[2].M.Factor[1].PreBlock = AlwaysTrue;
 AntDirect[2].M.Factor[1].PostBlock = AlwaysTrue;
}
//*****************************************************************************
//������: II.2, private, class member-function
//������������ �������: void CreateTableOfNoiseLaw()
//����������:
//�������� ������� ��������� ������� ��������� �� ������� ��������� � ���� ����
//�� ������ �������.
//��� ������ ������� ���������� ���������� ��������� TableNoiseLaw �
//TableNoiseLawCount
//������ ������� (�� �������, ����� ���������� - 6):
//1. RandLaw; 2. Rand2PiLaw; 3. RandABLaw; 4. RelayLaw; 5. Det1Law; 6. Det0Law
//-----------------------------------------------------------------------------
//������������ ���������: ���
//-----------------------------------------------------------------------------
//������������ ��������:  ���
//*****************************************************************************
void Antenna::CreateTableOfNoiseLaw()
{
 TableNoiseLawCount = 6;
 TableNoiseLaw = new LAW[6];

 //********************************************************
 //������ ����� - �������������� ����� � ��������� [0..1]
 //��������� ������� � �������� �����������.
 //********************************************************
 // {RandLaw, RAND_LAW, InitFactors, GetMathNotateRandLaw, NONDIM, 0}
 TableNoiseLaw[0].turn = ON;
 TableNoiseLaw[0].law = RandLaw;
 TableNoiseLaw[0].type = RAND_LAW;
 TableNoiseLaw[0].inputfactor = InitFactors;
 TableNoiseLaw[0].mathlawfactor = GetMathNotateRandLaw;
 TableNoiseLaw[0].M.DimFunc = NONDIM;
 TableNoiseLaw[0].M.Size = 1;
 TableNoiseLaw[0].M.Factor = new FACTOR[1];

 //RandValue
 TableNoiseLaw[0].M.Factor[0].SourceDim = NONDIM;
 TableNoiseLaw[0].M.Factor[0].TargetDim = NONDIM;
 TableNoiseLaw[0].M.Factor[0].Name = new char[5];
 strcpy(TableNoiseLaw[0].M.Factor[0].Name,"Rand");
 TableNoiseLaw[0].M.Factor[0].Convert = Copy;
 TableNoiseLaw[0].M.Factor[0].Invert = Copy;
 TableNoiseLaw[0].M.Factor[0].PreBlock = AlwaysTrue;
 TableNoiseLaw[0].M.Factor[0].PostBlock = AlwaysTrue;

 //*************************************************************
 //������ ����� - �������������� ����� � ��������� [0..2PI]
 //*************************************************************
 //{Ran2PiLaw,RAND2PI_LAW, InitFactors, GetMathNotateRand2PiLaw, NONDIM, 0}
 TableNoiseLaw[1].turn = ON;
 TableNoiseLaw[1].law = Rand2PiLaw;
 TableNoiseLaw[1].type = RAND2PI_LAW;
 TableNoiseLaw[1].inputfactor = InitFactors;
 TableNoiseLaw[1].mathlawfactor = GetMathNotateRand2PiLaw;
 TableNoiseLaw[1].M.DimFunc = NONDIM;
 TableNoiseLaw[1].M.Size = 1;
 TableNoiseLaw[1].M.Factor = new FACTOR[1];

 //RandValue
 TableNoiseLaw[1].M.Factor[0].SourceDim = NONDIM;
 TableNoiseLaw[1].M.Factor[0].TargetDim = NONDIM;
 TableNoiseLaw[1].M.Factor[0].Name = new char[5];
 strcpy(TableNoiseLaw[1].M.Factor[0].Name,"Rand");
 TableNoiseLaw[1].M.Factor[0].Convert = Copy;
 TableNoiseLaw[1].M.Factor[0].Invert = Copy;
 TableNoiseLaw[1].M.Factor[0].PreBlock = AlwaysTrue;
 TableNoiseLaw[1].M.Factor[0].PostBlock = AlwaysTrue;

 //*************************************************************
 //������ ����� - �������������� ����� � ��������� [A..B]
 //*************************************************************
 //{RanABLaw,RANDAB_LAW, InitFactors, GetMathNotateRandABLaw, NONDIM, 2}
 TableNoiseLaw[2].turn = ON;
 TableNoiseLaw[2].law = RandABLaw;
 TableNoiseLaw[2].type = RANDAB_LAW;
 TableNoiseLaw[2].inputfactor = InitFactors;
 TableNoiseLaw[2].mathlawfactor = GetMathNotateRandABLaw;
 TableNoiseLaw[2].M.DimFunc = NONDIM;
 TableNoiseLaw[2].M.Size = 3;
 TableNoiseLaw[2].M.Factor = new FACTOR[3];

 //RandValue
 TableNoiseLaw[2].M.Factor[0].SourceDim = NONDIM;
 TableNoiseLaw[2].M.Factor[0].TargetDim = NONDIM;
 TableNoiseLaw[2].M.Factor[0].Name = new char[5];
 strcpy(TableNoiseLaw[2].M.Factor[0].Name,"Rand");
 TableNoiseLaw[2].M.Factor[0].Convert = Copy;
 TableNoiseLaw[2].M.Factor[0].Invert = Copy;
 TableNoiseLaw[2].M.Factor[0].PreBlock = AlwaysTrue;
 TableNoiseLaw[2].M.Factor[0].PostBlock = AlwaysTrue;

 //A  {NONDIM,NONDIM,"Lower border A",Copy,Copy,AlwaysTrue,AlwaysTrue}
 TableNoiseLaw[2].M.Factor[1].SourceDim = NONDIM;
 TableNoiseLaw[2].M.Factor[1].TargetDim = NONDIM;
 TableNoiseLaw[2].M.Factor[1].Name = new char[15];
 strcpy(TableNoiseLaw[2].M.Factor[1].Name,"Lower border A");
 TableNoiseLaw[2].M.Factor[1].Convert = Copy;
 TableNoiseLaw[2].M.Factor[1].Invert = Copy;
 TableNoiseLaw[2].M.Factor[1].PreBlock = AlwaysTrue;
 TableNoiseLaw[2].M.Factor[1].PostBlock = AlwaysTrue;

 //B  {NONDIM,NONDIM,"Upper border B",Copy,Copy,AlwaysTrue,AlwaysTrue}
 TableNoiseLaw[2].M.Factor[2].SourceDim = NONDIM;
 TableNoiseLaw[2].M.Factor[2].TargetDim = NONDIM;
 TableNoiseLaw[2].M.Factor[2].Name = new char[15];
 strcpy(TableNoiseLaw[2].M.Factor[2].Name,"Upper border B");
 TableNoiseLaw[2].M.Factor[2].Convert = Copy;
 TableNoiseLaw[2].M.Factor[2].Invert = Copy;
 TableNoiseLaw[2].M.Factor[2].PreBlock = AlwaysTrue;
 TableNoiseLaw[2].M.Factor[2].PostBlock = AlwaysTrue;

 //*************************************************************
 //��������� ����� - ��������� ����� ����� [0..+INFINITE]
 //*************************************************************
 //{RelayLaw,RELAY_LAW, InitFactors, GetMathNotateRandLaw, NONDIM, 0}
 TableNoiseLaw[3].turn = ON;
 TableNoiseLaw[3].law = RelayLaw;
 TableNoiseLaw[3].type = RELAY_LAW;
 TableNoiseLaw[3].inputfactor = InitFactors;
 TableNoiseLaw[3].mathlawfactor = GetMathNotateRelayLaw;
 TableNoiseLaw[3].M.DimFunc = NONDIM;
 TableNoiseLaw[3].M.Size = 1;
 TableNoiseLaw[3].M.Factor = new FACTOR[1];

 //RandValue
 TableNoiseLaw[3].M.Factor[0].SourceDim = NONDIM;
 TableNoiseLaw[3].M.Factor[0].TargetDim = NONDIM;
 TableNoiseLaw[3].M.Factor[0].Name = new char[5];
 strcpy(TableNoiseLaw[3].M.Factor[0].Name,"Rand");
 TableNoiseLaw[3].M.Factor[0].Convert = Copy;
 TableNoiseLaw[3].M.Factor[0].Invert = Copy;
 TableNoiseLaw[3].M.Factor[0].PreBlock = AlwaysTrue;
 TableNoiseLaw[3].M.Factor[0].PostBlock = AlwaysTrue;

 //*************************************************************
 //����� ����� - ����� ���������� �� �� ������������
 //*************************************************************
 //{Det1Law, DET1_LAW, InitFactors, GetMathNotateDet1Law, NONDIM, 0}
 TableNoiseLaw[4].turn = ON;
 TableNoiseLaw[4].law = Det1Law;
 TableNoiseLaw[4].type = DET1_LAW;
 TableNoiseLaw[4].inputfactor = InitFactors;
 TableNoiseLaw[4].mathlawfactor = GetMathNotateDet1Law;
 TableNoiseLaw[4].M.DimFunc = NONDIM;
 TableNoiseLaw[4].M.Size = 1;
 TableNoiseLaw[4].M.Factor = new FACTOR[1];

 //RandValue
 TableNoiseLaw[4].M.Factor[0].SourceDim = NONDIM;
 TableNoiseLaw[4].M.Factor[0].TargetDim = NONDIM;
 TableNoiseLaw[4].M.Factor[0].Name = new char[5];
 strcpy(TableNoiseLaw[4].M.Factor[0].Name,"Rand");
 TableNoiseLaw[4].M.Factor[0].Convert = Copy;
 TableNoiseLaw[4].M.Factor[0].Invert = Copy;
 TableNoiseLaw[4].M.Factor[0].PreBlock = AlwaysTrue;
 TableNoiseLaw[4].M.Factor[0].PostBlock = AlwaysTrue;

 //*************************************************************
 //������ ����� - ����� ���������� �� �� �����
 //*************************************************************
 //{Det0Law, DET0_LAW, InitFactors, GetMathNotateDet0Law, NONDIM, 0}
 TableNoiseLaw[5].turn = ON;
 TableNoiseLaw[5].law = Det0Law;
 TableNoiseLaw[5].type = DET0_LAW;
 TableNoiseLaw[5].inputfactor = InitFactors;
 TableNoiseLaw[5].mathlawfactor = GetMathNotateDet0Law;
 TableNoiseLaw[5].M.DimFunc = NONDIM;
 TableNoiseLaw[5].M.Size = 1;
 TableNoiseLaw[5].M.Factor = new FACTOR[1];

 //RandValue
 TableNoiseLaw[5].M.Factor[0].SourceDim = NONDIM;
 TableNoiseLaw[5].M.Factor[0].TargetDim = NONDIM;
 TableNoiseLaw[5].M.Factor[0].Name = new char[5];
 strcpy(TableNoiseLaw[5].M.Factor[0].Name,"Rand");
 TableNoiseLaw[5].M.Factor[0].Convert = Copy;
 TableNoiseLaw[5].M.Factor[0].Invert = Copy;
 TableNoiseLaw[5].M.Factor[0].PreBlock = AlwaysTrue;
 TableNoiseLaw[5].M.Factor[0].PostBlock = AlwaysTrue;

}
//*****************************************************************************
//������: II.3., private, class member-function
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
void Antenna::DeleteTable(LAW* Table, uint& Size)
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

//======================================================
//III. �������� ���������� ������-������� ������ ANTENNA
//     PROTECTED MEMBER-FUNCTION OF CLASS
//======================================================

//*****************************************************************************
//������: III.1., protected, class member-function
//������������ �������:
// uint GetIndexLawByType(const LAW* Table, uint Count, uint Type)
//����������:
//������� ���������� ������ ������������ ������ � ������� ������� �� ��� ����.
//������� ������������ ��� ����������� ������ ��������� ���� �� ������ �������.
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
uint Antenna::GetIndexLawByType(const LAW* Table, const uint Count, uint Type)
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
//������: III.2., protected, class member-function
//������������ �������:
//ARGLIST* SetLaw(const LAW* Table, const uint Count, const uint SLawType,
//                uint& TLawType, LAWFUNC& RefFunc, ARGLIST* SArg = NULL);
//����������:
//������� ������������� ����� ��������� �������� ��������� ���� SLawType c ��-
//������� ����������� SArg.
//���� ����� ������ ���� �� ���������� � ������� �������� ������� Table, ��
//��������� ��������� ���� ������.
//���� ������ ���������� SArg �� ������������� ���������� ����� ���������� ���
//��������� ������, �� ��������� ����� ��������� ���� ������.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. const LAW* Table - ������� �������
//2. const uint Count - ���������� ������� � �������
//3. const uint SLawType - ������������� ������, ������� ����� ����������
//4. uint& TLawType - ������������� ������, ����������� �� SLawType
//5. LAWFUNC& RefFunc - �������, ����������� ����� �� ��������
//   ����� ���������� ��������
//6. const ARGLIST* SArg - ������ ����������, ���������������� ������
//-----------------------------------------------------------------------------
//������������ ��������:
//ARGLIST* Arg - ��������� �� ������ ����������, ������� ����� ���������������
//���������������� ������
//*****************************************************************************
ARGLIST* Antenna::SetLaw(const LAW* Table, const uint Count, const uint SLawType,
                         uint& TLawType, LAWFUNC& RefFunc, ARGLIST* SArg)
{uint TLen, SLen;
 uint Index;
 ARGLIST* Arg = NULL;
 //���������� ������������ ���������������� ������ � ������� �������
 Index = GetIndexLawByType(Table, Count, SLawType);

 //���������� ������ ������ ���������� ���������������� ������ � ���������� ���
 //� ��������� ��������
 SLen = (SArg == NULL) ? 0 : SArg->Size;
 TLen = Table[Index].M.Size;
 //������� �� ����� ����������, ����� ���������
 if ((TLen == 1) && (SLen == 0)) {SLen = 1; SArg = CreateArgList(SLen);}
 try
 { //������� ������������� SLen � ���������� TLen ������ ���������� ���������
   if (SLen == TLen) Arg = CopyArgList(SArg, Arg);
   //�������������� ��������� � ���������� ���������� ���������� ��� ������
   //� ��������������� SLawType
   else throw ARG_MISMATCH;

   TLawType = SLawType;
   RefFunc = Table[Index].law;
 }
 catch (int error_type)
 {
  if (error_type == ARG_MISMATCH)
  {
   #if defined(CONSOLE)
    cout << "\nMISMATCH ARGUMENT ERROR !!!" << endl;
    cout << "Error in function Antenna::SetLaw" << endl;
    cout << "Discordance of initial and required parameters amount for the law\n"
         << "with the identifier SLawType" << endl;
    cout << "SourceArgSize = " << SLen << endl;
    cout << "TargetArgSize = " << TLen << endl;
    getch();
   #endif
    exit(-1);
  }
 }
return Arg;
}

//*************************************************************************
// IV. �������, ����������� �������������� �������������� �������
//     ��������� ���� � E- � H-����������
//*************************************************************************

//*****************************************************************************
//������: IV.1
//������������ �������: double E_HWDipoleFunc(const ARGLIST* Arg)
//����������: F(x)=|cos(sin(x)*PI/2)/cos(x)|
//������������� �������������� �������������� ��������� ������������� ���������
//(������������ ���� � H-���������) � ������������� ��������� E.
//��� ������ ������� ����������� �� �������� ���� �������� ����� ������ Arg.
//��������� ������ Arg ��� ���������� ������ E_HWDipoleFunc()
//���������� ����������: 1
//1. Arg.List[0] - �������������� (������������) ��� �������������� (���� �����)
//���� ����� �������� � �������� ������� � ������������ �� �������� �������.
//���� �������� � ��������.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. struct ARGLIST* Arg - ������ ���������� ������������ � E_HWDipoleFunc
//-----------------------------------------------------------------------------
//������������ ��������: double value
//*****************************************************************************
double E_HWDipoleFunc(const ARGLIST* Arg)
{double value;
 try
  {
   if (Arg == NULL) //����������� ���������
    throw ARG_MISMATCH;
   if (Arg->Size != 1) //������ ���� ����� ���� ��������
    throw ARG_MISMATCH;
  //���������� �������������� �������������� �������
  value = cos(Arg->List[0]);
  if (fabs(value) <= 1.0e-6) return 0.0; //Angle t = PI/2
  else value = fabs(cos(sin(Arg->List[0])*M_PI_2)/value);
  }
 catch (int error_type)
  {
   if (error_type == ARG_MISMATCH)
   {
    #if defined(CONSOLE)
      cout << "\nMISMATCH ARGUMENT ERROR !!!" << endl;
      cout << "Function ANTENNA::E_HWDipoleFunc() should accept one parameter: "
           << "angle [RADIAN] .\n";
      cout << "Mathematical notation: F(t) = |cos(sin(x)*PI/2)/cos(x)|,"
           << "t - angle." << endl;
      getch();
    #endif
    exit(-1);
   }
  }
return value;
}
//*****************************************************************************
//������: IV.2
//������������ �������: double H_HWDipoleFunc(const ARGLIST* Arg)
//����������: F(x)=1
//������������� �������������� �������������� ��������� ������������� ���������
//(������������ ���� � E-���������) � ��������� H-���������.
//��� ������ ������� ����������� �� �������� ���� �������� ����� ������ Arg.
//��������� ������ Arg ��� ���������� ������ H_HWDipoleFunc()
//���������� ����������: 1
//1. Arg.List[0] - �������������� (������������) ��� �������������� (���� �����)
//���� ����� �������� � �������� ������� � ������������ �� �������� �������.
//���� �������� � ��������.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. struct ARGLIST* Arg - ������ ���������� ������������ � E_HWDipoleFunc
//-----------------------------------------------------------------------------
//������������ ��������: double Arg.List[1]
//*****************************************************************************
double H_HWDipoleFunc(const ARGLIST* Arg)
{
 try
  {
   if (Arg == NULL) //����������� ���������
    throw ARG_MISMATCH;
   if (Arg->Size != 1) //������ ���� ����� ���� ��������
    throw ARG_MISMATCH;
  }
 catch (int error_type)
  {
   if (error_type == ARG_MISMATCH)
   {
    #if defined(CONSOLE)
      cout << "\nMISMATCH ARGUMENT ERROR !!!" << endl;
      cout << "Function ANTENNA::H_HWDipoleFunc() should accept one parameter: "
           << "angle [RADIAN] .\n";
      cout << "Mathematical notation: F(t) = 1, t - angle." << endl;
      getch();
    #endif
    exit(-1);
   }
  }
return 1.0;
}
//*****************************************************************************
//������: IV.3
//������������ �������: double EH_ParabAntFunc(const ARGLIST* Arg)
//����������: F(x)=|sin(2*PI*La*sin(x))/2*PI*La*sin(x)|
//������������� �������������� �������������� �������������� ������� � E- �
//H-���������.
//��� ������ ������� ����������� �� �������� ��� ��������� ����� ������ Arg.
//��������� ������ Arg ��� ���������� ������ E_HWDipoleFunc()
//���������� ����������: 2
//1. Arg.List[0] - �������������� (������������) ��� �������������� (���� �����)
//���� ����� �������� � �������� ������� � ������������ �� �������� �������.
//���� �������� � ��������.
//2. Arg.List[1] - ������������� ������� �������������� �������: ��������� ���-
//����� �������������� ������� � ����� �����. ������������ ��������.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. struct ARGLIST* Arg - ������ ���������� ������������ � EH_ParabAntFunc
//-----------------------------------------------------------------------------
//������������ ��������: double value
//*****************************************************************************
double EH_ParabAntFunc(const ARGLIST* Arg)
{double value;
 try
  {
   if (Arg == NULL) //����������� ���������
    throw ARG_MISMATCH;
   if (Arg->Size != 2) //������ ���� ����� ��� ���������
    throw ARG_MISMATCH;
   //���������� �������������� �������������� �������������� �������
   value = sin(Arg->List[0]);
   double La = Arg->List[1]; //������������� ������� �������������� �������
   double t;
   if (fabs(value) <= 1.0e-6) return 1.0; //Angle t = 0.0
   else { t = La*value*TWO_PI; value = fabs(sin(t)/t); }
  }
 catch (int error_type)
 {
  if (error_type == ARG_MISMATCH)
  {
   #if defined(CONSOLE)
    cout << "\nMISMATCH ARGUMENT ERROR !!!" << endl;
    cout << "Function ANTENNA::EH_ParabAntFunc() should accept two parameters:\n"
         << "1. Angle [RAD]; 2. La - Relative aerial aperture.\n";
    cout << "Mathematical notation: F(t) = |sin(2*PI*La*sin(t))/2*PI*La*sin(t)|,"
         << "t - angle." << endl;
    getch();
   #endif
   exit(-1);
  }
 }
return value;
}

//*****************************************************************************
// V. �������, ����������� �������������� ������ ������� ��������������
//    �������������� ������� � ��������� �����������
// ��� ������� � ���������� ������ ���������� ��������� �� ������ � �����������-
// ��� ������� ������� �������������� �������������� � ��������� ��������������.
// ������� ��������� � �������� ��������� ������ ���������� ������� ����������-
// ���� �������������� ������� ��������� ����.
//*****************************************************************************

//*****************************************************************************
//������: V.1
//������������ �������: char* MathNotateE_HWDipole(const ARGLIST* Arg)
//����������:
//������� ��������� �������������� ������ ������� �������������� ��������������
//������������� ��������� � E-��������� (������������ ���� � H-���������) � ����
//������.
//��������� C����� Arg ��� ���������� ������ �������
//���������� ����������: 1
//1. Arg.List[0] - �������������� (������������) ��� �������������� (���� �����)
//���� ����� �������� � �������� ������� � ������������ �� �������� �������.
//���� �������� � ��������.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. ARGLIST *Arg - ������ ���������� ������ ��������� E_HWDipole
//-----------------------------------------------------------------------------
//������������ ��������:
// char *s - ����� ��������� E_HWDipole � ���� ���������� ������
//*****************************************************************************
char* MathNotateE_HWDipole(const ARGLIST* Arg)
{
 char* s = new char[80];//������ � �������������� ������� ������
 ostrstream text(s, 80);
 try
  {
  	if (Arg == NULL)  //��������� �� ��������
    throw ARG_MISMATCH;
   if (Arg->Size != 1) //����e� ���� ������� ������ ���� ��������
    throw ARG_MISMATCH;
   text << "F(t) = |cos(sin(t)*PI/2)/cos(t)|"  << ends;
  }
 catch (int error_type)
  {
   if (error_type == ARG_MISMATCH)
   {
    #if defined(CONSOLE)
      cout << "\nMISMATCH ARGUMENT ERROR !!!" << endl;
      cout << "ANTENNA::MathNotateE_HWDipole() should accept one parameter: "
           << "t - angle.\n";
      cout << "Math. notation: F(t) = |cos(sin(t)*PI/2)/cos(t)|."
           << endl;
      getch();
    #endif
    exit(-1);
   }
  }
return s;
}
//*****************************************************************************
//������: V.2
//������������ �������: char* MathNotateH_HWDipole(const ARGLIST* Arg)
//����������:
//������� ��������� �������������� ������ ������� �������������� ��������������
//������������� ��������� � H-��������� (������������ ���� � E-���������) � ����
//������.
//��������� C����� Arg ��� ���������� ������ �������
//���������� ����������: 1
//1. Arg.List[0] - �������������� (������������) ��� �������������� (���� �����)
//���� ����� �������� � �������� ������� � ������������ �� �������� �������.
//���� �������� � ��������.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. ARGLIST *Arg - ������ ���������� ������ ��������� H_HWDipole
//-----------------------------------------------------------------------------
//������������ ��������:
// char *s - ����� ��������� H_HWDipole � ���� ���������� ������
//*****************************************************************************
char* MathNotateH_HWDipole(const ARGLIST* Arg)
{
 char* s = new char[80];//������ � �������������� ������� ������
 ostrstream text(s, 80);
 try
  {
  	if (Arg == NULL)  //��������� �� ��������
    throw ARG_MISMATCH;
   if (Arg->Size != 1) //����e� ���� ������� ������ ���� ��������
    throw ARG_MISMATCH;
   text << "F(t) = 1, 0 <= t <= 360, t - angle."  << ends;
  }
 catch (int error_type)
  {
   if (error_type == ARG_MISMATCH)
   {
    #if defined(CONSOLE)
      cout << "\nMISMATCH ARGUMENT ERROR !!!" << endl;
      cout << "ANTENNA::MathNotateH_HWDipole() should accept one parameter: "
           << "t - angle.\n";
      cout << "Math. notation: F(t) = 1."
           << endl;
      getch();
    #endif
    exit(-1);
   }
  }
return s;
}
//*****************************************************************************
//������: V.3
//������������ �������: char* MathNotateEH_ParabAnt(const ARGLIST* Arg)
//����������:
//������� ��������� �������������� ������ ������� �������������� ��������������
//�������������� ������� � ���� ������.
//��������� C����� Arg ��� ���������� ������ �������
//���������� ����������: 2
//1. Arg.List[0] - �������������� (������������) ��� �������������� (���� �����)
//���� ����� �������� � �������� ������� � ������������ �� �������� �������.
//���� �������� � ��������.
//2. Arg.List[1] - ������������� ������� �������������� �������: ��������� ���-
//����� �������������� ������� � ����� �����. ������������ ��������.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. ARGLIST *Arg - ������ ���������� ������ ��������� EH_ParabAnt
//-----------------------------------------------------------------------------
//������������ ��������:
// char *s - ����� ��������� EH_ParabAnt � ���� ���������� ������
//*****************************************************************************
char* MathNotateEH_ParabAnt(const ARGLIST* Arg)
{
 char* s = new char[80];//������ � �������������� ������� ������
 ostrstream text(s, 80);
 try
  {
  	if (Arg == NULL)  //��������� �� ��������
    throw ARG_MISMATCH;
   if (Arg->Size != 2) //������ ���� �������� ����� ��� ���������
    throw ARG_MISMATCH;
   text << "F(t) = |sin(2*PI*" << Arg->List[1] << "*sin(t))/2*PI*"
        << Arg->List[1] << "*sin(t)|" << ends;
  }
 catch (int error_type)
  {
   if (error_type == ARG_MISMATCH)
   {
    #if defined(CONSOLE)
      cout << "\nMISMATCH ARGUMENT ERROR !!!" << endl;
      cout << "ANTENNA::MathNotateEH_ParabAnt() should accept two parameters: "
           << "\nt - angle, La - relative aerial aperture.\n";
      cout << "Math. notation: F(t) = |sin(2*PI*La*sin(t))/2*PI*La*sin(t)|."
           << endl;
      getch();
    #endif
    exit(-1);
   }
  }
return s;
}

//******************************************************************
// VI. �������� �������, ���������� � �������� AntennInfo
//******************************************************************

//*****************************************************************************
//������: VI.1
//������������ �������: uint GetIdEHDirByAntType(uint AntType, bool AntPlace)
//����������:
//������� ����������  ������������� ������� �������������� ������� � E- ��� H-
//��������� � ����������� �� �������� AntPlace �� ���� ������� AntType �� �����-
//�� AntennInfo. ����� ������� �� ���� type. ���� ������������� ���� �������
//�� ������ � AntennInfo, �� ������������ EHtype = UNEXIST.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. uint AntType - ������������� ���� �������
//2. bool AntPlace - ���� ������������, ����� ������������� ������� ������������-
//�� (� E- ��� H-���������) ����������.
//-----------------------------------------------------------------------------
//������������ ��������:
//uint type - ������������� �������������� ������� � E- ��� H-���������
//*****************************************************************************
uint GetIdEHDirByAntType(uint AntType, bool AntPlace)
{ uint type = UNEXIST;
  uint Count = sizeof(AntennInfo)/sizeof(AntennInfo[0]);
  //����� �������������� �������������� ������� � E- ��� H-��������� �� �� ����
  for (uint i = 1; i < Count; i++)
  {
   if (AntennInfo[i].type == AntType)
   { if (AntPlace == E_PLANE) type = AntennInfo[i].EDirect;
     else type = AntennInfo[i].HDirect;
     break; //����� �� ����� ������
   }
  }
return type;
}
//*****************************************************************************
//������: VI.2
//������������ �������: uint GetIndexByAntType(uint AntType)
//����������:
//������� ���������� ������ ���������� ���������� �� ������� � AntennInfo �� ��
//����. ����� ������� �� ���� type. ���� ��� ������� �� ������ � AntennInfo, ��
//������������ Index = 0.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. uint AntType - ������������� ���� �������
//-----------------------------------------------------------------------------
//������������ ��������:
//uint index - ������ ���������� ���������� �� ������� � AntennInfo.
//*****************************************************************************
uint GetIndexByAntType(uint AntType)
{ uint index = 0;
  uint Count = sizeof(AntennInfo)/sizeof(AntennInfo[0]);
  //����� �������������� �������������� ������� � E- ��� H-��������� �� �� ����
  for (uint i = 1; i < Count; i++)
  {
   if (AntennInfo[i].type == AntType)
   { index = i;
     break; //����� �� ����� ������
   }
  }
return index;
}
//*****************************************************************************
//������: VI.3
//������������ �������: uint GetIdEDirByIndex(uint Index)
//����������:
//������� ���������� ������������� ������� �������������� ������� � E-���������
//�� �� ������� ������������ � ������� AntennInfo.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. uint Index - ������ ���������� ���������� �� ������� � ������� AntennInfo
//-----------------------------------------------------------------------------
//������������ ��������:
//uint IdEDirect - ������������� ������� �������������� ������� � E-���������
//*****************************************************************************
uint GetIdEDirByIndex(uint Index)
{ uint IdEDirect = UNEXIST;
  uint Count = sizeof(AntennInfo)/sizeof(AntennInfo[0]);

  if (Index < Count) IdEDirect = AntennInfo[Index].EDirect;

 return IdEDirect;
}
//*****************************************************************************
//������: VI.4
//������������ �������: uint GetIdEDirByIndex(uint Index)
//����������:
//������� ���������� ������������� ������� �������������� ������� � H-���������
//�� �� ������� ������������ � ������� AntennInfo.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. uint Index - ������ ���������� ���������� �� ������� � ������� AntennInfo
//-----------------------------------------------------------------------------
//������������ ��������:
//uint IdHDirect - ������������� ������� �������������� ������� � H-���������
//*****************************************************************************
uint GetIdHDirByIndex(uint Index)
{ uint IdHDirect = UNEXIST;
  uint Count = sizeof(AntennInfo)/sizeof(AntennInfo[0]);

  if (Index < Count) IdHDirect = AntennInfo[Index].HDirect;

 return IdHDirect;
}
//*****************************************************************************
//������: VI.5
//������������ �������: char* GetAntNameByIndex(uint Index)
//����������:
//������� ���������� ������������ ���� ������� �� �� ������� ������������
//� ������� AntennInfo.
//-----------------------------------------------------------------------------
//������������ ���������:
//1. uint Index - ������ ���������� ���������� �� ������� � ������� AntennInfo
//-----------------------------------------------------------------------------
//������������ ��������:
//char *s - ������������ ���� �������.
//*****************************************************************************
char* GetAntNameByIndex(uint Index)
{ uint Count = sizeof(AntennInfo)/sizeof(AntennInfo[0]);

  if (Index >= Count) Index = 0;

 return AntennInfo[Index].eng_name;
}
