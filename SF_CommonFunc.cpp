//----------------------------------------------------------------------------------------
//SF_CommonFunc.cpp     C++ Builder 6.0 JohnSoft Initial date: 23/03/2007
//���������� ������������������� ������������ �������, ������������ ��� �������������
//���������� ������ (��) ���������������� ��������� �������� �� ���� ������� ������� �����
//� ������ ��������������� ���������� ���������������� ������� ��.
//----------------------------------------------------------------------------------------
#pragma hdrstop

#include "SF_CommonFunc.h"                          

//========================================================================================
// ���������� ������� ������ ���������� -- IMPLEMENTATION of THE COMMON FUNCTION
//========================================================================================
//****************************************************************************************
//������: Common function # 1
//������������ �������: Edit_CoDec(TFuncGen_DSClipper** pCoDec)
//����������: �������������� ���������� ������-�������� 2D-Signal: ��������� ��������
//������� ���������� � ���������, ����� ������� �����������-�������������, �������� �������
//������-��������. ������� ��������� �������� ������-�������� �� ���������� ��������:
//1) �������� ������������ ����� ��������������;
//2) �������� ��������� ����� ��������������;
//3) ����� �������������� �� ���������������������� ������;
//4) ���������������� ������������ ����� ��������������;
//5) ���������������� ��������� ����� ��������������;
//6) �������������� ����� ��������������;
//7) ���������������� ����� ��������������;
//8) ��������������� ����� ��������������.
//----------------------------------------------------------------------------------------
//������������ ���������:
// TFuncGen_DSClipper** pCoDec - ������� ��������� �� ����������� (�������) ��������������
// ��������������� �������� ������� � ������������ ������������ �� ��������� � ��������
// ������� �������� � ��������
//----------------------------------------------------------------------------------------
//������������ ��������: bool lResult - ��������� ��������������
//****************************************************************************************
bool Edit_CoDec(TFuncGen_DSClipper** pCoDec)
{STR_UINT Option[11] = {
  { 1, "Select < EnCoding By Linear Law >" },
  { 2, "Select < EnCoding By Decreasing Linear Law >" },
  { 3, "Select < EnCoding By SquareRoot Law >" },
  { 4, "Select < EnCoding By Exponential Law >" },
  { 5, "Select < EnCoding By Decreasing Exponential Law >" },
  { 6, "Select < EnCoding By Sinus Law >" },
  { 7, "Select < EnCoding By Cosinus Law >" },
  { 8, "Select < EnCoding By Hyperbolic Law >" },
  { 9, "Edit < Internal Properties >"},
  {10, "View < Properties >" },
  {11, "Exit" } };
 TB_STR_UINT Menu = {11,Option};
 string head = " Choice menu option [1-11]: ";
 //������� ����������
 string line(79,'-');
 const string sPressKey = "Press any key...";
 const string sReselect = "WARNING! You have already selected the same CoDec Type !?\
                           \nTIP: You should select another CoDec Type.";
 const string sNoCoDec  = "WARNING! CoDec hasn't been set yet!!!\
                          \nTIP: First of all You should select some CoDec Type [1-8].";
 const string s = "2D-Signal CoDec: < Editing/Choosing >";
 string t;
 bool flag = true;
 bool lCodec;//������� ������-�������� �������
 double Umin, Umax, Pmin, Pmax;

 clrscr();

 do
 {lCodec = ((*pCoDec) != NULL) ? true : false;
  if (lCodec)
  {(*pCoDec)->GetThreshold_U(Umin,Umax);
   (*pCoDec)->GetThreshold_P(Pmin,Pmax);
   cout << "Current 2D-Signal CoDec: " << typeid(**pCoDec).name() << endl; //New !!!
  }
  else
  {Umin = 0; Umax = 255; Pmin = 10; Pmax = 100;
   cout << "CoDec hasn't been set yet !" << endl;
  }
  cout << line << endl << s << endl << line << endl;
  uint choice = ConsoleChoice(Menu,10,head.c_str(),2);
  switch (choice)
  {case 1: //����� TFuncGen_DSC_LinLaw - ����� � �������� ������� �����������
    if (lCodec)
    {//�������������� ���������� ������ ������� ���� ������
     if (typeid(**pCoDec) == typeid(TFuncGen_DSC_LinLaw))
     {cout << sReselect << endl;
      cout << sPressKey; getch();
      break;
     }
     delete (*pCoDec);//�������� ����� �������������� ������
    }
    (*pCoDec) = new TFuncGen_DSC_LinLaw(Umin,Umax,Pmin,Pmax);//��������������� ������
    (*pCoDec)->Edit();//�������������� ������� ������
    break;

   case 2: //����� TFuncGen_DSC_DecLinLaw - ����� � ��������� �������� ������� �����������
    if (lCodec)
    {//�������������� ���������� ������ ������� ���� ������
     if (typeid(**pCoDec) == typeid(TFuncGen_DSC_DecLinLaw))
     {cout << sReselect << endl;
      cout << sPressKey; getch();
      break;
     }
     delete (*pCoDec);//�������� ����� �������������� ������
    }
    (*pCoDec) = new TFuncGen_DSC_DecLinLaw(Umin,Umax,Pmin,Pmax);//��������������� ������
    (*pCoDec)->Edit();//�������������� ������� ������
    break;

   case 3: //����� TFuncGen_DSC_SqrtLaw - ����� � ��������������������� �������
    if (lCodec)
    {//�������������� ���������� ������ ������� ���� ������
     if (typeid(**pCoDec) == typeid(TFuncGen_DSC_SqrtLaw))
     {cout << sReselect << endl;
      cout << sPressKey; getch();
      break;
     }
     delete (*pCoDec);//�������� ����� �������������� ������
    }
    (*pCoDec) = new TFuncGen_DSC_SqrtLaw(Umin,Umax,Pmin,Pmax);//��������������� ������
    (*pCoDec)->Edit();//�������������� ������� ������
    break;

   case 4: //����� TFuncGen_DSC_ExpLaw - ����� � ���������������� ������� �����������
    if (lCodec)
    {//�������������� ���������� ������ ������� ���� ������
     if (typeid(**pCoDec) == typeid(TFuncGen_DSC_ExpLaw))
     {cout << sReselect << endl;
      cout << sPressKey; getch();
      break;
     }
     delete (*pCoDec);//�������� ����� �������������� ������
    }
    (*pCoDec) = new TFuncGen_DSC_ExpLaw(Umin,Umax,Pmin,Pmax);//��������������� ������
    (*pCoDec)->Edit();//�������������� ������� ������
    break;

   //����� TFuncGen_DSC_DecExpLaw - ����� � ��������� �� ���������� ������� �����������
   case 5:
    if (lCodec)
    {//�������������� ���������� ������ ������� ���� ������
     if (typeid(**pCoDec) == typeid(TFuncGen_DSC_DecExpLaw))
     {cout << sReselect << endl;
      cout << sPressKey; getch();
      break;
     }
     delete (*pCoDec);//�������� ����� �������������� ������
    }
    (*pCoDec) = new TFuncGen_DSC_DecExpLaw(Umin,Umax,Pmin,Pmax);//��������������� ������
    (*pCoDec)->Edit();//�������������� ������� ������
    break;

   case 6: //����� TFuncGen_DSC_SinLaw - ����� � �������������� ������� �����������
    if (lCodec)
    {//�������������� ���������� ������ ������� ���� ������
     if (typeid(**pCoDec) == typeid(TFuncGen_DSC_SinLaw))
     {cout << sReselect << endl;
      cout << sPressKey; getch();
      break;
     }
     delete (*pCoDec);//�������� ����� �������������� ������
    }
    (*pCoDec) = new TFuncGen_DSC_SinLaw(Umin,Umax,Pmin,Pmax);//��������������� ������
    (*pCoDec)->Edit();//�������������� ������� ������
    break;

   case 7: //����� TFuncGen_DSC_CosLaw - ����� � ���������������� ������� �����������
    if (lCodec)
    {//�������������� ���������� ������ ������� ���� ������
     if (typeid(**pCoDec) == typeid(TFuncGen_DSC_CosLaw))
     {cout << sReselect << endl;
      cout << sPressKey; getch();
      break;
     }
     delete (*pCoDec);//�������� ����� �������������� ������
    }
    (*pCoDec) = new TFuncGen_DSC_CosLaw(Umin,Umax,Pmin,Pmax);//��������������� ������
    (*pCoDec)->Edit();//�������������� ������� ������
    break;

   case 8: //����� TFuncGen_DSC_HypebolicLaw - ����� � ��������������� ������� �����������
    if (lCodec)
    {//�������������� ���������� ������ ������� ���� ������
     if (typeid(**pCoDec) == typeid(TFuncGen_DSC_HyperbolicLaw))
     {cout << sReselect << endl;
      cout << sPressKey; getch();
      break;
     }
     delete (*pCoDec);//�������� ����� �������������� ������
    }
    (*pCoDec) = new TFuncGen_DSC_HyperbolicLaw(Umin,Umax,Pmin,Pmax);//��������������� ������
    (*pCoDec)->Edit();//�������������� ������� ������
    break;

   case 9: //�������������� ���������� ���������� ������-�������� 2D-Signal
    if (lCodec) (*pCoDec)->Edit();
    else {cout << sNoCoDec << endl << sPressKey << endl; getch();}
    break;

   case 10: //�������� ������-�������� 2D-Signal
    if (lCodec) cout << (*pCoDec)->Info(t);
    else cout << sNoCoDec << endl;
    cout << line << endl;
    cout << sPressKey << endl; getch();
    break;

   case 11: flag = false; break;//����� �� ������ ��������������
  }
  clrscr();
 } while (flag);
 return true;
}
//****************************************************************************************
//������: Common function # 2
//������������ �������: bool Edit_DispOperator(TCovMatrixPerturbOperator** pDistOp)
//����������: ����� ��������� ���������� �������������� ������� �������� �������� (�����
//������ ������������� ��������������� ������):
//1) Random Law;
//2) Gauss Law.
//������� ������������ ��� ��������������� �������������.
//----------------------------------------------------------------------------------------
//������������ ���������:
// TCovMatrixPerturbOperator** pDistOp - ������� ��������� �� ����������� (�������) �����
// ��������� ���������� �������������� �������.
//----------------------------------------------------------------------------------------
//������������ ��������: bool lResult - ��������� ��������������
//****************************************************************************************
bool Edit_DispOperator(TCovMatrixPerturbOperator** pDistOp)
{STR_UINT Option[4] = {
  {1, "Select < Perturbation By Random Law >" },
  {2, "Select < Perturbation By Gauss Law >" },
  {3, "View < Properties >" },
  {4, "Exit" } };
 TB_STR_UINT Menu = {4,Option};
 string head = " Choice menu option [1-4]: ";
 //������� ����������
 string line(79,'-');
 const string sPressKey = "Press any key...";
 const string sReselect = "WARNING! You have already selected the same DispOp Type !?\
                           \nTIP: You should select another DispOp Type.";
 const string sNoObj  = "WARNING! Distortion Operator hasn't been set yet!!!\
                         \nTIP: First of all You should select some DistOp Type [1-2].";
 const string s = "Covariation Matrix Disturbance Operator: < Selection >";
 string t;
 bool flag = true;
 bool lObject;//������� ��������� ����������

 clrscr();
 do
 {lObject = ((*pDistOp) != NULL) ? true : false;
  if (lObject)
   cout << "Current Distortion Operator: " << typeid(**pDistOp).name() << endl; //New !!!
  else cout << "Distortion Operator hasn't been set yet !" << endl;

  cout << line << endl << s << endl << line << endl;
  uint choice = ConsoleChoice(Menu,3,head.c_str(),2);
  switch (choice)
  {case 1: //����� TDisturbCovMatrix_Random - ���������� �� ��������������� ������
    if (lObject)
    {//�������������� ���������� ������ ������� ���� ��������� ����������
     if (typeid(**pDistOp) == typeid(TDisturbCovMatrix_Random))
     {cout << sReselect << endl;
      cout << sPressKey; getch();
      break;
     }
     delete (*pDistOp);//�������� ����� �������������� ��������� ����������
    }
    (*pDistOp) = new TDisturbCovMatrix_Random();//��������������� ��������� ����������
    break;

   case 2: //����� TDisturbCovMatrix_Gauss - ���������� �� ������������ ������
    if (lObject)
    {//�������������� ���������� ������ ������� ���� ��������� ����������
     if (typeid(**pDistOp) == typeid(TDisturbCovMatrix_Gauss))
     {cout << sReselect << endl;
      cout << sPressKey; getch();
      break;
     }
     delete (*pDistOp);//�������� ����� �������������� ��������� ����������
    }
    (*pDistOp) = new TDisturbCovMatrix_Gauss();//��������������� ��������� ����������
    break;

   case 3: //�������� ��������� ���������� �������������� �������
    if (lObject) cout << (*pDistOp)->Info(t) << endl;
    else cout << sNoObj << endl;
    cout << line << endl;
    cout << sPressKey << endl; getch();
    break;

   case 4: //����� �� ������ �������������� ��������� ���������� �������������� �������
    flag = false;
    break;
  }
  clrscr();
 } while (flag);
 return true;
}
//****************************************************************************************
//������: Common function # 3
//������������ �������: bool Edit_InvOperator(TCovMatrixInversion** pInvOp)
//����������: ����� ������ ��������� �������������� ������� �������� �������� � �� ������-
//��������� �������������� �� ����������:
//1) �����������;
//2) ��������;
//3) ����������������� ��������;
//4) ��������;
//5) ������������� ������ - ��� �������������.
//----------------------------------------------------------------------------------------
//������������ ���������:
// TCovMatrixInversion** pInvOp - ������� ��������� �� ����������� (�������) ����� ���������
// ��������� �������������� �������.
//----------------------------------------------------------------------------------------
//������������ ��������: bool lResult - ��������� ��������������
//****************************************************************************************
bool Edit_InvOperator(TCovMatrixInversion** pInvOp)
{STR_UINT Option[8] = {
  {1, "Select < Inversion without regularizing >" },
  {2, "Select < Inversion by Laurentiev method >" },
  {3, "Select < Inversion by Tychonoff method >" },
  {4, "Select < Inversion by Modified Tychonoff method >" },
  {5, "Select < Inversion by Skachkov method >" },
  {6, "Edit < Properties >" },
  {7, "View < Properties >" },
  {8, "Exit" } };
 TB_STR_UINT Menu = {8,Option};
 string head = " Choice menu option [1-8]: ";
 //������� ����������
 string line(79,'-');
 const string sPressKey = "Press any key...";
 const string sReselect = "WARNING! You have already selected the same InvOperator Type !?\
                           \nTIP: You should select another InvOperator Type.";
 const string sNoObj  = "WARNING! Inversion Operator hasn't been set yet!!!\
                         \nTIP: First of all You should select some InvOperator Type [1-4].";
 const string s = "Covariation Matrix Inversion Operator: < Selection >";
 string t;
 bool flag = true;
 bool lObject;//������� ��������� ��������� �������������� �������

 clrscr();
 do
 {lObject = ((*pInvOp) != NULL) ? true : false;
  if (lObject)
   cout << "Current Inversion Operator: " << typeid(**pInvOp).name() << endl; //New !!!
  else cout << "Inversion Operator hasn't been set yet !" << endl;

  cout << line << endl << s << endl << line << endl;
  uint choice = ConsoleChoice(Menu,7,head.c_str(),2);
  switch (choice)
  {case 1: //����� TInvCovMatrix_Classical - ��������� ������� ��� �� �������������
    if (lObject)
    {//�������������� ���������� ������ ������� ���� ��������� ���������
     if (typeid(**pInvOp) == typeid(TInvCovMatrix_Classical))
     {cout << sReselect << endl;
      cout << sPressKey; getch();
      break;
     }
     delete (*pInvOp);//�������� ����� �������������� ��������� ���������
    }
    (*pInvOp) = new TInvCovMatrix_Classical();//��������������� ��������� ���������
    break;

   case 2: //����� TInvCovMatrix_Laurentiev - ��������� ������� �� ������ �����������
    if (lObject)
    {//�������������� ���������� ������ ������� ���� ��������� ���������
     if (typeid(**pInvOp) == typeid(TInvCovMatrix_Laurentiev))
     {cout << sReselect << endl;
      cout << sPressKey; getch();
      break;
     }
     delete (*pInvOp);//�������� ����� �������������� ��������� ���������
    }
    (*pInvOp) = new TInvCovMatrix_Laurentiev();//��������������� ��������� ���������
    break;

   case 3: //����� TInvCovMatrix_Tychonoff - ��������� ������� �� ������ ��������
    if (lObject)
    {//�������������� ���������� ������ ������� ���� ��������� ���������
     if (typeid(**pInvOp) == typeid(TInvCovMatrix_Tychonoff))
     {cout << sReselect << endl;
      cout << sPressKey; getch();
      break;
     }
     delete (*pInvOp);//�������� ����� �������������� ��������� ���������
    }
    (*pInvOp) = new TInvCovMatrix_Tychonoff();//��������������� ��������� ���������
    break;

   //����� TInvCovMatrix_ModTychonoff - ��������� ������� �� ����������������� ������ ��������
   case 4:
    if (lObject)
    {//�������������� ���������� ������ ������� ���� ��������� ���������
     if (typeid(**pInvOp) == typeid(TInvCovMatrix_ModTychonoff))
     {cout << sReselect << endl;
      cout << sPressKey; getch();
      break;
     }
     delete (*pInvOp);//�������� ����� �������������� ��������� ���������
    }
    (*pInvOp) = new TInvCovMatrix_ModTychonoff();//��������������� ��������� ���������
    break;

   case 5: //����� TInvCovMatrix_Skachkov - ��������� ������� �� ������ ��������
    if (lObject)
    {//�������������� ���������� ������ ������� ���� ��������� ���������
     if (typeid(**pInvOp) == typeid(TInvCovMatrix_Skachkov))
     {cout << sReselect << endl;
      cout << sPressKey; getch();
      break;
     }
     delete (*pInvOp);//�������� ����� �������������� ��������� ���������
    }
    (*pInvOp) = new TInvCovMatrix_Skachkov();//��������������� ��������� ���������
    break;

   case 6: //�������������� ������� ��������� ��������� �������������� �������
    if (lObject) (*pInvOp)->Edit();
    else cout << sNoObj << endl;
    cout << line << endl;
    cout << sPressKey << endl; getch();
    break;

   case 7: //�������� ��������� ��������� �������������� �������
    if (lObject) cout << (*pInvOp)->Info(t) << endl;
    else cout << sNoObj << endl;
    cout << line << endl;
    cout << sPressKey << endl; getch();
    break;

   case 8: //����� �� ������ �������������� ��������� ��������� �������������� �������
    flag = false;
    break;
  }
  clrscr();
 } while (flag);
 return true;
}
//****************************************************************************************
//������: Common function # 4
//������������ �������: void Edit_InputProcess(enProcess& ProcessType, string& sProcess)
//����������: ����� ������������������ (�������������� ������� Rsjn) ��� ���������� (���-
//����������� ������ Usjn) �������� ��������.
//----------------------------------------------------------------------------------------
//������������ ���������:
//1. enProcess& ProcessType - ������ �� ����������-�������� �� ��������� ������������� ����
//   ���������� �������� {DetProcess ��� RandProcess};
//2. string& sProcess - ������ �� ������, � ������� ������������� ������������ ����������
//   �������� ��������.
//----------------------------------------------------------------------------------------
//������������ ��������: ���
//****************************************************************************************
void Edit_InputProcess(enProcess& ProcessType, string& sProcess)
{//��������� ���������
 ProcessType = enRandProcess;
 sProcess.assign("Statistical Vector Usjn");
 //������� ����������
 STR_UINT Options[3] = {
 {1, "Select < Determinate process -> Rsjn >"},
 {2, "Select < Random process -> Usjn >"},
 {3, "Exit"} };
 TB_STR_UINT Menu = {3,Options};
 int row_start = wherey();
 int choice;
 bool flag = true;
 string s, line(78,'-');
 string h("Choose < Input Process Type: CovMatrix Rsjn or StatVector Usjn >");

 gotoxy(1,row_start); clreol();
 do //���� ������ ���� �������� �����������
 {cout << line << endl << h << endl;
  cout << "Input Process Type is " << sProcess << endl << line << endl;
  //����� ����� ��������������
  s = "Choose option [1..3]: ";
  choice = ConsoleChoice(Menu,2,s.c_str(),2);
  switch (choice)
  {case 1:
    ProcessType = enDetProcess;
    sProcess.assign("Determinate Covariation Matrix Rsjn");
    break;
   case 2:
    ProcessType = enRandProcess;
    sProcess.assign("Statistical Vector Usjn");
    break;
   case 3: flag = false; break;
  }
  ClrScr(1,row_start);//������� ������
 } while (flag);
 ClrScr(1,row_start);//������� ������
}
//****************************************************************************************
//������: Common function # 5
//������������ �������: void Edit_JamParams(TDoubleSet& Angle, TDoubleSet& Psum)
//����������: �������������� ���������� ���������� ��������� (�����) � ����� ������� ����-
//������� � ���������� ��������� ��������� ���������.
//----------------------------------------------------------------------------------------
//������������ ���������:
//1. TDoubleSet& Angle - ������ �� ��������� �������� ������� ��������� ����������
//   ���������;
//2. TDoubleSet& Psum - ������ �� ��������� �������� ��������� �������� �������� ����������
//   ���������.
//----------------------------------------------------------------------------------------
//������������ ��������: ���
//****************************************************************************************
void Edit_JamParams(TDoubleSet& Angle, TDoubleSet& Psum)
{//��������� ���������
 //�������� ��������������� ����� (value >= 0) ��� ����� �������� ��������� ��������
 TGreaterOrEqual<double,rvector> PowerPred(0);
 //�������� ����������� ��������� ����������� ���������� - [-90.0..90]
 TBelongClosedInterval<double,rvector> AnglePred(-90.0,90.0);
 //��������� ���� �����������
 Angle.SetName("AngleCoords");
 Psum.SetName("SumPower");
 //��������������� �������� �� ����������� ��������, �� ��������������� ����������
 Angle.Filtering(&AnglePred);
 Psum.Filtering(&PowerPred);

 //������� ����������
 STR_UINT Options[4] = {
 {1, "Edit < Angle Coordinates >"},
 {2, "Edit < Total Power >"},
 {3, "View < AngleCoords & Psum >"},
 {4, "Exit"} };
 TB_STR_UINT Menu = {4,Options};
 int row_start = wherey();
 int choice;
 bool flag = true;
 string s, line(78,'-');
 string sAngle, sPsum;
 string h("Edit < Angle coordinates and Psum values >");

 gotoxy(1,row_start); clreol();
 do //���� �������������� ���������� ���������� ��������� (�����)
 {cout << line << endl << h << endl;
  //����� ����� ��������������
  s = "Choose option [1..4]: ";
  choice = ConsoleChoice(Menu,3,s.c_str(),2);
  switch (choice)
  {case 1: //���� ������� ��������� ���������� ���������
    Angle.EditData(&AnglePred);
    if (Angle.IsEmpty()) Angle.Append(0.0);
    break;
   case 2: //���� �������� ��������� �������� ���������� ���������
    Psum.EditData(&PowerPred);
    if (Psum.IsEmpty()) Psum.Append(0.0);
    break;
   case 3: //�������� �������� ������� ��������� � �������� Psum
    Angle.About(sAngle,80);
    Psum.About(sPsum,80);
    cout << sAngle << endl;
    cout << sPsum << endl;
    cout << line << endl;
    cout << "Press any key..."; getch();
    break;
   case 4: flag = false; break;
  }
  ClrScr(1,row_start);//������� ������
 } while (flag);
 ClrScr(1,row_start);//������� ������
}
//****************************************************************************************
//������: Common function # 6
//������������ �������:
// bool CovMatrixAnalysis(const TUnEqLAA_ODA& LAA, const TJammers_2D& Jams)
//����������: ��������� ������ �������������� ������ ���������� ������ � ����������� ����
//Rjn � Ajn �� ����� ���������� ������� (�� ������ �������� ������� LAA):
//1. ������ �������������� ������ ���������� ������ � ����������� ���� �� ������ �������-
//   ������� PreProcessor (����� ����������������� �������) Rjn (����������� ������� �
//   ������ ��������� ������ �������) � Ajn (�������������� ������� �������������� �������
//   �������);
//2. ������� ������ �������� ����������� ����� (���������� ���� ����������� ����� � ��
//   ����������� ��������) ��� ������ Rjn & Ajn;
//3. ����������� ������ � ������������� ������ Rjn & Ajn ����� �������� �� �����������
//   �����: trRjn, detRjn, trAjn, detAjn �� ��������:
//   trA = Ev1+Ev2+..+EvN, (1) � detA = Ev1*Ev2*..*EvN (2), ��� EVi (1 <= i <= N) i-��
//   ����������� �������� ������� A ������� N;
//4. ����������� ������������� � ������������ �������� ����������� ����� ������, ������
//   ����� ��������������� ������: K = EVmax/EVmin, EVmax, EVmin - ������������ � �����������
//   ����������� ����� �������;
//5. ���������� �������������� ������������ ���������� �������:
//   Ksup = 10*Log10(InPjn*detAjn/detRjn) � dB.
//----------------------------------------------------------------------------------------
//������������ ���������:
//1. const TUnEqLAA_ODA& LAA - ������ �� ������ - �������� �������� �������;
//2. const TJammers_2D& Jams - ������ �� ������ - ���������� ������ (������� ������� ����);
//3. string& sInfo - ������ �� ������ - ��������� ������� �������������� ������� ��������
//   ��������.
//----------------------------------------------------------------------------------------
//������������ ��������: ���
//****************************************************************************************
bool CovMatrixAnalysis(const TUnEqLAA_ODA& LAA, const TJammers_2D& Jams, string& sInfo)
{//���������� ������� ����������
 cmatrix R;  //�������������� ������� ���������� ������ � ����������� ���� �� �����
 cmatrix A;  //�������������� ������� ���������� ������ � ����������� ���� �� �����
 rmatrix EVect_R; //������� ����������� �������� ������� R
 rvector EVals_R; //������ ����������� �������� ������� R
 rmatrix EVect_A; //������� ����������� �������� ������� R
 rvector EVals_A; //������ ����������� �������� ������� R
 double trR, trA; //�������� ������ ������ R & A
 double detR, detA; //������������ ������ R & A
 double MaxEv_R, MinEv_R; //������������ � ����������� ����������� ����� ������� R
 double MaxEv_A, MinEv_A; //������������ � ����������� ����������� ����� ������� A
 double CondNum_R; //����� ��������������� ������� R
 double CondNum_A; //����� ��������������� ������� A
 double Ksup;      //������������� ����������� ���������� �������
 double Norm_A, Norm_R; //��������� ����� ������ A � R
 string s, line(78,'-');
 char buf[1024];
 ostrstream text(buf,sizeof(buf));

 //������� �������������� ������
 sInfo.clear();

 text << line << endl;
 text << "Matrix Analysis of Input Process of Additive Jams & Noise (Rjn & Ajn)" << endl;
 text << line << endl << ends;
 sInfo.append(text.str()); text.seekp(0);

 if (LAA.GetDimLAA() < 2)
 {text << "Error: Linear Antenna Array doesn't have enough reception channels (N < 2) !" << endl;
  text << "Matrix Analysis of Input Process is impossible !!!" << endl << ends;
  sInfo.append(text.str()); text.seekp(0);
  return false;
 }
 //---------------------------------------------------------------------------------------
 //1. ������ �������������� ������ R � A � ������ ����������� ���� ������� � �����������
 //   ������ (�������) �������, ���������� � ��������� �������, ��������� � ������� R
 //   R - ����������� �������������� ������� (� �������� ������� - ��������� ����� LAA)
 //   A - ������� ���������� ��������������� ������� ����������� �������, ������������
 //       �� ������ LAA.
 //---------------------------------------------------------------------------------------
 CalcRjj(R,LAA,Jams,true,true);

 //������������ ������� A �� ����������� �������������� ������� R
 A.Set(R.Rows()-1,false);
 for (ulong i = 0L; i < A.Rows(); i++)
  for (ulong j = 0L; j < A.Cols(); j++) A.Put(i,j,R.Get(i,j));

 //---------------------------------------------------------------------------------------
 //2. ������� ������ �������� ����������� ����� ��� ������ R & A � �� ����������� ��
 //   �������� � ���������� ������������� � ������������ ����������� ��������
 //---------------------------------------------------------------------------------------
 R.EVV_Jacobi(EVals_R,EVect_R);
 EVals_R.BubbleSort(EVect_R,DESCENDING);//���������� ������-� �������� � ��������
 MaxEv_R = EVals_R.Get(0);
 MinEv_R = EVals_R.Get(EVals_R.Size()-1);

 A.EVV_Jacobi(EVals_A,EVect_A);
 EVals_A.BubbleSort(EVect_A,DESCENDING);//���������� ������-� �������� � ��������
 MaxEv_A = EVals_A.Get(0);
 MinEv_A = EVals_A.Get(EVals_A.Size()-1);

 //---------------------------------------------------------------------------------------
 //3. ���������� ����� � ������������ ������ R & A ����� �� ����������� ��������
 //---------------------------------------------------------------------------------------
 detR = 0; trR = 0;
 if (EVals_R.Size() != 0)
 {detR = 1;
  for (ulong i = 0; i < EVals_R.Size(); i += 2)
  {detR *= EVals_R.Get(i); trR += EVals_R.Get(i);}
 }

 detA = 0; trA = 0;
 if (EVals_A.Size() != 0)
 {detA = 1;
  for (ulong i = 0; i < EVals_A.Size(); i += 2)
  {detA *= EVals_A.Get(i); trA += EVals_A.Get(i);}
 }
 //---------------------------------------------------------------------------------------
 //4. ���������� ���������� ���� ������ R & A
 //---------------------------------------------------------------------------------------
 Norm_R = R.ThirdNorm();
 Norm_A = A.ThirdNorm();
 //---------------------------------------------------------------------------------------
 //5. ���������� ����� ��������������� ������ R & A
 //---------------------------------------------------------------------------------------
 CondNum_R = MaxEv_R/MinEv_R;
 CondNum_A = MaxEv_A/MinEv_A;
 //---------------------------------------------------------------------------------------
 //6. ���������� �������������� ������������ ���������� ������� Ksup � ������ ������
 //   �� ����� �������
 //---------------------------------------------------------------------------------------
 ulong N = LAA.GetDimLAA();
 double InPn = LAA.GetPn();
 double InPjn = abs(R.Get(N-1,N-1));
 Ksup = 0;
 if (detR != 0 && detA != 0) Ksup = 10*log10(InPjn*detA/detR);

 //---------------------------------------------------------------------------------------
 //������������� ����������� ������� �������������� ������ �������� �������� � sInfo
 //---------------------------------------------------------------------------------------
 text << line << endl;
 //�������� LAA, �� ������ �������� �������� ���������� ���������������� ������ � ��������
 //�������
 text << LAA << endl;
 //�������� ���������� ������
 text << "Additive Jammer: " << Jams.Count() << ", Psum: " << Jams.TotalPower() << endl;
 text << " " << Jams.Brief(s) << endl << ends;
 sInfo.append(text.str()); text.seekp(0);

 text << line << endl;
 text << "Analysis CovMatrix Rjn: " << endl;
 if (N <= 5) text << "Matrix Rjn: " << R << endl;
 text << "EigenVals Rjn: " << EVals_R << endl;
 text << "MaxEVal: " << MaxEv_R << ", MinEVal: " << MinEv_R << endl;
 text << "Trace Rjn: " << trR << endl;
 text << "Det Rjn: " << detR << endl;
 text << "Eucledian Norm Rjn: " << Norm_R << endl;
 text << "Condition Number Rjn: " << CondNum_R << endl;
 text << line << endl << ends;
 sInfo.append(text.str()); text.seekp(0);

 text << "Analysis CovMatrix Ajn: " << endl;
 text << "EigenVals Ajn: " << EVals_A << endl;
 text << "MaxEVal: " << MaxEv_A << ", MinEVal: " << MinEv_A << endl;
 text << "Trace Ajn: " << trA << endl;
 text << "Det Ajn: " << detA << endl;
 text << "Eucledian Norm Ajn: " << Norm_A << endl;
 text << "Condition Number Ajn: " << CondNum_A << endl;
 text << line << endl << ends;
 sInfo.append(text.str()); text.seekp(0);

 text << "InPn: " << InPn << ", InPjn: " << InPjn << endl;
 text << "InPjn = Pjn/Pn: " << 10*log10(InPjn/InPn) << " dB" << endl;
 text << "Max. Suppression Ratio of Filter Ksup: " << Ksup << " dB" << endl << ends;
 sInfo.append(text.str()); text.seekp(0);

 return true;
}
//****************************************************************************************
//������: Common function # 7
//������������ �������:
// bool LagrangeMethod(cmatrix& B, const cmatrix& InvA, const cmatrix& C)
//����������: ������ ������� �������� ����������� �������� ������� W ���������� ������� ��
//������ ��������: B = I - C*Inv(trC*InvA*C)*trC*InvA, ���:
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
//����� ����� ������� C ������ ���� ����� ����������� ������� A. ������� ���������� true �
//������ ��������� ���������� ������� B � false - � ��������� ������.
//----------------------------------------------------------------------------------------
//������������ ���������:
//1. cmatrix& B - ������ �� �������-�������� �������� ����������� �������� ������� ��;
//2. const cmatrix& InvA - ������ �� �������� ������� � �������������� ������� �����������-
//   ��� ������� ���������� ������� (����������������� �������);
//3. const cmatrix& C - ������ �� ������� ������� ������� ����� ��������������� � ��������
//   �������� �� ��� �������� ����������� ����������.
//----------------------------------------------------------------------------------------
//������������ ��������: bool lSuccess - ��������� ���������� ������� �������� �����������
//****************************************************************************************
bool LagrangeMethod(cmatrix& B, const cmatrix& InvA, const cmatrix& C)
{//�������� �� ������������ ����������
 if (InvA.IsZeroSize() || (InvA.Rows() != InvA.Cols()) ||
     C.IsZeroSize() || (C.Rows() != InvA.Rows())) {B.Set(0,false); return false;}
 //������ ������� B
 cmatrix T;
 //1. ��������� ��������� ������������ ����: T = trC*InvA*C
 Transform_trCAC(T,InvA,C);
 //2. �������� ������� InvT = INV(trC*InvA*C) ������� ������
 T = T.INV_Krauth();
 //3. ��������� ��������� ������������ ����: B = C*InvT*trC
 Transform_trCAC(B,T,C);
 //4. ����������� �������: � = B*InvA
 B = B*InvA;
 //5. ��������� ���������: B = I - B
 if (B.IsZeroSize()) return false;
 ulong N = B.Rows();
 for (ulong i = 0; i < N; i++)
  for (ulong j = 0; j < N; j++)
  {if (i == j) B.Put(i,j,1.0-B.Get(i,j));
   else B.Put(i,j,-1.0*B.Get(i,j));
  }
 return true;
}
//****************************************************************************************
//������: Common function # 8
//������������ �������: bool InputSingleNonNegateValue(rvector& V, const char* name = NULL)
//����������: ������ �� ���� ������ ���������������� �������� � ������ V � ���������� ���
//����������� �� 1 � ������ ������������� ������� � ������ ������ ������ �������� (�������-
//��� �������). ����������� � �������� ������� V ����� ���� ��������. ������� ����������
//true ��� ��������� ����������� ������� �� ���������� ������� ���� ��� ��������� ��������
//���������� �������.
//----------------------------------------------------------------------------------------
//������������ ���������:
//1. rvector& V - ������ �� �������������� ������;
//2. const char* name - ������������ ������� V.
//----------------------------------------------------------------------------------------
//������������ ��������: bool lSuccess - ��������� ����������� ������� V
//****************************************************************************************
bool InputSingleNonNegateValue(rvector& V, const char* name)
{string sName(name);
 char c;
 int row_start = wherey();
 int y;
 bool flag = true;
 bool lSuccess = false;
 string s, line(78,'-');
 double value;

 if (sName.empty()) sName.assign("Vector");
 gotoxy(1,row_start); clreol();
 do //���� ������� �� ��������� ����������� ��� �������� �������
 {//������������ �������� � �������: <sName>: <V.Size()> {V1 V2 ... Vn};
  V.ToString(s,70,6);
  cout << line << endl;
  cout << sName << ": " << V.Size() << " {" << s << "}" << endl;
  cout << line << endl;
  if (V.Size() != 1) //������ �� ��������� ����������� �������
  {y = wherey();
   cout << "Would you like to specify single non-negative value for <" << sName
        << "> [y/n]?: "; cin >> c;
   ClrScr(1,y);//������� ������
   if ((c != 'Y') && (c != 'y')) break;//����� �� �����
   //���� ���������������� ��������
   cout << "Input real value (d >= 0): "; cin >> value;
   V.Set(1,COL,fabs(value));
   lSuccess = true;
  }
  else //������ �� ��������� �������� ���������� �������
  {y = wherey();
   cout << "Would you like to edit value <" << V.Get(0) << "> [y/n]?: "; cin >> c;
   ClrScr(1,y);//������� ������
   if ((c != 'Y') && (c != 'y')) break;//����� �� �����
   //���� ���������������� ��������
   cout << "Input real value (d >= 0): "; cin >> value;
   V.Put(0,fabs(value));
   lSuccess = true;
  }
  ClrScr(1,row_start);//������� ������
 } while (flag);
 ClrScr(1,row_start);//������� ������
 return lSuccess;
}
//****************************************************************************************
//������: Common function # 9
//������������ �������: ulong InputTests(ulong& Test)
//����������: ���������� ���� ����� ������ ��� ���������� ��������������� �������������.
//��������� ������������� �������� ����������� � ���������� Test, ������������ �� ������.
//----------------------------------------------------------------------------------------
//������������ ���������:
// ulong& Test - ������ �� ����������-�������� ����� ��������� �������������.
//----------------------------------------------------------------------------------------
//������������ ��������: ulong Test - ��������� ����� ������
//****************************************************************************************
ulong InputTests(ulong& Test)
{uint y, row_start;
 char c;
 row_start = wherey();
 do //���� ����� ���������� ������
 {cout << "Edit < Number of Tests >" << endl;
  //���� ����� ������
  y = wherey();
  cout << "Test [Test > 0]: "; cin >> Test;
  ClrScr(1,y);//������� ������
  if (Test == 0) Test = 30;
  ClrScr(1,row_start);//������� ������
  cout << "Test: " << Test << endl;
  //������������� ���������� ����� ������ Test
  cout << "Would you like to exit? [y/n]: "; cin >> c;
  ClrScr(1,row_start);//������� ������
  if (c == 'Y' || c == 'y') break;//����� �� ����� ��������������
 } while (true);
 return Test;
}
//****************************************************************************************
//������: Common function # 10
//������������ �������: double InputNonNegateValue(double& rval, const char* name = NULL)
//����������: ���������� ���� ���������������� ������������� �������� � ����������� ������-
//���� � rval, ������������ � ������� �� ������. ������� ���������� ��������� �����.
//----------------------------------------------------------------------------------------
//������������ ���������:
//1. double& rval - ������ �� ������������� �������������� �����;
//2. const char* name - ������������ �������������� ������������� �����.
//----------------------------------------------------------------------------------------
//������������ ��������: double rval - ��������� ������������ ��������������� �����
//****************************************************************************************
double InputNonNegateValue(double& rval, const char* name)
{string sName(name);
 uint row_start;
 char c;
 row_start = wherey();
 if (sName.empty()) sName.assign("Real number");

 do //���� ����� ���������������� ������������� �����
 {cout << "Input <" << sName << "> [val > 0]: "; cin >> rval;
  rval = fabs(rval);
  ClrScr(1,row_start);//������� ������
  cout << sName << ": " << rval << endl;
  //������������� ���������� ��������
  cout << "Would you like to exit? [y/n]: "; cin >> c;
  ClrScr(1,row_start);//������� ������
  if (c == 'Y' || c == 'y') break;//����� �� ����� ��������������
 } while (true);
 return rval;
}
//****************************************************************************************
//������: Common function # 11
//������������ �������:
// bool InputBooleanValue(bool& bval, const char* VarName = NULL, const char* TrueMsg = NULL,
//                        const char* FalseMsg = NULL);
//����������: ���������� ���� �������� �������� � ����������� ���������� � bval, ������������
//�� ������. ��� ����� ������������ ��������, ������� ����� ��������� ����� ��� ��������
//True � False ������������ ����� �����. ���� ������������ ������ ������������� ����� (d>0),
//�� bval ������������� true, ����� false.
//----------------------------------------------------------------------------------------
//������������ ���������:
//1. bool& bval - ������ ������ �� ������������� ������ (����������) �����;
//2. const char* VarName - ������������ ������������� ������� ����������;
//3. const char* TrueMsg - ������������, ��������������� ������ �������� True;
//4. const char* FalseMsg - ������������, ��������������� ������ �������� False.
//----------------------------------------------------------------------------------------
//������������ ��������: bool bval - ��������� ������ �����
//****************************************************************************************
bool InputBooleanValue(bool& bval, const char* name, const char* TrueMsg,
                       const char* FalseMsg)
{uint y, row_start;
 int ival;
 char c;
 const char* sBool[2] = {"False","True"};
 string sName(name);
 if (sName.empty()) sName.assign("Boolean value");
 if (TrueMsg != NULL) sBool[1] = TrueMsg;
 if (FalseMsg != NULL) sBool[0] = FalseMsg;
 row_start = wherey();
 do //���� ����� ����������� ��������
 {cout << "Edit < Boolean Value > 1 => True, 0 => False" << endl;
  //���� ������ ����� � ��� ������������� ��� ������ ��������
  y = wherey();
  cout << "Input <" << sName << ">: "; cin >> ival;
  bval = (ival > 0) ? true : false;
  ival = (bval) ? 1 : 0;
  ClrScr(1,y);//������� ������
  cout << sName << ": " << sBool[ival] << endl;
  //������������� ���������� ��������
  cout << "Would you like to exit? [y/n]: "; cin >> c;
  ClrScr(1,row_start);//������� ������
  if (c == 'Y' || c == 'y') break;//����� �� ����� ��������������
 } while (true);
 return bval;
}
//****************************************************************************************
//������: Common function # 12
//������������ �������: ulong InputNonNegateValue(ulong& ival, const char* name = NULL)
//����������: ���������� ���� ���������������� ������ �������� � ����������� ���������� �
//ival, ������������ � ������� �� ������. ������� ���������� ��������� �����.
//----------------------------------------------------------------------------------------
//������������ ���������:
//1. ulong& ival - ������ �� ������������� ����� �����;
//2. const char* name - ������������ �������������� ������ �����.
//----------------------------------------------------------------------------------------
//������������ ��������: ulong ival - ��������� ����� ��������������� �����
//****************************************************************************************
ulong InputNonNegateValue(ulong& ival, const char* name)
{string sName(name);
 uint row_start;
 char c;
 row_start = wherey();
 if (sName.empty()) sName.assign("Integer number");
 do //���� ����� ���������������� ������ �����
 {cout << "Input <" << sName << "> [val > 0]: "; cin >> ival;
  if (ival == 0) ival = 10;
  ClrScr(1,row_start);//������� ������
  cout << sName << ": " << ival << endl;
  //������������� ���������� ��������
  cout << "Would you like to exit? [y/n]: "; cin >> c;
  ClrScr(1,row_start);//������� ������
  if (c == 'Y' || c == 'y') break;//����� �� ����� ��������������
 } while (true);
 return ival;
}
//****************************************************************************************
//������: Common function # 13
//������������ �������:
// bool StatCalcQadaptDM(TResultStatCalcQadaptDM& Result, const TUnEqLAA_ODA& LAA,
//     const TGrayScaleImgSrc_2D& Signal_2D, const TDoubleSet& ElimDirs,
//     const TJammers_2D& Jams, TCovMatrixInversion* pInvOp, double alpha, ulong n,
//     ulong Test)
//����������: ������ ���������� �������� ��������� ����������������� ������� ��������������
//�������. ������� ������ ������� ����������� ����� ��������� ������������������ ���������
//�������������� ������� �������� �������� EstRsjn. ���. �������� ���������� �������������
//����������� �� �������: Q=10*Log10(Ps/Psjn) ����� ���������� Test ������������� �
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
//4) ����� ������� ������� �� ���� n > 0;
//5) ����� ������������� Test ������� �� ���� (Test > 0).
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
//6) ������������ �������� �� ������: Psjn = Tr[ExtW]*EstRsjn*ExtW.
//����� �� ��������� Ps � OutPsjn ����������� ���������� ������������� ���������� �������.
//��� ������������ ���������� �������� Q (���. �������� Q) ����������� Test �������������.
//������� ���������� true � ������ ��������� ���������� Q (���� � ������ ������������� A)
//� false - � ��������� ������.
//----------------------------------------------------------------------------------------
//������������ ���������:
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
//������������ ��������: bool lSuccess - ��������� ��������� ���������� �������
//****************************************************************************************
bool StatCalcQadaptDM(TResultStatCalcQadaptDM& Result, const TUnEqLAA_ODA& LAA,
     const TGrayScaleImgSrc_2D& Signal_2D, const TDoubleSet& ElimDirs,
     const TJammers_2D& Jams, TCovMatrixInversion* pInvOp, double alpha, ulong n,
     ulong Test)
{//������� ����������
 string ErrMsg;
 bool lUseLCC;//������������� �������� ����������� �������� ������� Waux
 ulong N = LAA.GetDimLAA();//���������� ������� LAA - ����������� ����������� �������
 ulong Naux = (N > 0) ? N-1 : 0;//���������� ��������������� ������� �������
 rvector Series_Q;//������� �������� ����������� ������������� ������� ������� Test
 //������� ��������� ����� ��� ������������� ������������ ����������� ���� �������� �������
 //LAA � ���������� ������, ����������� ��������
 TRandGen_64 RandNoise;
 TRandGen_64 RandJams;

 cvector Un; //�������������� ������ ����������� ���� �� ����� �������
 cvector Uj; //�������������� ������ ���������� ������ �� ����� �������
 cvector Us; //����������������� ������ ��������� ������� �� ����� �������
 cvector U;  //��������� ������ U = Us + Uj + Un

 cmatrix EstRsjn;//��������� �������������� ������� �������� �������� �� n ����������
 cmatrix R;    //�������������� ������� �������� �������� �� 1 ����������
 cmatrix A;    //�������������� ������� ��������������� ������� ����������� �������
 cmatrix InvAa;//�������� ������� � ������������������ ������� A
 cvector b;    //������ �������� ���������� ��������� � ��������������� �������
 cvector Waux; //������� ������, ������������ ��� EstAa: Waux = -InvAa*B*b
 cvector ExtW;//����������� ������� ������ ExtW = {Waux 1}

 double Ps = Signal_2D.GetPower();//�������� ��������� �������
 double Psjn;//�������� �� ������ ���������� ������� (������-������-���)
 double q;//���������� �������������: q = 10*Log10(Ps/Psjn)
 complex z;
 //���������� ������ ��� ������� �������� ����������� �� ����������� ������� ������
 //� ����� ���������� ���������� �������� � �������� ����������� ���������� ��������
 cmatrix B; //B = I - C*Inv(trC*InvAa*C)*trC*InvAa - �������� ����������� Wa
 //������� ������� ������� ����� ��������������� � �������� �������� ������� ��� ��������
 //����������� ���������� ����������� (NxK), N - ����� �������������� ������� �������,
 //K - ����� ����������� ���������� (��� �������, ���� - � ����������� �� �������� ������)
 cmatrix C;

 //---------------------------------------------------------------------------------------
 //�������� �� ����������� ���������� �������� � ��������� ������������� ���������
 //�������� ������
 //---------------------------------------------------------------------------------------
 Result.Q = -100.0;//��������� �������� ���������� ������������� ���������� �������
 Result.MSD_Q = 0.0;//��������� �������� ���� ��� Q
 Result.InvErr = 0L;//���-� ��������� �������� ��������� ��������� ������� A
 Result.Msg.clear();//������� ������ ���������

 ErrMsg.clear();
 if (Naux < 1) ErrMsg.append("LAA Channels are less than 2;\n");
 if (Ps == 0) ErrMsg.append("Signal is bad: Ps == 0;\n");
 if (pInvOp == NULL) ErrMsg.append("Inversion Operator is absent;\n");
 if (n == 0) ErrMsg.append("Sample size must be greater than 0;\n");
 if (Test == 0) ErrMsg.append("Number of Test must be greater than 0;\n");

 if (!ErrMsg.empty()) //���������� ���������� ������������� ����������
 {Result.Msg.assign("Statistical Calculating of the Qadapt is impossible:\n");
  Result.Msg.append(ErrMsg);
  return false;
 }

 //======================================================================================
 //���������� � �������������
 //======================================================================================
 //--------------------------------------------------------------------------------------
 //������ ������� C, ������������ ��� ������������ ������� �������� ����������� B ��������
 //������� �� ��������� �������� ��� ������� ����������� ����������
 //--------------------------------------------------------------------------------------
 lUseLCC = LAA.TFunc(C,ElimDirs,false);//false - ��� ������� ������� � �������� ������ LAA

 //--------------------------------------------------------------------------------------
 //������������ ������ EstRsjn, A, �������� b � ExtW. ��������� ��������� ������� ExtWa.
 //���������� �������� ������� ��������� ������� Us (����������������� �������).
 //������������ Series_Q - ������� �������� q, ����������� �� Test ���������
 //--------------------------------------------------------------------------------------
 //������� �������������� ������� �������������� �������
 A.Set(Naux,false);
 //������ �������� ���������� ��������� � �������������� ������� �������
 b.Set(Naux,COL,false);
 //��������� ��������� �������� ������� ExtW
 ExtW.Set(N,COL,C_ZERO);
 ExtW.Put(Naux,C_ONE);
 //��������� ������������� ��������� �������������� ������� EstRsjn
 EstRsjn.Set(N,N,false);
 //������ ������� Us
 CalcSignal(Us,LAA,Ps,Signal_2D.GetAngle(DEG),true);
 //������������ ������� �������� ���������� �������� ���������, �������������� �� Test
 //���������
 Series_Q.Set(Test,COL,false);

 //--------------------------------------------------------------------------------------
 // ���������� �������������� ������� Qadapt ����� ���������� Test �������������
 //--------------------------------------------------------------------------------------
 //-------------------------------------------------------------------------------------
 //���� � 1 - �� ����� ������������� Test
 //-------------------------------------------------------------------------------------
 for (ulong t = 0; t < Test; t++)
 {//��������� ������������� ������� EstRjn
  for (ulong i = 0; i < N; i++)
   for (ulong j = 0; j < N; j++) EstRsjn.Put(i,j,C_ZERO);
  //------------------------------------------------------------------------------------
  //���� � 2 - ������ EstRjn �� ������ ���������� n
  //------------------------------------------------------------------------------------
  for (ulong k = 0; k < n; k++)
  {//��� 1. ������������ ��������������� ������� �������� �������� (���������� ������ +
   //���������� ��� �������� ������� ����������� �������)
   CalcJams(Uj,LAA,Jams,RandJams,true);
   CalcNoise(Un,LAA,RandNoise,true);
   U = Us+Uj+Un;
   //��� 2. ������������ �������������� ������� �������� �������� Rjn(n) �� ������� Ujn
   CorrelationMatrix(U,R);
   //��� 3. ������������ ������� EstRjn
   for (ulong i = 0; i < N; i++)
    for (ulong j = 0; j < N; j++) EstRsjn.Put(i,j,EstRsjn.Get(i,j)+R.Get(i,j)/n);
  }//end ����� � 2 - �� ����� ���������� n

  //������������ �������������� ������� ��������������� ������� A � ������ ��������
  //���������� b ����� �������� � ���������������� �������� ������� �� ������� EstRsjn
  for (ulong i = 0L; i < Naux; i++)
  {b.Put(i,EstRsjn.Get(i,Naux));
   for (ulong j = 0L; j < Naux; j++) A.Put(i,j,EstRsjn.Get(i,j));
  }

  //��������� ������� A c �� ��������������� �������������� �� ���������� ���������
  if (pInvOp->Inverse(InvAa,A,alpha) == false) //������� A - �����������
  {Result.InvErr++;//������� ��������� ���������
   //�������� �� ������ ���������� ������� � ������ ����������� ������� A - ��������
   //�� ������ ��������� ������
   Psjn = abs(EstRsjn.Get(Naux,Naux));
  }
  else //���������� ������� ������ ���������� �������
  {//������ B = I - C*Inv[trC*InvAa*C]*trC*InvAa
   if (lUseLCC) LagrangeMethod(B,InvAa,C);
   //���������� ������� Wa = -InvAa*B*b
   if (lUseLCC) Waux = InvAa*(B*b);//-1.0*(InvAa*(B*b))
   else Waux = InvAa*b;
   //������������ ������� ExtW
   for (ulong i = 0; i < Waux.Size(); i++) ExtW.Put(i,-1.0*Waux.Get(i));
   //���������� �������� �� ������ �������: Psjn = abs(TrExtWa*Rsjn*ExtWa)
   QuadraticForm(z,EstRsjn,ExtW);
   Psjn = abs(z);
  }
  //������������ ���������� ������������� ���������� ������� q
  q = 10*log10(Ps/Psjn);
  Series_Q.Put(t,q);
 }//End ����� � 1 - �� ����� ������������� Test

 //---------------------------------------------------------------------------------------
 //�������������� ������ ���������� �����������: ������ �� � ���� �������� q, ����������
 //�� Test �������������
 //---------------------------------------------------------------------------------------
 StatAnalysis(Series_Q,Result.Q,Result.MSD_Q);
 return true;
}
