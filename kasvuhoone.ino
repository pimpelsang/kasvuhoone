
/*
 * Sketch for testing sleep mode with wake up on WDT.
 * Donal Morrissey - 2011.
 *
 */
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>

#define LED_PIN (13)
#define POT_PIN (0)

int val = 0;
/***************************************************
 *  Name:        setup
 *
 *  Returns:     Nothing.
 *
 *  Parameters:  None.
 *
 *  Description: Setup for the serial comms and the
 *                Watch dog timeout. 
 *
 ***************************************************/
void setup()
{
  Serial.begin(9600);
  Serial.println("Initialising...");
  delay(100); //Allow for serial print to complete.
  
  pinMode(LED_PIN,OUTPUT);
  

  Serial.println("Initialisation complete.");
  delay(100); //Allow for serial print to complete.
  
  
}



/***************************************************
 *  Name:        enterSleep
 *
 *  Returns:     Nothing.
 *
 *  Parameters:  None.
 *
 *  Description: Main application loop.
 *
 ***************************************************/
void loop()
{
  /* Toggle the LED */
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  val = analogRead(POT_PIN);
  Serial.println("POTE RESULT: ");
  Serial.println(val);
  delay(1000);
}
