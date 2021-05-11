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
#include "Timer.h"

namespace emb {

    struct State;        //!< Pre-definition of type state
    struct Transition;   //!< Pre-definition of type transition

    typedef std::function<bool (const Transition *transition)> TransitionConditionCallback; //!< Type definition for transition condition callbacks
    typedef std::function<void (const Transition *transition)> StateInterfaceCallback;      //!< Type definition for callbacks when entering or leaving state
    typedef std::function<void (State *state)> StateStepCallback;                           //!< Type definition for callbacks within state
    typedef std::vector<std::unique_ptr<Transition>> TransitionVector;                      //!< Type definition for transition vector
    typedef std::vector<std::unique_ptr<State>> StateVector;                                //!< Type definition for state vector

    struct Transition {

        State *from;          //!< Start node of the transition
        State *to;            //!< End node of the transition

        TransitionConditionCallback condition; //!< Condition to follow the transition

    };

    struct State {

        StateInterfaceCallback onEnter{}; //!< Callback to be called on entry
        StateInterfaceCallback onLeave{}; //!< Callback to be called on exit
        StateStepCallback onStep{};       //!< Callback to be called every performStep


        /**
         * The performStep function for the state
         */
        virtual void step();


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
         * Adds a transition to the target state
         * @param condition Condition callback to be checked
         * @param targetState Target state to be reached
         */
        virtual void addTransition(TransitionConditionCallback &&condition, State *targetState);


        /**
         * Creates a transition to target state with the condition that given time (after) has passed
         * @param after Time to be passed for transition condition
         * @param targetState Target state to be reached
         */
        virtual void addTimedTransition(double after, State *targetState);


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


    protected:


        Timer _timer{};                  //!< The timer (is started with entry)

        State *_parent = nullptr;        //!< The parent state machine

        StateVector _states{};           //!< Vector of states for memory purposes
        TransitionVector _transitions{}; //!< All transitions


        /** Activates the state */
        virtual void _activate();

        /** Deactivates the state */
        virtual void _deactivate();

        /** Call enter function */
        virtual void _enter(const Transition *transition);

        /** Call exit function */
        virtual void _exit(const Transition *transition);

        /** Check the transitions */
        virtual bool _checkTransitions();

        State *_currentState = nullptr;
    };

}

#endif // STATE_MACHINE_STATE_H
