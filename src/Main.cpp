#include "bge_mtrl.h"
#include "bge_input.h"
#include "bge_core.h"
#include "common.h"

canvashdl core;
keyboardhdl keys;
mousehdl mouse;

bool windowed = false;

void init()
{
	srand(time(0));
	mouse.init(200.0, core.getheight(), core.getwidth());
	keys.reset();
	core.initialize(&keys, &mouse);
}

void displayfunc()
{
	core.render();
}

void reshape(int w, int h)
{
	core.setviewport(w, h);
}

void pmotionfunc(int x, int y)
{
	vec delta = mouse.getdelta(x, y);
	core.mouse_move(delta);
}

void mousefunc(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
		mouse.mouse_down(button - GLUT_LEFT_BUTTON);
	else if (state == GLUT_UP)
		mouse.mouse_up(button - GLUT_LEFT_BUTTON);

	core.mouse_change(x, y, button - GLUT_LEFT_BUTTON);
}

void motionfunc(int x, int y)
{
	vec delta = mouse.getdelta(x, y);
	core.mouse_move(delta);
}

void keydownfunc(unsigned char key, int x, int y)
{
	if (key == 27)
		exit(0);
	if (key == 'm')
		mouse.switchbind();

	keys.keydown(key);
}

void keyupfunc(unsigned char key, int x, int y)
{
	keys.keyup(key);
}

void release()
{
	core.release();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);

	atexit(release);

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

	GLint max_2d_texture_size,
		  max_3d_texture_size,
		  max_texture_layer_count,
		  max_texture_units;

	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_2d_texture_size);
	glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &max_3d_texture_size);
	glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &max_texture_layer_count);
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &max_texture_units);

	fprintf(stdout, "Maximum 2D Texture Size: %d\n", max_2d_texture_size);
	fprintf(stdout, "Maximum 3D Texture Size: %d\n", max_3d_texture_size);
	fprintf(stdout, "Maximum Texture Layer Count: %d\n", max_texture_layer_count);
	fprintf(stdout, "Maximum Texture Unit Count: %d\n", max_texture_units);

	glutReshapeFunc(reshape);
	glutDisplayFunc(displayfunc);
	glutIdleFunc(displayfunc);
	glutPassiveMotionFunc(pmotionfunc);
	glutMotionFunc(motionfunc);
	glutMouseFunc(mousefunc);
	glutKeyboardFunc(keydownfunc);
	glutKeyboardUpFunc(keyupfunc);

	glutSetCursor(GLUT_CURSOR_NONE);

	init();
	glutMainLoop();
	if (!windowed)
		glutLeaveGameMode();
}
