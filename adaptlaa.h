//AdaptLAA.h
//Copyright (c) JohnSoft 1999. All rights reserved. Borland C++ 5.01.
//Initial date: Octomber,1,1999. Final date:
//    ���������� ������ TSAdaptLAA - ��������������  ������  �������� ����������
//�������� ������� � ����������������  ���������� �������� � ������ ������������
//��������� ������.
//
#if !defined(__ADAPTLAA_H)
#define __ADAPTLAA_H
//******************************************************************************
// ���������� ������������ ������
//******************************************************************************
#include "const.h"
//#pragma hdrstop

#include "input.h" //������� �����������
#include "mathlib.h" //���������� �������������� �������
#include "weightsum.h" //������� ���������
#include "RandGen.h" //��������� ��������� �����


const bool STATISTICAL = true;
const bool ANALYTICAL  = false;
const bool STAT   = true;
const bool ANALYT = false;

//���� ���������� ������������ ������� ������������ �����
const uint ABSENT = 0;//���������� �����������
const uint LOCAL  = 1;//���������� �������� �������� ��� ������� ���� ��������
const uint GLOBAL = 2;//����� ���������� �������� �������� ��� ���� �����

//******************************************************************************
//��������������� ���������� ���� �������
//******************************************************************************
class TAnalInSignalProperty;
class TInDataBaseCh;
class TBaseChannel;
class TPreProcessorSpaceFilterBaseCh;
class TAnalPreProcessorSpaceFilterBaseCh;
class TStatPreProcSFilterBaseCh;
class TASpaceFilterBaseCh;
class TSpaceFilterBaseCh;

class TAuxChSpaceRejectFilter;
class TMultiStageAuxChSRF;
class TSingleStageAuxChSRF;

class TMultiBeamFormer;
class TAMultiChSpaceFilter;
class TMultiChSpaceFilter;


//****************************************************************************************
//   ���������� ������ - TAnalInSignalProperty - �������� ������������� ������ ��������
//                ������� ������������ ���������� �������� ��������
//������ ����� ������ �������� "��������������" � ��������������� ������������
//����������� ���������������� ���������� �������� ��� ��������� �������� �����-
//��: ��������� �������, ���������� �������� ������ � ����������� ����.
//���� �����-���� ���������� �������� ����������� �������� ������������ ��  ���-
//����, �� �������������� �� �������������� ������� ����� �������� LAA, ������-
//������� � ����������������� ����������� �������, ����������� ���� ��� � ������
//������� t = 0, ��� ��������� ����� ������ ������������� ������ �������.
//� �������������� ������ �������������� ������� ��� ���������� �������� �������
//������ ������������� �� ������ ���� �������������. � ����������� ������ ����-
//������ ������ � ���������� ��� �������� �������������, �������� ������ �����
//������ ��������������.
//******************************************************************************
class TAnalInSignalProperty {
 //-----------------------------------------------------------------------------
 // ���������� ���������� ������-������ ������ TAnalInSignalProperty
 //-----------------------------------------------------------------------------
 protected:
  bool SignalStat;//������� "��������������" ��������� �������
  bool JammerStat;//������� "��������������" ���������� ������
  bool NoiseStat;//������� "��������������" ����������� ����
 //-----------------------------------------------------------------------------
 // I. ���������� ������������� ������-������� ������ TAnalInSignalProperty
 //-----------------------------------------------------------------------------
 public:
 //*****************************************************************************
 // ���������� ������������� � ����������� ������ TAnalInSignalProperty
 //*****************************************************************************
 TAnalInSignalProperty();//���������� ������������ �� ���������
 //���������� ������������ � �����������
 TAnalInSignalProperty(bool lSignal, bool lJammer, bool lNoise);
 //���������� ������������ �����������
 TAnalInSignalProperty(const TAnalInSignalProperty& Property) {*this = Property;};
 //���������� ����������� ������
 ~TAnalInSignalProperty();
 //*****************************************************************************
 // ������������� ������� ��� ������ � ������� TAnalInSignalProperty
 //*****************************************************************************
 //��������� ������� ������������� ������ �������� �������
 void Set(bool lSignal, bool lJammer, bool lNoise);
 //��������� ������� ������ � ���� ���������� ������� string
 string& Properties(string &s, string& title, uint indent = 0) const;
 bool GetSignalStat() const {return SignalStat;};
 bool GetJammerStat() const {return JammerStat;};
 bool GetNoiseStat() const {return NoiseStat;};
 //*****************************************************************************
 // ������������� ������� ������ TAnalInSignalProperty
 // 1. ������� ����� - ������ ����������
 //*****************************************************************************
 //���� ���������� ������� �� ������������ ������ ����� cin
 void Edit();
 //���������� ��������� ����� (>>) ��� ����� ���������� ������
 friend istream& operator >>(istream& in, TAnalInSignalProperty& InSignal);
 //���������� ��������� ������ (<<) ��� ������ ���������� ������
 friend ostream& operator <<(ostream& out, const TAnalInSignalProperty& InSignal)
 {string s; return out << InSignal.Properties(s,STR_NULL);};
 //���������� �������������� ��������� ������������ operator =
 void operator =(const TAnalInSignalProperty& InSignal);
 //-----------------------------------------------------------------------------
 // II. ���������� ���������� ������-������� ������ TAnalInSignalProperty
 //-----------------------------------------------------------------------------
 protected:
  void Reset(); //��������� ��������� ���� ������������� TAnalInSignalProperty
};
//����� ���������� ������ TAnalInSignalProperty
//The end of class TAnalInSignalProperty declaration

//******************************************************************************
//       ���������� ������� ������������ ������������ ���������� LAA
// � �������������� ������, ������������ � ������ ����������� �����������������
//                          ��������������� �������
//******************************************************************************
//------------------------------------------------------------------------------
//     ������������ �������������� ����� ���������� LAA - TMultiBeamFormer
//------------------------------------------------------------------------------
//   ������������ ����� ����������� ������� TMultiBeamFormer. ����� ���������
//����������� m ����� � ������������ ���������������� ���������� -90 �� +90 ���-
//����� �� n ������� LAA, ������������ � ������������ �����. � ����� ����� ����
//���������� �� 1 �� ���� ������� LAA. ��� ������������ � ������������ ����� ��-
//���� LAA ��������� � ����������� ���� �������� ����������� ������������ �����.
//��� ���� ����� ������ ���������������� ���������. ������ ����� ������������
//����������� ������� LAA, ������������ � MultiBeamFormer � �� ����������������
//����������. ������ ������� LAA, ������������ � MultiBeamFormer, ������������.
//   ������������ ����� ����������� �� m ������������ ������� ���������� (�� ��-
//�������� �����). �  ��������  ����� ��� k-�� ��������� (1 <= k <= m) ���������
//��������������  ������������ ��� ������� ������ LAA ������������� � MultiBeam-
//Former, ������� ��������� �������� ���� ������������. ����� ������������ ����-
//������� ���������� �������� ������� LAA, ������������ � MultiBeamFormer, ��
//��������������� ������� ������ �������������� ������������ ��� k-�� ���������,
//����������  ������������ ����������� �� ����� ���������, �������� �������� ��-
//������ ���������� ����������� �������� ���������� ���������� ������, ���������
//������� � ����������� ���� k-�� ����.
//  ����� �������, ��� ������������ m ����� � �������������� n ������� LAA, ���-
//�������� �������������� ������� B ������������ n x m. � ������ ������� �����-
//�� B, �������� �������������� ������������ ��� ������������ ������ �� �����.
//   ������������ PHI(i,j) ���������� ������� ����� � i-�� ������ LAA, ���������-
//��� � BFC, ��� ������������ j-�� ���� �� ������� ��� �� ���� ����� LAA. ����-
//���������� ����������� ����������� �� ��������� �������:
// PHI(i,j) = 2*PI*LV[i]*sin(Beam[j]), ���
// Beam[j] - ����������� ������������ j-�� ���� ������������ ����� (1<=j<= m);
// LV[i] - ������������� ���������� ����� 0 � i-� ������� �������� �������, ���-
// ��������� � ������������ ����� (1<=i<=N).
//   ��������������  �������  B ��� m ����� �� n ������� LAA, ������������ � ���
//����� ��������� ��������� (n = 7, m = 3):
//Beam:   1   2   3    Ch
//     | b11 b12 b13 | 1
//     | b21 b22 b23 | 2  bij - �������������� ��������� ��� ������������ j-��
//     | b31 b32 b33 | 3        ���� i-�� ������� LAA, ������������ � ��������-
// B = | b41 b42 b43 | 4        ������ �����.
//     | b51 b52 b53 | 5
//     | b61 b62 b63 | 6
//     | b71 b72 b73 | 7
//
//------------------------------------------------------------------------------
//******************************************************************************
//���������� ������ - TMultiBeamFormer - ������������ �������������� ����� LAA
//******************************************************************************
class TMultiBeamFormer {
 //-----------------------------------------------------------------------------
 // ���������� ���������� ������-������ ������ TMultiBeamFormer
 //-----------------------------------------------------------------------------
 protected:
  TInputSignal* pLAA;//��������� �� LAA, � ������� ������������ MultiBeamFormer
  ivector Channel;//������ ������� LAA, ������������ � MultiBeamFormer
  bool lUsedAllCh;//������� ����������� ���� ������� LAA � MultiBeamFormer
  rvector Beam;//���������������� ��������� �������� ��������� ����� (� ��������)
  cmatrix B;//�������������� �������
 //-----------------------------------------------------------------------------
 // I. ���������� ������������� ������-������� ������ TMultiBeamFormer
 //-----------------------------------------------------------------------------
 public:
 //*****************************************************************************
 // ���������� ������������� � ����������� ������ TMultiBeamFormer
 //*****************************************************************************
 //���������� ������������ �� ���������
 TMultiBeamFormer() {_Reset();};
 //���������� ������������� � �����������
 TMultiBeamFormer(TInputSignal* InSignal, const ivector& Index = ivector(),
                  const rvector& Angle = rvector())
  {Set(InSignal,Index,Angle);};
 TMultiBeamFormer(TInputSignal* InSignal, const ivector& Index)
  {rvector Angle; Set(InSignal,Index,Angle);};
 TMultiBeamFormer(TInputSignal* InSignal, const rvector& Angle)
  {ivector Index; Set(InSignal,Index,Angle);};
 //���������� ������������ �����������
 TMultiBeamFormer(const TMultiBeamFormer& MultiBeam) {*this = MultiBeam;};
 //���������� ����������� ������
 ~TMultiBeamFormer() {_Reset();};
 //*****************************************************************************
 // ������������� ������� ��� ������ � ������� TMultiBeamFormer
 //*****************************************************************************
 //-----------------------------------------------------------------------------
 //��������� ������� ������
 //-----------------------------------------------------------------------------
 bool Set(TInputSignal* InSignal, const ivector& Index = ivector(),
          const rvector& Angle = rvector());
 bool SetLAACh(const ivector& Index = ivector());
 bool SetBeams(const rvector& Angle = rvector());
 void Set(const TMultiBeamFormer& MultiBeam) {*this = MultiBeam;};
 //-----------------------------------------------------------------------------
 //��������� ������������� (�������) ������
 //-----------------------------------------------------------------------------
 //�������� ����������������� ������������ �����
 bool IsReady() const {return (pLAA != NULL) ? true : false;};
 //��������� ��������� �� LAA
 TInputSignal* GetLAAPtr() const {return pLAA;};
 //������������ LAA ������������ ����������� �����
 bool DispLAA() const {return (IsReady()) ? pLAA->GetDisp() : EQUATOR;};
 //���������� ������� LAA, ������������ � MultiBeamFormer
 ulong GetLAACh() const {return B.Rows();};
 //���������� ����� (����������� ������������) MultiBeamFormer
 ulong GetBeams() const {return B.Cols();};
 bool GetLAACh(ivector& Index) const;
 bool GetBeams(rvector& Angle) const;
 bool IsConnectedAllLAACh() const {return lUsedAllCh;}
 //��������� ������������� �������������� �������
 bool GetBeamFactors(cmatrix& A) const {A = B; return (IsReady()) ? true : false;};
 //��������� ������� ������ � ���� ���������� ������� string
 string& Properties(string& s, string& title, ulong MaxCh = 0L, ulong MaxBeam = 0L,
                    uint indent = 0) const;
 string& GetLAACh(string& s, ulong max = 0L, uint indent = 0) const;
 string& GetBeams(string& s, ulong max = 0L, uint indent = 0) const;
 //-----------------------------------------------------------------------------
 //������������ ������� MultiBeamFormer (��������� ��������������)
 //���������� �������� ��������������� ������������ ����� � �������� ������� �
 //��������� ������������ LAA ������������ ����������� ����� (�� ������� ���
 //���� �����)
 bool TFunc(rmatrix& P, double InitAngle = -90, double EndAngle = 90, double Step = 0.1,
            uint NormType = GLOBAL, bool dB = ON, double MindB = -60.0) const;
 bool TFunc(rmatrix& P, const TEvenCloseInterval& Sector, uint NormType = GLOBAL,
            bool dB = ON, double MindB = -60.0) const;
 bool TFunc(rmatrix& P, const rvector& Angle, uint NormType = GLOBAL,
            bool dB = ON, double MindB = -60.0) const;
 //-----------------------------------------------------------------------------
 //���������� ��������� �������� ��������������� ������������ ����� � ��������
 //������� � ��������� ������������ LAA ������������ ����������� �����
 //(�� ������� ��� ���� �����)
 bool Envelope(rvector& P, const TEvenCloseInterval& Sector, bool Norm = ON,
               bool dB = ON, double MindB = -60.0) const;
 //���������� ������ ����� MultiBeamFormer �� ��������� ������ �������� � ��������
 //bool BeamWidth(rvector& Result, double PowerLevel = 0.5, double Prec = 0.01);
 //*****************************************************************************
 // ������������� ������� ������� �������� �������� TMultiBeamFormer
 //*****************************************************************************
 bool TFunc(const cvector& In, cvector& Out) const;
 bool TFunc(const cmatrix& In, cvector& Out) const;
 bool TFunc(const cvector& In, rvector& P) const;
 bool TFunc(const cmatrix& In, rvector& P) const;

 //OVERLOAD THE CALL FUNCTION OPERATOR ()
 //���������� ��������� ������ ������� ()
 bool operator() (const cvector& In, cvector& Out) const {return TFunc(In,Out);};
 bool operator() (const cmatrix& In, cvector& Out) const {return TFunc(In,Out);};
 bool operator() (const cvector& In, rvector& P) const {return TFunc(In,P);};
 bool operator() (const cmatrix& In, rvector& P) const {return TFunc(In,P);};

 //������ ��������� �������� ����������� �� ������ MultiBeamFormer ���
 //����������/��������������� ������������� (MultiBeamFormer ���������� � ������ LAA)
 bool Signal(cvector& Us, bool PolarType) const;
 bool Noise(cvector& Un, bool PolarType) const;
 bool Jammers(cvector& Ujam, bool PolarType) const;
 bool Jammers(cmatrix& Ujam, bool PolarType) const;

 bool Psignal(rvector& P, bool PolarType) const;
 bool Pnoise(rvector& P, bool PolarType) const;
 bool Pjams(rvector& P, bool PolarType) const;
 bool Pjams(rmatrix& P, bool PolarType) const;

 //*****************************************************************************
 // ������������� ������� ����� - ������ ���������� TMultiBeamFormer
 //*****************************************************************************
 //���� ���������� ������� �� ������������ ������ ����� cin
 bool Edit();
 //���� �������� ������� LAA, ������������ � MultiBeamFormer �� ������ ����� cin
 bool EditLAACh(bool Confirm = ON);
 //���� ����������� ������������ MultiBeamFormer �� ������ ����� cin
 bool EditBeams(bool Confirm = ON);

 //���������� ��������� ������ (<<) ��� ������ ���������� ������
 friend ostream& operator <<(ostream& out, const TMultiBeamFormer& Obj)
 {string s; return out << Obj.Properties(s,STR_NULL);};
 //���������� �������������� ��������� ������������ operator =
 void operator =(const TMultiBeamFormer& MultiBeam);
 //-----------------------------------------------------------------------------
 // II. ���������� ���������� ������-������� ������ TMultiBeamFormer
 //-----------------------------------------------------------------------------
 protected:
  void _Reset(); //��������� ��������� ���� ������������� ������
  //��������� �������� ������� LAA, ������������ � MultiBeamFormer
  void _SetChannel(const ivector& Index);
  //��������� ����������� ������������ MultiBeamFormer
  void _SetBeam(const rvector& Angle);
  //������ ������������� �������������� ����� - MultiBeamFormer - ������� B
  void _CalcMultiBeamFactors();
};
//����� ���������� ������ TMultiBeamFormer
//The end of class TMultiBeamFormer declaration

//****************************************************************************************
//                   ���������� ������ TAMultiChSpaceFilter
//��������� ������ ���������� LAA � ���������������� ���������� N-���������
//�������� � ������������ �������������� ������. ������������ ���������� LAA:
//1. LAA - �������� �������� �������;
//2. ���������� ���������������� ������ � N ������� � N ��������;
//3. ����� ���������� ������� - ������������ �������������� �����, ����������� �
//   ������� N-���������� ����������� ����������������� �������.
//����� ��������� �������� ������� TMultiBeamFormer � TAnalInSignalProperty.
//����� ��������� ����������� �� ����� ����������������� �������, �������������
//� LAA, � ���� �������������� ������ ��������� ������� Rss, ���������� ��������
//������ Rjj, ������������ ���� Rnn � ��������� ������� R ��������� �������� ���-
//����. ������� R ������������ ��� ��������� ������� ������������� �������������-
//���� ����������� ������� ������������ N x N. ����������� ������� N ������������
//������������� � �� ����� ��������� ���������� ������� LAA. ������ � �����������
//����������� � ������ TMultiBeamFormer. � ���� ������ ������������ ���������� �
//������ ������� LAA, ������������ � ����������������� �������, � ����� ��������
//��������� ������������ �����: ���������� ����������� ����� � �� �������������-
//��� ��������� (�� ������� ��� ���� �����) � ����������� �� ������������ LAA
//������������ ����������� �����. ����� �������, ����� TMultiBeamFormer ������
//������������ ������������ ������� LAA. �� ���������� ������������ ������� �����
//�������� ����������� ����������� ����������������� ������� � ������ �����, ���-
//�������� �������������� ������.
//����� TAnalInSignalProperty ������ �������� "��������������" ��������� �������,
//���������� �������� ������ � ����������� ����. ��� ������������ �� ������� ���-
//������� �������� ����������� �� �������������� ������� ����������� ������ ��
//������ ���� ������������� (T = 0), ��� ��������� �������� �������������. ���
//�������������� �� ������� ���������� �� �������������� ������� ����������� ��
//������ ���� �������������. ��� �������, ���������� �������� ������ � �������-
//��� ��� ����������� �� �������, � �������� ������ - �������������. ������������
//����� ������� ������������� ������ ��� ������� ����� ��� ������ ����������.
//��� ���������� ������������ ������� ���������� LAA ����� �� ��������� ������-
//������ ������ �������� ����������� (��� ��������� ������). ����� ��������� ��-
//������� ������������ ������� ����������������� ������� � ������������ ������-
//�������� ����� �� �����������, ������������ �������� �� ����� � ������ �������-
//��� LAA ��� �������, �������� �������� ������� �������������, ��������������
//������� ��������� �������� ������� ��� �� ����� � �.�.
//���������. ��� ������� �������� �� ����� � ������ ����������������� �������
//������������ ������ ��������� � ������ �� ������� LAA, ������������ ����������
//LAA. ��� ���� �������� ������� � ������/������� ������ ������� ��� ����� �����-
//������� �����. ��� ������� ��������� ��������� �������� ����������� �� ����� �
//������ ���������� LAA ������������ ������ �������� ��� ������� ���� ����������-
//�� MultiBeamFormer. ��� ���� �������� �������� ������� � ������ ���������� LAA,
//�.�. � ����� ������������ �����. ������� �������� ���������� LAA, ��������������
//����� �� ������ MultiBeamFormer, �� ��� ����� ����������� ������� ����� �����-
//����������� ���������� ������. ��� ������ ����������� ������������� ���� �����-
//�� ����������� ������ ��������� �� ����� � ������ ���������� LAA, � ��� ������
//������ ������ ������� - �������� �� ����� � ������ ��������������� �������.
//��� ������������� ������ ������ ������� � ����� �������� ��������� �� �������
//����� �� ������������� ����� "weightsum.h" - TMultiInMultiOutWeightSummator.
//��� ���� ����������� ������������ ������������ � �������� �����������������
//������� ������������ N x N ������ ��������� ������ ������� ���������� � N ���-
//���� � N ��������. ������ ������� ��������� ����� ���������� ���� �� ����� ��
//������������ (��������������� � ����������������), �� ���������� ��������� ��-
//����� ������������� (���������� ���������), �� ������ ������������ ������������
//���������� �������� (������� ��� ����������, ���� ����� ������������). ��� ���
//������� ��������� ������ ���������� ������ ����: ��� ������ ���� � N ������� �
//N ��������. ����������� ��������� ������������ ������������� ��� ������ ��� ����
//����������� ������� LAA, ������������ � MultiBeamFormer.
//����� ���������� ������ ���� ����������� ����������� � ������������� ����������
//LAA ��� �� �� ���������� ���������� (��������� �������� ������� ������������� �
//����������� �� �������������� ��������� ��������), ��� � �� �� ���������, ����-
//������ ��������� ������� ������������� � ������������ �� ������������ �������-
//���. ��������, ��� ������ ����������������� ������� ����������� � ������������
//�����  ��������� �������������� (������������ �������) � ����������� �� �����-
//���� �����. ����������� ������� ���������� ������������ ����� �������� (�������)
//�����, ������� ����� ���� ��������� ���������� LAA. ���������, ��� ��������
//������� ������������� � ������� ���� (��������) LAA.
//������������� ��������� ������ ���������� LAA � ���������������� �������� N x N
//� ������������ ������ ������������ �� ���� ����������. �  ������ �������������
//������� ������������ ����������� ������� ����������� � ��  ����������  �������
//����������� ��� ���������. �� ��������� ������������� ����� ���������  ������-
//������ ��������������  ���������� LAA, ������� ����� �������� �� ������ �����-
//���: �� �������� �����������, ������������ ���������� ������� � �������������-
//���� �������, ���������  ��������� � ���� ������������ �������, ���������� ��-
//�������� ������� � �.�.
//****************************************************************************************
class TAMultiChSpaceFilter:
 public TMultiBeamFormer, public TAnalInSignalProperty {
//----------------------------------------------------------------------------------------
// ���������� ���������� ������-������ ������ TAMultiChSpaceFilter
//----------------------------------------------------------------------------------------
protected:
 //��������������� ������������ �������, ������������ ��������
 bool Polar;
 //��������� �� ������� ����������� ����� �������� ��������� N x N
 TMultiInMultiOutWeightSummator* Filter;//���������������� ���������� ������
 //�������������� ������� ��������� �������, ���������� ������ � �����������
 //���� �� ����� �������
 cmatrix Rss;//�������������� ������� ��������� �������
 cmatrix Rjj;//�������������� ������� ���������� ������
 cmatrix Rnn;//�������������� ������� ����������� ����
 //��������� ������� ��������� �������� ����������� �� ����� �����������������
 //����������� �������: Rss+Rjj+Rnn
 cmatrix R;
//------------------------------------------------------------------------------
// I. ���������� ������������� ������-������� ������ TAMultiChSpaceFilter
//------------------------------------------------------------------------------
public:
//******************************************************************************
// ���������� ������������� � ����������� ������ TAMultiChSpaceFilter
//******************************************************************************
//���������� ������������ �� ���������
TAMultiChSpaceFilter() : TMultiBeamFormer(), TAnalInSignalProperty()
{Filter = NULL; _Reset();};
//���������� ������������ � �����������
TAMultiChSpaceFilter(TInputSignal* pLAA, const ivector& LAACh = ivector(),
 const rvector& Beams = rvector(), uint SF_Type = MS_SDM_MIMO_WSum,
 rvector& SF_Param = rvector(), bool PolarType = VP_SIGNAL) :
 TMultiBeamFormer(pLAA,LAACh,Beams), TAnalInSignalProperty()
 {Filter = NULL; Polar = VP_SIGNAL;
  if (IsBeamFormerReady()) SetSFilter(SF_Type,SF_Param);
 };
//���������� ������������ �����������
TAMultiChSpaceFilter(TAMultiChSpaceFilter& SFilter) {*this = SFilter;};
//���������� ����������� ������
~TAMultiChSpaceFilter() {_Reset();};
//------------------------------------------------------------------------------
//��������� ������� ������
//------------------------------------------------------------------------------
bool Set(TInputSignal* pLAA, const ivector& LAACh = ivector(),
         const rvector& Beams = rvector(), uint SF_Type = MS_SDM_MIMO_WSum,
         rvector& SF_Param = rvector(), bool PolarType = VP_SIGNAL)
{if (SetBeamFormer(pLAA,LAACh,Beams))
 {SetPolar(PolarType); return SetSFilter(SF_Type,SF_Param);}
 _Reset();//����� ���� ���������� ���������� LAA
 return false;
};
//��������� ������� �������������� �����
bool SetBeamFormer(TInputSignal* pLAA, const ivector& LAACh = ivector(),
     const rvector& Beams = rvector());
//��������� ������� ����������������� �������
bool SetSFilter(uint SF_Type, rvector& SF_Param = rvector());
//��������� ������� "��������������" �������� �����������
void SetInSignalStat(bool lSignal, bool lJammer, bool lNoise)
{TAnalInSignalProperty::Set(lSignal,lJammer,lNoise);};
//��������� ��������������� ������������ �������, ������������ ��������
bool SetPolar(bool PolarType) {return Polar = PolarType;};

//------------------------------------------------------------------------------
// ��������� �������� (�������������) ������ TAMultiChSpaceFilter
//------------------------------------------------------------------------------
//����������� ����������������� �������������� �����
bool IsBeamFormerReady() const {return TMultiBeamFormer::IsReady();};
//����������� ����������������� ����������������� �������
bool IsSpaceFilterReady() const
{return ((Filter != NULL) && (Filter->IsReady())) ? ON : OFF;};
//����������� ����������������� ���� ���������� Linear Antenna Array
bool IsReady() const
{return ((IsBeamFormerReady()) && (IsSpaceFilterReady())) ? ON : OFF;};
//����������� ������� - ���������� ������� LAA, ������������ � �������
ulong Size() const {return TMultiBeamFormer::GetLAACh();};
//��������� ��������������� ������������ �������
bool GetPolar() const {return Polar;};
//��������� ������������� �������� ���������
cmatrix& GetW(cmatrix& W) const
{if (!IsReady()) W.Set(0L,0L,false);
 else Filter->GetW(W);
 return W;
};
//��������� ����������� ������������� �������� ��������� �� �������� �����������
cmatrix& GetOptW(cmatrix& OptW, const cmatrix& R) const
{if (!IsReady()) OptW.Set(0L,0L,false);
 else Filter->GetOptW(OptW,R);
 return OptW;
};
//��������� ���� ����������������� �������
uint SFilterType() const {return (IsSpaceFilterReady()) ? Filter->Type() : 0;};
//��������� ���������� ����������������� �������
rvector& SFilterParam(rvector& Arg) const
{if (!IsSpaceFilterReady()) {Arg.Set(0L,ROW,false); return Arg;}
 return Filter->Param(Arg);
}

//------------------------------------------------------------------------------
//  �������������� ������� ��������� �������� ����������� �� ����� AdaptLAA
//      /���� ����������� ����������������� ������� SpaceFilter/
//------------------------------------------------------------------------------
//������� �������������� ������� ��������� ������� /���� AdaptLAA/
cmatrix& GetInRss(cmatrix& A) const {A = Rss; return A;};
//������� �������������� ������� ���������� �������� ������ /���� AdaptLAA/
cmatrix& GetInRjj(cmatrix& A) const {A = Rjj; return A;};
//������� �������������� ������� ����������� ���� /���� AdaptLAA/
cmatrix& GetInRnn(cmatrix& A) const {A = Rnn; return A;};
//������� �������������� ������� �������� �������: Rss+Rjj+Rnn
cmatrix& GetInR(cmatrix& A) const {A = Rss+Rjj+Rnn; return A;};
//������� �������������� ������� ��������� �������� �����������
cmatrix& GetInR(cmatrix& A, bool lSignal, bool lJammer, bool lNoise) const
{A.Set(0L,0L,false);
 if (!IsReady()) return A;
 if (lSignal) A = Rss;
 if (lJammer) A = (A.IsZeroSize()) ? Rjj : A + Rjj;
 if (lNoise)  A = (A.IsZeroSize()) ? Rnn : A + Rnn;
 return A;
};
//------------------------------------------------------------------------------
//   �������������� ������� ��������� �������� ����������� �� ������ AdaptLAA
//      /����� ����������� ����������������� ������� SpaceFilter/
//------------------------------------------------------------------------------
//�������� �������������� ������� ��������� ������� /����� SpaceFilter/
cmatrix& GetOutRss(cmatrix& A) const
{if (!IsReady()) {A.Set(0L,0L,false); return A;}
 A = Rss; //��������� ������� ��������� ������� �� ����� �������
 Filter->TF_WSum(A);//��������� ������� ��������� ������� �� ������ �������
 return A;
};
//�������� �������������� ������� ���������� �������� ������ /����� SpaceFilter/
cmatrix& GetOutRjj(cmatrix& A) const
{if (!IsReady()) {A.Set(0L,0L,false); return A;}
 A = Rjj;//��������� ������� �������� ������ �� ����� �������
 Filter->TF_WSum(A);//��������� ������� �������� ������ �� ������ �������
 return A;
};
//�������� �������������� ������� ����������� ���� /����� SpaceFilter/
cmatrix& GetOutRnn(cmatrix& A) const
{if (!IsReady()) {A.Set(0L,0L,false); return A;}
 A = Rnn;//��������� ������� ����������� ���� �� ����� �������
 Filter->TF_WSum(A);//��������� ������� ����������� ���� �� ������ �������
 return A;
};
//�������� �������������� ������� �������� �����������: Rss+Rjj+Rnn
cmatrix& GetOutR(cmatrix& A) const
{if (!IsReady()) {A.Set(0L,0L,false); return A;}
 GetInR(A);//��������� ������� �������� ����������� �� ����� �������
 Filter->TF_WSum(A);//��������� ������� �� ������ �������
 return A;
};
//�������� �������������� ������� ��������� �������� �����������
cmatrix& GetOutR(cmatrix& A, bool lSignal, bool lJammer, bool lNoise) const
{GetInR(A,lSignal,lJammer,lNoise);//��������� ������� �������� �����������
 Filter->TF_WSum(A);//������� ��������� �������� ����������� �� ������ �������
 return A;
};
//---------------  ������������ ������� ���������� LAA  ------------------------
//������������ ������� (Transfer function) ���������� LAA /����� MultiBeamFormer/
bool TFunc(rmatrix& P, const TEvenCloseInterval& Sector, uint NormType = GLOBAL,
           bool dB = ON, double MindB = -60.0) const;
//������������ ������� (Transfer function) ����������� ����������������� �������
//                      /����� SpaceFilter/
bool TF_SFilter(rmatrix& P, const TEvenCloseInterval& Sector, uint NormType = GLOBAL,
                bool dB = ON, double MindB = -60.0) const;
//������������ ������� (Transfer function) �������������� ����� ��� SpaceFilter
//             /����� MultiBeamformer - ������ ����������� � LAA/
bool TF_BeamFormer(rmatrix& P, const TEvenCloseInterval& Sector, uint NormType = GLOBAL,
                   bool dB = ON, double MindB = -60.0) const
{return TMultiBeamFormer::TFunc(P,Sector,NormType,dB,MindB);};
bool TF_BeamFormer(rvector& P, const cmatrix& R) const
{return TMultiBeamFormer::TFunc(R,P);};
//������������� ������������ ������� ���������� LAA ��� �������� ������ ��������
//������ � ����������� ���� (��������� ��������)
bool TF_Potent(rmatrix& P, double Time, const TEvenCloseInterval& Sector,
     uint NormType = GLOBAL, bool dB = ON, double MindB = -60.0);
//------------------------------------------------------------------------------
//           ������� ������������� ������ TAMultiChSpaceFilter
//------------------------------------------------------------------------------
//��������� ��������� ���������� AdaptLAA ��� �������������: ��������� �������-
//�� ������� ������������� ����������������� �������
bool SimulateInit() {return (IsReady()) ? Filter->SimulateInit() : OFF;};
//������ �������������� ������ ��������� �������� ����������� ��� �������������
bool Update(double Time);
//��������� ������� ������������� ����������������� �������
bool Tuning() {return (IsReady()) ? Filter->Tuning(R) : OFF;};
//��������� ������������� �������� ���������
bool SetW(cmatrix& W) {return (IsReady()) ? Filter->SetW(W) : OFF;};
//------------------------------------------------------------------------------
//               ������ ��������� �� ����� ���������� LAA
//   /�� ������ MultiBeamFormer, ������������ � LAA ��� SpaceFilter/
//------------------------------------------------------------------------------
//�������� ��������� ������� �� ����� ���������� LAA
bool InPSignal(rvector& Ps) const {return TMultiBeamFormer::TFunc(Rss,Ps);};
//�������� �������� ���������� ������ �� ����� ���������� LAA
bool InPJammer(rvector& Pjam) const {return TMultiBeamFormer::TFunc(Rjj,Pjam);};
//�������� ����������� ���� �� ����� ���������� LAA
bool InPNoise(rvector& Pn) const {return TMultiBeamFormer::TFunc(Rnn,Pn);};
//��������� �������� �� ����� ���������� LAA
bool InPSum(rvector& Psum) const {return TMultiBeamFormer::TFunc(R,Psum);};
bool InPSum(rvector& Psum, bool lSignal, bool lJammer, bool lNoise) const
{if (!IsReady()) {Psum.Set(0L,ROW,false); return false;}
 //������������ ������� �������� �����������
 cmatrix R; GetInR(R,lSignal,lJammer,lNoise);
 return TMultiBeamFormer::TFunc(R,Psum);
};

//------------------------------------------------------------------------------
//               ������ ��������� �� ������ ���������� LAA
//            /����� MultiBeamFormer � ������������ SpaceFilter/
//------------------------------------------------------------------------------
//�������� ��������� ������� �� ������ ���������� LAA
bool OutPSignal(rvector& Ps) const
{if (!IsReady()) {Ps.Set(0L,ROW,false); return false;}
 cmatrix A; GetOutRss(A);//������������ �������� ������� ��������� �������
 return TMultiBeamFormer::TFunc(A,Ps);
};
//�������� �������� ������ �� ������ ���������� LAA
bool OutPJammer(rvector& Pjam) const
{if (!IsReady()) {Pjam.Set(0L,ROW,false); return false;}
 cmatrix A; GetOutRjj(A);//������������ �������� ������� �������� ������
 return TMultiBeamFormer::TFunc(A,Pjam);
};
//�������� ����������� ���� �� ������ ���������� LAA
bool OutPNoise(rvector& Pn) const
{if (!IsReady()) {Pn.Set(0L,ROW,false); return false;}
 cmatrix A; GetOutRnn(A);//������������ �������� ������� ����������� ����
 return TMultiBeamFormer::TFunc(A,Pn);
};
//��������� �������� �� ������ ���������� LAA
bool OutPSum(rvector& Psum) const
{if (!IsReady()) {Psum.Set(0L,ROW,false); return false;}
 cmatrix A; GetOutR(A);//������������ �������� ������� �����������
 return TMultiBeamFormer::TFunc(A,Psum);
};
bool OutPSum(rvector& Psum, bool lSignal, bool lJammer, bool lNoise) const
{if (!IsReady()) {Psum.Set(0L,ROW,false); return false;}
 //������������ �������� ������� �����������
 cmatrix A; GetOutR(A,lSignal,lJammer,lNoise);
 return TMultiBeamFormer::TFunc(A,Psum);
};

//------------------------------------------------------------------------------
//         ������ ��������� �� ����� ����������������� �������
//        /�������� � ������� LAA, ������������ � SpaceFilter/
//------------------------------------------------------------------------------
//�������� ��������� ������� �� ����� ����������������� �������
bool InPs_SFilter(rvector& Ps) const;
//�������� �������� ���������� ������ �� ����� ����������������� �������
bool InPjam_SFilter(rvector& Pjam) const;
//�������� ����������� ���� �� ����� ����������������� �������
bool InPn_SFilter(rvector& Pn) const;
//------------------------------------------------------------------------------
//          ������ ��������� �� ������ ����������������� �������
//                         /����� SpaceFilter/
//------------------------------------------------------------------------------
//�������� ��������� ������� �� ������ ����������������� �������
bool OutPs_SFilter(rvector& Ps) const
{if (!IsReady()) {Ps.Set(0L,ROW,false); return false;}
 cmatrix A(Rss); Filter->TF_WSum(Ps,A);
 return true;
};
//�������� �������� ������ �� ������ ����������������� �������
bool OutPjam_SFilter(rvector& Pjam) const
{if (!IsReady()) {Pjam.Set(0L,ROW,false); return false;}
 cmatrix A(Rjj); Filter->TF_WSum(Pjam,A);
 return true;
};
//�������� ����������� ���� �� ������ ����������������� �������
bool OutPn_SFilter(rvector& Pn) const
{if (!IsReady()) {Pn.Set(0L,ROW,false); return false;}
 cmatrix A(Rnn); Filter->TF_WSum(Pn,A);
 return true;
};

//------------------------------------------------------------------------------
//       ������ ��������� �� ���� ������/������� ����������������� �������
//------------------------------------------------------------------------------
//�������� ��������� ������� �� ������/������� ����������������� �������
rmatrix& Ps_SFilter(rmatrix& P) const
{if (!IsReady()) {P.Set(0,0,false); return P;}
 cmatrix A(Rss); return Filter->Powers(P,A);
};
//�������� �������� ������ �� ������/������� ����������������� �������
rmatrix& Pjam_SFilter(rmatrix& P) const
{if (!IsReady()) {P.Set(0,0,false); return P;}
 cmatrix A(Rjj); return Filter->Powers(P,A);
};
//�������� ����������� ���� �� ������/������� ����������������� �������
rmatrix& Pn_SFilter(rmatrix& P) const
{if (!IsReady()) {P.Set(0,0,false); return P;}
 cmatrix A(Rnn); return Filter->Powers(P,A);
};
//------------------------------------------------------------------------------
//������ ������������� �������� �������� � ������������ ���������� ����������
//�������� ������ � ����������� ���� ����������� ����������������� �������
//------------------------------------------------------------------------------
rvector& OutPjnPotent_SFilter(rvector& P) const;
rvector& KsupPotent_SFilter(rvector& Ksup) const;
//------------------------------------------------------------------------------
//������ ������������� �������� �������� � ������������ ���������� ����������
//�������� ������ � ����������� ���� ���������� LAA /������ MultiBeamFormer/
//------------------------------------------------------------------------------
rvector& OutPjnPotent(rvector& P) const;
rvector& KsupPotent(rvector& Ksup) const;

//------------------------------------------------------------------------------
// ���������� ������� ����� - ������ ���������� ������ TAMultiChSpaceFilter
//------------------------------------------------------------------------------
//��������� ������� ������ � ���� ���������� ������� string
string& Properties(string& s, string& title, uint indent = 0) const;
string& Polarization(string& s, string& title, uint indent = 0) const;
//���� ���������� ������� �� ������������ ������ ����� cin
void Edit();
//�������������� ������� ����������������� �������
void EditSFilter();
//�������������� ��������������� ������� �������
void EditPolar();
//���������� ��������� ����� (>>) ��� ����� ���������� ������
friend istream& operator >>(istream& in, TAMultiChSpaceFilter& SFilter);
//���������� ��������� ������ (<<) ��� ������ ���������� ������
friend ostream& operator <<(ostream& out, const TAMultiChSpaceFilter& SFilter)
{string s; return out << SFilter.Properties(s,STR_NULL);};
//���������� �������������� ��������� ������������ operator =
void operator =(const TAMultiChSpaceFilter& SFilter);
//------------------------------------------------------------------------------
//II. ���������� ���������� ������-������� ������ TAMultiChSpaceFilter
//------------------------------------------------------------------------------
protected:
 void _Reset() //����� ���� ������������� ������
 {if (Filter != NULL) {delete Filter; Filter = NULL;}
  Rss = cmatrix(); Rjj = cmatrix(); Rnn = cmatrix(); R = cmatrix();
  TMultiBeamFormer::_Reset();
  TAnalInSignalProperty::Reset();
 };
 //��������� �������� �������������� ������
 void _MatrixSet()
 {ulong size = Size(); //complex C_ZERO = complex(0,0);
  Rss.Set(size,C_ZERO); Rjj.Set(size,C_ZERO); Rnn.Set(size,C_ZERO); R.Set(size,C_ZERO);
 };
 //��������� ����������� ������� � �������������� ������
 bool _SetSFilterSize()
 {if (!IsReady()) return false;
  Filter->SetInput(Size()); _MatrixSet();
  return true;
 }
 //������ �������������� ������ �� ����� ���������� LAA
 void _CalcInRss();
 void _CalcInRjj();
 void _CalcInRnn();
};
//����� ���������� ������ TAMultiChSpaceFilter
//The end of class TAMultiChSpaceFilter declaration
//******************************************************************************
//                   ���������� ������ TMultiChSpaceFilter
//�������������� ������ ���������� LAA � ���������������� ���������� N-���������
//�������� � ������������ �������������� ������. ������������ ���������� LAA:
//1. LAA - �������� �������� �������;
//2. ���������� ���������������� ������ � N ������� � N ��������;
//3. ������������ �������������� �����, ����������� � ����������������� �������.
//����� ��������� �������� ������� TMultiBeamFormer.
//����� ��������� ����������� �� ����� ����������������� �������, �������������
//� LAA, � ���� �������� ��������� ������� Us, ���������� �������� ������ Ujam,
//������������ ���� Unoise � ��������� ������� Usum ��������� �������� �������.
//������ Usum ������������ ��� ��������� ������� ������������� �����������������
//����������� ������� ������������ N x N. ����������� ������� ������������ ����-
//��������� � �� ����� ��������� ���������� ������� LAA. ������ � �����������
//����������� � ������ TMultiBeamFormer � ������������ ���������� � ������ ����-
//��� LAA, ������������ � ����������������� �������, � ����� �������� ���������
//������������ �����: ���������� ����������� ����� � �� ���������������� ���������
//(�� ������� ��� ���� �����) � ����������� �� ������������ LAA ������������ ��-
//��������� �����. ����� TMultiBeamFormer ������ ������������ ������������ ����-
//��� LAA. ���������� ������������  ������� ���������� ����������� ������� � ��-
//���� �����, ����������� �������������� ������.
//��� ������������� ���������� �������� ����������� �������������� �� ������ ����,
//��� ��� �������� ������, ���������� ��� � �������� ���������� ������ ����������
//�� ���������� ������.
//��� ���������� ������������ ������� ���������� LAA ����� �� ���������  ������-
//������ ������ ������� ��������� ��������. ����� ��������� ������������ �������
//����������������� ������� � ������������ �������������� ����� �� �����������,
//������������ �������� �� ������/������� ���������� LAA ��� �������, ���������-
//���� ����������� �������� ������� ������������� � �.�.
//���������. ��� ������� �������� �� ����� � ������ ����������������� �������
//������������ ������ ��������� � ������ �� ������� LAA, ������������ ����������
//LAA, �������� ����������� �� ������/������� ������ �������. ��� ������� ������-
//��� ��������� �������� ����������� �� �����/������ ���������� LAA ������������
//������ �������� ��� ������� ����, ������������ MultiBeamFormer. �������� ������-
//�� ����������� �� ������ ������������ ����� /����� AdaptLAA/. ������� ��������
//���������� LAA, �������������� �� ������ MultiBeamFormer, ��� �� �����������
//����� � LAA, �������� SpaceFilter. ��� ������ ������ ���� ������� ������������
//�������� �� �����/������ ���������� LAA, � ��� ������ ������ ������� - ��������
//�� �����/������ ����������������� �������.
//���������������� ������ ����������� � ������ ���������� �� ������� ����� -
//TMultiInMultiOutWeightSummator, ��� ���� ����������� ������������ � ��������
//����������������� ������� ������������ N x N ������ ��������� ������ �������
//���������� � N ������� � N ��������. ������� ��������� ����� ���������� ���� ��
//����� �� ������������ (��������������� � ����������������), ���������� ���������
//������� ������������� (���������� ���������), ������ ������������ ������������
//���������� �������� (�������/����������, ���� ����� ������������). ��� �������
//��������� ������ ���������� ����: ��� ������ ���� � N ������� � N ��������.
//����������� ��������� ������������ ����������� ������� LAA, ������������ �
//MultiBeamFormer. ����� ���������� ������ ���� ����������� ����������� ����������
//LAA ��� �� �� ���������� ���������� (��������� �������� ������� �������������
//�� ��������� ��������), ��� � �� �� ���������, ���������� ��������� �������
//������������� � ������������ �� ������������ ����������. ������ �������������-
//���� ������� ����������� � ������������ ����� ��������� �������������� (������-
//������ �������) � ����������� �� ��������� �����. ����������� ������� ����������
//������������ ����� �������� (�������) �����, ������� ����� ���� ��������� ����-
//������ LAA. ���������, ��� �������� ������� ������������� � ������� ���� LAA.
//�������������� ������������� ���������� LAA � ���������������� �������� N x N
//� ������������ ������ ������ ������������ �� ����� ���������� � ������������
//������������� ������ ����������. ������������� ����� ���������� ������ 20. ��
//���������� ����������� ���������� �������� �������� ����������� �������������.
//� ������ ������������� ������� ������������ ����������� ������� ����������� �
//�� ���������� ������� ����������� ��� ���������. ��� ��� ��� ��������������
//������������� ������ ���� ��������� ����� ���������, �� ������� ������������
//����������� �� ��������� ���� ������ ���������� � ��������� � ������� AvW. ��
//��������� ������������� ����� ��������� ������������ �������������� ����������
//LAA � �� ��� ������� ������ ���������� LAA, ������� ������� �� ������ ��������:
//�������� �����������, ������������ LAA � ����������������� �������, ���������
//��������� � ���� ������������ �������, ���������� ���������� ������� � �.�.
//������������ ������� ������ ����������� �� ����������� ������� �������������.
//******************************************************************************
class TMultiChSpaceFilter: public TMultiBeamFormer {
//------------------------------------------------------------------------------
// ���������� ���������� ������-������ ������ TMultiChSpaceFilter
//------------------------------------------------------------------------------
protected:
 //��������������� ������������ �������, ������������ ��������
 bool Polar;
 //��������� �� ������� ����������� ����� �������� ��������� N x N
 TMultiInMultiOutWeightSummator* Filter;//���������������� ���������� ������
 //������� ��������� �������, ���������� ������ � ����������� ���� �� ����� �������
 cvector Us;//������� ������� ��������� �������
 cvector Ujam;//������� ������� ���������� ������
 cvector Unoise;//������� ������� ����������� ����
 //������� ��������� �������� ����������� �� ����� ���������� LAA: Us+Ujam+Unoise
 cvector Usum;
 //����������� �� ����������� � ���� ������������� ������� ������������
 cmatrix AvW;
//------------------------------------------------------------------------------
// I. ���������� ������������� ������-������� ������ TMultiChSpaceFilter
//------------------------------------------------------------------------------
public:
//******************************************************************************
// ���������� ������������� � ����������� ������ TMultiChSpaceFilter
//******************************************************************************
//���������� ������������ �� ���������
TMultiChSpaceFilter() : TMultiBeamFormer() {Filter = NULL; _Reset();};
//���������� ������������ � �����������
TMultiChSpaceFilter(TInputSignal* pLAA, const ivector& LAACh = ivector(),
 const rvector& Beams = rvector(), uint SF_Type = MS_SDM_MIMO_WSum,
 rvector& SF_Param = rvector(), bool PolarType = VP_SIGNAL) :
 TMultiBeamFormer(pLAA,LAACh,Beams)
 {Filter = NULL; Polar = VP_SIGNAL;
  if (IsBeamFormerReady()) SetSFilter(SF_Type,SF_Param);
 };
//���������� ������������ �����������
TMultiChSpaceFilter(TMultiChSpaceFilter& SFilter) {*this = SFilter;};
//���������� ����������� ������
~TMultiChSpaceFilter() {_Reset();};
//------------------------------------------------------------------------------
//��������� ������� ������
//------------------------------------------------------------------------------
bool Set(TInputSignal* pLAA, const ivector& LAACh = ivector(),
         const rvector& Beams = rvector(), uint SF_Type = MS_SDM_MIMO_WSum,
         rvector& SF_Param = rvector(), bool PolarType = VP_SIGNAL)
{if (SetBeamFormer(pLAA,LAACh,Beams))
 {SetPolar(PolarType); return SetSFilter(SF_Type,SF_Param);}
 _Reset();//����� ���� ���������� ���������� LAA
 return false;
};
//��������� ������� �������������� �����
bool SetBeamFormer(TInputSignal* pLAA, const ivector& LAACh = ivector(),
     const rvector& Beams = rvector());
//��������� ������� ����������������� �������
bool SetSFilter(uint SF_Type, rvector& SF_Param = rvector());
//��������� ��������������� ������������ �������, ������������ ��������
bool SetPolar(bool PolarType) {return Polar = PolarType;};
//��������� ����������� ������������� AvW � ������� ��������
bool SetAvW() {if (!IsReady()) return false; return Filter->SetW(AvW);};
//------------------------------------------------------------------------------
// ��������� �������� (�������������) ������ TMultiChSpaceFilter
//------------------------------------------------------------------------------
//����������� ����������������� �������������� �����
bool IsBeamFormerReady() const {return TMultiBeamFormer::IsReady();};
//����������� ����������������� ����������������� �������
bool IsSpaceFilterReady() const
{return ((Filter != NULL) && (Filter->IsReady())) ? ON : OFF;};
//����������� ����������������� ���� ���������� Linear Antenna Array
bool IsReady() const
{return ((IsBeamFormerReady()) && (IsSpaceFilterReady())) ? ON : OFF;};
//����������� ������� - ���������� ������� LAA, ������������ � �������
ulong Size() const {return TMultiBeamFormer::GetLAACh();};
//��������� ��������������� ������������ �������
bool GetPolar() const {return Polar;};
//��������� ����������� ������������� �������� ���������
cmatrix& GetW(cmatrix& W) const
{if (!IsReady()) W.Set(0L,0L,false); else W = AvW; return W;};
//��������� ���� ����������������� �������
uint SFilterType() const {return (IsSpaceFilterReady()) ? Filter->Type() : 0;};
//��������� ���������� ����������������� �������
rvector& SFilterParam(rvector& Arg) const
{if (!IsSpaceFilterReady()) {Arg.Set(0L,ROW,false); return Arg;}
 return Filter->Param(Arg);
}
//---------------  ������������ ������� ���������� LAA  ------------------------
//������������ ������� (Transfer function) ���������� LAA /����� MultiBeamFormer/
bool TFunc(rmatrix& P, const TEvenCloseInterval& Sector, uint NormType = GLOBAL,
           bool dB = ON, double MindB = -60.0) const;
//������������ ������� (Transfer function) ����������� ����������������� �������
//                      /����� SpaceFilter/
bool TF_SFilter(rmatrix& P, const TEvenCloseInterval& Sector, uint NormType = GLOBAL,
                bool dB = ON, double MindB = -60.0) const;
//������������ ������� (Transfer function) �������������� ����� ��� SpaceFilter
//             /����� MultiBeamformer - ������ ����������� � LAA/
bool TF_BeamFormer(rmatrix& P, const TEvenCloseInterval& Sector, uint NormType = GLOBAL,
                   bool dB = ON, double MindB = -60.0) const
{return TMultiBeamFormer::TFunc(P,Sector,NormType,dB,MindB);};
//------------------------------------------------------------------------------
//           ������� ������������� ������ TMultiChSpaceFilter
//------------------------------------------------------------------------------
//��������� ��������� ���������� AdaptLAA ��� �������������: ��������� �������-
//�� ������� ������������� ����������������� �������
bool SimulateInit() {return (IsReady()) ? Filter->SimulateInit() : OFF;};
//������ ������� ��������� �������� ����������� ��� �������������
bool Update();
//��������� ������� ������������� ����������������� �������
bool Tuning() {return (IsReady()) ? Filter->Tuning(Usum) : OFF;};
//���������� ������� ������������� ����������������� �������
bool AverageW(ulong Realization);
//------------------------------------------------------------------------------
//               ������ ��������� �� ����� ���������� LAA
//   /�� ������ MultiBeamFormer, ������������ � LAA ��� SpaceFilter/
//------------------------------------------------------------------------------
//�������� ��������� ������� �� ����� ���������� LAA
bool InPSignal(rvector& Ps) const {return TMultiBeamFormer::TFunc(Us,Ps);};
//�������� �������� ���������� ������ �� ����� ���������� LAA
bool InPJammer(rvector& Pjam) const {return TMultiBeamFormer::TFunc(Ujam,Pjam);};
//�������� ����������� ���� �� ����� ���������� LAA
bool InPNoise(rvector& Pn) const {return TMultiBeamFormer::TFunc(Unoise,Pn);};
//��������� �������� �� ����� ���������� LAA
bool InPSum(rvector& Psum) const {return TMultiBeamFormer::TFunc(Usum,Psum);};
//------------------------------------------------------------------------------
//               ������ ��������� �� ������ ���������� LAA
//            /����� MultiBeamFormer � ������������ SpaceFilter/
//------------------------------------------------------------------------------
//�������� ��������� ������� �� ������ ���������� LAA
bool OutPSignal(rvector& Ps) const
{if (!IsReady()) {Ps.Set(0L,ROW,false); return false;}
 cvector V(Us); Filter->TF_WSum(V);
 return TMultiBeamFormer::TFunc(V,Ps);
};
//�������� �������� ������ �� ������ ���������� LAA
bool OutPJammer(rvector& Pjam) const
{if (!IsReady()) {Pjam.Set(0L,ROW,false); return false;}
 cvector V(Ujam); Filter->TF_WSum(V);
 return TMultiBeamFormer::TFunc(V,Pjam);
};
//�������� ����������� ���� �� ������ ���������� LAA
bool OutPNoise(rvector& Pn) const
{if (!IsReady()) {Pn.Set(0L,ROW,false); return false;}
 cvector V(Unoise); Filter->TF_WSum(V);
 return TMultiBeamFormer::TFunc(V,Pn);
};
//��������� �������� �� ������ ���������� LAA
bool OutPSum(rvector& Psum) const
{if (!IsReady()) {Psum.Set(0L,ROW,false); return false;}
 cvector V(Usum); Filter->TF_WSum(V);
 return TMultiBeamFormer::TFunc(V,Psum);
};

//------------------------------------------------------------------------------
//         ������ ��������� �� ����� ����������������� �������
//        /�������� � ������� LAA, ������������ � SpaceFilter/
//------------------------------------------------------------------------------
//�������� ��������� ������� �� ����� ����������������� �������
bool InPs_SFilter(rvector& Ps) const;
//�������� �������� ���������� ������ �� ����� ����������������� �������
bool InPjam_SFilter(rvector& Pjam) const;
//�������� ����������� ���� �� ����� ����������������� �������
bool InPn_SFilter(rvector& Pn) const;
//------------------------------------------------------------------------------
//          ������ ��������� �� ������ ����������������� �������
//                         /����� SpaceFilter/
//------------------------------------------------------------------------------
//�������� ��������� ������� �� ������ ����������������� �������
bool OutPs_SFilter(rvector& Ps) const
{if (!IsReady()) {Ps.Set(0L,ROW,false); return false;}
 cvector V(Us); Filter->TF_WSum(Ps,V);
 return true;
};
//�������� �������� ������ �� ������ ����������������� �������
bool OutPjam_SFilter(rvector& Pjam) const
{if (!IsReady()) {Pjam.Set(0L,ROW,false); return false;}
 cvector V(Ujam); Filter->TF_WSum(Pjam,V);
 return true;
};
//�������� ����������� ���� �� ������ ����������������� �������
bool OutPn_SFilter(rvector& Pn) const
{if (!IsReady()) {Pn.Set(0L,ROW,false); return false;}
 cvector V(Unoise); Filter->TF_WSum(Pn,V);
 return true;
};

//------------------------------------------------------------------------------
// ���������� ������� ����� - ������ ���������� ������ TMultiChSpaceFilter
//------------------------------------------------------------------------------
//��������� ������� ������ � ���� ���������� ������� string
string& Properties(string& s, string& title, uint indent = 0) const;
string& Polarization(string& s, string& title, uint indent = 0) const;
//���� ���������� ������� �� ������������ ������ ����� cin
void Edit();
//�������������� ������� ����������������� �������
void EditSFilter();
//�������������� ��������������� ������� �������
void EditPolar();
//���������� ��������� ����� (>>) ��� ����� ���������� ������
friend istream& operator >>(istream& in, TMultiChSpaceFilter& SFilter);
//���������� ��������� ������ (<<) ��� ������ ���������� ������
friend ostream& operator <<(ostream& out, const TMultiChSpaceFilter& SFilter)
{string s; return out << SFilter.Properties(s,STR_NULL);};
//���������� �������������� ��������� ������������ operator =
void operator =(const TMultiChSpaceFilter& SFilter);
//------------------------------------------------------------------------------
//II. ���������� ���������� ������-������� ������ TMultiChSpaceFilter
//------------------------------------------------------------------------------
protected:
 void _Reset() //����� ���� ������������� ������
 {if (Filter != NULL) {delete Filter; Filter = NULL;}
  Us = cvector(); Ujam = cvector(); Unoise = cvector(); Usum = cvector();
  AvW = cmatrix();
  TMultiBeamFormer::_Reset();
 };
 //��������� �������� �������� � ������� AvW
 void _VectorSet()
 {ulong size = Size();
  Us.Set(size,ROW,C_ZERO); Ujam.Set(size,ROW,C_ZERO); Unoise.Set(size,ROW,C_ZERO);
  Usum.Set(size,ROW,C_ZERO); Filter->GetW(AvW);
 };
 //��������� ����������� �������, ������� AvW � ��������
 bool _SetSFilterSize()
 {if (!IsReady()) return false;
  Filter->SetInput(Size()); _VectorSet();
  return true;
 }
 //������ ������� ��������� �������� ����������� �� ����� ���������� LAA
 void _CalcUs();
 void _CalcUjam();
 void _CalcUnoise();
};
//����� ���������� ������ TMultiChSpaceFilter
//The end of class TMultiChSpaceFilter declaration

//****************************************************************************************
//          ���������� ������������ ������ TAuxChSpaceRejectFilter
//****************************************************************************************
//����� �������� ������� ��� �������� ������� ���������������� ����������� �������� �����-
//��������� ������� ����������� ����������������� ������� � �������� �������. ����������
//������ ������� � ���������� �������� ����������� ������ �������� � �������������� �������
//����������� ����������������� �������.
//����������� ������ ������������ ����� ������� �������� � ����������� �������������� � M
//������� � N �������� (M > N). ���������������� �� ������ ���������� ������� ���������� �
//������ ���� �����: ���������������� (MultiStage) � ��������������� (SingleStage).
//��������������� ����������� ���������������� ������ ������������ ����� ������������
//��������������� ������� ����������, ���������� ������� ����� N. ������ ����� ��������
//����� M-N+1 ����� ������ � ���� �����.
//���������������� ����������� ���������������� ������ ������������ ����� ���� �����������-
//����� ������� �������� � M ������� � N ��������.
//�������� M-N ���������� ���������� ����������� ����������, ������� ����� ����������� ������.
//������ ������� ����� ������� ������������ ���������� ������ ����������� ���������� ������
//�������� � �������������� ������� ����������� ����������������� ������� � ���������� ��-
//�����. ��������� ��������� ������������� ������������ ������� ������� �� ��� ������� ���-
//�������. ��� ������� ������������� ������������ �������, ���������� �� ��� ������� �����-
//�����, ��� ��������������� ������� ���������� ����������� ������� ������������ MxK, ���
//M+1 - ���������� ������ �������, K - ����� ����������� ����������. ����� ������� �������
//N = M+1-K. ��� ��������� �������� ���������� � ����������� ������ �������������� �������
//�������� �������� �����, ������� �� ���������� ����������� ��������. ��� ��� ��������
//����� �� ���������� ��������, �� � ��������� ������� ��� �������� �� ����������.
//��������� ������� ������������ ����� K �������� ������������ M. � ������� �����������
//����������� �������� ���������� �������� ������� LAA, ������� ������������ � ������������
//�������, ��� �������� ��������� ���������� �������� � ������� �� K ����������� ����������.
//������ K ����� ������������� ������� ������� ������������ �������, ��������� N ����� -
//������� LAA, ������������ ��� ��������������. �������, ��� ������ ����� �� �������� �����-
//����� �� ������������ ����������, �������� ������� LAA, ������� � ���� ���������� � �����
//����������� ��������� ������ ��� �������� ������ ������������ �������. ����� ��������
//�������������� ����������, ����������� ���������� ������ �������� � �������� �����������
//� �������������� ������� Adaptive SpaceFilter with BaseChannel. ���������� �����������
//������ � �������������� ������� ����������� ������� � �������� ������� �������� � �����-
//�������� ���������� ���������� �������� � �������� �����������, ��� � ���� ������� �����-
//������ �����������. ��������� ��������� ������� ��� ���������� ����������������� ��������-
//���� ������� ��� 4-� �������������� ������� � 3-� ����������� ���������� ����� �����
//��������� ���: N = 4; K = 3; M = 7.
//����������� A: rows = M = 7; cols = K = 3.
//     | U11 U12 U13 |  Uij - ����������� ���������� � i-�� ������ ��� ��������
//     | U21 U22 U23 |        ������ ��������� ������� ��������� �������� � j-��
//     | U31 U32 U33 |        ����������� ����������;
// A = | U41 U42 U43 |  U1j..U3j - ������� ������ AuxChSpaceRejectFilter;
//     | U51 U52 U53 |  U4j..U7j - �������������� ������ AdaptSpaceFilterBaseCh
//     | U61 U62 U63 |
//     | U71 U72 U73 |
//������� ��������, ��� ��������� ��������� ������ � �������� �������� ������ ������������
//�������, ��������� ��������� ������� ����������� ����.
//������� ������������, ������������ �� "���������" ������� A ���������� � ������� �������
//������������� Q. ��������� ���������� ������������� ������� � ����������� ������� Q ����-
//��� �� ������� ���������� �������.
//�� ���� ������� ����� �������� ������ U ��� �������������� ������� R �������� �����������
//����������� M+1[BaseCh]. �� ������ ���������� ������ U' ��� ������� R' �����������
//N+1[BaseCh]. �������� ��������� ������ ������ ������ ��������� ��������� � ������� U �
//������������������ ������� �� ����� ������������ ������� ������ ��������������� �������-
//����������� ������� "���������" �������. ��� ���� ������������ ������� �������� ��������-
//������ �� �����. ����� ����, �� ���� ������������ ������� �.�. ����� ������ �����������
//������ ���������� ��� �������������� ������� ����������� M+1. ��� ���� ����������� ����-
//�������� �������� ������ ��� ������� �������� �������.
//������ ���������� ���������  ���������� ������������� ������������ �����������������
//������� � ��������� �������� ������������� ������� �� ������� ���������� MultiStage ���
//SingleStage � ����� ������� � ������ �������.
//****************************************************************************************
class TAuxChSpaceRejectFilter {
//------------------------------------------------------------------------------
// ���������� ���������� ������-������ ������ TAuxChSpaceRejectFilter
//------------------------------------------------------------------------------
protected:
 ulong Input;//���������� ������ �������
 ulong Output;//���������� ������� �������
//------------------------------------------------------------------------------
//���������� ������������� � ����������� ������ TAuxChSpaceRejectFilter
//------------------------------------------------------------------------------
public:
 //����������� �� ���������
 TAuxChSpaceRejectFilter() {Reset();};
 //����������� � �����������
 TAuxChSpaceRejectFilter(const cmatrix& A) {Tuning(A);};
 //���������� ����������� ������
 virtual ~TAuxChSpaceRejectFilter() {Reset();};
//------------------------------------------------------------------------------
//���������� ������������� ������-������� ������ TAuxChSpaceRejectFilter
//------------------------------------------------------------------------------
public:
 //����������� ����������������� ������������ �������
 bool IsReady() const {return (Input != 0L) ? true : false;};
 ulong In() const {return Input;};//���������� ������ ������������ �������
 ulong Out() const {return Output;};//���������� ������� ������������ �������
 //���������� ����������� ���������� (��������)
 ulong Directs() const {return Input-Output;};

 //-----------------------------------------------------------------------------
 //������������ ������� (TRANSFER FUNCTION) TAuxChSpaceRejectFilter
 //-----------------------------------------------------------------------------
 virtual bool TFunc(const cmatrix& InR, cmatrix& OutR) const = 0;
 virtual bool TFuncDM(const cmatrix& InR, cmatrix& OutR) const = 0;
 virtual bool TFunc(const cvector& InV, cvector& OutV) const = 0;

 bool operator ()(const cmatrix& InR, cmatrix& OutR) const
      {return TFunc(InR, OutR);};
 bool operator ()(const cvector& InV, cvector& OutV) const
      {return TFunc(InV, OutV);};
 //-----------------------------------------------------------------------------
 //��������� ������� ������������� ������������ ����������������� �������
 //-----------------------------------------------------------------------------
 virtual bool Tuning(const cmatrix& A) = 0;
 //-----------------------------------------------------------------------------
 //��������� ������� ������������� TAuxChSpaceRejectFilter
 //-----------------------------------------------------------------------------
 virtual bool GetW(cmatrix& W, bool lExtend = ON) const = 0;
 //-----------------------------------------------------------------------------
 //���������� ����-����� ������� ������
 //-----------------------------------------------------------------------------
 virtual uint Type() const = 0;//��� �������� ���������
 virtual string& Type(string& s) const = 0;//��� �������� ���������
 //����������� ������������ ������
 virtual string& Clip(string& s) const = 0;
 //������������ ������������ AuxChSpaceRejectFilter � ���� ���������� ������:
 //< <Input> x <Output>;\n >
 string& GetConfig(string& s) const
 {char buffer[80]; ostrstream text (buffer, sizeof(buffer));
  text << In() << " x " << Out() << ";" << endl << ends;
  s.append(text.str()); text.seekp(0);
  return s;
 };
 //������������ ������� �������� ��������� � ���� ���������� ������
 virtual string& Properties(string& s, string& head, uint indent = 0) const = 0;

 //���������� �������� ���������� ������ (<<) ������� ������
 friend ostream& operator <<(ostream& out, const TAuxChSpaceRejectFilter& SRF)
 {string s; return out << SRF.Properties(s,STR_NULL);};
//------------------------------------------------------------------------------
//���������� ���������� ������-������� ������ TAuxChSpaceRejectFilter
//------------------------------------------------------------------------------
protected:
 //��������� ��������� ���������� ������������ �������
 void Reset() {Input = 0L; Output = 0L;};
};//���������� ���������� ������ TAuxChSpaceRejectFilter
//------------- ���������� ���������� ������ TAuxChSpaceRejectFilter -----------

//****************************************************************************************
//          ���������� ������ TMultiStageAuxChSRF
//****************************************************************************************
//����� ��������� ������ ����������������� ����������������� ������������ ������� �������-
//������� ������� ����������� ����������������� ������� � �������� �������. ����������
//������ ������� � ���������� �������� ����������� ������ �������� ��������������� ��������
//����������� ����������������� �������.
//����������� ������ ������������ ����� ���������������� ������� �������� � �����������
//�������������� � M ������� � N �������� (M > N).
//�������� M-N ���������� ���������� ����������� ����������, ������� ������ ���������. ���-
//��� ������� ����� ������� ������������ ���� ����������� ���������� ������ �������� � ��-
//������������ ������� ����������� ����������������� ������� � ���������� �������. ��� ���-
//���� ������������� ������������ ������� ��� ��������������� ������� ���������� �����������
//"���������" ������� ������������ MxK, ��� M+1 - ���������� ������ �������, K - ����� ��-
//��������� ����������. ����� ������� ������� N = M+1-K. ��������� ������� ������������ K
//�������� ������������ M. ��������� ������� ������� � ������ TAuxChSpaceRejectFilter.
// �������� Q-��������������, ����������  ������� ������������� ����������������� �������
//                            � ����� �������� �������������
//������������ ������� �������� � ������� Q. ����������� ������� Q ������������:
//1. RDEChannel (������ LAA ������������ ��� ���������� ������ �������� � �������� ������-
//   ����� ���������������� �������� �������);
//2. AuxChannel (�������������� ������ �������, ������������ � LAA);
//3. BaseChannel ����������� 1 (�������� ����� �������).
//������� Q ������ ������������ � ����������� ����. ��� ���������� ������ �������� � N
//����������� ���������� 2*N ������������ ������ ������������ ����:
// R' = Qn*Qn-1*...*Q2*Q1*R*trQ1*trQ2*...*trQn-1*trQn, ���
// R - ������� �������������� �������;
// R' - ��������������� �������������� ������� � N ������������ �������������;
// Qn,Qn-1,...,Q2,Q1 - ����������� ������������ ������� ������������ ���� � ��������������
// ���������� ��� ������� ������ ��� N-�����������. ������ ������� - ������ ����������
// ������������ �������;
// trQ1,trQ2,...,trQn-1,trQn - ����������������� � Q1,tQ2,...,Qn-1,Qn ������� ������� ����-
// ���������. ������ ������� - ������ �������� ������������ �������.
//������ ����������� ��������� ������� ��� 2*N ������ � ����� ������� Q. ������������� �
//������� cvector � cmatrix �������� ��������� �� ������������ ������� ������������ ����
//� ������ ���������� ����� �� �������� � ����� ������� ��������� ��������� � ������ ��-
//������� ����������� �������� ��� ���������� �������� ����������� ���������� �� �������
//�������������� ������ � ��������  ��������.
//��� ������� N ����������� ���������� ������ �������� � �������������� ������� �������
//������������ N ������� LAA, ������� ������� �� ������ ��������� � ��������� ������� LAA,
//������� ������������ ��� �������������� ������ ����������������� ������� � ����������
//�������. � �������� ������ ���������� ������ �������� � �������� ����������� �� ������
//�����������, ������� � ������� Q ��������� ������ � ��������� ������� �������� �������
//������������, �� ����������� ���������� �������� ������� (�� ����������� ��������� �������
//� ������) ������� 1. ���� �� ������ �� ������ ����������� ����������, ������� Q ������
//���� ���������.
//������������ Space Reject Filter ������������ ������� ������ ������ �������� � ��������-
//��� ���������� � AuxCh. ����� �������, �������������� �������� �������� � �����������
//���������� ������ ����� �������� ����� ��� ��� ����������� �� ������� ��������� �������-
//���� �������. ������������ ��� ���� �������������� ������� � ������ ����������� �������-
//��� �������������� ������������ ������ RDE ������, ������� ������������ � �� �
//MultiStageAuxChSRF � �� ������ ��������� �� � ����� AuxCh. ��� ������� ������������� ���
//���� AuxCh � ������� ����������� ���������� ������ ���� ������������ ������ ������� �����
//SRF � ���  �����. �������� ���������� ������������� ���������:
//1. ����������� ������ ��������� ��������, ���������� � ������� ����������� ����������, �
// �������������� ����������� �������� ���������� �� ������� LAA, ������������ �
// MultiStageAuxChSRF. � ���� ������� ��������� RDECh � AuxCh.
// ����������� ������������ ��� ������� ����������� ���������� �������������� �� �������:
// Q(i,0) = - Ui/U0, Q(0,i) = conj(Q(i,0)),  0 < i < LAACh
// Q(i,0) - ����������� SRF ��� i-�� ������ LAA, ������������� � ����, ��� ������� ������-
// ����� ����������;
// Ui - ����������� ���������� i-�� ������ LAA, ������������� � AuxChRDEUnit,
// ��� �������� ��������� ��������� �������� � ������� ����������� ����������;
// U0 - ����������� ���������� 1-�� �������� ������ RDECh, ������� ������������ ����������
// ����������� ������ ������� � ������� ����������� � ��������� ������� ������������ �
// AuxChRDEUnit.
// 2. �������� ������� ��� j-�� ����������� ���������� (0 < j <= N). ����������� ������
// ��������� �������� � j-�� ����������� ���������� � ����������� ������ ����������� �����-
// ����� �� ������� LAACh, ������������ � SRF. ���������� ������ U ���������� �� �������-
// ����������� ����� ���������� ������������  ������ Lkc: U' = Lj-1c*...*L1c*L0c*U. � ��-
// �������� ������ U ���������� �� ������ U', �� ������� ������� ����������� ��������� ���-
// ���� � 0..j-1 ����������� ����������. ������������ ��� j-�� ����������� ���������� ���-
// ����������� ���������� �.1 �� �������: Q(i,j)=-U'i/U'j, Q(j,i)=conj(Q(i,j)), j<i<LAACh
// Q(i,j) - ����������� SRF ��� i-�� ������ LAA, ������������� � ����, ��� j-�� �����������
// ����������;
// U'i - ����������� ���������� i-�� ������ LAA, ������������� � AuxChSRF, ��� ��������
// ��������� ��������� �������� � j-�� ����������� ����������;
// U'j - ����������� ���������� j-�� �������� ������ RDECh, ������� ������������ ����������
// ����������� ������ ������� � j-�� ����������� � ��������� ������� ������������ � AuxChSRF.
// ���������� �.2 ������������ ��� ������� ���� N ����������� ����������.
// ����������: � ������ �������������� ������ ������ ������� ������������� SRF. ��������
// ������� ��� ������� LAA � ��������� ������ (��� ��� �����������), ������� ���������� �
// AuxChSRF, � �������� ������������ ���������� ���������� � ��������� ������� A. �� �����-
// ������� ������ ������� ������������ �����, ������� ���������� ������ ������ TMultiStage-
// AuxChSRF. ����� TMultiStageAuxChSRF ��������� ������ �������� ������������ �� �����������
// ������� A. ������� A �� ������ ���� ������� � ����� ����� ������� ������ ���� ������
// ������ ����� �������� �������. � ��������� ������, ������ ������ �� ����� ���������������.
// �������� ���������� ��������� ������� Q ��� ����� AuxChRDEUnit ��� ���� �����������
// ���������� � ����������������� ������� � �������� ������� � 5 ��������������� ��������.
// ����� �������, RDECh = 3, AuxCh = 5, Dimension Q = 9x9 (� ������ ��������� ���������
// ������). ������� Q ����� ����� ��������� ���:
//       U0  U1  U2  U3  U4  U5  U6  U7  U8
//     |  1  q01 q02 q03 q04 q05 q06 q07  0| U0
//     | q10  1  q12 q13 q14 q15 q16 q17  0| U1
//     | q20 q21  1  q23 q24 q25 q26 q27  0| U2
//     | q30 q31 q32  1   0   0   0   0   0| U3
// Q = | q40 q41 q42  0   1   0   0   0   0| U4
//     | q50 q51 q52  0   0   1   0   0   0| U5
//     | q60 q61 q62  0   0   0   1   0   0| U6
//     | q70 q71 q72  0   0   0   0   1   0| U7
//     |  0   0   0   0   0   0   0   0   1| U8
// U0..U2 - RDECh, U3..U7 - AuxCh, U8 - BaseChannel.
//� ����� ����������� ����� �������������� ��� ����������� ��������� ������� Q, ��� �����-
//������ �� ��� ������������� ������� Q � trQ. ������� Q � trQ ����� ����� ��������� ���:
//       U0  U1  U2
//     |  1   0   0  | U0
//     | q10  1   0  | U1
//     | q20 q21  1  | U2          U0  U1  U2  U3  U4  U5  U6  U7
//     | q30 q31 q32 | U3         | 1  q01 q02 q03 q04 q05 q06 q07 | U0
// Q = | q40 q41 q42 | U4   trQ = | 0   1  q12 q13 q14 q15 q16 q17 | U1
//     | q50 q51 q52 | U5         | 0   0   1  q23 q24 q25 q26 q27 | U2
//     | q60 q61 q62 | U6
//     | q70 q71 q72 | U7
// U0..U2 - RDECh, U3..U7 - AuxCh.
//� �������� �������� ����������� ����� �������������� ����������� �������������� �������
//R ��� ������ ���������� U � ����������� ��������� ������ ����������� �������. ��� Q-���-
//����������� ����������� ����������������� ������� � ������������ ����������� ��������
//����������� ����������� ����������������� ������������ �������. ��� �� ��������������
//��� ������������������� ������� ������������ ������ ��� ������� �������� �������.
//����������� �������� ������� ������ ���� ����� ����� ������ �������. ������� R ������
//���� ���������� � � ������������ ������ ����� ������ SRF. � �������� ��������� ��������
//������ ������ ���������� ������ ��� ���������� ������� � ������������� ������� �����
//������� Space Reject Filter.
//****************************************************************************************
class TMultiStageAuxChSRF : public TAuxChSpaceRejectFilter {
//---------------------------------------------------------------------------------------
// ���������� ���������� ������-������ ������ TMultiStageAuxChSRF
//---------------------------------------------------------------------------------------
protected:
 cmatrix Q;//������� Q-��������������
 cmatrix trQ;//������� ����������-����������������� � ������� Q
//------------------------------------------------------------------------------
//���������� ������������� � ����������� ������ TMultiStageAuxChSRF
//------------------------------------------------------------------------------
public:
 //����������� �� ���������
 TMultiStageAuxChSRF() {Reset();};
 //����������� � �����������
 TMultiStageAuxChSRF(const cmatrix& A) {Tuning(A);};
 //���������� ����������� ������
 ~TMultiStageAuxChSRF() {Reset();};
//------------------------------------------------------------------------------
//���������� ������������� ������-������� ������ TMultiStageAuxChSRF
//------------------------------------------------------------------------------
public:
 //-----------------------------------------------------------------------------
 //������������ ������� (TRANSFER FUNCTION) TMultiStageAuxChSRF
 //-----------------------------------------------------------------------------
 bool TFunc(const cmatrix& InR, cmatrix& OutR) const;
 bool TFuncDM(const cmatrix& InR, cmatrix& OutR) const;
 bool TFunc(const cvector& InV, cvector& OutV) const;
 //-----------------------------------------------------------------------------
 //��������� ������� ������������� ������������ ����������������� �������
 //-----------------------------------------------------------------------------
 bool Tuning(const cmatrix& A);
 //-----------------------------------------------------------------------------
 //��������� ������� ������������� TMultiStageAuxChSRF
 //-----------------------------------------------------------------------------
 bool GetW(cmatrix& W, bool lExtend = ON) const;
 //-----------------------------------------------------------------------------
 //���������� ����-����� ������� ������ TMultiStageAuxChSRF
 //-----------------------------------------------------------------------------
 uint Type() const {return MultiStageUnit;};//��� �������� ���������
 //��� �������� ���������
 string& Type(string& s) const
 {return s = "MultiStage Space Reject Filter of the Auxiliary Channels";};
 //����������� ������������ ������
 string& Clip(string& s) const {return s = "MultiStageAuxChSRF";};
 //������������ ������� �������� ��������� � ���� ���������� ������
 string& Properties(string& s, string& head, uint indent = 0) const;
 //���������� �������� ���������� ������ (<<) ������� ������
 friend ostream& operator <<(ostream& out, const TMultiStageAuxChSRF& SRF)
 {string s; return out << SRF.Properties(s,STR_NULL);};

//------------------------------------------------------------------------------
//���������� ���������� ������-������� ������ TMultiStageAuxChSRF
//------------------------------------------------------------------------------
protected:
 //��������� ��������� ���������� ������������ �������
 void Reset() {Input = 0L; Output = 0L; Q = cmatrix(); trQ = cmatrix();};
};//���������� ���������� ������ TMultiStageAuxChSRF
//------------- ���������� ���������� ������ TMultiStageAuxChSRF ---------------

//******************************************************************************
//          ���������� ������ TSingleStageAuxChSRF
//******************************************************************************
//����� ��������� ������ ���������������� ����������������� ������������ �������
//�������������� ������� ����������� �����������������  ������� � �������� ����-
//���. ���������� ������ ������� � ���������� �������� ����������� ������ �����-
//���  ��������������� �������� ����������� ����������������� �������. ��������-
//��� ������ ������������ ����� ������������ ��������������� ������� ����������,
//����������  �������  ����� N. ������ ����� �������� ����� M-N+1 ����� ������ �
//���� ����� (N - ����� AuxCh+BaseCh, M - RDECh+AuxCh+BaseCh, K = M-N - �����
//����������� ����������.
//�������� M-N ���������� ���������� ����������� ����������, ������� ����� �����-
//������ ������ ������. ������ ������� ����� ������� ������������ ���������� ��-
//���� ����������� ���������� ������ �������� � �������������� �������  �������-
//���� ����������������� ������� � ���������� �������. ��� ������� �������������
//������������ �������, ��� ��������������� ������� ���������� ����������� "���-
//������" ������� ������������ MxK, ��� M+1 - ���������� ������ �������, K - ���-
//�� ����������� ����������. ����� ������� ������� N = M+1-K. ���������  �������
//������������  ����� K �������� ������������ M. ��������� ������� ������� � ��-
//���������� ������ TAuxChSpaceRejectFilter.
//�������� Q-��������������, ����������  ������� �������������  ����������������
//                    ������� � ����� �������� �������������
//������������  �������  �������� � �������� Q � trQ. ������� trQ �������� ����-
//������-����������� � ������� Q � ������������ ������ � �������� Q Q-���������-
//�����  �������������� ������� �������� �����������. ������� Q �������� ������-
//�������. ����������� ������� Q �� ��������:
//1. RDEChannel (������ LAA ������������ ��� ���������� ������ �������� � �����-
//   ��� ����������� ���������������� �������� �������);
//2. AuxChannel (�������������� ������ �������, ������������ � LAA);
//3. BaseChannel ����������� 1 (�������� ����� �������).
//����������� ������� Q �� �������:
//1. AuxChannel (�������������� ������ �������, ������������ � LAA);
//2. BaseChannel ����������� 1 (�������� ����� �������).
//������ ������ ������� Q ������������ ����� ������������ ������ ��������������-
//�� �������� ���������, ������� ������������ �� ���� ���������������� ��������-
//��� ������ �� K ����������� ��� ������ ��������������� ������. ��������� ����-
//�� ������� Q �������� �������� ������� ������������� ��� ��������� ������, ���
//������������ ���� ������, �� ����������� ���������� (������� 1), ����� 0, �.�.
//� �������� ������ ���������������� ���������� �� ������ ��������������.
//��� ������������� Q-�������������� ���������� ������ �������� ������� ��������
//����� ��  ������� Q: V' = Q*V. ��� ������������� Q-�������������� ��� �������-
//������� �������� �������� ������� R ���������� �������� ��������� ��������:
//R' = Q*R*trQ.
//��� ������� K ����������� ���������� ������  �������� �  ��������  �����������
//��������������� �������� �������  ������������  K ������� LAA, ������� �������
//�� ������ ��������� � ��������� ������� LAA, ������� ������������ ��� �������-
//�������  ������ ����������������� ������� � ���������� �������.
//������������ Space Reject Filter ������������ ������� ������ ������ �������� �
//����������� ���������� � AuxCh. ����� �������, ��������������  �������� �����-
//��� � ����������� ���������� ������ ����� �������� ����� ��� ��� �����������
//�� ������� ��������� ����������� �������. �������� ���������� �������������
//��������������� ����������� ���������������� �������� ���������:
//1. ����������� ������� ��������� ��������, ���������� � K ����������� �������-
// ���, � �������������� ����������� �������� ���������� �� ������� LAA,
// ������������ � SingleStageAuxChSRF. � ���� ������� ��������� RDECh � AuxCh.
// �������� ������� ��� ������� LAA � ��������� ������ (��� ��� �����������),
// ������� ���������� � AuxChSRF, � �������� ������������ ���������� ����������
// � ��������� ������� L. �� ������������ ������ ������� ������������ �����, ��-
// ����� ���������� ������ ������ TSingleStageAuxChSRF. ����� TSingleStageAuxChSRF
// ��������� ������ �������� ������������ �� ����������� ������� L. ������� L ��
// ������ ���� ������� � ����� ����� ������� ������ ���� ������ ������ �����
// �������� �������. � ��������� ������, ������ ������ �� ����� ���������������.
// ��������� ��������� �������  ���  ����������  �����������������  ������������
// �������  ��� 4-�  ��������������  ������� � 3-� �����������  ���������� �����
// ����� ��������� ���: N = 4; K = 3; M = 7.
//����������� L: rows = M = 7; cols = K = 3.
//     | U11 U12 U13 |  Uij - ����������� ���������� � i-�� ������ ��� ��������
//     | U21 U22 U23 |        ������ ��������� ������� ��������� �������� � j-��
//     | U31 U32 U33 |        ����������� ����������;
// L = | U41 U42 U43 |  U1j..U3j - ������� ������ AuxChSpaceRejectFilter;
//     | U51 U52 U53 |  U4j..U7j - �������������� ������ AdaptSpaceFilterBaseCh
//     | U61 U62 U63 |
//     | U71 U72 U73 |
// 2. �� ������ ������� L �������� ������� �������� �������������� ��������� � N
// ������� ������� (�� ����� AuxCh ��� ����� ��������������� ����������) ������-
// ���� ����: A*W = B, ��� A - ���������� ������� L � �������������� Uij ��� ��-
// �����  �������  �������; � - ����������  ������� L �  �������������� Uij  ���
// ��������������� ������� AuxCh; W - ������� ������� ������������� ��� N ����-
// ����������� ����������, ������� ��������������. ��� ������ ������� L ���� �
// N ������� ������� ������������ ��������� �������:
// U11*W1 + U21*W2 + U31*W3 = -U41 -U51 -U61 -U71
// U12*W1 + U22*W2 + U32*W3 = -U42 -U52 -U62 -U72
// U13*W1 + U23*W2 + U33*W3 = -U43 -U53 -U63 -U73
// ��� ������� ������ ������� ��������� �������� ������������ ������� W:
//     | w11 w12 w13 w14 |
// W = | w21 w22 w23 w24 |
//     | w31 w32 w33 w34 |
// ������� ������� W �������� ������� ������������ ��� 4-x ��������������� ����-
// ����������� ����������� �������, ������� ������������ �� 3 ����������� �����-
// ����� � 4-� ��������������  �������  �����������  ����������������� ������� �
// �������� �������.
// �������, ��� ��� ����������� ���������� ������ ����������������� ������������
// ������� ����� ��� ���������, ������������������ ������� LAA � BaseCh (��� ���
// �������������) ������ ���� ����������� ��� � � ��������� ������� L.
// ������������ ������� W �������� ������ ������������� ������� Q. �������� ���-
// ���� Q ��� ����� AuxChRDEUnit � ����� ������������� ���������� ��� ���������-
// �� ����������������� ������� � �������� ������� � 4 ��������������� ��������.
// ����� �������, RDECh = 3, AuxCh = 4, Dimension Q = (3+4+1)x(4+1) = 8x5
// (� ������ ��������� ��������� ������). ������� Q ����� ����� ��������� ���:
//       U0  U1  U2  U3  U4  U5  U6  U7
//     | w11 w21 w31  1   0   0   0   0 | U3
//     | w12 w22 w32  0   1   0   0   0 | U4
// Q = | w13 w23 w33  0   0   1   0   0 | U5
//     | w14 w24 w34  0   0   0   1   0 | U6
//     |  0   0   0   0   0   0   0   1 | U7
// U0..U2 - RDECh, U3..U6 - AuxCh, U7 - BaseChannel.
//� ����� ����������� ����� �������������� ��� ����������� ��������� ������� Q,
//��� ������������� � ������� Q ���������� ����:
//       U0  U1  U2
//     | q00 q01 q02 | U3
//     | q10 q11 q12 | U4
// Q = | q20 q21 q22 | U5
//     | q30 q31 q32 | U6
//�� ������� �������������� ������� Q ����������  ���������  ������� � ���������
//�� ������, ��� ��� ���������� ������ ��������� ����� ����� ������ ��� �������-
//��� ��������� Q-��������������.
//� ��������  ��������  ����������� ����� ���� ������������ ����������� �������-
//������� ������� R ��� ������ ���������� U, ������� ������ ��������� ����������
//��������� ������ ����������� �������. ��� Q-�������������� ����������� ������-
//����������� ������� � ������������ ����������� �������� ����������� ���������-
//�� ����������������� ������������ �������. ��� �� �������������� ��� ��������-
//����������� ������� ������������ ������ ��� ������� �������� �������.
//����������� �������� ������� ������ ���� ����� ����� ������ �������. ������� R
//������ ���� ���������� � �� �����������, �����, �.�. ����� ����� ������ SRF. �
//�������� ��������� �������� ������ ������ ���������� ������ ��� ���������� ���-
//���� � ������������� ������� ����� ������� Space Reject Filter.
//******************************************************************************
class TSingleStageAuxChSRF : public TAuxChSpaceRejectFilter {
//------------------------------------------------------------------------------
// ���������� ���������� ������-������ ������ TSingleStageAuxChSRF
//------------------------------------------------------------------------------
protected:
 cmatrix Q;//������� Q-��������������
 cmatrix trQ;//������� ����������-����������������� � ������� Q
//------------------------------------------------------------------------------
//���������� ������������� � ����������� ������ TSingleStageAuxChSRF
//------------------------------------------------------------------------------
public:
 //����������� �� ���������
 TSingleStageAuxChSRF() {Reset();};
 //����������� � �����������
 TSingleStageAuxChSRF(const cmatrix& A) {Tuning(A);};
 //���������� ����������� ������
 ~TSingleStageAuxChSRF() {Reset();};
//------------------------------------------------------------------------------
//���������� ������������� ������-������� ������ TSingleStageAuxChSRF
//------------------------------------------------------------------------------
public:
 //-----------------------------------------------------------------------------
 //������������ ������� (TRANSFER FUNCTION) TSingleStageAuxChSRF
 //-----------------------------------------------------------------------------
 bool TFunc(const cmatrix& InR, cmatrix& OutR) const;
 bool TFuncDM(const cmatrix& InR, cmatrix& OutR) const;
 bool TFunc(const cvector& InV, cvector& OutV) const;
 //-----------------------------------------------------------------------------
 //��������� ������� ������������� ������������ ����������������� �������
 //-----------------------------------------------------------------------------
 bool Tuning(const cmatrix& A);
 //-----------------------------------------------------------------------------
 //��������� ������� ������������� TSingleStageAuxChSRF
 //-----------------------------------------------------------------------------
 bool GetW(cmatrix& W, bool lExtend = ON) const;
 //-----------------------------------------------------------------------------
 //���������� ����-����� ������� ������ TSingleStageAuxChSRF
 //-----------------------------------------------------------------------------
 uint Type() const {return SingleStageUnit;};//��� �������� ���������
 //��� �������� ���������
 string& Type(string& s) const
 {return s = "SingleStage Space Reject Filter of the Auxiliary Channels";};
 //����������� ������������ ������
 string& Clip(string& s) const {return s = "SingleStageAuxChSRF";};
 //������������ ������� �������� ��������� � ���� ���������� ������
 string& Properties(string& s, string& head, uint indent = 0) const;
 //���������� �������� ���������� ������ (<<) ������� ������
 friend ostream& operator <<(ostream& out, const TSingleStageAuxChSRF& SRF)
 {string s; return out << SRF.Properties(s,STR_NULL);};

//------------------------------------------------------------------------------
//���������� ���������� ������-������� ������ TSingleStageAuxChSRF
//------------------------------------------------------------------------------
protected:
 //��������� ��������� ���������� ������������ �������
 void Reset() {Input = 0L; Output = 0L; Q = cmatrix(); trQ = cmatrix();};
};//���������� ���������� ������ TSingleStageAuxChSRF
//------------- ���������� ���������� ������ TSingleStageAuxChSRF --------------

//******************************************************************************
//���������� ������ - TInDataBaseCh - ������� ���������� ����� ������������
//��������� ������ ��� ���������� ���������������� �������� � ���������� �������
//�� ������ LAA
//******************************************************************************
class TInDataBaseCh {
 //-----------------------------------------------------------------------------
 // ���������� ���������� ������-������ ������ TInDataBaseCh
 //-----------------------------------------------------------------------------
 protected:
 //-----------------------------------------------------------------------------
 //������� ������ � ������� LAA � BaseChannel
  TInputSignal* OutLAA; //��������� �� ������� ����������� � LAA
 //-----------------------------------------------------------------------------
 //������� ������� LAA, ������������ � ����� ������������ ��������� ������.
 //� BaseChannel ����� ������������ �� 1 �� InSignal->LAAElem() ������� LAA.
 ivector Channel;
 //�������������� �������������� ����� ����������� ������� - BFC.
 //BFC ������������, ���� Channel.Size() > 1.
 double ScanAngle;//����������� ���� ������������ ���������������� ���� [deg]
 //-----------------------------------------------------------------------------
 // I. ���������� ������������� ������-������� ������ TInDataBaseCh
 //-----------------------------------------------------------------------------
 public:
 //*****************************************************************************
 // ���������� ������������� � ����������� ������ TInDataBaseCh
 //*****************************************************************************
 TInDataBaseCh();//���������� ������������ �� ���������
 //���������� ������������ � �����������
 TInDataBaseCh(TInputSignal* InSignal);
 TInDataBaseCh(TInputSignal* InSignal, const ivector& Index, double Angle = 0);
 //���������� ������������ �����������
 TInDataBaseCh(TInDataBaseCh& InDataBaseCh) {*this = InDataBaseCh;};
 //���������� ����������� ������
 ~TInDataBaseCh();
 //*****************************************************************************
 // ������������� ������� ��� ������ � ������� TInDataBaseCh
 //*****************************************************************************
 //�������� ����������������� ��������� ������:
 bool IsReady() const {return (OutLAA == NULL) ? OFF : ON;};
 //��������� ������� ��������� ������
 void Set(TInputSignal* InSignal, const ivector& Index = ivector(), double Angle = 0,
          bool Check = OFF);
 //��������� ������� ������ � ���� ���������� ������� string
 string& Properties(string &s, string& title, uint indent = 0) const;
 //��������� ������� ������� LAA, ������������ � ��������� ������
 string& Channels(string& s, ulong count, char* sep = NULL, char* etc = NULL) const
         {return Channel.ConvertToString(s,count,sep,etc);};
 //����������� ������������� �������������� ����� - BeamFormerCircuit
 bool UsageBFC() const {return (Channel.Size() > 1L) ? ON : OFF;};
 //���������������� ��������� �������� �������� ��������� ������
 double GetScanAngle() const {return ScanAngle;};
 //������� ������� LAA, ������������ � BaseChannel
 ulong Channels(ivector& BaseCh) const
 {BaseCh = Channel; return BaseCh.Size();};
 const ivector& Channels() const {return Channel;};
 //������ ������ LAA, ������������ � BaseChannel, ��������������� ��� �����������
 //������ � ������� Channel
 long IndexLAACh(ulong number = 0) const
 {return (!IsReady() || (number >= Channel.Size())) ? -1L : Channel[number];};
 //��������� �� LAA, � ������� ���������� ����� ������������ ��������� ������
 TInputSignal* GetLAA() const {return OutLAA;};
 //*****************************************************************************
 // ������������� ������� �������� ������ ��� ������������ ��������� ������
 // 1. ������� ����� - ������ ���������� TInDataBaseCh
 //*****************************************************************************
 //���� ���������� ������� �� ������������ ������ ����� cin
 bool Edit();
 //���������� ��������� ����� (>>) ��� ����� ���������� ������
 friend istream& operator >>(istream& in, TInDataBaseCh& Obj);
 //���������� ��������� ������ (<<) ��� ������ ���������� ������
 friend ostream& operator <<(ostream& out, const TInDataBaseCh& Obj)
 {string s; return out << Obj.Properties(s,STR_NULL);};
 //���������� �������������� ��������� ������������ operator =
 void operator =(const TInDataBaseCh& InDataBaseCh);
 //-----------------------------------------------------------------------------
 // II. ���������� ���������� ������-������� ������ TInDataBaseCh
 //-----------------------------------------------------------------------------
 protected:
  void Reset(); //��������� ��������� ���� ������������� TInDataBaseCh
  //���� ��������� ���������� ��������� ������ �� ������������ ������ �����
  bool GetDataFromConsole(ivector& LAAChannel, double& Angle);
};
//����� ���������� ������ TInDataBaseCh
//The end of class TInDataBaseCh declaration
//----------------------------------------------------------------------------------------
//      ����� ������������ ��������� ������ ���������� LAA - TBaseChannel
//----------------------------------------------------------------------------------------
//    ����� ������������ ��������� ������ ����������� ������� TBaseChannel. � �������� ��-
//������� ������ ����� �������������� ����� ����� (�������� �������) LAA ��� ������������-
//��� �������� �����. ��������������� �������� ����� ���������� ������������ �� 1 �� N ��-
//����� LAA � ��������������  ����� - BeamFormer Circuit (BFC). ���  ��������� ������������
//������� ��� (�������� ��������) LAA ��������� ������ � ������������ ���������������� ��-
//�������� �� -90 �� +90 ��������. ������ ���������������� ���� ����� ������������ ������
//�������� ���������, ������������ � BFC, � ���������������� ���������� ����. ��� �������-
//��� �������� ���� �� 0 �������� �� �����������. ������������ ���� � ��������� �� -90 ��
//+90 �������� ����������� ���������� ����������� �������� ���������� ������� ��������� ��-
//���� LAA �� ��������������� ����������-����������� �������������� ����������� EXP(jPHIi)
//BFC. ��� ������������� ������ ������ LAA ��� ��������� ����� ����� ������ ������������
//����������� ������������. �� � ���� ������ ����������� ������������� �������� ���������
//�������� LAA � ������������ ����������� ����� ��������� ����� ��������� ���� ������� ���,
//����������� ����� ��������� LAA ����� ��������� ������� �������, ���� ������ ������� ��-
//������ �������������� ���������� �������������� � ������ ��������� ������������. �������
//� ������ ���������� ������ ��������� �������� � �������� ��������� ������ LAA ����� �����
//�������� ������� ��������� �������� ���� ������������ ������� � �������� LAA.
//    �������������� ����� ������������ ��������� ������ LAA ����������� ��� ������������
//������� ��������. � �������� ����� ��������� �������������� ������������ ������� ������
//LAA ������������� � BFC, ������� ��������� ���� ������������. ����� ������������ �������-
//��� �������� ������� LAA, ������������ � BFC, �� ��������������� ������� ��������������
//������������, ���������� ������������ ����������� �� ����� ���������, �������� ��������
//�������� ���������� ����������� �������� ���������� ���������� ������, ��������� �������
//� ����������� ���� ��������� ������. �������� �������� ��������� �������� ������� �����
//������������ ��������� ������ LAA ���������� �������� ���������� ������, ������� � ����-
//������� ���� �� ����� ���������� LAA.
//    ������������ PHIi ���������� ������� ����� � i-�� ������ LAA, ������������ � BFC,
//��� ������������ ��������� ���� ������������ �� ������� ��� �� ���� ����� LAA. ����������-
//����������� �������������� ����������� ����������� �� ��������� �������:
// PHIi = EXP(-2*PI*LVi*sin(ScanAngle)*j), ���
// ScanAngle - �������� ���� ������������ �� (����������� �������� ��������);
// LVi - ������������� ���������� ����� 0 � i-� ������� �������� �������;
// j - ������ ������� => sqrt(-1).
// ������� ����� ���������������� ���� ��������� � 0-�� ������� LAA. � BFC ��� ������������
//���������������� ����, ����� ���������� ������������ ������ LAA � ��������� �� 0 �� (N-1),
//��� N - ���������� �������� ��������� LAA.
//----------------------------------------------------------------------------------------
//****************************************************************************************
//���������� ������ - TBaseChannel - ����� ������������ ��������� ������ ��� �����������
//����������������� ������� � ���������� ������� �� ������ LAA
//****************************************************************************************
class TBaseChannel : public TInDataBaseCh {
 //---------------------------------------------------------------------------------------
 // ���������� ���������� ������-������ ������ TBaseChannel
 //---------------------------------------------------------------------------------------
 protected:
  cvector B;//�������������� ������, �������� ����������� ������������ ����
 //-----------------------------------------------------------------------------
 // I. ���������� ������������� ������-������� ������ TBaseChannel
 //-----------------------------------------------------------------------------
 public:
 //*****************************************************************************
 // ���������� ������������� � ����������� ������ TBaseChannel
 //*****************************************************************************
 TBaseChannel();//���������� ������������ �� ���������
 //���������� ������������ � �����������
 TBaseChannel(TInputSignal* InSignal);
 TBaseChannel(TInputSignal* InSignal, const ivector& Index, double Angle = 0);
 TBaseChannel(const TInDataBaseCh& InData) {*this = InData;};
 //���������� ������������ �����������
 TBaseChannel(TBaseChannel& BaseCh) {*this = BaseCh;};
 //���������� ����������� ������
 ~TBaseChannel();
 //*****************************************************************************
 // ������������� ������� ��� ������ � ������� TBaseChannel
 //*****************************************************************************
 //��������� ��������������� �������
 cvector BeamFormerVector() const {return B;};
 //��������� ������� ��������� ������
 void Set(TInputSignal* InSignal, const ivector& Index = ivector(),
          double Angle = 0, bool Check = OFF);
 void Set(const TInDataBaseCh& InData) {*this = InData;};
 //��������� ������� ������ � ���� ���������� ������� string
 //string& GetPerformance(string &s) const;
 //���������� ��������� �������������� ��������� ������ � �������� ������� � ���������
 //������������ LAA ������������ ����������� ����� (�� ������� ��� ���� �����)
 rvector AntPatterns(double InitAngle = -90, double EndAngle = 90,
         double Step = 0.1, bool Deg = DEG, bool Norm = ON) const;
 //������������ ������� ��������� ������ (��������� ��������������)
 rvector& DirectPattern(rvector& P, const TEvenCloseInterval& Sector,
          bool Norm = ON, bool Db = ON, double MinDb = -60.0) const;
 //���������� �������������� ��������� ������ ���������� LAA � �������� ����������� �
 //��������� �� ������������ ������������ ����������� �����
 double Directivity(double Angle, bool Deg = DEG) const;
 //���������� ������ ���� ��������� ������ �� ��������� ������ �������� � ��������
 //double BeamWidth(double PowerLevel = 0.5, double Prec = 0.01);
 //*****************************************************************************
 // ������������� ������� ������� �������� �������� ��������� ������
 //*****************************************************************************
 //1. OVERLOAD THE CALL FUNCTION OPERATOR ()
 //���������� ��������� ������ ������� ()
 //������������ ������� ��������� ������ ��� �������� ������� ��������� ��������
 //� ��������� �����������
 bool TFunc(cvector& Out, const rvector& Angle, bool Abs = ON) const;
 complex operator() (double Angle, bool Deg = DEG, bool Abs = ON) const;
 //������ ��������� �������� ����������� �� ������ ��������� ������ ���
 //�������������� �������������
 complex CalcSignal(bool PolarType) const;
 complex CalcNoise(bool PolarType) const;
 void CalcJammers(cvector& Jammers, bool PolarType) const;

 //������ ��������� �������� ����������� �� ������ ��������� ������ ���
 //��������������� �������������
 complex CalcStatSignal(bool PolarType) const;
 complex CalcStatNoise(bool PolarType) const;
 complex CalcStatJammer(bool PolarType) const;

 //*****************************************************************************
 // ������������� ������� ����� - ������ ���������� TBaseChannel
 //*****************************************************************************
 //���� ���������� ������� �� ������������ ������ ����� cin
 bool Edit();
 //���������� ��������� ����� (>>) ��� ����� ���������� ������
 //friend istream& operator >>(istream& in, TBaseChannel& BaseCh);
 //���������� ��������� ������ (<<) ��� ������ ���������� ������
 //friend ostream& operator <<(ostream& out, const TBaseChannel& BaseCh)
 //���������� �������������� ��������� ������������ operator =
 void operator =(const TBaseChannel& BaseCh);
 //���������� �������������� ��������� ������������ operator = ��������� ������
 //������� � ��� ��������� ����������� TInDataBaseCh
 void operator =(const TInDataBaseCh& InData);
 //-----------------------------------------------------------------------------
 // II. ���������� �������� ������-������� ������ TBaseChannel
 //-----------------------------------------------------------------------------
 private:
  void Reset(); //��������� ��������� ���� ������������� TBaseChannel
  //������ ������������� �������������� ����� - BeamFormer - ������ B
  void CalcBeamFormerFactor();
};
//����� ���������� ������ TBaseChannel
//The end of class TBaseChannel declaration

//****************************************************************************************
//            ���������� ������������ ������ - TPreProcessorSpaceFilterBaseCh
//�����  ���������  �����  ��������  ���  ������������� � �������������� ������� �������-
//������� ��������� �������� ������� ����������������� ����������� ������� � ����������
//������� �� ������ LAA. ����� ���������� ��������� ����� ��������������:
//1. ���������� ������� LAA. ������������ ����������� ������� LAA � ��������� ��������� �
//���������� ������������ ����������� (���������) ������ �������, � ���������� ������������
//��������������� ������� ������� � � ���������� ���������� ������ �������� � ��������
//����������� ���������������� �������� (� ������� ������, ����� ������ �������� ����) �
//������ ��� �������������.
//�������� ���������� ������� ���������. ���� ����� ������������ ����������� ������ ������-
//���� �������������� �����, ��� �������� ��� ����������� � BaseCh ������ ���������� ����-
//���, �� � �������� �������� ������� LAA, ������������ � �������� �������������� �������
//����������������� �������, ����� ������������ ��� ������ LAA. ���� BaseCh ���������� ������
//���� ����� LAA, �� � �������� �������������� ������� SpaceFilter with BaseCh �����,
//������������ � BaseCh ������������ ������, � ��� ��������� ������ LAA �����. ����� �������,
//��� ������������� ����� ������������ ���������������� ���� ������ BaseCh � AuxCh �����
//����� ����� ������� - ������������. ��� ����������� ����� ������������ ����������� �����-
//����� ������ �������� ��������������� �������� SpaceFilter ��� ������ ������� ����������
//������������� ������ LAA, ������� �� ������������ � �������� �������������� ������� �����-
//��. ����� �������, ������ ���������� ���������� ����������� ������ �������� �����������-
//���� �������� ������� �� ������ ������������ � AuxCh.
//2. ���������� ������������ ��������� ������ ����������������� �������. ������ ����� ����-
//�������� ������� TBaseChannel. ��� ������������ ��������� ������ ����� �������������� ��-
//������������ ����� (��� ����������� ���������� ������� LAA), ��� ��������� �����������
//�������� ��������� �������������� ��������� ���� � �������� -90 �� +90 �������� � �����-
//��� ������� ����, ������� ������� �� ���������� ������������ � BaseCh ������� LAA. �����
//�����, � �������� ��������� ������ ����� ���� ����������� ���� ������������ ����� LAA. �
//���� ������ ��������� �������������� ������������ ������ �������� ��������� LAA � �� �����
//���� ��������. ����� TBaseChannel ������������� ������������� � �������������� ��������
//�������������� ����������������� ������� � ���������� �������. ��� ������������� ������,
//�� ������������ �������� �������������� (��� ������������� �������������� �����) �������
//��������� ������ ��������. ��� �������������� ������, �� ������������ �������� ���������-
//����� ��������� ���������� ������. ���������� TBaseChannel ������������ �������� �������-
//������� ��� ��������� ������� � ����������� ����. ��� ��� �������� ������ � ���������� ���
//������������ ����� �������� ��� � �������������, ��� � � �������������� �������, �� ���
//��� �������� �������������� ���������.
//3. ������� ��������� ���������� ���������� ����������� ������ ���������������� ��������.
//� ��� ��������� ����������� ���������� ������ �������� AuxCh � ������ LAA, ������������
//� ������ �����, ������� ������� �� ������������ � AuxCh.
//���������� Signal Reception Directions Elimination Unit ������� �� ������������� ��� ���-
//����������� ������� ��������������. ��� ��� � �������� ������� ������ ������������� ����-
//�� ������� ������������ �������������� ������� ��������� �������, �������� ������ � ����-
//������� ���� ����� ����� ��� ��������, � �������� ������� ��� �������������� ������ - ���-
//���� ��������� �������, �������� ������ � ����������� ����, ������� ������������� ������
//LAA � �������� �����.
//������� ��������, ��� ������ ReceptDirectElimUnit �� ������ �� ����������� �������������-
//���� �������, ��� ��� �� ��� ���� ������������ ������ �������� ����� � AuxCh.
//� ������ ���������� �������� ������� ����������������� ���������������-���������������
//��������. � ���� ����� ���������������� ���������� ������ ����� ��������� ���� �� ������-
//��� ��������������� ������������ �������� �������:
//1. �����������-�������������� ���������� ������� - VP_SIGNAL;
//2. �������������-�������������� ���������� ������� - HP_SIGNAL;
//****************************************************************************************
class TPreProcessorSpaceFilterBaseCh {
//------------------------------------------------------------------------------
// ���������� ���������� ������-������ ������ TPreProcessorSpaceFilterBaseCh
//------------------------------------------------------------------------------
 protected:
  //������� ������ � ������� LAA �� ����� �������������� ����������� �����������������
  //������� � ���������� �������
  TInputSignal* OutLAA;
  //��������������� ������������ �������� ����������� ������������ ����������
  //���������������� ��������
  bool PolarType;
  //���������� ������������ ��������� ������ AdaptSpaceFilterBaseCh
  TBaseChannel BaseChannel;
  //****************************************************************************
  //���������� ������������ ����������� ���������� ������ �������� � �����������
  //DirectElim ���������������� �������� SpaceFilter - AuxChRDEUnit
  //****************************************************************************
  //������� (����������) ��������� ����� ���������� ����������� ������ ��������
  //��������������� �������� ����������������� �������
  rvector DirectElim; //����������� ����������
  bool lBaseChInRDEUnit; //����������� ��������� ������ � AuxChRDEUnit
  //��� ������� ���������� ��������� ���������� �������� ����������� ������:
  //1. MultiStageUnit - ���������������� �����;
  //2. SingleStageUnit - ��������������� �����.
  //��������� �� ������� ����� ��������� �� ���������� ������ �������� � �������� ������-
  //����� � �������������� ������� ���������� ���������������� �������� � �������� �������
  TAuxChSpaceRejectFilter* RDEUnit;
  //**************************************************************************************
  //������� ������� LAA (RDEChannel + AuxChannel), ������������ � �������� ������� �������
  //AuxChRDEUnit � ��������������� ������� ����������������� ������� ��� ����� ��������
  //������� LAA, ������������ ��� ������������ ��������� ������
  ivector LAACh;
//----------------------------------------------------------------------------------------
// I. ���������� ������������� ������-������� ������
//    TPreProcessorSpaceFilterBaseCh
//----------------------------------------------------------------------------------------
 public:
//*****************************************************************************
// ���������� ������������� � ����������� ������ TPreProcessorSpaceFilterBaseCh
//*****************************************************************************
 TPreProcessorSpaceFilterBaseCh();//���������� ������������ �� ���������
 //���������� ������������ � �����������
 TPreProcessorSpaceFilterBaseCh(TInputSignal* InSignal, bool Polar = VP_SIGNAL,
  const ivector& BaseCh = ivector(), double ScanAngle = 0, const ivector& AuxCh = ivector(),
  const rvector& Directs = rvector(), bool lBaseChInRDE = OFF, ivector& RDECh = ivector(),
  uint AuxChSRF = MultiStageUnit);
 TPreProcessorSpaceFilterBaseCh(TInputSignal* InSignal, bool Polar,
  const TInDataBaseCh& BaseChProp, const ivector& AuxCh = ivector(),
  const rvector& Directs = rvector(), bool lBaseChInRDE = OFF, ivector& RDECh = ivector(),
  uint AuxChSRF = MultiStageUnit);

 //���������� ������������ �����������
 TPreProcessorSpaceFilterBaseCh(const TPreProcessorSpaceFilterBaseCh& Property)
 {*this = Property;};
 //���������� ����������� ������
 ~TPreProcessorSpaceFilterBaseCh();
//******************************************************************************
// ������������� ������� ��� ������ � ������� TPreProcessorSpaceFilterBaseCh
//******************************************************************************
 //��������� ������� �������������� SpaceFilterBaseCh
 bool Set(TInputSignal* InSignal, bool Polar = VP_SIGNAL, const ivector& BaseCh = ivector(),
      double ScanAngle = 0, const ivector& AuxCh = ivector(), const rvector& Directs = rvector(),
      bool lBaseChInRDE = OFF,  ivector& RDECh = ivector(), uint AuxChSRF = MultiStageUnit);

 bool Set(TInputSignal* InSignal, bool Polar, const TInDataBaseCh& BaseChProp,
      const ivector& AuxCh = ivector(), const rvector& Directs = rvector(),
      bool lBaseChInRDE = OFF, ivector& RDECh = ivector(), uint AuxChSRF = MultiStageUnit);
 //�������� ����������������� ��������������:
 bool IsReady() const
  {return ((OutLAA == NULL) || (OutLAA->LAAElem() < 2) || (!BaseChannel.IsReady())) ? OFF : ON;};
  //��������� �� LAA, � ������� ��������� ������������� �������
 TInputSignal* GetLAA() const {return OutLAA;};
 //������������ LAA ������������ ����������� �����
 bool DispLAA() const {return (IsReady()) ? OutLAA->GetDisp() : EQUATOR;};
 //��������������� ������������ �������� �����������, ����������� ���������������
 bool Polarization() const {return PolarType;};
 //��������� ��������������� ������������ (VP_SIGNAL || HP_SIGNAL)
 bool SetPolar(bool Polar) {return PolarType = Polar;};
 //����������� ����������������� ������� � �������� �������
 ulong FilterSize() const {return (IsReady()) ? AuxChCount()+1 : 0L;};
 //���������� ������ �������������� � �������� �������
 ulong In() const {return (IsReady()) ? AuxChCount()+RDEChCount()+1 : 0L;};
 //���������� ������� �������������� � �������� �������
 ulong Out() const {return (IsReady()) ? AuxChCount()+1 : 0L;};
 //������������ �������� ����-������� ��������������
 bool TestSignal(cvector& V, double Direct, bool lBaseCh = ON, bool lAbs = ON) const;
 bool TestSignal(cmatrix& A, const rvector& Directs, bool lBaseCh = ON, bool lAbs = ON) const;
 //������������ ������� �������������� ������� ����-������� ��������������
 bool TestR(cmatrix& R, double Direct, bool lBaseCh = ON) const;
 //������������ ������� (Transfer function) �������������� ���������� LAA
 //� ���������������� �������� � �������� ������� /����� AuxCh + BaseChannel/
 bool TFunc(rmatrix& P, const TEvenCloseInterval& Sector, bool Norm = ON, bool dB = ON,
      double MinDb = -60.0) const;
 bool TFunc(cvector& Uout, double Direct, bool lAbs = ON) const;
 bool TFunc(cmatrix& Rout, double Direct) const;
 //���������� �������������� ������� ������� - AuxChannelCount
 ulong AuxChCount() const;
 //������������ �������� �������������� ������� �������
 ulong GetAuxCh(ivector& AuxCh) const;
 //-----------------------------------------------------------------------------
 //�������� BaseChannel - ��������� ������ ���������� LAA
 //-----------------------------------------------------------------------------
 //������������ ������� ��������� ������ (��������� ��������������)
 rvector& TF_BaseCh(rvector& P, const TEvenCloseInterval& Sector, bool Norm = ON,
  bool dB = ON, double MinDb = -60.0) const
  {if (!IsReady()) P.Resize(0L);
   else BaseChannel.DirectPattern(P,Sector,Norm,dB,MinDb);
   return P;
  };
 //���� ������������ ��������� ������
 double GetScanAngle() const {return BaseChannel.GetScanAngle();};
 //������� ������� LAA, ����������� �������� �����
 ulong GetBaseCh(ivector& BaseCh) const
 {BaseChannel.Channels(BaseCh); return BaseCh.Size();};
 //-----------------------------------------------------------------------------
 //�������� AuxChRDEUnit - ���������� ���������� �������� ����������� ������
 //���������� �������� ��������������� �������� ����������������� �������
 //-----------------------------------------------------------------------------
 //������������� ����� ������������ ����������� ���������� ������ ��������
 //���������������� �������� ����������������� ������� � ��������������
 bool UsageRDEUnit() const {return (DirectElim.Size() == 0L) ? OFF : ON;};
 //����������� ��������� ������ � AuxChRDEUnit
 bool IsBaseChInRDEUnit() const {return lBaseChInRDEUnit;};
 //���������� ������� ��� ����������� ������ �������� � �������������� �������
 ulong RDEChCount() const {return DirectElim.Size();};
 //������ ������� LAA � ��������� ������ (lBaseChInRDEUnit = ON), ������������
 //AuxChRDEUnit � �������� ������� �������.
 ulong GetRDECh(ivector& RDECh) const;
 //�������� ����������� ����������
 ulong GetDirectElim(rvector& Directs) const
 {Directs = DirectElim; return Directs.Size();};
 //��� AuxChRDEUnit
 uint RDEType() const {return (UsageRDEUnit()) ? RDEUnit->Type() : 0;};
 //������� ������������ AuxChRDEUnit
 bool RDEUnitFactors(cmatrix& W, bool lExtend = ON) const
 {if (!UsageRDEUnit()) {W.Set(0L,0L,false); return false;}
  RDEUnit->GetW(W,lExtend); return true;
 }
//***************************************************************************************
// ������������� ������� ������ TPreProcessorSpaceFilterBaseCh
// 1. ������� ����� - ������ ����������
//***************************************************************************************
 //��������� ������� ������ � ���� ���������� ������� string
 string& Properties(string& s, string& title, uint indent = 0) const;
 string& Polarization(string& s, string& title, uint indent = 0) const;
 string& BaseChProp(string& s, string& title, uint indent = 0) const
 {return BaseChannel.Properties(s,title,indent);};
 string& AuxChProp(string& s, string& title, uint indent = 0) const;
 string& RDEUnitProp(string& s, string& title, uint indent = 0) const;

 //���� ���������� ������� �� ������������ ������ ����� cin
 void Edit();
 //���������� ��������� ����� (>>) ��� ����� ���������� ������
 friend istream& operator >>(istream& in, TPreProcessorSpaceFilterBaseCh& Preprocessor);
 //���������� ��������� ������ (<<) ��� ������ ���������� ������
 friend ostream& operator <<(ostream& out, const TPreProcessorSpaceFilterBaseCh& Preprocessor)
 {string s; return out << Preprocessor.Properties(s,STR_NULL);};
 //���������� �������������� ��������� ������������ operator =
 void operator =(const TPreProcessorSpaceFilterBaseCh& Preprocessor);

//------------------------------------------------------------------------------
//II. ���������� �������� ������-������� ������ TPreProcessorSpaceFilterBaseCh
//------------------------------------------------------------------------------
 private:
  void Initial();//��������� ��������� ���������� ������ ��� ���������������
  void InitRDEUnit();//��������� ��������� AuxChRDEUnit
  void ResetRDEUnit();//����� ���� ������������� AuxChRDEUnit
  //��������� �������� ������� LAA, ������������ ��� �������������� ������
  //����������������� ����������� ������� � �������� �������
  void SetAuxCh(const ivector& Channel);
  //***************************************************************************
  //��������� ���������� AuxChRDEUnit
  //***************************************************************************
  //��������� ����� ������������� BaseChannel � AuxChRDEUnit
  bool SetUsageBaseChInRDEUnit(bool lUsageBaseCh)
  {return lBaseChInRDEUnit = lUsageBaseCh;};
  //����������� ����� ������� LAA, ������� ����� ���� ������������ � AuxChRDEUnit
  //��� ������� ������
  ulong AmountLAAChForRDEUnit() const;
  //������������ �������� ������� LAA, ������� ����� ���� ������������ �
  //AuxChRDEUnit ��� ������� ������
  ulong GetFreeChForRDEUnit(ivector& RDECh) const;
  //��������� ����������� ���������� ������ �������� ��������������� ��������
  bool SetDirectElim(const rvector& Directs);
  //��������������� �������� ������� LAA, ������������ ��� ������� ������
  //AuxChRDEUnit
  ulong AutoDefineRDECh(ivector& RDECh) const;
  //��������� �������� ������� LAA, ������������ ��� ������� ������ AuxChRDEUnit
  ulong SetRDECh(ivector& RDECh = ivector());
  //��������� ���� ���������� ����������� ������ �������� � �������� �����������
  //� �������������� ������� ����������� ����������������� ������� � BaseCh
  uint SetTypeRDEUnit(uint UnitType);

  void PolarEdit();//���������� ���� ��������������� ������������ InSignal
  void AuxChEdit();//���������� ���� �������� �������������� ������� �������
  void AuxChRDEUnitEdit();//���������� ���� ���������� ����� AuxChRDEUnit

//------------------------------------------------------------------------------
//III. ���������� ���������� ������-������� ������ TPreProcessorSpaceFilterBaseCh
//------------------------------------------------------------------------------
 protected:
  void Reset(); //����� ���� ������������� ������
};
//����� ���������� ������ TPreProcessorSpaceFilterBaseCh
//The end of class TPreProcessorSpaceFilterBaseCh declaration
//******************************************************************************
//              ���������� ������ - TAnalPreProcSFilterBaseCh
//�������������  ������ �������������� ��������� �������� ������� �������������-
//���� ����������� ������� � ���������� ������� �� ������ LAA.
//����� ��������� �������� ������ TPreProcessorSpaceFilter. �� ��������� �������
//����������� �������� ������. �������� ��� �������� �������� ������������ ����-
//��� ������� ������ ���������� �������� ����������� ��� ����������� ����������-
//�������  ������� � ���������� �������. � ������ ��������� �������������� ����-
//��������� ������� �������  ����������  �������� ������ ����������� ��� �������
//��������� ������ �������� � ��������� � ������� Jammer. ������� ������� �����-
//����  ������� �  �����������  ���� ����������� � �������� Signal � Noise ����-
//����������.
//******************************************************************************
class TAnalPreProcSFilterBaseCh : public TPreProcessorSpaceFilterBaseCh {
//------------------------------------------------------------------------------
// ���������� �������� ������-������ ������ TAnalPreProcSFilterBaseCh
//------------------------------------------------------------------------------
 private:
  // [BaseChannel] + RDECh == DirectElim.Size(),
  // [BaseChannel] -> �������� ����� ��������� � AuxChRDEUnit
  //----------------------------------------------------------------------------
  //������� ��������� ������� �� ����� � ������� LAA, ������������ � ����������-
  //����, � ��������� ������. ����������� ������� Signal:
  //1) AuxChRDEUnit = ON:  [BaseChannel] + RDECh + AuxCh + BaseChannel
  //2) AuxChRDEUnit = OFF: AuxCh + BaseChannel
  cvector Signal;
  //----------------------------------------------------------------------------
  //������� ����������� ���� �� ����� � ������� LAA, ������������ � ����������-
  //����, � ��������� ������. ����������� ������� Noise:
  //1) AuxChRDEUnit = ON:  [BaseChannel] + RDECh + AuxCh + BaseChannel
  //2) AuxChRDEUnit = OFF: AuxCh + BaseChannel
  cvector Noise;
  //----------------------------------------------------------------------------
  //������� ���������� �������� ������ �� ����� � ������� LAA, ������������ �
  //��������������, � ��������� ������. ����������� ������� Jammer:
  //���������� ����� �������:
  //1) AuxChRDEUnit = ON:  [BaseChannel] + RDECh + AuxCh + BaseChannel
  //2) AuxChRDEUnit = OFF: AuxCh + BaseChannel
  //���������� �������� �������:
  //OutLAA->ActiveJammer() - ���-�� �������� ���������� �����
  cmatrix Jammer;
//------------------------------------------------------------------------------
// I. ���������� ������������� ������-������� ������ TAnalPreProcSFilterBaseCh
//------------------------------------------------------------------------------
 public:
//*****************************************************************************
// ���������� ������������� � ����������� ������ TAnalPreProcSFilterBaseCh
//*****************************************************************************
 //���������� ������������ �� ���������
 TAnalPreProcSFilterBaseCh() : TPreProcessorSpaceFilterBaseCh() {};
 //���������� ������������ � �����������
 TAnalPreProcSFilterBaseCh(TInputSignal* InSignal, bool Polar = VP_SIGNAL,
  const ivector& BaseCh = ivector(), double ScanAngle = 0,
  const ivector& AuxCh = ivector(), const rvector& Directs = rvector(),
  bool lBaseChInRDE = OFF, ivector& RDECh = ivector(),
  uint AuxChSRF = MultiStageUnit) : TPreProcessorSpaceFilterBaseCh()
 {Set(InSignal,Polar,BaseCh,ScanAngle,AuxCh,Directs,lBaseChInRDE,RDECh,AuxChSRF);};

 TAnalPreProcSFilterBaseCh(TInputSignal* InSignal, bool Polar,
  const TInDataBaseCh& BaseChProp, const ivector& AuxCh = ivector(),
  const rvector& Directs = rvector(), bool lBaseChInRDE = OFF,
  ivector& RDECh = ivector(), uint AuxChSRF = MultiStageUnit) :
  TPreProcessorSpaceFilterBaseCh()
 {Set(InSignal,Polar,BaseChProp,AuxCh,Directs,lBaseChInRDE,RDECh,AuxChSRF);};

 //���������� ������������ �����������
 TAnalPreProcSFilterBaseCh(const TAnalPreProcSFilterBaseCh& Property)
 {*this = Property;};
 //���������� ����������� ������
 ~TAnalPreProcSFilterBaseCh() {};

//******************************************************************************
// ������������� ������� ������ TAnalPreProcSFilterBaseCh
//******************************************************************************
 //��������� ������� �������������� SpaceFilterBaseCh
 bool Set(TInputSignal* InSignal, bool Polar = VP_SIGNAL,
      const ivector& BaseCh = ivector(), double ScanAngle = 0,
      const ivector& AuxCh = ivector(), const rvector& Directs = rvector(),
      bool lBaseChInRDE = OFF,  ivector& RDECh = ivector(),
      uint AuxChSRF = MultiStageUnit)
 {bool lSuccess = TPreProcessorSpaceFilterBaseCh::Set(InSignal,Polar,BaseCh,
             ScanAngle,AuxCh,Directs,lBaseChInRDE,RDECh,AuxChSRF); SetSJNData();
  return lSuccess;
 };
 bool Set(TInputSignal* InSignal, bool Polar, const TInDataBaseCh& BaseChProp,
      const ivector& AuxCh = ivector(), const rvector& Directs = rvector(),
      bool lBaseChInRDE = OFF, ivector& RDECh = ivector(),
      uint AuxChSRF = MultiStageUnit)
 {bool lSuccess = TPreProcessorSpaceFilterBaseCh::Set(InSignal,Polar,BaseChProp,
                  AuxCh,Directs,lBaseChInRDE,RDECh,AuxChSRF); SetSJNData();
  return lSuccess;
 };

 // 1. ������� ����� - ������ ����������
 //���� ���������� ������� �� ������������ ������ ����� cin
 void Edit() {TPreProcessorSpaceFilterBaseCh::Edit(); SetSJNData();};
 //���������� ��������� ������ (<<) ��� ������ ���������� ������
 friend ostream& operator <<(ostream& out,
 const TAnalPreProcSFilterBaseCh& Preprocessor)
 {string s; return out << Preprocessor.Properties(s,STR_NULL);};
 //���������� �������������� ��������� ������������ operator =
 void operator =(const TAnalPreProcSFilterBaseCh& Preprocessor);
 //-----------------------------------------------------------------------------
 //    �������, ������������ ��� ������������� �������������� ���������� LAA
 //            c ���������������� ���������� �������� � �������� �������
 //-----------------------------------------------------------------------------
 //������ ��������� �������� ����������� ��� �������������
 bool Update(bool lSignal = ON, bool lJammer = ON, bool lNoise = ON);

 //-----------------------------------------------------------------------------
 // ����������� ���������� �������� ����������� �� ����� �������������� AdaptLAA
 //              / [RDEChannels] + AuxChannels + BaseChannel /
 //-----------------------------------------------------------------------------
 //������� ������� ��������� �������
 cvector& InSignal(cvector& U, bool lExtend = ON) const;
 //������� ������� ���������� �������� ������ (�� ������ ������)
 cmatrix& InJammer(cmatrix& A, bool lExtend = ON) const;
 //������� ������� ����������� ����
 cvector& InNoise(cvector& U, bool lExtend = ON) const;

 //-----------------------------------------------------------------------------
 //����������� ���������� �������� ����������� �� ������ �������������� AdaptLAA
 //             / [RDEChannels] + AuxChannels + BaseChannel /
 //-----------------------------------------------------------------------------
 //�������� ������� ��������� �������
 cvector& OutSignal(cvector& Uout, bool lExtend = ON) const;
 //�������� ������� ���������� �������� ������ (�� ������ ������)
 cmatrix& OutJammer(cmatrix& Aout, bool lExtend = ON) const;

 //-----------------------------------------------------------------------------
 //  �������� ��������� �������� ����������� �� �����/������ ��������� ������
 //                      �������������� AdaptLAA / BaseChannel /
 //�������� ��������� �������� ����������� �� ����� � ������ ��������� ������
 //                        �������������� AdaptLAA ���������
 //-----------------------------------------------------------------------------
 //������� �������� ��������� ������� � ��������� ������
 double InPsignal() const;
 //������� �������� ���������� �������� ������ � ��������� ������
 double InPjammer() const;
 //������� �������� ����������� ���� � ��������� ������
 double InPnoise() const;
 //-----------------------------------------------------------------------------
 //  �������� ��������� �������� ����������� �� ����� �������������� AdaptLAA
 //                 / [RDEChannels] + AuxChannels + BaseChannel /
 //-----------------------------------------------------------------------------
 //������� �������� ��������� �������
 rvector& InPsignal(rvector& P, bool lExtend = ON) const;
 //������� �������� ���������� �������� ������
 rvector& InPjammer(rvector& P, bool lExtend = ON) const;
 //������� �������� ����������� ����
 rvector& InPnoise(rvector& P, bool lExtend = ON) const;

 //-----------------------------------------------------------------------------
 //  �������� ��������� �������� ����������� �� ������ �������������� AdaptLAA
 //                 / [RDEChannels] + AuxChannels + BaseChannel /
 //-----------------------------------------------------------------------------
 //�������� �������� ��������� �������
 rvector& OutPsignal(rvector& P, bool lExtend = ON) const;
 //�������� �������� ���������� �������� ������
 rvector& OutPjammer(rvector& P, bool lExtend = ON) const;
 //�������� �������� ����������� ����
 rvector& OutPnoise(rvector& P, bool lExtend = ON) const;

 //������� �������������� ������� ��������� �������
 cmatrix& GetInRss(cmatrix& R, bool lExtend = ON) const;
 //������� �������������� ������� ���������� �������� ������
 cmatrix& GetInRjj(cmatrix& R, bool lExtend = ON) const;
 //������� �������������� ������� ����������� ����
 cmatrix& GetInRnn(cmatrix& R, bool lExtend = ON) const;
 //�������� �������������� ������� ��������� �������
 cmatrix& GetOutRss(cmatrix& R, bool lExtend = ON) const;
 //�������� �������������� ������� ���������� �������� ������
 cmatrix& GetOutRjj(cmatrix& R, bool lExtend = ON) const;
 //�������� �������������� ������� ����������� ����
 cmatrix& GetOutRnn(cmatrix& R, bool lExtend = ON) const;

 //������� �������������� ������� ��������� �������� �����������
 cmatrix& GetInR(cmatrix& R, bool lSignal = ON, bool lJammer = ON,
                 bool lNoise = ON, bool lExtend = ON) const;
 //�������� �������������� ������� ��������� �������� ����������� ��������������
 //����������� ����������������� �������
 cmatrix& GetOutR(cmatrix& R, bool lSignal = ON, bool lJammer = ON,
                  bool lNoise = ON, bool lExtend = ON) const;

//------------------------------------------------------------------------------
//II. ���������� �������� ������-������� ������ TAnalPreProcSFilterBaseCh
//------------------------------------------------------------------------------
 private:
  //��������� ������-������ ������ � ������� ����������� ������� �������
  //��������� �������, ���������� ������ � ����������� ����
  void SetSJNData();
}; //����� ���������� ������ TAnalPreProcSFilterBaseCh
//----------  The end of class TAnalPreProcSFilterBaseCh declaration  ----------

//******************************************************************************
//              ���������� ������ - TStatPreProcSFilterBaseCh
//�������������� ������ �������������� ��������� �������� ������� �������������-
//���� ����������� ������� � ���������� ������� �� ������ LAA.
//����� ��������� �������� ������ TPreProcessorSpaceFilter. �� ��������� �������
//����������� �������� ������. �������� ��� �������� �������� ������������ ����-
//��� ������� ������ ���������� �������� ����������� ��� ����������� ����������-
//������� ������� � ���������� �������. ������� ������� ��������� �������, ����-
//������ ������ � �����������  ���� ����������� � �������� Signal, Jammer, Noise
//��������������.
//******************************************************************************
class TStatPreProcSFilterBaseCh : public TPreProcessorSpaceFilterBaseCh {
//------------------------------------------------------------------------------
// ���������� �������� ������-������ ������ TStatPreProcSFilterBaseCh
//------------------------------------------------------------------------------
 private:
  // [BaseChannel] + RDECh == DirectElim.Size(),
  // [BaseChannel] -> �������� ����� ��������� � AuxChRDEUnit
  //----------------------------------------------------------------------------
  //������� ��������� ������� �� ����� � ������� LAA, ������������ � ����������-
  //����, � ��������� ������. ����������� ������� Signal:
  //1) AuxChRDEUnit = ON:  [BaseChannel] + RDECh + AuxCh + BaseChannel
  //2) AuxChRDEUnit = OFF: AuxCh + BaseChannel
  cvector Signal;
  //----------------------------------------------------------------------------
  //������� ����������� ���� �� ����� � ������� LAA, ������������ � ����������-
  //����, � ��������� ������. ����������� ������� Noise:
  //1) AuxChRDEUnit = ON:  [BaseChannel] + RDECh + AuxCh + BaseChannel
  //2) AuxChRDEUnit = OFF: AuxCh + BaseChannel
  cvector Noise;
  //----------------------------------------------------------------------------
  //������� ���������� �������� ������ �� ����� � ������� LAA, ������������ �
  //��������������, � ��������� ������. ����������� ������� Jammer:
  //���������� ����� �������:
  //1) AuxChRDEUnit = ON:  [BaseChannel] + RDECh + AuxCh + BaseChannel
  //2) AuxChRDEUnit = OFF: AuxCh + BaseChannel
  cvector Jammer;
//------------------------------------------------------------------------------
// I. ���������� ������������� ������-������� ������ TStatPreProcSFilterBaseCh
//------------------------------------------------------------------------------
 public:
//*****************************************************************************
// ���������� ������������� � ����������� ������ TStatPreProcSFilterBaseCh
//*****************************************************************************
 //���������� ������������ �� ���������
 TStatPreProcSFilterBaseCh() : TPreProcessorSpaceFilterBaseCh() {};
 //���������� ������������ � �����������
 TStatPreProcSFilterBaseCh(TInputSignal* InSignal, bool Polar = VP_SIGNAL,
  const ivector& BaseCh = ivector(), double ScanAngle = 0, const ivector& AuxCh = ivector(),
  const rvector& Directs = rvector(), bool lBaseChInRDE = OFF, ivector& RDECh = ivector(),
  uint AuxChSRF = MultiStageUnit) : TPreProcessorSpaceFilterBaseCh()
  {Set(InSignal,Polar,BaseCh,ScanAngle,AuxCh,Directs,lBaseChInRDE,RDECh,AuxChSRF);};

 TStatPreProcSFilterBaseCh(TInputSignal* InSignal, bool Polar,
  const TInDataBaseCh& BaseChProp, const ivector& AuxCh = ivector(),
  const rvector& Directs = rvector(), bool lBaseChInRDE = OFF, ivector& RDECh = ivector(),
  uint AuxChSRF = MultiStageUnit) : TPreProcessorSpaceFilterBaseCh()
 {Set(InSignal,Polar,BaseChProp,AuxCh,Directs,lBaseChInRDE,RDECh,AuxChSRF);};

 //���������� ������������ �����������
 TStatPreProcSFilterBaseCh(const TAnalPreProcSFilterBaseCh& Obj) {*this = Obj;};
 //���������� ����������� ������
 ~TStatPreProcSFilterBaseCh() {};

//******************************************************************************
// ������������� ������� ������ TStatPreProcSFilterBaseCh
// 1. ������� ����� - ������ ����������
//******************************************************************************
 //��������� ������� �������������� SpaceFilterBaseCh
 bool Set(TInputSignal* InSignal, bool Polar = VP_SIGNAL,
      const ivector& BaseCh = ivector(), double ScanAngle = 0,
      const ivector& AuxCh = ivector(), const rvector& Directs = rvector(),
      bool lBaseChInRDE = OFF,  ivector& RDECh = ivector(), uint AuxChSRF = MultiStageUnit)
 {bool lRes = TPreProcessorSpaceFilterBaseCh::Set(InSignal,Polar,BaseCh,
              ScanAngle,AuxCh,Directs,lBaseChInRDE,RDECh,AuxChSRF); SetSJNData();
  return lRes;
 };

 bool Set(TInputSignal* InSignal, bool Polar, const TInDataBaseCh& BaseChProp,
      const ivector& AuxCh = ivector(), const rvector& Directs = rvector(),
      bool lBaseChInRDE = OFF, ivector& RDECh = ivector(),
      uint AuxChSRF = MultiStageUnit)
 {bool lRes = TPreProcessorSpaceFilterBaseCh::Set(InSignal,Polar,BaseChProp,AuxCh,Directs,
              lBaseChInRDE,RDECh,AuxChSRF); SetSJNData();
  return lRes;
 };

 //���� ���������� ������� �� ������������ ������ ����� cin
 void Edit() {TPreProcessorSpaceFilterBaseCh::Edit(); SetSJNData();};
 //���������� ��������� ������ (<<) ��� ������ ���������� ������
 friend ostream& operator <<(ostream& out, const TStatPreProcSFilterBaseCh& Preprocessor)
 {string s; return out << Preprocessor.Properties(s,STR_NULL);};
 //���������� �������������� ��������� ������������ operator =
 void operator =(const TStatPreProcSFilterBaseCh& Preprocessor);
 //-----------------------------------------------------------------------------
 //    �������, ������������ ��� ������������� �������������� ���������� LAA
 //            c ���������������� ���������� �������� � �������� �������
 //-----------------------------------------------------------------------------
 bool Update(); //������ ��������� �������� ����������� ��� �������������

 //-----------------------------------------------------------------------------
 // ����������� ���������� �������� ����������� �� ����� �������������� AdaptLAA
 //              / [RDEChannels] + AuxChannels + BaseChannel /
 //-----------------------------------------------------------------------------
 //������� ������� ��������� �������
 cvector& InSignal(cvector& U, bool lExtend = ON) const;
 //������� ������� ���������� �������� ������ (�� ������ ������)
 cvector& InJammer(cvector& U, bool lExtend = ON) const;
 //������� ������� ����������� ����
 cvector& InNoise(cvector& U, bool lExtend = ON) const;

 //-----------------------------------------------------------------------------
 //����������� ���������� �������� ����������� �� ������ �������������� AdaptLAA
 //             / [RDEChannels] + AuxChannels + BaseChannel /
 //-----------------------------------------------------------------------------
 //�������� ������� ��������� �������
 cvector& OutSignal(cvector& Uout, bool lExtend = ON) const;
 //�������� ������� ���������� �������� ������ (�� ������ ������)
 cvector& OutJammer(cvector& Uout, bool lExtend = ON) const;
 //�������� ������� ����������� ����
 cvector& OutNoise(cvector& Uout, bool lExtend = ON) const;

 //-----------------------------------------------------------------------------
 //  �������� ��������� �������� ����������� �� �����/������ ��������� ������
 //                      �������������� AdaptLAA / BaseChannel /
 //�������� ��������� �������� ����������� �� ����� � ������ ��������� ������
 //                        �������������� AdaptLAA ���������
 //-----------------------------------------------------------------------------
 //������� �������� ��������� ������� � ��������� ������
 double InPsignal() const;
 //������� �������� ���������� �������� ������ � ��������� ������
 double InPjammer() const;
 //������� �������� ����������� ���� � ��������� ������
 double InPnoise() const;
 //-----------------------------------------------------------------------------
 //  �������� ��������� �������� ����������� �� ����� �������������� AdaptLAA
 //                 / [RDEChannels] + AuxChannels + BaseChannel /
 //-----------------------------------------------------------------------------
 //������� �������� ��������� �������
 rvector& InPsignal(rvector& P, bool lExtend = ON) const;
 //������� �������� ���������� �������� ������
 rvector& InPjammer(rvector& P, bool lExtend = ON) const;
 //������� �������� ����������� ����
 rvector& InPnoise(rvector& P, bool lExtend = ON) const;

 //-----------------------------------------------------------------------------
 //  �������� ��������� �������� ����������� �� ������ �������������� AdaptLAA
 //                 / [RDEChannels] + AuxChannels + BaseChannel /
 //-----------------------------------------------------------------------------
 //�������� �������� ��������� �������
 rvector& OutPsignal(rvector& P, bool lExtend = ON) const;
 //�������� �������� ���������� �������� ������
 rvector& OutPjammer(rvector& P, bool lExtend = ON) const;
 //�������� �������� ����������� ����
 rvector& OutPnoise(rvector& P, bool lExtend = ON) const;

//------------------------------------------------------------------------------
//II. ���������� �������� ������-������� ������ TStatPreProcSFilterBaseCh
//------------------------------------------------------------------------------
 private:
  //��������� ������-������ ������ � ������� ����������� ������� �������
  //��������� �������, ���������� ������ � ����������� ����
  void SetSJNData();
}; //����� ���������� ������ TStatPreProcSFilterBaseCh
//----------  The end of class TStatPreProcSFilterBaseCh declaration  ----------

//******************************************************************************
//���������� ������ - TASpaceFilterBaseCh - �������������  ������ �������������-
//����  �����������  ������� � ���������� ������� �� ������ LAA. ����� ���������
//��������  �������  TAnalPreProcSFilterBaseCh � TAnalInSignalProperty. �������-
//�������  �����������   ������  �����������, �  ������  �������, � ������������
//����������� �� ����� ����������������� ������� ���������� LAA � ������������ �
//��  �������������  ����������  �������������: ������� � ������������ ���������
//����, �����������  ��������������� ������� �������, �������������� � �� �����-
//��� ������������ �����  ����������  ������  �������� �  �������� ����������� �
//���������������  �������  �������. ���  ������������� ������ �����������������
//������� � ���������� (��������) ������� � �������� �������� ����������� ������-
//������ �������������� ������� ��������� �������, ����������  �������� ������ �
//����������� ����, � �����  ���������  �������  ����  ���������  ��� ����������
//���������  �������  ������������� �������. �����������  ��������������  ������
//���������  ��������  �������  ����� ���������� ��������������� ������� �������
//AuxChannel+1 (BaseChannel).
//��� ���������� ������������ ������� ������� ����� ��� ��������� ������������
//������ �������� ����������� (��� ��������� ������). ������ ����� ��������� �
//��������������  ������������  �������  ������  TPreProcessorSaceFilterBaseCh
//��������� ������������ ������� ����� ������������ ��������� ������ BaseChannel,
//��������������� ������� ������� AuxChannel, ����� AuxChRDEUnit, �������� �����-
//����� �� ��������� �� ����� � ������ �������, ������� �������������,  �������-
//������� �������� � �.�.
//��� ������������� ������ ������ ������� � ����� �������� ��������� �� �������
//����� �� ������������� ����� "weightsum.h" - TMultiInSingleOutWeightSummator.
//���  ����  �����������  ������������ ������������ � �������� �����������������
//������� �������������  ������������� ������ ������� ���������� �� ������� ���-
//���� � ����� �������. ������  ������� ��������� ����� ���������� ���� �� �����
//��  ������������ (��������������� � ����������������), �� ���������� ���������
//�������  ������������� (���������� ���������) � �� ������ ������������ ������-
//������ ���������� �������� (�������/����������, ���� ����� ������������ ��� ��
//�������������). ��� ��� ������� ��������� ������ ���������� ������ ����: ���
//��� ������ ���� �� ������� ������� (more than 1) � ����� �������. ����� ������
//���������  ������������  ������������� ��� ������ ����������� ���� ��������� �
//������������  �����������  AuxChannel+1. �����  ���������� ������� ������ ����
//�����������  ����������� �  �������������  ���������� ���������������� �������
//���  �� ��  ����������  ���������� (��������� �������� ������� ������������� �
//����������� �� �������������� ��������� ��������), ��� � �� �� ���������, ����-
//������ ��������� ������� ������������� � ������������ �� ������������ �������-
//���. ��������, ��� ������ ����������������� ������� ����������� � ������������
//�����  ��������� �������������� (������������ �������) � ����������� �� �����-
//���� �����. ���������� ��������������  ������� ����������  ������������  �����
//�������� (�������) �����, ������� ����� ���� ��������� ��������. ���������, ���
//�������� �������� ������� ������������� � ������� ���� (��������) LAA. � �����
//����������� ���������� ��� ������ ���������������� �������� ������������ �����
//������ �������� ����, ������������� ����������� AuxChRDEUnit.
//������������� ������������� ������ ���������� LAA � ���������������� ��������
//� �������� ������� ������������ �� ���� ����������. � ������ ������������� ��-
//����� ������������ ������� ����������� � ��� ���������� ������� ����������� ���
//��� ���������. �� ��������� ������������� ������� ����������� ��������� ������-
//������ ��������������  ���������� LAA, ������� ����� �������� �� ������ �����-
//���, ����� ���, ��  ��������  �����������, ������������  ����������  ������� �
//����������������� �������, ��������� ��������� � ������������ ������ �������,
//���������� ���������� ������� � �.�.
//******************************************************************************
class TASpaceFilterBaseCh:
 public TAnalPreProcSFilterBaseCh, public TAnalInSignalProperty {
//------------------------------------------------------------------------------
// ���������� ���������� ������-������ ������ TASpaceFilterBaseCh
//------------------------------------------------------------------------------
 protected:
  //��������� �� ������� ����������� ����� �������� ���������
  TMultiInSingleOutWeightSummator* Filter;//���������������� ���������� ������
  //�������������� ������� ��������� �������, ���������� ������ � �����������
  //���� �� ����� �������
  cmatrix Rss;//�������������� ������� ��������� �������
  cmatrix Rjj;//�������������� ������� ���������� ������
  cmatrix Rnn;//�������������� ������� ����������� ����
  //��������� ������� ��������� �������� ����������� �� ����� �����������������
  //����������� �������: Rss+Rjj+Rnn
  cmatrix R;
//------------------------------------------------------------------------------
// I. ���������� ������������� ������-������� ������ TASpaceFilterBaseCh
//------------------------------------------------------------------------------
 public:
//*****************************************************************************
// ���������� ������������� � ����������� ������ TASpaceFilterBaseCh
//*****************************************************************************
 //���������� ������������ �� ���������
 TASpaceFilterBaseCh() :
  TAnalPreProcSFilterBaseCh(), TAnalInSignalProperty() {Initial();};
 //���������� ������������ � �����������
 TASpaceFilterBaseCh(TInputSignal* InSignal) :
  TAnalPreProcSFilterBaseCh(InSignal), TAnalInSignalProperty() {Initial();};
 //���������� ������������ �����������
 TASpaceFilterBaseCh(TASpaceFilterBaseCh& SFilter) {*this = SFilter;};
 //���������� ����������� ������
 ~TASpaceFilterBaseCh() {Reset();};
//******************************************************************************
// ������������� ������� ��� ������ � ������� TASpaceFilterBaseCh
//******************************************************************************
 //����������� ����������������� �������������� �������
 bool IsPreProcessorReady() const {return TAnalPreProcSFilterBaseCh::IsReady();};
 //����������� ����������������� ����������������� �������
 bool IsSpaceFilterReady() const
 {return ((Filter != NULL) && (Filter->IsReady())) ? ON : OFF;};
 //����������� ����������������� ���� ���������� Linear Antenna Array
 bool IsReady() const
 {return ((IsPreProcessorReady()) && (IsSpaceFilterReady())) ? ON : OFF;};
 //-----------------------------------------------------------------------------
 //��������� ������������� �������� ���������
 //-----------------------------------------------------------------------------
 cmatrix& GetW(cmatrix& W) const
 {if (!IsReady()) W.Set(0L,0L,false);
  else Filter->GetW(W);
  return W;
 };

 //-----------------------------------------------------------------------------
 //   �������������� ������� ��������� �������� ����������� �� ����� AdaptLAA
 //                     /�� ����� ������������� AdaptLAA/
 //-----------------------------------------------------------------------------
 //������� �������������� ������� ��������� ������� /���� AdaptLAA/
 cmatrix& GetExtRss(cmatrix& A, bool lExtend = ON) const
 {return TAnalPreProcSFilterBaseCh::GetInRss(A,lExtend);};
 //������� �������������� ������� ���������� �������� ������ /���� AdaptLAA/
 cmatrix& GetExtRjj(cmatrix& A, bool lExtend = ON) const
 {return TAnalPreProcSFilterBaseCh::GetInRjj(A,lExtend);};
 //������� �������������� ������� ����������� ���� /���� AdaptLAA/
 cmatrix& GetExtRnn(cmatrix& A, bool lExtend = ON) const
 {return TAnalPreProcSFilterBaseCh::GetInRnn(A,lExtend);};

 //-----------------------------------------------------------------------------
 //�������������� ������� ��������� �������� ����������� �� ����� SpaceFilter
 //          /����� ������������� - ���� AdaptSpaceFilter with BaseCh/
 //-----------------------------------------------------------------------------
 //������� �������������� ������� ��������� ������� /���� SpaceFilter/
 cmatrix& GetInRss(cmatrix& A, bool lExtend = ON) const;
 //������� �������������� ������� ���������� �������� ������ /���� SpaceFilter/
 cmatrix& GetInRjj(cmatrix& A, bool lExtend = ON) const;
 //������� �������������� ������� ����������� ���� /���� SpaceFilter/
 cmatrix& GetInRnn(cmatrix& A, bool lExtend = ON) const;

 //-----------------------------------------------------------------------------
 //�������������� ������� ��������� �������� ����������� �� ������ SpaceFilter
 //-----------------------------------------------------------------------------
 //�������� �������������� ������� ��������� ������� /����� SpaceFilter/
 cmatrix& GetOutRss(cmatrix& A) const
 {if (!IsReady()) {A.Set(0L,0L,false); return A;}
  A = Rss; //��������� ������� ��������� ������� �� ����� �������
  Filter->TF_WSum(A); //��������� ������� ��������� ������� �� ������ �������
  return A;
 };
 //�������� �������������� ������� ���������� �������� ������ /����� SpaceFilter/
 cmatrix& GetOutRjj(cmatrix& A) const
 {if (!IsReady()) {A.Set(0L,0L,false); return A;}
  A = Rjj; //��������� ������� �������� ������ �� ����� �������
  Filter->TF_WSum(A); //��������� ������� �������� ������ �� ������ �������
  return A;
 };
 //�������� �������������� ������� ����������� ���� /����� SpaceFilter/
 cmatrix& GetOutRnn(cmatrix& A) const
 {if (!IsReady()) {A.Set(0L,0L,false); return A;}
  A = Rnn; //��������� ������� ����������� ���� �� ����� �������
  Filter->TF_WSum(A); //��������� ������� ����������� ���� �� ������ �������
  return A;
 };

 //������� �������������� ������� ��������� �������� �����������
 // /���� AdaptLAA - ���� �������������/
 cmatrix& GetExtR(cmatrix& A, bool lSignal = ON, bool lJammer = ON,
                  bool lNoise = ON, bool lExtend = ON) const
 {return TAnalPreProcSFilterBaseCh::GetInR(A,lSignal,lJammer,lNoise,lExtend);
 };
 //�������� �������������� ������� ��������� �������� ����������� ��������������
 //����������� ����������������� ������� /���� SpaceFilter/
 cmatrix& GetInR(cmatrix& A, bool lSignal = ON, bool lJammer = ON,
                 bool lNoise = ON, bool lExtend = ON) const;
 //�������� �������������� ������� ��������� �������� �����������
 //����������� ����������������� ������� /����� SpaceFilter/
 cmatrix& GetOutR(cmatrix& A, bool lSignal = ON, bool lJammer = ON,
                  bool lNoise = ON) const
 {if (!IsReady()) {A.Set(0L,0L,false); return A;}
  //��������� ������� �������� �����������
  GetInR(A,lSignal,lJammer,lNoise,ON);
  Filter->TF_WSum(A);//������� �������� ����������� �� ������ �������
  return A;
 };
 //������������ ������� (Transfer function) ���������� LAA � ����������������
 //�������� � �������� ������� /����� ��������� ������/
 rvector& TFunc(rvector& P, const TEvenCloseInterval& Sector, bool Norm = ON,
                bool Db = ON, double MinDb = -60.0) const;
 //������������ ������� (Transfer function) �������������� ���������� LAA
 //� ���������������� �������� � �������� ������� /����� AuxCh + BaseChannel/
 bool TF_Preprocessor(rmatrix& P, const TEvenCloseInterval& Sector,
                      bool Norm = ON, bool Db = ON, double MinDb = -60.0) const
 {return TAnalPreProcSFilterBaseCh::TFunc(P,Sector,Norm,Db,MinDb);};
 //������������� ������������ ������� ���������� LAA ��� �������� ������ ��������
 //������ � ����������� ���� (��������� ��������)
 bool TF_Potential(rvector& P, double Time, const TEvenCloseInterval& Sector,
                   bool Norm = ON, bool dB = ON, double MindB = -60.0);
 //----------------------------------------------------------------------------------
 //�������, ������������ ��� ������������� ������ TASpaceFilterBaseCh
 //----------------------------------------------------------------------------------
 //��������� ��������� ���������� AdaptLAA ��� �������������: ��������� ���������
 //������� ������������� ����������������� �������
 bool SimulateInit()
 {return (IsReady()) ? Filter->SimulateInit() : OFF;};

 //������ �������������� ������ ��������� �������� ����������� ��� �������������
 //������ ����������� �������
 bool Update(double Time);

 //��������� ������� ������������� ����������������� �������
 bool Tuning() {return (IsReady()) ? Filter->Tuning(R) : OFF;};

 //������ ��������� �� ����� ���������� LAA /�������� �����/
 //�������� ��������� ������� �� ����� �������
 double InPSignal() const
 {ulong n = Rss.Rows()-1; return (IsReady()) ? abs(Rss.Get(n,n)) : 0;};
 //�������� �������� ���������� ������ �� ����� �������
 double InPJammer() const
 {ulong n = Rjj.Rows()-1; return (IsReady()) ? abs(Rjj.Get(n,n)) : 0;};
 //�������� ����������� ���� �� ����� �������
 double InPNoise() const
 {ulong n = Rnn.Rows()-1; return (IsReady()) ? abs(Rnn.Get(n,n)) : 0;};

 //������ ��������� �� ������ ���������� LAA /�������� �����/
 //�������� ��������� ������� �� ������ �������
 double OutPSignal() const
 {if (!IsReady()) return 0.0;//���������� ������ ����������������
  cmatrix A; GetOutRss(A);//������������ �������� ������� ��������� �������
  ulong n = A.Rows()-1;
  return abs(A.Get(n,n));//�������� ��������� ������� �� ������ �������
 };
 //�������� �������� ������ �� ������ �������
 double OutPJammer() const
 {if (!IsReady()) return 0.0;//���������� ������ ����������������
  cmatrix A; GetOutRjj(A);//������������ �������� ������� �������� ������
  ulong n = A.Rows()-1;
  return abs(A.Get(n,n));//�������� �������� ������ �� ������ �������
 };
 //�������� ����������� ���� �� ������ �������
 double OutPNoise() const
 {if (!IsReady()) return 0.0;//���������� ������ ����������������
  cmatrix A; GetOutRnn(A);//������������ �������� ������� ����������� ����
  ulong n = A.Rows()-1;
  return abs(A.Get(n,n));//�������� ����������� ���� �� ������ �������
 };

 //-----------------------------------------------------------------------------
 // �������� ��������� �������� ����������� �� ����� Adaptive LAA
 // / [RDEChannels] + AuxChannels + BaseChannel /
 //-----------------------------------------------------------------------------
 //������� �������� ��������� �������
 rvector& ExtPsignal(rvector& P, bool lExtend = ON) const
 {if (IsReady()) TAnalPreProcSFilterBaseCh::InPsignal(P, lExtend);
  else P.Resize(0);
  return P;
 };
 //������� �������� ���������� �������� ������
 rvector& ExtPjammer(rvector& P, bool lExtend = ON) const
 {if (IsReady()) TAnalPreProcSFilterBaseCh::InPjammer(P, lExtend);
  else P.Resize(0);
  return P;
 };
 //������� �������� ����������� ����
 rvector& ExtPnoise(rvector& P, bool lExtend = ON) const
 {if (IsReady()) TAnalPreProcSFilterBaseCh::InPnoise(P, lExtend);
  else P.Resize(0);
  return P;
 };
 //-----------------------------------------------------------------------------
 //  �������� ��������� �������� ����������� �� ������ �������������� AdaptLAA
 //  /���� ����������� �������/  / [RDEChannels] + AuxChannels + BaseChannel /
 //-----------------------------------------------------------------------------
 //�������� ��������� ������� �� ����� �������
 rvector& InPsignal(rvector& P, bool lExtend = ON) const;
 //�������� ���������� �������� ������ �� ����� �������
 rvector& InPjammer(rvector& P, bool lExtend = ON) const;
 //�������� ����������� ���� �� ����� �������
 rvector& InPnoise(rvector& P, bool lExtend = ON) const;

 //-----------------------------------------------------------------------------
 //  �������� ��������� �������� ����������� �� ������ Adaptive LAA
 //  /����� ����������� �������/  / [RDEChannels] + AuxChannels + BaseChannel /
 //  ��� ������ ������� ������� /��� ������������� �������� ����������������
 //  ���������/
 //-----------------------------------------------------------------------------
 //�������� �������� ��������� �������
 rvector& OutPsignal(rvector& P) const;
 //�������� �������� ���������� �������� ������
 rvector& OutPjammer(rvector& P) const;
 //�������� �������� ����������� ����
 rvector& OutPnoise(rvector& P) const;
 //-----------------------------------------------------------------------------
 //������ ������������� �������� �������� ���������� �������� ������ � �������-
 //���� ���� ����������� ������� �� ������� �������������� �������:
 // PotentialPout = |Rjn|/|AuxRjn|,
 // |AuxRjn| - ������������ �������������� ������� ������ � ����������� ���� ���
 //            ��������������� ������� �������;
 // |Rjn| - ������������ �������������� ������� ������ � ����������� ���� ���
 //         ��������� � ��������������� ������� �������
 //-----------------------------------------------------------------------------
 double PotentialOutPjn() const
 {if (!IsReady()) return 0; //���������� ������ ����������������
  cmatrix R, Raux;
  GetInR(R,OFF,ON,ON,ON); GetInR(Raux,OFF,ON,ON,OFF);
  return abs(R.DET_UnSymmetric())/abs(Raux.DET_UnSymmetric());
 };
 //-----------------------------------------------------------------------------
 //������ �������������� ������������ ���������� ����������� ������� �� �������
 //�������������� ������� ������ (���������� �������� ������ + ���������� ���)
 // PotentialKsup = 10*log[(Pin*|AuxRjn|)/|Rjn|] [dB],
 // Pin - �������� ������ � ����������� ���� �� ����� ��������� ������;
 // |AuxRjn| - ������������ �������������� ������� ������ � ����������� ���� ���
 //            ��������������� ������� �������;
 // |Rjn| - ������������ �������������� ������� ������ � ����������� ���� ���
 //         ��������� � ��������������� ������� �������
 //-----------------------------------------------------------------------------
 double PotentialKsup() const
 {if (!IsReady()) return 0; //���������� ������ ����������������
  double InPjn = InPJammer() + InPNoise();
  double OutPjn = PotentialOutPjn();
  return 10.0*log10(InPjn/OutPjn);
 };

 //-----------------------------------------------------------------------------
 // ������� ����� - ������ ���������� ������ TASpaceFilterBaseCh
 //-----------------------------------------------------------------------------
 //��������� ������� ������ � ���� ���������� ������� string
 string& Properties(string& s, string& title, uint indent = 0) const;

 //���� ���������� ������� �� ������������ ������ ����� cin
 void Edit();
 //�������������� ������� ����������������� �������
 void EditFilter();
 //���������� ��������� ����� (>>) ��� ����� ���������� ������
 friend istream& operator >>(istream& in, TASpaceFilterBaseCh& SFilter);
 //���������� ��������� ������ (<<) ��� ������ ���������� ������
 friend ostream& operator <<(ostream& out, const TASpaceFilterBaseCh& SFilter)
 {string s; return out << SFilter.Properties(s,STR_NULL);};
 //���������� �������������� ��������� ������������ operator =
 void operator =(const TASpaceFilterBaseCh& SFilter);
//---------------------------------------------------------------------------------------
//II. ���������� ���������� ������-������� ������ TASpaceFilterBaseCh
//---------------------------------------------------------------------------------------
 protected:
  //��������� ������������� ������-������ ������ ��� ���������������
  void Initial()
  {Filter = NULL; Rss = cmatrix(); Rjj = cmatrix(); Rnn = cmatrix(); R = cmatrix();};
  void Reset() //����� ���� ������������� ������
  {if (Filter != NULL) {delete Filter; Filter = NULL;}
   Rss = cmatrix(); Rjj = cmatrix(); Rnn = cmatrix(); R = cmatrix();
   TAnalPreProcSFilterBaseCh::Reset();
   TAnalInSignalProperty::Reset();
  };
  //��������� ����������� ������� � �������� �������������� ������
  void Set()
  {ulong size = FilterSize();
   if (Filter != NULL) Filter->SetInput(size);
   Rss.Set(size,C_ZERO); Rjj.Set(size,C_ZERO); Rnn.Set(size,C_ZERO); R.Set(size,C_ZERO);
  };
};
//����� ���������� ������ TASpaceFilterBaseCh
//The end of class TASpaceFilterBaseCh declaration

//******************************************************************************
//���������� ������ - TSpaceFilterBaseCh - ��������������  ������ �������������-
//���� ����������� ������� � ���������� ������� ��  ������ LAA. �����  ���������
//��������  ������ TStatPreProcSFilterBaseCh.  �������������� ����������� ������
//�����������, � ������ �������, � ������������ ����������� �� ����� ����������-
//�������  �������  ���������� LAA �  ������������ � �� ������������� ����������
//�������������: ������� � ������������ ��������� ����, ����������� ������������-
//���  ������� �������, �������������� � �� �������� ������������ ����� �������-
//���  ������ �������� � �������� ����������� � ��������������� ������� �������.
//���  ���������������  ������ ����������������� ������� � ���������� (��������)
//������� � �������� �������� ����������� ������������ ������� ��������� �������,
//����������  ��������  ������ � ����������� ����, � ����� ��������� ������ ����
//���������  ��� ���������� ��������� ������� ������������� �������. �����������
//��������  ���������  �������� ������� ����� ���������� ��������������� �������
//������� AuxChannel+1 (BaseChannel).
//��� ���������� ������������ ������� ������� ����� ��� ��������� ������������
//������ �������� ����������� (��� ��������� ������). ������ ����� ��������� �
//��������������  ������������  �������  ������  TPreProcessorSaceFilterBaseCh
//��������� ������������ ������� ����� ������������ ��������� ������ BaseChannel,
//��������������� ������� ������� AuxChannel, ����� AuxChRDEUnit, �������� �����-
//����� �� ��������� �� ����� � ������ �������, ������� �������������,  �������-
//������� �������� � �.�.
//��� ������������� ������ ������ ������� � ����� �������� ��������� �� �������
//����� �� ������������� ����� "weightsum.h" - TMultiInSingleOutWeightSummator.
//���  ����  �����������  ������������ ������������ � �������� �����������������
//������� ������������� �������������� ������ ������� ���������� �� ������� ���-
//���� � ����� �������. ������  ������� ��������� ����� ���������� ���� �� �����
//��  ������������ (��������������� � ����������������), �� ���������� ���������
//�������  ������������� (���������� ���������) � �� ������ ������������ ������-
//������ ���������� �������� (�������/����������, ���� ����� ������������ ��� ��
//�������������). ��� ��� ������� ��������� ������ ���������� ������ ����: ���
//��� ������ ���� �� ������� ������� (more than 1) � ����� �������. ����� ������
//���������  ������������  ������������� ��� ������ ����������� ���� ��������� �
//������������  �����������  AuxChannel+1. �����  ���������� ������� ������ ����
//�����������  ����������� �  �������������  ���������� ���������������� �������
//���  �� ��  ����������  ���������� (��������� �������� ������� ������������� �
//����������� �� �������������� ��������� ��������), ��� � �� �� ���������, ����-
//������ ��������� ������� ������������� � ������������ �� ������������ �������-
//���. ��������, ��� ������ ����������������� ������� ����������� � ������������
//�����  ��������� �������������� (������������ �������) � ����������� �� �����-
//���� �����. ���������� ��������������  ������� ����������  ������������  �����
//�������� (�������) �����, ������� ����� ���� ��������� ��������. ���������, ���
//�������� �������� ������� ������������� � ������� ���� (��������) LAA. � �����
//����������� ���������� ��� ������ ���������������� �������� ������������ �����
//������ �������� ����, ������������� ����������� AuxChRDEUnit.
//��������������  �������������  ����������  LAA  � ����������������  �������� �
//��������  �������  ������ ������������ �� ����� ���������� � ������������ ���-
//���������� ������ ����������. ������������� ���������� ���������� ������ 20.
//���  ������ ��������� ����������, ��� ������ ���������� ���������� ����������-
//���� � ��������������� �������������. � ������  �������������  ������� ������-
//������ ������� ����������� � ��� ���������� ������� �����������  ��� ��� ����-
//�����. ���  ��� ���  ��������������  ������������� ������ ���� ��������� �����
//����������, �� ������� ������������ ����������� ������� �� ��������� ���� ���-
//��� ���������� ����������� � ��������� � ������� AvW. �� ����������� ���������
//������� ������������� ����������� ������������ �������������� ���������� LAA.
//�� ��������� ������������� ������� ����������� ��������� ������������ �������-
//�������  ���������� LAA, ������� ����� �������� �� ������ ��������, ����� ���,
//��  ��������  �����������, ������������ ���������� ������� � �����������������
//�������, ��������� ��������� � ������������ ������ �������, ���������� �������-
//��� ������� � �.�.
//******************************************************************************
class TSpaceFilterBaseCh: public TStatPreProcSFilterBaseCh {
//------------------------------------------------------------------------------
// ���������� ���������� ������-������ ������ TSpaceFilterBaseCh
//------------------------------------------------------------------------------
 protected:
  //��������� �� ������� ����������� ����� �������� ���������
  TMultiInSingleOutWeightSummator* Filter;//���������������� ���������� ������
  //������� ��������� �������, ���������� ������ � ����������� ���� �� ����� �������
  cvector Us;//������� ������� ��������� �������
  cvector Ujam;//������� ������� ���������� ������
  cvector Unoise;//������� ������� ����������� ����
  //��������� ������� ��������� �������� ����������� �� ����� �����������������
  //����������� �������: Us+Ujam+Unoise
  cvector Usum;
  //����������� �� ����������� � ���� ������������� ������� ������������
  cmatrix AvW;
//------------------------------------------------------------------------------
// I. ���������� ������������� ������-������� ������ TSpaceFilterBaseCh
//------------------------------------------------------------------------------
 public:
//*****************************************************************************
// ���������� ������������� � ����������� ������ TSpaceFilterBaseCh
//*****************************************************************************
 //���������� ������������ �� ���������
 TSpaceFilterBaseCh() : TStatPreProcSFilterBaseCh() {Initial();};
 //���������� ������������ � �����������
 TSpaceFilterBaseCh(TInputSignal* InSignal) :
  TStatPreProcSFilterBaseCh(InSignal) {Initial();};
 //���������� ������������ �����������
 TSpaceFilterBaseCh(TSpaceFilterBaseCh& SFilter) {*this = SFilter;};
 //���������� ����������� ������
 ~TSpaceFilterBaseCh() {Reset();};
//******************************************************************************
// ������������� ������� ��� ������ � ������� TSpaceFilterBaseCh
//******************************************************************************
 //����������� ����������������� �������������� �������
 bool IsPreProcessorReady() const {return TStatPreProcSFilterBaseCh::IsReady();};
 //����������� ����������������� ����������������� �������
 bool IsSpaceFilterReady() const
 {return ((Filter != NULL) && (Filter->IsReady())) ? ON : OFF;};
 //����������� ����������������� ���� ���������� Linear Antenna Array
 bool IsReady() const
 {return ((IsPreProcessorReady()) && (IsSpaceFilterReady())) ? ON : OFF;};
 //-----------------------------------------------------------------------------
 //��������� ����������� ������������� �������� ���������
 //-----------------------------------------------------------------------------
 cmatrix& GetW(cmatrix& W) const
 {if (!IsReady()) W.Set(0L,0L,false);
  else W = AvW;
  return W;
 };
 //-----------------------------------------------------------------------------
 //��������� ����������� ������������� AvW � ������� ��������
 //-----------------------------------------------------------------------------
 bool SetW()
 {if (!IsReady()) return false;
  return Filter->SetW(AvW);
 };
 //������������ ������� (Transfer function) ���������� LAA � ����������������
 //�������� � �������� ������� /����� ��������� ������/
 rvector& TFunc(rvector& P, const TEvenCloseInterval& Sector, bool Norm = ON,
                bool Db = ON, double MinDb = -60.0) const;
 //������������ ������� (Transfer function) �������������� ���������� LAA
 //� ���������������� �������� � �������� ������� /����� AuxCh + BaseChannel/
 bool TF_Preprocessor(rmatrix& P, const TEvenCloseInterval& Sector,
                      bool Norm = ON, bool Db = ON, double MinDb = -60.0) const
 {return TStatPreProcSFilterBaseCh::TFunc(P,Sector,Norm,Db,MinDb);};

 //-----------------------------------------------------------------------------
 //�������, ������������ ��� ������������� ������ TSpaceFilterBaseCh
 //-----------------------------------------------------------------------------
 //��������� ��������� ���������� AdaptLAA ��� �������������: ��������� �������-
 //�� ������� ������������� ����������������� �������
 bool SimulateInit()
 {return (IsReady()) ? Filter->SimulateInit() : OFF;};

 //������ �������������� ������ ��������� �������� ����������� ��� �������������
 //������ ����������� �������
 bool Update();

 //��������� ������� ������������� ����������������� �������
 bool Tuning() {return (IsReady()) ? Filter->Tuning(Usum) : OFF;};

 //���������� ������� ������������� ����������������� �������
 bool AverageW(uint Realization);

 //������ ��������� �� ����� ���������� LAA /�������� �����/
 //�������� ��������� ������� �� ����� �������
 double InPSignal() const
 {return (IsReady()) ? norm(Us.Get(Us.Size()-1)) : 0;};
 //�������� �������� ���������� ������ �� ����� �������
 double InPJammer() const
 {return (IsReady()) ? norm(Ujam.Get(Ujam.Size()-1)) : 0;};
 //�������� ����������� ���� �� ����� �������
 double InPNoise() const
 {return (IsReady()) ? norm(Unoise.Get(Unoise.Size()-1)) : 0;};

 //������ ��������� �� ������ ���������� LAA /�������� �����/
 //�������� ��������� ������� �� ������ �������
 double OutPSignal() const
 {if (!IsReady()) return 0.0;//���������� ������ ����������������
  cvector V = Us;//������������ ������� ������� ��������� �������
  return Filter->TF_WSum(V);//�������� ��������� ������� �� ������ �������
 };
 //�������� ���������� �������� ������ �� ������ �������
 double OutPJammer() const
 {if (!IsReady()) return 0.0;//���������� ������ ����������������
  cvector V = Ujam;//������������ ������� ������� ���������� ������
  return Filter->TF_WSum(V);//�������� ���������� ������ �� ������ �������
 };
 //�������� ����������� ���� �� ������ �������
 double OutPNoise() const
 {if (!IsReady()) return 0.0;//���������� ������ ����������������
  cvector V = Unoise;//������������ ������� ������� ����������� ����
  return Filter->TF_WSum(V);//�������� ����������� ���� �� ������ �������
 };

 //-----------------------------------------------------------------------------
 // �������� ��������� �������� ����������� �� ����� Adaptive LAA
 // / [RDEChannels] + AuxChannels + BaseChannel /
 //-----------------------------------------------------------------------------
 //������� �������� ��������� �������
 rvector& ExtPsignal(rvector& P, bool lExtend = ON) const
 {if (IsReady()) TStatPreProcSFilterBaseCh::InPsignal(P, lExtend);
  else P.Resize(0);
  return P;
 };
 //������� �������� ���������� �������� ������
 rvector& ExtPjammer(rvector& P, bool lExtend = ON) const
 {if (IsReady()) TStatPreProcSFilterBaseCh::InPjammer(P, lExtend);
  else P.Resize(0);
  return P;
 };
 //������� �������� ����������� ����
 rvector& ExtPnoise(rvector& P, bool lExtend = ON) const
 {if (IsReady()) TStatPreProcSFilterBaseCh::InPnoise(P, lExtend);
  else P.Resize(0);
  return P;
 };
 //-----------------------------------------------------------------------------
 //  �������� ��������� �������� ����������� �� ������ �������������� AdaptLAA
 //  /���� ����������� �������/  / [RDEChannels] + AuxChannels + BaseChannel /
 //-----------------------------------------------------------------------------
 //�������� ��������� ������� �� ����� �������
 rvector& InPsignal(rvector& P, bool lExtend = ON) const;
 //�������� ���������� �������� ������ �� ����� �������
 rvector& InPjammer(rvector& P, bool lExtend = ON) const;
 //�������� ����������� ���� �� ����� �������
 rvector& InPnoise(rvector& P, bool lExtend = ON) const;

 //-----------------------------------------------------------------------------
 //  �������� ��������� �������� ����������� �� ������ Adaptive LAA
 //  /����� ����������� �������/  / [RDEChannels] + AuxChannels + BaseChannel /
 //  ��� ������ ������� ������� /��� ������������� �������� ����������������
 //  ���������/
 //-----------------------------------------------------------------------------
 //�������� �������� ��������� �������
 rvector& OutPsignal(rvector& P) const
 {if (!IsReady()) {P.Resize(0); return P;} //���������� ������ ����������������
  cvector V = Us;
  return Filter->TF_WSum(P,V);//�������� ��������� ������� �� ������� �������
 };
 //�������� �������� ���������� �������� ������
 rvector& OutPjammer(rvector& P) const
 {if (!IsReady()) {P.Resize(0); return P;} //���������� ������ ����������������
  cvector V = Ujam;
  return Filter->TF_WSum(P,V);//�������� ���������� ������ �� ������� �������
 };
 //�������� �������� ����������� ����
 rvector& OutPnoise(rvector& P) const
 {if (!IsReady()) {P.Resize(0); return P;} //���������� ������ ����������������
  cvector V = Unoise;
  return Filter->TF_WSum(P,V);//�������� ����������� ���� �� ������� �������
 };

 //-----------------------------------------------------------------------------
 // ������� ����� - ������ ���������� ������ TSpaceFilterBaseCh
 //-----------------------------------------------------------------------------
 //��������� ������� ������ � ���� ���������� ������� string
 string& Properties(string& s, string& title, uint indent = 0) const;
 //���� ���������� ������� �� ������������ ������ ����� cin
 void Edit();
 //�������������� ������� ����������������� �������
 void EditFilter();
 //���������� ��������� ����� (>>) ��� ����� ���������� ������
 friend istream& operator >>(istream& in, TSpaceFilterBaseCh& SFilter);
 //���������� ��������� ������ (<<) ��� ������ ���������� ������
 friend ostream& operator <<(ostream& out, const TSpaceFilterBaseCh& SFilter)
 {string s; return out << SFilter.Properties(s,STR_NULL);};
 //���������� �������������� ��������� ������������ operator =
 void operator =(const TSpaceFilterBaseCh& SFilter);
//------------------------------------------------------------------------------
//II. ���������� ���������� ������-������� ������ TSpaceFilterBaseCh
//------------------------------------------------------------------------------
 protected:
  //��������� ������������� ������-������ ������ ��� ���������������
  void Initial()
  {Filter = NULL; AvW =cmatrix();
   Us = cvector(); Ujam = cvector(); Unoise = cvector(); Usum = cvector();
  };
  void Reset() //����� ���� ������������� ������
  {if (Filter != NULL) {delete Filter; Filter = NULL;}
   Us = cvector(); Ujam = cvector(); Unoise = cvector(); Usum = cvector();
   AvW =cmatrix();
   TStatPreProcSFilterBaseCh::Reset();
  };
  //��������� ����������� ������� � �������� �������� ��������� �������� �������
  void Set()
  {ulong size = FilterSize();
   AvW = cmatrix();
   if (Filter != NULL) {Filter->SetInput(size); Filter->GetW(AvW);}
   Us.Resize(size); Ujam.Resize(size); Unoise.Resize(size); Usum.Resize(size);
   Us = C_ZERO; Ujam = C_ZERO; Unoise = C_ZERO; Usum = C_ZERO;
  };
};
//����� ���������� ������ TSpaceFilterBaseCh
//The end of class TSpaceFilterBaseCh declaration

//--------------------- The end of file "adaptlaa.h" ---------------------------
#endif //���������� ���������� ������������� ����� "ADAPTLAA.H"

