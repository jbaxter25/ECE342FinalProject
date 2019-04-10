/**************************************************************************/
/*!
    File Name:     outlet.h
    Author:        Jordan Baxter

    Group: Blue(1) "Reverse Biased"
    Group Members: Jordan Baxter, Suyang Liu, Trenton Kilgore

    Description: This file contains the function prototypes for the outlet class.
*/
/**************************************************************************/

#ifndef _OUTLET_H_
#define _OUTLET_H_

#include <stdint.h>
#include <Arduino.h>

class Outlet
{
    protected:
        bool _onOff;
        bool _timerOnOff;
        int _timerMinutes;
        int _timerSeconds;
        float _current;
        unsigned long timerStart;

    public:
        Outlet();
        void switchOnOff();
        bool getOnOff();
        void timerOnOff();
        bool getTimerOnOff();
        void setTimer(int minutes, int seconds);
        int getTimer();
        void updateTimer();
        float getCurrent();
        unsigned long getTimerStart();
};

#endif /*_OUTLET_H_*/