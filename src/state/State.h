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
// Created by Jens Klimke on 2021-04-21.
//


#ifndef STATE_MACHINE_STATE_H
#define STATE_MACHINE_STATE_H

#include <functional>
#include <memory>
#include <vector>
#include "Timer.h"

namespace emb {

    struct State;      //!< Pre-definition of type state
    struct Transition; //!< Pre-definition of type transition

    typedef std::function<bool (const Transition *transition)> TransitionCallback;     //!< Type definition for transition condition callbacks
    typedef std::function<void (const Transition *transition)> StateInterfaceCallback; //!< Type definition for callbacks when entering or leaving state
    typedef std::function<void (State *state)> StateCallback;                          //!< Type definition for callbacks within state
    typedef std::vector<std::unique_ptr<Transition>> TransitionVector;                 //!< Type definition for transition vector

    struct Transition {

        State *from;          //!< Start node of the transition
        State *to;            //!< End node of the transition
        TransitionCallback condition; //!< Condition to follow the transition

    };

    struct State {

        StateInterfaceCallback entry{}; //!< Callback to be called on entry
        StateInterfaceCallback exit{};  //!< Callback to be called on exit
        StateCallback step{};           //!< Callback to be called every step


        /**
         * @brief Sets the state to the initial state
         * Sets the state to current state without entry
         * Starts timer
         */
        void init();


        /**
         * Returns the time since last entry (or init, if so) in seconds
         * @return State time in seconds
         */
        double getTime() const;


        /**
         * Performs a global state step (Checks all conditions within the actual state.
         */
        static void globalStep();


        /**
         * Adds a transition to the target state
         * @param condition Condition callback to be checked
         * @param targetState Target state to be reached
         */
        void addTransition(TransitionCallback &&condition, State *targetState);


        /**
         * Creates a transition to target state with the condition that given time (after) has passed
         * @param after Time to be passed for transition condition
         * @param targetState Target state to be reached
         */
        void addTimedTransition(double after, State *targetState);


    protected:

        Timer _timer{};                  //!< The timer (is started with entry)
        static State *_currentState;     //!< The actual state
        TransitionVector _transitions{}; //!< All transitions

        /** Sets the state as current one and starts timer */
        void _setActive();

        /** Call enter function */
        void _enter(const Transition *transition);

        /** Call step function */
        void _step();

        /** Call exit function */
        void _exit(const Transition *transition);

        /** Check the transitions */
        bool _checkTransitions();

    };

}

#endif // STATE_MACHINE_STATE_H
