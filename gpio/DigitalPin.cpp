/*
** input/output GPIO class
** Copyright (C) 2009 - 2015 Radu Motisan, radu.motisan@gmail.com, www.pocketmagic.net
**
** This file is a part of "ILI9341" open source library.
**
** ILI9341 Library is free software; you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published
** by the Free Software Foundation; either version 3 of the License,
** or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "DigitalPin.h"

// Set the output, specified as 0 or 1 (int)
void DigitalPin::write(bool state) {
	state? *m_pport |= mask1 : *m_pport &= mask0;
}

// Return the output setting, represented as 0 or 1 (bool)
bool DigitalPin::read() {
	return *PORT2PIN(m_pport) & mask1;
}

// negate state
bool DigitalPin::toggle() {
	write(!read());
}

