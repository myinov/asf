//exsignal.h
//���������� ������ ExternSignal
//Copyright (c) JohnSoft 1998. All rights reserved.
//Initial date: August,18,1998. Final date:
//������������ ���� extsignal.h ��������� ����� ExternSignal, ������� ���������
//������� ������� ������, ����������� �� ���� ������� - ��������  �������. ����-
//��� ������ ����� ���� ����������� ���������� ������������  ������������ (����-
//��� ������) ��� ����� ����������� (������� ������). �������  ������ ������� ��
//����  ������������:  ���������  �������  (SIGNAL)  � �������� ����� (JAMMERS).
//��������������  �������  ������ ���������  ������� � ��������� �����. ��������
//������  ����������  �������  �����������  ���������� COMPONENT (������������).
//������ �������� COMPONENT ��������� �������� �������� ������� (external signal)
//��� �������� ����������� (external action).

#if !defined(__EXSIGNAL_H)
#define __EXSIGNAL_H

//������������ ���� �������� ���������� ������� �� ������� ����� ����������
//������������ �������� � ��� ��������
#include "mathfunc.h"

#define CONSOLE 1  //� ��������� ������������ ���������� ����-�����

const uint NONPOLAR = 0; //�� ��������� ��������������� �������� �������
const uint V_POLAR  = 1; //������������ ������� � ������������ ������������
const uint H_POLAR  = 2; //������������ ������� � �������������� ������������

class ExtSignal; //�������� ���������� ������

//*****************************************************************************
//���������� ��������� ������������� (�������) ����� ������������ (����������)
//                     �������� ������� COMPONENT
//*****************************************************************************
typedef struct
{
 /*        ����� �������������� ������������ �������� �������        */

 bool Type; //��� ���������� �������� �������: SIGNAL ��� PARASITE
 bool Usage;//������������� ���������� ��� ��������: ON ��� OFF

 //������������ ���� ������������ ��������� ���������� ������� ������������
 //������� � �������� �������� ������� (� ��������) - azimuth angle
 //��� ������ ��������� ���� �� ������� ��������� ������� �� �������
 uint AzAngleLawType;
 double (*AzimuthAngle)(const ARGLIST *Arg);//����� ��������� ������������� ����
 ARGLIST *AzAngleArg; //������ ���������� ������ ��������� ������������� ����

 //���� ����� ������������ ��������� ���������� �������� �������
 //������������ ������� � �������� �������� ������� (� ��������) -
 //angle of elevation
 //��� ������ ��������� ���� ����� ��������� ������� �� �������
 uint ElevAngleLawType;
 double (*ElevateAngle)(const ARGLIST *Arg);//����� ��������� ���� �����
 ARGLIST *ElevAngleArg; //������ ���������� ������ ��������� ���� �����

 //���� ����������� ������������ �������� ������� (� ��������) - polarization
 //angle
 //��� ������ ��������� ���� ���������������� ��������� ������� �� �������
 uint PolAngleLawType;
 double (*PolAngle)(const ARGLIST *Arg);//����� ��������� ���������������� ����
 ARGLIST *PolAngleArg;//������ ���������� ������ ��������� ���������������� ����

 /*     �������������� ��������� ������������ �������� �������     */

 //����������� ��������� ������������ �������� �������, ������������ �� �������-
 //��� ��� ������� ����������� ���� ��������� ������(��)
 uint AmplLawType; //��� ������ ��������� ������������ ���������
 double (*AmplLaw)(const ARGLIST *Arg); //����� ��������� ������������ ���������
 ARGLIST *AmplArg; //������ ���������� ������ ��������� ������������ ���������

 //��������� ����� ��������� �� ������� ��������� ������������ �������� �������
 //Casual law of amplitude modification in time
 //��� ���������� ������ ��������� ��������� ������������ �������� �������
 uint AmplCasualLawType;
 double (*AmplCasualLaw)(const ARGLIST *Arg);//��������� ����� ��������� ���������
 //������ ���������� ���������� ������ ��������� ���������
 ARGLIST *AmplCasualLawArg;

 /*     ��������������  ����  ������������  ��������  �������      */

 //������� ����������� (�������) ������� ������������ �������� ������� (rad/sec)
 //carrier pulsatance   w = 2*PI*f
 double CarPulsatance;

 //������� ����� ��������� ������������ ������������ ����������� �������
 //�� ������� (� ��������) - vertical polarization phase law
 uint VertPolPhaseLawType;//��� ������ ��������� ���� ����. ����������� �������
 //����� ��������� ���� ������������ ������������ ����������� �������
 double (*VertPolPhase)(const ARGLIST *Arg);
 //������ ���������� ������ ��������� ���� ����. ����������� �������
 ARGLIST* VertPolPhaseArg;

 //������� ����� ��������� �������������� ������������ ����������� �������
 //�� ������� (� ��������) - horizontal polarization phase law
 uint HorPolPhaseLawType;//��� ������ ��������� ���� �����. ����������� �������
 //����� ��������� ���� �������������� ������������ ����������� �������
 double (*HorPolPhase)(const ARGLIST *Arg);
 //������ ���������� ������ ��������� ���� �����. ����������� �������
 ARGLIST* HorPolPhaseArg;

 //������� ����� ��������� ������������ �������� ������� �� �������
 uint PhaseLawType; //��� ������ ��������� ���� ������� �� �������
 double (*PhaseLaw)(const ARGLIST *Arg);//����� ��������� ���� �������
 ARGLIST *PhaseArg;//������ ���������� ������ ��������� ����

 //��������� ����� ��������� �� ������� ���� ������������ �������� �������
 //Casual law of phase modification law in time
 uint PhaseCasualLawType;//��� ���������� ������ ��������� ���� ������������
 double (*PhaseCasualLaw)(const ARGLIST *Arg);//��������� ����� ��������� ����
 //������ ���������� ���������� ������ ��������� ����
 ARGLIST *PhaseCasualLawArg;
} COMPONENT;

typedef COMPONENT Comp;
//��������� �������� ����������� ������ ��������� COMPONENT
//���� TYPE
const bool SIGNAL = true; //�������� ������������ �������� �������
const bool JAMMER = false;//��������� ������������ �������� �������
//���� USAGE
//ON  = true; //���������� �������� ������� ����� ������ � ��������
//OFF = false;//���������� �������� ������� �� ����� ������ � ��������


//*****************************************************************************
// ���������� ������ - ������� ������ (EXTERNAL SIGNAL) - ExtSignal
//*****************************************************************************
class ExtSignal {
 //���������� ���������� ������-������ ������ ExtSignal
 protected:
  uint Count; //����� ���������� ���� ������������ �������� �������
  //��������� �� ������ �������� COMPONENT, ����������� ������� ������
  COMPONENT *data;

  //��� ����� � ��������� ������� �� �������� �������
  string fname;
  //��� ����� ����������� ��������� ������ � fname
  string ferrname;
  //���������� �������� ��������� ������ � ������� �� ExtSignal
  ifstream fin;
  //���������� ��������� ��������� ������ � ���������� �������� � fin
  ofstream ferr;

 //---------------------------------------------------
 //���������� �������� ������-������� ������ ExtSignal
 //---------------------------------------------------
 private:
  //�������� ������� data � ������������ �������� �������
  bool CreateArray(const uint size);
  //������������� ������������� ����� ���������� �������� �������
  void ResetProperty(uint index);
  //���������� ������� ����� � ���������� ��������� ������������� ���
  //����� ������������ �������� �������
  void AzimuthAngleInPut(uint index,uint language);  //1.
  void ElevateAngleInPut(uint index,uint language);  //2.
  void PolAngleInPut(uint index,uint language);      //3.
  void AmplLawInPut(uint index,uint language);       //4.
  void AmplCasualLawInPut(uint index,uint language); //5.
  void VertPhasePolInPut(uint index,uint language);  //6.
  void HorPhasePolInPut(uint index,uint language);   //7.
  void PhaseLawInPut(uint index,uint language);      //8.
  void PhaseCasualLawInPut(uint index,uint language);//9.
  void CarPulsatanceInPut(uint index,uint language); //10.
  //���� ������������� �� ���� �� ��������� �������� ������� � ���������
  void ConsoleDataEntry(uint index);

  //�������������� ���� ���������� ������ �� �������� ����������� � ��������
  //� ��������
  ARGLIST* InvertAllLawParam(const LAW* Law, ARGLIST* Arg);
  ARGLIST* ConvertAllLawParam(const LAW* Law, ARGLIST* Arg);

  //���� ������������� (�������������) ���� ��������� �������� ������� ��
  //����� ������
  bool FileDataEntry(const char* filename);
  bool GetInPutFilename();
  bool GetNoiseCountFromFile(ulong& CurRow);
  bool GetCompSignalFromFile(uint index, ulong& CurRow);
  bool GetPropertySignalFromFile(string& s ,uint CompSignal, uint PropSignal,
       ulong& CurRow);
  bool GetArgLawFromFile(string& s, LAW* PropLaw, ARGLIST* Arg, uint CompSignal,
       uint PropSignal, ulong& CurRow);

  //�������� ������� ������� ��������� �� ������� ������������� ����, ����
  //����� ���������, � ����� ��������� � ���� (�������������� � ������������
  //������������) ����������� ������ ���������� �������� �������
  void CreateTableOfAngleLaw();
  //�������� ������� ����������������� ������� ��������� �� ������� �����������
  //������������ ���������� �������� �������
  void CreateTableOfAmplLaw();
  //�������� ������� ����������������� ������� ��������� �� ������� �������
  //������������ ���������� �������� �������
  void CreateTableOfPhaseLaw();
  //�������� ������� ��������� ������� ��������� ����������� � ������� �������-
  //������� ���������� �������� �������
  void CreateTableOfCasualLaw();
  //�������� ������� �������
  void DeleteTable(LAW* Table, uint& Size);

  //���������� ��������� ������� ��� ������ � ��������� �������
  uint AccessibleLaw(const LAW* Table, uint Count);
  bool SetLaw(LAW* Table, uint Count, uint LawType);
  bool ResetLaw(LAW* Table, uint Count, uint LawType);
  bool SetAllLaw(LAW* Table, uint Count);
  bool ResetAllLaw(LAW* Table, uint Count);
  uint GetIndexLaw(const LAW* Table, uint Count, uint Select);
  uint GetIndexLawByType(const LAW* Table, uint Count, uint Type);

 //---------------------------------------------------------
 //���������� ������������� ������-������� ������ ExtSignal
 //---------------------------------------------------------
 public:
  //����������� ������������� ���������� �������� ������� � ������ Console
  //� ������������ ������������� ���������� �������������
  bool ConsoleDataOutPut(uint index, bool dim = TDIM, bool confirm = false);

 //**********  ���������� ������������� � ����������� ������ rmatrix  **********
 //���������� ������������ �� ���������
 ExtSignal();
 //���������� ������������ � �����������
 ExtSignal(COMPONENT *ptr, uint Count);
 ExtSignal(const char* filename);
 //���������� ������������ �����������
 ExtSignal(ExtSignal& s);
 //���������� ����������� ������
 ~ExtSignal();

 //***************************************************************************
 //III. ���������� ���������� �������� ������� � ��� ���������
 //���������� ������-������� ������ ExtSignal �� ���������� ���������� ������-
 //������ �������� �������: ���������� �������� �������� � ���, �����������
 //���������� � ������ � ��� ����� ��������������� ������� ��������� ��������
 //�������, ��������� ��������� �������, ���������� �������, ��������� ������ �
 //�.�.
 //***************************************************************************
 public:
  void SetTime(double time); //��������� �������� �������� ���������� �������
  //��������� ��������� �������� ��������� � ���� ��� ���� �������� ExtSignal
  void SetRandNumber();
  //���������� �������� ����� �� ������� �������
  uint Jammers() const {return (Count < 2) ? 0 : Count-1;};
  //������� ���������� ����������� �������� ����� � Usage == ON
  uint ActiveJammers() const;
  //����������� ���������� i-�� ���������� �������� �������
  bool Usage(uint Number)
  {if (Number >= Count) return OFF;
   return data[Number].Usage;
  };
  //��������� �������� ���� ������� ���������� �������� ������� (� ��������)
  double AzimuthAngle(uint Number) const
   {if (Number >= Count) return 0.0;
    return data[Number].AzimuthAngle(data[Number].AzAngleArg);
   };
  //��������� �������� ���� ����� ���������� �������� ������� (� ��������)
  double ElevateAngle(uint Number) const
   {if (Number >= Count) return 0.0;
    return data[Number].ElevateAngle(data[Number].ElevAngleArg);
   };
  //��������� �������� ���� ����������� ���������� �������� ������� (� ��������)
  double PolarAngle(uint Number) const
   {if (Number >= Count) return 0.0;
    return data[Number].PolAngle(data[Number].PolAngleArg);
   };
  //��������� ������������ ������������ ���� ����������� ���������� ��������
  //������� (� ��������)
  double VCPPhase(uint Number) const
   {if (Number >= Count) return 0.0;
    return data[Number].VertPolPhase(data[Number].VertPolPhaseArg);
   };
  //��������� �������������� ������������ ���� ����������� ���������� ��������
  //������� (� ��������)
  double HCPPhase(uint Number) const
   {if (Number >= Count) return 0.0;
    return data[Number].HorPolPhase(data[Number].HorPolPhaseArg);
   };
  //��������� ���������� �������� �������� ��������� �������� �������
  double AmplSignalComp(uint Number, uint PolarCompType = NONPOLAR) const;
  double AmplSignal(uint PolarCompType = NONPOLAR) const
         {return AmplSignalComp(0,PolarCompType);};
  double VertAmplSignal() const {return AmplSignalComp(0,V_POLAR);};
  double HorAmplSignal() const {return AmplSignalComp(0,H_POLAR);};
  double AmplJammer(uint Number, uint PolarCompType = NONPOLAR) const
         {return AmplSignalComp(Number,PolarCompType);};
  double VertAmplJammer(uint Number) const
         {return AmplJammer(Number,V_POLAR);};
  double HorAmplJammer(uint Number) const
         {return AmplJammer(Number,H_POLAR);};
  //��������� ���������� �������� ��� ��������� �������� �������
  double PhaseSignalComp(uint Number, uint PolarCompType = NONPOLAR) const;
  double PhaseSignal(uint PolarCompType = NONPOLAR) const
         {return PhaseSignalComp(0,PolarCompType);};
  double VertPolPhaseSignal() const {return PhaseSignalComp(0,V_POLAR);};
  double HorPolPhaseSignal() const {return PhaseSignalComp(0,H_POLAR);};
  double PhaseJammer(uint Number, uint PolarCompType = NONPOLAR) const
         {return PhaseSignalComp(Number,PolarCompType);};
  double VPolPhaseJammer(uint Number) const
         {return PhaseJammer(Number,V_POLAR);};
  double HPolPhaseJammer(uint Number) const
         {return PhaseJammer(Number,H_POLAR);};
  //��������� ���������� �������� �������� � ��� ��������� �������� ������� ���
  //��������� ������������.
  double DetAmpl(uint Number, uint PolarCompType = NONPOLAR) const;
  double DetPhase(uint Number, uint PolarCompType = NONPOLAR) const;
  //���������� ����������� �������� ���������� ��������� �������� �������
  complex CalcUSignalComp(uint Number, uint PolarCompType = NONPOLAR)
          { return polar(AmplSignalComp(Number,PolarCompType),
                         PhaseSignalComp(Number, PolarCompType));};
  //������ ����������� �������� ���������� ��� ��������� �������
  complex CalcUSignal(uint PolarCompType)
          { return polar(AmplSignalComp(0,PolarCompType),
                         PhaseSignalComp(0, PolarCompType));};
  complex CalcVertPolUSignal() {return polar(AmplSignalComp(0,V_POLAR),
                                             PhaseSignalComp(0, V_POLAR));};
  complex CalcHorPolUSignal()  {return polar(AmplSignalComp(0,H_POLAR),
                                             PhaseSignalComp(0, H_POLAR));};
  complex CalcUSignal() {return CalcUSignal(V_POLAR)+CalcUSignal(H_POLAR);};
 //������ ����������� �������� ���������� ��� �����
 complex CalcUJammer(uint Number, uint PolarCompType)
          { return polar(AmplSignalComp(Number, PolarCompType),
                         PhaseSignalComp(Number, PolarCompType));};
  complex CalcVertPolUJammer(uint Number)
          { return polar(AmplSignalComp(Number,V_POLAR),
                         PhaseSignalComp(Number, V_POLAR));};
  complex CalcHorPolUJammer(uint Number)
          { return polar(AmplSignalComp(Number,H_POLAR),
                         PhaseSignalComp(Number, H_POLAR));};
  complex CalcUJammer(uint Number)
          {return CalcUJammer(Number, V_POLAR)+CalcUJammer(Number, H_POLAR);};
  //������ ����������� �������� ���������� ��� ��������� ������
  complex CalcSumUJammer(uint PolarCompType);
  complex CalcVertPolSumUJammer() {return CalcSumUJammer(V_POLAR);};
  complex CalcHorPolSumUJammer()  {return CalcSumUJammer(H_POLAR);};
  complex CalcSumUJammer() {return CalcSumUJammer(V_POLAR) +
                                   CalcSumUJammer(H_POLAR);};
  //���������� ����������� �������� �������� ��������� �������� �������
  double CalcPSignalComp(uint Number, uint PolarCompType = NONPOLAR)
          { return norm(polar(AmplSignalComp(Number,PolarCompType),
                              PhaseSignalComp(Number, PolarCompType)));};
  double CalcPSignalComp(complex UVertPol, complex UHorPol)
          { return norm(UVertPol+UHorPol);};
  double CalcPSignalComp(complex USignalComp) {return norm(USignalComp);};
  //������ ����������� �������� �������� ��� ��������� �������
  double CalcPSignal(uint PolarCompType)
          { return norm(polar(AmplSignalComp(0,PolarCompType),
                              PhaseSignalComp(0, PolarCompType)));};
  double CalcVertPolPSignal() {return norm(polar(AmplSignalComp(0,V_POLAR),
                                             PhaseSignalComp(0, V_POLAR)));};
  double CalcHorPolPSignal() {return norm(polar(AmplSignalComp(0,H_POLAR),
                                             PhaseSignalComp(0, H_POLAR)));};
  double CalcPSignal() {return norm(CalcUSignal());};
  //������ ����������� �������� �������� ��� ������
  double CalcPJammer(uint Number, uint PolarCompType)
          {return norm(polar(AmplSignalComp(Number,PolarCompType),
                             PhaseSignalComp(Number, PolarCompType)));};
  double CalcVertPolPJammer(uint Number)
        {return norm(polar(AmplSignalComp(Number,V_POLAR),
                           PhaseSignalComp(Number, V_POLAR)));};
  double CalcHorPolPJammer(uint Number)
         {return norm(polar(AmplSignalComp(Number,H_POLAR),
                            PhaseSignalComp(Number, H_POLAR)));};
  double CalcPJammer(uint Number) {return norm(CalcUJammer(Number));};
  //������ ����������� �������� �������� ��� ��������� ������
  double CalcSumPJammer(uint PolarCompType);
  double CalcVertPolSumPJammer() {return norm(CalcSumUJammer(V_POLAR));};
  double CalcHorPolSumPJammer() {return norm(CalcSumUJammer(H_POLAR));};
  double CalcSumPJammer()
         {return norm(CalcSumUJammer(V_POLAR)+CalcSumUJammer(H_POLAR));};
  //������ ��������� ������-������ �� ���������� ��������� ��������� ���������
  //������� � ���������� ������: SJR = 10*lg(Ps/SUM(Pj)) � ���������
  double SignalJammerRatio();
};
//���������� ���������� ������ ExtSignal - ������� ������

#endif //���������� ���������� ������������� ����� "EXSIGNAL.H"
