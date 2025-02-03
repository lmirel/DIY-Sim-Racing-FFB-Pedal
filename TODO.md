# Todo

ESP code:
- [ ] Add automatic system identification of pedal response
- [ ] Add model-predictive-control to the ESP code for the improved pedal response
- [x] Add field to invert motor and losdcell direction
- [x] send joystick data to simhub plugin and provide data as vJoy gamecontroller
- [x] allow effects to move stepper beyond configured max/min position, but not the measured homing positions
- [x] Optimize iSV57 communication
  - [ ] Let the communication task run from the beginning of the setup routine
  - [x] Read pedal state every cycle (currently, the pedal performance is degraded)

      
SimHub plugin:
- [ ] Send SimHub data via wifi to ESP 
- [x] GUI design improvements for the SimHub plugin 
- [x] JSON deserialization make compatible with older revisions
- [ ] include the types header file and use it
- [ ] Make use of effects from the ShakeIt plugin
- [ ] add OTA update for esp firmware
- [x] automatic serial monitor update
- [ ] serial plotter
- [x] add different abs effect patterns, e.g. sawtooth
- [x] make effects proportional to force or travel selectable by dropdown menu
      
Misc:
- [ ] Create a video describing the build progress and the features
- [ ] Add Doxygen + Graphviz to the project to automatically generate documentation, architectural design overview, etc.
