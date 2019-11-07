/*********************************************************************
** Classification : UNCLASSIFIED                                    **
**********************************************************************
**                                                                  **
** SW Name:           General Purpose SW                            **
** SW Id:             0.0.0.1                                       **
** Module Name:       Counter.c                                     **
** Created on:        14/12/2016                                    **
**                                                                  **
** Author/Company:                                                  **
**                                                                  **
**********************************************************************
**                                                                  **
** Description :                                                    **
** This module implment the counters library                        **
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

/**
 * @file Counters.c
 * @author Andrea Emili
 * @company
 * @date  14/12/2016
 * @brief This module implment the counters library
 * @see
 */


/*AE TODO - The include should be reviewed */
#include <p24FJ128GA310.h>
#include "Counters.h"


/*USER DEFINITION - WRITE THIS PART TO USE THE LIBRARY*/

/*Define here you counter variables for Application*/
counter_us tw_1_counter;    /*1 - Counter Tw ( time windows for latency hysteresis )*/
counter_us tw_2_counter;    /*2 - Counter Tw ( time windows for latency hysteresis )*/
counter_us tp_counter;      /*3 - Counter Tp ( time pulse )*/
counter_us ts_counter;      /*4 - Counter Ts ( time strobe )*/
counter_us t_delay_id_23;   /*5 - conter tdelay for id 23*/
counter_us t_pulse_id_23;   /*6 - conter tpulse for id 23*/
counter_us t_id_24_1;       /*7 - counter for latency id 24 for GPIO 1*/
counter_us t_id_24_2;       /*8 - counter for latency id 24 for GPIO 2*/
counter_us t_id_27;         /*9 - counter for latency id 27 for GPIO 1*/
counter_us t_id_27_tw;      /*10 - counter for latency id 27 for GPIO 1*/
counter_us t_id_22_tw;      /*11 - Twindow for cmd id 22*/
counter_us t_id_28;         /*12 - counter for latency id 28 for GPIO 2*/
counter_us t_id_28_tw;      /*13 - Twindow for cmd id 28*/
counter_us t_id_gpio_lat_1; /*14 - */
counter_us t_id_gpio_lat_2; /*15 - */
counter_us t_id_ext_ext_lat; /*16 - */

/**/

/*Address list of all counters defined, insert here all counters
    1) You have to use at least one counter
    2) Every counter you declare allocate space in memory
    3) Update the COUNTER_NUMBER define
*/
#define COUNTER_NUMBER      16           /* Define the number of counters*/
#define US_OVERFLOW_VALUE   65535     /*us - TMR1 16 bit hw counter*/
counter_us *const counter_list[COUNTER_NUMBER] =
{
    &tw_1_counter,      /*1*/
    &tw_2_counter,      /*2*/
    &tp_counter,        /*3*/
    &ts_counter,        /*4*/
    &t_delay_id_23,     /*5*/
    &t_pulse_id_23,     /*6*/
    &t_id_24_1,         /*7*/
    &t_id_24_2,         /*8*/
    &t_id_27,           /*9*/
    &t_id_27_tw,        /*10*/
    &t_id_22_tw,        /*11*/
    &t_id_28,           /*12*/
    &t_id_28_tw,        /*13*/
    &t_id_gpio_lat_1,   /*14*/
    &t_id_gpio_lat_2,   /*15*/
    &t_id_ext_ext_lat   /*16*/
};


/*
 * Function: Counter_Incrementms
 * Description: This function increment the counter in case of overflow. The counter is attached to a 16 bit hardware counter (1us) 
 * and after 655535us it will go in overflow. The number of overflow is taken to count more than 655535us.
 * Auhtor: Andrea Emili
 *  Notes:
 *  - To be inserted in the Timer ISR to count the ms
 *  - COUNTER_NUMBER: The function performs a scan of all counter available
 *  - COUNTER_START: It the status of the counter is equal to Start the functions increment by 1 the c
 */
void Counter_Incrementms()
{
    uint8_t i = 0;
    /*Insert here the list of counter to check*/

    for(i = 0; i < COUNTER_NUMBER; i++)
    {
        if(counter_list[i]->status == COUNTER_START)
        {
            counter_list[i]->over_flow_number++;
        }

    }
}/*eof*/

/*
 * Function: Counter_InitCounter
 * Description: Init the software counter
 * Auhtor: Andrea Emili
 * Input:
 *  - counter_us* counter --> Pointer to counter defined in the struct counter list
 *  - target --> Target value in us of counter
 * Notes:
 *  - You have to call the start counter to start the counting
 */
void Counter_InitCounter(counter_us* counter, uint32_t target)
{
    counter->target = target;
    counter->over_flow_number = 0;
    counter->elapsed = TIMER_COUNTING;
}/*eof*/

/*
 * Function: Counter_CheckCounterStatus
 * Description: Report the status of the counter
 * Auhtor: Andrea Emili
 *  Notes:
 *  -
 */
uint8_t Counter_CheckCounterStatus(counter_us* counter)
{
    return(counter->status);
}/*eof*/

/*
 * Function: Counter_StartCounter
 * Description: Use this function to start the counter
 * Auhtor: Andrea Emili
 *  Notes:
 *  -
 */
void Counter_StartCounter(counter_us* counter)
{
    counter->status = COUNTER_START;
    counter->start_time = GetSysTimeUs_TMR4();
}/*eof*/

/*
 * Function: Counter_StopCounter
 * Description: Stop a counter
 * Auhtor: Andrea Emili
 *  Notes:
 *  -
 */
void Counter_StopCounter(counter_us* counter)
{
    counter->status = COUNTER_STOP;
}/*eof*/

/*
 * Function: Counter_Elapsed
 * Description: Report if the counter is elapsed or already running
 * Auhtor: Andrea Emili
 *  Notes:
 *  -
 */
uint8_t Counter_Elapsed(counter_us* counter)
{
 uint16_t start_time = 0;
        if(counter->over_flow_number > 0)
        {
            start_time = (US_OVERFLOW_VALUE - counter->start_time );
            if((start_time + (counter->over_flow_number * US_OVERFLOW_VALUE)-(US_OVERFLOW_VALUE - GetSysTimeUs_TMR4())) > counter->target)
            {
                return TIMER_ELAPSED;
            }
            return TIMER_COUNTING;
        }
        else
        {
            if((GetSysTimeUs_TMR4() - counter->start_time ) > counter->target)
            {
                return TIMER_ELAPSED;
            }
            return TIMER_COUNTING;
        }    
}/*eof*/


/*
 * Function: Counter_CountersCheck
 * Description: Check the counter status and change the status if needed
 * Auhtor: Andrea Emili
 *  Notes:
 *  -
 */
void Counter_CountersCheck()
{
    uint8_t i = 0;
    /*Insert here the list of counter to check*/

    for(i = 0; i < COUNTER_NUMBER; i++)
    {
        if(counter_list[i]->status == COUNTER_START)
        {
            if(Counter_Elapsed(counter_list[i]))
            {
                counter_list[i]->elapsed = TIMER_ELAPSED;
                counter_list[i]->status = COUNTER_STOP;
            }
            else
            {
                counter_list[i]->elapsed = TIMER_COUNTING;
            }
        }

    }
}/*eof*/

/*
 * Function: Counter_GetElapsedTime
 * Description: Return the elapsed time
 * Auhtor: Andrea Emili
 *  Notes:
 *  -
 */
uint32_t Counter_GetElapsedTime(counter_us* counter)
{
    uint16_t start_time = 0;

    if(counter->over_flow_number > 0)
    {
            start_time = (US_OVERFLOW_VALUE - counter->start_time );
            return((start_time + (counter->over_flow_number * US_OVERFLOW_VALUE)-(US_OVERFLOW_VALUE - GetSysTimeUs_TMR4())));
    }
    else
    {
        return((GetSysTimeUs_TMR4() - counter->start_time ));
    }
}/*eof*/
