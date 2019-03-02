//Image.h
//Copyright (c) JohnSoft 2002. All rights reserved. Borland C++ 5.02.
//Initial date: January,20,2002. Final date:
//���������� ������� �� ��������� ���������� �����������
#if !defined(__IMAGE_H)
#define __IMAGE_H

//������������ �����
#include "mvector.h"

const bool N_WHITE = true;//����� ���
const bool N_COLOR = false;//������� ���


//***********************************************************************************
//��������������� ���������� ���� �������
//***********************************************************************************
//���������� ���� ������� �� ������ � �������������
class TImpulseResponse;//����������� �������� ����������� ������� �������
//�������� ����������� ������� � ��������� �������� ���� �������
class TIR_UnitFunction;
//�������� ����������� ������� � ��������� �������� ���� �������
class TIR_GaussFunction;
//�������� ����������� ������� � �������� ���������� ���� �������
class TIR_RoundFunction;
//�������� ����������� ������� � �������� ������� ���� �������
class TIR_EllipseFunction;

//�������������� ��������� ��������� ���������, ����������� �������� ����������� W,
//��� ��� ��������������� ������� ���������� ������� - ��������� ��������� �����������
//������� �������
class TDisturbance;

//-----------------------------------------------------------------------------------
//�������������� ������� ���� ��������� �������
//������������: CK_2D_NameFunction
//1) CK - Convolution Kernel (���� �������);
//2) 2D - TwoDimensional (������� ���������);
//3) NameFunction - ���������� (����������) ������������ ������� ���� �������
//-----------------------------------------------------------------------------------
const uint CK_2D_UNIT  = 1;//��������� ������� ���� ��������� ������� (x)
const uint CK_2D_GAUSS = 2;//�������� ������� ���� ��������� �������  (x)
const uint CK_2D_ROUND = 3;//�������� ������� ���� ��������� �������  (?)
const uint CK_2D_ELLIPSE = 4;//�����c��� ������� ���� ��������� ������� (?)


//�������������� ������� ��������� ��������� �������:
//1) ����������� ������� ������� HeightX & WidthX (����������� �� lEditIn)
//2) ����������� �������� ������� HeightY & WidthY
//3) ����� ������� ���� ������� (Convolution Kernel) � �������������� �� ����������
//������� ��� �������������� �������� �������� ��������� H
void Edit_2DConvolutionOperator(TImpulseResponse** pH, bool lEditIn = true);
void Edit_2DConvolutionKernel(TImpulseResponse** pH);
bool Modify_2DConvKernel(TImpulseResponse** pH, const uint type);

//class TGaussDistortionOperator;//����������� �������� ���������
//typedef TGaussDistortionOperator TGaussDistortion;

typedef enum {SLEEP = 1, OPTIMAL, FULL} HOLDING;

using namespace std;
//****************************************************************************************
//               ���������� �������� ������������ ������ TImpulseResponse
//      �������� ����������� ������� ������� ��������� ���������� ��������� ��������
//  ������������ �������� ��� ���������� ������� ����������� ������� ������� ����������
//                     ��������� ��������� ����������� (��������)
//������� ����� TImpulseResponse ������������ ��� ������������ ���������� ��������� H �
//�������������� � ���� �������� ������� invH � ������� ��������� H. ����� ��������� ���-
//��������� ������������ ������� ����������� ������� ��� ���� ����� ������ ��������� ����-
//������ �����������: ���������������� �������, ���������������� ������� � ������������
//�������.
//����� X(M,N) - ������� ����������� � ��������� �� ��������� M, �� ����������� - N. �����
//���������� �������� P �������� ����������� X: P = M*N. ����� Y(K,L) - �������� �����������
//������������ �� ��������� - K, �� ����������� - L. ����� ���������� �������� Q ���������
//�����������: Q = K*L.
//���������������� �������: P > Q, ���������� ������� �������� ������ ����� ��������.
//������������ �������: P == Q, ���������� ������� � �������� �������� ���������, ��� ����
//�������������� ������� �������� � ��������� ����������� ����� �� ���������.
//���������������� �������: P < Q, ���������� ������� �������� ������ ����� ��������.
//��� �������������� �������� ����������� X(M,N) � �������� ����������� Y(K,L) ����������
//������������ ������� ����������� ������� H(Q,P) � ��������� ��������� ��������:
// y(Q,1) = H(Q,P) * x(P,1), ��� y(Q,1) � x(P,1) - �������-������� ��������� Y � ��������
//����������� X ��������������: X(M,N) -> x(P,1) -> H(Q,P) -> y(Q,1) -> Y(K,L)
//����� ������������ �������� ������������� ������������ h[i,j] ������� H, � ������������
//� �������� ����������� ������������ ������� ����������� ������� ��� ��������� �������.
//��� ��� ������� ����������� ������� �������� ��� ��������� ���������� ��������, �� �������
//h[i,j] ����� ������������ ��� ��������: ��� i-���������� � ��� j-����������. ��� ��������
//X(3,3) � ��������� Y(3,3) ����������� �������� ��� h[i,j] ����� ���� ������������ ���������
//�������� (��������� ������� H(9,9)):
//   |           J=0          |           J=1            |           J=2             |
//   | m=0    m=1     m=2     | m=0    m=1     m=2       | m=0    m=1     m=2        |
//---+-------------------------------------------------------------------------------+--
//k=0| h(0,0) h(0,-1) h(0,-2) | h(0,0)  h(0,-1) h(0,-2)  | h(-2,0) h(-2,-1) h(-2,-2) |
//k=1| h(0,1) h(0,0)  h(0,-1) | h(-1,1) h(-1,0) h(-1,-1) | h(-2,1) h(-2,0)  h(-2,-1) |I=0
//k=2| h(0,2) h(0,1)  h(0,0)  | h(-1,2) h(-1,1) h(-1,0)  | h(-2,2) h(-2,0)  h(-2,0)  |
//---|-------------------------------------------------------------------------------|--
//k=0| h(1,0) h(1,-1) h(1,-2) | h(0,0)  h(0,-1) h(0,-2)  | h(-1,0) h(-1,-1) h(-1,-2) |
//k=1| h(1,1) h(1,0)  h(1,-1) | h(0,1)  h(0,0)  h(0,-1)  | h(-1,1) h(-1,0)  h(-1,-1) |I=1
//k=2| h(1,2) h(1,1)  h(1,0)  | h(0,2)  h(0,1)  h(0,0)   | h(-1,2) h(-1,1)  h(-1,0)  |
//---|-------------------------------------------------------------------------------|--
//k=0| h(2,0) h(2,-1) h(2,-2) | h(1,0)  h(1,-1) h(1,-2)  | h(0,0)  h(0,-1)  h(0,-2)  |
//k=1| h(2,1) h(2,0)  h(2,-1) | h(1,1)  h(1,0)  h(1,-1)  | h(0,1)  h(0,0)   h(0,-1)  |I=2
//k=2| h(2,2) h(2,1)  h(2,0)  | h(2,2)  h(1,1)  h(1,0)   | h(0,2)  h(0,1)   h(0,0)   |
//---|-------------------------------------------------------------------------------|--
//�������� i � j h[i,j]-�� �������� H ����������� �� �������: i=I-J; j=k-m.
//������������ k,I � m,J ���������� � ��� �� ��������, ��� � ������� ����� � ��������
//� ������� ��������� Y � �������� X ����������� ��������������. ������������ k,m �������-
//����� ���������� ������ ������� ����� ������� H �� 0 �� K � M ��������������.
//������������ I,J ���������� �� 0 �� L � N ��� �������� �� ����� � ����� ������� H �� ���-
//������ � ����������� ��������������. ���������� ������ ������� H ����� �� �����������,
//����� �������, ������� I � J ���������� ���������� ������ ���� ������� � ��������� �������
//���������. ������������ k � m ���������� ���������� ������ ������� ����� (I,J) ������� H.
//�������� �������� i,j h[i,j]-�� �������� ������� H, ������������ ��� ���������� ��������
//������� �������� ������� ����������� �������.
//����� �������� �����������.
//****************************************************************************************
class TImpulseResponse {
//----------------------------------------------------------------------------------------
// ���������� �������� ������-������ ������ TImpulseResponse
//----------------------------------------------------------------------------------------
private:
 ulong _HeightX; //���������� �������� �������� ����������� �� ������ (���-�� �����)
 ulong _WidthX;  //���������� �������� �������� ����������� �� ������ (���-�� ��������)
 ulong _HeightY; //���������� �������� ��������� ����������� �� ������ (���-�� �����)
 ulong _WidthY;  //���������� �������� ��������� ����������� �� ������ (���-�� ��������)
 ulong _OriginHX;//������ ������� �� ������ ��� �������� ����������� [0.._HeightX)
 ulong _OriginWX;//������ ������� �� ������ ��� �������� ����������� [0.._WidthX)
 ulong _OriginHY;//������ ������� �� ������ ��� ��������� ����������� [0.._HeightY)
 ulong _OriginWY;//������ ������� �� ������ ��� ��������� ����������� [0.._WidthY)

//----------------------------------------------------------------------------------------
// ���������� ������������� � ����������� ������ TImpulseResponse
//----------------------------------------------------------------------------------------
public:
 //����������� �� ���������
 TImpulseResponse() {Set(0L,0L,0L,0L); SetDefOrigins();};
 //������������ � �����������
 TImpulseResponse(ulong hX, ulong wX, ulong hY, ulong wY)
 {Set(hX,wX,hY,wY); SetDefOrigins();};
 TImpulseResponse(ulong hX, ulong wX, ulong hY, ulong wY, ulong hX0, ulong wX0,
                  ulong hY0, ulong wY0)
 {Set(hX,wX,hY,wY); SetOrigins(hX0,wX0,hY0,wY0);};

 TImpulseResponse(ulong hX, ulong wX) {Set(hX,wX,hX,wX); SetDefOrigins();};

 TImpulseResponse(ulong hX, ulong wX, ulong hY, ulong wY, const rvector& Args)
 {Set(hX,wX,hY,wY); SetDefOrigins();};
 TImpulseResponse(ulong hX, ulong wX, ulong hY, ulong wY, ulong hX0, ulong wX0,
                  ulong hY0, ulong wY0, const rvector& Args)
 {Set(hX,wX,hY,wY); SetOrigins(hX0,wX0,hY0,wY0);};

 TImpulseResponse(ulong hX, ulong wX, const rvector& Args)
 {Set(hX,wX,hX,wX); SetDefOrigins();};
 //���������� ������������ �����������
 TImpulseResponse(const TImpulseResponse &Operator) {*this = Operator;};
 //���������� ����������� ������
 virtual ~TImpulseResponse() {Reset();};

//---------------------------------------------------------------------------------------
// ���������� ������������� ������-������� ������ TImpulseResponse
//---------------------------------------------------------------------------------------
 //��������� ������� ������
 bool IsReady() const
 {return (_HeightX && _WidthX && _HeightY && _WidthY) ? true : false;};
 ulong GetInHeight() const {return _HeightX;};
 ulong GetInWidth() const {return _WidthX;};
 ulong GetInSize() const {return _HeightX*_WidthX;};
 ulong GetInSize(ulong& hX, ulong& wX) const
 {hX = _HeightX; wX = _WidthX; return _HeightX*_WidthX;};
 ulong GetOutHeight() const {return _HeightY;};
 ulong GetOutWidth() const {return _WidthY;};
 ulong GetOutSize() const {return _HeightY*_WidthY;};
 ulong GetOutSize(ulong& hY, ulong& wY) const
 {hY = _HeightY; wY = _WidthY; return _HeightY*_WidthY;};
 void GetInOutSizes(ulong& hX, ulong& wX, ulong& hY, ulong& wY) const
 {hX = _HeightX; wX = _WidthX; hY = _HeightY; wY = _WidthY;};
 //���������� ������ �������� ��� �������� � ��������� �����������
 ulong GetInH0() const {return _OriginHX;};
 ulong GetInW0() const {return _OriginWX;};
 void GetInOrigin(ulong& hX0, ulong& wX0 ) const {hX0 = _OriginHX; wX0 = _OriginWX;};
 ulong GetOutH0() const {return _OriginHY;};
 ulong GetOutW0() const {return _OriginWY;};
 void GetOutOrigin(ulong& hY0, ulong& wY0 ) const {hY0 = _OriginHY; wY0 = _OriginWY;};

 virtual rvector& GetConvKernelArgs(rvector& Param) const = 0;//#1
 virtual uint GetTypeConvKernel() const = 0;//#2
 //----------------------------------------------------------------------------------
 // ������� ��������� ������������� ������� ������
 //----------------------------------------------------------------------------------
 //������������ �������� ������� ����������� ������� H �� ���� �����������
 bool GetHSamples(imatrix& H_1, imatrix& H_2) const;
 //������������ �������� ������� ����������� ������� H �� ���� �����������
 bool GetTrHSamples(imatrix& trH_1, imatrix& trH_2) const;
 //��� ������� ����������� ������� ������� (������)
 virtual string& GetType(string& s) const = 0;//#3
 //������������ �������� �������� � ��������� ����������� � ���� ������
 string& GetHSamplesSize(string& s, uint left_indent = 0) const;
 string& GetInSize(string& s) const;
 string& GetOutSize(string& s) const;
 //������������ �������� � ������ ��������� ��� �������� � ��������� ��������
 string& GetInOrigin(string& s) const;
 string& GetOutOrigin(string& s) const;
 string& GetOrigins(string& s, uint left_indent = 0) const;
 //������������ �������� � ������� � ������ ��������� ��� �������� � ��������� ��������
 string& GetInSettings(string& s) const;
 string& GetOutSettings(string& s) const;
 //������������ �������� � ������� ���� �������
 virtual string& GetConvKernel(string& s, uint left_indent = 0) const = 0;//#4
 virtual string& BriefConvKernel(string& s) const = 0;//#5
 //������������ �������� ������� ����������� ������� �� ���� ����������� � ���� ������
 string& GetHSamples(string& s, const string& delim = string(1,' '),
                     uint left_indent = 0) const;//string(' ')
  //������������ �������� ����������������� ������� ����������� ������� �� ����
  //����������� � ���� ������
 string& GetTrHSamples(string& s, const string& delim = string(1,' '),
                       uint left_indent = 0) const;//string(' ')
 //������������ ���� ������� ������ � ���� ������
 string& Info(string& s, string& head = string(), uint left_indent = 0) const;
 string& BriefInfo(string& s, string& head = string(), uint left_indent = 0) const;

 string& Properties(string& s, string& head = string(), uint left_indent = 0) const
 {return BriefInfo(s,head,left_indent);};


 //OVERLOAD THE STREAM INSERTION OPERATOR
 //���������� �������� ���������� ������ (<<) ������� ������
 friend ostream& operator <<(ostream& out, const TImpulseResponse& Samples)
 {string s; return out << Samples.BriefInfo(s);};
 //---------------------------------------------------------------------------------------
 // ���������� ������� ����������� �������������� ������� ������
 //---------------------------------------------------------------------------------------
 void EditInSampleSize(uint left_indent = 0);//�������������� �������� ������� �������
 void EditOutSampleSize(uint left_indent = 0);//�������������� �������� �������� �������
 //�������������� �������� ������ ��������� ��� ������� � �������� �������
 void EditInOrigin(uint left_indent = 0);
 void EditOutOrigin(uint left_indent = 0);
 //�������������� ���������� ������� ���� �������
 virtual void EditConvKernel(uint left_indent = 0) = 0;//#6
 //�������������� ���� ������� ������� ������
 void Edit(bool lEditInSample = true);
 //---------------------------------------------------------------------------------------
 //��������� ������� ������
 bool Set(ulong hX, ulong wX, ulong hY, ulong wY)
 {_HeightX = hX; _WidthX = wX; _HeightY = hY; _WidthY = wY;
  SetDefOrigins(); return IsReady();};
 bool Set(ulong hX, ulong wX, ulong hY, ulong wY, ulong hX0, ulong wX0, ulong hY0, ulong wY0)
 {Set(hX,wX,hY,wY); SetOrigins(hX0,wX0,hY0,wY0); return IsReady();};

 bool Set(ulong hX, ulong wX) {SetDefOrigins(); return Set(hX,wX,hX,wX);};
 bool SetInSize(ulong hX, ulong wX)
 {_HeightX = hX; _WidthX = wX; SetInOrigin(0,0); return IsReady();};
 bool SetOutSize(ulong hY, ulong wY)
 {_HeightY = hY; _WidthY = wY; SetOutOrigin(0,0); return IsReady();};
 //��������� ��������� ������ ������� ��� ������� � �������� �������
 void SetDefOrigins() {_OriginHX = _OriginWX = _OriginHY = _OriginWY = 0L;};
 void SetInOrigin(ulong hX0, ulong wX0)
 {_OriginHX = (hX0 < GetInHeight()) ? hX0 : 0L;
  _OriginWX = (wX0 < GetInWidth())  ? wX0 : 0L;
 }
 void SetOutOrigin(ulong hY0, ulong wY0)
 {_OriginHY = (hY0 < GetOutHeight()) ? hY0 : 0L;
  _OriginWY = (wY0 < GetOutWidth())  ? wY0 : 0L;
 }
 void SetOrigins(ulong hX0, ulong wX0, ulong hY0, ulong wY0)
 {SetInOrigin(hX0,wX0); SetOutOrigin(hY0,wY0);};

 virtual bool SetConvKernelArgs(const rvector& Param) = 0;//#7
 virtual bool SetDefConvKernelArgs() = 0;//#8
 virtual bool ResetConvKernelArgs() = 0;//#9
 bool Reset() {Set(0L,0L,0L,0L); SetDefOrigins(); return IsReady();};
 //---------------------------------------------------------------------------------------
 //���������� ���������� ��� ������ TImpulseResponse
 //���������� ��������� ������������ (=)
 TImpulseResponse* operator =(const TImpulseResponse& H)
 {if (this == &H) return this;
  rvector Args;
  Set(H.GetInHeight(),H.GetInWidth(),H.GetOutHeight(),H.GetOutWidth(),H.GetInH0(),
      H.GetInW0(),H.GetOutH0(),H.GetOutW0());
  SetConvKernelArgs(H.GetConvKernelArgs(Args));
  return this;
 };
 //���������� ��������� ��������� (==)
 bool operator ==(const TImpulseResponse& H) const
 {return ((GetInHeight() == H.GetInHeight()) && (GetInWidth() == H.GetInWidth()) &&
         (GetOutHeight() == H.GetOutHeight()) && (GetOutWidth() == H.GetOutWidth()))
         ? true : false;
 };
 //���������� �������������� ��������� ������ ������� ��� ���������� ��������� �����������
 //�� ��������� �������� � ������� ����������� ������� ������� � �������� �����
 bool operator() (const rmatrix& X, rmatrix& Y, ulong hY = 0L, ulong wY = 0L);
 bool operator() (const rmatrix& X, rvector& Y, ulong hY = 0L, ulong wY = 0L);

 bool TFunc(const rmatrix& X, rmatrix& Y, ulong hY = 0L, ulong wY = 0L)
 {return (*this)(X,Y,hY,wY);};
 bool TFunc(const rmatrix& X, rvector& Y, ulong hY = 0L, ulong wY = 0L)
 {return (*this)(X,Y,hY,wY);};
 //������������ ������� ����������� ������� � ������� H
 bool ImpulseResponse(rmatrix& H) const;
 bool ImpulseResponse(rmatrix& H, double& normH) const;
 bool GetH(rmatrix& H) const {return ImpulseResponse(H);};
 //��������� �������������� ��������� H
 bool FrequencyResponse(rmatrix& H, ulong HCount = 0L, ulong VCount = 0L) const;
 //������������ ����������������� ������� ����������� ������� � ������� trH
 bool TrImpulseResponse(rmatrix& trH) const;
 bool GetTrH(rmatrix& trH) const {return TrImpulseResponse(trH);};
 //������������ ��������� ��������� ������� ����������� ������� (��������� ���������
 //�������). �������� ��������� ��������� ������� � ������ ����������� �������� �������
 //������� N � �������� ������� M �������������� ��������� �������:
 // a) inv(H) - complete system M == N  [inv(*) - �������� ���������]
 // b) inv(trH x H) - overdetermined system M > N
 // c) inv(H x trH) - undetermined system M < N
 bool GetInverseOperator(rmatrix& invH) const;
 bool GetInverseOperator(rmatrix& invH, double& normH) const;
 bool GetInverseOperator(rmatrix& invH, double& normH, double& normA) const;
 //�������������� ��������� ��������� ������� ����������� �������:
 //������� ��������� ��������� ������� ����������� ������� �����������:
 //1. ��� ���������������� � ���������������� ������ ������, �.�. ������������ ������
 //   trH x H � H x trH - �������������� ������� � � ������ ������������� ��������
 //   ������ - inv(trH x H) � inv(H x trH) - ��� ����� ����� ���������������;
 //2. ��� ������ �������, inv(H) ����� ��������������, ���� H - ����� �������������� �
 //   ������������� ��������. �������������� H ������������ �������� ���� �������.
 bool IsInvOpSymmetric() const
 {if (!IsReady()) return false;
  return (GetOutSize() != GetInSize()) ? true : IsConvKernelSymmetric();
 };
 //������������ ��������� ������������� (���������� �������) ����������� �����������
 //� ������ ����������� �������� ������� ������� N � �������� ������� M:
 // a) inv(H) - complete system M == N
 // b) inv(trH x H) x trH - overdetermined system M > N
 // c) trH x inv(H x trH) - undetermined system M < N
 bool GetRecoveryOperator(rmatrix& W) const;
 bool GetRecoveryOperator(rmatrix& W, rmatrix& InvA, double& normH, double& normA) const;

 //---------------------------------------------------------------------------------------
 // ���������� ������������� �������
 //---------------------------------------------------------------------------------------
 //��������� ����������� X ����������� �������� � �������� ����������� ������� H
 //�������� ����������� ����������� � ������� Y. ����������� ������� ������� X ������
 //���� ����������� � �������� ��������� H
 friend bool StrictConvolution(const TImpulseResponse& H, const rmatrix& X, rmatrix& Y);
 friend bool StrictConvolution(const TImpulseResponse* H, const rmatrix& X, rmatrix& Y);
 friend bool StrictConvolution(const TImpulseResponse* H, const rmatrix& X, rvector& Y);
 friend bool AdaptConvolution(TImpulseResponse& H, const rmatrix& X, rmatrix& Y);
 //����������� ����������� ����������� Y ������� ���������� �������. ����������������
 //����������� ����������� � ������� EstX.
 friend bool StrictInvRecovery(const TImpulseResponse& H, const rmatrix& Y, rmatrix& EstX);
 friend bool StrictInvRecovery(const TImpulseResponse* H, const rmatrix& Y, rmatrix& EstX);
 friend bool StrictInvRecovery(const TImpulseResponse* H, const rvector& Y, rvector& EstX);
 friend bool AdaptInvRecovery(TImpulseResponse& H, const rmatrix& Y, rmatrix& EstX);

//----------------------------------------------------------------------------------------
//II. ���������� ���������� ������-������� ������ TImpulseResponse
//----------------------------------------------------------------------------------------
protected:
 //���� ������� (Convolution kernel)
 virtual double ConvolutionKernel(long i, long j) const = 0;//#10
 //�������������� ������� ���� �������
 virtual bool IsConvKernelSymmetric() const = 0;//#11
 //���������� ��������� ������� ��� ������� �������� ����������� X � ������� Y
 bool Convolute(const rmatrix& X, rmatrix& Y) const;
 //���������� ��������� ������� ��� ������� �������� ����������� X � ������� Y
 bool Convolute(const rmatrix& X, rvector& Y) const;
 //���������� ��������� ������� ��� ������� �������� ����������� X � ������� Y
 bool Convolute(const rvector& X, rvector& Y) const;
 //����������� ����������� ����������� Y ������� ���������� �������
 bool InvRecovery(const rmatrix& Y, rmatrix& EstX) const;
 bool InvRecovery(const rvector& Y, rvector& EstX) const;
};//���������� ���������� ������ TImpulseResponse
//-------------------- ���������� ������ TImpulseResponse ---------------------------

//***********************************************************************************
//            ���������� ������ TIR_UnitFunction
//   �������� ����������� ������� ������� ��������� ���������� ��������� ��������
//����� �������� ����������� �� ������������ ������ TImpulseResponse. � ��������
//���� ������� ������������ ��������� ������� (��������� ��������):
// (i^2 + j^2) <  1 : 1.0
// (i^2 + j^2) >= 1 : 0.0
//***********************************************************************************
class TIR_UnitFunction : public TImpulseResponse {
//-----------------------------------------------------------------------------------
// ���������� �������� ������-������ ������ TIR_UnitFunction
//-----------------------------------------------------------------------------------
//private:
//-----------------------------------------------------------------------------------
// ���������� ������������� � ����������� ������ TIR_UnitFunction
//-----------------------------------------------------------------------------------
public:
 //����������� �� ���������
 TIR_UnitFunction() : TImpulseResponse() {SetDefConvKernelArgs();};
 //������������ � �����������
 TIR_UnitFunction(ulong hX, ulong wX, ulong hY, ulong wY) :
 TImpulseResponse(hX, wX, hY, wY) {SetDefConvKernelArgs();};

 TIR_UnitFunction(ulong hX, ulong wX) : TImpulseResponse(hX, wX) {SetDefConvKernelArgs();};


 TIR_UnitFunction(ulong hX, ulong wX, ulong hY, ulong wY, const rvector& Args) :
 TImpulseResponse(hX, wX, hY, wY, Args) {SetConvKernelArgs(Args);};

 TIR_UnitFunction(ulong hX, ulong wX, const rvector& Args) :
 TImpulseResponse(hX, wX, Args) {SetConvKernelArgs(Args);};
 //���������� ������������ �����������
 TIR_UnitFunction(const TIR_UnitFunction &Operator) {*this = Operator;};
 //���������� ����������� ������
 virtual ~TIR_UnitFunction() {ResetConvKernelArgs();};

//-----------------------------------------------------------------------------------
// ���������� ������������� ������-������� ������ TIR_UnitFunction
//-----------------------------------------------------------------------------------
 //��������� ���������� ������� ���� �������
 virtual rvector& GetConvKernelArgs(rvector& Param) const
 {Param.Set(1L,ROW,1.0); return Param;};//#1
 virtual uint GetTypeConvKernel() const {return CK_2D_UNIT;};//#2
 //----------------------------------------------------------------------------------
 // ������� ��������� ������������� ������� ������
 //----------------------------------------------------------------------------------
 //��� ��������� ����������� ������� ������� (������)
 virtual string& GetType(string& s) const
 {s.assign("2D-ImpulseResponse with UnitFunction ConvKernel"); return s;};//#3
 //������������ �������� � ������� ���� �������
 virtual string& GetConvKernel(string& s, uint left_indent = 0) const;//#4
 virtual string& BriefConvKernel(string& s) const {return s.assign("Unit()");};//#5
 //----------------------------------------------------------------------------------
 // ���������� ������� ����������� �������������� ������� ������
 //----------------------------------------------------------------------------------
 //�������������� ���������� ������� ���� �������
 virtual void EditConvKernel(uint left_indent = 0);//#6
 //----------------------------------------------------------------------------------
 //��������� ������� ������
 //----------------------------------------------------------------------------------
 virtual bool SetConvKernelArgs(const rvector& Param) {return true;};//#7
 virtual bool SetDefConvKernelArgs() {return true;};//#8
 virtual bool ResetConvKernelArgs() {return true;}; //#9

 //----------------------------------------------------------------------------------
 //���������� ���������� ��� ������ TIR_UnitFunction
 //----------------------------------------------------------------------------------
 //���������� ��������� ������������ (=)
 TIR_UnitFunction* operator =(const TIR_UnitFunction& H)
 {if (this == &H) return this;
  rvector Args;
  Set(H.GetInHeight(),H.GetInWidth(),H.GetOutHeight(),H.GetOutWidth());
  SetConvKernelArgs(H.GetConvKernelArgs(Args));
  return this;
 };
//-----------------------------------------------------------------------------------
//II. ���������� ���������� ������-������� ������ TIR_UnitFunction
//-----------------------------------------------------------------------------------
protected:
 //���� ������� (Convolution kernel)
 virtual double ConvolutionKernel(long i, long j) const
 {return ((i == 0L) && (j == 0L)) ? 1.0 : 0.0;};//#10
 //�������������� ������� ���� �������
 virtual bool IsConvKernelSymmetric() const {return true;};//#11
};//���������� ���������� ������ TIR_UnitFunction
//-------------------- ���������� ������ TIR_UnitFunction ---------------------------

//***********************************************************************************
//            ���������� ������ TIR_GaussFunction
//   �������� ����������� ������� ������� ��������� ���������� ��������� ��������
//����� �������� ����������� �� ������������ ������ TImpulseResponse. � ��������
//���� ������� ������������ �������� �������:
//                H(i,j) = EXP(-[t1*Hi(i,j)^2 + t2*Hj(i,j)^2]),
//t1, t2 - ��������� ������� ������ (0 <= t1, t2 < +INF);
//Hi(i,j) - �������� ������� ������� ��������� ������� ������ ��� (i,j) ��������
//          ������� ����������� ������� H
//Hj(i,j) - �������� ������� ������� ��������� ������� ������ ��� (i,j) ��������
//          ������� ����������� ������� H
//***********************************************************************************
class TIR_GaussFunction : public TImpulseResponse {
//-----------------------------------------------------------------------------------
// ���������� �������� ������-������ ������ TIR_GaussFunction
//-----------------------------------------------------------------------------------
private:
 double _t1;//�������� ��������� ������� ��������� �� �������  (i)
 double _t2;//�������� ��������� ������� ��������� �� �������� (j)
//-----------------------------------------------------------------------------------
// ���������� ������������� � ����������� ������ TIR_GaussFunction
//-----------------------------------------------------------------------------------
public:
 //����������� �� ���������
 TIR_GaussFunction() : TImpulseResponse() {SetDefConvKernelArgs();};
 //������������ � �����������
 TIR_GaussFunction(ulong hX, ulong wX, ulong hY, ulong wY) :
 TImpulseResponse(hX, wX, hY, wY) {SetDefConvKernelArgs();};

 TIR_GaussFunction(ulong hX, ulong wX) : TImpulseResponse(hX, wX) {SetDefConvKernelArgs();};

 TIR_GaussFunction(ulong hX, ulong wX, ulong hY, ulong wY, const rvector& Args) :
 TImpulseResponse(hX, wX, hY, wY, Args) {SetConvKernelArgs(Args);};

 TIR_GaussFunction(ulong hX, ulong wX, const rvector& Args) :
 TImpulseResponse(hX, wX, Args) {SetConvKernelArgs(Args);};
 //���������� ������������ �����������
 TIR_GaussFunction(const TIR_GaussFunction &Operator) {*this = Operator;};
 //���������� ����������� ������
 virtual ~TIR_GaussFunction() {ResetConvKernelArgs();};

//-----------------------------------------------------------------------------------
// ���������� ������������� ������-������� ������ TIR_GaussFunction
//-----------------------------------------------------------------------------------
 //��������� ���������� ������� ���� �������
 virtual rvector& GetConvKernelArgs(rvector& Param) const
 {Param.Set(2L,ROW,false); Param.Put(0L,_t1); Param.Put(1L,_t2); return Param;};//#1
 virtual uint GetTypeConvKernel() const {return CK_2D_GAUSS;};//#2
 //----------------------------------------------------------------------------------
 // ������� ��������� ������������� ������� ������
 //----------------------------------------------------------------------------------
 //��� ��������� ����������� ������� ������� (������)
 virtual string& GetType(string& s) const
 {s.assign("2D-ImpulseResponse with GaussFunction ConvKernel"); return s;};//#3
 //������������ �������� � ������� ���� �������
 virtual string& GetConvKernel(string& s, uint left_indent = 0) const;//#4
 virtual string& BriefConvKernel(string& s) const;//#5
 //----------------------------------------------------------------------------------
 // ���������� ������� ����������� �������������� ������� ������
 //----------------------------------------------------------------------------------
 //�������������� ���������� ������� ���� �������
 virtual void EditConvKernel(uint left_indent = 0);//#6
 //----------------------------------------------------------------------------------
 //��������� ������� ������
 //----------------------------------------------------------------------------------
 //��������� ���������� ��������� ������� �� ������ �� ������� ���� rvector:
 //_t1 = Param[0]; _t2 = Param[1]
 virtual bool SetConvKernelArgs(const rvector& Param) //#7
 {if (Param.Size() != 2) return false;
  SetConvKernelArgs(Param.Get(0L),Param.Get(1L));
  return true;
 };
 virtual bool SetDefConvKernelArgs() {_t1=1.0; _t2=1.0; return true;};//#8
 virtual bool ResetConvKernelArgs() {_t1 = _t2 = 0.0; return true;}; //#9
 void SetConvKernelArgs(double t1, double t2) {SetT1(t1); SetT2(t2);};
 //----------------------------------------------------------------------------------
 //���������� ���������� ��� ������ TIR_GaussFunction
 //----------------------------------------------------------------------------------
 //���������� ��������� ������������ (=)
 TIR_GaussFunction* operator =(const TIR_GaussFunction& H)
 {if (this == &H) return this;
  rvector Args;
  Set(H.GetInHeight(),H.GetInWidth(),H.GetOutHeight(),H.GetOutWidth());
  SetConvKernelArgs(H.GetConvKernelArgs(Args));
  return this;
 };
//-----------------------------------------------------------------------------------
//II. ���������� ���������� ������-������� ������ TIR_GaussFunction
//-----------------------------------------------------------------------------------
protected:
 //���� ������� (Convolution kernel)
 virtual double ConvolutionKernel(long i, long j) const
 {return ((i == 0L) && (j == 0L)) ? 1.0 : exp(-(_t1*i*i + _t2*j*j));};//#10
 //�������������� ������� ���� �������
 virtual bool IsConvKernelSymmetric() const {return true;};//#11
 //��������� ���������� ������� ���� ��������� �������
 void SetT1(double t1) {_t1 = fabs(t1);};
 void SetT2(double t2) {_t2 = fabs(t2);};
};//���������� ���������� ������ TIR_GaussFunction
//-------------------- ���������� ������ TIR_GaussFunction --------------------------

//***********************************************************************************
//            ���������� ������ TIR_RoundFunction
//   �������� ����������� ������� ������� ��������� ���������� ��������� ��������
//����� �������� ����������� �� ������������ ������ TImpulseResponse. � ��������
//���� ������� ������������ ������� ����������:
//          | 1/(pi*R^2), Hi(i,j)^2 + Hj(i,j)^2 <= R^2;
// H(i,j) = |
//          | 0, Hi(i,j)^2 + Hj(i,j)^2 > R^2,
//R - ������ ����������;
//Hi(i,j) - �������� ������� ������� ��������� ������� ������ ��� (i,j) ��������
//          ������� ����������� ������� H
//Hj(i,j) - �������� ������� ������� ��������� ������� ������ ��� (i,j) ��������
//          ������� ����������� ������� H
//***********************************************************************************
class TIR_RoundFunction : public TImpulseResponse {
//-----------------------------------------------------------------------------------
// ���������� �������� ������-������ ������ TIR_RoundFunction
//-----------------------------------------------------------------------------------
private:
 double _R;//������ ���������� (�����)
 double _Brightness;// 1/(PI*R^2) PI = 3.14159265359
 double _SqR;// _R^2
//-----------------------------------------------------------------------------------
// ���������� ������������� � ����������� ������ TIR_RoundFunction
//-----------------------------------------------------------------------------------
public:
 //����������� �� ���������
 TIR_RoundFunction() : TImpulseResponse() {SetDefConvKernelArgs();};
 //������������ � �����������
 TIR_RoundFunction(ulong hX, ulong wX, ulong hY, ulong wY) :
 TImpulseResponse(hX, wX, hY, wY) {SetDefConvKernelArgs();};

 TIR_RoundFunction(ulong hX, ulong wX) : TImpulseResponse(hX, wX) {SetDefConvKernelArgs();};

 TIR_RoundFunction(ulong hX, ulong wX, ulong hY, ulong wY, const rvector& Args) :
 TImpulseResponse(hX, wX, hY, wY, Args) {SetConvKernelArgs(Args);};

 TIR_RoundFunction(ulong hX, ulong wX, const rvector& Args) :
 TImpulseResponse(hX, wX, Args) {SetConvKernelArgs(Args);};
 //���������� ������������ �����������
 TIR_RoundFunction(const TIR_RoundFunction &Operator) {*this = Operator;};
 //���������� ����������� ������
 virtual ~TIR_RoundFunction() {ResetConvKernelArgs();};

//-----------------------------------------------------------------------------------
// ���������� ������������� ������-������� ������ TIR_RoundFunction
//-----------------------------------------------------------------------------------
 //��������� ���������� ������� ���� �������
 virtual rvector& GetConvKernelArgs(rvector& Param) const
 {Param.Set(1L,ROW,false); Param.Put(0L,_R); return Param;};//#1
 virtual uint GetTypeConvKernel() const {return CK_2D_ROUND;};//#2
 //----------------------------------------------------------------------------------
 // ������� ��������� ������������� ������� ������
 //----------------------------------------------------------------------------------
 //��� ��������� ����������� ������� ������� (������)
 virtual string& GetType(string& s) const
 {s.assign("2D-ImpulseResponse with RoundFunction ConvKernel"); return s;};//#3
 //������������ �������� � ������� ���� �������
 virtual string& GetConvKernel(string& s, uint left_indent = 0) const;//#4
 virtual string& BriefConvKernel(string& s) const;//#5
 //----------------------------------------------------------------------------------
 // ���������� ������� ����������� �������������� ������� ������
 //----------------------------------------------------------------------------------
 //�������������� ���������� ������� ���� �������
 virtual void EditConvKernel(uint left_indent = 0);//#6
 //----------------------------------------------------------------------------------
 //��������� ������� ������
 //----------------------------------------------------------------------------------
 //��������� ���������� ������� ���������� �� ������ �� ������� ���� rvector: _R = Param[0]
 virtual bool SetConvKernelArgs(const rvector& Param) //#7
 {if (Param.Size() != 1) return false;
  SetConvKernelArgs(Param.Get(0L));
  return true;
 };
 virtual bool SetDefConvKernelArgs() {SetR(1.0); return true;};//#8
 virtual bool ResetConvKernelArgs()
 {_R = 0.0; _SqR = 0.0; _Brightness = 0.0; return true;}; //#9
 void SetConvKernelArgs(double radius) {SetR(radius);};
 //----------------------------------------------------------------------------------
 //���������� ���������� ��� ������ TIR_RoundFunction
 //----------------------------------------------------------------------------------
 //���������� ��������� ������������ (=)
 TIR_RoundFunction* operator =(const TIR_RoundFunction& H)
 {if (this == &H) return this;
  rvector Args;
  Set(H.GetInHeight(),H.GetInWidth(),H.GetOutHeight(),H.GetOutWidth());
  SetConvKernelArgs(H.GetConvKernelArgs(Args));
  return this;
 };
//-----------------------------------------------------------------------------------
//II. ���������� ���������� ������-������� ������ TIR_RoundFunction
//-----------------------------------------------------------------------------------
protected:
 //���� ������� (Convolution kernel)
 virtual double ConvolutionKernel(long i, long j) const
 {return ((i*i + j*j) <= _SqR) ? _Brightness : 0.0;};//#10
 //�������������� ������� ���� �������
 virtual bool IsConvKernelSymmetric() const {return true;};//#11
 //��������� ���������� ������� ���� ��������� �������
 void SetR(double radius)
 {_R = fabs(radius);
  if (_R == 0.0) _R = 1.0;
  _SqR = _R*_R; _Brightness = 1.0/(3.14159265359*_SqR);
 };
};//���������� ���������� ������ TIR_RoundFunction
//-------------------- ���������� ������ TIR_RoundFunction --------------------------

//***********************************************************************************
//            ���������� ������ TIR_EllipseFunction
//   �������� ����������� ������� ������� ��������� ���������� ��������� ��������
//����� �������� ����������� �� ������������ ������ TImpulseResponse. � ��������
//���� ������� ������������ ������� ������:
//          | �, Hi(i,j)^2/a^2 + Hj(i,j)^2/b^2 <= 1;
// H(i,j) = |
//          | 0, Hi(i,j)^2/a^2 + Hj(i,j)^2/b^2 > 1,
//a - ������� ��� �������;
//b - ����� ��� �������;
//� - ������� ������� (brightness) ������ �������;
//Hi(i,j) - �������� ������� ������� ��������� ������� ������ ��� (i,j) ��������
//          ������� ����������� ������� H;
//Hj(i,j) - �������� ������� ������� ��������� ������� ������ ��� (i,j) ��������
//          ������� ����������� ������� H.
//***********************************************************************************
class TIR_EllipseFunction : public TImpulseResponse {
//-----------------------------------------------------------------------------------
// ���������� �������� ������-������ ������ TIR_EllipseFunction
//-----------------------------------------------------------------------------------
private:
 double _A;//longer axes (������� ������� �������, �� ������) - ��� OX
 double _B;//shorter axes (����� ������� �������, �� ������) - ��� OY
 double _Brightness;//������� ������� ������ �������
 double _SqA;// _A^2
 double _SqB;// _B^2
//-----------------------------------------------------------------------------------
// ���������� ������������� � ����������� ������ TIR_EllipseFunction
//-----------------------------------------------------------------------------------
public:
 //����������� �� ���������
 TIR_EllipseFunction() : TImpulseResponse() {SetDefConvKernelArgs();};
 //������������ � �����������
 TIR_EllipseFunction(ulong hX, ulong wX, ulong hY, ulong wY) :
 TImpulseResponse(hX, wX, hY, wY) {SetDefConvKernelArgs();};

 TIR_EllipseFunction(ulong hX, ulong wX) :
 TImpulseResponse(hX, wX) {SetDefConvKernelArgs();};

 TIR_EllipseFunction(ulong hX, ulong wX, ulong hY, ulong wY, const rvector& Args) :
 TImpulseResponse(hX, wX, hY, wY, Args) {SetConvKernelArgs(Args);};

 TIR_EllipseFunction(ulong hX, ulong wX, const rvector& Args) :
 TImpulseResponse(hX, wX, Args) {SetConvKernelArgs(Args);};
 //���������� ������������ �����������
 TIR_EllipseFunction(const TIR_EllipseFunction &Operator) {*this = Operator;};
 //���������� ����������� ������
 virtual ~TIR_EllipseFunction() {ResetConvKernelArgs();};

//-----------------------------------------------------------------------------------
// ���������� ������������� ������-������� ������ TIR_EllipseFunction
//-----------------------------------------------------------------------------------
 //��������� ���������� ������� ���� �������
 virtual rvector& GetConvKernelArgs(rvector& Param) const //#1
 {Param.Set(3L,ROW,false);
  Param.Put(0L,_A); Param.Put(1L,_B); Param.Put(2L,_Brightness);
  return Param;
 };
 virtual uint GetTypeConvKernel() const {return CK_2D_ELLIPSE;};//#2
 //----------------------------------------------------------------------------------
 // ������� ��������� ������������� ������� ������
 //----------------------------------------------------------------------------------
 //��� ��������� ����������� ������� ������� (������)
 virtual string& GetType(string& s) const
 {s.assign("2D-ImpulseResponse with EllipseFunction ConvKernel"); return s;};//#3
 //������������ �������� � ������� ���� �������
 virtual string& GetConvKernel(string& s, uint left_indent = 0) const;//#4
 virtual string& BriefConvKernel(string& s) const;//#5
 //----------------------------------------------------------------------------------
 // ���������� ������� ����������� �������������� ������� ������
 //----------------------------------------------------------------------------------
 //�������������� ���������� ������� ���� �������
 virtual void EditConvKernel(uint left_indent = 0);//#6
 //----------------------------------------------------------------------------------
 //��������� ������� ������
 //----------------------------------------------------------------------------------
 //��������� ���������� ������� ���������� �� ������ �� ������� ���� rvector:
 //_A = Param[0]; _B = Param[1]; _Brightnrss = Param[2];
 virtual bool SetConvKernelArgs(const rvector& Param) //#7
 {if (Param.Size() != 3) return false;
  SetConvKernelArgs(Param.Get(0L),Param.Get(1L),Param.Get(2L));
  return true;
 };
 virtual bool SetDefConvKernelArgs()
 {SetA(1.0); SetB(1.0); SetBrightness(1.0); return true;};//#8
 virtual bool ResetConvKernelArgs() //#9
 {_A = 0.0; _SqA = 0.0; _B = 0.0; _SqB = 0.0; _Brightness = 0.0; return true;};
 void SetConvKernelArgs(double a, double b, double brightness)
 {SetA(a); SetB(b); SetBrightness(brightness);};
 //----------------------------------------------------------------------------------
 //���������� ���������� ��� ������ TIR_EllipseFunction
 //----------------------------------------------------------------------------------
 //���������� ��������� ������������ (=)
 TIR_EllipseFunction* operator =(const TIR_EllipseFunction& H)
 {if (this == &H) return this;
  rvector Args;
  Set(H.GetInHeight(),H.GetInWidth(),H.GetOutHeight(),H.GetOutWidth());
  SetConvKernelArgs(H.GetConvKernelArgs(Args));
  return this;
 };
//-----------------------------------------------------------------------------------
//II. ���������� ���������� ������-������� ������ TIR_EllipseFunction
//-----------------------------------------------------------------------------------
protected:
 //���� ������� (Convolution kernel)
 virtual double ConvolutionKernel(long i, long j) const
 {return (((i*i)/_SqA + (j*j)/_SqB) <= 1) ? _Brightness : 0.0;};//#10
 //�������������� ������� ���� �������
 virtual bool IsConvKernelSymmetric() const {return true;};//#11
 //��������� ���������� ������� ���� ��������� �������
 void SetA(double a) {_A = (a == 0.0) ? 1.0 : fabs(a); _SqA = _A*_A;};
 void SetB(double b) {_B = (b == 0.0) ? 1.0 : fabs(b); _SqB = _B*_B;};
 void SetBrightness(double c) {_Brightness = fabs(c);};
};//���������� ���������� ������ TIR_EllipseFunction
//------------------- ���������� ������ TIR_EllipseFunction -------------------------

//***********************************************************************************
//                 ���������� ������ TDisturbance
//����� ��������� �������������� �������� ��������������� ��������� ��������� � ����-
//������� ����, ������� ����� ����� � �������� ����������� �������� ������� (��������-
//���) �� ������������ (���������) ������� � ���������� ��������� ����������� �������
//������� ���������� �������. ��������������, ��� ��� ���������� ��������� �����������
//������� ���������� ������� (��������� ��������� ����������� ������� ������� ��������)
//������ �������� ����������� W ����������� ����� ������������ ���������� ����������
//� ��������������� ���������� ������� ���� � ��������� �������� ���������. ����������
//��� ��������� ����������:
//1. ��������������� ������ ������ ��������� ����������� ������� ������� H.
//   ��������������� ������� ��������� - DispH (DispH >= 0). ��������� ������� �����-
//   ����� � ������ � ������ ��������� H ����� ���� ����� ��� ������� �����.
//   �������������� � ���� ���� ������������ - lNoiseH (true - ����� ���).
//2. ��������������� ������ ��� ��������� ��������� H (��� ������ ���������������� �
//   ���������������� ������). ��������������� ������� ��������� - DispInvH
//   (DispInvH >= 0). ��������� ������� ���������� � ������ ��� ��������� ��������� H
//   ����� ���� ����� ��� ������� �����. �������������� � ���� ���� ������������ -
//   lNoiseInvH (true - ����� ���).
//3. ���������� ��� ������� (������ �����). ��������������� ������� ����������� ����
//   ���������� - DispNoise (DispNoise >= 0).
//����� ������������ ��� ������ ������� ��������������� ������ � ����������� ���� ��
//�������� ����������� �������� (���������) ������� (�����������) ������� ����������
//������� ��� ���������� ��������� �����������. ���������� ��������� ����������� W
//�������� ������������������ ���������� ����������.
//������� ������: ��������, �������������� � �������� �������������� ������� ��������-
//��� ��������� ��� ������ �� ������� �� �������� �����������.
//***********************************************************************************
class TDisturbance {
//-----------------------------------------------------------------------------------
// ���������� �������� ������-������ ������ TDisturbance
//-----------------------------------------------------------------------------------
private:
 //1. �������������� �������� ��������������� ������ ������ ��������� H
 double _DispH;//���������
 bool lNoiseH;//��� ���� [����� (true) ��� ������� (false)]
 //2. �������������� �������� ��������������� ������ ��� ��������� ��������� H
 double _DispInvH;//���������
 bool lNoiseInvH;//��� ���� [����� (true) ��� ������� (false)]
 //3. �������������� �������� ����������� ����
 double _DispNoise;//��������� ����������� ����

//-----------------------------------------------------------------------------------
// ���������� ������������� � ����������� ������ TDisturbance
//-----------------------------------------------------------------------------------
public:
 //����������� �� ���������
 TDisturbance() {Set(0.0,false,0.0,false,0.0);};
 //������������ � �����������
 TDisturbance(double DispH, bool NoiseH, double DispInvH, bool NoiseInvH, double DispNoise)
 {Set(DispH, NoiseH, DispInvH, NoiseInvH, DispNoise);};
 //���������� ����������� ������
 ~TDisturbance() {Set(0.0,false,0.0,false,0.0);};

//-----------------------------------------------------------------------------------
// ���������� ������������� ������-������� ������ TDisturbance
//-----------------------------------------------------------------------------------
 //��������� ������� ������
 double GetDispH() const {return _DispH;};
 bool GetNoiseH() const {return lNoiseH;};
 double GetDispInvH() const {return _DispInvH;};
 bool GetNoiseInvH() const {return lNoiseInvH;};
 double GetDispNoise() const {return _DispNoise;};

 //----------------------------------------------------------------------------------
 // ������� ��������� ������������� ������� ������ TDisturbance
 //----------------------------------------------------------------------------------
 //�������������� �������� ��������������� ������ ������ ��������� H
 string& RandProcH(string& s, uint left_indent = 0) const;
 string& BriefRandProcH(string& s) const;
 //�������������� �������� ��������������� ������ ��� ��������� ��������� H
 string& RandProcInvH(string& s, uint left_indent = 0) const;
 string& BriefRandProcInvH(string& s) const;
 //�������������� �������� ����������� ����
 string& RandProcNoise(string& s, uint left_indent = 0) const;
 string& BriefRandProcNoise(string& s) const;

 //������������ ���� ������� ������ � ���� ������
 string& Info(string& s, bool lInvH = true, string& head = string(),
              uint left_indent = 0) const;
 string& BriefInfo(string& s, bool lInvH = true, bool lNoise = true) const;

 string& Properties(string& s, bool lInvH = true, string& head = string(),
         uint left_indent = 0) const {return Info(s, lInvH, head, left_indent);};

 //OVERLOAD THE STREAM INSERTION OPERATOR
 //���������� �������� ���������� ������ (<<) ������� ������
 friend ostream& operator <<(ostream& out, const TDisturbance& Disturb)
 {string s; return out << Disturb.Info(s);};
 //----------------------------------------------------------------------------------
 // ���������� ������� ����������� �������������� ������� ������
 //----------------------------------------------------------------------------------
 //�������������� c������������� ������� ��������������� ������ ������ ��������� H
 void EditRandProcH(uint left_indent = 0);
 //�������������� �������������� ������� ��������������� ������ ��� ��������� ��������� H
 void EditRandProcInvH(uint left_indent = 0);
 //�������������� �������������� ������� ����������� ����
 void EditRandProcNoise(uint left_indent = 0);
 //�������������� ���� ������� ������� ������
 void Edit();
 //----------------------------------------------------------------------------------
 //��������� ������� ������
 //----------------------------------------------------------------------------------
 //��������� c������������� ������� ��������������� ������ ������ ��������� H
 double SetDispH(double Disp) {_DispH = fabs(Disp); return GetDispH();};
 bool SetNoiseH(bool Noise) {lNoiseH = Noise; return GetNoiseH();};
 void SetRandProcH(double Disp, bool Noise) {SetDispH(Disp); SetNoiseH(Noise);};

 //��������� �������������� ������� ��������������� ������ ��� ��������� ��������� H
 double SetDispInvH(double Disp) {_DispInvH = fabs(Disp); return GetDispInvH();};
 bool SetNoiseInvH(bool Noise) {lNoiseInvH = Noise; return GetNoiseInvH();};
 void SetRandProcInvH(double Disp, bool Noise) {SetDispInvH(Disp); SetNoiseInvH(Noise);};

 //��������� �������������� ������� ����������� ����
 double SetDispNoise(double Disp) {_DispNoise = fabs(Disp); return GetDispNoise();};
 void SetRandProcNoise(double Disp) {SetDispNoise(Disp);};

 //��������� ���� ������� ������
 void Set(double DispH, bool NoiseH, double DispInvH, bool NoiseInvH, double DispNoise)
 {SetRandProcH(DispH,NoiseH);
  SetRandProcInvH(DispInvH,NoiseInvH);
  SetRandProcNoise(DispNoise);
 };

//-----------------------------------------------------------------------------------
//II. ���������� ���������� ������-������� ������ TDisturbance
//-----------------------------------------------------------------------------------

};//���������� ���������� ������ TDisturbance
//---------------------- ���������� ������ TDisturbance -----------------------------

//***********************************************************************************
//                 ���������� ������ TRandomProcess
//����� ��������� �������������� �������� ���������� �������� � �������������� �����-
//���� ������ ���� � ���������� �������� ��������� (Disp >= 0). ��������� ������� ��-
//��� ���� ������� � ������ ��� �������� (��������) ���� (lNoise).
//����� ������������ ��� ������ ������� ��������������� ������ � ����������� ���� ��
//�������� ����������� �������� (���������) ������� (�����������) ������� ����������
//������� ��� ���������� ��������� �����������. ���������� ��������� ����������� W
//�������� ���������(�) ���������� � ���. ��������� ������ ����.
//����� ��������� ������� ��������� ��������� �������� ��������� ��� ���������� ���-
//��������� ��������, ��� ��������� �� ���� ������ ������������� ���������� ���������
//��������� � ����������� �������� ���������� ���������, ��� ���� �� ����������� ����-
//��������� ��� ������ �������� ��������� �� ���� ������.
//������� ������: ��������, �������������� � �������� �������������� ������� ��������
//��� ������ ��� ������� �� �������� �����������.
//***********************************************************************************
class TRandomProcess {
//-----------------------------------------------------------------------------------
// ���������� �������� ������-������ ������ TRandomProcess
//-----------------------------------------------------------------------------------
private:
 rvector DispSet;//������ ��������� (dispersion) ���������� ��������
 bool lNoise;//��� ���� [����� - N_WHITE(true) ��� ������� - N_COLOR(false)]
//-----------------------------------------------------------------------------------
// ���������� ������������� � ����������� ������ TRandomProcess
//-----------------------------------------------------------------------------------
public:
 //����������� �� ���������
 TRandomProcess() {Set();};
 //������������ � �����������
 TRandomProcess(const rvector& Disp, bool Noise) {Set(Disp, Noise);};
 //���������� ����������� ������
 ~TRandomProcess() {Reset();};
//-----------------------------------------------------------------------------------
// ���������� ������������� ������-������� ������ TRandomProcess
//-----------------------------------------------------------------------------------
 //��������� ������� ������
 ulong DispCount() const {return DispSet.Size();};
 double GetDisp(ulong i) const {return (i >= DispCount()) ? 0 : DispSet.Get(i);};
 rvector& GetDisp(rvector& Disp) const {Disp = DispSet; return Disp;};
 bool GetNoise() const {return lNoise;};
 bool IsWhiteNoise() const {return GetNoise();};//lNoise == N_WHITE == true
 //��������� ������� � ������ ��������� �������� ��������� �� ����
 bool IsNonZeroDisp() const
 {return (DispCount() > 1) ? true : (GetDisp(0) != 0.0) ? true : false;}
 //----------------------------------------------------------------------------------
 // ������� ��������� ������������� ������� ������ TRandomProcess
 //----------------------------------------------------------------------------------
 //������������ ����������� ������������� �������� ��������� � ������ ������ maxlen
 string& Disp(string& s, uint maxlen = 0, uint prec = 6) const;
 //������������ ������������ ���� ���������� �������� (WHITE || COLOR)
 string& Noise(string& s, bool brief = true) const;
 //������������ ����������� ������������� �������� i-�� ���������
 string& Value(string& s, ulong i, uint precision = 6) const;
 //������������ ���������� ������������� �������� i-�� ��������� � ���� ����
 string& ValueInfo(string& s, ulong i, bool brief = true, uint precision = 6) const;
 //������������ ������� ������ � ���� ������
 string& Info(string& s, uint maxlen = 0, uint precision = 6) const;
 //OVERLOAD THE STREAM INSERTION OPERATOR
 //���������� �������� ���������� ������ (<<) ������� ������
 friend ostream& operator <<(ostream& out, const TRandomProcess& Random)
 {string s; return out << Random.Info(s);};
 //----------------------------------------------------------------------------------
 // ���������� ������� ����������� �������������� ������� ������
 //----------------------------------------------------------------------------------
 //�������������� �������� ���������
 void EditDisp(uint left_indent = 0);
 //�������������� ���� ����
 void EditNoise(uint left_indent = 0);
 //�������������� ���� ������� ������� ������
 void Edit(string& head = string(), bool lDispEdit = true, bool lNoiseEdit = true);
 //----------------------------------------------------------------------------------
 //��������� ������� ������
 //----------------------------------------------------------------------------------
 //��������� �������� ��������� ���������� ��������
 ulong SetDisp(const rvector& Disp);
 //��������� ���� ���������� ��������
 bool SetNoise(bool Noise) {return lNoise = Noise;};
 //��������� �������� ��������� � ���� ���������� ��������
 ulong Set() {DispSet.Set(1,ROW,0.0); lNoise = N_WHITE; return DispCount();};
 ulong Set(const rvector& Disp, bool Noise) {SetNoise(Noise); return SetDisp(Disp);};

//-----------------------------------------------------------------------------------
//II. ���������� ���������� ������-������� ������ TRandomProcess
//-----------------------------------------------------------------------------------
protected:
 void Reset() {DispSet.Resize(0L); lNoise = N_WHITE;};

};//���������� ���������� ������ TRandomProcess
//---------------------- ���������� ������ TRandomProcess ---------------------------

//***********************************************************************************
//                 ���������� ������ TRecoveryDisturbance
//����� ��������� �������������� �������� ��������������� ��������� ��������� � ����-
//������� ����, ������� ����� ����� � �������� ����������� �������� ������� (��������-
//���) �� ������������ (���������) ������� � ���������� ��������� ����������� �������
//������� ���������� �������. ��������������, ��� ��� ���������� ��������� �����������
//������� ���������� ������� (��������� ��������� ����������� ������� ������� ��������)
//������ �������� ����������� W ����������� ����� ������������ ���������� ����������
//� ��������������� ���������� ������� ���� � ��������� �������� ���������. ������
//��������� ������� ����������� ������� TRandomProcess, ������� ��������� ��������
//������ ������� ��������� ��� ������� ���������� �������� � ���������������� ������-
//��� ������� ��� ����� (WHITE) ��� ������� (COLOR) ���. ��������� ����������:
//1. ��������������� ������ ������ ��������� ErrH ����������� ������� ������� H.
//2. ��������������� ������ ErrInvH ��� ��������� ��������� H (��� ������ ����������-
//   ������ � ���������������� ������).
//3. ���������� ��� ErrNoise ������� (������ �����).
//����� ������������ ��� ������ ������� ��������������� ������ � ����������� ���� ��
//�������� ����������� �������� (���������) ������� (�����������) ������� ����������
//������� ��� ���������� ��������� �����������. ���������� ��������� ����������� W
//�������� ������������������ ���������� ����������.
//������� ������: ��������, �������������� � �������� �������������� ������� ��������-
//��� ��������� ��� ������ �� ������� �� �������� �����������.
//***********************************************************************************
class TRecoveryDisturbance {
//-----------------------------------------------------------------------------------
// ���������� �������� ������-������ ������ TRecoveryDisturbance
//-----------------------------------------------------------------------------------
private:
 //1. �������������� �������� ��������������� ������ ������ ��������� H
 TRandomProcess _ErrH;
 //2. �������������� �������� ��������������� ������ ��� ��������� ��������� H
 TRandomProcess _ErrInvH;
 //3. �������������� �������� ����������� ����
 TRandomProcess _Noise;
//-----------------------------------------------------------------------------------
// ���������� ������������� � ����������� ������ TRecoveryDisturbance
//-----------------------------------------------------------------------------------
public:
 //����������� �� ���������
 TRecoveryDisturbance() {Set();};
 //������������ � �����������
 TRecoveryDisturbance(const rvector& DispH, bool NoiseH, const rvector& DispInvH,
                      bool NoiseInvH, const rvector& DispNoise)
                      {Set(DispH, NoiseH, DispInvH, NoiseInvH, DispNoise);};
 //���������� ����������� ������
 ~TRecoveryDisturbance() {};

//-----------------------------------------------------------------------------------
// ���������� ������������� ������-������� ������ TRecoveryDisturbance
//-----------------------------------------------------------------------------------
 //��������� ������� ������
 ulong DispErrH() const {return _ErrH.DispCount();};
 ulong DispErrInvH() const {return _ErrInvH.DispCount();};
 ulong DispNoise() const {return _Noise.DispCount();};

 double DispErrH(ulong i) const {return _ErrH.GetDisp(i);};
 double DispErrInvH(ulong i) const {return _ErrInvH.GetDisp(i);};
 double DispNoise(ulong i) const {return _Noise.GetDisp(i);};

 bool NoiseErrH() const {return _ErrH.GetNoise();};
 bool NoiseErrInvH() const {return _ErrInvH.GetNoise();};
 bool Noise() const {return _Noise.GetNoise();};

 bool IsWhiteErrH() const {return _ErrH.IsWhiteNoise();};
 bool IsWhiteErrInvH() const {return _ErrInvH.IsWhiteNoise();};
 bool IsWhiteNoise() const {return _Noise.IsWhiteNoise();};

 bool IsNonZeroErrH() const {return _ErrH.IsNonZeroDisp();};
 bool IsNonZeroErrInvH() const {return _ErrInvH.IsNonZeroDisp();};
 bool IsNonZeroNoise() const {return _Noise.IsNonZeroDisp();};

 //----------------------------------------------------------------------------------
 // ������� ��������� ������������� ������� ������ TRecoveryDisturbance
 //----------------------------------------------------------------------------------
 //�������������� �������� ��������������� ������ ������ ��������� H
 string& DispErrH(string& s, uint maxlen = 0, uint precision = 6) const
         {return _ErrH.Disp(s,maxlen,precision);};
 string& NoiseErrH(string& s, bool brief = true) const
         {return _ErrH.Noise(s,brief);};
 string& InfoErrH(string& s, uint maxlen = 0, uint precision = 6) const
         {return _ErrH.Info(s,maxlen,precision);};

 //�������������� �������� ��������������� ������ ��� ��������� ��������� H
 string& DispErrInvH(string& s, uint maxlen = 0, uint precision = 6) const
         {return _ErrInvH.Disp(s,maxlen,precision);};
 string& NoiseErrInvH(string& s, bool brief = true) const
         {return _ErrInvH.Noise(s,brief);};
 string& InfoInvErrH(string& s, uint maxlen = 0, uint precision = 6) const
         {return _ErrInvH.Info(s,maxlen,precision);};

 //�������������� �������� ����������� ����
 string& DispNoise(string& s, uint maxlen = 0, uint precision = 6) const
         {return _Noise.Disp(s,maxlen,precision);};
 string& Noise(string& s, bool brief = true) const
         {return _Noise.Noise(s,brief);};
 string& InfoNoise(string& s, uint maxlen = 0, uint precision = 6) const
         {return _Noise.Info(s,maxlen,precision);};

 //������������ ���� ������� ������ � ���� ������
 string& Info(string& s, bool lErrH = true, bool lErrInvH = true, bool lNoise = true,
              uint maxlen = 0, uint precision = 6) const;
 //OVERLOAD THE STREAM INSERTION OPERATOR
 //���������� �������� ���������� ������ (<<) ������� ������
 friend ostream& operator <<(ostream& out, const TRecoveryDisturbance& Disturb)
 {string s; return out << Disturb.Info(s);};

 //----------------------------------------------------------------------------------
 // ���������� ������� ����������� �������������� ������� ������
 //----------------------------------------------------------------------------------
 //�������������� ���� ������� ������� ������
 void Edit(bool lErrH = true, bool lErrInvH = true, bool lNoise = true);
 //----------------------------------------------------------------------------------
 //��������� ������� ������
 //----------------------------------------------------------------------------------
 //��������� c������������� ������� ��������������� ������ ������ ��������� H
 ulong SetErrH(const rvector& Disp, bool Noise) {return _ErrH.Set(Disp,Noise);};

 //��������� �������������� ������� ��������������� ������ ��� ��������� ��������� H
 ulong SetErrInvH(const rvector& Disp, bool Noise) {return _ErrInvH.Set(Disp,Noise);};

 //��������� �������������� ������� ����������� ����
 ulong SetNoise(const rvector& Disp) {return _Noise.Set(Disp,N_WHITE);};

 //��������� ���� ������� ������
 void Set(const rvector& DispH, bool NoiseH, const rvector& DispInvH, bool NoiseInvH,
          const rvector& DispNoise)
 {SetErrH(DispH,NoiseH);
  SetErrInvH(DispInvH,NoiseInvH);
  SetNoise(DispNoise);
 };
 void Set() {_ErrH.Set(); _ErrInvH.Set(); _Noise.Set();};

//-----------------------------------------------------------------------------------
//II. ���������� ���������� ������-������� ������ TRecoveryDisturbance
//-----------------------------------------------------------------------------------


};//���������� ���������� ������ TRecoveryDisturbance
//------------------ ���������� ������ TRecoveryDisturbance -------------------------


//***********************************************************************************
// ���������� ������ TGaussDistortionOperator - ������� �������� ���������
//����� ��������� ����������� �������� ��������� � ���� ���������� ������� H �
//�������� �������������� � ���� �������� ������� invH � ������� ��������� H.
//����� ������������ ��� �������������� ������������� ���������� ����������
//����������� ��� ���������� ��������� ��������������. ��� ����������� �������
//���������� ����������� X(K x M), ����������� ����������� ��������� H(N x N),
//��� N = K*M. ������ ������� ������� H ����������� �� ���������� ������:
//                H(i,j) = EXP(-[t1*(k-i)^2 + t2*(m-j)^2]),
//t1, t2 - ��������� ���������� ��������� ��������� (0 <= t1, t2 < +INF);
//k, m - ������� ������� X (0 <= k < K; 0 <= m < M).
//������� H - ��������������, ������������ ������������ � ��������� �������
//����������. ������� invH - �������� ������� � ������� H, ������������ ���
//�������������� ����������� �����������.
//H(i-k,j-m) - ������ ����������� ������� ������������ �������, �������� ����������.
//         ��������  ����������  ���������� ��������� ��������� H
//����� �������� ������� �����������, ��������������, ��������, �������� A(3x3),
//���������� ������� ������� H ������������ (9x9) ��������� ���������:
//   |           J=0          |           J=1            |           J=2             |
//   | m=0    m=1     m=2     | m=0    m=1     m=2       | m=0    m=1     m=2        |
//---+-------------------------------------------------------------------------------+--
//k=0| h(0,0) h(0,-1) h(0,-2) | h(0,0)  h(0,-1) h(0,-2)  | h(-2,0) h(-2,-1) h(-2,-2) |
//k=1| h(0,1) h(0,0)  h(0,-1) | h(-1,1) h(-1,0) h(-1,-1) | h(-2,1) h(-2,0)  h(-2,-1) |I=0
//k=2| h(0,2) h(0,1)  h(0,0)  | h(-1,2) h(-1,1) h(-1,0)  | h(-2,2) h(-2,0)  h(-2,0)  |
//---|-------------------------------------------------------------------------------|--
//k=0| h(1,0) h(1,-1) h(1,-2) | h(0,0)  h(0,-1) h(0,-2)  | h(-1,0) h(-1,-1) h(-1,-2) |
//k=1| h(1,1) h(1,0)  h(1,-1) | h(0,1)  h(0,0)  h(0,-1)  | h(-1,1) h(-1,0)  h(-1,-1) |I=1
//k=2| h(1,2) h(1,1)  h(1,0)  | h(0,2)  h(0,1)  h(0,0)   | h(-1,2) h(-1,1)  h(-1,0)  |
//---|-------------------------------------------------------------------------------|--
//k=0| h(2,0) h(2,-1) h(2,-2) | h(1,0)  h(1,-1) h(1,-2)  | h(0,0)  h(0,-1)  h(0,-2)  |
//k=1| h(2,1) h(2,0)  h(2,-1) | h(1,1)  h(1,0)  h(1,-1)  | h(0,1)  h(0,0)   h(0,-1)  |I=2
//k=2| h(2,2) h(2,1)  h(2,0)  | h(2,2)  h(1,1)  h(1,0)   | h(0,2)  h(0,1)   h(0,0)   |
//---|-------------------------------------------------------------------------------|--
//�������� i � j h(i,j)-�� �������� H ����������� �� �������: i=I-J; j=k-m.
//������������ k,I � m,J ���������� � ��� �� ��������, ��� � ������� ����� � ��������
//� ������� ��������� ����������� A. ������������ k,m ������������ ���������� ������
//������� ����� ������� H �� 0 �� K � M ��������������. ������������ I,J ����������
//�� 0 �� K � M ��� �������� �� ����� � ����� ������� H �� ��������� � �����������
//��������������. ���������� ������ ������� H ����� �� �����������, ����� �������,
//������� I � J ���������� ���������� ������ ���� ������� � ���������� ������� ���������.
//������������ k � m ���������� ���������� ������ ������� ����� (I,J) ������� H.
//�������� �������� i,j h(i,j)-�� �������� ������� H, ������������ ��� ���������� ��������
//������� �������� ��������� ��������� �� �������������� �������:
// H(i,j) = EXP(-[t1*(I-J)^2 + t2*(k-m)^2]).
//    �������� ��������� ��������� (��������) ����������� �� ����������� ������
//��� ���������� �������� ��������� �����������, ��������������� � ���� �������������
//������� A(K x M) ���������� ��������� ��������� ����:
//1) ��������� ������� �������� ��������� H ����������� N = K*M c ����������� t1, t2
//   �� �������������� ������.
//2) ������������� ������� A � ������-������ (������-�������) X �������� N=K*M.
//   ��������, ��� A(5x3) � ������-������ X(15):
//    | 1 1 1 |         i=0  |  i=1 | i=2 |  i=3 |  i=4
//    | 1 0 0 |       -----------------------------------
//A = | 1 0 0 | --> X |1 1 1  1 0 0  1 0 0  1 0 0  1 1 1|
//    | 1 0 0 |       -----------------------------------
//    | 1 1 1 |
//
//3) ������ ����������� ����������� Y ���������� ��������� ��������� ������ ���������
//   ��������� H �� �������� ����������� X: Y = X*H.
//4) ��� ��������� ������� ����������� ����������� B ���������� ������������� ������-������
//   ����������� ����������� Y � ������� � ������������ ����������� ������� ���������
//   ����������� A.
//     ����������� ����������� ����������� �� ���������� ��������� ��������� H
//1) ��������� �������� ������� invH � ������� ��������� ��������� H.
//2) ����������� ������� ����������� ����������� B � ������-������ (������-�������) Y
//   �� ������������� � ������ 2 ��������.
//3) ��������������� �������� ������ ����������� X ���������: X = Y*invH.
//4) ����������� ������-������ ����������������� ����������� X � ������� ���������
//   ����������� A ����������� ������� ����������� ����������� B.
//-----------------------------------------------------------------------------------
//���������:
//1. ���������� ��������� ��������� ��������� (��������� ��������������) ��������
//�������������� �������� ��� ������� ������. � ����� �������� ������ � ������� ������
//��������� ������������ ����� ��� ���������� ������������� ���������� invH ��� ���
//��������� ������������ ��������� ��������� H, ��� � ��� ����������� ��� ������������
//� ���� �������������.
//2. ��� ����������� �� ���� ������� ������� ����������� ����������� ����� �������������
//��������� �������� ��������� H (�������������� invH) �������� ����������� �������
//������� ��� ���������� �������� ��������� (��������������) ��� ���. ������������
//�������������� ������ ���������� ����� �������� (��������� ��� ��������������) ���
//������� �������� ����������� �� ����� �����������, �.�. ������������ �������� ��
//������� �������� ��� ���������� ������������.
//3. ��� ��������������� ������� TGaussDistortionOperator �� ��������� ���������
//������� H � invH �������� �������, ��������� t1 � t2 ����� 0,
//����� �������� Mode = SLEEP.
//4. ����� ��������� �������, ��������� � ������������� ������ H � invH, ����������
//�� ������������� � ������ ������ ������� �� ������ ������ �������, ������� ������������
//��������� Mode. ����� Mode �������� ������������� � ������ �� �������� ���������
//������ �������, ��������� � ��������� H � invH, � ����� �� ����� ������, �������
//������ ������ TGaussDistortionOperator �������� � ������.
//        �������� �������� ������-������ ������ TGaussDistortionOperator
//1. t1, t2 - ��������� ���������� ��������� ��������� : t1, t2 >= 0;
//2. H - ���������� �������������� ������������-������������ ������� ��������� ���������;
//3. invH - ������� ��������� �������������� (������� �������� ������� H);
//4. Height, Width - ������ � ������ ������� ����������� ��� �������� ��������
//   ������� ���������� ��������� � �������������� - H � invH. ��� Height==Width==0
//   ��������� �� ������� (������ ������ �� �������);
//5. Mode - ����� �������� ������ ���������� ��������� � ��������������:
//   SLEEP - ������� H � invH �������� �������, ��������� ������ � ������ ���������
//   �������, ��������� � ����, ����� ��������� ������� ��� ��������� �� ������.
//   ������������: ������ �������� ���������� ��������� ������ � ������.
//   ����������: ������������� �������� ���������� ��������� � �������������� ������
//   ��� ��� ������������� ���������� � ���� �������, ����� ����������� ����� ������,
//   ���� ��������� �������� (������������) ������������������ ����������� �����������
//   ������� ��� ������������� ���������� t1, t2.
//   OPTIMAL - ������ ���������� ������� H, ������� invH - �������� ������� � ���������
//   ������ ��� ��������� �������, ���������� � ���, ����� ��������� ������� �������
//   invH ��������� �� ������.
//   ������������: �������� �������������� ��������� �������, ��������� � ����������
//   ������������������ ����������� ������ ������� ��� ������������� ���������� t1, t2
//   �� ��������� � ������� SLEEP.
//   ����������: ������ �������� � ������ ������ �����, ��� � ������ SLEEP, �� ����
//   ����������� �������� � ������ ������� H.
//   FULL - ������� H � invH �������� � ������ ������ ��� �����. ����������� ������
//   �������� ��������� � ����������� �� ����������� �����������, ������� ����������
//   ����������. ����� ��������� �������, ���������� � ��������� ���� ������� H ����
//   ������� invH, ��� �������� � ������ � ������ �������.
//   ������������: �������� �������������� ��������� �������, ��������� � ����������
//   � ������������� ��������������� ������������������ ����������� ������ ������� ���
//   ������������� ���������� t1, t2 �� ��������� � ������� OPTIMAL.
//   ����������: ������ �������� ����������� ��������� ������ � ������; �������
//   �������������� ��������� �������, ��������� ������ � ���������� ������������������
//   ����������� ������ �����������, �.�. ������ ��� ��������� ��������� � ������� invH
//   ���� � �� ������������� ��� �������������.
//
//***********************************************************************************

//--------------------- The end of file "image.h" ---------------------------
#endif //���������� ���������� ������������� ����� "IMAGE.H"