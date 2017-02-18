/*
** ILI9341 Library demo
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

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

// local headers
#include "timeout.h"
#include "gpio/DigitalPin.h"
#include "ili9341/ili9341.h"
#include "touch/TouchScreen.h"

DigitalPin			pinDC(&PORTC, PC4), pinRESET(&PORTC, PC5), pinCS(&PORTC, PC6),
					touchIRQ(&PORTA, PA0), touchDO(&PORTA, PA1), touchDI(&PORTA, PA2), touchCS(&PORTA, PA3),touchCLK(&PORTA, PA4);
ILI9341 			lcd;
TouchScreen			touch;
bool				cmdRefreshText = true; 			// to increase speed, only draw once per second


// define some colored areas as color picker buttons
uint16_t buttons [5][5]= {
		{0,0,48,48, BLUE},
		{48,0,48,48, YELLOW},
		{96,0,48,48, RED},
		{144,0,48,48,GREEN},
		{192,0,48,48, WHITE}
}, penColor = RED;

int main(void) {
	
	lcd.init(&pinDC, &pinRESET, &pinCS);
	lcd.setRotation(ILI9341::ROT0);

	touch.init(&touchCLK, &touchCS, &touchDI, &touchDO, &touchIRQ);

	lcd.drawClear(BLACK);

	// draw color picker buttons
	for (int i=0;i<5;i++) {
		lcd.drawRectFilled(buttons[i][0],buttons[i][1],buttons[i][2],buttons[i][3],buttons[i][4]);
	}

	while (1) {
		// check if there is a click
		uint16_t x = 0, y = 0;
		if (touch.isData() && touch.read(&x, &y)) {
			for (int i=0;i<5;i++)
				// check if click was in any of the few colored rectangles
				if (x > buttons[i][0] && x < buttons[i][0] + buttons[i][2] &&
						y > buttons[i][1] && y < buttons[i][1] + buttons[i][3])
					penColor = buttons[i][4]; // if a match is found, save color
			// on click draw with current color
			lcd.drawRectFilled(x, y, 10, 10, penColor);
		}
		// draw some text but only once per second
		if (cmdRefreshText) {
			char tmp[255] = {0};
			sprintf(tmp, "%02u:%02u %03d %03d %04X", 0,0, x, y, penColor);
			lcd.drawString(0, textY(18,2), tmp, 2, YELLOW, BLACK);
			lcd.drawString(0, textY(19,2), "www.pocketmagic.net", 2, RED, BLACK);
			cmdRefreshText = false;
		}
	}
}

