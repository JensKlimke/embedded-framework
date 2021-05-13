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
// Created by Jens Klimke on 2021-05-08.
//


#ifndef STATE_MACHINE_STATE_H
#define STATE_MACHINE_STATE_H

#include <functional>
#include <memory>
#include <vector>
#include <iostream>
#include "Event.h"
#include "Timer.h"
#include "Transition.h"

namespace emb {

    struct State;        //!< Pre-definition of type state

    typedef std::function<void (const Transition *transition)> StateInterfaceCallback;      //!< Type definition for callbacks when entering or leaving state
    typedef std::function<void (State *state)> StateStepCallback;                           //!< Type definition for callbacks within state
    typedef std::vector<std::unique_ptr<State>> StateVector;                                //!< Type definition for state vector


    /**
     * @brief Class for complex states
     * These states implement the following features
     * - timer which is started when entering the state
     * - enter, step and leave callbacks to be set
     * - step function can be executed in real-time
     * - belongs to a parent state
     */
    struct State {

        StateInterfaceCallback onEnter{}; //!< Callback to be called on entry
        StateInterfaceCallback onLeave{}; //!< Callback to be called on exit
        StateStepCallback onStep{};       //!< Callback to be called every performStep


        /**
         * The performStep function for the state
         */
        virtual void step();


        /**
         * Call enter function
         */
        virtual void enter(const Transition *transition);


        /**
         * Call exit function
         */
        virtual void exit(const Transition *transition);


        /**
         * Returns the timer of the state
         * @return The timer of the state
         */
        virtual Timer * getTimer();


        /**
         * Returns the time of the state since activation in seconds
         * @return Time of the state
         */
        virtual double getTime() const;


        /**
         * Creates a transition to target state with the condition that given time (after) has passed
         * @param after Time to be passed for transition condition
         * @param targetState Target state to be reached
         */
        virtual void addTimedTransition(double after, State *targetState);


        /**
         * Adds an event based transition
         * @param targetState Target state
         * @return The event
         */
        virtual Event addEventTransition(State *targetState);


        /**
         * Sets this state to current state
         */
        void initialize();


        /**
         * Creates a state in the state machine
         * @return The created state
         */
        virtual State *createState();


        /**
         * Adds a state to the state machine
         * @param state State to be added
         */
        virtual void addState(State *state);


        /**
         * Return current sub-state
         * @return Current sub-state
         */
        virtual State *currentState() const;


        /**
         * Returns the parent state
         * @return Parent state
         */
        virtual const State *getParent() const;


        /**
         * @brief Sets the time step size for each step.
         * Delays the step function until the time step size is reached. Also takes into account the run-time of the
         * step function content. The accuracy is 10-times the time step size.
         * @param timeStepSize
         */
        virtual void setTimeStepSize(double timeStepSize);


    protected:


        Timer _timer{};                  //!< The timer (is started with entry)
        double _timeStepSize = 0.0;      //!< The time step size of a step (is just delayed)

        State *_parent = nullptr;        //!< The parent state machine
        State *_currentState = nullptr;  //!< The currently active child state

        StateVector _states{};           //!< Vector of states for memory purposes


        /** Activates the state */
        virtual void _activate();

        /** Deactivates the state */
        virtual void _deactivate();


    };

}

#endif // STATE_MACHINE_STATE_H
