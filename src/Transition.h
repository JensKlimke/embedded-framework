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

#define ALWAYS_TRUE_CONDITION {[] (const emb::Transition *) { return true; }}
#define ALWAYS_FALSE_CONDITION {[] (const emb::Transition *) { return false; }}

namespace emb {


    /**!< pre-definition of transition */
    class Transition;
    class StateInterface;

    /**!< Type definition of condition callback */
    typedef std::function<bool (const Transition *transition)> TransitionConditionCallback; //!< Type definition for transition condition callbacks


    /**
     * @brief Transition class
     * Transitions are unidirectional links between states and have conditions or events
     */
    struct Transition {

        StateInterface *from;                  //!< Start node of the transition
        StateInterface *to;                    //!< End node of the transition
        TransitionConditionCallback condition; //!< Condition to follow the transition

        /**
         * @brief Follows the transition and returns true if successfully.
         * Successful means that a state is reached in which the state machine can remain. If for example a choice node
         * is the target of the transition and the following transition cannot be followed (due to their conditions),
         * the method returns false and the previous state needs to be activated, if possible.
         * @return
         */
        bool execute() const;


        /**
         * Follows the transitions until final state is found.
         * @return The final transition
         */
        const Transition *follow() const;

    };


}

#endif //EMBEDDED_FRAMEWORK_TRANSITION_H
