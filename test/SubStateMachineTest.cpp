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
// Created by Jens Klimke on 2021-05-09.
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


/**
 *                  (o)
 *                   |
 *  +----------------|--------+              +--------------------------+
 *  | NoExtraction   |        |              | Extraction               |
 *  |                |        |              |                          |
 *  |    +---------------+    |  [pump=on]   |    +---------------+     |
 *  |    |   Idle        |    |------------------>|   InTime      |     |
 *  |    +---------------+    |              |    +---------------+     |
 *  |       | [t > 5s]        |              |       | [t > 25 s]       |
 *  |    +---------------+    |  [pump=off]  |    +---------------+     |
 *  |    |   Stopped     |<------------------|    |   OverTime    |     |
 *  |    +---------------+    |              |    +---------------+     |
 *  |                         |              |                          |
 *  +-------------------------+              +--------------------------+
 *
 */



class SubStateMachineTest : public ::testing::Test, public State {

};


TEST_F(SubStateMachineTest, CoffeeExtraction) {

    // flags
    bool pump = false;
    bool timeOver = false;
    bool delayOver = false;

    // link states
    auto stateNoExtraction = this->createState();
    auto stateExtraction = this->createState();

    // link sub-states
    auto stateIdle = stateNoExtraction->createState();
    auto stateStopped = stateNoExtraction->createState();
    auto stateInTime = stateExtraction->createState();
    auto stateOverTime = stateExtraction->createState();

    // add transitions
    stateNoExtraction->addTransition([&pump](const Transition *transition){ return pump; }, stateInTime);
    stateInTime->addTransition([&timeOver](const Transition *transition){ return timeOver; }, stateOverTime);
    stateExtraction->addTransition([&pump](const Transition *transition){ return !pump; }, stateStopped);
    stateStopped->addTransition([&delayOver](const Transition *transition){ return delayOver; }, stateIdle);

    // initialize
    stateIdle->initialize();

    // check current states
    EXPECT_EQ(stateNoExtraction, currentState());
    EXPECT_EQ(stateIdle, currentState()->currentState());


    // step
    step();

    // check current states
    EXPECT_EQ(stateNoExtraction, currentState());
    EXPECT_EQ(stateIdle, currentState()->currentState());


    // step
    pump = true;
    step();

    // check current states
    EXPECT_EQ(stateExtraction, currentState());
    EXPECT_EQ(stateInTime, currentState()->currentState());


    // step
    step();

    // check current states
    EXPECT_EQ(stateExtraction, currentState());
    EXPECT_EQ(stateInTime, currentState()->currentState());


    // step
    pump = false;
    step();

    // check current states
    EXPECT_EQ(stateNoExtraction, currentState());
    EXPECT_EQ(stateStopped, currentState()->currentState());


    // step
    delayOver = true;
    step();

    // check current states
    EXPECT_EQ(stateNoExtraction, currentState());
    EXPECT_EQ(stateIdle, currentState()->currentState());



}


#pragma clang diagnostic pop
