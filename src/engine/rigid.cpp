#include "rigid.h"
#include "graphics/opengl.h"

rigidhdl::rigidhdl()
{
}

rigidhdl::~rigidhdl()
{

}

void rigidhdl::render(int vertex_location, int normal_location, int texcoord_location)
{
	glEnableVertexAttribArray(vertex_location);
	glVertexAttribPointer(vertex_location, 3, GL_FLOAT, false, 32, geometry.data);

	if (normal_location >= 0)
	{
		glEnableVertexAttribArray(normal_location);
		glVertexAttribPointer(normal_location, 3, GL_FLOAT, true, 32, geometry.data);
	}

	if (texcoord_location >= 0)
	{
		glEnableVertexAttribArray(texcoord_location);
		glVertexAttribPointer(texcoord_location, 2, GL_FLOAT, false, 32, geometry.data);
	}

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data);

	glDisableVertexAttribArray(vertex_location);

	if (normal_location >= 0)
		glDisableVertexAttribArray(normal_location);

	if (texcoord_location >= 0)
		glDisableVertexAttribArray(texcoord_location);
}
