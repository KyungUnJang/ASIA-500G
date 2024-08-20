
#include "bsp_adc.h"
#include "cli.h"
#include "motor.h"
#include "hw_config.h"

#define NUMBER_ADC_CHANNEL 4
#define NUMBER_ADC_CHANNEL_AVERAGE_PER_CHANNEL 8

uint16_t ADC_DMA_BUFF[NUMBER_ADC_CHANNEL * NUMBER_ADC_CHANNEL_AVERAGE_PER_CHANNEL]={0};
extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;

#define Speed_MAX_SIZE		5
#define Angle_MAX_SIZE      5

float   outValue; 
float   out_val_angle;
uint8_t angle_value;
uint8_t remote_value;

void get_remote_val();

uint16_t ADC_DMA_AVERAGE(int channel)
{
	uint32_t adc_sum;
	int i;
	
	adc_sum = 0;
	if(channel < NUMBER_ADC_CHANNEL )
	{
		for(i=0; i<NUMBER_ADC_CHANNEL_AVERAGE_PER_CHANNEL; i++)
			adc_sum += ADC_DMA_BUFF[channel+i*NUMBER_ADC_CHANNEL];
	}
	else
		return 1;
	
	return adc_sum/NUMBER_ADC_CHANNEL_AVERAGE_PER_CHANNEL;
}



uint32_t adc_value[3];

void bsp_adc_init()
{
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)ADC_DMA_BUFF, NUMBER_ADC_CHANNEL * NUMBER_ADC_CHANNEL_AVERAGE_PER_CHANNEL);
}

void get_adc_value()
{
	get_remote_val();
}

typedef struct
{
	float angleMPa;
} Angle_MPa_t;

typedef struct
{
	float angleAdc;
} Angle_Adc_t;


Angle_MPa_t angleMpa[Angle_MAX_SIZE] = {
		40,
		63,
		90,
		115,
		140
};

Angle_Adc_t angleAdc[Angle_MAX_SIZE] = {
		300,
		1015,
		1735,
		2610,
		3500
};


typedef struct
{
	float remoteMPa;
} Speed_MPa_t;

typedef struct
{
	float remoteAdc;
} Speed_Adc_t;


Speed_MPa_t remoteMpa[Speed_MAX_SIZE] = {
		100,
		125,
		150,
		175,
		200
};

Speed_Adc_t remoteAdc[Speed_MAX_SIZE] = {
		0,
		1300,
		2000,
		2600,
		3450
};




float map_function(float x, float in_min, float in_max, float out_min, float out_max) 
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


float multiMap(float val, float* _in, float* _out, uint16_t size){
    if (val <= _in[0]) return _out[0];
    if (val >= _in[size-1]) return _out[size-1];
    uint8_t pos = 1; 
    while(val > _in[pos]) pos++;
    if (val == _in[pos]) return _out[pos];
    return map_function(val, _in[pos-1], _in[pos], _out[pos-1], _out[pos]);
}



void get_remote_val()
{
	V_Battery = ((60.5 * ADC_DMA_AVERAGE(0))/4095)-1; 
    outValue = multiMap(ADC_DMA_AVERAGE(1), &remoteAdc, &remoteMpa, Speed_MAX_SIZE);
	remote_value = (uint8_t)outValue ;
	
	if(remote_value <= 120)
		DRV_Speed = 1;
	else if(remote_value <= 140)
		DRV_Speed = 2;
	else if(remote_value <= 160)
		DRV_Speed = 3;
	else if(remote_value <= 180)
		DRV_Speed = 4;
	else
		DRV_Speed = 5;


}

