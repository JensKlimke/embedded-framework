// Copyright (c) 2021 Jens Klimke. All rights reserved.
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

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <gtest/gtest.h>
#include <State.h>

#ifndef EPS_TIME
#define EPS_TIME 1e-2
#endif

using namespace emb;

class StateMachineTest : public ::testing::Test, public State {};


TEST_F(StateMachineTest, Stepping) {

    // counters
    unsigned int entryCount = 0;
    unsigned int stepCount  = 0;
    unsigned int exitCount  = 0;

    this->entry = [this, &entryCount] (const Transition *transition) {
        entryCount++;
        EXPECT_EQ(this, transition->to);
    };

    this->step = [this, &stepCount] (State *state) {
        stepCount++;
        EXPECT_EQ(this, state);
    };

    this->exit = [this, &exitCount] (const Transition *transition) {
        exitCount++;
        EXPECT_EQ(this, transition->from);
    };

    // define start and end state
    State start{};
    State end{};

    // define flags for transitions
    bool fromStart = false;
    bool toEnd = false;
    bool backToStart = false;

    // set start to this transition
    start.addTransition([this, &start, &fromStart](const Transition *transition){

        // check
        EXPECT_EQ(&start, transition->from);
        EXPECT_EQ(this, transition->to);

        // return
        return fromStart;

    }, this);

    // set this to end transition
    this->addTransition([this, &end, &toEnd](const Transition *transition){

        // check
        EXPECT_EQ(this, transition->from);
        EXPECT_EQ(&end, transition->to);

        // return
        return toEnd;

    }, &end);

    // set end back to start transition
    end.addTransition([&end, &start, &backToStart](const Transition *transition){

        // check
        EXPECT_EQ(&end, transition->from);
        EXPECT_EQ(&start, transition->to);

        // return
        return backToStart;

    }, &start);

    // initialize
    start.init();

    // check time
    EXPECT_NEAR(0.0, start.getTime(), 1e-2);

    // perform step
    globalStep();
    EXPECT_EQ(&start, _currentState);

    // perform step
    globalStep();
    EXPECT_EQ(&start, _currentState);

    // set condition
    fromStart = true;

    // perform step
    globalStep();
    EXPECT_EQ(this, _currentState);

    // perform step
    globalStep();
    EXPECT_EQ(this, _currentState);

    // set condition
    toEnd = true;

    // perform step
    globalStep();
    EXPECT_EQ(&end, _currentState);

    // perform step
    globalStep();
    EXPECT_EQ(&end, _currentState);


    // set condition
    backToStart = true;

    // perform step
    globalStep();
    EXPECT_EQ(&start, _currentState);

    // perform step
    globalStep();
    EXPECT_EQ(this, _currentState);

    // perform step
    globalStep();
    EXPECT_EQ(&end, _currentState);

}


TEST_F(StateMachineTest, CustomState) {

    // define custom state type
    struct CustomState : public State {
        int valueToBeChanged = 0;
    } dynamicState;

    // create start state
    this->addTransition([](const Transition *) { return true; }, &dynamicState);

    // set transition back
    dynamicState.addTransition([](const Transition* transition) {
        return ((CustomState*) transition->from)->valueToBeChanged >= 10;
    }, this);

    // set incrementer
    dynamicState.entry = [] (const Transition *transition) {
        ((CustomState*) transition->to)->valueToBeChanged = 5;
    };

    // set incrementer
    dynamicState.step = [] (State *state) {
        ((CustomState*) state)->valueToBeChanged++;
    };

    // stop condition
    bool stop = false;
    this->entry = [&stop](const Transition *transition) {
        stop = true;
    };

    // init state machine
    this->init();

    // run
    unsigned int steps = 0;
    while(!stop) {
        State::globalStep();
        steps++;
    }

    // check
    EXPECT_EQ(10, dynamicState.valueToBeChanged);
    EXPECT_EQ(7, steps);

}


TEST_F(StateMachineTest, Timing) {

    // create objects: timer and init state
    Timer timer{};
    State start{};

    // add timed transition
    start.addTimedTransition(0.1, this);
    this->addTimedTransition(0.3, &start);

    // add entry checker
    this->entry = [&timer](const Transition *transition) {

        // check global time
        EXPECT_NEAR(0.1, timer.time(), EPS_TIME);

    };

    // add exit checker
    this->exit = [&timer](const Transition *transition) {

        // check global time
        EXPECT_NEAR(0.4, timer.time(), EPS_TIME);

    };

    // set stop trigger
    bool stop = false;
    start.entry = [&stop](const Transition *transition) {
        stop = true;
    };

    // start timer and init state machine
    timer.start();
    start.init();

    // run
    while(!stop)
        State::globalStep();

    // check total time
    EXPECT_NEAR(0.4, timer.time(), EPS_TIME);

}

#pragma clang diagnostic pop
