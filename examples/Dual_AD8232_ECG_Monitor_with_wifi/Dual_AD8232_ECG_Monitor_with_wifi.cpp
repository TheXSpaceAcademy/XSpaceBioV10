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

// Create an instance of the XSpaceBioV10Board to interact with the hardware board
XSpaceBioV10Board Board;
XSEthernet XSnet;

// Variables for signal processing
#define MaxDataIndex 400

double ECGData[MaxDataIndex];
double ECGData_procesed[MaxDataIndex];
double ECGData_time[MaxDataIndex];
double bpm = 0;
int i = 0;

//Declaration por GetBPM() function
double GetBPM();

void setup() {
  // Initialize serial communication at 115200 bps for output to the computer
  Serial.begin(115200);

  Board.init();

  XSnet.Wifi_init("Delta","c9aa28ba93");
  XSnet.UDP_Connect("192.168.31.150",55000); //You can use Aurora App from www.xspace.pe for wireless com
  
  // Activate the AD8232 sensors to begin ECG data acquisition
  Board.AD8232_Wake(AD8232_XS1);
  Board.AD8232_Wake(AD8232_XS2);
}

void loop() {

  double AD8232_XS1_voltage = Board.AD8232_GetVoltage(AD8232_XS1);
  double AD8232_XS2_voltage = Board.AD8232_GetVoltage(AD8232_XS2);

  ECGData[i]=AD8232_XS1_voltage; //Storing a sammple
  ECGData_time[i]=micros(); //Storing time (uS) of the current sammple

  // Read and print the voltage (millivolts) readings from both AD8232 sensors to the serial monitor
  Serial.println((String)ECGData_procesed[i] + " " + (String)ECGData[i] + " " + (String)bpm);
  XSnet.println((String)ECGData_procesed[i] + " " + (String)ECGData[i] + " " + (String)bpm);
  
  i++;

  if(i==MaxDataIndex){
    bpm =  GetBPM();
    i = 0;
  }

  // Introduce a delay of 10 milliseconds to control the data output rate
  delay(10);
}

double GetBPM(){
  double max,min,freq;
  Board.SignalAnalizer(ECGData,ECGData_time,ECGData_procesed,MaxDataIndex,&max,&min,&freq);

  return freq*60.0;
}