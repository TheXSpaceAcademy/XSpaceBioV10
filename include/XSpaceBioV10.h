/*
	XSpaceBioV10 Library
	Author: Pablo Cardenas
	Description: An open-source library designed for biomedical projects. Please remember to reference this library in your projects!
*/

#ifndef XSPACEBIOV10_H
#define XSPACEBIOV10_H

#include <Arduino.h>
#include <SPI.h>

//afe44xx Register definition
#define CONTROL0      0x00
#define LED2STC       0x01
#define LED2ENDC      0x02
#define LED2LEDSTC    0x03
#define LED2LEDENDC   0x04
#define ALED2STC      0x05
#define ALED2ENDC     0x06
#define LED1STC       0x07
#define LED1ENDC      0x08
#define LED1LEDSTC    0x09
#define LED1LEDENDC   0x0a
#define ALED1STC      0x0b
#define ALED1ENDC     0x0c
#define LED2CONVST    0x0d
#define LED2CONVEND   0x0e
#define ALED2CONVST   0x0f
#define ALED2CONVEND  0x10
#define LED1CONVST    0x11
#define LED1CONVEND   0x12
#define ALED1CONVST   0x13
#define ALED1CONVEND  0x14
#define ADCRSTCNT0    0x15
#define ADCRSTENDCT0  0x16
#define ADCRSTCNT1    0x17
#define ADCRSTENDCT1  0x18
#define ADCRSTCNT2    0x19
#define ADCRSTENDCT2  0x1a
#define ADCRSTCNT3    0x1b
#define ADCRSTENDCT3  0x1c
#define PRPCOUNT      0x1d
#define CONTROL1      0x1e
#define SPARE1        0x1f
#define TIAGAIN       0x20
#define TIA_AMB_GAIN  0x21
#define LEDCNTRL      0x22
#define CONTROL2      0x23
#define SPARE2        0x24
#define SPARE3        0x25
#define SPARE4        0x26
#define SPARE4        0x26
#define RESERVED1     0x27
#define RESERVED2     0x28
#define ALARM         0x29
#define LED2VAL       0x2a
#define ALED2VAL      0x2b
#define LED1VAL       0x2c
#define ALED1VAL      0x2d
#define LED2ABSVAL    0x2e
#define LED1ABSVAL    0x2f
#define DIAG          0x30


#define ECG_OUT_1 34
#define SDN_1 33
#define LOH_1 25
#define AD8232_XS1 0

#define ECG_OUT_2 35
#define SDN_2 27
#define LOH_2 14
#define AD8232_XS2 1

#define PWDN_AFE4490 5
#define RESET_AFE4490 4
#define CS_AFE4490 15

struct XSAD8232{
  int SDN;
  int LO_PLUS;
  int LO_MINUS;
  int OUT;
};

class XSpaceBioV10Board {
  private:
    int cs_pin;             // Chip select pin for SPI communication.
    int pwdn_pin;           // Power down pin for controlling power state of the sensor.
    XSAD8232 AD8232x[4];    // Array of 4 AD8232 instances, each managing a separate sensor.

    /* Writes data to a specified address in the AFE4490 via SPI.
       @param address The register address to write to.
       @param data The data to write to the specified address.
    */
    void AFE4490_write(uint8_t address, uint32_t data);

    /* Reads data from a specified address in the AFE4490 via SPI.
       @param address The register address to read from.
       @return Returns the data read from the specified address.
    */
    unsigned long AFE4490_read(uint8_t address);

  public:
    // Initializes the board and any necessary peripherals or configurations.
    void init();

    /* Initializes an AD8232 sensor with specific pin configurations.
       @param index Sensor index in the array (0-3).
       @param SDN_pin Shutdown control pin.
       @param LO_PLUS_pin Lead-off detection positive pin.
       @param LO_MINUS_pin Lead-off detection negative pin.
       @param OUT_pin Output signal pin.
    */
    void AD8232_init(int index, int SDN_pin, int LO_PLUS_pin, int LO_MINUS_pin, int OUT_pin);

    /* Wakes up an AD8232 sensor specified by its array index.
       @param ADnumber The sensor index (0-3) to be woken up.
    */
    void AD8232_Wake(int ADnumber);

    /* Puts an AD8232 sensor into sleep mode by its array index.
       @param ADnumber The sensor index (0-3) to put into sleep mode.
    */
    void AD8232_Sleep(int ADnumber);

    /* Retrieves the voltage from an AD8232 sensor specified by its array index.
       @param ADnumber The sensor index (0-3).
       @return The diferential voltage (in millivolts) measured by the sensor.
    */
    double AD8232_GetVoltage(int ADnumber);

    /* Initializes the AFE4490 sensor with specific control pins.
       @param cs_pin Chip select pin for SPI communication.
       @param pwdn_pin Power down pin for controlling power state.
    */
    void AFE4490_init(int cs_pin, int pwdn_pin);

    /* Retrieves the infrared (IR) light intensity value from the AFE4490 sensor.
       @return The IR value measured by the sensor.
    */
    unsigned long AFE4490_GetIR();

    /* Retrieves the red light intensity value from the AFE4490 sensor.
       @return The RED value measured by the sensor.
    */
    unsigned long AFE4490_GetRED();

    /* Sets the intensity of IR and RED LEDs on the AFE4490 sensor.
       @param percentageIR Intensity percentage for the IR LED.
       @param percentageRED Intensity percentage for the RED LED.
    */
    void AFE4490_Led_intesity(float percentageIR, float percentageRED);

    /* Calculates the SpO2 (blood oxygen saturation) level using the ratio of red to IR light.
       @param Rx The Red over IR ratio.
       @return The calculated SpO2 percentage.
    */
    double AFE4490_SPO2(double Rx);

    /* Analyzes a signal from the sensor data.
       @param samples Array of signal samples.
       @param time_m Array of time marks corresponding to the samples.
       @param processed_samples Array to store processed samples.
       @param num_samples Number of samples.
       @param max Pointer to store the maximum value found.
       @param min Pointer to store the minimum value found.
       @param freq Pointer to store the dominant frequency in the signal.
    */
    void SignalAnalizer(double *samples, double *time_m, double *processed_samples, int num_samples, double *max, double *min, double *freq);
};



#endif