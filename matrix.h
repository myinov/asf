//matrix.h
//���������� ������� TComplexMatrix & TRealMatrix
//Copyright (c) JohnSoft 1999. All rights reserved.
//Initial date: February,13,1999; Final date:
//���� �������� ������, ������� ������������ �������� ������, ��������� � �� ��������-
//��� � �������� ��������, ������� ����� ����������� ��� ��������� � ������������ �
//��������������� �������. ������ ��������� ����������� ������� ������������ ������
//���� complex (16 ����) - �������� ����� ����� ���� double (8 ����) � ������ �����
//����� ���� double (8 ����). ������ ��������� �������������� ������� ����������� ���-
//��� ���� double. ���������� ��������, ������������� ��������� ������� ��������� �
//�������� �� 1.7x10-308 �� 1.7x10+308, ������������ ����� �������� ���� ���������� -
//19 ����.
//������������ ����������� ����������� ������ 4,294,967,296 ����� �� 4,294,967,296
//��������.
//� ������� ������� TComplexMatrix & TRealMatrix, ����� ������, ����� ������������
//����������� � �������������� ������-������� � ������-������.
//�� ������ ������ ������ � ������� ������� ������� ������ 0 (����).


#if !defined(__MATRIX_H)
#define __MATRIX_H

#include "const.h"
#pragma hdrstop

#pragma warn -inl      //IGNORE not expanded inline WARNINGS.

class TComplexMatrix;
typedef TComplexMatrix cmatrix;

class TRealMatrix;
typedef TRealMatrix rmatrix;

class TIntMatrix;
typedef TIntMatrix imatrix;

#define RUS 1 //������� ����
#define ENG 2 //���������� ����

//const bool LEFT  = false;
//const bool RIGHT = true;

#define SQUARE  true  //���������� �������
#define RECTANG false //������������� �������


#if !defined(__MVECTOR_H)
 #include "mvector.h"
#endif //__MVECTOR_H


#if !defined(__MATHLIB_H)
 #include <mathlib.h>
#endif //__MATHLIB.H


//***********************************************************************************
//���������� ��������� ������������ ���� ������� - MATRIX_NAME
//***********************************************************************************
typedef struct name
{ uint type;      //��� ������� (�������)
  char *rus_name; //������������� ������������ ������� (�������)
  char *eng_name; //������������ ������������ ������� (�������)
} MATRIX_NAME;

//***********************************************************************************
//���������� ��������� ������������� ������� - PF_MATRIX
//***********************************************************************************
typedef struct info
{ bool type; //��� ������� (���������� ��� �������������)
  uint subtype; //������ ������� (����������� ��� �������)
  ulong rows;//���������� ����� �������
  ulong cols;//���������� �������� �������
} PF_MATRIX;

typedef PF_MATRIX* pPF_MATRIX; //��������� �� ��������� PF_MATRIX

//������ ������, ����������� ��� ������ � ���������
const int M_SIZE_ERROR     = -1; //�������������� ����������� �������
const int M_TYPE_ERROR     = -2; //�������������� ���� �������
const int M_ZERO_ERROR     = -3; //��������� � ������� �������� �������
const int M_ACCESS_ERROR   = -4; //��������� � �������������� ��������� �������
const int M_DIVIDE_BY_ZERO = -5; //������� ������� �� ����
const int M_SIZE_MISMATCH  = -6; //�������������� ������������ 2-� ������
const int M_TYPE_MISMATCH  = -7; //�������������� ����� 2-� ������
const int M_UNKNOWN_TYPE   = -8; //����������� ��� �������
const int M_DEGENERACY     = -9; //����������� ������� (det A = 0)

//-----------------------------------------------------------------------------------
//        ���������� ��������, �������� � ������ TRealMatrix & TIntMatrix
//-----------------------------------------------------------------------------------
//������������� �������������� ������
//��������� �������� ����������� ���������� Type ������ rmatrix
const uint RM_ZEROSIZE    = 0; //������� �������� ������� - 0 x 0
const uint RM_ZERO        = 1;  //������� � �������� ����������

const uint RM_NUMBER      = 2; //������� �������� 1 x 1

const uint RV_ROW         = 3;//�������������� ������-������
const uint RV_COLUMN      = 4;//�������������� ������-�������

const uint RM_RECTANGULAR = 5; //������������� �������
const uint RM_RECTANG     = 5; //������������� �������
const uint RM_RECT        = 5; //������������� �������

const uint RM_SQUARE      = 6; //���������� �������
const uint RM_SQ          = 6; //���������� �������

//**************** �������� ���������� ������ ****************************
//1. ���������� ������������ �������
const uint RM_DIAGONAL    = 7; //������������ �������


//****************** �������� ������������ ������ ************************
//1.1 ��������� ������� (a[i][j] = c, ��� ���� i = j)
//1.2 ��������� ������� (a[i][j] = 1, ��� ���� i = j)
//1.3 ��������� ������� (a[i][j] = 0, ���� |i-j| < N, N - ����� ����������)
const uint RM_SCALAR      = 8; //��������� �������
const uint RM_UNIT        = 9; //��������� �������
const uint RM_BAND        = 10; //��������� ������� (N-������������, N > 1)

//2. ���������� ����������� �������
//2.1. ������� ����������� (a[i][j] = 0, ���� i > j)
//2.2. ������ �����������  (a[i][j] = 0, ���� i < j)
const uint RM_UPPERTRIANG = 11; //������� ����������� �������
const uint RM_UTRIANGULAR = 11; //������� ����������� �������
const uint RM_UTRIANG     = 11; //������� ����������� �������
const uint RM_U           = 11; //������� ����������� �������
const uint RM_LOWERTRIANG = 12; //������ ����������� �������
const uint RM_LTRIANGULAR = 12; //������ ����������� �������
const uint RM_LTRIANG     = 12; //������ ����������� �������
const uint RM_L           = 12; //������ ����������� �������

//3. ���������� �������������� � ������������������ �������
//3.1. �������������� ������� (a[i][j] = a[j][i])
//3.2. ������������������ ������� (a[i][j] = -a[j][i])
//3.3. ��������������� ������� (a[i][j] = -a[j][i], a[i][i] = 0)
const uint RM_SYMMETRICAL     = 13; //�������������� �������
const uint RM_SYMMETR         = 13; //�������������� �������
const uint RM_SKEWSYMMETRIC   = 14; //������������������ �������
const uint RM_SKEWSYMMETR     = 14; //������������������ �������
const uint RM_ALTERNATINGSIGN = 15; //��������������� �������
const uint RM_ALTSIGN         = 15; //��������������� �������


//***********************************************************************************
//          ���������� ������ - ������� �������������� ����� - TRealMatrix
//TRealMatrix ������� ������� � ���������� ���� double ����������� �����������,
//��������� �������� �������������� �������� ��� ������������� ��������� � �� ������-
//����, � ����� ������������ ��� ������ ��������� �������.
//***********************************************************************************
class TRealMatrix {
//���������� ������������� �������

//-----------------------------------------------------------------------------------
//���������� ���������� ������-������ ������
//-----------------------------------------------------------------------------------
protected:
 bool type;//��� ������������ ������� (���������� ��� �������������)
 ulong m;  //����� ����� �������/����� �������� ��� ���������� ������
 ulong n;  //����� �������� �������/����� ���������� ��� ��������� �������
 double **data; //��������� �� ��������� ������ �������� ��������� �������
//-----------------------------------------------------------------------------------
//I. ���������� ������������� ������-������� ������ TRealMatrix
//-----------------------------------------------------------------------------------
public:
 //**********************************************************************************
 //     ���������� ������������� � ����������� ������ TRealMatrix
 //**********************************************************************************
 TRealMatrix(); //���������� ������������ �� ���������
 //----------------------------------------------------------------------------------
 //���������� ������������� � �����������
 //----------------------------------------------------------------------------------
 //��������������� ������ �� ��������� ��������� ���� PF_MATRIX
 TRealMatrix(const PF_MATRIX& Info);
 //��������������� ������������� (m != n) ��� ���������� ������ (m == n)
 TRealMatrix(ulong row, ulong col, double value);
 TRealMatrix(ulong row, ulong col, bool init);
 //��������������� ���������� ������ (m == n)
 TRealMatrix(ulong size, double value);
 TRealMatrix(ulong size, bool init);
 TRealMatrix(ulong size, uint sq_type, ulong diag = 3L);
 //���������� ������������ �����������
 TRealMatrix(const TRealMatrix& R);
 //���������� ����������� ������
 ~TRealMatrix();
 //----------------------------------------------------------------------------------
 //���������� ������� ������ ���������� ������ TRealMatrix
 //----------------------------------------------------------------------------------
 //**********************************************************************************
 //                         ��������� ������� �������
 //**********************************************************************************
 //----------------------------------------------------------------------------------
 //1. ����������� ���� � ������� �������
 bool Type() const {return type;};
 bool GetType() const {return type;};
 //�������� ������� �� ����������
 bool IsSquare() const {return (type == SQUARE) ? true : false;};
 //�������� ������� �� �������������
 bool IsRectang() const {return (type == RECTANG) ? true : false;};
 bool IsZeroSize() const;       //�������� �� ������� �������� �������
 bool IsZero() const;           //�������� �� ������� � �������� ����������
 bool IsSymmetric() const;      //�������� ������� �� ��������������
 bool IsSkewSymmetric() const;  //�������� ������� �� ������������������
 bool IsAltSign() const;        //�������� ������� �� �����������������
 bool IsUpperTriang() const;    //�������� �� ������� ����������� �������
 bool IsLowerTriang() const;    //�������� �� ������ ����������� �������
 bool IsDiagonal() const;       //�������� �� ������������ �������
 bool IsScalar() const;         //�������� �� ��������� �������
 bool IsUnit() const;           //�������� �� ��������� �������
 bool IsBand(ulong& diags) const;//�������� �� ��������� �������
 uint SubType() const; //����������� ������� �������
 uint SubType(ulong& n) const; //����������� ������� �������
 //----------------------------------------------------------------------------------
 //2. ����������� ����������� �������
 //����������� ���������� ����� �������
 inline ulong GetRows() const {return m;};
 inline ulong Rows() const {return m;};
 //����������� ���������� �������� �������
 inline ulong GetCols() const {return (type == SQUARE) ? m : n;};
 inline ulong Cols() const {return (type == SQUARE) ? m : n;};
 //----------------------------------------------------------------------------------
 //3. ��������� ������� �������
 void Properties(ulong& rows, ulong& cols, bool& type) const
 {rows = GetRows(); cols = GetCols(); type = GetType();};
 void Properties(ulong& rows, ulong& cols, bool& type, uint& subtype) const;
 void Properties(PF_MATRIX& Info) const;
 string& Properties(string& s) const;//�������� ������� � ���������� ����
 //��������� ������������ ���� ������� (���������� ��� �������������)
 string& GetType(string& s, uint language = ENG) const;
 //��������� ������������ ������� �������
 string& GetSubType(string& s, uint language = ENG) const;
 //���������� ��������� ������� � ���������� ������
 string& GetData(string& s, uint precision = 6, string& delim = string(1,' ')) const;
 //����������� ������������ ���������� ����� ��������� ������� � ������������
 //������ ������� ������� �������.
 uint WidthCols(ivector& Width, uint Precision) const;

 //**********************************************************************************
 //                    ��������� ������� �������
 //**********************************************************************************
 bool Set(ulong rows, ulong cols, double value);
 bool Set(ulong rows, ulong cols, bool init);
 bool Set(ulong size, double value) {return Set(size, size, value);};
 bool Set(ulong size, bool init) {return Set(size, size, init);};
 bool Set(ulong size, uint sq_type, ulong diags = 3L);
 bool Set(const PF_MATRIX& Info);
 //**********************************************************************************
 // ������������� ��������� ������� ���������� ����������
 //**********************************************************************************
 bool Random(double a = 0.0, double b = 1.0);//�� ��������������� ������ [a..b]
 bool Rand2PI();//�� ��������������� ������ [0..2PI]
 bool Gauss(double E = 0.0, double D = 1.0);//�� ����������� ������
 bool Relay();//�� ������ �����
 bool Exp(double t = 1.0);//�� ����������������� ������

 //���������� �������������� (����) ��������� �������
 //void Edit(bool confirm = true, const char* name = "A");
 void Edit(const string& name, bool confirm = true);

 //**********************************************************************************
 // ������� ������� � ��������� �������
 //**********************************************************************************
 //��������� �������� �������� �������
 inline double Get(ulong row, ulong col) const {return data[row][col];};
 //���������� �������� �������� �������
 inline void Put(ulong row, ulong col, double value) {data[row][col] = value;};

 //��������� ������ ������� � �������� �������� �� 0 �� Rows()-1
 rvector Row(ulong index) const;
 bool Row(rvector& V, ulong index) const;
 //��������� ������� ������� � �������� �������� �� 0 �� Cols()-1
 rvector Col(ulong index) const;
 bool Col(rvector& V, ulong index) const;

 //������������ ���� ����� �������
 bool ExchangeRows(ulong i, ulong j);
 //������������ ����� ������� �������� �������-������������
 bool ExchangeRows(const ivector& permutation);
 //������������ ���� �������� �������
 bool ExchangeCols(ulong i, ulong j);
 //������������ �������� ������� �������� �������-������������
 bool ExchangeCols(const ivector& permutation);
 //----------------------------------------------------------------------------------
 //������������ ��������� ������� � �������� ��������� �������� (�����)
 //� �������� ������� �� �������� (�� �������)
 //----------------------------------------------------------------------------------
 rmatrix ReverseByCol(ulong left = 0L, ulong right = MAX_SIZE) const;
 rmatrix ReverseByRow(ulong left = 0L, ulong right = MAX_SIZE) const;
 bool ReverseCols();
 bool ReverseRows();
 //������� ��������� ������� �� 90 �������� �� ��� ������ ������� �������
 bool Rotate(rmatrix& A, bool clockwise = true) const;
 //----------------------------------------------------------------------------------
 //������������ ��������� �������� �������
 //----------------------------------------------------------------------------------
 //������������ ���������� ���������� A(0 1 ... k-1 k; 0 1 ... k-1 g), ��� g > k
 rmatrix U_SubMatrix(ulong k, ulong g) const;
 //������������ ���������� ���������� A(0 1 ... k-1 g; 0 1 ... k-1 k), ��� g > k
 rmatrix L_SubMatrix(ulong g, ulong k) const;
 //������������ ���������� ���������� A(0 1 ... k-1 k; 0 1 ... k-1 k)
 rmatrix P_SubMatrix(ulong k) const;

 //������������ ���������� �� �������� ������� �� �������� ��������, ��������� �
 //������� ivector
 rmatrix SubMatrixByCols(const ivector& IndexCols, bool Check = false) const;
 //������������ ���������� �� �������� ������� �� �������� �����, ��������� �
 //������� ivector
 rmatrix SubMatrixByRows(const ivector& IndexRows, bool Check = false) const;
 //������������ ���������� �� �������� ������� �� �������� ����� � ��������,
 //����������� � �������� IndexRows & IndexCols
 rmatrix SubMatrix(const ivector& IndexRows, const ivector& IndexCols,
                   bool Check = false) const;
 //----------------------------------------------------------------------------------
 // ������������ �������� ������� �������, �������-������, �������-�������
 //----------------------------------------------------------------------------------
 rmatrix AssignRow(const rvector& V, ulong row, ulong pos = 0L) const;
 rmatrix AssignRow(const rvector& V, ulong left, ulong right, ulong pos) const;
 rmatrix AssignRow(const rvector& V, const ivector& rows, ulong pos = 0L) const;

 rmatrix AssignCol(const rvector& V, ulong col, ulong pos = 0L) const;
 rmatrix AssignCol(const rvector& V, ulong left = 0L, ulong right = MAX_SIZE,
                   ulong pos = 0L) const;
 rmatrix AssignCol(const rvector& V, const ivector& cols, ulong pos = 0L) const;

 rmatrix Assign(const rmatrix& M, ulong row = 0L, ulong col = 0L) const;
 //----------------------------------------------------------------------------------
 //��������� ������� �� ������������ ������� � ������������ ������� ������������ ����
 //(elementary matrix)
 //������� ���������:
 //1. ������������ ������� ����� ���������� �� �������� ������� ������
 //2. ������������ ������� ����� ���������� �� �������� ������� �����
 //3. ������������ ������� ������ ����������
 //4. ����������� ������� � ������������ ������� ������ ���� ����� � ������� �� 0
 //������������ ������� - ������� E = I + B, ��� I - ��������� �������, B - �������
 //����� �������, �.�. ������� � ��������� ������� ���������� � ����� ���������������
 //��������� �������� �� ����.
 //       |1        |
 //       |  1      |
 // Ekl = |    1    |, E(k,l) != 0, k=4,l=2.
 //       |  x   1  |
 //       |        1|
 //----------------------------------------------------------------------------------
 //������������ ������� ������������ ����. � ��� ���������:
 //1. ������������ ������������  ������� Dk (DiagonalEM). ������������ ������� �
 //   ���������� Dk(i,i)=1 ��� ���� i != k � Dk(k,k)!=0.
 //      |1        |
 //      |  1      |
 // Dk = |    x    |, k = 3.
 //      |      1  |
 //      |        1|
 //2. ������ ���������� ������������ ������� Lkc (LowerColumnEM). ���������������
 //   ��������� �������� ����������� ������ � k-� ������� ��� ����������.
 //       |1        |
 //       |  1      |
 // Lkc = |    1    |, k = 3.
 //       |    x 1  |
 //       |    x   1|
 //3. ������� ���������� ������������ ������� Ukc (UpperColumnEM). ���������������
 //   ��������� �������� ����������� ������ � k-� ������� ��� ����������.
 //       |1   x    |
 //       |  1 x    |
 // Ukc = |    1    |, k = 3.
 //       |      1  |
 //       |        1|
 //4. ������ ���������� ������������ ������� Tkc (CompleteColumnEM). ���������������
 //   ��������� �������� ����������� ������ � k-� �������.
 //       |1   x    |
 //       |  1 x    |
 // Tkc = |    1    |, k = 3.
 //       |    x 1  |
 //       |    x   1|
 //5. ����� �������� ������������ ������� Lkr (LeftRowEM). ��������������� ���������
 //   �������� ����������� ������ � k-� ������ ����� ���������.
 //       |1        |
 //       |  1      |
 // Lkr = |x x 1    |, k = 3.
 //       |      1  |
 //       |        1|
 //6. ������ �������� ������������ ������� Ukr (RightRowEM). ���������������
 //   ��������� �������� ����������� ������ � k-� ������ ������ �� ���������.
 //       |1        |
 //       |  1      |
 // Ukr = |    1 x x|, k = 3.
 //       |      1  |
 //       |        1|
 //7. ������ �������� ������������ ������� Tkr (CompleteRowEM). ���������������
 //   ��������� �������� ����������� ������ � k-� ������.
 //       |1        |
 //       |  1      |
 // Tkr = |x x 1 x x|, k = 3.
 //       |      1  |
 //       |        1|
 //��� ��������� ������� �� ������������, ����������� ������������ ��� ������ ��������-
 //��� ������������ ������ ������������ � �������� ������������ ������������ �������-
 //��� �������. ���������� ������� ��������� ��������� ������ ������������ �����������
 //������. ������� ����� ����������� ���������� �������� ��������� ������� �� ���������
 //������������ ������, �������� ������� ����� ������� � ����� ���������� �������.
 //��������, ���������� ������� ����������� N, ��������� ������� ������������ N
 //DiagonalEM, N LowerColumnEM � N Upper-ColumnEM.
 //----------------------------------------------------------------------------------

 //��������� �� ������������ ������������ ������� Dk
 //k < Q.Rows(), Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool DiagonalEM(const rmatrix& Q, bool Disp, ulong k);
 //��������� �� ������������ ������������ ������� Dk, k=[first..last]
 //first < last < Q.Rows(), Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool DiagonalEM(const rmatrix& Q, bool Disp, ulong first, ulong last);
 //��������� �� ��� ������������ ������������ ������� Dk, k=[0..Q.Rows()-1]
 //Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool DiagonalEM(const rmatrix& Q, bool Disp);

 //��������� �� ������ ���������� ������������ ������� Lkc
 //k < Q.Rows(), Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool LowerColumnEM(const rmatrix& Q, bool Disp, ulong k);
 //��������� �� ������ ���������� ������������ ������� Lkc, k=[first..last]
 //first < last < Q.Rows(), Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool LowerColumnEM(const rmatrix& Q, bool Disp, ulong first, ulong last);
 //��������� �� ��� ������ ���������� ������������ ������� Lkc, k=[0..Q.Rows()-1]
 //Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool LowerColumnEM(const rmatrix& Q, bool Disp);

 //��������� �� ������� ���������� ������������ ������� Ukc
 //k < Q.Rows(), Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool UpperColumnEM(const rmatrix& Q, bool Disp, ulong k);
 //��������� �� ������� ���������� ������������ ������� Ukc, k=[first..last]
 //first < last < Q.Rows(), Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool UpperColumnEM(const rmatrix& Q, bool Disp, ulong first, ulong last);
 //��������� �� ��� ������� ���������� ������������ ������� Ukc, k=[0..Q.Rows()-1]
 //Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool UpperColumnEM(const rmatrix& Q, bool Disp);

 //��������� �� ������ ���������� ������������ ������� Tkc
 //k < Q.Rows(), Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool CompleteColumnEM(const rmatrix& Q, bool Disp, ulong k);
 //��������� �� ������ ���������� ������������ ������� Tkc, k=[first..last]
 //first < last < Q.Rows(), Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool CompleteColumnEM(const rmatrix& Q, bool Disp, ulong first, ulong last);
 //��������� �� ��� ������ ���������� ������������ ������� Tkc, k=[0..Q.Rows()-1]
 //Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool CompleteColumnEM(const rmatrix& Q, bool Disp);

 //��������� �� ����� �������� ������������ ������� Lkr
 //k < Q.Rows(), Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool LeftRowEM(const rmatrix& Q, bool Disp, ulong k);
 //��������� �� ����� �������� ������������ ������� Lkr, k=[first..last]
 //first < last < Q.Rows(), Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool LeftRowEM(const rmatrix& Q, bool Disp, ulong first, ulong last);
 //��������� �� ��� ����� �������� ������������ ������� Lkr, k=[0..Q.Rows()-1]
 //Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool LeftRowEM(const rmatrix& Q, bool Disp);

 //��������� �� ������ �������� ������������ ������� Ukr
 //k < Q.Rows(), Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool RightRowEM(const rmatrix& Q, bool Disp, ulong k);
 //��������� �� ������ �������� ������������ ������� Ukr, k=[first..last]
 //first < last < Q.Rows(), Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool RightRowEM(const rmatrix& Q, bool Disp, ulong first, ulong last);
 //��������� �� ��� ������ �������� ������������ ������� Ukr, k=[0..Q.Rows()-1]
 //Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool RightRowEM(const rmatrix& Q, bool Disp);

 //��������� �� ������ �������� ������������ ������� Tkr
 //k < Q.Rows(), Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool CompleteRowEM(const rmatrix& Q, bool Disp, ulong k);
 //��������� �� ������ �������� ������������ ������� Tkr, k=[first..last]
 //first < last < Q.Rows(), Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool CompleteRowEM(const rmatrix& Q, bool Disp, ulong first, ulong last);
 //��������� �� ��� ������ �������� ������������ ������� Tkr, k=[0..Q.Rows()-1]
 //Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool CompleteRowEM(const rmatrix& Q, bool Disp);

 //��������� �� ������������ ������������ ������� Bij
 //Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 //row < Rows() && col < Cols()
 bool OrdinaryEM(const rmatrix& Q, bool Disp, ulong row, ulong col);

//-----------------------------------------------------------------------------------
//              ���������� ������������� ����������
//-----------------------------------------------------------------------------------
 //1. OVERLOAD THE CALL FUNCTION OPERATOR
 //���������� �������������� ��������� ������ ������� ��� ������� � ����������
 //������� (���������� �������). ������������ � ���������� ������������.
 double& operator() (ulong i, ulong j);
 //���������� �������������� ��������� ������ ������� ��� ������� � ����������
 //������� (���������� �������). ��� ������ ���������� � ��������� ������������
 //� � ������������ �����������.
 //This version is called in ASSIGNMENT OPERATOR and COPY the CONSTRUCTOR
 const double& operator() (ulong i, ulong j) const;
 //2. OVERLOAD THE ASSIGNMENT OPERATOR
 //���������� �������������� ��������� ������������ operator=
 bool operator =(const TRealMatrix& R);
 //3. ���������� �������������� ��������� operator= �������������� �����������
 //   ������� � �������������� ������� (���������� ����������� �������)
 bool operator =(const TComplexMatrix& C);
 //4. ���������� �������������� ��������� operator= �������������� ������������� ���-
 //   ���� � �������������� �������
 bool operator =(const TIntMatrix& A);
 //5. OVERLOAD THE ASSIGNMENT OPERATOR
 //���������� �������������� ��������� ������������ operator = �����������
 //������������ ������� ��������������� ��������
 bool operator =(const double value);
 //6. OVERLOAD THE COMPARISON OPERATOR (==)
 //���������� �������������� ��������� ��������� ���� ������ operator ==
 bool operator ==(const TRealMatrix& A) const;
 //7. OVERLOAD THE STREAM INSERTION OPERATOR
 //���������� �������� ���������� ������ (<<) ��� ����������� ��������� �������
 friend ostream& operator <<(ostream& out, const TRealMatrix& R);

 //**********************************************************************************
 //             �������� �������� ������� ��� ��������������� ���������
 //   ��� �������� �������� ������� ������������ � ������ �������� �������.
 //**********************************************************************************
 //������������ ��������� ���������� ������� ������������ �������������� ���������
 bool Reverse();
 //�������� ���������������� ������������ �������
 rmatrix Transpose() const;
 bool Transpose(rmatrix& A) const;
 //���������� �������� ��������� (!) ��� ��������� ����������������� �������
 rmatrix operator !() const;
 //���������� ����� ���������� �������
 double Trace() const;
 double Spur() const {return Trace();};
 //����������� ������������� � ������������ �������� ��������� �������
 bool MaxMin(double& Max, double& Min) const;
 //----------------------------------------------------------------------------------
 //����������� ���� (���) ������������ �������
 double NFirstNorm() const;
 double FirstNorm() const;
 double SecondNorm() const;
 double ThirdNorm() const;
 double PNorm(uint p = 2) const;
 double Norm(uint type = EUCLIDEAN_NORM, uint p = 2) const;
 //���������� ����� ������ ������� � �������� �������� �� 0 �� Rows()-1
 double NormRow(ulong row, uint type = EUCLIDEAN_NORM, uint p = 2) const;
 //���������� ����� ������� ������� � �������� �������� �� 0 �� Cols()-1
 double NormCol(ulong col, uint type = EUCLIDEAN_NORM, uint p = 2) const;
 //���������� ���� ���� ����� �������
 bool NormAllRows(rvector& V, uint type = EUCLIDEAN_NORM, uint p = 2) const;
 //���������� ���� ���� �������� �������
 bool NormAllCols(rvector& V, uint type = EUCLIDEAN_NORM, uint p = 2) const;
 //----------------------------------------------------------------------------------
 //������������ ��������� �������
 bool Normalize(rmatrix& A, uint NormType, uint p = 0) const;
 bool Normalize(uint NormType, uint p = 0);
 bool NormalizeToDB(rmatrix& A, bool Power = true, double MindB = -60) const;
 bool NormalizeToDB(bool Power = true, double MindB = -60);
 //������������ ��������� ������� �� �������
 bool NormalizeRows(rmatrix& A, uint NormType, uint p = 0) const;
 bool NormalizeRows(uint NormType, uint p = 0);
 bool NormalizeRowsToDB(rmatrix& A, bool Power = true, double MindB = -60) const;
 bool NormalizeRowsToDB(bool Power = true, double MindB = -60);
 //������������ ��������� ������� �� ��������
 bool NormalizeCols(rmatrix& A, uint NormType, uint p = 0) const;
 bool NormalizeCols(uint NormType, uint p = 0);
 bool NormalizeColsToDB(rmatrix& A, bool Power = true, double MindB = -60) const;
 bool NormalizeColsToDB(bool Power = true, double MindB = -60);

 //���������� ������� �� �������������� ����� value
 //B = const + A, B(i,j) = B(i,j) + const
 friend rmatrix operator +(const double value, const rmatrix& A);
 //B = A + const, B(i,j) = A(i,j) + const
 friend rmatrix operator +(const rmatrix& A, const double value);

 //���������� ������� �� �������������� ����� value
 //B = A - const, B(i,j) = A(i,j) - const
 friend rmatrix operator -(const rmatrix& A, const double value);

 //��������� ������� �� �������������� ����� value
 //B = const * A, B(i,j) = A(i,j) * const
 friend rmatrix operator *(const double value, const rmatrix& A);
 //B = A * const, B(i,j) = A(i,j) * const
 friend rmatrix operator *(const rmatrix& A, const double value);

 //������� ������� �� �������������� ����� value
 //B = A / const, B(i,j) = A(i,j) / const
 rmatrix operator /(const double value) const;

 //�������� ���� ������������ ������
 //� = A + B, C(i,j) = A(i,j) + B(i,j);
 rmatrix operator +(const rmatrix& A) const;

 //��������� ���� ������������ ������
 //� = A - B, C(i,j) = A(i,j) - B(i,j);
 rmatrix operator -(const rmatrix& A) const;

 //��������� ���� ������������ ������
 rmatrix operator *(const rmatrix& A) const;

 //��������� ������� �� �� ����������������� ������� ������ B = A*trA
 bool MultByTransposedMatrixOnRight(rmatrix& A) const;

 //��������� ������� �� �� ����������������� ������� ����� B = trA*A
 bool MultByTransposedMatrixOnLeft(rmatrix& A) const;

 //���������� ������� � �������: A^2 = A*A � ������������ ����� �� ��������������
 //������ ��� ���������� ������
 bool Squaring(rmatrix& A, bool lSymmetrical = false) const;

 //������������ ��������� �������� (�����) ������������ ������� �� ������������
 //������ ����������� � �������� ������� �� ������� (��������)
 rmatrix MultByCols(const rvector& V) const;
 rmatrix MultByCols(const rvector& V, ulong index) const;
 rmatrix MultByCols(const rvector& V, ulong left, ulong right) const;
 rmatrix MultByCols(const rvector& V, const ivector& index, bool check = false) const;

 rmatrix MultByRows(const rvector& V) const;
 rmatrix MultByRows(const rvector& V, ulong index) const;
 rmatrix MultByRows(const rvector& V, ulong left, ulong right) const;
 rmatrix MultByRows(const rvector& V, const ivector& index, bool check = false) const;

 //�������� �������� (�����) ������������ ������� � ������������ ��������
 //����������� � �������� ������� �� ������� (��������)
 rmatrix AddByCols(const rvector& V) const;
 rmatrix AddByCols(const rvector& V, ulong index) const;
 rmatrix AddByCols(const rvector& V, ulong left, ulong right) const;
 rmatrix AddByCols(const rvector& V, const ivector& index, bool check = false) const;

 rmatrix AddByRows(const rvector& V) const;
 rmatrix AddByRows(const rvector& V, ulong index) const;
 rmatrix AddByRows(const rvector& V, ulong left, ulong right) const;
 rmatrix AddByRows(const rvector& V, const ivector& index, bool check = false) const;

 //������������ �������� (�����) ������������ �������
 rvector SumCols(ulong left = 0L, ulong right = MAX_SIZE) const;
 rvector SumCols(const ivector& index) const;

 rvector SumRows(ulong left = 0L, ulong right = MAX_SIZE) const;
 rvector SumRows(const ivector& index) const;

 //----------------------------------------------------------------------------------
 //������ ������� �������� �������
 //----------------------------------------------------------------------------------
 //������ ������ ���� M(0 1 ... k-1 k; 0 1 ... k-1 g), ��� g > k
 double U_Minor(ulong k, ulong g) const;
 //������ ������ ���� M(0 1 ... k-1 g; 0 1 ... k-1 k), ��� g > k
 double L_Minor(ulong g, ulong k) const;
 //������ ������ ���� M(0 1 ... k-1 k; 0 1 ... k-1 k) - ������� �����
 double P_Minor(ulong k) const;
 //������ ���� ������� ���� M(0 1 ... k-1 k; 0 1 ... k-1 g), ��� g > k
 // 0 < g < min(Cols(),Rows()), 0 <= k < g
 rmatrix U_AllMinors() const;
 //������ ������ ���� M(0 1 ... k-1 g; 0 1 ... k-1 k), ��� g > k
 // 0 < g < min(Cols(),Rows()), 0 <= k < g
 rmatrix L_AllMinors() const;
 //������ ������ ���� M(0 1 ... k-1 k; 0 1 ... k-1 k) - ������� �����
 // 0 <= k < min(Cols(),Rows())
 rmatrix P_AllMinors() const;
 rvector& P_AllMinors(rvector& P) const;
 //������ ���� ������� �������
 rmatrix AllMinors(bool L = true, bool P = true, bool U = true) const;

 //----------------------------------------------------------------------------------
 // ���������� �������������� ���������� ������ �� ������, ������� � ������������
 // �������: A = LDU
 // � ��������� ������������ ���������� ���� ������� �������� ������� ��� �������
 // ������������� ������ L, ������� U � ������������ D ������. � ��������������
 // ����� ������ ���� �������� ������ ������� ����������� ��� ���������� �������.
 //----------------------------------------------------------------------------------
 bool LDU_Gauss(rmatrix& LDU) const;
 bool LDU_Gauss(rmatrix& L, rmatrix& D, rmatrix& U) const;

 //----------------------------------------------------------------------------------
 //������ ������������� �������������� ���������� ������ � �� ���������� ��
 //������ � ������� ����������� �������: A = LU
 //----------------------------------------------------------------------------------
 //���������� ������������ ���� Ak*Bk � ������������ �� � ��������� ���������
 friend double InnerProduct(ulong l, ulong s, ulong u, double init,
                            const rmatrix& A, bool Avar, ulong Aconst,
                            const rmatrix& B, bool Bvar, ulong Bconst);

 //�������� ������ (����������� ���������� ����� ������ � ��������� �������
 //�������� �������� �� �������): �������, ���������������� ���������� �������
 bool LU_UnSymmetric(double& Det, rmatrix& LU, ivector& ExChange,
                     double* Base = NULL, long* Power = NULL) const;
 bool LU_UnSymmetric(rmatrix& LU, ivector& ExChange, double Eps = 1e-16) const;
 double DET_UnSymmetric(double* Base = NULL, long* Power = NULL) const;

 //----------------------------------------------------------------------------------
 // ������� �������������� ������ �������� ���������
 //----------------------------------------------------------------------------------
 //������� ������� ��������� AX = B, ��� A = LU, B - ������� ������ ������
 //������� A - ���������������� �������. ������� ���������� �� ���������� �����
 //������ (�������� ������).
 rmatrix LE_UnSymSolution(const rmatrix& B, const ivector& ExChange) const;
 friend rmatrix LE_UnSymSolution(const rmatrix& A, const rmatrix& B);

 //----------------------------------------------------------------------------------
 // ��������� ������
 //----------------------------------------------------------------------------------
 //��������� ���������������� ���������� ������� n x n ������� ���������� �� �� �����������
 //������� L & U c ������� ��������� ������ � ������� ������� �������� ��������� � ������-
 //��� �������� ����������� n x n � ������ ����� �������
 rmatrix INV_Krauth(double Eps = 1e-16) const;
 bool INV_Krauth(rmatrix& invA, double Eps = 1e-16) const;
 //----------------------------------------------------------------------------------
 //������ ����������� �������� � ����������� �������� ��������������
 //���������� ������
 //----------------------------------------------------------------------------------
 //�������� �����: �������, �������������� �������
 uint EVV_Jacobi(TRealVector& D, TRealMatrix& V, double EPS = 1e-19) const;
 int EV_Jacobi(TRealVector& D, double EPS = 1e-19, bool lSort = true) const;
//-----------------------------------------------------------------------------------
//    ����������� ���������� ������������ ������������ �������������� ������
//                     (���������� �� ����� ���������)
// ���� A - ��������������  ������������ ������������ �������, �� ���������� �������-
// ������� ������������� ������ ����������� ������� L, ����� ���   L*tr(L) = A.  (1)
// ����� ����, ���� ������������ �������� ������� L ������� ��������������, �� �����-
// ����� �����������. �������� ������� L ����� ���������� �� ������� ��� �� ��������,
// ����������� ��������������� �������� ������ � ��������� (1). ���� ������� ���������
// �� �������, �� ��� ��������� i-�� ������ ����������� ��������� �����������:
//  j
// SUM L[ik]*L[jk] = A[ij] ���                        (2)
// k=1
//                  j-1
//          A[ij] - SUM L[ik]*L[jk]
//                  k=1
// L[ij] = ------------------------ , j = 1,...,i-1;  (2)
//                  L[jj]
//
//  i
// SUM L[ik]*L[ik] = A[ii], ���                       (3)
// k=1
//
//                   i-1          ^1/2
// L[ii] = { A[ii] - SUM L[ik]^2 }                    (3)
//                   k=1
// ��� ���������� ������ ���������� N �������� ���������� ����������� ����� � ������-
// �������� (N^3)/6 ���������, N - ����������� ���������� �������.
//-----------------------------------------------------------------------------------
//���������� �� ����� ��������� ���� A = LU, U = tr(L) � ����������� ������������
 bool DET_Cholesky(rmatrix& LU, double& Det) const;

//������� ������� ��������� AX = B, ��� A = LU, B - ������� ������ ������
//������� A - �������������� ������� ������������ ������������ �������. �������
//���������� �� ����� ���������.
 friend bool LE_Cholesky(const rmatrix& A, const rmatrix& B, rmatrix& X);

//��������� �������������� ������������ ������������ �������
 bool INV_Cholesky(rmatrix& invA) const;

//-----------------------------------------------------------------------------------
//         ���������� ������������� ������� ������ TRealMatrix
//-----------------------------------------------------------------------------------
//�������� �� ��������� ������������ ������ A � B
 friend bool IsEqualSize(const rmatrix& A, const rmatrix& B)
 {return ((A.Rows() == B.Rows()) && (A.Cols() == B.Cols())) ? true : false;};
//����������� ������������ ���������� ����� ��������� ������� � ������������
//������ ������� ������� �������.
 friend uint* MaxStrLen(const TRealMatrix& R, uint precision);
//-----------------------------------------------------------------------------------
//���������� �������������� �������
//-----------------------------------------------------------------------------------
 friend bool CorrelationMatrix(const rvector& V, rmatrix& R);
//-----------------------------------------------------------------------------------
//�������������� ������� � ������ � ������� � �������
//-----------------------------------------------------------------------------------
 friend bool MatrixToVectorByCols(const rmatrix& A, rvector& V);
 friend bool MatrixToVectorByRows(const rmatrix& A, rvector& V);
 friend bool VectorToMatrixByCols(const rvector& V, rmatrix& A, ulong rows, ulong cols);
 friend bool VectorToMatrixByRows(const rvector& V, rmatrix& A, ulong rows, ulong cols);
//-----------------------------------------------------------------------------------
//���������� �������� ��������� ��������
//1) ��������� ������� �� ������� C = A*B;
//2) ��������� ������� �� �����   B = A*c || c*A;
//3) �������� ���� ������ C = A+B;
//
//������ ������� ������� ������������ ��� ������ � ��������� ������� ��������.
//�������� �� ���������� ���� � ���������� � ������� ������ ������, ��� � �����������
//�������� � ����������� ����������. � �������� � ����������� ���������� ����������
//���������� ���������� �� ��������, ��� �������� � ���������� ����� �������� �������
//� � ������� �������� ������ �� �������� ���������� ���������� (������, ��������)
//������ ��������.
//-----------------------------------------------------------------------------------
//��������� ������ C = A*B (����� �������� ������������ ���� ������)
friend bool Multiply(rmatrix& C, const rmatrix& A, const rmatrix& B);
//��������� ���� A = A * B
friend bool Multiply(rmatrix& A, const rmatrix& B);
//��������� ���� D = A * B * C
friend bool Multiply(rmatrix& D, const rmatrix& A, const rmatrix& B, const rmatrix& C);
//��������� ��������� ������� A �� ��������� c: B = A*c = c*A
friend bool Multiply(rmatrix& B, const rmatrix& A, double c);
//��������� ��������� ������� A �� ��������� c: A = A*c = c*A
friend bool Multiply(rmatrix& A, double c);
//�������� ���� ������ C = A+B = B+A
friend bool Add(rmatrix& C, const rmatrix& A, const rmatrix& B);
//�������� ���� ������ A = A+B
friend bool Add(rmatrix& A, const rmatrix& B);
//���������� ������� �������� ������� A �� �������� value
friend bool Add(rmatrix& A, double value);
//��������� ������� A �� ������ X: Y = A*X
friend bool Multiply(rvector& Y, const rmatrix& A, const rvector& X);
//�������������� ����� B = H*A*trH
friend bool QuadraticForm(rmatrix& B, const rmatrix& H, const rmatrix& A);
//�������������� ����� ���� Q(m,m) = H(m,n)*Rxx(n,n)*trH(n,m), Rxx = X*trX
//X - ������-�������, trX - ������-������, H - �������, trH - ����������������� H
friend bool QuadraticForm(rmatrix& Q, const rmatrix& H, const rvector& X);
//������ ��������� �������������� ����� ����������� ������� X
friend bool DirectFourierTransform(rmatrix& FT_Y, const rmatrix& X, long hX0 = 0L,
            long wX0 = 0L, ulong VCount = 0L, ulong HCount = 0L);
//���������������� ���� ��������� ������� A �� ��������� 10
friend bool Log10(rmatrix& A);
friend bool Log10(rmatrix& A, double Min);
//���������������� ���� ��������� ������� A �� ��������� 10 � ����������� ����������
//�� ��������� factor � ������������ �� �������� �� �������� Min. ��� �������� �������
//������ ��� ������� ���� ��������������� �������� Min.
friend long Log10(rmatrix& B, const rmatrix& A, double factor = 10, double Min = -60);
//���������� ���� �������� ��������� ������� A �� �������� r_val � �������� eps
friend ulong Round(rmatrix& A, const double r_val, const double eps = 1e-10);
//���������� ���� ��������� ������� A � �������
friend bool Square(rmatrix& A);
//���������� ���� ��������� ������� A � ������� � ���������� �� ������� B
friend bool Square(rmatrix& B, const rmatrix& A);
//���������� ����������� ����� �� ���� ��������� ������� A
friend bool Sqrt(rmatrix& A);
//���������� ���������� ������� ���� ��������� ������� A
friend bool Abs(rmatrix& A);
//���������� ������������ ��������� ������������ ���� ������ ����:
// diag(Q(n,n)) = trH(n,m)*Rxx(m,m)*H(m,n),
//H - ������� � trH - �������, ����������������� � H, Rxx - ���������� �������
friend bool CalcDiagElems(rvector& V, const rmatrix& H, const rmatrix& R, bool sq_root);
//���������� �������� ���������� ��������� ������� B � ����������� ������������� � �����-
//��� A: A = A + B/n
friend bool Average(rmatrix& A, const rmatrix& B, ulong n);
//���������� �������� ������������ ���������� ��� ������� ������ ����� ����������
//�� ������ k-�� ���� ���������� �������� ������� AvX ����������� �� k ����� (k >= 0)
friend bool InfiniteAverage(rmatrix& AvX, const rmatrix& Xk, ulong k);

//��������� �������������� ������ ������� A � ������������� ��������� ������� �� ��������
//� ������ sTbl ���� string
friend string& MakeTable(string& sTbl, const UStringMap& HeadColl, const rmatrix& A,
       uint precision = 6, string& delim = string(1,' '));

//-----------------------------------------------------------------------------------
//II. ���������� �������� ������-������� ������ TRealMatrix
//-----------------------------------------------------------------------------------
private:
 void Initial();//������������� ������-������ ������ ��� ��� ���������������
 //�������� ������� ������������� ������� � ����������� ������ ��� �������������
 bool Create();
 void Delete();//�������� ��������� ������� �� ����������� ������

//------------------------------------------------------------------------------
//III. ���������� ���������� ������-������� ������ TRealMatrix
//------------------------------------------------------------------------------
protected:
 //*****************************************************************************
 //���������� ������� ����������� ����� ��������� ������ ���������� ����
 //*****************************************************************************
 //���� ��������� ������������� � ���������� ������, �������-������,
 //�������-������� �������������� �����
 void InPutRealMatrix(const string& name, bool confirm = true);
 //���� ��������� ������������ �������
 void InPutDiagonal(const string& name, bool confirm = true);
 //���� ��������� ��������� �������
 void InPutScalar(const string& name, bool confirm = true);
 //���� ��������� ��������� �������
 void InPutBand(ulong diags, const string& name, bool confirm = true);
 //���� ��������� ������� ����������� �������
 void InPutUTriang(const string& name, bool confirm = true);
 //���� ��������� ������ ����������� �������
 void InPutLTriang(const string& name, bool confirm = true);
 //���� ��������� �������������� �������
 void InPutSymmetric(const string& name, bool confirm = true);
 //���� ��������� ������������������ �������
 void InPutSkewSymmetric(const string& name, bool confirm = true);
 //���� ��������� ��������������� �������
 void InPutAltSign(const string& name, bool confirm = true);
 //**********************************************************************************
 //���������� ������� ������������� ��������� ������ ���������� ����
 //**********************************************************************************
 void InitZeroMatrix();
 void InitMatrix();
 void InitDiagonal();
 void InitScalar();
 void InitUnit();
 void InitBand(ulong diags);
 void InitUTriang();
 void InitLTriang();
 void InitSymmetric();
 void InitSkewSymmetric();
 void InitAltSign();
};
/****** ����� ���������� ������ - �������������� ������� - class TRealMatrix *******/

//-----------------------------------------------------------------------------------
//          ���������� ��������, �������� � ������ TComplexMatrix
//-----------------------------------------------------------------------------------
//������������� ����������� ������
//��������� �������� ����������� type - ������-������ ������ cmatrix
const uint CM_ZEROSIZE    = 0;//����������� ������� ������� �����������
const uint CM_ZERO        = 1;//����������� ������� � �������� ����������
const uint CM_NUMBER      = 2;//����������� ������� �������� 1 x 1
const uint CV_ROW         = 3;//����������� ������-������
const uint CV_COLUMN      = 4;//����������� ������-�������
const uint CM_RECTANG     = 5;//������������� ����������� �������
const uint CM_RECTANGULAR = 5;//������������� ����������� �������
const uint CM_RECT        = 5;//������������� ����������� �������

//���������� ����������� �������
const uint CM_SQUARE = 6;//���������� �������
const uint CM_SQ     = 6;//���������� �������
/******************** �������� ���������� ������ ******************************/
//1. ���������� ������������ �������
//������������ �������: a[i][j] = 0, ��� ���� i != j
const uint CM_DIAGONAL = 7;//������������ �������

/********************* �������� ������������ ������ ***************************/
//1.1 ��������� ������� (a[i][j] = c, ��� ���� i = j)
//1.2 ��������� ������� (a[i][j] = 1, ��� ���� i = j)
//1.3 ��������� ������� (a[i][j] = 0, ���� |i-j| < N, N - ����� ����������)
const uint CM_SCALAR = 8;//��������� �������
const uint CM_UNIT   = 9;//��������� �������
const uint CM_BAND   = 10;//��������� ������� (N-������������, N > 1)

//2. ���������� ����������� �������
//2.1. ������� ����������� (a[i][j] = 0, ���� i > j)
//2.2. ������ �����������  (a[i][j] = 0, ���� i < j)
const uint CM_UPPERTRIANG = 11;//������� ����������� �������
const uint CM_UTRIANGULAR = 11;//������� ����������� �������
const uint CM_UTRIANG     = 11;//������� ����������� �������
const uint CM_U           = 11;//������� ����������� �������
const uint CM_LOWERTRIANG = 12;//������ ����������� �������
const uint CM_LTRIANGULAR = 12;//������ ����������� �������
const uint CM_LTRIANG     = 12;//������ ����������� �������
const uint CM_L           = 12;//������ ����������� �������

//3. ���������� �������������� � ������������������ �������
//3.1. �������������� ������� (a[i][j] = a[j][i])
//3.2. ������������������ ������� (a[i][j] = -a[j][i])
//3.3. ��������������� ������� (a[i][j] = -a[j][i], a[i][i] = 0)
const uint CM_SYMMETRICAL     = 13;//�������������� �������
const uint CM_SYMMETR         = 13;//�������������� �������
const uint CM_SKEWSYMMETRIC   = 14;//������������������ �������
const uint CM_SKEWSYMMETR     = 14;//������������������ �������
const uint CM_SKEWSYM         = 14;//������������������ �������
const uint CM_ALTERNATINGSIGN = 15;//��������������� �������
const uint CM_ALTSIGN         = 15;//��������������� �������

//4. �������� �������
//�������� �������: a[i][i] = real_number; a[i][j] = conj(a[j][i]).
const uint CM_HERMITIAN = 16;//��������� �������

//******************************************************************************
//���������� ������ - ������� ����������� ����� - TComplexMatrix
//TComplexMatrix ������������ �������� ����������� �������, �������� �� ���������,
//���������� �������� �������������� �������� ��� ������������ ��������� � ��
//����������, � ����� ������������ ��� ������ ��������� �������.
//******************************************************************************
class TComplexMatrix {
//���������� ������������� �������

//------------------------------------------------------------------------------
//���������� ���������� ������-������ ������
//------------------------------------------------------------------------------
protected:
 bool type;   //��� ����������� ������� (���������� ��� �������������)
 ulong m;     //����� ����� �������
 ulong n;     //����� �������� �������
 complex **data;//��������� �� ��������� ������ �������� ��������� �������
//------------------------------------------------------------------------------
//I. ���������� ������������� ������-������� ������ TComplexMatrix
//------------------------------------------------------------------------------
public:
 //-----------------------------------------------------------------------------
 //    ���������� ������������� � ����������� ������ TComplexMatrix
 //-----------------------------------------------------------------------------
 //���������� ������������ �� ���������
 TComplexMatrix();
 //���������� ������������� � �����������
 //��������������� ������������� (m != n) ��� ���������� (m == n) ������
 TComplexMatrix(ulong row, ulong col, complex value);
 TComplexMatrix(ulong row, ulong col, bool init);
 TComplexMatrix(const PF_MATRIX& Info);
 //��������������� ���������� ������
 TComplexMatrix(ulong size, complex value);
 TComplexMatrix(ulong size, bool init);
 TComplexMatrix(ulong size, uint sq_type, ulong diag = 3);
 //���������� ������������ �����������
 TComplexMatrix(const TComplexMatrix& A);
 //���������� ����������� ������
 ~TComplexMatrix();
 //-----------------------------------------------------------------------------
 // ��������� ������� �������
 //-----------------------------------------------------------------------------
 //1. ����������� ���� � ������� �������
 bool Type() const {return type;};
 bool GetType() const {return type;};
 bool IsSquare() const          //�������� ������� �� ����������
 {return (type == SQUARE) ? true : false;};
 bool IsRectang() const         //�������� ������� �� �������������
 {return (type == RECTANG) ? true : false;};
 bool IsZeroSize() const;       //�������� �� ������� �������� �������
 bool IsZero() const;           //�������� �� ������� � �������� ����������
 bool IsSymmetric() const;      //�������� ������� �� ��������������
 bool IsSkewSymmetric() const;  //�������� ������� �� ������������������
 bool IsAltSign() const;        //�������� ������� �� �����������������
 bool IsUpperTriang() const;    //�������� �� ������� ����������� �������
 bool IsLowerTriang() const;    //�������� �� ������ ����������� �������
 bool IsDiagonal() const;       //�������� �� ������������ �������
 bool IsScalar() const;         //�������� �� ��������� �������
 bool IsUnit() const;           //�������� �� ��������� �������
 bool IsBand(ulong& diags) const;//�������� �� ��������� �������
 bool IsHermitian() const;//�������� ������� �� ���������

 uint SubType() const; //����������� ������� �������
 uint SubType(ulong& n) const; //����������� ������� �������
 //-----------------------------------------------------------------------------
 //2. ����������� ����������� �������
 //����������� ���������� ����� �������
 ulong GetRows() const {return m;};
 ulong Rows() const {return m;};
 //����������� ���������� �������� �������
 ulong GetCols() const {return (type == SQUARE) ? m : n;};
 ulong Cols() const {return (type == SQUARE) ? m : n;};
 //-----------------------------------------------------------------------------
 //3. ��������� ������� �������
 void Properties(ulong& rows, ulong& cols, bool& type) const
 {rows = GetRows(); cols = GetCols(); type = GetType();};
 void Properties(ulong& rows, ulong& cols, bool& type, uint& subtype) const;
 void Properties(PF_MATRIX& Info) const;
 string& Properties(string& s) const;//�������� ������� � ���������� ����
 //��������� ������������ ���� ������� (���������� ��� �������������)
 string& GetType(string& s, uint language = ENG) const;
 //��������� ������������ ������� �������
 string& GetSubType(string& s, uint language = ENG) const;
 //*****************************************************************************
 //                    ��������� ������� �������
 //*****************************************************************************
 bool Set(ulong rows, ulong cols, complex value);
 bool Set(ulong rows, ulong cols, bool init);
 bool Set(ulong size, complex value) {return Set(size, size, value);};
 bool Set(ulong size, bool init) {return Set(size, size, init);};
 bool Set(ulong size, uint sq_type, ulong diags = 3L);
 bool Set(const PF_MATRIX& Info);
 //*****************************************************************************
 // ������������� ��������� ������� ���������� ����������
 //*****************************************************************************
 bool Random(double a = 0.0, double b = 1.0);//�� ��������������� ������ [a..b]
 bool Rand2PI();//�� ��������������� ������ [0..2PI]
 bool Gauss(double E = 0.0, double D = 1.0);//�� ����������� ������
 bool Relay();//�� ������ �����
 bool Exp(double t = 1.0);//�� ����������������� ������

 //���������� �������������� (����) ��������� �������
 void Edit(const string& name, bool confirm = true);

 //*****************************************************************************
 // ������� ������� � ��������� �������
 //*****************************************************************************
 //��������� �������� �������� �������
 inline complex Get(ulong row, ulong col) const {return data[row][col];};
 //���������� �������� �������� �������
 inline void Put(ulong row, ulong col, complex value) {data[row][col] = value;};
 inline void Put(ulong row, ulong col, double value) {data[row][col] = complex(value,0);};

 //��������� ������ ������� � �������� �������� �� 0 �� Rows()-1
 cvector Row(ulong index) const;
 bool Row(cvector& V, ulong index) const;
 //��������� ������� ������� � �������� �������� �� 0 �� Cols()-1
 cvector Col(ulong index) const;
 bool Col(cvector& V, ulong index) const;

 //������������ ���� ����� �������
 bool ExchangeRows(ulong i, ulong j);
 //������������ ����� ������� �������� �������-������������
 bool ExchangeRows(const ivector& permutation);
 //������������ ���� �������� �������
 bool ExchangeCols(ulong i, ulong j);
 //������������ �������� ������� �������� �������-������������
 bool ExchangeCols(const ivector& permutation);
 //-----------------------------------------------------------------------------
 //������������ ��������� ������� � �������� ��������� �������� (�����)
 //� �������� ������� �� �������� (�� �������)
 //-----------------------------------------------------------------------------
 cmatrix ReverseByCol(ulong left = 0L, ulong right = MAX_SIZE) const;
 cmatrix ReverseByRow(ulong left = 0L, ulong right = MAX_SIZE) const;
 bool ReverseCols();
 bool ReverseRows();
 //-----------------------------------------------------------------------------
 //������������ ��������� �������� �������
 //-----------------------------------------------------------------------------
 //������������ ���������� ���������� A(0 1 ... k-1 k; 0 1 ... k-1 g), ��� g > k
 cmatrix U_SubMatrix(ulong k, ulong g) const;
 //������������ ���������� ���������� A(0 1 ... k-1 g; 0 1 ... k-1 k), ��� g > k
 cmatrix L_SubMatrix(ulong g, ulong k) const;
 //������������ ���������� ���������� A(0 1 ... k-1 k; 0 1 ... k-1 k)
 cmatrix P_SubMatrix(ulong k) const;

 //������������ ���������� �� �������� ������� �� �������� ��������, ��������� �
 //������� ivector
 cmatrix SubMatrixByCols(const ivector& IndexCols, bool Check = false) const;
 //������������ ���������� �� �������� ������� �� �������� �����, ��������� �
 //������� ivector
 cmatrix SubMatrixByRows(const ivector& IndexRows, bool Check = false) const;
 //������������ ���������� �� �������� ������� �� �������� ����� � ��������,
 //����������� � �������� IndexRows & IndexCols
 cmatrix SubMatrix(const ivector& IndexRows, const ivector& IndexCols,
                   bool Check = false) const;
 //-----------------------------------------------------------------------------
 // ������������ �������� ������� �������, �������-������, �������-�������
 //-----------------------------------------------------------------------------
 cmatrix AssignRow(const cvector& V, ulong row, ulong pos = 0L) const;
 cmatrix AssignRow(const cvector& V, ulong left, ulong right, ulong pos) const;
 cmatrix AssignRow(const cvector& V, const ivector& rows, ulong pos = 0L) const;

 cmatrix AssignCol(const cvector& V, ulong col, ulong pos = 0L) const;
 cmatrix AssignCol(const cvector& V, ulong left = 0L, ulong right = MAX_SIZE,
                   ulong pos = 0L) const;
 cmatrix AssignCol(const cvector& V, const ivector& cols, ulong pos = 0L) const;

 cmatrix Assign(const cmatrix& M, ulong row = 0L, ulong col = 0L) const;

 //-----------------------------------------------------------------------------
 //��������� ������� �� ������������ ������� � ������������ ������� ������������
 //                              ���� (elementary matrix)
 //������� ���������:
 //1. ������������ ������� ����� ���������� �� �������� ������� ������
 //2. ������������ ������� ����� ���������� �� �������� ������� �����
 //3. ������������ ������� ������ ����������
 //4. ����������� ������� � ������������ ������� ������ ���� ����� � ������� �� 0
 //������������ ������� - ������� E = I + B, ��� I - ��������� �������, B - ���-
 //���� ����� �������, �.�. ������� � ��������� ������� ���������� � ����� ���-
 //������������ ��������� �������� �� ����.
 //       |1        |
 //       |  1      |
 // Ekl = |    1    |, E(k,l) != 0, k=4,l=2.
 //       |  x   1  |
 //       |        1|
 //-----------------------------------------------------------------------------
 //������������ ������� ������������ ����. � ��� ���������:
 //1. ������������ ������������  ������� Dk (DiagonalEM). ������������ ������� �
 //   ���������� Dk(i,i)=1 ��� ���� i != k � Dk(k,k)!=0.
 //      |1        |
 //      |  1      |
 // Dk = |    x    |, k = 3.
 //      |      1  |
 //      |        1|
 //2. ������ ���������� ������������ ������� Lkc (LowerColumnEM).  ������������-
 //   ��� ��������� �������� ����������� ������ � k-� ������� ��� ����������.
 //       |1        |
 //       |  1      |
 // Lkc = |    1    |, k = 3.
 //       |    x 1  |
 //       |    x   1|
 //3. ������� ���������� ������������ ������� Ukc (UpperColumnEM). ������������-
 //   ��� ��������� �������� ����������� ������ � k-� ������� ��� ����������.
 //       |1   x    |
 //       |  1 x    |
 // Ukc = |    1    |, k = 3.
 //       |      1  |
 //       |        1|
 //4. ������ ���������� ������������ ������� Tkc (CompleteColumnEM). ������������-
 //   ��� ��������� �������� ����������� ������ � k-� �������.
 //       |1   x    |
 //       |  1 x    |
 // Tkc = |    1    |, k = 3.
 //       |    x 1  |
 //       |    x   1|
 //5. ����� �������� ������������ ������� Lkr (LeftRowEM). ��������������� ����-
 //   ����� �������� ����������� ������ � k-� ������ ����� ���������.
 //       |1        |
 //       |  1      |
 // Lkr = |x x 1    |, k = 3.
 //       |      1  |
 //       |        1|
 //6. ������ �������� ������������ ������� Ukr (RightRowEM). ���������������
 //   ��������� �������� ����������� ������ � k-� ������ ������ �� ���������.
 //       |1        |
 //       |  1      |
 // Ukr = |    1 x x|, k = 3.
 //       |      1  |
 //       |        1|
 //7. ������ �������� ������������ ������� Tkr (CompleteRowEM). ���������������
 //   ��������� �������� ����������� ������ � k-� ������.
 //       |1        |
 //       |  1      |
 // Tkr = |x x 1 x x|, k = 3.
 //       |      1  |
 //       |        1|
 //���  ���������  ������� ��  ������������, ����������� ������������ ��� ������
 //�����������  ������������ ������ ������������ � �������� ������������ ������-
 //������ ���������� �������. ���������� ������� ��������� ��������� ������ ���-
 //��������� ����������� ������. �������  �����  ����������� ���������� ��������
 //���������  �������  �� ���������  ������������ ������, �������� ������� �����
 //������� � ����� ����������  �������. ��������, ���������� ������� �����������
 //N, ���������  �������  ������������  N DiagonalEM, N LowerColumnEM � N Upper-
 //ColumnEM.
 //-----------------------------------------------------------------------------

 //��������� �� ������������ ������������ ������� Dk
 //k < Q.Rows(), Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool DiagonalEM(const cmatrix& Q, bool Disp, ulong k);
 //��������� �� ������������ ������������ ������� Dk, k=[first..last]
 //first < last < Q.Rows(), Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool DiagonalEM(const cmatrix& Q, bool Disp, ulong first, ulong last);
 //��������� �� ��� ������������ ������������ ������� Dk, k=[0..Q.Rows()-1]
 //Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool DiagonalEM(const cmatrix& Q, bool Disp);

 //��������� �� ������ ���������� ������������ ������� Lkc
 //k < Q.Rows(), Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool LowerColumnEM(const cmatrix& Q, bool Disp, ulong k);
 //��������� �� ������ ���������� ������������ ������� Lkc, k=[first..last]
 //first < last < Q.Rows(), Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool LowerColumnEM(const cmatrix& Q, bool Disp, ulong first, ulong last);
 //��������� �� ��� ������ ���������� ������������ ������� Lkc, k=[0..Q.Rows()-1]
 //Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool LowerColumnEM(const cmatrix& Q, bool Disp);

 //��������� �� ������� ���������� ������������ ������� Ukc
 //k < Q.Rows(), Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool UpperColumnEM(const cmatrix& Q, bool Disp, ulong k);
 //��������� �� ������� ���������� ������������ ������� Ukc, k=[first..last]
 //first < last < Q.Rows(), Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool UpperColumnEM(const cmatrix& Q, bool Disp, ulong first, ulong last);
 //��������� �� ��� ������� ���������� ������������ ������� Ukc, k=[0..Q.Rows()-1]
 //Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool UpperColumnEM(const cmatrix& Q, bool Disp);

 //��������� �� ������ ���������� ������������ ������� Tkc
 //k < Q.Rows(), Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool CompleteColumnEM(const cmatrix& Q, bool Disp, ulong k);
 //��������� �� ������ ���������� ������������ ������� Tkc, k=[first..last]
 //first < last < Q.Rows(), Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool CompleteColumnEM(const cmatrix& Q, bool Disp, ulong first, ulong last);
 //��������� �� ��� ������ ���������� ������������ ������� Tkc, k=[0..Q.Rows()-1]
 //Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool CompleteColumnEM(const cmatrix& Q, bool Disp);

 //��������� �� ����� �������� ������������ ������� Lkr
 //k < Q.Rows(), Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool LeftRowEM(const cmatrix& Q, bool Disp, ulong k);
 //��������� �� ����� �������� ������������ ������� Lkr, k=[first..last]
 //first < last < Q.Rows(), Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool LeftRowEM(const cmatrix& Q, bool Disp, ulong first, ulong last);
 //��������� �� ��� ����� �������� ������������ ������� Lkr, k=[0..Q.Rows()-1]
 //Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool LeftRowEM(const cmatrix& Q, bool Disp);

 //��������� �� ������ �������� ������������ ������� Ukr
 //k < Q.Rows(), Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool RightRowEM(const cmatrix& Q, bool Disp, ulong k);
 //��������� �� ������ �������� ������������ ������� Ukr, k=[first..last]
 //first < last < Q.Rows(), Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool RightRowEM(const cmatrix& Q, bool Disp, ulong first, ulong last);
 //��������� �� ��� ������ �������� ������������ ������� Ukr, k=[0..Q.Rows()-1]
 //Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool RightRowEM(const cmatrix& Q, bool Disp);

 //��������� �� ������ �������� ������������ ������� Tkr
 //k < Q.Rows(), Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool CompleteRowEM(const cmatrix& Q, bool Disp, ulong k);
 //��������� �� ������ �������� ������������ ������� Tkr, k=[first..last]
 //first < last < Q.Rows(), Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool CompleteRowEM(const cmatrix& Q, bool Disp, ulong first, ulong last);
 //��������� �� ��� ������ �������� ������������ ������� Tkr, k=[0..Q.Rows()-1]
 //Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 bool CompleteRowEM(const cmatrix& Q, bool Disp);

 //��������� �� ������������ ������������ ������� Bij
 //Q.Rows()==Q.Cols()==Cols()==Rows() != 0
 //row < Rows() && col < Cols()
 bool OrdinaryEM(const cmatrix& Q, bool Disp, ulong row, ulong col);

//------------------------------------------------------------------------------
//              ���������� ������������� ����������
//------------------------------------------------------------------------------
 //1. OVERLOAD THE CALL FUNCTION OPERATOR
 //���������� �������������� ��������� ������ ������� ��� ������� � ����������
 //������� (���������� �������). ������������ � ���������� ������������.
 complex& operator() (ulong i, ulong j);
 //���������� �������������� ��������� ������ ������� ��� ������� � ����������
 //������� (���������� �������). ��� ������ ���������� � ��������� ������������
 //� � ������������ �����������.
 //This version is called in ASSIGNMENT OPERATOR and COPY the CONSTRUCTOR
 const complex& operator() (ulong i, ulong j) const;
 //2. OVERLOAD THE ASSIGNMENT OPERATOR
 //���������� �������������� ��������� ���������� operator =
 bool operator =(const TComplexMatrix& C);
 //3. OVERLOAD THE ASSIGNMENT OPERATOR
 //���������� �������������� ��������� ������������ operator = �����������
 //����������� ������� ��������������� ��������
 bool operator =(const double value);
 //4. OVERLOAD THE ASSIGNMENT OPERATOR
 //���������� �������������� ��������� ������������ operator = ���� �����������
 //����������� ������� ������������ ��������
 bool operator =(const complex value);
 //5. OVERLOAD THE COMPARISON OPERATOR (==)
 //���������� �������������� ��������� ��������� ���� ������ operator ==
 bool operator ==(const TComplexMatrix& A) const;
 //6. OVERLOAD THE STREAM INSERTION OPERATOR
 //���������� �������� ���������� ������ (<<) ��� ����������� ��������� �������
 friend ostream& operator <<(ostream& out, const TComplexMatrix& C);

 //*****************************************************************************
 //             �������� �������� ������� ��� ������������ ���������
 //   ��� �������� �������� ������� ������������ � ������ �������� �������.
 //*****************************************************************************
 //������������ ��������� ���������� ������� ������������ �������������� ���������
 bool Reverse();

 //�������� ���������������� � ������������ ���������� ����������� �������
 cmatrix Transpose(bool conjugate = true) const;

 //���������� �������� ��������� (!) ��� ��������� ����������������� �������
 cmatrix operator !() const;

 //�������� ���������� ����������� �������
 cmatrix Conj() const;

 //���������� �������� ��������� (~) ��� ��������� ����������� �������
 cmatrix operator ~() const;

 //���������� ����� ���������� ����������� �������
 complex Trace() const;
 complex Spur() const {return Trace();};

 //-----------------------------------------------------------------------------
 //����������� ���� (���) ����������� �������
 double NFirstNorm() const;
 double FirstNorm() const;
 double SecondNorm() const;
 double ThirdNorm() const;
 double PNorm(uint p = 2) const;
 double Norm(uint type = EUCLIDEAN_NORM, uint p = 2) const;
 //���������� ����� ������ ������� � �������� �������� �� 0 �� Rows()-1
 double NormRow(ulong index, uint type = EUCLIDEAN_NORM, uint p = 2) const;
 //���������� ����� ������� ������� � �������� �������� �� 0 �� Cols()-1
 double NormCol(ulong index, uint type = EUCLIDEAN_NORM, uint p = 2) const;
 //���������� ���� ���� ����� �������
 rvector NormAllRows(uint type = EUCLIDEAN_NORM, uint p = 2) const;
 bool NormAllRows(rvector& V, uint type = EUCLIDEAN_NORM, uint p = 2) const;
 //���������� ���� ���� �������� �������
 rvector NormAllCols(uint type = EUCLIDEAN_NORM, uint p = 2) const;
 bool NormAllCols(rvector& V, uint type = EUCLIDEAN_NORM, uint p = 2) const;
 //-----------------------------------------------------------------------------
 //������������ ��������� �������
 bool Normalize(cmatrix& A, uint NormType, uint p = 0) const;
 bool Normalize(uint NormType, uint p = 0);
 bool NormalizeToDB(rmatrix& A, double MindB = -60) const;
 //������������ ��������� ������� �� �������
 bool NormalizeRows(cmatrix& A, uint NormType, uint p = 0) const;
 bool NormalizeRows(uint NormType, uint p = 0);
 bool NormalizeRowsToDB(rmatrix& A, double MindB = -60) const;
 //������������ ��������� ������� �� ��������
 bool NormalizeCols(cmatrix& A, uint NormType, uint p = 0) const;
 bool NormalizeCols(uint NormType, uint p = 0);
 bool NormalizeColsToDB(rmatrix& A, double MindB = -60) const;

 //���������� ������� �� �������������� ����� value
 //B = const + A, B(i,j) = B(i,j) + const
 friend cmatrix operator +(const double value, const cmatrix& A);
 //B = A + const, B(i,j) = A(i,j) + const
 friend cmatrix operator +(const cmatrix& A, const double value);

 //���������� ������� �� ����������� ����� value
 //B = const + A, B(i,j) = B(i,j) + const
 friend cmatrix operator +(const complex value, const cmatrix& A);
 //B = A + const, B(i,j) = A(i,j) + const
 friend cmatrix operator +(const cmatrix& A, const complex value);

 //���������� ������� �� �������������� ����� value
 //B = A - const, B(i,j) = A(i,j) - const
 friend cmatrix operator -(const cmatrix& A, const double value);

 //���������� ������� �� ����������� ����� value
 //B = A - const, B(i,j) = A(i,j) - const
 friend cmatrix operator -(const cmatrix& A, const complex value);

 //��������� ������� �� �������������� ����� value
 //B = const * A, B(i,j) = A(i,j) * const
 friend cmatrix operator *(const double value, const cmatrix& A);
 //B = A * const, B(i,j) = A(i,j) * const
 friend cmatrix operator *(const cmatrix& A, const double value);

 //��������� ������� �� ����������� ����� value
 //B = const * A, B(i,j) = A(i,j) * const
 friend cmatrix operator *(const complex value, const cmatrix& A);
 //B = A * const, B(i,j) = A(i,j) * const
 friend cmatrix operator *(const cmatrix& A, const complex value);

 //������� ������� �� �������������� ����� value
 //B = A / const, B(i,j) = A(i,j) / const
 cmatrix operator /(const double value) const;

 //������� ������� �� ����������� ����� value
 //B = A / const, B(i,j) = A(i,j) / const
 cmatrix operator /(const complex value) const;

 //�������� ���� ����������� ������
 //� = A + B, C(i,j) = A(i,j) + B(i,j);
 cmatrix operator +(const cmatrix& A) const;

 //�������� ����������� � ������������ ������ ����� � ������
 friend cmatrix operator +(const rmatrix& A, const cmatrix& B);
 friend cmatrix operator +(const cmatrix& A, const rmatrix& B);

 //��������� ���� ����������� ������
 //� = A - B, C(i,j) = A(i,j) - B(i,j);
 cmatrix operator -(const cmatrix& A) const;

 //��������� �� ����������� ������� ������������ �������
 //� = A - B, C(i,j) = A(i,j) - B(i,j);
 cmatrix operator -(const rmatrix& A) const;

 //��������� ���� ����������� ������
 cmatrix operator *(const cmatrix& A) const;

 //��������� ������������ ������� �� ����������� ������� ����� � ������
 friend cmatrix operator *(const rmatrix& A, const cmatrix& B);
 friend cmatrix operator *(const cmatrix& A, const rmatrix& B);

 //������������ ��������� �������� (�����) ����������� ������� �� �����������
 //������ ����������� � �������� ������� �� �������� (�������)
 cmatrix MultByCols(const cvector& V) const;
 cmatrix MultByCols(const cvector& V, ulong index) const;
 cmatrix MultByCols(const cvector& V, ulong left, ulong right) const;
 cmatrix MultByCols(const cvector& V, const ivector& index, bool check = false) const;

 cmatrix MultByRows(const cvector& V) const;
 cmatrix MultByRows(const cvector& V, ulong index) const;
 cmatrix MultByRows(const cvector& V, ulong left, ulong right) const;
 cmatrix MultByRows(const cvector& V, const ivector& index, bool check = false) const;

 //�������� �������� (�����) ����������� ������� � ����������� ��������
 //����������� � �������� ������� �� ������� (��������)
 cmatrix AddByCols(const cvector& V) const;
 cmatrix AddByCols(const cvector& V, ulong index) const;
 cmatrix AddByCols(const cvector& V, ulong left, ulong right) const;
 cmatrix AddByCols(const cvector& V, const ivector& index, bool check = false) const;

 cmatrix AddByRows(const cvector& V) const;
 cmatrix AddByRows(const cvector& V, ulong index) const;
 cmatrix AddByRows(const cvector& V, ulong left, ulong right) const;
 cmatrix AddByRows(const cvector& V, const ivector& index, bool check = false) const;

 //������������ �������� (�����) ����������� �������
 cvector SumCols(ulong left = 0L, ulong right = MAX_SIZE) const;
 cvector SumCols(const ivector& index) const;

 cvector SumRows(ulong left = 0L, ulong right = MAX_SIZE) const;
 cvector SumRows(const ivector& index) const;

 //-----------------------------------------------------------------------------
 //������ ������� �������� �������
 //-----------------------------------------------------------------------------
 //������ ������ ���� M(0 1 ... k-1 k; 0 1 ... k-1 g), ��� g > k
 complex U_Minor(ulong k, ulong g) const;
 //������ ������ ���� M(0 1 ... k-1 g; 0 1 ... k-1 k), ��� g > k
 complex L_Minor(ulong g, ulong k) const;
 //������ ������ ���� M(0 1 ... k-1 k; 0 1 ... k-1 k) - ������� �����
 complex P_Minor(ulong k) const;
 //������ ���� ������� ���� M(0 1 ... k-1 k; 0 1 ... k-1 g), ��� g > k
 // 0 < g < min(Cols(),Rows()), 0 <= k < g
 cmatrix U_AllMinors() const;
 //������ ������ ���� M(0 1 ... k-1 g; 0 1 ... k-1 k), ��� g > k
 // 0 < g < min(Cols(),Rows()), 0 <= k < g
 cmatrix L_AllMinors() const;
 //������ ������ ���� M(0 1 ... k-1 k; 0 1 ... k-1 k) - ������� �����
 // 0 <= k < min(Cols(),Rows())
 cmatrix P_AllMinors() const;
 cvector& P_AllMinors(cvector& P) const;
 //������ ���� ������� �������
 cmatrix AllMinors(bool L = true, bool P = true, bool U = true) const;

 //-----------------------------------------------------------------------------
 // ���������� ����������� ���������� ������ �� ������, ������� � ������������
 // �������: A = LDU
 // � ��������� ������������ ���������� ���� ������� �������� ������� ��� �������
 // ������������� ������ L, ������� U � ������������ D ������. � ��������������
 // ����� ������ ���� �������� ������ ������� ����������� ��� ���������� �������.
 //-----------------------------------------------------------------------------
 bool LDU_Gauss(cmatrix& LDU) const;
 bool LDU_Gauss(cmatrix& L, cmatrix& D, cmatrix& U) const;

 //-----------------------------------------------------------------------------
 //������ ������������� ����������� ���������� ������ � �� ���������� ��
 //������ � ������� ����������� �������: A = LU
 //-----------------------------------------------------------------------------
 //���������� ������������ ���� Ak*Bk � ������������ �� � ��������� ���������
 friend complex InnerProduct(ulong l, ulong s, ulong u, complex init,
                             const cmatrix& A, bool Avar, ulong Aconst,
                             const cmatrix& B, bool Bvar, ulong Bconst);

 //�������� ������ (����������� ���������� ����� ������ � ��������� �������
 //�������� �������� �� �������): �������, ���������������� ���������� �������
 bool LU_UnSymmetric(complex& Det, cmatrix& LU, ivector& ExChange, double* Real = NULL,
                     double* Imag = NULL, long* Power = NULL) const;
 complex DET_UnSymmetric(double* Real = NULL, double* Imag = NULL, long* Power = NULL) const;

 //-----------------------------------------------------------------------------
 // ������� ����������� ������ �������� ���������
 //-----------------------------------------------------------------------------
 //������� ������� ��������� AX = B, ��� A = LU, B - ������� ������ ������
 //������� A - ���������������� �������. ������� ���������� �� ���������� �����
 //������ (�������� ������).
 cmatrix LE_UnSymSolution(const cmatrix& B, const ivector& ExChange) const;
 friend cmatrix LE_UnSymSolution(const cmatrix& A, const cmatrix& B);

 //-----------------------------------------------------------------------------
 // ��������� ������
 //-----------------------------------------------------------------------------
 //��������� ���������������� ���������� ������� n x n ������� ���������� �� ��
 //����������� ������� L & U c ������� ��������� ������ � ������� ������� �����-
 //��� ��������� � ��������� �������� ����������� n x n � ������ ����� �������
 cmatrix INV_Krauth() const;

 //-----------------------------------------------------------------------------
 //������ ����������� �������� � ����������� �������� ����������� ������
 //-----------------------------------------------------------------------------
 //�������� �����: �������, ��������� �������
 uint EVV_Jacobi(TRealVector& D, TRealMatrix& V, double EPS = 1e-19);
 int EV_Jacobi(TRealVector& D, double EPS = 1e-19, bool lExcludePairVals = true) const;
 //--------------------------------------------------------------------------------
 //������� ������ �������� ����������� �������� ��� ����������� ������ ������ ����
 //--------------------------------------------------------------------------------
 //���������� � ����� ����������� �������� ����������� �������. �������� � ������ �����
 //������� ����������� H ����� �������� � ������������ �������� Ar � Ai
 bool HessenbergForm(rmatrix& Ar, rmatrix& Ai, ivector& permut) const;
 //LR-��������: ������� ������ �������� ����������� �������� ��� ����������� ����������
 //������� � ������������ ��������������� ����������� �������� �������� �������
 bool EVV_LR_algorithm(cvector& W, cmatrix& V, double eps = 1e-19) const;
 //����� ����� � ���������� ����� ��� ����������� ������ ������ ����
 bool EVV_NormReducingJacobi(cvector& W, cmatrix& V, double eps = 1e-19) const;

 //-----------------------------------------------------------------------------
 //���������� �������������� �������
 //-----------------------------------------------------------------------------
 friend bool CorrelationMatrix(const cvector& V, cmatrix& R);
 friend bool CorrelationMatrix(const cmatrix& A, cmatrix& R);

//------------------------------------------------------------------------------
//         ���������� ������������� ������� ������ TComplexMatrix
//------------------------------------------------------------------------------
 //�������� �� ��������� ������������ ������ A � B
 friend bool IsEqualSize(const cmatrix& A, const cmatrix& B)
 {return ((A.Rows() == B.Rows()) && (A.Cols() == B.Cols())) ? true : false;};
 //����������� ������������ ���������� ����� ��������� ������� � ������������
 //������ ������� ������� �������.
 friend uint* MaxStrLen(const TComplexMatrix& C, uint precision);
 //���������� �������������� ����� c_val = trW*A*W,
 //������ trW ����������-����������� ������� W
 friend bool QuadraticForm(complex& c_val, const cmatrix& A, const cvector& W);
 //���������� ������������ ����: �_val = L*A*R, ��� L � R - �������, ������������� ��
 //������� � �������� A (A - ���������� �������)
 friend bool Multiply(complex& c_val, const cvector& L, const cmatrix& A, const cvector& R);
 //��������� �������������� ����: B(m,m) = trC(m,n)*A(n,n)*C(n,m), trC - �����������������
 //����������-����������� ������� � ������� C, A - ���������� �������
 friend bool Transform_trCAC(cmatrix& B, const cmatrix& A, const cmatrix& C);
 //���������� ����� ���������� ��������������: B(m,m) = trC(m,n)*A(n,n)*C(n,m),
 //trC - ����������������� ����������-����������� ������� � C, A - ���������� �������
 friend bool Trace_trCAC(double& Trace, const cmatrix& A, const cmatrix& C);

 //������������� ������� C = Ar + i*Ai, ����������� �������� � ������ ������ ������� C
 //� ������������ �������� Ar � Ai.
 friend bool ComplexToReal(const cmatrix& C, rmatrix& Ar, rmatrix& Ai);
 //�������� ����������� ������� C �� ���� ������������ ������ Ar � Ai, ������� ��������
 //������������ (Ar) � ������ (Ai) ����� ����������� ������� C
 friend bool RealToComplex(cmatrix& C, const rmatrix& Ar, const rmatrix& Ai);

 //���������� �������� ���������� ��������� ������� B � ����������� ������������� � �����-
 //��� A: A = A + B/n, n - ����� ����������
 friend bool Average(cmatrix& A, const cmatrix& B, ulong n);
 //���������� �������� ������������ ���������� ��� ������� ������ ����� ����������
 //�� ������ k-�� ���� ���������� �������� ������� AvX ����������� �� k ����� (k >= 0)
 friend bool InfiniteAverage(cmatrix& AvX, const cmatrix& Xk, ulong k);

 //�������� ������������ ���������� ��������� ������� C
 friend bool Conj(cmatrix& C);
 //���������� ������������ ���������� ��������� ������� B � ����������� ���������� A
 //A = Conj(B), �������� ������� B �� ����������.
 friend bool Conj(cmatrix& A, const cmatrix& B);

//------------------------------------------------------------------------------
//II. ���������� �������� ������-������� ������ TComplexMatrix
//------------------------------------------------------------------------------
private:
 void Initial();//������������� ������-������ ������ ��� ��� ���������������
 //�������� ������� ������������� ������� � ����������� ������ ��� �������������
 bool Create();
 void Delete();//�������� ������� �� ����������� ������

//------------------------------------------------------------------------------
//III. ���������� ���������� ������-������� ������ TComplexMatrix
//------------------------------------------------------------------------------
protected:
 //*****************************************************************************
 // ���������� ������� ����������� ����� ������ ���������� ����
 //*****************************************************************************
 //���� ��������� ����������� ������� (������������� ��� ����������)
 void InPutComplexMatrix(const string& name, bool confirm = true);
 //���� ��������� ��������� �������
 void InPutHermitianMatrix(const string& name, bool confirm = true);
 //���� ��������� ������������ �������
 void InPutDiagonal(const string& name, bool confirm = true);
 //���� ��������� ��������� �������
 void InPutScalar(const string& name, bool confirm = true);
 //���� ��������� ��������� �������
 void InPutBand(ulong diags, const string& name, bool confirm = true);
 //���� ��������� ������� ����������� �������
 void InPutUTriang(const string& name, bool confirm = true);
 //���� ��������� ������ ����������� �������
 void InPutLTriang(const string& name, bool confirm = true);
 //���� ��������� �������������� �������
 void InPutSymmetric(const string& name, bool confirm = true);
 //���� ��������� ������������������ �������
 void InPutSkewSymmetric(const string& name, bool confirm = true);
 //���� ��������� ��������������� �������
 void InPutAltSign(const string& name, bool confirm = true);
 //*****************************************************************************
 //���������� ������� ������������� ��������� ������ ���������� ����
 //*****************************************************************************
 void InitZeroMatrix();
 void InitMatrix();
 void InitDiagonal();
 void InitScalar();
 void InitUnit();
 void InitBand(ulong diags);
 void InitUTriang();
 void InitLTriang();
 void InitSymmetric();
 void InitSkewSymmetric();
 void InitAltSign();
 void InitHermitian();
};
/**** ����� ���������� ������ - ����������� ������� - class TComplexMatrix ****/

//***********************************************************************************
//                ���������� ������ - ������� ����� ����� - TIntMatrix
//TIntMatrix ������� ��������� ������� ����� ����� ���� long ����������� �����������,
//��������� �������� �������� �������� ������� ��� ������ ��������� � �� ����������,
//� ����� ������������ ��� ������ ��������� �������.
//***********************************************************************************
class TIntMatrix {
//���������� ������������� �������

//-----------------------------------------------------------------------------------
//���������� ���������� ������-������ ������
//-----------------------------------------------------------------------------------
protected:
 bool type;//��� ������� (���������� ��� �������������)
 ulong m;  //����� ����� �������/����� �������� ��� ���������� ������
 ulong n;  //����� �������� �������/����� ���������� ��� ��������� �������
 long **data; //��������� �� ��������� ������ �������� ��������� �������
//-----------------------------------------------------------------------------------
//I. ���������� ������������� ������-������� ������ TIntMatrix
//-----------------------------------------------------------------------------------
public:
 //**********************************************************************************
 //     ���������� ������������� � ����������� ������ TIntMatrix
 //**********************************************************************************
 TIntMatrix() {Initial();}; //���������� ������������ �� ���������
 //----------------------------------------------------------------------------------
 //���������� ������������� � �����������
 //----------------------------------------------------------------------------------
 //��������������� ������������� (m != n) ��� ���������� ������ (m == n)
 TIntMatrix(ulong row, ulong col, long value) {Initial(); Set(row,col,value);};
 TIntMatrix(ulong row, ulong col, bool init)  {Initial(); Set(row,col,init);};
 //��������������� ���������� ������ (m == n)
 TIntMatrix(ulong size, long value) {Initial(); Set(size,value);};
 TIntMatrix(ulong size, bool init)  {Initial(); Set(size,init);};
 TIntMatrix(ulong size, uint sq_type, ulong diag = 3L)
 {Initial();Set(size, sq_type, diag);};
 //���������� ������������ �����������
 TIntMatrix(const TIntMatrix& A) {Initial(); *this = A;};
 //���������� ����������� ������
 ~TIntMatrix() {Delete();};
 //----------------------------------------------------------------------------------
 //���������� ������� ������ ���������� ������ TIntMatrix
 //----------------------------------------------------------------------------------
 //**********************************************************************************
 //                         ��������� ������� �������
 //**********************************************************************************
 //----------------------------------------------------------------------------------
 //1. ����������� ���� � ������� �������
 bool Type() const {return type;};
 bool GetType() const {return type;};
 //�������� ������� �� ����������
 bool IsSquare() const {return (type == SQUARE) ? true : false;};
 //�������� ������� �� �������������
 bool IsRectang() const {return (type == RECTANG) ? true : false;};
 //�������� �� ������� �������� �������
 bool IsZeroSize() const {return ((Rows() == 0L) || (Cols() == 0L)) ? true : false;};
 bool IsZero() const;           //�������� �� ������� � �������� ����������
 bool IsSymmetric() const;      //�������� ������� �� ��������������
 bool IsSkewSymmetric() const;  //�������� ������� �� ������������������
 bool IsAltSign() const;        //�������� ������� �� �����������������
 bool IsUpperTriang() const;    //�������� �� ������� ����������� �������
 bool IsLowerTriang() const;    //�������� �� ������ ����������� �������
 bool IsDiagonal() const;       //�������� �� ������������ �������
 bool IsScalar() const;         //�������� �� ��������� �������
 bool IsUnit() const;           //�������� �� ��������� �������
 bool IsBand(ulong& diags) const;//�������� �� ��������� �������
 uint SubType() const; //����������� ������� �������
 uint SubType(ulong& n) const; //����������� ������� �������
 //----------------------------------------------------------------------------------
 //2. ����������� ����������� �������
 //����������� ���������� ����� �������
 ulong Rows() const {return m;};
 //����������� ���������� �������� �������
 ulong Cols() const {return (type == SQUARE) ? m : n;};
 //----------------------------------------------------------------------------------
 //3. ��������� ������� �������
 void Properties(ulong& rows, ulong& cols, bool& type) const
 {rows = Rows(); cols = Cols(); type = GetType();};
 void Properties(ulong& rows, ulong& cols, bool& type, uint& subtype) const;
 void Properties(PF_MATRIX& Info) const;
 string& Properties(string& s) const;//�������� ������� � ���������� ����
 //��������� ������������ ���� ������� (���������� ��� �������������)
 string& GetType(string& s, uint language = ENG) const;
 //��������� ������������ ������� �������
 string& GetSubType(string& s, uint language = ENG) const;
 //���������� ��������� ������� � ���������� ������
 string& GetData(string& s, string& delim) const;
 //����������� ������������ ���������� ����� ��������� ������� � ������������
 //������ ������� ������� �������.
 uint WidthCols(ivector& Width) const;

 //**********************************************************************************
 //                    ��������� ������� �������
 //**********************************************************************************
 bool Set(ulong rows, ulong cols, long value);
 bool Set(ulong rows, ulong cols, bool init);
 bool Set(ulong size, long value) {return Set(size, size, value);};
 bool Set(ulong size, bool init) {return Set(size, size, init);};
 bool Set(ulong size, uint sq_type, ulong diags = 3L);
 bool Set(const PF_MATRIX& Info);
 //**********************************************************************************
 // ������������� ��������� ������� ���������� ����������
 //**********************************************************************************
 bool Rand();//�� ��������������� ������
 bool Rand(long A, long B);//�� ��������������� ������ � ���������
 //���������� �������������� (����) ��������� �������
 //void Edit(bool confirm = true, const char* name = "A");

 //**********************************************************************************
 // ������� ������� � ��������� �������
 //**********************************************************************************
 //��������� �������� �������� �������
 double Get(ulong row, ulong col) const {return data[row][col];};
 //���������� �������� �������� �������
 void Put(ulong row, ulong col, double value) {data[row][col] = value;};

 //��������� ������ ������� � �������� �������� �� 0 �� Rows()-1
 ivector Row(ulong index) const;
 bool Row(ivector& V, ulong index) const;
 //��������� ������� ������� � �������� �������� �� 0 �� Cols()-1
 ivector Col(ulong index) const;
 bool Col(ivector& V, ulong index) const;

 //������������ ���� ����� �������
 bool ExchangeRows(ulong i, ulong j);
 //������������ ����� ������� �������� �������-������������
 bool ExchangeRows(const ivector& permutation);
 //������������ ���� �������� �������
 bool ExchangeCols(ulong i, ulong j);
 //������������ �������� ������� �������� �������-������������
 bool ExchangeCols(const ivector& permutation);
 //----------------------------------------------------------------------------------
 //������������ ��������� ������� � �������� ��������� �������� (�����)
 //� �������� ������� �� �������� (�� �������)
 //----------------------------------------------------------------------------------
 imatrix& ReverseByCol(imatrix& A, ulong left = 0L, ulong right = MAX_SIZE) const;
 imatrix& ReverseByRow(imatrix& A, ulong left = 0L, ulong right = MAX_SIZE) const;
 bool ReverseCols();
 bool ReverseRows();
 //----------------------------------------------------------------------------------
 //������������ ��������� �������� �������
 //----------------------------------------------------------------------------------
 //������������ ���������� ���������� A(0 1 ... k-1 k; 0 1 ... k-1 g), ��� g > k
 imatrix& U_SubMatrix(imatrix& SubMatrix, ulong k, ulong g) const;
 //������������ ���������� ���������� A(0 1 ... k-1 g; 0 1 ... k-1 k), ��� g > k
 imatrix& L_SubMatrix(imatrix& SubMatrix, ulong g, ulong k) const;
 //������������ ���������� ���������� A(0 1 ... k-1 k; 0 1 ... k-1 k)
 imatrix& P_SubMatrix(imatrix& SubMatrix, ulong k) const;

 //������������ ���������� �� �������� ������� �� �������� ��������, ��������� �
 //������� ivector
 imatrix& SubMatrixByCols(imatrix& SubMatrix, const ivector& IndexCols,
                          bool Check=false) const;
 //������������ ���������� �� �������� ������� �� �������� �����, ��������� �
 //������� ivector
 imatrix& SubMatrixByRows(imatrix& SubMatrix, const ivector& IndexRows,
                          bool Check=false) const;
 //������������ ���������� �� �������� ������� �� �������� ����� � ��������,
 //����������� � �������� IndexRows & IndexCols
 imatrix& SubMatrix(imatrix& SubMatrix,  const ivector& IndexRows,
                    const ivector& IndexCols, bool Check = false) const;
 //----------------------------------------------------------------------------------
 // ������������ �������� ������� �������, �������-������, �������-�������
 //----------------------------------------------------------------------------------
 imatrix& AssignRow(imatrix& A, const ivector& V, ulong row, ulong pos = 0L) const;
 imatrix& AssignRow(imatrix& A, const ivector& V, ulong left, ulong right, ulong pos) const;
 imatrix& AssignRow(imatrix& A, const ivector& V, const ivector& rows, ulong pos = 0L) const;

 imatrix& AssignCol(imatrix& A, const ivector& V, ulong col, ulong pos = 0L) const;
 imatrix& AssignCol(imatrix& A, const ivector& V, ulong left = 0L, ulong right = MAX_SIZE,
                    ulong pos = 0L) const;
 imatrix& AssignCol(imatrix& A, const ivector& V, const ivector& cols, ulong pos = 0L) const;

 imatrix& Assign(imatrix& A, const imatrix& B, ulong row = 0L, ulong col = 0L) const;

 //----------------------------------------------------------------------------------
 //              ���������� ������������� ����������
 //----------------------------------------------------------------------------------
 //1. OVERLOAD THE CALL FUNCTION OPERATOR
 //���������� �������������� ��������� ������ ������� ��� ������� � ����������
 //������� (���������� �������). ������������ � ���������� ������������.
 long& operator() (ulong i, ulong j);
 //���������� �������������� ��������� ������ ������� ��� ������� � ����������
 //������� (���������� �������). ��� ������ ���������� � ��������� ������������
 //� � ������������ �����������.
 //This version is called in ASSIGNMENT OPERATOR and COPY the CONSTRUCTOR
 const long& operator() (ulong i, ulong j) const;
 //2. OVERLOAD THE ASSIGNMENT OPERATOR
 //���������� �������������� ��������� ������������ operator=
 bool operator =(const TIntMatrix& A);
 //3. ���������� �������������� ��������� operator= ���������� �� ������ ��������
 //   ����� ����� � � ���������� �� ������������� �������
 bool operator =(const string& s);
 //4. OVERLOAD THE ASSIGNMENT OPERATOR
 //���������� �������������� ��������� ������������ operator = �����������
 //������������ ������� ��������������� ��������
 bool operator =(const long value);
 //5. OVERLOAD THE COMPARISON OPERATOR (==)
 //���������� �������������� ��������� ��������� ���� ������ operator ==
 bool operator ==(const TIntMatrix& A) const;
 //6. OVERLOAD THE STREAM INSERTION OPERATOR
 //���������� �������� ���������� ������ (<<) ��� ����������� ��������� �������
 friend ostream& operator <<(ostream& out, const TIntMatrix& A);

 //**********************************************************************************
 //             �������� �������� ������� ��� �������������� ���������
 //   ��� �������� �������� ������� ������������ � ������ �������� �������.
 //**********************************************************************************
 //������������ ��������� ���������� ������� ������������ �������������� ���������
 bool Reverse();
 imatrix& Reverse(imatrix& A) const;
 //�������� ���������������� �������
 bool Transpose();
 imatrix& Transpose(imatrix& A) const;
 //���������� �������� ��������� (!) ��� ��������� ����������������� �������
 imatrix& operator !() const;
 //���������� ����� ���������� �������
 double Trace() const;
 double Spur() const {return Trace();};
 //----------------------------------------------------------------------------------
 //����������� ���� (���) �������
 double NFirstNorm() const;
 double FirstNorm() const;
 double SecondNorm() const;
 double ThirdNorm() const;
 double PNorm(uint p = 2) const;
 double Norm(uint type = EUCLIDEAN_NORM, uint p = 2) const;

 //���������� ������� �� ����� ����� value
 //B = const + A, B(i,j) = B(i,j) + const
 friend imatrix operator +(const long value, const imatrix& A);
 //B = A + const, B(i,j) = A(i,j) + const
 friend imatrix operator +(const imatrix& A, const long value);

 //���������� ������� �� ����� ����� value
 //B = A - const, B(i,j) = A(i,j) - const
 friend imatrix operator -(const imatrix& A, const long value);

 //��������� ������� �� ����� ����� value
 //B = const * A, B(i,j) = A(i,j) * const
 friend imatrix operator *(const long value, const imatrix& A);
 //B = A * const, B(i,j) = A(i,j) * const
 friend imatrix operator *(const imatrix& A, const long value);

 //������� ������� �� ����� ����� value
 //B = A / const, B(i,j) = A(i,j) / const
 rmatrix operator /(const long value) const;

 //�������� ���� ������������� ������
 //� = A + B, C(i,j) = A(i,j) + B(i,j);
 imatrix operator +(const imatrix& A) const;

 //��������� ���� ������������� ������
 //� = A - B, C(i,j) = A(i,j) - B(i,j);
 imatrix operator -(const imatrix& A) const;

 //��������� ���� ������������� ������
 imatrix operator *(const imatrix& A) const;

 //��������� �������������� ������� ������� ����� � ������
 //��������� �� ������� �����
 friend ivector operator *(const ivector& V, const imatrix& A);
 //��������� �� ������� ������
 friend ivector operator *(const imatrix& A, const ivector& V);

//-----------------------------------------------------------------------------------
//         ���������� ������������� ������� ������ TIntMatrix
//-----------------------------------------------------------------------------------
//��������� �������������� ������ ������� A � ������������� ��������� ������� �� ��������
//� ������ sTbl ���� string
friend string& MakeTable(string& sTbl, const UStringMap& HeadColl, const imatrix& A,
       string& delim = string(1,' '));

//-----------------------------------------------------------------------------------
//II. ���������� �������� ������-������� ������ TIntMatrix
//-----------------------------------------------------------------------------------
private:
 void Initial();//������������� ������-������ ������ ��� ��� ���������������
 //�������� ������� ������������� ������� � ����������� ������ ��� �������������
 bool Create();
 void Delete();//�������� ��������� ������� �� ����������� ������

//-----------------------------------------------------------------------------------
//III. ���������� ���������� ������-������� ������ TIntMatrix
//-----------------------------------------------------------------------------------
protected:
 //**********************************************************************************
 //���������� ������� ������������� ��������� ������ ���������� ����
 //**********************************************************************************
 void InitZeroMatrix();
 void InitMatrix();
 void InitDiagonal();
 void InitScalar();
 void InitUnit();
 void InitBand(ulong diags);
 void InitUTriang();
 void InitLTriang();
 void InitSymmetric();
 void InitSkewSymmetric();
 void InitAltSign();
};
/******* ����� ���������� ������ - ������������� ������� - class TIntMatrix ********/


//***********************************************************************************
//���������� ��������� ������������ ���������� ������ �������� ������������ ������ -
//STR_UINT
//***********************************************************************************
typedef struct
{ uint value; //��������� (��������) ���������� ������
  char* str;  //���������� ������
} STR_UINT;

typedef STR_UINT* pSTR_UINT; //��������� �� ��������� STR_UINT

//***********************************************************************************
//���������� ��������� ������� ������������ ���������� ����� ��������� �����������
//����� ����� - TB_STR_UINT
//***********************************************************************************
typedef struct
{ ulong size;    //���������� ������� � �������
  pSTR_UINT rec; //������� ������������ ���������� ����� ������������� ������
} TB_STR_UINT;

typedef TB_STR_UINT* pTB_STR_UINT; //��������� �� ��������� TB_STR_UINT

//***********************************************************************************
// ���������� ��������� �������
//***********************************************************************************
//���������� ������ ������������ ����� Xr + i*Xi �� �������:
//cabs(X) = sqrt(Xr*Xr + Xi*Xi)
double cabs(double Xr, double Xi);
//������� ���� ����������� �����
//Z = X/Y: Zr + i*Zi = (Xr + i*Xi)/(Yr + i*Yi)
complex cdiv(double Xr, double Xi, double Yr, double Yi);
//���������� ����������� ����� �� ������������ �����
//Y = sqrt(X): Yr + i*Yi = sqrt(Xr + i*Xi)
complex csqrt(double Xr, double Xi);

//������� ������ � ������ s �������� �� ����������� ��������� charset �� ������ str
ulong ReplaceCharSet(string& s, const string& charset, const string& str);
//������� �������������� ������ � ����� ���� complex
bool StrToComplex(complex& z, const string& s, string& ends);
//������� �������������� (�����) ������������ �����
complex EditComplex(bool confirm = true);
//������� �������������� ������ � ����� ���� double
bool StrToDouble(double& z, const string& s, string& ends);
//������� �������������� (�����) ������������� �����
double EditDouble(bool confirm = true);
//������� �������������� ������ � ����� ���� long
bool StrToLong(long& z, const string& s, string& ends);
//������� �������������� (�����) �������� ������ �����
long EditLong(bool confirm = true);
//������� ����� ������������� �������������� �������
void InPutPF_RMATRIX(PF_MATRIX& Info, const char* Name = NULL);
//������� ����� ������������� ����������� �������
void InPutPF_CMATRIX(PF_MATRIX& Info, const char* Name = NULL);
//���������� ������� ������ ������ �� ������� ���� TB_STR_UINT �� �����������
//���������� ������
uint ConsoleChoice(const TB_STR_UINT& Table, uint Default,
                   const char* Header = NULL, uint Offset = 0);
//��������� ���������� ������ �� ������� ���� TB_STR_UINT �� ���� value
const char* GetName(const TB_STR_UINT& Table, uint Value);
void ClrScr(uint x, uint y);

#endif //���������� ���������� ������������� ����� "MATRIX.H"