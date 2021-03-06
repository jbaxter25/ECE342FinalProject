/**************************************************************************/
/*!
    File Name:     outlet.cpp
    Author:        Jordan Baxter

    Group: Blue(1) "Reverse Biased"
    Group Members: Jordan Baxter, Suyang Liu, Trenton Kilgore

    Description: This file contains the function declarations for the outlet class.
*/
/**************************************************************************/

#include <string.h>
#include <Arduino.h>

#include "outlet.h"
#include "HW.h"

/******************************************************************************/
/*!
    Parameters: Corresponding relay and current sensor pins.
    Description: Constructor
*/
/******************************************************************************/

Outlet::Outlet(char* name, int onOffPin, int currentPin)
{
    _name = name;
    _onOff = outletOff;
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
    if(_onOff == outletOn && _timerOnOff == true){ //Checks to see if the outlet and timer are on.
        _timerOnOff = false; //If so, turns timer off.
    }
    _onOff = !_onOff; //Switches the outlet on or off.
    digitalWrite(_onOffPin, _onOff); //Write the _onOff value to the _onOffPin
    Serial.print(String(_name));
    if(_onOff == outletOn){
        Serial.println(" is on.");
    }
    else{
        Serial.println(" is off.");
    }
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
    Serial.print(String(_name));
    Serial.println(" timer canceled.");
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
void Outlet::setTimer(uint16_t seconds){
    _timerStart = millis();
    _timerSeconds = seconds;
    if(!_onOff){
        this->switchOnOff();
    }
    _timerOnOff = true;
}
/******************************************************************************/
/*!
    Description: Calculates the time that has elapsed since the timer started.
                 Then calculates the difference between the timer setting and the 
                 amount of time elapsed since the timer was turned on (time remaining), 
                 truncates the result into an integer, if the time remaining 
                 is 0, turns off the timer, then returns the integer result in seconds.
*/
/******************************************************************************/
uint16_t Outlet::getTimeRemaining(){
    if(_timerOnOff){
        unsigned long timeElapsed = millis() - _timerStart;
        int timeRemaining = _timerSeconds - (timeElapsed/1000);
        bool timerDone = timeRemaining <= 0;
        if(timerDone){
         this->switchOnOff();
         return 0;
        }
        Serial.print(String(_name)); //For use with debugging only, comment out for expo.
        Serial.print(F(" has "));
        Serial.print(timeRemaining, DEC);
        Serial.println(F(" seconds remaining."));
        return (uint16_t)timeRemaining;
    } else {
        return 0;
    }

}
/******************************************************************************/
/*!
    Description: Reads the current sensor pin for 250 msec and returns the amount of current passing
                 through that sensor in amps. Based on the set up of the current sense 
                 from the manufacturer and lab tests, we have determined that the waveform
                 generated by the current sensor fluctuates, but its peak value directly correlates
                 to the AC current flowing through the circuit. This is significant because
                 no RMS calculations need to be done in the firmware, only a max value sample.

    ****NOTE**** Everytime this function is called, it will initiate a busywait of 250 msec,
                 do not call this function more than is necessary.
*/
/******************************************************************************/
uint16_t Outlet::getCurrent(){
    int currentSample = 0;
    unsigned long startTime = millis();
    while((millis() - startTime < 50) && currentSample <= 600)
    {
       int sample = analogRead(_currentPin); 
       if(sample > currentSample){
           currentSample = sample;
       } 
    }
    float voltsIn = (currentSample * 3.3/1024) - 1.65;
    float current = voltsIn / .132;
    /*Algorithm explanation: The input reading is biased by half of the reference voltage 
                             at the IC, meaning a 0 AMP current will output 3.3v / 2.  
                             So after reading the pin, we must convert the bit reading
                             to volts and subtract this bias. Then on the next line, we convert
                             the processed input voltage to its corresponding current. (132mV / Amp)
    */
    // Serial.print(String(_name)); //For use with debugging only, comment out for expo.
    // Serial.print(F(" is drawing "));
    // Serial.print(current, DEC);
    // Serial.println(F(" amps."));
    _avgCurrent = .875 * _avgCurrent + .125 * current;
    uint16_t current16 = _avgCurrent * 1000;
    return current16;
}
/******************************************************************************/
/*!
    Description: Checks the status of the timer, if the timer is done it turns 
                 the timer off and returns that it is done.
*/
/******************************************************************************/




