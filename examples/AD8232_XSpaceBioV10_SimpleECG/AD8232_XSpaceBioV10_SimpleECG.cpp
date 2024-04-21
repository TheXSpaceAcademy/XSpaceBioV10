/**
 * @file    AD8232_XSpaceBioV10_SimpleECG.cpp
 * @brief   ECG Signal Acquisition Using XSpace Bio v1.0 Board
 * 
 * This example demonstrates how to set up and read an ECG signal from the AD8232 module
 * on the XSpace Bio v1.0 development board. It initializes the board, wakes up one of the
 * AD8232 modules, and continuously reads the differential voltage from this module to monitor
 * the heart rate. The readings are printed to the Serial Monitor. This example uses one of
 * the two available AD8232 modules on the board.
 * 
 * @author Pablo Cardenas
 */
#include <Arduino.h>
#include <XSpaceBioV10.h>

// Create an instance of the XSpaceBioV10Board to interact with the board
XSpaceBioV10Board Board;

void setup() {
  // Initialize serial communication with the computer
  Serial.begin(115200);
  // Perform initial setup for the board, preparing it for use
  Board.init();
  // Activate the AD8232 sensor at slot XS1 to start monitoring
  Board.AD8232_Wake(AD8232_XS1);
}

void loop() {
  // Read the differential voltage from the AD8232 at slot XS1
  Serial.println(Board.AD8232_GetVoltage(AD8232_XS1));
  // Pause the loop for 10 milliseconds to control the data rate output
  delay(10);
}