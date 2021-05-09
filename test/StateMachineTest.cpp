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
// Created by Jens Klimke on 2021-05-08.
//

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <gtest/gtest.h>
#include <StateMachine.h>

#ifndef EPS_TIME
#define EPS_TIME 1e-2
#endif

using namespace emb;

class StateMachineTest : public ::testing::Test, public StateMachine, public State {

protected:

    void SetUp() override {

        this->addState(this);

    }

};


TEST_F(StateMachineTest, Stepping) {

    // counters
    unsigned int entryCount = 0;
    unsigned int stepCount  = 0;
    unsigned int exitCount  = 0;

    this->onEnter = [this, &entryCount] (const Transition *transition) {
        entryCount++;
        EXPECT_EQ(this, transition->to);
    };

    this->onStep = [this, &stepCount] (State *state) {
        stepCount++;
        EXPECT_EQ(this, state);
    };

    this->onLeave = [this, &exitCount] (const Transition *transition) {
        exitCount++;
        EXPECT_EQ(this, transition->from);
    };

    // create start and end state
    auto start = this->createState();
    auto end = this->createState();

    // define flags for transitions
    bool fromStart = false;
    bool toEnd = false;
    bool backToStart = false;

    // set start to this transition
    start->addTransition([this, &start, &fromStart](const Transition *transition){

        // check
        EXPECT_EQ(start, transition->from);
        EXPECT_EQ(this, transition->to);

        // return
        return fromStart;

    }, this);

    // set this to end transition
    this->addTransition([this, &end, &toEnd](const Transition *transition){

        // check
        EXPECT_EQ(this, transition->from);
        EXPECT_EQ(end, transition->to);

        // return
        return toEnd;

    }, end);

    // set end back to start transition
    end->addTransition([&end, &start, &backToStart](const Transition *transition){

        // check
        EXPECT_EQ(end, transition->from);
        EXPECT_EQ(start, transition->to);

        // return
        return backToStart;

    }, start);

    // initialize
    initialize(start);

    // check time
    EXPECT_NEAR(0.0, start->getTime(), 1e-2);

    // perform step
    StateMachine::StateMachine::step();
    EXPECT_EQ(start, currentState);

    // perform step
    StateMachine::step();
    EXPECT_EQ(start, currentState);

    // set condition
    fromStart = true;

    // perform step
    StateMachine::step();
    EXPECT_EQ(this, currentState);

    // perform step
    StateMachine::step();
    EXPECT_EQ(this, currentState);

    // set condition
    toEnd = true;

    // perform step
    StateMachine::step();
    EXPECT_EQ(end, currentState);

    // perform step
    StateMachine::step();
    EXPECT_EQ(end, currentState);


    // set condition
    backToStart = true;

    // perform step
    StateMachine::step();
    EXPECT_EQ(start, currentState);

    // perform step
    StateMachine::step();
    EXPECT_EQ(this, currentState);

    // perform step
    StateMachine::step();
    EXPECT_EQ(end, currentState);

}


TEST_F(StateMachineTest, CustomState) {

    // define custom state type
    struct CustomState : public State {
        int valueToBeChanged = 0;
    } dynamicState;

    // add state
    this->addState(&dynamicState);

    // create start state
    this->addTransition([](const Transition *) { return true; }, &dynamicState);

    // set transition back
    dynamicState.addTransition([](const Transition* transition) {
        return ((CustomState*) transition->from)->valueToBeChanged >= 10;
    }, this);

    // set incrementer
    dynamicState.onEnter = [] (const Transition *transition) {
        ((CustomState*) transition->to)->valueToBeChanged = 5;
    };

    // set incrementer
    dynamicState.onStep = [] (State *state) {
        ((CustomState*) state)->valueToBeChanged++;
    };

    // stop condition
    bool stop = false;
    this->onEnter = [&stop](const Transition *transition) {
        stop = true;
    };

    // init state machine
    initialize(this);

    // run
    unsigned int steps = 0;
    while(!stop) {
        StateMachine::step();
        steps++;
    }

    // check
    EXPECT_EQ(10, dynamicState.valueToBeChanged);
    EXPECT_EQ(7, steps);

}


TEST_F(StateMachineTest, Timing) {

    // create objects: timer and init state
    Timer timer{};
    auto start = this->createState();

    // add timed transition
    start->addTimedTransition(0.1, this);
    this->addTimedTransition(0.3, start);

    // add entry checker
    this->onEnter = [&timer](const Transition *transition) {

        // check global time
        EXPECT_NEAR(0.1, timer.time(), EPS_TIME);

    };

    // add exit checker
    this->onLeave = [&timer](const Transition *transition) {

        // check global time
        EXPECT_NEAR(0.4, timer.time(), EPS_TIME);

    };

    // set stop trigger
    bool stop = false;
    start->onEnter = [&stop](const Transition *transition) {
        stop = true;
    };

    // start timer and init state machine
    timer.start();
    initialize(start);

    // run
    while(!stop)
        StateMachine::step();

    // check total time
    EXPECT_NEAR(0.4, timer.time(), EPS_TIME);

}

#pragma clang diagnostic pop
