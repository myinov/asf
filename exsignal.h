//exsignal.h
//Объявление класса ExternSignal
//Copyright (c) JohnSoft 1998. All rights reserved.
//Initial date: August,18,1998. Final date:
//Заголовочный файл extsignal.h объявляет класс ExternSignal, который позволяет
//описать внешний сигнал, поступающий на вход системы - антенной  решетки. Внеш-
//ний сигнал может быть представлен отдельными независимыми  компонентами (слож-
//ный сигнал) или одной компонентой (простой сигнал). Сложный  сигнал состоит из
//двух  составляющих:  ПОЛЕЗНОГО  СИГНАЛА  (SIGNAL)  и АКТИВНЫХ ПОМЕХ (JAMMERS).
//Предполагается  наличие  ОДНОГО ПОЛЕЗНОГО  СИГНАЛА и МНОЖЕСТВА ПОМЕХ. Свойства
//каждой  компоненты  сигнала  описывается  структурой COMPONENT (составляющая).
//Массив структур COMPONENT описывает свойства внешнего сигнала (external signal)
//или внешнего воздействия (external action).

#if !defined(__EXSIGNAL_H)
#define __EXSIGNAL_H

//Заголовочный файл содержит объявления законов по которым могут изменяться
//составляющие амплитуд и фаз сигналов
#include "mathfunc.h"

#define CONSOLE 1  //В программе используется консольный ввод-вывод

const uint NONPOLAR = 0; //Не учитывать поляризационные свойства сигнала
const uint V_POLAR  = 1; //Составляющая сигнала с вертикальной поляризацией
const uint H_POLAR  = 2; //Составляющая сигнала с горизонтальной поляризацией

class ExtSignal; //Неполное объявление класса

//*****************************************************************************
//Объявление структуры характеристик (свойств) одной составляющей (компоненты)
//                     внешнего сигнала COMPONENT
//*****************************************************************************
typedef struct
{
 /*        Общая характеристика составляющей внешнего сигнала        */

 bool Type; //Тип компоненты внешнего сигнала: SIGNAL или PARASITE
 bool Usage;//Использование компоненты для расчетов: ON или OFF

 //Азимутальный угол расположения источника компоненты сигнала относительно
 //нормали к раскрыву ОСНОВНОЙ АНТЕННЫ (в радианах) - azimuth angle
 //Вид закона изменения угла по азимуту источника сигнала от времени
 uint AzAngleLawType;
 double (*AzimuthAngle)(const ARGLIST *Arg);//Закон изменения азимутального угла
 ARGLIST *AzAngleArg; //Список параметров закона изменения азимутального угла

 //Угол места расположения источника компоненты внешнего сигнала
 //относительно нормали к раскрыву ОСНОВНОЙ АНТЕННЫ (в радианах) -
 //angle of elevation
 //Вид закона изменения угла места источника сигнала от времени
 uint ElevAngleLawType;
 double (*ElevateAngle)(const ARGLIST *Arg);//Закон изменения угла места
 ARGLIST *ElevAngleArg; //Список параметров закона изменения угла места

 //Угол поляризации составляющей внешнего сигнала (в радианах) - polarization
 //angle
 //Вид закона изменения угла поляризационного отношения сигнала во времени
 uint PolAngleLawType;
 double (*PolAngle)(const ARGLIST *Arg);//Закон изменения поляризационного угла
 ARGLIST *PolAngleArg;//Список параметров закона изменения поляризационного угла

 /*     Характеристика АМПЛИТУДЫ составляющей внешнего сигнала     */

 //Амплитудный множитель составляющей внешнего сигнала, определяющий ее превыше-
 //ние над уровнем внутреннего шума приемного канала(ов)
 uint AmplLawType; //Вид закона изменения амплитудного множителя
 double (*AmplLaw)(const ARGLIST *Arg); //Закон изменения амплитудного множителя
 ARGLIST *AmplArg; //Список параметров закона изменения амплитудного множителя

 //Случайный закон изменения во времени амплитуды составляющей внешнего сигнала
 //Casual law of amplitude modification in time
 //Тип случайного закона изменения амплитуды составляющей внешнего сигнала
 uint AmplCasualLawType;
 double (*AmplCasualLaw)(const ARGLIST *Arg);//Случайный закон изменения амплитуды
 //Список параметров случайного закона изменения амплитуды
 ARGLIST *AmplCasualLawArg;

 /*     Характеристика  ФАЗЫ  составляющей  внешнего  сигнала      */

 //Несущая циклическая (угловая) частота составляющей внешнего сигнала (rad/sec)
 //carrier pulsatance   w = 2*PI*f
 double CarPulsatance;

 //Фазовый закон изменения вертикальной составляющей поляризации сигнала
 //во времени (в радианах) - vertical polarization phase law
 uint VertPolPhaseLawType;//Вид закона изменения фазы верт. поляризации сигнала
 //Закон изменения фазы вертикальной составляющей поляризации сигнала
 double (*VertPolPhase)(const ARGLIST *Arg);
 //Список параметров закона изменения фазы верт. поляризации сигнала
 ARGLIST* VertPolPhaseArg;

 //Фазовый закон изменения горизонтальной составляющей поляризации сигнала
 //во времени (в радианах) - horizontal polarization phase law
 uint HorPolPhaseLawType;//Вид закона изменения фазы гориз. поляризации сигнала
 //Закон изменения фазы горизонтальной составляющей поляризации сигнала
 double (*HorPolPhase)(const ARGLIST *Arg);
 //Список параметров закона изменения фазы гориз. поляризации сигнала
 ARGLIST* HorPolPhaseArg;

 //Фазовый закон изменения составляющей внешнего сигнала от времени
 uint PhaseLawType; //Вид закона изменения фазы сигнала от времени
 double (*PhaseLaw)(const ARGLIST *Arg);//Закон изменения фазы сигнала
 ARGLIST *PhaseArg;//Список параметров закона изменения фазы

 //Случайный закон изменения во времени фазы составляющей внешнего сигнала
 //Casual law of phase modification law in time
 uint PhaseCasualLawType;//Тип случайного закона изменения фазы составляющей
 double (*PhaseCasualLaw)(const ARGLIST *Arg);//Случайный закон изменения фазы
 //Список параметров случайного закона изменения фазы
 ARGLIST *PhaseCasualLawArg;
} COMPONENT;

typedef COMPONENT Comp;
//Возможные значения принимаемые полями структуры COMPONENT
//Поле TYPE
const bool SIGNAL = true; //Полезная составляющая внешнего сигнала
const bool JAMMER = false;//Помеховая составляющая внешнего сигнала
//Поле USAGE
//ON  = true; //Компонента внешнего сигнала будет учтена в расчетах
//OFF = false;//Компонента внешнего сигнала не будет учтена в расчетах


//*****************************************************************************
// Объявление класса - ВНЕШНИЙ СИГНАЛ (EXTERNAL SIGNAL) - ExtSignal
//*****************************************************************************
class ExtSignal {
 //Объявление защищенных членов-данных класса ExtSignal
 protected:
  uint Count; //Общее количество всех составляющих внешнего сигнала
  //Указатель на массив структур COMPONENT, описывающих внешний сигнал
  COMPONENT *data;

  //Имя файла с исходными данными по внешнему сигналу
  string fname;
  //Имя файла регистрации возможных ошибок в fname
  string ferrname;
  //Объявление входного файлового потока с данными по ExtSignal
  ifstream fin;
  //Объявление выходного файлового потока с возможными ошибками в fin
  ofstream ferr;

 //---------------------------------------------------
 //Объявление закрытых членов-функций класса ExtSignal
 //---------------------------------------------------
 private:
  //Создание массива data с компонентами внешнего сигнала
  bool CreateArray(const uint size);
  //Аннулирование характеристик одной компоненты внешнего сигнала
  void ResetProperty(uint index);
  //Объявление функций ввода с клавиатуры отдельных характеристик для
  //одной составляющей внешнего сигнала
  void AzimuthAngleInPut(uint index,uint language);  //1.
  void ElevateAngleInPut(uint index,uint language);  //2.
  void PolAngleInPut(uint index,uint language);      //3.
  void AmplLawInPut(uint index,uint language);       //4.
  void AmplCasualLawInPut(uint index,uint language); //5.
  void VertPhasePolInPut(uint index,uint language);  //6.
  void HorPhasePolInPut(uint index,uint language);   //7.
  void PhaseLawInPut(uint index,uint language);      //8.
  void PhaseCasualLawInPut(uint index,uint language);//9.
  void CarPulsatanceInPut(uint index,uint language); //10.
  //Ввод характеристик на одну из компонент внешнего сигнала с терминала
  void ConsoleDataEntry(uint index);

  //Преобразование всех параметров закона из заданной размерности в исходную
  //и наоборот
  ARGLIST* InvertAllLawParam(const LAW* Law, ARGLIST* Arg);
  ARGLIST* ConvertAllLawParam(const LAW* Law, ARGLIST* Arg);

  //Ввод характеристик (инициализация) всех компонент внешнего сигнала из
  //файла данных
  bool FileDataEntry(const char* filename);
  bool GetInPutFilename();
  bool GetNoiseCountFromFile(ulong& CurRow);
  bool GetCompSignalFromFile(uint index, ulong& CurRow);
  bool GetPropertySignalFromFile(string& s ,uint CompSignal, uint PropSignal,
       ulong& CurRow);
  bool GetArgLawFromFile(string& s, LAW* PropLaw, ARGLIST* Arg, uint CompSignal,
       uint PropSignal, ulong& CurRow);

  //Создание таблицы законов изменения во времени азимутального угла, угла
  //места источника, а также амплитуды и фазы (горизонтальной и вертикальной
  //составляющих) поляризации каждой компоненты внешнего сигнала
  void CreateTableOfAngleLaw();
  //Создание таблицы детерминированных законов изменения во времени амплитудной
  //составляющей компоненты внешнего сигнала
  void CreateTableOfAmplLaw();
  //Создание таблицы детерминированных законов изменения во времени фазовой
  //составляющей компоненты внешнего сигнала
  void CreateTableOfPhaseLaw();
  //Создание таблицы случайных законов изменения АМПЛИТУДНОЙ и ФАЗОВОЙ характе-
  //ристики компоненты внешнего сигнала
  void CreateTableOfCasualLaw();
  //Удаление таблицы законов
  void DeleteTable(LAW* Table, uint& Size);

  //Объявление сервисных функций для работы с таблицами законов
  uint AccessibleLaw(const LAW* Table, uint Count);
  bool SetLaw(LAW* Table, uint Count, uint LawType);
  bool ResetLaw(LAW* Table, uint Count, uint LawType);
  bool SetAllLaw(LAW* Table, uint Count);
  bool ResetAllLaw(LAW* Table, uint Count);
  uint GetIndexLaw(const LAW* Table, uint Count, uint Select);
  uint GetIndexLawByType(const LAW* Table, uint Count, uint Type);

 //---------------------------------------------------------
 //Объявление общедоступных членов-функций класса ExtSignal
 //---------------------------------------------------------
 public:
  //Отображение характеристик компоненты внешнего сигнала в режиме Console
  //с возможностью подтверждения выведенных характеристик
  bool ConsoleDataOutPut(uint index, bool dim = TDIM, bool confirm = false);

 //**********  ОБЪЯВЛЕНИЕ КОНСТРУКТОРОВ И ДЕСТРУКТОРА КЛАССА rmatrix  **********
 //Объявление конструктора по умолчанию
 ExtSignal();
 //Объявление конструктора с аргументами
 ExtSignal(COMPONENT *ptr, uint Count);
 ExtSignal(const char* filename);
 //Объявление конструктора копирования
 ExtSignal(ExtSignal& s);
 //Объявление деструктора класса
 ~ExtSignal();

 //***************************************************************************
 //III. ВЫЧИСЛЕНИЕ ПАРАМЕТРОВ ВНЕШНЕГО СИГНАЛА И ЕГО КОМПОНЕНТ
 //Объявление членов-функций класса ExtSignal по вычислению параметров состав-
 //ляющих внешнего сигнала: мгновенных значений амплитуд и фаз, комплексных
 //напряжений с учетом и без учета поляризационных свойств компонент внешнего
 //сигнала, мощностей полезного сигнала, помехового сигнала, отдельной помехи и
 //т.д.
 //***************************************************************************
 public:
  void SetTime(double time); //Установка текущего значения модельного времени
  //Получение случайных значений амплитуды и фазы для всех компонет ExtSignal
  void SetRandNumber();
  //Количество активных помех во внешнем сигнале
  uint Jammers() const {return (Count < 2) ? 0 : Count-1;};
  //Подсчет количества действующих активных помех с Usage == ON
  uint ActiveJammers() const;
  //Определение активности i-ой компоненты внешнего сигнала
  bool Usage(uint Number)
  {if (Number >= Count) return OFF;
   return data[Number].Usage;
  };
  //Получение текущего угла азимута компоненты внешнего сигнала (в радианах)
  double AzimuthAngle(uint Number) const
   {if (Number >= Count) return 0.0;
    return data[Number].AzimuthAngle(data[Number].AzAngleArg);
   };
  //Получение текущего угла места компоненты внешнего сигнала (в радианах)
  double ElevateAngle(uint Number) const
   {if (Number >= Count) return 0.0;
    return data[Number].ElevateAngle(data[Number].ElevAngleArg);
   };
  //Получение текущего угла поляризации компоненты внешнего сигнала (в радианах)
  double PolarAngle(uint Number) const
   {if (Number >= Count) return 0.0;
    return data[Number].PolAngle(data[Number].PolAngleArg);
   };
  //Получение вертикальной составляющей фазы поляризации компоненты внешнего
  //сигнала (в радианах)
  double VCPPhase(uint Number) const
   {if (Number >= Count) return 0.0;
    return data[Number].VertPolPhase(data[Number].VertPolPhaseArg);
   };
  //Получение горизонтальной составляющей фазы поляризации компоненты внешнего
  //сигнала (в радианах)
  double HCPPhase(uint Number) const
   {if (Number >= Count) return 0.0;
    return data[Number].HorPolPhase(data[Number].HorPolPhaseArg);
   };
  //Получение мгновенных значений амплитуд компонент внешнего сигнала
  double AmplSignalComp(uint Number, uint PolarCompType = NONPOLAR) const;
  double AmplSignal(uint PolarCompType = NONPOLAR) const
         {return AmplSignalComp(0,PolarCompType);};
  double VertAmplSignal() const {return AmplSignalComp(0,V_POLAR);};
  double HorAmplSignal() const {return AmplSignalComp(0,H_POLAR);};
  double AmplJammer(uint Number, uint PolarCompType = NONPOLAR) const
         {return AmplSignalComp(Number,PolarCompType);};
  double VertAmplJammer(uint Number) const
         {return AmplJammer(Number,V_POLAR);};
  double HorAmplJammer(uint Number) const
         {return AmplJammer(Number,H_POLAR);};
  //Получение мгновенных значений фаз компонент внешнего сигнала
  double PhaseSignalComp(uint Number, uint PolarCompType = NONPOLAR) const;
  double PhaseSignal(uint PolarCompType = NONPOLAR) const
         {return PhaseSignalComp(0,PolarCompType);};
  double VertPolPhaseSignal() const {return PhaseSignalComp(0,V_POLAR);};
  double HorPolPhaseSignal() const {return PhaseSignalComp(0,H_POLAR);};
  double PhaseJammer(uint Number, uint PolarCompType = NONPOLAR) const
         {return PhaseSignalComp(Number,PolarCompType);};
  double VPolPhaseJammer(uint Number) const
         {return PhaseJammer(Number,V_POLAR);};
  double HPolPhaseJammer(uint Number) const
         {return PhaseJammer(Number,H_POLAR);};
  //Получение мгновенных значений амплитуд и фаз компонент внешнего сигнала без
  //случайной составляющей.
  double DetAmpl(uint Number, uint PolarCompType = NONPOLAR) const;
  double DetPhase(uint Number, uint PolarCompType = NONPOLAR) const;
  //Вычисление комплексных значений напряжений компонент внешнего сигнала
  complex CalcUSignalComp(uint Number, uint PolarCompType = NONPOLAR)
          { return polar(AmplSignalComp(Number,PolarCompType),
                         PhaseSignalComp(Number, PolarCompType));};
  //Расчет комплексных значений напряжения для полезного сигнала
  complex CalcUSignal(uint PolarCompType)
          { return polar(AmplSignalComp(0,PolarCompType),
                         PhaseSignalComp(0, PolarCompType));};
  complex CalcVertPolUSignal() {return polar(AmplSignalComp(0,V_POLAR),
                                             PhaseSignalComp(0, V_POLAR));};
  complex CalcHorPolUSignal()  {return polar(AmplSignalComp(0,H_POLAR),
                                             PhaseSignalComp(0, H_POLAR));};
  complex CalcUSignal() {return CalcUSignal(V_POLAR)+CalcUSignal(H_POLAR);};
 //Расчет комплексных значений напряжения для помех
 complex CalcUJammer(uint Number, uint PolarCompType)
          { return polar(AmplSignalComp(Number, PolarCompType),
                         PhaseSignalComp(Number, PolarCompType));};
  complex CalcVertPolUJammer(uint Number)
          { return polar(AmplSignalComp(Number,V_POLAR),
                         PhaseSignalComp(Number, V_POLAR));};
  complex CalcHorPolUJammer(uint Number)
          { return polar(AmplSignalComp(Number,H_POLAR),
                         PhaseSignalComp(Number, H_POLAR));};
  complex CalcUJammer(uint Number)
          {return CalcUJammer(Number, V_POLAR)+CalcUJammer(Number, H_POLAR);};
  //Расчет комплексных значений напряжения для суммарной помехи
  complex CalcSumUJammer(uint PolarCompType);
  complex CalcVertPolSumUJammer() {return CalcSumUJammer(V_POLAR);};
  complex CalcHorPolSumUJammer()  {return CalcSumUJammer(H_POLAR);};
  complex CalcSumUJammer() {return CalcSumUJammer(V_POLAR) +
                                   CalcSumUJammer(H_POLAR);};
  //Вычисление комплексных значений мощности компонент внешнего сигнала
  double CalcPSignalComp(uint Number, uint PolarCompType = NONPOLAR)
          { return norm(polar(AmplSignalComp(Number,PolarCompType),
                              PhaseSignalComp(Number, PolarCompType)));};
  double CalcPSignalComp(complex UVertPol, complex UHorPol)
          { return norm(UVertPol+UHorPol);};
  double CalcPSignalComp(complex USignalComp) {return norm(USignalComp);};
  //Расчет комплексных значений мощности для полезного сигнала
  double CalcPSignal(uint PolarCompType)
          { return norm(polar(AmplSignalComp(0,PolarCompType),
                              PhaseSignalComp(0, PolarCompType)));};
  double CalcVertPolPSignal() {return norm(polar(AmplSignalComp(0,V_POLAR),
                                             PhaseSignalComp(0, V_POLAR)));};
  double CalcHorPolPSignal() {return norm(polar(AmplSignalComp(0,H_POLAR),
                                             PhaseSignalComp(0, H_POLAR)));};
  double CalcPSignal() {return norm(CalcUSignal());};
  //Расчет комплексных значений мощности для помехи
  double CalcPJammer(uint Number, uint PolarCompType)
          {return norm(polar(AmplSignalComp(Number,PolarCompType),
                             PhaseSignalComp(Number, PolarCompType)));};
  double CalcVertPolPJammer(uint Number)
        {return norm(polar(AmplSignalComp(Number,V_POLAR),
                           PhaseSignalComp(Number, V_POLAR)));};
  double CalcHorPolPJammer(uint Number)
         {return norm(polar(AmplSignalComp(Number,H_POLAR),
                            PhaseSignalComp(Number, H_POLAR)));};
  double CalcPJammer(uint Number) {return norm(CalcUJammer(Number));};
  //Расчет комплексных значений мощности для суммарной помехи
  double CalcSumPJammer(uint PolarCompType);
  double CalcVertPolSumPJammer() {return norm(CalcSumUJammer(V_POLAR));};
  double CalcHorPolSumPJammer() {return norm(CalcSumUJammer(H_POLAR));};
  double CalcSumPJammer()
         {return norm(CalcSumUJammer(V_POLAR)+CalcSumUJammer(H_POLAR));};
  //Расчет отношения сигнал-помеха по мгновенным значениям мощностей полезного
  //сигнала и аддитивной помехи: SJR = 10*lg(Ps/SUM(Pj)) в децибелах
  double SignalJammerRatio();
};
//Завершение объявления класса ExtSignal - внешний сигнал

#endif //ЗАВЕРШЕНИЕ ОБЪЯВЛЕНИЯ ЗАГОЛОВОЧНОГО ФАЙЛА "EXSIGNAL.H"
