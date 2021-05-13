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
// Created by Jens Klimke on $date.get('yyyy-M-d').
// Contributors:
//


#include "Transition.h"
#include "StateInterface.h"

using namespace emb;

bool Transition::execute() const {

    return to->enter(this);

}

const Transition *Transition::follow() const {

    // check condition, if false return null
    if(!condition(this))
        return nullptr;

    // check target state transitions
    auto final = to->checkTransition();
    if(final != nullptr)
        return final;

    // try to enter target state
    if(to->enter(this))
        return this;

    // return null (no target found)
    return nullptr;

}
