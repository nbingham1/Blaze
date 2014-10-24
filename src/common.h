/*
 * common.h
 *
 *  Created on: Aug 18, 2011
 *      Author: Ned Bingham
 */

#ifndef common_h
#define common_h

#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <math.h>
#include <sys/time.h>

#include <iostream>

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

using namespace std;

template <class type>
struct node : type
{
	node()
	{
		owning_list = NULL;
		prev = NULL;
		next = NULL;
	}

	node(const type &t) : type(t)
	{
		owning_list = NULL;
		prev = NULL;
		next = NULL;
	}

	~node()
	{

	}

	void *owning_list;
	node<type> *prev;
	node<type> *next;
};

template <class type>
struct list
{
private:
	node<type> *first;
	node<type> *last;
	int count;

public:
	list()
	{
		first = NULL;
		last = NULL;
		count = 0;
	}

	list(const list<type> &l)
	{
		first = NULL;
		last = NULL;
		count = 0;
		for (node<type> *n = l.first; n != NULL; n = n->next)
			push_back(*(type*)n);
	}

	~list()
	{
		clear();
	}

	node<type> *begin()
	{
		return first;
	}

	node<type> *end()
	{
		return NULL;
	}

	node<type> *rbegin()
	{
		return last;
	}

	node<type> *rend()
	{
		return NULL;
	}

	int size()
	{
		return count;
	}

	node<type> *push_back(type t)
	{
		node<type> *n = new node<type>(t);
		if (first == NULL || last == NULL)
		{
			first = n;
			last = n;
		}
		else
		{
			last->next = n;
			n->prev = last;
			last = n;
		}

		n->owning_list = this;
		count++;

		return n;
	}

	node<type> *push_front(type t)
	{
		node<type> *n = new node<type>(t);
		if (first == NULL || last == NULL)
		{
			first = n;
			last = n;
		}
		else
		{
			first->prev = n;
			n->next = first;
			first = first->prev;
		}

		n->owning_list = this;
		count++;

		return n;
	}

	node<type> *pop_front()
	{
		node<type> *result = first;

		if (first == NULL)
		{
			cerr << "Error: Tried to pop a type off the front of an empty list." << endl;
			return NULL;
		}
		else if (first == last)
		{
			first = NULL;
			last = NULL;
		}
		else
		{
			first = first->next;
			first->prev = NULL;
		}

		result->owning_list = NULL;
		result->next = NULL;
		result->prev = NULL;
		count--;

		return result;
	}

	node<type> *pop_back()
	{
		node<type> *result = last;

		if (last == NULL)
		{
			cerr << "Error: Tried to pop a type off the back of an empty list." << endl;
			return NULL;
		}
		else if (first == last)
		{
			first = NULL;
			last = NULL;
		}
		else
		{
			last = last->prev;
			last->next = NULL;
		}

		result->owning_list = NULL;
		result->next = NULL;
		result->prev = NULL;
		count--;

		return result;
	}

	void drop_front()
	{
		node<type> *result = first;

		if (first == NULL)
		{
			cerr << "Error: Tried to drop a type off the front of an empty list." << endl;
			return;
		}
		else if (first == last)
		{
			first = NULL;
			last = NULL;
		}
		else
		{
			first = first->next;
			first->prev = NULL;
		}

		delete result;
		count--;
	}

	void drop_back()
	{
		node<type> *result = last;

		if (last == NULL)
		{
			cerr << "Error: Tried to drop a type off the back of an empty list." << endl;
			return;
		}
		else if (first == last)
		{
			first = NULL;
			last = NULL;
		}
		else
		{
			last = last->prev;
			last->next = NULL;
		}

		delete result;
		count--;
	}

	node<type> *remove(node<type> *n)
	{
		if (n == NULL)
		{
			cerr << "Error: Tried to remove NULL type from list." << endl;
			return n;
		}
		else if (n->owning_list != this)
		{
			cerr << "Error: Tried to remove type from a list to which it did not belong." << endl;
			return n;
		}
		else if (n == first && n == last)
		{
			first = NULL;
			last = NULL;
		}
		else if (n == first)
		{
			first = first->next;
			first->prev = NULL;
		}
		else if (n == last)
		{
			last = last->prev;
			last->next = NULL;
		}
		else
		{
			n->prev->next = n->next;
			n->next->prev = n->prev;
		}

		n->prev = NULL;
		n->next = NULL;
		n->owning_list = NULL;
		count--;

		return n;
	}

	node<type> *erase(node<type> *n)
	{
		node<type> *result = n->next;

		if (n == NULL)
		{
			cerr << "Error: Tried to erase NULL type from list." << endl;
			return NULL;
		}
		else if (n->owning_list != this)
		{
			cerr << "Error: Tried to erase type from a list to which it did not belong." << endl;
			return NULL;
		}
		else if (n == first && n == last)
		{
			first = NULL;
			last = NULL;
		}
		else if (n == first)
		{
			first = first->next;
			first->prev = NULL;
		}
		else if (n == last)
		{
			last = last->prev;
			last->next = NULL;
		}
		else
		{
			n->prev->next = n->next;
			n->next->prev = n->prev;
		}

		delete n;
		count--;
		return result;
	}

	void clear()
	{
		node<type> *curr = first;
		node<type> *prev;
		while (curr != NULL)
		{
			prev = curr;
			curr = curr->next;
			delete prev;
			prev = NULL;
		}

		first = NULL;
		last = NULL;
		count = 0;
	}

	void release()
	{
		node<type> *curr = first;
		node<type> *prev;
		while (curr != NULL)
		{
			prev = curr;
			curr = curr->next;
			delete prev;
			prev = NULL;
		}

		first = NULL;
		last = NULL;
		count = 0;
	}

	list<type> &operator=(list<type> l)
	{
		clear();
		for (node<type> *n = l.begin(); n != l.end(); n = n->next)
			push_back(*(type*)n);

		return *this;
	}

	node<type> *insert_after(node<type> *pos, type t)
	{
		if (pos == NULL)
			return push_front(t);
		else if (pos->owning_list != this)
		{
			cerr << "Error: Confused insertion." << endl;
			return NULL;
		}
		else
		{
			node<type> *n = new node<type>(t);
			n->prev = pos;
			n->next = pos->next;
			n->prev->next = n;

			if (n->next != NULL)
				n->next->prev = n;
			else
				last = n;

			n->owning_list = this;
			count++;

			return n;
		}
	}

	node<type> *insert_before(node<type> *pos, type t)
	{
		if (pos == NULL)
			return push_back(t);
		else if (pos->owning_list != this)
		{
			cerr << "Error: Confused insertion." << endl;
			return NULL;
		}
		else
		{
			node<type> *n = new node<type>(t);
			n->next = pos;
			n->prev = pos->prev;
			n->next->prev = n;

			if (n->prev != NULL)
				n->prev->next = n;
			else
				first = n;

			n->owning_list = this;
			count++;
			return n;
		}
	}

	node<type> *erase(node<type> *rfirst, node<type> *rlast)
	{
		node<type> *nlast = rlast->next;
		for (node<type> *n = rfirst; n != nlast; n = erase(n));
		return nlast;
	}
};

#endif
