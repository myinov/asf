#include "const.h"

#if !defined(__PREDICATE_HPP)
#define __PREDICATE_HPP
//**********************************************************************************
// ���������� ��������� ������� ��� ������ � ���������
// 1. template <class TArg, class TVector> class TArgVectorPredicate
// 2. template <class TArg, class TVector> class TAlwaysTrue
// 3. template <class TArg, class TVector> class TBelongClosedInterval
// 4. template <class TArg, class TVector> class TClosedIntervalWithDiscontPoints
// 5. template <class TArg, class TVector> class TGreaterOrEqual
//**********************************************************************************
//----------------------------------------------------------------------------------
//���������� ���������� ������ TPredicate, ������� ���������� ��� bool � ���������
//��� ��������� - ���� ���� ������ �� TArg, ������ ������ �� ������ TVector.
//� �������� ������� ��������� ������� ������������ ������ �� ������ ivector,
//rvector, cvector.
//----------------------------------------------------------------------------------
template <class TArg, class TVector>
class TArgVectorPredicate
{protected:
  TVector v_param;//������ ���������� ���������
  bool lReady;//"�����������������" �������
 public:
  TArgVectorPredicate() {v_param = TVector(); SetReady();};
  TArgVectorPredicate(const TVector& param) {v_param = param; SetReady();}
  virtual bool operator() (const TArg& Value) const = 0;
  virtual string& GetPerform(string& s) const = 0;
  void operator =(const TVector& V) {v_param = V; SetReady();};
  bool IsReady() const {return lReady;};
 protected:
  void SetReady() {lReady = (v_param.Size() == 0) ? false : true;};
};

template <class TArg, class TVector>
class TAlwaysTrue: public TArgVectorPredicate<TArg, TVector>
{public:
  TAlwaysTrue() : TArgVectorPredicate<TArg, TVector>() {SetReady();};
  TAlwaysTrue(const TVector& param) :
  TArgVectorPredicate<TArg, TVector>(param) {SetReady();};
  bool operator() (const TArg& Value) const {return true;}
  string& GetPerform(string& s) const {return s = "(-INF..+INF)";}
 protected:
  void SetReady() {lReady = true;};
};
//-----------------------------------------------------------------------------------
//��������� ����� ����������� �������������� �������� ���� TArg ��������� ���������,
//����������� ����� ����������� left & right, ������� ���������� ����� TVector:
//Param[0] = left; Param[1] = right.
//-----------------------------------------------------------------------------------
template <class TArg, class TVector>
class TBelongClosedInterval: public TArgVectorPredicate<TArg, TVector>
{public:
  //������������ ������
  TBelongClosedInterval() : TArgVectorPredicate<TArg, TVector>() {SetReady();};
  TBelongClosedInterval(const TVector& param) :
  TArgVectorPredicate<TArg, TVector>(param) {SetReady();};
  TBelongClosedInterval(const TArg& bottom, const TArg& upper)
  {v_param.Resize(2); v_param[0] = bottom; v_param[1] = upper; SetReady();};

  //���������� ��������� ������ �������
  bool operator() (const TArg& Value) const
  {bool result = false;
   //�������� �� ������������ ����������, ����������� ����� TVector
   if (!IsReady()) return result;
   TArg left = v_param[0]; TArg right = v_param[1];
   //�������� �� �������������� ��������� ��������� [left..right] �������� Value
   if ((Value >= left) && (Value <= right)) result = true;
   return result;
  }
  void operator =(const TVector& V) {v_param = V; SetReady();};
  string& GetPerform(string& s) const
  {s.resize(0);
   if (v_param.Size() != 2) return s = "Closed Interval Bounds Indefinite.";
   if (v_param[1] < v_param[0])
   {s = "Closed Interval Bounds is Wrong.\n";
    s.append("Left Bound Value > Right Bound Value.\n");
   }
   char buffer[80]; ostrstream text (buffer, sizeof(buffer));
   text << "[" << v_param[0] << ".." << v_param[1] << "]" << ends;
   s.append(text.str()); text.seekp(0);
   return s;
  }
 protected:
  void SetReady()
  {ulong size = v_param.Size();
   lReady = ((size != 2) || (v_param[1] < v_param[0])) ? false : true;
  }
};
//-----------------------------------------------------------------------------------
//��������� ����� ����������� �������������� �������� ���� TArg ��������� ���������,
//������� �������� ����� �� ������������� ������� ��������� - ����� �������. ������
//���������� TVector ����� ��������� ���������:
//v_param[0] = left bound; v_param[1] = right bound; v_param[2]...v_param[N-1] -
//����� �������, ������� ������ ���������� ������ ���������; N - ����������� �������
//���������� ������ TClosedIntervalWithDiscontPoints. ����� ����� �� ��������� �����
//�������. � ���� ������ �� ���������� ���������� ������ TBelongClosedInterval.
//-----------------------------------------------------------------------------------
template <class TArg, class TVector>
class TClosedIntervalWithDiscontPoints: public TArgVectorPredicate<TArg, TVector>
{protected:
  TVector discont_points;
 public:
  TClosedIntervalWithDiscontPoints() : TArgVectorPredicate<TArg, TVector>()
  {SetReady(); Set();};
  TClosedIntervalWithDiscontPoints(const TVector& param) :
  TArgVectorPredicate<TArg, TVector>(param) {SetReady(); Set();};
  bool operator() (const TArg& Value) const
  {bool result = false;
   //�������� �� ������������ ����������, ����������� ����� TVector
   if (!IsReady()) return result;
   TArg left = v_param[0]; TArg right = v_param[1];
   //�������� �� �������������� ��������� ��������� [left..right] �������� Value
   if ((Value >= left) && (Value <= right)) result = true;
   else return false;
   //����� �� ���������� Value � ������� ������ ���������, ������� ��� ��
   //�����������
   result = (discont_points.Find(Value)) ? false : true;
   return result;
  }
  void operator =(const TVector& V) {v_param = V; SetReady(); Set();};
  string& GetPerform(string& s) const
  {s.resize(0);
   if (v_param.Size() < 2) return s = "Closed Interval Bounds Indefinite.";
   if (v_param[1] < v_param[0])
   {s = "Closed Interval Bounds is Wrong.\n";
    s.append("Left Bound Value > Right Bound Value.\n");
   }
   char buffer[80]; ostrstream text (buffer, sizeof(buffer));
   text << "[" << v_param[0] << ".." << v_param[1] << "] " << ends;
   s.append(text.str()); text.seekp(0);
   text << "Discontinuity Points = " << discont_points.Size() << ";" << ends;
   s.append(text.str()); text.seekp(0);
   return s;
  }
 protected:
  void SetReady()
  {ulong size = v_param.Size();
   lReady = ((size < 2) || (v_param[1] < v_param[0])) ? false : true;
  }
  void Set();//��������� ���������� ������
};//���������� �������� ������ TClosedIntervalWithDiscontPoints
//-----------------------------------------------------------------------------------
//���������� ������-������� ������ TClosedIntervalWithDiscontPoints
//-----------------------------------------------------------------------------------
template <class TArg, class TVector>
void TClosedIntervalWithDiscontPoints<TArg, TVector>::Set()
{discont_points = TVector();
 if ((!IsReady()) || (v_param.Size() < 3)) return;
 ulong count = 0L;
 //������� ����� ������� ���������, ������� ��� �� �����������
 TArg left = v_param[0]; TArg right = v_param[1]; TArg value;
 for (ulong i = 2; i < v_param.Size(); i++)
 {value = v_param[i]; if ((value >= left) && (value <= right)) count++;}
 if (count == 0L) return;
 discont_points.Resize(count);
 //��������� �������� ����� ������� ��������� �� v_param � discont_points
 ulong j = 0L;
 for (ulong i = 2; i < v_param.Size(); i++)
 {value = v_param[i];
  if ((value >= left) && (value <= right)) //����� ������� �� v_param ����������� ���������
  {discont_points[j++] = value;
   if (j == discont_points.Size()) break; //�������� �� ���������� ������� discont_points
  }
 }
 v_param.Resize(2,true); //����������� ������� v_param
}//���������� ������� Set()
//-----------------------------------------------------------------------------------
//��������� ����� TGreaterOrEqual ���������� �������������� �������� ���� TArg ���������:
//[LowerBound..+INF) ����������� ����� ���������� LowerBound, ������������ ����� TVector:
//Param[0] = LowerBound.
//-----------------------------------------------------------------------------------
template <class TArg, class TVector>
class TGreaterOrEqual: public TArgVectorPredicate<TArg, TVector>
{public:
  TGreaterOrEqual() : TArgVectorPredicate<TArg, TVector>() {SetReady();};
  TGreaterOrEqual(const TVector& param) :
  TArgVectorPredicate<TArg, TVector>(param) {SetReady();};
  TGreaterOrEqual(const TArg& LowerBound)
  {v_param.Resize(1); v_param[0] = LowerBound; SetReady();};

  //���������� ��������� ������ �������
  bool operator() (const TArg& Value) const
  {bool result = false;
   //�������� �� ������������ ����������, ����������� ����� TVector
   if (!IsReady()) return result;
   TArg LowerBound = v_param[0];
   //�������� �� �������������� ��������� [LowerBound..+INF] �������� Value
   if (Value >= LowerBound) result = true;
   return result;
  }
  void operator =(const TVector& V) {v_param = V; SetReady();};
  string& GetPerform(string& s) const
  {s.resize(0);
   if (v_param.Size() != 1) return s = "Interval [LowerBound..+INF) is undefined.";
   char buffer[40]; ostrstream text (buffer, sizeof(buffer));
   text << "[" << v_param[0] << "..+INF)" << ends;
   s.append(text.str()); text.seekp(0);
   return s;
  }
 protected:
  void SetReady() {lReady = (v_param.Size() != 1) ? false : true;}
};

#endif //���������� ���������� ������������� ����� "PREDICATE.HPP"

 