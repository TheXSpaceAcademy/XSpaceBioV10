/*
 * ECG Monitoring System Using Dual AD8232 Sensors with GENERIC ESP32-WROOM based BOARD
 * 
 * This program is designed to interface with two AD8232 heart rate sensors
 * using an ESP32 microcontroller. It demonstrates the initialization,
 * activation, and data reading from each sensor. The data collected from the sensors
 * is then sent to a computer over serial communication.
 * 
 * Pin Configuration:
 * - The pins for each AD8232 sensor are defined separately to allow for flexible hardware setup.
 * - Each sensor has dedicated pins for ECG output, shutdown control, and lead-off detection.
 *
 * Setup:
 * - Serial communication is established at 115200 bps.
 * - Each AD8232 sensor is initialized with specific pins and activated for data acquisition.
 * 
 * Loop:
 * - The program continuously reads the voltage output from each sensor and prints it to the serial monitor.
 * - A short delay is included in the loop to manage the rate of data output.
 *
 * This setup is useful for projects requiring real-time heart rate monitoring with multiple sensors,
 * and can be adapted for more complex health monitoring systems.
 *
 * Author: Pablo Cardenas
 */
#include <Arduino.h>
#include <XSpaceBioV10.h>

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

void setup() {
  // Initialize serial communication at 115200 bps for output to the computer
  Serial.begin(115200);
  
  // Initialize both AD8232 sensors with their specific pin configurations
  Board.AD8232_init(AD8232_A, SDN_A, LOH_A, LOL_A, ECG_OUT_A);
  Board.AD8232_init(AD8232_B, SDN_B, LOH_B, LOL_B, ECG_OUT_B);

  // Activate the AD8232 sensors to begin ECG data acquisition
  Board.AD8232_Wake(AD8232_A);
  Board.AD8232_Wake(AD8232_B);
}

void loop() {
  // Read and print the voltage (millivolts) readings from both AD8232 sensors to the serial monitor
  Serial.println((String)Board.AD8232_GetVoltage(AD8232_A) + " " + (String)Board.AD8232_GetVoltage(AD8232_B));

  // Introduce a delay of 10 milliseconds to control the data output rate
  delay(10);
}