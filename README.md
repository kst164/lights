A simple smart light controller I tried to make.

A single master (a raspberry pi running `./node-app`) controls multiple microcontrollers, each of
which can control multiple lights (ESPs running `./lights-base`). All requests to change lights are
made to the master RPi using http requests, and changes are communicated to the ESPs. Alternately,
buttons connected to individual ESPs can control lights (buttons on an ESP need not control a
light on that specific ESP).
