#include "Wiegand.h"

/*unsigned long WIEGAND::_sysTick=0;
unsigned long WIEGAND::_lastWiegand=0;
int			  WIEGAND::_GateActive=0;    // 1 = Active A   --   2 = Active B  ----  3 = Active C 

unsigned long WIEGAND::_cardTempHighA=0;
unsigned long WIEGAND::_cardTempA=0;
unsigned long WIEGAND::_codeA=0;
int 		  WIEGAND::_bitCountA=0;	
int			  WIEGAND::_wiegandTypeA=0;

unsigned long WIEGAND::_cardTempHighB=0;
unsigned long WIEGAND::_cardTempB=0;
unsigned long WIEGAND::_codeB=0;
int 		  WIEGAND::_bitCountB=0;	
int			  WIEGAND::_wiegandTypeB=0;

unsigned long WIEGAND::_cardTempHighC=0;
unsigned long WIEGAND::_cardTempC=0;
unsigned long WIEGAND::_codeC=0;
int 		  WIEGAND::_bitCountC=0;	
int			  WIEGAND::_wiegandTypeC=0;

*/

RFID_READER WIEGAND::readerA;// = new RFID_READER::RFID_READER();
RFID_READER WIEGAND::readerB;// = new RFID_READER::RFID_READER();
RFID_READER WIEGAND::readerC;// = new RFID_READER::RFID_READER();
RFID_READER WIEGAND::readerD;// = new RFID_READER::RFID_READER();
RFID_READER WIEGAND::readerE;// = new RFID_READER::RFID_READER();
RFID_READER WIEGAND::readerF;
//RFID_READER WIEGAND::readerG;
//RFID_READER WIEGAND::readerH;

WIEGAND::WIEGAND()
{
	
}

unsigned long WIEGAND::getCode()
{
  switch (RFID_READER::getGateActive()) {
    case 1:
		return readerA.getCode();
      	break;
    case 2:
		return readerB.getCode();
      	break;
	case 3:
		return readerC.getCode();
      	break;
    case 4:
		return readerD.getCode();
      	break;
    case 5:
		return readerE.getCode();
      	break;
    case 6:
		return readerF.getCode();
      	break;
    //case 7:
	//	return readerG.getCode();
    //  	break;
    //case 8:
	//	return readerH.getCode();
    //  	break;
    default: 
		return 0;
  }
}

int WIEGAND::getWiegandType()
{
  switch (RFID_READER::getGateActive()) {
    case 1:
		return readerA.getWiegandType();
      	break;
    case 2:
		return readerB.getWiegandType();
      	break;
	case 3:
		return readerC.getWiegandType();
      	break;
    case 4:
		return readerD.getWiegandType();
      	break;
    case 5:
		return readerE.getWiegandType();
      	break;
    case 6:
		return readerF.getWiegandType();
    	break;
    //case 7:
	//	return readerG.getWiegandType();
    //	break;
    //case 8:
	//	return readerH.getWiegandType();
    //	break;
    default: 
		return 0;
  }
	
}

void WIEGAND::signalCard(bool good=false)
{
  switch (RFID_READER::getGateActive()) {
    case 1:
		return readerA.signalCard(good);
      	break;
    case 2:
		return readerB.signalCard(good);
      	break;
	case 3:
		return readerC.signalCard(good);
      	break;
    case 4:
		return readerD.signalCard(good);
      	break;
    case 5:
		return readerE.signalCard(good);
      	break;
    case 6:
		return readerF.signalCard(good);
    	break;
    //case 7:
	//	return readerG.signalCard(good);
    //	break;
    //case 8:
	//	return readerH.signalCard(good);
    //	break;
    default: 
		break;
  }
	
}

int WIEGAND::getGateActive()
{
	return RFID_READER::getGateActive();
}

bool WIEGAND::available()
{
	return DoWiegandConversion();
}

void WIEGAND::begin()
{
/*	readerA.begin(D0PinA,D1PinA,1,BZPinA,LGPinA,LRPinA,CLOCKA);
	//attachInterrupt(D0PinA, readerAReadD0, FALLING);	// Hardware interrupt - high to low pulse
	//attachInterrupt(D1PinA, readerAReadD1, FALLING);	// Hardware interrupt - high to low pulse
    //attachInterrupt(CLOCKA, readerAClock, FALLING);
    attachInterrupt(D0PinA, readerAClock, FALLING);	// Hardware interrupt - high to low pulse
	attachInterrupt(D1PinA, readerAClock, FALLING);	// Hardware interrupt - high to low pulse
	

	readerB.begin(D0PinB,D1PinB,2,BZPinB,LGPinB,LRPinB,CLOCKB);
	//attachInterrupt(D0PinB, readerBReadD0, FALLING);	// Hardware interrupt - high to low pulse
	//attachInterrupt(D1PinB, readerBReadD1, FALLING);	// Hardware interrupt - high to low pulse
	//attachInterrupt(CLOCKB, readerBClock, FALLING);
	attachInterrupt(D0PinB, readerBClock, FALLING);	// Hardware interrupt - high to low pulse
	attachInterrupt(D1PinB, readerBClock, FALLING);	// Hardware interrupt - high to low pulse


	readerC.begin(D0PinC,D1PinC,3,BZPinC,LGPinC,LRPinC,CLOCKC);
	//attachInterrupt(D0PinC, readerCReadD0, FALLING);	// Hardware interrupt - high to low pulse
	//attachInterrupt(D1PinC, readerCReadD1, FALLING);	// Hardware interrupt - high to low pulse
	//attachInterrupt(CLOCKC, readerCClock, FALLING);
	attachInterrupt(D0PinC, readerCClock, FALLING);	// Hardware interrupt - high to low pulse
	attachInterrupt(D1PinC, readerCClock, FALLING);	// Hardware interrupt - high to low pulse

	readerD.begin(D0PinD,D1PinD,4,BZPinD,LGPinD,LRPinD,CLOCKD);
	//attachInterrupt(D0PinD, readerDReadD0, FALLING);	// Hardware interrupt - high to low pulse
	//attachInterrupt(D1PinD, readerDReadD1, FALLING);	// Hardware interrupt - high to low pulse
	attachInterrupt(CLOCKD, readerDClock, FALLING);

	readerE.begin(D0PinE,D1PinE,5,BZPinE,LGPinE,LRPinE,CLOCKE);
	//attachInterrupt(D0PinE, readerEReadD0, FALLING);	// Hardware interrupt - high to low pulse
	//attachInterrupt(D1PinE, readerEReadD1, FALLING);	// Hardware interrupt - high to low pulse
	attachInterrupt(CLOCKE, readerEClock, FALLING);
*/
	readerF.begin(D0PinF,D1PinF,6,BZPinF,LGPinF,LRPinF,CLOCKF);
	attachInterrupt(D0PinF, readerFReadD0, FALLING);	// Hardware interrupt - high to low pulse
	attachInterrupt(D1PinF, readerFReadD1, FALLING);	// Hardware interrupt - high to low pulse
	//attachInterrupt(CLOCKF, readerFClock, FALLING);
	//attachInterrupt(D0PinF, readerFClock, FALLING);	// Hardware interrupt - high to low pulse
	//attachInterrupt(D1PinF, readerFClock, FALLING);	// Hardware interrupt - high to low pulse

	//readerG.begin(D0PinG,D1PinG,7,BZPinG,LGPinG,LRPinG,CLOCKG);
	//attachInterrupt(D0PinG, readerGReadD0, FALLING);	// Hardware interrupt - high to low pulse
	//attachInterrupt(D1PinG, readerGReadD1, FALLING);	// Hardware interrupt - high to low pulse
	//attachInterrupt(CLOCKG, readerGClock, FALLING);

	//readerH.begin(D0PinH,D1PinH,8,BZPinH,LGPinH,LRPinH);

	//attachInterrupt(D0PinH, readerHReadD0, FALLING);	// Hardware interrupt - high to low pulse
	//attachInterrupt(D1PinH, readerHReadD1, FALLING);	// Hardware interrupt - high to low pulse
	//RFID_READER::_sysTick=millis();
	//RFID_READER::_lastWiegand = 0;

	//_sysTick=millis();
	//_lastWiegand = 0;
	
	//_cardTempHighA = 0;
	//_cardTempA = 0;
	//_codeA = 0;
	//_wiegandTypeA = 0;
	//_bitCountA = 0;  
	
	
	//_cardTempHighB = 0;
	//_cardTempB = 0;
	//_codeB = 0;
	//_wiegandTypeB = 0;
	//_bitCountB = 0;  

	
	//_cardTempHighC = 0;
	//_cardTempC = 0;
	//_codeC = 0;
	//_wiegandTypeC = 0;
	//_bitCountC = 0;  

	
	//pinMode(D0PinA, INPUT);					// Set D0 pin as input
	//pinMode(D1PinA, INPUT);					// Set D1 pin as input
	//attachInterrupt(0, ReadD0A, FALLING);	// Hardware interrupt - high to low pulse
	//attachInterrupt(1, ReadD1A, FALLING);	// Hardware interrupt - high to low pulse
		
	 //pinMode(D0PinB, INPUT);					// Set D0 pin as input
	 //pinMode(D1PinB, INPUT);					// Set D1 pin as input
	 //attachInterrupt(2, ReadD0B, FALLING);	// Hardware interrupt - high to low pulse
	 //attachInterrupt(3, ReadD1B, FALLING);	// Hardware interrupt - high to low pulse
	
	// pinMode(D0PinC, INPUT);					// Set D0 pin as input
	// pinMode(D1PinC, INPUT);					// Set D1 pin as input
	// attachInterrupt(4, ReadD0C, FALLING);	// Hardware interrupt - high to low pulse
	// attachInterrupt(5, ReadD1C, FALLING);	// Hardware interrupt - high to low pulse
}

//void WIEGAND::readerAReadD0 ()
//{	
//	Serial3.print("A");
//	readerA.ReadD0();
	//Serial3.print("0");
//}

//void WIEGAND::readerAReadD1 ()
//{	
//	Serial3.print("A");
//	readerA.ReadD1();
	
	//Serial3.print("1");
//}

void WIEGAND::readerAClock ()
{
	int b0 = digitalRead(D0PinA);
	int b1 = digitalRead(D1PinA);
	b0=b0+digitalRead(D0PinA);
	b1=b1+digitalRead(D1PinA);
	b0=b0+digitalRead(D0PinA);
	b1=b1+digitalRead(D1PinA);
	b0=b0+digitalRead(D0PinA);
	b1=b1+digitalRead(D1PinA);
	b0=b0+digitalRead(D0PinA);
	b1=b1+digitalRead(D1PinA);
	//Serial3.print(".");
	if ((b0>3) || (b1>3)) {
		Serial3.print(".");
		
    	if ((b0 < 3) && (b1> 3)) 
    	{
    		readerA.ReadD0(); 
    		return;
    	} 
    	if ((b0>3) && (b1 < 3))
    	{
    		readerA.ReadD1();
			return;
    	}
	}
}

// void WIEGAND::readerBReadD0 ()
// {
// 	Serial3.print("B");
// 	readerB.ReadD0();
	
// }

// void WIEGAND::readerBReadD1 ()
// {
// 	Serial3.print("B");
// 	readerB.ReadD1();
	
// }

void WIEGAND::readerBClock ()
{
	int b0 = digitalRead(D0PinB);
	int b1 = digitalRead(D1PinB);
	Serial3.print(".");
    if ((b0 == 0) && (b1== 1)) 
    {
    	readerB.ReadD0(); 
    	return;
    } 
    if ((b0==1) && (b1 == 0))
    {
    	readerB.ReadD1();
		return;
    }
}

// void WIEGAND::readerCReadD0 ()
// {
// 	Serial3.print("C");
// 	readerC.ReadD0();
	
// }

// void WIEGAND::readerCReadD1 ()
// {
// 	Serial3.print("C");
// 	readerC.ReadD1();
	
// }

void WIEGAND::readerCClock ()
{
	int b0 = digitalRead(D0PinC);
	int b1 = digitalRead(D1PinC);
	Serial3.print(".");
    if ((b0 == 0) && (b1== 1)) 
    {
    	readerC.ReadD0(); 
    	return;
    } 
    if ((b0==1) && (b1 == 0))
    {
    	readerC.ReadD1();
		return;
    }
}

// void WIEGAND::readerDReadD0 ()
// {
// 	Serial3.print("D");
// 	readerD.ReadD0();
	
// }

// void WIEGAND::readerDReadD1 ()
// {
// 	Serial3.print("D");
// 	readerD.ReadD1();
	
// }

void WIEGAND::readerDClock ()
{
	int b0 = digitalRead(D0PinD);
	int b1 = digitalRead(D1PinD);
	Serial3.print(".");
    if ((b0 == 0) && (b1== 1)) 
    {
    	readerD.ReadD0(); 
    	return;
    } 
    if ((b0==1) && (b1 == 0))
    {
    	readerD.ReadD1();
		return;
    }
}

// void WIEGAND::readerEReadD0 ()
// {
// 	Serial3.print("E");
// 	readerE.ReadD0();
	
// }

// void WIEGAND::readerEReadD1 ()
// {
// 	Serial3.print("E");
// 	readerE.ReadD1();
	
// }

void WIEGAND::readerEClock ()
{
	int b0 = digitalRead(D0PinE);
	int b1 = digitalRead(D1PinE);
	Serial3.print(".");
    if ((b0 == 0) && (b1== 1)) 
    {
    	readerE.ReadD0(); 
    	return;
    } 
    if ((b0==1) && (b1 == 0))
    {
    	readerE.ReadD1();
		return;
    }
}

void WIEGAND::readerFReadD0 ()
{
 	Serial3.print("f");
 	readerF.ReadD0();
	
}

void WIEGAND::readerFReadD1 ()
{
 	Serial3.print("F");
 	readerF.ReadD1();
	
}

void WIEGAND::readerFClock ()
{
	/*
	int b0 = digitalRead(D0PinF);
	int b1 = digitalRead(D1PinF);
	Serial3.print(".");
    if ((b0 == 0) && (b1== 1)) 
    {
    	readerF.ReadD0(); 
    	return;
    } 
    if ((b0==1) && (b1 == 0))
    {
    	readerF.ReadD1();
		return;
    } */
    int b0 = digitalRead(D0PinF);
	int b1 = digitalRead(D1PinF);
	b0=b0+digitalRead(D0PinF);
	b1=b1+digitalRead(D1PinF);
	b0=b0+digitalRead(D0PinF);
	b1=b1+digitalRead(D1PinF);
	b0=b0+digitalRead(D0PinF);
	b1=b1+digitalRead(D1PinF);
	b0=b0+digitalRead(D0PinF);
	b1=b1+digitalRead(D1PinF);
	//Serial3.print(".");
	if ((b0>3) || (b1>3)) {
		Serial3.print(".");
		
    	if ((b0 < 3) && (b1> 3)) 
    	{
    		Serial3.print("+");
    		readerF.ReadD0(); 
    		return;
    	} 
    	if ((b0>3) && (b1 < 3))
    	{
    		Serial3.print("-");
    		readerF.ReadD1();
			return;
    	}
	}
}

// void WIEGAND::readerGClock ()
// {
// 	int b0 = digitalRead(D0PinG);
// 	int b1 = digitalRead(D1PinG);
// 	Serial3.print(".");
//     if ((b0 == 0) && (b1== 1)) 
//     {
//     	readerG.ReadD0(); 
//     	return;
//     } 
//     if ((b0==1) && (b1 == 0))
//     {
//     	readerG.ReadD1();
// 		return;
//     }
// }

// void WIEGAND::readerGReadD0 ()
// {
// 	Serial3.print("G");
// 	readerG.ReadD0();
	
// }

// void WIEGAND::readerGReadD1 ()
// {
// 	Serial3.print("G");
// 	readerG.ReadD1();
	
// }

// void WIEGAND::readerHReadD0 ()
// {
// 	Serial3.print("H");
// 	readerH.ReadD0();
	
// }

// void WIEGAND::readerHReadD1 ()
// {
// 	Serial3.print("H");
// 	readerH.ReadD1();
	
// }

/*
void WIEGAND::ReadD0A ()
{
	_bitCountA++;				// Increament bit count for Interrupt connected to D0
	if (_bitCountA>31)			// If bit count more than 31, process high bits
	{
		_cardTempHighA |= ((0x80000000 & _cardTempA)>>31);	//	shift value to high bits
		_cardTempHighA <<= 1;
		_cardTempA <<=1;
	}
	else
	{
		_cardTempA <<= 1;		// D0 represent binary 0, so just left shift card data
	}
	_lastWiegand = _sysTick;	// Keep track of last wiegand bit received
}

void WIEGAND::ReadD1A()
{
	_bitCountA ++;				// Increment bit count for Interrupt connected to D1
	if (_bitCountA>31)			// If bit count more than 31, process high bits
	{
		_cardTempHighA |= ((0x80000000 & _cardTempA)>>31);	// shift value to high bits
		_cardTempHighA <<= 1;
		_cardTempA |= 1;
		_cardTempA <<=1;
	}
	else
	{
		_cardTempA |= 1;			// D1 represent binary 1, so OR card data with 1 then
		_cardTempA <<= 1;		// left shift card data
	}
	_lastWiegand = _sysTick;	// Keep track of last wiegand bit received
}


void WIEGAND::ReadD0B ()
{
	_bitCountB++;				// Increament bit count for Interrupt connected to D0
	if (_bitCountB>31)			// If bit count more than 31, process high bits
	{
		_cardTempHighB |= ((0x80000000 & _cardTempB)>>31);	//	shift value to high bits
		_cardTempHighB <<= 1;
		_cardTempB <<=1;
	}
	else
	{
		_cardTempB <<= 1;		// D0 represent binary 0, so just left shift card data
	}
	_lastWiegand = _sysTick;	// Keep track of last wiegand bit received
}

void WIEGAND::ReadD1B()
{
	_bitCountB ++;				// Increment bit count for Interrupt connected to D1
	if (_bitCountB>31)			// If bit count more than 31, process high bits
	{
		_cardTempHighB |= ((0x80000000 & _cardTempB)>>31);	// shift value to high bits
		_cardTempHighB <<= 1;
		_cardTempB |= 1;
		_cardTempB <<=1;
	}
	else
	{
		_cardTempB |= 1;			// D1 represent binary 1, so OR card data with 1 then
		_cardTempB <<= 1;		// left shift card data
	}
	_lastWiegand = _sysTick;	// Keep track of last wiegand bit received
}


void WIEGAND::ReadD0C ()
{
	_bitCountC++;				// Increament bit count for Interrupt connected to D0
	if (_bitCountC>31)			// If bit count more than 31, process high bits
	{
		_cardTempHighC |= ((0x80000000 & _cardTempC)>>31);	//	shift value to high bits
		_cardTempHighC <<= 1;
		_cardTempC <<=1;
	}
	else
	{
		_cardTempB <<= 1;		// D0 represent binary 0, so just left shift card data
	}
	_lastWiegand = _sysTick;	// Keep track of last wiegand bit received
}

void WIEGAND::ReadD1C()
{
	_bitCountC ++;				// Increment bit count for Interrupt connected to D1
	if (_bitCountC>31)			// If bit count more than 31, process high bits
	{
		_cardTempHighC |= ((0x80000000 & _cardTempC)>>31);	// shift value to high bits
		_cardTempHighC <<= 1;
		_cardTempC |= 1;
		_cardTempC <<=1;
	}
	else
	{
		_cardTempC |= 1;			// D1 represent binary 1, so OR card data with 1 then
		_cardTempC <<= 1;		// left shift card data
	}
	_lastWiegand = _sysTick;	// Keep track of last wiegand bit received
}
*/

/*
unsigned long WIEGAND::GetCardId (unsigned long *codehigh, unsigned long *codelow, char bitlength)
{
	unsigned long cardID=0;

	if (bitlength==26)								// EM tag
		cardID = (*codelow & 0x1FFFFFE) >>1;

	if (bitlength==34)								// Mifare 
	{
		*codehigh = *codehigh & 0x03;				// only need the 2 LSB of the codehigh
		*codehigh <<= 30;							// shift 2 LSB to MSB		
		*codelow >>=1;
		cardID = *codehigh | *codelow;
	}
	return cardID;
}

*/

bool WIEGAND::DoWiegandConversion ()
{
	//unsigned long cardIDA;
	//unsigned long cardIDB;
	//unsigned long cardIDC;

	
	RFID_READER::_sysTick=millis();
	if ((RFID_READER::_sysTick - RFID_READER::_lastWiegand) > 100)								// if no more signal coming through after 25ms
	{
		if (readerA.available()) { return true; }
		if (readerB.available()) { return true; }
		if (readerC.available()) { return true; }
		if (readerD.available()) { return true; }
		if (readerE.available()) { return true; }
		if (readerF.available()) { return true; }
		//if (readerG.available()) { return true; }
		//if (readerH.available()) { return true; }
		return false;
		/*
		if ((_bitCountA==26) || (_bitCountA==34) || (_bitCountA==8) || (_bitCountB==26) || (_bitCountB==34) || (_bitCountB==8) ) 	// bitCount for keypress=8, Wiegand 26=26, Wiegand 34=34
		{
			if ((_bitCountA==26) || (_bitCountA==34) || (_bitCountA==8)) 	// bitCount for keypress=8, Wiegand 26=26, Wiegand 34=34
			{
				_cardTempA >>= 1;			// shift right 1 bit to get back the real value - interrupt done 1 left shift in advance
				if (_bitCountA>32)			// bit count more than 32 bits, shift high bits right to make adjustment
					_cardTempHighA >>= 1;	

				if((_bitCountA==26) || (_bitCountA==34))		// wiegand 26 or wiegand 34
				{
					cardIDA = GetCardId (&_cardTempHighA, &_cardTempA, _bitCountA);
					_wiegandTypeA=_bitCountA;
					_bitCountA=0;
					_cardTempA=0;
					_cardTempHighA=0;
					_GateActive=1;
					_codeA=cardIDA;
					return true;				
				}
				else if (_bitCountA==8)		// keypress wiegand
				{
					// 8-bit Wiegand keyboard data, high nibble is the "NOT" of low nibble
					// eg if key 1 pressed, data=E1 in binary 11100001 , high nibble=1110 , low nibble = 0001 
					char highNibble = (_cardTempA & 0xf0) >>4;
					char lowNibble = (_cardTempA & 0x0f);
					_wiegandTypeA=_bitCountA;					
					_bitCountA=0;
					_cardTempA=0;
					_cardTempHighA=0;
					_GateActive=1;
					
					if (lowNibble == (~highNibble & 0x0f))		// check if low nibble matches the "NOT" of high nibble.
					{
						if (lowNibble==0x0b)					// ENT pressed
						{
							_codeA=0x0d;							
						}
						else if (lowNibble==0x0a)				// ESC pressed
						{
							_codeA=0x1b;							
						}
						else
						{
						_codeA=(int)lowNibble;				// 0 - 9 keys
						}
						return true;
					}
				}
			}
			else
			{
				// well time over 25 ms and bitCount !=8 , !=26, !=34 , must be noise or nothing then.
				_lastWiegand=_sysTick;
				_bitCountA=0;			
				_cardTempA=0;
				_cardTempHighA=0;
				_GateActive=0;
			}	
				// fine controllo accesso A 
				
				
				
				
				
				// inizio controllo accesso B 
			if ((_bitCountB==26) || (_bitCountB==34) || (_bitCountB==8)) 	// bitCount for keypress=8, Wiegand 26=26, Wiegand 34=34
			{
				_cardTempB >>= 1;			// shift right 1 bit to get back the real value - interrupt done 1 left shift in advance
				if (_bitCountB>32)			// bit count more than 32 bits, shift high bits right to make adjustment
					_cardTempHighB >>= 1;	

				if((_bitCountB==26) || (_bitCountB==34))		// wiegand 26 or wiegand 34
				{
					cardIDB = GetCardId (&_cardTempHighB, &_cardTempB, _bitCountB);
					_wiegandTypeB=_bitCountB;
					_bitCountB=0;
					_cardTempB=0;
					_cardTempHighB=0;
					_GateActive=2;
					_codeB=cardIDB;
					return true;				
				}
				else if (_bitCountB==8)		// keypress wiegand
				{
					// 8-bit Wiegand keyboard data, high nibble is the "NOT" of low nibble
					// eg if key 1 pressed, data=E1 in binary 11100001 , high nibble=1110 , low nibble = 0001 
					char highNibble = (_cardTempB & 0xf0) >>4;
					char lowNibble = (_cardTempB & 0x0f);
					_wiegandTypeB=_bitCountB;					
					_bitCountB=0;
					_cardTempB=0;
					_cardTempHighB=0;
					_GateActive=2;
				
					if (lowNibble == (~highNibble & 0x0f))		// check if low nibble matches the "NOT" of high nibble.
					{
						if (lowNibble==0x0b)					// ENT pressed
						{
							_codeB=0x0d;							
						}
						else if (lowNibble==0x0a)				// ESC pressed
						{
							_codeB=0x1b;							
						}
						else
						{
							_codeB=(int)lowNibble;				// 0 - 9 keys
						}
						return true;
					}
				}
			}
			else
			{
				// well time over 25 ms and bitCount !=8 , !=26, !=34 , must be noise or nothing then.
				_lastWiegand=_sysTick;
				_bitCountB=0;			
				_cardTempB=0;
				_cardTempHighB=0;
				_GateActive=0;
			}
		return false;
		}
		else
		    return false;
		    */
	}
	else
		return false;
}





unsigned long RFID_READER::_sysTick=0;
unsigned long RFID_READER::_lastWiegand=0;
unsigned long RFID_READER::_lastPulse=0;
int			  RFID_READER::_GateActive=0; 

RFID_READER::RFID_READER()
{
}

unsigned long RFID_READER::getCode()
{
  return _code;
}

int RFID_READER::getWiegandType()
{
	return _wiegandType;
}


bool RFID_READER::available()
{
	return DoWiegandConversion();
}

int RFID_READER::getGateActive()
{
	return _GateActive;
}

void RFID_READER::signalCard(bool good)
{
	if (!good) {
		digitalWrite(redLEDPin,0);
		digitalWrite(buzzerPin,0);
		delay(1000);
		digitalWrite(redLEDPin,1);
		digitalWrite(buzzerPin,1);
	} else {
		digitalWrite(greenLEDPin,0);
		digitalWrite(buzzerPin,0);
		delay(500);
		digitalWrite(greenLEDPin,1);
		digitalWrite(buzzerPin,1);
	}
}

void RFID_READER::begin(int D0p, int D1p, int gateID, int bzPin, int lgPin, int lrPin, int clock)
{
    pinMode(clock,INPUT);
    begin(D0p,D1p,gateID,bzPin,lgPin,lrPin);
}

void RFID_READER::begin(int D0p, int D1p, int gateID, int bzPin, int lgPin, int lrPin)
{
	buzzerPin = bzPin;
	greenLEDPin = lgPin;
	redLEDPin = lrPin;
	RFID_READER::_sysTick=millis();
	RFID_READER::_lastWiegand = 0;
	
	_cardTempHigh = 0;
	_cardTemp = 0;
	_code = 0;
	_wiegandType = 0;
	_bitCount = 0;  

	gateNumber=gateID;

	pinMode(D0p, INPUT);					// Set D0 pin as input
	pinMode(D1p, INPUT);					// Set D1 pin as input
	pinMode(buzzerPin, OUTPUT);
	pinMode(greenLEDPin, OUTPUT);
	pinMode(redLEDPin, OUTPUT);
//	attachInterrupt(D0p, ReadD0, FALLING);	// Hardware interrupt - high to low pulse
//	attachInterrupt(D1p, ReadD1, FALLING);	// Hardware interrupt - high to low pulse
	
}

void RFID_READER::ReadD0 ()
{
	unsigned long usTime = micros();
	unsigned long lastPulseTime = RFID_READER::_lastPulse;
	if ((usTime - RFID_READER::_lastPulse) < 1900) {
		return;
	}
	RFID_READER::_lastPulse = usTime;
	_bitCount++;				// Increament bit count for Interrupt connected to D0
	if (_bitCount>31)			// If bit count more than 31, process high bits
	{
		_cardTempHigh |= ((0x80000000 & _cardTemp)>>31);	//	shift value to high bits
		_cardTempHigh <<= 1;
		_cardTemp <<=1;
	}
	else
	{
		_cardTemp <<= 1;		// D0 represent binary 0, so just left shift card data
	}
	//String output=" 0 "+String(usTime,DEC)+" "+String(lastPulseTime,DEC);
	//Serial3.println(output);
	RFID_READER::_lastWiegand = _sysTick;	// Keep track of last wiegand bit received
}

void RFID_READER::ReadD1()
{	
	unsigned long usTime = micros();
	unsigned long lastPulseTime = RFID_READER::_lastPulse;
	if ((usTime - RFID_READER::_lastPulse) < 1900) {
		return;
	}
	RFID_READER::_lastPulse = usTime;
	
	_bitCount ++;				// Increment bit count for Interrupt connected to D1
	if (_bitCount>31)			// If bit count more than 31, process high bits
	{
		_cardTempHigh |= ((0x80000000 & _cardTemp)>>31);	// shift value to high bits
		_cardTempHigh <<= 1;
		_cardTemp |= 1;
		_cardTemp <<=1;
	}
	else
	{
		_cardTemp |= 1;			// D1 represent binary 1, so OR card data with 1 then
		_cardTemp <<= 1;		// left shift card data
	}
	//String output=" 1 "+String(usTime,DEC)+" "+String(lastPulseTime,DEC);
	//Serial3.println(output);
	RFID_READER::_lastWiegand = _sysTick;	// Keep track of last wiegand bit received
}





unsigned long RFID_READER::GetCardId (unsigned long *codehigh, unsigned long *codelow, char bitlength)
{
	unsigned long cardID=0;

	if (bitlength==26)								// EM tag
		cardID = (*codelow & 0x1FFFFFE) >>1;

	if (bitlength==34)								// Mifare 
	{
		*codehigh = *codehigh & 0x03;				// only need the 2 LSB of the codehigh
		*codehigh <<= 30;							// shift 2 LSB to MSB		
		*codelow >>=1;
		cardID = *codehigh | *codelow;
	}
	return cardID;
}

bool RFID_READER::DoWiegandConversion ()
{
	unsigned long cardID;
		
	//RFID_READER::_sysTick=millis();
	//if ((RFID_READER::_sysTick - RFID_READER::_lastWiegand) > 25)								// if no more signal coming through after 25ms
	//{
		if ((_bitCount==26) || (_bitCount==34) || (_bitCount==8)) 	// bitCount for keypress=8, Wiegand 26=26, Wiegand 34=34
		{
			
				_cardTemp >>= 1;			// shift right 1 bit to get back the real value - interrupt done 1 left shift in advance
				if (_bitCount>32)			// bit count more than 32 bits, shift high bits right to make adjustment
					_cardTempHigh >>= 1;	

				if((_bitCount==26) || (_bitCount==34))		// wiegand 26 or wiegand 34
				{
					cardID = GetCardId (&_cardTempHigh, &_cardTemp, _bitCount);
					_wiegandType=_bitCount;
					_bitCount=0;
					_cardTemp=0;
					_cardTempHigh=0;
					RFID_READER::_GateActive=gateNumber;
					_code=cardID;
					return true;				
				}
				else if (_bitCount==8)		// keypress wiegand
				{
					// 8-bit Wiegand keyboard data, high nibble is the "NOT" of low nibble
					// eg if key 1 pressed, data=E1 in binary 11100001 , high nibble=1110 , low nibble = 0001 
					char highNibble = (_cardTemp & 0xf0) >>4;
					char lowNibble = (_cardTemp & 0x0f);
					_wiegandType=_bitCount;					
					_bitCount=0;
					_cardTemp=0;
					_cardTempHigh=0;
					RFID_READER::_GateActive=gateNumber;
					
					if (lowNibble == (~highNibble & 0x0f))		// check if low nibble matches the "NOT" of high nibble.
					{
						if (lowNibble==0x0b)					// ENT pressed
						{
							_code=0x0d;							
						}
						else if (lowNibble==0x0a)				// ESC pressed
						{
							_code=0x1b;							
						}
						else
						{
						_code=(int)lowNibble;				// 0 - 9 keys
						}
						return true;
					}
				}
		}
		else
		{
			// well time over 25 ms and bitCount !=8 , !=26, !=34 , must be noise or nothing then.
			RFID_READER::_lastWiegand=_sysTick;
			_bitCount=0;			
			_cardTemp=0;
			_cardTempHigh=0;
			RFID_READER::_GateActive=0;
			return false;
		}	
	//}		
	return false;			
				
}				
				



