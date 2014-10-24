/*
 * array.h
 *
 *  Created on: Feb 4, 2014
 *      Author: nbingham
 */

#include <stdlib.h>
#include <stdio.h>

#ifndef array_h
#define array_h

#ifndef NULL
#define NULL 0
#endif

template <class type>
struct array
{
private:
	type *_data;
	int _size;
	int _capacity;

	int log2i(int i)
	{
		int r = 0;
		while (i >>= 1)
			r++;
		return r;
	}
public:
	array()
	{
		_data = NULL;
		_size = 0;
		_capacity = 0;
	}

	array(const array<type> &a)
	{
		_capacity = a._size + log2i(a._size);
		_data = new type[_capacity];
		for (_size = 0; _size < a._size; _size++)
			_data[_size] = a._data[_size];
	}

	array(int n)
	{
		if (n < 0)
		{
			perror("error: n < 0 in array<type>::array(n)\n");
			n = 0;
		}

		_capacity = n;
		_data = new type[_capacity];
		_size = 0;
	}

	array(int n, type t)
	{
		if (n < 0)
		{
			perror("error: n < 0 in array<type>::array(n, t)\n");
			n = 0;
		}

		_capacity = n + log2i(n);
		_data = new type[_capacity];
		for (_size = 0; _size < n; _size++)
			_data[_size] = t;
	}

	~array()
	{
		if (_data != NULL)
			delete [] _data;
		_data = NULL;
		_capacity = 0;
		_size = 0;
	}

	int size() const
	{
		return _size;
	}

	bool empty() const
	{
		return _size == 0;
	}

	int capacity() const
	{
		return _capacity;
	}

	type &back() const
	{
		return _data[_size-1];
	}

	type &front() const
	{
		return _data[0];
	}

	/**
	 * \fn void array<type>::resize(int n)
	 * \brief Resize the array to contain n elements.
	 */
	void resize(int n)
	{
		if (n < 0)
		{
			perror("error: n < 0 in array<type>::resize(n)\n");
			n = 0;
		}

		if (n <= _capacity)
			_size = n;
		else
		{
			_capacity = n + log2i(n);
			type *new_data = new type[_capacity];
			for (int i = 0; i < _size; i++)
				new_data[i] = _data[i];

			_size = n;

			if (_data != NULL)
				delete [] _data;
			_data = new_data;
		}
	}

	/**
	 * \fn void array<type>::extend(int n, type v)
	 * \brief Extend the size of the array by n elements initialized with the value v.
	 */
	void extend(int n, type v = type())
	{
		if (_size + n < 0)
		{
			perror("error: (size() + n) < 0 in array<type>::extend(n, v)\n");
			n = -_size;
		}

		if (_size + n <= _capacity)
		{
			for (int i = 0; i < n; i++)
				_data[_size + i] = v;

			_size += n;
		}
		else
		{
			_capacity = _size + n + log2i(_size + n);
			type *new_data = new type[_capacity];
			for (int i = 0; i < _size; i++)
				new_data[i] = _data[i];

			for (int i = 0; i < n; i++)
				new_data[_size + i] = v;
			_size += n;

			if (_data != NULL)
				delete [] _data;
			_data = new_data;
		}
	}

	/**
	 * \fn array<type>::assign(int i, type v)
	 * \brief Assign the value v to the element at index i, resizing the array if necessary.
	 */
	void assign(int i, type v = type())
	{
		if (i < 0)
			perror("error: i < 0 in array<type>::assign(i, v)\n");
		else if (i < _size)
			_data[i] = v;
		else if (i < _capacity)
		{
			_data[i] = v;
			_size = i+1;
		}
		else
		{
			_capacity = i+1 + log2i(i+1);
			type *new_data = new type[_capacity];
			for (int j = 0; j < _size; j++)
				new_data[j] = _data[j];

			new_data[i] = v;
			_size = i+1;

			if (_data != NULL)
				delete [] _data;
			_data = new_data;
		}
	}

	/**
	 * \fn void array<type>::reinit(int n, type v)
	 * \brief Resizes the array to contain n elements and initializes all n elements with the value v.
	 */
	void reinit(int n, type v = type())
	{
		if (n < 0)
		{
			perror("error: n < 0 in array<type>::reinit(n, v)\n");
			n = 0;
		}

		if (n > _capacity)
		{
			_capacity = n + log2i(n);
			type *new_data = new type[_capacity];
			for (int i = 0; i < _size; i++)
				new_data[i] = _data[i];

			if (_data != NULL)
				delete [] _data;
			_data = new_data;
		}

		for (_size = 0; _size < n; _size++)
			_data[_size] = v;
	}

	/**
	 * \fn void array<type>::reserve(int n)
	 * \brief Increase the capacity of the array to at least n possible elements.
	 */
	void reserve(int n)
	{
		if (n < 0)
			perror("warning: n < 0 in array<type>::reserve(n)\n");
		else if (n > _capacity)
		{
			_capacity = n;
			type *new_data = new type[_capacity];
			for (int i = 0; i < _size; i++)
				new_data[i] = _data[i];

			if (_data != NULL)
				delete [] _data;
			_data = new_data;
		}
	}

	/**
	 * \fn void array<type>::push_back(type v)
	 * \brief Extend the array by one element and assign that element with value v.
	 */
	void push_back(type v)
	{
		resize(_size+1);
		_data[_size-1] = v;
	}

	/**
	 * \fn void array<type>::drop_back()
	 * \brief Erase the last element in the array.
	 */
	void drop_back()
	{
		if (_size > 0)
			_size--;
	}

	/**
	 * \fn void array<type>::erase(int i)
	 * \brief Erase the element at index i.
	 */
	void erase(int i)
	{
		if (i < 0)
			perror("error: i < 0 in array<type>::erase(i)\n");
		else if (i >= _size)
			perror("error: i >= size() in array<type>::erase(i)\n");
		else
		{
			_size--;
			for (int j = i; j < _size; j++)
				_data[j] = _data[j+1];
		}
	}

	/**
	 * \fn void array<type>::erase(int f, int l)
	 * \brief Erase all elements from the array with indices from f to l.
	 */
	void erase(int f, int l)
	{
		if (f < 0)
			perror("error: f < 0 in array<type>::erase(f, l)\n");
		else if (l < f)
			perror("error: l < f in array<type>::erase(f, l)\n");
		else if (l > size())
			perror("error: l > size() in array<type>::erase(f, l)\n");
		else
		{
			_size -= (l - f);
			for (int i = f; i < _size; i++)
				_data[i] = _data[i + (l-f)];
		}
	}

	/**
	 * \fn void array<type>::clear()
	 * \brief Erase all elements from the array.
	 */
	void clear()
	{
		if (_data != NULL)
			delete [] _data;
		_data = NULL;
		_capacity = 0;
		_size = 0;
	}

	/**
	 * \fn void array<type>::insert(int i, type v)
	 * \brief Insert an element with value v in the the array at index i.
	 */
	void insert(int i, type v)
	{
		if (i < 0)
			perror("error: i < 0 in array<type>::insert(i, v)\n");
		else
		{
			resize((i > _size ? i : _size) + 1);

			for (int j = _size; j > i; j--)
				_data[j] = _data[j-1];
			_data[i] = v;
		}
	}

	/**
	 * \fn void array<type>::sort_insert(type v)
	 * \brief Assuming the array is already sorted, insert an element with value v in the the array at the sorted index.
	 */
	void sort_insert(type v)
	{
		int i = 0;
		for (i = 0; i < _size && _data[i] < v; i++);

		if (_size+1 < _capacity)
			resize(_size + 1);

		for (int j = _size; j > i; j--)
			_data[j] = _data[j-1];
		_data[i] = v;
	}

	/**
	 * \fn void array<type>::sort_insert(type v)
	 * \brief Assuming the array is already reverse sorted, insert an element with value v in the the array at the reverse sorted index.
	 */
	void rsort_insert(type v)
	{
		int i = 0;
		for (i = 0; i < _size && _data[i] > v; i++);

		if (size()+1 < _capacity)
			resize(size() + 1);

		if (_size+1 < _capacity)
			resize(_size + 1);

		for (int j = _size; j > i; j--)
			_data[j] = _data[j-1];
		_data[i] = v;
	}

	/**
	 * \fn int array<type>::find(type v, int i)
	 * \brief Find the first element in the array with value v and index greater than or equal to i. If no such element exists, return -1.
	 */
	int find(type v, int i = 0)  const
	{
		if (i < 0)
			perror("error: i < 0 in array<type>::find(v, i)\n");
		else
		{
			for (int j = i; j < _size; j++)
				if (_data[j] == v)
					return j;
		}

		return -1;
	}

	/**
	 * \fn int array<type>::rfind(type v, int i)
	 * \brief Find the last element in the array with value v and index less than or equal to i. If no such element exists, return -1.
	 */
	int rfind(type v, int i = -1)  const
	{
		if (i == -1)
			i = _size;

		if (i < 0)
			perror("error: i < 0 in array<type>::rfind(v, i)\n");
		else
		{
			for (int j = i; j >= 0; j--)
				if (_data[j] == v)
					return j;
		}

		return -1;
	}

	/**
	 * \fn bool array<type>::contains(type v, int i)
	 * \brief Return true if the array contains an element with value v at an index greater than or equal to i, and false otherwise.
	 */
	bool contains(type t, int i = 0)  const
	{
		return (find(t, i) != -1);
	}

	/**
	 * \fn bool array<type>::contains(type v, int i)
	 * \brief Return true if the array contains an element with value v at an index less than or equal to i, and false otherwise.
	 */
	bool rcontains(type t, int i = -1)  const
	{
		return (rfind(t, i) != -1);
	}

	/**
	 * \fn void array<type>::swap(int i, int j)
	 * \brief Swap two elements in the array. No error checking.
	 */
	void swap(int i, int j)
	{
		type temp = _data[i];
		_data[i] = _data[j];
		_data[j] = temp;
	}

	array<type> &sort()
	{
		for (int i = 0; i < _size; i++)
			for (int j = i+1; j < _size; j++)
				if (_data[i] > _data[j])
					swap(i, j);
		return *this;
	}

	array<type> &rsort()
	{
		for (int i = 0; i < _size; i++)
			for (int j = i+1; j < _size; j++)
				if (_data[i] < _data[j])
					swap(i, j);
		return *this;
	}

	array<type> &unique()
	{
		for (int i = 1; i < _size;)
		{
			if (_data[i-1] == _data[i])
				erase(i-1);
			else
				i++;
		}

		return *this;
	}

	array<type> &merge(array<type> a)
	{
		int s = _size;
		resize(_size + a._size);
		for (int i = 0; i < a._size; i++)
			_data[s + i] = a[i];
		return *this;
	}

	array<type> &copy(array<type> a)
	{
		if (a._size > _capacity)
		{
			if (_data != NULL)
				delete [] _data;

			_capacity = a._size + log2i(a._size);
			_data = new type[_capacity];
		}

		for (_size = 0; _size < a._size; _size++)
			_data[_size] = a._data[_size];

		return *this;
	}

	int compare(array<type> a) const
	{
		int i = 0;
		for (i = 0; i < _size && i < a._size; i++)
		{
			if (_data[i] < a[i])
				return -1;
			else if (_data[i] > a[i])
				return 1;
		}

		if (i < a._size)
			return -1;
		else if (i < _size)
			return 1;
		else
			return 0;
	}

	array<type> &operator=(array<type> a)
	{
		if (a._size > _capacity)
		{
			if (_data != NULL)
				delete [] _data;

			_capacity = a._size + log2i(a._size);
			_data = new type[_capacity];
		}

		for (_size = 0; _size < a._size; _size++)
			_data[_size] = a._data[_size];

		return *this;
	}

	type &operator[](int i) const
	{
		return _data[i];
	}

	type &at(int i) const
	{
		return _data[i];
	}

	type *data() const
	{
		return _data;
	}
};

template <class type>
bool operator==(array<type> s1, array<type> s2)
{
	return (s1.compare(s2) == 0);
}

template <class type>
bool operator!=(array<type> s1, array<type> s2)
{
	return (s1.compare(s2) != 0);
}

template <class type>
bool operator<(array<type> s1, array<type> s2)
{
	return (s1.compare(s2) < 0);
}

template <class type>
bool operator>(array<type> s1, array<type> s2)
{
	return (s1.compare(s2) > 0);
}

template <class type>
bool operator<=(array<type> s1, array<type> s2)
{
	return (s1.compare(s2) <= 0);
}

template <class type>
bool operator>=(array<type> s1, array<type> s2)
{
	return (s1.compare(s2) >= 0);
}

#endif
