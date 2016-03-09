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

pthread_t prepare_thread;
pthread_t display_thread;

bool windowed = true;

GLFWwindow *window = NULL;
canvashdl canvas;

void release(preference *pref, vec3f value)
{
	printf("closing window\n");
	glfwSetWindowShouldClose(window, true);
}

void split(preference *pref, vec3f value)
{
	if (value[1] > 0.0)
	{
		for (list<objecthdl*>::iterator i = canvas.objects.begin(); i != canvas.objects.end(); i++)
			if ((*i)->type == "planet")
				((planethdl*)(*i))->geometry.split(*i);
	}
}

void merge(preference *pref, vec3f value)
{
	if (value[1] > 0.0)
	{
		for (list<objecthdl*>::iterator i = canvas.objects.begin(); i != canvas.objects.end(); i++)
			if ((*i)->type == "planet")
				((planethdl*)(*i))->geometry.merge();
	}
}

int player = 0;
void next_player(preference *pref, vec3f value)
{
	if (value[1] > 0.0)
	{
		player = (player+1)%canvas.players.size();
		canvas.devices["keyboard"].buttons.control.insert(GLFW_KEY_W, preference(&canvas.players[player], playerhdl::forward));
		canvas.devices["keyboard"].buttons.control.insert(GLFW_KEY_A, preference(&canvas.players[player], playerhdl::left));
		canvas.devices["keyboard"].buttons.control.insert(GLFW_KEY_S, preference(&canvas.players[player], playerhdl::backward));
		canvas.devices["keyboard"].buttons.control.insert(GLFW_KEY_D, preference(&canvas.players[player], playerhdl::right));
		canvas.devices["keyboard"].buttons.control.insert(GLFW_KEY_E, preference(&canvas.players[player], playerhdl::up));
		canvas.devices["keyboard"].buttons.control.insert(GLFW_KEY_Q, preference(&canvas.players[player], playerhdl::down));
		canvas.devices["keyboard"].buttons.control.insert(GLFW_KEY_PERIOD, preference(&canvas.players[player], playerhdl::accelerate));
		canvas.devices["keyboard"].buttons.control.insert(GLFW_KEY_COMMA, preference(&canvas.players[player], playerhdl::deccelerate));
		canvas.devices["keyboard"].buttons.control.insert(GLFW_KEY_Z, preference(&canvas.players[player], playerhdl::stop));
		canvas.devices["keyboard"].buttons.control.insert(GLFW_KEY_G, preference(&canvas.players[player], split));
		canvas.devices["keyboard"].buttons.control.insert(GLFW_KEY_H, preference(&canvas.players[player], merge));

		canvas.devices["mouse"].axes[0].control = preference(&canvas.players[player], playerhdl::horizontal);
		canvas.devices["mouse"].axes[1].control = preference(&canvas.players[player], playerhdl::vertical);
	}
}

void init()
{
	srand(time(0));
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	canvas.initialize(1600, 900);
	canvas.devices.insert("mouse", controllerhdl(2));
	canvas.devices.insert("keyboard", controllerhdl());
	for (int i = 0; i < 1; i++)
		canvas.add_player()->camera->position = vec3f(-20.0, 0.0, 0.0);

	canvas.devices["keyboard"].buttons.control.insert(GLFW_KEY_ESCAPE, preference(NULL, release));
	canvas.devices["keyboard"].buttons.control.insert(GLFW_KEY_N, preference(NULL, next_player));
	canvas.devices["keyboard"].buttons.control.insert(GLFW_KEY_W, preference(&canvas.players[player], playerhdl::forward));
	canvas.devices["keyboard"].buttons.control.insert(GLFW_KEY_A, preference(&canvas.players[player], playerhdl::left));
	canvas.devices["keyboard"].buttons.control.insert(GLFW_KEY_S, preference(&canvas.players[player], playerhdl::backward));
	canvas.devices["keyboard"].buttons.control.insert(GLFW_KEY_D, preference(&canvas.players[player], playerhdl::right));
	canvas.devices["keyboard"].buttons.control.insert(GLFW_KEY_E, preference(&canvas.players[player], playerhdl::up));
	canvas.devices["keyboard"].buttons.control.insert(GLFW_KEY_Q, preference(&canvas.players[player], playerhdl::down));
	canvas.devices["keyboard"].buttons.control.insert(GLFW_KEY_PERIOD, preference(&canvas.players[player], playerhdl::accelerate));
	canvas.devices["keyboard"].buttons.control.insert(GLFW_KEY_COMMA, preference(&canvas.players[player], playerhdl::deccelerate));
	canvas.devices["keyboard"].buttons.control.insert(GLFW_KEY_Z, preference(&canvas.players[player], playerhdl::stop));
	canvas.devices["keyboard"].buttons.control.insert(GLFW_KEY_G, preference(&canvas.players[player], split));
	canvas.devices["keyboard"].buttons.control.insert(GLFW_KEY_H, preference(&canvas.players[player], merge));

	canvas.devices["mouse"].axes[0].control = preference(&canvas.players[player], playerhdl::horizontal);
	canvas.devices["mouse"].axes[1].control = preference(&canvas.players[player], playerhdl::vertical);
	canvas.devices["mouse"].axes[0].low = 0.25;
	canvas.devices["mouse"].axes[0].high = 0.75;
	canvas.devices["mouse"].axes[1].low = 0.25;
	canvas.devices["mouse"].axes[1].high = 0.75;
}

void *preparefunc(void *data)
{
	while (!glfwWindowShouldClose(window))
		canvas.prepare();

	pthread_exit(NULL);
	return NULL;
}

void *displayfunc(void *data)
{
	//while (!glfwWindowShouldClose(window))
	//{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		canvas.clock();
		canvas.input();
		canvas.render();
		glfwSwapBuffers(window);
	//}	
}

void reshapefunc(int w, int h)
{
	canvas.reshape(w, h);
}

void cursorfunc(GLFWwindow* window, double x, double y)
{
	cout << x << " " << y << endl;
	map<string, controllerhdl>::iterator mouse = canvas.devices.find("mouse");
	if (mouse != canvas.devices.end())
	{
		mouse->value.axes[0].set((float)x/(float)canvas.screen[0], canvas.real_current_time, canvas.game_current_time);
		mouse->value.axes[1].set((float)y/(float)canvas.screen[1], canvas.real_current_time, canvas.game_current_time);
	}
}

void mousefunc(GLFWwindow* window, int button, int action, int mods)
{
	map<string, controllerhdl>::iterator mouse = canvas.devices.find("mouse");
	if (mouse != canvas.devices.end())
		mouse->value.buttons.set(button, action, canvas.real_current_time, canvas.game_current_time);
}

void keyfunc(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	map<string, controllerhdl>::iterator keyboard = canvas.devices.find("keyboard");
	cout << key << " " << scancode << " " << action << " " << mods << endl;
	if (keyboard != canvas.devices.end())
		keyboard->value.buttons.set(key, action != 0, canvas.real_current_time, canvas.game_current_time);
}

void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

#include <sys/time.h>

int main(int argc, char **argv)
{
	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwSetErrorCallback(error_callback);
	window = glfwCreateWindow(1600, 900, "BGE", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "Error %s\n", glewGetErrorString(err));
		exit(1);
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
	fprintf(stdout, "Status: Using OpenGL %s\n", glGetString(GL_VERSION));
	fprintf(stdout, "Status: Using GLSL %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, cursorfunc);
	glfwSetMouseButtonCallback(window, mousefunc);
	glfwSetKeyCallback(window, keyfunc);

	init();

	// Initialize Threading
	pthread_create(&prepare_thread, NULL, preparefunc, NULL);
	//pthread_create(&display_thread, NULL, displayfunc, NULL);

	while (!glfwWindowShouldClose(window))
	{
		displayfunc(NULL);
		glfwPollEvents();
	}

	void *end;
	pthread_join(prepare_thread, &end);
	//pthread_join(display_thread, &end);
	
	glfwDestroyWindow(window);
	glfwTerminate();
}
