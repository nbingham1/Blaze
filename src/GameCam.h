/*
	GameCam.h
	Blaze Game Engine 0.01

	Created by Ned Bingham on 8/8/05.
	Copyright 2005 Sol Union. All rights reserved.

    Blaze Game Engine 0.01 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Blaze Game Engine 0.01 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Blaze Game Engine 0.01.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Main.h"
#include "GameMatrix.h"

#ifndef GameCam_h
#define GameCam_h

class GameCam
{
    public:
        GameCam();
        ~GameCam();
        
        Vector translate;
        Vector rotate;
		
        void Rotate(float x, float y, float z);
        void Translate(float x, float y, float z);
        void Render();
};

#endif
