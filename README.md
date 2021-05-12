# README

[![Build Status](https://travis-ci.com/JensKlimke/embedded-framework.svg?branch=main)](https://travis-ci.com/JensKlimke/embedded-framework)

A very small framework for embedded applications. Can be used for Arduino applications. Currently containing

# Features already implemented

* Very simple logical state machine implementation with timer functionality.


## Next steps

- [ ] Clean up workflow
- [ ] Start state and end state
- [ ] Check transition before entering
- [ ] Run step after entering
- [ ] Multiple chained transitions
- [ ] Events for transition (is valid only for one step)

## Features to be implemented

- [ ] MQTT wrapper
- [ ] JSON constructor and parser wrapper
- [ ] Filter function based on time
- [ ] Arduino implementations

# Examples

## Coffee Timer

@startuml
state NoExtraction {
  Idle : Entry: display off / reset saved time
  Paused : Entry: save extraction time
  Paused --> Idle : delay
}

Extraction --> Paused : EvPumpOff
NoExtraction --> Extraction : EvPumpOn
Extraction : Entry: resume with saved time
Extraction : Step: display Time

[*] --> Idle
@enduml
