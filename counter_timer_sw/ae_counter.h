/*********************************************************************
** Classification : UNCLASSIFIED                                    **
**********************************************************************
**                                                                  **
** SW Name:           General Purpose SW                            **
** SW Id:             0.0.0.1                                       **
** Module Name:       Timer2.h                                      **
** Created on:        22/01/2015                                    **
**                                                                  **
** Author/Company:    Andrea Emili                                  **
**                                                                  **
**********************************************************************
**                                                                  **
** Description :                                                    **
**                                                                  **
**                                                                  **
**********************************************************************
**                                                                  **
**                                                                  **
**********************************************************************
**                                                                  **
** Software change report :                                         **
** - None                                                           **
**                                                                  **
**********************************************************************/

#ifndef COUNTER_H
#define	COUNTER_H

#define COUNTER_TARGET_MAX  0xFFFFFFFF

#define COUNTER_FALSE 0
#define COUNTER_TRUE  1

#define COUNTER_START   1
#define COUNTER_STOP    0
#define TIMER_ELAPSED      1
#define TIMER_COUNTING     0

typedef struct {
    uint8_t status;    /*Value for hysteresis on pin movement*/
    uint16_t start_time;
    uint32_t target;
    uint32_t elapsed;
    uint32_t over_flow_number;    /* to manage overflow*/
}counter_us;


void Counter_InitCounter(counter_us* counter, uint32_t target);
uint8_t Counter_CheckCounterStatus(counter_us* counter);
void Counter_StartCounter(counter_us* counter);
void Counter_StopCounter(counter_us* counter);
uint8_t Counter_Elapsed(counter_us* counter);
void Counter_CountersCheck();
void Counter_Incrementms();
uint32_t Counter_GetElapsedTime(counter_us* counter);

/*Extern of application variables*/
extern counter_us tw_1_counter;
extern counter_us tw_2_counter;
extern counter_us tp_counter;
extern counter_us ts_counter;
extern counter_us t_delay_id_23;
extern counter_us t_pulse_id_23;
extern counter_us t_id_24_1;
extern counter_us t_id_24_2;
extern counter_us t_id_24_timeout;
extern counter_us t_id_27;
extern counter_us t_id_27_tw;
extern counter_us t_id_22_tw;
extern counter_us t_id_28;
extern counter_us t_id_28_tw;
extern counter_us t_id_gpio_lat_1;
extern counter_us t_id_gpio_lat_2;
extern counter_us t_id_ext_ext_lat;
/**/


#endif	/* TIMER2_H */


