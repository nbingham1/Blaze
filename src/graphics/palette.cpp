/*
 * palette.cpp
 *
 *  Created on: Aug 18, 2011
 *      Author: Ned Bingham
 */

#include "palette.h"

palettehdl::palettehdl()
{
}

palettehdl::~palettehdl()
{
	for (list_node<pair<string, materialhdl> > *n = materials.begin(); n != materials.end(); n = n->next)
		n->second.release();
	materials.clear();
}

materialhdl *palettehdl::insert(string path)
{
	list_node<pair<string, materialhdl> > *result = materials.find(path);
	if (result == materials.end())
		result = materials.insert(path, materialhdl(path));

	return &(result->second);
}
