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

#include <memory>
#include "State.h"

using namespace emb;



Timer * State::getTimer() {

    return &_timer;

}


double State::getTime() const {

    return _timer.time();

}


void State::_activate() {

    // set this to current
    if(_parent != nullptr)
        _parent->_currentState = this;

    // start timer
    _timer.start();

}


void State::_deactivate() {

    // unset current state
    if(_parent != nullptr)
        _parent->_currentState = nullptr;

}


void State::_enter(const Transition *transition) {

    // activate parent
    if(_parent != nullptr && _parent != transition->from->getParent())
        _parent->_enter(transition);

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

    // perform sub-step
    if(_currentState)
        _currentState->step();

}


void State::_exit(const Transition *transition) {

    // run user defined exit state
    if(onLeave)
        onLeave(transition);

    // deactivate state
    _deactivate();

    // deactivate parent
    if(_parent != nullptr && _parent != transition->to->getParent())
        _parent->_exit(transition);

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


void State::initialize() {

    // init parent
    if(_parent != nullptr)
        _parent->initialize();

    // activate state
    this->_activate();

}


State *State::createState() {

    // create state and add to vector
    _states.emplace_back(std::unique_ptr<State>(new State));

    // set parent
    _states.back()->_parent = this;

    // return state
    return _states.back().get();

}


void State::addState(State *state) {

    state->_parent = this;

}


State * State::currentState() const {

    return _currentState;

}


const State * State::getParent() const {

    return _parent;

}