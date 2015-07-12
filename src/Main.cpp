/*
 * main.cpp
 * Blaze Game Engine v0.11
 *
 * Created by Ned Bingham on October 20, 2011.
 * Modified by Ned Bingham on October 20, 2011.
 * Copyright 2011 Sol Union. All rights reserved.
 *
 * Blaze Game Engine v0.11 is free software: you can redistribute
 * it and/or modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * Blaze Game Engine v0.11 is distributed in the hope that it will
 * be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Blaze Game Engine v0.11.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include "core/list.h"
#include "core/geometry.h"
#include <time.h>
#include <pthread.h>

#include "graphics/opengl.h"
#include "engine/canvas.h"

using namespace core;

int num_threads = 0;
pthread_t *threads = NULL;

bool done = false;
bool windowed = true;

canvashdl canvas;

void release(preference *pref, vec3f value)
{
	glutLeaveMainLoop();
}

int player = 0;
void next_player(preference *pref, vec3f value)
{
	if (value[1] > 0.0)
	{
		player = (player+1)%canvas.players.size();
		canvas.devices["keyboard"].buttons.control.insert('w', preference(&canvas.players[player], playerhdl::forward));
		canvas.devices["keyboard"].buttons.control.insert('a', preference(&canvas.players[player], playerhdl::left));
		canvas.devices["keyboard"].buttons.control.insert('s', preference(&canvas.players[player], playerhdl::backward));
		canvas.devices["keyboard"].buttons.control.insert('d', preference(&canvas.players[player], playerhdl::right));
		canvas.devices["keyboard"].buttons.control.insert('e', preference(&canvas.players[player], playerhdl::up));
		canvas.devices["keyboard"].buttons.control.insert('q', preference(&canvas.players[player], playerhdl::down));
		canvas.devices["keyboard"].buttons.control.insert('.', preference(&canvas.players[player], playerhdl::accelerate));
		canvas.devices["keyboard"].buttons.control.insert(',', preference(&canvas.players[player], playerhdl::deccelerate));
		canvas.devices["keyboard"].buttons.control.insert('z', preference(&canvas.players[player], playerhdl::stop));

		canvas.devices["mouse"].axes[0].control = preference(&canvas.players[player], playerhdl::horizontal);
		canvas.devices["mouse"].axes[1].control = preference(&canvas.players[player], playerhdl::vertical);
	}
}

void init()
{
	srand(time(0));
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	canvas.initialize();
	canvas.devices.insert("mouse", controllerhdl(2));
	canvas.devices.insert("keyboard", controllerhdl());
	for (int i = 0; i < 1; i++)
		canvas.add_player()->camera->position = vec3f(-20.0, 0.0, 0.0);

	canvas.devices["keyboard"].buttons.control.insert(27, preference(NULL, release));
	canvas.devices["keyboard"].buttons.control.insert('n', preference(NULL, next_player));
	canvas.devices["keyboard"].buttons.control.insert('w', preference(&canvas.players[player], playerhdl::forward));
	canvas.devices["keyboard"].buttons.control.insert('a', preference(&canvas.players[player], playerhdl::left));
	canvas.devices["keyboard"].buttons.control.insert('s', preference(&canvas.players[player], playerhdl::backward));
	canvas.devices["keyboard"].buttons.control.insert('d', preference(&canvas.players[player], playerhdl::right));
	canvas.devices["keyboard"].buttons.control.insert('e', preference(&canvas.players[player], playerhdl::up));
	canvas.devices["keyboard"].buttons.control.insert('q', preference(&canvas.players[player], playerhdl::down));
	canvas.devices["keyboard"].buttons.control.insert('.', preference(&canvas.players[player], playerhdl::accelerate));
	canvas.devices["keyboard"].buttons.control.insert(',', preference(&canvas.players[player], playerhdl::deccelerate));
	canvas.devices["keyboard"].buttons.control.insert('z', preference(&canvas.players[player], playerhdl::stop));

	canvas.devices["mouse"].axes[0].control = preference(&canvas.players[player], playerhdl::horizontal);
	canvas.devices["mouse"].axes[1].control = preference(&canvas.players[player], playerhdl::vertical);
	canvas.devices["mouse"].axes[0].low = 0.25;
	canvas.devices["mouse"].axes[0].high = 0.75;
	canvas.devices["mouse"].axes[1].low = 0.25;
	canvas.devices["mouse"].axes[1].high = 0.75;
}

void *preparefunc(void *data)
{
	while (!done)
	{
		canvas.prepare();
	}

	pthread_exit(NULL);
	return NULL;
}

void displayfunc()
{
	canvas.clock();
	canvas.input();
	canvas.render();
}

void reshapefunc(int w, int h)
{
	canvas.reshape(w, h);
}

void pmotionfunc(int x, int y)
{
	map<string, controllerhdl>::iterator mouse = canvas.devices.find("mouse");
	mouse->value.axes[0].set((float)x/(float)canvas.screen[0], canvas.real_current_time, canvas.game_current_time);
	mouse->value.axes[1].set((float)y/(float)canvas.screen[1], canvas.real_current_time, canvas.game_current_time);

	if (!mouse->value.axes[0].contained() || !mouse->value.axes[1].contained())
	{
		glutWarpPointer(canvas.screen[0]/2, canvas.screen[1]/2);
		mouse->value.axes[0].warp(0.5);
		mouse->value.axes[1].warp(0.5);
	}
}

void mousefunc(int button, int state, int x, int y)
{
	map<string, controllerhdl>::iterator mouse = canvas.devices.find("mouse");
	mouse->value.axes[0].set((float)x/(float)canvas.screen[0], canvas.real_current_time, canvas.game_current_time);
	mouse->value.axes[1].set((float)y/(float)canvas.screen[1], canvas.real_current_time, canvas.game_current_time);

	if (!mouse->value.axes[0].contained() || !mouse->value.axes[1].contained())
	{
		glutWarpPointer(canvas.screen[0]/2, canvas.screen[1]/2);
		mouse->value.axes[0].warp(0.5);
		mouse->value.axes[1].warp(0.5);
	}

	if (state == GLUT_DOWN)
		mouse->value.buttons.press(button - GLUT_LEFT_BUTTON, canvas.real_current_time, canvas.game_current_time);
	else if (state == GLUT_UP)
		mouse->value.buttons.release(button - GLUT_LEFT_BUTTON, canvas.real_current_time, canvas.game_current_time);
}

void motionfunc(int x, int y)
{
	map<string, controllerhdl>::iterator mouse = canvas.devices.find("mouse");
	mouse->value.axes[0].set((float)x/(float)canvas.screen[0], canvas.real_current_time, canvas.game_current_time);
	mouse->value.axes[1].set((float)y/(float)canvas.screen[1], canvas.real_current_time, canvas.game_current_time);
}

void keydownfunc(unsigned char key, int x, int y)
{
	map<string, controllerhdl>::iterator keyboard = canvas.devices.find("keyboard");
	keyboard->value.buttons.press(key, canvas.real_current_time, canvas.game_current_time);
}

void keyupfunc(unsigned char key, int x, int y)
{
	map<string, controllerhdl>::iterator keyboard = canvas.devices.find("keyboard");
	keyboard->value.buttons.release(key, canvas.real_current_time, canvas.game_current_time);
}

#include <sys/time.h>

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);

	if (windowed)
	{
		glutInitWindowSize(1600, 900);
		glutInitWindowPosition(0, 0);
		glutCreateWindow("BGE");
	}
	else
	{
		glutGameModeString("1600x900:32@60");
		glutEnterGameMode();
	}

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "Error %s\n", glewGetErrorString(err));
		exit(1);
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
	fprintf(stdout, "Status: Using OpenGL %s\n", glGetString(GL_VERSION));
	fprintf(stdout, "Status: Using GLSL %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	glutReshapeFunc(reshapefunc);
	glutDisplayFunc(displayfunc);
	glutIdleFunc(displayfunc);

	glutPassiveMotionFunc(pmotionfunc);
	glutMotionFunc(motionfunc);
	glutMouseFunc(mousefunc);
	glutKeyboardFunc(keydownfunc);
	glutKeyboardUpFunc(keyupfunc);

	glutSetCursor(GLUT_CURSOR_NONE);

	init();

	// Initialize Threading
	if (num_threads > 0)
	{
		done = false;
		threads = new pthread_t[num_threads];
		for (int x = 0; x < num_threads; x++)
			pthread_create(&threads[x], NULL, preparefunc, NULL);
	}

	glutMainLoop();
	if (!windowed)
		glutLeaveGameMode();

	done = true;
	void *end;
	if (threads != NULL)
	{
		for (int x = 0; x < num_threads; x++)
			pthread_join(threads[x], &end);

		delete [] threads;
	}

	num_threads = 0;
	threads = NULL;
}
