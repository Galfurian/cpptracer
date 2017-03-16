/// @file   timeScale.cpp
/// @author Enrico Fraccaroli
/// @date   Jul 20, 2016
/// @copyright
/// Copyright (c) 2016 Enrico Fraccaroli <enrico.fraccaroli@gmail.com>
/// Permission to use, copy, modify, and distribute this software for any
/// purpose with or without fee is hereby granted, provided that the above
/// copyright notice and this permission notice appear in all copies.
///
/// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
/// WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
/// MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
/// ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
/// WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
/// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
/// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

#include "timeScale.hpp"

TimeScale::TimeScale(double _timeValue, Enum _scaleValue) :
    timeValue(_timeValue),
    scaleValue(_scaleValue)
{
    // Nothing to do.
}

double TimeScale::getValue() const
{
    return timeValue;
}

double TimeScale::getScaledValue() const
{
    return timeValue * this->getMagnitude();
}

std::string TimeScale::scaleToString() const
{
    if (scaleValue == SEC) return "s";
    if (scaleValue == MS) return "ms";
    if (scaleValue == US) return "us";
    if (scaleValue == NS) return "ns";
    if (scaleValue == PS) return "ps";
    return "s";
}

double TimeScale::getMagnitude() const
{
    if (scaleValue == SEC) return 1;
    if (scaleValue == MS) return 1e-03;
    if (scaleValue == US) return 1e-06;
    if (scaleValue == NS) return 1e-09;
    if (scaleValue == PS) return 1e-12;
    return 1;
}
