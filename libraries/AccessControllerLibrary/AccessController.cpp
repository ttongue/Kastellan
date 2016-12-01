#include "AccessController.h"
#include "AccessPermissionRecord.h"
#include "Ethernet.h"
#include "RTClib.h"

#define SERIAL_PORT Serial3
//#define SERIAL_PORT Serial
#define RETRY_LIMIT 10

AccessPermissionRecord AccessController::permissionRecords[NUM_RECORDS];
//byte AccessController::facilityCodes[NUM_RECORDS];
//unsigned short AccessController::cardNumbers[NUM_RECORDS];
//byte AccessController::doorIDs[NUM_RECORDS];
//byte AccessController::zoneIDs[NUM_RECORDS];
int numberOfRecords=0;

ZoneRecord AccessController::zoneRecords[NUM_ZONES];

//byte AccessController::zoneIDRecords[NUM_ZONES];
//String AccessController::zoneDayRecords[NUM_ZONES];
//unsigned short AccessController::zoneStartTime[NUM_ZONES];
//unsigned short AccessController::zoneEndTime[NUM_ZONES];
int numberOfZones=0;

ReaderGroupRecord AccessController::readerGroupRecords[NUM_READERGROUPS];

int numberOfReaderGroups=0;

AccessController::AccessController() {
	//initStorage();
	//facilityCodes=new byte[NUM_RECORDS];
	//cardNumbers=new int[NUM_RECORDS];
	//doorIDs=new int[NUM_RECORDS];
	//zoneIDs=new int[NUM_RECORDS];
}



bool AccessController::validateCard(byte facilityCode, int cardNumber) {
	for (int i=0; i < NUM_RECORDS; i++) {
		if ((permissionRecords[i].cardNumber == cardNumber) && 
			(permissionRecords[i].facilityCode == facilityCode))
		{
			return true;
		}
	}
	return false;
}

bool AccessController::validateCard(byte facilityCode, int cardNumber, int doorID, RTC_Millis rtc) {
	for (int i=0; i < NUM_RECORDS; i++) {
		if ((permissionRecords[i].cardNumber == cardNumber) && 
			(permissionRecords[i].facilityCode == facilityCode)) 
		{
			if ((permissionRecords[i].doorID == 0) && (permissionRecords[i].zoneID == 0)) {
				// This is a universal key, it opens every door, regardless of date / time
				SERIAL_PORT.println("Universal card presented, card number: "+String(cardNumber,DEC)+" in facility "+String(facilityCode,DEC));
				return true;
			}
		    if (isDoorInReaderList(permissionRecords[i].doorID,doorID))
		    {
				int thisZone = (int)permissionRecords[i].zoneID;
				SERIAL_PORT.println("Checking door "+String(cardNumber,DEC)+" in facility "+String(facilityCode,DEC)+ " at door "+String(doorID,DEC)+ " in Zone "+String(thisZone,DEC));
				if (isZoneActive(thisZone, rtc)) {
					return true;
				}
			}
		}
	}
	return false;
}

//bool AccessController::validateCard(byte facilityCode, int cardNumber, int doorID, int zoneID) {
//	for (int i=0; i < NUM_RECORDS; i++) {
//		if ((permissionRecords[i].cardNumber == cardNumber) && 
//			(permissionRecords[i].facilityCode == facilityCode) && 
//			(permissionRecords[i].doorID == doorID) && 
//			(permissionRecords[i].zoneID == zoneID))
//		{
//			return true;
//		}
//	}
//	return false;
//}

bool AccessController::isZoneActive(int zoneID, RTC_Millis rtc) {
  DateTime now = rtc.now();
  int hhmm=now.hour()*100+now.minute();
  byte dayNum=(byte)now.dayOfWeek();
  if (dayNum==0) {
    dayNum=7;    // Sunday is the 7th day. Days run from 1 - 7.
  }
  for (int i=0; i<NUM_ZONES; i++) {
  	if (zoneRecords[i].zoneID == zoneID) {
  		// Find the right zoneID record
  		if (zoneRecords[i].zoneDays.indexOf(String(dayNum,DEC)) > -1) {
  			// Ok, this zoneID happens today, now lets check the time range
  			if ((zoneRecords[i].zoneStartTime < hhmm) && (zoneRecords[i].zoneEndTime > hhmm)) {
  				// We're on the right day of the week, and within the time window for this zone
  				return true;
  			} else {
  				return false;
  			}
  		} else {
  			return false;
  		}
  	}
  }
  return false;
}

bool AccessController::isDoorInReaderList(int readerGroupID, int doorID) {
	for (int i=0; i<NUM_READERGROUPS; i++) {
		if (readerGroupID == readerGroupRecords[i].readerGroupID) {
			if (readerGroupRecords[i].readerID == doorID) {
				return true;
			}
		}
	}
	return false;
}

void AccessController::clearStorage() {
	SERIAL_PORT.println("Begin Clearing Storage...");
	for (int i=0; i < NUM_RECORDS; i++) {
		permissionRecords[i].cardNumber=0;
		permissionRecords[i].facilityCode=0;
		permissionRecords[i].doorID=0;
		permissionRecords[i].zoneID=0;
	}
	SERIAL_PORT.println("Storage Cleared!");
}

void AccessController::clearZoneStorage() {
	SERIAL_PORT.println("Begin Clearing Zone Storage...");
	for (int i=0; i < NUM_ZONES; i++) {
		zoneRecords[i].zoneID=0;
		zoneRecords[i].zoneDays="";
		zoneRecords[i].zoneStartTime=0;
		zoneRecords[i].zoneEndTime=0;
		//zoneIDRecords[i]=0;
		//zoneDayRecords[i]="";
		//zoneStartTime[i]=0;
		//zoneEndTime[i]=0;
	}
	SERIAL_PORT.println("Storage Cleared!");
}

void AccessController::clearReaderGroupStorage() {
	SERIAL_PORT.println("Begin Clearing Reader Group Storage...");
	for (int i=0; i < NUM_READERGROUPS; i++) {
		readerGroupRecords[i].readerGroupID=0;
		readerGroupRecords[i].readerID=0;
	}
	SERIAL_PORT.println("Storage Cleared!");
}

void AccessController::initStorage() {
	clearStorage();
}

void AccessController::printStorage() {
	SERIAL_PORT.println("Stored Card Database:");
	for (int i=0; i<numberOfRecords; i++)
	{		
		SERIAL_PORT.print(permissionRecords[i].cardNumber);
		SERIAL_PORT.print("   ");
		SERIAL_PORT.print(permissionRecords[i].facilityCode);
		SERIAL_PORT.print("   ");
		SERIAL_PORT.print(permissionRecords[i].doorID);
		SERIAL_PORT.print("   ");
		SERIAL_PORT.print(permissionRecords[i].zoneID);
		SERIAL_PORT.println();		
	}
	SERIAL_PORT.println("Stored Zone Database:");
	for (int i=0; i<numberOfZones; i++)
	{		
		SERIAL_PORT.print(zoneRecords[i].zoneID);
		SERIAL_PORT.print("   ");
		SERIAL_PORT.print(zoneRecords[i].zoneDays);
		SERIAL_PORT.print("   ");
		SERIAL_PORT.print(zoneRecords[i].zoneStartTime);
		SERIAL_PORT.print("   ");
		SERIAL_PORT.print(zoneRecords[i].zoneEndTime);
		SERIAL_PORT.println();		
	}
}

bool AccessController::loadRecordsFromEEPROM(I2C_eeprom eeprom) {
	bool successfulLoadFromEEPROM=true;
    uint16_t totalBytes = ((uint8_t) eeprom.readByte(EEPROM_READERGROUP_OFFSET))*256+((uint8_t) eeprom.readByte(EEPROM_READERGROUP_OFFSET+1));
    SERIAL_PORT.println("Reader Groups: "+String(totalBytes,DEC)+" bytes available.");
    uint16_t checksum = ((uint8_t) eeprom.readByte(EEPROM_READERGROUP_OFFSET+totalBytes-2))*256+((uint8_t) eeprom.readByte(EEPROM_READERGROUP_OFFSET+totalBytes-1));
    uint8_t outRGData[totalBytes];
    memset(outRGData,0,totalBytes);
    eeprom.readBlock(EEPROM_READERGROUP_OFFSET,(uint8_t*)outRGData,totalBytes);
    uint16_t outChecksum=eeprom.generateChecksum((uint8_t*)outRGData,totalBytes-2);
    int offset=0;
    if (outChecksum == checksum) {
    	SERIAL_PORT.println("Checksum is valid. Stored Checksum: "+String(checksum,DEC)+ "   Computed: "+String(outChecksum,DEC));
    	numberOfReaderGroups=(totalBytes-4)/2;
    	for (int i=0; i< numberOfReaderGroups; i++) 
		{
			offset=2+i*2;
			readerGroupRecords[i].readerGroupID=outRGData[offset];
			readerGroupRecords[i].readerID = outRGData[offset+1];		
		}
    } else {
    	SERIAL_PORT.println("Checksum FAILED. Stored Checksum: "+String(checksum,DEC)+ "   Computed: "+String(outChecksum,DEC));
    	successfulLoadFromEEPROM=false;
    }

    totalBytes = ((uint8_t) eeprom.readByte(EEPROM_ZONERECORDS_OFFSET))*256+((uint8_t) eeprom.readByte(EEPROM_ZONERECORDS_OFFSET+1));
    SERIAL_PORT.println("Zone Records: "+String(totalBytes,DEC)+" bytes available.");
    checksum = ((uint8_t) eeprom.readByte(EEPROM_ZONERECORDS_OFFSET+totalBytes-2))*256+((uint8_t) eeprom.readByte(EEPROM_ZONERECORDS_OFFSET+totalBytes-1));
    uint8_t outZData[totalBytes];
    memset(outZData,0,totalBytes);
    eeprom.readBlock(EEPROM_ZONERECORDS_OFFSET,(uint8_t*)outZData,totalBytes);
    outChecksum=eeprom.generateChecksum((uint8_t*)outZData,totalBytes-2);
    if (outChecksum == checksum) {
    	SERIAL_PORT.println("Checksum is valid. Stored Checksum: "+String(checksum,DEC)+ "   Computed: "+String(outChecksum,DEC));
    	numberOfZones=(totalBytes-4)/12;
    	for (int i=0; i<numberOfZones; i++)
		{		
			offset=2+i*12;
			zoneRecords[i].zoneID=outZData[offset];
			zoneRecords[i].zoneDays="";
			for (int j=1; j<8; j++)
			{
				if (outZData[offset+j] == 1) {
					zoneRecords[i].zoneDays=zoneRecords[i].zoneDays+String(j,DEC);
				}
			}
			zoneRecords[i].zoneStartTime=outZData[8]*256+outZData[9];
			zoneRecords[i].zoneEndTime=outZData[10]*256+outZData[11];
		}
    } else {
    	SERIAL_PORT.println("Checksum FAILED. Stored Checksum: "+String(checksum,DEC)+ "   Computed: "+String(outChecksum,DEC));
        successfulLoadFromEEPROM=false;
    }

    totalBytes = ((uint8_t) eeprom.readByte(EEPROM_PERMRECORDS_OFFSET))*256+((uint8_t) eeprom.readByte(EEPROM_PERMRECORDS_OFFSET+1));
    SERIAL_PORT.println("Permission Records: "+String(totalBytes,DEC)+" bytes available.");
    checksum = ((uint8_t) eeprom.readByte(EEPROM_PERMRECORDS_OFFSET+totalBytes-2))*256+((uint8_t) eeprom.readByte(EEPROM_PERMRECORDS_OFFSET+totalBytes-1));
    uint8_t outPData[totalBytes];
    memset(outPData,0,totalBytes);
    eeprom.readBlock(EEPROM_PERMRECORDS_OFFSET,(uint8_t*)outPData,totalBytes);
    outChecksum=eeprom.generateChecksum((uint8_t*)outPData,totalBytes-2);
    if (outChecksum == checksum) {
    	SERIAL_PORT.println("Checksum is valid. Stored Checksum: "+String(checksum,DEC)+ "   Computed: "+String(outChecksum,DEC));
    	numberOfRecords=(totalBytes-4)/5;
    	for (int i=0; i<numberOfRecords; i++)
		{			
			offset=2+i*5;
			permissionRecords[i].cardNumber=outPData[offset]*256+outPData[offset+1];
			permissionRecords[i].facilityCode=outPData[offset+2];
			permissionRecords[i].doorID=outPData[offset+3];
			permissionRecords[i].zoneID=outPData[offset+4];
		}
    } else {
    	SERIAL_PORT.println("Checksum FAILED. Stored Checksum: "+String(checksum,DEC)+ "   Computed: "+String(outChecksum,DEC));
    	successfulLoadFromEEPROM=false;
    }
    return successfulLoadFromEEPROM;
}

void AccessController::writeRecordsToEEPROM(I2C_eeprom eeprom) {
	uint16_t totalBytes = 4+numberOfReaderGroups*2;
	SERIAL_PORT.println("Writing "+String(totalBytes,DEC)+ " bytes to EEPROM for Reader Groups");
	uint8_t outRGData[totalBytes];
	outRGData[0]=(uint8_t)(totalBytes >> 8);
	outRGData[1]=(uint8_t)(totalBytes & 0xFF);
	int offset=0;
    for (int i=0; i< numberOfReaderGroups; i++) 
	{
		offset=2+i*2;
		outRGData[offset]=(uint8_t)readerGroupRecords[i].readerGroupID;
		outRGData[offset+1]=(uint8_t)readerGroupRecords[i].readerID;		
	}
	uint16_t checksum = eeprom.generateChecksum((uint8_t*) outRGData,totalBytes-2);
	outRGData[totalBytes-2]=(uint8_t)(checksum >> 8);
	outRGData[totalBytes-1]=(uint8_t)(checksum & 0xFF);
	eeprom.writeBlock(EEPROM_READERGROUP_OFFSET,(uint8_t*) outRGData,totalBytes);
	if (eeprom.verifyBlock(EEPROM_READERGROUP_OFFSET,(uint8_t*) outRGData,totalBytes)) {
		SERIAL_PORT.println("Data to EEPROM Verified.");
	} else {
		SERIAL_PORT.println("Data to EEPROM CORRUPTED!");
	}

	totalBytes = 4+numberOfZones*12;
	SERIAL_PORT.println("Writing "+String(totalBytes,DEC)+ " bytes to EEPROM for Zone Records");
	uint8_t outZData[totalBytes];
	outZData[0]=(uint8_t)(totalBytes >> 8);
	outZData[1]=(uint8_t)(totalBytes & 0xFF);
	offset=0;
	for (int i=0; i<numberOfZones; i++)
	{		
		offset=2+i*12;
		outZData[offset]=(uint8_t)zoneRecords[i].zoneID;
		for (int j=1; j<8; j++)
		{
			if (zoneRecords[i].zoneDays.indexOf(String(j,DEC)) > -1) {
				outZData[offset+j]=(uint8_t)1;
			} else {
				outZData[offset+j]=(uint8_t)0;
			}
		}
	    outZData[8]=(uint8_t)(((uint16_t)zoneRecords[i].zoneStartTime) >> 8);
	    outZData[9]=(uint8_t)(((uint16_t)zoneRecords[i].zoneStartTime) & 0xFF);
	    outZData[10]=(uint8_t)(((uint16_t)zoneRecords[i].zoneEndTime) >> 8);
	    outZData[11]=(uint8_t)(((uint16_t)zoneRecords[i].zoneEndTime) & 0xFF);
	}
	checksum = eeprom.generateChecksum((uint8_t*) outZData,totalBytes-2);
	outZData[totalBytes-2]=(uint8_t)(checksum >> 8);
	outZData[totalBytes-1]=(uint8_t)(checksum & 0xFF);
    eeprom.writeBlock(EEPROM_ZONERECORDS_OFFSET,(uint8_t*) outZData, totalBytes);
    if (eeprom.verifyBlock(EEPROM_ZONERECORDS_OFFSET,(uint8_t*) outZData,totalBytes)) {
		SERIAL_PORT.println("Data to EEPROM Verified.");
	} else {
		SERIAL_PORT.println("Data to EEPROM CORRUPTED!");
	}

    totalBytes = 4+numberOfRecords*5;
    SERIAL_PORT.println("Writing "+String(totalBytes,DEC)+ " bytes to EEPROM for Permission Records");
    uint8_t outPData[totalBytes];
    outPData[0]=(uint8_t)(totalBytes >> 8);
	outPData[1]=(uint8_t)(totalBytes & 0xFF);
	offset=0;
	for (int i=0; i<numberOfRecords; i++)
	{		
		offset=2+i*5;
		outPData[offset]=(uint8_t)(((uint16_t)permissionRecords[i].cardNumber) >> 8);
		outPData[offset+1]=(uint8_t)(((uint16_t)permissionRecords[i].cardNumber) & 0xFF);
		outPData[offset+2]=(uint8_t)permissionRecords[i].facilityCode;
		outPData[offset+3]=(uint8_t)permissionRecords[i].doorID;
		outPData[offset+4]=(uint8_t)permissionRecords[i].zoneID;
	}
	checksum = eeprom.generateChecksum((uint8_t*) outPData,totalBytes-2);
	outPData[totalBytes-2]=(uint8_t)(checksum >> 8);
	outPData[totalBytes-1]=(uint8_t)(checksum & 0xFF);
	eeprom.writeBlock(EEPROM_PERMRECORDS_OFFSET,(uint8_t*) outPData, totalBytes);
	if (eeprom.verifyBlock(EEPROM_PERMRECORDS_OFFSET,(uint8_t*) outPData,totalBytes)) {
		SERIAL_PORT.println("Data to EEPROM Verified.");
	} else {
		SERIAL_PORT.println("Data to EEPROM CORRUPTED!");
	}

    SERIAL_PORT.println("Completed EEPROM Update!");
}

void AccessController::printStorage(EthernetClient myClient) {
	myClient.println("<table border=0><tr valign=top><td>");
	myClient.println("<table cellpadding=0 cellspacing=0 border=1>");
	myClient.println("<tr><th colspan=4>Stored Card Database</th></tr>");
	myClient.println("<tr><th>Card #</th><th>Facility Code</th><th>Door ID</th><th>Zone ID</th></tr>");
	for (int i=0; i<numberOfRecords; i++)
	{
		myClient.print("<tr><td align=right>");		
		myClient.print(permissionRecords[i].cardNumber);
		myClient.print("</td><td align=right>");
		myClient.print(permissionRecords[i].facilityCode);
		myClient.print("</td><td align=right>");
		myClient.print(permissionRecords[i].doorID);
		myClient.print("</td><td align=right>");
		myClient.print(permissionRecords[i].zoneID);
		myClient.println("</td></tr>");		
	}
	myClient.println("</table><br><br></td><td>");
	myClient.println("<table cellpadding=0 cellspacing=0 border=1>");
	myClient.println("<tr><th colspan=4>Stored Zone Database</th></tr>");
	myClient.println("<tr><th>Zone #</th><th>Zone Days</th><th>Start Time</th><th>End Time</th></tr>");
	for (int i=0; i<numberOfZones; i++)
	{		
		myClient.print("<tr><td align=right>");
		myClient.print(zoneRecords[i].zoneID);
		myClient.print("   </td><td align=right>");
		myClient.print(zoneRecords[i].zoneDays);
		myClient.print("   </td><td align=right>");
		myClient.print(zoneRecords[i].zoneStartTime);
		myClient.print("   </td><td align=right>");
		myClient.print(zoneRecords[i].zoneEndTime);
		myClient.println("</td></tr>");		
	}
	myClient.println("</table><br><br></td><td>");
	myClient.println("<table cellpadding=0 cellspacing=0 border=1>");
	myClient.println("<tr><th colspan=2>Stored Reader Groups</th></tr>");
	myClient.println("<tr><th>Group #</th><th>Reader ID</th></tr>");
	for (int i=0; i< numberOfReaderGroups; i++) 
	{
		myClient.print("<tr><td align=right>");
		myClient.print(readerGroupRecords[i].readerGroupID);
		myClient.print("</td><td align=right>");
		myClient.print(readerGroupRecords[i].readerID);
		myClient.print("</td></tr>");		
	}
	myClient.println("</table><br><br></td></tr></table>");
}


bool AccessController::loadMemberRecordsFromServer(EthernetClient client, char *server, char *path) {
	clearStorage();
	client.connect(server,80);
	while (!client.connected()) {
		SERIAL_PORT.println("Waiting for Port 80 connection...");
		delay(1000);
	}
	SERIAL_PORT.println("Connected to Port 80");
		client.print("GET ");
		client.print(path);
		client.println(" HTTP/1.0");
		client.print("Host: ");
		client.println(server);
		client.println();

//		while (client.connected()) {
//			if (client.available()) {
//				char c=client.read();
//				SERIAL_PORT.print(c);
//			}
//		}
		bool foundStart=false;
		char startLine[ ]="---begin---";
		int i=0;
		while (!client.connected()) {
			// we need to wait for the connection to be acknowledged
			SERIAL_PORT.print(".");
			delay(50);
		}
		int tryAgainCounter=0;
		do {
			if (client.available()) {
				char c=client.read();
				SERIAL_PORT.print(c);
				if (c==startLine[i]) {
					i++;
				} else {
					i=0;
				}
				if (i==11) {
					foundStart=true;
					SERIAL_PORT.println("Found beginning of card list...");
			 	}
			} else {
				tryAgainCounter++;
				delay(50);
				if (tryAgainCounter > RETRY_LIMIT) {
					SERIAL_PORT.println("Retry limit exceeded!");
					return false;
				}
			}
		} while (!foundStart);

		int col=0;
		bool numberLatched=false;
		String inNumber="";
		int outNumber=0;
		int lineNum=0;
		while (client.connected()) {
			if (client.available()) {
				char c=client.read();
				if (isDigit(c)) {
					inNumber+=c;
				} else {
					if (inNumber.length() > 0) {
						outNumber=inNumber.toInt();
											
						numberLatched=true;
						switch (col) {
							case 0:
								SERIAL_PORT.print(lineNum);
								SERIAL_PORT.print(": ");
								permissionRecords[lineNum].cardNumber=outNumber;
								//cardNumbers[lineNum]=outNumber;
								col++;
								break;
							case 1:
								permissionRecords[lineNum].facilityCode=outNumber;
								//facilityCodes[lineNum]=outNumber;
								col++;
								break;
							case 2:
								permissionRecords[lineNum].doorID=outNumber;
								//doorIDs[lineNum]=outNumber;
								col++;
								break;
							case 3:
								permissionRecords[lineNum].zoneID=outNumber;
								//zoneIDs[lineNum]=outNumber;
								col++;
								break;
							default:
								// Huh, this should be ignored;
								break;
						}
						SERIAL_PORT.print(" ");
						SERIAL_PORT.print(outNumber);	
						inNumber="";
					
						if ((c=='\n') && (numberLatched==true)) {
							lineNum++;
							numberOfRecords=lineNum;
							col=0;
							numberLatched=false;
							SERIAL_PORT.println();
						}
					}
				}
				//SERIAL_PORT.print(c);
			}
		}
		SERIAL_PORT.println();
		SERIAL_PORT.println("Disconnecting");
	    return true;

}


bool AccessController::loadZoneRecordsFromServer(EthernetClient client, char *server, char *path) {
	clearZoneStorage();
	while (!client.connect(server,80)) {
		SERIAL_PORT.println("Retrying Port 80 connection...");
		delay(1000);
	}
	SERIAL_PORT.println("Connected to Port 80 -- Retrieving Zone Codes");
	client.print("GET ");
	client.print(path);
	client.println(" HTTP/1.0");
	client.print("Host: ");
	client.println(server);
	client.println();
	SERIAL_PORT.println(path);

//		while (client.connected()) {
//			if (client.available()) {
//				char c=client.read();
//				SERIAL_PORT.print(c);
//			}
//		}
		bool foundStart=false;
		char startLine[ ]="---begin-zone---";
		int i=0;
		while ((!client.connected()) || (!client.available())) {
			// we need to wait for the connection to be acknowledged
			SERIAL_PORT.print(".");
			delay(50);
		}
		int tryAgainCounter=0;
		do {
			if (client.available()) {
				char c=client.read();
				//SERIAL_PORT.print(c);
				if (c==startLine[i]) {
					i++;
				} else {
					i=0;
				}
				if (i==14) {
					foundStart=true;
					SERIAL_PORT.println("Found beginning of zone list...");
				}
			} else {
				tryAgainCounter++;
				delay(50);
				if (tryAgainCounter > RETRY_LIMIT) {
					SERIAL_PORT.println("Retry limit exceeded!");
					return false;
				}
			}
		} while (!foundStart);

		int col=0;
		bool numberLatched=false;
		String inNumber="";
		int outNumber=0;
		int lineNum=0;
		while (client.connected()) {
			if (client.available()) {
				char c=client.read();
				if (isDigit(c)) {
					inNumber+=c;
				} else {
					if (inNumber.length() > 0) {
						outNumber=inNumber.toInt();
											
						numberLatched=true;
						switch (col) {
							case 0:
								SERIAL_PORT.print(lineNum);
								SERIAL_PORT.print(": ");
								zoneRecords[lineNum].zoneID=outNumber;
								//zoneIDRecords[lineNum]=outNumber;
								col++;
								break;
							case 1:
							    zoneRecords[lineNum].zoneDays=String(outNumber);
								//zoneDayRecords[lineNum]=String(outNumber);
								col++;
								break;
							case 2:
								zoneRecords[lineNum].zoneStartTime=outNumber;
								//zoneStartTime[lineNum]=outNumber;
								col++;
								break;
							case 3:
								zoneRecords[lineNum].zoneEndTime=outNumber;
								//zoneEndTime[lineNum]=outNumber;
								col++;
								break;
							default:
								// Huh, this should be ignored;
								break;
						}
						SERIAL_PORT.print(" ");
						SERIAL_PORT.print(outNumber);	
						inNumber="";
					
						if ((c=='\n') && (numberLatched==true)) {
							lineNum++;
							numberOfZones=lineNum;
							col=0;
							numberLatched=false;
							SERIAL_PORT.println();
						}
					}
				}
				//SERIAL_PORT.print(c);
			}
		}
		SERIAL_PORT.println();
		SERIAL_PORT.println("Disconnecting");
		return true;

}


bool AccessController::loadReaderGroupRecordsFromServer(EthernetClient client, char *server, char *path) {
	clearReaderGroupStorage();
	while (!client.connect(server,80)) {
		SERIAL_PORT.println("Retrying Port 80 connection...");
		delay(1000);
	}
	SERIAL_PORT.println("Connected to Port 80 -- Retrieving Reader Group Codes");
		client.print("GET ");
		client.print(path);
		client.println(" HTTP/1.0");
		client.print("Host: ");
		client.println(server);
		client.println();

//		while (client.connected()) {
//			if (client.available()) {
//				char c=client.read();
//				SERIAL_PORT.print(c);
//			}
//		}
		bool foundStart=false;
		char startLine[ ]="---begin-readerlist---";
		int i=0;
		while (!client.connected()) {
			// we need to wait for the connection to be acknowledged
			SERIAL_PORT.print(".");
			delay(50);
		}
		int tryAgainCounter=0;
		do {
			if (client.available()) {
				char c=client.read();
				if (c==startLine[i]) {
					i++;
				} else {
					i=0;
				}
				if (i==20) {
					foundStart=true;
					SERIAL_PORT.println("Found beginning of reader group list...");
				}
			} else {
				tryAgainCounter++;
				delay(50);
				if (tryAgainCounter > RETRY_LIMIT) {
					SERIAL_PORT.println("Retry limit exceeded!");
					return false;
				}
			}
		} while (!foundStart);

		int col=0;
		bool numberLatched=false;
		String inNumber="";
		int outNumber=0;
		int lineNum=0;
		while (client.connected()) {
			if (client.available()) {
				char c=client.read();
				if (isDigit(c)) {
					inNumber+=c;
				} else {
					if (inNumber.length() > 0) {
						outNumber=inNumber.toInt();
											
						numberLatched=true;
						switch (col) {
							case 0:
								SERIAL_PORT.print(lineNum);
								SERIAL_PORT.print(": ");
								readerGroupRecords[lineNum].readerGroupID=outNumber;
								col++;
								break;
							case 1:
							    readerGroupRecords[lineNum].readerID=outNumber;
								col++;
								break;
							default:
								break;
						}
						SERIAL_PORT.print(" ");
						SERIAL_PORT.print(outNumber);	
						inNumber="";
					
						if ((c=='\n') && (numberLatched==true)) {
							lineNum++;
							numberOfReaderGroups=lineNum;
							col=0;
							numberLatched=false;
							SERIAL_PORT.println();
						}
					}
				}
				//SERIAL_PORT.print(c);
			}
		}
		SERIAL_PORT.println();
		SERIAL_PORT.println("Disconnecting");
}


String AccessController::getSecondsSince1970FromServer(EthernetClient client, char *server, char *path) {
	int tryAgainCounter=0;
	while (!client.connect(server,80)) {
		SERIAL_PORT.println("Retrying Port 80 connection...");
		delay(1000);
		tryAgainCounter++;
		if (tryAgainCounter>RETRY_LIMIT) {
			return "0";
		}
	}
	SERIAL_PORT.println("Connected to Port 80 -- Retrieving Epoch String");
    client.print("GET ");
	client.print(path);
	client.println(" HTTP/1.0");
	client.print("Host: ");
	client.println(server);
	client.println();
    bool foundStart=false;
    char startLine[ ]="|||";
	int i=0;
	while (!client.connected()) {
			// we need to wait for the connection to be acknowledged
			SERIAL_PORT.print(".");
			delay(50);
	}
	tryAgainCounter=0;
	do {
		if (client.available()) {
			char c=client.read();
			SERIAL_PORT.print(c);
			if (c==startLine[i]) {
					i++;
			} else {
					i=0;
			}
			if (i==3) {
					foundStart=true;
					SERIAL_PORT.println("Found beginning of epoch string...");
			}
		} else {
			tryAgainCounter++;
			delay(50);
			if (tryAgainCounter > RETRY_LIMIT) {
				SERIAL_PORT.println("Retry limit exceeded! Using last COMPILED TIME.");
				return "0";
			}
		}
	} while (!foundStart);
	String outString="";
	while (client.connected()) {
			if (client.available()) {
				char c=client.read();
				if (c!='|') {
					outString +=c;
				} else {
					return outString;
				}
			}
	}
    return "0";
}

void AccessController::loginRFID(EthernetClient client, char *server, char *path, int rfid) {
	int tryAgainCounter=0;
	while (!client.connect(server,80)) {
		SERIAL_PORT.println("Retrying Port 80 connection...");
		delay(1000);
		tryAgainCounter++;
		if (tryAgainCounter>RETRY_LIMIT) {
			client.stop();
			return;
		}
	}
	SERIAL_PORT.println("Connected to Port 80 -- Retrieving Epoch String");
    client.print("GET ");
    client.print(path);
    client.print("?rfid=");
    client.print(rfid);
	client.println(" HTTP/1.0");
	client.print("Host: ");
	client.println(server);
	client.println();
	tryAgainCounter=0;
	int conWaitLimit=RETRY_LIMIT;
	conWaitLimit=conWaitLimit*100;
	while (!client.connected()) {
			// we need to wait for the connection to be acknowledged
			SERIAL_PORT.print(".");
			delay(50);
			tryAgainCounter++;
			if (tryAgainCounter>conWaitLimit) {
				client.stop();
				return;
			}
	}
	tryAgainCounter=0;
	do {
		if (client.available()) {
			char c=client.read();
			//SERIAL_PORT.print(c);
		} 
		delay(10);
		tryAgainCounter++;
		if (tryAgainCounter > conWaitLimit) {
			client.stop();
		}
	} while (client.connected());
	String outString="";
	client.stop();
    return;
}
