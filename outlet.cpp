/**************************************************************************/
/*!
    File Name:     outlet.cpp
    Author:        Jordan Baxter

    Group: Blue(1) "Reverse Biased"
    Group Members: Jordan Baxter, Suyang Liu, Trenton Kilgore

    Description: This file contains the function declarations for the outlet class.
*/
/**************************************************************************/

#include "outlet.h"

/******************************************************************************/
/*!
    Parameters: Corresponding relay and current sensor pins.
    Description: Constructor
*/
/******************************************************************************/

Outlet::Outlet(int onOffPin, int currentPin)
{
    _onOff = false;
    _timerOnOff = false;
    _onOffPin = onOffPin;
    _currentPin = currentPin;

}

/******************************************************************************/
/*!
    Description: Switch an outlet on or off. If the outlet is on with the timer
                 running, this will turn the timer off as well. Then writes that 
                 value to the pin corresponding to the relay circuit.
*/
/******************************************************************************/
void Outlet::switchOnOff(){
    if(_onOff == true && _timerOnOff == true){ //Checks to see if the outlet and timer are on.
        _timerOnOff = false; //If so, turns timer off.
    }
    _onOff = !_onOff; //Switches the outlet on or off.
    digitalWrite(_onOffPin, _onOff); //Write the _onOff value to the _onOffPin
}
/******************************************************************************/
/*!
    Description: Returns whether the outlet is on or off.
*/
/******************************************************************************/
bool Outlet::getOnOff(){
    return _onOff;
}
/******************************************************************************/
/*!
    Description: Switches the timer off.
*/
/******************************************************************************/
void Outlet::timerCancel(){
    _timerOnOff = false;
}
/******************************************************************************/
/*!
    Description: Returns whether the timer is on or off.
*/
/******************************************************************************/
bool Outlet::getTimerOnOff(){
    return _timerOnOff;
}
/******************************************************************************/
/*!
    Parameters: Desired timer setting (seconds)
    Description: Receives the desired timer settings and records the start time
                 of the timer. Then turns the timer on.
*/
/******************************************************************************/
void Outlet::setTimer(int seconds){
    _timerStart = millis();
    _timerSeconds = seconds;
    _timerOnOff = true;
}
/******************************************************************************/
/*!
    Description: Calculates the time that has elapsed since the timer started.
                 Then calculates the difference between the timer setting and the 
                 amount of time elapsed since the timer was turned on (time remaining), 
                 truncates the result into an integer, and returns the integer result.
*/
/******************************************************************************/
int Outlet::getTimer(){
    unsigned long timeElapsed = millis() - _timerStart;
    int timeRemaining = _timerSeconds - (timeElapsed%1000) + 1;
    return timeRemaining;
}
/******************************************************************************/
/*!
    Description: Reads the current sensor pin and returns the amount of current passing
                 through that sensor in amps.
*/
/******************************************************************************/
float Outlet::getCurrent(){
    //TODO (After I receive interface definition)
    float current = analogRead(_currentPin);
    return current;
}
/******************************************************************************/
/*!
    Description: Checks the status of the timer, if the timer is done it turns 
                 the timer off and returns that it is done.
*/
/******************************************************************************/
void Outlet::timerRun(){
    unsigned long timeElapsed = millis() - _timerStart;
    int timeRemaining = _timerSeconds - (timeElapsed%1000) + 1;
    bool timerDone = timeRemaining <= 0;
    if(timerDone){
        _timerOnOff = false;
    }
}


