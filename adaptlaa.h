//AdaptLAA.h
//Copyright (c) JohnSoft 1999. All rights reserved. Borland C++ 5.01.
//Initial date: Octomber,1,1999. Final date:
//    Объявление класса TSAdaptLAA - статистическая  модель  линейной адаптивной
//антенной решетки с пространственным  адаптивным фильтром и схемой формирования
//основного канала.
//
#if !defined(__ADAPTLAA_H)
#define __ADAPTLAA_H
//******************************************************************************
// Объявление заголовочных файлов
//******************************************************************************
#include "const.h"
//#pragma hdrstop

#include "input.h" //Входное воздействие
#include "mathlib.h" //Библиотека математических классов
#include "weightsum.h" //Весовые сумматоры
#include "RandGen.h" //Генератор случайных чисел


const bool STATISTICAL = true;
const bool ANALYTICAL  = false;
const bool STAT   = true;
const bool ANALYT = false;

//Виды нормировки передаточной функции многолучевой схемы
const uint ABSENT = 0;//нормировка отсутствует
const uint LOCAL  = 1;//нормировка значений мощности для каждого луча отдельно
const uint GLOBAL = 2;//общая нормировка значений мощности для всех лучей

//******************************************************************************
//Предварительное объявление имен классов
//******************************************************************************
class TAnalInSignalProperty;
class TInDataBaseCh;
class TBaseChannel;
class TPreProcessorSpaceFilterBaseCh;
class TAnalPreProcessorSpaceFilterBaseCh;
class TStatPreProcSFilterBaseCh;
class TASpaceFilterBaseCh;
class TSpaceFilterBaseCh;

class TAuxChSpaceRejectFilter;
class TMultiStageAuxChSRF;
class TSingleStageAuxChSRF;

class TMultiBeamFormer;
class TAMultiChSpaceFilter;
class TMultiChSpaceFilter;


//****************************************************************************************
//   Объявление класса - TAnalInSignalProperty - свойства аналитической модели входного
//                сигнала принимаемого адаптивной антенной решеткой
//Данный класс задает признаки "стационарности" и поляризационную составляющую
//принимаемую пространственным адаптивным фильтром для компонент входного сигна-
//ла: ПОЛЕЗНОГО СИГНАЛА, АДДИТИВНОЙ АКТИВНОЙ ПОМЕХИ и ВНУТРЕННЕГО ШУМА.
//Если какая-либо компонента входного воздействия является СТАЦИОНАРНОЙ во  вре-
//мени, то соответсвующая ей корреляционная матрица между каналами LAA, подклю-
//ченными к пространственному адаптивному фильтру, формируется ОДИН РАЗ в момент
//времени t = 0, что сокращает время работы аналитической модели фильтра.
//В НЕСТАЦИОНАРНОМ случае корреляционная матрица для компоненты входного сигнала
//должна формироваться на каждом шаге моделирования. В большинстве случае АДДИ-
//ТИВНАЯ ПОМЕХА и ВНУТРЕННИЙ ШУМ являются СТАЦИОНАРНЫМИ, ПОЛЕЗНЫЙ СИГНАЛ часто
//бывает НЕСТАЦИОНАРНЫМ.
//******************************************************************************
class TAnalInSignalProperty {
 //-----------------------------------------------------------------------------
 // ОБЪЯВЛЕНИЕ ЗАЩИЩЕННЫХ ЧЛЕНОВ-ДАННЫХ КЛАССА TAnalInSignalProperty
 //-----------------------------------------------------------------------------
 protected:
  bool SignalStat;//Признак "стационарности" ПОЛЕЗНОГО СИГНАЛА
  bool JammerStat;//Признак "стационарности" АДДИТИВНОЙ ПОМЕХИ
  bool NoiseStat;//Признак "стационарности" ВНУТРЕННЕГО ШУМА
 //-----------------------------------------------------------------------------
 // I. ОБЪЯВЛЕНИЕ ОБЩЕДОСТУПНЫХ ЧЛЕНОВ-ФУНКЦИЙ КЛАССА TAnalInSignalProperty
 //-----------------------------------------------------------------------------
 public:
 //*****************************************************************************
 // ОБЪЯВЛЕНИЕ КОНСТРУКТОРОВ И ДЕСТРУКТОРА КЛАССА TAnalInSignalProperty
 //*****************************************************************************
 TAnalInSignalProperty();//Объявление конструктора по умолчанию
 //Объявление конструктора с аргументами
 TAnalInSignalProperty(bool lSignal, bool lJammer, bool lNoise);
 //Объявление конструктора копирования
 TAnalInSignalProperty(const TAnalInSignalProperty& Property) {*this = Property;};
 //Объявление деструктора класса
 ~TAnalInSignalProperty();
 //*****************************************************************************
 // ОБЩЕДОСТУПНЫЕ ФУНКЦИИ ДЛЯ РАБОТЫ С КЛАССОМ TAnalInSignalProperty
 //*****************************************************************************
 //Установка свойств аналитической модели входного сигнала
 void Set(bool lSignal, bool lJammer, bool lNoise);
 //Получение свойств класса в виде строкового объекта string
 string& Properties(string &s, string& title, uint indent = 0) const;
 bool GetSignalStat() const {return SignalStat;};
 bool GetJammerStat() const {return JammerStat;};
 bool GetNoiseStat() const {return NoiseStat;};
 //*****************************************************************************
 // ОБЩЕДОСТУПНЫЕ ФУНКЦИИ КЛАССА TAnalInSignalProperty
 // 1. ФУНКЦИИ ВВОДА - ВЫВОДА ПАРАМЕТРОВ
 //*****************************************************************************
 //Ввод параметров объекта из стандартного потока ввода cin
 void Edit();
 //Перегрузка оператора ввода (>>) для ввода параметров класса
 friend istream& operator >>(istream& in, TAnalInSignalProperty& InSignal);
 //Перегрузка оператора вывода (<<) для вывода параметров класса
 friend ostream& operator <<(ostream& out, const TAnalInSignalProperty& InSignal)
 {string s; return out << InSignal.Properties(s,STR_NULL);};
 //Объявление перегруженного оператора присваивания operator =
 void operator =(const TAnalInSignalProperty& InSignal);
 //-----------------------------------------------------------------------------
 // II. ОБЪЯВЛЕНИЕ ЗАЩИЩЕННЫХ ЧЛЕНОВ-ФУНКЦИЙ КЛАССА TAnalInSignalProperty
 //-----------------------------------------------------------------------------
 protected:
  void Reset(); //Начальная установка всех характеристик TAnalInSignalProperty
};
//Конец объявления класса TAnalInSignalProperty
//The end of class TAnalInSignalProperty declaration

//******************************************************************************
//       ОБЪЯВЛЕНИЕ КЛАССОВ МОДЕЛИРУЮЩИХ МНОГОЛУЧЕВУЮ АДАПТИВНУЮ LAA
// С ЛУЧЕОБРАЗУЮЩЕЙ СХЕМОЙ, ПОДКЛЮЧЕННОЙ К ВЫХОДУ АДАПТИВНОГО ПРОСТРАНСТВЕННОГО
//                          МНОГОКАНАЛЬНОГО ФИЛЬТРА
//******************************************************************************
//------------------------------------------------------------------------------
//     МНОГОЛУЧЕВАЯ ЛУЧЕОБРАЗУЮЩАЯ СХЕМА АДАПТИВНОЙ LAA - TMultiBeamFormer
//------------------------------------------------------------------------------
//   Многолучевая схема реализуется классом TMultiBeamFormer. Класс позволяет
//формировать m ЛУЧЕЙ с произвольным пространственным положением -90 до +90 гра-
//дусов из n КАНАЛОВ LAA, подключенных к многолучевой схеме. К схеме может быть
//подключено от 1 до ВСЕХ каналов LAA. Все подключенные к многолучевой схеме ка-
//налы LAA участвуют в образовании ВСЕХ заданных направлений формирования ЛУЧЕЙ.
//Все лучи имеют разные пространственные положения. Ширина лучей определяется
//количеством каналов LAA, подключенных к MultiBeamFormer и их пространственным
//положением. Номера каналов LAA, подключенных к MultiBeamFormer, произвольные.
//   Многолучевая схема реализована из m параллельных весовых сумматоров (по ко-
//личеству лучей). В  качестве  весов для k-го сумматора (1 <= k <= m) выступают
//фазосдвигающие  коэффициенты для каждого канала LAA подключенного к MultiBeam-
//Former, которые формируют заданные углы сканирования. После перемножения комп-
//лексных напряжений приемных каналов LAA, подключенных к MultiBeamFormer, на
//соответствующие каждому каналу фазосдвигающие коэффициенты для k-го сумматора,
//полученные  произведения суммируются на общем сумматоре, выходное значение ко-
//торого определяет комплексные значения напряжений аддитивной помехи, полезного
//сигнала и внутреннего шума k-го луча.
//  Таким образом, для формирования m лучей с использованием n каналов LAA, фор-
//мируется ЛУЧЕОБРАЗУЮЩАЯ МАТРИЦА B размерностью n x m. В каждом столбце матри-
//цы B, хранятся фазосдвигающие коэффициенты для формирования одного из лучей.
//   Коэффициенты PHI(i,j) определяют фазовый сдвиг в i-ом канале LAA, подключен-
//ном к BFC, для формирования j-го луча по азимуту или по углу места LAA. Фазо-
//сдвигающий коэффициент вычисляется по следующей формуле:
// PHI(i,j) = 2*PI*LV[i]*sin(Beam[j]), где
// Beam[j] - направление сканирования j-го луча многолучевой схемы (1<=j<= m);
// LV[i] - относительное расстояние между 0 и i-м каналом антенной решетки, под-
// ключенным к многолучевой схеме (1<=i<=N).
//   Лучеобразующая  матрица  B для m лучей из n каналов LAA, подключенных к ней
//имеет следующую структуру (n = 7, m = 3):
//Beam:   1   2   3    Ch
//     | b11 b12 b13 | 1
//     | b21 b22 b23 | 2  bij - фазосдвигающий множитель для формирования j-го
//     | b31 b32 b33 | 3        луча i-ым каналом LAA, подключенным к лучеобра-
// B = | b41 b42 b43 | 4        зующей схеме.
//     | b51 b52 b53 | 5
//     | b61 b62 b63 | 6
//     | b71 b72 b73 | 7
//
//------------------------------------------------------------------------------
//******************************************************************************
//Объявление класса - TMultiBeamFormer - многолучевая лучеобразующая схема LAA
//******************************************************************************
class TMultiBeamFormer {
 //-----------------------------------------------------------------------------
 // ОБЪЯВЛЕНИЕ ЗАЩИЩЕННЫХ ЧЛЕНОВ-ДАННЫХ КЛАССА TMultiBeamFormer
 //-----------------------------------------------------------------------------
 protected:
  TInputSignal* pLAA;//Указатель на LAA, к которой подключается MultiBeamFormer
  ivector Channel;//Номера каналов LAA, подключенных к MultiBeamFormer
  bool lUsedAllCh;//Признак подключения всех каналов LAA к MultiBeamFormer
  rvector Beam;//Пространственные положения основных лепестков лучей (в градусах)
  cmatrix B;//Лучеобразующая матрица
 //-----------------------------------------------------------------------------
 // I. ОБЪЯВЛЕНИЕ ОБЩЕДОСТУПНЫХ ЧЛЕНОВ-ФУНКЦИЙ КЛАССА TMultiBeamFormer
 //-----------------------------------------------------------------------------
 public:
 //*****************************************************************************
 // ОБЪЯВЛЕНИЕ КОНСТРУКТОРОВ И ДЕСТРУКТОРА КЛАССА TMultiBeamFormer
 //*****************************************************************************
 //Объявление конструктора по умолчанию
 TMultiBeamFormer() {_Reset();};
 //Объявление конструкторов с аргументами
 TMultiBeamFormer(TInputSignal* InSignal, const ivector& Index = ivector(),
                  const rvector& Angle = rvector())
  {Set(InSignal,Index,Angle);};
 TMultiBeamFormer(TInputSignal* InSignal, const ivector& Index)
  {rvector Angle; Set(InSignal,Index,Angle);};
 TMultiBeamFormer(TInputSignal* InSignal, const rvector& Angle)
  {ivector Index; Set(InSignal,Index,Angle);};
 //Объявление конструктора копирования
 TMultiBeamFormer(const TMultiBeamFormer& MultiBeam) {*this = MultiBeam;};
 //Объявление деструктора класса
 ~TMultiBeamFormer() {_Reset();};
 //*****************************************************************************
 // ОБЩЕДОСТУПНЫЕ ФУНКЦИИ ДЛЯ РАБОТЫ С КЛАССОМ TMultiBeamFormer
 //*****************************************************************************
 //-----------------------------------------------------------------------------
 //Установка свойств класса
 //-----------------------------------------------------------------------------
 bool Set(TInputSignal* InSignal, const ivector& Index = ivector(),
          const rvector& Angle = rvector());
 bool SetLAACh(const ivector& Index = ivector());
 bool SetBeams(const rvector& Angle = rvector());
 void Set(const TMultiBeamFormer& MultiBeam) {*this = MultiBeam;};
 //-----------------------------------------------------------------------------
 //Получение характеристик (свойств) класса
 //-----------------------------------------------------------------------------
 //Проверка работоспособности многолучевой схемы
 bool IsReady() const {return (pLAA != NULL) ? true : false;};
 //Получение указателя на LAA
 TInputSignal* GetLAAPtr() const {return pLAA;};
 //Расположение LAA относительно поверхности Земли
 bool DispLAA() const {return (IsReady()) ? pLAA->GetDisp() : EQUATOR;};
 //Количество каналов LAA, подключенных к MultiBeamFormer
 ulong GetLAACh() const {return B.Rows();};
 //Количество лучей (направлений сканирования) MultiBeamFormer
 ulong GetBeams() const {return B.Cols();};
 bool GetLAACh(ivector& Index) const;
 bool GetBeams(rvector& Angle) const;
 bool IsConnectedAllLAACh() const {return lUsedAllCh;}
 //Получение коэффициентов лучеобразующей матрицы
 bool GetBeamFactors(cmatrix& A) const {A = B; return (IsReady()) ? true : false;};
 //Получение свойств класса в виде строкового объекта string
 string& Properties(string& s, string& title, ulong MaxCh = 0L, ulong MaxBeam = 0L,
                    uint indent = 0) const;
 string& GetLAACh(string& s, ulong max = 0L, uint indent = 0) const;
 string& GetBeams(string& s, ulong max = 0L, uint indent = 0) const;
 //-----------------------------------------------------------------------------
 //Передаточная функция MultiBeamFormer (диаграмма направленности)
 //Вычисление диаграмм направленностей МНОГОЛУЧЕВОЙ СХЕМЫ в заданном секторе в
 //плоскости расположения LAA относительно поверхности Земли (по азимуту или
 //углу места)
 bool TFunc(rmatrix& P, double InitAngle = -90, double EndAngle = 90, double Step = 0.1,
            uint NormType = GLOBAL, bool dB = ON, double MindB = -60.0) const;
 bool TFunc(rmatrix& P, const TEvenCloseInterval& Sector, uint NormType = GLOBAL,
            bool dB = ON, double MindB = -60.0) const;
 bool TFunc(rmatrix& P, const rvector& Angle, uint NormType = GLOBAL,
            bool dB = ON, double MindB = -60.0) const;
 //-----------------------------------------------------------------------------
 //Построение огибающей диаграмм направленностей МНОГОЛУЧЕВОЙ СХЕМЫ в заданном
 //секторе в плоскости расположения LAA относительно поверхности Земли
 //(по азимуту или углу места)
 bool Envelope(rvector& P, const TEvenCloseInterval& Sector, bool Norm = ON,
               bool dB = ON, double MindB = -60.0) const;
 //Вычисление ширины лучей MultiBeamFormer по заданному уровню мощности в градусах
 //bool BeamWidth(rvector& Result, double PowerLevel = 0.5, double Prec = 0.01);
 //*****************************************************************************
 // ОБЩЕДОСТУПНЫЕ ФУНКЦИИ РАСЧЕТА ВЫХОДНЫХ ЗНАЧЕНИЙ TMultiBeamFormer
 //*****************************************************************************
 bool TFunc(const cvector& In, cvector& Out) const;
 bool TFunc(const cmatrix& In, cvector& Out) const;
 bool TFunc(const cvector& In, rvector& P) const;
 bool TFunc(const cmatrix& In, rvector& P) const;

 //OVERLOAD THE CALL FUNCTION OPERATOR ()
 //Перегрузка оператора вызова функции ()
 bool operator() (const cvector& In, cvector& Out) const {return TFunc(In,Out);};
 bool operator() (const cmatrix& In, cvector& Out) const {return TFunc(In,Out);};
 bool operator() (const cvector& In, rvector& P) const {return TFunc(In,P);};
 bool operator() (const cmatrix& In, rvector& P) const {return TFunc(In,P);};

 //Расчет компонент входного воздействия на выходе MultiBeamFormer для
 //ЧИСЛЕННОГО/СТАТИСТИЧЕСКОГО МОДЕЛИРОВАНИЯ (MultiBeamFormer подключена к выходу LAA)
 bool Signal(cvector& Us, bool PolarType) const;
 bool Noise(cvector& Un, bool PolarType) const;
 bool Jammers(cvector& Ujam, bool PolarType) const;
 bool Jammers(cmatrix& Ujam, bool PolarType) const;

 bool Psignal(rvector& P, bool PolarType) const;
 bool Pnoise(rvector& P, bool PolarType) const;
 bool Pjams(rvector& P, bool PolarType) const;
 bool Pjams(rmatrix& P, bool PolarType) const;

 //*****************************************************************************
 // ОБЩЕДОСТУПНЫЕ ФУНКЦИИ ВВОДА - ВЫВОДА ПАРАМЕТРОВ TMultiBeamFormer
 //*****************************************************************************
 //Ввод параметров объекта из стандартного потока ввода cin
 bool Edit();
 //Ввод индексов каналов LAA, подключаемых к MultiBeamFormer из потока ввода cin
 bool EditLAACh(bool Confirm = ON);
 //Ввод направлений сканирования MultiBeamFormer из потока ввода cin
 bool EditBeams(bool Confirm = ON);

 //Перегрузка оператора вывода (<<) для вывода параметров класса
 friend ostream& operator <<(ostream& out, const TMultiBeamFormer& Obj)
 {string s; return out << Obj.Properties(s,STR_NULL);};
 //Объявление перегруженного оператора присваивания operator =
 void operator =(const TMultiBeamFormer& MultiBeam);
 //-----------------------------------------------------------------------------
 // II. ОБЪЯВЛЕНИЕ ЗАЩИЩЕННЫХ ЧЛЕНОВ-ФУНКЦИЙ КЛАССА TMultiBeamFormer
 //-----------------------------------------------------------------------------
 protected:
  void _Reset(); //Начальная установка всех характеристик класса
  //Установка индексов каналов LAA, подключаемых к MultiBeamFormer
  void _SetChannel(const ivector& Index);
  //Установка направлений сканирования MultiBeamFormer
  void _SetBeam(const rvector& Angle);
  //Расчет коэффициентов лучеобразующей схемы - MultiBeamFormer - матрица B
  void _CalcMultiBeamFactors();
};
//Конец объявления класса TMultiBeamFormer
//The end of class TMultiBeamFormer declaration

//****************************************************************************************
//                   Объявление класса TAMultiChSpaceFilter
//Численная модель адаптивной LAA с пространственным адаптивным N-канальным
//фильтром и многолучевой лучеобразующей схемой. Конфигурация адаптивной LAA:
//1. LAA - линейная антенная решетка;
//2. Адаптивный пространственный фильтр с N входами и N выходами;
//3. Схема накопления сигнала - многолучевая лучеобразующая схема, подлюченная к
//   выходам N-канального адаптивного пространственного фильтра.
//Класс наследует свойства классов TMultiBeamFormer и TAnalInSignalProperty.
//Класс формирует воздействие на входе пространственного фильтра, подключенного
//к LAA, в виде корреляционных матриц полезного сигнала Rss, аддитивной активной
//помехи Rjj, внутренненго шума Rnn и суммарной матрицы R компонент входного сиг-
//нала. Матрица R используется для настройки весовых коэффициентов пространствен-
//ного адаптивного фильтра размерностью N x N. Размерность фильтра N определяется
//пользователем и не может превысить количества каналов LAA. Данные о размерности
//содержаться в классе TMultiBeamFormer. В этом классе определяются количество и
//номера каналов LAA, подключенные к пространственному фильтру, а также задаются
//параметры многолучевой схемы: количество формируемых лучей и их пространствен-
//ные положения (по азимуту или углу места) в зависимости от расположения LAA
//относительно поверхности Земли. Таким образом, класс TMultiBeamFormer служит
//одновременно коммутатором каналов LAA. От количества подключенных каналов будет
//зависеть размерность адаптивного пространственного фильтра и ширина лучей, фор-
//мируемых лучеобразующей схемой.
//Класс TAnalInSignalProperty задает признаки "стационарности" полезного сигнала,
//аддитивной активной помехи и внутреннего шума. Для стационарной во времени ком-
//понента входного воздействия ее корреляционная матрица формируется только на
//первом шаге моделирования (T = 0), что позволяет ускорить моделирование. Для
//нестационарной во времени компоненты ее корреляционная матрица формируется на
//каждом шаге моделирования. Как правило, аддитивная активная помеха и внутрен-
//ний шум стационарны во времени, а полезный сигнал - нестационарен. Пользователь
//перед началом моделирования должен сам выбрать опции для каждой компоненты.
//Для построения передаточной функции адаптивной LAA после ее адаптации исполь-
//зуется вектор входного воздействия (для ускорения работы). Класс позволяет вы-
//числять передаточные функции пространственного фильтра и многолучевой лучеоб-
//разующей схемы по отдельности, рассчитывать мощности на входе и выходе адаптив-
//ной LAA или фильтра, получать значения весовых коэффициентов, корреляционные
//матрицы компонент входного сигнала или их сумму и т.д.
//ЗАМЕЧАНИЕ. При расчете мощности на входе и выходе пространственного фильтра
//возвращается вектор мощностей в каждом из каналов LAA, используемых адаптивной
//LAA. При этом значения берутся с входов/выходов самого фильтра без учета много-
//лучевой схемы. При расчете мощностей компонент входного воздействия на входе и
//выходе адаптивной LAA возвращается вектор мощности для каждого луча формируемо-
//го MultiBeamFormer. При этом выходные мощности берутся с выхода адаптивной LAA,
//т.е. с віхода многолучевой схемы. Входные мощности адаптивной LAA, рассчитываются
//также на выходе MultiBeamFormer, но без учета прохождения сигнала через прост-
//ранственный адаптивный фильтр. Для оценки результатов моделирования всей систе-
//мы применяется оценка мощностей на входе и выходе адаптивной LAA, а для оценки
//работы только фильтра - мощности на входе и выходе непосредственно фильтра.
//Для моделирования работы самого фильтра в класс вводится указатель на базовый
//класс из заголовочного файла "weightsum.h" - TMultiInMultiOutWeightSummator.
//Это дает возможность пользователю использовать в качестве пространственного
//фильтра конфигурации N x N разные численный модели весовых сумматоров с N вхо-
//дами и N выходами. Данные весовые сумматоры могут отличаться друг от друга по
//конфигурации (одноступенчатые и многоступенчатые), по алгоритмам настройки ве-
//совых коэффициентов (алгоритмам адаптации), по схемам стабилизации динамических
//параметров фильтров (наличию или отсутствию, типу схемы стабилизации). Все эти
//весовые сумматоры должно объединять только одно: они должны быть с N ВХОДАМИ и
//N ВЫХОДАМИ. Размерность сумматора определяется автоматически при выборе его типа
//количеством каналов LAA, подключенных к MultiBeamFormer.
//Такая реализация класса дает возможность исследовать и реализовывать адаптивные
//LAA как по их внутренним параметрам (изменение значений весовых коэффициентов в
//зависимости от складывающейся помеховой ситуации), так и по их структуре, алго-
//ритмам настройки весовых коэффициентов и стабилизации их динамических парамет-
//ров. Напомним, что работа пространственного фильтра заключается в формировании
//нулей  диаграммы направленности (передаточной функции) в направлении на источ-
//ники помех. Размерность фильтра определяет максимальное число активных (шумовых)
//помех, которые могут быть подавлены адаптивной LAA. Считается, что полезный
//сигнала располагается в главном луче (лепестке) LAA.
//Моделирование численной модели адаптивной LAA с пространственным фильтром N x N
//и многолучевой схемой производится за одну реализацию. В  начале моделирования
//весовые коэффициенты адаптивного фильтра ненастроены и он  пропускает  входное
//воздействия без изменения. По окончании моделирования можно построить  переда-
//точную характеристику  адаптивной LAA, которая будет зависеть от многих факто-
//ров: от входного воздействия, конфигурации адаптивной решетки и пространствен-
//ного фильтра, алгоритма  настройки и схем стабилизации фильтра, внутренних па-
//раметров фильтра и т.д.
//****************************************************************************************
class TAMultiChSpaceFilter:
 public TMultiBeamFormer, public TAnalInSignalProperty {
//----------------------------------------------------------------------------------------
// ОБЪЯВЛЕНИЕ ЗАЩИЩЕННЫХ ЧЛЕНОВ-ДАННЫХ КЛАССА TAMultiChSpaceFilter
//----------------------------------------------------------------------------------------
protected:
 //Поляризационная составляющая сигнала, принимаемого фильтром
 bool Polar;
 //Указатель на базовый абстрактный класс весового сумматора N x N
 TMultiInMultiOutWeightSummator* Filter;//Пространственный адаптивный фильтр
 //Корреляционные матрицы полезного сигнала, аддитивной помехи и внутреннего
 //шума на входе фильтра
 cmatrix Rss;//Корреляционная матрица ПОЛЕЗНОГО СИГНАЛА
 cmatrix Rjj;//Корреляционная матрица АДДИТИВНОЙ ПОМЕХИ
 cmatrix Rnn;//Корреляционная матрица ВНУТРЕННЕГО ШУМА
 //Суммарная матрица компонент входного воздействия на входе пространственного
 //адаптивного фильтра: Rss+Rjj+Rnn
 cmatrix R;
//------------------------------------------------------------------------------
// I. ОБЪЯВЛЕНИЕ ОБЩЕДОСТУПНЫХ ЧЛЕНОВ-ФУНКЦИЙ КЛАССА TAMultiChSpaceFilter
//------------------------------------------------------------------------------
public:
//******************************************************************************
// ОБЪЯВЛЕНИЕ КОНСТРУКТОРОВ И ДЕСТРУКТОРА КЛАССА TAMultiChSpaceFilter
//******************************************************************************
//Объявление конструктора по умолчанию
TAMultiChSpaceFilter() : TMultiBeamFormer(), TAnalInSignalProperty()
{Filter = NULL; _Reset();};
//Объявление конструктора с аргументами
TAMultiChSpaceFilter(TInputSignal* pLAA, const ivector& LAACh = ivector(),
 const rvector& Beams = rvector(), uint SF_Type = MS_SDM_MIMO_WSum,
 rvector& SF_Param = rvector(), bool PolarType = VP_SIGNAL) :
 TMultiBeamFormer(pLAA,LAACh,Beams), TAnalInSignalProperty()
 {Filter = NULL; Polar = VP_SIGNAL;
  if (IsBeamFormerReady()) SetSFilter(SF_Type,SF_Param);
 };
//Объявление конструктора копирования
TAMultiChSpaceFilter(TAMultiChSpaceFilter& SFilter) {*this = SFilter;};
//Объявление деструктора класса
~TAMultiChSpaceFilter() {_Reset();};
//------------------------------------------------------------------------------
//Установка свойств класса
//------------------------------------------------------------------------------
bool Set(TInputSignal* pLAA, const ivector& LAACh = ivector(),
         const rvector& Beams = rvector(), uint SF_Type = MS_SDM_MIMO_WSum,
         rvector& SF_Param = rvector(), bool PolarType = VP_SIGNAL)
{if (SetBeamFormer(pLAA,LAACh,Beams))
 {SetPolar(PolarType); return SetSFilter(SF_Type,SF_Param);}
 _Reset();//Сброс всех параметров адаптивной LAA
 return false;
};
//Установка свойств лучеобразующей схемы
bool SetBeamFormer(TInputSignal* pLAA, const ivector& LAACh = ivector(),
     const rvector& Beams = rvector());
//Установка свойств пространственного фильтра
bool SetSFilter(uint SF_Type, rvector& SF_Param = rvector());
//Установка свойств "стационарности" входного воздействия
void SetInSignalStat(bool lSignal, bool lJammer, bool lNoise)
{TAnalInSignalProperty::Set(lSignal,lJammer,lNoise);};
//Установка поляризационной составляющей сигнала, принимаемого фильтром
bool SetPolar(bool PolarType) {return Polar = PolarType;};

//------------------------------------------------------------------------------
// ПОЛУЧЕНИЕ СВОЙСТВА (ХАРАКТЕРИСТИК) КЛАССА TAMultiChSpaceFilter
//------------------------------------------------------------------------------
//Определение РАБОТОСПОСОБНОСТИ лучеобразующей схемы
bool IsBeamFormerReady() const {return TMultiBeamFormer::IsReady();};
//Определение РАБОТОСПОСОБНОСТИ пространственного фильтра
bool IsSpaceFilterReady() const
{return ((Filter != NULL) && (Filter->IsReady())) ? ON : OFF;};
//Определение РАБОТОСПОСОБНОСТИ всей АДАПТИВНОЙ Linear Antenna Array
bool IsReady() const
{return ((IsBeamFormerReady()) && (IsSpaceFilterReady())) ? ON : OFF;};
//Размерность фильтра - количество каналов LAA, подключенных к фильтру
ulong Size() const {return TMultiBeamFormer::GetLAACh();};
//Получение поляризационной составляющей сигнала
bool GetPolar() const {return Polar;};
//Получение коэффициентов весового сумматора
cmatrix& GetW(cmatrix& W) const
{if (!IsReady()) W.Set(0L,0L,false);
 else Filter->GetW(W);
 return W;
};
//Получение оптимальных коэффициентов весового сумматора по входному воздействию
cmatrix& GetOptW(cmatrix& OptW, const cmatrix& R) const
{if (!IsReady()) OptW.Set(0L,0L,false);
 else Filter->GetOptW(OptW,R);
 return OptW;
};
//Получение типа пространственного фильтра
uint SFilterType() const {return (IsSpaceFilterReady()) ? Filter->Type() : 0;};
//Получение параметров пространственного фильтра
rvector& SFilterParam(rvector& Arg) const
{if (!IsSpaceFilterReady()) {Arg.Set(0L,ROW,false); return Arg;}
 return Filter->Param(Arg);
}

//------------------------------------------------------------------------------
//  Корреляционные матрицы компонент внешнего воздействия на входе AdaptLAA
//      /вход адаптивного пространственного фильтра SpaceFilter/
//------------------------------------------------------------------------------
//Входная корреляционная матрица ПОЛЕЗНОГО СИГНАЛА /вход AdaptLAA/
cmatrix& GetInRss(cmatrix& A) const {A = Rss; return A;};
//Входная корреляционная матрица АДДИТИВНОЙ АКТИВНОЙ ПОМЕХИ /вход AdaptLAA/
cmatrix& GetInRjj(cmatrix& A) const {A = Rjj; return A;};
//Входная корреляционная матрица ВНУТРЕННЕГО ШУМА /вход AdaptLAA/
cmatrix& GetInRnn(cmatrix& A) const {A = Rnn; return A;};
//Входная корреляционная матрица входного сигнала: Rss+Rjj+Rnn
cmatrix& GetInR(cmatrix& A) const {A = Rss+Rjj+Rnn; return A;};
//Входная корреляционная матрица компонент ВХОДНОГО ВОЗДЕЙСТВИЯ
cmatrix& GetInR(cmatrix& A, bool lSignal, bool lJammer, bool lNoise) const
{A.Set(0L,0L,false);
 if (!IsReady()) return A;
 if (lSignal) A = Rss;
 if (lJammer) A = (A.IsZeroSize()) ? Rjj : A + Rjj;
 if (lNoise)  A = (A.IsZeroSize()) ? Rnn : A + Rnn;
 return A;
};
//------------------------------------------------------------------------------
//   Корреляционные матрицы компонент внешнего воздействия на выходе AdaptLAA
//      /выход адаптивного пространственного фильтра SpaceFilter/
//------------------------------------------------------------------------------
//Выходная корреляционная матрица ПОЛЕЗНОГО СИГНАЛА /выход SpaceFilter/
cmatrix& GetOutRss(cmatrix& A) const
{if (!IsReady()) {A.Set(0L,0L,false); return A;}
 A = Rss; //Формируем матрицу полезного сигнала на входе фильтра
 Filter->TF_WSum(A);//Вычисляем матрицу полезного сигнала на выходе фильтра
 return A;
};
//Выходная корреляционная матрица АДДИТИВНОЙ АКТИВНОЙ ПОМЕХИ /выход SpaceFilter/
cmatrix& GetOutRjj(cmatrix& A) const
{if (!IsReady()) {A.Set(0L,0L,false); return A;}
 A = Rjj;//Формируем матрицу активной помехи на входе фильтра
 Filter->TF_WSum(A);//Вычисляем матрицу активной помехи на выходе фильтра
 return A;
};
//Выходная корреляционная матрица ВНУТРЕННЕГО ШУМА /выход SpaceFilter/
cmatrix& GetOutRnn(cmatrix& A) const
{if (!IsReady()) {A.Set(0L,0L,false); return A;}
 A = Rnn;//Формируем матрицу внутреннего шума на входе фильтра
 Filter->TF_WSum(A);//Вычисляем матрицу внутреннего шума на выходе фильтра
 return A;
};
//Выходная корреляционная матрица входного воздействия: Rss+Rjj+Rnn
cmatrix& GetOutR(cmatrix& A) const
{if (!IsReady()) {A.Set(0L,0L,false); return A;}
 GetInR(A);//Формируем матрицу входного воздействия на входе фильтра
 Filter->TF_WSum(A);//Вычисляем матрицу на выходе фильтра
 return A;
};
//Выходная корреляционная матрица компонент ВХОДНОГО ВОЗДЕЙСТВИЯ
cmatrix& GetOutR(cmatrix& A, bool lSignal, bool lJammer, bool lNoise) const
{GetInR(A,lSignal,lJammer,lNoise);//Формируем матрицу входного воздействия
 Filter->TF_WSum(A);//Матрица компонент входного воздействия на выходе фильтра
 return A;
};
//---------------  ПЕРЕДАТОЧНЫЕ ФУНКЦИИ АДАПТИВНОЙ LAA  ------------------------
//Передаточная функция (Transfer function) адаптивной LAA /выход MultiBeamFormer/
bool TFunc(rmatrix& P, const TEvenCloseInterval& Sector, uint NormType = GLOBAL,
           bool dB = ON, double MindB = -60.0) const;
//Передаточная функция (Transfer function) адаптивного пространственного фильтра
//                      /выход SpaceFilter/
bool TF_SFilter(rmatrix& P, const TEvenCloseInterval& Sector, uint NormType = GLOBAL,
                bool dB = ON, double MindB = -60.0) const;
//Передаточная функция (Transfer function) лучеобразующей схемы без SpaceFilter
//             /выход MultiBeamformer - прямое подключение к LAA/
bool TF_BeamFormer(rmatrix& P, const TEvenCloseInterval& Sector, uint NormType = GLOBAL,
                   bool dB = ON, double MindB = -60.0) const
{return TMultiBeamFormer::TFunc(P,Sector,NormType,dB,MindB);};
bool TF_BeamFormer(rvector& P, const cmatrix& R) const
{return TMultiBeamFormer::TFunc(R,P);};
//Потенциальная передаточная функция адаптивной LAA при заданном уровне активной
//помехи и внутреннего шума (помеховой ситуации)
bool TF_Potent(rmatrix& P, double Time, const TEvenCloseInterval& Sector,
     uint NormType = GLOBAL, bool dB = ON, double MindB = -60.0);
//------------------------------------------------------------------------------
//           ФУНКЦИИ МОДЕЛИРОВАНИЯ КЛАССА TAMultiChSpaceFilter
//------------------------------------------------------------------------------
//Начальная установка параметров AdaptLAA при моделировании: начальная установ-
//ка весовых коэффициентов пространственного фильтра
bool SimulateInit() {return (IsReady()) ? Filter->SimulateInit() : OFF;};
//Расчет корреляционных матриц компонент входного воздействия при моделировании
bool Update(double Time);
//Настройка весовых коэффициентов пространственного фильтра
bool Tuning() {return (IsReady()) ? Filter->Tuning(R) : OFF;};
//Установка коэффициентов весового сумматора
bool SetW(cmatrix& W) {return (IsReady()) ? Filter->SetW(W) : OFF;};
//------------------------------------------------------------------------------
//               РАСЧЕТ МОЩНОСТЕЙ НА ВХОДЕ адаптивной LAA
//   /на выходе MultiBeamFormer, подключенной к LAA без SpaceFilter/
//------------------------------------------------------------------------------
//Мощность ПОЛЕЗНОГО СИГНАЛА на входе адаптивной LAA
bool InPSignal(rvector& Ps) const {return TMultiBeamFormer::TFunc(Rss,Ps);};
//Мощность АКТИВНОЙ АДДИТИВНОЙ ПОМЕХИ на входе адаптивной LAA
bool InPJammer(rvector& Pjam) const {return TMultiBeamFormer::TFunc(Rjj,Pjam);};
//Мощность ВНУТРЕННЕГО ШУМА на входе адаптивной LAA
bool InPNoise(rvector& Pn) const {return TMultiBeamFormer::TFunc(Rnn,Pn);};
//Суммарная мощность на входе адаптивной LAA
bool InPSum(rvector& Psum) const {return TMultiBeamFormer::TFunc(R,Psum);};
bool InPSum(rvector& Psum, bool lSignal, bool lJammer, bool lNoise) const
{if (!IsReady()) {Psum.Set(0L,ROW,false); return false;}
 //Формирование матрицы входного воздействия
 cmatrix R; GetInR(R,lSignal,lJammer,lNoise);
 return TMultiBeamFormer::TFunc(R,Psum);
};

//------------------------------------------------------------------------------
//               РАСЧЕТ МОЩНОСТЕЙ НА ВЫХОДЕ адаптивной LAA
//            /выход MultiBeamFormer с подключением SpaceFilter/
//------------------------------------------------------------------------------
//Мощность ПОЛЕЗНОГО СИГНАЛА на выходе адаптивной LAA
bool OutPSignal(rvector& Ps) const
{if (!IsReady()) {Ps.Set(0L,ROW,false); return false;}
 cmatrix A; GetOutRss(A);//Формирование выходной матрицы полезного сигнала
 return TMultiBeamFormer::TFunc(A,Ps);
};
//Мощность АКТИВНОЙ ПОМЕХИ на выходе адаптивной LAA
bool OutPJammer(rvector& Pjam) const
{if (!IsReady()) {Pjam.Set(0L,ROW,false); return false;}
 cmatrix A; GetOutRjj(A);//Формирование выходной матрицы активной помехи
 return TMultiBeamFormer::TFunc(A,Pjam);
};
//Мощность ВНУТРЕННЕГО ШУМА на выходе адаптивной LAA
bool OutPNoise(rvector& Pn) const
{if (!IsReady()) {Pn.Set(0L,ROW,false); return false;}
 cmatrix A; GetOutRnn(A);//Формирование выходной матрицы внутреннего шума
 return TMultiBeamFormer::TFunc(A,Pn);
};
//Суммарная мощность на выходе адаптивной LAA
bool OutPSum(rvector& Psum) const
{if (!IsReady()) {Psum.Set(0L,ROW,false); return false;}
 cmatrix A; GetOutR(A);//Формирование выходной матрицы воздействия
 return TMultiBeamFormer::TFunc(A,Psum);
};
bool OutPSum(rvector& Psum, bool lSignal, bool lJammer, bool lNoise) const
{if (!IsReady()) {Psum.Set(0L,ROW,false); return false;}
 //Формирование выходной матрицы воздействия
 cmatrix A; GetOutR(A,lSignal,lJammer,lNoise);
 return TMultiBeamFormer::TFunc(A,Psum);
};

//------------------------------------------------------------------------------
//         РАСЧЕТ МОЩНОСТЕЙ НА ВХОДЕ пространственного фильтра
//        /мощности в каналах LAA, подключенных к SpaceFilter/
//------------------------------------------------------------------------------
//Мощность ПОЛЕЗНОГО СИГНАЛА на входе пространственного фильтра
bool InPs_SFilter(rvector& Ps) const;
//Мощность АКТИВНОЙ АДДИТИВНОЙ ПОМЕХИ на входе пространственного фильтра
bool InPjam_SFilter(rvector& Pjam) const;
//Мощность ВНУТРЕННЕГО ШУМА на входе пространственного фильтра
bool InPn_SFilter(rvector& Pn) const;
//------------------------------------------------------------------------------
//          РАСЧЕТ МОЩНОСТЕЙ НА ВЫХОДЕ пространственного фильтра
//                         /выход SpaceFilter/
//------------------------------------------------------------------------------
//Мощность ПОЛЕЗНОГО СИГНАЛА на выходе пространственного фильтра
bool OutPs_SFilter(rvector& Ps) const
{if (!IsReady()) {Ps.Set(0L,ROW,false); return false;}
 cmatrix A(Rss); Filter->TF_WSum(Ps,A);
 return true;
};
//Мощность АКТИВНОЙ ПОМЕХИ на выходе пространственного фильтра
bool OutPjam_SFilter(rvector& Pjam) const
{if (!IsReady()) {Pjam.Set(0L,ROW,false); return false;}
 cmatrix A(Rjj); Filter->TF_WSum(Pjam,A);
 return true;
};
//Мощность ВНУТРЕННЕГО ШУМА на выходе пространственного фильтра
bool OutPn_SFilter(rvector& Pn) const
{if (!IsReady()) {Pn.Set(0L,ROW,false); return false;}
 cmatrix A(Rnn); Filter->TF_WSum(Pn,A);
 return true;
};

//------------------------------------------------------------------------------
//       РАСЧЕТ МОЩНОСТЕЙ НА ВСЕХ ВХОДАХ/ВЫХОДАХ пространственного фильтра
//------------------------------------------------------------------------------
//Мощность ПОЛЕЗНОГО СИГНАЛА на входах/выходах пространственного фильтра
rmatrix& Ps_SFilter(rmatrix& P) const
{if (!IsReady()) {P.Set(0,0,false); return P;}
 cmatrix A(Rss); return Filter->Powers(P,A);
};
//Мощность АКТИВНОЙ ПОМЕХИ на входах/выходах пространственного фильтра
rmatrix& Pjam_SFilter(rmatrix& P) const
{if (!IsReady()) {P.Set(0,0,false); return P;}
 cmatrix A(Rjj); return Filter->Powers(P,A);
};
//Мощность ВНУТРЕННЕГО ШУМА на входах/выходах пространственного фильтра
rmatrix& Pn_SFilter(rmatrix& P) const
{if (!IsReady()) {P.Set(0,0,false); return P;}
 cmatrix A(Rnn); return Filter->Powers(P,A);
};
//------------------------------------------------------------------------------
//Расчет потенциальных выходной мощности и коэффициента подавления аддитивной
//активной помехи и внутреннего шума адаптивного пространственного фильтра
//------------------------------------------------------------------------------
rvector& OutPjnPotent_SFilter(rvector& P) const;
rvector& KsupPotent_SFilter(rvector& Ksup) const;
//------------------------------------------------------------------------------
//Расчет потенциальных выходной мощности и коэффициента подавления аддитивной
//активной помехи и внутреннего шума адаптивной LAA /выходы MultiBeamFormer/
//------------------------------------------------------------------------------
rvector& OutPjnPotent(rvector& P) const;
rvector& KsupPotent(rvector& Ksup) const;

//------------------------------------------------------------------------------
// КОНСОЛЬНЫЕ ФУНКЦИИ ВВОДА - ВЫВОДА ПАРАМЕТРОВ КЛАССА TAMultiChSpaceFilter
//------------------------------------------------------------------------------
//Получение свойств класса в виде строкового объекта string
string& Properties(string& s, string& title, uint indent = 0) const;
string& Polarization(string& s, string& title, uint indent = 0) const;
//Ввод параметров объекта из стандартного потока ввода cin
void Edit();
//Редактирование свойств пространственного фильтра
void EditSFilter();
//Редактирование поляризационных свойств сигнала
void EditPolar();
//Перегрузка оператора ввода (>>) для ввода параметров класса
friend istream& operator >>(istream& in, TAMultiChSpaceFilter& SFilter);
//Перегрузка оператора вывода (<<) для вывода параметров класса
friend ostream& operator <<(ostream& out, const TAMultiChSpaceFilter& SFilter)
{string s; return out << SFilter.Properties(s,STR_NULL);};
//Объявление перегруженного оператора присваивания operator =
void operator =(const TAMultiChSpaceFilter& SFilter);
//------------------------------------------------------------------------------
//II. ОБЪЯВЛЕНИЕ ЗАЩИЩЕННЫХ ЧЛЕНОВ-ФУНКЦИЙ КЛАССА TAMultiChSpaceFilter
//------------------------------------------------------------------------------
protected:
 void _Reset() //Сброс всех характеристик класса
 {if (Filter != NULL) {delete Filter; Filter = NULL;}
  Rss = cmatrix(); Rjj = cmatrix(); Rnn = cmatrix(); R = cmatrix();
  TMultiBeamFormer::_Reset();
  TAnalInSignalProperty::Reset();
 };
 //Установка размеров корреляционных матриц
 void _MatrixSet()
 {ulong size = Size(); //complex C_ZERO = complex(0,0);
  Rss.Set(size,C_ZERO); Rjj.Set(size,C_ZERO); Rnn.Set(size,C_ZERO); R.Set(size,C_ZERO);
 };
 //Установка размерности фильтра и корреляционных матриц
 bool _SetSFilterSize()
 {if (!IsReady()) return false;
  Filter->SetInput(Size()); _MatrixSet();
  return true;
 }
 //Расчет корреляционных матриц на входе адаптивной LAA
 void _CalcInRss();
 void _CalcInRjj();
 void _CalcInRnn();
};
//Конец объявления класса TAMultiChSpaceFilter
//The end of class TAMultiChSpaceFilter declaration
//******************************************************************************
//                   Объявление класса TMultiChSpaceFilter
//Статистическая модель адаптивной LAA с пространственным адаптивным N-канальным
//фильтром и многолучевой лучеобразующей схемой. Конфигурация адаптивной LAA:
//1. LAA - линейная антенная решетка;
//2. Адаптивный пространственный фильтр с N входами и N выходами;
//3. Многолучевая лучеобразующая схема, подлюченная к пространственному фильтру.
//Класс наследует свойства классов TMultiBeamFormer.
//Класс формирует воздействие на входе пространственного фильтра, подключенного
//к LAA, в виде векторов полезного сигнала Us, аддитивной активной помехи Ujam,
//внутренненго шума Unoise и суммарной выборки Usum компонент входного сигнала.
//Вектор Usum используется для настройки весовых коэффициентов пространственного
//адаптивного фильтра размерностью N x N. Размерность фильтра определяется поль-
//зователем и не может превысить количества каналов LAA. Данные о размерности
//содержаться в классе TMultiBeamFormer и определяются количество и номера кана-
//лов LAA, подключенные к пространственному фильтру, а также задаются параметры
//многолучевой схемы: количество формируемых лучей и их пространственные положения
//(по азимуту или углу места) в зависимости от расположения LAA относительно по-
//верхности Земли. Класс TMultiBeamFormer служит одновременно коммутатором кана-
//лов LAA. Количество подключенных  каналов определяют размерность фильтра и ши-
//рину лучей, формируемых лучеобразующей схемой.
//При моделировании компоненты входного воздействия рассчитываются на каждом шаге,
//так как полезный сигнал, внутренний шум и активная аддитивная помеха изменяются
//по случайному закону.
//При построения передаточной функции адаптивной LAA после ее адаптации  исполь-
//зуется вектор сигнала единичной мощности. Класс вычисляет передаточные функции
//пространственного фильтра и многолучевой лучеобразующей схемы по отдельности,
//рассчитывать мощности на входах/выходах адаптивной LAA или фильтра, предостав-
//ляет усредненные значения весовых коэффициентов и т.д.
//ЗАМЕЧАНИЕ. При расчете мощности на входе и выходе пространственного фильтра
//возвращается вектор мощностей в каждом из каналов LAA, используемых адаптивной
//LAA, значения вычисляются на входах/выходах самого фильтра. При расчете мощнос-
//тей компонент входного воздействия на входе/выходе адаптивной LAA возвращается
//вектор мощности для каждого луча, формируемого MultiBeamFormer. Выходные мощнос-
//ти вычисляются на выходе многолучевой схемы /выход AdaptLAA/. Входные мощности
//адаптивной LAA, рассчитываются на выходе MultiBeamFormer, при ее подключении
//прямо к LAA, исключая SpaceFilter. Для оценки работы всей системы используются
//мощности на входе/выходе адаптивной LAA, а для оценки работы фильтра - мощности
//на входе/выходе пространственного фильтра.
//Пространственный фильтр представлен в классе указателем на базовый класс -
//TMultiInMultiOutWeightSummator, что дает возможность использовать в качестве
//пространственного фильтра конфигурации N x N разные численный модели весовых
//сумматоров с N входами и N выходами. Весовые сумматоры могут отличаться друг от
//друга по конфигурации (одноступенчатые и многоступенчатые), алгоритмам настройки
//весовых коэффициентов (алгоритмам адаптации), схемам стабилизации динамических
//параметров фильтров (наличию/отсутствию, типу схемы стабилизации). Все весовые
//сумматоры должно объединять одно: они должны быть с N ВХОДАМИ и N ВЫХОДАМИ.
//Размерность сумматора определяется количеством каналов LAA, подключенных к
//MultiBeamFormer. Такая реализация класса дает возможность исследовать адаптивные
//LAA как по их внутренним параметрам (изменение значений весовых коэффициентов
//по помеховой ситуации), так и по их структуре, алгоритмам настройки весовых
//коэффициентов и стабилизации их динамических параметров. Работа пространствен-
//ного фильтра заключается в формировании нулей диаграммы направленности (переда-
//точной функции) в направлении на источники помех. Размерность фильтра определяет
//максимальное число активных (шумовых) помех, которые могут быть подавлены адап-
//тивной LAA. Считается, что полезный сигнала располагается в главном луче LAA.
//Статистическое моделирование адаптивной LAA с пространственным фильтром N x N
//и многолучевой схемой должно производится за много реализации с определенной
//длительностью каждой реализации. Рекомендуемое число реализаций больше 20. От
//количества проведенных реализаций завистит точность результатов моделирования.
//В начале моделирования весовые коэффициенты адаптивного фильтра ненастроены и
//он пропускает входное воздействия без изменения. Так как при статистическом
//моделировании должно быть проведено много испытаний, то весовые коэффициенты
//усредняются на последнем шаге каждой реализации и заносятся в матрицу AvW. По
//окончании моделирования можно построить передаточную характеристику адаптивной
//LAA и по ней оценить работу адаптивной LAA, которая зависит от многих факторов:
//входного воздействия, конфигурации LAA и пространственного фильтра, алгоритма
//настройки и схем стабилизации фильтра, внутренних параметров фильтра и т.д.
//Передаточная функция всегда вычисляется по усредненным весовым коэффициентам.
//******************************************************************************
class TMultiChSpaceFilter: public TMultiBeamFormer {
//------------------------------------------------------------------------------
// ОБЪЯВЛЕНИЕ ЗАЩИЩЕННЫХ ЧЛЕНОВ-ДАННЫХ КЛАССА TMultiChSpaceFilter
//------------------------------------------------------------------------------
protected:
 //Поляризационная составляющая сигнала, принимаемого фильтром
 bool Polar;
 //Указатель на базовый абстрактный класс весового сумматора N x N
 TMultiInMultiOutWeightSummator* Filter;//Пространственный адаптивный фильтр
 //Выборки полезного сигнала, аддитивной помехи и внутреннего шума на входе фильтра
 cvector Us;//Входная выборка ПОЛЕЗНОГО СИГНАЛА
 cvector Ujam;//Входная выборка АДДИТИВНОЙ ПОМЕХИ
 cvector Unoise;//Входная выборка ВНУТРЕННЕГО ШУМА
 //Выборка компонент входного воздействия на входе адаптивной LAA: Us+Ujam+Unoise
 cvector Usum;
 //Усредненные по реализациям в ходе моделирования весовые коэффициенты
 cmatrix AvW;
//------------------------------------------------------------------------------
// I. ОБЪЯВЛЕНИЕ ОБЩЕДОСТУПНЫХ ЧЛЕНОВ-ФУНКЦИЙ КЛАССА TMultiChSpaceFilter
//------------------------------------------------------------------------------
public:
//******************************************************************************
// ОБЪЯВЛЕНИЕ КОНСТРУКТОРОВ И ДЕСТРУКТОРА КЛАССА TMultiChSpaceFilter
//******************************************************************************
//Объявление конструктора по умолчанию
TMultiChSpaceFilter() : TMultiBeamFormer() {Filter = NULL; _Reset();};
//Объявление конструктора с аргументами
TMultiChSpaceFilter(TInputSignal* pLAA, const ivector& LAACh = ivector(),
 const rvector& Beams = rvector(), uint SF_Type = MS_SDM_MIMO_WSum,
 rvector& SF_Param = rvector(), bool PolarType = VP_SIGNAL) :
 TMultiBeamFormer(pLAA,LAACh,Beams)
 {Filter = NULL; Polar = VP_SIGNAL;
  if (IsBeamFormerReady()) SetSFilter(SF_Type,SF_Param);
 };
//Объявление конструктора копирования
TMultiChSpaceFilter(TMultiChSpaceFilter& SFilter) {*this = SFilter;};
//Объявление деструктора класса
~TMultiChSpaceFilter() {_Reset();};
//------------------------------------------------------------------------------
//Установка свойств класса
//------------------------------------------------------------------------------
bool Set(TInputSignal* pLAA, const ivector& LAACh = ivector(),
         const rvector& Beams = rvector(), uint SF_Type = MS_SDM_MIMO_WSum,
         rvector& SF_Param = rvector(), bool PolarType = VP_SIGNAL)
{if (SetBeamFormer(pLAA,LAACh,Beams))
 {SetPolar(PolarType); return SetSFilter(SF_Type,SF_Param);}
 _Reset();//Сброс всех параметров адаптивной LAA
 return false;
};
//Установка свойств лучеобразующей схемы
bool SetBeamFormer(TInputSignal* pLAA, const ivector& LAACh = ivector(),
     const rvector& Beams = rvector());
//Установка свойств пространственного фильтра
bool SetSFilter(uint SF_Type, rvector& SF_Param = rvector());
//Установка поляризационной составляющей сигнала, принимаемого фильтром
bool SetPolar(bool PolarType) {return Polar = PolarType;};
//Установка усредненных коэффициентов AvW в весовой сумматор
bool SetAvW() {if (!IsReady()) return false; return Filter->SetW(AvW);};
//------------------------------------------------------------------------------
// ПОЛУЧЕНИЕ СВОЙСТВА (ХАРАКТЕРИСТИК) КЛАССА TMultiChSpaceFilter
//------------------------------------------------------------------------------
//Определение РАБОТОСПОСОБНОСТИ лучеобразующей схемы
bool IsBeamFormerReady() const {return TMultiBeamFormer::IsReady();};
//Определение РАБОТОСПОСОБНОСТИ пространственного фильтра
bool IsSpaceFilterReady() const
{return ((Filter != NULL) && (Filter->IsReady())) ? ON : OFF;};
//Определение РАБОТОСПОСОБНОСТИ всей АДАПТИВНОЙ Linear Antenna Array
bool IsReady() const
{return ((IsBeamFormerReady()) && (IsSpaceFilterReady())) ? ON : OFF;};
//Размерность фильтра - количество каналов LAA, подключенных к фильтру
ulong Size() const {return TMultiBeamFormer::GetLAACh();};
//Получение поляризационной составляющей сигнала
bool GetPolar() const {return Polar;};
//Получение усредненных коэффициентов весового сумматора
cmatrix& GetW(cmatrix& W) const
{if (!IsReady()) W.Set(0L,0L,false); else W = AvW; return W;};
//Получение типа пространственного фильтра
uint SFilterType() const {return (IsSpaceFilterReady()) ? Filter->Type() : 0;};
//Получение параметров пространственного фильтра
rvector& SFilterParam(rvector& Arg) const
{if (!IsSpaceFilterReady()) {Arg.Set(0L,ROW,false); return Arg;}
 return Filter->Param(Arg);
}
//---------------  ПЕРЕДАТОЧНЫЕ ФУНКЦИИ АДАПТИВНОЙ LAA  ------------------------
//Передаточная функция (Transfer function) адаптивной LAA /выход MultiBeamFormer/
bool TFunc(rmatrix& P, const TEvenCloseInterval& Sector, uint NormType = GLOBAL,
           bool dB = ON, double MindB = -60.0) const;
//Передаточная функция (Transfer function) адаптивного пространственного фильтра
//                      /выход SpaceFilter/
bool TF_SFilter(rmatrix& P, const TEvenCloseInterval& Sector, uint NormType = GLOBAL,
                bool dB = ON, double MindB = -60.0) const;
//Передаточная функция (Transfer function) лучеобразующей схемы без SpaceFilter
//             /выход MultiBeamformer - прямое подключение к LAA/
bool TF_BeamFormer(rmatrix& P, const TEvenCloseInterval& Sector, uint NormType = GLOBAL,
                   bool dB = ON, double MindB = -60.0) const
{return TMultiBeamFormer::TFunc(P,Sector,NormType,dB,MindB);};
//------------------------------------------------------------------------------
//           ФУНКЦИИ МОДЕЛИРОВАНИЯ КЛАССА TMultiChSpaceFilter
//------------------------------------------------------------------------------
//Начальная установка параметров AdaptLAA при моделировании: начальная установ-
//ка весовых коэффициентов пространственного фильтра
bool SimulateInit() {return (IsReady()) ? Filter->SimulateInit() : OFF;};
//Расчет выборок компонент входного воздействия при моделировании
bool Update();
//Настройка весовых коэффициентов пространственного фильтра
bool Tuning() {return (IsReady()) ? Filter->Tuning(Usum) : OFF;};
//Усреднение весовых коэффициентов пространственного фильтра
bool AverageW(ulong Realization);
//------------------------------------------------------------------------------
//               РАСЧЕТ МОЩНОСТЕЙ НА ВХОДЕ адаптивной LAA
//   /на выходе MultiBeamFormer, подключенной к LAA без SpaceFilter/
//------------------------------------------------------------------------------
//Мощность ПОЛЕЗНОГО СИГНАЛА на входе адаптивной LAA
bool InPSignal(rvector& Ps) const {return TMultiBeamFormer::TFunc(Us,Ps);};
//Мощность АКТИВНОЙ АДДИТИВНОЙ ПОМЕХИ на входе адаптивной LAA
bool InPJammer(rvector& Pjam) const {return TMultiBeamFormer::TFunc(Ujam,Pjam);};
//Мощность ВНУТРЕННЕГО ШУМА на входе адаптивной LAA
bool InPNoise(rvector& Pn) const {return TMultiBeamFormer::TFunc(Unoise,Pn);};
//Суммарная мощность на входе адаптивной LAA
bool InPSum(rvector& Psum) const {return TMultiBeamFormer::TFunc(Usum,Psum);};
//------------------------------------------------------------------------------
//               РАСЧЕТ МОЩНОСТЕЙ НА ВЫХОДЕ адаптивной LAA
//            /выход MultiBeamFormer с подключением SpaceFilter/
//------------------------------------------------------------------------------
//Мощность ПОЛЕЗНОГО СИГНАЛА на выходе адаптивной LAA
bool OutPSignal(rvector& Ps) const
{if (!IsReady()) {Ps.Set(0L,ROW,false); return false;}
 cvector V(Us); Filter->TF_WSum(V);
 return TMultiBeamFormer::TFunc(V,Ps);
};
//Мощность АКТИВНОЙ ПОМЕХИ на выходе адаптивной LAA
bool OutPJammer(rvector& Pjam) const
{if (!IsReady()) {Pjam.Set(0L,ROW,false); return false;}
 cvector V(Ujam); Filter->TF_WSum(V);
 return TMultiBeamFormer::TFunc(V,Pjam);
};
//Мощность ВНУТРЕННЕГО ШУМА на выходе адаптивной LAA
bool OutPNoise(rvector& Pn) const
{if (!IsReady()) {Pn.Set(0L,ROW,false); return false;}
 cvector V(Unoise); Filter->TF_WSum(V);
 return TMultiBeamFormer::TFunc(V,Pn);
};
//Суммарная мощность на выходе адаптивной LAA
bool OutPSum(rvector& Psum) const
{if (!IsReady()) {Psum.Set(0L,ROW,false); return false;}
 cvector V(Usum); Filter->TF_WSum(V);
 return TMultiBeamFormer::TFunc(V,Psum);
};

//------------------------------------------------------------------------------
//         РАСЧЕТ МОЩНОСТЕЙ НА ВХОДЕ пространственного фильтра
//        /мощности в каналах LAA, подключенных к SpaceFilter/
//------------------------------------------------------------------------------
//Мощность ПОЛЕЗНОГО СИГНАЛА на входе пространственного фильтра
bool InPs_SFilter(rvector& Ps) const;
//Мощность АКТИВНОЙ АДДИТИВНОЙ ПОМЕХИ на входе пространственного фильтра
bool InPjam_SFilter(rvector& Pjam) const;
//Мощность ВНУТРЕННЕГО ШУМА на входе пространственного фильтра
bool InPn_SFilter(rvector& Pn) const;
//------------------------------------------------------------------------------
//          РАСЧЕТ МОЩНОСТЕЙ НА ВЫХОДЕ пространственного фильтра
//                         /выход SpaceFilter/
//------------------------------------------------------------------------------
//Мощность ПОЛЕЗНОГО СИГНАЛА на выходе пространственного фильтра
bool OutPs_SFilter(rvector& Ps) const
{if (!IsReady()) {Ps.Set(0L,ROW,false); return false;}
 cvector V(Us); Filter->TF_WSum(Ps,V);
 return true;
};
//Мощность АКТИВНОЙ ПОМЕХИ на выходе пространственного фильтра
bool OutPjam_SFilter(rvector& Pjam) const
{if (!IsReady()) {Pjam.Set(0L,ROW,false); return false;}
 cvector V(Ujam); Filter->TF_WSum(Pjam,V);
 return true;
};
//Мощность ВНУТРЕННЕГО ШУМА на выходе пространственного фильтра
bool OutPn_SFilter(rvector& Pn) const
{if (!IsReady()) {Pn.Set(0L,ROW,false); return false;}
 cvector V(Unoise); Filter->TF_WSum(Pn,V);
 return true;
};

//------------------------------------------------------------------------------
// КОНСОЛЬНЫЕ ФУНКЦИИ ВВОДА - ВЫВОДА ПАРАМЕТРОВ КЛАССА TMultiChSpaceFilter
//------------------------------------------------------------------------------
//Получение свойств класса в виде строкового объекта string
string& Properties(string& s, string& title, uint indent = 0) const;
string& Polarization(string& s, string& title, uint indent = 0) const;
//Ввод параметров объекта из стандартного потока ввода cin
void Edit();
//Редактирование свойств пространственного фильтра
void EditSFilter();
//Редактирование поляризационных свойств сигнала
void EditPolar();
//Перегрузка оператора ввода (>>) для ввода параметров класса
friend istream& operator >>(istream& in, TMultiChSpaceFilter& SFilter);
//Перегрузка оператора вывода (<<) для вывода параметров класса
friend ostream& operator <<(ostream& out, const TMultiChSpaceFilter& SFilter)
{string s; return out << SFilter.Properties(s,STR_NULL);};
//Объявление перегруженного оператора присваивания operator =
void operator =(const TMultiChSpaceFilter& SFilter);
//------------------------------------------------------------------------------
//II. ОБЪЯВЛЕНИЕ ЗАЩИЩЕННЫХ ЧЛЕНОВ-ФУНКЦИЙ КЛАССА TMultiChSpaceFilter
//------------------------------------------------------------------------------
protected:
 void _Reset() //Сброс всех характеристик класса
 {if (Filter != NULL) {delete Filter; Filter = NULL;}
  Us = cvector(); Ujam = cvector(); Unoise = cvector(); Usum = cvector();
  AvW = cmatrix();
  TMultiBeamFormer::_Reset();
 };
 //Установка размеров векторов и матрицы AvW
 void _VectorSet()
 {ulong size = Size();
  Us.Set(size,ROW,C_ZERO); Ujam.Set(size,ROW,C_ZERO); Unoise.Set(size,ROW,C_ZERO);
  Usum.Set(size,ROW,C_ZERO); Filter->GetW(AvW);
 };
 //Установка размерности фильтра, матрицы AvW и векторов
 bool _SetSFilterSize()
 {if (!IsReady()) return false;
  Filter->SetInput(Size()); _VectorSet();
  return true;
 }
 //Расчет выборок компонент входного воздействия на входе адаптивной LAA
 void _CalcUs();
 void _CalcUjam();
 void _CalcUnoise();
};
//Конец объявления класса TMultiChSpaceFilter
//The end of class TMultiChSpaceFilter declaration

//****************************************************************************************
//          Объявление абстрактного класса TAuxChSpaceRejectFilter
//****************************************************************************************
//Класс является БАЗОВЫМ для создания классов ПРОСТРАНСТВЕННЫХ РЕЖЕКТОРНЫХ ФИЛЬТРОВ ДОПОЛ-
//НИТЕЛЬНЫХ КАНАЛОВ АДАПТИВНОГО ПРОСТРАНСТВЕННОГО ФИЛЬТРА с ОСНОВНЫМ КАНАЛОМ. Назначение
//класса состоит в исключении заданных направлений приема сигналов в дополнительных каналах
//адаптивного пространственного фильтра.
//Режекторный фильтр представляет собой весовой сумматор с постоянными коэффициентами с M
//входами и N выходами (M > N). Классифицируются по схемам построения весовых сумматоров и
//бывают двух видов: МНОГОСТУПЕНЧАТЫЕ (MultiStage) и ОДНОСТУПЕНЧАТЫЕ (SingleStage).
//Одноступенчатый режекторный пространственный фильтр представляет собой совокупность
//одноступенчатых весовых сумматоров, количество которых равно N. Каждый такой сумматор
//имеет M-N+1 число входов и ОДИН выход.
//Многоступенчатый режекторный пространственный фильтр представляет собой один многоступен-
//чатый весовой сумматор с M входами и N выходами.
//Разность M-N определяет количество направлений исключения, которое может реализовать фильтр.
//Каждый рабочий канал фильтра обеспечивает реализацию одного направления исключения приема
//сигналов в дополнительных каналах адаптивного пространственного фильтра с выделенным ка-
//налом. Алгоритмы настройки коэффициентов режекторного фильтра зависит от его схемной реа-
//лизации. Для расчета коэффициентов режекторного фильтра, независимо от его схемной реали-
//зации, при конструировании объекта передается комплексная матрица размерностью MxK, где
//M+1 - количество входов фильтра, K - число направлений исключения. Число выходов фильтра
//N = M+1-K. Для повышения скорости вычислений в режекторный фильтр дополнительных каналов
//вводится ОСНОВНОЙ канал, который не изменяется режекторным фильтром. Так как ОСНОВНОЙ
//канал не изменяется фильтром, то в обучающую матрицу его значения не включаются.
//Обучающая матрица представляет собой K векторов размерностью M. В матрице размещаются
//комплексные значения напряжений приемных каналов LAA, которые подключаются к режекторному
//фильтру, при действии ЕДИНИЧНЫХ источников сигналов с каждого из K направлений исключения.
//Первые K строк соответствуют РАБОЧИМ каналам режекторного фильтра, остальные N строк -
//каналам LAA, используемым как ДОПОЛНИТЕЛЬНЫЕ. Отметим, что данный класс не содержит инфор-
//мации по направлениям исключения, индексам каналов LAA, которые к нему подключены и опции
//подключения ОСНОВНОГО канала как рабочего канала режекторного фильтра. Класс является
//исполнительным механизмом, реализующим исключение приема сигналов с заданных направлений
//в дополнительных каналах Adaptive SpaceFilter with BaseChannel. Исключение направлений
//приема в дополнительных каналах адаптивного фильтра с основным каналом приводит к невоз-
//можности подавления источников сигналов с заданных направлений, что в ряде случаев оказы-
//вается необходимым. Структура обучающей матрицы для реализации пространственного режектор-
//ного фильтра для 4-х дополнительных каналов и 3-х направлений исключения будет иметь
//следующий вид: N = 4; K = 3; M = 7.
//Размерность A: rows = M = 7; cols = K = 3.
//     | U11 U12 U13 |  Uij - комплексное напряжение в i-ом канале при действии
//     | U21 U22 U23 |        одного источника сигнала ЕДИНИЧНОЙ мощности с j-го
//     | U31 U32 U33 |        направления исключения;
// A = | U41 U42 U43 |  U1j..U3j - рабочие каналы AuxChSpaceRejectFilter;
//     | U51 U52 U53 |  U4j..U7j - дополнительные каналы AdaptSpaceFilterBaseCh
//     | U61 U62 U63 |
//     | U71 U72 U73 |
//Следует отметить, что включение ОСНОВНОГО канала в качестве рабочего канала режекторного
//фильтра, позволяет уменьшить уровень внутреннего шума.
//Весовые коэффициенты, рассчитанные по "обучающей" матрице A помещаются в матрицу весовых
//коэффициентов Q. Структура размещения коэффициентов фильтра и размерность матрицы Q зави-
//сит от схемной реализации фильтра.
//На вход фильтра можно подавать вектор U или корреляционную матрицу R входного воздействия
//размерности M+1[BaseCh]. На выходе получается вектор U' или матрица R' размерности
//N+1[BaseCh]. Значение ОСНОВНОГО канала всегда должно находится последним в векторе U и
//последовательность каналов на входе режекторного фильтра должна соответствовать последо-
//вательности каналов "обучающей" матрицы. При ином расположении каналов режекция осуществ-
//ляться не будет. Кроме того, на вход режекторного фильтра м.б. подан только комплексный
//вектор напряжений или корреляционная матрица размерности M+1. При иной размерности возв-
//ращается выходной вектор или матрица нулевого размера.
//Детали реализации алгоритма  вычисления коэффициентов режекторного пространственного
//фильтра и структура хранения коэффициентов зависит от схемной реализации MultiStage или
//SingleStage и будут описаны в данных классах.
//****************************************************************************************
class TAuxChSpaceRejectFilter {
//------------------------------------------------------------------------------
// Объявление защищенных членов-данных класса TAuxChSpaceRejectFilter
//------------------------------------------------------------------------------
protected:
 ulong Input;//Количество входов фильтра
 ulong Output;//Количество выходов фильтра
//------------------------------------------------------------------------------
//ОБЪЯВЛЕНИЕ КОНСТРУКТОРОВ и ДЕСТРУКТОРА КЛАССА TAuxChSpaceRejectFilter
//------------------------------------------------------------------------------
public:
 //Конструктор по умолчанию
 TAuxChSpaceRejectFilter() {Reset();};
 //Конструктор с аргументами
 TAuxChSpaceRejectFilter(const cmatrix& A) {Tuning(A);};
 //Объявление деструктора класса
 virtual ~TAuxChSpaceRejectFilter() {Reset();};
//------------------------------------------------------------------------------
//Объявление общедоступных членов-функций класса TAuxChSpaceRejectFilter
//------------------------------------------------------------------------------
public:
 //Определение работоспособности режекторного фильтра
 bool IsReady() const {return (Input != 0L) ? true : false;};
 ulong In() const {return Input;};//Количество входов режекторного фильтра
 ulong Out() const {return Output;};//Количество выходов режекторного фильтра
 //Количество направлений исключения (режекции)
 ulong Directs() const {return Input-Output;};

 //-----------------------------------------------------------------------------
 //Передаточная функция (TRANSFER FUNCTION) TAuxChSpaceRejectFilter
 //-----------------------------------------------------------------------------
 virtual bool TFunc(const cmatrix& InR, cmatrix& OutR) const = 0;
 virtual bool TFuncDM(const cmatrix& InR, cmatrix& OutR) const = 0;
 virtual bool TFunc(const cvector& InV, cvector& OutV) const = 0;

 bool operator ()(const cmatrix& InR, cmatrix& OutR) const
      {return TFunc(InR, OutR);};
 bool operator ()(const cvector& InV, cvector& OutV) const
      {return TFunc(InV, OutV);};
 //-----------------------------------------------------------------------------
 //Настройка весовых коэффициентов режекторного пространственного фильтра
 //-----------------------------------------------------------------------------
 virtual bool Tuning(const cmatrix& A) = 0;
 //-----------------------------------------------------------------------------
 //Получение весовых коэффициентов TAuxChSpaceRejectFilter
 //-----------------------------------------------------------------------------
 virtual bool GetW(cmatrix& W, bool lExtend = ON) const = 0;
 //-----------------------------------------------------------------------------
 //Консольный ввод-вывод свойств класса
 //-----------------------------------------------------------------------------
 virtual uint Type() const = 0;//Тип весового сумматора
 virtual string& Type(string& s) const = 0;//Тип весового сумматора
 //Сокращенное наименование класса
 virtual string& Clip(string& s) const = 0;
 //Формирование конфигурации AuxChSpaceRejectFilter в виде символьной строки:
 //< <Input> x <Output>;\n >
 string& GetConfig(string& s) const
 {char buffer[80]; ostrstream text (buffer, sizeof(buffer));
  text << In() << " x " << Out() << ";" << endl << ends;
  s.append(text.str()); text.seekp(0);
  return s;
 };
 //Формирование свойств весового сумматора в виде символьной строки
 virtual string& Properties(string& s, string& head, uint indent = 0) const = 0;

 //Перегрузка операции потокового вывода (<<) свойств класса
 friend ostream& operator <<(ostream& out, const TAuxChSpaceRejectFilter& SRF)
 {string s; return out << SRF.Properties(s,STR_NULL);};
//------------------------------------------------------------------------------
//Объявление защищенных членов-функций класса TAuxChSpaceRejectFilter
//------------------------------------------------------------------------------
protected:
 //Начальная установка параметров режекторного фильтра
 void Reset() {Input = 0L; Output = 0L;};
};//Завершение объявления класса TAuxChSpaceRejectFilter
//------------- Завершение объявления класса TAuxChSpaceRejectFilter -----------

//****************************************************************************************
//          Объявление класса TMultiStageAuxChSRF
//****************************************************************************************
//Класс реализует модель МНОГОСТУПЕНЧАТОГО ПРОСТРАНСТВЕННОГО РЕЖЕКТОРНОГО ФИЛЬТРА ДОПОЛНИ-
//ТЕЛЬНЫХ КАНАЛОВ АДАПТИВНОГО ПРОСТРАНСТВЕННОГО ФИЛЬТРА с ОСНОВНЫМ КАНАЛОМ. Назначение
//класса состоит в исключении заданных направлений приема сигналов дополнительными каналами
//адаптивного пространственного фильтра.
//Режекторный фильтр представляет собой многоступенчатый весовой сумматор с постоянными
//коэффициентами с M входами и N выходами (M > N).
//Разность M-N определяет количество направлений исключения, которые фильтр реализует. Каж-
//дый рабочий канал фильтра обеспечивает одно направление исключения приема сигналов в до-
//полнительных каналах адаптивного пространственного фильтра с выделенным каналом. Для рас-
//чета коэффициентов режекторного фильтра при конструировании объекта передается комплексная
//"обучающая" матрица размерностью MxK, где M+1 - количество входов фильтра, K - число на-
//правлений исключения. Число выходов фильтра N = M+1-K. Обучающая матрица представляет K
//векторов размерностью M. Структура матрица описана в классе TAuxChSpaceRejectFilter.
// АЛГОРИТМ Q-ПРЕОБРАЗОВАНИЯ, ВЫЧИСЛЕНИЯ  ВЕСОВЫХ КОЭФФИЦИЕНТОВ МНОГОСТУПЕНЧАТОГО ФИЛЬТРА
//                            и СХЕМА ХРАНЕНИЯ КОЭФФИЦИЕНТОВ
//Коэффициенты фильтра хранятся в матрице Q. Размерность матрицы Q складывается:
//1. RDEChannel (каналы LAA используемые для исключения приема сигналов с заданных направ-
//   лений вспомогательными каналами фильтра);
//2. AuxChannel (дополнительные каналы фильтра, подключаемые с LAA);
//3. BaseChannel размерности 1 (основной канал фильтра).
//Матрица Q хранит коэффициенты в упакованном виде. Для исключения приема сигналов с N
//направлений необходимо 2*N элементарных матриц специального вида:
// R' = Qn*Qn-1*...*Q2*Q1*R*trQ1*trQ2*...*trQn-1*trQn, где
// R - входная корреляционная матрица;
// R' - преобразованная корреляционная матрица с N исключенными направлениями;
// Qn,Qn-1,...,Q2,Q1 - комплексные элементарные матрицы специального вида с коэффициентами
// исключения для каждого канала для N-направлений. Данные матрицы - НИЖНИЕ СТОЛБЦОВЫЕ
// ЭЛЕМЕНТАРНЫЕ МАТРИЦЫ;
// trQ1,trQ2,...,trQn-1,trQn - транспонированные к Q1,tQ2,...,Qn-1,Qn матрицы весовых коэф-
// фициентов. Данные матрицы - ПРАВЫЕ СТРОЧНЫЕ ЭЛЕМЕНТАРНЫЕ МАТРИЦЫ.
//Данная особенность позволяет хранить все 2*N матриц в одной матрице Q. Реализованные в
//классах cvector и cmatrix операции умножения на элементарные матрицы специального вида
//с учетом компактной формы их хранения в одной матрице позволяют корректно и быстро вы-
//полнять необходимые операции для исключения заданных направлений исключения из входных
//корреляционных матриц и векторов  сигналов.
//Для задания N направлений исключения приема сигналов в дополнительных каналах фильтра
//используются N каналов LAA, индексы которых не должны совпадать с индексами каналов LAA,
//которые используются как дополнительные каналы пространственного фильтра с выделенным
//каналом. В основном канале подавление приема сигналов с заданных направлений не должно
//происходить, поэтому в матрице Q последняя строка и последний столбец содержат нулевые
//коэффициенты, за исключением последнего элемента матрицы (на пересечении последних столбца
//и строки) равного 1. Если не задано ни одного направления исключения, матрица Q должна
//быть ЕДИНИЧНОЙ.
//Коэффициенты Space Reject Filter обеспечивают нулевые уровни приема сигналов с направле-
//ний исключения в AuxCh. Таким образом, обеспечивается передача сигналов с направлений
//исключения только через ОСНОВНОЙ КАНАЛ без его компенсации на весовом сумматоре адаптив-
//ного фильтра. Коэффициенты для всех ДОПОЛНИТЕЛЬНЫХ КАНАЛОВ и ОДНОГО НАПРАВЛЕНИЯ ИСКЛЮЧЕ-
//НИЯ рассчитываются относительно ОДНОГО RDE КАНАЛА, который подключается с АР к
//MultiStageAuxChSRF и не должен совпадать ни с одним AuxCh. Для расчета коэффициентов для
//всех AuxCh и другого направления исключения должен быть задействован другой рабочий канал
//SRF и так  далее. Методика вычисления коэффициентов следующая:
//1. Формируется сигнал ЕДИНИЧНОЙ МОЩНОСТИ, приходящий с ПЕРВОГО НАПРАВЛЕНИЯ ИСКЛЮЧЕНИЯ, и
// рассчитываются комплексные значения напряжений на каналах LAA, подключенных к
// MultiStageAuxChSRF. К этим каналам относятся RDECh и AuxCh.
// Комплексные коэффициенты для ПЕРВОГО НАПРАВЛЕНИЯ исключения рассчитываются по формуле:
// Q(i,0) = - Ui/U0, Q(0,i) = conj(Q(i,0)),  0 < i < LAACh
// Q(i,0) - коэффициент SRF для i-го канала LAA, подключенного к нему, для первого направ-
// ления исключения;
// Ui - комплексное напряжение i-го канала LAA, подключенного к AuxChRDEUnit,
// при действии источника единичной мощности с первого направления исключения;
// U0 - комплексное напряжение 1-го рабочего канала RDECh, которое обеспечивает исключение
// направления приема сигнала с первого направления в остальных каналах подключенных к
// AuxChRDEUnit.
// 2. Алгоритм расчета для j-го направления исключения (0 < j <= N). Формируется сигнал
// ЕДИНИЧНОЙ МОЩНОСТИ с j-го направления исключения и формируется вектор комплексных напря-
// жений на каналах LAACh, подключенных к SRF. Полученный вектор U умножается на последо-
// вательность левых столбцовых элементарных  матриц Lkc: U' = Lj-1c*...*L1c*L0c*U. В ре-
// зультате вектор U изменяется на вектор U', из данного вектора исключаются источники сиг-
// нала с 0..j-1 направлений исключения. Коэффициенты для j-го направления исключения рас-
// считываются аналогично п.1 по формуле: Q(i,j)=-U'i/U'j, Q(j,i)=conj(Q(i,j)), j<i<LAACh
// Q(i,j) - коэффициент SRF для i-го канала LAA, подключенного к нему, для j-го направления
// исключения;
// U'i - комплексное напряжение i-го канала LAA, подключенного к AuxChSRF, при действии
// источника единичной мощности с j-го направления исключения;
// U'j - комплексное напряжение j-го рабочего канала RDECh, которое обеспечивает исключение
// направления приема сигнала с j-го направления в остальных каналах подключенных к AuxChSRF.
// Выполнение п.2 продолжается для расчета всех N направлений исключения.
// ПРИМЕЧАНИЕ: В классе осуществляется только расчет весовых коэффициентов SRF. Тестовые
// сигналы для каналов LAA и ОСНОВНОГО канала (при его подключении), которые подключены к
// AuxChSRF, и заданных направлениях исключения передаются в обучающей матрице A. За форми-
// рование данной матрицы ответственен класс, который использует объект класса TMultiStage-
// AuxChSRF. Класс TMultiStageAuxChSRF выполняет только проверку корректности по размерности
// матрицы A. Матрица A не должна быть нулевой и число строк матрицы должно быть строго
// больше числа столбцов матрицы. В противном случае, объект класса не будет работоспособным.
// Приведем обобщенную структуру матрицы Q для схемы AuxChRDEUnit для трех направлений
// исключения и пространственного фильтра с основным каналом и 5 дополнительными каналами.
// Таким образом, RDECh = 3, AuxCh = 5, Dimension Q = 9x9 (с учетом включения ОСНОВНОГО
// КАНАЛА). Матрица Q будет иметь следующий вид:
//       U0  U1  U2  U3  U4  U5  U6  U7  U8
//     |  1  q01 q02 q03 q04 q05 q06 q07  0| U0
//     | q10  1  q12 q13 q14 q15 q16 q17  0| U1
//     | q20 q21  1  q23 q24 q25 q26 q27  0| U2
//     | q30 q31 q32  1   0   0   0   0   0| U3
// Q = | q40 q41 q42  0   1   0   0   0   0| U4
//     | q50 q51 q52  0   0   1   0   0   0| U5
//     | q60 q61 q62  0   0   0   1   0   0| U6
//     | q70 q71 q72  0   0   0   0   1   0| U7
//     |  0   0   0   0   0   0   0   0   1| U8
// U0..U2 - RDECh, U3..U7 - AuxCh, U8 - BaseChannel.
//С целью минимизации числа неиспользуемых при вычислениях элементов матрицы Q, она разби-
//вается на две прямоугольные матрицы Q и trQ. Матрицы Q и trQ будут иметь следующий вид:
//       U0  U1  U2
//     |  1   0   0  | U0
//     | q10  1   0  | U1
//     | q20 q21  1  | U2          U0  U1  U2  U3  U4  U5  U6  U7
//     | q30 q31 q32 | U3         | 1  q01 q02 q03 q04 q05 q06 q07 | U0
// Q = | q40 q41 q42 | U4   trQ = | 0   1  q12 q13 q14 q15 q16 q17 | U1
//     | q50 q51 q52 | U5         | 0   0   1  q23 q24 q25 q26 q27 | U2
//     | q60 q61 q62 | U6
//     | q70 q71 q72 | U7
// U0..U2 - RDECh, U3..U7 - AuxCh.
//В качестве входного воздействия могут использоваться комплексные корреляционная матрица
//R или вектор напряжений U с компонентой ОСНОВНОГО канала адаптивного фильтра. При Q-пре-
//образовании проверяется работоспособность фильтра и соответствие размерности входного
//воздействия размерности пространственного режекторного фильтра. При их несоответствии
//или неработоспособности фильтра возвращается вектор или матрица нулевого размера.
//Размерность входного вектора должна быть равна числу входов фильтра. Матрица R должна
//быть квадратной и с размерностью равной числу входов SRF. В качестве выходного значения
//объект класса возвращает вектор или квадратную матрицу с размерностями равными числу
//выходов Space Reject Filter.
//****************************************************************************************
class TMultiStageAuxChSRF : public TAuxChSpaceRejectFilter {
//---------------------------------------------------------------------------------------
// Объявление защищенных членов-данных класса TMultiStageAuxChSRF
//---------------------------------------------------------------------------------------
protected:
 cmatrix Q;//Матрица Q-преобразования
 cmatrix trQ;//Матрица сопряженно-транспонированная к матрице Q
//------------------------------------------------------------------------------
//ОБЪЯВЛЕНИЕ КОНСТРУКТОРОВ и ДЕСТРУКТОРА КЛАССА TMultiStageAuxChSRF
//------------------------------------------------------------------------------
public:
 //Конструктор по умолчанию
 TMultiStageAuxChSRF() {Reset();};
 //Конструктор с аргументами
 TMultiStageAuxChSRF(const cmatrix& A) {Tuning(A);};
 //Объявление деструктора класса
 ~TMultiStageAuxChSRF() {Reset();};
//------------------------------------------------------------------------------
//Объявление общедоступных членов-функций класса TMultiStageAuxChSRF
//------------------------------------------------------------------------------
public:
 //-----------------------------------------------------------------------------
 //Передаточная функция (TRANSFER FUNCTION) TMultiStageAuxChSRF
 //-----------------------------------------------------------------------------
 bool TFunc(const cmatrix& InR, cmatrix& OutR) const;
 bool TFuncDM(const cmatrix& InR, cmatrix& OutR) const;
 bool TFunc(const cvector& InV, cvector& OutV) const;
 //-----------------------------------------------------------------------------
 //Настройка весовых коэффициентов режекторного пространственного фильтра
 //-----------------------------------------------------------------------------
 bool Tuning(const cmatrix& A);
 //-----------------------------------------------------------------------------
 //Получение весовых коэффициентов TMultiStageAuxChSRF
 //-----------------------------------------------------------------------------
 bool GetW(cmatrix& W, bool lExtend = ON) const;
 //-----------------------------------------------------------------------------
 //Консольный ввод-вывод свойств класса TMultiStageAuxChSRF
 //-----------------------------------------------------------------------------
 uint Type() const {return MultiStageUnit;};//Тип весового сумматора
 //Тип весового сумматора
 string& Type(string& s) const
 {return s = "MultiStage Space Reject Filter of the Auxiliary Channels";};
 //Сокращенное наименование класса
 string& Clip(string& s) const {return s = "MultiStageAuxChSRF";};
 //Формирование свойств весового сумматора в виде символьной строки
 string& Properties(string& s, string& head, uint indent = 0) const;
 //Перегрузка операции потокового вывода (<<) свойств класса
 friend ostream& operator <<(ostream& out, const TMultiStageAuxChSRF& SRF)
 {string s; return out << SRF.Properties(s,STR_NULL);};

//------------------------------------------------------------------------------
//Объявление защищенных членов-функций класса TMultiStageAuxChSRF
//------------------------------------------------------------------------------
protected:
 //Начальная установка параметров режекторного фильтра
 void Reset() {Input = 0L; Output = 0L; Q = cmatrix(); trQ = cmatrix();};
};//Завершение объявления класса TMultiStageAuxChSRF
//------------- Завершение объявления класса TMultiStageAuxChSRF ---------------

//******************************************************************************
//          Объявление класса TSingleStageAuxChSRF
//******************************************************************************
//Класс реализует модель ОДНОСТУПЕНЧАТОГО ПРОСТРАНСТВЕННОГО РЕЖЕКТОРНОГО ФИЛЬТРА
//ДОПОЛНИТЕЛЬНЫХ КАНАЛОВ АДАПТИВНОГО ПРОСТРАНСТВЕННОГО  ФИЛЬТРА с ОСНОВНЫМ КАНА-
//ЛОМ. Назначение класса состоит в исключении заданных направлений приема сигна-
//лов  дополнительными каналами адаптивного пространственного фильтра. Режектор-
//ный фильтр представляет собой совокупность одноступенчатых весовых сумматоров,
//количество  которых  равно N. Каждый такой сумматор имеет M-N+1 число входов и
//ОДИН выход (N - число AuxCh+BaseCh, M - RDECh+AuxCh+BaseCh, K = M-N - число
//направлений исключения.
//Разность M-N определяет количество направлений исключения, которое может реали-
//зовать данный фильтр. Каждый рабочий канал фильтра обеспечивает реализацию од-
//ного направления исключения приема сигналов в дополнительных каналах  адаптив-
//ного пространственного фильтра с выделенным каналом. Для расчета коэффициентов
//режекторного фильтра, при конструировании объекта передается комплексная "обу-
//чающая" матрица размерностью MxK, где M+1 - количество входов фильтра, K - чис-
//ло направлений исключения. Число выходов фильтра N = M+1-K. Обучающая  матрица
//представляет  собой K векторов размерностью M. Структура матрица описана в ро-
//дительском классе TAuxChSpaceRejectFilter.
//АЛГОРИТМ Q-ПРЕОБРАЗОВАНИЯ, ВЫЧИСЛЕНИЯ  ВЕСОВЫХ КОЭФФИЦИЕНТОВ  ОДНОСТУПЕНЧАТОГО
//                    ФИЛЬТРА и СХЕМА ХРАНЕНИЯ КОЭФФИЦИЕНТОВ
//Коэффициенты  фильтра  хранятся в матрицах Q и trQ. Матрица trQ является комп-
//лексно-сопряженной к матрице Q и осуществляет вместе с матрицей Q Q-преобразо-
//вание  корреляционной матрицы входного воздействия. Матрица Q является прямоу-
//гольной. Размерность матрицы Q по столбцам:
//1. RDEChannel (каналы LAA используемые для исключения приема сигналов с задан-
//   ных направлений вспомогательными каналами фильтра);
//2. AuxChannel (дополнительные каналы фильтра, подключаемые с LAA);
//3. BaseChannel размерности 1 (основной канал фильтра).
//Размерность матрицы Q по строкам:
//1. AuxChannel (дополнительные каналы фильтра, подключаемые с LAA);
//2. BaseChannel размерности 1 (основной канал фильтра).
//Каждая строка матрицы Q представляет собой коэффициенты одного одноступенчато-
//го весового сумматора, который представляет из себя пространственный режектор-
//ный фильтр на K направлений для ОДНОГО ДОПОЛНИТЕЛЬНОГО канала. Последняя стро-
//ка матрицы Q является вектором весовых коэффициентов для ОСНОВНОГО канала, все
//коэффициенты этой строки, за исключением последнего (равного 1), равны 0, т.к.
//в ОСНОВНОМ канале пространственная фильтрация не должна осуществляться.
//Для осуществления Q-преобразования необходимо вектор входного сигнала умножить
//слева на  матрицу Q: V' = Q*V. Для осуществления Q-преобразования над корреля-
//ционной матрицей входного сигнала R необходимо провести следующую операцию:
//R' = Q*R*trQ.
//Для задания K направлений исключения приема  сигналов с  заданных  направлений
//дополнительными каналами фильтра  используются  K каналов LAA, индексы которых
//не должны совпадать с индексами каналов LAA, которые используются как дополни-
//тельные  каналы пространственного фильтра с выделенным каналом.
//Коэффициенты Space Reject Filter обеспечивают нулевые уровни приема сигналов с
//направлений исключения в AuxCh. Таким образом, обеспечивается  передача сигна-
//лов с направлений исключения только через ОСНОВНОЙ КАНАЛ без его компенсации
//на весовом сумматоре адаптивного фильтра. Методика вычисления коэффициентов
//одноступенчатых режекторных пространственных фильтров следующая:
//1. Формируются сигналы ЕДИНИЧНОЙ МОЩНОСТИ, приходящие с K НАПРАВЛЕНИЙ ИСКЛЮЧЕ-
// НИЯ, и рассчитываются комплексные значения напряжений на каналах LAA,
// подключенных к SingleStageAuxChSRF. К этим каналам относятся RDECh и AuxCh.
// Тестовые сигналы для каналов LAA и ОСНОВНОГО канала (при его подключении),
// которые подключены к AuxChSRF, и заданных направлениях исключения передаются
// в обучающей матрице L. За формирование данной матрицы ответственен класс, ко-
// торый использует объект класса TSingleStageAuxChSRF. Класс TSingleStageAuxChSRF
// выполняет только проверку корректности по размерности матрицы L. Матрица L не
// должна быть нулевой и число строк матрицы должно быть строго больше числа
// столбцов матрицы. В противном случае, объект класса не будет работоспособным.
// Структура обучающей матрицы  для  реализации  пространственного  режекторного
// фильтра  для 4-х  дополнительных  каналов и 3-х направлений  исключения будет
// иметь следующий вид: N = 4; K = 3; M = 7.
//Размерность L: rows = M = 7; cols = K = 3.
//     | U11 U12 U13 |  Uij - комплексное напряжение в i-ом канале при действии
//     | U21 U22 U23 |        одного источника сигнала ЕДИНИЧНОЙ мощности с j-го
//     | U31 U32 U33 |        направления исключения;
// L = | U41 U42 U43 |  U1j..U3j - рабочие каналы AuxChSpaceRejectFilter;
//     | U51 U52 U53 |  U4j..U7j - дополнительные каналы AdaptSpaceFilterBaseCh
//     | U61 U62 U63 |
//     | U71 U72 U73 |
// 2. На основе матрицы L строится система линейных алгебраических уравнений с N
// правыми частями (по числу AuxCh или числу одноступенчатых сумматоров) следую-
// щего вида: A*W = B, где A - подматрица матрицы L с коэффициентами Uij для ра-
// бочих  каналов  фильтра; В - подматрица  матрицы L с  коэффициентами Uij  для
// вспомогательных каналов AuxCh; W - матрица весовых коэффициентов для N одно-
// ступенчатых сумматоров, которая рассчитывается. Для данной матрицы L СЛАУ с
// N правыми частями записывается следующим образом:
// U11*W1 + U21*W2 + U31*W3 = -U41 -U51 -U61 -U71
// U12*W1 + U22*W2 + U32*W3 = -U42 -U52 -U62 -U72
// U13*W1 + U23*W2 + U33*W3 = -U43 -U53 -U63 -U73
// При решении данной системы уравнений получаем коэффициенты матрицы W:
//     | w11 w12 w13 w14 |
// W = | w21 w22 w23 w24 |
//     | w31 w32 w33 w34 |
// Столбцы матрицы W содержат весовые коэффициенты для 4-x одноступенчатых прос-
// ранственных режекторных фильтра, которые обеспечивают по 3 направления исклю-
// чения в 4-х дополнительных  каналах  адаптивного  пространственного фильтра с
// ОСНОВНЫМ каналом.
// Отметим, что для обеспечения правильной работы пространственного режекторного
// фильтра после его настройки, последовательность каналов LAA и BaseCh (при его
// использовании) должна быть аналогичной как и в обучающей матрице L.
// Коэффициенты матрицы W являются частью коэффициентов матрицы Q. Приведем мат-
// рицу Q для схемы AuxChRDEUnit с тремя направлениями исключения для адаптивно-
// го пространственного фильтра с основным каналом и 4 дополнительными каналами.
// Таким образом, RDECh = 3, AuxCh = 4, Dimension Q = (3+4+1)x(4+1) = 8x5
// (с учетом включения ОСНОВНОГО КАНАЛА). Матрица Q будет иметь следующий вид:
//       U0  U1  U2  U3  U4  U5  U6  U7
//     | w11 w21 w31  1   0   0   0   0 | U3
//     | w12 w22 w32  0   1   0   0   0 | U4
// Q = | w13 w23 w33  0   0   1   0   0 | U5
//     | w14 w24 w34  0   0   0   1   0 | U6
//     |  0   0   0   0   0   0   0   1 | U7
// U0..U2 - RDECh, U3..U6 - AuxCh, U7 - BaseChannel.
//С целью минимизации числа неиспользуемых при вычислениях элементов матрицы Q,
//она преобразуется в матрицу Q следующего вида:
//       U0  U1  U2
//     | q00 q01 q02 | U3
//     | q10 q11 q12 | U4
// Q = | q20 q21 q22 | U5
//     | q30 q31 q32 | U6
//Из матрицы первоначальной матрицы Q вырезается  единичная  матрица и последняя
//ее строка, так как отсутствие данных элементов можно легко учесть при реализа-
//ции алгоритма Q-преобразования.
//В качестве  входного  воздействия может быть использованы комплексные корреля-
//ционная матрица R или вектор напряжений U, которые должны содержать компоненту
//ОСНОВНОГО канала адаптивного фильтра. При Q-преобразовании проверяется работо-
//способность фильтра и соответствие размерности входного воздействия размернос-
//ти пространственного режекторного фильтра. При их несоответствии или неработо-
//способности фильтра возвращается вектор или матрица нулевого размера.
//Размерность входного вектора должна быть равна числу входов фильтра. Матрица R
//должна быть квадратной и ее размерность, также, д.б. равна числу входов SRF. В
//качестве выходного значения объект класса возвращает вектор или квадратную мат-
//рицу с размерностями равными числу выходов Space Reject Filter.
//******************************************************************************
class TSingleStageAuxChSRF : public TAuxChSpaceRejectFilter {
//------------------------------------------------------------------------------
// Объявление защищенных членов-данных класса TSingleStageAuxChSRF
//------------------------------------------------------------------------------
protected:
 cmatrix Q;//Матрица Q-преобразования
 cmatrix trQ;//Матрица сопряженно-транспонированная к матрице Q
//------------------------------------------------------------------------------
//ОБЪЯВЛЕНИЕ КОНСТРУКТОРОВ и ДЕСТРУКТОРА КЛАССА TSingleStageAuxChSRF
//------------------------------------------------------------------------------
public:
 //Конструктор по умолчанию
 TSingleStageAuxChSRF() {Reset();};
 //Конструктор с аргументами
 TSingleStageAuxChSRF(const cmatrix& A) {Tuning(A);};
 //Объявление деструктора класса
 ~TSingleStageAuxChSRF() {Reset();};
//------------------------------------------------------------------------------
//Объявление общедоступных членов-функций класса TSingleStageAuxChSRF
//------------------------------------------------------------------------------
public:
 //-----------------------------------------------------------------------------
 //Передаточная функция (TRANSFER FUNCTION) TSingleStageAuxChSRF
 //-----------------------------------------------------------------------------
 bool TFunc(const cmatrix& InR, cmatrix& OutR) const;
 bool TFuncDM(const cmatrix& InR, cmatrix& OutR) const;
 bool TFunc(const cvector& InV, cvector& OutV) const;
 //-----------------------------------------------------------------------------
 //Настройка весовых коэффициентов режекторного пространственного фильтра
 //-----------------------------------------------------------------------------
 bool Tuning(const cmatrix& A);
 //-----------------------------------------------------------------------------
 //Получение весовых коэффициентов TSingleStageAuxChSRF
 //-----------------------------------------------------------------------------
 bool GetW(cmatrix& W, bool lExtend = ON) const;
 //-----------------------------------------------------------------------------
 //Консольный ввод-вывод свойств класса TSingleStageAuxChSRF
 //-----------------------------------------------------------------------------
 uint Type() const {return SingleStageUnit;};//Тип весового сумматора
 //Тип весового сумматора
 string& Type(string& s) const
 {return s = "SingleStage Space Reject Filter of the Auxiliary Channels";};
 //Сокращенное наименование класса
 string& Clip(string& s) const {return s = "SingleStageAuxChSRF";};
 //Формирование свойств весового сумматора в виде символьной строки
 string& Properties(string& s, string& head, uint indent = 0) const;
 //Перегрузка операции потокового вывода (<<) свойств класса
 friend ostream& operator <<(ostream& out, const TSingleStageAuxChSRF& SRF)
 {string s; return out << SRF.Properties(s,STR_NULL);};

//------------------------------------------------------------------------------
//Объявление защищенных членов-функций класса TSingleStageAuxChSRF
//------------------------------------------------------------------------------
protected:
 //Начальная установка параметров режекторного фильтра
 void Reset() {Input = 0L; Output = 0L; Q = cmatrix(); trQ = cmatrix();};
};//Завершение объявления класса TSingleStageAuxChSRF
//------------- Завершение объявления класса TSingleStageAuxChSRF --------------

//******************************************************************************
//Объявление класса - TInDataBaseCh - входных параметров схемы формирования
//основного канала для адаптивных пространственных фильтров с выделенным каналом
//на основе LAA
//******************************************************************************
class TInDataBaseCh {
 //-----------------------------------------------------------------------------
 // ОБЪЯВЛЕНИЕ ЗАЩИЩЕННЫХ ЧЛЕНОВ-ДАННЫХ КЛАССА TInDataBaseCh
 //-----------------------------------------------------------------------------
 protected:
 //-----------------------------------------------------------------------------
 //Входной сигнал с каналов LAA в BaseChannel
  TInputSignal* OutLAA; //Указатель на входное воздействие с LAA
 //-----------------------------------------------------------------------------
 //Индексы каналов LAA, подключенных к схеме формирования основного канала.
 //К BaseChannel может подключаться от 1 до InSignal->LAAElem() каналов LAA.
 ivector Channel;
 //Характеристики лучеобразующей схемы адаптивного фильтра - BFC.
 //BFC используется, если Channel.Size() > 1.
 double ScanAngle;//Направление угла сканирования синтезированного луча [deg]
 //-----------------------------------------------------------------------------
 // I. ОБЪЯВЛЕНИЕ ОБЩЕДОСТУПНЫХ ЧЛЕНОВ-ФУНКЦИЙ КЛАССА TInDataBaseCh
 //-----------------------------------------------------------------------------
 public:
 //*****************************************************************************
 // ОБЪЯВЛЕНИЕ КОНСТРУКТОРОВ И ДЕСТРУКТОРА КЛАССА TInDataBaseCh
 //*****************************************************************************
 TInDataBaseCh();//Объявление конструктора по умолчанию
 //Объявление конструктора с аргументами
 TInDataBaseCh(TInputSignal* InSignal);
 TInDataBaseCh(TInputSignal* InSignal, const ivector& Index, double Angle = 0);
 //Объявление конструктора копирования
 TInDataBaseCh(TInDataBaseCh& InDataBaseCh) {*this = InDataBaseCh;};
 //Объявление деструктора класса
 ~TInDataBaseCh();
 //*****************************************************************************
 // ОБЩЕДОСТУПНЫЕ ФУНКЦИИ ДЛЯ РАБОТЫ С КЛАССОМ TInDataBaseCh
 //*****************************************************************************
 //Проверка работоспособности ОСНОВНОГО КАНАЛА:
 bool IsReady() const {return (OutLAA == NULL) ? OFF : ON;};
 //Установка свойств ОСНОВНОГО КАНАЛА
 void Set(TInputSignal* InSignal, const ivector& Index = ivector(), double Angle = 0,
          bool Check = OFF);
 //Получение свойств класса в виде строкового объекта string
 string& Properties(string &s, string& title, uint indent = 0) const;
 //Получение номеров каналов LAA, подключенных к ОСНОВНОМУ КАНАЛУ
 string& Channels(string& s, ulong count, char* sep = NULL, char* etc = NULL) const
         {return Channel.ConvertToString(s,count,sep,etc);};
 //Определение использования лучеобразующей схемы - BeamFormerCircuit
 bool UsageBFC() const {return (Channel.Size() > 1L) ? ON : OFF;};
 //Пространственное положение главного лепестка основного канала
 double GetScanAngle() const {return ScanAngle;};
 //Индексы каналов LAA, подключенные к BaseChannel
 ulong Channels(ivector& BaseCh) const
 {BaseCh = Channel; return BaseCh.Size();};
 const ivector& Channels() const {return Channel;};
 //Индекс канала LAA, подключенный к BaseChannel, соответствующий его порядковому
 //номеру в векторе Channel
 long IndexLAACh(ulong number = 0) const
 {return (!IsReady() || (number >= Channel.Size())) ? -1L : Channel[number];};
 //Указатель на LAA, с которой подключена схема формирования ОСНОВНОГО КАНАЛА
 TInputSignal* GetLAA() const {return OutLAA;};
 //*****************************************************************************
 // ОБЩЕДОСТУПНЫЕ ФУНКЦИИ ИСХОДНЫХ ДАННЫХ ДЛЯ ФОРМИРОВАНИЯ ОСНОВНОГО КАНАЛА
 // 1. ФУНКЦИИ ВВОДА - ВЫВОДА ПАРАМЕТРОВ TInDataBaseCh
 //*****************************************************************************
 //Ввод параметров объекта из стандартного потока ввода cin
 bool Edit();
 //Перегрузка оператора ввода (>>) для ввода параметров класса
 friend istream& operator >>(istream& in, TInDataBaseCh& Obj);
 //Перегрузка оператора вывода (<<) для вывода параметров класса
 friend ostream& operator <<(ostream& out, const TInDataBaseCh& Obj)
 {string s; return out << Obj.Properties(s,STR_NULL);};
 //Объявление перегруженного оператора присваивания operator =
 void operator =(const TInDataBaseCh& InDataBaseCh);
 //-----------------------------------------------------------------------------
 // II. ОБЪЯВЛЕНИЕ ЗАЩИЩЕННЫХ ЧЛЕНОВ-ФУНКЦИЙ КЛАССА TInDataBaseCh
 //-----------------------------------------------------------------------------
 protected:
  void Reset(); //Начальная установка всех характеристик TInDataBaseCh
  //Ввод начальных параметров ОСНОВНОГО КАНАЛА со стандартного потока ввода
  bool GetDataFromConsole(ivector& LAAChannel, double& Angle);
};
//Конец объявления класса TInDataBaseCh
//The end of class TInDataBaseCh declaration
//----------------------------------------------------------------------------------------
//      СХЕМА ФОРМИРОВАНИЯ ОСНОВНОГО КАНАЛА АДАПТИВНОЙ LAA - TBaseChannel
//----------------------------------------------------------------------------------------
//    Схема формирования основного канала реализуется классом TBaseChannel. В качестве ос-
//новного канала может использоваться любой канал (антенный элемент) LAA или СИНТЕЗИРОВАН-
//НЫЙ ОСНОВНОЙ КАНАЛ. Синтезированный основной канал образуется подключением от 1 до N ка-
//налов LAA к лучеобразующей  схеме - BeamFormer Circuit (BFC). Это  позволяет сформировать
//ГЛАВНЫЙ ЛУЧ (основной лепесток) LAA различной ширины и произвольным пространственным по-
//ложением от -90 до +90 градусов. Ширина синтезированного луча будет определяться числом
//антенных элементов, подключенных к BFC, и пространственным положением луча. При отклоне-
//нии главного луча от 0 градусов он расширяется. Сканирование луча в диапазоне от -90 до
//+90 градусов достигается умножением комплексных значений напряжений каждого приемного ка-
//нала LAA на соответствующий комплексно-сопряженный фазосдвигающий коэффициент EXP(jPHIi)
//BFC. При использовании одного канала LAA как ОСНОВНОГО можно также задать произвольное
//направление сканирования. Но в этом случае коэффициент направленного действия антенного
//элемента LAA в произвольном направлении может оказаться очень маленьким либо главный луч,
//формируемый одним элементом LAA может оказаться слишком широким, если данный элемент об-
//ладает ненаправленной диаграммой направленности в данной плоскости сканирования. Поэтому
//в случае применения одного антенного элемента в качестве основного канала LAA лучше всего
//задавать нулевое положение ГЛАВНОГО ЛУЧА относительно нормали к раскрыву LAA.
//    Лучеобразующая схема формирования основного канала LAA реализована как параллельный
//весовой сумматор. В качестве весов выступают фазосдвигающие коэффициенты каждого канала
//LAA подключенного к BFC, которые формируют угол сканирования. После перемножения напряже-
//ний приемных каналов LAA, подключенных к BFC, на соответствующие каналам фазосдвигающие
//коэффициенты, полученные произведения суммируются на общем сумматоре, выходное значение
//которого определяет комплексные значения напряжений аддитивной помехи, полезного сигнала
//и внутреннего шума ОСНОВНОГО КАНАЛА. ВЫХОДНЫЕ ЗНАЧЕНИЯ компонент входного сигнала СХЕМЫ
//ФОРМИРОВАНИЯ ОСНОВНОГО КАНАЛА LAA ОПРЕДЕЛЯЮТ ЗНАЧЕНИЯ аддитивной помехи, сигнала и внут-
//реннего шума НА ВХОДЕ адаптивной LAA.
//    Коэффициенты PHIi определяют фазовый сдвиг в i-ом канале LAA, подключенном к BFC,
//для формирования заданного угла сканирования по азимуту или по углу места LAA. Комплексно-
//сопряженный фазосдвигающий коэффициент вычисляется по следующей формуле:
// PHIi = EXP(-2*PI*LVi*sin(ScanAngle)*j), где
// ScanAngle - заданный угол сканирования АР (направление главного лепестка);
// LVi - относительное расстояние между 0 и i-м каналом антенной решетки;
// j - мнимая единица => sqrt(-1).
// Фазовый центр синтезированного луча совпадает с 0-ым каналом LAA. К BFC для формирования
//СИНТЕЗИРОВАННОГО ЛУЧА, можно подключать произвольные каналы LAA с индексами от 0 до (N-1),
//где N - количество антенных элементов LAA.
//----------------------------------------------------------------------------------------
//****************************************************************************************
//Объявление класса - TBaseChannel - схема формирования основного канала для адаптивного
//пространственного фильтра с выделенным каналом на основе LAA
//****************************************************************************************
class TBaseChannel : public TInDataBaseCh {
 //---------------------------------------------------------------------------------------
 // ОБЪЯВЛЕНИЕ ЗАЩИЩЕННЫХ ЧЛЕНОВ-ДАННЫХ КЛАССА TBaseChannel
 //---------------------------------------------------------------------------------------
 protected:
  cvector B;//Лучеобразующий вектор, задающий направление сканирования луча
 //-----------------------------------------------------------------------------
 // I. ОБЪЯВЛЕНИЕ ОБЩЕДОСТУПНЫХ ЧЛЕНОВ-ФУНКЦИЙ КЛАССА TBaseChannel
 //-----------------------------------------------------------------------------
 public:
 //*****************************************************************************
 // ОБЪЯВЛЕНИЕ КОНСТРУКТОРОВ И ДЕСТРУКТОРА КЛАССА TBaseChannel
 //*****************************************************************************
 TBaseChannel();//Объявление конструктора по умолчанию
 //Объявление конструктора с аргументами
 TBaseChannel(TInputSignal* InSignal);
 TBaseChannel(TInputSignal* InSignal, const ivector& Index, double Angle = 0);
 TBaseChannel(const TInDataBaseCh& InData) {*this = InData;};
 //Объявление конструктора копирования
 TBaseChannel(TBaseChannel& BaseCh) {*this = BaseCh;};
 //Объявление деструктора класса
 ~TBaseChannel();
 //*****************************************************************************
 // ОБЩЕДОСТУПНЫЕ ФУНКЦИИ ДЛЯ РАБОТЫ С КЛАССОМ TBaseChannel
 //*****************************************************************************
 //Получение лучеобразующего вектора
 cvector BeamFormerVector() const {return B;};
 //Установка свойств ОСНОВНОГО КАНАЛА
 void Set(TInputSignal* InSignal, const ivector& Index = ivector(),
          double Angle = 0, bool Check = OFF);
 void Set(const TInDataBaseCh& InData) {*this = InData;};
 //Получение свойств класса в виде строкового объекта string
 //string& GetPerformance(string &s) const;
 //Вычисление диаграммы направленности ОСНОВНОГО КАНАЛА в заданном секторе в плоскости
 //расположения LAA относительно поверхности Земли (по азимуту или углу места)
 rvector AntPatterns(double InitAngle = -90, double EndAngle = 90,
         double Step = 0.1, bool Deg = DEG, bool Norm = ON) const;
 //Передаточная функция ОСНОВНОГО КАНАЛА (диаграмма направленности)
 rvector& DirectPattern(rvector& P, const TEvenCloseInterval& Sector,
          bool Norm = ON, bool Db = ON, double MinDb = -60.0) const;
 //Вычисление направленности ОСНОВНОГО КАНАЛА адаптивной LAA в заданном направлении в
 //плоскости ее расположения относительно поверхности Земли
 double Directivity(double Angle, bool Deg = DEG) const;
 //Вычисление ширины луча ОСНОВНОГО КАНАЛА по заданному уровню мощности в градусах
 //double BeamWidth(double PowerLevel = 0.5, double Prec = 0.01);
 //*****************************************************************************
 // ОБЩЕДОСТУПНЫЕ ФУНКЦИИ РАСЧЕТА ВЫХОДНЫХ ЗНАЧЕНИЙ ОСНОВНОГО КАНАЛА
 //*****************************************************************************
 //1. OVERLOAD THE CALL FUNCTION OPERATOR ()
 //Перегрузка оператора вызова функции ()
 //Передаточная функция ОСНОВНОГО КАНАЛА при действии сигнала ЕДИНИЧНОЙ МОЩНОСТИ
 //с заданного направления
 bool TFunc(cvector& Out, const rvector& Angle, bool Abs = ON) const;
 complex operator() (double Angle, bool Deg = DEG, bool Abs = ON) const;
 //Расчет компонент входного воздействия на выходе ОСНОВНОГО канала для
 //АНАЛИТИЧЕСКОГО МОДЕЛИРОВАНИЯ
 complex CalcSignal(bool PolarType) const;
 complex CalcNoise(bool PolarType) const;
 void CalcJammers(cvector& Jammers, bool PolarType) const;

 //Расчет компонент входного воздействия на выходе ОСНОВНОГО канала для
 //СТАТИСТИЧЕСКОГО МОДЕЛИРОВАНИЯ
 complex CalcStatSignal(bool PolarType) const;
 complex CalcStatNoise(bool PolarType) const;
 complex CalcStatJammer(bool PolarType) const;

 //*****************************************************************************
 // ОБЩЕДОСТУПНЫЕ ФУНКЦИИ ВВОДА - ВЫВОДА ПАРАМЕТРОВ TBaseChannel
 //*****************************************************************************
 //Ввод параметров объекта из стандартного потока ввода cin
 bool Edit();
 //Перегрузка оператора ввода (>>) для ввода параметров класса
 //friend istream& operator >>(istream& in, TBaseChannel& BaseCh);
 //Перегрузка оператора вывода (<<) для вывода параметров класса
 //friend ostream& operator <<(ostream& out, const TBaseChannel& BaseCh)
 //Объявление перегруженного оператора присваивания operator =
 void operator =(const TBaseChannel& BaseCh);
 //Объявление перегруженного оператора присваивания operator = ОСНОВНОМУ КАНАЛУ
 //объекта с его исходными параметрами TInDataBaseCh
 void operator =(const TInDataBaseCh& InData);
 //-----------------------------------------------------------------------------
 // II. ОБЪЯВЛЕНИЕ ЗАКРЫТЫХ ЧЛЕНОВ-ФУНКЦИЙ КЛАССА TBaseChannel
 //-----------------------------------------------------------------------------
 private:
  void Reset(); //Начальная установка всех характеристик TBaseChannel
  //Расчет коэффициентов лучеобразующей схемы - BeamFormer - вектор B
  void CalcBeamFormerFactor();
};
//Конец объявления класса TBaseChannel
//The end of class TBaseChannel declaration

//****************************************************************************************
//            Объявление абстрактного класса - TPreProcessorSpaceFilterBaseCh
//Класс  описывает  общие  свойства  для  аналитической и статистической моделей предпро-
//цессора обработки входного сигнала пространственного адаптивного фильтра с выделенным
//каналом на основе LAA. Класс моделирует следующие блоки предпроцессора:
//1. Коммутатор каналов LAA. Осуществляет подключение каналов LAA с заданными индексами к
//устройству формирования ВЫДЕЛЕННОГО (ОСНОВНОГО) КАНАЛА ФИЛЬТРА, к устройству формирования
//ВСПОМОГАТЕЛЬНЫХ КАНАЛОВ ФИЛЬТРА и к устройству исключения приема сигналов с заданных
//направлений ВСПОМОГАТЕЛЬНЫМИ КАНАЛАМИ (в частном случае, СХЕМА ЗАЩИТЫ ГЛАВНОГО ЛУЧА) в
//случае его использования.
//АЛГОРИТМ КОММУТАЦИИ каналов следующий. Если СХЕМА ФОРМИРОВАНИЯ ВЫДЕЛЕННОГО КАНАЛА исполь-
//зует лучеобразующую схему, что возможно при подключении к BaseCh только нескольких кана-
//лов, то в качестве индексов каналов LAA, используемых в качестве ДОПОЛНИТЕЛЬНЫХ КАНАЛОВ
//пространственного фильтра, можно использовать ВСЕ КАНАЛЫ LAA. Если BaseCh использует только
//ОДИН КАНАЛ LAA, то в качестве ДОПОЛНИТЕЛЬНЫХ КАНАЛОВ SpaceFilter with BaseCh канал,
//подключенный к BaseCh использовать нельзя, а все остальные каналы LAA можно. Таким образом,
//при использовании схемы формирования СИНТЕЗИРОВАННОГО ЛУЧА каналы BaseCh и AuxCh могут
//иметь общие индексы - пересекаться. При подключении схемы формирования направлений исклю-
//чения приема сигналов дополнительными каналами SpaceFilter для работы данного устройства
//задействуются каналы LAA, которые не используются в качестве дополнительных каналов фильт-
//ра. Таким образом, каналы устройства исключения направлений приема сигналов дополнитель-
//ными каналами фильтра не должны пересекаться с AuxCh.
//2. Устройство формирования ОСНОВНОГО КАНАЛА пространственного фильтра. Данная схема моде-
//лируется классом TBaseChannel. Для формирования основного канала может использоваться лу-
//чеобразующая схема (при подключении нескольких каналов LAA), что позволяет формировать
//заданную диаграмму направленности основного луча в пределах -90 до +90 градусов с задан-
//ной шириной луча, которая зависит от количества подключаемых к BaseCh каналов LAA. Кроме
//этого, в качестве основного канала может быть использован ОДИН произвольный канал LAA. В
//этом случае диаграмма направленности определяется данным антенным элементом LAA и не может
//быть изменена. Класс TBaseChannel используеться аналитической и статистической моделями
//предпроцессора пространственного фильтра с выделенным каналом. Для аналитической модели,
//он осуществляет линейное преобразование (при использовании лучеобразующей схемы) каждого
//источника помехи отдельно. Для статистической модели, он осуществляет линейное преобразо-
//вание суммарной аддитивной помехи. Аналогично TBaseChannel осуществляет линейное преобра-
//зование для полезного сигнала и внутреннего шума. Так как полезный сигнал и внутренний шум
//представлены одним вектором как в аналитической, так и в статистической моделях, то для
//них линейное преобразование однотипно.
//3. Входные параметры устройства исключения направлений приема вспомогательными каналами.
//К ним относятся направления исключения приема сигналов AuxCh и каналы LAA, подключенные
//к данной схеме, индексы которых не пересекаются с AuxCh.
//Реализация Signal Reception Directions Elimination Unit зависит от аналитической или ста-
//тистической моделей предпроцессора. Так как в качестве входных данных аналитической моде-
//ли фильтра используются корреляционные матрицы полезного сигнала, активной помехи и внут-
//реннего шума между всеми его каналами, а входными данными для статистической модели - век-
//торы полезного сигнала, активной помехи и внутреннего шума, которым соответствуют каналы
//LAA и ОСНОВНОЙ КАНАЛ.
//Следует отметить, что каналы ReceptDirectElimUnit не влияют на размерность пространствен-
//ного фильтра, так как на его вход подключаются только ОСНОВНОЙ КАНАЛ и AuxCh.
//В модели адаптивной антенной решетки предусматривается пространственно-поляризационная
//селекция. С этой целью пространственный адаптивный фильтр может принимать одну из следую-
//щих поляризационных составляющих входного сигнала:
//1. Вертикально-поляризованную компоненту сигнала - VP_SIGNAL;
//2. Горизонтально-поляризованную компоненту сигнала - HP_SIGNAL;
//****************************************************************************************
class TPreProcessorSpaceFilterBaseCh {
//------------------------------------------------------------------------------
// ОБЪЯВЛЕНИЕ ЗАЩИЩЕННЫХ ЧЛЕНОВ-ДАННЫХ КЛАССА TPreProcessorSpaceFilterBaseCh
//------------------------------------------------------------------------------
 protected:
  //Входной сигнал с каналов LAA на входе предпроцессора адаптивного пространственного
  //фильтра с выделенным каналом
  TInputSignal* OutLAA;
  //Поляризационная составляющая входного воздействия принимаемого адаптивным
  //пространственным фильтром
  bool PolarType;
  //Устройство формирования ОСНОВНОГО канала AdaptSpaceFilterBaseCh
  TBaseChannel BaseChannel;
  //****************************************************************************
  //Устройство формирования направлений исключения приема сигналов с направлений
  //DirectElim вспомогательными каналами SpaceFilter - AuxChRDEUnit
  //****************************************************************************
  //Входные (задаваемые) параметры схемы исключения направлений приема сигналов
  //дополнительными каналами пространственного фильтра
  rvector DirectElim; //Направления исключения
  bool lBaseChInRDEUnit; //Подключение ОСНОВНОГО КАНАЛА к AuxChRDEUnit
  //Тип схемной реализации алгоритма исключения заданных направлений приема:
  //1. MultiStageUnit - многоступенчатая схема;
  //2. SingleStageUnit - одноступенчатая схема.
  //Указатель на базовый класс устройств по исключению приема сигналов с заданных направ-
  //лений в дополнительных каналах адаптивных пространственных фильтрах с ОСНОВНЫМ каналом
  TAuxChSpaceRejectFilter* RDEUnit;
  //**************************************************************************************
  //Индексы каналов LAA (RDEChannel + AuxChannel), используемые в качестве рабочих каналов
  //AuxChRDEUnit и вспомогательных каналов пространственного фильтра без учета индексов
  //каналов LAA, используемых для формирования ОСНОВНОГО КАНАЛА
  ivector LAACh;
//----------------------------------------------------------------------------------------
// I. ОБЪЯВЛЕНИЕ ОБЩЕДОСТУПНЫХ ЧЛЕНОВ-ФУНКЦИЙ КЛАССА
//    TPreProcessorSpaceFilterBaseCh
//----------------------------------------------------------------------------------------
 public:
//*****************************************************************************
// ОБЪЯВЛЕНИЕ КОНСТРУКТОРОВ И ДЕСТРУКТОРА КЛАССА TPreProcessorSpaceFilterBaseCh
//*****************************************************************************
 TPreProcessorSpaceFilterBaseCh();//Объявление конструктора по умолчанию
 //Объявление конструктора с аргументами
 TPreProcessorSpaceFilterBaseCh(TInputSignal* InSignal, bool Polar = VP_SIGNAL,
  const ivector& BaseCh = ivector(), double ScanAngle = 0, const ivector& AuxCh = ivector(),
  const rvector& Directs = rvector(), bool lBaseChInRDE = OFF, ivector& RDECh = ivector(),
  uint AuxChSRF = MultiStageUnit);
 TPreProcessorSpaceFilterBaseCh(TInputSignal* InSignal, bool Polar,
  const TInDataBaseCh& BaseChProp, const ivector& AuxCh = ivector(),
  const rvector& Directs = rvector(), bool lBaseChInRDE = OFF, ivector& RDECh = ivector(),
  uint AuxChSRF = MultiStageUnit);

 //Объявление конструктора копирования
 TPreProcessorSpaceFilterBaseCh(const TPreProcessorSpaceFilterBaseCh& Property)
 {*this = Property;};
 //Объявление деструктора класса
 ~TPreProcessorSpaceFilterBaseCh();
//******************************************************************************
// ОБЩЕДОСТУПНЫЕ ФУНКЦИИ ДЛЯ РАБОТЫ С КЛАССОМ TPreProcessorSpaceFilterBaseCh
//******************************************************************************
 //Установка свойств предпроцессора SpaceFilterBaseCh
 bool Set(TInputSignal* InSignal, bool Polar = VP_SIGNAL, const ivector& BaseCh = ivector(),
      double ScanAngle = 0, const ivector& AuxCh = ivector(), const rvector& Directs = rvector(),
      bool lBaseChInRDE = OFF,  ivector& RDECh = ivector(), uint AuxChSRF = MultiStageUnit);

 bool Set(TInputSignal* InSignal, bool Polar, const TInDataBaseCh& BaseChProp,
      const ivector& AuxCh = ivector(), const rvector& Directs = rvector(),
      bool lBaseChInRDE = OFF, ivector& RDECh = ivector(), uint AuxChSRF = MultiStageUnit);
 //Проверка работоспособности предпроцессора:
 bool IsReady() const
  {return ((OutLAA == NULL) || (OutLAA->LAAElem() < 2) || (!BaseChannel.IsReady())) ? OFF : ON;};
  //Указатель на LAA, к которой подключен предпроцессор фильтра
 TInputSignal* GetLAA() const {return OutLAA;};
 //Расположение LAA относительно поверхности Земли
 bool DispLAA() const {return (IsReady()) ? OutLAA->GetDisp() : EQUATOR;};
 //Поляризационная составляющая входного воздействия, принимаемая предпроцессором
 bool Polarization() const {return PolarType;};
 //Установка поляризационной составляющей (VP_SIGNAL || HP_SIGNAL)
 bool SetPolar(bool Polar) {return PolarType = Polar;};
 //Размерность пространственного фильтра с ОСНОВНЫМ КАНАЛОМ
 ulong FilterSize() const {return (IsReady()) ? AuxChCount()+1 : 0L;};
 //Количество входов предпроцессора с ОСНОВНЫМ каналом
 ulong In() const {return (IsReady()) ? AuxChCount()+RDEChCount()+1 : 0L;};
 //Количество выходов предпроцессора с ОСНОВНЫМ каналом
 ulong Out() const {return (IsReady()) ? AuxChCount()+1 : 0L;};
 //Формирование входного тест-сигнала предпроцессора
 bool TestSignal(cvector& V, double Direct, bool lBaseCh = ON, bool lAbs = ON) const;
 bool TestSignal(cmatrix& A, const rvector& Directs, bool lBaseCh = ON, bool lAbs = ON) const;
 //Формирование входной корреляционной матрицы тест-сигнала предпроцессора
 bool TestR(cmatrix& R, double Direct, bool lBaseCh = ON) const;
 //Передаточная функция (Transfer function) предпроцессора адаптивной LAA
 //с пространственным фильтром и ОСНОВНЫМ каналом /выход AuxCh + BaseChannel/
 bool TFunc(rmatrix& P, const TEvenCloseInterval& Sector, bool Norm = ON, bool dB = ON,
      double MinDb = -60.0) const;
 bool TFunc(cvector& Uout, double Direct, bool lAbs = ON) const;
 bool TFunc(cmatrix& Rout, double Direct) const;
 //Количество дополнительных каналов фильтра - AuxChannelCount
 ulong AuxChCount() const;
 //Формирование индексов дополнительных каналов фильтра
 ulong GetAuxCh(ivector& AuxCh) const;
 //-----------------------------------------------------------------------------
 //Свойства BaseChannel - ОСНОВНОГО канала адаптивной LAA
 //-----------------------------------------------------------------------------
 //Передаточная функция ОСНОВНОГО КАНАЛА (диаграмма направленности)
 rvector& TF_BaseCh(rvector& P, const TEvenCloseInterval& Sector, bool Norm = ON,
  bool dB = ON, double MinDb = -60.0) const
  {if (!IsReady()) P.Resize(0L);
   else BaseChannel.DirectPattern(P,Sector,Norm,dB,MinDb);
   return P;
  };
 //Угол сканирования ОСНОВНОГО канала
 double GetScanAngle() const {return BaseChannel.GetScanAngle();};
 //Индексы каналов LAA, формирующие ОСНОВНОЙ канал
 ulong GetBaseCh(ivector& BaseCh) const
 {BaseChannel.Channels(BaseCh); return BaseCh.Size();};
 //-----------------------------------------------------------------------------
 //Свойства AuxChRDEUnit - устройство исключения заданных направлений приема
 //источников сигналов дополнительными каналами пространственного фильтра
 //-----------------------------------------------------------------------------
 //Использование схемы формирования направлений исключения приема сигналов
 //вспомогательными каналами пространственного фильтра в предпроцессоре
 bool UsageRDEUnit() const {return (DirectElim.Size() == 0L) ? OFF : ON;};
 //Подключение ОСНОВНОГО канала к AuxChRDEUnit
 bool IsBaseChInRDEUnit() const {return lBaseChInRDEUnit;};
 //Количество каналов для компенсации приема сигналов в дополнительных каналах
 ulong RDEChCount() const {return DirectElim.Size();};
 //Номера каналов LAA и ОСНОВНОГО КАНАЛА (lBaseChInRDEUnit = ON), используемые
 //AuxChRDEUnit в качестве рабочих каналов.
 ulong GetRDECh(ivector& RDECh) const;
 //Значения направлений исключения
 ulong GetDirectElim(rvector& Directs) const
 {Directs = DirectElim; return Directs.Size();};
 //Тип AuxChRDEUnit
 uint RDEType() const {return (UsageRDEUnit()) ? RDEUnit->Type() : 0;};
 //Весовые коэффициенты AuxChRDEUnit
 bool RDEUnitFactors(cmatrix& W, bool lExtend = ON) const
 {if (!UsageRDEUnit()) {W.Set(0L,0L,false); return false;}
  RDEUnit->GetW(W,lExtend); return true;
 }
//***************************************************************************************
// ОБЩЕДОСТУПНЫЕ ФУНКЦИИ КЛАССА TPreProcessorSpaceFilterBaseCh
// 1. ФУНКЦИИ ВВОДА - ВЫВОДА ПАРАМЕТРОВ
//***************************************************************************************
 //Получение свойств класса в виде строкового объекта string
 string& Properties(string& s, string& title, uint indent = 0) const;
 string& Polarization(string& s, string& title, uint indent = 0) const;
 string& BaseChProp(string& s, string& title, uint indent = 0) const
 {return BaseChannel.Properties(s,title,indent);};
 string& AuxChProp(string& s, string& title, uint indent = 0) const;
 string& RDEUnitProp(string& s, string& title, uint indent = 0) const;

 //Ввод параметров объекта из стандартного потока ввода cin
 void Edit();
 //Перегрузка оператора ввода (>>) для ввода параметров класса
 friend istream& operator >>(istream& in, TPreProcessorSpaceFilterBaseCh& Preprocessor);
 //Перегрузка оператора вывода (<<) для вывода параметров класса
 friend ostream& operator <<(ostream& out, const TPreProcessorSpaceFilterBaseCh& Preprocessor)
 {string s; return out << Preprocessor.Properties(s,STR_NULL);};
 //Объявление перегруженного оператора присваивания operator =
 void operator =(const TPreProcessorSpaceFilterBaseCh& Preprocessor);

//------------------------------------------------------------------------------
//II. ОБЪЯВЛЕНИЕ ЗАКРЫТЫХ ЧЛЕНОВ-ФУНКЦИЙ КЛАССА TPreProcessorSpaceFilterBaseCh
//------------------------------------------------------------------------------
 private:
  void Initial();//Начальная установка параметров класса при конструировании
  void InitRDEUnit();//Начальная установка AuxChRDEUnit
  void ResetRDEUnit();//Сброс всех характеристик AuxChRDEUnit
  //Установка индексов каналов LAA, используемых как ДОПОЛНИТЕЛЬНЫЕ каналы
  //пространственного адаптивного фильтра с ОСНОВНЫМ каналом
  void SetAuxCh(const ivector& Channel);
  //***************************************************************************
  //Установка параметров AuxChRDEUnit
  //***************************************************************************
  //Установка опции использования BaseChannel в AuxChRDEUnit
  bool SetUsageBaseChInRDEUnit(bool lUsageBaseCh)
  {return lBaseChInRDEUnit = lUsageBaseCh;};
  //Определение числа каналов LAA, которые могут быть использованы в AuxChRDEUnit
  //как РАБОЧИЕ КАНАЛЫ
  ulong AmountLAAChForRDEUnit() const;
  //Формирование индексов каналов LAA, которые могут быть использованы в
  //AuxChRDEUnit как РАБОЧИЕ КАНАЛЫ
  ulong GetFreeChForRDEUnit(ivector& RDECh) const;
  //Установка направлений исключения приема сигналов ДОПОЛНИТЕЛЬНЫМИ каналами
  bool SetDirectElim(const rvector& Directs);
  //Автоопределение индексов каналов LAA, используемых как РАБОЧИЕ КАНАЛЫ
  //AuxChRDEUnit
  ulong AutoDefineRDECh(ivector& RDECh) const;
  //Установка индексов каналов LAA, используемых как РАБОЧИЕ КАНАЛЫ AuxChRDEUnit
  ulong SetRDECh(ivector& RDECh = ivector());
  //Установка типа устройства компенсации приема сигналов с заданных направлений
  //в дополнительных каналах адаптивного пространственного фильтра с BaseCh
  uint SetTypeRDEUnit(uint UnitType);

  void PolarEdit();//Консольный ввод поляризационной составляющей InSignal
  void AuxChEdit();//Консольный ввод индексов дополнительных каналов фильтра
  void AuxChRDEUnitEdit();//Консольный ввод параметров схемы AuxChRDEUnit

//------------------------------------------------------------------------------
//III. ОБЪЯВЛЕНИЕ ЗАЩИЩЕННЫХ ЧЛЕНОВ-ФУНКЦИЙ КЛАССА TPreProcessorSpaceFilterBaseCh
//------------------------------------------------------------------------------
 protected:
  void Reset(); //Сброс всех характеристик класса
};
//Конец объявления класса TPreProcessorSpaceFilterBaseCh
//The end of class TPreProcessorSpaceFilterBaseCh declaration
//******************************************************************************
//              Объявление класса - TAnalPreProcSFilterBaseCh
//Аналитическая  модель предпроцессора обработки входного сигнала пространствен-
//ного адаптивного фильтра с выделенным каналом на основе LAA.
//Класс наследует свойства класса TPreProcessorSpaceFilter. Он выполняет функции
//аналогичные базовому классу. Основной его функцией является формирование вход-
//ной выборки каждой компоненты внешнего воздействия для адаптивного пространст-
//венного  фильтра с выделенным каналом. С учетом специфики аналитического моде-
//лирования входная выборка  аддитивной  активной помехи формируется для каждого
//источника помехи отдельно и заносится в матрицу Jammer. Входные выборки полез-
//ного  сигнала и  внутреннего  шума формируются в векторах Signal и Noise соот-
//ветственно.
//******************************************************************************
class TAnalPreProcSFilterBaseCh : public TPreProcessorSpaceFilterBaseCh {
//------------------------------------------------------------------------------
// ОБЪЯВЛЕНИЕ ЗАКРЫТЫХ ЧЛЕНОВ-ДАННЫХ КЛАССА TAnalPreProcSFilterBaseCh
//------------------------------------------------------------------------------
 private:
  // [BaseChannel] + RDECh == DirectElim.Size(),
  // [BaseChannel] -> ОСНОВНОЙ КАНАЛ подключен к AuxChRDEUnit
  //----------------------------------------------------------------------------
  //Выборка ПОЛЕЗНОГО СИГНАЛА на входе с каналов LAA, подключенных к предпроцес-
  //сору, и ОСНОВНОГО канала. Размерность вектора Signal:
  //1) AuxChRDEUnit = ON:  [BaseChannel] + RDECh + AuxCh + BaseChannel
  //2) AuxChRDEUnit = OFF: AuxCh + BaseChannel
  cvector Signal;
  //----------------------------------------------------------------------------
  //Выборка ВНУТРЕННЕГО ШУМА на входе с каналов LAA, подключенных к предпроцес-
  //сору, и ОСНОВНОГО канала. Размерность вектора Noise:
  //1) AuxChRDEUnit = ON:  [BaseChannel] + RDECh + AuxCh + BaseChannel
  //2) AuxChRDEUnit = OFF: AuxCh + BaseChannel
  cvector Noise;
  //----------------------------------------------------------------------------
  //Выборка АДДИТИВНОЙ АКТИВНОЙ ПОМЕХИ на входе с каналов LAA, подключенных к
  //предпроцессору, и ОСНОВНОГО канала. Размерность матрицы Jammer:
  //Количество строк матрицы:
  //1) AuxChRDEUnit = ON:  [BaseChannel] + RDECh + AuxCh + BaseChannel
  //2) AuxChRDEUnit = OFF: AuxCh + BaseChannel
  //Количество столбцов матрицы:
  //OutLAA->ActiveJammer() - кол-во активных источников помех
  cmatrix Jammer;
//------------------------------------------------------------------------------
// I. ОБЪЯВЛЕНИЕ ОБЩЕДОСТУПНЫХ ЧЛЕНОВ-ФУНКЦИЙ КЛАССА TAnalPreProcSFilterBaseCh
//------------------------------------------------------------------------------
 public:
//*****************************************************************************
// ОБЪЯВЛЕНИЕ КОНСТРУКТОРОВ И ДЕСТРУКТОРА КЛАССА TAnalPreProcSFilterBaseCh
//*****************************************************************************
 //Объявление конструктора по умолчанию
 TAnalPreProcSFilterBaseCh() : TPreProcessorSpaceFilterBaseCh() {};
 //Объявление конструктора с аргументами
 TAnalPreProcSFilterBaseCh(TInputSignal* InSignal, bool Polar = VP_SIGNAL,
  const ivector& BaseCh = ivector(), double ScanAngle = 0,
  const ivector& AuxCh = ivector(), const rvector& Directs = rvector(),
  bool lBaseChInRDE = OFF, ivector& RDECh = ivector(),
  uint AuxChSRF = MultiStageUnit) : TPreProcessorSpaceFilterBaseCh()
 {Set(InSignal,Polar,BaseCh,ScanAngle,AuxCh,Directs,lBaseChInRDE,RDECh,AuxChSRF);};

 TAnalPreProcSFilterBaseCh(TInputSignal* InSignal, bool Polar,
  const TInDataBaseCh& BaseChProp, const ivector& AuxCh = ivector(),
  const rvector& Directs = rvector(), bool lBaseChInRDE = OFF,
  ivector& RDECh = ivector(), uint AuxChSRF = MultiStageUnit) :
  TPreProcessorSpaceFilterBaseCh()
 {Set(InSignal,Polar,BaseChProp,AuxCh,Directs,lBaseChInRDE,RDECh,AuxChSRF);};

 //Объявление конструктора копирования
 TAnalPreProcSFilterBaseCh(const TAnalPreProcSFilterBaseCh& Property)
 {*this = Property;};
 //Объявление деструктора класса
 ~TAnalPreProcSFilterBaseCh() {};

//******************************************************************************
// ОБЩЕДОСТУПНЫЕ ФУНКЦИИ КЛАССА TAnalPreProcSFilterBaseCh
//******************************************************************************
 //Установка свойств предпроцессора SpaceFilterBaseCh
 bool Set(TInputSignal* InSignal, bool Polar = VP_SIGNAL,
      const ivector& BaseCh = ivector(), double ScanAngle = 0,
      const ivector& AuxCh = ivector(), const rvector& Directs = rvector(),
      bool lBaseChInRDE = OFF,  ivector& RDECh = ivector(),
      uint AuxChSRF = MultiStageUnit)
 {bool lSuccess = TPreProcessorSpaceFilterBaseCh::Set(InSignal,Polar,BaseCh,
             ScanAngle,AuxCh,Directs,lBaseChInRDE,RDECh,AuxChSRF); SetSJNData();
  return lSuccess;
 };
 bool Set(TInputSignal* InSignal, bool Polar, const TInDataBaseCh& BaseChProp,
      const ivector& AuxCh = ivector(), const rvector& Directs = rvector(),
      bool lBaseChInRDE = OFF, ivector& RDECh = ivector(),
      uint AuxChSRF = MultiStageUnit)
 {bool lSuccess = TPreProcessorSpaceFilterBaseCh::Set(InSignal,Polar,BaseChProp,
                  AuxCh,Directs,lBaseChInRDE,RDECh,AuxChSRF); SetSJNData();
  return lSuccess;
 };

 // 1. ФУНКЦИИ ВВОДА - ВЫВОДА ПАРАМЕТРОВ
 //Ввод параметров объекта из стандартного потока ввода cin
 void Edit() {TPreProcessorSpaceFilterBaseCh::Edit(); SetSJNData();};
 //Перегрузка оператора вывода (<<) для вывода параметров класса
 friend ostream& operator <<(ostream& out,
 const TAnalPreProcSFilterBaseCh& Preprocessor)
 {string s; return out << Preprocessor.Properties(s,STR_NULL);};
 //Объявление перегруженного оператора присваивания operator =
 void operator =(const TAnalPreProcSFilterBaseCh& Preprocessor);
 //-----------------------------------------------------------------------------
 //    ФУНКЦИИ, ИСПОЛЬЗУЕМЫЕ ПРИ МОДЕЛИРОВАНИИ ПРЕДПРОЦЕССОРА АДАПТИВНОЙ LAA
 //            c ПРОСТРАНСТВЕННЫМ АДАПТИВНЫМ ФИЛЬТРОМ с ОСНОВНЫМ КАНАЛОМ
 //-----------------------------------------------------------------------------
 //Расчет компонент входного воздействия при моделировании
 bool Update(bool lSignal = ON, bool lJammer = ON, bool lNoise = ON);

 //-----------------------------------------------------------------------------
 // Комплексные напряжения входного воздействия на ВХОДЕ предпроцессора AdaptLAA
 //              / [RDEChannels] + AuxChannels + BaseChannel /
 //-----------------------------------------------------------------------------
 //Входная выборка ПОЛЕЗНОГО СИГНАЛА
 cvector& InSignal(cvector& U, bool lExtend = ON) const;
 //Входная выборка АДДИТИВНОЙ АКТИВНОЙ ПОМЕХИ (по каждой помехе)
 cmatrix& InJammer(cmatrix& A, bool lExtend = ON) const;
 //Входная выборка ВНУТРЕННЕГО ШУМА
 cvector& InNoise(cvector& U, bool lExtend = ON) const;

 //-----------------------------------------------------------------------------
 //Комплексные напряжения входного воздействия на ВЫХОДЕ предпроцессора AdaptLAA
 //             / [RDEChannels] + AuxChannels + BaseChannel /
 //-----------------------------------------------------------------------------
 //Выходная выборка ПОЛЕЗНОГО СИГНАЛА
 cvector& OutSignal(cvector& Uout, bool lExtend = ON) const;
 //Выходная выборка АДДИТИВНОЙ АКТИВНОЙ ПОМЕХИ (по каждой помехе)
 cmatrix& OutJammer(cmatrix& Aout, bool lExtend = ON) const;

 //-----------------------------------------------------------------------------
 //  Мощности компонент входного воздействия на ВХОДЕ/ВЫХОДЕ ОСНОВНОГО канала
 //                      предпроцессора AdaptLAA / BaseChannel /
 //Мощности компонент входного воздействия на входе и выходе ОСНОВНОГО канала
 //                        предпроцессора AdaptLAA идентичны
 //-----------------------------------------------------------------------------
 //Входная мощность ПОЛЕЗНОГО СИГНАЛА с ОСНОВНОГО канала
 double InPsignal() const;
 //Входная мощность АДДИТИВНОЙ АКТИВНОЙ ПОМЕХИ с ОСНОВНОГО канала
 double InPjammer() const;
 //Входная мощность ВНУТРЕННЕГО ШУМА с ОСНОВНОГО канала
 double InPnoise() const;
 //-----------------------------------------------------------------------------
 //  Мощности компонент входного воздействия на ВХОДЕ предпроцессора AdaptLAA
 //                 / [RDEChannels] + AuxChannels + BaseChannel /
 //-----------------------------------------------------------------------------
 //Входная мощность ПОЛЕЗНОГО СИГНАЛА
 rvector& InPsignal(rvector& P, bool lExtend = ON) const;
 //Входная мощность АДДИТИВНОЙ АКТИВНОЙ ПОМЕХИ
 rvector& InPjammer(rvector& P, bool lExtend = ON) const;
 //Входная мощность ВНУТРЕННЕГО ШУМА
 rvector& InPnoise(rvector& P, bool lExtend = ON) const;

 //-----------------------------------------------------------------------------
 //  Мощности компонент входного воздействия на ВЫХОДЕ предпроцессора AdaptLAA
 //                 / [RDEChannels] + AuxChannels + BaseChannel /
 //-----------------------------------------------------------------------------
 //Выходная мощность ПОЛЕЗНОГО СИГНАЛА
 rvector& OutPsignal(rvector& P, bool lExtend = ON) const;
 //Выходная мощность АДДИТИВНОЙ АКТИВНОЙ ПОМЕХИ
 rvector& OutPjammer(rvector& P, bool lExtend = ON) const;
 //Выходная мощность ВНУТРЕННЕГО ШУМА
 rvector& OutPnoise(rvector& P, bool lExtend = ON) const;

 //Входная корреляционная матрица ПОЛЕЗНОГО СИГНАЛА
 cmatrix& GetInRss(cmatrix& R, bool lExtend = ON) const;
 //Входная корреляционная матрица АДДИТИВНОЙ АКТИВНОЙ ПОМЕХИ
 cmatrix& GetInRjj(cmatrix& R, bool lExtend = ON) const;
 //Входная корреляционная матрица ВНУТРЕННЕГО ШУМА
 cmatrix& GetInRnn(cmatrix& R, bool lExtend = ON) const;
 //Выходная корреляционная матрица ПОЛЕЗНОГО СИГНАЛА
 cmatrix& GetOutRss(cmatrix& R, bool lExtend = ON) const;
 //Выходная корреляционная матрица АДДИТИВНОЙ АКТИВНОЙ ПОМЕХИ
 cmatrix& GetOutRjj(cmatrix& R, bool lExtend = ON) const;
 //Выходная корреляционная матрица ВНУТРЕННЕГО ШУМА
 cmatrix& GetOutRnn(cmatrix& R, bool lExtend = ON) const;

 //Входная корреляционная матрица компонент ВХОДНОГО ВОЗДЕЙСТВИЯ
 cmatrix& GetInR(cmatrix& R, bool lSignal = ON, bool lJammer = ON,
                 bool lNoise = ON, bool lExtend = ON) const;
 //Выходная корреляционная матрица компонент ВХОДНОГО ВОЗДЕЙСТВИЯ предпроцессора
 //адаптивного пространственного фильтра
 cmatrix& GetOutR(cmatrix& R, bool lSignal = ON, bool lJammer = ON,
                  bool lNoise = ON, bool lExtend = ON) const;

//------------------------------------------------------------------------------
//II. ОБЪЯВЛЕНИЕ ЗАКРЫТЫХ ЧЛЕНОВ-ФУНКЦИЙ КЛАССА TAnalPreProcSFilterBaseCh
//------------------------------------------------------------------------------
 private:
  //Установка членов-данных класса в которых формируются входные выборки
  //ПОЛЕЗНОГО СИГНАЛА, АДДИТИВНОЙ ПОМЕХИ и ВНУТРЕННЕГО ШУМА
  void SetSJNData();
}; //Конец объявления класса TAnalPreProcSFilterBaseCh
//----------  The end of class TAnalPreProcSFilterBaseCh declaration  ----------

//******************************************************************************
//              Объявление класса - TStatPreProcSFilterBaseCh
//Статистическая модель предпроцессора обработки входного сигнала пространствен-
//ного адаптивного фильтра с выделенным каналом на основе LAA.
//Класс наследует свойства класса TPreProcessorSpaceFilter. Он выполняет функции
//аналогичные базовому классу. Основной его функцией является формирование вход-
//ной выборки каждой компоненты внешнего воздействия для адаптивного пространст-
//венного фильтра с выделенным каналом. Входные выборки полезного сигнала, адди-
//тивной помехи и внутреннего  шума формируются в векторах Signal, Jammer, Noise
//соответственно.
//******************************************************************************
class TStatPreProcSFilterBaseCh : public TPreProcessorSpaceFilterBaseCh {
//------------------------------------------------------------------------------
// ОБЪЯВЛЕНИЕ ЗАКРЫТЫХ ЧЛЕНОВ-ДАННЫХ КЛАССА TStatPreProcSFilterBaseCh
//------------------------------------------------------------------------------
 private:
  // [BaseChannel] + RDECh == DirectElim.Size(),
  // [BaseChannel] -> ОСНОВНОЙ КАНАЛ подключен к AuxChRDEUnit
  //----------------------------------------------------------------------------
  //Выборка ПОЛЕЗНОГО СИГНАЛА на входе с каналов LAA, подключенных к предпроцес-
  //сору, и ОСНОВНОГО канала. Размерность вектора Signal:
  //1) AuxChRDEUnit = ON:  [BaseChannel] + RDECh + AuxCh + BaseChannel
  //2) AuxChRDEUnit = OFF: AuxCh + BaseChannel
  cvector Signal;
  //----------------------------------------------------------------------------
  //Выборка ВНУТРЕННЕГО ШУМА на входе с каналов LAA, подключенных к предпроцес-
  //сору, и ОСНОВНОГО канала. Размерность вектора Noise:
  //1) AuxChRDEUnit = ON:  [BaseChannel] + RDECh + AuxCh + BaseChannel
  //2) AuxChRDEUnit = OFF: AuxCh + BaseChannel
  cvector Noise;
  //----------------------------------------------------------------------------
  //Выборка АДДИТИВНОЙ АКТИВНОЙ ПОМЕХИ на входе с каналов LAA, подключенных к
  //предпроцессору, и ОСНОВНОГО канала. Размерность матрицы Jammer:
  //Количество строк матрицы:
  //1) AuxChRDEUnit = ON:  [BaseChannel] + RDECh + AuxCh + BaseChannel
  //2) AuxChRDEUnit = OFF: AuxCh + BaseChannel
  cvector Jammer;
//------------------------------------------------------------------------------
// I. ОБЪЯВЛЕНИЕ ОБЩЕДОСТУПНЫХ ЧЛЕНОВ-ФУНКЦИЙ КЛАССА TStatPreProcSFilterBaseCh
//------------------------------------------------------------------------------
 public:
//*****************************************************************************
// ОБЪЯВЛЕНИЕ КОНСТРУКТОРОВ И ДЕСТРУКТОРА КЛАССА TStatPreProcSFilterBaseCh
//*****************************************************************************
 //Объявление конструктора по умолчанию
 TStatPreProcSFilterBaseCh() : TPreProcessorSpaceFilterBaseCh() {};
 //Объявление конструктора с аргументами
 TStatPreProcSFilterBaseCh(TInputSignal* InSignal, bool Polar = VP_SIGNAL,
  const ivector& BaseCh = ivector(), double ScanAngle = 0, const ivector& AuxCh = ivector(),
  const rvector& Directs = rvector(), bool lBaseChInRDE = OFF, ivector& RDECh = ivector(),
  uint AuxChSRF = MultiStageUnit) : TPreProcessorSpaceFilterBaseCh()
  {Set(InSignal,Polar,BaseCh,ScanAngle,AuxCh,Directs,lBaseChInRDE,RDECh,AuxChSRF);};

 TStatPreProcSFilterBaseCh(TInputSignal* InSignal, bool Polar,
  const TInDataBaseCh& BaseChProp, const ivector& AuxCh = ivector(),
  const rvector& Directs = rvector(), bool lBaseChInRDE = OFF, ivector& RDECh = ivector(),
  uint AuxChSRF = MultiStageUnit) : TPreProcessorSpaceFilterBaseCh()
 {Set(InSignal,Polar,BaseChProp,AuxCh,Directs,lBaseChInRDE,RDECh,AuxChSRF);};

 //Объявление конструктора копирования
 TStatPreProcSFilterBaseCh(const TAnalPreProcSFilterBaseCh& Obj) {*this = Obj;};
 //Объявление деструктора класса
 ~TStatPreProcSFilterBaseCh() {};

//******************************************************************************
// ОБЩЕДОСТУПНЫЕ ФУНКЦИИ КЛАССА TStatPreProcSFilterBaseCh
// 1. ФУНКЦИИ ВВОДА - ВЫВОДА ПАРАМЕТРОВ
//******************************************************************************
 //Установка свойств предпроцессора SpaceFilterBaseCh
 bool Set(TInputSignal* InSignal, bool Polar = VP_SIGNAL,
      const ivector& BaseCh = ivector(), double ScanAngle = 0,
      const ivector& AuxCh = ivector(), const rvector& Directs = rvector(),
      bool lBaseChInRDE = OFF,  ivector& RDECh = ivector(), uint AuxChSRF = MultiStageUnit)
 {bool lRes = TPreProcessorSpaceFilterBaseCh::Set(InSignal,Polar,BaseCh,
              ScanAngle,AuxCh,Directs,lBaseChInRDE,RDECh,AuxChSRF); SetSJNData();
  return lRes;
 };

 bool Set(TInputSignal* InSignal, bool Polar, const TInDataBaseCh& BaseChProp,
      const ivector& AuxCh = ivector(), const rvector& Directs = rvector(),
      bool lBaseChInRDE = OFF, ivector& RDECh = ivector(),
      uint AuxChSRF = MultiStageUnit)
 {bool lRes = TPreProcessorSpaceFilterBaseCh::Set(InSignal,Polar,BaseChProp,AuxCh,Directs,
              lBaseChInRDE,RDECh,AuxChSRF); SetSJNData();
  return lRes;
 };

 //Ввод параметров объекта из стандартного потока ввода cin
 void Edit() {TPreProcessorSpaceFilterBaseCh::Edit(); SetSJNData();};
 //Перегрузка оператора вывода (<<) для вывода параметров класса
 friend ostream& operator <<(ostream& out, const TStatPreProcSFilterBaseCh& Preprocessor)
 {string s; return out << Preprocessor.Properties(s,STR_NULL);};
 //Объявление перегруженного оператора присваивания operator =
 void operator =(const TStatPreProcSFilterBaseCh& Preprocessor);
 //-----------------------------------------------------------------------------
 //    ФУНКЦИИ, ИСПОЛЬЗУЕМЫЕ ПРИ МОДЕЛИРОВАНИИ ПРЕДПРОЦЕССОРА АДАПТИВНОЙ LAA
 //            c ПРОСТРАНСТВЕННЫМ АДАПТИВНЫМ ФИЛЬТРОМ с ОСНОВНЫМ КАНАЛОМ
 //-----------------------------------------------------------------------------
 bool Update(); //Расчет компонент входного воздействия при моделировании

 //-----------------------------------------------------------------------------
 // Комплексные напряжения входного воздействия на ВХОДЕ предпроцессора AdaptLAA
 //              / [RDEChannels] + AuxChannels + BaseChannel /
 //-----------------------------------------------------------------------------
 //Входная выборка ПОЛЕЗНОГО СИГНАЛА
 cvector& InSignal(cvector& U, bool lExtend = ON) const;
 //Входная выборка АДДИТИВНОЙ АКТИВНОЙ ПОМЕХИ (по каждой помехе)
 cvector& InJammer(cvector& U, bool lExtend = ON) const;
 //Входная выборка ВНУТРЕННЕГО ШУМА
 cvector& InNoise(cvector& U, bool lExtend = ON) const;

 //-----------------------------------------------------------------------------
 //Комплексные напряжения входного воздействия на ВЫХОДЕ предпроцессора AdaptLAA
 //             / [RDEChannels] + AuxChannels + BaseChannel /
 //-----------------------------------------------------------------------------
 //Выходная выборка ПОЛЕЗНОГО СИГНАЛА
 cvector& OutSignal(cvector& Uout, bool lExtend = ON) const;
 //Выходная выборка АДДИТИВНОЙ АКТИВНОЙ ПОМЕХИ (по каждой помехе)
 cvector& OutJammer(cvector& Uout, bool lExtend = ON) const;
 //Выходная выборка ВНУТРЕННЕГО ШУМА
 cvector& OutNoise(cvector& Uout, bool lExtend = ON) const;

 //-----------------------------------------------------------------------------
 //  Мощности компонент входного воздействия на ВХОДЕ/ВЫХОДЕ ОСНОВНОГО канала
 //                      предпроцессора AdaptLAA / BaseChannel /
 //Мощности компонент входного воздействия на входе и выходе ОСНОВНОГО канала
 //                        предпроцессора AdaptLAA идентичны
 //-----------------------------------------------------------------------------
 //Входная мощность ПОЛЕЗНОГО СИГНАЛА с ОСНОВНОГО канала
 double InPsignal() const;
 //Входная мощность АДДИТИВНОЙ АКТИВНОЙ ПОМЕХИ с ОСНОВНОГО канала
 double InPjammer() const;
 //Входная мощность ВНУТРЕННЕГО ШУМА с ОСНОВНОГО канала
 double InPnoise() const;
 //-----------------------------------------------------------------------------
 //  Мощности компонент входного воздействия на ВХОДЕ предпроцессора AdaptLAA
 //                 / [RDEChannels] + AuxChannels + BaseChannel /
 //-----------------------------------------------------------------------------
 //Входная мощность ПОЛЕЗНОГО СИГНАЛА
 rvector& InPsignal(rvector& P, bool lExtend = ON) const;
 //Входная мощность АДДИТИВНОЙ АКТИВНОЙ ПОМЕХИ
 rvector& InPjammer(rvector& P, bool lExtend = ON) const;
 //Входная мощность ВНУТРЕННЕГО ШУМА
 rvector& InPnoise(rvector& P, bool lExtend = ON) const;

 //-----------------------------------------------------------------------------
 //  Мощности компонент входного воздействия на ВЫХОДЕ предпроцессора AdaptLAA
 //                 / [RDEChannels] + AuxChannels + BaseChannel /
 //-----------------------------------------------------------------------------
 //Выходная мощность ПОЛЕЗНОГО СИГНАЛА
 rvector& OutPsignal(rvector& P, bool lExtend = ON) const;
 //Выходная мощность АДДИТИВНОЙ АКТИВНОЙ ПОМЕХИ
 rvector& OutPjammer(rvector& P, bool lExtend = ON) const;
 //Выходная мощность ВНУТРЕННЕГО ШУМА
 rvector& OutPnoise(rvector& P, bool lExtend = ON) const;

//------------------------------------------------------------------------------
//II. ОБЪЯВЛЕНИЕ ЗАКРЫТЫХ ЧЛЕНОВ-ФУНКЦИЙ КЛАССА TStatPreProcSFilterBaseCh
//------------------------------------------------------------------------------
 private:
  //Установка членов-данных класса в которых формируются входные выборки
  //ПОЛЕЗНОГО СИГНАЛА, АДДИТИВНОЙ ПОМЕХИ и ВНУТРЕННЕГО ШУМА
  void SetSJNData();
}; //Конец объявления класса TStatPreProcSFilterBaseCh
//----------  The end of class TStatPreProcSFilterBaseCh declaration  ----------

//******************************************************************************
//Объявление класса - TASpaceFilterBaseCh - аналитическая  модель пространствен-
//ного  адаптивного  фильтра с выделенным каналом на основе LAA. Класс наследует
//свойства  классов  TAnalPreProcSFilterBaseCh и TAnalInSignalProperty. Функцио-
//нальные  возможности   класса  заключаются, в  первую  очередь, в формировании
//воздействия на входе пространственного фильтра адаптивной LAA в соответствии с
//ее  конфигурацией  задаваемой  пользователем: шириной и направлением основного
//луча, количеством  вспомогательных каналов фильтра, использованием и по сколь-
//ким направлениям схемы  исключения  приема  сигналов с  заданных направлений в
//вспомогательных  каналах  фильтра. Для  аналитической модели пространственного
//фильтра с выделенным (основным) каналом в качестве входного воздействия исполь-
//зуются корреляционные матрицы полезного сигнала, аддитивной  активной помехи и
//внутреннего шума, а также  суммарная  матрица  этих  компонент  для адаптивной
//настройки  весовых  коэффициентов фильтра. Размерность  корреляционных  матриц
//компонент  входного  сигнала  равна количеству вспомогательных каналов фильтра
//AuxChannel+1 (BaseChannel).
//Для построения передаточной функции фильтра после его адаптации используется
//вектор входного воздействия (для ускорения работы). Данный класс позволяет с
//использованием  наследования  свойств  класса  TPreProcessorSaceFilterBaseCh
//вычислять передаточные функции схемы формирования ОСНОВНОГО канала BaseChannel,
//вспомогательных каналов фильтра AuxChannel, схемы AuxChRDEUnit, получать инфор-
//мацию по мощностям на входе и выходе фильтра, весовым коэффициентам,  корреля-
//ционным матрицам и т.д.
//Для моделирования работы самого фильтра в класс вводится указатель на базовый
//класс из заголовочного файла "weightsum.h" - TMultiInSingleOutWeightSummator.
//Это  дает  возможность  пользователю использовать в качестве пространственного
//фильтра разнообразные  аналитические модели весовых сумматоров со многими вхо-
//дами и одним выходом. Данные  весовые сумматоры могут отличаться друг от друга
//по  конфигурации (одноступенчатые и многоступенчатые), по алгоритмам настройки
//весовых  коэффициентов (алгоритмам адаптации) и по схемам стабилизации динами-
//ческих параметров фильтров (наличию/отсутствию, типу схемы стабилизации при ее
//использовании). Все эти весовые сумматоры должно объединять только одно: все
//они должны быть со МНОГИМИ ВХОДАМИ (more than 1) и ОДНИМ ВЫХОДОМ. Число входов
//сумматора  определяется  автоматически при выборе конкретного типа сумматора и
//определяется  количеством  AuxChannel+1. Такая  реализация данного класса дает
//возможность  исследовать и  реализовывать  адаптивные пространственные фильтры
//как  по их  внутренним  параметрам (изменение значений весовых коэффициентов в
//зависимости от складывающейся помеховой ситуации), так и по их структуре, алго-
//ритмам настройки весовых коэффициентов и стабилизации их динамических парамет-
//ров. Напомним, что работа пространственного фильтра заключается в формировании
//нулей  диаграммы направленности (передаточной функции) в направлении на источ-
//ники помех. Количество дополнительных  каналов определяет  максимальное  число
//активных (шумовых) помех, которые могут быть подавлены фильтром. Считается, что
//полезный источник сигнала располагается в главном луче (лепестке) LAA. С целью
//недопущения подавления его приема пространственным фильтром используется схема
//защиты главного луча, реализованная посредством AuxChRDEUnit.
//Моделирование аналитической модели адаптивной LAA с пространственным фильтром
//и ОСНОВНЫМ каналом производится за одну реализацию. В начале моделирования ве-
//совые коэффициенты фильтра ненастроены и они пропускают входное воздействия без
//его изменения. По окончании моделирования имеется возможность построить переда-
//точную характеристику  адаптивной LAA, которая будет зависеть от многих факто-
//ров, таких как, от  входного  воздействия, конфигурации  адаптивной  решетки и
//пространственного фильтра, алгоритма настройки и стабилизации работы фильтра,
//внутренних параметров фильтра и т.д.
//******************************************************************************
class TASpaceFilterBaseCh:
 public TAnalPreProcSFilterBaseCh, public TAnalInSignalProperty {
//------------------------------------------------------------------------------
// ОБЪЯВЛЕНИЕ ЗАЩИЩЕННЫХ ЧЛЕНОВ-ДАННЫХ КЛАССА TASpaceFilterBaseCh
//------------------------------------------------------------------------------
 protected:
  //Указатель на базовый абстрактный класс весового сумматора
  TMultiInSingleOutWeightSummator* Filter;//Пространственный адаптивный фильтр
  //Корреляционные матрицы полезного сигнала, аддитивной помехи и внутреннего
  //шума на входе фильтра
  cmatrix Rss;//Корреляционная матрица ПОЛЕЗНОГО СИГНАЛА
  cmatrix Rjj;//Корреляционная матрица АДДИТИВНОЙ ПОМЕХИ
  cmatrix Rnn;//Корреляционная матрица ВНУТРЕННЕГО ШУМА
  //Суммарная матрица компонент входного воздействия на входе пространственного
  //адаптивного фильтра: Rss+Rjj+Rnn
  cmatrix R;
//------------------------------------------------------------------------------
// I. ОБЪЯВЛЕНИЕ ОБЩЕДОСТУПНЫХ ЧЛЕНОВ-ФУНКЦИЙ КЛАССА TASpaceFilterBaseCh
//------------------------------------------------------------------------------
 public:
//*****************************************************************************
// ОБЪЯВЛЕНИЕ КОНСТРУКТОРОВ И ДЕСТРУКТОРА КЛАССА TASpaceFilterBaseCh
//*****************************************************************************
 //Объявление конструктора по умолчанию
 TASpaceFilterBaseCh() :
  TAnalPreProcSFilterBaseCh(), TAnalInSignalProperty() {Initial();};
 //Объявление конструктора с аргументами
 TASpaceFilterBaseCh(TInputSignal* InSignal) :
  TAnalPreProcSFilterBaseCh(InSignal), TAnalInSignalProperty() {Initial();};
 //Объявление конструктора копирования
 TASpaceFilterBaseCh(TASpaceFilterBaseCh& SFilter) {*this = SFilter;};
 //Объявление деструктора класса
 ~TASpaceFilterBaseCh() {Reset();};
//******************************************************************************
// ОБЩЕДОСТУПНЫЕ ФУНКЦИИ ДЛЯ РАБОТЫ С КЛАССОМ TASpaceFilterBaseCh
//******************************************************************************
 //Определение РАБОТОСПОСОБНОСТИ предпроцессора фильтра
 bool IsPreProcessorReady() const {return TAnalPreProcSFilterBaseCh::IsReady();};
 //Определение РАБОТОСПОСОБНОСТИ пространственного фильтра
 bool IsSpaceFilterReady() const
 {return ((Filter != NULL) && (Filter->IsReady())) ? ON : OFF;};
 //Определение РАБОТОСПОСОБНОСТИ всей АДАПТИВНОЙ Linear Antenna Array
 bool IsReady() const
 {return ((IsPreProcessorReady()) && (IsSpaceFilterReady())) ? ON : OFF;};
 //-----------------------------------------------------------------------------
 //Получение коэффициентов весового сумматора
 //-----------------------------------------------------------------------------
 cmatrix& GetW(cmatrix& W) const
 {if (!IsReady()) W.Set(0L,0L,false);
  else Filter->GetW(W);
  return W;
 };

 //-----------------------------------------------------------------------------
 //   Корреляционные матрицы компонент внешнего воздействия на входе AdaptLAA
 //                     /на входе препроцессора AdaptLAA/
 //-----------------------------------------------------------------------------
 //Входная корреляционная матрица ПОЛЕЗНОГО СИГНАЛА /вход AdaptLAA/
 cmatrix& GetExtRss(cmatrix& A, bool lExtend = ON) const
 {return TAnalPreProcSFilterBaseCh::GetInRss(A,lExtend);};
 //Входная корреляционная матрица АДДИТИВНОЙ АКТИВНОЙ ПОМЕХИ /вход AdaptLAA/
 cmatrix& GetExtRjj(cmatrix& A, bool lExtend = ON) const
 {return TAnalPreProcSFilterBaseCh::GetInRjj(A,lExtend);};
 //Входная корреляционная матрица ВНУТРЕННЕГО ШУМА /вход AdaptLAA/
 cmatrix& GetExtRnn(cmatrix& A, bool lExtend = ON) const
 {return TAnalPreProcSFilterBaseCh::GetInRnn(A,lExtend);};

 //-----------------------------------------------------------------------------
 //Корреляционные матрицы компонент внешнего воздействия на входе SpaceFilter
 //          /выход препроцессора - вход AdaptSpaceFilter with BaseCh/
 //-----------------------------------------------------------------------------
 //Входная корреляционная матрица ПОЛЕЗНОГО СИГНАЛА /вход SpaceFilter/
 cmatrix& GetInRss(cmatrix& A, bool lExtend = ON) const;
 //Входная корреляционная матрица АДДИТИВНОЙ АКТИВНОЙ ПОМЕХИ /вход SpaceFilter/
 cmatrix& GetInRjj(cmatrix& A, bool lExtend = ON) const;
 //Входная корреляционная матрица ВНУТРЕННЕГО ШУМА /вход SpaceFilter/
 cmatrix& GetInRnn(cmatrix& A, bool lExtend = ON) const;

 //-----------------------------------------------------------------------------
 //Корреляционные матрицы компонент внешнего воздействия на выходе SpaceFilter
 //-----------------------------------------------------------------------------
 //Выходная корреляционная матрица ПОЛЕЗНОГО СИГНАЛА /выход SpaceFilter/
 cmatrix& GetOutRss(cmatrix& A) const
 {if (!IsReady()) {A.Set(0L,0L,false); return A;}
  A = Rss; //Формируем матрицу ПОЛЕЗНОГО СИГНАЛА на входе фильтра
  Filter->TF_WSum(A); //Формируем матрицу ПОЛЕЗНОГО СИГНАЛА на выходе фильтра
  return A;
 };
 //Выходная корреляционная матрица АДДИТИВНОЙ АКТИВНОЙ ПОМЕХИ /выход SpaceFilter/
 cmatrix& GetOutRjj(cmatrix& A) const
 {if (!IsReady()) {A.Set(0L,0L,false); return A;}
  A = Rjj; //Формируем матрицу АКТИВНОЙ ПОМЕХИ на входе фильтра
  Filter->TF_WSum(A); //Формируем матрицу АКТИВНОЙ ПОМЕХИ на выходе фильтра
  return A;
 };
 //Выходная корреляционная матрица ВНУТРЕННЕГО ШУМА /выход SpaceFilter/
 cmatrix& GetOutRnn(cmatrix& A) const
 {if (!IsReady()) {A.Set(0L,0L,false); return A;}
  A = Rnn; //Формируем матрицу ВНУТРЕННЕГО ШУМА на входе фильтра
  Filter->TF_WSum(A); //Формируем матрицу ВНУТРЕННЕГО ШУМА на выходе фильтра
  return A;
 };

 //Входная корреляционная матрица компонент ВХОДНОГО ВОЗДЕЙСТВИЯ
 // /вход AdaptLAA - вход препроцессора/
 cmatrix& GetExtR(cmatrix& A, bool lSignal = ON, bool lJammer = ON,
                  bool lNoise = ON, bool lExtend = ON) const
 {return TAnalPreProcSFilterBaseCh::GetInR(A,lSignal,lJammer,lNoise,lExtend);
 };
 //Выходная корреляционная матрица компонент ВХОДНОГО ВОЗДЕЙСТВИЯ предпроцессора
 //адаптивного пространственного фильтра /вход SpaceFilter/
 cmatrix& GetInR(cmatrix& A, bool lSignal = ON, bool lJammer = ON,
                 bool lNoise = ON, bool lExtend = ON) const;
 //Выходная корреляционная матрица компонент ВХОДНОГО ВОЗДЕЙСТВИЯ
 //адаптивного пространственного фильтра /выход SpaceFilter/
 cmatrix& GetOutR(cmatrix& A, bool lSignal = ON, bool lJammer = ON,
                  bool lNoise = ON) const
 {if (!IsReady()) {A.Set(0L,0L,false); return A;}
  //Формируем матрицу ВХОДНОГО ВОЗДЕЙСТВИЯ
  GetInR(A,lSignal,lJammer,lNoise,ON);
  Filter->TF_WSum(A);//Матрица ВХОДНОГО ВОЗДЕЙСТВИЯ на выходе фильтра
  return A;
 };
 //Передаточная функция (Transfer function) адаптивной LAA с пространственным
 //фильтром и ОСНОВНЫМ каналом /выход ОСНОВНОГО канала/
 rvector& TFunc(rvector& P, const TEvenCloseInterval& Sector, bool Norm = ON,
                bool Db = ON, double MinDb = -60.0) const;
 //Передаточная функция (Transfer function) предпроцессора адаптивной LAA
 //с пространственным фильтром и ОСНОВНЫМ каналом /выход AuxCh + BaseChannel/
 bool TF_Preprocessor(rmatrix& P, const TEvenCloseInterval& Sector,
                      bool Norm = ON, bool Db = ON, double MinDb = -60.0) const
 {return TAnalPreProcSFilterBaseCh::TFunc(P,Sector,Norm,Db,MinDb);};
 //Потенциальная передаточная функция адаптивной LAA при заданном уровне активной
 //помехи и внутреннего шума (помеховой ситуации)
 bool TF_Potential(rvector& P, double Time, const TEvenCloseInterval& Sector,
                   bool Norm = ON, bool dB = ON, double MindB = -60.0);
 //----------------------------------------------------------------------------------
 //ФУНКЦИИ, ИСПОЛЬЗУЕМЫЕ ДЛЯ МОДЕЛИРОВАНИЯ КЛАССА TASpaceFilterBaseCh
 //----------------------------------------------------------------------------------
 //Начальная установка параметров AdaptLAA при моделировании: начальная установка
 //весовых коэффициентов пространственного фильтра
 bool SimulateInit()
 {return (IsReady()) ? Filter->SimulateInit() : OFF;};

 //Расчет корреляционных матриц компонент входного воздействия при моделировании
 //работы адаптивного фильтра
 bool Update(double Time);

 //Настройка весовых коэффициентов пространственного фильтра
 bool Tuning() {return (IsReady()) ? Filter->Tuning(R) : OFF;};

 //Расчет мощностей на входе адаптивной LAA /ОСНОВНОЙ канал/
 //Мощность ПОЛЕЗНОГО СИГНАЛА на входе фильтра
 double InPSignal() const
 {ulong n = Rss.Rows()-1; return (IsReady()) ? abs(Rss.Get(n,n)) : 0;};
 //Мощность АКТИВНОЙ АДДИТИВНОЙ ПОМЕХИ на входе фильтра
 double InPJammer() const
 {ulong n = Rjj.Rows()-1; return (IsReady()) ? abs(Rjj.Get(n,n)) : 0;};
 //Мощность ВНУТРЕННЕГО ШУМА на входе фильтра
 double InPNoise() const
 {ulong n = Rnn.Rows()-1; return (IsReady()) ? abs(Rnn.Get(n,n)) : 0;};

 //Расчет мощностей на выходе адаптивной LAA /ОСНОВНОЙ канал/
 //Мощность ПОЛЕЗНОГО СИГНАЛА на выходе фильтра
 double OutPSignal() const
 {if (!IsReady()) return 0.0;//Адаптивный фильтр НЕРАБОТОСПОСОБЕН
  cmatrix A; GetOutRss(A);//Формирование выходной матрицы ПОЛЕЗНОГО СИГНАЛА
  ulong n = A.Rows()-1;
  return abs(A.Get(n,n));//Мощность ПОЛЕЗНОГО СИГНАЛА на выходе фильтра
 };
 //Мощность АКТИВНОЙ ПОМЕХИ на выходе фильтра
 double OutPJammer() const
 {if (!IsReady()) return 0.0;//Адаптивный фильтр НЕРАБОТОСПОСОБЕН
  cmatrix A; GetOutRjj(A);//Формирование выходной матрицы АКТИВНОЙ ПОМЕХИ
  ulong n = A.Rows()-1;
  return abs(A.Get(n,n));//Мощность АКТИВНОЙ ПОМЕХИ на выходе фильтра
 };
 //Мощность ВНУТРЕННЕГО ШУМА на выходе фильтра
 double OutPNoise() const
 {if (!IsReady()) return 0.0;//Адаптивный фильтр НЕРАБОТОСПОСОБЕН
  cmatrix A; GetOutRnn(A);//Формирование выходной матрицы ВНУТРЕННЕГО ШУМА
  ulong n = A.Rows()-1;
  return abs(A.Get(n,n));//Мощность ВНУТРЕННЕГО ШУМА на выходе фильтра
 };

 //-----------------------------------------------------------------------------
 // Мощности компонент входного воздействия на ВХОДЕ Adaptive LAA
 // / [RDEChannels] + AuxChannels + BaseChannel /
 //-----------------------------------------------------------------------------
 //Входная мощность ПОЛЕЗНОГО СИГНАЛА
 rvector& ExtPsignal(rvector& P, bool lExtend = ON) const
 {if (IsReady()) TAnalPreProcSFilterBaseCh::InPsignal(P, lExtend);
  else P.Resize(0);
  return P;
 };
 //Входная мощность АДДИТИВНОЙ АКТИВНОЙ ПОМЕХИ
 rvector& ExtPjammer(rvector& P, bool lExtend = ON) const
 {if (IsReady()) TAnalPreProcSFilterBaseCh::InPjammer(P, lExtend);
  else P.Resize(0);
  return P;
 };
 //Входная мощность ВНУТРЕННЕГО ШУМА
 rvector& ExtPnoise(rvector& P, bool lExtend = ON) const
 {if (IsReady()) TAnalPreProcSFilterBaseCh::InPnoise(P, lExtend);
  else P.Resize(0);
  return P;
 };
 //-----------------------------------------------------------------------------
 //  Мощности компонент входного воздействия на ВЫХОДЕ предпроцессора AdaptLAA
 //  /ВХОД АДАПТИВНОГО ФИЛЬТРА/  / [RDEChannels] + AuxChannels + BaseChannel /
 //-----------------------------------------------------------------------------
 //Мощность ПОЛЕЗНОГО СИГНАЛА на входе фильтра
 rvector& InPsignal(rvector& P, bool lExtend = ON) const;
 //Мощность АДДИТИВНОЙ АКТИВНОЙ ПОМЕХИ на входе фильтра
 rvector& InPjammer(rvector& P, bool lExtend = ON) const;
 //Мощность ВНУТРЕННЕГО ШУМА на входе фильтра
 rvector& InPnoise(rvector& P, bool lExtend = ON) const;

 //-----------------------------------------------------------------------------
 //  Мощности компонент входного воздействия на ВЫХОДЕ Adaptive LAA
 //  /ВЫХОД АДАПТИВНОГО ФИЛЬТРА/  / [RDEChannels] + AuxChannels + BaseChannel /
 //  Для каждой ступени фильтра /при использовании фильтров многоступенчатой
 //  структуры/
 //-----------------------------------------------------------------------------
 //Выходная мощность ПОЛЕЗНОГО СИГНАЛА
 rvector& OutPsignal(rvector& P) const;
 //Выходная мощность АДДИТИВНОЙ АКТИВНОЙ ПОМЕХИ
 rvector& OutPjammer(rvector& P) const;
 //Выходная мощность ВНУТРЕННЕГО ШУМА
 rvector& OutPnoise(rvector& P) const;
 //-----------------------------------------------------------------------------
 //Расчет потенциальной выходной мощности аддитивной активной помехи и внутрен-
 //него шума адаптивного фильтра по входной корреляционной матрице:
 // PotentialPout = |Rjn|/|AuxRjn|,
 // |AuxRjn| - определитель корреляционной матрицы помехи и внутреннего шума для
 //            вспомогательных каналов фильтра;
 // |Rjn| - определитель корреляционной матрицы помехи и внутреннего шума для
 //         ОСНОВНОГО и вспомогательных каналов фильтра
 //-----------------------------------------------------------------------------
 double PotentialOutPjn() const
 {if (!IsReady()) return 0; //Адаптивный фильтр НЕРАБОТОСПОСОБЕН
  cmatrix R, Raux;
  GetInR(R,OFF,ON,ON,ON); GetInR(Raux,OFF,ON,ON,OFF);
  return abs(R.DET_UnSymmetric())/abs(Raux.DET_UnSymmetric());
 };
 //-----------------------------------------------------------------------------
 //Расчет потенциального коэффициента подавления адаптивного фильтра по входной
 //корреляционной матрицы помехи (аддитивная активная помеха + внутренний шум)
 // PotentialKsup = 10*log[(Pin*|AuxRjn|)/|Rjn|] [dB],
 // Pin - мощность помехи и внутреннего шума на входе основного канала;
 // |AuxRjn| - определитель корреляционной матрицы помехи и внутреннего шума для
 //            вспомогательных каналов фильтра;
 // |Rjn| - определитель корреляционной матрицы помехи и внутреннего шума для
 //         ОСНОВНОГО и вспомогательных каналов фильтра
 //-----------------------------------------------------------------------------
 double PotentialKsup() const
 {if (!IsReady()) return 0; //Адаптивный фильтр НЕРАБОТОСПОСОБЕН
  double InPjn = InPJammer() + InPNoise();
  double OutPjn = PotentialOutPjn();
  return 10.0*log10(InPjn/OutPjn);
 };

 //-----------------------------------------------------------------------------
 // ФУНКЦИИ ВВОДА - ВЫВОДА ПАРАМЕТРОВ КЛАССА TASpaceFilterBaseCh
 //-----------------------------------------------------------------------------
 //Получение свойств класса в виде строкового объекта string
 string& Properties(string& s, string& title, uint indent = 0) const;

 //Ввод параметров объекта из стандартного потока ввода cin
 void Edit();
 //Редактирование свойств пространственного фильтра
 void EditFilter();
 //Перегрузка оператора ввода (>>) для ввода параметров класса
 friend istream& operator >>(istream& in, TASpaceFilterBaseCh& SFilter);
 //Перегрузка оператора вывода (<<) для вывода параметров класса
 friend ostream& operator <<(ostream& out, const TASpaceFilterBaseCh& SFilter)
 {string s; return out << SFilter.Properties(s,STR_NULL);};
 //Объявление перегруженного оператора присваивания operator =
 void operator =(const TASpaceFilterBaseCh& SFilter);
//---------------------------------------------------------------------------------------
//II. ОБЪЯВЛЕНИЕ ЗАЩИЩЕННЫХ ЧЛЕНОВ-ФУНКЦИЙ КЛАССА TASpaceFilterBaseCh
//---------------------------------------------------------------------------------------
 protected:
  //Начальная инициализация членов-данных класса при конструировании
  void Initial()
  {Filter = NULL; Rss = cmatrix(); Rjj = cmatrix(); Rnn = cmatrix(); R = cmatrix();};
  void Reset() //Сброс всех характеристик класса
  {if (Filter != NULL) {delete Filter; Filter = NULL;}
   Rss = cmatrix(); Rjj = cmatrix(); Rnn = cmatrix(); R = cmatrix();
   TAnalPreProcSFilterBaseCh::Reset();
   TAnalInSignalProperty::Reset();
  };
  //Установка размерности фильтра и размеров корреляционных матриц
  void Set()
  {ulong size = FilterSize();
   if (Filter != NULL) Filter->SetInput(size);
   Rss.Set(size,C_ZERO); Rjj.Set(size,C_ZERO); Rnn.Set(size,C_ZERO); R.Set(size,C_ZERO);
  };
};
//Конец объявления класса TASpaceFilterBaseCh
//The end of class TASpaceFilterBaseCh declaration

//******************************************************************************
//Объявление класса - TSpaceFilterBaseCh - статистическая  модель пространствен-
//ного адаптивного фильтра с выделенным каналом на  основе LAA. Класс  наследует
//свойства  класса TStatPreProcSFilterBaseCh.  Функциональные возможности класса
//заключаются, в первую очередь, в формировании воздействия на входе пространст-
//венного  фильтра  адаптивной LAA в  соответствии с ее конфигурацией задаваемой
//пользователем: шириной и направлением основного луча, количеством вспомогатель-
//ных  каналов фильтра, использованием и по скольким направлениям схемы исключе-
//ния  приема сигналов с заданных направлений в вспомогательных каналах фильтра.
//Для  статиситической  модели пространственного фильтра с выделенным (основным)
//каналом в качестве входного воздействия используются векторы полезного сигнала,
//аддитивной  активной  помехи и внутреннего шума, а также суммарный вектор этих
//компонент  для адаптивной настройки весовых коэффициентов фильтра. Размерность
//векторов  компонент  входного сигнала равна количеству вспомогательных каналов
//фильтра AuxChannel+1 (BaseChannel).
//Для построения передаточной функции фильтра после его адаптации используется
//вектор входного воздействия (для ускорения работы). Данный класс позволяет с
//использованием  наследования  свойств  класса  TPreProcessorSaceFilterBaseCh
//вычислять передаточные функции схемы формирования ОСНОВНОГО канала BaseChannel,
//вспомогательных каналов фильтра AuxChannel, схемы AuxChRDEUnit, получать инфор-
//мацию по мощностям на входе и выходе фильтра, весовым коэффициентам,  корреля-
//ционным матрицам и т.д.
//Для моделирования работы самого фильтра в класс вводится указатель на базовый
//класс из заголовочного файла "weightsum.h" - TMultiInSingleOutWeightSummator.
//Это  дает  возможность  пользователю использовать в качестве пространственного
//фильтра разнообразные статистические модели весовых сумматоров со многими вхо-
//дами и одним выходом. Данные  весовые сумматоры могут отличаться друг от друга
//по  конфигурации (одноступенчатые и многоступенчатые), по алгоритмам настройки
//весовых  коэффициентов (алгоритмам адаптации) и по схемам стабилизации динами-
//ческих параметров фильтров (наличию/отсутствию, типу схемы стабилизации при ее
//использовании). Все эти весовые сумматоры должно объединять только одно: все
//они должны быть со МНОГИМИ ВХОДАМИ (more than 1) и ОДНИМ ВЫХОДОМ. Число входов
//сумматора  определяется  автоматически при выборе конкретного типа сумматора и
//определяется  количеством  AuxChannel+1. Такая  реализация данного класса дает
//возможность  исследовать и  реализовывать  адаптивные пространственные фильтры
//как  по их  внутренним  параметрам (изменение значений весовых коэффициентов в
//зависимости от складывающейся помеховой ситуации), так и по их структуре, алго-
//ритмам настройки весовых коэффициентов и стабилизации их динамических парамет-
//ров. Напомним, что работа пространственного фильтра заключается в формировании
//нулей  диаграммы направленности (передаточной функции) в направлении на источ-
//ники помех. Количество дополнительных  каналов определяет  максимальное  число
//активных (шумовых) помех, которые могут быть подавлены фильтром. Считается, что
//полезный источник сигнала располагается в главном луче (лепестке) LAA. С целью
//недопущения подавления его приема пространственным фильтром используется схема
//защиты главного луча, реализованная посредством AuxChRDEUnit.
//Статистическое  моделирование  адаптивной  LAA  с пространственным  фильтром и
//ОСНОВНЫМ  каналом  должно производится за много реализаций с определенной дли-
//тельностью каждой реализации. Рекомендуемое количество реализаций больше 20.
//Чем  больше проведено реализаций, тем точнее становятся результаты аналитичес-
//кого и статистического моделирования. В начале  моделирования  весовые коэффи-
//циенты фильтра ненастроены и они пропускают входное воздействия  без его изме-
//нения. Так  как при  статистическом  моделировании должно быть проведено много
//реализаций, то весовые коэффициенты адаптивного фильтра на последнем шаге каж-
//дой реализации усредняются и заносятся в матрицу AvW. По усредненным значениям
//весовых коэффициентов вычисляется передаточная характеристика адаптивной LAA.
//По окончании моделирования имеется возможность построить передаточную характе-
//ристику  адаптивной LAA, которая будет зависеть от многих факторов, таких как,
//от  входного  воздействия, конфигурации адаптивной решетки и пространственного
//фильтра, алгоритма настройки и стабилизации работы фильтра, внутренних парамет-
//ров фильтра и т.д.
//******************************************************************************
class TSpaceFilterBaseCh: public TStatPreProcSFilterBaseCh {
//------------------------------------------------------------------------------
// ОБЪЯВЛЕНИЕ ЗАЩИЩЕННЫХ ЧЛЕНОВ-ДАННЫХ КЛАССА TSpaceFilterBaseCh
//------------------------------------------------------------------------------
 protected:
  //Указатель на базовый абстрактный класс весового сумматора
  TMultiInSingleOutWeightSummator* Filter;//Пространственный адаптивный фильтр
  //Выборки полезного сигнала, аддитивной помехи и внутреннего шума на входе фильтра
  cvector Us;//Входная выборка ПОЛЕЗНОГО СИГНАЛА
  cvector Ujam;//Входная выборка АДДИТИВНОЙ ПОМЕХИ
  cvector Unoise;//Входная выборка ВНУТРЕННЕГО ШУМА
  //Суммарная выборка компонент входного воздействия на входе пространственного
  //адаптивного фильтра: Us+Ujam+Unoise
  cvector Usum;
  //Усредненные по реализациям в ходе моделирования весовые коэффициенты
  cmatrix AvW;
//------------------------------------------------------------------------------
// I. ОБЪЯВЛЕНИЕ ОБЩЕДОСТУПНЫХ ЧЛЕНОВ-ФУНКЦИЙ КЛАССА TSpaceFilterBaseCh
//------------------------------------------------------------------------------
 public:
//*****************************************************************************
// ОБЪЯВЛЕНИЕ КОНСТРУКТОРОВ И ДЕСТРУКТОРА КЛАССА TSpaceFilterBaseCh
//*****************************************************************************
 //Объявление конструктора по умолчанию
 TSpaceFilterBaseCh() : TStatPreProcSFilterBaseCh() {Initial();};
 //Объявление конструктора с аргументами
 TSpaceFilterBaseCh(TInputSignal* InSignal) :
  TStatPreProcSFilterBaseCh(InSignal) {Initial();};
 //Объявление конструктора копирования
 TSpaceFilterBaseCh(TSpaceFilterBaseCh& SFilter) {*this = SFilter;};
 //Объявление деструктора класса
 ~TSpaceFilterBaseCh() {Reset();};
//******************************************************************************
// ОБЩЕДОСТУПНЫЕ ФУНКЦИИ ДЛЯ РАБОТЫ С КЛАССОМ TSpaceFilterBaseCh
//******************************************************************************
 //Определение РАБОТОСПОСОБНОСТИ предпроцессора фильтра
 bool IsPreProcessorReady() const {return TStatPreProcSFilterBaseCh::IsReady();};
 //Определение РАБОТОСПОСОБНОСТИ пространственного фильтра
 bool IsSpaceFilterReady() const
 {return ((Filter != NULL) && (Filter->IsReady())) ? ON : OFF;};
 //Определение РАБОТОСПОСОБНОСТИ всей АДАПТИВНОЙ Linear Antenna Array
 bool IsReady() const
 {return ((IsPreProcessorReady()) && (IsSpaceFilterReady())) ? ON : OFF;};
 //-----------------------------------------------------------------------------
 //Получение усредненных коэффициентов весового сумматора
 //-----------------------------------------------------------------------------
 cmatrix& GetW(cmatrix& W) const
 {if (!IsReady()) W.Set(0L,0L,false);
  else W = AvW;
  return W;
 };
 //-----------------------------------------------------------------------------
 //Установка усредненных коэффициентов AvW в весовой сумматор
 //-----------------------------------------------------------------------------
 bool SetW()
 {if (!IsReady()) return false;
  return Filter->SetW(AvW);
 };
 //Передаточная функция (Transfer function) адаптивной LAA с пространственным
 //фильтром и ОСНОВНЫМ каналом /выход ОСНОВНОГО канала/
 rvector& TFunc(rvector& P, const TEvenCloseInterval& Sector, bool Norm = ON,
                bool Db = ON, double MinDb = -60.0) const;
 //Передаточная функция (Transfer function) предпроцессора адаптивной LAA
 //с пространственным фильтром и ОСНОВНЫМ каналом /выход AuxCh + BaseChannel/
 bool TF_Preprocessor(rmatrix& P, const TEvenCloseInterval& Sector,
                      bool Norm = ON, bool Db = ON, double MinDb = -60.0) const
 {return TStatPreProcSFilterBaseCh::TFunc(P,Sector,Norm,Db,MinDb);};

 //-----------------------------------------------------------------------------
 //ФУНКЦИИ, ИСПОЛЬЗУЕМЫЕ ДЛЯ МОДЕЛИРОВАНИЯ КЛАССА TSpaceFilterBaseCh
 //-----------------------------------------------------------------------------
 //Начальная установка параметров AdaptLAA при моделировании: начальная установ-
 //ка весовых коэффициентов пространственного фильтра
 bool SimulateInit()
 {return (IsReady()) ? Filter->SimulateInit() : OFF;};

 //Расчет корреляционных матриц компонент входного воздействия при моделировании
 //работы адаптивного фильтра
 bool Update();

 //Настройка весовых коэффициентов пространственного фильтра
 bool Tuning() {return (IsReady()) ? Filter->Tuning(Usum) : OFF;};

 //Усреднение весовых коэффициентов пространственного фильтра
 bool AverageW(uint Realization);

 //Расчет мощностей на входе адаптивной LAA /ОСНОВНОЙ канал/
 //Мощность ПОЛЕЗНОГО СИГНАЛА на входе фильтра
 double InPSignal() const
 {return (IsReady()) ? norm(Us.Get(Us.Size()-1)) : 0;};
 //Мощность АКТИВНОЙ АДДИТИВНОЙ ПОМЕХИ на входе фильтра
 double InPJammer() const
 {return (IsReady()) ? norm(Ujam.Get(Ujam.Size()-1)) : 0;};
 //Мощность ВНУТРЕННЕГО ШУМА на входе фильтра
 double InPNoise() const
 {return (IsReady()) ? norm(Unoise.Get(Unoise.Size()-1)) : 0;};

 //Расчет мощностей на выходе адаптивной LAA /ОСНОВНОЙ канал/
 //Мощность ПОЛЕЗНОГО СИГНАЛА на выходе фильтра
 double OutPSignal() const
 {if (!IsReady()) return 0.0;//Адаптивный фильтр НЕРАБОТОСПОСОБЕН
  cvector V = Us;//Формирование входной выборки ПОЛЕЗНОГО СИГНАЛА
  return Filter->TF_WSum(V);//Мощность ПОЛЕЗНОГО СИГНАЛА на выходе фильтра
 };
 //Мощность АДДИТИВНОЙ АКТИВНОЙ ПОМЕХИ на выходе фильтра
 double OutPJammer() const
 {if (!IsReady()) return 0.0;//Адаптивный фильтр НЕРАБОТОСПОСОБЕН
  cvector V = Ujam;//Формирование входной выборки АДДИТИВНОЙ ПОМЕХИ
  return Filter->TF_WSum(V);//Мощность АДДИТИВНОЙ ПОМЕХИ на выходе фильтра
 };
 //Мощность ВНУТРЕННЕГО ШУМА на выходе фильтра
 double OutPNoise() const
 {if (!IsReady()) return 0.0;//Адаптивный фильтр НЕРАБОТОСПОСОБЕН
  cvector V = Unoise;//Формирование входной выборки ВНУТРЕННЕГО ШУМА
  return Filter->TF_WSum(V);//Мощность ВНУТРЕННЕГО ШУМА на выходе фильтра
 };

 //-----------------------------------------------------------------------------
 // Мощности компонент входного воздействия на ВХОДЕ Adaptive LAA
 // / [RDEChannels] + AuxChannels + BaseChannel /
 //-----------------------------------------------------------------------------
 //Входная мощность ПОЛЕЗНОГО СИГНАЛА
 rvector& ExtPsignal(rvector& P, bool lExtend = ON) const
 {if (IsReady()) TStatPreProcSFilterBaseCh::InPsignal(P, lExtend);
  else P.Resize(0);
  return P;
 };
 //Входная мощность АДДИТИВНОЙ АКТИВНОЙ ПОМЕХИ
 rvector& ExtPjammer(rvector& P, bool lExtend = ON) const
 {if (IsReady()) TStatPreProcSFilterBaseCh::InPjammer(P, lExtend);
  else P.Resize(0);
  return P;
 };
 //Входная мощность ВНУТРЕННЕГО ШУМА
 rvector& ExtPnoise(rvector& P, bool lExtend = ON) const
 {if (IsReady()) TStatPreProcSFilterBaseCh::InPnoise(P, lExtend);
  else P.Resize(0);
  return P;
 };
 //-----------------------------------------------------------------------------
 //  Мощности компонент входного воздействия на ВЫХОДЕ предпроцессора AdaptLAA
 //  /ВХОД АДАПТИВНОГО ФИЛЬТРА/  / [RDEChannels] + AuxChannels + BaseChannel /
 //-----------------------------------------------------------------------------
 //Мощность ПОЛЕЗНОГО СИГНАЛА на входе фильтра
 rvector& InPsignal(rvector& P, bool lExtend = ON) const;
 //Мощность АДДИТИВНОЙ АКТИВНОЙ ПОМЕХИ на входе фильтра
 rvector& InPjammer(rvector& P, bool lExtend = ON) const;
 //Мощность ВНУТРЕННЕГО ШУМА на входе фильтра
 rvector& InPnoise(rvector& P, bool lExtend = ON) const;

 //-----------------------------------------------------------------------------
 //  Мощности компонент входного воздействия на ВЫХОДЕ Adaptive LAA
 //  /ВЫХОД АДАПТИВНОГО ФИЛЬТРА/  / [RDEChannels] + AuxChannels + BaseChannel /
 //  Для каждой ступени фильтра /при использовании фильтров многоступенчатой
 //  структуры/
 //-----------------------------------------------------------------------------
 //Выходная мощность ПОЛЕЗНОГО СИГНАЛА
 rvector& OutPsignal(rvector& P) const
 {if (!IsReady()) {P.Resize(0); return P;} //Адаптивный фильтр НЕРАБОТОСПОСОБЕН
  cvector V = Us;
  return Filter->TF_WSum(P,V);//Мощность ПОЛЕЗНОГО СИГНАЛА на выходах фильтра
 };
 //Выходная мощность АДДИТИВНОЙ АКТИВНОЙ ПОМЕХИ
 rvector& OutPjammer(rvector& P) const
 {if (!IsReady()) {P.Resize(0); return P;} //Адаптивный фильтр НЕРАБОТОСПОСОБЕН
  cvector V = Ujam;
  return Filter->TF_WSum(P,V);//Мощность АДДИТИВНОЙ ПОМЕХИ на выходах фильтра
 };
 //Выходная мощность ВНУТРЕННЕГО ШУМА
 rvector& OutPnoise(rvector& P) const
 {if (!IsReady()) {P.Resize(0); return P;} //Адаптивный фильтр НЕРАБОТОСПОСОБЕН
  cvector V = Unoise;
  return Filter->TF_WSum(P,V);//Мощность ВНУТРЕННЕГО ШУМА на выходах фильтра
 };

 //-----------------------------------------------------------------------------
 // ФУНКЦИИ ВВОДА - ВЫВОДА ПАРАМЕТРОВ КЛАССА TSpaceFilterBaseCh
 //-----------------------------------------------------------------------------
 //Получение свойств класса в виде строкового объекта string
 string& Properties(string& s, string& title, uint indent = 0) const;
 //Ввод параметров объекта из стандартного потока ввода cin
 void Edit();
 //Редактирование свойств пространственного фильтра
 void EditFilter();
 //Перегрузка оператора ввода (>>) для ввода параметров класса
 friend istream& operator >>(istream& in, TSpaceFilterBaseCh& SFilter);
 //Перегрузка оператора вывода (<<) для вывода параметров класса
 friend ostream& operator <<(ostream& out, const TSpaceFilterBaseCh& SFilter)
 {string s; return out << SFilter.Properties(s,STR_NULL);};
 //Объявление перегруженного оператора присваивания operator =
 void operator =(const TSpaceFilterBaseCh& SFilter);
//------------------------------------------------------------------------------
//II. ОБЪЯВЛЕНИЕ ЗАЩИЩЕННЫХ ЧЛЕНОВ-ФУНКЦИЙ КЛАССА TSpaceFilterBaseCh
//------------------------------------------------------------------------------
 protected:
  //Начальная инициализация членов-данных класса при конструировании
  void Initial()
  {Filter = NULL; AvW =cmatrix();
   Us = cvector(); Ujam = cvector(); Unoise = cvector(); Usum = cvector();
  };
  void Reset() //Сброс всех характеристик класса
  {if (Filter != NULL) {delete Filter; Filter = NULL;}
   Us = cvector(); Ujam = cvector(); Unoise = cvector(); Usum = cvector();
   AvW =cmatrix();
   TStatPreProcSFilterBaseCh::Reset();
  };
  //Установка размерности фильтра и размеров векторов компонент входного сигнала
  void Set()
  {ulong size = FilterSize();
   AvW = cmatrix();
   if (Filter != NULL) {Filter->SetInput(size); Filter->GetW(AvW);}
   Us.Resize(size); Ujam.Resize(size); Unoise.Resize(size); Usum.Resize(size);
   Us = C_ZERO; Ujam = C_ZERO; Unoise = C_ZERO; Usum = C_ZERO;
  };
};
//Конец объявления класса TSpaceFilterBaseCh
//The end of class TSpaceFilterBaseCh declaration

//--------------------- The end of file "adaptlaa.h" ---------------------------
#endif //ЗАВЕРШЕНИЕ ОБЪЯВЛЕНИЯ ЗАГОЛОВОЧНОГО ФАЙЛА "ADAPTLAA.H"

