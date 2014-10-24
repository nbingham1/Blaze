#include "object.h"

void objecthdl::init()
{
	//physics.init();
	//material.init();

	next = NULL;
	prev = NULL;
}

void objecthdl::render()
{
	physics.push();
	//material.bind();
	glutSolidSphere(10, 8, 8);
	physics.pop();
}
