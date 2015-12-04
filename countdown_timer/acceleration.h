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

#ifndef ACCELERATION_H_
#define ACCELERATION_H_

// *************************************************************************************************
// Include section

// *************************************************************************************************
// Prototypes section

// *************************************************************************************************
// Defines section
#define DISPLAY_ACCEL_Y         (1u)			// based on user's view

#define ACCEL_MODE_OFF          (0u)
#define ACCEL_MODE_ON           (1u)

// *************************************************************************************************
// Global Variable section
struct accel
{
    unsigned char mode;                    // ACC_MODE_OFF, ACC_MODE_ON
    unsigned char xyz[3];                  // Sensor raw data, use value xyz[0] - x value
    unsigned short data;                   // Acceleration data in 10 * mgrav

    // added part
    unsigned short count;
};
struct accel sAccel;

// *************************************************************************************************
// Extern section
void reset_acceleration(void);
void start_acceleration();
void do_acceleration_measurement(void);

// added function - Zhen Ling
void accelFunc();

#endif                          /*ACCELERATION_H_ */
