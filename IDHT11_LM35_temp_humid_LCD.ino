 /*
 Modified by Clovis Fritzen in June 11th, 2014:
 - The original program read the serial temperature/humidity sensor IDHT11 (which is still part of this program); 
 - "Delay" function substituted by a counter (when the counter overflows the program enters the serial routine)
 - Added the LM35 (10mV/C temperature sensor) reading via serial, to compare both sensors
 */
/*
  Board	          int.0	  int.1	  int.2	  int.3	  int.4	  int.5
 Uno, Ethernet	  2	  3
 Mega2560	  2	  3	  21	  20	  19	  18
 Leonardo	  3	  2	  0	  1
 Due	          (any pin, more info http://arduino.cc/en/Reference/AttachInterrupt)
 
 This example, as difference to the other, make use of the new method acquireAndWait()
 */
 
#include <idDHT11.h>
#include <LiquidCrystal.h>

int idDHT11pin = 2; //Digital pin for comunications
int idDHT11intNumber = 0; //interrupt number (must be the one that use the previus defined pin (see table above)
long count= 0; // counter for spacing the serial writings
long AcqPeriod= 0;
int sensorPin = A5; // input pin for LM35 (10mV/C)
int sensorValue = 0; // variable to store the LM35 temperature
int led= 13;


//declaration
void dht11_wrapper(); // must be declared before the lib initialization

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(9, 8, 5, 4, 3, 6);

// Lib instantiate
idDHT11 DHT11(idDHT11pin,idDHT11intNumber,dht11_wrapper);

void setup()
{
  pinMode(led, OUTPUT);   
  Serial.begin(9600);
  Serial.println("idDHT11 temp/humidity sensor - Example program");
  Serial.print("Lib version: ");
  Serial.println(IDDHT11LIB_VERSION);
  Serial.println("modified by Clovis Fritzen");
  Serial.println("---------------");
  
   // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("IDTH:|LM35:|HUM:");
}
// This wrapper is in charge of calling 
// mus be defined like this for the lib work
void dht11_wrapper() {
  DHT11.isrCallback();
}
void loop()
{
  
  AcqPeriod= 400000; // set here the interval in which data is sent into serial 
  
  if (count < AcqPeriod) // Still need to make sure what this number represents in terms of
                      // seconds or cycles of clock
  {
  count= count++;
  
  if (count < (AcqPeriod/2)) // just a routine to blink a led :)
  {
    digitalWrite(led, HIGH);
  } else{
    digitalWrite(led, LOW);
  }
 }
else
{
  
  //Serial.print("\nRetrieving information from sensor: ");
  Serial.print("\nRead sensor: ");
  //delay(100);
 count= '0';
 
 sensorValue = analogRead(sensorPin);
 sensorValue= sensorValue/2;
  
  int result = DHT11.acquireAndWait();
  switch (result)
  {
  case IDDHTLIB_OK: 
    Serial.println("done"); 
    break;
  case IDDHTLIB_ERROR_CHECKSUM: 
    Serial.println("Error\n\r\tChecksum error"); 
    break;
  case IDDHTLIB_ERROR_ISR_TIMEOUT: 
    Serial.println("Error\n\r\tISR time out error"); 
    break;
  case IDDHTLIB_ERROR_RESPONSE_TIMEOUT: 
    Serial.println("Error\n\r\tResponse time out error"); 
    break;
  case IDDHTLIB_ERROR_DATA_TIMEOUT: 
    Serial.println("Error\n\r\tData time out error"); 
    break;
  case IDDHTLIB_ERROR_ACQUIRING: 
    Serial.println("Error\n\r\tAcquiring"); 
    break;
  case IDDHTLIB_ERROR_DELTA: 
    Serial.println("Error\n\r\tDelta time to small"); 
    break;
  case IDDHTLIB_ERROR_NOTSTARTED: 
    Serial.println("Error\n\r\tNot started"); 
    break;
  default: 
    Serial.println("Unknown error"); 
    break;
  }
  
  lcd.setCursor(7, 1);
  // print the number of seconds since reset:
  lcd.print(sensorValue);
  
  Serial.print("Humidity (%): ");
  Serial.println(DHT11.getHumidity(), 0); // ", 0" represents the number of decimal positions after the comma

  Serial.print("DTH11 Temp (oC): ");
  Serial.println(DHT11.getCelsius(), 0); // ", 0" represents the number of decimal positions after the comma
  
  lcd.setCursor(13, 1);
  // print the number of seconds since reset:
  lcd.print(DHT11.getHumidity(), 0);
  
  Serial.print("LM35 Temp (oC): ");
  Serial.println(sensorValue);
  

  //Serial.print("Temperature (oF): ");
  //Serial.println(DHT11.getFahrenheit(), 2);

  //Serial.print("Temperature (K): ");
  //Serial.println(DHT11.getKelvin(), 2);

  //Serial.print("Dew Point (oC): ");
  //Serial.println(DHT11.getDewPoint());

  //Serial.print("Dew Point Slow (oC): ");
  //Serial.println(DHT11.getDewPointSlow());
  
   // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(1, 1);
  // print the number of seconds since reset:
  lcd.print(DHT11.getCelsius(),0);
  
   }
  
  }
