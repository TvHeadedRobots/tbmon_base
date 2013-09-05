/**
 * An Mirf example which copies back the data it recives.
 *
 * Pins:
 * Hardware SPI:
 * MISO -> 12
 * MOSI -> 11
 * SCK -> 13
 *
 * Configurable:
 * CE -> 8
 * CSN -> 7
 *
 */

#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

//GSM driver pins
byte gsmDriverPin[3] = {3,4,5};

//byte data[Mirf.payload]; // data buffer - unused currently
float data; // recieved data

void setup(){
  
  //setup GSM driver pins
  for(int i = 0 ; i < 3; i++){
    pinMode(gsmDriverPin[i],OUTPUT);
  }
  
  digitalWrite(5,HIGH);//Output GSM Timing 
  delay(1500);
  digitalWrite(5,LOW);  
  digitalWrite(3,LOW);//Enable the GSM mode
  digitalWrite(4,HIGH);//Disable the GPS mode
  delay(2000);
  
  Serial.begin(9600);
  delay(5000);//call ready & wait for comms 
  delay(5000);
  delay(5000);

  //Mirf.cePin = 9;
  //Mirf.csnPin = 10;   
  
  // Set the SPI Driver.
  Mirf.spi = &MirfHardwareSpi;
  
  // Setup pins / SPI
  Mirf.init();
  
  // Configure reciving address.   
  Mirf.setRADDR((byte *)"tbmn1");
  
  /*
   * NB: payload on client and server must be the same.
   */
   
  Mirf.payload = sizeof(float);
  
  // Write channel and payload config then power up reciver.
  Mirf.config();
  
  Serial.println("Listening..."); 
}

void loop(){
 pollSensor();
 
}

void pollSensor(){
 
  float data;
  
  /*
   * If a packet has been recived.
   *
   * isSending also restores listening mode when it 
   * transitions from true to false.
   */
   
  if(!Mirf.isSending() && Mirf.dataReady()){
    //Serial.println("Got packet");
    
    /*
     * Get load the packet into the buffer.
     */
     
    Mirf.getData((byte *)&data);
    
    //Serial.println(data);
    /*
     * Set the send address.
     */
     
     
    Mirf.setTADDR((byte *)"volt1");
    
    /*
     * Send the data back to the client.
     */
     
    Mirf.send((byte *)&data);
    
    /*
     * Wait untill sending has finished
     *
     * NB: isSending returns the chip to receving after returning true.
     */
      
    //Serial.println("Reply sent.");
  }
}
