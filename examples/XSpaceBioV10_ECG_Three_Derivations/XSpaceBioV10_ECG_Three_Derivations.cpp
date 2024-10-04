/*
 * ECG Monitoring System Using Dual AD8232 Sensors with ESP32
 * 
 * This program is designed to interface with two AD8232 heart rate sensors
 * using an ESP32 microcontroller. It demonstrates the initialization,
 * activation, and data reading from each sensor. The data collected from the sensors
 * is then sent to a computer over Wireless and Serial communication. BPM is calculated.
 *
 * Author: Pablo Cardenas
 */
#include <Arduino.h>
#include <XSpaceBioV10.h>
#include <XSpaceIoT.h>

XSpaceBioV10Board MyBioBoard;
XSEthernet XSerial;

void setup() {

  MyBioBoard.init();

  XSerial.Wifi_init("Delta","c9aa28ba93");
  XSerial.UDP_Connect("192.168.4.101",55000); //You can use Aurora App from www.xspace.pe for wireless com
  
  // Activate the AD8232 sensors to begin ECG data acquisition
  MyBioBoard.AD8232_Wake(AD8232_XS1);
  MyBioBoard.AD8232_Wake(AD8232_XS2);
}

void loop() {

  // I   = LA - RA
  // II  = LL - RA
  // III = LL - LA

  double DerivationI   = MyBioBoard.AD8232_GetVoltage(AD8232_XS1);  //   V+ -> LA     V- -> RA    I   = LA - RA
  double DerivationII = MyBioBoard.AD8232_GetVoltage(AD8232_XS2);   //   V+ -> LL     V- -> RA    II  = LL - RA
  double DerivarionIII  = DerivationII - DerivationI;               // III = II - I   ->          II  = (LL - RA) - (LA - RA) = LL - LA

  XSerial.println((String)DerivationI + " " +(String)DerivationII + " " + (String)DerivarionIII); // To Aurora

  // Introduce a delay of 10 milliseconds to control the data output rate
  delay(10);
}