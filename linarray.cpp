//linarray.cpp
//Реализация класса TLinearArray - ЛИНЕЙНАЯ АНТЕННАЯ РЕШЕТКА (LINEAR ARRAY)
//Initial date: October,1,1998. Final date:
//Copyright (c) JohnSoft 1998. All rights reserved. Borland C++ 5.01

#include "linarray.h"

//******************************************************************************
//Объявление глобальных переменных, которые используются при считывании характе-
//ристик линейной антенной решетки из текстового файла специального вида.
//Отдельные характеристики антенной решетки могут располагаться как на одной
//строке, так и на нескольких (например, в случае если относительные расстояния
//между элементами АР заданы в табличном виде и количество элементов решетки
//велико).
//******************************************************************************
string Buffer;//Текущая символьная строка считанная из файлового потока fin
string Line; //Текущая строка данных считанная из входного файлового потока fin
ulong CurRow = 0;//Порядковый номер текущей строки Buffer

//Символы-разделители комментарий от текста с данными. Весь текст считанной
//строки следующий за данными символами считается КОММЕНТАРИЯМИ.
const uint sz_CommentSep = 3;
const char* CommentSep[3] = {"//","#","rem"};

//Символы присваивания переменным (параметрам) значений. Используются при считы-
//вании цифровых данных. Они играют роль символов-разделителей идентификаторов
//свойств, параметров от их цифровых значений. В таких ситуациях, когда свойство
//антенной решетки, ее элемента или параметр закона (функции) являются цифровыми
//величинами присутствие  символов присваивания обязательно.
const uint sz_AssignSep = 2;
const char* AssignSep[2] = {":=","="};

const string EngABC = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const string Engabc = "abcdefghijklmnopqrstuvwxyz";
const string RusABC = "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
const string Rusabc = "абвгдеёжзийклмнонрстуфхцчшщъыьэюя";
const string Digits = "0123456789";
const string Sign = "-+";
const string RealNumber= "-+0123456789.eE";   //Sign+Digits+".eE";

//Шаблон ХАРАКТЕРИСТИК ЛИНЕЙНОЙ АНТЕННОЙ РЕШЕТКИ
const uint sz_LAAPerform = 7;
const char* LAAPerformPattern[7] = {
 "   LINEAR ARRAY PROPERTIES:",
 "1. CONFIGURATION: LEFT = < value >; TOGETHER = < value >; RIGHT = < value >;",
 "2. UNIFORMITY: < Uniform || Nonuniform >;",
 "3. EQUIDISTANCE: <Equidistant || Unequidistant>; LV = <value>;",
 "4. LEFTDISTLAW: <law name>; PARAMETERS: <value1,value2,...,valueN>;",
 "5. RIGHTDISTLAW: <law name>; PARAMETERS: <value1,value2,...,valueN>;",
 "6. DISPOSITION: <Equator || Meridian>;"
};

//Шаблон ЗАГОЛОВКА свойств линейной антенной решетки
const uint sz_LAAHeader = 3;
const char* LAAHeaderPattern[3] = { "LINEAR ARRAY PROPERTIES",
 "LINEAR ANTENNA ARRAY PROPERTIES", "СВОЙСТВА ЛИНЕЙНОЙ АНТЕННОЙ РЕШЕТКИ"};

//Шаблон наименований свойства <CONFIGURATION> антенной решетки
const uint sz_LAAConfig = 3;
const char* LAAConfigPattern[3] = {"CONFIGURATION","КОНФИГУРАЦИЯ", "CONFIG"};

//Шаблон наименований свойства <LEFT> антенной решетки - количество элементов
//антенной решетки, расположенных слева от основного (главного) элемента АР
const uint sz_LAALeft = 4;
const char* LAALeftPattern[4] = {"TO THE LEFT","FROM THE LEFT","LEFT","СЛЕВА"};

//Шаблон наименований свойства <TOGETHER> антенной решетки - количество элементов
//антенной решетки, расположенных вместе с основным (главным) элементом АР
const uint sz_LAATogether = 2;
const char* LAATogetherPattern[2] = {"TOGETHER","ВМЕСТЕ"};

//Шаблон наименований свойства <RIGHT> антенной решетки - количество элементов
//антенной решетки, расположенных справа от основного (главного) элемента АР
const uint sz_LAARight = 4;
const char* LAARightPattern[4] = {"TO THE RIGHT","FROM THE RIGHT","RIGHT",
                                  "СПРАВА"};

//Шаблон наименований свойства <UNIFORMITY> антенной решетки
const uint sz_LAAUniform = 3;
const char* LAAUniformPattern[3] = {"UNIFORMITY","HOMOGENEITY","ОДНОРОДНОСТЬ"};

const uint sz_TbUniform = 4;
const STR_BOOL TbUniform[4] = { {"NONUNIFORM",false},{"UNIFORM",true},
 {"НЕОДНОРОДНАЯ",false},{"ОДНОРОДНАЯ",true} };

//Шаблон наименований свойства <EQUIDISTANCE> антенной решетки
const uint sz_LAAEquidist = 2;
const char* LAAEquidistPattern[2] = {"EQUIDISTANCE","ЭКВИДИСТАНТНОСТЬ"};

const uint sz_TbEquidist = 6;
const STR_BOOL TbEquidist[6] = { {"UNEQUIDISTANT",false},{"EQUIDISTANT",true},
 {"НЕЭКВИДИСТАНТНАЯ",false},{"ЭКВИДИСТАНТНАЯ",true}, {"UNEQUAL",false},
 {"EQUAL",true} };

//Шаблон наименований характеристики АР, определяющей относительные расстояния
//между элементами эквидистантной АР
const uint sz_LAARelDist = 2;
const char* LAARelDistPattern[2] = { {"LV"}, {"RELATIVE DISTANCE"}};

//Шаблон наименований свойства <LEFTDISTLAW> - закон, определяющий относительные
//расстояния между двумя соседними элементами АР, расположенными левее основного
//элемента АР
const uint sz_LAALfLaw = 2;
const char* LAALfLawPattern[2] = {"LEFTDISTLAW","LEFTLAW"};

//Шаблон наименований свойства <RIGHTDISTLAW> - закон, определяющий относительные
//расстояния между двумя соседними элементами АР, расположенными правее основного
//элемента АР
const uint sz_LAARtLaw = 2;
const char* LAARtLawPattern[2] = {"RIGHTDISTLAW","RIGHTLAW"};

//Шаблон наименований свойства <PARAMETERS> - параметров закона
const uint sz_LawParam = 3;
const char* LawParamPattern[3] = {"PARAMETERS", "PARAM", "ПАРАМЕТРЫ"};

//Шаблон наименований свойства <DISPOSITION> антенной решетки - расположение ли-
//нейной антенной решетки относительно экваториальной или меридиональной плоскос-
//тей Земли
const uint sz_LAADisp = 2;
const char* LAADispPattern[2] = {"DISPOSITION","РАСПОЛОЖЕНИЕ"};

const uint sz_TbDispos = 6;
const STR_BOOL TbDispos[6] = { {"MERIDIAN",false},{"EQUATOR",true},
 {"VERTICAL",false},{"HORIZONTAL",true}, {"ПО ВЕРТИКАЛИ",false},
 {"ПО ГОРИЗОНТАЛИ",true} };

//Объявление таблицы законов, предназначенных для определения относительных
//расстояний между соседними элементами АР, расположенными слева и справа от ос-
//новного элемента АР, в случае неэквидистантной АР.
//1. Наименование постоянного закона F(x) = A;
uint sz_ConstLawName = 3;
const char* ConstLawName[3] = {"CONST_LAW","Constant law","Постоянный закон"};
//2. Наименования линейного закона F(x) = A*x+B
uint sz_LinearLawName = 3;
const char* LinearLawName[3] = {"LINEAR_LAW","Linear law","Линейный закон"};
//3. Наименование квадратичного закона F(x) = A*x^2+B*x+C
uint sz_SquareLawName = 3;
const char* SquareLawName[3] = {"SQUARE_LAW","Square law","Квадратичный закон"};
//4. Наименование экспоненциального закона F(x) = A*exp(B*x)
uint sz_ExpLawName = 3;
const char* ExpLawName[3] = {"EXP_LAW","Exponential law",
                             "Экспоненциальный закон"};
//5. Наименование гиперболического закона F(x) = A/x
uint sz_HyperbolLawName = 3;
const char* HyperbolLawName[3] = {"HYPERBOL_LAW","Hyperbolic law",
                                  "Гиперболический закон"};
//6. Наименование закона квадратного корня из двучлена F(x) = SQRT(A*x+B)
uint sz_SqrtLawName = 3;
const char* SqrtLawName[3] = {"SQRT_LAW","Square root","Квадратный корень"};
//7. Наименование закона натурального логарифма F(x) = LN(x)
uint sz_LnLawName = 3;
const char* LnLawName[3] = {"LN_LAW","Natural logarithm","Натуральный логарифм"};
//8. Наименование закона десятичного логарифма F(x) = LOG10(x)
uint sz_Log10LawName = 3;
const char* Log10LawName[3] = {"LOG10_LAW", "Decimal logarithm",
                               "Десятичный логарифм"};
//9. Наименование закона логарифма по основанию A F(x) = LOGA(x)
uint sz_LogALawName = 3;
const char* LogALawName[3] = {"LOGA_LAW", "Log on base A",
                              "Логарифм по основанию А"};
//10. Наименование равновероятного закона  F(x) = RAND[A..B]
uint sz_RandABLawName = 3;
const char* RandABLawName[3] = {"RANDAB_LAW", "Random law [A..B]",
                                "Равновероятный закон [A..B]"};
//11. Наименование табличного закона
uint sz_TableLawName = 3;
const char* TableLawName[3] = {"TABLE_LAW","Table law","Табличный закон"};

//ТАБЛИЦА ЗАКОНОВ, ОПРЕДЕЛЯЮЩИХ ОТНОСИТЕЛЬНЫЕ РАССТОЯНИЯ МЕЖДУ ДВУМЯ СОСЕДНИМИ
//ЭЛЕМЕНТАМИ АНТЕННОЙ РЕШЕТКИ
const uint sz_DistLawTable = 11;
const INFO_LAW DistLawTable[11] = {
 {ConstLawName,sz_ConstLawName,CONST_LAW,ConstLaw,2,2,0,"F(x)=A",DETLAW},
 {LinearLawName,sz_LinearLawName,LINEAR_LAW,LinearLaw,3,3,0,"F(x)=A*x+B",DETLAW},
 {SquareLawName,sz_SquareLawName,SQUARE_LAW,SquareLaw,4,4,0,"F(x)=A*x^2+B*x+C",
  DETLAW},
 {ExpLawName,sz_ExpLawName,EXP_LAW,ExpLaw,3,3,0,"F(x)=A*exp(B*x)",DETLAW},
 {HyperbolLawName,sz_HyperbolLawName,HYPERBOL_LAW,HyperbolLaw,2,2,0,"F(x)=A/x",
  DETLAW},
 {SqrtLawName,sz_SqrtLawName,SQRT_LAW,SqrtLaw,3,3,0,"F(x)=SQRT(A*x+B)",DETLAW},
 {LnLawName,sz_LnLawName,LN_LAW,LnLaw,1,1,0,"F(x)=LN(x)",DETLAW},
 {Log10LawName,sz_Log10LawName,LOG10_LAW,Log10Law,1,1,0,"F(x)=LOG10(x)",DETLAW},
 {LogALawName,sz_LogALawName,LOGA_LAW,LogALaw,2,2,0,"F(x) = LOGA(x)",DETLAW},
 {RandABLawName,sz_RandABLawName,RANDAB_LAW,RandABLaw,3,2,1,"F(x)=RAND[A..B]",
  CASUALLAW},
 {TableLawName,sz_TableLawName,TABLE_LAW,ZeroLaw,0,0,1,"Table law",DETLAW}
 };

//ТАБЛИЦА ЗАКОНОВ, ОПРЕДЕЛЯЮЩИХ ЗАКОНЫ ИЗМЕНЕНИЯ АМПЛИТУДЫ И ФАЗЫ ВНУТРЕННЕГО
//ШУМА В КАНАЛАХ АНТЕННОЙ РЕШЕТКИ
//1. Наименование равновероятного закона  F(x) = RAND[0..1]
uint sz_RandLawName = 3;
const char* RandLawName[3] = {"RAND_LAW", "Random law", "Равновероятный закон"};
//2. Наименование равновероятного закона  F(x) = RAND[0..2PI]
uint sz_Rand2PiLawName = 3;
const char* Rand2PiLawName[3] = {"RAND2PI_LAW", "Random law [0..2PI]",
                                 "Равновероятный закон [0..2PI]"};
//3. Наименование случайного закона Pелея F(x) = RELAY_LAW
uint sz_RelayLawName = 3;
const char* RelayLawName[3] = {"RELAY_LAW", "Relay law", "Закон Релея"};

//4. Наименование детерминированного закона  F(x) = 1
uint sz_Det1LawName = 1;
const char* Det1LawName[1] = {"DET1_LAW"};

//5. Наименование детерминированного закона  F(x) = 0
uint sz_Det0LawName = 1;
const char* Det0LawName[1] = {"DET0_LAW"};

const uint sz_NoiseLawTable = 6;
const INFO_LAW NoiseLawTable[6] = {
 {RelayLawName,sz_RelayLawName,RELAY_LAW,RelayLaw,1,0,0,
  "F(x)=sqrt(-2*ln(RAND[0..1])", CASUALLAW},
 {Rand2PiLawName,sz_Rand2PiLawName,RAND2PI_LAW,Rand2PiLaw,1,0,0,
  "F(x)=RAND[0..2PI]", CASUALLAW},
 {RandLawName,sz_RandLawName,RAND_LAW,RandLaw,1,0,0,"F(x)=RAND[0..1]",
  CASUALLAW},
 {RandABLawName,sz_RandABLawName,RANDAB_LAW,RandABLaw,3,2,1,"F(x)=RAND[A..B]",
  CASUALLAW},
 {Det1LawName,sz_Det1LawName,DET1_LAW,Det1Law,1,0,0,"F(x) = 1",DETLAW},
 {Det0LawName,sz_Det0LawName,DET0_LAW,Det0Law,1,0,0,"F(x) = 0",DETLAW}
 };

//Шаблон наименований антенн, используемых в антенной решетке
//1. Наименование полуволнового вибратора
uint sz_HWDipoleName = 3;
const char* HWDipoleName[3] = {"HW_DIPOLE", "Half-wave dipole",
                               "Полуволновой вибратор"};
//2. Наименование полуволновой щели
uint sz_HWSlotAntName = 3;
const char* HWSlotAntName[3] = {"HWSLOT_ANT", "Half-wave slot antenna",
                                "Полуволновая щель"};
//3. Наименование параболической антенны
uint sz_ParabAntName = 3;
const char* ParabAntName[3] = {"PARAB_ANT", "Parabolic antenna",
                               "Параболическая антенна"};
//Наименование параметров параболической антенны в E- и H-плоскостях
uint sz_ParabAntEH = 1;
const char* ParabAntEH[1] = {"La - Parabolic antenna relative aperture"};
//Дополнительные параметры в функциях, описывающих диаграммы направленности ан-
//тенн не нужны
uint sz_UnParam = 1;
const char* UnParam[1] = {"The additional parameters are not required"};

//ТАБЛИЦА ДАННЫХ ПО ЭЛЕМЕНТАМ АНТЕННОЙ РЕШЕТКИ, ИСПОЛЬЗУЕМАЯ ПРИ ПОЛУЧЕНИИ
//ХАРАКТЕРИСТИК ПО АНТЕННАМ ПРИ СЧИТЫВАНИИ ИХ С ФАЙЛА
const uint sz_AntTable = 3;
const ANTENNA_INFO AntTable[3] = {
 //1. Полуволновый вибратор
 {HWDipoleName,sz_HWDipoleName,HW_DIPOLE,1,0,UnParam,sz_UnParam,1,0,UnParam,
  sz_UnParam},
 //2. Полуволновая щель
 {HWSlotAntName,sz_HWSlotAntName,HWSLOT_ANT,1,0,UnParam,sz_UnParam,1,0,UnParam,
  sz_UnParam},
 //3. Параболическая антенна
 {ParabAntName,sz_ParabAntName,PARAB_ANT,2,1,ParabAntEH,sz_ParabAntEH,2,1,
  ParabAntEH,sz_ParabAntEH}
};
//Шаблон ХАРАКТЕРИСТИК ЭЛЕМЕНТА ЛИНЕЙНОЙ АНТЕННОЙ РЕШЕТКИ
const uint sz_AntPerform = 9;
const char* AntPerform[9] = {
 "   MASTER (SLAVE) ELEMENT of LINEAR ARRAY PROPERTIES:",
 "1. USAGE: < ON || OFF >;",
 "2. AMPLRELFACTOR: <value>;",
 "3. ANTENNA: <antenna name>;",
 "4. E_DIRECT: PARAMETERS: <value1,value2,...,valueN>;",
 "5. H_DIRECT: PARAMETERS: <value1,value2,...,valueN>;",
 "6. E_LOCATION: <Equator || Meridian>;",
 "7. NOISEAMPL: <law name>; PARAMETERS: <value1,value2,...,valueN>;",
 "8. NOISEPHASE: <law name>; PARAMETERS: <value1,value2,...,valueN>;"
};
//Шаблон ЗАГОЛОВКА свойств элемента линейной антенной решетки
const uint sz_MasterHeader = 3;
const char* MasterHeader[3] = { "MASTER ELEMENT of LAA PROPERTIES",
 "MASTER ELEMENT of LINEAR ARRAY PROPERTIES",
 "СВОЙСТВА ОСНОВНОГО ЭЛЕМЕНТА ЛИНЕЙНОЙ АНТЕННОЙ РЕШЕТКИ"};

const uint sz_SlaveHeader = 3;
const char* SlaveHeader[3] = { "SLAVE ELEMENT of LAA PROPERTIES",
 "SLAVE ELEMENT of LINEAR ARRAY PROPERTIES",
 "СВОЙСТВА ВСПОМОГАТЕЛЬНОГО ЭЛЕМЕНТА ЛИНЕЙНОЙ АНТЕННОЙ РЕШЕТКИ"};

//1. Шаблон наименований свойства <USAGE> элемента антенной решетки
const uint sz_LAAElemUsage = 3;
const char* LAAElemUsage[3] = {"USAGE","ИСПОЛЬЗОВАНИЕ","Использование"};

const uint sz_TbUsage = 4;
const STR_BOOL TbUsage[4] = { {"OFF",false},{"ON",true},
 {"Не использовать",false},{"Использовать",true} };

//2. Шаблон наименований свойства <AMPLRELFACTOR> элемента антенной решетки
const uint sz_LAAElemARF = 3;
const char* LAAElemARF[3] = {"AMPLRELFACTOR","ARF","ОтнКоэффУсиления"};

//Шаблон наименований хар-ки элемента АР, определяющей относительный коэффициент
//усиления элемента АР - amplify relative factor (ARF)
const uint sz_NameARF = 2;
const char* NameARF[2] = { {"ARF"}, {"N"}};

//3. Шаблон наименований свойства <ANTENNA> элемента АР
const uint sz_Antenna = 3;
const char* AntennaPattern[3] = {"ANTENNA","АНТЕННА","Антенна"};

//4. Шаблон наименований свойства <E_DIRECT> элемента АР
const uint sz_EDirect = 3;
const char* EDirectPattern[3] = {"E_DIRECT","E-PLANE","E-ПЛОСКОСТЬ"};

//5. Шаблон наименований свойства <H_DIRECT> элемента АР
const uint sz_HDirect = 3;
const char* HDirectPattern[3] = {"H_DIRECT","H-PLANE","H-ПЛОСКОСТЬ"};

//6. Шаблон наименований свойства <E_LOCATION> элемента АР - расположение антен-
//   ны относительно экваториальной или меридиональной плоскостей Земли
const uint sz_LAAElemELocation = 2;
const char* LAAElemELocation[2] = {"E_LOCATION","РАСПОЛОЖЕНИЕ E-ПЛОСКОСТИ"};

//6. Шаблон наименований свойства <NOISEAMPL> элемента АР - закон изменения
//   амплитуды внутреннего шума приемного канала
const uint sz_LAAElemNoiseAmpl = 2;
const char* LAAElemNoiseAmpl[2] = {"NOISEAMPL","АМПЛИТУДА ШУМА"};

//7. Шаблон наименований свойства <NOISEPHASE> элемента АР - закон изменения
//   фазы внутреннего шума приемного канала
const uint sz_LAAElemNoisePhase = 2;
const char* LAAElemNoisePhase[2] = {"NOISEPHASE","ФАЗА ШУМА"};

//==========================================================================
// I. ОПИСАНИЕ ОБЩЕДОСТУПНЫХ ЧЛЕНОВ-ФУНКЦИЙ КЛАССА TLinearArray
//    PUBLIC MEMBER-FUNCTION OF TLinearArray CLASS
//==========================================================================
//*****************************************************************************
//СТАТУС: I.1; public member-function of class TElArray
//НАИМЕНОВАНИЕ ФУНКЦИИ: string& Properties(string& s, bool headfoot = ON)
//НАЗНАЧЕНИЕ:
//Функция формирует характеристики объекта TLinearArray в виде строки. Каждое
//свойства линейной АР разделяются символом '\n'. Характеристики линейной антен-
//ной решетки следующие (пример):
// LINEAR ARRAY PROPERTIES
// 1. CONFIGURATION: LEFT = <value>; TOGETHER = <value>; RIGHT = <value>;
// 2. UNIFORMITY: < Uniform || Nonuniform >;
// 3. EQUIDISTANCE: <Equidistant || Unequidistant>;
// 4. LEFTDISTLAW: <law name>; PARAMETERS: <value1,value2,...,valueN>;
// 5. RIGHTDISTLAW: <law name>; PARAMETERS: <value1,value2,...,valueN>;",
// 6. DISPOSITION: <Equator || Meridian>;
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. string& s - ссылка на строковый объект
//2. bool headfoot - индикатор формирования заголовка и подвала строки
//                   характеристик линейной АР
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: string& s - строка со свойствами сигнала
//*****************************************************************************
string& TLinearArray::Properties(string& s, bool headfoot)
{char buffer[80];
 ostrstream text (buffer, sizeof(buffer));
 string t;
 s.resize(0);
 if (headfoot == ON)
 {text << "**************  Performance of Linear Antenna Array   **************"
       << endl << ends;
  s.append(text.str());
  text.seekp(0);
 }
 //Конфигурация АР
 s.append("1. CONFIGURATION: ");
 text << "TOTAL = " << Count;
 text << "; LEFT = " << Left << "; TOGETHER = " << Together << "; RIGHT = "
      << Right << ";" << endl << ends;
 s.append(text.str());
 text.seekp(0);
 //Однородность АР
 s.append("2. UNIFORMITY:    ");
 if (Uniformity == UNIFORM) s.append("Uniform\n");
 else s.append("Nonuniform\n");

 s.append("3. EQUIDISTANCE:  ");
 if (Equidistance == EQUAL) s.append("Equidistant\n");
 else s.append("Unequidistant\n");

 s.append("4. LEFTDISTLAW:   ");
 if (LeftLawType == CONST_LAW) //Постоянный закон или эквидистантная решетка
  text << "CONST_LAW; " << "LV = " << LeftLawArg->List[1] << endl << ends;
 else if (LeftLawType == TABLE_LAW) //Табличный закон
  text << "TABLE_LAW;" << endl << ends;
 else //Общий случай
 {//Поиск наименования закона соответствующего LeftLawType в DistLawTable
  uint index = 0;
  for (uint i = 0; i < sz_DistLawTable; i++)
   if(DistLawTable[i].ident == LeftLawType) { index = i; break; }
  text << DistLawTable[index].pattern[0] << ";" << endl << ends;
 }
 s.append(text.str());
 text.seekp(0);

 s.append("5. RIGHTDISTLAW:  ");
 if (RightLawType == CONST_LAW) //Постоянный закон или эквидистантная решетка
  text << "CONST_LAW; " << "LV = " << RightLawArg->List[1] << endl << ends;
 else if (RightLawType == TABLE_LAW) //Табличный закон
  text << "TABLE_LAW;" << endl << ends;
 else //Общий случай
 {//Поиск наименования закона соответствующего LeftLawType в DistLawTable
  uint index = 0;
  for (uint i = 0; i < sz_DistLawTable; i++)
   if(DistLawTable[i].ident == RightLawType) { index = i; break; }
  text << DistLawTable[index].pattern[0] << ";" << endl << ends;
 }
 s.append(text.str());
 text.seekp(0);

 s.append("6. DISPOSITION:   ");
 if (Disposition == EQUATOR) s.append("Equator (Horizontal)\n");
 else s.append("Meridian (Vertical)\n");

 if (headfoot == ON)
 {text << "********************************************************************"
       << endl << ends;
  s.append(text.str());
 }
return s;
}
//******************************************************************************
//СТАТУС: I.2; public member-function of class TElArray
//НАИМЕНОВАНИЕ ФУНКЦИИ:
// string& LAAElemProperties(string& s, ulong index, bool headfoot = ON)
//НАЗНАЧЕНИЕ:
//Функция формирует характеристики антенного элемента TElArray объекта TLinear-
//Array в виде строки с порядковым номером index. Каждое свойства элемента АР
//разделяются символом '\n'.
//------------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. string& s - ссылка на строковый объект
//2. ulong index - индекс элемента АР в массиве data для которого формируются
//   характеристики
//3. bool headfoot - индикатор формирования заголовка и подвала строки характе-
//   ристики элемента линейной АР
//------------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: string& s - строка со свойствами сигнала
//******************************************************************************
string& TLinearArray::LAAElemProperties(string& s,ulong index,bool headfoot)
{char buffer[80];
 ostrstream text (buffer, sizeof(buffer));
 s.resize(0);
 if (index >= Count)
 {text << "Element of LAA with number <" << index << "> does not exist !"
       << endl << ends;
  s.append(text.str());
  text.seekp(0);
 }
 else data[index].GetPerformance(s,headfoot);
return s;
}
//*****************************************************************************
//СТАТУС: I.3; public member-function of class TLinearArray
//НАИМЕНОВАНИЕ ФУНКЦИИ: void CalcRelDistance()
//НАЗНАЧЕНИЕ:
//Функция рассчитывает относительные расстояния между соседними элементами АР, а
//также между основным и вспомогательными элементами АР.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ: нет
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: нет
//*****************************************************************************
void TLinearArray::CalcRelDistance()
{double L;   //Расстояние между вспомогательным и основным элементом АР
 double LV;  //Расстояние между соседними вспомогательными элементами
 uint index;
 bool type;  //Случайный или детерминированный закон
 uint border;
 //****************************************************************************
 //1. Определяем относительные расстояния между элементами АР, расположенными
 //   слева от основного
 //****************************************************************************
 if (Left != 0) //Имеются элементы АР, расположенные СЛЕВА от основного.
 {//Поиск индекса закона соответствующего LeftLawType в DistLawTable
  for (uint i = 0; i < sz_DistLawTable; i++)
   if(DistLawTable[i].ident == LeftLawType) { index = i; break; }
  type = DistLawTable[index].type;
  //Детерминированный, нетабличный закон
  if ((type == DETLAW) && (LeftLawType != TABLE_LAW))
  {//Определяем верхнюю границу расположения элементов АР в массиве data
   border = Left - 1;
   data[Left].SetLv(-1.0*LeftLawArg->List[0]);
   data[Left].SetL(-1.0*LeftLawArg->List[0]);
   L = LeftLawArg->List[0];
   if (border != 0) //Слева находится не один элемент АР
   {for (uint i = border; i > 0; i--)
    {LV = LeftLaw(LeftLawArg); L += LV;
     data[i].SetLv(-1.0*LV); data[i].SetL(-1.0*L);
     LeftLawArg->List[0] = LV;
    }
   }
  }
  //Табличный закон
  else if (LeftLawType == TABLE_LAW)
  {uint j = 1; L = 0;
   for (uint i = Left; i > 0; i--)
   {LV = LeftLawArg->List[j++];
    data[i].SetLv(-1.0*LV); L += LV; data[i].SetL(-1.0*L);
   }
  }
  //Случайный закон
  else if (type == CASUALLAW)
  {L = 0;
   for (uint i = Left; i > 0; i--)
   {LV = LeftLaw(LeftLawArg);
    data[i].SetLv(-1.0*LV); L += LV; data[i].SetL(-1.0*L);
   }
  }
 }
 //****************************************************************************
 //2. Определяем относительные расстояния между элементами АР, расположенными
 //   справа от основного
 //****************************************************************************
 if (Right != 0) //Имеются элементы АР, расположенные СПРАВА от основного.
 {//Поиск индекса закона соответствующего RightLawType в DistLawTable
  for (uint i = 0; i < sz_DistLawTable; i++)
   if(DistLawTable[i].ident == RightLawType) { index = i; break; }
  type = DistLawTable[index].type;
  //Детерминированный, нетабличный закон
  if ((type == DETLAW) && (RightLawType != TABLE_LAW))
  {//Определяем нижнюю границу расположения элементов АР в массиве data
   border = Left + Together + 1;
   data[border].SetLv(RightLawArg->List[0]);
   data[border].SetL(RightLawArg->List[0]);
   L = RightLawArg->List[0];
   if (border != Count-1) //Справа находится не один элемент АР
   {for (uint i = border+1; i < Count; i++)
    {LV = RightLaw(RightLawArg); L += LV;
     data[i].SetLv(LV); data[i].SetL(L);
     RightLawArg->List[0] = LV;
    }
   }
  }
  //Табличный закон
  else if (RightLawType == TABLE_LAW)
  {uint j = 1; L = 0;
   border = Left + Together + 1;
   for (uint i = border; i < Count; i++)
   {LV = RightLawArg->List[j++];
    data[i].SetLv(LV); L += LV; data[i].SetL(L);
   }
  }
  //Случайный закон
  else if (type == CASUALLAW)
  {border = Left + Together + 1; L = 0;
   for (uint i = border; i < Count; i++)
   {LV = RightLaw(RightLawArg);
    data[i].SetLv(LV); L += LV; data[i].SetL(L);
   }
  }
 }
}
//*****************************************************************************
//СТАТУС: I.4; public member-function of class TLinearArray
//НАИМЕНОВАНИЕ ФУНКЦИИ:
//  void SetAngle(double AzimuthAngle, double ElevateAngle, bool Deg = DEG)
//НАЗНАЧЕНИЕ:
//Функция устанавливает углы источника сигнала, соответствующие E-плоскости и
//H-плоскости, для всех элементов АР в зависимости от их расположения в прост-
//ранстве.
//Если E-плоскость антенны совпадает с меридиональной плоскостью Земли, то ей
//будет соответствовать ElevateAngle, а H-плоскости - AzimuthAngle.
//Если E-плоскость антенны совпадает с экваториальной плоскостью Земли, то ей
//будет соответствовать AzimuthAngle, а H-плоскости - ElevateAngle.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. double AzimuthAngle - угол азимута источника сигнала
//2. double ElevateAngle - угол места источника сигнала
//3. bool Deg - индикатор измерения углов по азимуту и места (в градусах или в
//   в радианах)
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: нет
//*****************************************************************************
void TLinearArray::SetAngle(double AzimuthAngle, double ElevateAngle, bool Deg)
{
 //Цикл установки углов места и азимута источника сигнала для всех элементов АР,
 //которые используются в расчетах (с usage == ON)
 for (ulong i = 0; i < Count; i++)
  if (data[i].GetUsage() == ON) data[i].SetAngle(AzimuthAngle,ElevateAngle,Deg);
}
//*****************************************************************************
//СТАТУС: I.5; public; TLinearArray class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ: void SetRandNumber();
//НАЗНАЧЕНИЕ:
//Функция получает текущие значения случайных амплитуд и фаз для вычисления
//характеристик шума на выходе приемной антенны для всех элементов АР, которые
//используются при вычислениях.
//Случайные значения амплитуд (фаз) заносятся в список аргументов случайных
//законов под индексом равным 0. Данная функция позволяет исключить ситуацию
//двойного обращения к случайному закону, описывающему поведение фазы или ампли-
//туды шума, при определении мгновенных значений напряжений или мощностей
//шума в один и тот же момент времени t. Двойное обращение к случайным функциям
//привело бы к получению РАЗНЫХ случайных значений амплитуды (фазы), что
//является некорректным. Случайные характеристики шума приемной антенны:
//1. NoiseAmpl  - ARGLIST* NoiseAmplArg class Antenna
//2. NoisePhase - ARGLIST* NoisePhaseArg class Antenna
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ: нет
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: нет
//*****************************************************************************
void TLinearArray::SetRandNumber()
{//Цикл получения случайных значений амплитуды и фазы шума для всех элементов АР,
 //которые используются при вычислениях
 for (ulong i = 0; i < Count; i++)
  if (data[i].GetUsage() == ON) data[i].SetRandNumber();
}
//*****************************************************************************
//СТАТУС: I.6; public member-function of class TLinearArray
//НАИМЕНОВАНИЕ ФУНКЦИИ:
//rvector AntPatterns(bool EarthPlane, double ScanAngle = 0,
//        double InitAngle = -90.0, double EndAngle = 90.0, double Step = 0.1,
//        bool Deg = DEG);
//НАЗНАЧЕНИЕ:
//Функция определяет диаграмму направленности антенной решетки в меридиональной
//(по углу места) или в экваториальной (по азимуту) плоскости в секторе от
//InitAngle до EndAngle. Углы отсчитываются относительно нормали к раскрыву
//основного элемента антенной решетки.
//Функция характеристики направленности антенны в меридиональной или экваториаль-
//ной плоскости Земли выбирается в зависимости от значения EarthPlane.
//Так как АР может располагаться как в горизонтальной, так и вертикальной плос-
//кости, то ее диаграмма направленности будет зависеть от того в какой плоскости
//(экваториальной или меридиональной) будет она вычисляться. Если расположение
//АР будет совпадать с EarthPlane, то на диаграмму направленности АР окажут влия-
//ние фазовые набеги, возникающие в элементах АР вследствии их пространственного
//различия, а также диаграммы направленности элементов АР и их относительный
//коэффициент усиления, в случае неоднородной АР. Если расположение АР будет от-
//лично от EarthPlane, то фазовые набеги между элементами будут равны нулю и на
//диаграмму направленности АР окажут влияние только диаграммы направленности
//элементов АР и их относительные коэффициенты усиления в случае неоднородной АР.
//Данная функция вычисляет НОРМИРОВАННУЮ ДИАГРАММУ НАПРАВЛЕННОСТИ АР по формуле:
// G(q) = 10*log10{|A(q)|^2/N^2} [Db] - в децибелах,
// 1. q - угол между источником сигнала и нормалью к оси решетки, проходящая
//    через ОСНОВНОЙ элемент АР.
// 2. N - общее количество элементов АР.
// 3. A(q) - диаграмма направленности АР в направлении на источник сигнала
// ДИАГРАММА НАПРАВЛЕННОСТИ АР A(q) вычисляется:
// A(q) = SUM{ARF[i]*F(q)[i]*EXP(j(i-1)Ph[i]) by i = 1 to N},
// 1. i - i-ый элемент АР;
// 2. ARF[i] - относительный коэфф-т усиления i-го элемента АР
// 3. F(q)[i] - значение нормированной диаграммы направленности i-го элемента АР
//    в направлении источника сигнала
// 4. Ph[i] - фазовый набег (сдвиг) в i-ом элементе АР по сравнению с ОСНОВНЫМ.
// Ph[i] = 2*PI*(L[i])*sin(q), L[i] - относительное расстояние между i-ым и ос-
// новным элементами АР.
//Все значения характеристики направленности АР в заданном секторе заносятся в
//вектор Data. Структура хранимых данных в векторе Data:
//Data[0] - InitAngle; Data[1] - EndAngle; Data[2] - Step; Data[3]...Data[n] -
//данные.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. bool EathPlane - плоскость Земли, определяющая выбор функции направленности
//   антенны. (MERIDIAN; EQUATOR))
//2. double ScanAngle (0 deg) - направление главного лепестка
//3. double InitAngle (-90 deg) - начальный угол, построения диаграммы направлен-
//   ности антенны (antenna patterns)
//4. double EndAngle (+90 deg) - конечный угол, построения диаграммы направлен-
//   ности антенны.
//5. double Step - приращение угла
//6. bool Deg (DEG) - представление угла (в градусах - DEG или радианах - RAD)
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
// rvector Data - диаграмма направленности антенны
//*****************************************************************************
rvector TLinearArray::AntPatterns(bool EarthPlane, double ScanAngle,
        double InitAngle, double EndAngle, double Step, bool Deg)
{ulong Total; //Количество данных, которое необходимо разместить в списке data

 if (EndAngle < InitAngle)
 { double Value = InitAngle;
   InitAngle = EndAngle;
   EndAngle = Value;
 }
 //Приращение угла может быть только положительным значением
 Step = fabs(Step);
 if (Step == 0.0) Step = (Deg == DEG) ? 0.1 : 0.1*DEG_RAD;
 //Определяем количество данных, которые будут размещены в списке data
 Total = ceil((EndAngle-InitAngle)/Step);
 if (((EndAngle-InitAngle)/Step) != 0.0) Total += 1;
 //Создаем вектор Data для хранения значений направленности LAA
 rvector Data(Total+3,COL,0.0);
 //Заносим в список сектор определения диаграммы направленности антенны
 //в градусах
 if (Deg == RAD) //Перевод из радиан в градусы
 { InitAngle = InitAngle*RAD_DEG;
   EndAngle = EndAngle*RAD_DEG;
   Step = Step*RAD_DEG;
 }
 Data[0] = InitAngle; Data[1] = EndAngle; Data[2] = Step;
 double Angle = InitAngle;
 double Phi; //Фазовый набег в i-ом элементе АР
 double Ampl;
 complex Sum;
 complex Init(0,0);
 complex C_ONE(1,0);

 //Расчет коэффициентов лучеобразующей матрицы антенной решетки, задающих нап-
 //равление главного лепестка
 complex* W = new complex[Count];
 if (Deg == DEG) //Перевод из градусов в радианы
  ScanAngle *= DEG_RAD;
 if (ScanAngle == 0.0)
   for (ulong i = 0; i < Count; i++) W[i] = C_ONE;
  else
   for (ulong i = 0; i < Count; i++)
    {Phi = -1.0 * GetL(i) * sin(ScanAngle) * TWO_PI;
     W[i] = polar(1, Phi);
    }

 //Определение диаграммы направленности АР в заданном секторе обзора
 for (ulong index = 3; index < Data.Size(); index++)
 {Sum = Init;
  //Задаем угол Angle в зависимости от EarthPlane, в которой вычисляется
  //диаграмма направленности АР
  if (EarthPlane == EQUATOR) SetAngle(Angle,0,Deg);
  else SetAngle(0,Angle,Deg);
  //Цикл расчета диаграммы направленности АР в направлении на угол Angle
  for (ulong i = 0; i < Count; i++)
  { //Элемент АР используется при расчетах
    if (data[i].GetUsage() == ON)
    {//Вычисляем фазовый сдвиг в i-ом элементе АР
     //Плоскость расположения АР совпадает с EarthPlane
     if (EarthPlane == Disposition)
     {Phi = Angle*DEG_RAD;
      Phi = data[i].GetL()*sin(Phi)*TWO_PI;
     }
     //Плоскости расположения АР и EarthPlane не совпадают, фазовые набеги между
     //элементами АР отсутствуют
     else Phi = 0.0;
     //Вычисляем значение направленности для i-го элемента АР
     Ampl = data[i].GetN()*data[i].CalcAntPatterns(EarthPlane);
     Sum += polar(Ampl,Phi)*W[i];
    }
  } //Конец цикла по i - элементам АР
  Data[index] = norm(Sum);
  Angle = Angle+Step; //Вычисляем новую величину угла
  if (fabs(Angle) <= 1.0e-6) Angle = 0.0;
 } //Конец цикла по index - сектору определения диаграммы направленности АР
 delete[] W;
 //Поиск максимального значения
 double Max = Data[3];
 for (ulong index = 4; index < Data.Size(); index++)
  if (Max < Data[index]) Max = Data[index];
 //Вычисление нормированной диаграммы направленности адаптивного пространствен-
 //ного фильтра
 for (ulong index = 3; index < Data.Size(); index++)
  Data[index] = 10*log10(Data[index]/Max);
return Data;
}

//****************************************************************************
// I. ОПИСАНИЕ КОНСТРУКТОРОВ И ДЕСТРУКТОРОВ КЛАССА TLinearArray
//****************************************************************************

//*****************************************************************************
//СТАТУС: I.1; public; constructor by default
//НАИМЕНОВАНИЕ ФУНКЦИИ: TLinearArray()
//НАЗНАЧЕНИЕ: Конструктор по умолчанию класса TLinearArray.
//Функция предназначена для создания класса TLinearArray по умолчанию. Так как
//класс TLinearArray является производным класса TElArray, то перед вызовом
//конструктора вызывается конструктор по умолчанию класса TElArray. В ходе рабо-
//ты конструктора создается таблица функций DistanceLaw, определяющие относитель-
//ные расстояния между двумя соседними элементами антенной решетки.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ: нет
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: нет
//*****************************************************************************
TLinearArray::TLinearArray() 
{ //Начальная инициализация характеристик линейной АР
  InitPerformance();
}
//*****************************************************************************
//СТАТУС: II.2; public; constructor with argument
//НАИМЕНОВАНИЕ ФУНКЦИИ: TLinearArray(const char* filename)
//НАЗНАЧЕНИЕ: Конструктор с аргументом класса TLinearArray.
//Функция предназначена для создания класса TLinearArray по данным находящимся в
//текстовом файле filename. Этот файл специально организован. При считывании
//данных из этого файла осуществляется проверка его корректности. В случае оши-
//бок или несоответствия структуры файла требуемому формату генерируется прину-
//дительное завершение работы конструктора, ошибка, вызвавшая прерывание регист-
//рируется в файле filename.err. Расширение входного файла с данными по линейной
//АР не должно иметь расширение - ".err".
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ: const char* filename - имя входного файла
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: нет
//*****************************************************************************
TLinearArray::TLinearArray(const char* filename)
{bool lSuccess;
 //Начальная инициализация характеристик линейной АР
 InitPerformance();
 try
 { //Открываем файл с характеристиками линейной антенной решетки
   lSuccess = OpenDataFile(filename);
   if (!lSuccess) throw FOPEN_ERROR;
   //Считывание характеристик линейной АР из текстового файла и инициализация
   //членов-данных объекта класса TLinearArray с обработкой возможных ошибок
   lSuccess = GetDataFromFile();
   if (!lSuccess) throw FREAD_ERROR;
   if (lSuccess == true)
   {fin.close();
    ferr.close();
    remove(ferrname.c_str());//Удаление созданного файла регистрации ошибок
    #if defined(CONSOLE)
     clrscr();
     cout << "The input data on LAA components from file <" << fname
          << ">" << "\nare read out successfully." << endl;
     cout << "Press any key ...";
     getch();
    #endif
   }
 }
 //Обработка исключительной ситуации
 catch (int error_type)
 {if (error_type == FOPEN_ERROR) //Не удалось открыть файл данных
  {
   #if defined(CONSOLE)
    cout << endl;
    cout << "ERROR: CAN'T OPEN DATA FILE <" << uppercase << fname  //to_upper(fname)
         << "> !!!" << endl;
    cout << "The LINEAR ARRAY design on file data is impossible." << endl;
    cout << "The program will be terminated." << endl;
    cout << "Press any key ..."; getch();
   #endif
  }
  else if (error_type == FREAD_ERROR) //Ошибка считывания данных
  {
   #if defined(CONSOLE)
    cout << endl;
    cout << "ERROR: DETECT ERROR AT READING DATA FROM FILE <" << uppercase << fname //to_upper(fname)
         << "> !!!" << endl;
    cout << "The LINEAR ARRAY design on file data is impossible." << endl;
    cout << "Information about a probable error is in the file <"
         << uppercase << ferrname << ">." << endl;
    cout << "The program will be terminated." << endl;
    cout << "Press any key ..."; getch();
   #endif
  }
  //Удаление объекта TLinearArray
  #if defined(CONSOLE)
   cout << "\nDestroy object of TLinearArray class..." << endl;
  #endif
  //Удаление всех характеристик линейной АР
  ResetPerformance();
 } //Завершение блока обработки исключительных ситуаций

}
//*****************************************************************************
//СТАТУС: I.3; public; TLinearArray class destructor
//НАИМЕНОВАНИЕ ФУНКЦИИ: ~TLinearArray()
//НАЗНАЧЕНИЕ: Деструктор класса TLinearArray.
//Функция предназначена для разрушения класса. При работе функции удаляется таб-
//лица законов, определяющих относительные расстояния между двумя элементами
//линейной АР DistanceLaw, списки параметров законов, массив элементов АР,
//составляющих линейную антенную решетку.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ: нет
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: нет
//*****************************************************************************
TLinearArray::~TLinearArray()
{
 #if defined(CONSOLE)
   cout << "\nDestroy object of TLinearArray class..." << endl;
 #endif
 //Удаление всех характеристик линейной АР
 ResetPerformance();
}

//===================================================================
// II. ОПИСАНИЕ ЗАКРЫТЫХ ЧЛЕНОВ-ФУНКЦИЙ КЛАССА TLinearArray
//     PRIVATE MEMBER-FUNCTION OF TLinearArray CLASS
//===================================================================

//=============================================================
//III. ОПИСАНИЕ ЗАЩИЩЕННЫХ ЧЛЕНОВ-ФУНКЦИЙ КЛАССА TLinearArray
//     PROTECTED MEMBER-FUNCTION OF TLinearArray CLASS
//=============================================================
//*****************************************************************************
//СТАТУС: III.1., protected, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ: virtual void InitPerformance();
//НАЗНАЧЕНИЕ:
//Функция осуществляет начальную инициализацию характеристик линейной антенной
//решетки при ее конструировании. По умолчанию устанавливаются следующие харак-
//теристики Linear Array: однородная, эквидистантная, расположена по горизонтали.
//Начальное количество элементов АР равно нулю. Законы расположения элементов -
//нулевые. Всем указателям на данные присваивается NULL.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ: нет
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: нет
//*****************************************************************************
void TLinearArray::InitPerformance()
{ //Начальная инициализация характеристик объекта класса
  Uniformity = UNIFORM; //Однородная
  Equidistance = EQUIDISTANT; //Эквидистантная
  Left = 0; Together = 0; Right = 0; //Начальная конфигурация антенной решетки
  Disposition = EQUATOR; //Горизонтальное расположение
  //Инициализация законов и их параметров, определяющих относительные расстояния
  //между двумя соседними элементами линейной антенной решетки
  LeftLawType = ZERO_LAW;
  LeftLaw = ZeroLaw;
  LeftLawArg = NULL;
  RightLawType = ZERO_LAW;
  RightLaw = ZeroLaw;
  RightLawArg = NULL;
  Count = 0L;
  data = NULL;
}

//*****************************************************************************
//СТАТУС: III.2., protected, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ: virtual void ResetPerformance();
//НАЗНАЧЕНИЕ:
//Функция осуществляет сброс всех характеристик линейной антенной решетки. Функ-
//ция может использоваться при удалении объекта Linear Array, а также в случае
//обновления всех характеристик антенной решетки. Устанавливаются следующие
//характеристики Linear Array: однородная, эквидистантная, расположена по горизонтали.
//Начальное количество элементов АР равно нулю. Законы расположения элементов -
//нулевые. Всем указателям на данные присваивается NULL. Освобождается вся дина-
//мическая память выделенная под хранение элементов антенной решетки и параметры
//законов размещения элементов относительно друг друга.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ: нет
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: нет
//*****************************************************************************
void TLinearArray::ResetPerformance()
{ //Сброс ранее установленных характеристик объекта класса
  Uniformity = UNIFORM; //Однородная
  Equidistance = EQUIDISTANT; //Эквидистантная
  Left = 0; Together = 0; Right = 0; //Начальная конфигурация антенной решетки
  Disposition = EQUATOR; //Горизонтальное расположение
  //Инициализация законов и их параметров, определяющих относительные расстояния
  //между двумя соседними элементами линейной антенной решетки
  LeftLawType = ZERO_LAW;
  LeftLaw = ZeroLaw;
  if (LeftLawArg != NULL) LeftLawArg = DeleteArgList(LeftLawArg);
  RightLawType = ZERO_LAW;
  RightLaw = ZeroLaw;
  if (RightLawArg != NULL) RightLawArg = DeleteArgList(RightLawArg);
  Count = 0L;
  //Удаление элементов линейной антенной решетки
  if (data != NULL) { delete[] data; data = NULL; }
}

//*****************************************************************************
//ОПИСАНИЕ ФУНКЦИЙ, ПРЕДНАЗНАЧЕННЫХ ДЛЯ СЧИТЫВАНИЯ ХАРАКТЕРИСТИК ЛИНЕЙНОЙ АР
//ИЗ ТЕКСТОВОГО ФАЙЛА СПЕЦИАЛЬНОГО ВИДА ПРИ ЕЕ КОНСТРУИРОВАНИИ
//*****************************************************************************

//*****************************************************************************
//СТАТУС: III.3., protected, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ: OpenDataFile(const char* filename)
//НАЗНАЧЕНИЕ:
//Функция открывает входной файл с данными по линейной АР, создает файл регистра-
//ции возможных ошибок, которые могут появится в файле данных. Файл регистрации
//ошибок ferrname будет иметь имя сходное с входным файлом fname и расширением
//".err". По этой причине расширение ".err" в имени fname недопустимо. Входной
//файл открывается в режиме ЧТЕНИЯ. Если исходное имя файла не указано или файл
//с таким именем не существует, то в консольном режиме предлагается ввести новое
//имя входного файла. При неудачной попытке открытия файла fname или если файл
//будет иметь расширение ".err", функция возвращает false.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. const char* filename - наименование файла с входными данными по АР
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: bool lSuccess
// true - файла с данными открыт
// false - данные с данными открыть не удалось
//*****************************************************************************
bool TLinearArray::OpenDataFile(const char* filename)
{ bool lSuccess = true;
  char c;
  uint Y, CurY;
  //************************************************************************
  //ШАГ I.
  //Открываем текстовой файл с данными по EXTERNAL SIGNAL в режиме ЧТЕНИЯ
  //************************************************************************
  fname = filename;
  if (fname.length() == 0) //Имя входного файла с данными не задано
  {lSuccess = false;
   #if defined(CONSOLE)
    Y = wherey();
    cout << "\nERROR: filename is absent." << endl;
    cout << "Enter new filename? [Y/N]: ";
    cin >> c;
    //Очистка экрана
    CurY = wherey(); gotoxy(1,Y);
    for(uint i = Y; i <= CurY; i++) {clreol(); cout << endl;}
    gotoxy(1,Y);
    if ((c == 'Y') || (c == 'y')) lSuccess = OpenFileFromConsole();
    if (!lSuccess) return lSuccess;
   #endif
  }
  //Открываем входной файл
  else
  { fin.open(fname.c_str(),ios::in);
    if (!fin)
    { lSuccess = false;
     #if defined(CONSOLE)
      Y = wherey();
      cout << "\nERROR: unable to open <" << fname << ">" << endl;
      cout << "Try again ? [Y/N]: "; cin >> c;
      //Очистка экрана
      CurY = wherey();  gotoxy(1,Y);
      for(uint i = Y; i <= CurY; i++) {clreol(); cout << endl;}
      gotoxy(1,Y);
      if ((c == 'Y') || (c == 'y')) lSuccess = OpenFileFromConsole();
      if (!lSuccess) return lSuccess;
     #endif
    }
    else //Файл с именем filename открыт в режиме чтения
    {
     #if defined(CONSOLE)
      //Очистка экрана
      CurY = wherey();  gotoxy(1,Y);
      for(uint i = Y; i <= CurY; i++) {clreol(); cout << endl;}
      gotoxy(1,Y);
     #endif
    }
  }
  //**************************************************************************
  //ШАГ II. Открываем файл регистрации ошибок, которые могут быть обнаружены
  //        при считывании данных из входного файлового потока fin - *.err
  //**************************************************************************
  //Извлечение имени входного файла без расширения
  string s;
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
      cout << "Input data file <" << fname << "> contains the inadmissiblis "
           << "extension - .err." << endl;
     #endif
     return lSuccess;
    }
   //Формирование имени файла регистрации ошибок ferrname с именем совпадающим
   //с именем входного файла данных fname и расширением ".err"
   pos = fname.find(".");
   ferrname = fname.substr(0,pos)+".err";
  }
  s.resize(70);
  for (uint i = 0; i < s.length(); i++) s[i] = '=';
  ferr.open(ferrname.c_str(),ios::out);
  ferr << s << endl;
  string t("ФАЙЛ РЕГИСТРАЦИИ ОШИБОК, ВОЗНИКШИХ ПРИ СЧИТЫВАНИИ ДАННЫХ ИЗ ФАЙЛА");
  pos = (s.length()-t.length())/2;
  for (uint i = 0; i < s.length(); i++) s[i] = ' ';
  s.insert(pos,t);
  ferr << s << endl;
  s.resize(70);
  for (uint i = 0; i < s.length(); i++) s[i] = ' ';
  s.insert((s.length()-fname.length())/2,fname);//to_upper(fname)
  ferr << s << endl;
  s.resize(70);
  for (uint i = 0; i < s.length(); i++) s[i] = '=';
  ferr << s << endl;

 return lSuccess;
}
//*****************************************************************************
//СТАТУС: III.4., protected, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ: bool OpenFileFromConsole()
//НАЗНАЧЕНИЕ:
//Функция предназначена для открытия входного файлового потока в текстовом
//режиме и только для чтения. Имя файла хранится в защищенной переменной класса
//TLinearArray - fname, входной файловый поток выражается через защищенную пере-
//менную класса TLinearArray - fin.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ: нет
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: bool lSuccess
// true - текстовой файл удалось открыть в режиме чтения
// false - текстовой файл не удалось открыть в режиме чтения
//*****************************************************************************
bool TLinearArray::OpenFileFromConsole()
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
//СТАТУС: III.5., protected, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ:
// bool GetLineWithoutComment(ifstream fin, string& buffer, const string& ABC,
//      const char** comment = NULL, const uint sz_comment = 0)
//НАЗНАЧЕНИЕ:
//Функция предназначена для чтения очередной строки из файлового потока fin и
//записи ее в строковый объект buffer. Перед записью строки осуществляется уда-
//ление из нее комментариев, которые отделяются от основного текста символами -
//разделителями, содержащимися в массиве comment. Каждая строка файла должна
//заканчиваться символом - '\n'. Строки, которые не содержат символы, определен-
//ные в строковом объекте ABC, пропускаются. При достижении конца файла
//функция возвращает значение false, что означает событие, при котором строк
//с вышеопределенными требованиями в файле ассоциированном с входным потоком
//fin больше нет.
//Глобальные переменные, используемые функцией: 1) Buffer 2) CurRow
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. ifstream fin - входной файловый поток
//2. string& line - ссылка на строку, в которую будет считана строка из файла,
//                 без комментариев, если в ней имеются символы из алфавита ABC
//3. string& ABC - алфавит символов, которые должны присутствовать в считанной
//                 из файла строке для ее записи в строку line
//4. const char** comment - массив с символами-разделителями комментарий от данных
//5. const uint sz_comment - количество строк в массиве comment
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: bool lSuccess
// true -  найдена строка, которая имеет символы из допустимого алфавита, и она
//         записана в buffer
// false - достигнут конец файла, строк с определенным типом данных больше нет
//*****************************************************************************
bool TLinearArray::GetLineWithoutComment(ifstream& fin, string& line,
     const string& ABC, const char** comment, const uint sz_comment)
{ bool lSuccess = true;
  bool lFind = false;
  uint pos;
  //Чтение строки из файла
  while ((!fin.eof()) && (!lFind))
  { getline(fin,Buffer,'\n'); //Извлечение строки из файла
    CurRow++; //Счетчик числа строк в файле
   if (Buffer.length() != 0) //Буфер содержит символы
   {//Поиск комментарий в строке
    line = Buffer;
    if (comment != NULL)
    { //Цикл поиска символов-разделителей данных от комментарий
      for (uint i = 0; i < sz_comment; i++)
      { pos = line.find(comment[i]);
        //Символы-разделители комментарий от текста найдены
        //Отбрасываем комментарии
        if (pos != string::npos) {line = line.substr(0,pos); break;}
      }
    }
    //Анализируем строку line на присутствие в ней допустимых символов
    pos = line.find_first_of(ABC);
    //Допустимые символы найдены, завершаем цикл считывания строки
    if (pos != string::npos)
    {lFind = true; lSuccess = true;
     //Удаление начальных и конечных символов из строки line, которые не являют-
     //ся символами алфавита ABC.
     line = line.substr(pos); //Удаление начальных не-ABC символов
     //Поиск в строке line последнего символа из допустимого алфавита ABC
     pos = line.find_last_of(ABC);
     //Удаление конечных не-АВС символов
     if (pos != string::npos) line = line.substr(0,pos+1);
     else line = line.substr(0,1);
    }
   }
  } //Конец цикла считывания строк из файлового потока fin

  //Строку с заданными параметрами найти не удалось
  //Достигнут конец файла
  if (lFind == false) {lSuccess = false; line.resize(0);}
return lSuccess;
}
//****************************************************************************
//СТАТУС: III.6., protected, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ:
//       bool SearchKeyString(const string& s, const char** pattern,
//                            const uint sz_pattern, pKEYSTRPOS info = NULL)
//НАЗНАЧЕНИЕ:
//Функция предназначена для поиска ключевой строки (ключевого слова) возможные
//варианты которой содержатся в массиве шаблонов - pattern в строке s. Если сов-
//падение будет найдено, функция возвращает true, в противном случае - false.
//В случае удачного поиска и наличия ненулевого указателя на структуру KEYSTRPOS,
//она будет содержать индекс ключевой строки в массиве pattern, с которой
//произошло совпадение, ее правая и левая границы в строке s.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. const string& s - ссылка на строку, в которой будет осуществлятся поиск
//                     ключевой строки
//2. const char** pattern - массив шаблонов возможных вариантов ключевых строк
//3. const uint sz_pattern - количество строк в массиве pattern
//4. pKEYSTRPOS - указатель на структуру с информацией о местоположении ключевой
//   строки в строке s и массиве pattern при условии ее совпадения с одним из
//   ключевых шаблонов массива pattern. Данный параметр является необязательным,
//   по умолчанию - NULL.
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: bool lSuccess
// true -  ключевая строка в строке s найдена
// false - ключевая строка в строке s не найдена
//*****************************************************************************
bool TLinearArray::SearchKeyString(const string& s, const char** pattern,
                   const uint sz_pattern, pKEYSTRPOS info)
{ bool lSuccess = false;
  uint lpos;
  //Начальная инициализация структуры
  if (info != NULL) {info->index = info->lpos = info->rpos = string::npos;}
  //Поиск ключевой строки
  for (uint i = 0; i < sz_pattern; i++)
  { lpos = s.find(pattern[i]);
    //Ключевая строка найдена
    if (lpos != string::npos)
    {lSuccess = true;
     //Получение параметров по ключевой строке
     if (info != NULL)
     {info->index = i;
      info->lpos = lpos; info->rpos = lpos + strlen(pattern[i]) - 1;
      break;
     }
    }
  }
return lSuccess;
}
//****************************************************************************************
//СТАТУС: III.7., protected, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ:
// void Display(ofstream& out, const char** s, const char* title = NULL, const uint sz_s,
//              const char* left = NULL , const char* right = NULL, bool num = true);
//НАЗНАЧЕНИЕ: Функция выводит в поток строковый массив. Каждая строка массива выводиться с
//новой строки.
//----------------------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. ofstream& out - ссылка на поток, в который будет выводиться s;
//2. const char** s - ссылка на массив строк;
//3. const uint sz_s - количество строк в массиве s;
//4. const char* title - заголовок выводимой информации;
//5. const char* left - строка предшествующая очередной выводимой строке из массива s;
//6. const char* right - строка завершающая очередную выводимую строку из массива s;
//7. bool num - режим нумерации выводимых строк (num = true).
//----------------------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: нет
//****************************************************************************************
void TLinearArray::Display(ofstream& out, const char** s, const uint sz_s,
     const char* title, const char* left, const char* right, bool num)
{uint width;
 if (left == NULL) left = "< ";
 if (right == NULL) right = " >";

 if (num == true) //Нумеровать строки
 { if (sz_s < 10) width = 1;
   else if ((sz_s >= 10)   && (sz_s < 100)) width = 2;
   else if ((sz_s >= 100)  && (sz_s < 1000)) width = 3;
   else if ((sz_s >= 1000) && (sz_s < 10000)) width = 4;
   else width = 5;
 }
 //Отображение заголовка массива строк
 out << endl;
 if (title != NULL) {out << title << endl;}
 //Цикл вывода строк в поток
 //Очередная строка массива строк s выводиться с новой строки
 for(uint i = 0; i < sz_s; i++)
 { if (num) {out << setw(width) << (i+1) << ". ";}
   out << left << s[i] << right << endl;
 }
}
//****************************************************************************
//СТАТУС: III.8., protected, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ:
//bool FindKeyStrInFile(const string& TitleKeyStr, const char** KeyStr,
//     const uint sz_KeyStr, string& Line, const pKEYSTRPOS info, const string& ABC,
//     const char** Comment, const uint sz_Comment, ifstream& fin, ofstream& ferr,
//     const char** Perform, const uint sz_Perform);
//НАЗНАЧЕНИЕ:
//Функция предназначена для поиска ключевой строки (ключевого слова) возможные
//варианты которой содержатся в массиве шаблонов - KeyStr в файловом потоке fin.
//Если совпадение будет найдено, функция возвращает true, в противном случае -
//false. В случае удачного поиска указатель на структуру pKEYSTRPOS info, будет
//содержать индекс ключевой строки в массиве KeyStr, с которой произошло совпа-
//дение, ее правая и левая границы в строке Line. При поиске ключа функция анали-
//зирует только те строки файла, которые содержат символы алфавита ABC, исключая
//из рассмотрения комментарии, которые отделяются от данных символами из массива
//строк Comment. В случае неудачи, в файловом потоке ferr регистрируется инфор-
//мация о ее причине и с какой ключевой фразой она была связана. Кроме того, по
//информации из массива Perform в файловый поток ferr помещается информация о
//структуре данных (шаблон данных), которую необходимо было считать из файла.
//ЗАМЕЧАНИЕ. Если считанная из файла строка без комментариев будет содержать хотя
//бы один символ из алфавита ABC и в этой строке не окажется искомой ключевой
//строки, то функция завершает свою работу со значением false.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//01. const string& TitleKeyStr - ссылка на строку, в которой наименование иско-
//    мой ключевой строки в файловом потоке fin
//02. const char** KeyStr - массив шаблонов возможных вариантов ключевых строк
//03. const uint sz_KeyStr - количество строк в массиве KeyStr
//04. string& Line - ссылка на строку, в которую будет переписана строка из fin в
//    случае удачного поиска, исключая комментарии.
//05. pKEYSTRPOS - указатель на структуру с информацией о местоположении ключевой
//    строки в строке Line и массиве KeyStr при условии ее совпадения с одним из
//    ключевых шаблонов массива KeyStr.
//06. const string& ABC - ссылка на разрешенный алфавит символов. Если строка из
//    файлового потока не будет содержать хотя бы одного символа из ABC поиск в
//    ней вестись не будет.
//07. const char** Comment - массив с возможными символами-разделителями информа-
//    ции от комментарий.
//08. const uint sz_Comment - количество строк в массиве Comment
//09. ifstream& fin - входной файловый поток
//10. ofstream& ferr - выходной поток с ошибками
//11. const char** Perform - массив строк, содержащий шаблон структуры, которую
//    необходимо считать из файла
//12. const uint sz_Perform - количество строк в массиве Perform
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: bool lSuccess
// true -  ключевая строка в файловом потоке fin найдена
// false - ключевая строка в файловом потоке fin не найдена
//*****************************************************************************
bool TLinearArray::FindKeyStrInFile(const string& TitleKeyStr,const char** KeyStr,
     const uint sz_KeyStr,string& Line,const pKEYSTRPOS info,const string& ABC,
     const char** Comment,const uint sz_Comment,ifstream& fin,ofstream& ferr,
     const char** Perform,const uint sz_Perform)
{ bool lSuccess;
  string title;
  //Поиск ключевого слова TitleKeyStr
  lSuccess = GetLineWithoutComment(fin, Line, ABC, Comment, sz_Comment);
  //Строка содержащая символы ABC без комментариев не найдена.
  //Достигнут конец файла, формируем сообщение об ошибке
  if (lSuccess == false)
  {ferr << "ERROR:  End of file is reached. It was not possible to receive\n";
   ferr << "        a <" << TitleKeyStr << "> of Antenna Array.";
   title = "REASON: The following keywords were expected:";
   Display(ferr,KeyStr,sz_KeyStr,title.c_str(),"< "," >;");
   ferr << endl;
   ferr << "=============== The block diagram of Performances ==============";
   Display(ferr,Perform,sz_Perform,"","","",false);
   ferr << "================================================================";
   ferr << endl;
   return lSuccess; //Прекращение работы функции
  }
  lSuccess = SearchKeyString(Line, KeyStr, sz_KeyStr, info);
  //Ключевое слово не найдено, считанная из файла строка Line содержит
  //некорректную информацию. Формируем сообщение об ошибке
  if (lSuccess == false)
  {ferr << "ERROR:  The keyword <" << TitleKeyStr << "> is not found." << endl;
   ferr << "REASON: The following line of file contains an incorrect data.\n";
   ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
   ferr << "BUFFER: <" << Buffer << ">";
   title = "The following keywords were expected:";
   Display(ferr,KeyStr,sz_KeyStr,title.c_str(),"< "," >;");
   ferr << endl;
   ferr << "=============== The block diagram of Performances ==============";
   Display(ferr,Perform,sz_Perform,"","","",false);
   ferr << "================================================================";
   ferr << endl;
  }
return lSuccess;
}
//****************************************************************************
//СТАТУС: III.9., protected, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ:
//bool StrToBool(bool& value, const string& line, const pKEYSTRPOS info,
//              const pSTR_BOOL TableStrBool, const uint sz_TableStrBool,
//              const string& title,const char** Perform,const uint sz_Perform);
//НАЗНАЧЕНИЕ:
//Формат строки: <KEY STRING;>
//<KEY STRING> = <str1> || <str2> || <str3> ... || <strN>
//Функция предназначена для нахождения ключевой строки возможные варианты, кото-
//рых содержатся в таблице TableStrBool в строке Line и преобразования ее в ло-
//гическое значение по таблице соответствий TableStrBool. Формат строки приведен
//выше. В случае ошибок, функция возвращает значение false и информация об ошибке регист-
//рируется в файловом потоке ошибок ferr. Ошибкой считается необнаружение ключе-
//вой строки в строке Line, а также возможные синтаксические ошибки, связанные с
//отступлением от формата строки приведенного выше. К таким ошибкам относятся
//пропуск символа ';', наличие в строке других символов, кроме пробелов и табу-
//ляций. При успешном поиске и преобразовании в логическую переменную value за-
//носится результат преобразования строки в логическое значение.
//Глобальные переменные используемые функцией:
// 1. string Buffer; 2. string Line; 3. CurRow
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. bool& value - ссылка на логическую переменную, которой будет присвоен
//   результат работы функции в случае ее успешного завершения.
//2. const string& title - ссылка на строку, в которой наименование искомой
//   ключевой строки в файловом потоке fin
//3. string& line - ссылка на строку, в которой осуществляется поиск ключевой
//   строки.
//4. pKEYSTRPOS info - указатель на структуру с информацией о местоположении
//   предыдущей ключевой строки в строке line
//5. const pSTR_BOOL TableStrBool - таблица соответствия возможных вариантов
//   ключевых строк сопоставленным с ними логических значений
//6. const uint sz_TableStrBool - количество строк в таблице TableStrBool
//7. const char** Perform - массив строк, содержащий шаблон структуры, которую
//   необходимо считать из файла
//8. const uint sz_Perform - количество строк в массиве Perform
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: bool lSuccess
// true -  строка преобразована в логическое значение успешно
// false - преобразовать строку в логическое значение не удалось
//*****************************************************************************
bool TLinearArray::StrToBool(bool& value, const string& line,
     const pKEYSTRPOS info, const pSTR_BOOL TableStrBool,
     const uint sz_TableStrBool, const string& title, const char** Perform,
     const uint sz_Perform)
{bool lSuccess;
 uint rpos;
 //Поиск символа-разделителя ';' между ключевыми строками, наличие которого
 //обязательно, в строке Line
 rpos = line.find_first_of(';');
 if (rpos == string::npos) //Ошибка
 {lSuccess = false;
  ferr << "ERROR:  " << title << " to receive it was not possible." << endl;
  ferr << "REASON: Complete token <;> was expected for this data string" << endl;
  ferr << "Line N " << CurRow << ": <" << line << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << endl;
  ferr << "============== The block diagram of Performances ===============";
  Display(ferr,Perform,sz_Perform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return lSuccess;
 }
 //Извлекаем из Line подстроку s1 с завершающим символом ';'
 uint lpos = info->rpos+1;
 string s1 = line.substr(lpos,rpos-lpos+1);
 //s1.set_case_sensitive(0); //Отключаем контекстно-чувствительный поиск
 //Поиск ключевых строк из таблицы TableStrBool в строке s1
 lSuccess = false;
 uint index = 0;
 for (uint i = 0; i < sz_TableStrBool; i++)
 { lpos = s1.find(TableStrBool[i].s);
   if (lpos != string::npos) //Ключевая строка в таблице найдена
   {lSuccess = true;
    index = i;
    value = TableStrBool[i].value;
    rpos = lpos+strlen(TableStrBool[i].s)-1;
    break;
   }
 }
 if (lSuccess == false) //Ошибка: ключевая строка не найдена
 {ferr << "ERROR:  " << title << " to receive it was not possible." << endl;
  ferr << "REASON: Key string to find it was not possible, the logic value of\n";
  ferr << "        performance is not defined." << endl;
  ferr << "Line N " << CurRow << ": <" << s1 << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << "The following key strings were expected: " << endl;
  for (uint i = 0; i < sz_TableStrBool; i++)
  {
   ferr << setw(2) << (i+1) << ". <" << TableStrBool[i].s;
   if ((sz_TableStrBool-i) > 1) ferr << ">;" << endl;
   else ferr << ">." << endl;//Последняя строка
  }
  ferr << endl;
  ferr << "============== The block diagram of Performances ===============";
  Display(ferr,Perform,sz_Perform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return lSuccess;
 }
 //Анализ на присутствие посторонних символов в строке. Кроме символа пробела и
 //табуляции других символов быть не должно
 //Поиск посторонних символов слева от найденной ключевой строки
 string s2 = s1.substr(0,lpos);
 lpos = s2.find_first_not_of(" \t");
 if (lpos != string::npos) lSuccess = false;//Ошибка: запрещенные символы найдены
 //Поиск посторонних символов справа от найденной ключевой строки
 if (lSuccess != false)
 { s2 = s1.substr(rpos+1);
   lpos = s2.find_first_not_of(" \t;");
   if (lpos != string::npos) lSuccess = false;//Ошибка: запрещенные символы найдены
 }
 if (lSuccess == false) //Формируем сообщение об ошибке
 {ferr << "ERROR:  " << title << " to receive it was not possible." << endl;
  ferr << "REASON: The given line contains superfluous tokens, except a\n";
  ferr << "        value of performance - <" << TableStrBool[index].s << ">."
       << endl;
  ferr << "Line N " << CurRow << ": <" << line << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << endl;
  ferr << "============== The block diagram of Performances ===============";
  Display(ferr,Perform,sz_Perform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
 }
return lSuccess;
}
//****************************************************************************************
//СТАТУС: III.10., protected, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ:
//bool GetNextStrPosAfterSep(const string& line, int& lpos, const char* sep=NULL)
//НАЗНАЧЕНИЕ:
//Функция предназначена для определения позиции символа следующего за символом-
//разделителем, которые определены в массиве sep (по умолчанию - ';'). Поиск ве-
//дется в строке line слева направо. Если символ-разделитель не найден, функция
//возвращает false, а lpos = NPOS. Если символ-разделитель найден, но он является
//последним символом в строке, то lpos = NPOS.
//----------------------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. const string& line - ссылка на строку, в которой осуществляется определение
//   позиции символа, следующего за символом-разделителем.
//2. int lpos - положение первого символа в строке line, следующего за символом-
//   разделителем из массива sep.
//3. const char* sep - допустимые символы, принимаемые за символы-разделители
//----------------------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: bool lSuccess
//true - символ-разделитель найден в строке
//false - символ-разделитель в строке не найден
//****************************************************************************************
bool TLinearArray::GetNextStrPosAfterSep(const string& line, int& lpos, const char* sep)
{ bool lSuccess = false;
  if (sep == NULL) sep = ";";
  //Определяем местоположение символа-разделителя в строке line
  lpos = line.find_first_of(sep);
  //Символ-разделитель в строке отсутствует
  if (lpos == string::npos) return lSuccess;
  //Символ-разделитель найден. Анализируем его на то, что он последний в строке
  lSuccess = true;
  if (lpos == (int) line.length()-1) //Символ-разделитель последний в строке
   lpos = string::npos;
  else lpos++;
return lSuccess;
}
//****************************************************************************
//СТАТУС: III.11., protected, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ:
// bool StrToDouble(double& value, const string& line, const char** pattern,
//                  const uint sz_pattern, const string& title,
//                  const char** perform,const uint sz_perform);
//НАЗНАЧЕНИЕ:
//Формат строки: <KEY STRING <ASSIGNMENT OPERATOR> <value>;>
//<KEY STRING> = <str1> || <str2> || ... <strN>
//<ASSIGNMENT OPERATOR> = <=> || <:=> ...
//<value> - значение типа double
//<value> = [SIGN]DIGIT1_k[.]DIGITk+1_n[e||E[SIGN]DIGIT1_3]
//<SIGN> = <+> || <->; <DIGIT> = <0123456789>
//Функция предназначена для нахождения ключевой строки возможные варианты, кото-
//рых содержатся в символьном массиве pattern в строке line и преобразования
//символьного представления вещественного числа, которое должно следовать после
//оператора присваивания, в величину типа double. Формат строки приведен выше.
//В случае ошибок, функция возвращает значение false и информация об ошибке ре-
//гистрируется в файловом потоке ошибок ferr. Ошибкой считается необнаружение
//ключевой строки в строке Line, а также возможные синтаксические ошибки, свя-
//занные с отступлением от формата строки приведенного выше. К таким ошибкам
//относятся пропуск символа ';', наличие в строке других символов, кроме пробе-
//лов и табуляций, оператора присваивания, а также цифровых символов и знаков,
//которые необходимы для представления вещественного числа в символьном виде.
//При успешном поиске и преобразовании числа в символьном виде в переменную типа
//double  value заносится результат преобразования строки в вещественное значение.
//Глобальные переменные используемые функцией:
// 1. string Buffer; 2. string Line; 3. CurRow
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. double& value - ссылка на переменную, которой будет присвоен
//   результат работы функции в случае ее успешного завершения.
//2. const string& line - ссылка на строку, в которой осуществляется поиск
//   ключевой строки.
//3. const char** pattern - массив строк возможных вариантов ключевых строк
//4. const uint sz_pattern - количество строк в массиве pattern
//5. const string& title - ссылка на строку с наименованием искомой ключевой
//   строки в файловом потоке fin
//6. const char** perform - массив строк, содержащий шаблон структуры, которую
//   необходимо считать из файла
//7. const uint sz_perform - количество строк в массиве Perform
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: bool lSuccess
// true -  в строке найдено и успешно считано вещественное число
// false - найти или считать из строки вещественное число не удалось
//*****************************************************************************
bool TLinearArray::StrToDouble(double& value, const string& line,
     const char** pattern, const uint sz_pattern, const string& title,
     const char** perform, const uint sz_perform)
{bool lSuccess;
 int rpos;
 int lpos;
 //Поиск символа-разделителя ';' между ключевыми строками, наличие которого
 //обязательно, в строке Line
 rpos = line.find_first_of(';');
 if (rpos == string::npos) //Ошибка
 {lSuccess = false;
  ferr << "ERROR:  " << title << " to receive it was not possible." << endl;
  ferr << "REASON: Complete token <;> was expected for this data string" << endl;
  ferr << "Line N " << CurRow << ": <" << line << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << endl;
  ferr << "============== The block diagram of Performances ===============";
  Display(ferr,perform,sz_perform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return lSuccess;
 }
 //Извлекаем из line подстроку s1 с завершающим символом ';'
 string s1 = line.substr(0,rpos+1);
 //s1.set_case_sensitive(0); //Отключаем контекстно-чувствительный поиск
 //Поиск ключевых строк из массива строк pattern в строке s1
 KEYSTRPOS keystr_info;
 lSuccess = SearchKeyString(s1, pattern, sz_pattern, &keystr_info);

 if (lSuccess == false) //Ошибка: ключевая строка не найдена
 {ferr << "ERROR:  " << title << " to receive it was not possible." << endl;
  ferr << "REASON: Key string to find it was not possible, the logic value of\n";
  ferr << "        performance is not defined." << endl;
  ferr << "Line N " << CurRow << ": <" << s1 << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">";
  s1 = "The following key strings were expected:";
  Display(ferr, pattern, sz_pattern, s1.c_str());
  ferr << endl;
  ferr << "============== The block diagram of Performances ===============";
  Display(ferr,perform,sz_perform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return lSuccess;
 }
 //Поиск символа присваивания из массива-шаблона AssignSep в строке s1. Наличие
 //данного символа обязательно: <=>, <:=> и т.п.
 KEYSTRPOS assign_info;
 lSuccess = SearchKeyString(s1, AssignSep, sz_AssignSep, &assign_info);
 if (lSuccess == false) //Ошибка символы присваивания найти не удалось
 {ferr << "ERROR: " << title << " to receive it was not possible."
       << endl;
  ferr << "REASON: Assignment operator was expected for a key string <"
       << pattern[assign_info.index] << ">" << endl;
  ferr << "Line N " << CurRow << ": <" << s1 << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">";
  s1 = "The following ASSIGN OPERATORs were expected:";
  Display(ferr,AssignSep,sz_AssignSep,s1.c_str(),"< "," >;");
  ferr << endl;
  ferr << "======== The block diagram of Linear Array Performances ========";
  Display(ferr,LAAPerformPattern,sz_LAAPerform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return lSuccess;
 }
 //Поиск посторонних символов между НАЧАЛОМ s1 и оператором присваивания. Кроме
 //символа пробела и табуляции других символов быть не должно
 //Поиск посторонних символов слева от найденной ключевой строки
 string s2 = s1.substr(0,keystr_info.lpos);
 lpos = s2.find_first_not_of(" \t");
  if (lpos != string::npos) lSuccess = false;//Ошибка: запрещенные символы найдены
 //Поиск посторонних символов справа от найденной ключевой строки до оператора
 //присваивания
 if (lSuccess != false)
 { s2 = s1.substr(keystr_info.rpos+1,assign_info.lpos-keystr_info.rpos-1);
   lpos = s2.find_first_not_of(" \t");
   if (lpos != string::npos) lSuccess = false;//Ошибка: запрещенные символы найдены
 }
 if (lSuccess == false) //Формируем сообщение об ошибке
 {ferr << "ERROR:  " << title << " to receive it was not possible." << endl;
  ferr << "REASON: To the left of assignment operator a PROHIBITED TOKENS are";
  ferr << " detected." << endl;
  ferr << "Line N " << CurRow << ": <" << s1 << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << "The following allowed tokens were expected: <[SPACE],[TAB]>." << endl;
  ferr << endl;
  ferr << "============== The block diagram of Performances ===============";
  Display(ferr,perform,sz_perform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return lSuccess;
 }
 //Поиск вещественного числа, которое должно следовать после знака присваивания.
 s1 = s1.substr(assign_info.rpos+1);
 lpos = s1.find_first_of(RealNumber);

 if (lpos == string::npos) //Ошибка: после оператора присваивания следуют не цифры
 {lSuccess = false;
  ferr << "ERROR: " << title << " to receive it was not possible." << endl;
  ferr << "REASON: After assignment operator the non-digit token(s) are detected\n";
  ferr << "        for a key string <" << pattern[keystr_info.index] << ">"
       << endl;
  ferr << "Line N " << CurRow << ": <" << s1 << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << "The following digit tokens were expected: <" << RealNumber << ">.";
  ferr << endl << endl;
  ferr << "======== The block diagram of Linear Array Performances ========";
  Display(ferr,perform,sz_perform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return lSuccess;
 }
 //Поиск правой границы вещественного числа
 rpos = s1.find_first_not_of(RealNumber.c_str(),lpos);
 rpos--;
 //Поиск посторонних символов между оператором присваивания и ';'. Кроме
 //символа пробела и табуляции других символов быть не должно
 //Поиск посторонних символов слева от найденного цифрового значения
 s2 = s1.substr(0,lpos);
 int pos = s2.find_first_not_of(" \t");
 if (pos != string::npos) lSuccess = false;//Ошибка: запрещенные символы найдены
 //Поиск посторонних символов справа от найденного цифрового значения до ';'
 if (lSuccess != false)
 { s2 = s1.substr(rpos+1);
   pos = s2.find_first_not_of(" \t;");
   if (pos != string::npos) lSuccess = false;//Ошибка: запрещенные символы найдены
 }
 if (lSuccess == false) //Формируем сообщение об ошибке
 {ferr << "ERROR:  " << title << " to receive it was not possible." << endl;
  ferr << "REASON: To the right of assignment operator a PROHIBITED TOKENS are";
  ferr << " detected." << endl;
  ferr << "Line N " << CurRow << ": <" << s1 << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << "The following allowed tokens were expected: <[SPACE],[TAB]>." << endl;
  ferr << endl;
  ferr << "============== The block diagram of Performances ===============";
  Display(ferr,perform,sz_perform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return lSuccess;
 }
 //Преобразование числа из строкового представления в вещественное типа double
 s1 = s1.substr(lpos,rpos-lpos+1); //Извлечение числа в виде строки
 char* endptr = NULL;
 value = strtod(s1.c_str(),&endptr);
 if ((value == +HUGE_VAL) || (value == -HUGE_VAL)) //Ошибка
 {lSuccess = false;
  ferr << "ERROR:  " << title << " to receive it was not possible." << endl;
  ferr << "REASON: Transformation of character representation of number in a\n";
  ferr << "floating-point number is impossible. Check a format of number."<<endl;
  ferr << "Line N " << CurRow << ": <" << s1 << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << endl;
  ferr << "============== The block diagram of Performances ===============";
  Display(ferr,perform,sz_perform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return lSuccess;
 }
 lSuccess = true;
 return lSuccess;
}
//****************************************************************************
//СТАТУС: III.12., protected, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ:
// bool GetLawFromStr(const INFO_LAW* Table, const uint sz_Table, uint& index,
//      const string& title, const char** perform, const uint sz_perform);
//НАЗНАЧЕНИЕ:
//Формат строки: <KEY STRING;>
//<KEY STRING> = <str1> || <str2> || ... <strN>
//Функция предназначена для поиска ключевой строки - наименования закона, кото-
//рые хранятся в массиве Table в строке Line и в случае удачного поиска присвае-
//ния переменной index индекса записи, в которой содержится информация по найден-
//ному наименованию закона. Формат строки приведен выше.
//В случае ошибок, функция возвращает значение false и информация об ошибке ре-
//гистрируется в файловом потоке ошибок ferr. Ошибкой считается необнаружение
//ключевой строки в строке Line, а также возможные синтаксические ошибки, свя-
//занные с отступлением от формата строки приведенного выше. К таким ошибкам
//относятся пропуск символа ';', наличие в строке других символов, кроме пробе-
//лов и табуляций.
//Глобальные переменные используемые функцией:
// 1. string Buffer; 2. string Line; 3. CurRow
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. const INFO_LAW* Table - указатель на таблицу с информацией о возможных
//   наименованиях законов, которые может содержать строка Line
//2. const uint sz_Table - размер таблицы Table
//3. uint& index - ссылка на переменную, в которой будет храниться индекс
//   закона в таблице Table, в случае удачного поиска его в строке Line
//4. const string& title - ссылка на строку с наименованием искомой ключевой
//   строки в файловом потоке fin
//5. const char** perform - массив строк, содержащий шаблон структуры, которую
//   необходимо считать из файла
//6. const uint sz_perform - количество строк в массиве Perform
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: bool lSuccess
// true -  в строке Line наименование закона найдено
// false - в строке Line допустимого наименования закона найти не удалось
//*****************************************************************************
bool TLinearArray::GetLawFromStr(const INFO_LAW* Table, const uint sz_Table,
     uint& index,const string& title,const char** perform,const uint sz_perform)
{bool lSuccess;
 uint rpos;
 uint lpos;
 //Поиск символа-разделителя ';' между ключевыми строками, наличие которого
 //обязательно, в строке Line
 rpos = Line.find_first_of(';');
 if (rpos == string::npos) //Ошибка
 {lSuccess = false;
  ferr << "ERROR:  " << title << " to receive it was not possible." << endl;
  ferr << "REASON: Complete token <;> was expected for this data string" << endl;
  ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << endl;
  ferr << "============== The block diagram of Performances ===============";
  Display(ferr,perform,sz_perform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return lSuccess;
 }
 //Извлекаем из line подстроку s1 с завершающим символом ';'
 string s1 = Line.substr(1,rpos+1);
 //s1.set_case_sensitive(0); //Отключаем контекстно-чувствительный поиск
 //Поиск допустимого наименования закона из массива законов Table в строке s1
 KEYSTRPOS keystr_info;
 for (uint i = 0; i < sz_Table; i++)
 {lSuccess=SearchKeyString(s1,Table[i].pattern,Table[i].sz_pattern,&keystr_info);
  if (lSuccess) {index = i; break;}
 }
 if (lSuccess == false) //Ошибка: наименование закона не найдено
 {ferr << "ERROR:  " << title << " to receive it was not possible." << endl;
  ferr << "REASON: Law name to find it was not possible." << endl;
  ferr << "Line N " << CurRow << ": <" << s1 << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">";
  s1 = "The following LAW NAMEs were expected:";
  string s2;
  char str[20];
  for (uint i = 0; i < sz_Table; i++)
  {itoa(i+1,str,10); s2 = str; s2 =  s2 + ". " + s1;
   Display(ferr, Table[i].pattern, Table[i].sz_pattern, s2.c_str());
  }
  ferr << endl;
  ferr << "============== The block diagram of Performances ===============";
  Display(ferr,perform,sz_perform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return lSuccess;
 }
//Поиск посторонних символов слева и справа от наименования закона. Кроме
//символа пробела и табуляции других символов быть не должно.
//Поиск посторонних символов слева от наименования закона
 string s2 = s1.substr(0,keystr_info.lpos);
 lpos = s2.find_first_not_of(" \t");
  if (lpos != string::npos) //Ошибка: запрещенные символы найдены
  {s2 = "To the left of "; lSuccess = false;}
//Поиск посторонних символов справа от наименования закона до ';'
 if (lSuccess != false)
 { s2 = s1.substr(keystr_info.rpos+1);
   lpos = s2.find_first_not_of(" \t;");
   if (lpos != string::npos) //Ошибка: запрещенные символы найдены
   {s2 = "To the right of "; lSuccess = false;}
 }
 if (lSuccess == false) //Формируем сообщение об ошибке
 {ferr << "ERROR:  " << title << " to receive it was not possible." << endl;
  ferr << "REASON: " << s2 << "law name a PROHIBITED TOKENS are";
  ferr << " detected." << endl;
  ferr << "Line N " << CurRow << ": <" << s1 << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << "The following allowed tokens were expected: <[SPACE],[TAB]>." << endl;
  ferr << endl;
  ferr << "============== The block diagram of Performances ===============";
  Display(ferr,perform,sz_perform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return lSuccess;
 }
 //Наименование закона определено
 if (rpos != Line.length()-1) Line = Line.substr(rpos+1);
 else Line = "";
return lSuccess;
}
//****************************************************************************
//СТАТУС: III.13., protected, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ:
//ARGLIST* GetLawParamFromFile(const INFO_LAW* Table, const uint index,
//         const string& title, const char** perform, const uint sz_perform);
//НАЗНАЧЕНИЕ:
//Формат строки: <PARAMETERS: ARG1; ARG2; ... ARGn;>
//<ARGn> = [IDENT <=>] <value>; IDENT - идентификатор параметра (наименование)
//<IDENT> = LETTER+[LETTER||DIGIT||OTHER]+[LETTER||DIGIT||OTHER]+ ...]
//<OTHER> = <[]-_> for example: lv[-1_0]
//<value> - цифровая величина
//Функция предназначена для считывания параметров закона из файлового потока fin.
//Информация по количеству считываемых параметров и размеру списка ARGLIST arg,
//в котором они будут накапливаться, содержится в таблице законов Table. Индекс
//index указывает на адрес в Table по которому храниться закон, чьи параметры
//будут считываться из файла. Параметры могут располагаться на нескольких стро-
//ках файла. Формат строки приведен выше. Параметры, которые не поместились на
//первой строке следует размещать на следующих строках без указания ключевого
//слова - PARAMETERS. Если закон является табличным, то в первом параметре обяза-
//тельно указывается число значений, которые должны быть считаны далее.
//Параметры закона в общем случае являются вещественными числами. Перед каждым
//значением параметра может быть указан его идентификатор с последующим операто-
//ром присваивания типа <=>.
//В случае ошибок, функция возвращает значение LawArg == NULL и информация об
//ошибке регистрируется в файловом потоке ошибок ferr. Ошибкой считается необна-
//ружение ключевой строки в строке Line, а также возможные синтаксические ошибки,
//связанные с отступлением от формата строки приведенного выше. К таким ошибкам
//относятся пропуск символа ';', наличие в строке других символов, кроме пробе-
//лов и табуляций.
//Глобальные переменные используемые функцией:
// 1. string Buffer; 2. string Line; 3. CurRow
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. const INFO_LAW* Table - указатель на таблицу с информацией о возможных
//   наименованиях законов, которые может содержать строка Line
//2. const uint index - индекс закона в таблице Table для которого будут считы-
//   ваться параметры из файла
//3. const string& title - ссылка на строку с наименованием искомой ключевой
//   строки в файловом потоке fin
//4. const char** perform - массив строк, содержащий шаблон структуры, которую
//   необходимо считать из файла
//5. const uint sz_perform - количество строк в массиве Perform
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: ARGLIST* LawArg - список считанных из файла параметров
//закона из таблицы законов Table хранящийся под индексом index
//В случае неудачного считывания параметров LawArg == NULL
//*****************************************************************************
ARGLIST* TLinearArray::GetLawParamFromFile(const INFO_LAW* Table,
         const uint index, const string& title, const char** perform,
         const uint sz_perform)
{ARGLIST* LawArg = NULL;
 //Проверка соответствия между общим числом параметров, количеством считываемых
 //параметров и началом позицией, с которой будут располагаться в ARGLIST arg
 //считанные параметры
 if ((Table[index].total_arg != 0) && (Table[index].read_arg != 0))
 if ((Table[index].total_arg < Table[index].read_arg) ||
     (Table[index].total_arg < (Table[index].read_arg+Table[index].offset)))
 //Ошибка: несоответствие между числовыми значениями параметрами закона
 {ferr << "ERROR:  LAW PARAMETERS for " << title << " to receive it was not "
       << "possible." << endl;
  ferr << "REASON: " << "Detect mismatch between total parameters, amount of\n";
  ferr << "        reading parameters and initial position." << endl;
  ferr << "Law name: <" << Table[index].pattern[1] << ">;" << endl;
  ferr << "Total_Arg = " << Table[index].total_arg << endl;
  ferr << "Read_Arg  = " << Table[index].read_arg << endl;
  ferr << "Init_Pos  = " << Table[index].offset << endl;
  ferr << "TIPS: Total_Arg >= Read_Arg + Init_Pos." << endl;
  ferr << endl;
  ferr << "============== The block diagram of Performances ===============";
  Display(ferr,perform,sz_perform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return LawArg;
 }
 //*************************************************************************
 //1. Создание списка ARGLIST, в который будут заноситься параметры закона
 //*************************************************************************
 if (Table[index].ident != TABLE_LAW)
  LawArg = CreateArgList(Table[index].total_arg);
 //*************************************************************************
 //2. Проверка на необходимость считывания параметров из файла.
 //*************************************************************************
 if ((Table[index].ident != TABLE_LAW) && (Table[index].read_arg == 0))
  return LawArg;
 //******************************************************
 //3. Поиск ключевого слова <PARAMETERS>
 //******************************************************
 bool lSuccess;
 string name = "PARAMETERS";
 KEYSTRPOS info;
 lSuccess = SearchKeyString(Line, LawParamPattern, sz_LawParam, &info);
 if (lSuccess == false) //Ключевое слово не найдено
 {LawArg = DeleteArgList(LawArg);
  ferr << "ERROR:  " << name << " for " << title << " to receive it was not "
       << "possible." << endl;
  ferr << "REASON: KEY STRING to find it was not possible." << endl;
  ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">";
  name =  "The following KEY STRINGs were expected: ";
  Display(ferr, LawParamPattern, sz_LawParam, name.c_str());
  ferr << endl;
  ferr << "============== The block diagram of Performances ===============";
  Display(ferr,perform,sz_perform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return LawArg;
 }
 //***********************************************************************
 //4. Поиск символа разделителя между <PARAMETERS> и параметрами закона
 //***********************************************************************
 uint rpos = Line.find_first_of(":",info.rpos);
 if (rpos == string::npos) //Ошибка символы-разделители <:=> не найдены
 {LawArg = DeleteArgList(LawArg);
  ferr << "ERROR:  " << name << " for " << title << " to receive it was not "
       << "possible." << endl;
  ferr << "REASON: After <" << name << "> the token-separator should follow.\n";
  ferr << "The following token were expected: <:>." << endl;
  ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << endl;
  ferr << "============== The block diagram of Performances ===============";
  Display(ferr, perform, sz_perform, "", "", "", false);
  ferr << "================================================================";
  ferr << endl;
  return LawArg;
 }
//Поиск посторонних символов слева и справа от ключевого слова. Кроме символов
//пробела и табуляции других символов быть не должно.
//Поиск посторонних символов слева от ключевого слова.
 string s1 = Line.substr(0,info.lpos);
 uint lpos = s1.find_first_not_of(" \t");
  if (lpos != string::npos) //Ошибка: запрещенные символы найдены
  {s1 = "To the left of "; lSuccess = false;}
//Поиск посторонних символов справа от наименования закона до ':'
 if (lSuccess != false)
 { s1 = Line.substr(info.rpos+1,rpos-info.rpos-1);
   lpos = s1.find_first_not_of(" \t");
   if (lpos != string::npos) //Ошибка: запрещенные символы найдены
   {s1 = "To the right of "; lSuccess = false;}
 }
 if (lSuccess == false) //Формируем сообщение об ошибке
 {ferr << "ERROR:  " << name << " for " << title << " to receive it was not "
       << "possible." << endl;
  ferr << "REASON: " << s1 << " <" << name << "> a PROHIBITED TOKENS are";
  ferr << " detected." << endl;
  ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << "The following allowed tokens were expected: <[SPACE],[TAB]>.";
  name =  "The only following KEY STRINGs were expected: ";
  Display(ferr, LawParamPattern, sz_LawParam, name.c_str());
  ferr << endl;
  ferr << "============== The block diagram of Performances ===============";
  Display(ferr,perform,sz_perform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  LawArg = DeleteArgList(LawArg);
  return LawArg;
 }
 //Ключевое слово <PARAMETERS> определено
 if (rpos != Line.length()-1) Line = Line.substr(rpos+1);
 else Line = "";
//***************************************************************************
//5. Последовательное считывание параметров данного закона из файла и
//   занесение их в список LawArg
//***************************************************************************
//Определяем является ли закон табличным. Для табличного закона первый параметр
//указывает на кол-во аргументов, которые предстоит считать из файла
 ulong ReadArg = 0L;  //Количество считанных параметров
 ulong TotalArg = 0L;//Общее число аргументов которое предстоит считать
 double Value = 0.0;
 name = "PARAMETERs for "; name = name + title;
 //Табличный закон без указания количества считываемых из файла параметров
 if ((Table[index].ident == TABLE_LAW) && (Table[index].total_arg == 0))
 {long value = 0L;
  lSuccess = GetLongFromFile(value,TotalArg,ReadArg,name,perform,sz_perform);
  if (lSuccess == false) //Ошибка при считывании числа из файла
  {LawArg = DeleteArgList(LawArg); return LawArg;}
  if (value < 0) TotalArg = value*(-1L);
  else TotalArg = value;
  LawArg = CreateArgList(TotalArg+1);
 }
 //Табличный закон с указанием количества считываемых из файла параметров
 else if ((Table[index].ident == TABLE_LAW) && (Table[index].total_arg != 0))
 {LawArg = CreateArgList(Table[index].total_arg);
  TotalArg = Table[index].total_arg;
 }
 else {TotalArg = Table[index].read_arg;}
 //Цикл считывания параметров закона
 for (ulong i = 0; i < TotalArg; i++)
 {lSuccess = GetDoubleFromFile(Value,TotalArg,ReadArg,name,perform,sz_perform);
  //Ошибка при считывании данных
  if (lSuccess == false) {LawArg = DeleteArgList(LawArg); return LawArg;}
  //Очередной параметр считан из файлового потока успешно
  ReadArg++;
  LawArg->List[Table[index].offset+i] = Value;
 }
return LawArg;
}
//****************************************************************************
//СТАТУС: III.14., protected, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ:
// bool GetDoubleFromFile(double& value, const ulong total, const ulong read,
//      const string& title, const char** perform, const uint sz_perform);
//НАЗНАЧЕНИЕ:
//Формат строки: [<IDENTIFICATOR <ASSIGNMENT OPERATOR>] <value>;>
//<IDENTIFICATOR> = LETTER+[LETTER||DIGIT||OTHER]+[LETTER||DIGIT||OTHER]+ ...]
//<OTHER> = <[]-_> for example: lv[-1_0]
//<ASSIGNMENT OPERATOR> = <=> || <:=> ...
//<value> - значение типа double
//<value> = [SIGN]DIGIT1_k[.]DIGITk+1_n[e||E[SIGN]DIGIT1_3]
//<SIGN> = <+> || <->; <DIGIT> = <0123456789>
//Функция предназначена для отыскания в строке Line, извлеченной из файлового
//потока fin, символьного представления вещественного числа и преобразования его
//в вещественное число типа double. Допустимый формат строки приведен выше. Из
//формата видно, что вещественному числу может предшествовать идентификатор, ко-
//торый поясняет назначение считываемого параметра, далее за идентификатором дол-
//жен следовать оператор присваивания и затем - символьное представление числа.
//В случае ошибок, функция возвращает значение false и информация об ошибке ре-
//гистрируется в файловом потоке ошибок ferr. Ошибкой считается необнаружение
//числа в файловом потоке fin, а также возможные синтаксические ошибки, связан-
//ные с отступлением от формата строки приведенного выше. К таким ошибкам
//относятся пропуск символа ';', наличие в строке других символов, кроме пробе-
//лов и табуляций, оператора присваивания, а также цифровых символов и знаков,
//которые необходимы для представления вещественного числа в символьном виде.
//При успешном поиске и преобразовании числа из символьного вида в переменную
//value типа double заносится результат преобразования этой строки.
//Остальные переменные передаваемые функцией служат для формирования сообщений
//в случае возникновения ошибки считывания вещественного числа из файла.
//Глобальные переменные используемые функцией:
// 1. string Buffer; 2. string Line; 3. CurRow; 4. ifstream fin; ofstream ferr;
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. double& value - ссылка на переменную, которой будет присвоен
//   результат работы функции в случае ее успешного завершения.
//2. const ulong total - общее количество вещественных чисел, которые необходимо
//   считать из файлового потока fin
//3. const ulong read - число считанных из файлового потока вещественных чисел
//4. const string& title - ссылка на строку с наименованием поля структуры для
//   которого считывается вещественное числа из файлового потока fin
//5. const char** perform - массив строк, содержащий шаблон структуры, которую
//   необходимо считать из файла
//6. const uint sz_perform - количество строк в массиве perform
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: bool lSuccess
// true -  очередное вещественное число считано из файла успешно
// false - возникла ошибка при считывании вещественного числа из файла
//*****************************************************************************
bool TLinearArray::GetDoubleFromFile(double& value, const ulong total,
     const ulong read, const string& title, const char** perform,
     const uint sz_perform)
{bool lSuccess = true;
 uint rpos;
 uint lpos;
 string ABC(EngABC); //EngABC + Engabc + RusABC + Rusabc + RealNumber + ';';
 ABC.append(Engabc); ABC.append(RusABC); ABC.append(Rusabc);
 ABC.append(RealNumber); ABC.append(1,';');
 //Цикл поиска символа-разделителя ';' между числами, наличие которого
 //обязательно, в строке Line
 do
 {rpos = Line.find_first_of(';');
  if (rpos == string::npos) //Символ ';' в текущей строке Line файла не найден
  {//Анализ на присутствие других символов в строке, кроме пробелов и табуляций
   //других символов быть не должно
   lpos = Line.find_first_not_of(" \t");
   if (lpos != string::npos) //Ошибка!
   {lSuccess = false; value = +HUGE_VAL;
    ferr << "ERROR:  " << title << " to receive it was not possible." << endl;
    ferr << "REASON: Complete token <;> was expected for this data string or\n";
    ferr << "        at this line there are superfluous tokens." << endl;
    ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
    ferr << "BUFFER: <" << Buffer << ">" << endl;
    ferr << "Total parameters: " << total << endl;
    ferr << "Amount of obtained parameters: " << read << endl;
    ferr << endl;
    ferr << "============== The block diagram of Performances ===============";
    Display(ferr,perform,sz_perform,"","","",false);
    ferr << "================================================================";
    ferr << endl;
    return lSuccess;
   }
   //Считываем новую строку из файлового потока fin
   lSuccess = GetLineWithoutComment(fin, Line, ABC, CommentSep, sz_CommentSep);
   if (lSuccess == false) //Ошибка! Достигнут конец файла
   {value = +HUGE_VAL;
    ferr << "ERROR:  " << title << " to receive it was not possible." << endl;
    ferr << "REASON: End of the file is reached. It was't possible to receive\n";
    ferr << "        from the file the next real number." << endl;
    ferr << "Total parameters: " << total << endl;
    ferr << "Amount of obtained parameters: " << read << endl;
    ferr << endl;
    ferr << "============== The block diagram of Performances ===============";
    Display(ferr,perform,sz_perform,"","","",false);
    ferr << "================================================================";
    ferr << endl;
    return lSuccess;
   }
  }
 } while(rpos == string::npos); //Конец цикла поиска символа ';'
 //Извлекаем подстроку до символа ';' включительно из Line и формируем новую
 //строку Line, исключая символы, находящиеся левее rpos
 string s1 = Line.substr(0,rpos+1);
 string s2 = s1;
 if (rpos != Line.length()-1) Line = Line.substr(rpos+1);
 else Line = "";
 //Осуществляем поиск первого непробельного символа в строке s1. Такой символ
 //обязательно будет присутствовать в этой строке.
 rpos = s1.find_first_not_of(" \t");
 //Анализируем найденный символ на символ английского или русского алфавита
 ABC = EngABC + Engabc + RusABC + Rusabc;
 lpos = ABC.find(s1[rpos]);
 //Найденный символ является буквой. Ожидаем ИДЕНТИФИКАТОР ПАРАМЕТРА
 if (lpos != string::npos)
 {//Ищем последний символ идентификатора
  s1 = s1.substr(rpos);
  ABC = EngABC + Engabc + RusABC + Rusabc + Digits + Sign + "_[],";
  //Индекс последнего символа идентификатора в s1
  rpos = s1.find_first_not_of(ABC);
  //Осуществляем поиск ОПЕРАТОРА ПРИСВАИВАНИЯ, который должен следовать после
  //идентификатора параметра, в строке s1
  KEYSTRPOS info;
  lSuccess = SearchKeyString(s1, AssignSep, sz_AssignSep, &info);
  if (lSuccess == false)//Ошибка! Символ присваивания не найден
  {value = +HUGE_VAL;
   ferr << "ERROR:  " << title << " to receive it was not possible." << endl;
   ferr << "REASON: ASSIGNMENT OPERATOR after the parameter identifier <"
        << s1.substr(0,rpos) << ">" << endl;
   ferr << "        was expected." << endl;
   ferr << "Line N " << CurRow << ": <" << s1 << ">" << endl;
   ferr << "BUFFER: <" << Buffer << ">" << endl;
   ferr << "Total parameters: " << total << endl;
   ferr << "Amount of obtained parameters: " << read;
   s1 = "The following ASSIGNMENT OPERATORs were expected:";
   Display(ferr,AssignSep,sz_AssignSep,s1.c_str(),"< "," >;");
   ferr << endl;
   ferr << "============== The block diagram of Performances ===============";
   Display(ferr,perform,sz_perform,"","","",false);
   ferr << "================================================================";
   ferr << endl;
   return lSuccess;
  }
  //Поиск лишних символов между идентификатором и оператором присваивания
  //Кроме символов пробела и табуляции других символов не должно быть
  s2 = s1.substr(rpos,info.lpos-rpos);
  lpos = s2.find_first_not_of(" \t");
  if (lpos != string::npos) //Ошибка! Найдены лишние символы
  {lSuccess = false; value = +HUGE_VAL;
   ferr << "ERROR:  " << title << " to receive it was not possible." << endl;
   ferr << "REASON: Between identifier <" << s1.substr(0,rpos) << ">"
        << " and ASSIGNMENT OPERATOR\nsuperfluous tokens are detected." << endl;
   ferr << "Line N " << CurRow << ": <" << s1 << ">" << endl;
   ferr << "BUFFER: <" << Buffer << ">" << endl;
   ferr << "Total parameters: " << total << endl;
   ferr << "Amount of obtained parameters: " << read << endl;
   ferr << endl;
   ferr << "============== The block diagram of Performances ===============";
   Display(ferr,perform,sz_perform,"","","",false);
   ferr << "================================================================";
   ferr << endl;
   return lSuccess;
  }
  //Извлекаем подстроку, находящуюся правее символа присваивания из s1
  s1 = s1.substr(info.rpos+1);
 }//Идентификатор параметра и оператор присваивания считаны успешно
 //Анализируем строку s1 на присутствие в ней цифровых символов
 rpos = s1.find_first_not_of(" \t");
 ABC = Sign + Digits + ".";
 lpos = ABC.find(s1[rpos]);
 //Найденный символ является цифровым символом. Ожидаем ЗНАЧЕНИЯ ПАРАМЕТРА
 if (lpos != string::npos)
 {//Ищем последний цифровой символ значения параметра
  s1 = s1.substr(rpos);
  ABC = RealNumber;
  //Индекс последнего цифрового символа значения параметра в s1
  rpos = s1.find_first_not_of(ABC);
  //Преобразуем цифровые символы в вещественное число
  s2 = s1.substr(0,rpos);
  char* endptr;
  value = strtod(s2.c_str(), &endptr);
  //Ошибка! Слишком большое или маленькое число
  if ((value == +HUGE_VAL) || (value == -HUGE_VAL))
  {lSuccess = false;
   ferr << "ERROR:  " << title << " to receive it was not possible." << endl;
   ferr << "REASON: The real number <" << s2 << "> oversteps the bounds of\n"
        << "        an admissible range." << endl;
   ferr << "Line N " << CurRow << ": <" << s1 << ">" << endl;
   ferr << "BUFFER: <" << Buffer << ">" << endl;
   ferr << "Total parameters: " << total << endl;
   ferr << "Amount of obtained parameters: " << read << endl;
   ferr << endl;
   return lSuccess;
  }
  //Анализ на присутствие лишних символов после числа
  s2 = s1.substr(rpos);
  lpos = s2.find_first_not_of(" \t;");
  if (lpos != string::npos) //Ошибка! Найдены лишние символы
  {lSuccess = false;
   ferr << "ERROR:  " << title << " to receive it was not possible." << endl;
   ferr << "REASON: After number <" << value << "> superfluous numerals "
        << "are found." << endl;
   ferr << "Line N " << CurRow << ": <" << s1 << ">" << endl;
   ferr << "BUFFER: <" << Buffer << ">" << endl;
   ferr << "Total parameters: " << total << endl;
   ferr << "Amount of obtained parameters: " << read << endl;
   ferr << endl;
   ferr << "============== The block diagram of Performances ===============";
   Display(ferr,perform,sz_perform,"","","",false);
   ferr << "================================================================";
   ferr << endl;
   value = +HUGE_VAL;
   return lSuccess;
  }
 }
 else //Ошибка! Символ не является цифровым
 {lSuccess = false; value = +HUGE_VAL;
  ferr << "ERROR:  " << title << " to receive it was not possible." << endl;
  ferr << "REASON: Token <" << s1[rpos] << "> is found which is not a DIGITAL "
       << "NUMERAL." << endl;
  ferr << "Line N " << CurRow << ": <" << s1 << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << "Total parameters: " << total << endl;
  ferr << "Amount of obtained parameters: " << read << endl;
  ferr << endl;
  ferr << "============== The block diagram of Performances ===============";
  Display(ferr,perform,sz_perform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
 }
return lSuccess;
}
//****************************************************************************
//СТАТУС: III.15., protected, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ:
// bool GetLongFromFile(long& value, const ulong total, const ulong read,
//      const string& title, const char** perform, const uint sz_perform);
//НАЗНАЧЕНИЕ:
//Формат строки: [<IDENTIFICATOR <ASSIGNMENT OPERATOR>] <value>;>
//<IDENTIFICATOR> = LETTER+[LETTER||DIGIT||OTHER]+[LETTER||DIGIT||OTHER]+ ...]
//<OTHER> = <[]-_> for example: lv[-1_0]
//<ASSIGNMENT OPERATOR> = <=> || <:=> ...
//<value> - значение типа long
//<value> = [SIGN]DIGIT1_k
//<SIGN> = <+> || <->; <DIGIT> = <0123456789>
//Функция предназначена для отыскания в строке Line, извлеченной из файлового
//потока fin, символьного представления целого числа и преобразования его
//в длинное целое число типа long. Допустимый формат строки приведен выше. Из
//формата видно, что целому  числу может предшествовать идентификатор, который
//поясняет назначение считываемого параметра, далее за идентификатором должен
//следовать оператор присваивания и затем - символьное представление числа.
//В случае ошибок, функция возвращает значение false и информация об ошибке ре-
//гистрируется в файловом потоке ошибок ferr. Ошибкой считается необнаружение
//числа в файловом потоке fin, а также возможные синтаксические ошибки, связан-
//ные с отступлением от формата строки приведенного выше. К таким ошибкам
//относятся пропуск символа ';', наличие в строке других символов, кроме пробе-
//лов и табуляций, оператора присваивания, а также цифровых символов и знаков,
//которые необходимы для представления длинного целого числа в символьном виде.
//При успешном поиске и преобразовании числа из символьного вида в переменную
//value типа long заносится результат преобразования этой строки.
//Остальные переменные передаваемые функцией служат для формирования сообщений
//в случае возникновения ошибки считывания вещественного числа из файла.
//Глобальные переменные используемые функцией:
// 1. string Buffer; 2. string Line; 3. CurRow; 4. ifstream fin; ofstream ferr;
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. long& value - ссылка на переменную, которой будет присвоен
//   результат работы функции в случае ее успешного завершения.
//2. const ulong total - общее количество вещественных чисел, которые необходимо
//   считать из файлового потока fin
//3. const ulong read - число считанных из файлового потока вещественных чисел
//4. const string& title - ссылка на строку с наименованием поля структуры для
//   которого считывается вещественное числа из файлового потока fin
//5. const char** perform - массив строк, содержащий шаблон структуры, которую
//   необходимо считать из файла
//6. const uint sz_perform - количество строк в массиве perform
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: bool lSuccess
// true -  очередное вещественное число считано из файла успешно
// false - возникла ошибка при считывании вещественного числа из файла
//*****************************************************************************
bool TLinearArray::GetLongFromFile(long& value, const ulong total,
     const ulong read, const string& title, const char** perform,
     const uint sz_perform)
{bool lSuccess = true;
 uint rpos;
 uint lpos;
 string ABC(EngABC); //EngABC + Engabc + RusABC + Rusabc + Digits + Sign + ';';
 ABC.append(Engabc); ABC.append(RusABC); ABC.append(Rusabc);
 ABC.append(Digits); ABC.append(Sign); ABC.append(1,';');
 //Цикл поиска символа-разделителя ';' между числами, наличие которого
 //обязательно, в строке Line
 do
 {rpos = Line.find_first_of(';');
  if (rpos == string::npos) //Символ ';' в текущей строке Line файла не найден
  {//Анализ на присутствие других символов в строке, кроме пробелов и табуляций
   //других символов быть не должно
   lpos = Line.find_first_not_of(" \t");
   if (lpos != string::npos) //Ошибка!
   {lSuccess = false; value = 0L;
    ferr << "ERROR:  " << title << " to receive it was not possible." << endl;
    ferr << "REASON: Complete token <;> was expected for this data string or\n";
    ferr << "        at this line there are superfluous tokens." << endl;
    ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
    ferr << "BUFFER: <" << Buffer << ">" << endl;
    ferr << "Total parameters: " << total << endl;
    ferr << "Amount of obtained parameters: " << read << endl;
    ferr << endl;
    ferr << "============== The block diagram of Performances ===============";
    Display(ferr,perform,sz_perform,"","","",false);
    ferr << "================================================================";
    ferr << endl;
    return lSuccess;
   }
   //Считываем новую строку из файлового потока fin
   lSuccess = GetLineWithoutComment(fin, Line, ABC, CommentSep, sz_CommentSep);
   if (lSuccess == false) //Ошибка! Достигнут конец файла
   {value = 0L;
    ferr << "ERROR:  " << title << " to receive it was not possible." << endl;
    ferr << "REASON: End of the file is reached. It was't possible to receive\n";
    ferr << "        from the file the next real number." << endl;
    ferr << "Total parameters: " << total << endl;
    ferr << "Amount of obtained parameters: " << read << endl;
    ferr << endl;
    ferr << "============== The block diagram of Performances ===============";
    Display(ferr,perform,sz_perform,"","","",false);
    ferr << "================================================================";
    ferr << endl;
    return lSuccess;
   }
  }
 } while(rpos == string::npos); //Конец цикла поиска символа ';'
 //Извлекаем подстроку до символа ';' включительно из Line и формируем новую
 //строку Line, исключая символы, находящиеся левее rpos
 string s1 = Line.substr(0,rpos+1);
 string s2 = s1;
 if (rpos != Line.length()-1) Line = Line.substr(rpos+1);
 else Line = "";
 //Осуществляем поиск первого непробельного символа в строке s1. Такой символ
 //обязательно будет присутствовать в этой строке.
 rpos = s1.find_first_not_of(" \t");
 //Анализируем найденный символ на символ английского или русского алфавита
 ABC = EngABC + Engabc + RusABC + Rusabc;
 lpos = ABC.find(s1[rpos]);
 //Найденный символ является буквой. Ожидаем ИДЕНТИФИКАТОР ПАРАМЕТРА
 if (lpos != string::npos)
 {//Ищем последний символ идентификатора
  s1 = s1.substr(rpos);
  ABC = EngABC + Engabc + RusABC + Rusabc + Digits + Sign + "_[],";
  //Индекс последнего символа идентификатора в s1
  rpos = s1.find_first_not_of(ABC);
  //Осуществляем поиск ОПЕРАТОРА ПРИСВАИВАНИЯ, который должен следовать после
  //идентификатора параметра, в строке s1
  KEYSTRPOS info;
  lSuccess = SearchKeyString(s1, AssignSep, sz_AssignSep, &info);
  if (lSuccess == false)//Ошибка! Символ присваивания не найден
  {value = 0L;
   ferr << "ERROR:  " << title << " to receive it was not possible." << endl;
   ferr << "REASON: ASSIGNMENT OPERATOR after the parameter identifier <"
        << s1.substr(0,rpos) << ">" << endl;
   ferr << "        was expected." << endl;
   ferr << "Line N " << CurRow << ": <" << s1 << ">" << endl;
   ferr << "BUFFER: <" << Buffer << ">" << endl;
   ferr << "Total parameters: " << total << endl;
   ferr << "Amount of obtained parameters: " << read;
   s1 = "The following ASSIGNMENT OPERATORs were expected:";
   Display(ferr,AssignSep,sz_AssignSep,s1.c_str(),"< "," >;");
   ferr << endl;
   ferr << "============== The block diagram of Performances ===============";
   Display(ferr,perform,sz_perform,"","","",false);
   ferr << "================================================================";
   ferr << endl;
   return lSuccess;
  }
  //Поиск лишних символов между идентификатором и оператором присваивания
  //Кроме символов пробела и табуляции других символов не должно быть
  s2 = s1.substr(rpos,info.lpos-rpos);
  lpos = s2.find_first_not_of(" \t");
  if (lpos != string::npos) //Ошибка! Найдены лишние символы
  {lSuccess = false; value = 0L;
   ferr << "ERROR:  " << title << " to receive it was not possible." << endl;
   ferr << "REASON: Between identifier <" << s1.substr(0,rpos) << ">"
        << " and ASSIGNMENT OPERATOR\nsuperfluous tokens are detected." << endl;
   ferr << "Line N " << CurRow << ": <" << s1 << ">" << endl;
   ferr << "BUFFER: <" << Buffer << ">" << endl;
   ferr << "Total parameters: " << total << endl;
   ferr << "Amount of obtained parameters: " << read << endl;
   ferr << endl;
   ferr << "============== The block diagram of Performances ===============";
   Display(ferr,perform,sz_perform,"","","",false);
   ferr << "================================================================";
   ferr << endl;
   return lSuccess;
  }
  //Извлекаем подстроку, находящуюся правее символа присваивания из s1
  s1 = s1.substr(info.rpos+1);
 }//Идентификатор параметра и оператор присваивания считаны успешно
 //Анализируем строку s1 на присутствие в ней цифровых символов
 rpos = s1.find_first_not_of(" \t");
 ABC = Sign + Digits;
 lpos = ABC.find(s1[rpos]);
 //Найденный символ является цифровым символом. Ожидаем ЗНАЧЕНИЯ ПАРАМЕТРА
 if (lpos != string::npos)
 {//Ищем последний цифровой символ значения параметра
  s1 = s1.substr(rpos);
  //Индекс последнего цифрового символа значения параметра в s1
  rpos = s1.find_first_not_of(ABC);
  //Преобразуем цифровые символы в длинное целое число
  s2 = s1.substr(0,rpos);
  char* endptr;
  value = strtol(s2.c_str(), &endptr, 10);
  //Анализ на присутствие лишних символов после числа
  s2 = s1.substr(rpos);
  lpos = s2.find_first_not_of(" \t;");
  if (lpos != string::npos) //Ошибка! Найдены лишние символы
  {lSuccess = false;
   ferr << "ERROR:  " << title << " to receive it was not possible." << endl;
   ferr << "REASON: After number <" << value << "> superfluous numerals "
        << "are found." << endl;
   ferr << "Line N " << CurRow << ": <" << s1 << ">" << endl;
   ferr << "BUFFER: <" << Buffer << ">" << endl;
   ferr << "Total parameters: " << total << endl;
   ferr << "Amount of obtained parameters: " << read << endl;
   ferr << endl;
   ferr << "============== The block diagram of Performances ===============";
   Display(ferr,perform,sz_perform,"","","",false);
   ferr << "================================================================";
   ferr << endl;
   value = 0L;
   return lSuccess;
  }
 }
 else //Ошибка! Символ не является цифровым
 {lSuccess = false; value = 0L;
  ferr << "ERROR:  " << title << " to receive it was not possible." << endl;
  ferr << "REASON: Token <" << s1[rpos] << "> is found which is not a DIGITAL "
       << "NUMERAL." << endl;
  ferr << "Line N " << CurRow << ": <" << s1 << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << "Total parameters: " << total << endl;
  ferr << "Amount of obtained parameters: " << read << endl;
  ferr << endl;
  ferr << "============== The block diagram of Performances ===============";
  Display(ferr,perform,sz_perform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
 }
return lSuccess;
}
//****************************************************************************
//СТАТУС: III.16., protected, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ: bool GetDataFromFile()
//НАЗНАЧЕНИЕ:
//Функция предназначена для инициализации характеристик линейной антенной решет-
//ке по данным, размещенным в текстовом файле специального вида. Функция в ходе
//своей работы использует вызовы двух других функций:
//1. GetLAAPerformFromFile() 2. GetElemArrayPerformFromFile(TElArray& ElArray)
//Первая функция предназначена для инициализации характеристик свойственных
//исключительно линейной антенной решетке. Вторая функция инициализирует пара-
//метры отдельных элементов антенной решетки. Порядок использования второй функ-
//ции зависит от характеристик линейной АР: конфигурации и однородности. Если
//линейная АР является ОДНОРОДНОЙ, то вторая функция вызывается всего два раза:
//для инициализации характеристик ОСНОВНОГО ЭЛЕМЕНТА АР и для инициализации ха-
//рактеристик всех остальных однородных элементов АР. Если решетка НЕОДНОРОДНАЯ,
//то количество вызовов функции определяется конфигурацией АР, которая в частнос-
//ти определяет общее количество вспомогательных элементов решетки.
//Данные по характеристикам линейной АР считываются из входного файлового потока
//fin. В ходе считывания данных осуществляется их синтаксический и семантический
//анализ на корректность информации, которую содержит файл. При обнаружении оши-
//бок функция завершает свою работу, ошибка приведшая к преждевременному завер-
//шению работы функции документируется в выходном файловом потоке ошибок ferr c
//соответствующими комментариями.
//Глобальные переменные используемые функцией:
// 1. string Buffer; 2. string Line; 3. CurRow
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ: нет
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: bool lSuccess
// true -  свойства линейной АР из файла fname считаны успешно
// false - свойства линейной АР из файла fname считать не удалось
//*****************************************************************************
bool TLinearArray::GetDataFromFile()
{ bool lSuccess;

 //Начальная инициализация глобальных переменных
 CurRow = 0; //Buffer.skip_whitespace(0);
 //Line.skip_whitespace(0); Line.set_case_sensitive(0);
 #if defined(CONSOLE)
  cout << endl;
  cout << "Reading data on Linear Array Properties from <"
       << uppercase << fname << "> ...";
 #endif

 //Считывание характеристик присущих только линейной АР
 lSuccess = GetLAAPerformFromFile();
 #if defined(CONSOLE)
  gotoxy(1,wherey()); clreol(); gotoxy(1,wherey()); //Очистка экрана
 #endif
 //В ходе считывания данных обнаружена ошибка - завершение работы
 if (lSuccess == false) return lSuccess;
 //Выделение памяти для хранения информации по элементам АР
 //Создание массива объектов TElArray с вызовом конструктора по умолчанию
 Count = Left + Together + Right + 1;
 data = new TElArray[Count];

 //Считывание характеристик по элементам антенной решетки
 //1. Получение характеристик основного элемента АР
 TElArray ElArray;
 lSuccess = GetElemArrayPerformFromFile(ElArray, MASTER);
 if (lSuccess) { ElArray.SetNum(0L); data[0] = ElArray; }
 else return lSuccess;

 //2. Получение характеристик по вспомогательным элементам АР
 //   Для однородной линейной антенной решетки считывается только один вспомога-
 //   тельный антенный элемент
 bool flag = true;
 for (ulong i = 1; i <= (Count-1); i++)
 { if (flag) lSuccess = GetElemArrayPerformFromFile(ElArray, SLAVE);
   flag = (Uniformity == UNIFORM) ? false : true;
   if (lSuccess) //Считывание данных произошло успешно
   {//Установка прорядковых номеров расположения вспомогательных элементов АР
    if (i <= Left) ElArray.SetNum((Left-i+1)*(-1L));
    else if ((i > Left) && (i <= (Left+Together))) ElArray.SetNum(0L);
    else ElArray.SetNum(i-Left-Together);
    data[i] = ElArray;
   }
   else //Ошибка при считывании характеристик i-го элемента АР
   {

    return lSuccess;
   }
 }
 //Считывание из файла произошло успешно. Вычисляем относительные расстояния
 //между элементами АР
 CalcRelDistance();
return lSuccess;
}
//****************************************************************************
//СТАТУС: III.17., protected, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ: bool GetLAAPerformFromFile()
//НАЗНАЧЕНИЕ:
//Функция предназначена для инициализации характеристик свойственных только ли-
//нейной антенной решетке. К таким характеристикам относятся следующие:
//1. CONFIGURATION: LEFT <uint>; TOGETHER <uint>; RIGHT <uint> - конфигурация
//   линейной решетки, количество элементов расположенных левее, правее и вместе
//   с основным элементом антенной решетки. Каждый элемент антенной решетки
//   представляет из себя антенну. Максимальное кол-во элементов в каждой из 3
//   позиций 65535. Допускается отсутствие элементов в какой-либо из 3 позиций
//   как совместное, так и несовместное. Единственное ограничение состоит в том,
//   что суммарное количество элементов слева и справа должно быть больше 1, ибо
//   в противном случае антенная решетка выраждается в антенну.
//2. UNIFORMITY: <uniform || nonuniform> - однородность линейной АР. Под ОДНОРОД-
//   НОСТЬЮ АР понимается, что все вспомогательные элементы АР имеют одинаковые
//   антенные характеристики - относятся к одному типу антенн, одинаковым образом
//   расположенных в пространстве, с одинаковыми шумовыми характеристиками.
//   Основной элемент АР может иметь другой тип антенны. К классу НЕОДНОРОДНОЙ
//   АР относятся все остальные возможные варианты - различные типы антенн, раз-
//   личная расположение в пространстве, различные параметры антенн одного типа
//   и т.д. Однородные антенные решетки имеют наибольшее распространение.
//3. EQUIDISTANCE: <equidistant || unequidistant>, LV = <double> - эквидистант-
//   ность АР. Под ЭКВИДИСТАНТНОСТЬЮ АР понимается одинаковое (равное) межэле-
//   ментное расстояние между всеми соседними элементами АР. Расстояние для
//   удобства расчетов выражают через ОТНОСИТЕЛЬНОЕ РАССТОЯНИЕ - отношение гео-
//   метрического расстояния между соседними элементами к длине волны, которая
//   принимается антенной решеткой. Если решетка является эквидистантной, то да-
//   лее в этой же строке указывается LV = <double> - относительная база антенной
//   системы (относительное расстояние между соседними элементами АР), которое
//   будет ОДИНАКОВО для всех пар соседних элементов решетки.
//4. LEFTDISTLAW: <law name> PARAMETERS <double_1,...double_N>
//5. RIGHTDISTLAW: <law name> PARAMETERS <double_1,...double_N> - законы, кото-
//   рые определяют относительные расстояния между соседними элементами АР, рас-
//   положенными, соответственно, слева и справа от основного элемента, в случае
//   если решетка НЕ ЯВЛЯЕТСЯ ЭКВИДИСТАНТНОЙ. В противном случае, эти законы
//   игнорируются. За БАЗОВОЕ расстояние относительно, которого происходит опре-
//   деление расстояния между остальными парами соседних элементов АР принимается
//   относительное расстояние между первым по счету элементу АР, лежащим левее
//   (правее) ОСНОВНОГО элемента. Значение базового расстояния должно следовать
//   сразу после ключевого слова PARAMETERS, далее следуют параметры характеризую-
//   щий закон <law name>, их количество определяется конкретным законом. Пара-
//   метры закона могут располагаться на нескольких строках, конкретной величине
//   параметра закона может предшествовать соответствующее пояснение, которое
//   обязательно должно начинаться с БУКВЫ, после него должен следовать раздели-
//   тельный символ (separator - "= :") и далее вещественное или целое число.
//   БАЗОВОЕ расстояние может быть только положительным значением (>0). Если за-
//   дан произвольный (табличный закон) определения относительных расстояний
//   между соседними элементами, то после наименования закона <TABLE_LAW>, воз-
//   можно начиная с новой строки, в направлении СЛЕВА НАПРАВО должны быть пере-
//   числены все относительные расстояния между соседними элементами, которые
//   располагаются левее и правее основного элемента. Элементы расположенные
//   вместе с основным элементом в расчет не принимаются, так как их относитель-
//   ные расстояния друг с другом равны нулю. ВСЕ ОТНОСИТЕЛЬНЫЕ РАССТОЯНИЯ ДОЛЖ-
//   НЫ БЫТЬ ПОЛОЖИТЕЛЬНЫМИ (>0.0) ВЕЛИЧИНАМИ, в противном случае генерируется
//   ошибка. В случае произвольного закона задания относительных расстояний, каж-
//   дому значению, также может предшествовать пояснение, которое должно начи-
//   наться с БУКВЫ, между значением и пояснением должен быть разделитель выше-
//   указанного вида - "= :". Если закон не является ПРОИЗВОЛЬНЫМ, то вычисления
//   относительных расстояний по БАЗОВОМУ между остальными элементами АР осуществ-
//   ляется по следующей схеме. БАЗОВОЕ расстояние между первым элементом АР и
//   основным элементом будет служить аргументом для функции LEFTDISTLAW (RIGHT-
//   DISTLAW) для определения расстояния (далее LV) между вторым и первым элемен-
//   том (LV2-1). Далее LV2-1 будет служить аргументом для той же функции для
//   вычисления LV3-2 и так далее по формуле: LV[i+1,i] = FUNC(LV[i,i-1]).
//6. DISPOSITION: <equator || meridian> - относительное расположение линейной АР
//   относительно плоскостей Земли. EQUATOR - расположение по горизонтали, диа-
//   грамма направленности такой линейной АР будет меняться в зависимости от уг-
//   ла по азимуту на источник сигнала. MERIDIAN - расположение по вертикали,
//   диаграмма направленности АР будет меняться в зависимости от угла места ис-
//   точника сигнала. Под диаграммой направленности понимается ФАЗОВАЯ ДИАГРАММА
//   НАПРАВЛЕННОСТИ АР. Если АР расположена вертикально, то элементы расположен-
//   ные НИЖЕ основного элемента будут относиться к ЛЕВЫМ элементам АР, а распо-
//   ложенные ВЫШЕ основного элемента будут относиться к ПРАВЫМ элементам АР.
//В процессе считывания характеристик осуществлятся синтаксический и семантичес-
//кий контроль правильности данны. В случае ошибки функция завершает свою работу
//преждевременно с регистрацией возможной ошибке и поясняющими комментариями в
//файле ferrname. В качестве источника данных функция использует входной файло-
//вый поток fin, ассоциированный с именем файла fname.
//
//Глобальные переменные используемые функцией:
// 1. string Buffer; 2. string Line; 3. CurRow
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ: нет
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: bool lSuccess
// true -  свойства линейной АР из файла fname считаны успешно
// false - свойства линейной АР из файла fname считать не удалось
//*****************************************************************************
bool TLinearArray::GetLAAPerformFromFile()
{ bool lSuccess;
  string ABC(EngABC); //допустимый набор символов
  ABC.append(RusABC);
  string title;
  //******************************************************
  //1. Поиск строки-заголовка характеристик линейной АР
  //******************************************************
  //ABC = EngABC + RusABC;
  lSuccess = GetLineWithoutComment(fin, Line, ABC, CommentSep, sz_CommentSep);
  //Строка содержащая символы ABC без комментариев не найдена.
  //Достигнут конец файла, формируем сообщение об ошибке
  if (lSuccess == false)
  {ferr << "ERROR:  End of file is reached. It was not possible to receive\n";
   ferr << "        a TITLE of Linear Array performances.";
   title = "REASON: The following names of Linear Array TITLE were expected:";
   Display(ferr,LAAHeaderPattern,sz_LAAHeader,title.c_str(),"< "," >;");
   ferr << endl;
   ferr << "======== The block diagram of Linear Array Performances ========";
   Display(ferr,LAAPerformPattern,sz_LAAPerform,"","","",false);
   ferr << "================================================================";
   ferr << endl;
   return lSuccess; //Прекращение работы функции
  }
  lSuccess = SearchKeyString(Line, LAAHeaderPattern, sz_LAAHeader);
  //Строка-заголовок не найдена, считанная из файла строка Line содержит
  //некорректную информацию. Формируем сообщение об ошибке
  if (lSuccess == false)
  {ferr << "ERROR:  The TITLE of Linear Array not found." << endl;
   ferr << "REASON: The following line of file contains an incorrect data.\n";
   ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
   ferr << "BUFFER: <" << Buffer << ">";
   title = "The following names of Linear Array TITLE were expected:";
   Display(ferr,LAAHeaderPattern,sz_LAAHeader,title.c_str(),"< "," >;");
   ferr << endl;
   ferr << "======== The block diagram of Linear Array Performances ========";
   Display(ferr,LAAPerformPattern,sz_LAAPerform,"","","",false);
   ferr << "================================================================";
   ferr << endl;
   return lSuccess;
  }
  //******************************************************************
  //2. Определение КОНФИГУРАЦИИ линейной АР - CONFIGURATION PROPERTY
  //******************************************************************
  uint left = 0, together = 0, right = 0;
  lSuccess = GetConfigurationLAA(left,together,right);
  if (lSuccess == false) return lSuccess;
  Left = left; Together = together; Right = right;

  //******************************************************************
  //3. Определение ОДНОРОДНОСТИ линейной АР - UNIFORMITY PROPERTY
  //******************************************************************
  bool uniform = NONUNIFORM;
  lSuccess = GetUniformLAA(uniform);
  if (lSuccess == false) return lSuccess;
  Uniformity = uniform;

  //*********************************************************************
  //4. Определение ЭКВИДИСТАНТНОСТИ линейной АР - EQUIDISTANCE PROPERTY
  //*********************************************************************
  bool equal = UNEQUIDISTANT;
  double lv = 0.0;
  lSuccess = GetEquidistLAA(equal, lv);
  if (lSuccess == false) return lSuccess;
  Equidistance = equal;
  //Антенная решетка - эквидистантная - F(i+1,i) = lv, для всех относительных
  //расстояний между элементами АР.
  if (equal == EQUIDISTANT)
  { ARGLIST* LawArg = CreateArgList(2);
    LawArg->List[0] = lv; LawArg->List[1] = lv;
    if (left > 0) //В АР имеются элементы расположенные слева от основного
    { LeftLawType = CONST_LAW;
      LeftLaw = ConstLaw;
      LeftLawArg = CopyArgList(LawArg, LeftLawArg);
    }
    if (right > 0) //В АР имеются элементы расположенные справа от основного
    { RightLawType = CONST_LAW;
      RightLaw = ConstLaw;
      RightLawArg = CopyArgList(LawArg, RightLawArg);
    }
  }
  //****************************************************************************
  //5. Определение ЗАКОНА, ОПРЕДЕЛЯЮЩЕГО ОТНОСИТЕЛЬНЫЕ РАССТОЯНИЯ МЕЖДУ ЭЛЕМЕН-
  //   ТАМИ АР, РАСПОЛОЖЕННЫМИ СЛЕВА ОТ ОСНОВНОГО ЭЛЕМЕНТА - LEFTDISTLAW. Данный
  //   закон необходим, если АР является неэквидистантной и количество элементов
  //   АР, расположенных слева от основного отлично от нуля.
  //****************************************************************************
  if ((equal == UNEQUIDISTANT) && (left > 0))
  { lSuccess = GetLeftDistLawLAA();
    if (lSuccess == false) return lSuccess;
  }
  //****************************************************************************
  //6. Определение ЗАКОНА, ОПРЕДЕЛЯЮЩЕГО ОТНОСИТЕЛЬНЫЕ РАССТОЯНИЯ МЕЖДУ ЭЛЕМЕН-
  //   ТАМИ АР, РАСПОЛОЖЕННЫМИ СПРАВА ОТ ОСНОВНОГО ЭЛЕМЕНТА - LEFTDISTLAW. Данный
  //   закон необходим, если АР является неэквидистантной и количество элементов
  //   АР, расположенных справа от основного отлично от нуля.
  //****************************************************************************
  if ((equal == UNEQUIDISTANT) && (right > 0))
  { lSuccess = GetRightDistLawLAA();
    if (lSuccess == false) return lSuccess;
  }
  //***************************************************************************
  //7. Определение расположения линейной АР относительно земной поверхности
  //***************************************************************************
  bool disp = EQUATOR; //по горизонатали - азимутальный угол
  lSuccess = GetDisposLAA(disp);
  if (lSuccess) Disposition = disp;

return lSuccess;
}
//****************************************************************************
//СТАТУС: III.18., protected, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ:
//  bool GetConfigurationLAA(uint& left, uint& together, uint& right)
//НАЗНАЧЕНИЕ:
//Функция предназначена для получения сведений о конфигурации линейной АР, инфор-
//мация о которой хранится во входной файловом потоке fin. Функция должна отыс-
//кать ключевое слово <CONFIGURATION> и определить количество элементов АР, ко-
//торые располагаются правее, левее и вместе с основным элементом. Кол-во элемен-
//тов АР в отдельной позиции не должно превышать 65535 элементов. В случае воз-
//никновения ошибок синтаксического или семантического характера осуществляется
//их регистрация в файле ferrname, связанном с файловым потоком ошибок ferr, а
//функция прерывает свою работу и возвращает значение false.
//Глобальные переменные используемые функцией:
// 1. string Buffer; 2. string Line; 3. CurRow
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. uint& left - количество вспомогательных элементов АР, расположенных левее
//   основного элемента АР
//2. uint& together - количество вспомогательных элементов АР, расположенных
//   вместе с основным элементом АР
//3. uint& right - количество вспомогательных элементов АР, расположенных правее
//   основного элемента АР
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: bool lSuccess
// true -  конфигурация линейной АР из файла fname считана успешно
// false - конфигурацию линейной АР из файла fname считать не удалось
//*****************************************************************************
bool TLinearArray::GetConfigurationLAA(uint& left, uint& together, uint& right)
{ bool lSuccess;
  string ABC(EngABC); //допустимый набор символов
  ABC.append(RusABC);
  ABC.append(Digits);
  ABC.append(1,';');
  string title;
  //******************************************************
  //1. Поиск ключевого слова <CONFIGURATION>
  //******************************************************
  //ABC = EngABC + RusABC + Digits + ';';

  lSuccess = GetLineWithoutComment(fin, Line, ABC, CommentSep, sz_CommentSep);
  //Строка содержащая символы ABC без комментариев не найдена.
  //Достигнут конец файла, формируем сообщение об ошибке
  if (lSuccess == false)
  {ferr << "ERROR:  End of file is reached. It was not possible to receive\n";
   ferr << "        a CONFIGURATION of Linear Antenna Array.";
   title = "REASON: The following keywords were expected:";
   Display(ferr,LAAConfigPattern,sz_LAAConfig,title.c_str(),"< "," >;");
   ferr << endl;
   ferr << "======== The block diagram of Linear Array Performances ========";
   Display(ferr,LAAPerformPattern,sz_LAAPerform,"","","",false);
   ferr << "================================================================";
   ferr << endl;
   return lSuccess; //Прекращение работы функции
  }
  lSuccess = SearchKeyString(Line, LAAConfigPattern, sz_LAAConfig);
  //Ключевое слово не найдено, считанная из файла строка Line содержит
  //некорректную информацию. Формируем сообщение об ошибке
  if (lSuccess == false)
  {ferr << "ERROR:  The keyword <CONFIGURATION> is not found." << endl;
   ferr << "REASON: The following line of file contains an incorrect data.\n";
   ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
   ferr << "BUFFER: <" << Buffer << ">";
   title = "The following keywords were expected:";
   Display(ferr,LAAConfigPattern,sz_LAAConfig,title.c_str(),"< "," >;");
   ferr << endl;
   ferr << "======== The block diagram of Linear Array Performances ========";
   Display(ferr,LAAPerformPattern,sz_LAAPerform,"","","",false);
   ferr << "================================================================";
   ferr << endl;
  }
  //Ключевое слово <CONFIGURATION> найдено. Осуществляем поиск в этой же строке
  //ключевых слов <LEFT> или <RIGHT>. Одно из них должно обязательно присутство-
  //вать в строке Line
  bool lLeft;
  bool lRight;
  string OldLine;
  KEYSTRPOS info;
  ulong count = 0;
  lSuccess = SearchKeyString(Line, LAALeftPattern, sz_LAALeft, &info);
  lLeft = lSuccess;
  if (lSuccess == false) left = 0; //Ключевое слово <LEFT> не найдено
  else //Ключевое слово <LEFT> найдено, определяем кол-во элементов СЛЕВА
  {OldLine = Line;
   Line = Line.substr(info.lpos);
   info.rpos -= info.lpos; info.lpos = 0;
   lSuccess = GetCountElArray(count, &info, LAALeftPattern);
   if (lSuccess == false) return lSuccess; //Ошибка
   //Число элементов антенной решетки в одной из трех позиций не должно > 65535
   if (count > 65535) //Ошибка
   {lSuccess = false;
    ferr << "ERROR:  Linear Array CONFIGURATION to receive it was not possible."
         << endl;
    ferr << "REASON: Total of the elements located to the left of a basic\n"
            "        element of Antenna Array should not exceed 65535." << endl;
    ferr << "Total of the LEFT LOCATED ELEMENTS is <" << count << ">" << endl;
    ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
    ferr << "BUFFER: <" << Buffer << ">" << endl;
    ferr << endl;
    ferr << "======== The block diagram of Linear Array Performances ========";
    Display(ferr,LAAPerformPattern,sz_LAAPerform,"","","",false);
    ferr << "================================================================";
    ferr << endl;
    return lSuccess;
   }
    left = count;
    Line = OldLine; //Восстанавливаем первоначальное содержимое строки Line
  }
  //Поиск ключевого слова <RIGHT>
  lSuccess = SearchKeyString(Line, LAARightPattern, sz_LAARight, &info);
  lRight = lSuccess;
  if (lSuccess == false) right = 0; //Ключевое слово <RIGHT> не найдено
  else //Ключевое слово <RIGHT> найдено, определяем кол-во элементов СПРАВА
  {OldLine = Line;
   Line = Line.substr(info.lpos);
   info.rpos -= info.lpos; info.lpos = 0;
   lSuccess = GetCountElArray(count, &info, LAARightPattern);
   if (lSuccess == false) return lSuccess; //Ошибка
   //Число элементов антенной решетки в одной из трех позиций не должно > 65535
   if (count > 65535) //Ошибка
   {lSuccess = false;
    ferr << "ERROR:  Linear Array CONFIGURATION to receive it was not possible."
         << endl;
    ferr << "REASON: Total of the elements located to the right of a basic\n"
            "        element of Antenna Array should not exceed 65535." << endl;
    ferr << "Total of the RIGHT LOCATED ELEMENTS is <" << count << ">" << endl;
    ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
    ferr << "BUFFER: <" << Buffer << ">" << endl;
    ferr << endl;
    ferr << "======== The block diagram of Linear Array Performances ========";
    Display(ferr,LAAPerformPattern,sz_LAAPerform,"","","",false);
    ferr << "================================================================";
    ferr << endl;
    return lSuccess;
   }
   right = count;
   Line = OldLine; //Восстанавливаем первоначальное содержимое строки Line
  }
  //Анализируем кол-во общее число элементов АР, расположенных слева и справа
  //Ошибка: элементы АР, расположенные слева и справа от основного отсутствуют
  if ((left+right) == 0)
  {lSuccess = false;
   ferr << "ERROR:  Linear Array CONFIGURATION to receive it was not possible."
        << endl;
   ferr << "REASON: Total of the Linear Array Elements located at the left and\n"
        << "        to the right of a basic element, is equal to zero." << endl;
   ferr << "TIPS:   The following line of file contains an incorrect data.\n";
   ferr << "        The Linear Array Elements located at the left or to the\n"
        << "        right of a basic element, it is necessary should be."<<endl;
   ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
   ferr << "BUFFER: <" << Buffer << ">" << endl;
   if ((lLeft == false) || (lRight == false))
   {ferr << "The following keywords were expected:";
    if (!lLeft) Display(ferr,LAALeftPattern,sz_LAALeft,NULL,"< "," >;");
    if (!Right) Display(ferr,LAARightPattern,sz_LAARight,NULL,"< "," >;");
   }
   ferr << endl;
   ferr << "======== The block diagram of Linear Array Performances ========";
   Display(ferr,LAAPerformPattern,sz_LAAPerform,"","","",false);
   ferr << "================================================================";
   ferr << endl;
   return lSuccess;
  }
  //Поиск ключевого слова <TOGETHER>
  lSuccess = SearchKeyString(Line, LAATogetherPattern, sz_LAATogether, &info);
  if (lSuccess == false) together = 0; //Ключевое слово <TOGETHER> не найдено
  else //Ключевое слово <TOGETHER> найдено, определяем кол-во элементов АР, рас-
       //положенных вместе с основным элементом АР.
  {OldLine = Line;
   Line = Line.substr(info.lpos);
   info.rpos -= info.lpos; info.lpos = 0;
   lSuccess = GetCountElArray(count, &info, LAATogetherPattern);
   if (lSuccess == false) return lSuccess; //Ошибка
   //Число элементов антенной решетки в одной из трех позиций не должно > 65535
   if (count > 65535) //Ошибка
   {lSuccess = false;
    ferr << "ERROR:  Linear Array CONFIGURATION to receive it was not possible."
         << endl;
    ferr << "REASON: Total of the elements located together with a basic\n"
            "        element of Antenna Array should not exceed 65535." << endl;
    ferr << "Total of the TOGETHER LOCATED ELEMENTS is <" << count << ">" << endl;
    ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
    ferr << "BUFFER: <" << Buffer << ">" << endl;
    ferr << endl;
    ferr << "======== The block diagram of Linear Array Performances ========";
    Display(ferr,LAAPerformPattern,sz_LAAPerform,"","","",false);
    ferr << "================================================================";
    ferr << endl;
    return lSuccess;
   }
   together = count;
   Line = OldLine; //Восстанавливаем первоначальное содержимое строки Line
  }
return lSuccess;
}
//****************************************************************************
//СТАТУС: III.19., protected, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ:
// bool GetCountElArray(ulong& count,const pKEYSTRPOS info,const char** pattern)
//НАЗНАЧЕНИЕ:
//Формат строки: <KEY STRING> = <value> <;>
// value - целое положительное число типа ulong
// <KEY STRING> = <LEFT> || <RIGHT> || <TOGETHER>
//Функция предназначена для определения количества элементов линейной АР, инфор-
//мация о которой должна хранится в строке Line. Формат строки приведен выше. В
//случае ошибок, функция возвращает значение false и информация об ошибке регист-
//рируется в файловом потоке ошибок ferr.
//Глобальные переменные используемые функцией:
// 1. string Buffer; 2. string Line; 3. CurRow
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. uint& count - количество вспомогательных элементов АР, расположенных левее
//   (правее или вместе) основного элемента АР
//2. const pKEYSTRPOS info - информация по местоположению ключевой строки
//3. const char** pattern - массив строк, ассоциированный с ключевой строкой
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: bool lSuccess
// true -  конфигурация линейной АР из файла fname считана успешно
// false - конфигурацию линейной АР из файла fname считать не удалось
//*****************************************************************************
bool TLinearArray::GetCountElArray(ulong& count, const pKEYSTRPOS info,
                                   const char** pattern)
{bool lSuccess;
 uint rpos;
 count = 0;
 //Поиск символа-разделителя ';' между ключевыми строками, наличие которого
 //обязательно, в строке Line
 rpos = Line.find_first_of(';');
 if (rpos == string::npos) //Ошибка
 {lSuccess = false;
  ferr << "ERROR:  Linear Array CONFIGURATION to receive it was not possible."
        << endl;
  ferr << "REASON: Complete token <;> was expected for a key string <"
       << pattern[info->index] << ">" << endl;
  ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << endl;
  ferr << "======== The block diagram of Linear Array Performances ========";
  Display(ferr,LAAPerformPattern,sz_LAAPerform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return lSuccess;
 }
 //Извлекаем из Line подстроку s1 с завершающим символом ';'
 uint lpos = info->rpos+1;
 string title;
 string s1 = Line.substr(lpos,rpos-lpos+1);
 string s = Line.substr(info->lpos,rpos+1);
 count = 0;
 //Поиск символа присваивания из массива-шаблона AssignSep в строке s1. Наличие
 //данного символа обязательно: <=>, <:=> и т.п.
 KEYSTRPOS InfoSep;
 lSuccess = SearchKeyString(s1, AssignSep, sz_AssignSep, &InfoSep);
 if (lSuccess == false) //Ошибка символы присваивания найти не удалось
 {ferr << "ERROR:  Linear Array CONFIGURATION to receive it was not possible."
       << endl;
  ferr << "REASON: Assignment operator was expected for a key string <"
       << pattern[info->index] << ">" << endl;
  ferr << "Line N " << CurRow << ": <" << s << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">";
  title = "The following ASSIGN OPERATORs were expected:";
  Display(ferr,AssignSep,sz_AssignSep,title.c_str(),"< "," >;");
  ferr << endl;
  ferr << "======== The block diagram of Linear Array Performances ========";
  Display(ferr,LAAPerformPattern,sz_LAAPerform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return lSuccess;
 }
 //Поиск запрещенных к использованию символов между концом <КЛЮЧЕВОГО ВЫРАЖЕНИЯ>
 //и символом присваивания '=' или ':='
 //Формирование разрешенного алфавита символов
 string ABC = " \t";
 string s2 = s1.substr(0,InfoSep.lpos);
 lpos = s2.find_first_not_of(ABC);
 if (lpos != string::npos) //Ошибка! Запрещенные символы найдены
 {lSuccess = false;
  ferr << "ERROR:  Linear Array CONFIGURATION to receive it was not possible."
       << endl;
  ferr << "REASON: Between a  key string <" << pattern[info->index] << "> and\n";
  ferr << "        assignment operator the PROHIBITED TOKENS are detected.\n";
  ferr << "Line N " << CurRow << ": <" << s << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << "The following allowed tokens were expected: <[SPACE],[TAB]>." << endl;
  ferr << endl;
  ferr << "======== The block diagram of Linear Array Performances ========";
  Display(ferr,LAAPerformPattern,sz_LAAPerform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return lSuccess;
 }
 //Поиск запрещенных к использованию символов между концом <ASSIGNMENT OPERATOR>
 //и завершающим символом ';'
 //Формирование разрешенного алфавита символов
 ABC = Digits + " \t;";
 s2 = s1.substr(InfoSep.rpos+1);
 lpos = s2.find_first_not_of(ABC);
 if (lpos != string::npos) //Ошибка! Запрещенные символы найдены
 {lSuccess = false;
  ferr << "ERROR:  Linear Array CONFIGURATION to receive it was not possible."
       << endl;
  ferr << "REASON: Between a assignment operator and completing token ';'\n";
  ferr << "        the PROHIBITED TOKENS are detected.\n";
  ferr << "Line N " << CurRow << ": <" << s << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << "The following allowed tokens were expected: <[SPACE],[TAB],"
       << Digits << ">." << endl;
  ferr << endl;
  ferr << "======== The block diagram of Linear Array Performances ========";
  Display(ferr,LAAPerformPattern,sz_LAAPerform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return lSuccess;
 }
 //Символ присваивания найден. Поиск целого положительного числа, которое должно
 //следовать после знака присваивания. Между числом и знаком присваивания допус-
 //каются только пробелы.
 lpos = InfoSep.rpos+1;
 s1 = s1.substr(lpos);
 lpos = s1.find_first_not_of(" \t");
 //Анализируем первый найденный после оператора присваивания символ. Он должен
 //быть только цифрой
 rpos = Digits.find(s1[lpos]);
 if (rpos == string::npos) //Ошибка: после оператора присваивания следуют не цифры
 {lSuccess = false;
  ferr << "ERROR:  Linear Array CONFIGURATION to receive it was not possible."
       << endl;
  ferr << "REASON: After assignment operator the non-digit token(s) are detected\n";
  ferr << "        for a key string <" << pattern[info->index] << ">" << endl;
  ferr << "Line N " << CurRow << ": <" << s << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << "The following digits were expected: <" << Digits << ">." << endl;
  ferr << endl;
  ferr << "======== The block diagram of Linear Array Performances ========";
  Display(ferr,LAAPerformPattern,sz_LAAPerform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return lSuccess;
 }
 //Первый символ (за исключением пробелов) является цифрой. Выделяем строку
 //содержащую только цифровые символы и преобразуем ее в беззнаковое целое число.
 s1 = s1.substr(lpos);
 rpos = s1.find_first_not_of(Digits);
 s1 = s1.substr(0,rpos);
 char *endptr;
 count = strtoul(s1.c_str(),&endptr,10);

return lSuccess = true;
}
//****************************************************************************
//СТАТУС: III.20., protected, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ: bool GetUniformLAA(bool& uniform);
//НАЗНАЧЕНИЕ:
//Формат строки: <UNIFORMITY>: <UNIFORM || NONUNIFORM> <;>
//Функция предназначена для получения сведений об однородности линейной АР,
//информация о которой хранится во входной файловом потоке fin. Функция должна
//отыскать ключевое слово <UNIFORMITY> и определить данное свойство по ключевым
//словам из таблицы соответствия ключевых слов и булевых значений .
//В случае возникновения ошибок синтаксического или семантического характера
//осуществляется их регистрация в файле ferrname, связанном с файловым потоком
//ошибок ferr, а функция прерывает свою работу и возвращает значение false.
//Глобальные переменные используемые функцией:
// 1. string Buffer; 2. string Line; 3. CurRow
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. bool& uniform - ссылка на индикатор однородности элементов антенной решетки
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: bool lSuccess
// true -  свойство ОДНОРОДНОСТИ из файла fname удалось определить
// false - свойство ОДНОРОДНОСТИ из файла fname не удалось определить
//*****************************************************************************
bool TLinearArray::GetUniformLAA(bool& uniform)
{bool lSuccess;
 string ABC(EngABC); //допустимый набор символов
 ABC.append(RusABC);
 ABC.append(Digits);
 ABC.append(":;");
 string Title = "UNIFORMITY";
 KEYSTRPOS info;
 //******************************************************
 //1. Поиск ключевого слова <UNIFORMITY>
 //******************************************************
 lSuccess = FindKeyStrInFile(Title, LAAUniformPattern, sz_LAAUniform, Line,
            &info, ABC, CommentSep, sz_CommentSep, fin, ferr, LAAPerformPattern,
            sz_LAAPerform);
 if (lSuccess == false) return lSuccess;//Ключевое слово не найдено
 //*****************************************************************
 //2. Поиск символа разделителя между <UNIFORMITY> и ее значением
 //*****************************************************************
 uint rpos = Line.find_first_of(":=",info.rpos);
 if (rpos == string::npos) //Ошибка символы-разделители <:=> не найдены
 {lSuccess = false;
  ferr << "ERROR:  Linear Array UNIFORMITY to receive it was not possible."
       << endl;
  ferr << "REASON: After <UNIFORMITY> the token-separator should follow.\n";
  ferr << "The following tokens were expected: <:> or <=>." << endl;
  ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << endl;
  ferr << "======== The block diagram of Linear Array Performances ========";
  Display(ferr,LAAPerformPattern,sz_LAAPerform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return lSuccess;
 }
 info.rpos = rpos;
 //*******************************************************
 //3. Определение значения свойства <UNIFORMITY>
 //*******************************************************
 Title = "Linear Array "+Title;
 lSuccess = StrToBool(uniform, Line, &info, TbUniform, sz_TbUniform, Title,
            LAAPerformPattern, sz_LAAPerform);
return lSuccess;
}
//****************************************************************************
//СТАТУС: III.21., protected, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ: bool GetEquidistLAA(bool& equal, double& lv);
//НАЗНАЧЕНИЕ:
//Формат строки: <EQUIDISTANCE>: <EQUIDISTANT || UNEQUIDISTANT;>
//[<LV> <=> <doublevalue>;]
//Функция предназначена для получения сведений об эквидистантности линейной АР,
//информация о которой хранится во входной файловом потоке fin. Функция должна
//отыскать ключевое слово <EQUIDISTANCE> и определить данное свойство по ключевым
//словам из таблицы соответствия ключевых слов и булевых значений .
//Если решетка ЭКВИДИСТАНТНАЯ, то после ключевого слова (ex. <EQUIDISTANT>)
//обязательно должно следовать значение относительного расстояния между соседни-
//ми элементами антенной решетки. Формат строки по которому будет происходить
//считывание характеристики эквидистантности антенной решетки приведен выше.
//В случае возникновения ошибок синтаксического или семантического характера
//осуществляется их регистрация в файле ferrname, связанном с файловым потоком
//ошибок ferr, а функция прерывает свою работу и возвращает значение false.
//Глобальные переменные используемые функцией:
// 1. string Buffer; 2. string Line; 3. CurRow
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. bool& equal - ссылка на признак эквидистантности элементов антенной решетки
//2. double& lv - ссылка на переменную с относительным расстоянием между сосед-
//   ними элементами АР. Если АР неэквидистантна - lv = 0; в случае эквидистантной
//   АР lv должна быть положительной величиной (lv > 0).
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: bool lSuccess
// true -  свойство ЭКВИДИСТАНТНОСТИ из файла fname удалось определить
// false - свойство ЭКВИДИСТАНТНОСТИ из файла fname не удалось определить
//*****************************************************************************
bool TLinearArray::GetEquidistLAA(bool& equal, double& lv)
{bool lSuccess;
 string ABC(EngABC); //EngABC + RusABC + Digits + ":;"; //допустимый набор символов
 ABC.append(RusABC); ABC.append(Digits); ABC.append(":;");
 string Title = "EQUIDISTANCE";
 KEYSTRPOS info;
 //******************************************************
 //1. Поиск ключевого слова <EQUIDISTANCE>
 //******************************************************
 lSuccess = FindKeyStrInFile(Title, LAAEquidistPattern, sz_LAAEquidist, Line,
            &info, ABC, CommentSep, sz_CommentSep, fin, ferr, LAAPerformPattern,
            sz_LAAPerform);
 if (lSuccess == false) return lSuccess;//Ключевое слово не найдено
 //*****************************************************************
 //2. Поиск символа разделителя между <EQUIDISTANCE> и ее значением
 //*****************************************************************
 int rpos = Line.find_first_of(":=",info.rpos);
 if (rpos == string::npos) //Ошибка символы-разделители <:=> не найдены
 {lSuccess = false;
  ferr << "ERROR:  Linear Array EQUIDISTANCE to receive it was not possible."
       << endl;
  ferr << "REASON: After <EQUIDISTANCE> the token-separator should follow.\n";
  ferr << "The following tokens were expected: <:> or <=>." << endl;
  ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << endl;
  ferr << "======== The block diagram of Linear Array Performances ========";
  Display(ferr,LAAPerformPattern,sz_LAAPerform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return lSuccess;
 }
 info.rpos = rpos;
 //*******************************************************
 //3. Определение значения свойства <EQUIDISTANCE>
 //*******************************************************
 Title = "Linear Array "+Title;
 lSuccess = StrToBool(equal, Line, &info, TbEquidist, sz_TbEquidist, Title,
            LAAPerformPattern, sz_LAAPerform);
 if (lSuccess == false) return lSuccess;
 //*************************************************************
 //4. Определение относительного расстояния между элементами
 //   ЭКВИДИСТАНТНОЙ АР
 //*************************************************************
 if (equal == EQUIDISTANT)
 {//Определяем положение символа следующего за первым символом-разделителем ';'
  GetNextStrPosAfterSep(Line, rpos);
  if (rpos == string::npos) //Ошибка! В строке Line отсутствует информация об LV
  {lSuccess = false;
   ferr << "ERROR:  Linear Array EQUIDISTANCE to receive it was not possible."
        << endl;
   ferr << "REASON: There is no information about a relative distance between\n";
   ferr << "        the elements by an Equidistant Antenna Array." << endl;
   ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
   ferr << "BUFFER: <" << Buffer << ">" << endl;
   ferr << endl;
   ferr << "======== The block diagram of Linear Array Performances ========";
   Display(ferr,LAAPerformPattern,sz_LAAPerform,"","","",false);
   ferr << "================================================================";
   ferr << endl;
   return lSuccess;
  }
  //Определяем относительное расстояние между элементами эквидистантной АР
  Line = Line.substr(rpos);
  lv = 0.0;
  lSuccess = StrToDouble(lv, Line, LAARelDistPattern, sz_LAARelDist, Title,
                         LAAPerformPattern, sz_LAAPerform);
  if (lSuccess == false) return lSuccess;
  //Анализ полученного значения lv (lv > 0.0)
  if (lv <= 0.0) //Ошибка
  {lSuccess = false;
   ferr << "ERROR:  Linear Array EQUIDISTANCE to receive it was not possible."
        << endl;
   ferr << "REASON: The relative distance (LV) between the elements by" << endl;
   ferr << "        an Equidistant Antenna Array is NONPOSITIVE NUMBER !!!"<<endl;
   ferr << "LV = " << lv << ";" << endl;
   ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
   ferr << "BUFFER: <" << Buffer << ">" << endl;
   ferr << endl;
   ferr << "======== The block diagram of Linear Array Performances ========";
   Display(ferr,LAAPerformPattern,sz_LAAPerform,"","","",false);
   ferr << "================================================================";
   ferr << endl;
   return lSuccess;
  }
 }
return lSuccess;
}
//****************************************************************************
//СТАТУС: III.22., protected, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ: bool GetLeftDistLawLAA();
//НАЗНАЧЕНИЕ:
//Формат строки: <LEFTDISTLAW>: <Law name;> [<PARAMETERS: ARG1;ARG2;...ARGn;>]
//<Law name> - наименование закона;
//<PARAMETERS:> - список параметров закона;
//<ARGn> = [LETTER+[LETTER || DIGIT || OTHER] <=> ] double value <;>
//<LETTER> = EngABC + RusABC; DIGIT = Digits; OTHER = <[],->
//double value - вещественное число типа double
//Функция предназначена для получения сведений о законе по которому размещаются
//соседние элементы АР, расположенные слева от основного элемента линейной АР,
//информация о которой хранится во входной файловом потоке fin, в случае неэкви-
//дистантной решетки. Функция должна отыскать ключевое слово <LEFTDISTLAW>,
//определить тип закона по ключевым словам из таблицы DistLawTable и, используя
//информацию о законе из этой таблицы, считать необходимое количество параметров,
//которые данный закон определяют.
//Формат строки по которому будет происходить считывание закона приведен выше.
//В случае возникновения ошибок синтаксического или семантического характера
//осуществляется их регистрация в файле ferrname, связанном с файловым потоком
//ошибок ferr, а функция прерывает свою работу и возвращает значение false.
//Глобальные переменные используемые функцией:
// 1. string Buffer; 2. string Line; 3. CurRow; 4. DistLawTable
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ: нет
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: bool lSuccess
// true -  LEFTDISTLAW из файла fname удалось определить
// false - LEFTDISTLAW из файла fname не удалось определить
//*****************************************************************************
bool TLinearArray::GetLeftDistLawLAA()
{bool lSuccess;
 string ABC(EngABC); //= EngABC + RusABC + Digits + ":;"; //допустимый набор символов
 ABC.append(RusABC); ABC.append(Digits); ABC.append(":;");
 string Title = "LEFTDISTLAW";
 KEYSTRPOS info;
 //******************************************************
 //1. Поиск ключевого слова <LEFTDISTLAW>
 //******************************************************
 lSuccess = FindKeyStrInFile(Title, LAALfLawPattern, sz_LAALfLaw, Line,
            &info, ABC, CommentSep, sz_CommentSep, fin, ferr, LAAPerformPattern,
            sz_LAAPerform);
 if (lSuccess == false) return lSuccess;//Ключевое слово не найдено
 //********************************************************************
 //2. Поиск символа разделителя между <LEFTDISTLAW> и ее значением
 //********************************************************************
 uint rpos = Line.find_first_of(":",info.rpos);
 if (rpos == string::npos) //Ошибка символы-разделители <:=> не найдены
 {lSuccess = false;
  ferr << "ERROR:  Linear Array <LEFTDISTLAW> to receive it was not possible."
       << endl;
  ferr << "REASON: After <LEFTDISTLAW> the token-separator should follow.\n";
  ferr << "The following tokens were expected: <:> or <=>." << endl;
  ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << endl;
  ferr << "======== The block diagram of Linear Array Performances ========";
  Display(ferr,LAAPerformPattern,sz_LAAPerform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return lSuccess;
 }
 Line = Line.substr(rpos);
 //****************************************************************************
 //3. Определение вида закона взаимного расположения элементов АР, находящихся
 //   слева от основного элемента
 //****************************************************************************
 uint Index = 0; //Индекс расположения закона в таблице DistLawTable
 lSuccess = GetLawFromStr(DistLawTable, sz_DistLawTable, Index, Title,
                          LAAPerformPattern, sz_LAAPerform);
 if (lSuccess == false) return lSuccess;
 //Вид закона расположения элементов АР считан успешно
 LeftLawType = DistLawTable[Index].ident;
 LeftLaw = DistLawTable[Index].law;
 //****************************************************************************
 //4. Получение параметров для закона взаимного расположения элементов АР,
 //   находящихся слева от основного элемента
 //****************************************************************************
 LeftLawArg = GetLawParamFromFile(DistLawTable, Index, Title, LAAPerformPattern,
                                  sz_LAAPerform);
 if (LeftLawArg == NULL) {lSuccess = false; return lSuccess;}
 //Аргумент LEFTDISTLAW должен быть положительной величиной
 if (LeftLawType != TABLE_LAW) LeftLawArg->List[0] = fabs(LeftLawArg->List[0]);
 else //Для табличного закона все относительные расстояния положительные
  for (ulong i = 0; i < LeftLawArg->Size; i++)
   LeftLawArg->List[i] = fabs(LeftLawArg->List[i]);
return lSuccess;
}
//****************************************************************************
//СТАТУС: III.23., protected, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ: bool GetRightDistLawLAA();
//НАЗНАЧЕНИЕ:
//Формат строки: <RIGHTDISTLAW>: <Law name;> [<PARAMETERS: ARG1;ARG2;...ARGn;>]
//<Law name> - наименование закона;
//<PARAMETERS:> - список параметров закона;
//<ARGn> = [LETTER+[LETTER || DIGIT || OTHER] <=> ] double value <;>
//<LETTER> = EngABC + RusABC; DIGIT = Digits; OTHER = <[],->
//double value - вещественное число типа double
//Функция предназначена для получения сведений о законе по которому размещаются
//соседние элементы АР, расположенные справа от основного элемента линейной АР,
//информация о которой хранится во входной файловом потоке fin, в случае неэкви-
//дистантной решетки. Функция должна отыскать ключевое слово <RIGHTDISTLAW>,
//определить тип закона по ключевым словам из таблицы DistLawTable и, используя
//информацию о законе из этой таблицы, считать необходимое количество параметров,
//которые данный закон определяют.
//Формат строки по которому будет происходить считывание закона приведен выше.
//В случае возникновения ошибок синтаксического или семантического характера
//осуществляется их регистрация в файле ferrname, связанном с файловым потоком
//ошибок ferr, а функция прерывает свою работу и возвращает значение false.
//Глобальные переменные используемые функцией:
// 1. string Buffer; 2. string Line; 3. CurRow; 4. DistLawTable
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ: нет
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: bool lSuccess
// true -  RIGHTDISTLAW из файла fname удалось определить
// false - RIGHTDISTLAW из файла fname не удалось определить
//*****************************************************************************
bool TLinearArray::GetRightDistLawLAA()
{bool lSuccess;

 string ABC(EngABC); //= EngABC + RusABC + Digits + ":;"; //допустимый набор символов
 ABC.append(RusABC); ABC.append(Digits); ABC.append(":;");

 string Title = "RIGHTDISTLAW";
 KEYSTRPOS info;
 //******************************************************
 //1. Поиск ключевого слова <RIGHTDISTLAW>
 //******************************************************
 lSuccess = FindKeyStrInFile(Title, LAARtLawPattern, sz_LAARtLaw, Line,
            &info, ABC, CommentSep, sz_CommentSep, fin, ferr, LAAPerformPattern,
            sz_LAAPerform);
 if (lSuccess == false) return lSuccess;//Ключевое слово не найдено
 //********************************************************************
 //2. Поиск символа разделителя между <RIGHTDISTLAW> и ее значением
 //********************************************************************
 uint rpos = Line.find_first_of(":",info.rpos);
 if (rpos == string::npos) //Ошибка символы-разделители <:=> не найдены
 {lSuccess = false;
  ferr << "ERROR:  Linear Array <RIGHTDISTLAW> to receive it was not possible."
       << endl;
  ferr << "REASON: After <RIGHTDISTLAW> the token-separator should follow.\n";
  ferr << "The following tokens were expected: <:> or <=>." << endl;
  ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << endl;
  ferr << "======== The block diagram of Linear Array Performances ========";
  Display(ferr,LAAPerformPattern,sz_LAAPerform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return lSuccess;
 }
 Line = Line.substr(rpos);
 //****************************************************************************
 //3. Определение вида закона взаимного расположения элементов АР, находящихся
 //   справа от основного элемента
 //****************************************************************************
 uint Index = 0; //Индекс расположения закона в таблице DistLawTable
 lSuccess = GetLawFromStr(DistLawTable, sz_DistLawTable, Index, Title,
                          LAAPerformPattern, sz_LAAPerform);
 if (lSuccess == false) return lSuccess;
 //Вид закона расположения элементов АР считан успешно
 RightLawType = DistLawTable[Index].ident;
 RightLaw = DistLawTable[Index].law;
 //****************************************************************************
 //4. Получение параметров для закона взаимного расположения элементов АР,
 //   находящихся справа от основного элемента
 //****************************************************************************
 RightLawArg = GetLawParamFromFile(DistLawTable,Index,Title,LAAPerformPattern,
                                   sz_LAAPerform);
 if (RightLawArg == NULL) {lSuccess = false; return lSuccess;}
 //Аргумент RIGHTDISTLAW должен быть положительной величиной
 if (RightLawType != TABLE_LAW)
  RightLawArg->List[0] = fabs(RightLawArg->List[0]);
 else //Для табличного закона все относительные расстояния положительные
  for (ulong i = 0; i < RightLawArg->Size; i++)
   RightLawArg->List[i] = fabs(RightLawArg->List[i]);
return lSuccess;
}
//****************************************************************************
//СТАТУС: III.24., protected, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ: bool GetDisposLAA(bool& disp);
//НАЗНАЧЕНИЕ:
//Формат строки: <DISPOSITION>: <EQUATOR || MERIDIAN> <;>
//Функция предназначена для получения сведений о расположении линейной АР отно-
//сительно земной поверхности, информация о которой хранится во входной файловом
//потоке fin. Функция должна отыскать ключевое слово <DISPOSITION> и определить
//данное свойство по ключевым словам из таблицы соответствия ключевых слов и
//булевых значений.
//Линейная АР может располагаться в двух плоскостях в экваториальной плоскости,
//т.е. горизонтально поверхности Земли или в меридианальной плоскости, т.е. вер-
//тикально поверхности Земли. В первом случае, фазовые набеги в элементах АР бу-
//дут формироваться за счет азимутального угла источника сигнала. Во втором слу-
//чае - за счет угла места источника сигнала, принимаемого антенной решеткой. 
//В случае возникновения ошибок синтаксического или семантического характера
//осуществляется их регистрация в файле ferrname, связанном с файловым потоком
//ошибок ferr, а функция прерывает свою работу и возвращает значение false.
//Глобальные переменные используемые функцией:
// 1. string Buffer; 2. string Line; 3. CurRow
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. bool& disp - ссылка на индикатор расположения относительно поверхности
//   Земли антенной решетки
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: bool lSuccess
// true -  свойство DISPOSITION из файла fname удалось определить
// false - свойство DISPOSITION из файла fname не удалось определить
//*****************************************************************************
bool TLinearArray::GetDisposLAA(bool& disp)
{bool lSuccess;
 string ABC(EngABC); //= EngABC + RusABC + Digits + ":;"; //допустимый набор символов
 ABC.append(RusABC); ABC.append(Digits); ABC.append(":;");

 string Title = "DISPOSITION";
 KEYSTRPOS info;
 //******************************************************
 //1. Поиск ключевого слова <DISPOSITION>
 //******************************************************
 lSuccess = FindKeyStrInFile(Title, LAADispPattern, sz_LAADisp, Line,
            &info, ABC, CommentSep, sz_CommentSep, fin, ferr, LAAPerformPattern,
            sz_LAAPerform);
 if (lSuccess == false) return lSuccess;//Ключевое слово не найдено
 //*****************************************************************
 //2. Поиск символа разделителя между <DISPOSITION> и ее значением
 //*****************************************************************
 uint rpos = Line.find_first_of(":=",info.rpos);
 if (rpos == string::npos) //Ошибка символы-разделители <:=> не найдены
 {lSuccess = false;
  ferr << "ERROR:  Linear Array DISPOSITION to receive it was not possible."
       << endl;
  ferr << "REASON: After <DISPOSITION> the token-separator should follow.\n";
  ferr << "The following tokens were expected: <:> or <=>." << endl;
  ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << endl;
  ferr << "======== The block diagram of Linear Array Performances ========";
  Display(ferr,LAAPerformPattern,sz_LAAPerform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return lSuccess;
 }
 info.rpos = rpos;
 //*******************************************************
 //3. Определение значения свойства <DISPOSITION>
 //*******************************************************
 Title = "Linear Array "+Title;
 lSuccess = StrToBool(disp, Line, &info, TbDispos, sz_TbDispos, Title,
            LAAPerformPattern, sz_LAAPerform);
return lSuccess;
}
//****************************************************************************
//СТАТУС: III.25., protected, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ:
// bool GetElemArrayPerformFromFile(TElArray& ElArray, bool status = SLAVE)
//НАЗНАЧЕНИЕ:
//Функция предназначена для инициализации характеристик свойственных элементам
//линейной антенной решетке. К таким характеристикам относятся следующие:
//1. STATUS: <MASTER || SLAVE> - ОСНОВНОЙ или ВСПОМОГАТЕЛЬНЫЙ элемент АР. В АР
//   должен присутствовать один ОСНОВНОЙ элемент. Это элемент относительно кото-
//   рого ведется расчет фазовых набегов в элементах АР. Если status = MASTER,
//   то ожидается считывание основного элемента.
//2. USAGE: <ON || OFF> - использование элемента АР в расчетах.
//3. RELAMPLFACTOR: <0 < value <= 1> - относительный коэффициент усиления элемен-
//   та АР. Он определяет уровень приема антенны относительно уровня приема
//   антенны с максимальным КНД. Используется, если АР - неоднородная, т.к. в
//   однородной АР RelAmplFactor = n = 1 для всех элементов решетки.
//4. ANTENNA: <antenna name> - наименование антенны данного элемента АР. По наи-
//   менованию антенны определяется ее свойства - характеристики направленности
//   в E- и H-плоскостях через специальную таблицу соответствия.
//5. E_DIRECT: PARAMETERS: <arg1, arg2,...argN> - параметры функции направлен-
//   ности в E-плоскости (по необходимости).
//6. H_DIRECT: PARAMETERS: <arg1, arg2,...argN> - параметры функции направлен-
//   ности в H-плоскости (по необходимости).
//   Наличие характеристик 5 и 6 зависят от типа антенны, которая косвенно
//   задает характеристики направленности антенны в E- и H-плоскостях. Характе-
//   ристики направленности в свою очередь описываются функциями, для которых
//   дополнительное задание параметров не всегда является обязательным. К таким
//   антеннам, например, относятся полуволновая щель и полуволновой вибратор.
//7. E_LOCATION: <EQUATOR || MERIDIAN> - расположение E-плоскости антенны отно-
//   сительно поверхности Земли.
//8. NOISEAMPL: <Law name;> PARAMETERS: <arg1,arg2,...,argN> - закон изменения
//   амплитуды шума элемента АР.
//9. NOISEPHASE: <Law name;> PARAMETERS: <arg1,arg2,...,argN> - закон изменения
//   фазы шума элемента АР.
//В процессе считывания характеристик осуществлятся синтаксический и семантичес-
//кий контроль правильности данных. В случае ошибки функция завершает свою рабо-
//ту преждевременно с регистрацией возможной ошибке и поясняющими комментариями
//в файле ferrname. В качестве источника данных функция использует входной фай-
//ловый поток fin, ассоциированный с именем файла fname.
//Глобальные переменные используемые функцией:
// 1. string Buffer; 2. string Line; 3. CurRow
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. TElArray& ElArray - ссылка на объект ElArray. В данный объект заносятся
//   характеристики элемента АР сведения о которых хранятся в файле.
//2. bool status - указывает на статус считываемого элемента АР:
//   основного (MASTER) или вспомогательного (SLAVE).
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: bool lSuccess
// true -  свойства элемента АР из файла fname считаны успешно
// false - свойства элемента АР из файла fname считать не удалось
//*****************************************************************************
bool TLinearArray::GetElemArrayPerformFromFile(TElArray& ElArray, bool status)
{bool lSuccess;
 string ABC; //допустимый набор символов
 string title;
 KEYSTRPOS info;
 //******************************************************
 //1. Поиск строки-заголовка характеристик элемента АР
 //******************************************************
 //ABC = EngABC + RusABC + Digits;
 ABC.append(EngABC); ABC.append(RusABC); ABC.append(Digits);
 
 if (status)
 {title = "MASTER ELEMENT of LAA";
  lSuccess = FindKeyStrInFile(title, MasterHeader, sz_MasterHeader, Line,
             &info, ABC, CommentSep, sz_CommentSep, fin, ferr, AntPerform,
             sz_AntPerform);
 }
 else
 {title = "SLAVE ELEMENT of LAA";
  lSuccess = FindKeyStrInFile(title, SlaveHeader, sz_SlaveHeader, Line, &info,
             ABC, CommentSep, sz_CommentSep, fin, ferr, AntPerform,
             sz_AntPerform);
 }
 if (lSuccess == false) return lSuccess;
 //************************************************************************
 //1. Определение статуса элемента АР из файлового потока fin
 //   STATUS: MASTER || SLAVE
 //************************************************************************
 if (status == MASTER) ElArray.SetStatus(MASTER);
 else ElArray.SetStatus(SLAVE);
 //**************************************************************************
 //2. Определение использования элемента АР при расчетах из файлового потока
 //   fin : USAGE: ON || OFF
 //**************************************************************************
 lSuccess = GetElArrayUsage(ElArray);
 if (lSuccess == false) return lSuccess;
 //*****************************************************************************
 //3. Определение относительного коэффициента усиления элемента АР из файлового
 //   потока fin : AMPLRELFACTOR: <value>,  0 < value <= 1
 //*****************************************************************************
 lSuccess = GetElArrayARF(ElArray);
 if (lSuccess == false) return lSuccess;
 //Анализ, считанного из файла значения относит-го коэфф-та усиления антенны
 if ((ElArray.GetN() <= 0.0) || (ElArray.GetN() > 1.0))
 {lSuccess = false;
  ferr << "ERROR: Property <AMPLRELFACTOR> of Element LAA to receive it was not"
       << " possible."  << endl;
  ferr << "REASON: The value of a antenna relative amplification factor\n"
       << "        to overstep the limits of admissible value." << endl;
  ferr << "Admissible value of factor: 0 < N <= 1." << endl;
  ferr << "Obtained value of factor: " << ElArray.GetN() << "." << endl;
  ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << endl;
  ferr << "==== The block diagram of Linear Array Element Performances ====";
  Display(ferr,AntPerform,sz_AntPerform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return lSuccess;
 }
 //************************************************************************
 //4. Определение типа антенны по ее наименованию из файлового потока fin
 //   По типу антенны функция определяет диаграммы направленности антенны в
 //   магнитной и электрической плоскостях, а также параметры функций этих
 //   диаграмм направленности, если они необходимы
 //************************************************************************
 lSuccess = GetAntennaType(ElArray);
 if (lSuccess == false) return lSuccess;
 //*****************************************************************************
 //5. Определение расположения элемента АР относительно поверхности Земли из
 //   файлового потока fin : E_LOCATION: EQUATOR || MERIDIAN
 //*****************************************************************************
 lSuccess = GetAntELocate(ElArray);
 if (lSuccess == false) return lSuccess;
 //*****************************************************************************
 //6. Определение законов и их параметров изменения амплитуды и фазы шума на вы-
 //   ходе приемного канала из файлового потока fin
 //*****************************************************************************
 lSuccess = GetAntNoiseLaw(ElArray);
return lSuccess;
}
//****************************************************************************
//СТАТУС: III.29., protected, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ: bool GetAntennaType(TElArray& ElArray)
//НАЗНАЧЕНИЕ:
//Формат строки: <ANTENNA>: <Antenna name;>
//<Antenna name> - наименование антенны;
//Функция предназначена для получения сведений о наименовании антенны, которая
//входит в состав линейной антенной решетки. В начале функция осуществляет поиск
//ключевого слова  <ANTENNA>, затем после двоеточия должно следовать допустимое
//имя антенны. Информация по именам антенн и их характеристикам хранится в таб-
//лице AntTable. После успешного поиска соответствия наименования антенны в стро-
//ке файла допустимым именам из AntTable в объект элемента АР ElArray из этой
//таблицы заносятся сведения о типе антенны, законах направленности антенны в
//E- и H-плоскостях, их идентификаторах, а также формируется два списка парамет-
//ров функций типа ARGLIST.
//Формат строки по которому будет происходить считывание закона приведен выше.
//В случае возникновения ошибок синтаксического или семантического характера
//осуществляется их регистрация в файле ferrname, связанном с файловым потоком
//ошибок ferr, а функция прерывает свою работу и возвращает значение false.
//Глобальные переменные используемые функцией:
// 1. string Buffer; 2. string Line; 3. CurRow; 4. AntTable 5. AntPerform
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. TElArray& ElArray - ссылка на объект класса TElArray, в который будет поме-
//   щаться информация по текущему антенному элементу АР, считываемой из файла
//2. uint& index - ссылка на переменную, которая будет содержать индекс размеще-
//   ния антенны в таблице AntTable, в случае удачного поиска.
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: bool lSuccess
// true -  тип антенны из файла fname удалось определить
// false - тип антенны из файла fname не удалось определить
//*****************************************************************************
bool TLinearArray::GetAntennaType(TElArray& ElArray)
{bool lSuccess;
 string ABC(EngABC); //= EngABC + RusABC + Digits + ":;"; //допустимый набор символов
 ABC.append(RusABC); ABC.append(Digits); ABC.append(":;");

 string Title = "ANTENNA";
 KEYSTRPOS info;
 //******************************************************
 //1. Поиск ключевого слова <ANTENNA>
 //******************************************************
 lSuccess = FindKeyStrInFile(Title, AntennaPattern, sz_Antenna, Line,
            &info, ABC, CommentSep, sz_CommentSep, fin, ferr, AntPerform,
            sz_AntPerform);
 if (lSuccess == false) return lSuccess;//Ключевое слово не найдено
 //********************************************************************
 //2. Поиск символа разделителя между <ANTENNA> и ее значением
 //********************************************************************
 uint rpos = Line.find_first_of(":",info.rpos);
 if (rpos == string::npos) //Ошибка символы-разделители <:=> не найдены
 {lSuccess = false;
  ferr << "ERROR: Property <ANTENNA> of Element LAA to receive it was not possible."
       << endl;
  ferr << "REASON: After <ANTENNA> the token-separator should follow.\n";
  ferr << "The following token were expected: <:>." << endl;
  ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << endl;
  ferr << "==== The block diagram of Linear Array Element Performances ====";
  Display(ferr,AntPerform,sz_AntPerform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return lSuccess;
 }
 Line = Line.substr(rpos);
 //****************************************************************************
 //3. Определение типа антенны элемента АР по ее наименованию из строки файла
 //****************************************************************************

 uint Index = 0; //Индекс расположения антенны в таблице AntTable
 lSuccess = GetAntNameFromStr(AntTable, sz_AntTable, Index, Title);
 //Тип антенны элемента АР определить не удалось
 if (lSuccess == false) return lSuccess;

 //****************************************************************************
 //4. Формирование списка параметров для диаграммы направленности антенны в
 //   E-плоскости
 //****************************************************************************
 ARGLIST* EArg = GetDirectParamFromFile(Index,E_PLANE);
 if (EArg == NULL) {lSuccess = false; return lSuccess;}

 //****************************************************************************
 //5. Формирование списка параметров для диаграммы направленности антенны в
 //   H-плоскости
 //****************************************************************************
 ARGLIST* HArg = GetDirectParamFromFile(Index,H_PLANE);
 if (HArg == NULL) {lSuccess = false; return lSuccess;}

 //Устанавливаем тип антенны, функции диаграмм направленности антенны в
 //E- и Н-плоскостях и их параметры
 ElArray.SetPerformance(AntTable[Index].type,MERIDIAN,EArg,HArg);

return lSuccess;
}
//****************************************************************************
//СТАТУС: III.30., protected, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ:
// bool GetAntNameFromStr(const ANTENNA_INFO* Table, const uint sz_Table,
//      uint& index, const string& title);
//НАЗНАЧЕНИЕ:
//Формат строки: <Antenna name;>
//<Antenna name> = <str1> || <str2> || ... <strN>
//Функция предназначена для поиска ключевой строки - наименования антенны, кото-
//рые хранятся в массиве Table в строке Line и в случае удачного поиска присвае-
//ния переменной index индекса записи, в которой содержится информация по найден-
//ному наименованию закона. Формат строки приведен выше.
//В случае ошибок, функция возвращает значение false и информация об ошибке ре-
//гистрируется в файловом потоке ошибок ferr. Ошибкой считается необнаружение
//ключевой строки в строке Line, а также возможные синтаксические ошибки, свя-
//занные с отступлением от формата строки приведенного выше. К таким ошибкам
//относятся пропуск символа ';', наличие в строке других символов, кроме пробе-
//лов и табуляций.
//Глобальные переменные используемые функцией:
//1. string Buffer; 2. string Line; 3. CurRow; 4. AntPerform; 
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. const ANTENNA_INFO* Table - указатель на таблицу с информацией о возможных
//   наименованиях антенны, которые может содержать строка Line
//2. const uint sz_Table - размер таблицы Table
//3. uint& index - ссылка на переменную, в которой будет храниться индекс
//   закона в таблице Table, в случае удачного поиска его в строке Line
//4. const string& title - ссылка на строку с наименованием искомой ключевой
//   строки в файловом потоке fin
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: bool lSuccess
// true -  в строке Line наименование антенны найдено
// false - в строке Line допустимого наименования антенны найти не удалось
//*****************************************************************************
bool TLinearArray::GetAntNameFromStr(const ANTENNA_INFO* Table,
     const uint sz_Table, uint& index, const string& title)
{bool lSuccess;
 uint rpos;
 uint lpos;
 //Поиск символа-разделителя ';' между ключевыми строками, наличие которого
 //обязательно, в строке Line
 rpos = Line.find_first_of(';');
 if (rpos == string::npos) //Ошибка
 {lSuccess = false;
  ferr << "ERROR:  " << title << " to receive it was not possible." << endl;
  ferr << "REASON: Complete token <;> was expected for this data string" << endl;
  ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << endl;
  ferr << "============== The block diagram of Performances ===============";
  Display(ferr,AntPerform,sz_AntPerform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return lSuccess;
 }
 //Извлекаем из line подстроку s1 с завершающим символом ';'
 string s1 = Line.substr(1,rpos+1);
 //s1.set_case_sensitive(0); //Отключаем контекстно-чувствительный поиск
 //Поиск допустимого наименования закона из массива законов Table в строке s1
 KEYSTRPOS keystr_info;
 for (uint i = 0; i < sz_Table; i++)
 {lSuccess=SearchKeyString(s1,Table[i].name,Table[i].sz_name,&keystr_info);
  if (lSuccess) {index = i; break;}
 }
 if (lSuccess == false) //Ошибка: наименование закона не найдено
 {ferr << "ERROR:  " << title << " to receive it was not possible." << endl;
  ferr << "REASON: Law name to find it was not possible." << endl;
  ferr << "Line N " << CurRow << ": <" << s1 << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">";
  s1 = "The following ANTENNA NAMEs were expected:";
  string s2;
  char str[20];
  for (uint i = 0; i < sz_Table; i++)
  {itoa(i+1,str,10); s2 = str; s2 =  s2 + ". " + s1;
   Display(ferr, Table[i].name, Table[i].sz_name, s2.c_str());
  }
  ferr << endl;
  ferr << "============== The block diagram of Performances ===============";
  Display(ferr,AntPerform,sz_AntPerform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return lSuccess;
 }
//Поиск посторонних символов слева и справа от наименования закона. Кроме
//символа пробела и табуляции других символов быть не должно.
//Поиск посторонних символов слева от наименования закона
 string s2 = s1.substr(0,keystr_info.lpos);
 lpos = s2.find_first_not_of(" \t");
  if (lpos != string::npos) //Ошибка: запрещенные символы найдены
  {s2 = "To the left of "; lSuccess = false;}
//Поиск посторонних символов справа от наименования закона до ';'
 if (lSuccess != false)
 { s2 = s1.substr(keystr_info.rpos+1);
   lpos = s2.find_first_not_of(" \t;");
   if (lpos != string::npos) //Ошибка: запрещенные символы найдены
   {s2 = "To the right of "; lSuccess = false;}
 }
 if (lSuccess == false) //Формируем сообщение об ошибке
 {ferr << "ERROR:  " << title << " to receive it was not possible." << endl;
  ferr << "REASON: " << s2 << "law name a PROHIBITED TOKENS are";
  ferr << " detected." << endl;
  ferr << "Line N " << CurRow << ": <" << s1 << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << "The following allowed tokens were expected: <[SPACE],[TAB]>." << endl;
  ferr << endl;
  ferr << "============== The block diagram of Performances ===============";
  Display(ferr,AntPerform,sz_AntPerform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return lSuccess;
 }
 //Наименование закона определено
 if (rpos != Line.length()-1) Line = Line.substr(rpos+1);
 else Line = "";
return lSuccess;
}
//****************************************************************************
//СТАТУС: III.31., protected, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ:
// ARGLIST* GetDirectParamFromFile(const uint index, bool plane);
//НАЗНАЧЕНИЕ:
//Формат строки: <KEY STRING: ARG1; ARG2; ... ARGn;>
//<KEY STRING> = <E_DIRECT> || <H_DIRECT>
//<ARGn> = [IDENT <=>] <value>; IDENT - идентификатор параметра (наименование)
//<IDENT> = LETTER+[LETTER||DIGIT||OTHER]+[LETTER||DIGIT||OTHER]+ ...]
//<OTHER> = <[]-_> for example: lv[-1_0]
//<value> - цифровая величина
//Функция предназначена для создания списка параметров диаграммы направленности
//антенны в E- или H-плоскости, при необходимости считывая данные из файлового
//потока fin.
//Информация по количеству считываемых параметров и размеру списка ARGLIST arg,
//в котором они будут накапливаться, содержится в таблице AntTable. Индекс
//index указывает на адрес в AntTable по которому храниться информация о размере
//создаваемого списка параметров ARGLIST и количестве параметров, которые необ-
//ходимо считать из файла для нормальной работы функций, задающих диаграммы
//направленности антенны заданного типа. Переменная plane определяет для какой
//из плоскостей будет создаваться список параметров. Параметры могут распола-
//гаться на нескольких строках файла. Формат строки приведен выше. Параметры,
//которые не поместились на первой строке следует размещать на следующих строках
//без указания ключевого слова. Параметры диаграмм направленности в общем случае
//являются вещественными числами. Перед каждым значением параметра может быть
//указан его идентификатор с последующим оператором присваивания типа <=>.
//В случае ошибок, функция возвращает значение LawArg == NULL и информация об
//ошибке регистрируется в файловом потоке ошибок ferr. Ошибкой считается необна-
//ружение ключевой строки в строке Line, а также возможные синтаксические ошибки,
//связанные с отступлением от формата строки приведенного выше. К таким ошибкам
//относятся пропуск символа ';', наличие в строке других символов, кроме пробе-
//лов и табуляций.
//Глобальные переменные используемые функцией:
//1. string Buffer; 2. string Line; 3. CurRow; 4. AntTable; 5. EDirectPattern;
//6. HDirectPattern; 7. AntPerform
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. const uint index - индекс антенны в таблице AntTable, для которой будет
//   создан список параметров закона направленности антенны в E- или H-плоскости
//2. bool plane - определяет создание списка параметров для E- или H-плоскости.
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: ARGLIST* LawArg - список считанных из файла параметров
//закона диаграммы направленности антенны  для электрической или магнитной плос-
//кости из таблицы AntTable расположенной в ней под индексом index
//В случае неудачного считывания параметров LawArg == NULL
//*****************************************************************************
ARGLIST* TLinearArray::GetDirectParamFromFile(const uint index, bool plane)
{ARGLIST* LawArg = NULL;
 string title;
 uint total_arg;
 uint read_arg;
 if (plane == E_PLANE)
 {title = "E_DIRECT PARAMETERS for Antenna";
  total_arg = AntTable[index].E_total;
  read_arg = AntTable[index].E_read;
 }
 else
 {title = "H_DIRECT PARAMETERS for Antenna";
  total_arg = AntTable[index].H_total;
  read_arg = AntTable[index].H_read;
 }
 //Проверка соответствия между общим числом параметров и количеством считываемых
 //параметров
 if ((total_arg == 0) || (total_arg <= read_arg))
 //Ошибка: несоответствие между числовыми значениями параметрами закона
 {ferr << "ERROR:  " << title << " to receive it was not possible." << endl;
  ferr << "REASON: " << "Detect mismatch between total parameters and amount of\n";
  ferr << "        reading parameters." << endl;
  ferr << "Antenna name: <" << AntTable[index].name[1] << ">;" << endl;
  ferr << "Total_Arg = " << total_arg << endl;
  ferr << "Read_Arg  = " << read_arg << endl;
  ferr << "TIPS: Total_Arg > Read_Arg && Total_Arg > 0" << endl;
  ferr << endl;
  ferr << "============== The block diagram of Performances ===============";
  Display(ferr,AntPerform,sz_AntPerform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return LawArg;
 }
 //*************************************************************************
 //1. Создание списка ARGLIST, в который будут заноситься параметры закона
 //*************************************************************************
 LawArg = CreateArgList(total_arg);
 //*************************************************************************
 //2. Проверка на необходимость считывания параметров из файла.
 //*************************************************************************
 if (read_arg == 0)  return LawArg;
 //******************************************************
 //3. Поиск ключевого слова <KEYSTRING>
 //******************************************************
 bool lSuccess;
 string ABC(EngABC); //= EngABC + RusABC + Digits + ":;"; //допустимый набор символов
 ABC.append(RusABC); ABC.append(Digits); ABC.append(":;");

 KEYSTRPOS info;

 if (plane == E_PLANE)
  lSuccess = FindKeyStrInFile(title, EDirectPattern, sz_EDirect, Line,
             &info, ABC, CommentSep, sz_CommentSep, fin, ferr, AntPerform,
             sz_AntPerform);
 else
  lSuccess = FindKeyStrInFile(title, HDirectPattern, sz_HDirect, Line,
             &info, ABC, CommentSep, sz_CommentSep, fin, ferr, AntPerform,
             sz_AntPerform);
 //Ключевое слово не найдено
 if (lSuccess == false) {LawArg = DeleteArgList(LawArg); return LawArg;}

 //********************************************************************
 //2. Поиск символа разделителя между <KEYSTRING> и параметрами
 //********************************************************************
 uint rpos = Line.find_first_of(":",info.rpos);
 if (rpos == string::npos) //Ошибка символы-разделители <:=> не найдены
 {LawArg = DeleteArgList(LawArg);
  ferr << "ERROR:  " << title << " to receive it was not possible." << endl;
  ferr << "REASON: After <KEYSTRING> the token-separator should follow.\n";
  ferr << "The following token were expected: <:>." << endl;
  ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << endl;
  ferr << "==== The block diagram of Linear Array Element Performances ====";
  Display(ferr,AntPerform,sz_AntPerform,"","","",false);
  ferr << "================================================================";
  ferr << endl;
  return LawArg;
 }
 //Ключевое слово <KEYSTRING> определено
 if (rpos != Line.length()-1) Line = Line.substr(rpos+1);
 else Line = "";
//***************************************************************************
//5. Последовательное считывание параметров данного закона из файла и
//   занесение их в список LawArg
//***************************************************************************
 ulong ReadArg = 0L;  //Количество считанных параметров
 double Value = 0.0;
 //Цикл считывания параметров закона
 for (ulong i = 0; i < read_arg; i++)
 {lSuccess = GetDoubleFromFile(Value,total_arg,ReadArg,title,AntPerform,
             sz_AntPerform);
  //Ошибка при считывании данных
  if (lSuccess == false) {LawArg = DeleteArgList(LawArg); break;}
  //Очередной параметр считан из файлового потока успешно
  ReadArg++;
  LawArg->List[i+1] = Value;
 }
 if (LawArg == NULL) //Ошибка при считывании параметров из файла
 {ferr << "****************************************************************\n";
  ferr << "The following parameters were expected:";
  if (plane == E_PLANE)
   Display(ferr,AntTable[index].EParam,AntTable[index].sz_EParam);
  else
   Display(ferr,AntTable[index].HParam,AntTable[index].sz_HParam);
  ferr << "****************************************************************\n";
 }
return LawArg;
}
//****************************************************************************
//СТАТУС: III.32., protected, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ: bool GetElArrayUsage(TElArray& ElArray)
//НАЗНАЧЕНИЕ:
//Формат строки: <USAGE>: <ON || OFF> <;>
//Функция предназначена для получения сведений об использовании элемента линей-
//ной АР при расчетах, информация о котором хранится во входной файловом потоке
//fin. Функция должна отыскать ключевое слово <USAGE> и определить данное
//свойство по ключевым словам из таблицы соответствия ключевых слов и булевых
//значений.
//В случае возникновения ошибок синтаксического или семантического характера
//осуществляется их регистрация в файле ferrname, связанном с файловым потоком
//ошибок ferr, а функция прерывает свою работу и возвращает значение false.
//Глобальные переменные используемые функцией:
// 1. string Buffer; 2. string Line; 3. CurRow; 4. AntPerform; 5. TbUsage;
// 6. LAAElemUsage
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. TElArray& ElArray - ссылка на объект элемента антенной решетки
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: bool lSuccess
// true -  свойство USAGE из файла fname удалось определить
// false - свойство USAGE из файла fname не удалось определить
//*****************************************************************************
bool TLinearArray::GetElArrayUsage(TElArray& ElArray)
{bool lSuccess;
 string ABC(EngABC); //= EngABC + RusABC + Digits + ":;"; //допустимый набор символов
 ABC.append(RusABC); ABC.append(Digits); ABC.append(":;");

 string Title = "USAGE";
 KEYSTRPOS info;
 //******************************************************
 //1. Поиск ключевого слова <USAGE>
 //******************************************************
 lSuccess = FindKeyStrInFile(Title, LAAElemUsage, sz_LAAElemUsage, Line,
            &info, ABC, CommentSep, sz_CommentSep, fin, ferr, AntPerform,
            sz_AntPerform);
 if (lSuccess == false) return lSuccess;//Ключевое слово не найдено
 //*****************************************************************
 //2. Поиск символа разделителя между <USAGE> и ее значением
 //*****************************************************************
 uint rpos = Line.find_first_of(":=",info.rpos);
 if (rpos == string::npos) //Ошибка символы-разделители <:=> не найдены
 {lSuccess = false;
  ferr << "ERROR:  USAGE of LAA Element to receive it was not possible." << endl;
  ferr << "REASON: After <USAGE> the token-separator should follow.\n";
  ferr << "The following tokens were expected: <:> or <=>." << endl;
  ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << endl;
  ferr << "=========  The block diagram of LAA Element Performances  =========";
  Display(ferr,AntPerform,sz_AntPerform,"","","",false);
  ferr << "===================================================================";
  ferr << endl;
  return lSuccess;
 }
 info.rpos = rpos;
 //*******************************************************
 //3. Определение значения свойства <USAGE>
 //*******************************************************
 bool usage = OFF;
 Title = Title + " of Antenna Array Element";
 lSuccess = StrToBool(usage, Line, &info, TbUsage, sz_TbUsage, Title,
            AntPerform, sz_AntPerform);
 if (lSuccess) ElArray.SetUsage(usage);
return lSuccess;
}
//****************************************************************************
//СТАТУС: III.33., protected, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ: bool GetElArrayARF(TElArray& ElArray)
//НАЗНАЧЕНИЕ:
//Формат строки: <AMPLRELFACTOR>: [<Ident> <=> ] <value>;  0 < value <= 1
//Функция предназначена для получения сведений об относительном коэффициенте уси-
//ления элемента АР, информация о котором хранится во входном файловом потоке
//fin. Функция должна отыскать ключевое слово <AMPLRELFACTOR> и определить дан-
//ное значение коэффициента, которое должно быть положительным вещественным чис-
//лом не превосходящим 1. Формат строки по которому будет происходить считывание
//данной характеристики элемента антенной решетки приведен выше.
//В случае возникновения ошибок синтаксического или семантического характера
//осуществляется их регистрация в файле ferrname, связанном с файловым потоком
//ошибок ferr, а функция прерывает свою работу и возвращает значение false.
//Глобальные переменные используемые функцией:
// 1. string Buffer; 2. string Line; 3. CurRow; 4. AntPerform; 5. LAAElemARF
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. TElArray& ElArray - ссылка на объект элемента антенной решетки
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: bool lSuccess
//true -  свойство элемента АР AMPLRELFACTOR из файла fname удалось определить
//false - свойство элемента АР AMPLRELFACTOR из файла fname не удалось определить
//*****************************************************************************
bool TLinearArray::GetElArrayARF(TElArray& ElArray)
{bool lSuccess;
 string ABC(EngABC); //= EngABC + RusABC + Digits + ":;"; //допустимый набор символов
 ABC.append(RusABC); ABC.append(Digits); ABC.append(":;");

 string Title = "AMPLRELFACTOR";
 KEYSTRPOS info;
 //******************************************************
 //1. Поиск ключевого слова <AMPLRELFACTOR>
 //******************************************************
 lSuccess = FindKeyStrInFile(Title, LAAElemARF, sz_LAAElemARF, Line,
            &info, ABC, CommentSep, sz_CommentSep, fin, ferr, AntPerform,
            sz_AntPerform);
 if (lSuccess == false) return lSuccess;//Ключевое слово не найдено
 //*****************************************************************
 //2. Поиск символа разделителя между <AMPLRELFACTOR> и ее значением
 //*****************************************************************
 uint rpos = Line.find_first_of(":=",info.rpos);
 if (rpos == string::npos) //Ошибка символы-разделители <:=> не найдены
 {lSuccess = false;
  ferr << "ERROR:  AMPLRELFACTOR of Element LAA  to receive it was not possible."
       << endl;
  ferr << "REASON: After <AMPLRELFACTOR> the token-separator should follow.\n";
  ferr << "The following tokens were expected: <:> or <=>." << endl;
  ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << endl;
  ferr << "==== The block diagram of Antenna Array Element Performances ====";
  Display(ferr,AntPerform,sz_AntPerform,"","","",false);
  ferr << "=================================================================";
  ferr << endl;
  return lSuccess;
 }
 info.rpos = rpos;
 //**************************************************************************
 //3. Определение значения коэффициента относительного усиления элемента АР
 //**************************************************************************
 if (rpos == string::npos) //Ошибка! В строке Line отсутствует информация об LV
 {lSuccess = false;
  ferr << "ERROR:  AMPLRELFACTOR of LAA Element to receive it was not possible."
       << endl;
  ferr << "REASON: There is no information about a amplify relative factor\n";
  ferr << "        the element of Antenna Array." << endl;
  ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << endl;
  ferr << "========  The block diagram of LAA Element Performances  ========";
  Display(ferr,AntPerform,sz_AntPerform,"","","",false);
  ferr << "=================================================================";
  ferr << endl;
  return lSuccess;
 }
  //Определяем относительное расстояние между элементами эквидистантной АР
  Line = Line.substr(rpos+1);
  double factor = 0.0;
  lSuccess = StrToDouble(factor, Line, NameARF, sz_NameARF, Title, AntPerform,
             sz_AntPerform);
  if (lSuccess == false) return lSuccess;
  ElArray.SetN(factor);
return lSuccess;
}
//****************************************************************************
//СТАТУС: III.34., protected, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ: bool GetAntELocate(TElArray& ElArray);
//НАЗНАЧЕНИЕ:
//Формат строки: <E_LOCATION>: <EQUATOR || MERIDIAN> <;>
//Функция предназначена для получения сведений о расположении антенны элемента
//АР относительно земной поверхности, информация о которой хранится во входной
//файловом потоке fin. Функция должна отыскать ключевое слово <E_LOCATION> и
//определить данное свойство по ключевым словам из таблицы соответствия ключевых слов и
//булевых значений.
//Элемент АР может располагаться в двух плоскостях в экваториальной плоскости,
//т.е. горизонтально поверхности Земли или в меридианальной плоскости, т.е. вер-
//тикально поверхности Земли.
//В случае возникновения ошибок синтаксического или семантического характера
//осуществляется их регистрация в файле ferrname, связанном с файловым потоком
//ошибок ferr, а функция прерывает свою работу и возвращает значение false.
//Глобальные переменные используемые функцией:
// 1. string Buffer; 2. string Line; 3. CurRow; 4. AntPerform;
// 5. LAAElemELocation
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. TElArray& ElArray - ссылка на объект элемента антенной решетки
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: bool lSuccess
// true -  свойство E_LOCATION из файла fname удалось определить
// false - свойство E_LOCATION из файла fname не удалось определить
//*****************************************************************************
bool TLinearArray::GetAntELocate(TElArray& ElArray)
{bool lSuccess;
 string ABC(EngABC); //= EngABC + RusABC + Digits + ":;"; //допустимый набор символов
 ABC.append(RusABC); ABC.append(Digits); ABC.append(":;");

 string Title = "E_LOCATION";
 KEYSTRPOS info;
 //******************************************************
 //1. Поиск ключевого слова <E_LOCATION>
 //******************************************************
 lSuccess = FindKeyStrInFile(Title, LAAElemELocation, sz_LAAElemELocation, Line,
            &info, ABC, CommentSep, sz_CommentSep, fin, ferr, AntPerform,
            sz_AntPerform);
 if (lSuccess == false) return lSuccess;//Ключевое слово не найдено
 //*****************************************************************
 //2. Поиск символа разделителя между <E_LOCATION> и ее значением
 //*****************************************************************
 uint rpos = Line.find_first_of(":=",info.rpos);
 if (rpos == string::npos) //Ошибка символы-разделители <:=> не найдены
 {lSuccess = false;
  ferr << "ERROR:  E_LOCATION of LAA Element to receive it was not possible."
       << endl;
  ferr << "REASON: After <E_LOCATION> the token-separator should follow.\n";
  ferr << "The following tokens were expected: <:> or <=>." << endl;
  ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << endl;
  ferr << "========  The block diagram of LAA Element Performances  ========";
  Display(ferr,AntPerform,sz_AntPerform,"","","",false);
  ferr << "=================================================================";
  ferr << endl;
  return lSuccess;
 }
 info.rpos = rpos;
 //*******************************************************
 //3. Определение значения свойства <E_LOCATION>
 //*******************************************************
 bool disp = MERIDIAN;
 Title = Title +" LAA Element";
 lSuccess = StrToBool(disp, Line, &info, TbDispos, sz_TbDispos, Title,
            AntPerform, sz_AntPerform);
 if (lSuccess) ElArray.SetELocation(disp);
return lSuccess;
}
//****************************************************************************
//СТАТУС: III.35., protected, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ: bool GetAntNoiseLaw(TElArray& ElArray)
//НАЗНАЧЕНИЕ:
//Формат строки: <NOISEAMPL>: <Law name;> [<PARAMETERS: ARG1;ARG2;...ARGn;>]
//Формат строки: <NOISEPHASE>: <Law name;> [<PARAMETERS: ARG1;ARG2;...ARGn;>]
//<Law name> - наименование закона;
//<PARAMETERS:> - список параметров закона;
//<ARGn> = [LETTER+[LETTER || DIGIT || OTHER] <=> ] double value <;>
//<LETTER> = EngABC + RusABC; DIGIT = Digits; OTHER = <[],->
//double value - вещественное число типа double
//Функция предназначена для получения сведений о законах, по которым изменяется
//амплитуда и фаза внутреннего шума на выходе приемного канала, информация о ко-
//торых хранится во входной файловом потоке fin. Функция должна отыскать ключе-
//вое слово <NOISEAMPL> или <NOISEPHASE>, определить тип закона по ключевым
//словам из таблицы NoiseLawTable и, используя информацию о законе из этой таб-
//лицы, считать необходимое количество параметров, которые данный закон опреде-
//ляют. Формат строки по которому будет происходить считывание законов приведен
//выше.
//В случае возникновения ошибок синтаксического или семантического характера
//осуществляется их регистрация в файле ferrname, связанном с файловым потоком
//ошибок ferr, а функция прерывает свою работу и возвращает значение false.
//Глобальные переменные используемые функцией:
// 1. string Buffer; 2. string Line; 3. CurRow; 4. NoiseLawTable;
// 5. LAAElemNoiseAmpl; 6. LAAElemNoisePhase
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. TElArray& ElArray - ссылка на объект элемента антенной решетки
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: bool lSuccess
// true -  NOISEAMPL и NOISEPHASE из файла fname удалось определить
// false - NOISEAMPL и NOISEPHASE из файла fname не удалось определить
//*****************************************************************************
bool TLinearArray::GetAntNoiseLaw(TElArray& ElArray)
{bool lSuccess;
 string ABC(EngABC); //= EngABC + RusABC + Digits + ":;"; //допустимый набор символов
 ABC.append(RusABC); ABC.append(Digits); ABC.append(":;");

 string Title = "NOISEAMPL";
 KEYSTRPOS info;
 //******************************************************
 //1. Поиск ключевого слова <NOISEAMPL>
 //******************************************************
 lSuccess = FindKeyStrInFile(Title, LAAElemNoiseAmpl, sz_LAAElemNoiseAmpl, Line,
            &info, ABC, CommentSep, sz_CommentSep, fin, ferr, AntPerform,
            sz_AntPerform);
 if (lSuccess == false) return lSuccess;//Ключевое слово не найдено
 //********************************************************************
 //2. Поиск символа разделителя между <NOISEAMPL> и ее значением
 //********************************************************************
 uint rpos = Line.find_first_of(":",info.rpos);
 if (rpos == string::npos) //Ошибка символы-разделители <:=> не найдены
 {lSuccess = false;
  ferr << "ERROR:  <NOISEAMPL> of LAA Element to receive it was not possible."
       << endl;
  ferr << "REASON: After <NOISEAMPL> the token-separator should follow.\n";
  ferr << "The following tokens were expected: <:>." << endl;
  ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << endl;
  ferr << "========  The block diagram of LAA Element Performances  ========";
  Display(ferr,AntPerform,sz_AntPerform,"","","",false);
  ferr << "=================================================================";
  ferr << endl;
  return lSuccess;
 }
 Line = Line.substr(rpos);
 //****************************************************************************
 //3. Определение вида закона изменения амплитуды внутреннего шума
 //****************************************************************************
 uint Index = 0; //Индекс расположения закона в таблице NoiseLawTable
 lSuccess = GetLawFromStr(NoiseLawTable, sz_NoiseLawTable, Index, Title,
                          AntPerform, sz_AntPerform);
 if (lSuccess == false) return lSuccess;
 //Вид закона амплитуды шума
 uint AmplType = NoiseLawTable[Index].ident;
 //****************************************************************************
 //4. Получение параметров для закона изменения амплитуды шума
 //****************************************************************************
 ARGLIST* AmplArg = GetLawParamFromFile(NoiseLawTable,Index,Title,AntPerform,
                                   sz_AntPerform);
 if (AmplArg == NULL) {lSuccess = false; return lSuccess;}

 //******************************************************
 //1. Поиск ключевого слова <NOISEPHASE>
 //******************************************************
 Title = "NOISEPHASE";
 lSuccess = FindKeyStrInFile(Title,LAAElemNoisePhase,sz_LAAElemNoisePhase,Line,
            &info, ABC, CommentSep, sz_CommentSep, fin, ferr, AntPerform,
            sz_AntPerform);
 if (lSuccess == false) return lSuccess;//Ключевое слово не найдено
 //********************************************************************
 //2. Поиск символа разделителя между <NOISEPHASE> и ее значением
 //********************************************************************
 rpos = Line.find_first_of(":",info.rpos);
 if (rpos == string::npos) //Ошибка символы-разделители <:=> не найдены
 {lSuccess = false;
  ferr << "ERROR:  <NOISEPHASE> of LAA Element to receive it was not possible."
       << endl;
  ferr << "REASON: After <NOISEPHASE> the token-separator should follow.\n";
  ferr << "The following tokens were expected: <:>." << endl;
  ferr << "Line N " << CurRow << ": <" << Line << ">" << endl;
  ferr << "BUFFER: <" << Buffer << ">" << endl;
  ferr << endl;
  ferr << "========  The block diagram of LAA Element Performances  ========";
  Display(ferr,AntPerform,sz_AntPerform,"","","",false);
  ferr << "=================================================================";
  ferr << endl;
  return lSuccess;
 }
 Line = Line.substr(rpos);
 //****************************************************************************
 //3. Определение вида закона изменения фазы внутреннего шума
 //****************************************************************************
 Index = 0; //Индекс расположения закона в таблице NoiseLawTable
 lSuccess = GetLawFromStr(NoiseLawTable, sz_NoiseLawTable, Index, Title,
                          AntPerform, sz_AntPerform);
 if (lSuccess == false) return lSuccess;
 //Вид закона фазы шума
 uint PhaseType = NoiseLawTable[Index].ident;
 //****************************************************************************
 //4. Получение параметров для закона изменения фазы шума
 //****************************************************************************
 ARGLIST* PhaseArg = GetLawParamFromFile(NoiseLawTable,Index,Title,AntPerform,
                                         sz_AntPerform);
 if (PhaseArg == NULL) {lSuccess = false; return lSuccess;}
 //Законы и их параметры определены успешно
 ElArray.SetNoisePerform(AmplType, PhaseType, AmplArg, PhaseArg);

return lSuccess;
}

//----------------------- The end of Linarray.cpp -----------------------------
