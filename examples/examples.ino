#include <SoftwareSerial.h>
#include <String.h>

/*
Solar power level log to internet
Power cutoff when voltage too low
*/
int reportDelayMin = 10;
float voltage = 0;
float minVoltage = 11.5;
float errorVoltage = 9; //must be connected from usb

/*
Setup
*/
void setup()
{
  //logging to PC
  Serial.begin(19200);
  Serial.println("SETUP start!");
  
  //check sensors, gprs work
  setupGPRS();
  setupVolts();
  //setupRelay();
  
  voltage = readVolts();
  Serial.print("Voltage:");
  Serial.println(voltage);
  
  delay(2000);
  Serial.println("SETUP ready!\n");
}

/*
Loop
*/
void loop()
{
  Serial.println("LOOP start()");
  
  voltage = readVolts();
  Serial.print("Voltage:");
  Serial.println(voltage);
  
  if (voltage > minVoltage || voltage < errorVoltage) { 
    Serial.println("power ON!");
    //relayPowerOn();
  } else {
    Serial.println("POWER OFF!!!");
    //relayPowerOff();
  }

  powerUpGPRS();
  if (!gprsWorking()) {
    Serial.println("ERROR: Could not power up gprs! aborting...");
    delay(10000);
    return;
  }
  
  //give some time for proper network connection
  waitProperSignal();
  
  connectionSetup();
  boolean sent = false;
  
  if (connectServer()) 
  {
    Serial.println("connected, sending current value!");
    uploadCurrentValue();
    disconnectServer();
    sent = true;
  } else {
    Serial.println("failed to connect to server, aborting!");
  }

  powerDownGPRS();
  
  Serial.println("LOOP DONE");
  Serial.println("\n\n--------------------------------------------------------------------------------\n");
  
  if (sent)
    delay(reportDelayMin * 60000); // Wait for a period of time, the restart the steps
  else
    delay(10000);
}

//***********************************************************************************************
// Functions for GPRS connection and connecting to SERVER
// Emulate HTTP and use PUT command to upload temperature datapoint using Comma Seperate Value Method
//***********************************************************************************************
SoftwareSerial gprs(7, 8);

void setupGPRS() {
    gprs.begin(19200);
}

void waitProperSignal() {
  Serial.println("wait for proper network connection...");

  gprs.println("AT+CSQ"); //signal quality
  printResponse("5min Signal:");
  delay(3000);
  /*
  
  gprs.println("AT+CSQ"); //signal quality
  printResponse("4min Signal:");
  delay(60000);

  gprs.println("AT+CSQ"); //signal quality
  printResponse("3min Signal:");
  delay(60000);

  gprs.println("AT+CSQ"); //signal quality
  printResponse("2min Signal:");
  delay(60000);
  */

  gprs.println("AT+CSQ"); //signal quality
  printResponse("Signal:");
  delay(30000);
}

void connectionSetup() {
  Serial.println("\n\rconnectionSetup()!");

  gprs.println("ATE0\r"); //turn echo off
  delay(100);
  
  gprs.flush();
  gprs.println("AT+CREG?"); //check cellular network status
  printResponse("Network:");
  
  gprs.println("AT+CSQ"); //signal quality
  printResponse("Signal:");
  
  gprs.println("AT+CGATT=1"); // attach to GPRS network 
  printResponse("GPRS mode:");
  delay(3000);
  
  gprs.println("AT+CGATT?");
  printResponse("GPRS network:");
  
  //don't care of response error
  gprs.println("AT+CIPMUX=0"); //We only want a single IP connection
  gprs.println("AT+CIPMODE=0"); //Selecting Normal Mode

  //gprs context definition, at+cgdcont=1,"IP","web.vodafone.de"
  gprs.println("AT+CGDCONT=1,\"IP\",\"internet.emt.ee\""); 
  printResponse("AT+CGDCONT:");
  delay(2000);

  //AT+CSTT="internet.emt.ee"
  command("AT+CSTT=\"internet.emt.ee\",\"\",\"\"", "OK", 3000); //Start Task and set Access Point Name
  delay(1000);
  
  command("AT+CIICR", "OK", 10000); ////BRING UP WIRELESS CONNECTION WITH GPRS OR CSD 

  gprs.println("AT+CIFSR"); //my IP like 83.178.59.57
  delay(2000);
  printResponse("My IP:");
  
  gprs.println("AT+CIPSTATUS");
  printResponse("Status:");
  
  Serial.println("CONNECTION ready!\n\r");
  delay(2000);
}

boolean connectServer() {
  Serial.println("\nconnectServer() to erki.pelltech.eu");
  gprs.println("AT+CIPSTART=\"TCP\",\"erki.pelltech.eu\",\"8234\"");

  command("AT+CIPSEND");
  return true;
  
/*  if (response("ERROR", 10000)) {
     Serial.println("ERROR CONNECTING! aboring...\n");
     return false;
  }
  else
  {
    Serial.println("\nSENDING data");
    gprs.flush();
    if(command("AT+CIPSEND",">", 5000)) {
      return true;
    }
  }
  return false;
  */
}

void disconnectServer() {
  Serial.println("SENDING END\n");
  Serial.println("CONNECTING erki.pelltech.eeend\n");
  delay(5000);
  Serial.println("Closing gprs");
  command("AT+CIPCLOSE", "CLOSED"); 

  delay(1000);
  command("AT+CIPSHUT"); 
}

//***********************************************************************************************
// Functions for DATA UPLOAD
// Emulate HTTP and use PUT command to upload temperature datapoint using Comma Seperate Value Method
//***********************************************************************************************
#define USERAGENT      "Robot" // user agent is the project name

void uploadCurrentValue()
{
  Serial.println("uploadCurrentValue()");
  gprs.flush();
  // send the HTTP PUT request:
  gprs.print("PUT /jama/");
  gprs.println(".csv HTTP/1.1");
  gprs.println("Host: erki.pelltech.eu");
  gprs.print("User-Agent: ");
  gprs.println(USERAGENT);
  gprs.println("Content-Length:7");

  // last pieces of the HTTP PUT request:
  gprs.println("Content-Type: text/csv");
  gprs.println("Connection: close");
  gprs.println();

  // here's the actual content of the PUT request:
  gprs.print("0,");
  
  //format to 5 digits
  char output[5]; 
  dtostrf(voltage, 5, 2, output);
  Serial.print("0,");
  Serial.println(output);
  gprs.println(output);
  
  //eof
  gprs.println((char)26);

  Serial.println("uploadCurrentValue().done!");
  
  delay(3000);
  printResponse("Response:");
}
/*
void uploadHistory()
{
  Serial.println("uploadHistory()");
  gprs.flush();
  
  // api.cosm.com/v2/feeds/1977/datastreams/1/datapoints
  gprs.print("POST /v2/feeds/");
  gprs.print(FEEDID);
  gprs.println("/datastreams/0/datapoints.csv HTTP/1.1");
  gprs.println("Host: api.cosm.com");
  gprs.print("X-ApiKey: ");
  gprs.println(APIKEY);
  gprs.print("User-Agent: ");
  gprs.println(USERAGENT);
  gprs.print("Content-Length: ");
  gprs.println(28);

  // last pieces of the HTTP PUT request:
  gprs.println("Content-Type: text/csv");
  gprs.println("Connection: close");
  gprs.println();


"00/01/08,19:32:48+00"
  gprs.println("2012-08-24T13:21:53.00Z,121");
  gprs.println((char)26);

  Serial.println("uploadHistory().done!");
  
  delay(5000);
  printResponse("Response:");
}
*/

//***********************************************************************************************
// Functions for GPRS communication
//***********************************************************************************************

boolean command(String cmd) {
  return command(cmd, "OK");
}
boolean command(String cmd, String expect) {
  return command(cmd, expect, 200);
}
boolean command(String cmd, String expect, int wait) {
  Serial.println(" command("+cmd+","+expect+")");
  
  gprs.println(cmd);
  if(response(expect, wait))
  {
     Serial.println(" command.SUCCEED");
     return true; 
  } else {
     Serial.println(" command.FAILED");
     return false; 
  }
}

boolean response(String expect) {
  return response(expect, 200);
}
boolean response(String expect, int wait) {
    //Serial.println(" response("+expect+","+wait+")");
    delay(wait);
    
    String got = "";
    while(gprs.available()!=0) {
      got += (char) gprs.read();
    }

    //check response
    if (got.equals("")) {
      Serial.println(" response().no responce!");
      return false;
    }
    
    if (got.indexOf(expect) == -1) {
      Serial.println(" response().no match! got: "+got);
      return false;
    }
    
    //Serial.println(" response().match!");
    return true;
}

void printResponse(String prefix) {
  Serial.print(prefix);
  delay(100);
  while(gprs.available()!=0) {
      Serial.write((char)gprs.read()); 
  }
}

boolean powerUpGPRS(){
  Serial.println("powerUpGPRS()");

  if(gprsWorking()){
    //Serial.println("powerUpGPRS().gprs already working!");
    return true;
  }
  
  //Serial.println("powerUpGPRS().starting GRPS");
  powerUpGPRSOrDown();

  //Serial.println("powerUpGPRS().checking again in 10sec");
  delay(20000); 
  gprs.flush();
  
  return gprsWorking();
}
boolean powerDownGPRS(){
  Serial.println("powerDownGPRS()");
  
  powerUpGPRSOrDown();

  if(response("NORMAL POWER DOWN", 2000))
  {
    return true;
  }
  else {
    return gprsWorking();
  }
}

boolean gprsWorking() {
  delay(100);
  //Serial.println("gprsWorking()");
  
  if (command("AT", "OK")) {
    //Serial.println("gprsWorking.YES");
    return true;
  }
  else
  {
    //Serial.println("gprsWorking.NO");
    return false;
  }
}

void powerUpGPRSOrDown()
{
  pinMode(9, OUTPUT); 
  digitalWrite(9,LOW);
  delay(1000);
  digitalWrite(9,HIGH);
  delay(2000);
  digitalWrite(9,LOW);
  delay(3000);
}



/************************************************************************************************
Voltage monitor
prints the voltage on analog pin to the serial port
Do not connect more than 5 volts directly to an Arduino pin.
/***********************************************************************************************/


const int R1 = 9870;
const int R2 = 2170;

// Analog 0 pin

// determine by voltage divider resistors, see text
const float resistorFactor = (float) R2 / ( (float)R1 + (float)R2 );
float Vcc;

void setupVolts() {
   Vcc = readVcc() / 1000;
   Serial.print("setupVolts() Vcc:");
   Serial.println( Vcc );
}

long readVcc() {
  long result;
  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA,ADSC));
  result = ADCL;
  result |= ADCH<<8;
  result = 1125300L / result; // Back-calculate AVcc in mV
  return result;
}

float readVolts() {
  float tambov = 1.1;
  
  int val = analogRead(A0); // read the value from the sensor
  float volts = val * Vcc / resistorFactor / 1024.0 * tambov; // calculate the ratio

  return volts;
}


/************************************************************************************************
RELAY controller
/***********************************************************************************************/

int relayPin = 2;//arduino digital pin!

void setupRelay()
{
   pinMode(relayPin, OUTPUT);  
}
void relayPowerOn() 
{
   digitalWrite(relayPin, LOW);  
}
void relayPowerOff() 
{
   digitalWrite(relayPin, HIGH);  
}
