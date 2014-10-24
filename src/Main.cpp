#include "graphics.h"
#include "standard.h"

#include "renderer.h"

#include "keyboard.h"
#include "mouse.h"

renderhdl renderer;
keyboardhdl keys;
mousehdl mouse;

void init()
{
	glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GEQUAL, 0.05);
	glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glClearDepth(1.5);

	glEnable(GL_CULL_FACE);
	{
		glFrontFace(GL_CCW);
		glCullFace(GL_BACK);
	}

	glClearColor(0.0, 0.0, 0.0, 0.0);

	renderer.init();
	noiseinit();

	mouse.init(200.0, renderer.display.height, renderer.display.width);
}

void displayfunc()
{
	if (keys.keystates['w'])
		renderer.camera.forward(0.001);
	if (keys.keystates['a'])
		renderer.camera.left(0.001);
	if (keys.keystates['s'])
		renderer.camera.backward(0.001);
	if (keys.keystates['d'])
		renderer.camera.right(0.001);
	if (keys.keystates['e'])
		renderer.camera.up(0.001);
	if (keys.keystates['q'])
		renderer.camera.down(0.001);
	if (keys.keystates['z'])
	{
		renderer.camera.object->linear_velocity = vec();
		renderer.camera.object->linear_acceleration = vec();
		renderer.camera.object->forces = vec();
	}

	renderer.displayf();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	renderer.display.height = h;
	renderer.display.width = w;
}

void pmotionfunc(int x, int y)
{
	renderer.camera.rotate(mouse.getdelta(x, y));

	if (mouse.v > renderer.display.height*3/4 || mouse.v < renderer.display.height*1/4 || mouse.h > renderer.display.width*3/4 || mouse.h < renderer.display.width*1/4)
		mouse.setmouseloc(renderer.display.width/2, renderer.display.height/2);
}

void mousefunc(int button, int state, int x, int y)
{
}

void motionfunc(int x, int y)
{
	renderer.camera.rotate(mouse.getdelta(x, y));

	if (mouse.v > renderer.display.height*3/4 || mouse.v < renderer.display.height*1/4 || mouse.h > renderer.display.width*3/4 || mouse.h < renderer.display.width*1/4)
		mouse.setmouseloc(renderer.display.width/2, renderer.display.height/2);
}

void keydownfunc(unsigned char key, int x, int y)
{
	if (key == 'u')
		exit(0);
	keys.keydown(key);
	if (keys.keystates['.'])
		renderer.camera.speed *= 2.0;
	if (keys.keystates[','])
		renderer.camera.speed /= 2.0;
}

void keyupfunc(unsigned char key, int x, int y)
{
	keys.keyup(key);
}

void release()
{
	renderer.release();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);

	atexit(release);

	renderer.display.windowed = false;
	if (renderer.display.windowed)
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

	if (GLEW_ARB_vertex_program)
		fprintf(stdout, "Status: ARB vertex programs available.\n");

	if (glewGetExtension("GL_ARB_fragment_program"))
		fprintf(stdout, "Status: ARB fragment programs available.\n");

	if (glewIsSupported("GL_VERSION_1_4  GL_ARB_point_sprite"))
		fprintf(stdout, "Status: ARB point sprites available.\n");

	glutReshapeFunc(reshape);
	glutDisplayFunc(displayfunc);
	glutIdleFunc(displayfunc);
	glutPassiveMotionFunc(pmotionfunc);
	glutMotionFunc(motionfunc);
	glutMouseFunc(mousefunc);
	glutKeyboardFunc(keydownfunc);
	glutKeyboardUpFunc(keyupfunc);

	glutSetCursor(GLUT_CURSOR_NONE);

	printf("\n");

	init();
	glutMainLoop();
	if (!renderer.display.windowed)
		glutLeaveGameMode();
}
