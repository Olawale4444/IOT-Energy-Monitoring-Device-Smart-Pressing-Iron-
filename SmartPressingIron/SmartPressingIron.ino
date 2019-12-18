//
#include <SoftwareSerial.h>
#include "EmonLib.h"
#define DEBUG true
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

//I2C pins declaration for the LCD
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 

#define VOLT_CAL 620.7
#define CURRENT_CAL 111.1

//Initializing the EnergyMonitor Library
EnergyMonitor emon1;

SoftwareSerial espSerial(7, 8);   //Pin 2 and 3 act as RX and TX. Connect them to TX and RX of ESP8266 
String mySSID = "WIFI_SSID";       // WiFi SSID
String myPWD = "XXXXXXX"; // WiFi Password
String myAPI = "YYYYYYYYYYYY";   // API Key
String myHOST = "api.thingspeak.com";
String myPORT = "80";
String myFIELD = "&field1";
String myFIELD1 = "&field2";
String myFIELD2 = "&field3";
float current,voltage,power;

int range;
float ranger;
char telit[20];
unsigned long timeout;
void setup()
{
  lcd.begin(16,2);//Starts up 16 columns and 2 rows of lcd display
  lcd.backlight();// Powers On the LCD back light
 
  Serial.begin(9600);
  espSerial.begin(115200);
  emon1.voltage(A0, VOLT_CAL, 1.7);
  emon1.current(A1, CURRENT_CAL);

  //Set up wifi
  espData("AT+RST", 1000, DEBUG);                      //Resets the ESP8266 module
  espData("AT+CWMODE=1", 1000, DEBUG);                 //Sets up the ESP mode to station mode
  espData("AT+CWJAP=\""+ mySSID +"\",\""+ myPWD +"\"", 1000, DEBUG);   //Connects to the WiFi network

 timeout = millis(); 
}
  void loop()
  {
    emon1.calcVI(20,5000); 
    
  float currentDraw = emon1.Irms;
  float voltageDraw = emon1.Vrms;

    if(millis() - timeout > 4000) //send data every one minutes
    {

       emon1.calcVI(20,5000); 
    
  float currentDraw = emon1.Irms;
  float voltageDraw = emon1.Vrms;

    Serial.print("Voltage : ");
    Serial.println(voltageDraw);
    
    Serial.print(",");
    Serial.print("Current : ");
    Serial.println(currentDraw);
    
    Serial.print(",");
    Serial.print("Watts : ");
    Serial.println(currentDraw * voltageDraw);
    
    Serial.print("\n\n ");
    
      voltage = voltageDraw;
      current = currentDraw;
      power = voltageDraw * currentDraw;
          
    lcd.setCursor(0,0); //Defining positon to write from first row,first column .
    lcd.print("Olowonyo Olawale"); //You can write 16 Characters per line .
    delay(1000);//Delay used to give a dynamic effect
    lcd.setCursor(0,1);  //Defining positon to write from second row,first column .
    lcd.print("SmartPressingIron");
    delay(5000); 

    lcd.clear();//Clean the screen
    lcd.setCursor(0,0); 
    
    lcd.print("Voltage(V) ");
    lcd.setCursor(0,1);
    lcd.print(voltage);
    delay(4000); 
    ////

    lcd.clear();//Clean the screen
    lcd.setCursor(0,0); 
    
    lcd.print("Current(A)");
    lcd.setCursor(0,1);
    lcd.print(current);
    delay(4000); 
    
    lcd.clear();//Clean the screen
    lcd.setCursor(0,0); 
    
    lcd.print("Power(W)");
    lcd.setCursor(0,1);
    lcd.print(power);
    delay(4000); 
      
    
    String sendData = "GET https://api.thingspeak.com/update?api_key="+ myAPI + myFIELD +"="+String(current)+ myFIELD1 +"="+String(voltage)+ myFIELD2 +"="+String(power);
    espData("AT+CIPMUX=1", 1000, DEBUG);       //Allow multiple connections
    espData("AT+CIPSTART=0,\"TCP\",\""+ myHOST +"\","+ myPORT, 1000, false);
    espData("AT+CIPSEND=0," +String(sendData.length()+4),1000,false);  
    espSerial.find(">"); 
    espSerial.println(sendData);
        
    espData("AT+CIPCLOSE=0",1000,false);
    timeout = millis();
    delay(500);
    }   
  }
  
  String espData(String command, const int timeout, boolean debug)
{ 
  String response = "";
  espSerial.println(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  return response;
}  
