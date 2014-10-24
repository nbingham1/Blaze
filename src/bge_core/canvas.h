#include "../bge_mtrl.h"
#include "../bge_gui.h"
#include "../bge_input.h"
#include "../bge_chem.h"
#include "../common.h"

#include "background.h"
#include "camera.h"
#include "object.h"
#include "planet.h"
#include "star.h"
#include "system.h"
#include "plant.h"

#ifndef canvas_h
#define canvas_h

void *run_thread(void *data);

struct canvashdl
{
	canvashdl();
	~canvashdl();

	int height;
	int width;

	double frame_number;
	// average frame rate calculations
	double overall_start_time;
	double overall_current_time;
	double overall_frame_rate;

	double time_elapsed;

	// instantaneous frame rate calculations
	double start_frame;
	double start_time;
	double frame_rate;

	// gui stuff
	bool create_camera;
	bool create_planet;
	bool create_star;
	bool wireframe;

	// threading
	bool done;
	int num_threads;
	pthread_t *threads;

	// input
	mousehdl *mouse;
	keyboardhdl *keys;

	periodichdl chemistry;
	palettehdl	palette;

	backgroundhdl background;

	camerahdl *active_camera;
	objecthdl *start;
	objecthdl *end;

	// menus
	containerhdl object_info;
	containerhdl object_menu;
	containerhdl option_menu;
	containerhdl diagnostics;

	GLuint default_vx;
	GLuint default_ft;
	GLuint default_pm;

	fonthdl *font;

	// base functions
	void initialize(keyboardhdl *k, mousehdl *m);
	void release();

	void prepare();
	void render();

	// objecthdl management functions
	objecthdl *addobject(objecthdl *master, int type);
	void addobject(objecthdl *object);
	void removeobject(objecthdl *obj);

	// input functions
	void mouse_move(vec delta);
	void mouse_change(int x, int y, int button);
	void handle_clicks();
	void handle_keys();

	// time and frame management
	double time();
	double frame();
	double framerate();
	void   setviewport(int w, int h);
	int    getheight();
	int	   getwidth();

	// coordinate management
	vec wintoworld(double u, double v);
	vec wintogui(double u, double v);
};

#endif
