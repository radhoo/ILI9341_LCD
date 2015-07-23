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

#pragma once

#include "../gpio/DigitalPin.h"

#define TOUCH_HEIGHT			 					240	// touchscreen height in landscape mode
#define TOUCH_WIDTH 								320	// touchscreen width in landscape mode

#define TOUCH_CMD_XP  								0x90 // CH=X+, Mode=12bit, DiffEnded, PwrDown
#define TOUCH_CMD_YP  								0xD0 // CH=Y+, Mode=12bit, DiffEnded, PwrDown


class TouchScreen {
	private:
	DigitalPin *m_clk, *m_cs, *m_din, *m_dout, *m_irq;

	public:
		enum Precision {
			PRECISION_LOW = 1,	// low precision
			PRECISION_MED = 2,	// medium precision
			PRECISION_HIG = 4,	// high precision
			PRECISION_EXT = 8	// extreme precision
		};
		enum Orientation {
			PORTRAIT = 0,		// portrait orientation
			LANDSCAPE = 1		// landscape orientation
		};

		// init the GPIO pins and set the orientation (default portrait)
		void 	init(DigitalPin *clk, DigitalPin *cs, DigitalPin *din, DigitalPin *dout, DigitalPin *irq, Precision precision = PRECISION_MED, Orientation orientation = PORTRAIT);
		// change the precision (default low)
		void	setPrecision(Precision precision = PRECISION_MED);
		// change the orientation (default portrait)
		void	setOrientation(Orientation orientation = PORTRAIT);
		// returns true is data is available, on touch down the IRQ channel goes high
		bool	isData();
		// read coordinates
		bool	read(uint16_t *x, uint16_t *y);


    private:
		Precision m_precision;
		Orientation m_orientation;

		// I/O serial communication with the touchscreen module
		void		writeData(uint8_t data);
		uint16_t	readData();
};
