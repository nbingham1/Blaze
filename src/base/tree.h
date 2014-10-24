/*
 * tree.h
 *
 *  Created on: Feb 15, 2014
 *      Author: nbingham
 */

#include "list.h"
#include "array.h"

#ifndef tree_h
#define tree_h

#ifndef NULL
#define NULL 0
#endif

template <class type>
struct ltree : type
{
	ltree()
	{
		parent = NULL;
	}

	ltree(const ltree<type> &n) : type((type)n)
	{
		parent = n.parent;
		for (list_node<ltree<type> > *c = n.children.begin(); c != n.children.end(); c = c->next)
			this->children.push_back(ltree<type>(*c, this));
	}

	ltree(const ltree<type> &n, ltree<type> *parent) : type((type)n)
	{
		this->parent = parent;
		for (list_node<ltree<type> > *c = n.children.begin(); c != n.children.end(); c = c->next)
			this->children.push_back(ltree<type>(*c, this));
	}

	ltree(const type &n) : type(n)
	{
		this->parent = NULL;
	}

	~ltree()
	{
	}

	ltree<type> *parent;
	list<ltree<type> > children;

	list_node<ltree<type> > *begin() const
	{
		return children.begin();
	}

	list_node<ltree<type> > *end() const
	{
		return children.end();
	}

	list_node<ltree<type> > *rbegin() const
	{
		return children.begin();
	}

	list_node<ltree<type> > *rend() const
	{
		return children.end();
	}

	ltree<type> &front() const
	{
		return children.front();
	}

	ltree<type> &back() const
	{
		return children.back();
	}

	bool empty() const
	{
		return children.empty();
	}

	int size() const
	{
		return children.size();
	}

	list_node<ltree<type> > *push_back(type t)
	{
		list<ltree<type> > *result = children.push_back(t);
		result->parent = this;
		return result;
	}

	list_node<ltree<type> > *push_front(type t)
	{
		list<ltree<type> > *result = children.push_front(t);
		result->parent = this;
		return result;
	}

	list_node<ltree<type> > *push_back(list_node<ltree<type> > *t)
	{
		list<ltree<type> > *result = children.push_back(t);
		result->parent = this;
		return result;
	}

	list_node<ltree<type> > *push_front(list_node<ltree<type> > *t)
	{
		list<ltree<type> > *result = children.push_front(t);
		result->parent = this;
		return result;
	}

	list_node<ltree<type> > *pop_back()
	{
		children.back().parent = NULL;
		return children.pop_back();
	}

	list_node<ltree<type> > *push_front()
	{
		children.front().parent = NULL;
		return children.pop_front();
	}

	void drop_back()
	{
		children.drop_back();
	}

	void drop_front()
	{
		children.drop_front();
	}

	list_node<ltree<type> > *remove(list_node<ltree<type> > *n)
	{
		if (n->parent == this)
		{
			n->parent = NULL;
			return children.remove(n);
		}
		else
			return NULL;
	}

	list_node<ltree<type> > *erase(list_node<ltree<type> > *n)
	{
		if (n->parent == this)
			return children.erase(n);
		else
			return NULL;
	}

	list_node<type> *rerase(list_node<type> *n)
	{
		if (n->parent == this)
			return children.rerase(n);
		else
			return NULL;
	}

	void clear()
	{
		children.clear();
	}

	operator type()
	{
		return *(type*)this;
	}

	ltree<type> &operator=(ltree<type> n)
	{
		type::operator=(n);
		parent = NULL;
		for (list_node<ltree<type> > *c = n.children.begin(); c != n.children.end(); c = c->next)
			this->children.push_back(ltree<type>(*c, this));
		return *this;
	}
};

template <class type>
bool operator==(ltree<type> n1, ltree<type> n2)
{
	return ((type)n1 == (type)n2);
}

template <class type>
bool operator!=(ltree<type> n1, ltree<type> n2)
{
	return ((type)n1 != (type)n2);
}

template <class type>
bool operator<(ltree<type> n1, ltree<type> n2)
{
	return ((type)n1 < (type)n2);
}

template <class type>
bool operator>(ltree<type> n1, ltree<type> n2)
{
	return ((type)n1 > (type)n2);
}

template <class type>
bool operator<=(ltree<type> n1, ltree<type> n2)
{
	return ((type)n1 <= (type)n2);
}

template <class type>
bool operator>=(ltree<type> n1, ltree<type> n2)
{
	return ((type)n1 >= (type)n2);
}

template <class type>
struct iltree : ilist<type>
{
	iltree()
	{
		parent = NULL;
		prev = NULL;
		next = NULL;
	}

	iltree(const iltree<type> &n) : ilist<type>(n)
	{
		parent = n.parent;
		prev = n.prev;
		next = n.next;
	}

	~iltree()
	{
	}

	type *parent;
	type *prev;
	type *next;

	iltree<type> &operator=(iltree<type> n)
	{
		parent = n.parent;
		prev = n.prev;
		next = n.next;
		ilist<type>::operator=(n);
		return *this;
	}
};

template <class type>
struct iatree : array<type>
{
	iatree()
	{
		parent = NULL;
	}

	iatree(const iatree<type> &n) : array<type>(n)
	{
		parent = n.parent;
	}

	~iatree()
	{
	}

	type *parent;

	iatree<type> &operator=(iatree<type> n)
	{
		parent = n.parent;
		array<type>::operator=(n);
		return *this;
	}
};

#endif
