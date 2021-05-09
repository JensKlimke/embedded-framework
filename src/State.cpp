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



double State::getTime() const {

    return _timer.time();

}


void State::init() {

    _activate();

}


void State::_activate() {

    // set this to current
    _parent->currentState = this;

    // start timer
    _timer.start();

}


void State::_deactivate() {

    // unset current state
    _parent->currentState = nullptr;

}


void State::_enter(const Transition *transition) {

    // activate the state
    _activate();

    // run user defined entry function
    if(onEnter)
        onEnter(transition);

}


void State::step() {

    // check transitions
    if(_checkTransitions())
        return;

    // run step
    if(onStep)
        onStep(this);

}


void State::_exit(const Transition *transition) {

    // run user defined exit state
    if(onLeave)
        onLeave(transition);

    // deactivate state
    _deactivate();

}


bool State::_checkTransitions() {

    // iterate over transitions
    for(auto &t : _transitions) {

        if(t->condition(t.get())) {

            // leave current
            _exit(t.get());

            // enter new one
            t->to->_enter(t.get());

            // return with true
            return true;

        }

    }

    // no transition active
    return false;

}


void State::addTransition(TransitionConditionCallback &&condition, State *targetState) {

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