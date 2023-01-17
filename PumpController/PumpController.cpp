#include "Arduino.h"
#include <PumpController.h>


//***************************************************************
//Constructor
//
//
//***************************************************************
PumpController::PumpController(int pin, float pump_max_flow_rate){
  _pin = pin;
  _pumpStartTime = 0;
  _pumpCycleTime = 0;
  
  _maxFlowRate_60s = pump_max_flow_rate;
  _pumpTime_1ml = 60000.00 / _maxFlowRate_60s;

  _calibrationComplete = false;
  pinMode(_pin, OUTPUT);
  off();
}


//***************************************************************
// @private
//
//
//***************************************************************
void PumpController::on(){
  if(!_pumpState){
   digitalWrite(_pin, HIGH);
   _pumpStartTime = millis();
   _pumpState = true;
   if(this->_onStateChangeCallback) this->_onStateChangeCallback(true); //Call the _cycleCompleteCallBack
  }
}


//***************************************************************
// @private
//
//
//
//***************************************************************
void PumpController::off(){
  if(_pumpState){
    digitalWrite(_pin, LOW);
    _pumpCycleTime = 0; //reset cycle timer.. shouldn't be need but for added safetey;
    _pumpState = false;
    _isPriming = false;
    if(this->_onStateChangeCallback) this->_onStateChangeCallback(false); //Call the _cycleCompleteCallBack
  }
}


//***************************************************************
// @private
//
//
//***************************************************************
bool PumpController::isDosingCycleComplete(){
    // Serial.println(_pumpStartTime);
    // Serial.println(millis());
    // Serial.println("*****");
    return (millis() - _pumpStartTime) > _pumpCycleTime;
}


//***************************************************************
//@private
//
//
//***************************************************************
bool PumpController::startPumpCycle(int cycle_time){

  if(_pumpState){
    Serial.println("WARNING - startPumpCycle(): pump is already running cycle");
    return false; //return false if the pump is already running a cycle
  }
  //Check if valid time has been passed to function
  if(cycle_time <= 0){
    Serial.println("WARNING - startPumpCycle(): invalid cycle time passed to function: " + String(cycle_time));
    return false; //return false if the cycle time is invalid
  }
  _pumpCycleTime = cycle_time;
  on();    
  return true;
}


//***************************************************************
//@private
//
//
//***************************************************************
int PumpController::calculateDoseTime(int millilitres){
  return (int)(_pumpTime_1ml * millilitres);
}

//***************************************************************
//@public
//
//
//***************************************************************
bool PumpController::dose(int millilitres) { 
  int dose_time = calculateDoseTime(millilitres);
  return startPumpCycle(dose_time);
}

//***************************************************************
//@public
//
//
//***************************************************************
bool PumpController::prime(bool new_state) { 
  if(new_state && !_isPriming){
    _isPriming = startPumpCycle(30000);
  } else if(!new_state && _isPriming) { //if toggling off only if we're priming
    off();
  }
  return _isPriming;
}

//***************************************************************
//@public
//
//
//***************************************************************
void PumpController::setCallback(Callback callback) { 
  this->_onStateChangeCallback = callback;
}


//***************************************************************
//@public
//
//
//***************************************************************
void PumpController::update(){
  if(!_pumpState){
    return;  //Exit the update if the pump isn't running
  }

  if(isDosingCycleComplete()){
    //if(this->_cycleCompleteCallBack) this->_cycleCompleteCallBack(_pumpCycleTime); //Call the _cycleCompleteCallBack
    off();
  }

}




