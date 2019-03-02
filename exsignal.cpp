//exsignal.cpp
//Реализация КЛАССА ВНЕШНИЙ СИГНАЛ -- ExtSignal
//Initial date: August,18,1998. Final date:
//Copyright (c) JohnSoft 1998. All rights reserved

//*****************************************************************************
//СТАТУС:
//НАИМЕНОВАНИЕ ФУНКЦИИ:
//НАЗНАЧЕНИЕ:
//Функция предназначена для ...
//
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. -
//2. -
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
//
//*****************************************************************************

#include "exsignal.h"

//Объявление таблицы возможных законов изменения азимутального угла
//Azimuth Angle и угла места Elevate Angle источника составляющей внешнего
//сигнала относительно нормали к раскрыву антенны основного канала антенной
//решетки, а также амплитуды и фазы (горизонтальной и вертикальной составляющих)
//угла поляризации каждой компоненты внешнего сигнала.
LAW* TableAngleLaw = NULL;
uint TableAngleLawCount = 0;

//Объявление таблицы возможных детерминированных законов изменения амплитуды
//составляющей внешнего сигнала
LAW* TableAmplLaw = NULL;
uint TableAmplLawCount = 0;

//Объявление таблицы возможных случайных законов изменения АМПЛИТУДНОЙ и ФАЗОВОЙ
//характеристики компоненты внешнего сигнала
LAW* TableCasualLaw = NULL;
uint TableCasualLawCount = 0;

//Объявление таблицы наименования характеристик компонент внешнего сигнала
const char* SignalPropertyName[] =
   //0    1. Type        2. Usage         3. Azimuth Angle   4. Elevate Angle
{"Noname","SIGNAL TYPE", "SIGNAL USAGE,", "AZIMUTH ANGLE",   "ELEVATE ANGLE",
  //5. Polar. Angle    6. Phase of vert. comp. polar.
 "POLARIZATION ANGLE", "PHASE OF VERTCOMP POLARIZATION",
 //7. Phase of hor. polar.        8. Ampitude   9. Phase  10. Casual amplitude
 "PHASE OF HORCOMP POLARIZATION", "AMPLITUDE",  "PHASE",  "CASUAL AMPLITUDE",
 //11. Casual phase
 "CASUAL PHASE" };


//*****************************************************************************
// ОПИСАНИЕ ФУНКЦИЙ СОЗДАНИЯ ТАБЛИЦ С ВОЗМОЖНЫМИ ЗАКОНАМИ ИЗМЕНЕНИЯ
// ХАРАКТЕРИСТИК СОСТАВЛЯЮЩИХ ВНЕШНЕГО СИГНАЛА ВО ВРЕМЕНИ
//*****************************************************************************

//Сервисные функции
//*****************************************************************************
//СТАТУС: II.1., private, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ: uint AccessibleLaw(const LAW* Table, uint Count)
//НАЗНАЧЕНИЕ: ПОДСЧЕТ ЧИСЛА ДОСТУПНЫХ К ВЫБОРУ ЗАКОНОВ В ТАБЛИЦЕ ЗАКОНОВ Table
//Функция возвращает число доступных для выбора законов. Каждый закон в таблице
//имеет флаг turn, который определяет доступность закона для выбора его пользо-
//вателем. Каждый закон с turn = ON увеличивает кол-во доступных для выбора за-
//нов, число которых не может превысить Count.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. LAW* Table - таблица законов
//2. uint Count - общее количество законов в таблице
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
//uint Total - кол-во доступных для выбора законов в таблице Table
//*****************************************************************************
uint ExtSignal::AccessibleLaw(const LAW* Table, uint Count)
{ uint Total = 0;
  for (uint i = 0; i < Count; i++)
   if (Table[i].turn == ON) Total++;
return Total;
}
//*****************************************************************************
//СТАТУС: II.2., private, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ: bool SetLaw(LAW* Table, uint Count, uint LawType)
//НАЗНАЧЕНИЕ: РАЗРЕШЕНИЕ ВЫБОРА ЗАКОНА LawType В ТАБЛИЦЕ ЗАКОНОВ Table
//Функция устанавливает флаг turn = ON для закона с идентификатором LawType.
//Если закона типа LawType в таблице обнаружено не будет, то функция возвращает
//false, иначе - true.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. LAW* Table - таблица законов
//2. uint Count - общее количество законов в таблице
//3. uint LawType - идентификатор закона выбор которого будет разрешен
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
//bool Success - true - закон найден в таблице и выбор его разрешен
//               false - закон не найден в таблице
//*****************************************************************************
bool ExtSignal::SetLaw(LAW* Table, uint Count, uint LawType)
{ bool lSuccess = false;
  for (uint i = 0; i < Count; i++)
   if (Table[i].type == LawType)
   { Table[i].turn = ON;
     lSuccess = true;
     break; //Выход из цикла
   }
return lSuccess;
}
//*****************************************************************************
//СТАТУС: II.3., private, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ: bool ResetLaw(LAW* Table, uint Count, uint LawType)
//НАЗНАЧЕНИЕ: ЗАПРЕЩЕНИЕ ВЫБОРА ЗАКОНА LawType В ТАБЛИЦЕ ЗАКОНОВ Table
//Функция устанавливает флаг turn = OFF для закона с идентификатором LawType.
//Если закона типа LawType в таблице обнаружено не будет, то функция возвращает
//false, иначе - true.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. LAW* Table - таблица законов
//2. uint Count - общее количество законов в таблице
//3. uint LawType - идентификатор закона выбор которого будет запрещен
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
//bool Success - true - закон найден в таблице и выбор его запрещен
//               false - закон не найден в таблице
//*****************************************************************************
bool ExtSignal::ResetLaw(LAW* Table, uint Count, uint LawType)
{ bool lSuccess = false;
  for (uint i = 0; i < Count; i++)
   if (Table[i].type == LawType)
   { Table[i].turn = OFF;
     lSuccess = true;
     break; //Выход из цикла
   }
return lSuccess;
}
//*****************************************************************************
//СТАТУС: II.4., private, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ: bool SetAllLaw(LAW* Table, uint Count)
//НАЗНАЧЕНИЕ: РАЗРЕШЕНИЕ ВЫБОРА ВСЕХ ЗАКОНОВ В ТАБЛИЦЕ ЗАКОНОВ Table
//Функция устанавливает флаг turn = ON для всех законов.
//Если таблица пуста (Count == 0), то функция возвращает false, иначе - true.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. LAW* Table - таблица законов
//2. uint Count - общее количество законов в таблице
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
//bool Success - true - выбор всех законов разрешен
//               false - таблица законов не содержит (таблица пуста)
//*****************************************************************************
bool ExtSignal::SetAllLaw(LAW* Table, uint Count)
{ bool lSuccess = true;
 try
 {
  if (Count == 0) throw EMPTY_TABLE;
  for (uint i = 0; i < Count; i++) Table[i].turn = ON;
 }
 catch (int error_type)
 {
  if (error_type == EMPTY_TABLE)
  { lSuccess = false;
    #if defined(CONSOLE)
      cout << "\nLAWS TABLE IS EMPTY !!!" << endl;
      getch();
    #endif
    exit(-1);
  }
 }
return lSuccess;
}
//*****************************************************************************
//СТАТУС: II.5., private, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ: bool ResetAllLaw(LAW* Table, uint Count)
//НАЗНАЧЕНИЕ: ЗАПРЕЩЕНИЕ ВЫБОРА ВСЕХ ЗАКОНОВ В ТАБЛИЦЕ ЗАКОНОВ Table
//Функция устанавливает флаг turn = OFF для всех законов.
//Если таблица пуста (Count == 0), то функция возвращает false, иначе - true.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. LAW* Table - таблица законов
//2. uint Count - общее количество законов в таблице
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
//bool Success - true - выбор всех законов разрешен
//               false - таблица законов не содержит (таблица пуста)
//*****************************************************************************
bool ExtSignal::ResetAllLaw(LAW* Table, uint Count)
{ bool lSuccess = true;
 try
 {
  if (Count == 0) throw EMPTY_TABLE;
  for (uint i = 0; i < Count; i++) Table[i].turn = OFF;
 }
 catch (int error_type)
 {
  if (error_type == EMPTY_TABLE)
  { lSuccess = false;
    #if defined(CONSOLE)
      cout << "\nLAWS TABLE IS EMPTY !!!" << endl;
      getch();
    #endif
    exit(-1);
  }
 }
return lSuccess;
}
//*****************************************************************************
//СТАТУС: II.6., private, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ: uint GetIndexLaw(LAW* Table, uint Count, uint Select)
//НАЗНАЧЕНИЕ:
//Функция возвращает индекс по которому расположен в таблице законов, выбранный
//к использованию закон. Работа этой функции необходима в том случае, если в
//таблице имеются запрещенные к выбору законы.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. LAW* Table - таблица законов
//2. uint Count - количество законов в таблице
//3. uint Select - выбранный законов в таблице
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
//uint Index - индекс выбранного пользователем закона в таблице законов
//*****************************************************************************
uint ExtSignal::GetIndexLaw(const LAW* Table, uint Count, uint Select)
{uint Index = 0;
 try
 {
  if (Count == 0) throw EMPTY_TABLE; //Таблица не содержит законов

  for (uint i = 0; i < Count; i++)
   if (Table[i].turn == ON)
    if (Select == Index) {Index = i; break;}
    else Index++;
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
 }
return Index;
}
//*****************************************************************************
//СТАТУС: II.7., private, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ:
// uint GetIndexLawByType(LAW* Table, uint Count, uint Type)
//НАЗНАЧЕНИЕ:
//Функция возвращает индекс расположения закона в таблице законов по его типу.
//Функция используется при отображении свойств компоненты сигнала, считывании
//данных по аддитивному внешнему сигналу (полезный сигнал + помехи) из файла и
//их размещении в оперативной памяти в структуре COMPONENT.
//Если закон такого типа не содержится в таблице описания законов Table, то
//программа завершает свою работу.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. LAW* Table - таблица законов
//2. uint Count - количество законов в таблице
//3. uint Type - идентификатор закона, который требуется найти в таблице
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
//uint Index - индекс закона в таблице законов, соответствующего Type
//*****************************************************************************
uint ExtSignal::GetIndexLawByType(const LAW* Table, uint Count, uint Type)
{uint Index = 0;
 bool lSuccess = false;
 try
 {
  if (Count == 0) throw EMPTY_TABLE; //Таблица не содержит законов
  if (Table == NULL) throw EMPTY_TABLE; //Таблица не содержит законов

  for (uint i = 0; i < Count; i++)
   if (Table[i].type == Type) //Закон с идент. Type найден в таблице законов
   {Index = i; lSuccess = true;  break;}

  if (lSuccess == false) throw ABSENT_LAW; //Закон типа Type не найден
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
//СТАТУС: II.7., private, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ: void CreateTableOfAngleLaw()
//НАЗНАЧЕНИЕ:
//Создание таблицы законов изменения во времени азимутального угла, угла
//места источника, а также амплитуды и фазы (горизонтальной и вертикальной
//составляющих) угла поляризации каждой компоненты внешнего сигнала.
//Для работы функция использует глобальный указатель TableAngleLaw и
//TableAngleLawCount
//СПИСОК ФУНКЦИЙ (по порядку, общее количество - 5):
//1. ConstLaw; 2. LinearLaw; 3. SquareLaw; 4. CosLaw; 5. SinLaw; 6. ExpLaw
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ: нет
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:  нет
//*****************************************************************************
void ExtSignal::CreateTableOfAngleLaw()
{
 TableAngleLawCount = 6;
 TableAngleLaw = new LAW[6];
 //Первый закон - F(t) = A
 TableAngleLaw[0].turn = ON;
 TableAngleLaw[0].law = ConstLaw;
 TableAngleLaw[0].type = CONST_LAW;
 TableAngleLaw[0].inputfactor = InitFactors;
 TableAngleLaw[0].mathlawfactor = GetMathNotateConstLaw;
 TableAngleLaw[0].M.DimFunc = RADIAN;
 TableAngleLaw[0].M.Size = 2;
 TableAngleLaw[0].M.Factor = new FACTOR[2];
 //t
 TableAngleLaw[0].M.Factor[0].SourceDim = SECOND;
 TableAngleLaw[0].M.Factor[0].TargetDim = SECOND;
 TableAngleLaw[0].M.Factor[0].Name = new char[2];
 strcpy(TableAngleLaw[0].M.Factor[0].Name,"t");
 TableAngleLaw[0].M.Factor[0].Convert = Copy;
 TableAngleLaw[0].M.Factor[0].Invert = Copy;
 TableAngleLaw[0].M.Factor[0].PreBlock = AlwaysTrue;
 TableAngleLaw[0].M.Factor[0].PostBlock = AlwaysTrue;
 //A
 TableAngleLaw[0].M.Factor[1].SourceDim = DEGREE;
 TableAngleLaw[0].M.Factor[1].TargetDim = RADIAN;
 TableAngleLaw[0].M.Factor[1].Name = new char[9];
 strcpy(TableAngleLaw[0].M.Factor[1].Name,"Factor A");
 TableAngleLaw[0].M.Factor[1].Convert = DegToRad;
 TableAngleLaw[0].M.Factor[1].Invert = RadToDeg;
 TableAngleLaw[0].M.Factor[1].PreBlock = AlwaysTrue;
 TableAngleLaw[0].M.Factor[1].PostBlock = AlwaysTrue;

 //Второй закон - Линейный закон F(t) = A*t+B
 TableAngleLaw[1].turn = ON;
 TableAngleLaw[1].law = LinearLaw;
 TableAngleLaw[1].type = LINEAR_LAW;
 TableAngleLaw[1].inputfactor = InitFactors;
 TableAngleLaw[1].mathlawfactor = GetMathNotateLinearLaw;
 TableAngleLaw[1].M.DimFunc = RADIAN;
 TableAngleLaw[1].M.Size = 3;
 TableAngleLaw[1].M.Factor = new FACTOR[3];

 //t {SECOND,SECOND,"t",Copy,Copy,AlwaysTrue,AlwaysTrue}
 TableAngleLaw[1].M.Factor[0].SourceDim = SECOND;
 TableAngleLaw[1].M.Factor[0].TargetDim = SECOND;
 TableAngleLaw[1].M.Factor[0].Name = new char[2];
 strcpy(TableAngleLaw[1].M.Factor[0].Name,"t");
 TableAngleLaw[1].M.Factor[0].Convert = Copy;
 TableAngleLaw[1].M.Factor[0].Invert = Copy;
 TableAngleLaw[1].M.Factor[0].PreBlock = AlwaysTrue;
 TableAngleLaw[1].M.Factor[0].PostBlock = AlwaysTrue;

 //A {DEG_SEC,RAD_SEC,"Factor A",DegToRad,RadToDeg,AlwaysTrue,AlwaysTrue}
 TableAngleLaw[1].M.Factor[1].SourceDim = DEG_SEC;
 TableAngleLaw[1].M.Factor[1].TargetDim = RAD_SEC;
 TableAngleLaw[1].M.Factor[1].Name = new char[9];
 strcpy(TableAngleLaw[1].M.Factor[1].Name,"Factor A");
 TableAngleLaw[1].M.Factor[1].Convert = DegToRad;
 TableAngleLaw[1].M.Factor[1].Invert = RadToDeg;
 TableAngleLaw[1].M.Factor[1].PreBlock = AlwaysTrue;
 TableAngleLaw[1].M.Factor[1].PostBlock = AlwaysTrue;

 //B {DEGREE,RADIAN,"Factor B",DegToRad,RadToDeg,AlwaysTrue,AlwaysTrue}
 TableAngleLaw[1].M.Factor[2].SourceDim = DEGREE;
 TableAngleLaw[1].M.Factor[2].TargetDim = RADIAN;
 TableAngleLaw[1].M.Factor[2].Name = new char[9];
 strcpy(TableAngleLaw[1].M.Factor[2].Name,"Factor B");
 TableAngleLaw[1].M.Factor[2].Convert = DegToRad;
 TableAngleLaw[1].M.Factor[2].Invert = RadToDeg;
 TableAngleLaw[1].M.Factor[2].PreBlock = AlwaysTrue;
 TableAngleLaw[1].M.Factor[2].PostBlock = AlwaysTrue;

 //Третий закон - Квадратичный закон F(t) = A*t^2+B*t+C
 TableAngleLaw[2].turn = ON;
 TableAngleLaw[2].law = SquareLaw;
 TableAngleLaw[2].type = SQUARE_LAW;
 TableAngleLaw[2].inputfactor = InitFactors;
 TableAngleLaw[2].mathlawfactor = GetMathNotateSquareLaw;
 TableAngleLaw[2].M.DimFunc = RADIAN;
 TableAngleLaw[2].M.Size = 4;
 TableAngleLaw[2].M.Factor = new FACTOR[4];

 //t {SECOND,SECOND,"t",Copy,Copy,AlwaysTrue,AlwaysTrue}
 TableAngleLaw[2].M.Factor[0].SourceDim = SECOND;
 TableAngleLaw[2].M.Factor[0].TargetDim = SECOND;
 TableAngleLaw[2].M.Factor[0].Name = new char[2];
 strcpy(TableAngleLaw[2].M.Factor[0].Name,"t");
 TableAngleLaw[2].M.Factor[0].Convert = Copy;
 TableAngleLaw[2].M.Factor[0].Invert = Copy;
 TableAngleLaw[2].M.Factor[0].PreBlock = AlwaysTrue;
 TableAngleLaw[2].M.Factor[0].PostBlock = AlwaysTrue;

 //A {DEG_SEC2,RAD_SEC2,"Factor A",DegToRad,RadToDeg,AlwaysTrue,AlwaysTrue}
 TableAngleLaw[2].M.Factor[1].SourceDim = DEG_SEC2;
 TableAngleLaw[2].M.Factor[1].TargetDim = RAD_SEC2;
 TableAngleLaw[2].M.Factor[1].Name = new char[9];
 strcpy(TableAngleLaw[2].M.Factor[1].Name,"Factor A");
 TableAngleLaw[2].M.Factor[1].Convert = DegToRad;
 TableAngleLaw[2].M.Factor[1].Invert = RadToDeg;
 TableAngleLaw[2].M.Factor[1].PreBlock = AlwaysTrue;
 TableAngleLaw[2].M.Factor[1].PostBlock = AlwaysTrue;

 //B {DEG_SEC,RAD_SEC,"Factor B",DegToRad,RadToDeg,AlwaysTrue,AlwaysTrue}
 TableAngleLaw[2].M.Factor[2].SourceDim = DEG_SEC;
 TableAngleLaw[2].M.Factor[2].TargetDim = RAD_SEC;
 TableAngleLaw[2].M.Factor[2].Name = new char[9];
 strcpy(TableAngleLaw[2].M.Factor[2].Name,"Factor B");
 TableAngleLaw[2].M.Factor[2].Convert = DegToRad;
 TableAngleLaw[2].M.Factor[2].Invert = RadToDeg;
 TableAngleLaw[2].M.Factor[2].PreBlock = AlwaysTrue;
 TableAngleLaw[2].M.Factor[2].PostBlock = AlwaysTrue;

 //C {DEGREE,RADIAN,"Factor C",DegToRad,RadToDeg,AlwaysTrue,AlwaysTrue}
 TableAngleLaw[2].M.Factor[3].SourceDim = DEGREE;
 TableAngleLaw[2].M.Factor[3].TargetDim = RADIAN;
 TableAngleLaw[2].M.Factor[3].Name = new char[9];
 strcpy(TableAngleLaw[2].M.Factor[3].Name,"Factor C");
 TableAngleLaw[2].M.Factor[3].Convert = DegToRad;
 TableAngleLaw[2].M.Factor[3].Invert = RadToDeg;
 TableAngleLaw[2].M.Factor[3].PreBlock = AlwaysTrue;
 TableAngleLaw[2].M.Factor[3].PostBlock = AlwaysTrue;

 //Четвертый закон - Косинусоидальный закон F(t) = A*COS(B*t+C)
 TableAngleLaw[3].turn = ON;
 TableAngleLaw[3].law = CosLaw;
 TableAngleLaw[3].type = COS_LAW;
 TableAngleLaw[3].inputfactor = InitFactors;
 TableAngleLaw[3].mathlawfactor = GetMathNotateCosLaw;
 TableAngleLaw[3].M.DimFunc = RADIAN;
 TableAngleLaw[3].M.Size = 4;
 TableAngleLaw[3].M.Factor = new FACTOR[4];

 //t {SECOND,SECOND,"t",Copy,Copy,AlwaysTrue,AlwaysTrue}
 TableAngleLaw[3].M.Factor[0].SourceDim = SECOND;
 TableAngleLaw[3].M.Factor[0].TargetDim = SECOND;
 TableAngleLaw[3].M.Factor[0].Name = new char[2];
 strcpy(TableAngleLaw[3].M.Factor[0].Name,"t");
 TableAngleLaw[3].M.Factor[0].Convert = Copy;
 TableAngleLaw[3].M.Factor[0].Invert = Copy;
 TableAngleLaw[3].M.Factor[0].PreBlock = AlwaysTrue;
 TableAngleLaw[3].M.Factor[0].PostBlock = AlwaysTrue;

 //A {DEGREE,RADIAN,"Amplitude A",DegToRad,RadToDeg,AlwaysTrue,AlwaysTrue}
 TableAngleLaw[3].M.Factor[1].SourceDim = DEGREE;
 TableAngleLaw[3].M.Factor[1].TargetDim = RADIAN;
 TableAngleLaw[3].M.Factor[1].Name = new char[12];
 strcpy(TableAngleLaw[3].M.Factor[1].Name,"Amplitude A");
 TableAngleLaw[3].M.Factor[1].Convert = DegToRad;
 TableAngleLaw[3].M.Factor[1].Invert = RadToDeg;
 TableAngleLaw[3].M.Factor[1].PreBlock = AlwaysTrue;
 TableAngleLaw[3].M.Factor[1].PostBlock = AlwaysTrue;

 //B {HZ,PULSAT,"Frequence B",FreqToPulsatance,PulsatanceToFreq,AlwaysTrue,
 //   AlwaysTrue},
 TableAngleLaw[3].M.Factor[2].SourceDim = HZ;
 TableAngleLaw[3].M.Factor[2].TargetDim = PULSAT;
 TableAngleLaw[3].M.Factor[2].Name = new char[12];
 strcpy(TableAngleLaw[3].M.Factor[2].Name,"Frequence B");
 TableAngleLaw[3].M.Factor[2].Convert = FreqToPulsatance;
 TableAngleLaw[3].M.Factor[2].Invert = PulsatanceToFreq;
 TableAngleLaw[3].M.Factor[2].PreBlock = AlwaysTrue;
 TableAngleLaw[3].M.Factor[2].PostBlock = AlwaysTrue;

 //C {DEGREE,RADIAN,"Epoch Angle C",DegToRad,RadToDeg,AlwaysTrue,AlwaysTrue}
 TableAngleLaw[3].M.Factor[3].SourceDim = DEGREE;
 TableAngleLaw[3].M.Factor[3].TargetDim = RADIAN;
 TableAngleLaw[3].M.Factor[3].Name = new char[14];
 strcpy(TableAngleLaw[3].M.Factor[3].Name,"Epoch Angle C");
 TableAngleLaw[3].M.Factor[3].Convert = DegToRad;
 TableAngleLaw[3].M.Factor[3].Invert = RadToDeg;
 TableAngleLaw[3].M.Factor[3].PreBlock = AlwaysTrue;
 TableAngleLaw[3].M.Factor[3].PostBlock = AlwaysTrue;

 //Пятый закон - Синусоидальный закон F(t) = A*SIN(B*t+C)
 TableAngleLaw[4].turn = ON;
 TableAngleLaw[4].law = SinLaw;
 TableAngleLaw[4].type = SIN_LAW;
 TableAngleLaw[4].inputfactor = InitFactors;
 TableAngleLaw[4].mathlawfactor = GetMathNotateSinLaw;
 TableAngleLaw[4].M.DimFunc = RADIAN;
 TableAngleLaw[4].M.Size = 4;
 TableAngleLaw[4].M.Factor = new FACTOR[4];

 //t {SECOND,SECOND,"t",Copy,Copy,AlwaysTrue,AlwaysTrue}
 TableAngleLaw[4].M.Factor[0].SourceDim = SECOND;
 TableAngleLaw[4].M.Factor[0].TargetDim = SECOND;
 TableAngleLaw[4].M.Factor[0].Name = new char[2];
 strcpy(TableAngleLaw[4].M.Factor[0].Name,"t");
 TableAngleLaw[4].M.Factor[0].Convert = Copy;
 TableAngleLaw[4].M.Factor[0].Invert = Copy;
 TableAngleLaw[4].M.Factor[0].PreBlock = AlwaysTrue;
 TableAngleLaw[4].M.Factor[0].PostBlock = AlwaysTrue;

 //A {DEGREE,RADIAN,"Amplitude A",DegToRad,RadToDeg,AlwaysTrue,AlwaysTrue}
 TableAngleLaw[4].M.Factor[1].SourceDim = DEGREE;
 TableAngleLaw[4].M.Factor[1].TargetDim = RADIAN;
 TableAngleLaw[4].M.Factor[1].Name = new char[12];
 strcpy(TableAngleLaw[4].M.Factor[1].Name,"Amplitude A");
 TableAngleLaw[4].M.Factor[1].Convert = DegToRad;
 TableAngleLaw[4].M.Factor[1].Invert = RadToDeg;
 TableAngleLaw[4].M.Factor[1].PreBlock = AlwaysTrue;
 TableAngleLaw[4].M.Factor[1].PostBlock = AlwaysTrue;

 //B {HZ,PULSAT,"Frequence B",FreqToPulsatance,PulsatanceToFreq,AlwaysTrue,
 //   AlwaysTrue},
 TableAngleLaw[4].M.Factor[2].SourceDim = HZ;
 TableAngleLaw[4].M.Factor[2].TargetDim = PULSAT;
 TableAngleLaw[4].M.Factor[2].Name = new char[12];
 strcpy(TableAngleLaw[4].M.Factor[2].Name,"Frequence B");
 TableAngleLaw[4].M.Factor[2].Convert = FreqToPulsatance;
 TableAngleLaw[4].M.Factor[2].Invert = PulsatanceToFreq;
 TableAngleLaw[4].M.Factor[2].PreBlock = AlwaysTrue;
 TableAngleLaw[4].M.Factor[2].PostBlock = AlwaysTrue;

 //C {DEGREE,RADIAN,"Epoch Angle C",DegToRad,RadToDeg,AlwaysTrue,AlwaysTrue}
 TableAngleLaw[4].M.Factor[3].SourceDim = DEGREE;
 TableAngleLaw[4].M.Factor[3].TargetDim = RADIAN;
 TableAngleLaw[4].M.Factor[3].Name = new char[14];
 strcpy(TableAngleLaw[4].M.Factor[3].Name,"Epoch Angle C");
 TableAngleLaw[4].M.Factor[3].Convert = DegToRad;
 TableAngleLaw[4].M.Factor[3].Invert = RadToDeg;
 TableAngleLaw[4].M.Factor[3].PreBlock = AlwaysTrue;
 TableAngleLaw[4].M.Factor[3].PostBlock = AlwaysTrue;

 //Шестой закон - Экспоненциальный закон F(t) = A*EXP(B*t)
 TableAngleLaw[5].turn = ON;
 TableAngleLaw[5].law = ExpLaw;
 TableAngleLaw[5].type = EXP_LAW;
 TableAngleLaw[5].inputfactor = InitFactors;
 TableAngleLaw[5].mathlawfactor = GetMathNotateExpLaw;
 TableAngleLaw[5].M.DimFunc = RADIAN;
 TableAngleLaw[5].M.Size = 3;
 TableAngleLaw[5].M.Factor = new FACTOR[3];

 //t {SECOND,SECOND,"t",Copy,Copy,AlwaysTrue,AlwaysTrue}
 TableAngleLaw[5].M.Factor[0].SourceDim = SECOND;
 TableAngleLaw[5].M.Factor[0].TargetDim = SECOND;
 TableAngleLaw[5].M.Factor[0].Name = new char[2];
 strcpy(TableAngleLaw[5].M.Factor[0].Name,"t");
 TableAngleLaw[5].M.Factor[0].Convert = Copy;
 TableAngleLaw[5].M.Factor[0].Invert = Copy;
 TableAngleLaw[5].M.Factor[0].PreBlock = AlwaysTrue;
 TableAngleLaw[5].M.Factor[0].PostBlock = AlwaysTrue;

 //A {DEG_SEC,RAD_SEC,"Factor A",DegToRad,RadToDeg,AlwaysTrue,AlwaysTrue}
 TableAngleLaw[5].M.Factor[1].SourceDim = DEGREE;
 TableAngleLaw[5].M.Factor[1].TargetDim = RADIAN;
 TableAngleLaw[5].M.Factor[1].Name = new char[9];
 strcpy(TableAngleLaw[5].M.Factor[1].Name,"Factor A");
 TableAngleLaw[5].M.Factor[1].Convert = DegToRad;
 TableAngleLaw[5].M.Factor[1].Invert = RadToDeg;
 TableAngleLaw[5].M.Factor[1].PreBlock = AlwaysTrue;
 TableAngleLaw[5].M.Factor[1].PostBlock = AlwaysTrue;

 //B {DEGREE,RADIAN,"Factor B",DegToRad,RadToDeg,AlwaysTrue,AlwaysTrue}
 TableAngleLaw[5].M.Factor[2].SourceDim = DEG_SEC;
 TableAngleLaw[5].M.Factor[2].TargetDim = RAD_SEC;
 TableAngleLaw[5].M.Factor[2].Name = new char[9];
 strcpy(TableAngleLaw[5].M.Factor[2].Name,"Factor B");
 TableAngleLaw[5].M.Factor[2].Convert = DegToRad;
 TableAngleLaw[5].M.Factor[2].Invert = RadToDeg;
 TableAngleLaw[5].M.Factor[2].PreBlock = AlwaysTrue;
 TableAngleLaw[5].M.Factor[2].PostBlock = AlwaysTrue;

}
//*****************************************************************************
//СТАТУС: II.8., private, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ: void CreateTableOfAmplLaw()
//НАЗНАЧЕНИЕ:
//Создание таблицы детерминированных законов изменения во времени
//Для работы функция использует глобальный указатель TableAmplLaw и
//TableAmplLawCount
//СПИСОК ФУНКЦИЙ (по порядку, общее количество - 4):
//1. ConstLaw; 2. CosLaw; 3. SinLaw; 4. SquarePulseLaw
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ: нет
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:  нет
//*****************************************************************************
void ExtSignal::CreateTableOfAmplLaw()
{
 TableAmplLawCount = 4;
 TableAmplLaw = new LAW[4];

 //****************************
 //Первый закон - F(t) = A
 //****************************
 // {ConstLaw, CONST_LAW, InitFactors, GetMathNotateConstLaw, RADIAN, 2}
 TableAmplLaw[0].turn = ON;
 TableAmplLaw[0].law = ConstLaw;
 TableAmplLaw[0].type = CONST_LAW;
 TableAmplLaw[0].inputfactor = InitFactors;
 TableAmplLaw[0].mathlawfactor = GetMathNotateConstLaw;
 TableAmplLaw[0].M.DimFunc = NONDIM;
 TableAmplLaw[0].M.Size = 2;
 TableAmplLaw[0].M.Factor = new FACTOR[2];

 //t {SECOND,SECOND,"t",Copy,Copy,AlwaysTrue,AlwaysTrue}
 TableAmplLaw[0].M.Factor[0].SourceDim = SECOND;
 TableAmplLaw[0].M.Factor[0].TargetDim = SECOND;
 TableAmplLaw[0].M.Factor[0].Name = new char[2];
 strcpy(TableAmplLaw[0].M.Factor[0].Name,"t");
 TableAmplLaw[0].M.Factor[0].Convert = Copy;
 TableAmplLaw[0].M.Factor[0].Invert = Copy;
 TableAmplLaw[0].M.Factor[0].PreBlock = AlwaysTrue;
 TableAmplLaw[0].M.Factor[0].PostBlock = AlwaysTrue;

 //A  {NONDIM,NONDIM,"Amplitude A",Copy,Copy,AlwaysTrue,AlwaysTrue}
 TableAmplLaw[0].M.Factor[1].SourceDim = NONDIM;
 TableAmplLaw[0].M.Factor[1].TargetDim = NONDIM;
 TableAmplLaw[0].M.Factor[1].Name = new char[12];
 strcpy(TableAmplLaw[0].M.Factor[1].Name,"Amplitude A");
 TableAmplLaw[0].M.Factor[1].Convert = Copy;
 TableAmplLaw[0].M.Factor[1].Invert = Copy;
 TableAmplLaw[0].M.Factor[1].PreBlock = AlwaysTrue;
 TableAmplLaw[0].M.Factor[1].PostBlock = AlwaysTrue;

 //***********************************
 //Второй закон - F(t) = A*COS(B*t+C)
 //**********************************
 //{CosLaw,COS_LAW, InitFactors, GetMathNotateCosLaw, RADIAN, 4}
 TableAmplLaw[1].turn = ON;
 TableAmplLaw[1].law = CosLaw;
 TableAmplLaw[1].type = COS_LAW;
 TableAmplLaw[1].inputfactor = InitFactors;
 TableAmplLaw[1].mathlawfactor = GetMathNotateCosLaw;
 TableAmplLaw[1].M.DimFunc = NONDIM;
 TableAmplLaw[1].M.Size = 4;
 TableAmplLaw[1].M.Factor = new FACTOR[4];

 //t  {SECOND,SECOND,"t",Copy,Copy,AlwaysTrue,AlwaysTrue}
 TableAmplLaw[1].M.Factor[0].SourceDim = SECOND;
 TableAmplLaw[1].M.Factor[0].TargetDim = SECOND;
 TableAmplLaw[1].M.Factor[0].Name = new char[2];
 strcpy(TableAmplLaw[1].M.Factor[0].Name,"t");
 TableAmplLaw[1].M.Factor[0].Convert = Copy;
 TableAmplLaw[1].M.Factor[0].Invert = Copy;
 TableAmplLaw[1].M.Factor[0].PreBlock = AlwaysTrue;
 TableAmplLaw[1].M.Factor[0].PostBlock = AlwaysTrue;

 //A {NONDIM,NONDIM,"Amplitude A",DegToRad,RadToDeg,AlwaysTrue,AlwaysTrue}
 TableAmplLaw[1].M.Factor[1].SourceDim = NONDIM;
 TableAmplLaw[1].M.Factor[1].TargetDim = NONDIM;
 TableAmplLaw[1].M.Factor[1].Name = new char[12];
 strcpy(TableAmplLaw[1].M.Factor[1].Name,"Amplitude A");
 TableAmplLaw[1].M.Factor[1].Convert = Copy;
 TableAmplLaw[1].M.Factor[1].Invert = Copy;
 TableAmplLaw[1].M.Factor[1].PreBlock = AlwaysTrue;
 TableAmplLaw[1].M.Factor[1].PostBlock = AlwaysTrue;

 //B {HZ,PULSAT,"Frequence B",FreqToPulsatance,PulsatanceToFreq,AlwaysTrue,
 //   AlwaysTrue}
 TableAmplLaw[1].M.Factor[2].SourceDim = HZ;
 TableAmplLaw[1].M.Factor[2].TargetDim = PULSAT;
 TableAmplLaw[1].M.Factor[2].Name = new char[12];
 strcpy(TableAmplLaw[1].M.Factor[2].Name,"Frequence B");
 TableAmplLaw[1].M.Factor[2].Convert = FreqToPulsatance;
 TableAmplLaw[1].M.Factor[2].Invert = PulsatanceToFreq;
 TableAmplLaw[1].M.Factor[2].PreBlock = AlwaysTrue;
 TableAmplLaw[1].M.Factor[2].PostBlock = AlwaysTrue;

 //C {DEGREE,RADIAN,"Epoch Angle C",DegToRad,RadToDeg,AlwaysTrue,AlwaysTrue}
 TableAmplLaw[1].M.Factor[3].SourceDim = DEGREE;
 TableAmplLaw[1].M.Factor[3].TargetDim = RADIAN;
 TableAmplLaw[1].M.Factor[3].Name = new char[14];
 strcpy(TableAmplLaw[1].M.Factor[3].Name,"Epoch Angle C");
 TableAmplLaw[1].M.Factor[3].Convert = DegToRad;
 TableAmplLaw[1].M.Factor[3].Invert = RadToDeg;
 TableAmplLaw[1].M.Factor[3].PreBlock = AlwaysTrue;
 TableAmplLaw[1].M.Factor[3].PostBlock = AlwaysTrue;

 //***********************************
 //Третий закон - F(t) = A*SIN(B*t+C)
 //**********************************
 //{SinLaw,SIN_LAW, InitFactors, GetMathNotateSinLaw, RADIAN, 4}
 TableAmplLaw[2].turn = ON;
 TableAmplLaw[2].law = SinLaw;
 TableAmplLaw[2].type = SIN_LAW;
 TableAmplLaw[2].inputfactor = InitFactors;
 TableAmplLaw[2].mathlawfactor = GetMathNotateSinLaw;
 TableAmplLaw[2].M.DimFunc = NONDIM;
 TableAmplLaw[2].M.Size = 4;
 TableAmplLaw[2].M.Factor = new FACTOR[4];

 //t  {SECOND,SECOND,"t",Copy,Copy,AlwaysTrue,AlwaysTrue}
 TableAmplLaw[2].M.Factor[0].SourceDim = SECOND;
 TableAmplLaw[2].M.Factor[0].TargetDim = SECOND;
 TableAmplLaw[2].M.Factor[0].Name = new char[2];
 strcpy(TableAmplLaw[2].M.Factor[0].Name,"t");
 TableAmplLaw[2].M.Factor[0].Convert = Copy;
 TableAmplLaw[2].M.Factor[0].Invert = Copy;
 TableAmplLaw[2].M.Factor[0].PreBlock = AlwaysTrue;
 TableAmplLaw[2].M.Factor[0].PostBlock = AlwaysTrue;

 //A {NONDIM,NONDIM,"Amplitude A",DegToRad,RadToDeg,AlwaysTrue,AlwaysTrue}
 TableAmplLaw[2].M.Factor[1].SourceDim = NONDIM;
 TableAmplLaw[2].M.Factor[1].TargetDim = NONDIM;
 TableAmplLaw[2].M.Factor[1].Name = new char[12];
 strcpy(TableAmplLaw[2].M.Factor[1].Name,"Amplitude A");
 TableAmplLaw[2].M.Factor[1].Convert = Copy;
 TableAmplLaw[2].M.Factor[1].Invert = Copy;
 TableAmplLaw[2].M.Factor[1].PreBlock = AlwaysTrue;
 TableAmplLaw[2].M.Factor[1].PostBlock = AlwaysTrue;

 //B {HZ,PULSAT,"Frequence B",FreqToPulsatance,PulsatanceToFreq,AlwaysTrue,
 //   AlwaysTrue}
 TableAmplLaw[2].M.Factor[2].SourceDim = HZ;
 TableAmplLaw[2].M.Factor[2].TargetDim = PULSAT;
 TableAmplLaw[2].M.Factor[2].Name = new char[12];
 strcpy(TableAmplLaw[2].M.Factor[2].Name,"Frequence B");
 TableAmplLaw[2].M.Factor[2].Convert = FreqToPulsatance;
 TableAmplLaw[2].M.Factor[2].Invert = PulsatanceToFreq;
 TableAmplLaw[2].M.Factor[2].PreBlock = AlwaysTrue;
 TableAmplLaw[2].M.Factor[2].PostBlock = AlwaysTrue;

 //C {DEGREE,RADIAN,"Epoch Angle C",DegToRad,RadToDeg,AlwaysTrue,AlwaysTrue}
 TableAmplLaw[2].M.Factor[3].SourceDim = DEGREE;
 TableAmplLaw[2].M.Factor[3].TargetDim = RADIAN;
 TableAmplLaw[2].M.Factor[3].Name = new char[14];
 strcpy(TableAmplLaw[2].M.Factor[3].Name,"Epoch Angle C");
 TableAmplLaw[2].M.Factor[3].Convert = DegToRad;
 TableAmplLaw[2].M.Factor[3].Invert = RadToDeg;
 TableAmplLaw[2].M.Factor[3].PreBlock = AlwaysTrue;
 TableAmplLaw[2].M.Factor[3].PostBlock = AlwaysTrue;

 //**********************************************
 //Четвертый закон - F(t) = Прямоугольный импульс
 //**********************************************
 //{SquarePulseLaw, SQUAREPULSE_LAW, InitFactors, GetMathNotateSquarePulseLaw,
 // NONDIM, 8}
 TableAmplLaw[3].turn = ON;
 TableAmplLaw[3].law = SquarePulseLaw;
 TableAmplLaw[3].type = SQUAREPULSE_LAW;
 TableAmplLaw[3].inputfactor = InitFactors;
 TableAmplLaw[3].mathlawfactor = GetMathNotateSquarePulseLaw;
 TableAmplLaw[3].M.DimFunc = NONDIM;
 TableAmplLaw[3].M.Size = 8;
 TableAmplLaw[3].M.Factor = new FACTOR[8];

 //t  {SECOND,SECOND,"t",Copy,Copy,AlwaysTrue,AlwaysTrue}
 TableAmplLaw[3].M.Factor[0].SourceDim = SECOND;
 TableAmplLaw[3].M.Factor[0].TargetDim = SECOND;
 TableAmplLaw[3].M.Factor[0].Name = new char[2];
 strcpy(TableAmplLaw[3].M.Factor[0].Name,"t");
 TableAmplLaw[3].M.Factor[0].Convert = Copy;
 TableAmplLaw[3].M.Factor[0].Invert = Copy;
 TableAmplLaw[3].M.Factor[0].PreBlock = AlwaysTrue;
 TableAmplLaw[3].M.Factor[0].PostBlock = AlwaysTrue;

 //t0 {NONDIM,NONDIM,"t0",Copy,Copy,AlwaysTrue,AlwaysTrue}
 TableAmplLaw[3].M.Factor[1].SourceDim = NONDIM;
 TableAmplLaw[3].M.Factor[1].TargetDim = NONDIM;
 TableAmplLaw[3].M.Factor[1].Name = new char[3];
 strcpy(TableAmplLaw[3].M.Factor[1].Name,"t0");
 TableAmplLaw[3].M.Factor[1].Convert = Copy;
 TableAmplLaw[3].M.Factor[1].Invert = Copy;
 TableAmplLaw[3].M.Factor[1].PreBlock = AlwaysTrue;
 TableAmplLaw[3].M.Factor[1].PostBlock = AlwaysTrue;

 //timp {NONDIM,NONDIM,"timp",Copy,Copy,AlwaysTrue,AlwaysTrue}
 TableAmplLaw[3].M.Factor[2].SourceDim = NONDIM;
 TableAmplLaw[3].M.Factor[2].TargetDim = NONDIM;
 TableAmplLaw[3].M.Factor[2].Name = new char[5];
 strcpy(TableAmplLaw[3].M.Factor[2].Name,"timp");
 TableAmplLaw[3].M.Factor[2].Convert = Copy;
 TableAmplLaw[3].M.Factor[2].Invert = Copy;
 TableAmplLaw[3].M.Factor[2].PreBlock = AlwaysTrue;
 TableAmplLaw[3].M.Factor[2].PostBlock = AlwaysTrue;

 //Timp {NONDIM,NONDIM,"Timp",Copy,Copy,AlwaysTrue,AlwaysTrue}
 TableAmplLaw[3].M.Factor[3].SourceDim = NONDIM;
 TableAmplLaw[3].M.Factor[3].TargetDim = NONDIM;
 TableAmplLaw[3].M.Factor[3].Name = new char[5];
 strcpy(TableAmplLaw[3].M.Factor[3].Name,"Timp");
 TableAmplLaw[3].M.Factor[3].Convert = Copy;
 TableAmplLaw[3].M.Factor[3].Invert = Copy;
 TableAmplLaw[3].M.Factor[3].PreBlock = AlwaysTrue;
 TableAmplLaw[3].M.Factor[3].PostBlock = AlwaysTrue;

 //Umax {NONDIM,NONDIM,"Umax",Copy,Copy,AlwaysTrue,AlwaysTrue}
 TableAmplLaw[3].M.Factor[4].SourceDim = NONDIM;
 TableAmplLaw[3].M.Factor[4].TargetDim = NONDIM;
 TableAmplLaw[3].M.Factor[4].Name = new char[5];
 strcpy(TableAmplLaw[3].M.Factor[4].Name,"Umax");
 TableAmplLaw[3].M.Factor[4].Convert = Copy;
 TableAmplLaw[3].M.Factor[4].Invert = Copy;
 TableAmplLaw[3].M.Factor[4].PreBlock = AlwaysTrue;
 TableAmplLaw[3].M.Factor[4].PostBlock = AlwaysTrue;

 //Umin {NONDIM,NONDIM,"Umin",Copy,Copy,AlwaysTrue,AlwaysTrue}
 TableAmplLaw[3].M.Factor[5].SourceDim = NONDIM;
 TableAmplLaw[3].M.Factor[5].TargetDim = NONDIM;
 TableAmplLaw[3].M.Factor[5].Name = new char[5];
 strcpy(TableAmplLaw[3].M.Factor[5].Name,"Umin");
 TableAmplLaw[3].M.Factor[5].Convert = Copy;
 TableAmplLaw[3].M.Factor[5].Invert = Copy;
 TableAmplLaw[3].M.Factor[5].PreBlock = AlwaysTrue;
 TableAmplLaw[3].M.Factor[5].PostBlock = AlwaysTrue;

 //Flag {NONDIM,NONDIM,"Flag",Copy,Copy,AlwaysTrue,AlwaysTrue}
 TableAmplLaw[3].M.Factor[6].SourceDim = NONDIM;
 TableAmplLaw[3].M.Factor[6].TargetDim = NONDIM;
 TableAmplLaw[3].M.Factor[6].Name = new char[5];
 strcpy(TableAmplLaw[3].M.Factor[6].Name,"Flag");
 TableAmplLaw[3].M.Factor[6].Convert = Copy;
 TableAmplLaw[3].M.Factor[6].Invert = Copy;
 TableAmplLaw[3].M.Factor[6].PreBlock = AlwaysTrue;
 TableAmplLaw[3].M.Factor[6].PostBlock = AlwaysTrue;

 //Count {NONDIM,NONDIM,"Count",Copy,Copy,AlwaysTrue,AlwaysTrue}
 TableAmplLaw[3].M.Factor[7].SourceDim = NONDIM;
 TableAmplLaw[3].M.Factor[7].TargetDim = NONDIM;
 TableAmplLaw[3].M.Factor[7].Name = new char[6];
 strcpy(TableAmplLaw[3].M.Factor[7].Name,"Count");
 TableAmplLaw[3].M.Factor[7].Convert = Copy;
 TableAmplLaw[3].M.Factor[7].Invert = Copy;
 TableAmplLaw[3].M.Factor[7].PreBlock = AlwaysTrue;
 TableAmplLaw[3].M.Factor[7].PostBlock = AlwaysTrue;

}
//*****************************************************************************
//СТАТУС: II.9., private, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ: void CreateTableOfCasualLaw()
//НАЗНАЧЕНИЕ:
//Создание таблицы случайных законов изменения величин во времени
//Для работы функция использует глобальный указатель TableCasualLaw и
//TableCasualLawCount
//СПИСОК ФУНКЦИЙ (по порядку, общее количество - 6):
//1. RandLaw; 2. Rand2PiLaw; 3. RandABLaw; 4. RelayLaw; 5. Det1Law; 6. Det0Law
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ: нет
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:  нет
//*****************************************************************************
void ExtSignal::CreateTableOfCasualLaw()
{
 TableCasualLawCount = 6;
 TableCasualLaw = new LAW[6];

 //********************************************************
 //Первый закон - Равновероятный закон в интервале [0..1]
 //Параметры функции и аргумент отсутствуют.
 //********************************************************
 // {RandLaw, RAND_LAW, InitFactors, GetMathNotateRandLaw, NONDIM, 0}
 TableCasualLaw[0].turn = ON;
 TableCasualLaw[0].law = RandLaw;
 TableCasualLaw[0].type = RAND_LAW;
 TableCasualLaw[0].inputfactor = InitFactors;
 TableCasualLaw[0].mathlawfactor = GetMathNotateRandLaw;
 TableCasualLaw[0].M.DimFunc = NONDIM;
 TableCasualLaw[0].M.Size = 1;
 TableCasualLaw[0].M.Factor = new FACTOR[1];

 //RandValue
 TableCasualLaw[0].M.Factor[0].SourceDim = NONDIM;
 TableCasualLaw[0].M.Factor[0].TargetDim = NONDIM;
 TableCasualLaw[0].M.Factor[0].Name = new char[5];
 strcpy(TableCasualLaw[0].M.Factor[0].Name,"Rand");
 TableCasualLaw[0].M.Factor[0].Convert = Copy;
 TableCasualLaw[0].M.Factor[0].Invert = Copy;
 TableCasualLaw[0].M.Factor[0].PreBlock = AlwaysTrue;
 TableCasualLaw[0].M.Factor[0].PostBlock = AlwaysTrue;

 //*************************************************************
 //Второй закон - Равновероятный закон в интервале [0..2PI]
 //*************************************************************
 //{Ran2PiLaw,RAND2PI_LAW, InitFactors, GetMathNotateRand2PiLaw, NONDIM, 0}
 TableCasualLaw[1].turn = ON;
 TableCasualLaw[1].law = Rand2PiLaw;
 TableCasualLaw[1].type = RAND2PI_LAW;
 TableCasualLaw[1].inputfactor = InitFactors;
 TableCasualLaw[1].mathlawfactor = GetMathNotateRand2PiLaw;
 TableCasualLaw[1].M.DimFunc = NONDIM;
 TableCasualLaw[1].M.Size = 1;
 TableCasualLaw[1].M.Factor = new FACTOR[1];

 //RandValue
 TableCasualLaw[1].M.Factor[0].SourceDim = NONDIM;
 TableCasualLaw[1].M.Factor[0].TargetDim = NONDIM;
 TableCasualLaw[1].M.Factor[0].Name = new char[5];
 strcpy(TableCasualLaw[1].M.Factor[0].Name,"Rand");
 TableCasualLaw[1].M.Factor[0].Convert = Copy;
 TableCasualLaw[1].M.Factor[0].Invert = Copy;
 TableCasualLaw[1].M.Factor[0].PreBlock = AlwaysTrue;
 TableCasualLaw[1].M.Factor[0].PostBlock = AlwaysTrue;

 //*************************************************************
 //Третий закон - Равновероятный закон в интервале [A..B]
 //*************************************************************
 //{RanABLaw,RANDAB_LAW, InitFactors, GetMathNotateRandABLaw, NONDIM, 2}
 TableCasualLaw[2].turn = ON;
 TableCasualLaw[2].law = RandABLaw;
 TableCasualLaw[2].type = RANDAB_LAW;
 TableCasualLaw[2].inputfactor = InitFactors;
 TableCasualLaw[2].mathlawfactor = GetMathNotateRandABLaw;
 TableCasualLaw[2].M.DimFunc = NONDIM;
 TableCasualLaw[2].M.Size = 3;
 TableCasualLaw[2].M.Factor = new FACTOR[3];

 //RandValue
 TableCasualLaw[2].M.Factor[0].SourceDim = NONDIM;
 TableCasualLaw[2].M.Factor[0].TargetDim = NONDIM;
 TableCasualLaw[2].M.Factor[0].Name = new char[5];
 strcpy(TableCasualLaw[2].M.Factor[0].Name,"Rand");
 TableCasualLaw[2].M.Factor[0].Convert = Copy;
 TableCasualLaw[2].M.Factor[0].Invert = Copy;
 TableCasualLaw[2].M.Factor[0].PreBlock = AlwaysTrue;
 TableCasualLaw[2].M.Factor[0].PostBlock = AlwaysTrue;

 //A  {NONDIM,NONDIM,"Lower border A",Copy,Copy,AlwaysTrue,AlwaysTrue}
 TableCasualLaw[2].M.Factor[1].SourceDim = NONDIM;
 TableCasualLaw[2].M.Factor[1].TargetDim = NONDIM;
 TableCasualLaw[2].M.Factor[1].Name = new char[15];
 strcpy(TableCasualLaw[2].M.Factor[1].Name,"Lower border A");
 TableCasualLaw[2].M.Factor[1].Convert = Copy;
 TableCasualLaw[2].M.Factor[1].Invert = Copy;
 TableCasualLaw[2].M.Factor[1].PreBlock = AlwaysTrue;
 TableCasualLaw[2].M.Factor[1].PostBlock = AlwaysTrue;

 //B  {NONDIM,NONDIM,"Upper border B",Copy,Copy,AlwaysTrue,AlwaysTrue}
 TableCasualLaw[2].M.Factor[2].SourceDim = NONDIM;
 TableCasualLaw[2].M.Factor[2].TargetDim = NONDIM;
 TableCasualLaw[2].M.Factor[2].Name = new char[15];
 strcpy(TableCasualLaw[2].M.Factor[2].Name,"Upper border B");
 TableCasualLaw[2].M.Factor[2].Convert = Copy;
 TableCasualLaw[2].M.Factor[2].Invert = Copy;
 TableCasualLaw[2].M.Factor[2].PreBlock = AlwaysTrue;
 TableCasualLaw[2].M.Factor[2].PostBlock = AlwaysTrue;

 //*************************************************************
 //Четвертый закон - Случайный закон Релея [0..+INFINITE]
 //*************************************************************
 //{RelayLaw,RELAY_LAW, InitFactors, GetMathNotateRandLaw, NONDIM, 0}
 TableCasualLaw[3].turn = ON;
 TableCasualLaw[3].law = RelayLaw;
 TableCasualLaw[3].type = RELAY_LAW;
 TableCasualLaw[3].inputfactor = InitFactors;
 TableCasualLaw[3].mathlawfactor = GetMathNotateRelayLaw;
 TableCasualLaw[3].M.DimFunc = NONDIM;
 TableCasualLaw[3].M.Size = 1;
 TableCasualLaw[3].M.Factor = new FACTOR[1];

 //RandValue
 TableCasualLaw[3].M.Factor[0].SourceDim = NONDIM;
 TableCasualLaw[3].M.Factor[0].TargetDim = NONDIM;
 TableCasualLaw[3].M.Factor[0].Name = new char[5];
 strcpy(TableCasualLaw[3].M.Factor[0].Name,"Rand");
 TableCasualLaw[3].M.Factor[0].Convert = Copy;
 TableCasualLaw[3].M.Factor[0].Invert = Copy;
 TableCasualLaw[3].M.Factor[0].PreBlock = AlwaysTrue;
 TableCasualLaw[3].M.Factor[0].PostBlock = AlwaysTrue;

 //*************************************************************
 //Пятый закон - закон исключения СВ из произведения
 //*************************************************************
 //{Det1Law, DET1_LAW, InitFactors, GetMathNotateDet1Law, NONDIM, 0}
 TableCasualLaw[4].turn = ON;
 TableCasualLaw[4].law = Det1Law;
 TableCasualLaw[4].type = DET1_LAW;
 TableCasualLaw[4].inputfactor = InitFactors;
 TableCasualLaw[4].mathlawfactor = GetMathNotateDet1Law;
 TableCasualLaw[4].M.DimFunc = NONDIM;
 TableCasualLaw[4].M.Size = 1;
 TableCasualLaw[4].M.Factor = new FACTOR[1];

 //RandValue
 TableCasualLaw[4].M.Factor[0].SourceDim = NONDIM;
 TableCasualLaw[4].M.Factor[0].TargetDim = NONDIM;
 TableCasualLaw[4].M.Factor[0].Name = new char[5];
 strcpy(TableCasualLaw[4].M.Factor[0].Name,"Rand");
 TableCasualLaw[4].M.Factor[0].Convert = Copy;
 TableCasualLaw[4].M.Factor[0].Invert = Copy;
 TableCasualLaw[4].M.Factor[0].PreBlock = AlwaysTrue;
 TableCasualLaw[4].M.Factor[0].PostBlock = AlwaysTrue;

 //*************************************************************
 //Шестой закон - закон исключения СВ из суммы
 //*************************************************************
 //{Det0Law, DET0_LAW, InitFactors, GetMathNotateDet0Law, NONDIM, 0}
 TableCasualLaw[5].turn = ON;
 TableCasualLaw[5].law = Det0Law;
 TableCasualLaw[5].type = DET0_LAW;
 TableCasualLaw[5].inputfactor = InitFactors;
 TableCasualLaw[5].mathlawfactor = GetMathNotateDet0Law;
 TableCasualLaw[5].M.DimFunc = NONDIM;
 TableCasualLaw[5].M.Size = 1;
 TableCasualLaw[5].M.Factor = new FACTOR[1];

 //RandValue
 TableCasualLaw[5].M.Factor[0].SourceDim = NONDIM;
 TableCasualLaw[5].M.Factor[0].TargetDim = NONDIM;
 TableCasualLaw[5].M.Factor[0].Name = new char[5];
 strcpy(TableCasualLaw[5].M.Factor[0].Name,"Rand");
 TableCasualLaw[5].M.Factor[0].Convert = Copy;
 TableCasualLaw[5].M.Factor[0].Invert = Copy;
 TableCasualLaw[5].M.Factor[0].PreBlock = AlwaysTrue;
 TableCasualLaw[5].M.Factor[0].PostBlock = AlwaysTrue;

}
//*****************************************************************************
//СТАТУС: II.9., private, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ: void DeleteTable(LAW* Table, uint Size)
//НАЗНАЧЕНИЕ:
//Удаление таблицы законов изменения величин во времени.
//Для работы функция использует глобальный указатель Law* Table и размер таблицы
//Size
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. LAW* Table - указатель на удаляемую таблицу
//2. uint Size - размер удаляемой таблицы
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:  нет
//*****************************************************************************
void ExtSignal::DeleteTable(LAW* Table, uint& Size)
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

//=============================================================================
//                    ОПИСАНИЕ ФУНКЦИЙ - ЧЛЕНОВ КЛАССА ExtSignal
//=============================================================================

//**********************************************
// I. ЗАКРЫТЫЕ ФУНКЦИИ - ЧЛЕНЫ КЛАССА ExtSignal
//    PRIVATE MEMBER - FUNCTION
//**********************************************

//*****************************************************************************
//СТАТУС: I.1., private, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ: bool CreateArray(const uint size)
//НАЗНАЧЕНИЕ:
//Функция предназначена для выделения памяти под размещение компонент внешнего
//сигнала. Результатом работы является создание одномерного массива типа
//COMPONENT размером size. В данной версии функции при невозможности выделения
//памяти заданного размера обработчик исключительной ситуации просто сообщает об
//ошибке распределения памяти и на этом программа завершает свою работу.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. const uint size - количество компонентов во внешнем сигнале
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: bool
//1. true - создан массив заданного размера
//2. false - требуемый размер памяти выделить не удалось
//*****************************************************************************
bool ExtSignal::CreateArray(const uint size)
{
 bool lSuccess = true;
 try //Распределение памяти под элементы матрицы
  {   //и первоначальная их инициализация
  	data = new COMPONENT[size];
   for(uint i = 0; i < Count; i++)
   { data[i].AzAngleArg = NULL;
     data[i].ElevAngleArg = NULL;
     data[i].PolAngleArg = NULL;
     data[i].AmplArg = NULL;
     data[i].AmplCasualLawArg = NULL;
     data[i].VertPolPhaseArg = NULL;
     data[i].HorPolPhaseArg = NULL;
     data[i].PhaseArg = NULL;
     data[i].PhaseCasualLawArg = NULL;
   }
  }
  // ENTER THIS BLOCK ONLY IF xalloc IS THROWN.
  // YOU COULD REQUEST OTHER ACTIONS BEFORE TERMINATING
  catch (xalloc x)
  {
    lSuccess = false;
    #if defined(CONSOLE)
      cout << "\nMEMORY ALLOCATION ERROR !!!" << endl;
      cout << "Could not allocate all components external signal in memory."
           << endl;
      cout << "Memory request is " << x.requested() << " byte." << endl;
      getch();
    #endif
    exit(-1);
  }
return lSuccess;
}
//*****************************************************************************
//СТАТУС: I.2., private, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ: void ResetProperty(uint index)
//НАЗНАЧЕНИЕ:
//Функция предназначена для аннулирования всех характеристик компоненты внешнего
//сигнала с порядковым номером index. Под индексом равным 0 массива data всегда
//хранится полезная составляющая внешнего сигнала.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. const uint index - порядковый номер компонента внешнего сигнала в массиве
//                      data
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: нет
//*****************************************************************************
void ExtSignal::ResetProperty(uint index)
{
  //Общая характеристика составляющей
  data[index].Type = (index == 0) ? SIGNAL : JAMMER;
  data[index].Usage = OFF;

  //Азимутальный угол источника сигнала
  data[index].AzAngleLawType = ZERO_LAW;
  if (data[index].AzAngleArg != NULL)
   data[index].AzAngleArg = DeleteArgList(data[index].AzAngleArg);
  data[index].AzimuthAngle = ZeroLaw;

  //Угол места источника сигнала
  data[index].ElevAngleLawType = ZERO_LAW;
  if (data[index].ElevAngleArg != NULL)
   data[index].ElevAngleArg = DeleteArgList(data[index].ElevAngleArg);
  data[index].ElevateAngle = ZeroLaw;

  //Угол поляризации сигнала
  data[index].PolAngleLawType = ZERO_LAW;
  if (data[index].PolAngleArg != NULL)
   data[index].PolAngleArg = DeleteArgList(data[index].PolAngleArg);
  data[index].PolAngle = ZeroLaw;

  //АМПЛИТУДНАЯ ХАРАКТЕРИСТИКА СОСТАВЛЯЮЩЕЙ СИГНАЛА
  //Детерминированный закон измененения амплитуды сигнала во времени
  data[index].AmplLawType = ZERO_LAW;
  if (data[index].AmplArg != NULL)
   data[index].AmplArg = DeleteArgList(data[index].AmplArg);
  data[index].AmplLaw = ZeroLaw;

  //Случайный закон изменения амплитуды во времени
  data[index].AmplCasualLawType = ZERO_LAW;
  if (data[index].AmplCasualLawArg != NULL)
   data[index].AmplCasualLawArg = DeleteArgList(data[index].AmplCasualLawArg);
  data[index].AmplCasualLaw = ZeroLaw;

  //ФАЗОВАЯ ХАРАКТЕРИСТИКА СОСТАВЛЯЮЩЕЙ СИГНАЛА
  //Несущая частота сигнала (необязательный параметр)
  data[index].CarPulsatance  = 0.0; //(in rad/sec)

  //Фазовый закон изменения вертикальной составляющей компоненты внешнего
  //сигнала во времени
  data[index].VertPolPhaseLawType = ZERO_LAW;
  if (data[index].VertPolPhaseArg != NULL)
   data[index].VertPolPhaseArg = DeleteArgList(data[index].VertPolPhaseArg);
  data[index].VertPolPhase = ZeroLaw;

  //Фазовый закон изменения горизонтальной составляющей компоненты внешнего
  //сигнала во времени
  data[index].HorPolPhaseLawType = ZERO_LAW;
  if (data[index].HorPolPhaseArg != NULL)
   data[index].HorPolPhaseArg = DeleteArgList(data[index].HorPolPhaseArg);
  data[index].HorPolPhase = ZeroLaw;


  //Детерминированный фазовый закон изменения составляющей сигнала во времени
  data[index].PhaseLawType = ZERO_LAW;
  if (data[index].PhaseArg != NULL)
   data[index].PhaseArg = DeleteArgList(data[index].PhaseArg);
  data[index].PhaseLaw = ZeroLaw;

  //Случайный фазовый закон изменения составляющей сигнала во времени
  data[index].PhaseCasualLawType = ZERO_LAW;
  if (data[index].PhaseCasualLawArg != NULL)
   data[index].PhaseCasualLawArg = DeleteArgList(data[index].PhaseCasualLawArg);
  data[index].PhaseCasualLaw = ZeroLaw;
}
//*****************************************************************************
//СТАТУС: I.2., private, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ: void ConsoleDataEntry(uint index)
//НАЗНАЧЕНИЕ:
//Функция предназначена для ввода данных по всем характеристикам компоненты
//внешнего сигнала с порядковым номером index. Под индексом равным 0 массива
//data всегда хранится полезная составляющая внешнего сигнала.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. const uint index - порядковый номер компонента внешнего сигнала в массиве
// data
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: нет
//*****************************************************************************
void ExtSignal::ConsoleDataEntry(uint index)
{ uint X = 1;
  uint Y = wherey();
  uint CurX, CurY;
  //Устанавливаем по умолчанию
  data[index].Type = (index == 0) ? SIGNAL : JAMMER;
  data[index].Usage = ON;
  //Отображаем информацию по вводимой компоненте внешнего сигнала
  cout << "INPUT DATA by: ";
  if (index == 0) cout << "DESIRED SIGNAL ";
  else cout << "JAMMER N " << index;
  cout << " PROPERTY: ";
  CurX = wherex();

  //Ввод параметров по азимутальному углу источника сигнала
  cout << "1. Azimuth Angle of Signal Source" << endl;
  AzimuthAngleInPut(index,ENG);

  //Ввод параметров по углу места источника сигнала
  gotoxy(CurX,Y);
  clreol();
  cout << "2. Elevate Angle of Signal Source" << endl;
  ElevateAngleInPut(index,ENG);

  //*************************************************************************
  //АМПЛИТУДНАЯ СОСТАВЛЯЮЩАЯ СИГНАЛА
  //Включает в себя ПОЛЯРИЗАЦИОННУЮ (I) и ПРОСТРАНСТВЕННУЮ (II) КОМПОНЕНТЫ
  //1. Поляризационная амплитуда сигнала - PolAngle
  //2. Пространственная амплитуда сигнала - AmplLaw
  //3. Случайный закон изменения амплитуды во времени - AmplCasualLaw
  //1-3 - образуют АМПЛИТУДУ (РАДИУС-ВЕКТОР) КОМПЛЕКСНОЙ СОСТАВЛЯЮЩЕЙ СИГНАЛА
  //*************************************************************************

  //Ввод параметров по углу поляризации сигнала
  gotoxy(CurX,Y);
  clreol();
  cout << "3. Signal Polarization Amplitude" << endl;
  ResetLaw(TableAngleLaw,TableAngleLawCount,SQUARE_LAW);
  PolAngleInPut(index,ENG);
  SetAllLaw(TableAngleLaw,TableAngleLawCount);

  //Ввод параметров по закону изменения амплитуды сигнала
  gotoxy(CurX,Y);
  clreol();
  cout << "4. Signal Space Amplitude" << endl;
  AmplLawInPut(index,ENG);

  //Выбор случайного закона изменения амплитуды сигнала
  gotoxy(CurX,Y);
  clreol();
  cout << "5. Amplitude Casual Law" << endl;
  ResetLaw(TableCasualLaw,TableCasualLawCount,DET0_LAW);
  AmplCasualLawInPut(index,ENG);
  SetLaw(TableCasualLaw,TableCasualLawCount,DET0_LAW);

  //****************************************************************************
  //ФАЗОВАЯ СОСТАВЛЯЮЩАЯ СИГНАЛА
  //Включает в себя ПОЛЯРИЗАЦИОННУЮ (I) и ПРОСТРАНСТВЕННУЮ (II) КОМПОНЕНТЫ
  //1. Фаза вертикальной составляющей поляризации сигнала - VertPolPhaseLaw
  //2. Фаза горизонтальной составляющей поляризации сигнала - HorPolPhaseLaw
  //3. Пространственная фаза сигнала - PhaseLaw
  //4. Случайный закон изменения фазы во времени - PhaseCasualLaw
  //5. Угловая частота несущей (не учитывается в расчетах) - Carrier Pulsatance
  //1-4 - составляют ФАЗУ (УГОЛ) КОМПЛЕКСНОЙ ОГИБАЮЩЕЙ СИГНАЛА
  //****************************************************************************

  //Выбор закона изменения фазы вертикальной составляющей поляризации сигнала
  gotoxy(CurX,Y);
  clreol();
  cout << "6. Vert. Polarization Phase of Signal" << endl;
  ResetLaw(TableAngleLaw,TableAngleLawCount,SQUARE_LAW);
  VertPhasePolInPut(index,ENG);
  SetLaw(TableAngleLaw,TableAngleLawCount,SQUARE_LAW);

  //Выбор закона изменения фазы горизонтальной составляющей поляризации сигнала
  gotoxy(CurX,Y);
  clreol();
  cout << "7. Hor. Polarization Phase of Signal" << endl;
  ResetLaw(TableAngleLaw,TableAngleLawCount,SQUARE_LAW);
  HorPhasePolInPut(index,ENG);
  SetLaw(TableAngleLaw,TableAngleLawCount,SQUARE_LAW);

  //Выбор закона изменения пространственной фазы сигнала
  gotoxy(CurX,Y);
  clreol();
  cout << "8. Space Phase of Signal" << endl;
  ResetLaw(TableAngleLaw,TableAngleLawCount,SQUARE_LAW);
  PhaseLawInPut(index,ENG);
  SetLaw(TableAngleLaw,TableAngleLawCount,SQUARE_LAW);

  //Выбор случайного закона изменения фазы сигнала
  gotoxy(CurX,Y);
  clreol();
  cout << "9. Phase Casual Law" << endl;
  ResetLaw(TableCasualLaw,TableCasualLawCount,DET1_LAW);
  PhaseCasualLawInPut(index,ENG);
  SetLaw(TableCasualLaw,TableCasualLawCount,DET1_LAW);

  //Очистка экрана
  CurY = wherey();
  gotoxy(X,Y);
  for(uint i = Y; i <= CurY; i++) {clreol(); cout << endl;}
  gotoxy(X,Y);

}

//*****************************************************************************
//СТАТУС: I.3., private, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ: void AzimuthAngleInPut(uint index,uint language);
//НАЗНАЧЕНИЕ:
//Функция предназначена для ввода данных об азимутальном угле компоненты внешнего
//сигнала с порядковым номером index. Под индексом равным 0 массива data всегда
//хранится полезная составляющая внешнего сигнала.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. const uint index - порядковый номер компонента внешнего сигнала в массиве
//data
//2. uint language - язык интерфейса
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: нет
//*****************************************************************************
void ExtSignal::AzimuthAngleInPut(uint index,uint language)
{ uint X = 1;
  uint Y = wherey();
  uint CurX,CurY;
  uint Select;
  char c;
  char* str;
  bool lBreak;
  //Вывод списка законов по которым может изменяться во времени угол по азимуту
  //источника составляющей внешнего сигнала
  uint Count = AccessibleLaw(TableAngleLaw,TableAngleLawCount);
  uint Index;
  gotoxy(X,Y);
 do
  {
    cout << "*------------------------------------------------------------*\n";
    cout << "*            SELECT AZIMUTH ANGLE MODIFICATION LAW           *\n";
    cout << "*------------------------------------------------------------*\n";
    for(uint i = 0; i<Count; i++)
    { Index = GetIndexLaw(TableAngleLaw, TableAngleLawCount, i);
      cout << (i+1) << ". ";
      cout << GetLawName(TableAngleLaw[Index].type,language);
      cout << "\t: " << GetLawNotate(TableAngleLaw[Index].type) << endl;
    }
    cout << "*------------------------------------------------------------*\n";
   do
    { do
       { CurX = wherex(); CurY = wherey();
         cout << "Enter number [1 - " << Count << "]: ";
         cin >> Select;
         gotoxy(CurX,CurY); clreol();
       } while((Select > Count) || (Select < 1));
      Select--;
      Select = GetIndexLaw(TableAngleLaw, TableAngleLawCount, Select);
      cout << "Have You selected <";
      cout << GetLawName(TableAngleLaw[Select].type,language);
      cout << ": " << GetLawNotate(TableAngleLaw[Select].type) << "> ? [Y/N]: ";
      cin >> c;
      if((c == 'Y') || (c == 'y')) lBreak = true;
      else lBreak = false;
      gotoxy(CurX,CurY); clreol();
    } while(!lBreak);

    data[index].AzAngleLawType = TableAngleLaw[Select].type;
    data[index].AzimuthAngle = TableAngleLaw[Select].law;

    //Ввод параметров закона изменения азимутального угла составляющей
    //внешнего сигнала
    const char* dimstr = GetMeasureName(TableAngleLaw[Select].M.DimFunc);
    gotoxy(X,Y);
    for(uint i = Y; i<=CurY; i++) { clreol(); cout << endl; }
    gotoxy(X,Y);
    cout << "AZIMUTH ANGLE MODIFICATION LAW: "
         << GetLawNotate(TableAngleLaw[Select].type) << " [" << dimstr
         << "]" << endl;

    //Определяем размерность закона, аргумента и параметров функции, характери-
    //зующих изменение азимутального угла источника сигнала во времени

    data[index].AzAngleArg = TableAngleLaw[Select].inputfactor(TableAngleLaw[Select].M);

    gotoxy(X,Y);
    clreol();
    str = TableAngleLaw[Select].mathlawfactor(data[index].AzAngleArg);
    cout << "AZIMUTH ANGLE LAW: " << str << " [" << dimstr << "]" << endl;
    delete[] str;
    cout << "Have You accept this law? [Y/N]: ";
    cin >> c;
    CurY = wherey();
    if((c == 'Y') || (c == 'y')) lBreak = true;
    else lBreak = false;
    gotoxy(X,Y);
    for(uint i = Y; i<=CurY; i++) { clreol(); cout << endl; }
    gotoxy(X,Y);
  } while(!lBreak);
}
//*****************************************************************************
//СТАТУС: I.4., private, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ: ElevateAngleInPut(uint index,uint language);
//НАЗНАЧЕНИЕ:
//Функция предназначена для ввода данных об угле места компоненты внешнего
//сигнала с порядковым номером index. Под индексом равным 0 массива data всегда
//хранится полезная составляющая внешнего сигнала.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. const uint index - порядковый номер компонента внешнего сигнала в массиве
//   data
//2. uint language - язык интерфейса
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: нет
//*****************************************************************************
void ExtSignal::ElevateAngleInPut(uint index,uint language)
{ uint X = 1;
  uint Y = wherey();
  uint CurX,CurY;
  uint Select;
  char c;
  char* str;
  bool lBreak;
  //Вывод списка законов по которым может изменяться во времени угол места
  //источника составляющей внешнего сигнала
  uint Count = AccessibleLaw(TableAngleLaw,TableAngleLawCount);
  uint Index;
  gotoxy(X,Y);
 do
  {
    cout << "*------------------------------------------------------------*\n";
    cout << "*            SELECT ELEVATE ANGLE MODIFICATION LAW           *\n";
    cout << "*------------------------------------------------------------*\n";
    for(uint i = 0; i<Count; i++)
    { Index = GetIndexLaw(TableAngleLaw, TableAngleLawCount, i);
      cout << (i+1) << ". ";
      cout << GetLawName(TableAngleLaw[Index].type,language);
      cout << "\t: " << GetLawNotate(TableAngleLaw[Index].type) << endl;
    }
    cout << "*------------------------------------------------------------*\n";
   do
    { do
       { CurX = wherex(); CurY = wherey();
         cout << "Enter number [1 - " << Count << "]: ";
         cin >> Select;
         gotoxy(CurX,CurY); clreol();
       } while((Select > Count) || (Select < 1));
      Select--;
      Select = GetIndexLaw(TableAngleLaw, TableAngleLawCount, Select);
      cout << "Have You selected <";
      cout << GetLawName(TableAngleLaw[Select].type,language);
      cout << ": " << GetLawNotate(TableAngleLaw[Select].type) << "> ? [Y/N]: ";
      cin >> c;
      if((c == 'Y') || (c == 'y')) lBreak = true;
      else lBreak = false;
      gotoxy(CurX,CurY); clreol();
    } while(!lBreak);

    data[index].ElevAngleLawType = TableAngleLaw[Select].type;
    data[index].ElevateAngle = TableAngleLaw[Select].law;

    //Инициализация параметров закона изменения угла места составляющей
    //внешнего сигнала
    const char* dimstr = GetMeasureName(TableAngleLaw[Select].M.DimFunc);
    gotoxy(X,Y);
    for(uint i = Y; i<=CurY; i++) { clreol(); cout << endl; }
    gotoxy(X,Y);
    cout << "ELEVATE ANGLE MODIFICATION LAW: "
         << GetLawNotate(TableAngleLaw[Select].type) << " [" << dimstr
         << "]" << endl;

    //Определяем размерность закона, аргумента и параметров функции, характери-
    //зующих изменение угла места источника сигнала во времени

    data[index].ElevAngleArg = TableAngleLaw[Select].inputfactor(TableAngleLaw[Select].M);

    gotoxy(X,Y);
    clreol();
    str = TableAngleLaw[Select].mathlawfactor(data[index].ElevAngleArg);
    cout << "ELEVATE ANGLE LAW: " << str << " [" << dimstr << "]" << endl;
    delete[] str;
    cout << "Have You accept this law? [Y/N]: ";
    cin >> c;
    CurY = wherey();
    if((c == 'Y') || (c == 'y')) lBreak = true;
    else lBreak = false;
    gotoxy(X,Y);
    for(uint i = Y; i<=CurY; i++) { clreol(); cout << endl; }
    gotoxy(X,Y);
  } while(!lBreak);
}
//*****************************************************************************
//СТАТУС: I.5., private, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ: void PolAngleInPut(uint index, uint language)
//НАЗНАЧЕНИЕ:
//Функция предназначена для ввода данных об АМПЛИТУДЕ поляризации компоненты
//внешнего сигнала с порядковым номером index.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. const uint index - порядковый номер компонента внешнего сигнала в массиве
//data
//2. uint language - язык интерфейса
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: нет
//*****************************************************************************
void ExtSignal::PolAngleInPut(uint index,uint language)
{ uint X = 1;
  uint Y = wherey();
  uint CurX,CurY;
  uint Select;
  char c;
  char* str;
  bool lBreak;
  //Вывод списка законов по которым может изменяться во времени амплитуда
  //поляризации составляющей внешнего сигнала
  uint Count = AccessibleLaw(TableAngleLaw,TableAngleLawCount);
  uint Index;
  gotoxy(X,Y);
 do
  {
    cout << "*------------------------------------------------------------*\n";
    cout << "*       SELECT POLARIZATION AMPLITUDE MODIFICATION LAW       *\n";
    cout << "*------------------------------------------------------------*\n";
    for(uint i = 0; i<Count; i++)
    { Index = GetIndexLaw(TableAngleLaw, TableAngleLawCount, i);
      cout << (i+1) << ". ";
      cout << GetLawName(TableAngleLaw[Index].type,language);
      cout << "\t: " << GetLawNotate(TableAngleLaw[Index].type) << endl;
    }
    cout << "*------------------------------------------------------------*\n";
   do
    { do
       { CurX = wherex(); CurY = wherey();
         cout << "Enter number [1 - " << Count << "]: ";
         cin >> Select;
         gotoxy(CurX,CurY); clreol();
       } while((Select > Count) || (Select < 1));
      Select--;
      Select = GetIndexLaw(TableAngleLaw, TableAngleLawCount, Select);
      cout << "Have You selected <";
      cout << GetLawName(TableAngleLaw[Select].type,language);
      cout << ": " << GetLawNotate(TableAngleLaw[Select].type) << "> ? [Y/N]: ";
      cin >> c;
      if((c == 'Y') || (c == 'y')) lBreak = true;
      else lBreak = false;
      gotoxy(CurX,CurY); clreol();
    } while(!lBreak);

    data[index].PolAngleLawType = TableAngleLaw[Select].type;
    data[index].PolAngle = TableAngleLaw[Select].law;

    //Инициализация параметров закона изменения амплитуды поляризации
    //составляющей внешнего сигнала
    const char* dimstr = GetMeasureName(TableAngleLaw[Select].M.DimFunc);
    gotoxy(X,Y);
    for(uint i = Y; i<=CurY; i++) { clreol(); cout << endl; }
    gotoxy(X,Y);
    cout << "LAW of POLARIZATION AMPLITUDE: "
         << GetLawNotate(TableAngleLaw[Select].type) << " [" << dimstr
         << "]" << endl;

    //Определяем размерность закона, аргумента и параметров функции, характери-
    //зующих изменение амплитуды поляризации сигнала во времени

    data[index].PolAngleArg = TableAngleLaw[Select].inputfactor(TableAngleLaw[Select].M);

    gotoxy(X,Y);
    clreol();
    str = TableAngleLaw[Select].mathlawfactor(data[index].PolAngleArg);
    cout << "LAW of AMPLITUDE POLARIZATION: " << str << " [" << dimstr << "]"
         << endl;
    delete[] str;
    cout << "Have You accept this law? [Y/N]: ";
    cin >> c;
    CurY = wherey();
    if((c == 'Y') || (c == 'y')) lBreak = true;
    else lBreak = false;
    gotoxy(X,Y);
    for(uint i = Y; i<=CurY; i++) { clreol(); cout << endl; }
    gotoxy(X,Y);
  } while(!lBreak);
}

//*****************************************************************************
//СТАТУС: I.6., private, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ: void AmplLawInPut(uint index,uint language)
//НАЗНАЧЕНИЕ:
//Функция предназначена для выбора закона изменения амплитуды компоненты внешнего
//сигнала с порядковым номером index. Под индексом равным 0 массива data всегда
//хранится полезная составляющая внешнего сигнала.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. const uint index - порядковый номер компонента внешнего сигнала в массиве
//2. uint language - язык вывода информационных сообщений
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: нет
//*****************************************************************************
void ExtSignal::AmplLawInPut(uint index,uint language)
{ uint X = 1;
  uint Y = wherey();
  uint CurX,CurY;
  uint Select;
  char c;
  char* str;
  bool lBreak;
  //Вывод списка детерминированных законов по которым может изменяться во
  //времени амплитуда составляющей внешнего сигнала
  uint Count = AccessibleLaw(TableAmplLaw,TableAmplLawCount);
  uint Index;
  gotoxy(X,Y);
 do
  {
    cout << "*------------------------------------------------------------*\n";
    cout << "*          SELECT AMPLITUDE MODIFICATION DETERMINE LAW       *\n";
    cout << "*------------------------------------------------------------*\n";
    for(uint i = 0; i<Count; i++)
    { Index = GetIndexLaw(TableAmplLaw, TableAmplLawCount, i);
      cout << (i+1) << ". ";
      cout << GetLawName(TableAmplLaw[Index].type,language);
      cout << "\t: " << GetLawNotate(TableAmplLaw[Index].type) << endl;
    }
    cout << "*------------------------------------------------------------*\n";
   do
    { do
       { CurX = wherex(); CurY = wherey();
         cout << "Enter number [1 - " << Count << "]: ";
         cin >> Select;
         gotoxy(CurX,CurY); clreol();
       } while((Select > Count) || (Select < 1));
      Select--;
      Select = GetIndexLaw(TableAmplLaw, TableAmplLawCount, Select);
      cout << "Have You selected <";
      cout << GetLawName(TableAmplLaw[Select].type,language);
      cout << ": " << GetLawNotate(TableAmplLaw[Select].type) << "> ? [Y/N]: ";
      cin >> c;
      if((c == 'Y') || (c == 'y')) lBreak = true;
      else lBreak = false;
      gotoxy(CurX,CurY); clreol();
    } while(!lBreak);

    data[index].AmplLawType = TableAmplLaw[Select].type;
    data[index].AmplLaw = TableAmplLaw[Select].law;

    //Инициализация параметров закона изменения амплитуды составляющей
    //внешнего сигнала
    const char* dimstr = GetMeasureName(TableAmplLaw[Select].M.DimFunc);
    gotoxy(X,Y);
    for(uint i = Y; i<=CurY; i++) { clreol(); cout << endl; }
    gotoxy(X,Y);
    cout << "AMPLITUDE DETERMINE LAW: "
         << GetLawNotate(TableAmplLaw[Select].type) << " [" << dimstr
         << "]" << endl;

    //Определяем размерность закона, аргумента и параметров функции, характери-
    //зующих изменение амплитуды источника сигнала во времени

    data[index].AmplArg = TableAmplLaw[Select].inputfactor(TableAmplLaw[Select].M);

    gotoxy(X,Y);
    clreol();
    str = TableAmplLaw[Select].mathlawfactor(data[index].AmplArg);
    cout << "AMPLITUDE DETERMINE LAW: " << str << " [" << dimstr << "]" << endl;
    delete[] str;
    cout << "Have You accept this law? [Y/N]: ";
    cin >> c;
    CurY = wherey();
    if((c == 'Y') || (c == 'y')) lBreak = true;
    else lBreak = false;
    gotoxy(X,Y);
    for(uint i = Y; i<=CurY; i++) { clreol(); cout << endl; }
    gotoxy(X,Y);
  } while(!lBreak);
}
//*****************************************************************************
//СТАТУС: I.6., private, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ: void AmplCasualLawInPut(uint index,uint language)
//НАЗНАЧЕНИЕ:
//Функция предназначена для выбора случайного закона изменения амплитуды
//компоненты внешнего сигнала с порядковым номером index. Возможно исключение
//влияния случайных факторов на АМПЛИТУДУ сигнала. В этом случае необходимо
//выбрать опцию меню - "Casuality is absent: F(t) = 1"
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. const uint index - порядковый номер компонента внешнего сигнала в массиве
//2. uint language - язык вывода информационных сообщений
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: нет
//*****************************************************************************
void ExtSignal::AmplCasualLawInPut(uint index,uint language)
{ uint X = 1;
  uint Y = wherey();
  uint CurX,CurY;
  uint Select;
  char c;
  char* str;
  bool lBreak;
  //Вывод списка случайных законов по которым может изменяться во
  //времени амплитуда составляющей внешнего сигнала
  uint Count = AccessibleLaw(TableCasualLaw,TableCasualLawCount);
  uint Index;
  gotoxy(X,Y);
 do
  {
    cout << "*------------------------------------------------------------*\n";
    cout << "*          SELECT AMPLITUDE MODIFICATION CASUAL LAW          *\n";
    cout << "*------------------------------------------------------------*\n";
    for(uint i = 0; i<Count; i++)
    { Index = GetIndexLaw(TableCasualLaw, TableCasualLawCount, i);
      cout << (i+1) << ". ";
      cout << GetLawName(TableCasualLaw[Index].type,language);
      cout << "\t: " << GetLawNotate(TableCasualLaw[Index].type) << endl;
    }
    cout << "*------------------------------------------------------------*\n";
   do
    { do
       { CurX = wherex(); CurY = wherey();
         cout << "Enter number [1 - " << Count << "]: ";
         cin >> Select;
         gotoxy(CurX,CurY); clreol();
       } while((Select > Count) || (Select < 1));
      Select--;
      Select = GetIndexLaw(TableCasualLaw, TableCasualLawCount, Select);
      cout << "Have You selected <";
      cout << GetLawName(TableCasualLaw[Select].type,language);
      cout << ": " << GetLawNotate(TableCasualLaw[Select].type) << "> ? [Y/N]: ";
      cin >> c;
      if((c == 'Y') || (c == 'y')) lBreak = true;
      else lBreak = false;
      gotoxy(CurX,CurY); clreol();
    } while(!lBreak);

    data[index].AmplCasualLawType = TableCasualLaw[Select].type;
    data[index].AmplCasualLaw = TableCasualLaw[Select].law;

    //Инициализация параметров случайного закона изменения амплитуды
    //внешнего сигнала
    gotoxy(X,Y);
    for(uint i = Y; i<=CurY; i++) { clreol(); cout << endl; }
    gotoxy(X,Y);
    cout << "AMPLITUDE CASUAL LAW: "
         << GetLawNotate(TableCasualLaw[Select].type) << endl;

    //Определяем параметры (при необходимости) случайного закона, характеризую-
    //щих изменение амплитуды сигнала во времени
    data[index].AmplCasualLawArg =
    TableCasualLaw[Select].inputfactor(TableCasualLaw[Select].M);

    gotoxy(X,Y);
    clreol();
    str = TableCasualLaw[Select].mathlawfactor(data[index].AmplCasualLawArg);
    cout << "AMPLITUDE CASUAL LAW: " << str << endl;
    delete[] str;
    cout << "Have You accept this law? [Y/N]: ";
    cin >> c;
    CurY = wherey();
    if((c == 'Y') || (c == 'y')) lBreak = true;
    else lBreak = false;
    gotoxy(X,Y);
    for(uint i = Y; i<=CurY; i++) { clreol(); cout << endl; }
    gotoxy(X,Y);
  } while(!lBreak);
}
//*****************************************************************************
//СТАТУС: I.7., private, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ: void VertPhasePolInPut(uint index,uint language)
//НАЗНАЧЕНИЕ:
//Функция предназначена для ввода данных по фазе вертикальной составляющей
//поляризации компоненты внешнего сигнала с порядковым номером index.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. const uint index - порядковый номер компонента внешнего сигнала в массиве
//data
//2. uint language - язык интерфейса
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: нет
//*****************************************************************************
void ExtSignal::VertPhasePolInPut(uint index,uint language)
{ uint X = 1;
  uint Y = wherey();
  uint CurX,CurY;
  uint Select;
  char c;
  char* str;
  bool lBreak;
  //Вывод списка законов по которым может изменяться во времени фаза вертикальной
  //составляющей поляризации компоненты внешнего сигнала
  uint Count = AccessibleLaw(TableAngleLaw,TableAngleLawCount);
  uint Index;
  gotoxy(X,Y);
 do
  {
    cout << "*------------------------------------------------------------*\n";
    cout << "*   SELECT MODIFICATION LAW OF VERTICAL POLARIZATION PHASE   *\n";
    cout << "*------------------------------------------------------------*\n";
    for(uint i = 0; i<Count; i++)
    { Index = GetIndexLaw(TableAngleLaw, TableAngleLawCount, i);
      cout << (i+1) << ". ";
      cout << GetLawName(TableAngleLaw[Index].type,language);
      cout << "\t: " << GetLawNotate(TableAngleLaw[Index].type) << endl;
    }
    cout << "*------------------------------------------------------------*\n";
   do
    { do
       { CurX = wherex(); CurY = wherey();
         cout << "Enter number [1 - " << Count << "]: ";
         cin >> Select;
         gotoxy(CurX,CurY); clreol();
       } while((Select > Count) || (Select < 1));
      Select--;
      Select = GetIndexLaw(TableAngleLaw, TableAngleLawCount, Select);
      cout << "Have You selected <";
      cout << GetLawName(TableAngleLaw[Select].type,language);
      cout << ": " << GetLawNotate(TableAngleLaw[Select].type) << "> ? [Y/N]: ";
      cin >> c;
      if((c == 'Y') || (c == 'y')) lBreak = true;
      else lBreak = false;
      gotoxy(CurX,CurY); clreol();
    } while(!lBreak);

    data[index].VertPolPhaseLawType = TableAngleLaw[Select].type;
    data[index].VertPolPhase = TableAngleLaw[Select].law;

    //Ввод параметров закона изменения фазы вертикальной составляющей поляризации
    //внешнего сигнала
    const char* dimstr = GetMeasureName(TableAngleLaw[Select].M.DimFunc);
    gotoxy(X,Y);
    for(uint i = Y; i<=CurY; i++) { clreol(); cout << endl; }
    gotoxy(X,Y);
    cout << "VERTICAL POLARIZATION PHASE LAW: "
         << GetLawNotate(TableAngleLaw[Select].type) << " [" << dimstr
         << "]" << endl;

    //Определяем размерность закона, аргумента и параметров функции, характери-
    //зующих изменение фазы вертикальной составляющей поляризации сигнала
    //во времени

    data[index].VertPolPhaseArg =
    TableAngleLaw[Select].inputfactor(TableAngleLaw[Select].M);

    gotoxy(X,Y);
    clreol();
    str = TableAngleLaw[Select].mathlawfactor(data[index].VertPolPhaseArg);
    cout << "VERT. POLAR. PHASE LAW: " << str << " [" << dimstr << "]" << endl;
    delete[] str;
    cout << "Have You accept this law? [Y/N]: ";
    cin >> c;
    CurY = wherey();
    if((c == 'Y') || (c == 'y')) lBreak = true;
    else lBreak = false;
    gotoxy(X,Y);
    for(uint i = Y; i<=CurY; i++) { clreol(); cout << endl; }
    gotoxy(X,Y);
  } while(!lBreak);
}
//*****************************************************************************
//СТАТУС: I.8., private, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ: void HorPhasePolInPut(uint index,uint language)
//НАЗНАЧЕНИЕ:
//Функция предназначена для ввода данных по фазе горизонтальной составляющей
//поляризации компоненты внешнего сигнала с порядковым номером index.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. const uint index - порядковый номер компонента внешнего сигнала в массиве
//data
//2. uint language - язык интерфейса
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: нет
//*****************************************************************************
void ExtSignal::HorPhasePolInPut(uint index,uint language)
{ uint X = 1;
  uint Y = wherey();
  uint CurX,CurY;
  uint Select;
  char c;
  char* str;
  bool lBreak;
  //Вывод списка законов по которым может изменяться во времени фаза горизонталь-
  //ной составляющей поляризации компоненты внешнего сигнала
  uint Count = AccessibleLaw(TableAngleLaw,TableAngleLawCount);
  uint Index;
  gotoxy(X,Y);
 do
  {
    cout << "*------------------------------------------------------------*\n";
    cout << "* SELECT MODIFICATION LAW OF HORIZONTAL POLARIZATION PHASE   *\n";
    cout << "*------------------------------------------------------------*\n";
    for(uint i = 0; i<Count; i++)
    { Index = GetIndexLaw(TableAngleLaw, TableAngleLawCount, i);
      cout << (i+1) << ". ";
      cout << GetLawName(TableAngleLaw[Index].type,language);
      cout << "\t: " << GetLawNotate(TableAngleLaw[Index].type) << endl;
    }
    cout << "*------------------------------------------------------------*\n";
   do
    { do
       { CurX = wherex(); CurY = wherey();
         cout << "Enter number [1 - " << Count << "]: ";
         cin >> Select;
         gotoxy(CurX,CurY); clreol();
       } while((Select > Count) || (Select < 1));
      Select--;
      Select = GetIndexLaw(TableAngleLaw, TableAngleLawCount, Select);
      cout << "Have You selected <";
      cout << GetLawName(TableAngleLaw[Select].type,language);
      cout << ": " << GetLawNotate(TableAngleLaw[Select].type) << "> ? [Y/N]: ";
      cin >> c;
      if((c == 'Y') || (c == 'y')) lBreak = true;
      else lBreak = false;
      gotoxy(CurX,CurY); clreol();
    } while(!lBreak);

    data[index].HorPolPhaseLawType = TableAngleLaw[Select].type;
    data[index].HorPolPhase = TableAngleLaw[Select].law;

    //Ввод параметров закона изменения фазы горизонтальной составляющей
    //поляризации внешнего сигнала
    const char* dimstr = GetMeasureName(TableAngleLaw[Select].M.DimFunc);
    gotoxy(X,Y);
    for(uint i = Y; i<=CurY; i++) { clreol(); cout << endl; }
    gotoxy(X,Y);
    cout << "HORIZONTAL POLARIZATION PHASE LAW: "
         << GetLawNotate(TableAngleLaw[Select].type) << " [" << dimstr
         << "]" << endl;

    //Определяем размерность закона, аргумента и параметров функции, характери-
    //зующих изменение фазы горизонтальной составляющей поляризации сигнала
    //во времени

    data[index].HorPolPhaseArg =
    TableAngleLaw[Select].inputfactor(TableAngleLaw[Select].M);

    gotoxy(X,Y);
    clreol();
    str = TableAngleLaw[Select].mathlawfactor(data[index].HorPolPhaseArg);
    cout << "HORIZ. POLAR. PHASE LAW: " << str << " [" << dimstr << "]" << endl;
    delete[] str;
    cout << "Have You accept this law? [Y/N]: ";
    cin >> c;
    CurY = wherey();
    if((c == 'Y') || (c == 'y')) lBreak = true;
    else lBreak = false;
    gotoxy(X,Y);
    for(uint i = Y; i<=CurY; i++) { clreol(); cout << endl; }
    gotoxy(X,Y);
  } while(!lBreak);
}
//*****************************************************************************
//СТАТУС: I.9., private, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ: void PhaseLawInPut(uint index,uint language)
//НАЗНАЧЕНИЕ:
//Функция предназначена для ввода данных по пространственной фазе
//компоненты внешнего сигнала с порядковым номером index.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. const uint index - порядковый номер компонента внешнего сигнала в массиве
//data
//2. uint language - язык интерфейса
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: нет
//*****************************************************************************
void ExtSignal::PhaseLawInPut(uint index,uint language)
{ uint X = 1;
  uint Y = wherey();
  uint CurX,CurY;
  uint Select;
  char c;
  char* str;
  bool lBreak;
  //Вывод списка законов по которым может изменяться во времени пространственная
  //фаза составляющей внешнего сигнала
  uint Count = AccessibleLaw(TableAngleLaw,TableAngleLawCount);
  uint Index;
  gotoxy(X,Y);
 do
  {
    cout << "*------------------------------------------------------------*\n";
    cout << "*             SELECT MODIFICATION LAW OF SPACE PHASE         *\n";
    cout << "*------------------------------------------------------------*\n";
    for(uint i = 0; i<Count; i++)
    { Index = GetIndexLaw(TableAngleLaw, TableAngleLawCount, i);
      cout << (i+1) << ". ";
      cout << GetLawName(TableAngleLaw[Index].type,language);
      cout << "\t: " << GetLawNotate(TableAngleLaw[Index].type) << endl;
    }
    cout << "*------------------------------------------------------------*\n";
   do
    { do
       { CurX = wherex(); CurY = wherey();
         cout << "Enter number [1 - " << Count << "]: ";
         cin >> Select;
         gotoxy(CurX,CurY); clreol();
       } while((Select > Count) || (Select < 1));
      Select--;
      Select = GetIndexLaw(TableAngleLaw, TableAngleLawCount, Select);
      cout << "Have You selected <";
      cout << GetLawName(TableAngleLaw[Select].type,language);
      cout << ": " << GetLawNotate(TableAngleLaw[Select].type) << "> ? [Y/N]: ";
      cin >> c;
      if((c == 'Y') || (c == 'y')) lBreak = true;
      else lBreak = false;
      gotoxy(CurX,CurY); clreol();
    } while(!lBreak);

    data[index].PhaseLawType = TableAngleLaw[Select].type;
    data[index].PhaseLaw = TableAngleLaw[Select].law;

    //Ввод параметров закона изменения пространственной фазы  составляющей
    //внешнего сигнала
    const char* dimstr = GetMeasureName(TableAngleLaw[Select].M.DimFunc);
    gotoxy(X,Y);
    for(uint i = Y; i<=CurY; i++) { clreol(); cout << endl; }
    gotoxy(X,Y);
    cout << "SPACE PHASE LAW: "
         << GetLawNotate(TableAngleLaw[Select].type) << " [" << dimstr
         << "]" << endl;

    //Определяем размерность закона, аргумента и параметров функции, характери-
    //зующих изменение пространственной фазы сигнала во времени

    data[index].PhaseArg =
    TableAngleLaw[Select].inputfactor(TableAngleLaw[Select].M);

    gotoxy(X,Y);
    clreol();
    str = TableAngleLaw[Select].mathlawfactor(data[index].PhaseArg);
    cout << "SPACE PHASE LAW: " << str << " [" << dimstr << "]" << endl;
    delete[] str;
    cout << "Have You accept this law? [Y/N]: ";
    cin >> c;
    CurY = wherey();
    if((c == 'Y') || (c == 'y')) lBreak = true;
    else lBreak = false;
    gotoxy(X,Y);
    for(uint i = Y; i<=CurY; i++) { clreol(); cout << endl; }
    gotoxy(X,Y);
  } while(!lBreak);
}
//*****************************************************************************
//СТАТУС: I.10., private, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ: void PhaseCasualLawInPut(uint index,uint language)
//НАЗНАЧЕНИЕ:
//Функция предназначена для выбора случайного закона изменения ФАЗЫ
//компоненты внешнего сигнала с порядковым номером index. Возможно исключение
//влияния случайных факторов на ФАЗУ сигнала. В этом случае необходимо
//выбрать опцию меню - "UnCasual law: F(t) = 0"
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. const uint index - порядковый номер компонента внешнего сигнала в массиве
//2. uint language - язык вывода информационных сообщений
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: нет
//*****************************************************************************
void ExtSignal::PhaseCasualLawInPut(uint index,uint language)
{ uint X = 1;
  uint Y = wherey();
  uint CurX,CurY;
  uint Select;
  char c;
  char* str;
  bool lBreak;
  //Вывод списка случайных законов по которым может изменяться во
  //времени фаза составляющей внешнего сигнала
  uint Count = AccessibleLaw(TableCasualLaw,TableCasualLawCount);
  uint Index;
  gotoxy(X,Y);
 do
  {
    cout << "*------------------------------------------------------------*\n";
    cout << "*            SELECT PHASE MODIFICATION CASUAL LAW            *\n";
    cout << "*------------------------------------------------------------*\n";
    for(uint i = 0; i<Count; i++)
    { Index = GetIndexLaw(TableCasualLaw, TableCasualLawCount, i);
      cout << (i+1) << ". ";
      cout << GetLawName(TableCasualLaw[Index].type,language);
      cout << "\t: " << GetLawNotate(TableCasualLaw[Index].type) << endl;
    }
    cout << "*------------------------------------------------------------*\n";
   do
    { do
       { CurX = wherex(); CurY = wherey();
         cout << "Enter number [1 - " << Count << "]: ";
         cin >> Select;
         gotoxy(CurX,CurY); clreol();
       } while((Select > Count) || (Select < 1));
      Select--;
      Select = GetIndexLaw(TableCasualLaw, TableCasualLawCount, Select);
      cout << "Have You selected <";
      cout << GetLawName(TableCasualLaw[Select].type,language);
      cout << ": " << GetLawNotate(TableCasualLaw[Select].type) << "> ? [Y/N]: ";
      cin >> c;
      if((c == 'Y') || (c == 'y')) lBreak = true;
      else lBreak = false;
      gotoxy(CurX,CurY); clreol();
    } while(!lBreak);

    data[index].PhaseCasualLawType = TableCasualLaw[Select].type;
    data[index].PhaseCasualLaw = TableCasualLaw[Select].law;

    //Инициализация параметров случайного закона изменения амплитуды
    //внешнего сигнала
    gotoxy(X,Y);
    for(uint i = Y; i<=CurY; i++) { clreol(); cout << endl; }
    gotoxy(X,Y);
    cout << "PHASE CASUAL LAW: "
         << GetLawNotate(TableCasualLaw[Select].type) << endl;

    //Определяем параметры (при необходимости) случайного закона, характеризую-
    //щих изменение фазы сигнала во времени
    data[index].PhaseCasualLawArg =
    TableCasualLaw[Select].inputfactor(TableCasualLaw[Select].M);

    gotoxy(X,Y);
    clreol();
    str = TableCasualLaw[Select].mathlawfactor(data[index].PhaseCasualLawArg);
    cout << "PHASE CASUAL LAW: " << str << endl;
    delete[] str;
    cout << "Have You accept this law? [Y/N]: ";
    cin >> c;
    CurY = wherey();
    if((c == 'Y') || (c == 'y')) lBreak = true;
    else lBreak = false;
    gotoxy(X,Y);
    for(uint i = Y; i<=CurY; i++) { clreol(); cout << endl; }
    gotoxy(X,Y);
  } while(!lBreak);
}
//*****************************************************************************
//СТАТУС: I.11., private, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ: bool ConsoleDataOutPut(uint index, bool dim = TDIM,
//                           bool confirm = false)
//НАЗНАЧЕНИЕ:
//Функция предназначена для вывода данных по всем характеристикам компоненты
//внешнего сигнала с порядковым номером index. Под индексом равным 0 массива
//data всегда хранится полезная составляющая внешнего сигнала.
//Функция может запрашивать у пользователя его согласие (несогласие) с выведен-
//ными данными. Запрос осуществляется, если confirm = true (режим по умолчанию -
//без запроса confirm == false).
//Параметры законов могут отображаться в двух размерностях:
//dim == TDIM в заданной размерности (по умолчанию), при которой происходят вы-
//числения величин изменения свойств компонент сигнала во времени
//dim == SDIM в исходной размерности, при которой параметры законов вводятся
//пользователем (с клавиатуры или из файла с данными).
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. const uint index - порядковый номер компонента внешнего сигнала в массиве
//                      компонент внешнего сигнала data
//2. bool dim - размерность, в которой отображаются параметры законов изменения
//              свойств компонент внешнего сигнала
//3. bool confirm - опция подтверждения выведенных характеристик сигнала
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: bool confirm - подтверждение выведенных данных
//*****************************************************************************
bool ExtSignal::ConsoleDataOutPut(uint index, bool dim, bool confirm)
{ uint X = 1;
  uint Y = wherey();
  uint Select, CurY;
  char c;
  const char* dimstr;
  char* str;
  LAW* Table = TableAngleLaw;
  uint Count = TableAngleLawCount;

  cout << "*******************************************************************\n";
  cout << "*            Performances  of  signal  component                  *\n";
  cout << "*******************************************************************\n";
  //Вывод общих характеристик компоненты сигнала
  cout << " I. TOTAL PROPERTIES" << endl;

  cout << "  1. Type: ";
  if (index == 0) cout << "DESIRED SIGNAL " << endl;
  else cout << "JAMMER N " << index << endl;

  cout << "  2. Usage: ";
  if (data[index].Usage == ON) cout << "YES" << endl;
  else cout << "NO" << endl;

  cout << "  3. Azimuth Angle: ";
  Select = GetIndexLawByType(Table, Count, data[index].AzAngleLawType);
  dimstr = GetMeasureName(TableAngleLaw[Select].M.DimFunc);
  if (dim != TDIM)
   InvertAllLawParam(TableAngleLaw+Select, data[index].AzAngleArg);
  str = TableAngleLaw[Select].mathlawfactor(data[index].AzAngleArg);
  cout << str << " [" << dimstr << "]" << endl;

  if (dim != TDIM)
   ConvertAllLawParam(TableAngleLaw+Select, data[index].AzAngleArg);
  delete str;

  cout << "  4. Elevate Angle: ";
  Select = GetIndexLawByType(Table, Count, data[index].ElevAngleLawType);
  dimstr = GetMeasureName(TableAngleLaw[Select].M.DimFunc);
  if (dim != TDIM)
   InvertAllLawParam(TableAngleLaw+Select, data[index].ElevAngleArg);
  str = TableAngleLaw[Select].mathlawfactor(data[index].ElevAngleArg);
  cout << str << " [" << dimstr << "]" << endl;
  if (dim != TDIM)
   ConvertAllLawParam(TableAngleLaw+Select, data[index].ElevAngleArg);
  delete str;

  //Вывод поляризационных свойств компоненты сигнала
  cout << " II. POLARIZING PROPERTIES" << endl;

  cout << "  5. Polarization Angle: ";
  Select = GetIndexLawByType(Table, Count, data[index].PolAngleLawType);
  dimstr = GetMeasureName(TableAngleLaw[Select].M.DimFunc);
  if (dim != TDIM)
   InvertAllLawParam(TableAngleLaw+Select, data[index].PolAngleArg);
  str = TableAngleLaw[Select].mathlawfactor(data[index].PolAngleArg);
  cout << str << " [" << dimstr << "]" << endl;
  if (dim != TDIM)
   ConvertAllLawParam(TableAngleLaw+Select, data[index].PolAngleArg);
  delete str;

  cout << "  6. Phase of Vert. Comp. Polar.: ";
  Select = GetIndexLawByType(Table, Count, data[index].VertPolPhaseLawType);
  dimstr = GetMeasureName(TableAngleLaw[Select].M.DimFunc);
  if (dim != TDIM)
   InvertAllLawParam(TableAngleLaw+Select, data[index].VertPolPhaseArg);
  str = TableAngleLaw[Select].mathlawfactor(data[index].VertPolPhaseArg);
  cout << str << " [" << dimstr << "]" << endl;
  if (dim != TDIM)
   ConvertAllLawParam(TableAngleLaw+Select, data[index].VertPolPhaseArg);
  delete str;

  cout << "  7. Phase of Hor. Comp. Polar.: ";
  Select = GetIndexLawByType(Table, Count, data[index].HorPolPhaseLawType);
  dimstr = GetMeasureName(TableAngleLaw[Select].M.DimFunc);
  if (dim != TDIM)
   InvertAllLawParam(TableAngleLaw+Select, data[index].HorPolPhaseArg);
  str = TableAngleLaw[Select].mathlawfactor(data[index].HorPolPhaseArg);
  cout << str << " [" << dimstr << "]" << endl;
  if (dim != TDIM)
   ConvertAllLawParam(TableAngleLaw+Select, data[index].HorPolPhaseArg);
  delete str;

  //Вывод пространственнных свойств компоненты сигнала
  cout << " III. SPACE PROPERTIES" << endl;

  cout << "  8. Space Amplitude: ";
  Table = TableAmplLaw; Count = TableAmplLawCount;
  Select = GetIndexLawByType(Table, Count, data[index].AmplLawType);
  dimstr = GetMeasureName(TableAmplLaw[Select].M.DimFunc);
  if (dim != TDIM)
   InvertAllLawParam(TableAmplLaw+Select, data[index].AmplArg);
  str = TableAmplLaw[Select].mathlawfactor(data[index].AmplArg);
  cout << str << " [" << dimstr << "]" << endl;
  if (dim != TDIM)
   ConvertAllLawParam(TableAmplLaw+Select, data[index].AmplArg);
  delete str;

  cout << "  9. Space Phase: ";
  Table = TableAngleLaw; Count = TableAngleLawCount;
  Select = GetIndexLawByType(Table, Count, data[index].PhaseLawType);
  dimstr = GetMeasureName(TableAngleLaw[Select].M.DimFunc);
  if (dim != TDIM)
   InvertAllLawParam(TableAngleLaw+Select, data[index].PhaseArg);
  str = TableAngleLaw[Select].mathlawfactor(data[index].PhaseArg);
  cout << str << " [" << dimstr << "]" << endl;
  if (dim != TDIM)
   ConvertAllLawParam(TableAngleLaw+Select, data[index].PhaseArg);
  delete str;

  //Вывод случайных составляющих АМПЛИТУДЫ и ФАЗЫ компоненты сигнала
  cout << " IV. CASUAL COMPONENTS AMPLITUDE AND PHASE" << endl;

  cout << "  10. Casual Amplitude: ";
  Table = TableCasualLaw; Count = TableCasualLawCount;
  Select = GetIndexLawByType(Table, Count, data[index].AmplCasualLawType);
  str = TableCasualLaw[Select].mathlawfactor(data[index].AmplCasualLawArg);
  cout << str << endl;
  delete str;

  cout << "  11. Casual Phase: ";
  Select = GetIndexLawByType(Table, Count, data[index].PhaseCasualLawType);
  str = TableCasualLaw[Select].mathlawfactor(data[index].PhaseCasualLawArg);
  cout << str << endl;
  cout << "*******************************************************************\n";
  delete str;
  if (confirm == false) //Без подтверждения
  { cout << "Press any key for continue..." << endl;
    getch();
    confirm = true;
  }
  else //Режим подтверждения
  {
   cout << "Have You accept this performances signal component? [Y/N]: ";
   cin >> c;
   if ((c == 'Y') || (c == 'y')) confirm = true;
   else confirm = false;
  }

  //Очистка экрана
  CurY = wherey();
  gotoxy(X,Y);
  for(uint i = Y; i <= CurY; i++) {clreol(); cout << endl;}
  gotoxy(X,Y);

 return confirm;
}
//*****************************************************************************
//СТАТУС: I.12., private, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ: ARGLIST* InvertAllLawParam(const LAW* Law, ARGLIST* Arg)
//НАЗНАЧЕНИЕ:
//Функция предназначена для преобразования всех параметров закона (без аргумента)
//из заданной размерности (TDIM) в исходную размерность (SDIM). Под исходной
//размерностью принимается размерность в которой пользователь вводил параметры
//законов изменения свойств компонент сигнала (напр. в DEG). Под заданной раз-
//мерностью понимается размерность параметров при которой осуществляются расчеты
//(напр. RAD). При вводе данных происходит автоматическое преобразование из
//исходной размерности в заданную. Этой причиной вызвана необходимость данной
//функции, которая позволяет отобразить параметры законов с исходными размернос-
//тями.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. const LAW* Law - закон изменения свойства компоненты внешнего сигнала
//2. ARGLIST* Arg - список параметров закона
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: ARGLIST* Arg - преобразованный список параметров закона
//из заданной размерности в исходную
//*****************************************************************************
ARGLIST* ExtSignal::InvertAllLawParam(const LAW* Law, ARGLIST* Arg)
{
 if ((Arg != NULL) && (Arg->Size > 1))
  for (uint i = 1; i < Arg->Size; i++)
   Arg->List[i] = Law->M.Factor[i].Invert(Arg->List[i]);

 return Arg;
}
//*****************************************************************************
//СТАТУС: I.13., private, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ: ARGLIST* ConvertAllLawParam(const LAW* Law, ARGLIST* Arg)
//НАЗНАЧЕНИЕ:
//Функция предназначена для преобразования всех параметров закона (без аргумента)
//из заданной размерности (TDIM) в исходную размерность (SDIM). Под исходной
//размерностью принимается размерность в которой пользователь вводил параметры
//законов изменения свойств компонент сигнала (напр. в DEG). Под заданной раз-
//мерностью понимается размерность параметров при которой осуществляются расчеты
//(напр. RAD). При вводе данных происходит автоматическое преобразование из
//исходной размерности в заданную. Этой причиной вызвана необходимость данной
//функции, которая позволяет отобразить параметры законов с исходными размернос-
//тями.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. const LAW* Law - закон изменения свойства компоненты внешнего сигнала
//2. ARGLIST* Arg - список параметров закона
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: ARGLIST* Arg - преобразованный список параметров закона
//из исходной размерности в заданную
//*****************************************************************************
ARGLIST* ExtSignal::ConvertAllLawParam(const LAW* Law, ARGLIST* Arg)
{
 if ((Arg != NULL) && (Arg->Size > 1))
  for (uint i = 1; i < Arg->Size; i++)
   Arg->List[i] = Law->M.Factor[i].Convert(Arg->List[i]);

 return Arg;
}

//*****************************************************************************
//СТАТУС: I.14., private, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ: bool FileDataEntry(const char* filename)
//НАЗНАЧЕНИЕ:
//Функция предназначена для ввода данных по всем характеристикам компонент
//внешнего сигнала из текстового файла.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. string filename - наименование файла с входными данными по внешнему сигналу
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: bool lSuccess
// true - данные считаны из файла успешно
// false - данные считать из файла не удалось
//*****************************************************************************
bool ExtSignal::FileDataEntry(const char* filename)
{ bool lSuccess = true;
  ulong CurRow = 0; //Текущий номер строки входного файла filename
  uint Y = wherey();
  uint CurY;
  string s;
  char c;
  fname.append(filename);
  //************************************************************************
  //ШАГ I.
  //Открываем текстовой файл с данными по EXTERNAL SIGNAL в режиме ЧТЕНИЯ
  //************************************************************************
  if (fname.length() == 0) //Имя входного файла с данными не задано
  { lSuccess = false;
    cout << "\nERROR: filename is absent." << endl;
    cout << "Enter new filename? [Y/N]: ";
    cin >> c;

    //Очистка экрана
    CurY = wherey();
    gotoxy(1,Y);
    for(uint i = Y; i <= CurY; i++) {clreol(); cout << endl;}
    gotoxy(1,Y);

    if ((c == 'Y') || (c == 'y')) lSuccess = GetInPutFilename();
    if (!lSuccess) return lSuccess;
  }
  //Открываем входной файл
  else
  { fin.open(fname.c_str(),ios::in);
    if (!fin)
    { lSuccess = false;
      cout << "\nERROR: unable to open <" << fname << ">" << endl;
      cout << "Try again ? [Y/N]: ";
      cin >> c;
      //Очистка экрана
      CurY = wherey();  gotoxy(1,Y);
      for(uint i = Y; i <= CurY; i++) {clreol(); cout << endl;}
      gotoxy(1,Y);

      if ((c == 'Y') || (c == 'y')) lSuccess = GetInPutFilename();
      if (!lSuccess) return lSuccess;
    }
    else //Файл с именем filename открыт в режиме чтения
    { cout << "You open file <" << fname << ">" << endl;
      //Очистка экрана
      CurY = wherey();  gotoxy(1,Y);
      for(uint i = Y; i <= CurY; i++) {clreol(); cout << endl;}
      gotoxy(1,Y);
    }
  }
  //**************************************************************************
  //ШАГ II. Открываем файл регистрации ошибок, которые могут быть обнаружены
  //        при считывании данных из входного файлового потока fin - signal.err
  //**************************************************************************
  //Извлечение имени входного файла без расширения
  uint pos = fname.find(".");
  //Входной файл не имеет расширения
  if (pos == string::npos) ferrname = fname+".err";
  else //Файл имеет расширение
  { s = fname.substr(pos);
    pos = s.find(".err");
    if (pos != string::npos) //Входной файл имеет недопустимое расширение ".err"
    {lSuccess = false;
     #if defined(CONSOLE)
      cout << "\nERROR IN INPUT DATA FILE NAME !!!" << endl;
      cout << "Input data file <" << fname << "> contains the inadmissibles "
           << "extension - .err." << endl;
     #endif
     return lSuccess;
    }
   //Формирование имени файла регистрации ошибок ferrname с именем совпадающим
   //с именем входного файла данных fname и расширением ".err"
   pos = fname.find(".");
   ferrname = fname.substr(0,pos)+".err";
  }
  ferr.open(ferrname.c_str(),ios::out);
  s.assign(67,' ');
  ferr<<"===================================================================" << endl;
  ferr<<"*ФАЙЛ РЕГИСТРАЦИИ ОШИБОК, ВОЗНИКШИХ ПРИ СЧИТЫВАНИИ ДАННЫХ ИЗ ФАЙЛА*" << endl;
  s.insert((s.length()-fname.length())/2,fname);//to_upper(fname)
  ferr << s << endl;
  ferr<<"===================================================================" << endl;
  //***************************************************************************
  //ШАГ III. Определяем структуру внешнего сигнала:
  //         DESIRED SIGNAL (1) + N (JAMMERS)
  //Во входном файле должна присутствовать строка, содержащая число помех вида
  //JAMMER = 2. Эта строка не должна быть закомментированной символами '//'
  //***************************************************************************
  //Получение из файла fname количества помех, присутствующих во внешнем сигнале
  if (GetNoiseCountFromFile(CurRow))
  { //Количество составляющих внешнего сигнала с учетом полезного сигнала
    Count += 1;
    //Распределение памяти под компоненты внешнего сигнала
    CreateArray(Count);
    //Начальная установка всех характеристик внешнего сигнала
    for (uint i = 0; i < Count; i++) ResetProperty(i);

    if (TableAngleLawCount == 0) CreateTableOfAngleLaw();
    if (TableAmplLawCount == 0) CreateTableOfAmplLaw();
    if (TableCasualLawCount == 0) CreateTableOfCasualLaw();
  }
  //Получить данные из файла fname по числу помех не удалось
  else
  { lSuccess = false;
    cout << "\nERROR: Error in file <" << uppercase << fname << ">" << endl;
    cout << "To receive data on noises amount it was not possible" << endl;
    cout << "Press any key...";
    getch();
    return lSuccess;
  }
  //***************************************************************************
  // ШАГ IV. Считываем из файла fname данные по каждому компоненту внешнего
  //         сигнала. Первым компонентом всегда должен быть ПОЛЕЗНЫЙ СИГНАЛ.
  //***************************************************************************
  uint i;
  for (i = 0; i < Count; i++)
  {
   clrscr();
   cout << "Reading data from file about ";
   if (i == 0) cout << "< DESIRED SIGNAL > ..." << endl;
   else cout << "< JAMMER_" << i << " > ..." << endl;
   lSuccess = GetCompSignalFromFile(i, CurRow);
   if (lSuccess == false) //Ошибка чтения данных о компоненте сигнала
   { cout << "\nERROR: Error in file <" << uppercase << fname << ">" << endl;
     cout << "Error at deriving of signal component performance from the file."
          << endl;
     cout << "The information about an error is placed in an error log file "
          << "< SIGNAL.ERR > ." << endl;
     cout << "Press any key...";
     getch();
     clrscr();
     break;
   }
  }

 clrscr();
return lSuccess;
}
//*****************************************************************************
//СТАТУС: I.15., private, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ: bool GetInPutFilename()
//НАЗНАЧЕНИЕ:
//Функция предназначена для открытия входного файлового потока в режиме
//текстовом режиме и только для чтения. Имя файла хранится в защищенной перемен-
//ной класса ExtSignal - fname, входной файловый поток выражается через защищен-
//ную переменную класса ExtSignal - fin.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ: нет
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: bool lSuccess
// true - текстовой файл открыть удалось в режиме чтения
// false - текстовой файл открыть не удалось в режиме чтения
//*****************************************************************************
bool ExtSignal::GetInPutFilename()
{ bool lSuccess = true;
  bool lBreak = false;
  uint Y = wherey();
  uint CurY;
  char c;
  do
  { cout << "Enter filename: ";
    cin >> fname; //Ввод имени файла
    //Открываем файл с именем s в режиме чтения как текстовой файл
    fin.open(fname.c_str(),ios::in);
    //Проверка на открытие
    if (!fin) //Файл открыть не удалось
    { cout << "\nERROR: unable to open file <" << fname << ">" << endl;
      cout << "Enter new filename ? [Y/N]: ";
      cin >> c;
      //Прекратить попытки открыть файл
      if ((c != 'Y') && (c != 'y')) {lBreak = true; lSuccess = false;}
    }
    //Файл открыт, выходим из цикла
    else
    { lBreak = true; lSuccess = true;
      cout << "You open file <" << uppercase << fname << ">" << endl;
      cout << "Press any key... ";
      getch();
    }
    //Очистка экрана
    CurY = wherey(); gotoxy(1,Y);
    for(uint i = Y; i <= CurY; i++) {clreol(); cout << endl;}
    gotoxy(1,Y);
  } while (!lBreak);
return lSuccess;
}
//*****************************************************************************
//СТАТУС: I.16., private, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ: bool GetNoiseCountFromFile(ulong& CurRow)
//НАЗНАЧЕНИЕ:
//Функция предназначена для определения числа помех, которые присутствуют во
//внешнем сигнале по данным из входного файла fname. Строка, в которой должна
//располагаться эта информация должна содержать ключевое слово <JAMMER>, после
//которого должна следовать число, указывающее на количество помех. Например,
//< JAMMER = 4 >. Данная строка не должна быть закомментирована с помощью симво-
//лов "//". Если данную информацию найти не удалось, то функция возвращает false
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. ulong& CurRow - номер текущей строки файла fname
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: bool lSuccess
// true - количество помех в External Signal определено
// false - количество помех в External Signal определить не удалось
//*****************************************************************************
bool ExtSignal::GetNoiseCountFromFile(ulong& CurRow)
{ bool lSuccess = true;
  bool lFind = false;
  string s;
  ulong value;
  uint pos;
  uint endpos;
  //s.set_case_sensitive(0);
  //Цикл сканирования строк входного файлового потока fin
  //Поиск ключевого слова <JAMMER>
  while ((fin.eof() == 0) && (!lFind))
  { getline(fin,s,'\n');
    CurRow++;
    pos = s.find("//");
    if ( pos != 0 )
    { pos = (pos != string::npos) ? pos-1 : s.length();
      s = s.substr(0,pos);
      pos = s.find("JAMMER");
      if (pos != string::npos) lFind = true; //Ключевое слово найдено
    }
  }
  if (lFind == false) //Ключевое слово не найдено
  { lSuccess = false;
    ferr << endl;
    ferr << "ERROR: Noises amount to determine it was not possible." << endl;
    ferr << "REASON: The key word < JAMMER > not found in file <" << fname << ">"
         << endl;
    return lSuccess;
  }
  //Определение числа помех
  pos = s.find_first_of("123456789",pos+5);
  if (pos != string::npos) //Цифры найдены
  { endpos = s.find_first_not_of("0123456789",pos+1);
    s = s.substr(pos,endpos);
    char *endptr;
    value = strtol(s.c_str(),&endptr,10);
    if (value < 65534) //
     Count = value;
    else //Очень большое число помех
    { lSuccess = false;
      ferr << endl;
      ferr << "ERROR: Very large number of jammers = " << value << endl;
      ferr << "TIP: Total number admissible range of jammers = [0...65534]"
           << endl;
      return lSuccess;
    }
  }
  else //Число помех определить не удалось
  { lSuccess = false;
    ferr << endl;
    ferr << "ERROR: Jammers amount to determine it was not possible." << endl;
    ferr << "Line N " << CurRow << " <" << s << ">" << endl;
    ferr << "REASON: In this line total number of jammers not found"  << endl;
    return lSuccess;
  }
return lSuccess;
}
//*****************************************************************************
//СТАТУС: I.17., private, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ: bool GetCompSignalFromFile(uint index, ulong& CurRow)
//НАЗНАЧЕНИЕ:
//Функция предназначена для получения данных об очередной внешней компоненте
//сигнала с порядковым номером index из входного файла fname. Каждая компонента
//сигнала характеризуется 11 характеристиками. Первая компонента (index = 0)
//всегда считается полезным сигналом, остальные компоненты - помехи. Каждая
//характеристика составляющей сигнала проверяется на корректность. Если
//характеристика не является корректной, то считывание данных прекращается с вы-
//дачей сообщения об ошибке в файле signal.err.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. uint index - порядковый номер компоненты внешнего сигнала
//2. ulong& CurRow - номер текущей строки файла fname
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: bool lSuccess
// true - характеристики составляющей сигнала считаны корректно
// false - характеристики составляющей сигнала считаны не корректно
//*****************************************************************************
bool ExtSignal::GetCompSignalFromFile(uint index, ulong& CurRow)
{ bool lSuccess = true;
  bool lFind;
  ulong num;
  uint pos;
  string s;
  string digit = "0123456789";
  uint Property;

  //s.skip_whitespace(0);
  //Цикл считывания характеристик сигнала из входного файла
  for (Property = 1; Property <= 11; Property++)
  {
   lFind = false;
   num = 0;
   //Поиск строки, которая начинается с номера соответствующего номеру характе-
   //ристики сигнала, которая в данный момент определяется
   while ((fin.eof() == 0) && (!lFind))
   { getline(fin,s,'\n');
     CurRow++;
     pos = s.find("//");
     if (pos != 0)
     { pos = (pos != string::npos) ? pos : s.length();
       s = s.substr(0,pos);
       //Поиск первого непробельного символа в строке
       pos = s.find_first_not_of(" \t");
       if (pos != string::npos) //Символ отличный от пробела найден
       { s = s.substr(pos);
         //Первый отличный от пробела символ в строке должен быть цифрой
         pos = digit.find(s[0]);
         if (pos != string::npos) //Этот символ является цифрой
         {
          char *endptr;
          num = strtol(s.c_str(),&endptr,10);
          //Порядковый номер характеристики совпадает с ожидаемым
          if (num == Property)
          {//Поиск символа ':' - начало данных по очередному свойству компоненты
           //сигнала с порядковым номером num
           pos = s.find(":");
           if (pos == string::npos) //Ошибка! Символ ':' не найден
           {lSuccess = false;
            ferr << "ERROR: Incorrect format of signal component performance.\n";
            ferr << "Signal Component: " << index << endl;
            ferr << "Signal Property: " << SignalPropertyName[Property] << endl;
            ferr << "Line N " << CurRow << ": <" << s << ">" << endl;
            ferr << "REASON: The token ':' is not found - token of a\n";
            ferr << "        beginning of a signal component property." << endl;
            return lSuccess;
           }
           //Убираем начальные пробелы в строке характеристик
           if (pos != s.length()-1) pos = s.find_first_not_of(" \t",pos+1);
           else pos = string::npos;

           if (pos == string::npos) s = "ERROR";
           else s = s.substr(pos);
           //Убираем конечные пробелы в строке характеристик
           pos = s.find_last_not_of(" \t");
           s = s.substr(0,pos+1);
           //Определяем очередную характеристику компоненты сигнала, которая
           //соответствует свойству с номером Property
           lSuccess = GetPropertySignalFromFile(s,index,Property,CurRow);
           //Ошибка при считывании очередного свойства сигнала из файла
           if (lSuccess == false) return lSuccess;
           lFind = true;
          }
          else //Порядковый номер характеристики не совпадает с ожидаемым
          { lSuccess = false;
            ferr << "ERROR: The serial number signal property does't coincide "
                 << "expected number." << endl;
            ferr << "Signal Component: " << index << endl;
            ferr << "Signal Property: " << SignalPropertyName[Property] << endl;
            ferr << "Serial number: " << num << " Expected number: " << Property
                 << endl;
            ferr << "Line N " << CurRow << ": <" << s << ">" << endl;
            return lSuccess;
          }
         }
         else //Ошибка: Этот символ не цифра
         { lSuccess = false;
           ferr << "ERROR: Incorrect format of signal component performance.\n";
           ferr << "Signal Component: " << index << endl;
           ferr << "Signal Property: " << SignalPropertyName[Property] << endl;
           ferr << "Line N " << CurRow << ": <" << s << ">" << endl;
           ferr << "REASON: The line with signal property should begin "
                << "with figure" << endl;
           return lSuccess;
         }
       }

     }
   }//Конец цикла while cчитывания строк из файла

   //Достигнут конец файла, считаны не все характеристики компоненты сигнала
   //или считаны не все компоненты сигнала
   if (lFind == false)
   { lSuccess = false;
    if ((num != 11) && (num != 0))
    {ferr << "ERROR: End of file is reached. It was not possible to receive\n";
     ferr << "       all performances of a signal component." << endl;
     ferr << "Signal Component: " << index << endl;
     ferr << "Signal Property: " << SignalPropertyName[Property] << endl;
     ferr << "Line N " << CurRow << ": <" << s << ">" << endl;
    }
    else if (num == 0) //Считаны не все компоненты сигнала
    {ferr << "ERROR: End of file is reached. It was not possible to receive\n";
     ferr << "       all signal components." << endl;
     ferr << "Total Signal Components: " << Count << endl;
     ferr << "Getted Signal Components: " << index << endl;
    }
     return lSuccess;
   }
  }//Конец цикла for считывания характеристик сигнала - Property
return lSuccess;
}
//*****************************************************************************
//СТАТУС: I.18., private, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ: bool GetPropertySignalFromFile(string& s,
//                      uint CompSignal, uint PropSignal, ulong& CurRow)
//НАЗНАЧЕНИЕ:
//Функция предназначена для определения очередного свойства с порядковым номером
//Property компоненты внешнего сигнала. Каждая составляющая сигнала характери-
//зуется 11 свойствами. Структура характеристики компоненты сигнала следующая:
//1.  TYPE: [DESIRED SIGNAL || JAMMER]
//2.  USAGE: [YES || NO]
//3.  AZIMUTH ANGLE: {NameLAW, [Param_1],...[Param_N]}
//4.  ELEVATE ANGLE: {NameLAW, [Param_1],...[Param_N]}
//5.  POLARIZATION ANGLE: {NameLAW, [Param_1],...[Param_N]}
//6.  PHASE OF VERT. COMP. POLAR.: {NameLAW, [Param_1],...[Param_N]}
//7.  PHASE OF HOR. COMP. POLAR.: {NameLAW, [Param_1],...[Param_N]}
//8.  AMPLITUDE: {NameLAW, [Param_1],...[Param_N]}
//9.  PHASE: {NameLAW, [Param_1],...[Param_N]}
//10. CASUAL AMPLITUDE: {NameLAW, [Param_1],...[Param_N]}
//11. CASUAL PHASE: {NameLAW, [Param_1],...[Param_N]}
//Если при определении очередного свойства компоненты сигнала обнаруживаются
//ошибки, то работа функции завершается с отрицательным результатом и формирова-
//нием соответствующего сообщения в файле "signal.err"
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. string& s - строка с очередным свойством компоненты сигнала
//2. uint CompSignal - порядковый номер компоненты внешнего сигнала
//3. uint PropSignal - порядковый номер свойства компоненты внешнего сигнала
//4. ulong& CurRow - номер текущей строки файла fname
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: bool lSuccess
// true - очередная характеристика составляющей сигнала считана корректно
// false - очередная характеристика составляющей сигнала считана не корректно
//*****************************************************************************
bool ExtSignal::GetPropertySignalFromFile(string& s, uint CompSignal,
                                          uint PropSignal, ulong& CurRow)
{ bool lSuccess = true;
  uint pos;

  //s.set_case_sensitive(0);
  //***********************************************
  //1. Определение типа компоненты сигнала - TYPE
  //***********************************************
  if (PropSignal == 1)
  { if (CompSignal == 0) //Полезный сигнал
    { pos = s.find("DESIRED SIGNAL");
      if (pos == string::npos) pos = s.find("SIGNAL");
      //Ошибка! Неправильный идентификатор компоненты сигнала
      if (pos == string::npos)
      {lSuccess = false;
       ferr << "ERROR: The incorrect identifier of a signal component." << endl;
       ferr << "Signal Component: " << CompSignal << endl;
       ferr << "Signal Property: " << SignalPropertyName[PropSignal] << endl;
       ferr << "Line N " << CurRow << ": <" << s << ">" << endl;
       ferr << "REASON: Expected <DESIRED SIGNAL> or <SIGNAL> for the first\n";
       ferr << "        component external signal" << endl;
       return lSuccess;
      }
      data[CompSignal].Type = SIGNAL;
    }
    else if(CompSignal != 0) //Помеха
    { pos = s.find("JAMMER");
      if (pos == string::npos) //Ошибка! Неправильный идентификатор компоненты сигнала
      {lSuccess = false;
       ferr << "ERROR: The incorrect identifier of a signal component." << endl;
       ferr << "Signal Component: " << CompSignal << endl;
       ferr << "Signal Property: " << SignalPropertyName[PropSignal] << endl;
       ferr << "Line N " << CurRow << ": <" << s << ">" << endl;
       ferr << "REASON: Expected <JAMMER> for this\n";
       ferr << "        component external signal" << endl;
       return lSuccess;
      }
      data[CompSignal].Type = JAMMER;
    }
  }
  //*********************************************************
  //2. Использование компоненты сигнала в расчетах - USAGE
  //*********************************************************
  else if (PropSignal == 2)
  { pos = s.find("YES");
    if (pos == string::npos)
    { pos = s.find("NO");
      //Флаг использования или не использования компоненты сигнала отсутствует
      if (pos == string::npos) //Ошибка!
      { lSuccess = false;
        ferr << "ERROR: Can't find indicator of use of a signal component\n";
        ferr << "       at calculation." << endl;
        ferr << "Signal Component: " << CompSignal << endl;
        ferr << "Signal Property: " << SignalPropertyName[PropSignal] << endl;
        ferr << "Line N " << CurRow << ": <" << s << ">" << endl;
        ferr << "REASON: Expected <YES> or <NO> for this ";
        ferr << "component external signal" << endl;
        return lSuccess;
      }
      else //Компонент сигнала не будет использоваться в расчетах
       data[CompSignal].Usage = OFF;
    }
    else //Компонент сигнала будет использоваться в расчетах
     data[CompSignal].Usage = ON;
  }
  //****************************************************************
  //3. Определение законов изменения и их параметров амплитуды и
  //   фазы компоненты внешнего сигнала (3 - 11 свойства сигнала)
  //****************************************************************
  else
  { string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    //Определяем первый непробельный символ в строке характеристики сигнала.
    //Данный символ должен быть буквой англ. алфавита, т.к. наименование законов
    //должны всегда начинаться с буквы. Другие символы не допустимы.
    pos = s.find_first_not_of(" \t\n");
    if (pos != string::npos) //Cтрока не пустая
     pos = alphabet.find_first_of(s[pos]); //Должна быть буква
    //Ошибка! Строка пустая либо первый непробельный символ не буква
    if (pos == string::npos) //Обработка ошибки
    { lSuccess = false;
      ferr << "ERROR: Incorrect format of signal component performance.\n";
      ferr << "Signal Component: " << CompSignal << endl;
      ferr << "Signal Property: " << SignalPropertyName[PropSignal] << endl;
      ferr << "Line N " << CurRow << ": <" << s << ">" << endl;
      ferr << "REASON: Expected modification law name of "
           << SignalPropertyName[PropSignal] << "." << endl;
      ferr << "The law name should begin with token <" << alphabet << ">" << endl;
      return lSuccess;
    }
    //Поиск считанной из файла аббревиатуры закона в таблице наименований законов
    //LawName[] с целью получения идентификатора закона type по его аббревиатуре
    //и инициализации соответствующего свойства компоненты сигнала. Если аббре-
    //виатуру закона найти не удалось, то генерируется ошибка.
    bool detlaw;
    uint LawType;
    string abbr;

    pos = s.find_first_not_of(" \t\n");
    s = s.substr(pos);
    abbr = s;
    pos = abbr.find_first_of(" ,;\t\n",pos);
    //Выделяем аббревиатуру закона из строки свойств компоненты сигнала
    abbr = abbr.substr(0,pos);
    detlaw = (PropSignal < 10) ? DETLAW : CASUALLAW;
    //Поиск идентификатора закона по его аббревиатуре в таблице LawName
    LawType = GetTypeLawByAbbrLaw(abbr,detlaw);
    if (LawType == UNEXIST) //Ошибка! Такой аббревиатуры закона нет
    { lSuccess = false;
      ferr << "ERROR: Incorrect law name abbreviate of signal component " <<
              "performance." << endl;
      ferr << "Signal Component: " << CompSignal << endl;
      ferr << "Signal Property: " << SignalPropertyName[PropSignal] << endl;
      ferr << "Line N " << CurRow << ": <" << abbr << ">" << endl;
      ferr << "The list of possible abbreviations of ";
      if (detlaw == DETLAW) ferr << "determine laws:" << endl;
      else ferr << "causal laws:" << endl;
      s = GetAbbrLaw(s, detlaw);
      uint i = 1;
      bool lBreak = false;
      do
      { pos = s.find_first_of(";");
        if (pos == string::npos) lBreak = true;
        ferr << setw(2) << i++ << ". " << s.substr(0,pos) << endl;
        if (pos == s.length()-1) lBreak = true;
        else s = s.substr(++pos);
      } while (!lBreak);
      return lSuccess;
    }
 //Идентификатор закона изменения заданного свойства компоненты сигнала
 //определен. Поиск закона в таблице законов TableAngleLaw, TableAmplLaw и
 //TableCasualLaw в зависимости от характеристики компоненты сигнала по LawType
   uint Index;
   LAW* PropLaw;
   ARGLIST* Arg = NULL;
   if (pos != string::npos) s = s.substr(pos);
   else s = " ";
   //Поиск в TableAngleLaw
   if ((PropSignal < 10) && (PropSignal != 8))
    {Index = GetIndexLawByType(TableAngleLaw,TableAngleLawCount,LawType);
     PropLaw = (TableAngleLaw+Index);
      switch (PropSignal)
      { case 3 : //Азимутальный угол
         data[CompSignal].AzAngleLawType = TableAngleLaw[Index].type;
         data[CompSignal].AzimuthAngle = TableAngleLaw[Index].law;
         //Выделяем память под размещение параметров закона
         data[CompSignal].AzAngleArg = CreateArgList(PropLaw->M.Size);
         Arg = data[CompSignal].AzAngleArg;
         //Определение параметров закона изменения угла по азимуту сигнала
         lSuccess = GetArgLawFromFile(s,PropLaw,Arg,CompSignal,PropSignal,CurRow);
         if (lSuccess == false) return lSuccess;
         break;
        case 4 : //Угол места источника сигнала
         data[CompSignal].ElevAngleLawType = TableAngleLaw[Index].type;
         data[CompSignal].ElevateAngle = TableAngleLaw[Index].law;
         //Выделяем память под размещение параметров закона
         data[CompSignal].ElevAngleArg = CreateArgList(PropLaw->M.Size);
         Arg = data[CompSignal].ElevAngleArg;
         //Определение параметров закона изменения угла места сигнала
         lSuccess = GetArgLawFromFile(s,PropLaw,Arg,CompSignal,PropSignal,CurRow);
         if (lSuccess == false) return lSuccess;
         break;
        case 5 : //Угол поляризационного отношения сигнала
         data[CompSignal].PolAngleLawType = TableAngleLaw[Index].type;
         data[CompSignal].PolAngle = TableAngleLaw[Index].law;
         //Выделяем память под размещение параметров закона
         data[CompSignal].PolAngleArg = CreateArgList(PropLaw->M.Size);
         Arg = data[CompSignal].PolAngleArg;
         //Определение параметров закона изменения угла поляризационного отн-я
         lSuccess = GetArgLawFromFile(s,PropLaw,Arg,CompSignal,PropSignal,CurRow);
         if (lSuccess == false) return lSuccess;
         break;
        case 6 : //Фаза вертикальной составляющей поляризации сигнала
         data[CompSignal].VertPolPhaseLawType = TableAngleLaw[Index].type;
         data[CompSignal].VertPolPhase = TableAngleLaw[Index].law;
         //Выделяем память под размещение параметров закона
         data[CompSignal].VertPolPhaseArg = CreateArgList(PropLaw->M.Size);
         Arg = data[CompSignal].VertPolPhaseArg;
         //Определение параметров закона изменения фазы вертикальной составляю-
         //щей поляризации сигнала
         lSuccess = GetArgLawFromFile(s,PropLaw,Arg,CompSignal,PropSignal,CurRow);
         if (lSuccess == false) return lSuccess;
         break;
        case 7 : //Фаза горизонтальной составляющей поляризации сигнала
         data[CompSignal].HorPolPhaseLawType = TableAngleLaw[Index].type;
         data[CompSignal].HorPolPhase = TableAngleLaw[Index].law;
         //Выделяем память под размещение параметров закона
         data[CompSignal].HorPolPhaseArg = CreateArgList(PropLaw->M.Size);
         Arg = data[CompSignal].HorPolPhaseArg;
         //Определение параметров закона изменения фазы горизонтальной составляю-
         //щей поляризации сигнала
         lSuccess = GetArgLawFromFile(s,PropLaw,Arg,CompSignal,PropSignal,CurRow);
         if (lSuccess == false) return lSuccess;
         break;
        case 9 : //Пространственная фаза сигнала
         data[CompSignal].PhaseLawType = TableAngleLaw[Index].type;
         data[CompSignal].PhaseLaw = TableAngleLaw[Index].law;
         //Выделяем память под размещение параметров закона
         data[CompSignal].PhaseArg = CreateArgList(PropLaw->M.Size);
         Arg = data[CompSignal].PhaseArg;
         //Определение параметров закона изменения пространственной фазы сигнала
         lSuccess = GetArgLawFromFile(s,PropLaw,Arg,CompSignal,PropSignal,CurRow);
         if (lSuccess == false) return lSuccess;
         break;
      }
   }
   else if (PropSignal == 8) //Амплитуда сигнала
   { Index = GetIndexLawByType(TableAmplLaw,TableAmplLawCount,LawType);
     PropLaw = (TableAmplLaw+Index);
     data[CompSignal].AmplLawType = TableAmplLaw[Index].type;
     data[CompSignal].AmplLaw = TableAmplLaw[Index].law;
     //Выделяем память под размещение параметров закона
     data[CompSignal].AmplArg = CreateArgList(PropLaw->M.Size);
     Arg = data[CompSignal].AmplArg;
     //Определение параметров закона изменения амплитуды сигнала
     lSuccess = GetArgLawFromFile(s,PropLaw,Arg,CompSignal,PropSignal,CurRow);
     if (lSuccess == false) return lSuccess;
   }
   else if (PropSignal > 9)
   { Index = GetIndexLawByType(TableCasualLaw,TableCasualLawCount,LawType);
     PropLaw = (TableCasualLaw+Index);
     switch (PropSignal)
     { case 10 : //Случайный закон изменения амплитуды сигнала
        data[CompSignal].AmplCasualLawType = TableCasualLaw[Index].type;
        data[CompSignal].AmplCasualLaw = TableCasualLaw[Index].law;
        //Выделяем память под размещение параметров закона
        data[CompSignal].AmplCasualLawArg = CreateArgList(PropLaw->M.Size);
        Arg = data[CompSignal].AmplCasualLawArg;
        //Определение параметров закона изменения случайной амплитуды сигнала
        lSuccess = GetArgLawFromFile(s,PropLaw,Arg,CompSignal,PropSignal,CurRow);
        if (lSuccess == false) return lSuccess;
        break;
       case 11 : //Случайный закон изменения фазы сигнала
        data[CompSignal].PhaseCasualLawType = TableCasualLaw[Index].type;
        data[CompSignal].PhaseCasualLaw = TableCasualLaw[Index].law;
        //Выделяем память под размещение параметров закона
        data[CompSignal].PhaseCasualLawArg = CreateArgList(PropLaw->M.Size);
        Arg = data[CompSignal].PhaseCasualLawArg;
        //Определение параметров закона изменения случайной фазы сигнала
        lSuccess = GetArgLawFromFile(s,PropLaw,Arg,CompSignal,PropSignal,CurRow);
        if (lSuccess == false) return lSuccess;
        break;
     }
   }
  }
return lSuccess;
}
//*****************************************************************************
//СТАТУС: I.19., private, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ:
//bool GetArgLawFromFile(string& s, LAW* PropLaw, ARGLIST* Arg, uint CompSignal,
//                       uint PropSignal, ulong& CurRow);
//НАЗНАЧЕНИЕ:
//Функция предназначена для получения параметров закона изменения характеристики
//компоненты внешнего сигнала. Все параметры могут быть числами типа double.
//Параметры могут располагаться на нескольких строках. При этом допускаются сле-
//дующие разделители между значениями параметров: <Space,;Tab>, другие символы
//в строках где располагаются параметры не допускаются (если только они не заком-
//ментированы символами //). В случае некорректного задания формата строк, содер-
//жащих параметры закона или несоответствия их числа - требуемому количеству,
//генерируется ошибка с записью возможных причин в файл signal.err. Если число
//параметров превышает необходимое число, то они игнорируются. Если лишние пара-
//метры продолжают следовать на других строках, то это приведет к ошибке считыва-
//ния очередного свойства компонента сигнала. Считываемые параметры автоматически
//преобразуются в размерности, которые определены в правилах хранения и перевода
//исходных значений величин в заданные для законов, хранящихся в таблицах зако-
//нов (например, перевод из DEG to RAD, из Hz to RAD/SEC и т.п.). Успешно считан-
//ные параметры заносятся в список параметров законов соответствующих характерис-
//тик компонент внешнего сигнала. Список аргументов типа ARGLIST создается дан-
//ной функцией. Для всех детерминированных законов запись параметров в ARGLIST
//начинается со второго элемента списка, т.к. первым в таком списке всегда хра-
//нится аргумент функции - переменная величина. Для случайных законов такой спи-
//сок может отсутствовать совсем, т.к. они не зависят от аргумента (например,t).
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. string& s - строка с параметрами закона изменения свойства сигнала
//2. LAW* PropLaw - закон изменения свойства PropSignal компоненты сигнала
//3. ARGLIST* Arg - список параметров закона PropLaw
//4. uint CompSignal - порядковый номер компоненты внешнего сигнала
//5. uint PropSignal - порядковый номер свойства компоненты внешнего сигнала
//6. ulong& CurRow - номер текущей строки файла fname
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: bool lSuccess
// true  - параметры закона изменения свойства составляющей сигнала считаны
//         корректно
// false - параметры закона изменения свойства составляющей сигнала считаны
//         не корректно
//*****************************************************************************
bool ExtSignal::GetArgLawFromFile(string& s, LAW* PropLaw, ARGLIST* Arg,
                uint CompSignal, uint PropSignal, ulong& CurRow)
{ bool lSuccess = true;
  uint Count = PropLaw->M.Size; //Количество параметров закона

  //Вероятностный закон без параметров: случайная амплитуда или фаза сигнала
  if ((PropSignal > 9) && (Count == 1)) return lSuccess;

  //Создание списка аргументов
  //Arg = CreateArgList(Count);
  //Детерминированный закон без параметров, только с аргументом (напр., ZERO_LAW)
  if ((Count == 1) && (PropSignal < 10)) return lSuccess;

  //Определение фактического количества параметров закона и начального индекса,
  //с которого они должны заноситься в список ARGLIST, в зависимости от типа
  //закона: детерминированного или вероятностного.
  uint offset = 1;
  Count  = Count-1;
  uint GetParam = 0; //Количество считанных параметров
  uint pos;
  string separator = " ,;\t";
  string realnum = "-+.0123456789Ee";
  string sep_rnum(" ,;\t-+.0123456789Ee");
  bool lFind;
  //Считывание параметров закона изменения характеристики компоненты сигнала
  //из файла с исходными данными

  while (GetParam != Count)
  {//Получаем первый символ, не отноcящийся к символам-разделителям
   pos = s.find_first_not_of(separator);
   //Символ отличный от разделителя найти в данной строке не удалось
   if (pos == string::npos)
   { //Считываем новую строку из файла, исключаем комментарии
     lFind = false;
     while ((fin.eof() == 0) && (!lFind))
     { getline(fin,s,'\n');
       CurRow++;
       pos = s.find("//");
       if (pos != 0)
       { pos = (pos != string::npos) ? pos : s.length();
         s = s.substr(0,pos);
         //Поиск первого символа в строке, не относящегося к разделителям
         pos = s.find_first_not_of(separator);
         if (pos != string::npos) //Символ отличный от разделителя найден
          lFind = true;
       }
     }//Конец цикла считывания строк из файла
      //Достигнут конец файла, считаны не все параметры закона изменения
      //компоненты сигнала
      if (lFind == false)
      { lSuccess = false;
       ferr << "ERROR: End of file is reached. It was not possible to receive\n";
       ferr << "       all parameters of law modification of a signal component."
            << endl;
       ferr << "Signal Component: " << CompSignal << endl;
       ferr << "Signal Property: " << SignalPropertyName[PropSignal] << endl;
       ferr << "Line N " << CurRow << ": <" << s << ">" << endl;
       ferr << "Abbreviate Law: " << GetAbbrLawByType(PropLaw->type) << endl;
       ferr << "Total Law Parameters: " << Count << " without argument" <<endl;
       ferr << "Getted Law Parameters: " << GetParam << endl;
       DeleteArgList(Arg);
       return lSuccess;
      }
   }
   //Символ отличный от символа-разделителя найден
   //Анализируем символ на допустимый символ представления вещественного числа
   //Ошибка недопустимый символ в строке параметров закона
   if (s.find_first_not_of(sep_rnum) != string::npos) //realnum+separator
   { lSuccess = false;
     ferr << "ERROR: Incorrect token in law parameters line of signal component"
          <<" performance." << endl;
     ferr << "Signal Component: " << CompSignal << endl;
     ferr << "Signal Property: " << SignalPropertyName[PropSignal] << endl;
     ferr << "Abbreviate Law: " << GetAbbrLawByType(PropLaw->type) << endl;
     ferr << "Total Law Parameters: " << Count << " without argument" <<endl;
     ferr << "Getted Law Parameters: " << GetParam << endl;
     ferr << "Line N " << CurRow << ": <" << s << ">" << endl;
     ferr << "EXPECTED TOKENS: " << endl;
     ferr << "SEPAROTORS: " << "<" <<separator << ">" << endl;
     ferr << "REALNUMBER TOKENS: " << "<" <<realnum << ">" << endl;
    DeleteArgList(Arg);
    return lSuccess;
   }
   s = s.substr(pos);
   double value;
   char *endptr;
   //Преобразуем параметр закона из символьного представления в вещественное
   //типа double
   value = strtod(s.c_str(), &endptr);
   //Анализируем полученное значение
   if ((value == +HUGE_VAL) || (value == -HUGE_VAL) || (endptr == 0))
   {//Неправильное число
    lSuccess = false;
    ferr << "ERROR: Incorrect token in law parameters line of signal component"
         <<" performance." << endl;
    ferr << "Signal Component: " << CompSignal << endl;
    ferr << "Signal Property: " << SignalPropertyName[PropSignal] << endl;
    ferr << "Abbreviate Law: " << GetAbbrLawByType(PropLaw->type) << endl;
    ferr << "Total Law Parameters: " << Count << " without argument" <<endl;
    ferr << "Getted Law Parameters: " << GetParam << endl;
    ferr << "Line N " << CurRow << ": <" << s << ">" << endl;
    ferr << "EXPECTED TOKENS: " << endl;
    ferr << "SEPARATORS: " << "<" << separator << ">" << endl;
    ferr << "REALNUMBER TOKENS: " << "<" <<realnum << ">" << endl;
    DeleteArgList(Arg);
    return lSuccess;
   }
   //Преобразуем полученное значение из исходной размерности в заданную
   value = PropLaw->M.Factor[offset+GetParam].Convert(value);
   //Заносим параметр закона в список параметров
   Arg->List[offset+GetParam] = value;
   //Получаем cледующий параметр
   GetParam++;
   pos = s.find_first_of(separator);
   if (pos == string::npos) s = " ";
   else s = s.substr(pos);
  }//Конец цикла считывания параметров закона
return lSuccess;
}
//----------------------------------------------------------------------------

//*****************************************************************************
// II. ОБЩЕДОСТУПНЫЕ ФУНКЦИИ - ЧЛЕНЫ КЛАССА ExtSignal
//    PUBLIC MEMBER - FUNCTION ExtSignal CLASS
//*****************************************************************************

//*****************************************************************************
//СТАТУС: II.1; public; constructor by default
//НАИМЕНОВАНИЕ ФУНКЦИИ: ExtSignal()
//НАЗНАЧЕНИЕ: Конструктор по умолчанию класса ExtSignal.
//Функция предназначена для создания класса ExtSignal по умолчанию. В данном ре-
//жиме создания объекта класса осуществляется консольный ввод характеристик всех
//компонент внешнего сигнала
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ: нет
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: нет
//*****************************************************************************
ExtSignal::ExtSignal()
{
 data = NULL; Count = 0; fname.resize(0);
 //Создание объекта класса в режиме консольного ввода
 #if defined(CONSOLE)
  clrscr(); //Очистка экрана дисплея
  cout << "*******************************************************************\n";
  cout << "*           Designing of an EXTERNAL SIGNAL by default.           *\n";
  cout << "* All performances component of external signal are necessary for *\n";
  cout << "* entering from the keyboard.                                     *\n";
  cout << "*******************************************************************\n";
  cout << "* EXTERNAL SIGNAL is always ONE useful signal and N of parasites. *\n";
  cout << "* The number of parasites N can be in a range from 0 up to 65535. *\n";
  cout << "*******************************************************************\n";
  cout << endl;
  cout << "Enter total of parasites N: ";
  cin >> Count;
  //Количество составляющих внешнего сигнала с учетом полезного сигнала
  Count += 1;
  //Распределение памяти под компоненты внешнего сигнала
  CreateArray(Count);
  //Начальная установка всех характеристик внешнего сигнала
  for (uint i = 0; i < Count; i++) ResetProperty(i);

  //Создание таблицы возможных законов изменения азимутального угла и угла
  //места источников внешнего сигнала, а также амплитудной и фазовой (горизон-
  //тальной и вертикальной) составляющей угла поляризации сигнала.
  if (TableAngleLawCount == 0) CreateTableOfAngleLaw();

  //Создание таблицы детерминированных законов изменения во времени амплитудной
  //составляющей компоненты внешнего сигнала
  if (TableAmplLawCount == 0) CreateTableOfAmplLaw();

  //Создание таблицы случайных законов изменения АМПЛИТУДНОЙ и ФАЗОВОЙ характе-
  //ристики компоненты внешнего сигнала
  if (TableCasualLawCount == 0) CreateTableOfCasualLaw();

  clrscr(); gotoxy(1,1);
  cout << "********************************************************************"
       << endl;
  cout << " EXTERNAL SIGNAL STRUCTURE: Desired Signal - 1, Interference - "
       << (Count-1) << endl;
  cout << "********************************************************************"
       << endl;
  //Ввод свойств каждой компоненты внешнего сигнала с клавиатуры
  for (uint i = 0; i < Count; i++)
   do ConsoleDataEntry(i);
   //Вывод характеристик компоненты с подтверждением
   while(!ConsoleDataOutPut(i,SDIM,true));
  clrscr();
 #endif
}
//*****************************************************************************
//СТАТУС: II.2; public; constructor with argument
//НАИМЕНОВАНИЕ ФУНКЦИИ: ExtSignal(const char* filename)
//НАЗНАЧЕНИЕ: Конструктор с аргументом класса ExtSignal.
//Функция предназначена для создания класса ExtSignal по данным находящимся в
//текстовом файле filename. Этот файл специально организован. При считывании
//данных из этого файла осуществляется проверка его корректности. В случае оши-
//бок или несоответствия структуры файла требуемому формату генерируется прину-
//дительное завершение работы конструктора, ошибка, вызвавшая прерывание регист-
//рируется в файле signal.err
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ: const char* filename - имя входного файла
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: нет
//*****************************************************************************
ExtSignal::ExtSignal(const char* filename)
{bool lSuccess;

 Count = 0; data = NULL;
 fname.resize(0);
 lSuccess = FileDataEntry(filename);
 if (lSuccess == true)
 {fin.close();
  //Удаление созданного файла регистрации ошибок
  ferr.close();
  remove(ferrname.c_str());
  #if defined(CONSOLE)
   clrscr();
   cout << "The input data on EXTERNAL SIGNAL components from file <" << fname
        << ">" << "\nare read out successfully." << endl;
   cout << "Total noise: " << (Count-1) << endl;
   cout << "Press any key ...";
   getch();
  #endif
 }
 else if (lSuccess == false) //Ошибка при считывании данных из файла
 {
  #if defined(CONSOLE)
   cout << endl;
   cout << "The error is detected at reading of datas from the file." << endl;
   cout << "The EXTERNAL SIGNAL design on file data is impossible." << endl;
   cout << "The program will be terminated." << endl;
   cout << "Press any key ...";
   getch();
  #endif

  #if defined(CONSOLE)
   cout << "\nDestroy object of EXTERNAL SIGNAL class..." << endl;
   cout << "Delete law tables." << endl;
  #endif
  fname.resize(0);
  DeleteTable(TableAngleLaw, TableAngleLawCount);
  DeleteTable(TableAmplLaw, TableAmplLawCount);
  DeleteTable(TableCasualLaw, TableCasualLawCount);

  #if defined(CONSOLE)
   cout << "Delete " << Count << " components EXTERNAL SIGNAL." << endl;
  #endif
  for (uint i = 0; i < Count; i++) ResetProperty(i);
  delete[] data;
  Count = 0;

  exit(-1);
 }
}

//*****************************************************************************
//СТАТУС: II.3; public; class destructor
//НАИМЕНОВАНИЕ ФУНКЦИИ: ~ExtSignal()
//НАЗНАЧЕНИЕ: Деструктор класса ExtSignal.
//Функция предназначена для разрушения класса ExtSignal. Функция удаляет из памя-
//ти таблицы законов изменения характеристик компонент внешнего сигнала и список
//(в виде массива структур COMPONENT) cоставляющих внешнего сигнала.
//Список удаляемых таблиц законов:
//1. TableAngleLaw 2. TableAmplLaw 3. TableCasualLaw
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ: нет
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: нет
//*****************************************************************************
ExtSignal::~ExtSignal()
{
 //Удаление таблиц законов из памяти
 #if defined(CONSOLE)
  cout << "\nDestroy object of EXTERNAL SIGNAL class..." << endl;
  cout << "Delete law tables." << endl;
 #endif
  fname.resize(0);
  DeleteTable(TableAngleLaw, TableAngleLawCount);
  DeleteTable(TableAmplLaw, TableAmplLawCount);
  DeleteTable(TableCasualLaw, TableCasualLawCount);

 #if defined(CONSOLE)
  cout << "Delete " << Count << " components EXTERNAL SIGNAL." << endl;
 #endif
  for (uint i = 0; i < Count; i++) ResetProperty(i);
  delete[] data;
  Count = 0;

 #if defined(CONSOLE)
  cout << "EXTERNAL SIGNAL destroyed." << endl;
 #endif
}


//******************************************************************************
//III. ВЫЧИСЛЕНИЕ ПАРАМЕТРОВ ВНЕШНЕГО СИГНАЛА И ЕГО КОМПОНЕНТ
//Описание членов-функций класса ExtSignal по вычислению параметров составляющих
//внешнего сигнала: мгновенных значений амплитуд и фаз, комплексных напряжений с
//учетом и без учета поляризационных свойств компонент внешнего сигнала, мощнос-
//тей полезного сигнала, помехового сигнала, отдельной помехи и т.д.
//******************************************************************************

//*****************************************************************************
//СТАТУС: III.1; public; ExtSignal class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ: void SetTime(double time)
//НАЗНАЧЕНИЕ:
//Функция устанавливает текущее модельное время для вычисления характеристик
//компонент внешнего сигнала, которые изменяются по законам зависящим от времени.
//Значение модельного времени заносятся в список аргументов этих законов под
//индексом равным 0. Характеристики компонент внешнего сигнала зависящие от t:
//1. AzimuthAngle - ARGLIST* AzAngleArg
//2. ElevateAngle - ARGLIST* ElevAngleArg
//3. PolAngle     - ARGLIST* PolAngleArg
//4. AmplLaw      - ARGLIST* AmplArg
//5. VertPolPhase - ARGLIST* VertPolPhaseArg
//6. HorPolPhase  - ARGLIST* HorPolPhaseArg
//7. PhaseLaw     - ARGLIST* PhaseArg
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. double time - текущее модельное время
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: нет
//*****************************************************************************
void ExtSignal::SetTime(double time)
{
 for (uint i = 0; i < Count; i++)
 {
  data[i].AzAngleArg->List[0] = time;
  data[i].ElevAngleArg->List[0] = time;
  data[i].PolAngleArg->List[0] = time;
  data[i].AmplArg->List[0] = time;
  data[i].VertPolPhaseArg->List[0] = time;
  data[i].HorPolPhaseArg->List[0] = time;
  data[i].PhaseArg->List[0] = time;
 }
}
//*****************************************************************************
//СТАТУС: III.2; public; ExtSignal class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ: void SetRandNumber();
//НАЗНАЧЕНИЕ:
//Функция получает текущие значения случайных амплитуд и фаз для вычисления
//характеристик всех компонент внешнего сигнала.
//Случайные значения амплитуд (фаз) заносятся в список аргументов случайных
//законов под индексом равным 0. Данная функция позволяет исключить ситуацию
//двойного обращения к случайному закону, описывающему поведение фазы или ампли-
//туды компоненты внешнего сигнала, при определении мгновенных значений напряже-
//ний или мощностей компонент сигнала в один и тот же момент времени t. Такое
//событие может произойти, так как амплитуда и фаза компоненты внешнего сигнала
//расскладываются на горизонтальную и вертикальную поляризационные составляющие.
//Следовательно, необходимо получение ОДИНАКОВЫХ случайных значений амплитуды и
//фазы для двух составляющих компоненты сигнала. Двойное обращение к случайным
//функциям привело бы к получению РАЗНЫХ случайных значений амплитуды (фазы), что
//является некорректным. Случайные характеристики компонент внешнего сигнала:
//1. AmplCasualLaw  - ARGLIST* AmplCasualLawArg
//2. PhaseCasualLaw - ARGLIST* PhaseCasualLawArg
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ: нет
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: нет
//*****************************************************************************
void ExtSignal::SetRandNumber()
{
 for (uint i = 0; i < Count; i++)
 {
  data[i].AmplCasualLawArg->List[0]  = data[i].AmplCasualLaw(data[i].AmplCasualLawArg);
  data[i].PhaseCasualLawArg->List[0] = data[i].PhaseCasualLaw(data[i].PhaseCasualLawArg);
 }
}
//*****************************************************************************
//СТАТУС: III.3; public; ExtSignal class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ: uint ActiveJammers() const
//НАЗНАЧЕНИЕ: Подсчет количества действующих активных помех с Usage == ON
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ: нет
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: uint amount - кол-во действующих активных помех
//*****************************************************************************
uint ExtSignal::ActiveJammers() const
{uint amount = Jammers();
 if (amount == 0) return amount;
 for (uint i = 1; i < Count; i++) {if (data[i].Usage == OFF) amount--;}
return amount;
};
//*****************************************************************************
//СТАТУС: III.4; public; ExtSignal class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ:
// double AmplSignalComp(uint Number, uint PolarCompType = NONPOLAR) const
//НАЗНАЧЕНИЕ:
//Функция возвращает значение мгновенной амплитуды компоненты внешнего сигнала
//с порядковым номером Number. Number == 0 - соответствует полезному сигналу,
// 0 < Number < Count - помехе с номером Number. Если помехи с заданным номером
//Number не содержится в массиве data, то возвращается нулевое значение амплиту-
//ды. Переменная PolarCompType позволяет учитывать или не учитывать поляризацион-
//ные свойства компоненты сигнала. При PolarCompType = V_POLAR определяется
//амплитуда сигнала, соответствующая вертикальной составляющей поляризационного
//отношения сигнала, при PolarCompType = H_POLAR - горизонтальной составляющей
//поляризационного отношения сигнала, при PolarCompType = NONPOLAR (значение по
//умолчанию) - поляризационные свойства сигнала не учитываются.
//Функция проверяет поле Usage на необходимость использования компоненты сигнала
//с порядковым номером Number в расчетах. Если Usage == OFF (компонента в расче-
//тах не используется), то функция возвращает ampl = 0.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. uint Number - порядковый номер компоненты External Signal в массиве data
//2. uint PolarCompType - тип составляющей поляризации сигнала.
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: double ampl - мгновенное значение амплитуды компоненты
//внешнего сигнала с порядковым номером Number
//*****************************************************************************
double ExtSignal::AmplSignalComp(uint Number, uint PolarCompType) const
{ double ampl = 0.0;
  //Компонента внешнего сигнала с таким номером не существует
  if ((Count == 0) || (Number >= Count)) return ampl;
  //Проверка на использование компоненты сигнала с номером Number в расчетах
  //Компонент внешнего сигнала в расчетах не должен использоваться
  if (data[Number].Usage == OFF) return ampl;

  ampl = data[Number].AmplLaw(data[Number].AmplArg) *
         data[Number].AmplCasualLawArg->List[0];
  //Поляризационные свойства сигнала не учитываются
  if (PolarCompType == NONPOLAR) return ampl;
  //Поляризационные свойства сигнала учитываются
  //Вертикальная составляющая поляризации компоненты сигнала
  if (PolarCompType == V_POLAR)
   ampl *= sin(data[Number].PolAngle(data[Number].PolAngleArg));
  //Горизонтальная составляющая поляризации компоненты сигнала
  if (PolarCompType == H_POLAR)
   ampl *= cos(data[Number].PolAngle(data[Number].PolAngleArg));
 return ampl;
}
//*****************************************************************************
//СТАТУС: III.5; public; ExtSignal class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ:
// double PhaseSignalComp(uint Number, uint PolarCompType = NONPOLAR) const
//НАЗНАЧЕНИЕ:
//Функция возвращает значение мгновенной фазы компоненты внешнего сигнала
//с порядковым номером Number. Number == 0 - соответствует полезному сигналу,
// 0 < Number < Count - помехе с номером Number. Если помехи с заданным номером
//Number не содержится в массиве data, то возвращается нулевое значение фазы.
//Переменная PolarCompType позволяет учитывать или не учитывать поляризационные
//свойства компоненты сигнала. При PolarCompType = V_POLAR определяется
//фаза сигнала, соответствующая вертикальной составляющей поляризационного
//отношения сигнала, при PolarCompType = H_POLAR - горизонтальной составляющей
//поляризационного отношения сигнала, при PolarCompType = NONPOLAR (значение по
//умолчанию) - поляризационные свойства сигнала не учитываются.
//Функция проверяет поле Usage на необходимость использования компоненты сигнала
//с порядковым номером Number в расчетах. Если Usage == OFF (компонента в расче-
//тах не используется), то функция возвращает phase = 0.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. uint Number - порядковый номер компоненты External Signal в массиве data
//2. uint PolarCompType - тип составляющей поляризации сигнала.
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: double phase - мгновенное значение фазы компоненты
//внешнего сигнала с порядковым номером Number (в радианах)
//*****************************************************************************
double ExtSignal::PhaseSignalComp(uint Number, uint PolarCompType) const
{ double phase = 0.0;
  //Компонента внешнего сигнала с таким номером не существует
  if ((Count == 0) || (Number >= Count)) return phase;
  //Проверка на использование компоненты сигнала с номером Number в расчетах
  //Компонент внешнего сигнала в расчетах не должен использоваться
  if (data[Number].Usage == OFF) return phase;

  phase = data[Number].PhaseLaw(data[Number].PhaseArg) +
          data[Number].PhaseCasualLawArg->List[0];

  //Поляризационные свойства сигнала не учитываются
  if (PolarCompType == NONPOLAR) return phase;
  //Поляризационные свойства сигнала учитываются
  //Вертикальная составляющая поляризации компоненты сигнала
  if (PolarCompType == V_POLAR)
   phase += data[Number].VertPolPhase(data[Number].VertPolPhaseArg);
  //Горизонтальная составляющая поляризации компоненты сигнала
  if (PolarCompType == H_POLAR)
   phase += data[Number].HorPolPhase(data[Number].HorPolPhaseArg);

 return phase;
}
//*****************************************************************************
//СТАТУС: III.6; public; ExtSignal class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ:
// double DetAmpl(uint Number, uint PolarCompType = NONPOLAR) const
//НАЗНАЧЕНИЕ:
//Функция возвращает значение мгновенной амплитуды компоненты внешнего сигнала
//с порядковым номером Number без случайной составляющей. Number == 0 - соответст-
//вует полезному сигналу, 0 < Number < Count - помехе с номером Number. Если по-
//мехи с заданным номером Number не содержится в массиве data, то возвращается
//нулевое значение амплитуды. Переменная PolarCompType позволяет учитывать или
//не учитывать поляризационные свойства компоненты сигнала.
//При PolarCompType = V_POLAR определяется амплитуда сигнала, соответствующая
//вертикальной составляющей поляризационного отношения сигнала, при
//PolarCompType = H_POLAR - горизонтальной составляющей поляризационного отноше-
//ния сигнала, при PolarCompType = NONPOLAR (значение по умолчанию) - поляриза-
//ционные свойства сигнала не учитываются.
//Функция проверяет поле Usage на необходимость использования компоненты сигнала
//с порядковым номером Number в расчетах. Если Usage == OFF (компонента в расче-
//тах не используется), то функция возвращает ampl = 0.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. uint Number - порядковый номер компоненты External Signal в массиве data
//2. uint PolarCompType - тип составляющей поляризации сигнала.
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: double ampl - мгновенное значение амплитуды компоненты
//внешнего сигнала с порядковым номером Number без СЛУЧАЙНОЙ СОСТАВЛЯЮЩЕЙ.
//*****************************************************************************
double ExtSignal::DetAmpl(uint Number, uint PolarCompType) const
{ double ampl = 0.0;
  //Компонента внешнего сигнала с таким номером не существует
  if ((Count == 0) || (Number >= Count)) return ampl;
  //Проверка на использование компоненты сигнала с номером Number в расчетах
  //Компонент внешнего сигнала в расчетах не должен использоваться
  if (data[Number].Usage == OFF) return ampl;

  ampl = data[Number].AmplLaw(data[Number].AmplArg);

  //Поляризационные свойства сигнала не учитываются
  if (PolarCompType == NONPOLAR) return ampl;
  //Поляризационные свойства сигнала учитываются
  //Вертикальная составляющая поляризации компоненты сигнала
  if (PolarCompType == V_POLAR)
   ampl *= sin(data[Number].PolAngle(data[Number].PolAngleArg));
  //Горизонтальная составляющая поляризации компоненты сигнала
  if (PolarCompType == H_POLAR)
   ampl *= cos(data[Number].PolAngle(data[Number].PolAngleArg));
 return ampl;
}
//*****************************************************************************
//СТАТУС: III.7; public; ExtSignal class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ:
// double DetPhase(uint Number, uint PolarCompType = NONPOLAR) const
//НАЗНАЧЕНИЕ:
//Функция возвращает значение мгновенной фазы компоненты внешнего сигнала
//с порядковым номером Number без случайной составляющей фазы. Number == 0 -
//соответствует полезному сигналу, 0 < Number < Count - помехе с номером Number.
//Если помехи с заданным номером Number не содержится в массиве data, то возвра-
//щается нулевое значение фазы.
//Переменная PolarCompType позволяет учитывать или не учитывать поляризационные
//свойства компоненты сигнала. При PolarCompType = V_POLAR определяется
//фаза сигнала, соответствующая вертикальной составляющей поляризационного
//отношения сигнала, при PolarCompType = H_POLAR - горизонтальной составляющей
//поляризационного отношения сигнала, при PolarCompType = NONPOLAR (значение по
//умолчанию) - поляризационные свойства сигнала не учитываются.
//Функция проверяет поле Usage на необходимость использования компоненты сигнала
//с порядковым номером Number в расчетах. Если Usage == OFF (компонента в расче-
//тах не используется), то функция возвращает phase = 0.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. uint Number - порядковый номер компоненты External Signal в массиве data
//2. uint PolarCompType - тип составляющей поляризации сигнала.
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: double phase - мгновенное значение фазы компоненты
//внешнего сигнала с порядковым номером Number (в радианах) без СЛУЧАЙНОЙ
//СОСТАВЛЯЮЩЕЙ ФАЗЫ.
//*****************************************************************************
double ExtSignal::DetPhase(uint Number, uint PolarCompType) const
{ double phase = 0.0;
  //Компонента внешнего сигнала с таким номером не существует
  if ((Count == 0) || (Number >= Count)) return phase;
  //Проверка на использование компоненты сигнала с номером Number в расчетах
  //Компонент внешнего сигнала в расчетах не должен использоваться
  if (data[Number].Usage == OFF) return phase;

  phase = data[Number].PhaseLaw(data[Number].PhaseArg);

  //Поляризационные свойства сигнала не учитываются
  if (PolarCompType == NONPOLAR) return phase;
  //Поляризационные свойства сигнала учитываются
  //Вертикальная составляющая поляризации компоненты сигнала
  if (PolarCompType == V_POLAR)
   phase += data[Number].VertPolPhase(data[Number].VertPolPhaseArg);
  //Горизонтальная составляющая поляризации компоненты сигнала
  if (PolarCompType == H_POLAR)
   phase += data[Number].HorPolPhase(data[Number].HorPolPhaseArg);

 return phase;
}
//*****************************************************************************
//СТАТУС: III.8; public; ExtSignal class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ: complex CalcSumUJammer(uint PolarCompType)
//НАЗНАЧЕНИЕ:
//Функция вычисляет суммарное комплексное напряжение всех помех внешнего сигнала.
//Переменная PolarCompType позволяет учитывать или не учитывать поляризационные
//свойства компоненты сигнала. При PolarCompType = V_POLAR определяется
//фаза сигнала, соответствующая вертикальной составляющей поляризационного
//отношения сигнала, при PolarCompType = H_POLAR - горизонтальной составляющей
//поляризационного отношения сигнала, при PolarCompType = NONPOLAR (значение по
//умолчанию) - поляризационные свойства сигнала не учитываются.
//Функция проверяет поле Usage на необходимость использования компоненты сигнала
//с порядковым номером Number в расчетах. Если Usage == OFF (компонента в расче-
//тах не используется), то данная помеха в расчетах не учитывается.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. uint PolarCompType - тип составляющей поляризации сигнала.
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: complex UNoise - мгновенное значение суммарного
//комплексного напряжения всех помех внешнего сигнала
//*****************************************************************************
complex ExtSignal::CalcSumUJammer(uint PolarCompType)
{ complex UNoise(0.0,0.0);
  //Внешний сигнал помех не содержит
  if (Jammers() == 0) return UNoise;

  for (uint i = 1; i < Count; i++)
   //Проверка на использование помехи с номером i в расчетах
   if (data[i].Usage == ON)
    UNoise += polar(AmplSignalComp(i,PolarCompType),
                    PhaseSignalComp(i, PolarCompType));
 return UNoise;
}
//*****************************************************************************
//СТАТУС: III.9; public; ExtSignal class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ: complex CalcSumPJammer(uint PolarCompType)
//НАЗНАЧЕНИЕ:
//Функция вычисляет суммарную комплексную мощность всех помех внешнего сигнала.
//Переменная PolarCompType позволяет учитывать или не учитывать поляризационные
//свойства компоненты сигнала. При PolarCompType = V_POLAR определяется
//фаза сигнала, соответствующая вертикальной составляющей поляризационного
//отношения сигнала, при PolarCompType = H_POLAR - горизонтальной составляющей
//поляризационного отношения сигнала, при PolarCompType = NONPOLAR (значение по
//умолчанию) - поляризационные свойства сигнала не учитываются.
//Функция проверяет поле Usage на необходимость использования компоненты сигнала
//с порядковым номером Number в расчетах. Если Usage == OFF (компонента в расче-
//тах не используется), то данная помеха в расчетах не учитывается.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. uint PolarCompType - тип составляющей поляризации сигнала.
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: complex UNoise - мгновенное значение суммарного
//комплексного напряжения всех помех внешнего сигнала
//*****************************************************************************
double ExtSignal::CalcSumPJammer(uint PolarCompType)
{ double PNoise = 0.0;
  //Внешний сигнал помех не содержит
  if (Jammers() == 0) return PNoise;

  for (uint i = 1; i < Count; i++)
   //Проверка на использование помехи с номером i в расчетах
   if (data[i].Usage == ON)
    PNoise += norm(polar(AmplSignalComp(i,PolarCompType),
                         PhaseSignalComp(i, PolarCompType)));
 return PNoise;
}
//*****************************************************************************
//СТАТУС: III.10; public; ExtSignal class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ: double SignalJammerRatio()
//НАЗНАЧЕНИЕ:
//Функция вычисляет отношение сигнал-помеха (в децибелах) по мгновенным значе-
//ниям мощностей полезного сигнала и аддитивной помехе (jammer)
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ: нет
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: double SJR - отношение сигнал-помеха в Дб (Db)
//*****************************************************************************
double ExtSignal::SignalJammerRatio()
{ double SJR;
  double SumPj = CalcSumPJammer();
  double Ps = CalcPSignal();
  //Суммарная мощность помехи равна нулю
  if (SumPj == 0.0) return 100.0;
  //Мощность сигнала равна нулю
  if (Ps == 0.0) return -100.0;
  //Расчет отношения сигнал-помеха
  SJR = 10*log10(Ps/SumPj);
 return SJR;
}
