// *************************************************************************************************
//
//      Copyright (C) 2009 Texas Instruments Incorporated - http://www.ti.com/
//
//
//        Redistribution and use in source and binary forms, with or without
//        modification, are permitted provided that the following conditions
//        are met:
//
//          Redistributions of source code must retain the above copyright
//          notice, this list of conditions and the following disclaimer.
//
//          Redistributions in binary form must reproduce the above copyright
//          notice, this list of conditions and the following disclaimer in the
//          documentation and/or other materials provided with the
//          distribution.
//
//          Neither the name of Texas Instruments Incorporated nor the names of
//          its contributors may be used to endorse or promote products derived
//          from this software without specific prior written permission.
//
//        THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//        "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
//        LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
//        A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
//        OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
//        SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
//        LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
//        DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
//        THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//        (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//        OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// *************************************************************************************************
// Temperature measurement functions.
// *************************************************************************************************
#include <msp430.h>

/* A macro that generates a bit mask according to a given bit number.
 * Example:
 * - BIT(0) expands to 1 (== 0x01)		== 0000 0001
 * - BIT(3) expands to 8 (== 0x08)		== 0000 1000
 */

#define BIT(x) (1uL << (x))					// uL: unsigned long

// *************************************************************************************************
// Include section

// driver
#include "bmp_as.h"
#include "as.h"

// logic
#include "acceleration.h"

// *************************************************************************************************
// Global Variable section
struct accel sAccel;

// Conversion values from data to mgrav taken from BMA250 datasheet (rev 1.05, figure 4)
	const unsigned short bmp_mgrav_per_bit[7] = { 16, 31, 63, 125, 250, 500, 1000 };

// *************************************************************************************************
// Extern section

// *************************************************************************************************
// @fn          reset_acceleration
// @brief       Reset acceleration variables.
// @param       none
// @return      none
// *************************************************************************************************
void reset_acceleration(void)
{
    // Default mode is off
    sAccel.mode = ACCEL_MODE_OFF;

    sAccel.count = 0;
}

// *************************************************************************************************
// @fn          acceleration_value_is_positive
// @brief       Returns 1 if 2's complement number is positive
// @param       unsigned char value        2's complement number
// @return      unsigned char                      1 = number is positive, 0 = number is negative
// *************************************************************************************************
//to check whether the value is positive or negative
unsigned char acceleration_value_is_positive(unsigned char value)
{
    return ((value & BIT7) == 0);
}

// *************************************************************************************************
// @fn          convert_acceleration_value_to_mgrav
// @brief       Converts measured value to mgrav units
// @param       unsigned char value        g data from sensor
// @return      unsigned short                     Acceleration (mgrav)
// *************************************************************************************************
unsigned short convert_acceleration_value_to_mgrav(unsigned char value)
{
    unsigned short result;
    unsigned char i;

    if (!acceleration_value_is_positive(value))
    {
        // Convert 2's complement negative number to positive number
        value = ~value;
        value += 1;
    }

    result = 0;					// initialize the value of result to 0
    for (i = 0; i < 7; i++)		// sum all the result value gathering from the acceleration reading in bmp_as.c & as.c
		result += ((value & (BIT(i))) >> i) * bmp_mgrav_per_bit[i]; // multiply by the value in bmp_mgrav_per_bit that is predefined for each bit

    return (result);
}

// *************************************************************************************************
// @fn          do_acceleration_measurement
// @brief       Get sensor data and store in sAccel struct
// @param       none
// @return      none
// *************************************************************************************************
void do_acceleration_measurement(void)
{
    // Get data from sensor
    bmp_as_get_data(sAccel.xyz);

}

// *************************************************************************************************
// @fn          start_acceleration
// @brief       Display routine.
// @param       unsigned char line                 LINE1
//                              unsigned char update               DISPLAY_LINE_UPDATE_FULL, DISPLAY_LINE_CLEAR
// @return      none
// *************************************************************************************************
void start_acceleration()
{
    unsigned char raw_data;
    unsigned short accel_data;

    // Show warning if acceleration sensor was not initialised properly
    if (!as_ok)
    {
        __no_operation();
    }
    else
    {

        // Start acceleration sensor
        if (sAccel.mode != ACCEL_MODE_ON)			// if sAccel.mode is in ACCEL_MODE_OFF
        {
            // Clear previous acceleration value
            sAccel.data = 0;

            // Start sensor
            bmp_as_start();

            // Set mode
			sAccel.mode = ACCEL_MODE_ON;
        }

        raw_data = sAccel.xyz[0];               // raw_data is the x value in sAccel.xyz array

        accel_data = convert_acceleration_value_to_mgrav(raw_data) / 10;

        // Filter acceleration
        accel_data = (unsigned short) ((accel_data * 0.2) + (sAccel.data * 0.8));

        // Store average acceleration
        sAccel.data = accel_data;

		// take in the value from bmp_as.c and as.c			-- added by ZL
		// if the user tilts the watch in upright-downwards position (sensor pointing downwards to ground - positive)
        if (acceleration_value_is_positive(raw_data)){
			if (sAccel.data > 70) {					// if the data reading is more than 70, increment the count
                sAccel.count = sAccel.count + 1;
            }
        }
    }
}


void accelFunc() { // added by ZL - to call two other functions that are required for acceleration measurement
	do_acceleration_measurement();
	start_acceleration();
}