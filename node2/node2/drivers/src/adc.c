#include "../include/ADC.h"
#include "../include/time.h"

Time last_time;
Time GOAL_TIME_TRESHOLD = (Time){.msecs = 500};

void adc_init(){
	PMC->PMC_PCER1 |= PMC_PCER1_PID37;
	ADC->ADC_MR = ADC_MR_FREERUN_ON | ADC_MR_PRESCAL(32);

	//Enable the specific ADC channel for AD0
	ADC->ADC_CHER = ADC_CHER_CH0; // Enable channel 0 (AD0 pin)

	//Enable interrupt for low threshold comparison
	ADC->ADC_CWR = ADC_CWR_LOWTHRES(ADC_THRESHOLD); //threshold for low-level comparison
	ADC->ADC_IER = ADC_IER_COMPE; //Enable comparison event interrupt
	
	NVIC_EnableIRQ(ADC_IRQn);
	ADC->ADC_CR = ADC_CR_START;
}

void ADC_Handler(void) {
	if (ADC->ADC_ISR & ADC_ISR_COMPE) { // Check if the comparison event triggered the interrupt
		if (time_now() - time_combine(last_time) >= time_combine(GOAL_TIME_TRESHOLD)){
			last_time = time_split(time_now());
			uint32_t result = ADC->ADC_CDR[0];
			CanMsg can_msg = (CanMsg){.id=CAN_ID_GOAL, .length=1, .unsigned_data={(uint8_t)result}};
			can_tx(can_msg);
		}
	}
	//ADC->ADC_CR |= ADC_CR_START;
}