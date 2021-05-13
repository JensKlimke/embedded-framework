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
// Created by Jens Klimke on 2021-05-13
//


#ifndef EMBEDDED_FRAMEWORK_STATE_INTERFACE_H
#define EMBEDDED_FRAMEWORK_STATE_INTERFACE_H

#include <vector>
#include "Transition.h"

namespace emb {

    struct Transition; //!< Pre-definition of type state

    typedef std::vector<std::unique_ptr<Transition>> TransitionVector; //!< Type definition for transition vector


    /**
     * @brief Interface for states.
     * Defines basically the transitions and an enter method.
     */
    struct StateInterface {

        TransitionVector transitions; //!< Outgoing transitions

        /**
         * @brief Enters the state, if possible.
         * Choice nodes return false because they cannot be entered.
         * @param The transition via this state is entered
         * @return Flag if can stay in state.
         */
        virtual bool enter(const Transition *) = 0;


        /**
         * Adds a transition to the target state
         * @param condition Condition callback to be checked
         * @param targetState Target state to be reached
         * @return Returns a pointer to the transition added
         */
        virtual const Transition * addTransition(TransitionConditionCallback &&condition, StateInterface *targetState);


        /**
         * @brief Checks the its transitions for a state to be reached.
         * This method uses recursion to find a final state.
         * @return The final transition
         */
        virtual const Transition *checkTransition() const;

    };

}

#endif // EMBEDDED_FRAMEWORK_STATE_INTERFACE_H
