#ifndef iarduino_RTC_DS1307_h
#define iarduino_RTC_DS1307_h
#define	RTC_DS1307 2																//	Модуль часов реального времени с протоколом передачи данных I2C, памятью 064x8 (56 байт которой доступны для хранения данных пользователя)

class iarduino_RTC_DS1307: public iarduino_RTC_BASE{

	public:

//		Инициализация модуля:
		void	begin(void){																														//		(без параметров)
//					Инициализация работы с шиной I2C:
					funcBegin(100);																													//		(скорость шины в кГц)
//					Установка флагов управления и состояния модуля:
					varI=funcReadReg(0x00); if( varI & 0b10000000                         ){funcWriteReg(0x00, (varI&~0b10000000)            );}	//		(если установлен 7 бит в 0 регистре, то сбрасываем его - запускаем генератор)
					varI=funcReadReg(0x02); if( varI & 0b01000000                         ){funcWriteReg(0x02, (varI&~0b01000000)            );}	//		(если установлен 6 бит в 2 регистре, то сбрасываем его - переводим модуль в 24 часовой режим)
					varI=funcReadReg(0x07); if((varI & 0b00000011) || !(varI & 0b00010000)){funcWriteReg(0x07, (varI&~0b00000011)|0b00010000 );}	//		(если установлены 1 и 0 биты или сброшен 4 бит в 7 регистре, то сбрасываем 1 с 0 битами, а 4 устанавливаем - выводим меандр с частотой 1 Гц на выводе SQW/OUT модуля)
		}

//		Чтение одного значения из регистров даты и времени модуля:
		uint8_t	funcReadTimeIndex(uint8_t i){delay(1); return funcReadReg(arrTimeRegAddr[i]) & arrTimeRegMack[i];}									//		(i = 0-секунды / 1-минуты / 2-часы / 3-день / 4-месяц / 5-год / 6-день недели)

//		Запись одного значения в регистры даты и времени модуля:
		void	funcWriteTimeIndex(uint8_t i, uint8_t j){																							//		(i = 0-секунды / 1-минуты / 2-часы / 3-день / 4-месяц / 5-год / 6-день недели, j = значение)
					varI=funcReadTimeIndex(i);																										//		Читаем данные из регистра i
					j |= ~arrTimeRegMack[i] & varI;																									//		Устанавливаем биты значения j по маске arrTimeRegMack[i] в прочитанные из регистра i
					j &=  arrTimeRegMack[i] | varI;																									//		Сбрасываем    биты значения j по маске arrTimeRegMack[i] в прочитанные из регистра i
					funcWriteReg(arrTimeRegAddr[i], j);																								//		Сохраняем значение j в регистр arrTimeRegAddr[i]
		}

	private:

	/**	Внутренние переменные **/

		uint8_t	valAddress				=	 0x68;									//	Адрес модуля на шине I2C
		uint8_t	arrTimeRegAddr[7]		=	{0x00,0x01,0x02,0x04,0x05,0x06,0x03};	//	Определяем массив с адресами регистров даты и времени				(сек, мин, час, день, месяц, год, день недели)
		uint8_t	arrTimeRegMack[7]		=	{0x7F,0x7F,0x3F,0x3F,0x1F,0xFF,0x07};	//	Определяем маскировочный массив для регистров даты и времени		(при чтении/записи, нужно совершить побитовое «и»)
		uint8_t	varI;

	/**	Внутренние функции **/

//		Функция чтения данных из регистра модуля:
		uint8_t	funcReadReg(uint8_t i){												//	Определяем функцию читения данных из регистра модуля				(аргумент: адрес_регистра)
					varI=1;															//	Предустанавливаем переменную varI в значение 1, чтоб не вывести: 45 апреля 255 часов 127 минут и 200 секунд
					if (	funcStart		()				){						//	Если на шине I2C установилось состояние START, то ...
					if (	funcSendID		(valAddress,0)	){						//	Если модуль ответил ACK на получение адреса устройства valAddress с битом RW=0 (запись), то ...
					if (	funcWriteByte	(i)				){						//	Если модуль ответил ACK на получение адреса регистра i, то ...
					if (	funcReStart		()				){						//	Если на шине I2C установилось состояние RESTART, то ...
					if (	funcSendID		(valAddress,1)	){						//	Если модуль ответил ACK на получение адреса устройства valAddress с битом RW=1 (чтение), то ...
					varI =	funcReadByte	(false);								//	Читаем байт в переменную varI с отправкой бита NACK по шине I2C
					}}}}}	funcStop();												//	Устанавливаем состояние STOP на шине I2C
					return	varI;													//	Возвращаем значение переменной varI
		}

//		Функция записи данных в регистр модуля:
		bool	funcWriteReg(uint8_t i, uint8_t j){									//	Определяем функцию записи данных в регистр модуля					(аргументы: адрес_регистра, байт_данных)
							varI=1;													//	Сбрасываем переменную varI в 0
					if (	funcStart		()				){	varI=1;				//	Если на шине I2C установилось состояние START, то ...
					if (	funcSendID		(valAddress,0)	){	varI=2;				//	Если модуль ответил ACK на получение адреса устройства valAddress с битом RW=0 (запись), то ...
					if (	funcWriteByte	(i)				){	varI=3;				//	Если модуль ответил ACK на получение адреса регистра i, то ...
					if (	funcWriteByte	(j)				){	varI=4;				//	Если модуль ответил ACK на получение байта данных j, то ...
					}}}}	funcStop		();		return		varI==4;			//	Отправляем команду STOP и возвращаем результат записи
		}

	/**	функции для работы с шиной I2C **/
		void	funcBegin		(uint32_t j)		/*	Установка регистров шины и подтяжка выводов	(скорость шины в кГц)			*/	{pinMode(SDA, INPUT); pinMode(SCL, INPUT); digitalWrite(SDA, 1); digitalWrite(SCL, 1); TWBR=((F_CPU/(j*1000))-16)/2; if(TWBR<10){TWBR=10;} TWSR&=(~(_BV(TWPS1)|_BV(TWPS0)));                                                                 }
		bool	funcStart		(void)				/*	Установка состояния START					(без параметров)				*/	{uint16_t i=0;                  TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWSTA); while(!(TWCR & _BV(TWINT))){i++; if(i>60000){return false;}} if((TWSR & 0xF8)==0x08)     {return true;}                                             return false;}
		bool	funcReStart		(void)				/*	Установка состояния RESTART					(без параметров)				*/	{uint16_t i=0;                  TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWSTA); while(!(TWCR & _BV(TWINT))){i++; if(i>60000){return false;}} if((TWSR & 0xF8)==0x10)     {return true;}                                             return false;}
		void	funcStop		(void)				/*	Установка состояния STOP					(без параметров)				*/	{uint16_t i=0;                  TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWSTO); while(!(TWCR & _BV(TWSTO))){i++; if(i>60000){break;       }} delayMicroseconds(20);                                                                              }
		bool	funcSendID		(uint8_t j, bool k)	/*	Передача  первого байта						(ID-адрес модуля, бит RW)		*/	{uint16_t i=0; TWDR = (j<<1)+k; TWCR = _BV(TWINT) | _BV(TWEN);              while(!(TWCR & _BV(TWINT))){i++; if(i>60000){return false;}} if((TWSR & 0xF8)==0x40 && k){return true;} if((TWSR & 0xF8)==0x18 && !k){return true;} return false;}
		bool	funcWriteByte	(uint8_t j)			/*	Передача  одного  байта						(байт для передачи)				*/	{uint16_t i=0; TWDR =  j;       TWCR = _BV(TWINT) | _BV(TWEN);              while(!(TWCR & _BV(TWINT))){i++; if(i>60000){return false;}} if((TWSR & 0xF8)==0x28)     {return true;}                                             return false;}
		uint8_t	funcReadByte	(bool j)			/*	Получение одного  байта						(бит подтверждения ACK/NACK)	*/	{uint16_t i=0;                  TWCR = _BV(TWINT) | _BV(TWEN) |  j<<TWEA;   while(!(TWCR & _BV(TWINT))){i++; if(i>60000){return 0;    }} if((TWSR & 0xF8)==0x50 && j){return TWDR;} if((TWSR & 0xF8)==0x58 && !j){return TWDR;} return 0;    }
};

#endif