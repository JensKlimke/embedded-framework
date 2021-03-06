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


#ifndef STATE_MACHINE_TIMER_H
#define STATE_MACHINE_TIMER_H

namespace emb {

    class Timer {

    protected:

        double _startTime = 0.0;
        double _pauseTime = 0.0;

    public:

        /**
         * @brief Returns the absolute time in seconds.
         * The time-origin (time zero) depends on the underlying clock framework.
         * @return The absolute time
         */
        static double absoluteTime();


        /**
         * @brief Starts the timer.
         * Sets the local timer time origin to the current actual time. The timer can be reset by calling this method
         * also while timer is "running".
         */
        void start();


        /**
         * @brief Starts the timer with an initial offset.
         * @param offset Offset to be started with.
         */
        void startWithOffset(double offset);


        /**
         * @brief Stops the timer.
         * Resets the timer states.
         */
        void stop();


        /**
         * @brief Pauses the current timer.
         * Sets the pause time to the current time. On resume the paused time is added to the start time.
         */
        void pause();


        /**
         * @brief Resumes the paused timer.
         */
        void resume();


        /**
         * @brief Return the relative time
         * The relative time is the time in seconds from the start of the timer.
         * @return The relative time
         */
        double time() const;


        /**
         * @brief Returns whether the timer is paused.
         * Checks the pause time. If it is set, the timer is paused.
         * @return Pause flag
         */
        bool isPaused() const;


        /**
         * @brief Delays the execution for given seconds.
         * Millisecond accuracy
         * @param seconds Time to delay in seconds.
         */
        static void delay(double seconds);

    };

}

#endif // STATE_MACHINE_TIMER_H
