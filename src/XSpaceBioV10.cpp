/*
	XSpaceBioV10 Library
	Author: Pablo Cardenas
	Description: An open-source library designed for biomedical projects. Please remember to reference this library in your projects!
*/

#include <XSpaceBioV10.h>
#include <SPI.h>

/******************************XSpaceBioV10***********************************/

#define AFE44XX_SPI_SPEED 2000000
SPISettings SPI_SETTINGS(AFE44XX_SPI_SPEED, MSBFIRST, SPI_MODE0); 

unsigned long IRtemp,REDtemp;


void XSpaceBioV10Board::init(){
	this->AD8232_init(AD8232_XS1, SDN_1, LOH_1, 0, ECG_OUT_1);
	this->AD8232_init(AD8232_XS2, SDN_2, LOH_2, 0, ECG_OUT_2);

	this->AFE4490_init(CS_AFE4490, PWDN_AFE4490);
}

void XSpaceBioV10Board::AD8232_init(int index, int SDN_pin, int LO_PLUS_pin, int LO_MINUS_pin, int OUT_pin){
	AD8232x[index].SDN = SDN_pin;
	AD8232x[index].LO_PLUS = LO_PLUS_pin;
	AD8232x[index].OUT = OUT_pin;

	pinMode(AD8232x[index].SDN,OUTPUT);
	pinMode(AD8232x[index].LO_PLUS,INPUT_PULLUP);
}

void XSpaceBioV10Board::AD8232_Wake(int ADnumber){
	digitalWrite(AD8232x[ADnumber].SDN, HIGH);
}

void XSpaceBioV10Board::AD8232_Sleep(int ADnumber){
	digitalWrite(AD8232x[ADnumber].SDN, LOW);
}

double XSpaceBioV10Board::AD8232_GetVoltage(int ADnumber){
	return (double)analogReadMilliVolts(AD8232x[ADnumber].OUT)*0.0010843544;
}

unsigned long XSpaceBioV10Board::AFE4490_GetIR(){
	AFE4490_write(CONTROL0, 0x000001);
	return AFE4490_read(LED1ABSVAL);
}

unsigned long XSpaceBioV10Board::AFE4490_GetRED(){
	AFE4490_write(CONTROL0, 0x000001);
	return AFE4490_read(LED2ABSVAL);
}

void XSpaceBioV10Board::AFE4490_init(int cs_pinx, int pwdn_pinx){
		SPI.begin();
		SPI.setFrequency(8000000);

		cs_pin=cs_pinx;
		
		pwdn_pin=pwdn_pinx;
 
		pinMode(cs_pin, OUTPUT);
		digitalWrite(cs_pin,HIGH);

		pinMode (pwdn_pin,OUTPUT);

		digitalWrite(pwdn_pin, LOW);
		delay(500);
		digitalWrite(pwdn_pin, HIGH);
		delay(500);

		//afe4490Write(CONTROL0, 0x000000);
		AFE4490_write(CONTROL0, 0x000008); //reset all registers

		AFE4490_write(LED2STC, 0x0017C0);
		AFE4490_write(LED2ENDC, 0x001F3E);
		AFE4490_write(LED2LEDSTC, 0x001770);
		AFE4490_write(LED2LEDENDC, 0x001F3F);
		AFE4490_write(ALED2STC, 0x000050);
		AFE4490_write(ALED2ENDC, 0x0007CE);

		AFE4490_write(LED1STC, 0x000820);
		AFE4490_write(LED1ENDC, 0x000F9E);
		AFE4490_write(LED1LEDSTC, 0x0007D0);
		AFE4490_write(LED1LEDENDC, 0x000F9F);
		AFE4490_write(ALED1STC, 0x000FF0);
		AFE4490_write(ALED1ENDC, 0x00176E);

		AFE4490_write(LED2CONVST, 0x000006);
		AFE4490_write(LED2CONVEND, 0x0007CF);
		AFE4490_write(ALED2CONVST, 0x0007D6);
		AFE4490_write(ALED2CONVEND, 0x000F9F);

		AFE4490_write(LED1CONVST, 0x000FA6);
		AFE4490_write(LED1CONVEND, 0x00176F);
		AFE4490_write(ALED1CONVST, 0x001776);
		AFE4490_write(ALED1CONVEND, 0x001F3F);
		AFE4490_write(ADCRSTCNT0, 0x000000);

		AFE4490_write(ADCRSTENDCT0, 0x000005);
		AFE4490_write(ADCRSTCNT1, 0x0007D0);
		AFE4490_write(ADCRSTENDCT1, 0x0007D5);
		AFE4490_write(ADCRSTCNT2, 0x000FA0);
		AFE4490_write(ADCRSTENDCT2, 0x000FA5);
		AFE4490_write(ADCRSTCNT3, 0x001770);
		AFE4490_write(ADCRSTENDCT3, 0x001775);
		
		AFE4490_write(PRPCOUNT, 0x001F3F);
		AFE4490_write(CONTROL1, 0x000101); // Timers ON, average 2 samples
		AFE4490_write(SPARE1, 0x000000);
		AFE4490_write(TIAGAIN, 0x000000); // CF = 5pF, RF = 500kR
		AFE4490_write(TIA_AMB_GAIN, 0x000000);
		AFE4490_write(LEDCNTRL, 0x010000); //RANGE=50mA
		AFE4490_write(CONTROL2, 0x030000); // TX_REF=0.5

		AFE4490_write(SPARE2, 0x000000);
		AFE4490_write(SPARE3, 0x000000);
		AFE4490_write(SPARE4, 0x000000);
		AFE4490_write(ALARM, 0x000000);

		delay(500);
}

void XSpaceBioV10Board::AFE4490_write (uint8_t address, uint32_t data)
{
	SPI.beginTransaction(SPI_SETTINGS);
	digitalWrite (cs_pin, LOW); // enable device
	SPI.transfer (address); // send address to device
	SPI.transfer ((data >> 16) & 0xFF); // write top 8 bits
	SPI.transfer ((data >> 8) & 0xFF); // write middle 8 bits
	SPI.transfer (data & 0xFF); // write bottom 8 bits
	digitalWrite (cs_pin, HIGH); // disable device
	SPI.endTransaction();
}

unsigned long XSpaceBioV10Board::AFE4490_read (uint8_t address)
{
	unsigned long data = 0;

	SPI.beginTransaction(SPI_SETTINGS);
	digitalWrite (cs_pin, LOW); // enable device
	SPI.transfer (address); // send address to device
	data |= ((unsigned long)SPI.transfer (0) << 16); // read top 8 bits data
	data |= ((unsigned long)SPI.transfer (0) << 8); // read middle 8 bits  data
	data |= SPI.transfer (0); // read bottom 8 bits data
	digitalWrite (cs_pin, HIGH); // disable device
	SPI.endTransaction();

	return data; // return with 24 bits of read data
}

void XSpaceBioV10Board::AFE4490_Led_intesity(float percentageIR, float percentageRED){
		uint32_t value1 = uint32_t(percentageIR/100.0*255.0);
		uint32_t value2 = uint32_t(percentageRED/100.0*255.0);
		AFE4490_write(CONTROL0, 0x000000);
		AFE4490_write(LEDCNTRL, ((value1<<8)|value2));
}

double R_lookup[10] = {0.53861, 0.55417, 0.58322, 0.63, 0.697, 0.829, 0.9199, 1.04, 1.126, 1.2};
double SPO2_lookup[10] = {100, 99, 98, 97, 95, 90, 85, 80, 75, 70};

double XSpaceBioV10Board::AFE4490_SPO2(double Rx){
	double SPO2;

	int i = 0;
	for(i=0; i<9; i++) if(Rx>R_lookup[i] && Rx<R_lookup[i+1]) break;
	
	double proporcion = (Rx-R_lookup[i])/(R_lookup[i+1]-R_lookup[i]);

	SPO2 = SPO2_lookup[i] - (SPO2_lookup[i]-SPO2_lookup[i+1])*proporcion;

	if(Rx<R_lookup[0])SPO2=100;
	if(Rx>R_lookup[9])SPO2=70;

	return SPO2;

}


void XSpaceBioV10Board::SignalAnalizer(double *muestras, double *time_m, double *muestras_pro, int no_muestras, double *max, double *min, double *freq){
	double temp_max = 0;
	double temp_min = 10000000000000;
	
	for(int i=0; i<no_muestras; i++){
		if(muestras[i]>temp_max)temp_max = muestras[i];
		if(muestras[i]<temp_min)temp_min = muestras[i];
	}

	double moffset = (temp_max+temp_min)/2;

	for(int i=0; i<no_muestras; i++){
		muestras_pro[i] =  muestras[i] - moffset;
	}

	*max = temp_max-moffset;
	*min = temp_min-moffset;

	int Posiciones[50];
	int j=0;
	bool numx=true;

	for(int i=5; i<no_muestras; i++){
		if( (muestras_pro[i]>0) && muestras_pro[i-5]<0  && numx==true){
			Posiciones[j] = time_m[i];
	//muestras_pro[i]=10000;
			j++;
			numx=false;
		}
		if(muestras_pro[i]<0 && !numx) numx=true;
	}

	double SumaPeriodos = 0;
	for(int k=j-1; k>0; k--){
		SumaPeriodos = SumaPeriodos + (double)(Posiciones[k]-Posiciones[k-1]);
	}
	SumaPeriodos = SumaPeriodos/1000000;
	
	//muestras_pro[0]=20000;
	//muestras_pro[no_muestras-1]=-20000;

	//*freq = ((double)(j-1))/SumaPeriodos;

}