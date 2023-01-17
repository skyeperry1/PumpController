
//************************************************************************
//ARDUINO ET-73 MEAT & PIT PROBE LIBRARY 
//By: Skye Perry
//Version 1.0.2  Last Modified: 2016-03-02
//This Library is licensed under a GPLv3 License
//Tested with Maverick Smoker Probes for the ET73. PN: PR-004(smoker probe) & PR-005(meat probe)
//************************************************************************

#ifndef PumpController_h
#define PumpController_h
#define LIBRARY_VERSION 1.1.1

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif
#include <functional>
typedef std::function<void(const int)> Callback;

class PumpController {
    private:
      int _pin; //5v relay controll pin to turn pump on and off
      bool _pumpState; //stores the current on/off state of the pump 
      bool _isPriming;
      int _pumpCycleTime;  //Stores the time the pump should run for in milliseconds
      float _maxFlowRate_60s; //stored in ml/min at pump max voltage
      float _pumpTime_1ml; //stores the time the pump needs to run in order to dispense 1 ml of liquid
      unsigned long _pumpStartTime; //stores the start time of the pump run cycle

      //Callback _cycleCompleteCallBack = NULL;

      //TO IMPLEMENT
      Callback _onStateChangeCallback = NULL;

      bool _calibrationComplete;
      bool _debug;

      //Private Methods
      void on();
      void off();
      bool isDosingCycleComplete();
      int calculateDoseTime(int millilitres);
      bool startPumpCycle(int cycle_time);

    public:
      //Constructor      
      PumpController(int pin, float pump_max_flow_rate);
      //Public Methods
      void update();
      void setCallback(Callback);
      bool prime(bool run_state);
      bool dose(int millilitres); //dispense a set ml of liquid

      //TO IMPLEMENT
      bool calibrate();
      bool isPumping(); //should return the run state of the pump

};

#endif