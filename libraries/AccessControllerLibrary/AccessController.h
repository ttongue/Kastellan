// AccessController.h header file
//
// This Library contains all the classes for data storage and management of the access control system
// and is meant to take most of the code out of the Energia loop.
//
// Written by Thomas Tongue
// Last update 5/22/15
//

#ifndef _ACCESS_CONTROLLER_H
#define _ACCESS_CONTROLLER_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Energia.h"
#else
#include "WProgram.h"
#endif
#include "Ethernet.h"
#include "RTClib.h"
#include "I2C_eeprom.h"

#include "AccessPermissionRecord.h"

//#define NUM_RECORDS 500
#define NUM_RECORDS 5000
#define NUM_ZONES 256
#define NUM_READERGROUPS 32

#define EEPROM_READERGROUP_OFFSET 4096
#define EEPROM_ZONERECORDS_OFFSET 8192
#define EEPROM_PERMRECORDS_OFFSET 16384

class AccessController {

public:
	AccessController();
	bool validateCard(byte facilityCode, int cardNumber);
	bool validateCard(byte facilityCode, int cardNumber, int doorID, RTC_Millis rtc);
	//bool validateCard(byte facilityCode, int cardNumber, int doorID, int zoneID);
	void printStorage();
	void printStorage(EthernetClient myClient);
	void writeRecordsToEEPROM(I2C_eeprom eeprom);
	bool loadRecordsFromEEPROM(I2C_eeprom eeprom);
    bool loadMemberRecordsFromServer(EthernetClient client, char *server, char *recordPath); 
	bool loadZoneRecordsFromServer(EthernetClient client, char *server, char *recordPath); 
	bool loadReaderGroupRecordsFromServer(EthernetClient client, char *server, char *path);
	String getSecondsSince1970FromServer(EthernetClient client, char *server, char *path);
	void loginRFID(EthernetClient client, char *server, char *path, int rfid);


private:
	static AccessPermissionRecord permissionRecords[NUM_RECORDS];
	static ZoneRecord zoneRecords[NUM_ZONES];
	static ReaderGroupRecord readerGroupRecords[NUM_READERGROUPS];
	//static byte facilityCodes[NUM_RECORDS];
	//static unsigned short cardNumbers[NUM_RECORDS];
	//static byte doorIDs[NUM_RECORDS];
	//static byte zoneIDs[NUM_RECORDS];

	//static byte zoneIDRecords[NUM_ZONES];
	//static String zoneDayRecords[NUM_ZONES];
	//static unsigned short zoneStartTime[NUM_ZONES];
	//static unsigned short zoneEndTime[NUM_ZONES];

	void clearStorage();
	void initStorage();

	void clearZoneStorage();
	void clearReaderGroupStorage();
	bool isZoneActive(int zoneID, RTC_Millis rtc);
	bool isDoorInReaderList(int readerGroupID, int doorID);

};


#endif