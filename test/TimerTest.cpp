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
#include <Timer.h>

class TimerTest : public ::testing::Test, public emb::Timer {

};


TEST_F(TimerTest, Start) {

    EXPECT_NEAR(0.0, this->_startTime, 1e-12);
    EXPECT_NEAR(0.0, this->_pauseTime, 1e-12);
    EXPECT_NO_THROW(this->start());

    EXPECT_NEAR(this->_startTime, Timer::absoluteTime(), 1.0);
    EXPECT_NEAR(0.0, this->time(), 0.01);

}


TEST_F(TimerTest, StartWithOffset) {

    EXPECT_NEAR(0.0, this->_startTime, 1e-12);
    EXPECT_NEAR(0.0, this->_pauseTime, 1e-12);
    EXPECT_NO_THROW(this->startWithOffset(10.0));
    EXPECT_NEAR(10.0, this->time(), 0.1);

}


TEST_F(TimerTest, StartWaitStop) {

    // start test timer
    this->start();

    // perform a few steps
    while(this->time() < 0.2)
        Timer::delay(0.01);

    EXPECT_NEAR(0.2, this->time(), 0.1);

    stop();

    EXPECT_NEAR(0.0, this->_startTime, 1e-12);
    EXPECT_NEAR(0.0, this->_pauseTime, 1e-12);

}


TEST_F(TimerTest, Pause) {

    // start test timer
    start();

    // perform a few steps
    for(unsigned i = 0; i < 5; ++i) {

        // pause and resume
        if(i == 2)
            pause();
        else if(i == 4)
            start();

        // check time
        if(i < 2)
            EXPECT_GT(0.2, time());
        else if(i <= 4)
            EXPECT_NEAR(0.2, time(), 0.1);
        else
            EXPECT_LT(0.4, time());

        // wait 0.1 s
        Timer::delay(0.1);

    }

    EXPECT_NEAR(0.3, time(), 0.1);

}


#pragma clang diagnostic pop
