/*
 * map.h
 *
 *  Created on: Feb 5, 2014
 *      Author: nbingham
 */

#include "list.h"
#include "pair.h"

#ifndef map_h
#define map_h

template <class ktype, class vtype>
struct map : list<pair<ktype, vtype> >
{
	map()
	{

	}

	~map()
	{

	}

	list_node<pair<ktype, vtype> > *find(ktype key)
	{
		for (list_node<pair<ktype, vtype> > *i = this->begin(); i != this->end(); i = i->next)
			if (i->first == key)
				return i;

		return NULL;
	}

	list_node<pair<ktype, vtype> > *insert(ktype key, vtype value)
	{
		list_node<pair<ktype, vtype> > *pos = NULL;
		for (pos = list<pair<ktype, vtype> >::first; pos != NULL && pos->first < key; pos = pos->next);

		if (pos != NULL && pos->first == key)
		{
			pos->second = value;
			return pos;
		}
		else
		{
			list_node<pair<ktype, vtype> > *n = new list_node<pair<ktype, vtype> >(pair<ktype, vtype>(key, value));
			n->next = pos;

			if (pos != NULL)
				n->prev = pos->prev;

			if (n->prev != NULL)
				n->prev->next = n;
			else
				list<pair<ktype, vtype> >::first = n;

			if (n->next != NULL)
				n->next->prev = n;
			else
				list<pair<ktype, vtype> >::last = n;

			n->parent = this;
			list<pair<ktype, vtype> >::count++;

			return n;
		}
	}

	vtype &operator[](ktype key)
	{
		return this->find(key)->second;
	}
};

#endif
