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

// Pin definitions for the first AD8232 sensor setup
#define ECG_OUT_A 34  // ECG output pin for the first AD8232 sensor
#define SDN_A 33      // Shutdown pin for the first AD8232 sensor
#define LOH_A 25      // Lead-off detection high pin for the first sensor
#define LOL_A 26      // Lead-off detection low pin for the first sensor
#define AD8232_A 0    // Identifier for the first AD8232 sensor

// Pin definitions for the second AD8232 sensor setup
#define ECG_OUT_B 35  // ECG output pin for the second AD8232 sensor
#define SDN_B 27      // Shutdown pin for the second AD8232 sensor
#define LOH_B 14      // Lead-off detection high pin for the second sensor
#define LOL_B 13      // Lead-off detection low pin for the second sensor
#define AD8232_B 1    // Identifier for the second AD8232 sensor

// Create an instance of the XSpaceBioV10Board to interact with the hardware board
XSpaceBioV10Board Board;
XSEthernet XSnet;

double GetBPM();

void setup() {
  // Initialize serial communication at 115200 bps for output to the computer
  Serial.begin(115200);

  XSnet.Wifi_init("Delta","c9aa28ba93");
  XSnet.UDP_Connect("192.168.31.150",55000); //You can use Aurora app from www.xspace.pe
  
  
  // Initialize both AD8232 sensors with their specific pin configurations
  Board.AD8232_init(AD8232_XS1, SDN_A, LOH_A, LOL_A, ECG_OUT_A);
  Board.AD8232_init(AD8232_XS2, SDN_B, LOH_B, LOL_B, ECG_OUT_B);

  // Activate the AD8232 sensors to begin ECG data acquisition
  Board.AD8232_Wake(AD8232_XS1);
  Board.AD8232_Wake(AD8232_XS2);
}




double ECGData[400];
double ECGData_procesed[400];
double ECGData_time[400];
int i = 0;
double bpm = 0;

void loop() {

  double AD8232_XS1_voltage = Board.AD8232_GetVoltage(AD8232_XS1);
  double AD8232_XS2_voltage = Board.AD8232_GetVoltage(AD8232_XS2);

  ECGData[i]=AD8232_XS1_voltage; //Storing a sammple
  ECGData_time[i]=micros(); //Storing time (uS) of the current sammple

  // Read and print the voltage (millivolts) readings from both AD8232 sensors to the serial monitor
  Serial.println((String)ECGData_procesed[i] + " " + (String)ECGData_procesed[i] + " " + (String)bpm);
  XSnet.println((String)ECGData_procesed[i] + " " + (String)ECGData_procesed[i] + " " + (String)bpm);
  
  if(i==400){
    bpm =  GetBPM();
    i = 0;
  }

  // Introduce a delay of 10 milliseconds to control the data output rate
  delay(10);
}

double GetBPM(){
  double max,min,freq;
  Board.SignalAnalizer(ECGData,ECGData_time,ECGData_procesed,400,&max,&min,&freq);

  return freq*60.0;
}