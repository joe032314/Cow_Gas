/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "math.h"
#include "MG811.h"
#include "main.h"

float PPM_MG811 = 0;
int sample_counter = 0;
int add_counter = 0;
float ADC_value_total = 0;
float ADC_value_average = 0;
float Sensor_value_MG811 = 0;
float SensorVolt_MG811 = 0;

float ADC_MQ2,ADC_MQ136,ADC_MQ137 = 0;

float sample_value_sec[10000] = {0};

float Ro_MQ2, Ro_MQ136,Ro_MQ137,Ro_MG811, ADC_Raw_Value, Ro_MQ4, Ro_MQ135 = 0.0;
float PPM_MQ137, PPM_MQ2, PPM_MQ136, PPM_MQ4, PPM_MQ135_CO2, PPM_MQ135_NH3 = 0.0;
float CH4Curve[3] = {2.3, 0.47, -1.07};
int i = 0;
float ratio_MQ2,ratio_MQ136,ratio_MQ137,ratio_MQ135, ratio_MQ4 = 0.0;
float Volt_MQ2,Volt_MQ136,Volt_MQ137, Volt_MQ4, Volt_MQ135 = 0.0;
float VRL_MQ2, VRL_MQ136, VRL_MQ137, VRL_MQ135, VRL_MQ4 = 0.0;
float RS_MQ2,RS_MQ136,RS_MQ137,RS_MQ4, RS_MQ135 = 0.0; 


void MG811_convert(float CO2_Voltage)
{
		/*
			3-order linear eq
			E.Q. ppm = -175200 * SensorVolt_MG811^3 + 532900 * SensorVolt_MG811^2 -5411000 * SensorVolt_MG811 + 1835000
		*/
		
		PPM_MG811 = (1752 * pow(CO2_Voltage,3.0)) + (5329 * pow(CO2_Voltage,2.0)) + (-5411 * CO2_Voltage) + 1835;
		//printf("%2.3f\r\n", CO2_Voltage);
		printf("%1.0f", PPM_MG811);
}


void Calibration_MQ2(float ADC_MQ2)
{
	//printf("%4.0f\r\n",ADC_MQ2);
	VRL_MQ2 = (ADC_MQ2 / 4096) * 2.81;
	RS_MQ2 = ((5.0 / VRL_MQ2)- 1) *  RL_MQ2;
	Ro_MQ2 =  RS_MQ2 / 9.8;
	//printf("%f  %f  %f\r\n",VRL_MQ2, RS_MQ2 , Ro_MQ2);
	//printf("Ro_MQ2 : %f\r\n",Ro_MQ2);
}

void Calculation_MQ2(float ADC_MQ2)
{
	//printf("##%f\r\n",ADC_MQ2);
	Volt_MQ2 = (ADC_MQ2 / 4096) * 2.81;
	//printf("Volt_MQ2 :  %f\r\n",Volt_MQ2);
	RS_MQ2 = ((5.0 * RL_MQ2) / Volt_MQ2) - RL_MQ2;
	//printf("%f\r\n",RS_MQ2);
	
	ratio_MQ2 = RS_MQ2 / Ro_MQ2;
	//printf("### %f\r\n",ratio_MQ2);
	//printf("%f\r\n",Ro_MQ2);
	PPM_MQ2 = pow(10, ((log(ratio_MQ2) - CH4Curve[1]) / CH4Curve[2]) + CH4Curve[0]);
	printf("%1.0f", PPM_MQ2);
}

void Calibration_MQ4(float ADC_MQ4)
{
	//printf("MQ4: %1.0f\r\n", ADC_MQ4);
	VRL_MQ4 = (ADC_MQ4 / 4096) * 2.95;
	RS_MQ4 = ((5.0 / VRL_MQ4)- 1) *  RL_MQ4;
	Ro_MQ4 =  RS_MQ4 / 4.4;
	//printf("%f\r\n",Ro_MQ4);
	//printf("%f  %f  %f\r\n",VRL_MQ2, RS_MQ2 , Ro_MQ2);
	//printf("Ro_MQ2 : %f\r\n",Ro_MQ2);
}

void Calculation_MQ4(float ADC_MQ4)
{
	//printf("##%f\r\n",ADC_MQ4);
	Volt_MQ4 = (ADC_MQ4 / 4096) * 2.95;
	//printf("%f\r\n", Volt_MQ4);
	RS_MQ4 = ((5.0 * RL_MQ4) / Volt_MQ4) - RL_MQ4;
	//printf("%f\r\n",RS_MQ4);
	
	ratio_MQ4 = RS_MQ4 / Ro_MQ4;
	//printf("### %f\r\n",ratio_MQ4);
	//printf("%f\r\n",Ro_MQ4);
	PPM_MQ4 = pow(10, ((log(ratio_MQ4) - 1.133) / (-0.318)));
	//PPM_MQ4 = 12.69 * pow(ratio_MQ4, (-0.3727)) + 0.03781;
	printf("CH4: %1.5f\r\n", PPM_MQ4);
}

void Calibration_MQ135(float ADC_MQ135)
{
	//printf("MQ135: %1.0f\r\n",ADC_MQ135);
	//printf("MQ135: %1.0f\r\n",ADC_MQ135);
	VRL_MQ135 = (ADC_MQ135 / 4096) * 2.95;
	RS_MQ135 = ((5.0 / VRL_MQ135)- 1) *  RL_MQ135;
	Ro_MQ135 =  RS_MQ135 / 3.7;
	//printf("%f  %f  %f\r\n",VRL_MQ2, RS_MQ2 , Ro_MQ2);
	//printf("Ro_MQ2 : %f\r\n",Ro_MQ2);
}

void Calculation_MQ135_CO2(float ADC_MQ135)
{
	Volt_MQ135 = (ADC_MQ135 / 4096) * 2.95;
	//printf("Volt_MQ2 :  %f\r\n",Volt_MQ2);
	RS_MQ135 = ((5.0 * RL_MQ135) / Volt_MQ135) - RL_MQ135;
	//printf("%f\r\n",RS_MQ2);
	
	ratio_MQ135 = RS_MQ135 / Ro_MQ135;
	//printf("### %f\r\n",ratio_MQ2);
	//printf("%f\r\n",Ro_MQ2);
	//PPM_MQ135_CO2 = pow(10, ((log(ratio_MQ135) - 0.584) / (-0.204)));   //need to modify
	//PPM_MQ135_CO2 = 114.3544 * pow(ratio_MQ135, (-2.93599));
	PPM_MQ135_CO2 = (146.15 * (2.868 - ratio_MQ135) + 10);
	printf("CO2: %1.0f\r\n", PPM_MQ135_CO2);
}

void Calculation_MQ135_NH3(float ADC_MQ135)
{
	//printf("MQ136: %1.0f\r\n",ADC_MQ135);
	Volt_MQ135 = (ADC_MQ135 / 4096) * 2.95;
	//printf("Volt_MQ2 :  %f\r\n",Volt_MQ135);
	RS_MQ135 = ((5.0 * RL_MQ135) / Volt_MQ135) - RL_MQ135;
	//printf("%f\r\n",RS_MQ2);
	
	ratio_MQ135 = RS_MQ135 / Ro_MQ135;
	//printf("### %f\r\n",ratio_MQ2);
	//printf("%f\r\n",Ro_MQ2);
	//PPM_MQ135_NH3 = pow(10, ((log10(ratio_MQ135) - 0.828) / (-0.414)));
	PPM_MQ135_NH3 = 30.694 * pow(ratio_MQ135, (-2.58818));
	//PPM_MQ135_NH3 = 6.915 * pow(ratio_MQ135, (-0.4394)) + 0.0859;
	printf("NH3: %1.2f\r\n", PPM_MQ135_NH3);
}

void Calibration_MQ136(float ADC_MQ136)
{
	//printf("MQ136: %1.0f\r\n", ADC_MQ136);
	VRL_MQ136 = (ADC_MQ136 / 4096) * 2.95;
	//printf("%f\r\n", VRL_MQ136);
	RS_MQ136 = ((5.0 / VRL_MQ136)- 1) *  RL_MQ136;
	//printf("%f\r\n", RS_MQ136);
	Ro_MQ136 =  RS_MQ136 / 3.7;
	//Ro_MQ136 = (((5.0 / ((ADC_MQ136 / 4096) * 2.81)) - 1) * RL_MQ136) / 13.0;
}


void Calculation_MQ136(float ADC_MQ136)
{
	//printf("##%f\r\n",ADC_MQ136);
	Volt_MQ136 = (ADC_MQ136 / 4096) * 2.95;
	//printf("%1.2f\r\n", Volt_MQ136);
	ratio_MQ136 = ((5.0 * RL_MQ136 / Volt_MQ136) - RL_MQ136) / Ro_MQ136;
	//printf("%f\r\n", ratio_MQ136);
	//printf("%f\r\n", Ro_MQ136);
	PPM_MQ136 = pow(10, ((log(ratio_MQ136) +0.181) / -0.809));
	printf("H2S: %1.5f\r\n", PPM_MQ136);
}

void Calibration_MQ137(float ADC_MQ137)
{
	VRL_MQ137 = (ADC_MQ137 / 4096) * 2.81;
	RS_MQ137 = ((5.0 / VRL_MQ137)- 1) *  RL_MQ137;
	Ro_MQ137 =  RS_MQ137 / 3.6;
	//Ro_MQ137 = (((5.0 / ((ADC_MQ137 / 4096) * 2.81)) - 1) * RL_MQ137) / 3.6;
}

void Calculation_MQ137(float ADC_MQ137)
{
	Volt_MQ137 = (ADC_MQ137 / 4096) * 2.81;
	ratio_MQ137 = ((5.0 * RL_MQ137 / Volt_MQ137) - RL_MQ137) / Ro_MQ137;
	PPM_MQ137 = pow(10, ((log10(ratio_MQ137) - 0.42) / -0.263));
	printf("%1.0f", PPM_MQ137);	
}