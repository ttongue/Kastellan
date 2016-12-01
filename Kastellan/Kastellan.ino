#include <Wiegand.h>
#include <AccessController.h>
#include <Ethernet.h>
#include <Wire.h>
#include "RTClib.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/eeprom.h"
#include "I2C_eeprom.h"

//#define SERIAL_PORT Serial3
#define SERIAL_PORT Serial3
#define LED D4_LED

const int timeZonesFromGMT=-4;
RTC_Millis rtc;
AccessController AC; // = new AccessController::AccessController();
EthernetClient client;
EthernetServer server(80);
EthernetClient incomingClient;
byte mac[]= { 0x00,0x1A,0xB6,0x02,0xBB,0xAF };
byte gateToDoorMap[]= { 6,2,3,4,5,1 };
int doorToPinMap[] = { PC_7, PD_3, PE_5, PC_6, PC_5, PC_4 };
//char serverName[] = "systems.techvalleycenterofgravity.com";
//char serverName[] = "192.168.11.250";
char serverName[] = "198.199.88.110";
char loggingServerName[] = "198.199.88.110";
char recordsPath[] = "/kastellan/cardlist.txt";
char zonePath[] = "/kastellan/zones.txt";
char readerGroupPath[] = "/kastellan/readerlist.txt";
char loginPath[] = "/cardsystem/rfidlog.py";


int doorOpenTime_ms=7000;
String lastRFID="000000";
byte lastDoorNum=0;
byte lastFacilityCode=0;
int lastCardNum=0;
String lastDateTimeString="-----------";
String lastDatabaseUpdateDateTimeString="-------------------";
WIEGAND wg;
I2C_eeprom ee(0x50,1024000);
//00:1A:B6:02:BB:AF
#define EEPROM_WP_PIN PE_4
#define EEPROM_PWR_PIN PE_0

bool LOAD_FROM_WEB=false;

void setup()
{
  SERIAL_PORT.begin(115200);
  SERIAL_PORT.println("Serial Setup Complete-----------");
  pinMode(EEPROM_WP_PIN,OUTPUT);
  digitalWrite(EEPROM_WP_PIN,1);
  pinMode(EEPROM_PWR_PIN,OUTPUT);
  digitalWrite(EEPROM_PWR_PIN,1);  // Turn on the EEPROM.
  ee.begin();
  confirmEEPROMPresent();
  wg.begin();
  pinMode(LED, OUTPUT);
        
  pinMode(D1_LED, OUTPUT);
  pinMode(D2_LED, OUTPUT);
  pinMode(PUSH1, INPUT_PULLUP); // released = HIGH, pressed = LOW
  pinMode(PUSH2, INPUT_PULLUP);
  pinMode(PC_7,OUTPUT);
  pinMode(PD_3,OUTPUT);
  pinMode(PE_5,OUTPUT);
  pinMode(PC_6,OUTPUT);
  pinMode(PC_5,OUTPUT);
  pinMode(PC_4,OUTPUT);
  SERIAL_PORT.println("Wiegand Initialized-------------");
  initializeEthernet();
  setRTCFromServer();
  //if (AC.loadRecordsFromEEPROM(ee)) {
  //  AC.printStorage();
  //} else {
    // The EEPROM does not have the database in it for some reason,
    // load from the server
    loadAllDatabases();
  //}
  SERIAL_PORT.print("Program last uploaded on ");
  SERIAL_PORT.print(F(__DATE__));
  SERIAL_PORT.print(" ");
  SERIAL_PORT.println(F(__TIME__));

}


void confirmEEPROMPresent() {
  int size = ee.determineSize();
  if (size > 0)
  {
    SERIAL_PORT.print("EEPROM SIZE: ");
    SERIAL_PORT.print(size);
    SERIAL_PORT.println(" KB");
  } 
  else if (size == 0)
  {
    SERIAL_PORT.println("WARNING: Can't determine eeprom size");
  }
  else
  {
    SERIAL_PORT.println("ERROR: Can't find eeprom!");
  }
}


void setRTCFromServer() {
  String epochString = AC.getSecondsSince1970FromServer(client,serverName,"/kastellan/timeSince1970.py");
  if (epochString == "0") {
    rtc.begin(DateTime(F(__DATE__), F(__TIME__)));
  } else {
    SERIAL_PORT.println(epochString);
    rtc.begin(DateTime((uint32_t)(epochString.toFloat()+(timeZonesFromGMT*3600))));
  }
  SERIAL_PORT.print("RTC Clock set to: ");
  SERIAL_PORT.println(getTimeString());
  SERIAL_PORT.print("Today is ");
  SERIAL_PORT.print(getDateString());
  SERIAL_PORT.print(" and the day number is ");
  SERIAL_PORT.println(getDayNumber());
}


void loadAllDatabases() {
  bool loaded=false;
  do {
    loaded=AC.loadMemberRecordsFromServer(client,serverName,recordsPath);
  } while (!loaded);
  loaded=false;
  do {
    loaded=AC.loadZoneRecordsFromServer(client,serverName,zonePath);
  } while (!loaded);
  loaded=false;
  do {
    loaded=AC.loadReaderGroupRecordsFromServer(client,serverName,readerGroupPath);
  } while (!loaded);
  digitalWrite(EEPROM_WP_PIN,0);    // Set the Write-enable bit to write data
  AC.writeRecordsToEEPROM(ee);
  digitalWrite(EEPROM_WP_PIN,1);    // Set the Write-enable bit to protect data
  //ee.setBlock(1024,0,128); // Clear out some EEPROM snot;
  //AC.printStorage();
  //AC.loadRecordsFromEEPROM(ee);
  AC.printStorage();
  lastDatabaseUpdateDateTimeString=getDateString()+" "+getTimeString();
}

String getTimeString() {
  DateTime now = rtc.now();
  String output=String(now.hour(),DEC)+":"+String(now.minute(),DEC)+":"+String(now.second(),DEC);
  return output;
}

String getDateString() {
  DateTime now = rtc.now();
  String output=String(now.month(),DEC)+"/"+String(now.day(),DEC)+"/"+String(now.year(),DEC);
  return output;
}

int getDayNumber() {
  DateTime now = rtc.now();
  byte dayNum=(byte)now.dayOfWeek();
  if (dayNum==0) {
    dayNum=7;    // Sunday is the 7th day. Days run from 1 - 7.
  }
  return dayNum;
}

void loop()
{
  if (wg.available())
  {
    handleWeigand();
  // put your main code here, to run repeatedly:
  }
  incomingClient=server.available();
  if (incomingClient) {
    handleServerRequest(incomingClient);
  }
}

void handleWeigand() {
    lastDateTimeString=getDateString()+"  "+getTimeString();
    lastRFID=String(wg.getCode(),HEX);
    lastDoorNum=gateToDoorMap[wg.getGateActive()-1];
    SERIAL_PORT.print(lastDateTimeString);
    SERIAL_PORT.print(": Card Code Received at door ");
    SERIAL_PORT.print(lastDoorNum);
    SERIAL_PORT.print(" - ");
    //SERIAL_PORT.print(lastRFID);
    
    // Extract the Facility Code
    String facString="0x00"+lastRFID.substring(0,2);
    char buf[7];
    facString.toCharArray(buf,7);
    lastFacilityCode=(byte)strtoul(buf,0,16);
    SERIAL_PORT.print("  Facility: ");
    SERIAL_PORT.print(lastFacilityCode);
    
    // Extract the card number
    String cardString = "0x"+lastRFID.substring(2,6);
    cardString.toCharArray(buf,7);
    lastCardNum = (int)strtoul(buf,0,16);
    SERIAL_PORT.print("   Card#: ");
    SERIAL_PORT.print(lastCardNum);
    
    if (AC.validateCard(lastFacilityCode,lastCardNum, (int)lastDoorNum, rtc)) {
      SERIAL_PORT.println("   Access Granted!");
      wg.signalCard(true);
      openDoor((int)lastDoorNum);
      AC.loginRFID(client,loggingServerName,loginPath,lastCardNum); 
    } else {
      SERIAL_PORT.println("   ACCESS DENIED!");
      wg.signalCard(false);
    }
    delay(1000);
}

void handleServerRequest(EthernetClient myClient) {
    SERIAL_PORT.print("new client on port ");           // print a message out the serial port
    SERIAL_PORT.println(myClient.port());
    String currentLine = "";                // make a String to hold incoming data from the client
    boolean newConnection = true;     // flag for new connections
    unsigned long connectionActiveTimer;  // will hold the connection start time

    while (myClient.connected()) {       // loop while the client's connected
      if (newConnection){                 // it's a new connection, so
        connectionActiveTimer = millis(); // log when the connection started
        newConnection = false;          // not a new connection anymore
      }
      if ((!newConnection) && (connectionActiveTimer + 1000 < millis())){ 
        // if this while loop is still active 1000ms after a web client connected, something is wrong
        break;  // leave the while loop, something bad happened
      }


      if (myClient.available()) {             // if there's bytes to read from the client,    
        char c = myClient.read();             // read a byte, then
        // This lockup is because the recv function is blocking.
        //SERIAL_PORT.print(c);
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {  
            break;         
          } 
          else {      // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }     
        else if (c != '\r') {    // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
        if (currentLine.endsWith("GET / ")) {
          printIndex(myClient);
        }
        if (currentLine.endsWith("GET /cfg ")) {
          printConfig(myClient);
        }
        if (currentLine.endsWith("GET /index.html ")) {
          printIndex(myClient);
        }
        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /LED1_H")) {
          digitalWrite(D1_LED, HIGH);
          printConfig(myClient);
        }         
        if (currentLine.endsWith("GET /LED1_L")) {
          digitalWrite(D1_LED, LOW);
          printConfig(myClient);
        }     
        if (currentLine.endsWith("GET /LED2_H")) {
          digitalWrite(D2_LED, HIGH);
          printConfig(myClient);
        }       
        if (currentLine.endsWith("GET /LED2_L")) {
          digitalWrite(D2_LED, LOW);
          printConfig(myClient);
        }
        if ((currentLine.indexOf("GET /tst?try=") > -1) && (currentLine.endsWith("HTTP"))) {
          handleTestRequest(myClient,currentLine);
        }
        if (currentLine.endsWith("GET /door1")) {
          openDoor((int)1);
          printIndex(myClient);
        }
        if (currentLine.endsWith("GET /door2")) {
          openDoor((int)2);
          printIndex(myClient);
        }
        if (currentLine.endsWith("GET /door3")) {
          openDoor((int)3);
          printIndex(myClient);
        }
        if (currentLine.endsWith("GET /door4")) {
          openDoor((int)4);
          printIndex(myClient);
        }
        if (currentLine.endsWith("GET /door5")) {
          openDoor((int)5);
          printIndex(myClient);
        }
        if (currentLine.endsWith("GET /door6")) {
          openDoor((int)6);
          printIndex(myClient);
        }
        if (currentLine.endsWith("GET /reload")) {
          setRTCFromServer();
          loadAllDatabases();       
          printConfig(myClient);
        }
      }
    }
    // close the connection:
    myClient.stop();
}

void initializeEthernet() {
  IPAddress ip = IPAddress(192,168,13,22);
  IPAddress dns = IPAddress(192,168,11,250);
  IPAddress gw = IPAddress(192,168,13,1);
  IPAddress mask = IPAddress(255,255,255,0);
  Ethernet.begin(0,ip,dns,gw);
  //  SERIAL_PORT.println("Failed fixed IP setup, trying DHCP");
  //  if (Ethernet.begin(mac) == 0) {
  //    SERIAL_PORT.println("Failed to configure Ethernet using DHCP");
  //  } else {
  //    SERIAL_PORT.println("Ethernet initialized using DHCP");
  //  }
  //}
  SERIAL_PORT.println("IP Address Information:");  
  ip = Ethernet.localIP();
  SERIAL_PORT.print("IP Address:\t");
  SERIAL_PORT.println(ip);

      // print your MAC address:

  IPAddress subnet = Ethernet.subnetMask();
  SERIAL_PORT.print("NetMask:\t");
  SERIAL_PORT.println(subnet);

    // print your gateway address:
  IPAddress gateway = Ethernet.gatewayIP();
  SERIAL_PORT.print("Gateway:\t");
  SERIAL_PORT.println(gateway);

    // print your gateway address:
  dns = Ethernet.dnsServerIP();
  SERIAL_PORT.print("DNS:\t\t");
  SERIAL_PORT.println(dns);
  
  server.begin();
   
}





void printIndex(EthernetClient myClient)
{
  printHeader(myClient);
  myClient.println("<p>More information on Kastellan is available from <a href=http://www.tungadyn.com/>Tunga Dynamics</a>.");
  myClient.println("Please consult the product manual for details on ");
  myClient.println("configuration and monitoring.</p></br></br>");
  printFooter(myClient);
}

void printFooter(EthernetClient myClient)
{
  myClient.println("<hr><table border=0 cellpadding=0 cellspacing=0 width=100%><tr><td align=left>&copy;2015 Tunga Dynamics Corporation - All Rights Reserved.<br>");
  myClient.print("Firmware last uploaded on ");
  myClient.print(F(__DATE__));
  myClient.print(" ");
  myClient.println(F(__TIME__));
  myClient.println("<br>Database last updated on "+lastDatabaseUpdateDateTimeString);
  myClient.println("</td>");
  myClient.println("<td align=right>"+getDateString()+"  "+getTimeString()+"</td></tr></table");
  myClient.println("</body></html>");

  myClient.println();
}

void printHeader(EthernetClient myClient)
{
  myClient.println("HTTP/1.1 200 OK");

  myClient.println("Content-type:text/html");
  myClient.println();
  myClient.println("<html><head><title>Kastellan</title></head><body align=center>");
  myClient.println("<h1 align=center><font color=\"blue\">Kastellan</font></h1>");
}

void printConfig(EthernetClient myClient)
{
  printHeader(myClient);
  
  myClient.println("<table border=0><tr valign=top><td>");
  
  myClient.println("<table border=2 cellpadding=0 cellspacing=0><tr><td colspan=4><center>Last Access Record</center></td></tr>");
  myClient.println("<tr><th>Date/Time</th><th>Door</th><th>Facility</th><th>Card #</th></tr>");
  myClient.println("<td align=right>"+lastDateTimeString+"</td>");
  myClient.println("<td align=right>"+String(lastDoorNum,DEC)+"</td>");
  myClient.println("<td align=right>"+String(lastFacilityCode,DEC)+"</td>");
  myClient.println("<td align=right>"+String(lastCardNum,DEC)+"</td></tr></table>");
  
  myClient.println("</td><td>");
  
  myClient.println("<table border=1 cellpadding=3 cellspacing=0>");
  myClient.println("<tr><th colspan=9> Gate to Door Mapping</th></tr>");
  myClient.println("<tr><td>Gate #:</td>");
  for (int i=0; i< sizeof(gateToDoorMap); i++) 
  {
    myClient.print("<td>"+String(i,DEC)+"</td>");
  }
  myClient.println("</tr><tr><td>Door #:</td>");
  for (int i=0; i< sizeof(gateToDoorMap); i++) 
  {
    myClient.print("<td>"+String(gateToDoorMap[i],DEC)+"</td>");
  }
  myClient.println("</tr></table></td></tr></table><br><Br>");
  
  AC.printStorage(myClient);
  
  myClient.println("<table border=2 cellpadding=0 cellspacing=0><tr><th>Hardware Test Utilities</th><tr><tr><td>");
  myClient.print("LED 1<button onclick=\"location.href='/LED1_H'\">HIGH</button>");
  myClient.println(" <button onclick=\"location.href='/LED1_L'\">LOW</button><br>");
  myClient.print("LED 2 <button onclick=\"location.href='/LED2_H'\">HIGH</button>");
  myClient.println(" <button onclick=\"location.href='/LED2_L'\">LOW</button><br><br>");
  myClient.println("</td></tr></table>");
  myClient.println("<a href=\"/cfg\" >refresh</a> <br>");
  myClient.println("<a href=\"/\" >home</a> <br>");
  printFooter(myClient);
  myClient.println();
  //AC.loadRecordsFromEEPROM(ee);
}

void handleTestRequest(EthernetClient myClient, String currentline) 
{
  // Test String needs to be Card#.Facility#.Gate#
  // Gate# is translated into the door number, since each instance of
  // the controller could have different door numbers mapped to each gate.
  // For example, the second door controller could have doors 9 - 12 on it
  // with gate 0 = door 9, gate 1 = door 10, etc. Gates are from 0 - 7.
  int hEnd=currentline.indexOf("HTTP")-1;
  String tryString=currentline.substring(13,hEnd);
  int firstPointIndex=tryString.indexOf(".");
  int secondPointIndex=tryString.indexOf(".",firstPointIndex+1);
  if ((secondPointIndex < firstPointIndex) || (firstPointIndex < 1)) {
    printHeader(myClient);
    myClient.println("Incorrect test string. RTFM.");
    printFooter(myClient);
    return;
  }
  int cardNum=tryString.substring(0,firstPointIndex).toInt();
  byte facNum =(byte) tryString.substring(firstPointIndex+1,secondPointIndex).toInt();
  byte gateNum=tryString.substring(secondPointIndex+1).toInt();
  byte doorNum = (byte) gateToDoorMap[gateNum];
  
  printHeader(myClient);
  myClient.println("Test String: "+tryString+"<br>");
  myClient.println("Card Number: "+String(cardNum,DEC)+"<br>Facility Code: ");
  myClient.println(String(facNum,DEC)+"<br> Gate Number: "+String(gateNum,DEC));
  myClient.println("<br> Door Number: "+String(doorNum,DEC));
  myClient.println("<br>");
  if (AC.validateCard(facNum,cardNum, doorNum, rtc)) {
      myClient.println("   Access Granted!");
  } else {
      myClient.println("   ACCESS DENIED!");
  }
 
  printFooter(myClient);
}

void openDoor(int doorNum) {
  digitalWrite(doorToPinMap[doorNum-1],HIGH);
  delay(doorOpenTime_ms);
  digitalWrite(doorToPinMap[doorNum-1],LOW);
//String P3="";
//  String P5="00";
//  uint8_t bitvalue=1;
//  if (doorNum>1) {
//    bitvalue=bitvalue << doorNum-1;   
//  }
//  char tmpP3[2];
//  sprintf(tmpP3,"%02x",bitvalue);
//  String request=String(relayControllerPathRoot)+"01="+tmpP3+"&02=00";
//  String usernamepass=String(username)+":"+String(password);
//  char tmpUsPs[usernamepass.length()+1];
//  int inputLen = sizeof(tmpUsPs);
//  usernamepass.toCharArray(tmpUsPs,inputLen);
//  SERIAL_PORT.println("username pass: "+String(tmpUsPs));
//  int encodedLen = base64_enc_len(inputLen);
//  char encoded[encodedLen];
//  
//  SERIAL_PORT.print(tmpUsPs); SERIAL_PORT.print(" = ");
//  
//  // note input is consumed in this step: it will be empty afterwards
//  base64_encode(encoded, tmpUsPs, inputLen); 
//  
//  SERIAL_PORT.println(encoded);
//  SERIAL_PORT.println("Connecting to "+String(relayControllerHost));
//  while (!client.connect(relayControllerHost,80)) {
//		SERIAL_PORT.println("Retrying Port 80 connection...");
//		delay(1000);
//	}
//  SERIAL_PORT.println("Connected to Port 80");
//  client.print("GET ");
//  client.print(request);
//  client.println(" HTTP/1.1");
//  client.println("Host: 192.168.0.34");
//  //client.println("Accept: */*");
//  //client.println("Authorization: Basic "+String(encoded));
//  client.println("Authorization: Basic YWRtaW46YWRtaW4=");
//  client.println();
//  SERIAL_PORT.println(request);
//  while (client.connected()) {
//	if (client.available()) {
//	  char c=client.read();
//          SERIAL_PORT.print(c);
//        }
//  }
}
