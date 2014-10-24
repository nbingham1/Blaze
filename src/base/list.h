/*
 * list.h
 *
 *  Created on: Feb 4, 2014
 *      Author: nbingham
 */

#ifndef list_h
#define list_h

#ifndef NULL
#define NULL 0
#endif

template <class type>
struct list;

template <class type>
struct list_node : type
{
	list_node()
	{
		parent = NULL;
		prev = NULL;
		next = NULL;
	}

	list_node(const list_node<type> &n) : type((type)n)
	{
		parent = n.parent;
		prev = n.prev;
		next = n.next;
	}

	list_node(const type &t) : type(t)
	{
		parent = NULL;
		prev = NULL;
		next = NULL;
	}

	~list_node()
	{

	}

	list<type> *parent;
	list_node<type> *prev;
	list_node<type> *next;

	operator type()
	{
		return *(type*)this;
	}

	list_node<type> &operator=(list_node<type> n)
	{
		type::operator=(n);
		parent = n.parent;
		prev = n.prev;
		next = n.next;
		return *this;
	}
};

template <class type>
bool operator==(list_node<type> n1, list_node<type> n2)
{
	return ((type)n1 == (type)n2);
}

template <class type>
bool operator!=(list_node<type> n1, list_node<type> n2)
{
	return ((type)n1 != (type)n2);
}

template <class type>
bool operator<(list_node<type> n1, list_node<type> n2)
{
	return ((type)n1 < (type)n2);
}

template <class type>
bool operator>(list_node<type> n1, list_node<type> n2)
{
	return ((type)n1 > (type)n2);
}

template <class type>
bool operator<=(list_node<type> n1, list_node<type> n2)
{
	return ((type)n1 <= (type)n2);
}

template <class type>
bool operator>=(list_node<type> n1, list_node<type> n2)
{
	return ((type)n1 >= (type)n2);
}

template <>
struct list_node<int>
{
	list_node()
	{
		parent = NULL;
		prev = NULL;
		next = NULL;
		data = 0;
	}

	list_node(const list_node<int> &n) : data(n.data)
	{
		parent = n.parent;
		prev = n.prev;
		next = n.next;
	}

	list_node(const int &t) : data(t)
	{
		parent = NULL;
		prev = NULL;
		next = NULL;
	}

	~list_node()
	{

	}

	int data;
	list<int> *parent;
	list_node<int> *prev;
	list_node<int> *next;

	operator int()
	{
		return data;
	}

	list_node<int> &operator=(list_node<int> n)
	{
		data = n.data;
		parent = n.parent;
		prev = n.prev;
		next = n.next;
		return *this;
	}
};

template <>
struct list_node<float>
{
	list_node()
	{
		parent = NULL;
		prev = NULL;
		next = NULL;
		data = 0.0f;
	}

	list_node(const list_node<float> &n) : data(n.data)
	{
		parent = n.parent;
		prev = n.prev;
		next = n.next;
	}

	list_node(const float &t) : data(t)
	{
		parent = NULL;
		prev = NULL;
		next = NULL;
	}

	~list_node()
	{

	}

	float data;
	list<float> *parent;
	list_node<float> *prev;
	list_node<float> *next;

	operator float() const
	{
		return data;
	}

	list_node<float> &operator=(list_node<float> n)
	{
		data = n.data;
		parent = n.parent;
		prev = n.prev;
		next = n.next;
		return *this;
	}
};

template <class type>
struct list
{
	list()
	{
		first = NULL;
		last = NULL;
		count = 0;
	}

	list(int num, type t)
	{
		first = NULL;
		last = NULL;
		count = 0;
		for (int i = 0; i < num; i++)
			push_back(t);
	}

	list(const list<type> &l)
	{
		first = NULL;
		last = NULL;
		count = 0;
		for (list_node<type> *n = l.first; n != NULL; n = n->next)
			push_back(*n);
	}

	~list()
	{
		clear();
	}

	list_node<type> *first;
	list_node<type> *last;
	int count;

	list_node<type> *begin() const
	{
		return first;
	}

	list_node<type> *end() const
	{
		return NULL;
	}

	list_node<type> *rbegin() const
	{
		return last;
	}

	list_node<type> *rend() const
	{
		return NULL;
	}

	int size() const
	{
		return count;
	}

	bool empty() const
	{
		return (count == 0);
	}

	type &front() const
	{
		return *first;
	}

	type &back() const
	{
		return *last;
	}

	list_node<type> *push_back(type t)
	{
		list_node<type> *n = new list_node<type>(t);
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

		n->parent = this;
		count++;

		return n;
	}

	list_node<type> *push_back(list_node<type> *t)
	{
		if (t == NULL || t->parent != NULL)
			return NULL;

		if (first == NULL || last == NULL)
		{
			first = t;
			last = t;
		}
		else
		{
			last->next = t;
			t->prev = last;
			last = t;
		}

		t->parent = this;
		count++;

		return t;
	}

	list_node<type> *push_front(type t)
	{
		list_node<type> *n = new list_node<type>(t);
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

		n->parent = this;
		count++;

		return n;
	}

	list_node<type> *push_front(list_node<type> *t)
	{
		if (t == NULL || t->parent != NULL)
			return NULL;

		if (first == NULL || last == NULL)
		{
			first = t;
			last = t;
		}
		else
		{
			first->prev = t;
			t->next = first;
			first = first->prev;
		}

		t->parent = this;
		count++;

		return t;
	}

	list_node<type> *pop_front()
	{
		list_node<type> *result = first;

		if (first == NULL)
			return NULL;
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

		result->parent = NULL;
		count--;
		return result;
	}

	list_node<type> *pop_back()
	{
		list_node<type> *result = last;

		if (last == NULL)
			return NULL;
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

		result->parent = NULL;
		count--;
		return result;
	}

	void drop_front()
	{
		list_node<type> *result = first;

		if (first == NULL)
			return;
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
		list_node<type> *result = last;

		if (last == NULL)
			return;
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

	list_node<type> *remove(list_node<type> *n)
	{
		if (n == NULL || n->parent != this)
			return NULL;

		if (n->next)
			n->next->prev = n->prev;
		else
			last = n->prev;

		if (n->prev)
			n->prev->next = n->next;
		else
			first = n->next;

		n->parent = NULL;
		count--;

		return n;
	}

	void erase(type t)
	{
		for (list_node<type> *n = first; n != NULL;)
		{
			if (*n == t)
				n = erase(n);
			else
				n = n->next;
		}
	}

	list_node<type> *erase(list_node<type> *n)
	{
		list_node<type> *result = n->next;

		if (n == NULL || n->parent != this)
			return NULL;

		if (n->next)
			n->next->prev = n->prev;
		else
			last = n->prev;

		if (n->prev)
			n->prev->next = n->next;
		else
			first = n->next;

		delete n;
		count--;

		return result;
	}

	list_node<type> *rerase(list_node<type> *n)
	{
		list_node<type> *result = n->prev;

		if (n == NULL || n->parent != this)
			return NULL;

		if (n->next)
			n->next->prev = n->prev;
		else
			last = n->prev;

		if (n->prev)
			n->prev->next = n->next;
		else
			first = n->next;

		delete n;
		count--;

		return result;
	}

	list_node<type> *erase(list_node<type> *f, list_node<type> *l)
	{
		if (f == NULL || l == NULL || f->parent != this || f->parent != this)
			return NULL;

		list_node<type> result = l->next;

		if (f->prev)
			f->prev->next = l->next;
		else
			first = l->next;

		if (l->next)
			l->next->prev = f->prev;
		else
			last = f->prev;

		f->prev = NULL;
		l->next = NULL;

		list_node<type> *curr = f;
		list_node<type> *prev;
		while (curr != NULL)
		{
			prev = curr;
			curr = curr->next;
			delete prev;
			count--;
		}
		return result;
	}

	list_node<type> *rerase(list_node<type> *f, list_node<type> *l)
	{
		if (f == NULL || l == NULL || f->parent != this || f->parent != this)
			return NULL;

		list_node<type> result = l->prev;

		if (f->prev)
			f->prev->next = l->next;
		else
			first = l->next;

		if (l->next)
			l->next->prev = f->prev;
		else
			last = f->prev;

		f->prev = NULL;
		l->next = NULL;

		list_node<type> *curr = f;
		list_node<type> *prev;
		while (curr != NULL)
		{
			prev = curr;
			curr = curr->next;
			delete prev;
			count--;
		}
		return result;
	}

	void clear()
	{
		list_node<type> *curr = first;
		list_node<type> *prev;
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

	list_node<type> *insert(list_node<type> *pos, type t)
	{
		if (pos == NULL)
			return push_front(t);
		else if (pos->parent != this)
			return NULL;
		else
		{
			list_node<type> *n = new list_node<type>(t);
			n->prev = pos;
			n->next = pos->next;
			n->prev->next = n;

			if (n->next != NULL)
				n->next->prev = n;
			else
				last = n;

			n->parent = this;
			count++;

			return n;
		}
	}

	list_node<type> *rinsert(list_node<type> *pos, type t)
	{
		if (pos == NULL)
			return push_back(t);
		else if (pos->parent != this)
			return NULL;
		else
		{
			list_node<type> *n = new list_node<type>(t);
			n->next = pos;
			n->prev = pos->prev;
			n->next->prev = n;

			if (n->prev != NULL)
				n->prev->next = n;
			else
				first = n;

			n->parent = this;
			count++;
			return n;
		}
	}

	// assumes list is sorted
	list_node<type> *sort_insert(type t)
	{
		list_node<type> *pos = NULL;
		for (pos = first; pos != NULL && *(type*)pos < t; pos = pos->next);

		list_node<type> *n = new list_node<type>(t);
		n->next = pos;

		if (pos != NULL)
			n->prev = pos->prev;

		if (n->prev != NULL)
			n->prev->next = n;
		else
			first = n;

		if (n->next != NULL)
			n->next->prev = n;
		else
			last = n;

		n->parent = this;
		count++;

		return n;
	}

	// assumes list is sorted in reverse order
	list_node<type> *rsort_insert(type t)
	{
		list_node<type> *pos = NULL;
		for (pos = last; pos != NULL && *(type*)pos < t; pos = pos->prev);

		list_node<type> *n = new list_node<type>(t);
		n->prev = pos;

		if (pos != NULL)
			n->next = pos->next;

		if (n->prev != NULL)
			n->prev->next = n;
		else
			first = n;

		if (n->next != NULL)
			n->next->prev = n;
		else
			last = n;

		n->parent = this;
		count++;

		return n;
	}

	list_node<type> *find(type t, list_node<type> *f = NULL, list_node<type> *l = NULL)
	{
		if (f == NULL)
			f = first;
		if (l == NULL)
			l = last;

		for (list_node<type> *n = first; n != l->next; n = n->next)
			if (*(type*)n == t)
				return n;

		return NULL;
	}

	void swap(list_node<type> *n1, list_node<type> *n2)
	{
		if (n1 == NULL || n2 == NULL || n1->parent != this || n2->parent != this || n1 == n2)
			return;

		if (n1->next == n2 && n2->prev == n1)
		{
			n1->next = n2->next;
			n2->next = n1;
			n2->prev = n1->prev;
			n1->prev = n2;
		}
		else if (n2->next == n1 && n1->prev == n2)
		{
			n2->next = n1->next;
			n1->next = n2;
			n1->prev = n2->prev;
			n2->prev = n1;
		}
		else
		{
			list_node<type> *next = n1->next;
			list_node<type> *prev = n1->prev;
			n1->next = n2->next;
			n1->prev = n2->prev;
			n2->next = next;
			n2->prev = prev;
		}

		if (n1->next != NULL)
			n1->next->prev = n1;
		else
			last = n1;

		if (n1->prev != NULL)
			n1->prev->next = n1;
		else
			first = n1;

		if (n2->next != NULL)
			n2->next->prev = n2;
		else
			last = n2;

		if (n2->prev != NULL)
			n2->prev->next = n2;
		else
			first = n2;
	}

	list<type> &sort()
	{
		for (list_node<type> *n1 = first; n1 != NULL; n1 = n1->next)
			for (list_node<type> *n2 = n1->next; n2 != NULL; n2 = n2->next)
				if (*n1 > *n2)
				{
					swap(n1, n2);
					list_node<type> *temp = n1;
					n1 = n2;
					n2 = temp;
				}
		return *this;
	}

	list<type> &rsort()
	{
		for (list_node<type> *n1 = first; n1 != NULL; n1 = n1->next)
			for (list_node<type> *n2 = n1->next; n2 != NULL; n2 = n2->next)
				if (*(type*)n1 < *(type*)n2)
				{
					swap(n1, n2);
					list_node<type> *temp = n1;
					n1 = n2;
					n2 = temp;
				}
		return *this;
	}

	// assumes the list is sorted
	list<type> &unique()
	{
		list_node<type> *curr = first;
		list_node<type> *prev = NULL;
		while (curr != NULL)
		{
			if (prev != NULL && *(type*)prev == *(type*)curr)
				prev = rerase(prev);
			else
			{
				prev = curr;
				curr = curr->next;
			}
		}

		return *this;
	}

	list<type> &merge(list<type> l)
	{
		for (list_node<type> *n = l.begin(); n != l.end(); n = n->next)
			push_back(*(type*)n);

		return *this;
	}

	list<type> &operator=(list<type> l)
	{
		clear();
		for (list_node<type> *n = l.begin(); n != l.end(); n = n->next)
			push_back(*(type*)n);

		return *this;
	}

	list_node<type> &operator[](int i)
	{
		list_node<type> *result = begin();
		for (int j = 0; j < i; j++)
			result = result->next;
		return *result;
	}

	list_node<type> &at(int i)
	{
		list_node<type> *result = begin();
		for (int j = 0; j < i; j++)
			result = result->next;
		return *result;
	}
};


/**
 * ilist: Inherit List
 * To use this structure, the structure you want as your list element must inherit ilist_node.
 * Then, you can use this list as you normally would. This can only be used for classes,
 * structures, or unions, and not basic types. For example:
 *
 * struct mystruct : ilist_node<myclass>
 * {
 * 		...
 * };
 *
 * int main()
 * {
 * 		ilist<mystruct> mylist;
 * }
 */
template <class type>
struct ilist;

template <class type>
struct ilist_node
{
	ilist_node()
	{
		parent = NULL;
		prev = NULL;
		next = NULL;
	}

	ilist_node(const ilist_node<type> &n)
	{
		parent = n.parent;
		prev = n.prev;
		next = n.next;
	}

	~ilist_node()
	{

	}

	ilist<type> *parent;
	type *prev;
	type *next;

	ilist_node<type> &operator=(ilist_node<type> n)
	{
		parent = n.parent;
		prev = n.prev;
		next = n.next;
		return *this;
	}
};

template <class type>
struct ilist
{
private:
	type *first;
	type *last;
	int count;

public:
	ilist()
	{
		first = NULL;
		last = NULL;
		count = 0;
	}

	ilist(int num, type t)
	{
		first = NULL;
		last = NULL;
		count = 0;
		for (int i = 0; i < num; i++)
			push_back(t);
	}

	ilist(const ilist<type> &l)
	{
		first = NULL;
		last = NULL;
		count = 0;
		for (type *n = l.first; n != NULL; n = n->next)
			push_back(*n);
	}

	~ilist()
	{
		clear();
	}

	type *begin() const
	{
		return first;
	}

	type *end() const
	{
		return NULL;
	}

	type *rbegin() const
	{
		return last;
	}

	type *rend() const
	{
		return NULL;
	}

	int size() const
	{
		return count;
	}

	bool empty() const
	{
		return (count == 0);
	}

	type &front() const
	{
		return *first;
	}

	type &back() const
	{
		return *last;
	}

	type *push_back(type t)
	{
		type *n = new type(t);
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

		n->parent = this;
		count++;

		return n;
	}

	type *push_back(type *t)
	{
		if (t == NULL || t->parent != NULL)
			return NULL;

		if (first == NULL || last == NULL)
		{
			first = t;
			last = t;
		}
		else
		{
			last->next = t;
			t->prev = last;
			last = t;
		}

		t->parent = this;
		count++;

		return t;
	}

	type *push_front(type t)
	{
		type *n = new type(t);
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

		n->parent = this;
		count++;

		return n;
	}

	type *push_front(type *t)
	{
		if (t == NULL || t->parent != NULL)
			return NULL;

		if (first == NULL || last == NULL)
		{
			first = t;
			last = t;
		}
		else
		{
			first->prev = t;
			t->next = first;
			first = first->prev;
		}

		t->parent = this;
		count++;

		return t;
	}

	type *pop_front()
	{
		type *result = first;

		if (first == NULL)
			return NULL;
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

		result->parent = NULL;
		count--;
		return result;
	}

	type *pop_back()
	{
		type *result = last;

		if (last == NULL)
			return NULL;
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

		result->parent = NULL;
		count--;
		return result;
	}

	void drop_front()
	{
		type *result = first;

		if (first == NULL)
			return;
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
		type *result = last;

		if (last == NULL)
			return;
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

	type *remove(type *n)
	{
		if (n == NULL || n->parent != this)
			return NULL;

		if (n->next)
			n->next->prev = n->prev;
		else
			last = n->prev;

		if (n->prev)
			n->prev->next = n->next;
		else
			first = n->next;

		n->parent = NULL;
		count--;

		return n;
	}

	void erase(type t)
	{
		for (type *n = first; n != NULL;)
		{
			if (*n == t)
				n = erase(n);
			else
				n = n->next;
		}
	}

	type *erase(type *n)
	{
		type *result = n->next;

		if (n == NULL || n->parent != this)
			return NULL;

		if (n->next)
			n->next->prev = n->prev;
		else
			last = n->prev;

		if (n->prev)
			n->prev->next = n->next;
		else
			first = n->next;

		delete n;
		count--;

		return result;
	}

	type *rerase(type *n)
	{
		type *result = n->prev;

		if (n == NULL || n->parent != this)
			return NULL;

		if (n->next)
			n->next->prev = n->prev;
		else
			last = n->prev;

		if (n->prev)
			n->prev->next = n->next;
		else
			first = n->next;

		delete n;
		count--;

		return result;
	}

	type *erase(type *f, type *l)
	{
		if (f == NULL || l == NULL || f->parent != this || f->parent != this)
			return NULL;

		type result = l->next;

		if (f->prev)
			f->prev->next = l->next;
		else
			first = l->next;

		if (l->next)
			l->next->prev = f->prev;
		else
			last = f->prev;

		f->prev = NULL;
		l->next = NULL;

		type *curr = f;
		type *prev;
		while (curr != NULL)
		{
			prev = curr;
			curr = curr->next;
			delete prev;
			count--;
		}
		return result;
	}

	type *rerase(type *f, type *l)
	{
		if (f == NULL || l == NULL || f->parent != this || f->parent != this)
			return NULL;

		type result = l->prev;

		if (f->prev)
			f->prev->next = l->next;
		else
			first = l->next;

		if (l->next)
			l->next->prev = f->prev;
		else
			last = f->prev;

		f->prev = NULL;
		l->next = NULL;

		type *curr = f;
		type *prev;
		while (curr != NULL)
		{
			prev = curr;
			curr = curr->next;
			delete prev;
			count--;
		}
		return result;
	}

	void clear()
	{
		type *curr = first;
		type *prev;
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

	type *insert(type *pos, type t)
	{
		if (pos == NULL)
			return push_front(t);
		else if (pos->parent != this)
			return NULL;
		else
		{
			type *n = new type(t);
			n->prev = pos;
			n->next = pos->next;
			n->prev->next = n;

			if (n->next != NULL)
				n->next->prev = n;
			else
				last = n;

			n->parent = this;
			count++;

			return n;
		}
	}

	type *rinsert(type *pos, type t)
	{
		if (pos == NULL)
			return push_back(t);
		else if (pos->parent != this)
			return NULL;
		else
		{
			type *n = new type(t);
			n->next = pos;
			n->prev = pos->prev;
			n->next->prev = n;

			if (n->prev != NULL)
				n->prev->next = n;
			else
				first = n;

			n->parent = this;
			count++;
			return n;
		}
	}

	// assumes ilist is sorted
	type *sort_insert(type t)
	{
		type *pos = NULL;
		for (type *curr = first; curr != NULL; curr = curr->next)
		{
			if (*curr < t)
				pos = curr;
			else
				break;
		}

		if (pos == NULL)
			return push_front(t);
		else
		{
			type *n = new type(t);
			n->prev = pos;
			n->next = pos->next;
			n->prev->next = n;

			if (n->next != NULL)
				n->next->prev = n;
			else
				last = n;

			n->parent = this;
			count++;

			return n;
		}
	}

	// assumes ilist is sorted in reverse order
	type *rsort_insert(type t)
	{
		type *pos = NULL;
		for (type *curr = last; curr != NULL; curr = curr->prev)
		{
			if (*curr < t)
				pos = curr;
			else
				break;
		}

		if (pos == NULL)
			return push_back(t);
		else
		{
			type *n = new type(t);
			n->next = pos;
			n->prev = pos->prev;
			n->next->prev = n;

			if (n->prev != NULL)
				n->prev->next = n;
			else
				first = n;

			n->parent = this;
			count++;
			return n;
		}
	}

	type *find(type t, type *f = NULL, type *l = NULL)
	{
		if (f == NULL)
			f = first;
		if (l == NULL)
			l = last;

		for (type *n = first; n != l->next; n = n->next)
			if (*n == t)
				return n;

		return NULL;
	}

	void swap(type *n1, type *n2)
	{
		if (n1 == NULL || n2 == NULL || n1->parent != this || n2->parent != this || n1 == n2)
			return;

		if (n1->next == n2 && n2->prev == n1)
		{
			n1->next = n2->next;
			n2->next = n1;
			n2->prev = n1->prev;
			n1->prev = n2;
		}
		else if (n2->next == n1 && n1->prev == n2)
		{
			n2->next = n1->next;
			n1->next = n2;
			n1->prev = n2->prev;
			n2->prev = n1;
		}
		else
		{
			type *next = n1->next;
			type *prev = n1->prev;
			n1->next = n2->next;
			n1->prev = n2->prev;
			n2->next = next;
			n2->prev = prev;
		}

		if (n1->next != NULL)
			n1->next->prev = n1;
		else
			last = n1;

		if (n1->prev != NULL)
			n1->prev->next = n1;
		else
			first = n1;

		if (n2->next != NULL)
			n2->next->prev = n2;
		else
			last = n2;

		if (n2->prev != NULL)
			n2->prev->next = n2;
		else
			first = n2;
	}

	ilist<type> &sort()
	{
		for (type *n1 = first; n1 != NULL; n1 = n1->next)
			for (type *n2 = n1->next; n2 != NULL; n2 = n2->next)
				if (*n1 > *n2)
				{
					swap(n1, n2);
					type *temp = n1;
					n1 = n2;
					n2 = temp;
				}
		return *this;
	}

	ilist<type> &rsort()
	{
		for (type *n1 = first; n1 != NULL; n1 = n1->next)
			for (type *n2 = n1->next; n2 != NULL; n2 = n2->next)
				if (*n1 < *n2)
				{
					swap(n1, n2);
					type *temp = n1;
					n1 = n2;
					n2 = temp;
				}
		return *this;
	}

	// assumes the ilist is sorted
	ilist<type> &unique()
	{
		type *curr = first;
		type *prev = NULL;
		while (curr != NULL)
		{
			if (prev != NULL && *prev == *curr)
				prev = rerase(prev);
			else
			{
				prev = curr;
				curr = curr->next;
			}
		}

		return *this;
	}

	ilist<type> &merge(ilist<type> l)
	{
		for (type *n = l.begin(); n != l.end(); n = n->next)
			push_back(*n);

		return *this;
	}

	ilist<type> &operator=(ilist<type> l)
	{
		clear();
		for (type *n = l.begin(); n != l.end(); n = n->next)
			push_back(*n);

		return *this;
	}

	type &operator[](int i)
	{
		type *result = begin();
		for (int j = 0; j < i; j++)
			result = result->next;
		return *result;
	}

	type &at(int i)
	{
		type *result = begin();
		for (int j = 0; j < i; j++)
			result = result->next;
		return *result;
	}
};

#endif
