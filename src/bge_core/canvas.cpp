#include "canvas.h"
#include "../bge_phys/newtonian.h"

void *run_thread(void *data)
{
	canvashdl *canvas = (canvashdl*)data;

	while (!canvas->done)
		canvas->prepare();

	return NULL;
}

canvashdl::canvashdl()
{
	height = 0;
	width = 0;
	create_planet = false;
	wireframe = false;
	create_star = false;

	frame_number = 0.0;
	// average frame rate calculations
	overall_start_time = 0.0;
	overall_current_time = 0.0;
	overall_frame_rate = 0.0;

	time_elapsed = 0.0;

	// instantaneous frame rate calculations
	start_frame = 0.0;
	start_time = 0.0;
	frame_rate = 0.0;

	// gui stuff
	create_camera = false;

	// threading
	done = false;
	num_threads = 0;
	threads = NULL;

	mouse = NULL;
	keys = NULL;

	active_camera = NULL;
	start = NULL;
	end = NULL;

	default_vx = 0;
	default_ft = 0;
	default_pm = 0;

	font = NULL;
}

canvashdl::~canvashdl()
{
	release();
}

void canvashdl::initialize(keyboardhdl *k, mousehdl *m)
{
	// Initialize Input Devices
	mouse = m;
	keys = k;

	// Initialize Display
	width = glutGet(GLUT_WINDOW_WIDTH);
	height = glutGet(GLUT_WINDOW_HEIGHT);

	// Initialize Time
	timeval gtime;
	gettimeofday(&gtime, NULL);
	overall_start_time = gtime.tv_sec + gtime.tv_usec*1.0E-6;
	overall_current_time = overall_start_time;

	start_time = overall_current_time;

	// Create GUI Menus
	valuehdl *timeelapsed = new valuehdl();
	timeelapsed->init((char*)"Time Elapsed:", &time_elapsed, double_ty);
	valuehdl *framenum = new valuehdl();
	framenum->init((char*)"Frame Number:", &frame_number, double_ty);
	valuehdl *aframerate = new valuehdl();
	aframerate->init((char*)"Average Frame Rate:", &overall_frame_rate, double_ty);
	valuehdl *framerate = new valuehdl();
	framerate->init((char*)"Frame Rate:", &frame_rate, double_ty);

	diagnostics.init("Diagnostics", -0.9, 0.9, 0.3);
	diagnostics.addobj(timeelapsed, value_t);
	diagnostics.addobj(framenum, value_t);
	diagnostics.addobj(aframerate, value_t);
	diagnostics.addobj(framerate, value_t);

	checkboxhdl *diagvis = new checkboxhdl();
	diagvis->init((char*)"Diagnostics", &diagnostics.visible);
	checkboxhdl *createcamera = new checkboxhdl();
	createcamera->init((char*)"Create New Camera", &create_camera);
	checkboxhdl *createplanet = new checkboxhdl();
	createplanet->init((char*)"Create New Planet", &create_planet);
	checkboxhdl *createstar = new checkboxhdl();
	createstar->init((char*)"Create New Star", &create_star);
	checkboxhdl *wireframe = new checkboxhdl();
	wireframe->init("Wireframe", &this->wireframe);

	option_menu.init("Options", 0.0, 0.0, 0.3);
	option_menu.addobj(diagvis, checkbox_t);
	option_menu.addobj(wireframe, checkbox_t);
	option_menu.addobj(createcamera, checkbox_t);
	option_menu.addobj(createplanet, checkbox_t);
	option_menu.addobj(createstar, checkbox_t);

	object_menu.init("Object Options", 0.0, 0.0, 0.4);
	object_info.init("Object Information", 0.0, 0.0, 0.45);

	// Load Shaders
	int TextLen;
	char ErrText[256];

	TextLen = 256;
	default_vx = LoadGLSLShader(GL_VERTEX_SHADER, "res/bge_core/default.vx", ErrText, &TextLen);
	printf("%s", ErrText);

	TextLen = 256;
	default_ft = LoadGLSLShader(GL_FRAGMENT_SHADER, "res/bge_core/default.ft", ErrText, &TextLen);
	printf("%s", ErrText);

	default_pm = glCreateProgram();
	glAttachShader(default_pm, default_vx);
	glAttachShader(default_pm, default_ft);
	glLinkProgram(default_pm);

	font = new fonthdl();
	font->initialize("res/bge_gui/font.tga", "res/bge_gui/font.vx", "res/bge_gui/font.ft");

	// Load Automatic Texture Generation
	chemistry.load((char*)"res/bge_chem/", 107);

	// Initialize some OpenGL Settings
	glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GEQUAL, 0.05);
	glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glClearDepth(1.0);
	glEnable(GL_CULL_FACE);

	glEnable(GL_LIGHTING);
	{
		GLfloat pos[] = {1.0, 0.0, 0.0, 0.0};
		GLfloat spec[] = {0.0, 0.0, 0.0, 1.0};
		GLfloat ambi[] = {0.0, 0.0, 0.0, 1.0};
		GLfloat diff[] = {1.0, 1.0, 1.0, 1.0};

		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambi);
		glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
		glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
		glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
		glEnable(GL_LIGHT0);
		{
			glLightfv(GL_LIGHT0, GL_POSITION, pos);
			glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
			glLightfv(GL_LIGHT0, GL_SPECULAR, spec);
			glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, -1);
			glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, -1);
			glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, -1);
		}
	}

	background.initialize();
	addobject(NULL, OBJ_SYSTEM);
	//addobject(start, OBJ_PLANT);

	// Initialize Threading
	num_threads = 1;
	if (num_threads > 0)
	{
		threads = new pthread_t[num_threads];
		for (int x = 0; x < num_threads; x++)
			pthread_create(&threads[x], NULL, run_thread, this);
		done = false;
	}
}

void canvashdl::release()
{
	printf("Average Framerate: %f\n", overall_frame_rate);
	done = true;
	if (threads != NULL)
	{
		for (int x = 0; x < num_threads; x++)
			pthread_join(threads[x], NULL);

		delete [] threads;
	}

	objecthdl *obj = start, *prev;
	while (obj != NULL)
	{
		prev = obj;
		obj = obj->next;
		prev->release();
		delete prev;
		prev = NULL;
	}

	chemistry.release();
	palette.release();
	background.release();

	diagnostics.release();
	option_menu.release();
	object_menu.release();
	object_info.release();

	if (font != NULL)
	{
		font->release();
		delete font;
	}
	font = NULL;

	if (default_vx != 0)
		glDeleteShader(default_vx);
	if (default_ft != 0)
		glDeleteShader(default_ft);
	if (default_pm != 0)
		glDeleteShader(default_pm);

	height = 0;
	width = 0;

	frame_number = 0.0;
	// average frame rate calculations
	overall_start_time = 0.0;
	overall_current_time = 0.0;
	overall_frame_rate = 0.0;

	time_elapsed = 0.0;

	// instantaneous frame rate calculations
	start_frame = 0.0;
	start_time = 0.0;
	frame_rate = 0.0;

	// gui stuff
	create_camera = false;

	// threading
	done = false;
	num_threads = 0;
	threads = NULL;

	mouse = NULL;
	keys = NULL;

	active_camera = NULL;
	start = NULL;
	end = NULL;

	default_vx = 0;
	default_ft = 0;
	default_pm = 0;
}

void canvashdl::prepare()
{
	objecthdl *obj = start;
	while (obj != NULL)
	{
		obj->prepare();
		obj = obj->next;
	}

	/*double m;
	vec d;
	if (start != NULL)
	{
		objecthdl *curr1 = start->children_s, *curr2;
		while (curr1 != NULL)
		{
			//if (curr1->children_s == NULL)
			//{
				curr2 = curr1->next;
				while (curr2 != NULL)
				{
					//if (curr2->children_s == NULL && curr1 != curr2)
					//{
						m = gravitational_force(curr1, curr2);
						d = norm((curr2->position - curr1->position).tovec());
						curr1->forces += m*d;
						curr2->forces -= m*d;
					//}

					curr2 = curr2->next;
				}
			//}
			curr1 = curr1->next;
		}
	}*/
}

void canvashdl::render()
{
	handle_keys();
	handle_clicks();

	frame_number++;
	overall_frame_rate = frame_number/time();

	if (frame_number - start_frame >= 5)
	{
		frame_rate = (frame_number - start_frame)/(overall_current_time - start_time);
		start_frame = frame_number;
		start_time = overall_current_time;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// render perspective
	if (active_camera != NULL)
		active_camera->frame();

	// render background
	if (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	background.render();

	glClear(GL_DEPTH_BUFFER_BIT);

	// render objects
	if (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (active_camera != NULL)
		active_camera->render_hierarchy();

	// render menus
	mouse->getdepth();
	if (diagnostics.visible || option_menu.visible || object_menu.visible || object_info.visible)
	{
		glClear(GL_DEPTH_BUFFER_BIT);
		glUseProgram(default_pm);
		glDisable(GL_LIGHTING);

		if (wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glPushMatrix();
		glLoadIdentity();
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();

		diagnostics.render(font);
		option_menu.render(font);
		object_menu.render(font);
		object_info.render(font);

		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

		glEnable(GL_LIGHTING);
	}

	glutSwapBuffers();

	if (create_camera)
	{
		create_camera = false;
		camerahdl *temp;
		if (active_camera == NULL)
		{
			temp = (camerahdl*)addobject(start, OBJ_CAMERA);
			temp->setup(-1.6/10.0, 1.6/10.0, -1.0/10.0, 1.0/10.0, 0.2, 5000000.0);//999999999);
			temp->position = location(100000000.0, 0.0, 0.0);
			temp->projection();
			active_camera = temp;
		}
		else
		{
			temp = (camerahdl*)addobject(active_camera->parent, OBJ_CAMERA);
			temp->setup(-1.6/10.0, 1.6/10.0, -1.0/10.0, 1.0/10.0, 0.2, 5000000.0);//999999999);
		}
	}
	if (create_planet)
	{
		create_planet = false;
		/*planethdl *temp;
		temp = (planethdl*)*/addobject(start, OBJ_PLANET);
	}
	if (create_star)
	{
		create_star = false;
		/*starhdl *temp;
		temp = (starhdl*)*/addobject(start, OBJ_STAR);
	}
}

void canvashdl::addobject(objecthdl *object)
{
	if (object->parent != NULL)
		object->parent->remove_child(object);

	if (start == NULL)
	{
		start = object;
		end = object;
	}
	else
	{
		end->next = object;
		object->prev = end;
		end = end->next;
	}
}

objecthdl *canvashdl::addobject(objecthdl *master, int type)
{
	objecthdl *newobject = NULL;
	if (type == OBJ_BASE)
		newobject = new objecthdl();
	else if (type == OBJ_CAMERA)
		newobject = new camerahdl();
	else if (type == OBJ_PLANET)
		newobject = new planethdl();
	else if (type == OBJ_STAR)
		newobject = new starhdl();
	else if (type == OBJ_SYSTEM)
		newobject = new systemhdl();
	else if (type == OBJ_PLANT)
		newobject = new planthdl();

	if (newobject == NULL)
		return NULL;

	newobject->super = this;
	newobject->initialize();

	if (master == NULL)
	{
		if (start == NULL)
		{
			start = newobject;
			end = newobject;
		}
		else
		{
			end->next = newobject;
			newobject->prev = end;
			end = end->next;
		}
	}
	else
		master->add_child(newobject);

	if (active_camera != NULL)
	{
		newobject->position = active_camera->position;
		newobject->orientation = active_camera->orientation;
	}

	newobject->prepare();

	return newobject;
}

void canvashdl::removeobject(objecthdl *obj)
{
	if (obj == NULL)
		return;

	if (obj->next != NULL)
		obj->next->prev = obj->prev;

	if (obj->prev != NULL)
		obj->prev->next = obj->next;

	obj->release();
	delete obj;
}

void canvashdl::mouse_move(vec delta)
{
	if (mouse->bind)
	{
		double temp = delta.x;
		delta.x = -delta.y/double(getheight());
		delta.y = temp/double(getwidth());
		if (active_camera != NULL)
			active_camera->rotate(mouse->speed_mult*delta);
		object_info.visible = false;
		object_menu.visible = false;
		option_menu.visible = false;

		if (mouse->v > getheight()*3/4 || mouse->v < getheight()*1/4 || mouse->h > getwidth()*3/4 || mouse->h < getwidth()*1/4)
			mouse->setmouseloc(getwidth()/2, getheight()/2);
	}
	else
	{
		delta.y = getheight()-delta.y;
		vec gui_space = wintogui((double)mouse->h, (double)mouse->v);
		vec world_space = wintoworld((double)mouse->h, (double)mouse->v);
		vec gui_delta = wintogui(delta.x, delta.y) + 1.0;

		if (mouse->buttons[0])
		{
			object_info.move(gui_space, gui_delta);
			object_menu.move(gui_space, gui_delta);
			option_menu.move(gui_space, gui_delta);
			diagnostics.move(gui_space, gui_delta);
		}

		object_info.visible = false;

		if (active_camera != NULL)
		{
			objecthdl *obj = active_camera->parent->children_s;
			bool hovered = false;

			if (obj == NULL)
				obj = start;
			while (obj != NULL)
			{
				if (obj->selected && mouse->buttons[0])
				{
					double d = mag((obj->position - active_camera->position).tovec());
					obj->position = location(-d*world_space.x, -d*world_space.y, -d*world_space.z) + active_camera->position;
				}
				else
					obj->selected = false;

				hovered = obj->hovered;
				obj->hover(world_space);

				if (obj != active_camera && hovered && obj->hovered)
				{
					object_info.x = gui_space.x;
					object_info.y = gui_space.y;
					object_info.visible = true;
				}
				else if (obj != active_camera && obj->hovered)
				{
					object_info.x = gui_space.x;
					object_info.y = gui_space.y;
					object_info.release();
					object_info.visible = true;
					obj->generate_info(&object_info);
				}
				obj = obj->next;
			}
		}
	}
}

void canvashdl::mouse_change(int x, int y, int button)
{
	bool selected = false;
	if (mouse->bind)
	{
	}
	else
	{
		vec gui_space = wintogui((double)x, (double)y);
		vec world_space = wintoworld((double)x, (double)y);

		if (button == 0)
		{
			if (mouse->buttons[button])
			{
				selected = false;

				if (!selected)
					selected = object_info.select(gui_space);
				if (!selected)
					selected = object_menu.select(gui_space);
				if (!selected)
					selected = option_menu.select(gui_space);
				if (!selected)
					selected = diagnostics.select(gui_space);

				if (active_camera != NULL)
				{
					objecthdl *obj = NULL;
					if (active_camera->parent != NULL)
						obj = active_camera->parent->children_s;
					if (obj == NULL)
						obj = start;
					while (obj != NULL && !selected)
					{
						if (obj != active_camera && obj->select(world_space))
							selected = true;
						obj = obj->next;
					}
				}
			}
			else
			{
				object_info.unselect();
				object_menu.unselect();
				option_menu.unselect();
				diagnostics.unselect();
			}
		}
		else if (button == 2)
		{
			if (mouse->buttons[button])
			{
				selected = false;

				if (active_camera != NULL)
				{
					objecthdl *obj = NULL;
					if (active_camera->parent != NULL)
						obj = active_camera->parent->children_s;
					if (obj == NULL)
						obj = start;
					while (obj != NULL && !selected)
					{
						if (obj != active_camera && obj->select(world_space))
						{
							object_menu.x = gui_space.x;
							object_menu.y = gui_space.y;
							object_menu.release();
							object_menu.visible = true;
							obj->generate_menu(&object_menu);
							selected = true;
						}
						obj = obj->next;
					}
				}

				if (!selected)
				{
					option_menu.x = gui_space.x;
					option_menu.y = gui_space.y;
					option_menu.visible = !option_menu.visible;
					object_menu.visible = false;
				}
			}
			else
			{

			}
		}
	}
}

void canvashdl::handle_clicks()
{

}

void canvashdl::handle_keys()
{
	if (active_camera != NULL)
	{
		if (keys->keystates[(int)'w'])
			active_camera->translate(vec(0.0, 0.0, -0.0001), false);
		if (keys->keystates[(int)'a'])
			active_camera->translate(vec(-0.0001, 0.0, 0.0), false);
		if (keys->keystates[(int)'s'])
			active_camera->translate(vec(0.0, 0.0, 0.0001), false);
		if (keys->keystates[(int)'d'])
			active_camera->translate(vec(0.0001, 0.0, 0.0), false);
		if (keys->keystates[(int)'e'])
			active_camera->translate(vec(0.0, 0.0001, 0.0), false);
		if (keys->keystates[(int)'q'])
			active_camera->translate(vec(0.0, -0.0001, 0.0), false);
		if (keys->keystates[(int)','])
			active_camera->speed /= 1.1;
		if (keys->keystates[(int)'.'])
			active_camera->speed *= 1.1;
		if (keys->keystates[(int)'z'])
			active_camera->velocity *= 0.0;

		if (!mouse->bind)
		{
			vec gui_space = wintogui((double)mouse->h, (double)mouse->v);
			vec world_space = wintoworld((double)mouse->h, (double)mouse->v);

			object_info.visible = false;

			objecthdl *obj = active_camera->parent->children_s;
			bool hovered = false;

			if (obj == NULL)
				obj = start;
			while (obj != NULL)
			{
				if (obj->selected && mouse->buttons[0])
				{
					double d = mag((obj->position - active_camera->position).tovec());
					obj->position = location(-d*world_space.x, -d*world_space.y, -d*world_space.z) + active_camera->position;
				}
				else
					obj->selected = false;

				hovered = obj->hovered;
				obj->hover(world_space);

				if (obj != active_camera && hovered && obj->hovered)
				{
					object_info.x = gui_space.x;
					object_info.y = gui_space.y;
					object_info.visible = true;
				}
				else if (obj != active_camera && obj->hovered)
				{
					object_info.x = gui_space.x;
					object_info.y = gui_space.y;
					object_info.release();
					object_info.visible = true;
					obj->generate_info(&object_info);
				}
				obj = obj->next;
			}
		}
	}
}

double canvashdl::time()
{
	timeval gtime;
	gettimeofday(&gtime, NULL);
	overall_current_time = gtime.tv_sec + gtime.tv_usec*1.0E-6;

	time_elapsed = overall_current_time - overall_start_time;

	return time_elapsed;
}

double canvashdl::frame()
{
	return frame_number;
}

double canvashdl::framerate()
{
	return frame_rate;
}

void canvashdl::setviewport(int w, int h)
{
	glViewport(0, 0, w, h);
	width = w;
	height = h;
}

int canvashdl::getheight()
{
	return height;
}

int	canvashdl::getwidth()
{
	return width;
}

vec canvashdl::wintoworld(double u, double v)
{
	double x = 1.0 - u/double(getwidth()),
		   y = v/double(getheight()),
		   z = 0.0;

	if (active_camera == NULL)
		return vec(2.0*x - 1.0, 2.0*y - 1.0, z);
	else
	{
		x = x*(active_camera->frustum.rightf - active_camera->frustum.leftf) + active_camera->frustum.leftf;
		y = y*(active_camera->frustum.upf - active_camera->frustum.downf) + active_camera->frustum.downf;
		z = active_camera->frustum.nearf;

		vec ret = norm(vec(x, y, z));

		vec o1 = degtorad(active_camera->orientation);
		vec o2; // for parent object
		if (active_camera->parent != NULL)
			o2 = degtorad(active_camera->parent->orientation);

		space s;
		s.rotate_xyz(o1);
		s.rotate_zyx(o2);
		return s.transform(ret);
	}

	return vec();
}

vec canvashdl::wintogui(double u, double v)
{
	return vec(2.0*u/double(getwidth()) - 1.0, 1.0 - 2.0*v/double(getheight()), 0.0);
}
