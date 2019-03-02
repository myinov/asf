//----------------------------------------------------------------------------------------
//SF_CommonFunc.h     C++ Builder 6.0 JohnSoft Initial date: 23/03/2007
//���������� ������������������� ������������ �������, ������������ ��� �������������
//���������� ������ (��) ���������������� ��������� �������� �� ���� ������� ������� �����
//� ������ ��������������� ���������� ���������������� ������� ��.
//----------------------------------------------------------------------------------------
#ifndef SF_CommonFuncH
#define SF_CommonFuncH
//----------------------------------------------------------------------------------------
//���������� ������������ ������
#include "const.h"
#include "matrix.h"
#include "arrays.h"
#include "Regularity.h"
#include "RandGen.h"
#include "CoDec.h"

//****************************************************************************************
//��������������� ���������� ���� ������� � ��������
//****************************************************************************************
//���������� ��������� ������ ����������� ������ ������� ���������� ���������� ��������
//��������� ����������������� ������� �������������� �������. ������� ������ ������� �����-
//������ ����� ��������� ������������������ ��������� �������������� ������� �������� ���-
//����� EstRsjn.
struct TResultStatCalcQadaptDM {
 double Q;    //���. �������� ���������� �������� ���������
 double MSD_Q;//���� ���������� �������� ���������
 ulong InvErr;//������� ����� ��������� ��������� EstRsjn
 string Msg; //������ ��������� ���� ���������� �������
};//����� ���������� ��������� TResultStatCalcQadaptDM

//****************************************************************************************
// ��������������� ���������� �������
//****************************************************************************************
//������� ������� ���������� �������� ��������� ����������������� ������� ��������������
//�������. ������� ������ ������� ����������� ����� ��������� ������������������ ���������
//�������������� ������� �������� �������� EstRsjn. ���. �������� ���������� �������������
//����������� �� �������: Q=10*Log10(Ps/OutPsjn) ����� ���������� Test ������������� �
//����������� ���������� �����������. ���������� ������������� Q ����������� �������������-
//���� ������� ������� �� ��������� ��������:
//1) �������� ������� (����� ��������������� ������� ������� - �������� ������� ��������
//   ������� � �� ����������� ����) => LAA;
//2) ��������� �������� - ��������� �������� ������ � ����� ���������� ����� => Jams;
//3) ��������� ��������� ������������������ �������������� ������� EstRsjn => pInvOp;
//4) �������� ��������� ������������� => alpha;
//5) ������ �������, �� ������� ����������� ��������� ������� EstRsjn => n;
//6) �������� ��������� ������� � ��� �������� ��������� => Signal_2D;
//7) �������������� ����� ��������� ����������� �������� ������� ����������� �������, ����-
//   ��� ��������� ��������� ���������� ������� � ��������� �����������. ��� ����� ��������
//   �����������, ����������� � ������������ ������� ��������� ������� => ElimDirs.
//� ������ ������� ������� ������ ������������������ �������� ������� � ��������������
//(���������) ��������� ���������� ������ � ����������� ����. � ����� � ���� �����������
//�� ������ �������� ���������� ������������� Q, � ��� �������������� ��������, ��� �������
//���������� Test �������������. ������ � ���. ��������� Q ����������� � ��� ����.
//������� �������� �������� �� ������������ ���������� ���������� ���������. ��� �����
//������ ����������� ��������� �������:
//1) ����� �������� ������� ������� Naux >= 1, ��� ������� ������� ������� ���� ������� LAA -
//   ��������� � ������ ���������������� ������� ��� ����������� ����������������� �������;
//2) �������� ��������� ������� Ps > 0;
//3) ������� ��������� ��������� ��������� ������� �������� �������� EstRsjn => pInvOp!=NULL;
//4) ����� ������� ������� �� ���� n > 0.
//������ ������� �� ����������� ��������� EstRsjn. ���, ��������, ��� ��������� ����������-
//���������� ������� EstRsjn (������������ ����� ���������) � ������ n < Naux ������ �������
//����� �����������. � ���� ������, �� �������� �������� �� ������ ����������� �������
//OutPsjn ��������� ������ �������� �� ������ ��������� ������, ��� ������������� ��������
//�������� ������� �������������� ������� ������� Waux (�������������� �������). � ���������
//�������, ��� ��������� ��������� �������������� ������� �������� �������� EstRsjn ��������
//�� ������ ����������� ������� ����������� �� ��������� �����:
//1) ��������� ��������� �������������� ������� EstRsjn ����� ���������� n ����������
//   �������������� ������ Rsjn(i), 1 < i <= n. ������ Rsjn(i) ����������� �� �����������-
//   ���� �������� ������� U(i) = Us+Ujam(i)+Un(i), Ujam(i), Un(i) - �������������� �������
//   �������� ����������� ������ � ����������� ���� ������������ N (�� ����� ������� LAA),
//   Us - ����������������� ������� ������������ N. Rsjn(i) = U(i)*Tr[U(i)], ���
//   Tr[*] - �������� ����������-������������ ���������������� �������(��� �������).
//2) �� ������� EstRsjn ������������ ������� ��������������� ������� A � ������ ��������
//   ���������� �������������� � ��������� ������� ������� b. ����������� A � b ����� �����
//   �������������� ������� ������� Naux = N - 1;
//3) �������� �������������� ������������������ ������� A � ������ ������ �� ���������
//   ���������� ��������� ������� (����� �����������, ��������, �������� n-�� ������� ���
//   ��������) ���� ��� ��������������� ������������� (������������ �����). �������� �������
//   InvAa �������� � A. ��������� ��������� ����� �������� ��� �� ���������� ������ ����-
//   �����, ��� � �� �������� ��������� ������������� alpha, ������������� ��� ������
//   ��������� �������.
//4) ���������� ������� ������ ��� �������������� ������� ������� Waux:
//   - ��� ����� ��������� ����������� �������� �������: Waux = -InvAa*b;
//   - � ������ ����� �������� �����������: Waux = -InvAa*B*b.
//   ����� ��������� ����������� (����� ��������) ��������� ��������� ���������� ���������
//   ������� ���������� ���������������� ��������. ������� B - �������� ��������� ������-
//   �����: B = I-C*Inv[Tr[C]*InvAa*C]*Tr[C]*InvAa. ����������� ���� ������ ����� Naux.
//    Inv[*] - �������� ��������� �������;
//    I - ��������� �������;
//    C - ������� (Naux x K) ������� ������� ����� ��������������� � �������� ��������
//    ������� ��� �������� ����������� ���������� (K - ����� ����������� ����������).
//5) ��������� ����������� ������� ������ (� ������ ���� ��� ��������� ������ ������ 1):
//   ExtW = {Waux 1}.
//6) ������������ �������� �� ������: OutPsjn = Tr[ExtW]*EstRsjn*ExtW.
//����� �� ��������� Ps � OutPsjn ����������� ���������� ������������� ���������� �������.
//��� ������������ ���������� �������� Q (���. �������� Q) ����������� Test �������������.
//������� ���������� true � ������ ��������� ���������� Q (���� � ������ ������������� A)
//� false - � ��������� ������. ������� ��������� �������:
//1. TResultStatCalcQadaptDM& Result - ������ � ������������ ������ �������;
//2. TUnEqLAA_ODA& LAA - �������� �������������� ���������� ������� � ����������������
//   ��������� ���������� (�������� �������� �������);
//3. TGrayScaleImgSrc_2D& Signal_2D - �������� ���������� ������� (�����������);
//4. TDoubleSet& ElimDirs - ����������� ���������� (������������ ��� ��������� �����������
//   ������� �������� �������� ������� Waux � ����� ���������� ���������� ������� � ��������
//   �����������;
//5. TJammers_2D& Jams - ���������� ������;
//6. TCovMatrixInversion* pInvOp - �������� ��������� �������������� ������� �������� ���-
//   ����� � ������������ �� ��������������� ������������� �� �������� ���������� � �����
//   ����������� ������� ��������������� ������ �� �������� ����������� �����������, ���-
//   ��������� �� ���� ���������� ������.
//7. double alpha - �������� �������������;
//8. ulong n - �������� ������ �������;
//9. ulong Test - ����� �������������� ������ ��� ��������� �������� ������ ����������
//   �������� ��������� (���. �������� � ����).
//----------------------------------------------------------------------------------------
bool StatCalcQadaptDM(TResultStatCalcQadaptDM& Result, const TUnEqLAA_ODA& LAA,
     const TGrayScaleImgSrc_2D& Signal_2D, const TDoubleSet& ElimDirs,
     const TJammers_2D& Jams, TCovMatrixInversion* pInvOp, double alpha, ulong n,
     ulong Test);

//�������������� ���������� ������-�������� 2D-Signal (��������� �������� ������� �������-
//��� � ���������, ����� ������� �����������-�������������)
bool Edit_CoDec(TFuncGen_DSClipper** pCoDec);

//����� ��������� ���������� �������������� ������� �������� �������� (����� ������ ������-
//������� ��������������� ������)
bool Edit_DispOperator(TCovMatrixPerturbOperator** pDistOp);

//����� ������ ��������� �������������� ������� �������� �������� (� �� ���������������
//�������������� �� ���������� �����������, ��������, ���������������� ��������, ��������
//��� ��� ������������� (������������ �����))
bool Edit_InvOperator(TCovMatrixInversion** pInvOp);

//����� ������������������ (�������������� ������� Rsjn) ��� ���������� (��������������
//������ Usjn) �������� �������� ����������� �������
void Edit_InputProcess(enProcess& ProcessType, string& sProcessName);

//�������������� ���������� ���������� ��������� (�����) � ����� ������� ����������� �
//���������� ��������� ��������� ���������.
void Edit_JamParams(TDoubleSet& Angle, TDoubleSet& Psum);

//������ �������� �������� (������ + ���������� ������ + ���������� ���):
//1. ����������� �������� � ����������� ������� ������ Ajn, Asjn, Rjn, Rsjn;
//2. ������������ ������ Ajn, Asjn, Rjn, Rsjn (������������ ���� ����������� ��������);
//3. ����� ��������������� ������ Ajn, Asjn, Rjn, Rsjn: Wmax/Wmin (Wmax, Wmin - ���������-
//   ��� � ����������� ����������� �������� �������;
//4. ���� ������ Ajn, Asjn, Rjn, Rsjn (����� ������������ ��������� ��� ����� �����������
//   �������� �������);
//5. ������������� ����������� ����������: Ksup = det|R|/det|A|
bool CovMatrixAnalysis(const TUnEqLAA_ODA& LAA, const TJammers_2D& Jams, string& sInfo);

//������ ������� �������� ����������� �������� ������� W ���������� ������� �� ������
//��������: B = I - C*Inv(trC*InvA*C)*trC*InvA, ���:
//1) InvA - �������� ������� � �������������� ������� �������� �������� ��������-� �������
//   ���������� ������� (��������� �������) ����������� N;
//2) C - ����������� ������� ����������� (N*K), N - ���-�� �������������� ������� ��, K -
//   ����� ����������� (����������� ���������� - ����������� �������� ������ ����������
//   ��������� ��������������� �������� ��). ������ ������� ������� C ������ �������� ����-
//   ��� ������� ����� i-�� �������������� � �������� �������� �� (����������������� �������)
//   ��� �������� ��������� ��������� ��������� �������� � k-�� ����������� ����������.
//   ��� LAA ������������ ������� C ����������� �� �������: C[i,k]=EXP(j*2PI*d[i]*sin(Qk)),
//   d[i] - ������������� ���������� (� ������ ����) ����� i-� ��������������� � ��������
//   ��������; Qk - ������� ��������� k-�� ��������� ��������� (� ��������), ������������
//   ��� ������ ��������������� �������� �� (����������� ����������);
//3) I - ������������ ��������� ������� ����������� N.
//������� B ������������, ��� �������, ��� ������ ��������� ������� ��� ��� ���������� ��,
//����� ��������� ��������� ��������� ��������� ������� ��������.
bool LagrangeMethod(cmatrix& B, const cmatrix& InvA, const cmatrix& C);

//������ �� ���� ������ ���������������� �������� � ������ V � ���������� ��� �����������
//�� 1 � ������ ������������� ������� � ������ ������ ������ �������� (���������� �������)
bool InputSingleNonNegateValue(rvector& V, const char* name = NULL);

//���������� ���� ����� ������ ��� ���������� ��������������� �������������. ���������
//������������� �������� ����������� � ���������� Test, ������������ �� ������.
ulong InputTests(ulong& Test);

//���������� ���� ���������������� ������������� �������� � ����������� ���������� � rval
double InputNonNegateValue(double& rval, const char* name = NULL);

//���������� ���� �������� �������� � ����������� ���������� � bval [0-false, 1-true]
bool InputBooleanValue(bool& bval, const char* VarName = NULL, const char* TrueMsg = NULL,
                       const char* FalseMsg = NULL);

//���������� ���� ���������������� ������ �������� � ����������� ���������� � ival
ulong InputNonNegateValue(ulong& ival, const char* name = NULL);


#endif //���������� ���������� ������������� ����� - SF_CommonFunc.h