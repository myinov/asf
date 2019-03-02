//input.h
//���������� ������ TInputSignal - ������� ������
//Copyright (c) JohnSoft 1998. All rights reserved. Borland C++ 5.01.
//Initial date: November,8,1998. Final date:
//������������ ���� input.h ��������� ����� TInputSignal, ������� ���������
//������� ������� �����������, ����������� �� ���� ������������������� �����
//���������� �������� �������� �������. ������� ������ ������������ �� ����
//N-������ ������ ���������� ����������� �������� ���������� � ������ ������� t
//��������� �������, ��������� ������ � ����������� ���� ��������� ������ �� ��-
//���� �������� �������� �������, ��� N - ���������� �������� ������� ����������
//��. ���������� ��������� �������, ��������� ������ � ���� ����������� �� ���
//��������������� ������������ - ������������ � ��������������. ����� TInputSignal
//���������� ����� �������������� ������������ ������� ������ ExtSignal � ������
//TLinearArray, ��� ��� ���������� �������� ���������� �� ������ ��, ������������
//���������� ���� ���� �������.

#if !defined(__INPUT_H)
#define __INPUT_H

#include "exsignal.h" //������������ ����, ����������� �������� �������
#include "linarray.h" //������������ ����, �������� �������� �������
//#include "mathlib.h"  //������������ ���� � ����������� ��������� �����

#define CONSOLE 1     //� ��������� ������������ ���������� ����-�����

//���������� ��������������� ���� ����������� ��������
const bool VP_SIGNAL = true;//������ � ������������ ������������
const bool HP_SIGNAL = false;//������ � �������������� ������������

//******************************************************************************
// ���������� ������ - TInputSignal - ������� ������ (������� �����������)
//******************************************************************************
class TInputSignal : public ExtSignal, public TLinearArray {
//------------------------------------------------------------------------------
// ���������� ���������� ������-������ ������ TInputSignal
//------------------------------------------------------------------------------
protected:
 //������� ������ � ������������ ������������
 cvector VPSignal;
 cmatrix VPJammer;
 cvector VPNoise;
 //������� ������ � �������������� ������������
 cvector HPSignal;
 cmatrix HPJammer;
 cvector HPNoise;
 //������� �� ���������� ��������� �������� �������
 rvector Ampl;//���������� �������� �������� ��������� �������� �������
 rvector Phase;//���������� �������� ��� ��������� �������� �������
 rvector Azimuth;//��������� ���������� �������� �� ����������� (� ��������)
 rvector Elevate;//��������� ���������� �������� �� ��������� (� ��������)
 rvector PolarAngle;//���� ����������� ��������� �������� ������� (� ��������)
 rvector PhaseVCP;//������������ ������������ ��� ����������� (� ��������)
 rvector PhaseHCP;//�������������� ������������ ��� ����������� (� ��������)

//-------------------------------------------------------------------
// I. ���������� ������������� ������-������� ������ TInputSignal
//-------------------------------------------------------------------
public:
 //���������� ��������� �������� ������� (� �������� ��������)
 uint GetSignalCount() const {return ExtSignal::Count;};
 //���������� �������� ���������� ����� �� ������� �������
 uint Jammers() const {return ExtSignal::Jammers();};
 //���������� ��������� �������� �� (� �������� ���������)
 ulong LAAElem() const {return TLinearArray::Count;};
 //���������� ������: ���������� �������, �������� ��������� ������� � �������
 //���������� ������� �������� ������� ������� ExtSignal � TLinearArray, � �����
 //������ ���������� ����������� �������� ���������� (�������������� � ���������-
 //��� ������������) �� ������ �������� ������� ��
 void Update(double time, bool casual = ON);

 //�������� ��������� �� ������ ��������� ������� � �������������� �
 //������������ ���������������� �������������
 const cvector* GetHPSignal() const {return &HPSignal;};
 const cvector* GetVPSignal() const {return &VPSignal;};
 //�������� ��������� �� ������� ���������� ������ � �������������� �
 //������������ ���������������� �������������
 const cmatrix* GetHPJammer() const {return &HPJammer;};
 const cmatrix* GetVPJammer() const {return &VPJammer;};
 //�������� ��������� �� ������ ����������� ���� � �������������� �
 //������������ ���������������� �������������
 const cvector* GetHPNoise() const {return &HPNoise;};
 const cvector* GetVPNoise() const {return &VPNoise;};

 //������������ ������� ��������� ��������� �������� ������� ��� �������������
 //������ ������������� �������� �������
 rvector GetPower(bool PolarType) const;
 //������������ ������� ������� ��������� (������������ ����� ��� ����� �����)
 //���������� �������� ������� ��� ������������� ������ ������������� ��������
 //�������
 rvector GetAngle(bool DispLAA, bool Deg = RAD) const;
 //������ ���������� ��������� ������� �� ������ ���� ������� LAA
 cvector& StatSignal(cvector& V, bool PolarType) const;
 //������ ���������� ���������� ������ �� ������ ���� ������� LAA
 cvector& StatJammer(cvector& V, bool PolarType) const;
 //������ ���������� ����������� ���� �� ������ ���� ������� LAA
 cvector& StatNoise(cvector& V, bool PolarType) const;

 //������ ���������� ��������� ������� �� ������ ���� ������� LAA
 cvector& Signal(cvector& V, bool PolarType) const
 {if (PolarType == VP_SIGNAL) V = VPSignal;
  else V = HPSignal;
  return V;
 };
 //������� ���������� ���������� �������� ����� �� ������ ���� ������� LAA
 cmatrix& Jammer(cmatrix& A, bool PolarType) const
 {if (PolarType == VP_SIGNAL) A = VPJammer;
  else A = HPJammer;
  return A;
 };
 //������ ���������� ���������� ������ �� ������ ���� ������� LAA
 cvector& Jammer(cvector& V, bool PolarType) const;
 //������ ���������� ����������� ���� �� ������ ���� ������� LAA
 cvector& Noise(cvector& V, bool PolarType) const
 {if (PolarType == VP_SIGNAL) V = VPNoise;
  else V = HPNoise;
  return V;
 };

 //�������� ������� ���������� ��������� ������� �� ����� ���������� LAA ���
 //���� ������� LAA
 cvector Signal(bool PolarType) const
 {if (PolarType == VP_SIGNAL) return VPSignal;
  else return HPSignal;
 };
 //������������ ������� ���������� ��������� ������� �� ����� ���������� LAA ���
 //������� LAA, ������ ������� ����������� � ������� Channel
 cvector Signal(const ivector& Channels, bool PolarType) const
 {if (PolarType == VP_SIGNAL) return VPSignal.SubVector(Channels);
  else return HPSignal.SubVector(Channels);
 };
 //�������� ������� ���������� ���������� �������� ����� �� ����� ���������� LAA
 //��� ���� ������� LAA
 cmatrix Jammer(bool PolarType) const
 {if (PolarType == VP_SIGNAL) return VPJammer;
  else return HPJammer;
 };
 //������������ ������� ���������� ���� ���������� �������� ����� �� �����
 //���������� LAA ��� �������, ������ ������� ����������� � ������� Channels
 cmatrix Jammer(const ivector& Channels, bool PolarType) const
 {if (PolarType == VP_SIGNAL) return VPJammer.SubMatrixByRows(Channels,ON);
  else return HPJammer.SubMatrixByRows(Channels,ON);
 };
 //�������� ������� ���������� ����������� ���� �� ����� ���������� LAA ���
 //���� ������� LAA
 cvector Noise(bool PolarType) const
 {if (PolarType == VP_SIGNAL) return VPNoise;
  else return HPNoise;
 };
 //������������ ������� ���������� ����������� ���� �� ����� ���������� LAA ���
 //������� LAA, ������ ������� ����������� � ������� Channels
 cvector Noise(const ivector& Channels, bool PolarType) const
 {if (PolarType == VP_SIGNAL) return VPNoise.SubVector(Channels);
  else return HPNoise.SubVector(Channels);
 };
 //������������ ������� ���������� �� ��������� LAA, ������ ������� �����������
 //� ������� Channels, ��� �������� ������ ��������� ������� ��������� ��������
 //� ����������� Angle ������������ �������� LAA
 cvector TestSignal(const ivector& Channels, double Angle, bool Deg = DEG,
                    bool Check = ON) const;
 //������������ ������� ���������� �� ��������� LAA, ������ ������� �����������
 //� ������� Channels, ��� �������� ������ ��������� ������� ��������� ��������
 //� ����������� ������������ � ������� Angle ������������ �������� LAA
 cmatrix TestSignal(const ivector& Channels, const rvector& Angle,
                    bool Deg = DEG, bool Check = ON) const;
//*****************************************************************************
// ���������� ������� ������� ��������� �������, ���������� ������ � �����������
// ���� � ������ �� �������� ������� ��, � ����� �� ��������� ��������
//*****************************************************************************
 double CalcPSignal(bool PolarType);
 double CalcPJammer(bool PolarType);
 double CalcPNoise(bool PolarType);
 double CalcPSignal(ulong Channel, bool PolarType);
 double CalcPJammer(ulong Channel, bool PolarType);
 double CalcPNoise(ulong Channel, bool PolarType);
 double SJNRatio(bool PolarType);//�������������� ��������� ������/������+���
 //��������� ������/������+��� � ������ ��
 double SJNRatio(ulong Channel, bool PolarType);

 //*****************************************************************************
 //������������ �������������� ������ �������� �����������: ��������� �������,
 //���������� �������� ������ � ����������� ����
 //*****************************************************************************
 bool GetRss(cmatrix& R, bool PolarType) const
 {if (PolarType == VP_SIGNAL) return CorrelationMatrix(VPSignal,R);
  else return CorrelationMatrix(HPSignal,R);
 };
 bool GetRjj(cmatrix& R, bool PolarType) const
 {if (PolarType == VP_SIGNAL) return CorrelationMatrix(VPJammer,R);
  else return CorrelationMatrix(HPJammer,R);
 };
 bool GetRnn(cmatrix& R, bool PolarType) const;

//******************************************************************************
// ���������� ������������� � ����������� ������ TInputSignal
//******************************************************************************
 //���������� ������������ �� ���������
 TInputSignal();
 //���������� ������������ � �����������
 //�������� ������� ������ TInputSignal �� ������ �� ������ extsignal � linarray
 TInputSignal(const char* extsignal, const char* linarray);
 //���������� ����������� ������
 ~TInputSignal();

//-------------------------------------------------------------------
// II. ���������� �������� ������-������� ������ TInputSignal
//-------------------------------------------------------------------
private:

//-------------------------------------------------------------------
// III. ���������� ���������� ������-������� ������ TInputSignal
//-------------------------------------------------------------------
protected:
 void Init();//��������� ������������� ������������� TInputSignal
 void Reset();//����� ���� ������������� TInputSignal

};//���������� ���������� ������ TInputSignal

//---------------------- The end of file "input.h" -----------------------------

#endif //���������� ���������� ������������� ����� "INPUT.H"

