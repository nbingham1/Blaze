/*
	ScaleController.cpp
	Blaze Game Engine 0.03

	Created by Ned Bingham on 11/14/06.
	Copyright 2006 Sol Union. All rights reserved.

    Blaze Game Engine 0.03 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Blaze Game Engine 0.03 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Blaze Game Engine 0.03.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ScaleController.h"

void ScaleController::SetScaleValue(int GridX, int GridY, int Planet, int System, int Galaxy)
{
	if (Galaxy == -1)
		scale_value = pow(10, -12);
	else if (System == -1)
		scale_value = pow(10, -6);
	else if (Planet == -1)
		scale_value = pow(10, -3);
	else if (GridX == -1 || GridY == -1)
		scale_value = 1;
	else
		scale_value = pow(10, 3)*6;
}

