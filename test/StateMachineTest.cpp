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
#include <State.h>

#ifndef EPS_TIME
#define EPS_TIME 1e-2
#endif

using namespace emb;

class StateMachineTest : public ::testing::Test, public State {

};


TEST_F(StateMachineTest, Stepping) {

    // counters
    unsigned int entryCount = 0;
    unsigned int stepCount  = 0;
    unsigned int exitCount  = 0;

    // create start, middle and end state
    auto start = createState();
    auto end = createState();
    auto middle = createState();

    middle->onEnter = [&middle, &entryCount] (const Transition *transition) {
        entryCount++;
        EXPECT_EQ(middle, transition->to());
    };

    middle->onStep = [&middle, &stepCount] (State *state) {
        stepCount++;
        EXPECT_EQ(middle, state);
    };

    middle->onLeave = [&middle, &exitCount] (const Transition *transition) {
        exitCount++;
        EXPECT_EQ(middle, transition->from());
    };

    // define flags for transitions
    bool fromStart = false;
    bool toEnd = false;
    bool backToStart = false;

    // set start to middle transition
    start->addTransition([&middle, &start, &fromStart](const Transition *transition){

        // check
        EXPECT_EQ(start, transition->from());
        EXPECT_EQ(middle, transition->to());

        // return
        return fromStart;

    }, middle);

    // set middle to end transition
    middle->addTransition([&middle, &end, &toEnd](const Transition *transition){

        // check
        EXPECT_EQ(middle, transition->from());
        EXPECT_EQ(end, transition->to());

        // return
        return toEnd;

    }, end);

    // set end back to start transition
    end->addTransition([&end, &start, &backToStart](const Transition *transition){

        // check
        EXPECT_EQ(end, transition->from());
        EXPECT_EQ(start, transition->to());

        // return
        return backToStart;

    }, start);

    // initialize
    start->initialize();

    // check time
    EXPECT_NEAR(0.0, start->getTime(), 1e-2);

    // perform step
    step();
    EXPECT_EQ(start, _currentState);

    // perform step
    step();
    EXPECT_EQ(start, _currentState);

    // set condition
    fromStart = true;

    // perform step
    step();
    EXPECT_EQ(middle, _currentState);

    // perform step
    step();
    EXPECT_EQ(middle, _currentState);

    // set condition
    toEnd = true;

    // perform step
    step();
    EXPECT_EQ(end, _currentState);

    // perform step
    step();
    EXPECT_EQ(end, _currentState);


    // set condition
    backToStart = true;

    // perform step
    step();
    EXPECT_EQ(start, _currentState);

    // perform step
    step();
    EXPECT_EQ(middle, _currentState);

    // perform step
    step();
    EXPECT_EQ(end, _currentState);

}


TEST_F(StateMachineTest, CustomState) {

    // define custom state type
    struct CustomState : public State {
        int valueToBeChanged = 0;
    } dynamicState;

    // add state
    addState(&dynamicState);
    auto middle = createState();

    // create start state
    middle->addTransition([](const Transition *) { return true; }, &dynamicState);

    // set transition back
    dynamicState.addTransition([](const Transition* transition) {
        return ((CustomState*) transition->from())->valueToBeChanged >= 10;
    }, middle);

    // set incrementer
    dynamicState.onEnter = [] (const Transition *transition) {
        ((CustomState*) transition->to())->valueToBeChanged = 5;
    };

    // set incrementer
    dynamicState.onStep = [] (State *state) {
        ((CustomState*) state)->valueToBeChanged++;
    };

    // stop condition
    bool stop = false;
    middle->onEnter = [&stop](const Transition *transition) {
        stop = true;
    };

    // init state machine
    middle->initialize();

    // run
    unsigned int steps = 0;
    while(!stop) {
        step();
        steps++;
    }

    // check
    EXPECT_EQ(10, dynamicState.valueToBeChanged);
    EXPECT_EQ(7, steps);

}


TEST_F(StateMachineTest, Timing) {

    // create objects: timer and init state
    Timer timer{};
    auto start = createState();
    auto middle = createState();

    // add timed transition
    start->addTimedTransition(0.1, middle);
    middle->addTimedTransition(0.3, start);

    // add entry checker
    middle->onEnter = [&timer](const Transition *transition) {

        // check global time
        EXPECT_NEAR(0.1, timer.time(), EPS_TIME);

    };

    // add exit checker
    middle->onLeave = [&timer](const Transition *transition) {

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
    start->initialize();

    // run
    while(!stop)
        step();

    // check total time
    EXPECT_NEAR(0.4, timer.time(), EPS_TIME);

}


TEST_F(StateMachineTest, ManipulateTimer) {

    // add state
    auto start = createState();
    auto state = createState();

    // add transition
    start->addTransition([] (const Transition *) { return true; }, state);

    // manipulate timer
    state->onEnter = [] (const Transition *t) {
        t->to()->getTimer()->startWithOffset(100.0);
    };

    // initialize state
    start->initialize();

    // step
    step();

    // check time
    EXPECT_EQ(state, currentState());
    EXPECT_NEAR(100.0, state->getTime(), 1e-3);

}


TEST_F(StateMachineTest, Events) {

    // start and end
    auto start = createState();
    auto end = createState();

    // create event
    auto event = start->addEventTransition(end);

    // set initial state
    start->initialize();
    EXPECT_EQ(start, currentState());

    // step (shouldn't do anything)
    step();
    EXPECT_EQ(start, currentState());

    // fire event (should change state)
    event.fire();
    EXPECT_EQ(end, currentState());


}


TEST_F(StateMachineTest, TimedExecution) {

    Timer timer{};

    // start and end
    auto start = createState();
    auto end = createState();

    // transitions
    start->addTimedTransition(1.0, end);
    end->addTimedTransition(2.0, start);

    // init
    setTimeStepSize(0.1);
    start->initialize();

    // start timer
    timer.start();

    // run
    while(timer.time() < 5.0) {

        auto t = timer.time();

        // step
        step();

        // check
        if(t < 0.9)
            EXPECT_EQ(start, currentState());
        else if(t >= 1.2 && t < 3.0)
            EXPECT_EQ(end, currentState());
        else if(t >= 3.2 && t < 4.0)
            EXPECT_EQ(start, currentState());
        else if(t >= 4.2)
            EXPECT_EQ(end, currentState());


    }

}


#pragma clang diagnostic pop
