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
// Created by Jens Klimke on 2021-05-13.
//

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <gtest/gtest.h>
#include <Transition.h>
#include <StateInterface.h>

#ifndef EPS_TIME
#define EPS_TIME 1e-2
#endif

using namespace emb;

bool ended = false;
StateInterface *current = nullptr;

struct EndMockup : public StateInterface {

    bool enter(const Transition *) override {
        current = nullptr;
        return (ended = true);
    }

};


struct ChoiceMockup : public StateInterface {

    bool enter(const Transition *) override {
        return false;
    }

};


struct StateMockup : public StateInterface {

    bool entered = false;

    bool enter(const Transition *transition) override {
        current = this;
        return (entered = true);
    }

};


class TransitionTest : public ::testing::Test, public Transition {

};



TEST_F(TransitionTest, CheckCondition) {

    condition = [this] (const Transition *transition) {
        return this == transition;
    };

    EXPECT_TRUE(condition(this));
    EXPECT_FALSE(condition(nullptr));

}


TEST_F(TransitionTest, ExecuteTransitons) {

    StateMockup state{};
    ChoiceMockup choice{};

    // check state
    EXPECT_FALSE(state.entered);

    // define start and end state
    from = nullptr;
    to = &state;

    // execute transition (without checking condition)
    EXPECT_TRUE(execute());
    EXPECT_TRUE(state.entered);

    // re-define end state
    to = &choice;

    // execute transition (without checking condition)
    EXPECT_FALSE(execute());

}


TEST_F(TransitionTest, FollowTransitions) {

    // reset current
    current = nullptr;

    // create states
    ChoiceMockup start{};
    StateMockup middle{};
    EndMockup end{};

    // define choices
    ChoiceMockup choice0{};
    ChoiceMockup choice1{};
    ChoiceMockup choice2{};

    // define transitions
    start.addTransition(ALWAYS_FALSE_CONDITION, &choice0);
    choice0.addTransition(ALWAYS_TRUE_CONDITION, &choice1);
    choice1.addTransition(ALWAYS_FALSE_CONDITION, &middle);
    middle.addTransition(ALWAYS_TRUE_CONDITION, &choice2);
    choice2.addTransition(ALWAYS_FALSE_CONDITION, &end);

    // check ended flag
    EXPECT_FALSE(ended);

    // check transitions
    EXPECT_EQ(nullptr, start.checkTransition());
    EXPECT_EQ(nullptr, current);

    // define transitions
    start.addTransition(ALWAYS_TRUE_CONDITION, &choice1);
    EXPECT_EQ(nullptr, start.checkTransition());
    EXPECT_EQ(nullptr, current);

    // define transitions
    auto t1 = choice1.addTransition(ALWAYS_TRUE_CONDITION, &middle);
    EXPECT_EQ(t1, start.checkTransition());
    EXPECT_EQ(&middle, current);

    // define transitions
    auto t2 = choice2.addTransition(ALWAYS_TRUE_CONDITION, &end);
    EXPECT_EQ(t2, start.checkTransition());
    EXPECT_EQ(nullptr, current);

    // check ended flag
    EXPECT_TRUE(ended);

}


#pragma clang diagnostic pop
