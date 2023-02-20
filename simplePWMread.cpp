#include "simplePWMread.hpp"

// test case - fixed pin number
const byte PWM_pin = 9;

const byte MAX_ISR_COUNT = 1;

byte isr_count = 0;
byte isr_pin[MAX_ISR_COUNT];
volatile unsigned int isr_value[MAX_ISR_COUNT];
volatile bool isr_last_state[MAX_ISR_COUNT];
volatile bool isr_trigger_state[MAX_ISR_COUNT];
volatile unsigned long isr_timer[MAX_ISR_COUNT];
volatile unsigned long isr_age[MAX_ISR_COUNT];
volatile unsigned long isr_counter;

void ISR_generic(byte isr) {
    unsigned long now = micros();
    bool state_now = digitalRead(isr_pin[isr]);
    if (state_now != isr_last_state[isr]) {
        if (state_now == isr_trigger_state[isr]) {
            isr_timer[isr] = now;
        } else {
            isr_value[isr] = (unsigned int)(now - isr_timer[isr]);
            isr_age[isr] = now;
        }
        isr_last_state[isr] = state_now;
    }
}


ISR (PCINT0_vect)
 {
	 isr_counter++;
	 ISR_generic(0);
 }  // end of PCINT1_vect


/*void ISR_0() {
    ISR_generic(0);
}
*/

PWM::PWM(byte pin) {
	pin = PWM_pin;
    my_isr = isr_count;
    isr_count++;
    
    isr_pin[my_isr] = pin;
    pinMode(isr_pin[my_isr], INPUT);
}

int PWM::begin(bool measure_pulse_high) {
    pinMode(isr_pin[my_isr], INPUT);

    isr_last_state[my_isr] = digitalRead(isr_pin[my_isr]);
    isr_trigger_state[my_isr] = measure_pulse_high;
    
    switch (my_isr) {
        case 0:
//            attachInterrupt(digitalPinToInterrupt(isr_pin[my_isr]), ISR_0, CHANGE);
//		attachInterrupt(5, ISR_0, CHANGE);
		PCIFR  |= bit (PCIF0);   // clear any outstanding interrupts
		PCICR |= 0x01;
		PCMSK0 |= PCINT1;
            break;
/*	    
        case 1:
            attachInterrupt(digitalPinToInterrupt(isr_pin[my_isr]), ISR_1, CHANGE);
            break;
        case 2:
            attachInterrupt(digitalPinToInterrupt(isr_pin[my_isr]), ISR_2, CHANGE);
            break;
        case 3:
            attachInterrupt(digitalPinToInterrupt(isr_pin[my_isr]), ISR_3, CHANGE);
            break;
        case 4:
            attachInterrupt(digitalPinToInterrupt(isr_pin[my_isr]), ISR_4, CHANGE);
            break;
        case 5:
            attachInterrupt(digitalPinToInterrupt(isr_pin[my_isr]), ISR_5, CHANGE);
            break;
        case 6:
            attachInterrupt(digitalPinToInterrupt(isr_pin[my_isr]), ISR_6, CHANGE);
            break;
        case 7:
            attachInterrupt(digitalPinToInterrupt(isr_pin[my_isr]), ISR_7, CHANGE);
            break;
        case 8:
            attachInterrupt(digitalPinToInterrupt(isr_pin[my_isr]), ISR_8, CHANGE);
            break;
        case 9:
            attachInterrupt(digitalPinToInterrupt(isr_pin[my_isr]), ISR_9, CHANGE);
            break;
        case 10:
            attachInterrupt(digitalPinToInterrupt(isr_pin[my_isr]), ISR_10, CHANGE);
            break;
        case 11:
            attachInterrupt(digitalPinToInterrupt(isr_pin[my_isr]), ISR_11, CHANGE);
            break;
        case 12:
            attachInterrupt(digitalPinToInterrupt(isr_pin[my_isr]), ISR_12, CHANGE);
            break;
        case 13:
            attachInterrupt(digitalPinToInterrupt(isr_pin[my_isr]), ISR_13, CHANGE);
            break;
        case 14:
            attachInterrupt(digitalPinToInterrupt(isr_pin[my_isr]), ISR_14, CHANGE);
            break;
        case 15:
            attachInterrupt(digitalPinToInterrupt(isr_pin[my_isr]), ISR_15, CHANGE);
            break;
        case 16:
            attachInterrupt(digitalPinToInterrupt(isr_pin[my_isr]), ISR_16, CHANGE);
            break;
        case 17:
            attachInterrupt(digitalPinToInterrupt(isr_pin[my_isr]), ISR_17, CHANGE);
            break;
        case 18:
            attachInterrupt(digitalPinToInterrupt(isr_pin[my_isr]), ISR_18, CHANGE);
            break;
        case 19:
            attachInterrupt(digitalPinToInterrupt(isr_pin[my_isr]), ISR_19, CHANGE);
            break;
*/
        default:
            return -1; // Error.
    }
    return 0; // Success.
}

unsigned int PWM::getValue() {
    return isr_value[my_isr];
}

void PWM::end() {
    detachInterrupt(digitalPinToInterrupt(isr_pin[my_isr]));
}

unsigned long PWM::getAge() {
    return (micros() - isr_age[my_isr]);
}

unsigned long PWM::getCounter() {
	return isr_counter;
}
