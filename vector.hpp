#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>

namespace sjtu
{
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
template<typename T>
class vector
{

private:
	T *arr;
	size_t arrSize, dataSize;
	void expand()
	{
		arrSize *= 2;
		T *ex_arr = new T[arrSize];
		for (int i = 0; i < dataSize; ++i)
			ex_arr[i] = arr[i];
		delete []arr;
		arr = ex_arr;
	}
public:
	/**
	 * TODO
	 * a type for actions of the elements of a vector, and you should write
	 *   a class named const_iterator with same interfaces.
	 */
	/**
	 * you can see RandomAccessIterator at CppReference for help.
	 */
	class const_iterator;
	class iterator
	{
		friend vector;
	
	private:
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */
		int index;
		T *arr;
	public:
		iterator(T *_arr = nullptr, int _index = 0)
		{
			arr = _arr;
			index = _index;
		}
		/**
		 * return a new iterator which pointer n-next elements
		 *   even if there are not enough elements, just return the answer.
		 * as well as operator-
		 */
		iterator operator+(const int &n) const
		{
			//TODO
			return iterator(arr, index + n);
		}
		iterator operator-(const int &n) const
		{
			//TODO
			return iterator(arr, index - n);
		}
		// return th distance between two iterator,
		// if these two iterators points to different vectors, throw invaild_iterator.
		int operator-(const iterator &rhs) const
		{
			//TODO
			return index - rhs.index;
		}
		iterator operator+=(const int &n)
		{
			//TODO
			index += n;
			return *this;
		}
		iterator operator-=(const int &n)
		{
			//TODO
			index -= n;
			return *this;
		}
		/**
		 * TODO iter++
		 */
		iterator operator++(int)
		{
			++index;
			return iterator(arr, index);
		}
		/**
		 * TODO ++iter
		 */
		iterator& operator++()
		{
			++index;
			return *this;
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int)
		{
			--index;
			return iterator(arr, index);
		}
		/**
		 * TODO --iter
		 */
		iterator& operator--()
		{
			--index;
			return *this;
		}
		/**
		 * TODO *it
		 */
		T& operator*() const
		{
			return arr[index];
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		bool operator==(const iterator &rhs) const
		{
			return index == rhs.index;
		}
		bool operator==(const const_iterator &rhs) const
		{
			return index == rhs.index;
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const
		{
			return index != rhs.index;
		}
		bool operator!=(const const_iterator &rhs) const
		{
			return index != rhs.index;
		}
	};
	/**
	 * TODO
	 * has same function as iterator, just for a const object.
	 */
	class const_iterator
	{
		friend vector;

	private:
		int index;
		T *arr;

	public:
		const_iterator(T *_arr = nullptr, int _index = 0)
		{
			arr = _arr;
			index = _index;
		}
		/**
		 * return a new iterator which pointer n-next elements
		 *   even if there are not enough elements, just return the answer.
		 * as well as operator-
		 */
		const_iterator operator+(const int &n) const
		{
			//TODO
			return const_iterator(arr, index + n);
		}
		const_iterator operator-(const int &n) const
		{
			//TODO
			return const_iterator(arr, index - n);
		}
		// return th distance between two iterator,
		// if these two iterators points to different vectors, throw invaild_iterator.
		int operator-(const const_iterator &rhs) const
		{
			//TODO
			return index - rhs.index;
		}
		const_iterator operator+=(const int &n)
		{
			//TODO
			index += n;
			return *this;
		}
		const_iterator operator-=(const int &n)
		{
			//TODO
			index -= n;
			return *this;
		}
		/**
		 * TODO iter++
		 */
		const_iterator operator++(int)
		{
			++index;
			return const_iterator(arr, index);
		}
		/**
		 * TODO ++iter
		 */
		const_iterator& operator++()
		{
			++index;
			return *this;
		}
		/**
		 * TODO iter--
		 */
		const_iterator operator--(int)
		{
			--index;
			return const_iterator(arr, index);
		}
		/**
		 * TODO --iter
		 */
		const_iterator& operator--()
		{
			--index;
			return *this;
		}
		/**
		 * TODO *it
		 */
		T& operator*() const
		{
			return arr[index];
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		bool operator==(const iterator &rhs) const
		{
			return index == rhs.index;
		}
		bool operator==(const const_iterator &rhs) const
		{
			return index == rhs.index;
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const
		{
			return index != rhs.index;
		}
		bool operator!=(const const_iterator &rhs) const
		{
			return index != rhs.index;
		}

	};
	/**
	 * TODO Constructs
	 * Atleast three: default constructor, copy constructor and a constructor for std::vector
	 */
	vector()
	{
		arr = new T[1];
		arrSize = 1;
		dataSize = 0;
	}
	vector(const vector &other)
	{
		arrSize = other.arrSize;
		arr = new T[arrSize];
		dataSize = other.dataSize;
		for (int i = 0; i < other.dataSize; ++i)
			arr[i] = other.arr[i];
	}
	/**
	 * TODO Destructor
	 */
	~vector()
	{
		delete []arr;
	}
	/**
	 * TODO Assignment operator
	 */
	vector &operator=(const vector &other)
	{
		if (arrSize < other.arrSize)
		{
			arrSize = other.arrSize;
			delete []arr;
			arr = new T[arrSize];
		}
		dataSize = other.dataSize;
		for (int i = 0; i < other.dataSize; ++i)
			arr[i] = other.arr[i];
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound() if pos is not in [0, size)
	 */
	T & at(const size_t &pos)
	{
		if (pos < 0 || pos >= dataSize)
			throw index_out_of_bound();
		return arr[pos];
	}
	const T & at(const size_t &pos) const
	{
		if (pos < 0 || pos >= dataSize)
			throw index_out_of_bound();
		return arr[pos];
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound() if pos is not in [0, size)
	 * !!! Pay attentions
	 *   In STL this operator does not check the boundary but I want you to do.
	 */
	T & operator[](const size_t &pos)
	{
		if (pos < 0 || pos >= dataSize)
			throw index_out_of_bound();
		return arr[pos];
	}
	const T & operator[](const size_t &pos) const
	{
		if (pos < 0 || pos >= dataSize)
			throw index_out_of_bound();
		return arr[pos];
	}
	/**
	 * access the first element.
	 * throw container_is_empty() if size == 0
	 */
	const T & front() const
	{
		if (empty())
			throw container_is_empty();
		return arr[0];
	}
	/**
	 * access the last element.
	 * throw container_is_empty() if size == 0
	 */
	const T & back() const
	{
		if (empty())
			throw container_is_empty();
		return arr[dataSize - 1];
	}
	/**
	 * returns an iterator to the beginning.
	 */
	iterator begin()
	{
		return iterator(arr, 0);
	}
	const_iterator cbegin() const
	{
		return const_iterator(arr, 0);
	}
	/**
	 * returns an iterator to the end.
	 */
	iterator end()
	{
		return iterator(arr, dataSize);
	}
	const_iterator cend() const
	{
		return const_iterator(arr, dataSize);
	}
	/**
	 * checks whether the container is empty
	 */
	bool empty() const
	{
		return dataSize == 0;
	}
	/**
	 * returns the number of elements
	 */
	size_t size() const
	{
		return dataSize;
	}
	/**
	 * returns the number of elements that can be held in currently allocated storage.
	 */
	size_t capacity() const
	{
		return arrSize;
	}
	/**
	 * clears the contents
	 */
	void clear()
	{
		dataSize = 0;
	}
	/**
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value.
	 */
	iterator insert(iterator pos, const T &value)
	{
		if (dataSize + 1 > arrSize)
			expand();
		++dataSize;
		for (int i = dataSize - 1; i > pos.index; --i)
			arr[i] = arr[i - 1];
		arr[pos.index] = value;
		return iterator(arr, pos.index);
	}
	/**
	 * inserts value at index ind.
	 * after inserting, this->at(ind) == value is true
	 * returns an iterator pointing to the inserted value.
	 * throw index_out_of_bound() if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
	 */
	iterator insert(const size_t &ind, const T &value)
	{
		if (ind < 0 || ind > dataSize)
			throw index_out_of_bound();
		if (dataSize + 1 > arrSize)
			expand();
		++dataSize;
		for (int i = dataSize - 1; i > ind; --i)
			arr[i] = arr[i - 1];
		arr[ind] = value;
		return iterator(arr, ind);
	}
	/**
	 * removes the element at pos.
	 * return an iterator pointing to the following element.
	 * If the iterator pos refers the last element, the end() iterator is returned.
	 */
	iterator erase(iterator pos)
	{
		--dataSize;
		for (int i = pos.index; i < dataSize; i++)
			arr[i] = arr[i + 1];
		return iterator(arr, pos.index);
	}
	/**
	 * removes the element with index ind.
	 * return an iterator pointing to the following element.
	 * throw index_out_of_bound() if ind >= size
	 */
	iterator erase(const size_t &ind)
	{
		if (ind >= dataSize)
			throw index_out_of_bound();
		--dataSize;
		for (int i = ind; i < dataSize; i++)
			arr[i] = arr[i + 1];
		return iterator(arr, ind);
	}
	/**
	 * adds an element to the end.
	 */
	void push_back(const T &value)
	{
		if (dataSize + 1 == arrSize)
			expand();
		arr[dataSize] = value;
		++dataSize;
	}
	/**
	 * remove the last element from the end.
	 * throw container_is_empty() if size() == 0
	 */
	void pop_back()
	{
		if (empty())
			throw container_is_empty();
		--dataSize;
	}
};


}

#endif
