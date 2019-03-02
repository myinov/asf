//----------------------------------------------------------------------------------------
//SF_CommonFunc.cpp     C++ Builder 6.0 JohnSoft Initial date: 23/03/2007
//Реализация общеупотребительных интерфейсных функций, используемых при моделировании
//адаптивных систем (АС) пространственной обработки сигналов на фоне внешних шумовых полей
//с учетом внутрисистемных возмущений параметрического вектора АС.
//----------------------------------------------------------------------------------------
#pragma hdrstop

#include "SF_CommonFunc.h"                          

//========================================================================================
// РЕАЛИЗАЦИЯ ФУНКЦИЙ ОБЩЕГО НАЗНАЧЕНИЯ -- IMPLEMENTATION of THE COMMON FUNCTION
//========================================================================================
//****************************************************************************************
//СТАТУС: Common function # 1
//НАИМЕНОВАНИЕ ФУНКЦИИ: Edit_CoDec(TFuncGen_DSClipper** pCoDec)
//НАЗНАЧЕНИЕ: Редактирование параметров кодера-декодера 2D-Signal: установка значений
//порогов напряжений и мощностей, выбор функции кодирования-декодирования, просмотр свойств
//кодера-декодера. Функция позволяет выбирать кодеры-декодеры со следующими законами:
//1) Линейный возрастающий закон преобразования;
//2) Линейный убывающий закон преобразования;
//3) Закон преобразования по обратноквадратическому закону;
//4) Экспоненциальный возрастающий закон преобразования;
//5) Экспоненциальный убывающий закон преобразования;
//6) Синусоидальный закон преобразования;
//7) Косинусоидальный закон преобразования;
//8) Гиперболический закон преобразования.
//----------------------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
// TFuncGen_DSClipper** pCoDec - двойной указатель на абстрактный (базовый) функциональный
// преобразователь значений сигнала с двусторонним ограничением по максимуму и минимуму
// уровней мощности и амплитуд
//----------------------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: bool lResult - результат редактирования
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
 //Рабочие переменные
 string line(79,'-');
 const string sPressKey = "Press any key...";
 const string sReselect = "WARNING! You have already selected the same CoDec Type !?\
                           \nTIP: You should select another CoDec Type.";
 const string sNoCoDec  = "WARNING! CoDec hasn't been set yet!!!\
                          \nTIP: First of all You should select some CoDec Type [1-8].";
 const string s = "2D-Signal CoDec: < Editing/Choosing >";
 string t;
 bool flag = true;
 bool lCodec;//Наличие кодера-декодера сигнала
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
  {case 1: //Выбор TFuncGen_DSC_LinLaw - кодек с линейным законом кодирования
    if (lCodec)
    {//Предотвращение повторного выбора данного типа кодека
     if (typeid(**pCoDec) == typeid(TFuncGen_DSC_LinLaw))
     {cout << sReselect << endl;
      cout << sPressKey; getch();
      break;
     }
     delete (*pCoDec);//Удаление ранее установленного кодека
    }
    (*pCoDec) = new TFuncGen_DSC_LinLaw(Umin,Umax,Pmin,Pmax);//Конструирование кодека
    (*pCoDec)->Edit();//Редактирование свойств кодека
    break;

   case 2: //Выбор TFuncGen_DSC_DecLinLaw - кодек с убывающим линейным законом кодирования
    if (lCodec)
    {//Предотвращение повторного выбора данного типа кодека
     if (typeid(**pCoDec) == typeid(TFuncGen_DSC_DecLinLaw))
     {cout << sReselect << endl;
      cout << sPressKey; getch();
      break;
     }
     delete (*pCoDec);//Удаление ранее установленного кодека
    }
    (*pCoDec) = new TFuncGen_DSC_DecLinLaw(Umin,Umax,Pmin,Pmax);//Конструирование кодека
    (*pCoDec)->Edit();//Редактирование свойств кодека
    break;

   case 3: //Выбор TFuncGen_DSC_SqrtLaw - кодек с обратноквадратическим законом
    if (lCodec)
    {//Предотвращение повторного выбора данного типа кодека
     if (typeid(**pCoDec) == typeid(TFuncGen_DSC_SqrtLaw))
     {cout << sReselect << endl;
      cout << sPressKey; getch();
      break;
     }
     delete (*pCoDec);//Удаление ранее установленного кодека
    }
    (*pCoDec) = new TFuncGen_DSC_SqrtLaw(Umin,Umax,Pmin,Pmax);//Конструирование кодека
    (*pCoDec)->Edit();//Редактирование свойств кодека
    break;

   case 4: //Выбор TFuncGen_DSC_ExpLaw - кодек с экспоненциальным законом кодирования
    if (lCodec)
    {//Предотвращение повторного выбора данного типа кодека
     if (typeid(**pCoDec) == typeid(TFuncGen_DSC_ExpLaw))
     {cout << sReselect << endl;
      cout << sPressKey; getch();
      break;
     }
     delete (*pCoDec);//Удаление ранее установленного кодека
    }
    (*pCoDec) = new TFuncGen_DSC_ExpLaw(Umin,Umax,Pmin,Pmax);//Конструирование кодека
    (*pCoDec)->Edit();//Редактирование свойств кодека
    break;

   //Выбор TFuncGen_DSC_DecExpLaw - кодек с убывающим по экспоненте законом кодирования
   case 5:
    if (lCodec)
    {//Предотвращение повторного выбора данного типа кодека
     if (typeid(**pCoDec) == typeid(TFuncGen_DSC_DecExpLaw))
     {cout << sReselect << endl;
      cout << sPressKey; getch();
      break;
     }
     delete (*pCoDec);//Удаление ранее установленного кодека
    }
    (*pCoDec) = new TFuncGen_DSC_DecExpLaw(Umin,Umax,Pmin,Pmax);//Конструирование кодека
    (*pCoDec)->Edit();//Редактирование свойств кодека
    break;

   case 6: //Выбор TFuncGen_DSC_SinLaw - кодек с синусоидальным законом кодирования
    if (lCodec)
    {//Предотвращение повторного выбора данного типа кодека
     if (typeid(**pCoDec) == typeid(TFuncGen_DSC_SinLaw))
     {cout << sReselect << endl;
      cout << sPressKey; getch();
      break;
     }
     delete (*pCoDec);//Удаление ранее установленного кодека
    }
    (*pCoDec) = new TFuncGen_DSC_SinLaw(Umin,Umax,Pmin,Pmax);//Конструирование кодека
    (*pCoDec)->Edit();//Редактирование свойств кодека
    break;

   case 7: //Выбор TFuncGen_DSC_CosLaw - кодек с косинусоидальным законом кодирования
    if (lCodec)
    {//Предотвращение повторного выбора данного типа кодека
     if (typeid(**pCoDec) == typeid(TFuncGen_DSC_CosLaw))
     {cout << sReselect << endl;
      cout << sPressKey; getch();
      break;
     }
     delete (*pCoDec);//Удаление ранее установленного кодека
    }
    (*pCoDec) = new TFuncGen_DSC_CosLaw(Umin,Umax,Pmin,Pmax);//Конструирование кодека
    (*pCoDec)->Edit();//Редактирование свойств кодека
    break;

   case 8: //Выбор TFuncGen_DSC_HypebolicLaw - кодек с гиперболическим законом кодирования
    if (lCodec)
    {//Предотвращение повторного выбора данного типа кодека
     if (typeid(**pCoDec) == typeid(TFuncGen_DSC_HyperbolicLaw))
     {cout << sReselect << endl;
      cout << sPressKey; getch();
      break;
     }
     delete (*pCoDec);//Удаление ранее установленного кодека
    }
    (*pCoDec) = new TFuncGen_DSC_HyperbolicLaw(Umin,Umax,Pmin,Pmax);//Конструирование кодека
    (*pCoDec)->Edit();//Редактирование свойств кодека
    break;

   case 9: //Редактирование внутренних параметров кодера-декодера 2D-Signal
    if (lCodec) (*pCoDec)->Edit();
    else {cout << sNoCoDec << endl << sPressKey << endl; getch();}
    break;

   case 10: //Свойства кодера-декодера 2D-Signal
    if (lCodec) cout << (*pCoDec)->Info(t);
    else cout << sNoCoDec << endl;
    cout << line << endl;
    cout << sPressKey << endl; getch();
    break;

   case 11: flag = false; break;//Выход из режима редактирования
  }
  clrscr();
 } while (flag);
 return true;
}
//****************************************************************************************
//СТАТУС: Common function # 2
//НАИМЕНОВАНИЕ ФУНКЦИИ: bool Edit_DispOperator(TCovMatrixPerturbOperator** pDistOp)
//НАЗНАЧЕНИЕ: Выбор оператора возмущения ковариационной матрицы входного процесса (выбор
//закона распределения внутрисистемных ошибок):
//1) Random Law;
//2) Gauss Law.
//Функция используется для статистического моделирования.
//----------------------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
// TCovMatrixPerturbOperator** pDistOp - двойной указатель на абстрактный (базовый) класс
// оператора возмущения ковариационной матрицы.
//----------------------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: bool lResult - результат редактирования
//****************************************************************************************
bool Edit_DispOperator(TCovMatrixPerturbOperator** pDistOp)
{STR_UINT Option[4] = {
  {1, "Select < Perturbation By Random Law >" },
  {2, "Select < Perturbation By Gauss Law >" },
  {3, "View < Properties >" },
  {4, "Exit" } };
 TB_STR_UINT Menu = {4,Option};
 string head = " Choice menu option [1-4]: ";
 //Рабочие переменные
 string line(79,'-');
 const string sPressKey = "Press any key...";
 const string sReselect = "WARNING! You have already selected the same DispOp Type !?\
                           \nTIP: You should select another DispOp Type.";
 const string sNoObj  = "WARNING! Distortion Operator hasn't been set yet!!!\
                         \nTIP: First of all You should select some DistOp Type [1-2].";
 const string s = "Covariation Matrix Disturbance Operator: < Selection >";
 string t;
 bool flag = true;
 bool lObject;//Наличие оператора возмущения

 clrscr();
 do
 {lObject = ((*pDistOp) != NULL) ? true : false;
  if (lObject)
   cout << "Current Distortion Operator: " << typeid(**pDistOp).name() << endl; //New !!!
  else cout << "Distortion Operator hasn't been set yet !" << endl;

  cout << line << endl << s << endl << line << endl;
  uint choice = ConsoleChoice(Menu,3,head.c_str(),2);
  switch (choice)
  {case 1: //Выбор TDisturbCovMatrix_Random - возмущение по равновероятному закону
    if (lObject)
    {//Предотвращение повторного выбора данного типа оператора возмущения
     if (typeid(**pDistOp) == typeid(TDisturbCovMatrix_Random))
     {cout << sReselect << endl;
      cout << sPressKey; getch();
      break;
     }
     delete (*pDistOp);//Удаление ранее установленного оператора возмущения
    }
    (*pDistOp) = new TDisturbCovMatrix_Random();//Конструирование оператора возмущения
    break;

   case 2: //Выбор TDisturbCovMatrix_Gauss - возмущение по гауссовскому закону
    if (lObject)
    {//Предотвращение повторного выбора данного типа оператора возмущения
     if (typeid(**pDistOp) == typeid(TDisturbCovMatrix_Gauss))
     {cout << sReselect << endl;
      cout << sPressKey; getch();
      break;
     }
     delete (*pDistOp);//Удаление ранее установленного оператора возмущения
    }
    (*pDistOp) = new TDisturbCovMatrix_Gauss();//Конструирование оператора возмущения
    break;

   case 3: //Свойства оператора возмущения ковариационной матрицы
    if (lObject) cout << (*pDistOp)->Info(t) << endl;
    else cout << sNoObj << endl;
    cout << line << endl;
    cout << sPressKey << endl; getch();
    break;

   case 4: //Выход из режима редактирования оператора возмущения ковариационной матрицы
    flag = false;
    break;
  }
  clrscr();
 } while (flag);
 return true;
}
//****************************************************************************************
//СТАТУС: Common function # 3
//НАИМЕНОВАНИЕ ФУНКЦИИ: bool Edit_InvOperator(TCovMatrixInversion** pInvOp)
//НАЗНАЧЕНИЕ: Выбор метода обращения ковариационной матрицы входного процесса с ее предва-
//рительной регуляризацией по алгоритмам:
//1) Лаврентьева;
//2) Тихонова;
//3) Модифицированному Тихонова;
//4) Скачкова;
//5) Классическому методу - без регуляризации.
//----------------------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
// TCovMatrixInversion** pInvOp - двойной указатель на абстрактный (базовый) класс оператора
// обращения ковариационной матрицы.
//----------------------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: bool lResult - результат редактирования
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
 //Рабочие переменные
 string line(79,'-');
 const string sPressKey = "Press any key...";
 const string sReselect = "WARNING! You have already selected the same InvOperator Type !?\
                           \nTIP: You should select another InvOperator Type.";
 const string sNoObj  = "WARNING! Inversion Operator hasn't been set yet!!!\
                         \nTIP: First of all You should select some InvOperator Type [1-4].";
 const string s = "Covariation Matrix Inversion Operator: < Selection >";
 string t;
 bool flag = true;
 bool lObject;//Наличие оператора обращения ковариационной матрицы

 clrscr();
 do
 {lObject = ((*pInvOp) != NULL) ? true : false;
  if (lObject)
   cout << "Current Inversion Operator: " << typeid(**pInvOp).name() << endl; //New !!!
  else cout << "Inversion Operator hasn't been set yet !" << endl;

  cout << line << endl << s << endl << line << endl;
  uint choice = ConsoleChoice(Menu,7,head.c_str(),2);
  switch (choice)
  {case 1: //Выбор TInvCovMatrix_Classical - обращение матрицы без ее регуляризации
    if (lObject)
    {//Предотвращение повторного выбора данного типа оператора обращения
     if (typeid(**pInvOp) == typeid(TInvCovMatrix_Classical))
     {cout << sReselect << endl;
      cout << sPressKey; getch();
      break;
     }
     delete (*pInvOp);//Удаление ранее установленного оператора обращения
    }
    (*pInvOp) = new TInvCovMatrix_Classical();//Конструирование оператора обращения
    break;

   case 2: //Выбор TInvCovMatrix_Laurentiev - обращение матрицы по методу Лаврентьева
    if (lObject)
    {//Предотвращение повторного выбора данного типа оператора обращения
     if (typeid(**pInvOp) == typeid(TInvCovMatrix_Laurentiev))
     {cout << sReselect << endl;
      cout << sPressKey; getch();
      break;
     }
     delete (*pInvOp);//Удаление ранее установленного оператора обращения
    }
    (*pInvOp) = new TInvCovMatrix_Laurentiev();//Конструирование оператора обращения
    break;

   case 3: //Выбор TInvCovMatrix_Tychonoff - обращение матрицы по методу Тихонова
    if (lObject)
    {//Предотвращение повторного выбора данного типа оператора обращения
     if (typeid(**pInvOp) == typeid(TInvCovMatrix_Tychonoff))
     {cout << sReselect << endl;
      cout << sPressKey; getch();
      break;
     }
     delete (*pInvOp);//Удаление ранее установленного оператора обращения
    }
    (*pInvOp) = new TInvCovMatrix_Tychonoff();//Конструирование оператора обращения
    break;

   //Выбор TInvCovMatrix_ModTychonoff - обращение матрицы по модифицированному методу Тихонова
   case 4:
    if (lObject)
    {//Предотвращение повторного выбора данного типа оператора обращения
     if (typeid(**pInvOp) == typeid(TInvCovMatrix_ModTychonoff))
     {cout << sReselect << endl;
      cout << sPressKey; getch();
      break;
     }
     delete (*pInvOp);//Удаление ранее установленного оператора обращения
    }
    (*pInvOp) = new TInvCovMatrix_ModTychonoff();//Конструирование оператора обращения
    break;

   case 5: //Выбор TInvCovMatrix_Skachkov - обращение матрицы по методу Скачкова
    if (lObject)
    {//Предотвращение повторного выбора данного типа оператора обращения
     if (typeid(**pInvOp) == typeid(TInvCovMatrix_Skachkov))
     {cout << sReselect << endl;
      cout << sPressKey; getch();
      break;
     }
     delete (*pInvOp);//Удаление ранее установленного оператора обращения
    }
    (*pInvOp) = new TInvCovMatrix_Skachkov();//Конструирование оператора обращения
    break;

   case 6: //Редактирование свойств оператора обращения ковариационной матрицы
    if (lObject) (*pInvOp)->Edit();
    else cout << sNoObj << endl;
    cout << line << endl;
    cout << sPressKey << endl; getch();
    break;

   case 7: //Свойства оператора обращения ковариационной матрицы
    if (lObject) cout << (*pInvOp)->Info(t) << endl;
    else cout << sNoObj << endl;
    cout << line << endl;
    cout << sPressKey << endl; getch();
    break;

   case 8: //Выход из режима редактирования оператора обращения ковариационной матрицы
    flag = false;
    break;
  }
  clrscr();
 } while (flag);
 return true;
}
//****************************************************************************************
//СТАТУС: Common function # 4
//НАИМЕНОВАНИЕ ФУНКЦИИ: void Edit_InputProcess(enProcess& ProcessType, string& sProcess)
//НАЗНАЧЕНИЕ: Выбор детерминированного (ковариационная матрица Rsjn) или случайного (ста-
//тистический вектор Usjn) входного процесса.
//----------------------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. enProcess& ProcessType - ссылка на переменную-приемник со значением перечислимого типа
//   выбранного процесса {DetProcess или RandProcess};
//2. string& sProcess - ссылка на строку, в которой присваивается наименование выбранного
//   входного процесса.
//----------------------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: нет
//****************************************************************************************
void Edit_InputProcess(enProcess& ProcessType, string& sProcess)
{//Начальная установка
 ProcessType = enRandProcess;
 sProcess.assign("Statistical Vector Usjn");
 //Рабочие переменные
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
 do //Цикл выбора типа входного воздействия
 {cout << line << endl << h << endl;
  cout << "Input Process Type is " << sProcess << endl << line << endl;
  //Выбор опции редактирования
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
  ClrScr(1,row_start);//Очистка экрана
 } while (flag);
 ClrScr(1,row_start);//Очистка экрана
}
//****************************************************************************************
//СТАТУС: Common function # 5
//НАИМЕНОВАНИЕ ФУНКЦИИ: void Edit_JamParams(TDoubleSet& Angle, TDoubleSet& Psum)
//НАЗНАЧЕНИЕ: Редактирование параметров источников излучения (помех) с одной угловой коор-
//динатой и изменяемой суммарной мощностью излучения.
//----------------------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. TDoubleSet& Angle - ссылка на контейнер значений угловых координат источников
//   излучения;
//2. TDoubleSet& Psum - ссылка на контейнер значений суммарных значений мощности источников
//   излучения.
//----------------------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: нет
//****************************************************************************************
void Edit_JamParams(TDoubleSet& Angle, TDoubleSet& Psum)
{//Начальная установка
 //Предикат неотрицательных чисел (value >= 0) для ввода значений суммарной мощности
 TGreaterOrEqual<double,rvector> PowerPred(0);
 //Предикат допустимого интервала направлений исключения - [-90.0..90]
 TBelongClosedInterval<double,rvector> AnglePred(-90.0,90.0);
 //Установка имен контейнеров
 Angle.SetName("AngleCoords");
 Psum.SetName("SumPower");
 //Предварительное удаление из контейнеров значений, не удовлетворяющих предикатам
 Angle.Filtering(&AnglePred);
 Psum.Filtering(&PowerPred);

 //Рабочие переменные
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
 do //Цикл редактирования параметров источников излучения (помех)
 {cout << line << endl << h << endl;
  //Выбор опции редактирования
  s = "Choose option [1..4]: ";
  choice = ConsoleChoice(Menu,3,s.c_str(),2);
  switch (choice)
  {case 1: //Ввод угловых координат источников излучения
    Angle.EditData(&AnglePred);
    if (Angle.IsEmpty()) Angle.Append(0.0);
    break;
   case 2: //Ввод значений суммарной мощности источников излучения
    Psum.EditData(&PowerPred);
    if (Psum.IsEmpty()) Psum.Append(0.0);
    break;
   case 3: //Просмотр значений угловых координат и значений Psum
    Angle.About(sAngle,80);
    Psum.About(sPsum,80);
    cout << sAngle << endl;
    cout << sPsum << endl;
    cout << line << endl;
    cout << "Press any key..."; getch();
    break;
   case 4: flag = false; break;
  }
  ClrScr(1,row_start);//Очистка экрана
 } while (flag);
 ClrScr(1,row_start);//Очистка экрана
}
//****************************************************************************************
//СТАТУС: Common function # 6
//НАИМЕНОВАНИЕ ФУНКЦИИ:
// bool CovMatrixAnalysis(const TUnEqLAA_ODA& LAA, const TJammers_2D& Jams)
//НАЗНАЧЕНИЕ: Матричный анализ ковариационных матриц аддитивной помехи и внутреннего шума
//Rjn и Ajn на входе адаптивной системы (на выходе приемных каналов LAA):
//1. Расчет ковариационных матриц аддитивной помехи и внутреннего шума на выходе предпро-
//   цессора PreProcessor (входе пространственного фильтра) Rjn (расширенная матрица с
//   учетом ОСНОВНОГО канала фильтра) и Ajn (ковариационная матрица дополнительных каналов
//   фильтра);
//2. Решение полной проблемы собственных чисел (вычисление всех собственных чисел и их
//   собственных векторов) для матриц Rjn & Ajn;
//3. Определение следов и определителей матриц Rjn & Ajn через значения их собственных
//   чисел: trRjn, detRjn, trAjn, detAjn по формулам:
//   trA = Ev1+Ev2+..+EvN, (1) и detA = Ev1*Ev2*..*EvN (2), где EVi (1 <= i <= N) i-ое
//   собственное значение матрицы A порядка N;
//4. Определение максимального и минимального значений собственных чисел матриц, расчет
//   чисел обусловленности матриц: K = EVmax/EVmin, EVmax, EVmin - максимальное и минимальное
//   собственные числа матрицы;
//5. Вычисление потенциального коэффициента подавления фильтра:
//   Ksup = 10*Log10(InPjn*detAjn/detRjn) в dB.
//----------------------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. const TUnEqLAA_ODA& LAA - ссылка на объект - линейная антенная решетка;
//2. const TJammers_2D& Jams - ссылка на объект - аддитивная помеха (внешнее шумовое поле);
//3. string& sInfo - ссылка на строку - результат анализа ковариационной матрицы входного
//   процесса.
//----------------------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: нет
//****************************************************************************************
bool CovMatrixAnalysis(const TUnEqLAA_ODA& LAA, const TJammers_2D& Jams, string& sInfo)
{//Объявление рабочих переменных
 cmatrix R;  //Ковариационная матрица аддитивной помехи и внутреннего шума на входе
 cmatrix A;  //Ковариационная матрица аддитивной помехи и внутреннего шума на входе
 rmatrix EVect_R; //Матрица собственных векторов матрицы R
 rvector EVals_R; //Вектор собственных значений матрицы R
 rmatrix EVect_A; //Матрица собственных векторов матрицы R
 rvector EVals_A; //Вектор собственных значений матрицы R
 double trR, trA; //Значения следов матриц R & A
 double detR, detA; //Определители матриц R & A
 double MaxEv_R, MinEv_R; //Максимальное и минимальное собственные числа матрицы R
 double MaxEv_A, MinEv_A; //Максимальное и минимальное собственные числа матрицы A
 double CondNum_R; //Число обусловленности матрицы R
 double CondNum_A; //Число обусловленности матрицы A
 double Ksup;      //Потенциальный коэффициент подавления фильтра
 double Norm_A, Norm_R; //Евклидовы нормы матриц A и R
 string s, line(78,'-');
 char buf[1024];
 ostrstream text(buf,sizeof(buf));

 //Очистка результирующей строки
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
 //1. Расчет ковариационных матриц R и A с учетом внутреннего шума каналов и размещением
 //   строки (столбца) матрицы, связанного с отсчетным каналом, последним в матрице R
 //   R - расширенная ковариационная матрица (с основным каналом - отсчетный канал LAA)
 //   A - матрица ковариаций вспомогательных каналов адаптивного фильтра, построенного
 //       на основе LAA.
 //---------------------------------------------------------------------------------------
 CalcRjj(R,LAA,Jams,true,true);

 //Формирование матрицы A по расширенной ковариационной матрице R
 A.Set(R.Rows()-1,false);
 for (ulong i = 0L; i < A.Rows(); i++)
  for (ulong j = 0L; j < A.Cols(); j++) A.Put(i,j,R.Get(i,j));

 //---------------------------------------------------------------------------------------
 //2. Решение полной проблемы собственных чисел для матриц R & A с их сортировкой по
 //   убыванию и отысканием максимального и минимального собственных значений
 //---------------------------------------------------------------------------------------
 R.EVV_Jacobi(EVals_R,EVect_R);
 EVals_R.BubbleSort(EVect_R,DESCENDING);//Сортировка собств-х значений и векторов
 MaxEv_R = EVals_R.Get(0);
 MinEv_R = EVals_R.Get(EVals_R.Size()-1);

 A.EVV_Jacobi(EVals_A,EVect_A);
 EVals_A.BubbleSort(EVect_A,DESCENDING);//Сортировка собств-х значений и векторов
 MaxEv_A = EVals_A.Get(0);
 MinEv_A = EVals_A.Get(EVals_A.Size()-1);

 //---------------------------------------------------------------------------------------
 //3. Вычисление следа и определителя матриц R & A через их собственные значения
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
 //4. Вычисление евклидовых норм матриц R & A
 //---------------------------------------------------------------------------------------
 Norm_R = R.ThirdNorm();
 Norm_A = A.ThirdNorm();
 //---------------------------------------------------------------------------------------
 //5. Вычисление чисел обусловленности матриц R & A
 //---------------------------------------------------------------------------------------
 CondNum_R = MaxEv_R/MinEv_R;
 CondNum_A = MaxEv_A/MinEv_A;
 //---------------------------------------------------------------------------------------
 //6. Вычисление потенциального коэффициента подавления фильтра Ksup и уровня помехи
 //   на входе фильтра
 //---------------------------------------------------------------------------------------
 ulong N = LAA.GetDimLAA();
 double InPn = LAA.GetPn();
 double InPjn = abs(R.Get(N-1,N-1));
 Ksup = 0;
 if (detR != 0 && detA != 0) Ksup = 10*log10(InPjn*detA/detR);

 //---------------------------------------------------------------------------------------
 //Формировавние результатов анализа ковариационных матриц входного процесса в sInfo
 //---------------------------------------------------------------------------------------
 text << line << endl;
 //Свойства LAA, на основе которого строится адаптивный пространственный фильтр с ОСНОВНЫМ
 //каналом
 text << LAA << endl;
 //Свойства аддитивной помехи
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
//СТАТУС: Common function # 7
//НАИМЕНОВАНИЕ ФУНКЦИИ:
// bool LagrangeMethod(cmatrix& B, const cmatrix& InvA, const cmatrix& C)
//НАЗНАЧЕНИЕ: Расчет матрицы линейных ограничений весового вектора W адаптивной системы по
//методу Лагранжа: B = I - C*Inv(trC*InvA*C)*trC*InvA, где:
//1) InvA - обратная матрица к ковариационной матрице входного процесса дополнит-х каналов
//   адаптивной системы (эрмитовая матрица) размерности N;
//2) C - комплексная матрица размерности (N*K), N - кол-во дополнительных каналов АС, K -
//   число ограничений (направлений исключения - направления режекции приема источников
//   излучения дополнительными каналами АС). Каждый столбец матрицы C задает значения фазо-
//   вых набегов между i-ым дополнительным и основным каналами АС (пространственного фильтра)
//   при действии источника излучения единичной мощности с k-го направления исключения.
//   Для LAA коэффициенты матрицы C вычисляются по формуле: C[i,k]=EXP(j*2PI*d[i]*sin(Qk)),
//   d[i] - относительное расстояние (в длинах волн) между i-м вспомогательным и основным
//   каналами; Qk - угловое положение k-го источника излучения (в радианах), исключаемого
//   при приеме дополнительными каналами АС (направление исключения);
//3) I - диагональная единичная матрица размерности N.
//Матрица B используется, как правило, для приема полезного сигнала без его подавления АС,
//когда положение источника излучения полезного сигнала известно.
//Число строк матрицы C должны быть равно размерности матрицы A. Функция возвращает true в
//случае успешного вычисления матрицы B и false - в противном случае.
//----------------------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. cmatrix& B - ссылка на матрицу-приемник линейных ограничений весового вектора АС;
//2. const cmatrix& InvA - ссылка на обратную матрица к ковариационной матрице дополнитель-
//   ных каналов адаптивной системы (пространственного фильтра);
//3. const cmatrix& C - ссылка на матрицу фазовых набегов между дополнительными и основным
//   каналами АС для заданных направлений исключения.
//----------------------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: bool lSuccess - результат вычисления матрицы линейных ограничений
//****************************************************************************************
bool LagrangeMethod(cmatrix& B, const cmatrix& InvA, const cmatrix& C)
{//Проверка на допустимость вычислений
 if (InvA.IsZeroSize() || (InvA.Rows() != InvA.Cols()) ||
     C.IsZeroSize() || (C.Rows() != InvA.Rows())) {B.Set(0,false); return false;}
 //Расчет матрицы B
 cmatrix T;
 //1. Вычисляем матричное произведение вида: T = trC*InvA*C
 Transform_trCAC(T,InvA,C);
 //2. Обращаем матрицу InvT = INV(trC*InvA*C) методом Краута
 T = T.INV_Krauth();
 //3. Вычисляем матричное произведение вида: B = C*InvT*trC
 Transform_trCAC(B,T,C);
 //4. Перемножаем матрицы: В = B*InvA
 B = B*InvA;
 //5. Вычисляем выражение: B = I - B
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
//СТАТУС: Common function # 8
//НАИМЕНОВАНИЕ ФУНКЦИИ: bool InputSingleNonNegateValue(rvector& V, const char* name = NULL)
//НАЗНАЧЕНИЕ: Запрос на ввод одного неотрицательного значения в вектор V с изменением его
//размерности до 1 в случае подтверждения запроса и вводом только одного значения (консоль-
//ная функция). Размерность и значения вектора V могут быть изменены. Функция возвращает
//true при изменении размерности вектора до единичного размера либо при изменении значения
//единичного вектора.
//----------------------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. rvector& V - ссылка на модифицируемый вектор;
//2. const char* name - наименование вектора V.
//----------------------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: bool lSuccess - результат модификации вектора V
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
 do //Цикл запроса на изменение размерности или значения вектора
 {//Формирование сведений о векторе: <sName>: <V.Size()> {V1 V2 ... Vn};
  V.ToString(s,70,6);
  cout << line << endl;
  cout << sName << ": " << V.Size() << " {" << s << "}" << endl;
  cout << line << endl;
  if (V.Size() != 1) //Запрос на изменение размерности вектора
  {y = wherey();
   cout << "Would you like to specify single non-negative value for <" << sName
        << "> [y/n]?: "; cin >> c;
   ClrScr(1,y);//Очистка экрана
   if ((c != 'Y') && (c != 'y')) break;//Выход из цикла
   //Ввод неотрицательного значения
   cout << "Input real value (d >= 0): "; cin >> value;
   V.Set(1,COL,fabs(value));
   lSuccess = true;
  }
  else //Запрос на изменение значения единичного вектора
  {y = wherey();
   cout << "Would you like to edit value <" << V.Get(0) << "> [y/n]?: "; cin >> c;
   ClrScr(1,y);//Очистка экрана
   if ((c != 'Y') && (c != 'y')) break;//Выход из цикла
   //Ввод неотрицательного значения
   cout << "Input real value (d >= 0): "; cin >> value;
   V.Put(0,fabs(value));
   lSuccess = true;
  }
  ClrScr(1,row_start);//Очистка экрана
 } while (flag);
 ClrScr(1,row_start);//Очистка экрана
 return lSuccess;
}
//****************************************************************************************
//СТАТУС: Common function # 9
//НАИМЕНОВАНИЕ ФУНКЦИИ: ulong InputTests(ulong& Test)
//НАЗНАЧЕНИЕ: Консольный ввод числа опытов для проведения статистического моделирования.
//Введенная пользователем величина сохраняется в переменной Test, передаваемой по ссылке.
//----------------------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
// ulong& Test - ссылка на переменную-приемник числа введенных экспериментов.
//----------------------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: ulong Test - введенное число опытов
//****************************************************************************************
ulong InputTests(ulong& Test)
{uint y, row_start;
 char c;
 row_start = wherey();
 do //Цикл ввода количества опытов
 {cout << "Edit < Number of Tests >" << endl;
  //Ввод числа опытов
  y = wherey();
  cout << "Test [Test > 0]: "; cin >> Test;
  ClrScr(1,y);//Очистка экрана
  if (Test == 0) Test = 30;
  ClrScr(1,row_start);//Очистка экрана
  cout << "Test: " << Test << endl;
  //Подтверждение введенного числа опытов Test
  cout << "Would you like to exit? [y/n]: "; cin >> c;
  ClrScr(1,row_start);//Очистка экрана
  if (c == 'Y' || c == 'y') break;//Выход из цикла редактирования
 } while (true);
 return Test;
}
//****************************************************************************************
//СТАТУС: Common function # 10
//НАИМЕНОВАНИЕ ФУНКЦИИ: double InputNonNegateValue(double& rval, const char* name = NULL)
//НАЗНАЧЕНИЕ: Консольный ввод неотрицательного вещественного значения с сохранением резуль-
//тата в rval, передаваемой в функцию по ссылке. Функция возвращает результат ввода.
//----------------------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. double& rval - ссылка на редактируемое действительное число;
//2. const char* name - наименование редактируемого вещественного числа.
//----------------------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: double rval - введенное вещественное неотрицательное число
//****************************************************************************************
double InputNonNegateValue(double& rval, const char* name)
{string sName(name);
 uint row_start;
 char c;
 row_start = wherey();
 if (sName.empty()) sName.assign("Real number");

 do //Цикл ввода неотрицательного вещественного числа
 {cout << "Input <" << sName << "> [val > 0]: "; cin >> rval;
  rval = fabs(rval);
  ClrScr(1,row_start);//Очистка экрана
  cout << sName << ": " << rval << endl;
  //Подтверждение введенного значения
  cout << "Would you like to exit? [y/n]: "; cin >> c;
  ClrScr(1,row_start);//Очистка экрана
  if (c == 'Y' || c == 'y') break;//Выход из цикла редактирования
 } while (true);
 return rval;
}
//****************************************************************************************
//СТАТУС: Common function # 11
//НАИМЕНОВАНИЕ ФУНКЦИИ:
// bool InputBooleanValue(bool& bval, const char* VarName = NULL, const char* TrueMsg = NULL,
//                        const char* FalseMsg = NULL);
//НАЗНАЧЕНИЕ: Консольный ввод булевого значения с сохранением результата в bval, передаваемой
//по ссылке. Для ввода буленовского значения, которое может принимать всего два значения
//True и False используется целое число. Если пользователь вводит положительное число (d>0),
//то bval присваивается true, иначе false.
//----------------------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. bool& bval - ссылка ссылка на редактируемое булево (логическое) число;
//2. const char* VarName - наименование редактируемой булевой переменной;
//3. const char* TrueMsg - наименование, соответствующее булеву значению True;
//4. const char* FalseMsg - наименование, соответствующее булеву значению False.
//----------------------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: bool bval - введенное булево число
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
 do //Цикл ввода логического значения
 {cout << "Edit < Boolean Value > 1 => True, 0 => False" << endl;
  //Ввод целого числа и его интерпретация как булево значение
  y = wherey();
  cout << "Input <" << sName << ">: "; cin >> ival;
  bval = (ival > 0) ? true : false;
  ival = (bval) ? 1 : 0;
  ClrScr(1,y);//Очистка экрана
  cout << sName << ": " << sBool[ival] << endl;
  //Подтверждение введенного значения
  cout << "Would you like to exit? [y/n]: "; cin >> c;
  ClrScr(1,row_start);//Очистка экрана
  if (c == 'Y' || c == 'y') break;//Выход из цикла редактирования
 } while (true);
 return bval;
}
//****************************************************************************************
//СТАТУС: Common function # 12
//НАИМЕНОВАНИЕ ФУНКЦИИ: ulong InputNonNegateValue(ulong& ival, const char* name = NULL)
//НАЗНАЧЕНИЕ: Консольный ввод неотрицательного целого значения с сохранением результата в
//ival, передаваемой в функцию по ссылке. Функция возвращает результат ввода.
//----------------------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. ulong& ival - ссылка на редактируемое целое число;
//2. const char* name - наименование редактируемого целого числа.
//----------------------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: ulong ival - введенное целое неотрицательное число
//****************************************************************************************
ulong InputNonNegateValue(ulong& ival, const char* name)
{string sName(name);
 uint row_start;
 char c;
 row_start = wherey();
 if (sName.empty()) sName.assign("Integer number");
 do //Цикл ввода неотрицательного целого числа
 {cout << "Input <" << sName << "> [val > 0]: "; cin >> ival;
  if (ival == 0) ival = 10;
  ClrScr(1,row_start);//Очистка экрана
  cout << sName << ": " << ival << endl;
  //Подтверждение введенного значения
  cout << "Would you like to exit? [y/n]: "; cin >> c;
  ClrScr(1,row_start);//Очистка экрана
  if (c == 'Y' || c == 'y') break;//Выход из цикла редактирования
 } while (true);
 return ival;
}
//****************************************************************************************
//СТАТУС: Common function # 13
//НАИМЕНОВАНИЕ ФУНКЦИИ:
// bool StatCalcQadaptDM(TResultStatCalcQadaptDM& Result, const TUnEqLAA_ODA& LAA,
//     const TGrayScaleImgSrc_2D& Signal_2D, const TDoubleSet& ElimDirs,
//     const TJammers_2D& Jams, TCovMatrixInversion* pInvOp, double alpha, ulong n,
//     ulong Test)
//НАЗНАЧЕНИЕ: Расчет показателя качества адаптации пространственного фильтра статистическим
//методом. Весовой вектор фильтра формируется путем обращения регуляризированной оценочной
//ковариационной матрицы входного процесса EstRsjn. Мат. ожидание показателя эффективности
//вычисляется по формуле: Q=10*Log10(Ps/Psjn) путем проведения Test экспериментов и
//усреднением полученных результатов. Показатель эффективности Q адаптивного пространствен-
//ного фильтра зависит от следующих факторов:
//1) Приемной системы (числа вспомогательных каналов фильтра - приемных каналов антенной
//   решетки и их внутреннего шума) => LAA;
//2) Помеховой ситуации - суммарной мощности помехи и числа источников помех => Jams;
//3) Алгоритма обращения регуляризированной ковариационной матрицы EstRsjn => pInvOp;
//4) Значения параметра регуляризации => alpha;
//5) Объема выборки, по которой формируется оценочная матрица EstRsjn => n;
//6) Мощности полезного сигнала и его углового положения => Signal_2D;
//7) Использованием схемы линейного ограничения весового вектора адаптивного фильтра, кото-
//   рая позволяет исключить подавление сигнала с заданного направления. Для этого задается
//   направление, совпадающее с направлением прихода полезного сигнала => ElimDirs.
//В данной функции принята модель детерминированного полезный сигнала и статистических
//(случайных) процессов аддитивной помехи и внутреннего шума. В связи с этим вычисляется
//не точное значение показателя эффективности Q, а его математическое ожидание, что требует
//проведения Test экспериментов. Наряду с мат. ожиданием Q вычисляется и его СКВО.
//Функция проводит проверку на допустимость вычисления показателя адаптации. Для этого
//должны выполняться следующие условия:
//1) Число приемных каналов фильтра Naux >= 1, что требует наличия минимум двух каналов LAA -
//   основного и одного вспомогательного каналов для адаптивного пространственного фильтра;
//2) Мощность полезного сигнала Ps > 0;
//3) Наличии алгоритма обращения оценочной матрицы входного процесса EstRsjn => pInvOp!=NULL;
//4) Объем выборки отличен от нуля n > 0;
//5) Число экспериментов Test отлично от нуля (Test > 0).
//Данные условия не гарантируют обращение EstRsjn. Так, например, при обращении нерегуляри-
//зированной матрицы EstRsjn (классический метод обращения) в случае n < Naux данная матрица
//будет вырожденной. В этом случае, за значение мощности на выходе адаптивного фильтра
//OutPsjn принимают просто мощность на выходе основного канала, что соответствует нулевому
//весовому вектору дополнительных каналов фильтра Waux (ненастроенному фильтру). В остальных
//случаях, при обращении оценочной ковариационной матрицы входного процесса EstRsjn мощность
//на выходе адаптивного фильтра вычисляется по следующей схеме:
//1) Формируем оценочную ковариационную матрицу EstRsjn путем проведения n усреднений
//   ковариационных матриц Rsjn(i), 1 < i <= n. Каждая Rsjn(i) формируется из статистичес-
//   кого входного вектора U(i) = Us+Ujam(i)+Un(i), Ujam(i), Un(i) - статистические входные
//   процессы аддитивного помехи и внутреннего шума размерностью N (по числу каналов LAA),
//   Us - детерминированный процесс размерностью N. Rsjn(i) = U(i)*Tr[U(i)], где
//   Tr[*] - оператор комплексно-сопряженного транспонирования вектора(или матрицы).
//2) По матрице EstRsjn рассчитываем матрицу вспомогательных каналов A и вектор взаимных
//   ковариаций дополнительных и основного каналов фильтра b. Размерности A и b равны числу
//   дополнительных каналов фильтра Naux = N - 1;
//3) Обращаем предварительно регуляризированную матрицу A с учетом одного из выбранных
//   алгоритмов обращения матрицы (схема Лаврентьева, Тихонова, Тихонова n-го порядка или
//   Скачкова) либо без предварительной регуляризации (классический метод). Получаем матрицу
//   InvAa обратную к A. Результат обращения будет зависеть как от выбранного метода обра-
//   щения, так и от значения параметра регуляризации alpha, используемого для данной
//   оценочной матрицы.
//4) Определяем весовой вектор для дополнительных каналов фильтра Waux:
//   - Без схемы линейного ограничения весового вектора: Waux = -InvAa*b;
//   - С учетом схемы весового ограничения: Waux = -InvAa*B*b.
//   Схема линейного ограничения (метод Лагранжа) позволяет исключить подавление полезного
//   сигнала адаптивным пространственным фильтром. Матрица B - оператор линейного ограни-
//   чения: B = I-C*Inv[Tr[C]*InvAa*C]*Tr[C]*InvAa. Размерности всех матриц равна Naux.
//    Inv[*] - оператор обращения матрицы;
//    I - единичная матрица;
//    C - матрица (Naux x K) фазовых набегов между дополнительными и основным каналами
//    фильтра для заданных направлений исключения (K - число направлений исключения).
//5) Формируем расширенный весовой вектор (с учетом веса для основного канала равный 1):
//   ExtW = {Waux 1}.
//6) Рассчитываем мощность на выходе: Psjn = Tr[ExtW]*EstRsjn*ExtW.
//Затем по значениям Ps и OutPsjn вычисляется показатель эффективности адаптивной системы.
//Для формирования оценочного значения Q (мат. ожидания Q) выполняется Test экспериментов.
//Функция возвращает true в случае успешного вычисления Q (даже в случае вырожденности A)
//и false - в противном случае.
//----------------------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. TResultStatCalcQadaptDM& Result - данные с результатами работы функции;
//2. TUnEqLAA_ODA& LAA - линейная эквидистантная однородная решетка с всенаправленными
//   антенными элементами (приемник входного сигнала);
//3. TGrayScaleImgSrc_2D& Signal_2D - источник двумерного сигнала (изображения);
//4. TDoubleSet& ElimDirs - направления исключения (используются для линейного ограничения
//   методом Лагранжа весового вектора Waux с целью исключения подавления сигнала с заданных
//   направлений;
//5. TJammers_2D& Jams - аддитивная помеха;
//6. TCovMatrixInversion* pInvOp - оператор обращения ковариационной матрицы входного про-
//   цесса с возможностью ее предварительной регуляризации по заданным алгоритмам с целью
//   компенсации влияния внутрисистемных ошибок на качество реставрации изображения, при-
//   нимаемого на фоне аддитивной помехи.
//7. double alpha - параметр регуляризации;
//8. ulong n - значение объема выборки;
//9. ulong Test - число статистических опытов для получения точечных оценок показателя
//   качества адаптации (мат. ожидания и СКВО).
//----------------------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: bool lSuccess - результат успешного выполнения функции
//****************************************************************************************
bool StatCalcQadaptDM(TResultStatCalcQadaptDM& Result, const TUnEqLAA_ODA& LAA,
     const TGrayScaleImgSrc_2D& Signal_2D, const TDoubleSet& ElimDirs,
     const TJammers_2D& Jams, TCovMatrixInversion* pInvOp, double alpha, ulong n,
     ulong Test)
{//Рабочие переменные
 string ErrMsg;
 bool lUseLCC;//Использование линейных ограничений весового вектора Waux
 ulong N = LAA.GetDimLAA();//Количество каналов LAA - размерность адаптивного фильтра
 ulong Naux = (N > 0) ? N-1 : 0;//Количество вспомогательных каналов фильтра
 rvector Series_Q;//Выборка значений показателей эффективности фильтра объемом Test
 //Датчики случайных чисел для моделирования гауссовского внутреннего шума приемных каналов
 //LAA и аддитивной помехи, принимаемой фильтром
 TRandGen_64 RandNoise;
 TRandGen_64 RandJams;

 cvector Un; //Статистический вектор внутреннего шума на входе фильтра
 cvector Uj; //Статистический вектор аддитивной помехи на входе фильтра
 cvector Us; //Детерминированный вектор полезного сигнала на входе фильтра
 cvector U;  //Суммарный вектор U = Us + Uj + Un

 cmatrix EstRsjn;//Оценочная ковариационная матрица входного процесса за n наблюдений
 cmatrix R;    //Ковариационная матрица входного процесса за 1 наблюдение
 cmatrix A;    //Ковариационная матрица вспомогательных каналов адаптивного фильтра
 cmatrix InvAa;//Обратная матрица к регуляризированной матрице A
 cvector b;    //Вектор взаимных ковариаций основного и вспомогательных каналов
 cvector Waux; //Весовой вектор, рассчитанный для EstAa: Waux = -InvAa*B*b
 cvector ExtW;//Расширенный весовой вектор ExtW = {Waux 1}

 double Ps = Signal_2D.GetPower();//Мощность полезного сигнала
 double Psjn;//Мощность на выходе адаптивной системы (сигнал-помеха-шум)
 double q;//Показатель эффективности: q = 10*Log10(Ps/Psjn)
 complex z;
 //Объявление матриц для задания линейных ограничений на формируемый весовой вектор
 //с целью исключения подавления сигналов с заданных направлений адаптивным фильтром
 cmatrix B; //B = I - C*Inv(trC*InvAa*C)*trC*InvAa - линейные ограничения Wa
 //Матрица фазовых набегов между дополнительными и основным каналами фильтра для заданных
 //направлений исключения размерности (NxK), N - число дополнительных каналов фильтра,
 //K - число направлений исключения (как правило, одно - в направлении на полезный сигнал)
 cmatrix C;

 //---------------------------------------------------------------------------------------
 //Проверка на возможность проведения расчетов и начальная инициализация структуры
 //выходных данных
 //---------------------------------------------------------------------------------------
 Result.Q = -100.0;//Установка значения показателя эффективности адаптивной системы
 Result.MSD_Q = 0.0;//Установка значения СКВО для Q
 Result.InvErr = 0L;//Нач-я установка счетчика неудачных обращений матрицы A
 Result.Msg.clear();//Очистка строки сообщений

 ErrMsg.clear();
 if (Naux < 1) ErrMsg.append("LAA Channels are less than 2;\n");
 if (Ps == 0) ErrMsg.append("Signal is bad: Ps == 0;\n");
 if (pInvOp == NULL) ErrMsg.append("Inversion Operator is absent;\n");
 if (n == 0) ErrMsg.append("Sample size must be greater than 0;\n");
 if (Test == 0) ErrMsg.append("Number of Test must be greater than 0;\n");

 if (!ErrMsg.empty()) //Вычисление показателя эффективности невозможно
 {Result.Msg.assign("Statistical Calculating of the Qadapt is impossible:\n");
  Result.Msg.append(ErrMsg);
  return false;
 }

 //======================================================================================
 //Подготовка к моделированию
 //======================================================================================
 //--------------------------------------------------------------------------------------
 //Расчет матрицы C, используемой для формирования матрицы линейных ограничений B весового
 //вектора по алгоритму Лагранжа при наличии направлений исключения
 //--------------------------------------------------------------------------------------
 lUseLCC = LAA.TFunc(C,ElimDirs,false);//false - без фазовых набегов в основном канале LAA

 //--------------------------------------------------------------------------------------
 //Формирование матриц EstRsjn, A, векторов b и ExtW. Начальная установка вектора ExtWa.
 //Вычисление входного вектора полезного сигнала Us (детерминированный процесс).
 //Формирование Series_Q - вектора значений q, вычисляемых за Test испытаний
 //--------------------------------------------------------------------------------------
 //Входная ковариационная матрица дополнительных каналов
 A.Set(Naux,false);
 //Вектор взаимных ковариаций ОСНОВНОГО и ДОПОЛНИТЕЛЬНЫХ каналов фильтра
 b.Set(Naux,COL,false);
 //Начальная установка весового вектора ExtW
 ExtW.Set(N,COL,C_ZERO);
 ExtW.Put(Naux,C_ONE);
 //Начальная инициализация оценочной ковариационной матрицы EstRsjn
 EstRsjn.Set(N,N,false);
 //Расчет вектора Us
 CalcSignal(Us,LAA,Ps,Signal_2D.GetAngle(DEG),true);
 //Формирование вектора значений показателя качества адаптации, рассчитываемых за Test
 //испытаний
 Series_Q.Set(Test,COL,false);

 //--------------------------------------------------------------------------------------
 // Вычисление статистическим методом Qadapt путем проведения Test экспериментов
 //--------------------------------------------------------------------------------------
 //-------------------------------------------------------------------------------------
 //Цикл № 1 - по числу экспериментов Test
 //-------------------------------------------------------------------------------------
 for (ulong t = 0; t < Test; t++)
 {//Начальная инициализация матрицы EstRjn
  for (ulong i = 0; i < N; i++)
   for (ulong j = 0; j < N; j++) EstRsjn.Put(i,j,C_ZERO);
  //------------------------------------------------------------------------------------
  //Цикл № 2 - расчет EstRjn за период наблюдения n
  //------------------------------------------------------------------------------------
  for (ulong k = 0; k < n; k++)
  {//Шаг 1. Формирование статистического вектора входного процесса (аддитивная помеха +
   //внутренний шум приемных каналов адаптивного фильтра)
   CalcJams(Uj,LAA,Jams,RandJams,true);
   CalcNoise(Un,LAA,RandNoise,true);
   U = Us+Uj+Un;
   //Шаг 2. Формирование ковариационной матрицы входного процесса Rjn(n) по вектору Ujn
   CorrelationMatrix(U,R);
   //Шаг 3. Формирование матрицы EstRjn
   for (ulong i = 0; i < N; i++)
    for (ulong j = 0; j < N; j++) EstRsjn.Put(i,j,EstRsjn.Get(i,j)+R.Get(i,j)/n);
  }//end цикла № 2 - по числу наблюдений n

  //Рассчитываем ковариационную матрицу вспомогательных каналов A и вектор взаимных
  //ковариаций b между основным и вспомогательными каналами фильтра по матрице EstRsjn
  for (ulong i = 0L; i < Naux; i++)
  {b.Put(i,EstRsjn.Get(i,Naux));
   for (ulong j = 0L; j < Naux; j++) A.Put(i,j,EstRsjn.Get(i,j));
  }

  //Обращение матрицы A c ее предварительной регуляризацией по выбранному алгоритму
  if (pInvOp->Inverse(InvAa,A,alpha) == false) //Матрица A - вырожденная
  {Result.InvErr++;//Подсчет неудачных обращений
   //Мощность на выходе адаптивной системы в случае вырожденной матрицы A - мощность
   //на выходе ОСНОВНОГО канала
   Psjn = abs(EstRsjn.Get(Naux,Naux));
  }
  else //Определяем весовой вектор адаптивной системы
  {//Расчет B = I - C*Inv[trC*InvAa*C]*trC*InvAa
   if (lUseLCC) LagrangeMethod(B,InvAa,C);
   //Вычисление вектора Wa = -InvAa*B*b
   if (lUseLCC) Waux = InvAa*(B*b);//-1.0*(InvAa*(B*b))
   else Waux = InvAa*b;
   //Формирование вектора ExtW
   for (ulong i = 0; i < Waux.Size(); i++) ExtW.Put(i,-1.0*Waux.Get(i));
   //Вычисление мощности на выходе фильтра: Psjn = abs(TrExtWa*Rsjn*ExtWa)
   QuadraticForm(z,EstRsjn,ExtW);
   Psjn = abs(z);
  }
  //Рассчитываем показатель эффективности адаптивной системы q
  q = 10*log10(Ps/Psjn);
  Series_Q.Put(t,q);
 }//End цикла № 1 - по числу экспериментов Test

 //---------------------------------------------------------------------------------------
 //Статистический анализ полученных результатов: расчет МО и СКВО значений q, полученных
 //по Test экспериментам
 //---------------------------------------------------------------------------------------
 StatAnalysis(Series_Q,Result.Q,Result.MSD_Q);
 return true;
}
