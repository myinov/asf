//antenna.cpp
//Реализация КЛАССА АНТЕННА -- Antenna
//Initial date: September,15,1998. Final date: September,16,1998.
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

#include "antenna.h"
//Информационный массив по наименованиям и характеристикам направленности антенн
const ANT_INFO AntennInfo[] = {
 {UNEXIST,UNEXIST,UNEXIST,"Антенна не найдена","Antenna not found","UNEXIST" },
 {HW_DIPOLE,FE_HWDIPOLE,FH_HWDIPOLE,"Полуволновой вибратор",       //1
  "Half-wave dipole", "HW_DIPOLE" },
 {HWSLOT_ANT, FH_HWDIPOLE, FE_HWDIPOLE, "Полуволновая щель",       //2
  "Half-wave slot antenna", "HWSLOT_ANT" },
 {PARAB_ANT, FEH_PARABANT, FEH_PARABANT, "Параболическая антенна", //3
  "Parabolic antenna", "PARAB_ANT" }
};

//Объявление таблицы функций, описывающих характеристики направленности антенн
//заданного типа в E- и H-плоскостях.
LAW* AntDirect = NULL;
uint AntDirectCount = 0;

//Объявление таблицы возможных случайных законов изменения АМПЛИТУДНОЙ и ФАЗОВОЙ
//характеристики шума на выходе антенны
LAW* TableNoiseLaw = NULL;
uint TableNoiseLawCount = 0;

const double DegRad = 0.01745329251994;
const double RadDeg = 57.2957795131;

//===========================================================
//I. ОПИСАНИЕ ОБЩЕДОСТУПНЫХ ЧЛЕНОВ-ФУНКЦИЙ КЛАССА ANTENNA
//   PUBLIC MEMBER-FUNCTION OF CLASS
//===========================================================

//*****************************************************************************
//СТАТУС: I.1; public; constructor by default
//НАИМЕНОВАНИЕ ФУНКЦИИ: Antenna()
//НАЗНАЧЕНИЕ: Конструктор по умолчанию класса Antenna.
//Функция предназначена для создания класса Antenna по умолчанию. Характеристики
//антенны по умолчанию:
//1. Type = HW_DIPOLE (линейный полуволновый вибратор)
//2. E_Location = MERIDIAN (всенаправленная в экваториальной плоскости - по азимуту)
//3. E_DirectType = FE_HWDIPOLE;
//4. H_DirectType = FH_HWDIPOLE;
//5. NoiseAmplLawType = RELAY_LAW
//6. NoisePhaseLawType = RAND2PI_LAW
//По умолчанию моделируется гауссовский (белый) шум на выходе антенны.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ: нет
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: нет
//*****************************************************************************
Antenna::Antenna()
{
 //Инициализация индикатора First
 if (TableNoiseLawCount == 0) First = true;
 else First = false;

 //Создание таблицы случайных законов изменения шума на выходе антенны
 if (TableNoiseLawCount == 0) CreateTableOfNoiseLaw();

 //Создание таблицы характеристик направленности антенн в E- и H-плоскостях
 if (AntDirectCount == 0) CreateTableOfAntDirect();

 //Инициализация характеристик антенны
 Type = HW_DIPOLE; //линейный полуволновый вибратор
 //(всенаправленная в экваториальной плоскости - по азимуту),т.к. магнитная
 //плоскость антенны будет совпадать с экваториальной плоскостью Земли.
 E_Location = MERIDIAN;

 E_DirectType = FE_HWDIPOLE; //Вид характеристики направленности в E-плоскости
 E_Direct = E_HWDipoleFunc;  //Характеристика направленности в E-плоскости
 E_DirectArg = CreateArgList(1);//Параметры хар-ки направленности в E-плоскости

 H_DirectType = FH_HWDIPOLE; //Вид характеристики направленности в H-плоскости
 H_Direct = H_HWDipoleFunc;  //Характеристика направленности в H-плоскости
 H_DirectArg = CreateArgList(1);//Параметры хар-ки направленности в H-плоскости

 NoiseAmplLawType = RELAY_LAW; //Вид закона изменения амплитуды шума
 NoiseAmpl = RelayLaw; //Закон изменения амплитуды шума
 NoiseAmplArg = CreateArgList(1); //Параметры закона изменения амплитуды шума

 NoisePhaseLawType = RAND2PI_LAW; //Вид закона изменения амплитуды шума
 NoisePhase = Rand2PiLaw; //Закон изменения амплитуды шума
 NoisePhaseArg = CreateArgList(1); //Параметры закона изменения амплитуды шума
}
//*****************************************************************************
//СТАТУС: I.2; public; constructor with arguments
//НАИМЕНОВАНИЕ ФУНКЦИИ: Antenna(uint AntType, bool EPlLocate = MERIDIAN,
//                      ARGLIST* E_Arg = NULL, ARGLIST* H_Arg = NULL)
//НАЗНАЧЕНИЕ: Конструктор класса Antenna c аргументами.
//Функция предназначена для создания класса Antenna. Конструктор принимает сле-
//дующие параметры антенны:
//1. AntType - тип антенны
//2. EPlLocate - сонаправленность E-плоскости антенны с меридиональной или эква-
//ториальной плоскостью Земли (по умолчанию - EPlLocate = MERIDIAN)
//3. E_Arg - список параметров функции диаграммы направленности в E-плоскости
//4. H_Arg - список параметров функции диаграммы направленности в H-плоскости
//По умолчанию E_Arg и H_Arg равны NULL.
//Параметры антенны устанавливаемые по умолчанию:
//1. NoiseAmplLawType = RELAY_LAW
//2. NoisePhaseLawType = RAND2PI_LAW
//По умолчанию моделируется гауссовский (белый) шум на выходе антенны.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ: см. выше
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: нет
//*****************************************************************************
Antenna::Antenna(uint AntType, bool EPlLocate, ARGLIST* E_Arg, ARGLIST* H_Arg)
{
 //Инициализация индикатора First
 if (TableNoiseLawCount == 0) First = true;
 else First = false;

 //Создание таблицы случайных законов изменения шума на выходе антенны
 if (TableNoiseLawCount == 0) CreateTableOfNoiseLaw();
 //Создание таблицы характеристик направленности антенн в E- и H-плоскостях
 if (AntDirectCount == 0) CreateTableOfAntDirect();

 //Начальная инициализация функций направленности антенны в E- и H-плоскости
 E_Direct = E_HWDipoleFunc;
 H_Direct = H_HWDipoleFunc;
 E_DirectArg = NULL;
 H_DirectArg = NULL;

 //Устанавливаем характеристики антенны
 SetPerformance(AntType, EPlLocate, E_Arg, H_Arg);

 NoiseAmplLawType = RELAY_LAW; //Вид закона изменения амплитуды шума
 NoiseAmpl = RelayLaw; //Закон изменения амплитуды шума
 NoiseAmplArg = CreateArgList(1); //Параметры закона изменения амплитуды шума

 NoisePhaseLawType = RAND2PI_LAW; //Вид закона изменения амплитуды шума
 NoisePhase = Rand2PiLaw; //Закон изменения амплитуды шума
 NoisePhaseArg = CreateArgList(1); //Параметры закона изменения амплитуды шума
}
//*****************************************************************************
//СТАТУС: I.3; public; constructor with arguments
//НАИМЕНОВАНИЕ ФУНКЦИИ:
// Antenna(uint AntType, bool EPlLocate, uint AmplLaw, uint PhaseLaw,
//         ARGLIST* E_Arg = NULL, ARGLIST* H_Arg = NULL, ARGLIST* AmplArg = NULL,
//         ARGLIST* PhaseArg = NULL);
//НАЗНАЧЕНИЕ: Конструктор класса Antenna c аргументами.
//Функция предназначена для создания класса Antenna. Данный конструктор инициали-
//зирует все члены-данные класса ANTENNA без умолчания.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. AntType - тип антенны
//2. EPlLocate - сонаправленность E-плоскости антенны с меридиональной или эква-
//   ториальной плоскостью Земли (по умолчанию - EPlLocate = MERIDIAN)
//3. uint AmplLaw - идентификатор закона изменения амплитуды шума антенны
//4. PhaseLaw - идентификатор закона изменения фазы шума антенны
//5. E_Arg - список параметров функции диаграммы направленности в E-плоскости
//6. H_Arg - список параметров функции диаграммы направленности в H-плоскости
//7. AmplArg - параметры закона изменения амплитуды (если они требуются)
//8. PhaseArg - параметры закона изменения фазы (если они требуются
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: нет
//*****************************************************************************
Antenna::Antenna(uint AntType, bool EPlLocate, uint AmplLaw, uint PhaseLaw,
         ARGLIST* E_Arg, ARGLIST* H_Arg, ARGLIST* AmplArg, ARGLIST* PhaseArg)
{
 //Инициализация индикатора First
 if (TableNoiseLawCount == 0) First = true;
 else First = false;

 //Создание таблицы случайных законов изменения шума на выходе антенны
 if (TableNoiseLawCount == 0) CreateTableOfNoiseLaw();
 //Создание таблицы характеристик направленности антенн в E- и H-плоскостях
 if (AntDirectCount == 0) CreateTableOfAntDirect();

 //Начальная инициализация функций направленности антенны в E- и H-плоскости
 E_Direct = E_HWDipoleFunc;
 H_Direct = H_HWDipoleFunc;
 E_DirectArg = NULL;
 H_DirectArg = NULL;

 //Начальная инициализация случайных законов изменения амплитуды и шума
 //на выходе приемной антенны
 NoiseAmplArg = NULL; //Параметры закона изменения амплитуды шума
 NoisePhaseArg = NULL; //Параметры закона изменения фазы шума
 NoiseAmpl = Det0Law;
 NoisePhase = Det0Law;

 //Устанавливаем характеристики антенны
 SetPerformance(AntType, EPlLocate, E_Arg, H_Arg);

 //Устанавливаем законы изменения амплитуды и фазы шума на выходе антенны
 SetNoisePerform(AmplLaw, PhaseLaw, AmplArg, PhaseArg);
}
//*****************************************************************************
//СТАТУС: I.4; public; overload assignment operator
//НАИМЕНОВАНИЕ ФУНКЦИИ: Antenna& operator =(const Antenna &A)
//НАЗНАЧЕНИЕ: Функция перегрузки оператора присваивания.
//Функция предназначена для присваивания объекта класса Antenna другому объекту
//этого класса (Ant1 = Ant2).
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. const Antenna &A - ссылка на копируемый объект класса Antenna
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Antenna& - ссылка на копирующий объект.
//*****************************************************************************
Antenna& Antenna::operator =(const Antenna &A)
{
  //Предотвращение попытки присваивания объекта самому себе.
  if (this == &A) return *this;

  //Копирование характеристик антенны
  Type = A.Type; //Идентификатор типа антенны
  //Расположение E-плоскости антенны относительно меридиональной или экваториаль-
  //ной плоскости Земли
  E_Location = A.E_Location;

  E_DirectType = A.E_DirectType; //Вид диаграммы направленности в E-плоскости
  E_Direct = A.E_Direct; //Диаграмма направленности в E-плоскости
  //Параметры диаграммы направленности в E-плоскости
  E_DirectArg = CopyArgList(A.E_DirectArg, E_DirectArg);

  H_DirectType = A.H_DirectType; //Вид диаграммы направленности в H-плоскости
  H_Direct = A.H_Direct; //Диаграмма направленности в H-плоскости
  //Параметры диаграммы направленности в H-плоскости
  H_DirectArg = CopyArgList(A.H_DirectArg, H_DirectArg);

  NoiseAmplLawType = A.NoiseAmplLawType; //Вид закона изменения амплитуды шума
  NoiseAmpl = A.NoiseAmpl; //Закон изменения амплитуды шума
  //Параметры закона изменения амплитуды шума
  NoiseAmplArg = CopyArgList(A.NoiseAmplArg, NoiseAmplArg);

  NoisePhaseLawType = A.NoisePhaseLawType; //Вид закона изменения амплитуды шума
  NoisePhase = A.NoisePhase; //Закон изменения амплитуды шума
  //Параметры закона изменения амплитуды шума
  NoisePhaseArg = CopyArgList(A.NoisePhaseArg, NoisePhaseArg);
  //Индикатор порядка создания антенны
  First = false;
  return *this;
}
//*****************************************************************************
//СТАТУС: I.5; public; copy constructor of object class Antenna
//НАИМЕНОВАНИЕ ФУНКЦИИ: Antenna(const Antenna& A)
//НАЗНАЧЕНИЕ: Конструктор копирования объектов класса Antenna.
//Функция предназначена для создания копии объекта класса Antenna. Данный конст-
//руктор создает безопасную копию, в частности, не дублирует указатели на списки
//параметров законов изменения амплитуды и фазы шума на выходе антенны.
//Конструктор копии используется в следующих ситуациях:
//1. Когда объект класса Antenna используется для инициализации нового объекта
//   этого же класса. (ex. Antenna Ant2 = Ant1)
//2. Когда объект класса Antenna передается функции параметром типа класса по
//   значению.
//3. Когда функция возвращает объект класса Antenna (но не ссылку или указатель
//   на объект).
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. const Antenna &A - ссылка на копируемый объект класса Antenna
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: нет
//*****************************************************************************
Antenna::Antenna(const Antenna& A)
{
 //Инициализируем имеющиеся указатели нулевыми значениями
 E_DirectArg = NULL; //Параметры диаграммы направленности в E-плоскости
 H_DirectArg = NULL; //Параметры диаграммы направленности в H-плоскости
 E_Direct = E_HWDipoleFunc;
 H_Direct = H_HWDipoleFunc;

 NoiseAmplArg = NULL; //Параметры закона изменения амплитуды шума
 NoisePhaseArg = NULL; //Параметры закона изменения фазы шума
 NoiseAmpl = Det0Law;
 NoisePhase = Det0Law;

 *this = A; //Вызываем перегруженный оператор присваивания
}

//*****************************************************************************
//СТАТУС: I.6; public; Antenna class destructor
//НАИМЕНОВАНИЕ ФУНКЦИИ: ~Antenna()
//НАЗНАЧЕНИЕ: Деструктор класса Antenna.
//Функция предназначена для разрушения класса Antenna. При работе функции
//удаляется таблица случайных законов изменения фазы и амплитуды шума на выходе
//антенны TableNoiseLaw, списки параметров законов изменения амплитуды и фазы
//шума.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ: нет
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: нет
//*****************************************************************************
Antenna::~Antenna()
{
 //#if defined(CONSOLE)
 //  uint Index = GetIndexByAntType(Type);
 //  cout << "\nDestroy object of ANTENNA class..." << endl;
 //  cout << "Antenna type: " << GetAntNameByIndex(Index) << endl;
 //#endif
 //Удаление таблицы законов из памяти
 if ((TableNoiseLawCount != 0) && (First == true))
 {
  //#if defined(CONSOLE)
  // cout << "Delete Law Noise Table." << endl;
  // cout << "Delete Antennas Patterns Table." << endl;
  //#endif
  DeleteTable(TableNoiseLaw, TableNoiseLawCount);
  DeleteTable(AntDirect, AntDirectCount);
 }

 if (E_DirectArg != NULL) DeleteArgList(E_DirectArg);
 if (H_DirectArg != NULL) DeleteArgList(H_DirectArg);

 if (NoiseAmplArg != NULL) DeleteArgList(NoiseAmplArg);
 if (NoisePhaseArg != NULL) DeleteArgList(NoisePhaseArg);
}
//*****************************************************************************
//СТАТУС: I.7; public member-function of class Antenna
//НАИМЕНОВАНИЕ ФУНКЦИИ: string& GetPerformance(string& s, bool headfoot = ON)
//НАЗНАЧЕНИЕ:
//Функция формирует характеристики объекта Antenna в виде строки. Каждое свойство
//антенны разделяется символом '\n'. Характеристики антенны (пример):
//1. Antenna Type = Half-wave dipole
//2. E-plane is oriented on a MERIDIAN plane of the Earth
//3. Noise Amplitude Law = RELAY_LAW
//4. Noise Phase Law = RAND2PI_LAW
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. string& s - ссылка на строковый объект
//2. bool headfoot - индикатор формирования заголовка и подвала строки
//                   характеристик антенны
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: string& s - строка со свойствами сигнала
//*****************************************************************************
string& Antenna::GetPerformance(string& s, bool headfoot)
{uint Select;
 uint Index = GetIndexByAntType(Type);
 char buffer[80];
 ostrstream text (buffer, sizeof(buffer));

 s.resize(0);
 if (headfoot == ON)
 { text << "*******************  Antenna Performance  *******************"
        << endl << ends;
   s.append(text.str());
   text.seekp(0);
 }
 s.append("1. Antenna Type: ");
 text << GetAntNameByIndex(Index) << endl << ends;
 s.append(text.str());
 text.seekp(0);

 s.append("2. E-plane is oriented on a ");
 if (E_Location == MERIDIAN) text << "MERIDIONAL ";
 else text << "EQUATORIAL ";
 text << "plane of the Earth." << endl << ends;
 s.append(text.str());
 text.seekp(0);

 s.append("3. Noise Amplitude Law = ");
 //Получаем индекс размещения закона изменения амплитуды шума в таблице законов
 Select = GetIndexLawByType(TableNoiseLaw,TableNoiseLawCount,NoiseAmplLawType);
 text << TableNoiseLaw[Select].mathlawfactor(NoiseAmplArg) << endl << ends;
 s.append(text.str());
 text.seekp(0);

 s.append("4. Noise Phase Law = ");
 //Получаем индекс размещения закона изменения фазы шума в таблице законов
 Select = GetIndexLawByType(TableNoiseLaw,TableNoiseLawCount,NoisePhaseLawType);
 text << TableNoiseLaw[Select].mathlawfactor(NoisePhaseArg) << endl << ends;
 s.append(text.str());
 text.seekp(0);

 if (headfoot == ON)
 { text << "*************************************************************"
        << endl << ends;
   s.append(text.str());
 }
return s;
}
//*****************************************************************************
//СТАТУС: I.8; public member-function of class Antenna
//НАИМЕНОВАНИЕ ФУНКЦИИ:
// void SetPerformance(uint AntType, bool EPlLocate = MERIDIAN,
//                     ARGLIST* E_Arg = NULL,  ARGLIST* H_Arg = NULL)
//НАЗНАЧЕНИЕ:
//Функция устанавливает характеристики антенны.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. AntType - тип антенны
//2. EPlLocate - сонаправленность E-плоскости антенны с меридиональной или эква-
//ториальной плоскостью Земли (по умолчанию - EPlLocate = MERIDIAN)
//3. E_Arg - список параметров функции диаграммы направленности в E-плоскости
//4. H_Arg - список параметров функции диаграммы направленности в H-плоскости
//По умолчанию E_Arg и H_Arg равны NULL.
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: нет
//*****************************************************************************
void Antenna::SetPerformance(uint AntType, bool EPlLocate, ARGLIST* E_Arg,
                             ARGLIST* H_Arg)
{uint Index;
 try
 { //Определяем индекс размещения информации о типе антенны AntType в массиве
   //AntennInfo
   Index = GetIndexByAntType(AntType);
   //Информация об антенне типа AntType отсутствует, генерировать исключительную
   //ситуацию
   if (Index == 0) throw ANTENNA_ABSENT;

   //Инициализация характеристик антенны
   Type = AntType;  //Идентификатор типа антенны
   //Размещение E-плоскости относительно меридианальной или экваториальной
   //плоскостей Земли
   E_Location = EPlLocate;

   //Удаление параметров
   if (E_DirectArg != NULL) DeleteArgList(E_DirectArg);
   if (H_DirectArg != NULL) DeleteArgList(H_DirectArg);

   //Устанавливаем функцию диаграммы направленности антенны в E-плоскости
   uint IdEHDirect = GetIdEDirByIndex(Index);
   E_DirectArg = SetLaw(AntDirect, AntDirectCount, IdEHDirect, E_DirectType,
                        E_Direct, E_Arg);

   //Устанавливаем функцию диаграммы направленности антенны в H-плоскости
   IdEHDirect = GetIdHDirByIndex(Index);
   H_DirectArg = SetLaw(AntDirect, AntDirectCount, IdEHDirect, H_DirectType,
                        H_Direct, H_Arg);
 }
 catch (int error_type)
 {
   if (error_type == ANTENNA_ABSENT) //Информация об антенне отсутствует
   {
    #if defined(CONSOLE)
     cout << "\nANTENNA IS ABSENT IN AntennInfo TABLE !!!" << endl;
     cout << "Error in constructor with arguments Antenna" << endl;
     cout << "Can't find antenna with the specific identifier " << AntType
          << endl;
     getch();
    #endif
     exit(-1);
   }
 }
}
//*****************************************************************************
//СТАТУС: I.9; public member-function of class Antenna
//НАИМЕНОВАНИЕ ФУНКЦИИ: void SetNoisePerform(uint AmplLaw, uint PhaseLaw,
//                      ARGLIST* AmplArg = NULL,  ARGLIST* PhaseArg = NULL);
//НАЗНАЧЕНИЕ:
//Функция устанавливает случайные законы изменения амплитуды и фазы шума на вы-
//ходе приемной антенны.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. uint AmplLaw - идентификатор закона изменения амплитуды шума антенны
//2. PhaseLaw - идентификатор закона изменения фазы шума антенны
//3. AmplArg - параметры закона изменения амплитуды (если они требуются)
//4. PhaseArg - параметры закона изменения фазы (если они требуются
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: нет
//*****************************************************************************
void Antenna::SetNoisePerform(uint AmplLaw, uint PhaseLaw, ARGLIST* AmplArg,
              ARGLIST* PhaseArg)
{
  if (NoiseAmplArg != NULL) DeleteArgList(NoiseAmplArg);
  if (NoisePhaseArg != NULL) DeleteArgList(NoisePhaseArg);


  //Устанавливаем закон изменения амплитуды шума на выходе антенны
  NoiseAmplArg = SetLaw(TableNoiseLaw, TableNoiseLawCount, AmplLaw,
                 NoiseAmplLawType, NoiseAmpl, AmplArg);
  //Устанавливаем закон изменения фазы шума на выходе антенны
  NoisePhaseArg = SetLaw(TableNoiseLaw, TableNoiseLawCount, PhaseLaw,
                  NoisePhaseLawType, NoisePhase, PhaseArg);
}
//*****************************************************************************
//СТАТУС: I.10; public member-function of class Antenna
//НАИМЕНОВАНИЕ ФУНКЦИИ: complex CalcUNoise()
//НАЗНАЧЕНИЕ: Calculate instantaneous value of voltage noise on antenna output
//Функция вычисляет мгновенное значение уровня шума на выходе антенны по случай-
//ным законам изменения АМПЛИТУДЫ и ФАЗЫ ШУМА.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ: нет
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: complex U  - комплексное значение уровня шума
//*****************************************************************************
complex Antenna::CalcUNoise()
{
 return polar(GetAmplNoise(),GetPhaseNoise());
}
//*****************************************************************************
//СТАТУС: I.11; public member-function of class Antenna
//НАИМЕНОВАНИЕ ФУНКЦИИ: double CalcAntPatterns(bool EathPlane)
//НАЗНАЧЕНИЕ:
//Функция вычисляет характеристику направленности антенны в меридиональной (по
//углу места) или в экваториальной (по азимуту) плоскости в направлении на
//объект, расположенный под углом Angle относительно к нормали раскрыва антенны.
//Функция характеристики направленности антенны в меридиональной или экваториаль-
//ной плоскости Земли выбирается в зависимости от значения EarthPlane.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. bool EathPlane - плоскость Земли, определяющая выбор функции направленности
//   антенны. (MERIDIAN; EQUATOR))
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: double value - значение хар-ки направленности антенны
//*****************************************************************************
double Antenna::CalcAntPatterns(bool EathPlane)
{double value;
 //Выбираем функцию характеристики направленности антенны, соответствующую
 //EarthPlane
 //E-плоскость антенны совпадает с EarthPlane
 if (EathPlane == E_Location) value = E_Direct(E_DirectArg);
 else value = H_Direct(H_DirectArg);

 return value;
}
//*****************************************************************************
//СТАТУС: I.12; public member-function of class Antenna
//НАИМЕНОВАНИЕ ФУНКЦИИ:
//ARGLIST* AntPatterns(bool EathPlane, double InitAngle = -90.0,
//                     double EndAngle = 90.0, double Step = 0.1, bool Deg = DEG);
//НАЗНАЧЕНИЕ:
//Функция определяет диаграмму направленности антенны в меридиональной (по
//углу места) или в экваториальной (по азимуту) плоскости в секторе от InitAngle
//до EndAngle. Углы отсчитываются относительно нормали к раскрыву антенны.
//Функция характеристики направленности антенны в меридиональной или экваториаль-
//ной плоскости Земли выбирается в зависимости от значения EarthPlane.
//Все значения характеристики направленности антенны в заданном секторе заносят-
//ся в список ARGLIST. Структура хранимых данных в списке ARGLIST:
//List[0] - InitAngle; List[1] - EndAngle; List[2] - Step; List[3]...List[n] -
//данные.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. bool EathPlane - плоскость Земли, определяющая выбор функции направленности
//   антенны. (MERIDIAN; EQUATOR))
//2. double InitAngle (-90 deg) - начальный угол, построения диаграммы направлен-
//   ности антенны (antenna patterns)
//3. double EndAngle (+90 deg) - конечный угол, построения диаграммы направлен-
//   ности антенны.
//4. double Step - приращение угла
//5. bool Deg (DEG) - представление угла (в градусах - DEG или радианах - RAD)
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
// ARGLIST* data - диаграмма направленности антенны
//*****************************************************************************
ARGLIST* Antenna::AntPatterns(bool EathPlane, double InitAngle, double EndAngle,
                  double Step, bool Deg)
{ulong Count; //Количество данных, которое необходимо разместить в списке data

 if (EndAngle < InitAngle)
 { double Value = InitAngle;
   InitAngle = EndAngle;
   EndAngle = Value;
 }
 //Приращение угла может быть только положительным значением
 Step = fabs(Step);
 if (Step == 0.0) Step = (Deg == DEG) ? 0.1 : 0.1*DegRad;
 //Определяем количество данных, которые будут размещены в списке data
 Count = ceil((EndAngle-InitAngle)/Step)+1;
 //Создаем список data для хранения данных
 ARGLIST* data = CreateArgList(Count+3);
 //Заносим в список сектор определения диаграммы направленности антенны
 //в градусах
 if (Deg == RAD) //Перевод из радиан в градусы
 { InitAngle = InitAngle*RadDeg;
   EndAngle = EndAngle*RadDeg;
   Step = Step*RAD_DEG;
 }
 data->List[0] = InitAngle; data->List[1] = EndAngle; data->List[2] = Step;
 double Angle = InitAngle;
 //Определение диаграммы направленности антенны в заданном секторе обзора
 for (ulong index = 3; index < data->Size; index++)
 { SetAngle(Angle,Angle,Deg);
   data->List[index] = CalcAntPatterns(EathPlane);
   Angle = Angle+Step;
 }
return data;
}
//*****************************************************************************
//СТАТУС: I.13; public member-function of class Antenna
//НАИМЕНОВАНИЕ ФУНКЦИИ:
//  void SetAngle(double AzimuthAngle, double ElevateAngle, bool Deg = DEG)
//НАЗНАЧЕНИЕ:
//Функция устанавливает углы источника сигнала, соответствующие E-плоскости и
//H-плоскости антенны в зависимости от ее расположения в пространстве.
//Если E-плоскость антенны совпадает с меридиональной плоскостью Земли, то ей
//будет соответствовать ElevateAngle, а H-плоскости - AzimuthAngle.
//Если E-плоскость антенны совпадает с экваториальной плоскостью Земли, то ей
//будет соответствовать AzimuthAngle, а H-плоскости - ElevateAngle.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: нет
//*****************************************************************************
void Antenna::SetAngle(double AzimuthAngle, double ElevateAngle, bool Deg)
{
 if (Deg == DEG) //Углы заданы в градусах
 { AzimuthAngle = AzimuthAngle*DegRad;
   ElevateAngle = ElevateAngle*DegRad;
 }
 //E-плоскость антенны совпадает с меридианальной плоскостью Земли
 if (E_Location == MERIDIAN)
 { E_DirectArg->List[0] = ElevateAngle;
   H_DirectArg->List[0] = AzimuthAngle;
 }
 //E-плоскость антенны совпадает с экваториальной плоскостью Земли
 else
 { E_DirectArg->List[0] = AzimuthAngle;
   H_DirectArg->List[0] = ElevateAngle;
 }
}
//*****************************************************************************
//СТАТУС: I.14; public; Antenna class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ: void SetRandNumber();
//НАЗНАЧЕНИЕ:
//Функция получает текущие значения случайных амплитуд и фаз для вычисления
//характеристик шума на выходе приемной антенны.
//Случайные значения амплитуд (фаз) заносятся в список аргументов случайных
//законов под индексом равным 0. Данная функция позволяет исключить ситуацию
//двойного обращения к случайному закону, описывающему поведение фазы или ампли-
//туды шума, при определении мгновенных значений напряжений или мощностей
//шума в один и тот же момент времени t. Двойное обращение к случайным функциям
//привело бы к получению РАЗНЫХ случайных значений амплитуды (фазы), что
//является некорректным. Случайные характеристики шума приемной антенны:
//1. NoiseAmpl  - ARGLIST* NoiseAmplArg
//2. NoisePhase - ARGLIST* NoisePhaseArg
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ: нет
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: нет
//*****************************************************************************
void Antenna::SetRandNumber()
{
  NoiseAmplArg->List[0]  = NoiseAmpl(NoiseAmplArg);
  NoisePhaseArg->List[0] = NoisePhase(NoisePhaseArg);
}


//======================================================
//II. ОПИСАНИЕ ЗАКРЫТЫХ ЧЛЕНОВ-ФУНКЦИЙ КЛАССА ANTENNA
//    PRIVATE MEMBER-FUNCTION OF CLASS
//======================================================

//*****************************************************************************
//СТАТУС: II.1, private, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ: void CreateTableOfAntDirect()
//НАЗНАЧЕНИЕ:
//Создание таблицы функций, описывающих характеристики направленности антенны
//заданного типа в E- и H-плоскостях.
//Для работы функция использует глобальный указатель AntDirect и AntDirectCount
//СПИСОК ФУНКЦИЙ (по порядку, общее количество - 3):
//1. E_HWDipoleFunc; 2. H_HWDipoleFunc; 3. EH_ParabAntFunc;
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ: нет
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:  нет
//*****************************************************************************
void Antenna::CreateTableOfAntDirect()
{
  AntDirectCount = 3;
  AntDirect = new LAW[3];

 //***************************************************************************
 //Первая функция - характеристика направленности полуволнового вибратора в
 //E-плоскости (идеальной полуволновой щели в H-плоскости)
 //Параметры функции отсутствуют. Аргумент - угол (в радианах)
 //***************************************************************************
 AntDirect[0].turn = ON;
 AntDirect[0].law = E_HWDipoleFunc;
 AntDirect[0].type = FE_HWDIPOLE;
 AntDirect[0].inputfactor = InitFactors;
 AntDirect[0].mathlawfactor = MathNotateE_HWDipole;
 AntDirect[0].M.DimFunc = NONDIM;
 AntDirect[0].M.Size = 1;
 AntDirect[0].M.Factor = new FACTOR[1];

 //Angle - Meridional or Equatorial
 AntDirect[0].M.Factor[0].SourceDim = DEGREE;
 AntDirect[0].M.Factor[0].TargetDim = RADIAN;
 AntDirect[0].M.Factor[0].Name = new char[6];
 strcpy(AntDirect[0].M.Factor[0].Name,"Angle");
 AntDirect[0].M.Factor[0].Convert = DegToRad;
 AntDirect[0].M.Factor[0].Invert = RadToDeg;
 AntDirect[0].M.Factor[0].PreBlock = AlwaysTrue;
 AntDirect[0].M.Factor[0].PostBlock = AlwaysTrue;

 //***************************************************************************
 //Вторая функция - характеристика направленности полуволнового вибратора в
 //H-плоскости (идеальной полуволновой щели в E-плоскости)
 //Параметры функции отсутствуют. Аргумент - угол (в радианах)
 //***************************************************************************
 AntDirect[1].turn = ON;
 AntDirect[1].law = H_HWDipoleFunc;
 AntDirect[1].type = FH_HWDIPOLE;
 AntDirect[1].inputfactor = InitFactors;
 AntDirect[1].mathlawfactor = MathNotateH_HWDipole;
 AntDirect[1].M.DimFunc = NONDIM;
 AntDirect[1].M.Size = 1;
 AntDirect[1].M.Factor = new FACTOR[1];

 //Angle - Meridional or Equatorial
 AntDirect[1].M.Factor[0].SourceDim = DEGREE;
 AntDirect[1].M.Factor[0].TargetDim = RADIAN;
 AntDirect[1].M.Factor[0].Name = new char[6];
 strcpy(AntDirect[1].M.Factor[0].Name,"Angle");
 AntDirect[1].M.Factor[0].Convert = DegToRad;
 AntDirect[1].M.Factor[0].Invert = RadToDeg;
 AntDirect[1].M.Factor[0].PreBlock = AlwaysTrue;
 AntDirect[1].M.Factor[0].PostBlock = AlwaysTrue;

 //***************************************************************************
 //Третья функция - характеристика направленности параболической антенны в
 //E- и в H-плоскостях
 //Аргумент - угол (в радианах). Параметры функции: 1)La - относительный раскрыв
 //параболической антенны (D/l, l-длина волны, D-диаметр параболического зеркала
 //в E- или H-плоскости).
 //***************************************************************************
 AntDirect[2].turn = ON;
 AntDirect[2].law = EH_ParabAntFunc;
 AntDirect[2].type = FEH_PARABANT;
 AntDirect[2].inputfactor = InitFactors;
 AntDirect[2].mathlawfactor = MathNotateEH_ParabAnt;
 AntDirect[2].M.DimFunc = NONDIM;
 AntDirect[2].M.Size = 2;
 AntDirect[2].M.Factor = new FACTOR[2];

 //Angle - Meridional or Equatorial
 AntDirect[2].M.Factor[0].SourceDim = DEGREE;
 AntDirect[2].M.Factor[0].TargetDim = RADIAN;
 AntDirect[2].M.Factor[0].Name = new char[6];
 strcpy(AntDirect[2].M.Factor[0].Name,"Angle");
 AntDirect[2].M.Factor[0].Convert = DegToRad;
 AntDirect[2].M.Factor[0].Invert = RadToDeg;
 AntDirect[2].M.Factor[0].PreBlock = AlwaysTrue;
 AntDirect[2].M.Factor[0].PostBlock = AlwaysTrue;

 //La - Relative aerial aperture
 AntDirect[2].M.Factor[1].SourceDim = NONDIM;
 AntDirect[2].M.Factor[1].TargetDim = NONDIM;
 AntDirect[2].M.Factor[1].Name = new char[18];
 strcpy(AntDirect[2].M.Factor[1].Name,"Relative Aperture");
 AntDirect[2].M.Factor[1].Convert = Copy;
 AntDirect[2].M.Factor[1].Invert = Copy;
 AntDirect[2].M.Factor[1].PreBlock = AlwaysTrue;
 AntDirect[2].M.Factor[1].PostBlock = AlwaysTrue;
}
//*****************************************************************************
//СТАТУС: II.2, private, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ: void CreateTableOfNoiseLaw()
//НАЗНАЧЕНИЕ:
//Создание таблицы случайных законов изменения во времени амплитуды и фазы шума
//на выходе антенны.
//Для работы функция использует глобальный указатель TableNoiseLaw и
//TableNoiseLawCount
//СПИСОК ФУНКЦИЙ (по порядку, общее количество - 6):
//1. RandLaw; 2. Rand2PiLaw; 3. RandABLaw; 4. RelayLaw; 5. Det1Law; 6. Det0Law
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ: нет
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:  нет
//*****************************************************************************
void Antenna::CreateTableOfNoiseLaw()
{
 TableNoiseLawCount = 6;
 TableNoiseLaw = new LAW[6];

 //********************************************************
 //Первый закон - Равновероятный закон в интервале [0..1]
 //Параметры функции и аргумент отсутствуют.
 //********************************************************
 // {RandLaw, RAND_LAW, InitFactors, GetMathNotateRandLaw, NONDIM, 0}
 TableNoiseLaw[0].turn = ON;
 TableNoiseLaw[0].law = RandLaw;
 TableNoiseLaw[0].type = RAND_LAW;
 TableNoiseLaw[0].inputfactor = InitFactors;
 TableNoiseLaw[0].mathlawfactor = GetMathNotateRandLaw;
 TableNoiseLaw[0].M.DimFunc = NONDIM;
 TableNoiseLaw[0].M.Size = 1;
 TableNoiseLaw[0].M.Factor = new FACTOR[1];

 //RandValue
 TableNoiseLaw[0].M.Factor[0].SourceDim = NONDIM;
 TableNoiseLaw[0].M.Factor[0].TargetDim = NONDIM;
 TableNoiseLaw[0].M.Factor[0].Name = new char[5];
 strcpy(TableNoiseLaw[0].M.Factor[0].Name,"Rand");
 TableNoiseLaw[0].M.Factor[0].Convert = Copy;
 TableNoiseLaw[0].M.Factor[0].Invert = Copy;
 TableNoiseLaw[0].M.Factor[0].PreBlock = AlwaysTrue;
 TableNoiseLaw[0].M.Factor[0].PostBlock = AlwaysTrue;

 //*************************************************************
 //Второй закон - Равновероятный закон в интервале [0..2PI]
 //*************************************************************
 //{Ran2PiLaw,RAND2PI_LAW, InitFactors, GetMathNotateRand2PiLaw, NONDIM, 0}
 TableNoiseLaw[1].turn = ON;
 TableNoiseLaw[1].law = Rand2PiLaw;
 TableNoiseLaw[1].type = RAND2PI_LAW;
 TableNoiseLaw[1].inputfactor = InitFactors;
 TableNoiseLaw[1].mathlawfactor = GetMathNotateRand2PiLaw;
 TableNoiseLaw[1].M.DimFunc = NONDIM;
 TableNoiseLaw[1].M.Size = 1;
 TableNoiseLaw[1].M.Factor = new FACTOR[1];

 //RandValue
 TableNoiseLaw[1].M.Factor[0].SourceDim = NONDIM;
 TableNoiseLaw[1].M.Factor[0].TargetDim = NONDIM;
 TableNoiseLaw[1].M.Factor[0].Name = new char[5];
 strcpy(TableNoiseLaw[1].M.Factor[0].Name,"Rand");
 TableNoiseLaw[1].M.Factor[0].Convert = Copy;
 TableNoiseLaw[1].M.Factor[0].Invert = Copy;
 TableNoiseLaw[1].M.Factor[0].PreBlock = AlwaysTrue;
 TableNoiseLaw[1].M.Factor[0].PostBlock = AlwaysTrue;

 //*************************************************************
 //Третий закон - Равновероятный закон в интервале [A..B]
 //*************************************************************
 //{RanABLaw,RANDAB_LAW, InitFactors, GetMathNotateRandABLaw, NONDIM, 2}
 TableNoiseLaw[2].turn = ON;
 TableNoiseLaw[2].law = RandABLaw;
 TableNoiseLaw[2].type = RANDAB_LAW;
 TableNoiseLaw[2].inputfactor = InitFactors;
 TableNoiseLaw[2].mathlawfactor = GetMathNotateRandABLaw;
 TableNoiseLaw[2].M.DimFunc = NONDIM;
 TableNoiseLaw[2].M.Size = 3;
 TableNoiseLaw[2].M.Factor = new FACTOR[3];

 //RandValue
 TableNoiseLaw[2].M.Factor[0].SourceDim = NONDIM;
 TableNoiseLaw[2].M.Factor[0].TargetDim = NONDIM;
 TableNoiseLaw[2].M.Factor[0].Name = new char[5];
 strcpy(TableNoiseLaw[2].M.Factor[0].Name,"Rand");
 TableNoiseLaw[2].M.Factor[0].Convert = Copy;
 TableNoiseLaw[2].M.Factor[0].Invert = Copy;
 TableNoiseLaw[2].M.Factor[0].PreBlock = AlwaysTrue;
 TableNoiseLaw[2].M.Factor[0].PostBlock = AlwaysTrue;

 //A  {NONDIM,NONDIM,"Lower border A",Copy,Copy,AlwaysTrue,AlwaysTrue}
 TableNoiseLaw[2].M.Factor[1].SourceDim = NONDIM;
 TableNoiseLaw[2].M.Factor[1].TargetDim = NONDIM;
 TableNoiseLaw[2].M.Factor[1].Name = new char[15];
 strcpy(TableNoiseLaw[2].M.Factor[1].Name,"Lower border A");
 TableNoiseLaw[2].M.Factor[1].Convert = Copy;
 TableNoiseLaw[2].M.Factor[1].Invert = Copy;
 TableNoiseLaw[2].M.Factor[1].PreBlock = AlwaysTrue;
 TableNoiseLaw[2].M.Factor[1].PostBlock = AlwaysTrue;

 //B  {NONDIM,NONDIM,"Upper border B",Copy,Copy,AlwaysTrue,AlwaysTrue}
 TableNoiseLaw[2].M.Factor[2].SourceDim = NONDIM;
 TableNoiseLaw[2].M.Factor[2].TargetDim = NONDIM;
 TableNoiseLaw[2].M.Factor[2].Name = new char[15];
 strcpy(TableNoiseLaw[2].M.Factor[2].Name,"Upper border B");
 TableNoiseLaw[2].M.Factor[2].Convert = Copy;
 TableNoiseLaw[2].M.Factor[2].Invert = Copy;
 TableNoiseLaw[2].M.Factor[2].PreBlock = AlwaysTrue;
 TableNoiseLaw[2].M.Factor[2].PostBlock = AlwaysTrue;

 //*************************************************************
 //Четвертый закон - Случайный закон Релея [0..+INFINITE]
 //*************************************************************
 //{RelayLaw,RELAY_LAW, InitFactors, GetMathNotateRandLaw, NONDIM, 0}
 TableNoiseLaw[3].turn = ON;
 TableNoiseLaw[3].law = RelayLaw;
 TableNoiseLaw[3].type = RELAY_LAW;
 TableNoiseLaw[3].inputfactor = InitFactors;
 TableNoiseLaw[3].mathlawfactor = GetMathNotateRelayLaw;
 TableNoiseLaw[3].M.DimFunc = NONDIM;
 TableNoiseLaw[3].M.Size = 1;
 TableNoiseLaw[3].M.Factor = new FACTOR[1];

 //RandValue
 TableNoiseLaw[3].M.Factor[0].SourceDim = NONDIM;
 TableNoiseLaw[3].M.Factor[0].TargetDim = NONDIM;
 TableNoiseLaw[3].M.Factor[0].Name = new char[5];
 strcpy(TableNoiseLaw[3].M.Factor[0].Name,"Rand");
 TableNoiseLaw[3].M.Factor[0].Convert = Copy;
 TableNoiseLaw[3].M.Factor[0].Invert = Copy;
 TableNoiseLaw[3].M.Factor[0].PreBlock = AlwaysTrue;
 TableNoiseLaw[3].M.Factor[0].PostBlock = AlwaysTrue;

 //*************************************************************
 //Пятый закон - закон исключения СВ из произведения
 //*************************************************************
 //{Det1Law, DET1_LAW, InitFactors, GetMathNotateDet1Law, NONDIM, 0}
 TableNoiseLaw[4].turn = ON;
 TableNoiseLaw[4].law = Det1Law;
 TableNoiseLaw[4].type = DET1_LAW;
 TableNoiseLaw[4].inputfactor = InitFactors;
 TableNoiseLaw[4].mathlawfactor = GetMathNotateDet1Law;
 TableNoiseLaw[4].M.DimFunc = NONDIM;
 TableNoiseLaw[4].M.Size = 1;
 TableNoiseLaw[4].M.Factor = new FACTOR[1];

 //RandValue
 TableNoiseLaw[4].M.Factor[0].SourceDim = NONDIM;
 TableNoiseLaw[4].M.Factor[0].TargetDim = NONDIM;
 TableNoiseLaw[4].M.Factor[0].Name = new char[5];
 strcpy(TableNoiseLaw[4].M.Factor[0].Name,"Rand");
 TableNoiseLaw[4].M.Factor[0].Convert = Copy;
 TableNoiseLaw[4].M.Factor[0].Invert = Copy;
 TableNoiseLaw[4].M.Factor[0].PreBlock = AlwaysTrue;
 TableNoiseLaw[4].M.Factor[0].PostBlock = AlwaysTrue;

 //*************************************************************
 //Шестой закон - закон исключения СВ из суммы
 //*************************************************************
 //{Det0Law, DET0_LAW, InitFactors, GetMathNotateDet0Law, NONDIM, 0}
 TableNoiseLaw[5].turn = ON;
 TableNoiseLaw[5].law = Det0Law;
 TableNoiseLaw[5].type = DET0_LAW;
 TableNoiseLaw[5].inputfactor = InitFactors;
 TableNoiseLaw[5].mathlawfactor = GetMathNotateDet0Law;
 TableNoiseLaw[5].M.DimFunc = NONDIM;
 TableNoiseLaw[5].M.Size = 1;
 TableNoiseLaw[5].M.Factor = new FACTOR[1];

 //RandValue
 TableNoiseLaw[5].M.Factor[0].SourceDim = NONDIM;
 TableNoiseLaw[5].M.Factor[0].TargetDim = NONDIM;
 TableNoiseLaw[5].M.Factor[0].Name = new char[5];
 strcpy(TableNoiseLaw[5].M.Factor[0].Name,"Rand");
 TableNoiseLaw[5].M.Factor[0].Convert = Copy;
 TableNoiseLaw[5].M.Factor[0].Invert = Copy;
 TableNoiseLaw[5].M.Factor[0].PreBlock = AlwaysTrue;
 TableNoiseLaw[5].M.Factor[0].PostBlock = AlwaysTrue;

}
//*****************************************************************************
//СТАТУС: II.3., private, class member-function
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
void Antenna::DeleteTable(LAW* Table, uint& Size)
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

//======================================================
//III. ОПИСАНИЕ ЗАЩИЩЕННЫХ ЧЛЕНОВ-ФУНКЦИЙ КЛАССА ANTENNA
//     PROTECTED MEMBER-FUNCTION OF CLASS
//======================================================

//*****************************************************************************
//СТАТУС: III.1., protected, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ:
// uint GetIndexLawByType(const LAW* Table, uint Count, uint Type)
//НАЗНАЧЕНИЕ:
//Функция возвращает индекс расположения закона в таблице законов по его типу.
//Функция используется при отображении закона изменения шума на выходе антенны.
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
uint Antenna::GetIndexLawByType(const LAW* Table, const uint Count, uint Type)
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
//СТАТУС: III.2., protected, class member-function
//НАИМЕНОВАНИЕ ФУНКЦИИ:
//ARGLIST* SetLaw(const LAW* Table, const uint Count, const uint SLawType,
//                uint& TLawType, LAWFUNC& RefFunc, ARGLIST* SArg = NULL);
//НАЗНАЧЕНИЕ:
//Функция устанавливает закон изменения величины заданного типа SLawType c за-
//данными параметрами SArg.
//Если закон такого типа не содержится в таблице описания законов Table, то
//программа завершает свою работу.
//Если список параметров SArg не соответствует требуемому числу параметров для
//заданного закона, то программа также завершает свою работу.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. const LAW* Table - таблица законов
//2. const uint Count - количество законов в таблице
//3. const uint SLawType - идентификатор закона, который будет установлен
//4. uint& TLawType - идентификатор закона, заменяемого на SLawType
//5. LAWFUNC& RefFunc - функция, описывающая закон по которому
//   будет изменяться величина
//6. const ARGLIST* SArg - список параметров, устанавливаемого закона
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
//ARGLIST* Arg - указатель на список параметров, который будет соответствовать
//устанавливаемому закону
//*****************************************************************************
ARGLIST* Antenna::SetLaw(const LAW* Table, const uint Count, const uint SLawType,
                         uint& TLawType, LAWFUNC& RefFunc, ARGLIST* SArg)
{uint TLen, SLen;
 uint Index;
 ARGLIST* Arg = NULL;
 //Определяем расположение устанавливаемого закона в таблице законов
 Index = GetIndexLawByType(Table, Count, SLawType);

 //Определяем размер списка аргументов устанавливаемого закона и сравниваем его
 //с требуемым размером
 SLen = (SArg == NULL) ? 0 : SArg->Size;
 TLen = Table[Index].M.Size;
 //Функция не имеет параметров, кроме аргумента
 if ((TLen == 1) && (SLen == 0)) {SLen = 1; SArg = CreateArgList(SLen);}
 try
 { //Размеры передаваемого SLen и требуемого TLen списка аргументов совпадают
   if (SLen == TLen) Arg = CopyArgList(SArg, Arg);
   //Несоответствие исходного и требуемого количества параметров для закона
   //с идентификатором SLawType
   else throw ARG_MISMATCH;

   TLawType = SLawType;
   RefFunc = Table[Index].law;
 }
 catch (int error_type)
 {
  if (error_type == ARG_MISMATCH)
  {
   #if defined(CONSOLE)
    cout << "\nMISMATCH ARGUMENT ERROR !!!" << endl;
    cout << "Error in function Antenna::SetLaw" << endl;
    cout << "Discordance of initial and required parameters amount for the law\n"
         << "with the identifier SLawType" << endl;
    cout << "SourceArgSize = " << SLen << endl;
    cout << "TargetArgSize = " << TLen << endl;
    getch();
   #endif
    exit(-1);
  }
 }
return Arg;
}

//*************************************************************************
// IV. ФУНКЦИИ, ОПИСЫВАЮЩИЕ ХАРАКТЕРИСТИКИ НАПРАВЛЕННОСТИ АНТЕННЫ
//     ЗАДАННОГО ТИПА В E- И H-плоскостях
//*************************************************************************

//*****************************************************************************
//СТАТУС: IV.1
//НАИМЕНОВАНИЕ ФУНКЦИИ: double E_HWDipoleFunc(const ARGLIST* Arg)
//НАЗНАЧЕНИЕ: F(x)=|cos(sin(x)*PI/2)/cos(x)|
//Нормированная характеристика направленности линейного полуволнового вибратора
//(полуволновой щели в H-плоскости) в электрической плоскости E.
//Для работы функции неообходимо ей передать один параметр через список Arg.
//СТРУКТУРА СПИСКА Arg ДЛЯ НОРМАЛЬНОЙ РАБОТЫ E_HWDipoleFunc()
//Количество параметров: 1
//1. Arg.List[0] - экваториальный (азимутальный) или меридиональный (угол места)
//угол между нормалью к раскрыву антенны и направлением на источник сигнала.
//Угол задается в радианах.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. struct ARGLIST* Arg - список параметров передаваемый в E_HWDipoleFunc
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: double value
//*****************************************************************************
double E_HWDipoleFunc(const ARGLIST* Arg)
{double value;
 try
  {
   if (Arg == NULL) //Отсутствуют параметры
    throw ARG_MISMATCH;
   if (Arg->Size != 1) //Должен быть всего один параметр
    throw ARG_MISMATCH;
  //Вычисление характеристики направленности антенны
  value = cos(Arg->List[0]);
  if (fabs(value) <= 1.0e-6) return 0.0; //Angle t = PI/2
  else value = fabs(cos(sin(Arg->List[0])*M_PI_2)/value);
  }
 catch (int error_type)
  {
   if (error_type == ARG_MISMATCH)
   {
    #if defined(CONSOLE)
      cout << "\nMISMATCH ARGUMENT ERROR !!!" << endl;
      cout << "Function ANTENNA::E_HWDipoleFunc() should accept one parameter: "
           << "angle [RADIAN] .\n";
      cout << "Mathematical notation: F(t) = |cos(sin(x)*PI/2)/cos(x)|,"
           << "t - angle." << endl;
      getch();
    #endif
    exit(-1);
   }
  }
return value;
}
//*****************************************************************************
//СТАТУС: IV.2
//НАИМЕНОВАНИЕ ФУНКЦИИ: double H_HWDipoleFunc(const ARGLIST* Arg)
//НАЗНАЧЕНИЕ: F(x)=1
//Нормированная характеристика направленности линейного полуволнового вибратора
//(полуволновой щели в E-плоскости) в магнитной H-плоскости.
//Для работы функции неообходимо ей передать один параметр через список Arg.
//СТРУКТУРА СПИСКА Arg ДЛЯ НОРМАЛЬНОЙ РАБОТЫ H_HWDipoleFunc()
//Количество параметров: 1
//1. Arg.List[0] - экваториальный (азимутальный) или меридиональный (угол места)
//угол между нормалью к раскрыву антенны и направлением на источник сигнала.
//Угол задается в радианах.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. struct ARGLIST* Arg - список параметров передаваемый в E_HWDipoleFunc
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: double Arg.List[1]
//*****************************************************************************
double H_HWDipoleFunc(const ARGLIST* Arg)
{
 try
  {
   if (Arg == NULL) //Отсутствуют параметры
    throw ARG_MISMATCH;
   if (Arg->Size != 1) //Должен быть всего один параметр
    throw ARG_MISMATCH;
  }
 catch (int error_type)
  {
   if (error_type == ARG_MISMATCH)
   {
    #if defined(CONSOLE)
      cout << "\nMISMATCH ARGUMENT ERROR !!!" << endl;
      cout << "Function ANTENNA::H_HWDipoleFunc() should accept one parameter: "
           << "angle [RADIAN] .\n";
      cout << "Mathematical notation: F(t) = 1, t - angle." << endl;
      getch();
    #endif
    exit(-1);
   }
  }
return 1.0;
}
//*****************************************************************************
//СТАТУС: IV.3
//НАИМЕНОВАНИЕ ФУНКЦИИ: double EH_ParabAntFunc(const ARGLIST* Arg)
//НАЗНАЧЕНИЕ: F(x)=|sin(2*PI*La*sin(x))/2*PI*La*sin(x)|
//Нормированная характеристика направленности параболической антенны в E- и
//H-плоскости.
//Для работы функции неообходимо ей передать два параметра через список Arg.
//СТРУКТУРА СПИСКА Arg ДЛЯ НОРМАЛЬНОЙ РАБОТЫ E_HWDipoleFunc()
//Количество параметров: 2
//1. Arg.List[0] - экваториальный (азимутальный) или меридиональный (угол места)
//угол между нормалью к раскрыву антенны и направлением на источник сигнала.
//Угол задается в радианах.
//2. Arg.List[1] - относительный раскрыв параболической антенны: отношение диа-
//метра параболической антенны к длине волны. Безразмерная величина.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. struct ARGLIST* Arg - список параметров передаваемый в EH_ParabAntFunc
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: double value
//*****************************************************************************
double EH_ParabAntFunc(const ARGLIST* Arg)
{double value;
 try
  {
   if (Arg == NULL) //Отсутствуют параметры
    throw ARG_MISMATCH;
   if (Arg->Size != 2) //Должно быть всего два параметра
    throw ARG_MISMATCH;
   //Вычисление характеристики направленности параболической антенны
   value = sin(Arg->List[0]);
   double La = Arg->List[1]; //Относительный раскрыв параболической антенны
   double t;
   if (fabs(value) <= 1.0e-6) return 1.0; //Angle t = 0.0
   else { t = La*value*TWO_PI; value = fabs(sin(t)/t); }
  }
 catch (int error_type)
 {
  if (error_type == ARG_MISMATCH)
  {
   #if defined(CONSOLE)
    cout << "\nMISMATCH ARGUMENT ERROR !!!" << endl;
    cout << "Function ANTENNA::EH_ParabAntFunc() should accept two parameters:\n"
         << "1. Angle [RAD]; 2. La - Relative aerial aperture.\n";
    cout << "Mathematical notation: F(t) = |sin(2*PI*La*sin(t))/2*PI*La*sin(t)|,"
         << "t - angle." << endl;
    getch();
   #endif
   exit(-1);
  }
 }
return value;
}

//*****************************************************************************
// V. ФУНКЦИИ, ФОРМИРУЮЩИЕ МАТЕМАТИЧЕСКУЮ ЗАПИСЬ ФУНКЦИИ ХАРАКТЕРИСТИКИ
//    НАПРАВЛЕННОСТИ АНТЕННЫ С ЗАДАННЫМИ ПАРАМЕТРАМИ
// Все функции в результате работы возвращают указатель на строку с математичес-
// кой записью функции характеристики направленности с реальными коэффициентами.
// Функции принимают в качестве аргумента список параметров функции характерис-
// тики направленности антенны заданного типа.
//*****************************************************************************

//*****************************************************************************
//СТАТУС: V.1
//НАИМЕНОВАНИЕ ФУНКЦИИ: char* MathNotateE_HWDipole(const ARGLIST* Arg)
//НАЗНАЧЕНИЕ:
//Функция формирует математическую запись функции характеристики направленности
//полуволнового вибратора в E-плоскости (полуволновой щели в H-плоскости) в виде
//строки.
//СТРУКТУРА CПИСКА Arg ДЛЯ НОРМАЛЬНОЙ РАБОТЫ ФУНКЦИИ
//Количество параметров: 1
//1. Arg.List[0] - экваториальный (азимутальный) или меридиональный (угол места)
//угол между нормалью к раскрыву антенны и направлением на источник сигнала.
//Угол задается в радианах.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. ARGLIST *Arg - список параметров закона изменения E_HWDipole
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
// char *s - закон изменения E_HWDipole в виде символьной строки
//*****************************************************************************
char* MathNotateE_HWDipole(const ARGLIST* Arg)
{
 char* s = new char[80];//Строка с математической записью закона
 ostrstream text(s, 80);
 try
  {
  	if (Arg == NULL)  //Аргументы не переданы
    throw ARG_MISMATCH;
   if (Arg->Size != 1) //Должeн быть передан только один параметр
    throw ARG_MISMATCH;
   text << "F(t) = |cos(sin(t)*PI/2)/cos(t)|"  << ends;
  }
 catch (int error_type)
  {
   if (error_type == ARG_MISMATCH)
   {
    #if defined(CONSOLE)
      cout << "\nMISMATCH ARGUMENT ERROR !!!" << endl;
      cout << "ANTENNA::MathNotateE_HWDipole() should accept one parameter: "
           << "t - angle.\n";
      cout << "Math. notation: F(t) = |cos(sin(t)*PI/2)/cos(t)|."
           << endl;
      getch();
    #endif
    exit(-1);
   }
  }
return s;
}
//*****************************************************************************
//СТАТУС: V.2
//НАИМЕНОВАНИЕ ФУНКЦИИ: char* MathNotateH_HWDipole(const ARGLIST* Arg)
//НАЗНАЧЕНИЕ:
//Функция формирует математическую запись функции характеристики направленности
//полуволнового вибратора в H-плоскости (полуволновой щели в E-плоскости) в виде
//строки.
//СТРУКТУРА CПИСКА Arg ДЛЯ НОРМАЛЬНОЙ РАБОТЫ ФУНКЦИИ
//Количество параметров: 1
//1. Arg.List[0] - экваториальный (азимутальный) или меридиональный (угол места)
//угол между нормалью к раскрыву антенны и направлением на источник сигнала.
//Угол задается в радианах.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. ARGLIST *Arg - список параметров закона изменения H_HWDipole
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
// char *s - закон изменения H_HWDipole в виде символьной строки
//*****************************************************************************
char* MathNotateH_HWDipole(const ARGLIST* Arg)
{
 char* s = new char[80];//Строка с математической записью закона
 ostrstream text(s, 80);
 try
  {
  	if (Arg == NULL)  //Аргументы не переданы
    throw ARG_MISMATCH;
   if (Arg->Size != 1) //Должeн быть передан только один параметр
    throw ARG_MISMATCH;
   text << "F(t) = 1, 0 <= t <= 360, t - angle."  << ends;
  }
 catch (int error_type)
  {
   if (error_type == ARG_MISMATCH)
   {
    #if defined(CONSOLE)
      cout << "\nMISMATCH ARGUMENT ERROR !!!" << endl;
      cout << "ANTENNA::MathNotateH_HWDipole() should accept one parameter: "
           << "t - angle.\n";
      cout << "Math. notation: F(t) = 1."
           << endl;
      getch();
    #endif
    exit(-1);
   }
  }
return s;
}
//*****************************************************************************
//СТАТУС: V.3
//НАИМЕНОВАНИЕ ФУНКЦИИ: char* MathNotateEH_ParabAnt(const ARGLIST* Arg)
//НАЗНАЧЕНИЕ:
//Функция формирует математическую запись функции характеристики направленности
//параболической антенны в виде строки.
//СТРУКТУРА CПИСКА Arg ДЛЯ НОРМАЛЬНОЙ РАБОТЫ ФУНКЦИИ
//Количество параметров: 2
//1. Arg.List[0] - экваториальный (азимутальный) или меридиональный (угол места)
//угол между нормалью к раскрыву антенны и направлением на источник сигнала.
//Угол задается в радианах.
//2. Arg.List[1] - относительный раскрыв параболической антенны: отношение диа-
//метра параболической антенны к длине волны. Безразмерная величина.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. ARGLIST *Arg - список параметров закона изменения EH_ParabAnt
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
// char *s - закон изменения EH_ParabAnt в виде символьной строки
//*****************************************************************************
char* MathNotateEH_ParabAnt(const ARGLIST* Arg)
{
 char* s = new char[80];//Строка с математической записью закона
 ostrstream text(s, 80);
 try
  {
  	if (Arg == NULL)  //Аргументы не переданы
    throw ARG_MISMATCH;
   if (Arg->Size != 2) //Должно быть передано точно два параметра
    throw ARG_MISMATCH;
   text << "F(t) = |sin(2*PI*" << Arg->List[1] << "*sin(t))/2*PI*"
        << Arg->List[1] << "*sin(t)|" << ends;
  }
 catch (int error_type)
  {
   if (error_type == ARG_MISMATCH)
   {
    #if defined(CONSOLE)
      cout << "\nMISMATCH ARGUMENT ERROR !!!" << endl;
      cout << "ANTENNA::MathNotateEH_ParabAnt() should accept two parameters: "
           << "\nt - angle, La - relative aerial aperture.\n";
      cout << "Math. notation: F(t) = |sin(2*PI*La*sin(t))/2*PI*La*sin(t)|."
           << endl;
      getch();
    #endif
    exit(-1);
   }
  }
return s;
}

//******************************************************************
// VI. ОПИСАНИЕ ФУНКЦИЙ, РАБОТАЮЩИХ С МАССИВОМ AntennInfo
//******************************************************************

//*****************************************************************************
//СТАТУС: VI.1
//НАИМЕНОВАНИЕ ФУНКЦИИ: uint GetIdEHDirByAntType(uint AntType, bool AntPlace)
//НАЗНАЧЕНИЕ:
//Функция возвращает  идентификатор функции направленности антенны в E- или H-
//плоскости в зависимости от значения AntPlace по типу антенны AntType из масси-
//ва AntennInfo. Поиск ведется по полю type. Если идентификатор типа антенны
//не найден в AntennInfo, то возвращается EHtype = UNEXIST.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. uint AntType - идентификатор типа антенны
//2. bool AntPlace - флаг определяющий, какой идентификатор функции направленнос-
//ти (в E- или H-плоскости) возвращать.
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
//uint type - идентификатор направленности антенны в E- или H-плоскости
//*****************************************************************************
uint GetIdEHDirByAntType(uint AntType, bool AntPlace)
{ uint type = UNEXIST;
  uint Count = sizeof(AntennInfo)/sizeof(AntennInfo[0]);
  //Поиск идентификатора направленности антенны в E- или H-плоскости по ее типу
  for (uint i = 1; i < Count; i++)
  {
   if (AntennInfo[i].type == AntType)
   { if (AntPlace == E_PLANE) type = AntennInfo[i].EDirect;
     else type = AntennInfo[i].HDirect;
     break; //Выход из цикла поиска
   }
  }
return type;
}
//*****************************************************************************
//СТАТУС: VI.2
//НАИМЕНОВАНИЕ ФУНКЦИИ: uint GetIndexByAntType(uint AntType)
//НАЗНАЧЕНИЕ:
//Функция возвращает индекс размещения информации об антенне в AntennInfo по ее
//типу. Поиск ведется по полю type. Если тип антенны не найден в AntennInfo, то
//возвращается Index = 0.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. uint AntType - идентификатор типа антенны
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
//uint index - индекс размещения информации об антенне в AntennInfo.
//*****************************************************************************
uint GetIndexByAntType(uint AntType)
{ uint index = 0;
  uint Count = sizeof(AntennInfo)/sizeof(AntennInfo[0]);
  //Поиск идентификатора направленности антенны в E- или H-плоскости по ее типу
  for (uint i = 1; i < Count; i++)
  {
   if (AntennInfo[i].type == AntType)
   { index = i;
     break; //Выход из цикла поиска
   }
  }
return index;
}
//*****************************************************************************
//СТАТУС: VI.3
//НАИМЕНОВАНИЕ ФУНКЦИИ: uint GetIdEDirByIndex(uint Index)
//НАЗНАЧЕНИЕ:
//Функция возвращает идентификатор функции направленности антенны в E-плоскости
//по ее индексу расположения в массиве AntennInfo.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. uint Index - индекс размещения информации об антенне в массиве AntennInfo
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
//uint IdEDirect - идентификатор функции направленности антенны в E-плоскости
//*****************************************************************************
uint GetIdEDirByIndex(uint Index)
{ uint IdEDirect = UNEXIST;
  uint Count = sizeof(AntennInfo)/sizeof(AntennInfo[0]);

  if (Index < Count) IdEDirect = AntennInfo[Index].EDirect;

 return IdEDirect;
}
//*****************************************************************************
//СТАТУС: VI.4
//НАИМЕНОВАНИЕ ФУНКЦИИ: uint GetIdEDirByIndex(uint Index)
//НАЗНАЧЕНИЕ:
//Функция возвращает идентификатор функции направленности антенны в H-плоскости
//по ее индексу расположения в массиве AntennInfo.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. uint Index - индекс размещения информации об антенне в массиве AntennInfo
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
//uint IdHDirect - идентификатор функции направленности антенны в H-плоскости
//*****************************************************************************
uint GetIdHDirByIndex(uint Index)
{ uint IdHDirect = UNEXIST;
  uint Count = sizeof(AntennInfo)/sizeof(AntennInfo[0]);

  if (Index < Count) IdHDirect = AntennInfo[Index].HDirect;

 return IdHDirect;
}
//*****************************************************************************
//СТАТУС: VI.5
//НАИМЕНОВАНИЕ ФУНКЦИИ: char* GetAntNameByIndex(uint Index)
//НАЗНАЧЕНИЕ:
//Функция возвращает наименование типа антенны по ее индексу расположения
//в массиве AntennInfo.
//-----------------------------------------------------------------------------
//ПЕРЕДАВАЕМЫЕ ПАРАМЕТРЫ:
//1. uint Index - индекс размещения информации об антенне в массиве AntennInfo
//-----------------------------------------------------------------------------
//ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
//char *s - наименование типа антенны.
//*****************************************************************************
char* GetAntNameByIndex(uint Index)
{ uint Count = sizeof(AntennInfo)/sizeof(AntennInfo[0]);

  if (Index >= Count) Index = 0;

 return AntennInfo[Index].eng_name;
}
