/*
* - Arduino uses digital pins 10, 11, 12, and 13 (SPI) to communicate with the W5100 on the ethernet
* - Add DS18B
* - Add Auto Mac REF: https://www.freetronics.com.au/pages/setting-arduino-ethernet-mac-addresses-using-a-ds18b20
* - ADD LDR to Board
* - Add LED indicator
* - Optimase Size - 26506/1192 
* - Made Program to resemble the basicks of a SonOff Devise Profile to emulate the inforamtion for a small program to monitor IT devices
*  * Genereate Mac: https://ssl.crox.net/arduinomac/
*/

#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <avr/wdt.h>
//#include <MemoryFree.h>;
//#include <pgmStrToRAM.h>; // not needed for new way. but good to have for reference.

byte ver = 12 ;
<<<<<<< HEAD
const char* topic = "wsAlarm";
=======

>>>>>>> 8f2e0c8db037f6e75938254d4b3a73d1112570b0
// Update these with values suitable for your hardware/network.
//byte mac[6] = { 0x90, 0xA2, 0xDA, 0x66, 0xB1, 0x90 }; // Test Bench
byte mac[6] = { 0x90, 0xA2, 0xDA, 0xF4, 0xDA, 0xB6 };
//const char* macS = "90:A2:DA:66:B1:90"; // This is used to report the mac address for the Monitor
const char* macS = "90:A2:DA:F4:DA:B6";  

IPAddress ip(192, 168, 8, 13);
IPAddress server(192, 168, 8, 30);
const char* Fname = "Workshop Alarm";
// Host Name MAX 12 Characters
//char SetHostName[] = "wsAlarm";

EthernetClient ethClient;
PubSubClient client(ethClient);
long lastReconnectAttempt = 0;
//+------------------------------------------------------------------+
//|Setup MQTT Watchdog
//+------------------------------------------------------------------+
byte connect_timeout = 0;
//+------------------------------------------------------------------+
//|Setup LDR
//+------------------------------------------------------------------+
const byte photocellPin = A0; // LDR and 10K pulldown registor are connected to A0
int photocellReading; // Variable to read the analog value

//+------------------------------------------------------------------+
//|Setup DS18B20
//+------------------------------------------------------------------+
#include <OneWire.h>
#include <DallasTemperature.h>
OneWire ds(9);
#define ONE_WIRE_BUS 9
// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);
DeviceAddress insideThermometer;
int dstemp = 0;
//+------------------------------------------------------------------+
//|Setup Buttons
//+------------------------------------------------------------------+
#include <Bounce2.h>
#define NUM_BUTTONS 12
const uint8_t BUTTON_PINS[NUM_BUTTONS] = {0,1,2,3,5,6,7,15,16,17,18,19 };
Bounce * buttons = new Bounce[NUM_BUTTONS];
//int ipt = 0;
bool inpt1 = false ;

//+------------------------------------------------------------------+
//| MQTT General
//+------------------------------------------------------------------+


//const char* stat = "stat/";
const char* LWT = "tele/wsAlarm/LWT" ;
//const char* inTopic = "cmd/wsAlarm/#";
//String val;
//const char* sensorTopic = "stat/wsAlarm1/SENSOR";
//const char* dataTopic2 = "stat/wsAlarm1/info2";
const char* outPinTopic = "stat/wsAlarm/SWITCH";

const char* cmdTopic = "cmnd/wsAlarm/STATUS"; // Need to compare incommin payload
//const char* recState = "tele/wsAlarm1/STATE" ;
//const char* statTopic = "stat/wsAlarm1/STATUS";
//const char* status_5 = "stat/wsAlarm1/STATUS5" ;


//char LWT[20] = "";
//char sensorTopic[20] = "";//tele/lightz4/SENSOR
//char status_5[25]="";
//char outPinTopic[25]="";
//char statTopic[25]="";


unsigned long newTmqtt;

//+------------------------------------------------------------------+
//| LED Flash
//+------------------------------------------------------------------+
unsigned long LEDpreviousMillis = 0;
const long LEDinterval = 500;    
int ledState = LOW;  
byte MQTTdisconnect = 0;

void setup(){ 
  Serial.begin(9600);
<<<<<<< HEAD
  Serial.println(F("Startup Begin"));
=======
  Serial.println("Startup Begin");
>>>>>>> 8f2e0c8db037f6e75938254d4b3a73d1112570b0
  pinMode(8, OUTPUT); // Setup LED
  digitalWrite(8, LOW); 
  //setupDSB18 ();
  Ethernet.begin(mac, ip);
 // Ethernet.begin(mac);
  Serial.println(F("Startup Complete"));
  start_mqtt ();
  setupInput();
  MQTTdisconnect = 0;
  wdt_reset();
  //wdt_enable(WDTO_2S);
   //Ethernet.begin(mac, ip);
 //delay(1500);
  
}



void loop(){
check_mqtt ();
read_input_pins ();
send_data ();
reset_arduino_mqtt ();
LEDflash();
//wdt_reset();


}
