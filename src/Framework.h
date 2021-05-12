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


#ifndef STATE_MACHINE_FRAMEWORK_H
#define STATE_MACHINE_FRAMEWORK_H

namespace emb {

    struct Framework {

        /**
         * @brief Returns the current time in milliseconds.
         * The origin of the time is dependent on the system.
         * @return
         */
        static unsigned long getMilliseconds();


        /**
         * @brief Delays the execution for the given time
         * @param milliseconds The delay time in milliseconds
         */
        static void delay(long long int milliseconds);

    };

}

#endif // STATE_MACHINE_FRAMEWORK_H
