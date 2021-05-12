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
// Created by Jens Klimke on 2021-05-12.
// Contributors:
//


#ifndef EMBEDDED_FRAMEWORK_TRANSITION_H
#define EMBEDDED_FRAMEWORK_TRANSITION_H

#include <functional>

namespace emb {

    /**!< pre-definition of state */
    class State;

    /**!< pre-definition of transition */
    class Transition;

    /**!< Type definition of condition callback */
    typedef std::function<bool (const Transition *transition)> TransitionConditionCallback; //!< Type definition for transition condition callbacks


    /**
     * @brief Transition class
     * Transitions are unidirectional links between states and have conditions or events
     */
    struct Transition {

    protected:

        State *_from = nullptr; //!< Start node of the transition
        State *_to = nullptr;   //!< End node of the transition

        TransitionConditionCallback _condition; //!< Condition to follow the transition

    public:

        /**
         * Constructor to create transition
         * @param from Source state
         * @param to Target state
         * @param condition Condition callback
         */
        Transition(State *from, State *to, TransitionConditionCallback &&condition);


        /**
         * @brief Check the transition.
         * Deactivates the source and activate the destination when positive.
         * @return True when positive
         */
        bool check() const;


        /**
         * Forces the transition
         */
        void execute() const;


        /**
         * Returns the source state
         * @return Source state
         */
        State *from() const;


        /**
         * Returns the target state
         * @return
         */
        State *to() const;

    };


}

#endif //EMBEDDED_FRAMEWORK_TRANSITION_H
