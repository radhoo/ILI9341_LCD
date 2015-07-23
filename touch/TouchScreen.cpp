/*
** ILI9341 resistive touchscreen driver
** Copyright (C) 2009 - 2015 Radu Motisan, radu.motisan@gmail.com
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

#include "TouchScreen.h"

// init the GPIO pins and set the orientation (default portrait)
void TouchScreen::init(DigitalPin *clk, DigitalPin *cs, DigitalPin *din, DigitalPin *dout, DigitalPin *irq, Precision precision, Orientation orientation) {
	// save pins
	m_clk = clk;
	m_cs = cs;
	m_din = din;
	m_dout = dout;
	m_irq = irq;
	// config IO pins
	m_clk->config(DigitalPin::OUTPUT);
	m_cs->config(DigitalPin::OUTPUT);
	m_din->config(DigitalPin::OUTPUT);
	m_dout->config(DigitalPin::INPUT);
	m_irq->config(DigitalPin::OUTPUT);
	// set default states
	*m_cs = 1;
	*m_clk = 1;
	*m_din = 1;
	*m_clk = 1;
	// set other parameters
	setPrecision(precision);
	setOrientation(orientation);
}

// change the precision (default low)
void TouchScreen::setPrecision(Precision precision) {
	m_precision = precision;
}

// change the orientation (default portrait)
void TouchScreen::setOrientation(Orientation orientation) {
	m_orientation = orientation;
}

/*
 * read coordinates
 * Calibration required for values calXX
 * raw corners: 180,340 - 3330, 3760
 */
bool TouchScreen::read(uint16_t *x, uint16_t *y) {
	uint32_t rawx = 0, rawy = 0;
	uint16_t calX1 = 200, calY1 = 360, calX2 = 3615, calY2 = 3800;

	*m_cs = 0;

	for (int i=0; i< m_precision; i++) {
		writeData(TOUCH_CMD_XP);
		*m_clk = 1;
		*m_clk = 0;
		rawx += readData();

		writeData(TOUCH_CMD_YP);
		*m_clk = 1;
		*m_clk = 0;
		rawy += readData();
	}
	rawx /= m_precision;
	rawy /= m_precision;

	if (rawx < calX1 || rawx > calX2 || rawy < calY1 || rawy > calY2)
		return false;

	rawx = (rawx - calX1) * TOUCH_WIDTH / (calX2 - calX1);
	rawy = (rawy - calY1) * TOUCH_HEIGHT / (calY2 - calY1);

	if (m_orientation == PORTRAIT) {
		*x = TOUCH_HEIGHT - rawy;
		*y = rawx;
	} else {
		*x = rawx;
		*y = rawy;
	}

	*m_cs = 1;
}

// returns true is data is available, on touch down the IRQ channel goes high
bool TouchScreen::isData() {
	m_irq->config(DigitalPin::INPUT);
	bool avail = !*m_irq;
	m_irq->config(DigitalPin::OUTPUT);
	return avail;
}

// I/O serial communication with the touchscreen module
void TouchScreen::writeData(uint8_t data) {
	*m_clk = 0;
	for(uint8_t count=0; count<8; count++) {
		*m_din = (data & 0x80);
		data <<= 1;
		*m_clk = 0;
		*m_clk = 1;
	}
}

uint16_t TouchScreen::readData() {
	uint16_t data = 0;
	for(uint8_t count=0; count<12; count++) {
		data <<= 1;
		*m_clk = 1;
		*m_clk = 0;
		if (*m_dout) data++;
	}
	return data;
}
