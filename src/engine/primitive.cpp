#include "primitive.h"

/* boxhdl
 *
 * Generate the geometry and indices required to make a box.
 */
boxhdl::boxhdl(float width, float height, float depth)
{
	geometry.reserve(24);
	indices.reserve(36);

	geometry.push_back(vec8f(-width/2, -height/2, -depth/2, 0.0, 0.0, -1.0, 0.0, 0.0));
	geometry.push_back(vec8f( width/2, -height/2, -depth/2, 0.0, 0.0, -1.0, 1.0, 0.0));
	geometry.push_back(vec8f( width/2,  height/2, -depth/2, 0.0, 0.0, -1.0, 1.0, 1.0));
	geometry.push_back(vec8f(-width/2,  height/2, -depth/2, 0.0, 0.0, -1.0, 0.0, 1.0));
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(3);

	geometry.push_back(vec8f(-width/2, -height/2,  depth/2, -1.0, 0.0, 0.0, 0.0, 1.0));
	geometry.push_back(vec8f(-width/2, -height/2, -depth/2, -1.0, 0.0, 0.0, 0.0, 0.0));
	geometry.push_back(vec8f(-width/2,  height/2, -depth/2, -1.0, 0.0, 0.0, 1.0, 0.0));
	geometry.push_back(vec8f(-width/2,  height/2,  depth/2, -1.0, 0.0, 0.0, 1.0, 1.0));
	indices.push_back(4);
	indices.push_back(5);
	indices.push_back(6);
	indices.push_back(4);
	indices.push_back(6);
	indices.push_back(7);

	geometry.push_back(vec8f(-width/2, -height/2, -depth/2, 0.0, -1.0, 0.0, 0.0, 0.0));
	geometry.push_back(vec8f( width/2, -height/2, -depth/2, 0.0, -1.0, 0.0, 1.0, 0.0));
	geometry.push_back(vec8f( width/2, -height/2,  depth/2, 0.0, -1.0, 0.0, 1.0, 1.0));
	geometry.push_back(vec8f(-width/2, -height/2,  depth/2, 0.0, -1.0, 0.0, 0.0, 1.0));
	indices.push_back(10);
	indices.push_back(9);
	indices.push_back(8);
	indices.push_back(11);
	indices.push_back(10);
	indices.push_back(8);

	geometry.push_back(vec8f( width/2,  height/2,  depth/2, 0.0, 0.0, 1.0, 1.0, 1.0));
	geometry.push_back(vec8f( width/2, -height/2,  depth/2, 0.0, 0.0, 1.0, 1.0, 0.0));
	geometry.push_back(vec8f(-width/2, -height/2,  depth/2, 0.0, 0.0, 1.0, 0.0, 0.0));
	geometry.push_back(vec8f(-width/2,  height/2,  depth/2, 0.0, 0.0, 1.0, 0.0, 1.0));
	indices.push_back(12);
	indices.push_back(13);
	indices.push_back(14);
	indices.push_back(12);
	indices.push_back(14);
	indices.push_back(15);

	geometry.push_back(vec8f( width/2,  height/2,  depth/2, 0.0, 1.0, 0.0, 1.0, 1.0));
	geometry.push_back(vec8f(-width/2,  height/2,  depth/2, 0.0, 1.0, 0.0, 0.0, 1.0));
	geometry.push_back(vec8f(-width/2,  height/2, -depth/2, 0.0, 1.0, 0.0, 0.0, 0.0));
	geometry.push_back(vec8f( width/2,  height/2, -depth/2, 0.0, 1.0, 0.0, 1.0, 0.0));
	indices.push_back(16);
	indices.push_back(17);
	indices.push_back(18);
	indices.push_back(16);
	indices.push_back(18);
	indices.push_back(19);

	geometry.push_back(vec8f( width/2,  height/2,  depth/2, 1.0, 0.0, 0.0, 1.0, 1.0));
	geometry.push_back(vec8f( width/2,  height/2, -depth/2, 1.0, 0.0, 0.0, 1.0, 0.0));
	geometry.push_back(vec8f( width/2, -height/2, -depth/2, 1.0, 0.0, 0.0, 0.0, 0.0));
	geometry.push_back(vec8f( width/2, -height/2,  depth/2, 1.0, 0.0, 0.0, 0.0, 1.0));
	indices.push_back(20);
	indices.push_back(21);
	indices.push_back(22);
	indices.push_back(20);
	indices.push_back(22);
	indices.push_back(23);
}

boxhdl::~boxhdl()
{

}

/* spherehdl
 *
 * Generate the geometry and indices required to make a sphere.
 */
spherehdl::spherehdl(float radius, int levels, int slices)
{
	geometry.reserve(2 + (levels-1)*slices);
	for (int i = 0; i <= levels; i++)
		for (int j = 0; j < slices; j++)
		{
			vec3f dir(sin(m_pi*(float)i/(float)levels)*cos(2.0*m_pi*(float)j/(float)(slices-1)),
					  sin(m_pi*(float)i/(float)levels)*sin(2.0*m_pi*(float)j/(float)(slices-1)),
					  cos(m_pi*(float)i/(float)levels));
			geometry.push_back(vec8f(radius*dir[0], radius*dir[1], radius*dir[2],
									 dir[0], dir[1], dir[2], (float)j/(float)(slices-1), (float)i/(float)levels));
		}

	for (int i = 0; i < levels; i++)
		for (int j = 0; j < slices-1; j++)
		{
			indices.push_back(i*slices + j);
			indices.push_back(i*slices + (j+1)%slices);
			indices.push_back((i+1)*slices + j);

			indices.push_back((i+1)*slices + j);
			indices.push_back(i*slices + (j+1)%slices);
			indices.push_back((i+1)*slices + (j+1)%slices);
		}
}

spherehdl::~spherehdl()
{

}

/* cylinderhdl
 *
 * Generate the geometry and indices required to make a cylinder.
 */
cylinderhdl::cylinderhdl(float radius, float height, int slices)
{
	geometry.push_back(vec8f(0.0, -height/2.0, 0.0, 0.0, -1.0, 0.0, 0.5, 0.5));
	for (int i = 0; i <= slices; i++)
		geometry.push_back(vec8f(radius*cos(2*m_pi*(float)i/(float)slices),
								 -height/2.0,
								 radius*sin(2*m_pi*(float)i/(float)slices),
								 0.0, -1.0, 0.0, (cos(2*m_pi*(float)i/(float)slices)+1.0)/2.0, (sin(2*m_pi*(float)i/(float)slices)+1.0)/2.0));

	for (int i = 0; i <= slices; i++)
		geometry.push_back(vec8f(radius*cos(2*m_pi*(float)i/(float)slices),
								 -height/2.0,
								 radius*sin(2*m_pi*(float)i/(float)slices),
								 cos(2*m_pi*(float)i/(float)slices),
								 0.0,
								 sin(2*m_pi*(float)i/(float)slices), 3.0*(float)i/(float)slices, 0.0));

	for (int i = 0; i <= slices; i++)
		geometry.push_back(vec8f(radius*cos(2*m_pi*(float)i/(float)slices),
								 height/2.0,
								 radius*sin(2*m_pi*(float)i/(float)slices),
								 cos(2*m_pi*(float)i/(float)slices),
								 0.0,
								 sin(2*m_pi*(float)i/(float)slices), 3.0*(float)i/(float)slices, 1.0));

	for (int i = 0; i <= slices; i++)
		geometry.push_back(vec8f(radius*cos(2*m_pi*(float)i/(float)slices),
								 height/2.0,
								 radius*sin(2*m_pi*(float)i/(float)slices),
								 0.0, 1.0, 0.0, (cos(2*m_pi*(float)i/(float)slices)+1.0)/2.0, (sin(2*m_pi*(float)i/(float)slices)+1.0)/2.0));

	geometry.push_back(vec8f(0.0, height/2.0, 0.0, 0.0, 1.0, 0.0, 0.5, 0.5));

	for (int i = 0; i < slices; i++)
	{
		indices.push_back(1 + (i+1)%(slices+1));
		indices.push_back(1 + i);
		indices.push_back(0);
	}

	for (int i = 0; i < slices; i++)
	{
		indices.push_back(1 + (slices+1) + i);
		indices.push_back(1 + (slices+1) + (i+1)%(slices+1));
		indices.push_back(1 + 2*(slices+1) + i);

		indices.push_back(1 + 2*(slices+1) + i);
		indices.push_back(1 + (slices+1) + (i+1)%(slices+1));
		indices.push_back(1 + 2*(slices+1) + (i+1)%(slices+1));
	}

	for (int i = 0; i < slices; i++)
	{
		indices.push_back(1 + 3*(slices+1) + i);
		indices.push_back(1 + 3*(slices+1) + (i+1)%(slices+1));
		indices.push_back(1 + 4*(slices+1));
	}
}

cylinderhdl::~cylinderhdl()
{

}

/* pyramidhdl
 *
 * Generate the geometry and indices required to make a pyramid.
 */
pyramidhdl::pyramidhdl(float radius, float height, int slices)
{
	float nheight = sqrt(1.0f/(1.0f + (height*height)/(radius*radius)));
	float nlength = height*nheight/radius;

	geometry.push_back(vec8f(0.0, -height/2.0, 0.0, 0.0, -1.0, 0.0, 0.5, 0.5));
	for (int i = 0; i <= slices; i++)
		geometry.push_back(vec8f(radius*cos(2*m_pi*(float)i/(float)slices),
								 -height/2.0,
								 radius*sin(2*m_pi*(float)i/(float)slices),
								 0.0, -1.0, 0.0, (cos(2*m_pi*(float)i/(float)slices)+1.0)/2.0, (sin(2*m_pi*(float)i/(float)slices)+1.0)/2.0));

	for (int i = 0; i <= slices; i++)
		geometry.push_back(vec8f(radius*cos(2*m_pi*(float)i/(float)slices),
								 -height/2.0,
								 radius*sin(2*m_pi*(float)i/(float)slices),
								 nlength*cos(2*m_pi*(float)i/(float)slices),
								 nheight,
								 nlength*sin(2*m_pi*(float)i/(float)slices), 3.0*(float)i/(float)slices, 0.0));

	for (int i = 0; i <= slices; i++)
		geometry.push_back(vec8f(0.0, height/2.0, 0.0,
								 nlength*cos(2*m_pi*((float)i + 0.5)/(float)slices),
								 nheight,
								 nlength*sin(2*m_pi*((float)i + 0.5)/(float)slices), 3.0*(float)i/(float)slices, 1.0));
	//geometry.push_back(vec8f(0.0, height/2.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0));

	for (int i = 0; i < slices; i++)
	{
		indices.push_back(1 + (i+1)%(slices+1));
		indices.push_back(1 + i);
		indices.push_back(0);
	}

	for (int i = 0; i < slices; i++)
	{
		indices.push_back(1 + (slices+1) + i);
		indices.push_back(1 + (slices+1) + (i+1)%(slices+1));
		indices.push_back(1 + 2*(slices+1) + i);
	}
}

pyramidhdl::~pyramidhdl()
{

}
