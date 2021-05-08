// Copyright (c) 2021 Jens Klimke.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// Created by Jens Klimke on 2021-05-08
//

#include "State.h"

#include <memory>

using namespace emb;


State *State::_currentState = nullptr;


void State::globalStep() {

    // run step of current state
    if(_currentState != nullptr)
        _currentState->_step();

}


void State::init() {

    // set current
    _setActive();

}


double State::getTime() const {

    return _timer.time();

}


void State::_setActive() {

    // start timer
    _timer.start();

    // set this state to current
    _currentState = this;

}


void State::_enter(const Transition *transition) {

    // set to current and start timer
    _setActive();

    // run user defined entry function
    if(entry)
        entry(transition);

}


void State::_step() {

    // check transitions
    if(_checkTransitions())
        return;

    // run step
    if(step)
        step(this);

}


void State::_exit(const Transition *transition) {

    // run user defined exit state
    if(exit)
        exit(transition);

}


bool State::_checkTransitions() {

    // iterate over transitions
    for(auto &t : _transitions) {

        if(t->condition(t.get())) {

            // leave current
            _exit(t.get());

            // set new current state
            _currentState = t->to;

            // enter new
            t->to->_enter(t.get());

            // return with true
            return true;

        }

    }

    // no transition active
    return false;

}


void State::addTransition(TransitionCallback &&condition, State *targetState) {

    // create and add transition
    _transitions.emplace_back(std::unique_ptr<Transition>(
            new Transition{this, targetState, std::move(condition)}
    ));

}


void State::addTimedTransition(double after, State *targetState) {

    // create transition
    _transitions.emplace_back(std::unique_ptr<Transition>(
            new Transition{this, targetState, [this, after](const Transition *transition) {
                return this->getTime() >= after;
            }}
    ));

}