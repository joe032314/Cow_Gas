#include <stdint.h>
#define ADC_ratio 0.0006860351
#define         RO_CLEAN_AIR_FACTOR      9.8
#define         READ_SAMPLE_TIMES        50
#define         CH4                      1
#define         RL_MQ135                 10.0
#define         RL_MQ136                 20.0
#define         RL_MQ137                 47.0
#define         RL_MQ2                   10.0
#define         RL_MQ4                   10.0

void MG811_getValue(int sample_time);
void MG811_convert(float Voltage);

void Calculation_MQ2(float ADC_MQ2);
void Calculation_MQ4(float ADC_MQ4);
void Calculation_MQ135_CO2(float ADC_MQ135);
void Calculation_MQ135_NH3(float ADC_MQ135);
void Calculation_MQ136(float ADC_MQ136);
void Calculation_MQ137(float ADC_MQ137);

void Calibration_MQ2(float ADC_MQ2);
void Calibration_MQ4(float ADC_MQ4);
void Calibration_MQ135(float ADC_MQ135);
void Calibration_MQ136(float ADC_MQ136);
void Calibration_MQ137(float ADC_MQ137);
