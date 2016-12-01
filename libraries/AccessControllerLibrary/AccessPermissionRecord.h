// AccessPermissionRecord
//    This file contains the header for the Access permission class, which is used to keep all the 
//    data and methods related to storing an access permission record that can be used to determine if
//    a set of credentials should grant access to a door.
//

//
// Written by Thomas Tongue
// Last update 5/22/15
//

#ifndef _ACCESS_PERMISSION_RECORD_H
#define _ACCESS_PERMISSION_RECORD_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Energia.h"
#else
#include "WProgram.h"
#endif
#include "Ethernet.h"



class AccessPermissionRecord {

public:
	AccessPermissionRecord();
	byte facilityCode;
	unsigned short cardNumber;
	byte doorID;
	byte zoneID;

};

class ZoneRecord {
public:
	ZoneRecord();
	byte zoneID;
    String zoneDays;
    unsigned short zoneStartTime;
    unsigned short zoneEndTime;
};

class ReaderGroupRecord {
public:
	ReaderGroupRecord();
	byte readerGroupID;
	byte readerID;
};


#endif