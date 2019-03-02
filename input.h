//input.h
//Объявление класса TInputSignal - Входной сигнал
//Copyright (c) JohnSoft 1998. All rights reserved. Borland C++ 5.01.
//Initial date: November,8,1998. Final date:
//Заголовочный файл input.h объявляет класс TInputSignal, который позволяет
//описать входное воздействие, поступающее на вход диаграммообразующей схемы
//адаптивной линейной антенной решетки. Входной сигнал представляет из себя
//N-мерный вектор мгновенных комплексных значений напряжений в момент времени t
//ПОЛЕЗНОГО СИГНАЛА, СУММАРНОЙ ПОМЕХИ и ВНУТРЕННЕГО ШУМА ПРИЕМНОГО КАНАЛА на вы-
//ходе линейной антенной решетки, где N - количество приемных каналов адаптивной
//АР. Напряжения полезного сигнала, суммарной помехи и шума разбиваются на две
//поляризационные составляющие - ВЕРТИКАЛЬНУЮ и ГОРИЗОНТАЛЬНУЮ. Класс TInputSignal
//образуется путем МНОЖЕСТВЕННОГО НАСЛЕДОВАНИЯ свойств класса ExtSignal и класса
//TLinearArray, так как мгновенные значения напряжения на выходе АР, определяются
//свойствами этих двух классов.

#if !defined(__INPUT_H)
#define __INPUT_H

#include "exsignal.h" //Заголовочный файл, аддитивного внешнего сигнала
#include "linarray.h" //Заголовочный файл, линейной антенной решетки
//#include "mathlib.h"  //Заголовочный файл с генератором случайных чисел

#define CONSOLE 1     //В программе используется консольный ввод-вывод

//ОБЪЯВЛЕНИЕ ИДЕНТИФИКАТОРОВ ВИДА ПОЛЯРИЗАЦИИ СИГНАЛОВ
const bool VP_SIGNAL = true;//Сигнал с вертикальной поляризацией
const bool HP_SIGNAL = false;//Сигнал с горизонтальной поляризацией

//******************************************************************************
// ОБЪЯВЛЕНИЕ КЛАССА - TInputSignal - ВХОДНОЙ СИГНАЛ (ВХОДНОЕ ВОЗДЕЙСТВИЕ)
//******************************************************************************
class TInputSignal : public ExtSignal, public TLinearArray {
//------------------------------------------------------------------------------
// ОБЪЯВЛЕНИЕ ЗАЩИЩЕННЫХ ЧЛЕНОВ-ДАННЫХ КЛАССА TInputSignal
//------------------------------------------------------------------------------
protected:
 //Входной сигнал с вертикальной поляризацией
 cvector VPSignal;
 cmatrix VPJammer;
 cvector VPNoise;
 //Входной сигнал с горизонтальной поляризацией
 cvector HPSignal;
 cmatrix HPJammer;
 cvector HPNoise;
 //Векторы со свойствами компонент внешнего сигнала
 rvector Ampl;//Мгновенное значение амплитуд компонент внешнего сигнала
 rvector Phase;//Мгновенное значение фаз компонент внешнего сигнала
 rvector Azimuth;//Положение источников сигналов по горизонтали (в радианах)
 rvector Elevate;//Положение источников сигналов по вертикали (в радианах)
 rvector PolarAngle;//Углы поляризации компонент внешнего сигнала (в радианах)
 rvector PhaseVCP;//Вертикальные составляющие фаз поляризации (в радианах)
 rvector PhaseHCP;//Горизонтальные составляющие фаз поляризации (в радианах)

//-------------------------------------------------------------------
// I. ОБЪЯВЛЕНИЕ ОБЩЕДОСТУПНЫХ ЧЛЕНОВ-ФУНКЦИЙ КЛАССА TInputSignal
//-------------------------------------------------------------------
public:
 //Количество компонент внешнего сигнала (с полезным сигналом)
 uint GetSignalCount() const {return ExtSignal::Count;};
 //Количество активных источников помех во внешнем сигнале
 uint Jammers() const {return ExtSignal::Jammers();};
 //Количество элементов линейной АР (с основным элементом)
 ulong LAAElem() const {return TLinearArray::Count;};
 //Обновление данных: модельного времени, значений случайных величин в списках
 //параметров функций объектов базовых классов ExtSignal и TLinearArray, а также
 //расчет мгновенных комплексных значений напряжений (горизонтальной и вертикаль-
 //ной составляющих) на выходе приемных каналов АР
 void Update(double time, bool casual = ON);

 //Передача указателя на вектор полезного сигнала с горизонтальной и
 //вертикальной поляризационными составляющими
 const cvector* GetHPSignal() const {return &HPSignal;};
 const cvector* GetVPSignal() const {return &VPSignal;};
 //Передача указателя на матрицу аддитивной помехи с горизонтальной и
 //вертикальной поляризационными составляющими
 const cmatrix* GetHPJammer() const {return &HPJammer;};
 const cmatrix* GetVPJammer() const {return &VPJammer;};
 //Передача указателя на вектор внутреннего шума с горизонтальной и
 //вертикальной поляризационными составляющими
 const cvector* GetHPNoise() const {return &HPNoise;};
 const cvector* GetVPNoise() const {return &VPNoise;};

 //Формирование вектора мощностей компонент внешнего сигнала для АНАЛИТИЧЕСКОЙ
 //МОДЕЛИ представления внешнего сигнала
 rvector GetPower(bool PolarType) const;
 //Формирование вектора угловых координат (азимутальных углов или углов места)
 //источников внешнего сигнала для АНАЛИТИЧЕСКОЙ МОДЕЛИ представления внешнего
 //сигнала
 rvector GetAngle(bool DispLAA, bool Deg = RAD) const;
 //Вектор напряжений полезного сигнала на выходе всех каналов LAA
 cvector& StatSignal(cvector& V, bool PolarType) const;
 //Вектор напряжений аддитивной помехи на выходе всех каналов LAA
 cvector& StatJammer(cvector& V, bool PolarType) const;
 //Вектор напряжений внутреннего шума на выходе всех каналов LAA
 cvector& StatNoise(cvector& V, bool PolarType) const;

 //Вектор напряжений полезного сигнала на выходе всех каналов LAA
 cvector& Signal(cvector& V, bool PolarType) const
 {if (PolarType == VP_SIGNAL) V = VPSignal;
  else V = HPSignal;
  return V;
 };
 //Матрица напряжений источников активных помех на выходе всех каналов LAA
 cmatrix& Jammer(cmatrix& A, bool PolarType) const
 {if (PolarType == VP_SIGNAL) A = VPJammer;
  else A = HPJammer;
  return A;
 };
 //Вектор напряжений аддитивной помехи на выходе всех каналов LAA
 cvector& Jammer(cvector& V, bool PolarType) const;
 //Вектор напряжений внутреннего шума на выходе всех каналов LAA
 cvector& Noise(cvector& V, bool PolarType) const
 {if (PolarType == VP_SIGNAL) V = VPNoise;
  else V = HPNoise;
  return V;
 };

 //Передача вектора напряжений полезного сигнала на входе адаптивной LAA для
 //всех каналов LAA
 cvector Signal(bool PolarType) const
 {if (PolarType == VP_SIGNAL) return VPSignal;
  else return HPSignal;
 };
 //Формирование вектора напряжений полезного сигнала на входе адаптивной LAA для
 //каналов LAA, номера которых расположены в векторе Channel
 cvector Signal(const ivector& Channels, bool PolarType) const
 {if (PolarType == VP_SIGNAL) return VPSignal.SubVector(Channels);
  else return HPSignal.SubVector(Channels);
 };
 //Передача матрицы напряжений источников активных помех на входе адаптивной LAA
 //для всех каналов LAA
 cmatrix Jammer(bool PolarType) const
 {if (PolarType == VP_SIGNAL) return VPJammer;
  else return HPJammer;
 };
 //Формирование матрицы напряжений всех источников активных помех на входе
 //адаптивной LAA для каналов, номера которых расположены в векторе Channels
 cmatrix Jammer(const ivector& Channels, bool PolarType) const
 {if (PolarType == VP_SIGNAL) return VPJammer.SubMatrixByRows(Channels,ON);
  else return HPJammer.SubMatrixByRows(Channels,ON);
 };
 //Передача вектора напряжений внутреннего шума на входе адаптивной LAA для
 //всех каналов LAA
 cvector Noise(bool PolarType) const
 {if (PolarType == VP_SIGNAL) return VPNoise;
  else return HPNoise;
 };
 //Формирование вектора напряжений внутреннего шума на входе адаптивной LAA для
 //каналов LAA, номера которых расположены в векторе Channels
 cvector Noise(const ivector& Channels, bool PolarType) const
 {if (PolarType == VP_SIGNAL) return VPNoise.SubVector(Channels);
  else return HPNoise.SubVector(Channels);
 };
 //Формирование вектора напряжений на элементах LAA, номера которых расположены
 //в векторе Channels, при действии одного источника сигнала ЕДИНИЧНОЙ МОЩНОСТИ
 //с направления Angle относительно раскрыва LAA
 cvector TestSignal(const ivector& Channels, double Angle, bool Deg = DEG,
                    bool Check = ON) const;
 //Формирование матрицы напряжений на элементах LAA, номера которых расположены
 //в векторе Channels, при действии одного источника сигнала ЕДИНИЧНОЙ МОЩНОСТИ
 //с направлений передаваемых в векторе Angle относительно раскрыва LAA
 cmatrix TestSignal(const ivector& Channels, const rvector& Angle,
                    bool Deg = DEG, bool Check = ON) const;
//*****************************************************************************
// Объявление функций расчета мощностей сигнала, аддитивной помехи и внутреннего
// шума в каждом из приемных каналов АР, а также их суммарное значение
//*****************************************************************************
 double CalcPSignal(bool PolarType);
 double CalcPJammer(bool PolarType);
 double CalcPNoise(bool PolarType);
 double CalcPSignal(ulong Channel, bool PolarType);
 double CalcPJammer(ulong Channel, bool PolarType);
 double CalcPNoise(ulong Channel, bool PolarType);
 double SJNRatio(bool PolarType);//Результирующее отношение сигнал/помеха+шум
 //Отношение сигнал/помеха+шум в канале АР
 double SJNRatio(ulong Channel, bool PolarType);

 //*****************************************************************************
 //Формирование корреляционных матриц входного воздействия: ПОЛЕЗНОГО СИГНАЛА,
 //АДДИТИВНОЙ АКТИВНОЙ ПОМЕХИ и ВНУТРЕННЕГО ШУМА
 //*****************************************************************************
 bool GetRss(cmatrix& R, bool PolarType) const
 {if (PolarType == VP_SIGNAL) return CorrelationMatrix(VPSignal,R);
  else return CorrelationMatrix(HPSignal,R);
 };
 bool GetRjj(cmatrix& R, bool PolarType) const
 {if (PolarType == VP_SIGNAL) return CorrelationMatrix(VPJammer,R);
  else return CorrelationMatrix(HPJammer,R);
 };
 bool GetRnn(cmatrix& R, bool PolarType) const;

//******************************************************************************
// ОБЪЯВЛЕНИЕ КОНСТРУКТОРОВ И ДЕСТРУКТОРА КЛАССА TInputSignal
//******************************************************************************
 //Объявление конструктора по умолчанию
 TInputSignal();
 //Объявление конструктора с аргументами
 //Создание объекта класса TInputSignal по данным из файлов extsignal и linarray
 TInputSignal(const char* extsignal, const char* linarray);
 //Объявление деструктора класса
 ~TInputSignal();

//-------------------------------------------------------------------
// II. ОБЪЯВЛЕНИЕ ЗАКРЫТЫХ ЧЛЕНОВ-ФУНКЦИЙ КЛАССА TInputSignal
//-------------------------------------------------------------------
private:

//-------------------------------------------------------------------
// III. ОБЪЯВЛЕНИЕ ЗАЩИЩЕННЫХ ЧЛЕНОВ-ФУНКЦИЙ КЛАССА TInputSignal
//-------------------------------------------------------------------
protected:
 void Init();//Начальная инициализация характеристик TInputSignal
 void Reset();//Сброс всех характеристик TInputSignal

};//Завершение объявления класса TInputSignal

//---------------------- The end of file "input.h" -----------------------------

#endif //ЗАВЕРШЕНИЕ ОБЪЯВЛЕНИЯ ЗАГОЛОВОЧНОГО ФАЙЛА "INPUT.H"

