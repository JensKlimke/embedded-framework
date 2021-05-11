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
// Created by Jens Klimke on 2021-05-08
//

#include "Framework.h"
#include "Timer.h"

double emb::Timer::absoluteTime() {

    return (double) Framework::getMilliseconds() * 1e-3;

}

void emb::Timer::start() {

    // set start time
    _startTime = absoluteTime();
    _pauseTime = 0.0;

}

void emb::Timer::pause() {

    // set paused time
    _pauseTime = absoluteTime();

}

void emb::Timer::resume() {

    // time diff
    auto p = absoluteTime() - _pauseTime;

    // shift start time and
    _startTime += p;
    _pauseTime = 0.0;

}

double emb::Timer::time() const {

    // get time difference
    return absoluteTime() - _startTime;

}
