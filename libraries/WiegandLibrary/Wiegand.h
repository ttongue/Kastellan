#ifndef _WIEGAND_H
#define _WIEGAND_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Energia.h"
#else
#include "WProgram.h"
#endif

/*#define D0PinA 2			// Arduino Pin 2 Hardware interrupt
#define D1PinA 3			// Arduino Pin 3 Hardware interrupt

#define D0PinB 21			// Arduino Pin 20 Hardware interrupt
#define D1PinB 20			// Arduino Pin 21 Hardware interrupt

#define D0PinC 19			// Arduino Pin 19 Hardware interrupt
#define D1PinC 18			// Arduino Pin 18 Hardware interrupt
*/
#define D0PinA PM_6			// RFID Reader A - Data0
#define D1PinA PQ_1			// RFID Reader A - Data1
#define BZPinA PK_7			// RFID Reader A - Buzzer
#define LGPinA PK_6			// RFID Reader A - Green LED
#define LRPinA PH_1			// RFID Reader A - Red LED
#define CLOCKA PP_3			// RFID Reader A - Clock

//#define D0PinB PQ_3			// RFID Reader B - Data0
//#define D1PinB PQ_2			// RFID Reader B - Data1
//#define BZPinB PH_0			// RFID Reader B - Buzzer
//#define LGPinB PM_2			// RFID Reader B - Green LED
//#define LRPinB PM_1			// RFID Reader B - Red LED

#define D0PinB PA_7			// RFID Reader B - Data0
#define D1PinB PP_5			// RFID Reader B - Data1
#define BZPinB PM_0			// RFID Reader B - Buzzer
#define LGPinB PK_5			// RFID Reader B - Green LED
#define LRPinB PK_4			// RFID Reader B - Red LED
#define CLOCKB PM_7			// RFID Reader B - Clock

#define D0PinC PP_2			// RFID Reader C - Data0
#define D1PinC PN_3			// RFID Reader C - Data1
#define BZPinC PL_3			// RFID Reader C - Buzzer
#define LGPinC PL_2			// RFID Reader C - Green LED
#define LRPinC PL_1			// RFID Reader C - Red LED
#define CLOCKC PN_3			// RFID Reader C - Clock

//#define D0PinE PD_0			// RFID Reader E - Data0
//#define D1PinE PD_1			// RFID Reader E - Data1
//#define BZPinE PL_0			// RFID Reader E - Buzzer
//#define LGPinE PL_5			// RFID Reader E - Green LED
//#define LRPinE PL_4			// RFID Reader E - Red LED

#define D0PinD PH_3			// RFID Reader D - Data0
#define D1PinD PH_2			// RFID Reader D - Data1
#define BZPinD PG_0			// RFID Reader D - Buzzer
#define LGPinD PF_3			// RFID Reader D - Green LED
#define LRPinD PF_2			// RFID Reader D - Red LED
#define CLOCKD PM_3			// RFID Reader D - Clock

#define D0PinE PK_3			// RFID Reader E - Data0
#define D1PinE PK_2			// RFID Reader E - Data1
#define CLOCKE PK_1			// RFID Reader E - Clock
#define BZPinE PP_4			// RFID Reader E - Buzzer
#define LGPinE PQ_0			// RFID Reader E - Green LED
#define LRPinE PD_5			// RFID Reader E - Red LED

#define D0PinF PK_0			// RFID Reader F - Data0
#define D1PinF PB_5			// RFID Reader F - Data1
#define BZPinF PD_4			// RFID Reader F - Buzzer
#define LGPinF PP_1			// RFID Reader F - Green LED
#define LRPinF PP_0			// RFID Reader F - Red LED
#define CLOCKF PB_4			// RFID Reader F - Clock

class RFID_READER {

public:
	RFID_READER();
	void begin(int d0Pin, int d1Pin, int gateNum, int bzPin, int gLEDPin, int rLEDPin);
	void begin(int d0Pin, int d1Pin, int gateNum, int bzPin, int gLEDPin, int rLEDPin, int clock);
	bool available();
	unsigned long getCode();
	int getWiegandType();
	static int getGateActive();
    void ReadD0();
	void ReadD1();
	void signalCard(bool good);

	static unsigned long 	_sysTick;
	static unsigned long 	_lastWiegand;
	static unsigned long	_lastPulse;
	static int				_GateActive;	
private:

	bool DoWiegandConversion();
	unsigned long GetCardId(unsigned long *codehigh, unsigned long *codelow, char bitlength);
	

	
	unsigned long 	_cardTempHigh;
	unsigned long 	_cardTemp;
	int				_bitCount;	
	int				_wiegandType;
	unsigned long	_code;

	int D0Pin;
	int D1Pin;
	int gateNumber;
	int buzzerPin;
	int greenLEDPin;
	int redLEDPin;

};

class WIEGAND {

public:
	WIEGAND();
	void begin();
	bool available();
	unsigned long getCode();
	int getWiegandType();
	int getGateActive();
	void signalCard(bool good);
	
private:
	//static void readerAReadD0();
	//static void readerAReadD1();
	//static void readerBReadD0();
	//static void readerBReadD1();
	//static void readerCReadD0();
	//static void readerCReadD1();
	//static void readerDReadD0();
	//static void readerDReadD1();
	//static void readerEReadD0();
	//static void readerEReadD1();
	static void readerFReadD0();
	static void readerFReadD1();
	static void readerAClock();
	static void readerBClock();
	static void readerCClock();
	static void readerDClock();
	static void readerEClock();
	static void readerFClock();
	//static void readerGReadD0();
	//static void readerGReadD1();
	//static void readerGClock();
	//static void readerHReadD0();
	//static void readerHReadD1();
	//static void ReadD0A();
	//static void ReadD1A();
	//static void ReadD0B();
	//static void ReadD1B();	
	//static void ReadD0C();
	//static void ReadD1C();		
	bool DoWiegandConversion ();
	/* static unsigned long GetCardId (unsigned long *codehigh, unsigned long *codelow, char bitlength);
	
	static unsigned long 	_sysTick;
	static unsigned long 	_lastWiegand;
	static int				_GateActive;	
	
	static unsigned long 	_cardTempHighA;
	static unsigned long 	_cardTempA;
	static int				_bitCountA;	
	static int				_wiegandTypeA;
	static unsigned long	_codeA;
	
	static unsigned long 	_cardTempHighB;
	static unsigned long 	_cardTempB;
	static int				_bitCountB;	
	static int				_wiegandTypeB;
	static unsigned long	_codeB;
	
	static unsigned long 	_cardTempHighC;
	static unsigned long 	_cardTempC;
	static int				_bitCountC;	
	static int				_wiegandTypeC;
	static unsigned long	_codeC; */

	static RFID_READER readerA;
	static RFID_READER readerB;
	static RFID_READER readerC;
	static RFID_READER readerD;
	static RFID_READER readerE;
	static RFID_READER readerF;
	//static RFID_READER readerG;
	//static RFID_READER readerH;
};





#endif