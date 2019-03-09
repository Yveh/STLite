#ifndef SJTU_LIST_HPP
#define SJTU_LIST_HPP

#include "exceptions.hpp"

#include <cstddef>

namespace sjtu { 

template<class T>
class list
{
private:
	class node
	{
	private:
		T data;
		node *prev, *succ;
	public:
		node(const T &_data = 0) : data(_data), prev(nullptr), succ(nullptr) {}
		~node() {}
	}
	node *head, *tail;
	size_t nodeSize;
public:
class iterator {
	private:
		node *index;
	public:
		iterator(const node *_index = nullptr) : index(_index);
		iterator& operator++()
		{
			index = index->succ;
			return *this;
		}
		iterator& operator--()
		{
			index = index->prev;
			return *this;
		}
		T& operator*() const
		{
			return this->key;
		}
		T* operator->() const noexcept
		{
			return &this;
		}
		bool operator==(const iterator &rhs) const
		{
			index == rhs.index;
		}
		bool operator!=(const iterator &rhs) const
		{
			index != rhs.index;
		}
	};
	list()
	{
		head = tail = nullptr;
		nodeSize = 0;
	}
	~list()
	{
		for (node *i = head, *j; i != nullptr; i = j)
		{
			j = i->succ;
			delete i;
		}
	}
	list(const list &other)
	{
		node **cur = head, *last = nullptr;
		for (node *i = other.head, *j; i != nullptr; i = j)
		{
			*cur = new node(i->data);
			(*cur)->prev = last;
			last = (*cur);
			cur = &(*cur->succ);
		}
		tail = cur;
		nodeSize = other.nodeSize;
	}
	list &operator=(const list &other)
	{
		node **cur = head, *last = nullptr;
		for (node *i = other.head, *j; i != nullptr; i = j)
		{
			*cur = new node(i->data);
			(*cur)->prev = last;
			last = (*cur);
			cur = &(*cur->succ);
		}
		tail = cur;
		nodeSize = other.nodeSize;
	}
	void push_front(const T &e)
	{
		if (empty())
			head = tail = new node(e);
		else
		{
			node *tmp = new node(e);
			tmp->succ = head;
			head->prev = tmp;
			head = tmp;
		}
		++nodeSize;
	}
	void push_back(const T &e)
	{
		if (empty())
			head = tail = new node(e);
		else
		{
			node *tmp = new node(e);
			tmp->prev = tail;
			tail->succ = tmp;
			tail = tmp;
		}
		++nodeSize;
	}
	void insert(size_t ind, const T &e)
	{
		if (ind < 1 || ind > nodeSize + 1)
			throw index_out_of_bound();
		if (ind == 1)
			push_front(e);
		else if (ind == nodeSize + 1)
			push_back(e);
		else if (inf <= nodeSize / 2)
		{
			node *tmp = head;
			for (int i = 2; i <= ind; ++i)
				tmp = tmp->succ;
			node *newbee = new node(e);
			newbee->prev = tmp->prev;
			newbee->succ = tmp;
			tmp->prev->succ = newbee;
			tmp->prev = newbee;
			++nodeSize;
		}
		else
		{
			node *tmp = tail;
			for (int i = ind + 1; i <= nodeSize; ++i)
				tmp = tmp->prev;
			node *newbee = new node(e);
			newbee->prev = tmp->prev;
			newbee->succ = tmp;
			tmp->prev->succ = newbee;
			tmp->prev = newbee;
			++nodeSize;
		}
	}
	void pop_front()
	{
		node *tmp = head;
		head->succ->prev = nullptr;
		head = head->succ;
		delete(tmp);
		--nodeSize;
	}
	void pop_back()
	{
		node *tmp = tail;
		tail->prev->succ = nullptr;
		tail = tail->prev;
		delete(tmp);
		--nodeSize;
	}
	void delete(size_t ind)
	{
		if (ind < 1 || ind > nodeSize)
			throw index_out_of_bound();
		if (ind == 1)
			pop_front();
		else if (ind == nodeSize)
			pop_back();
		else if (ind <= nodeSize / 2)
		{
			node *tmp = head;
			for (int i = 2; i <= ind; ++i)
				tmp = tmp->succ;
			tmp->prev->succ = tmp->succ;
			tmp->succ->prev = tmp->prev;
			delete tmp;
			--nodeSize;
		}
		else
		{
			node *tmp = tail;
			for (int i = ind + 1; i <= nodeSize; ++i)
				tmp->tmp->prev;
			tmp->prev->succ = tmp->succ;
			tmp->succ->prev = tmp->prev;
			delete tmp;
			--nodeSize;
		}
	}
	void empty()
	{
		return nodeSize == 0;
	}
	size_t size()
	{
		return nodeSize;
	}
	T & at(const size_t &ind)
	{
		if (ind < 1 || ind > nodeSize)
			throw index_out_of_bound();
		if (ind <= nodeSize / 2)
		{
			node *tmp = head;
			for (int i = 2; i <= ind; ++i)
				tmp = tmp->succ;
			return tmp->data;
		}
		else
		{
			node *tmp = tail;
			for (int i = ind + 1; i <= nodeSize; ++i)
				tmp->tmp->prev;
			return tmp->data;
		}
	}
	T & operator[](const size_t &ind)
	{
		if (ind < 1 || ind > nodeSize)
			throw index_out_of_bound();
		if (ind <= nodeSize / 2)
		{
			node *tmp = head;
			for (int i = 2; i <= ind; ++i)
				tmp = tmp->succ;
			return tmp->data;
		}
		else
		{
			node *tmp = tail;
			for (int i = ind + 1; i <= nodeSize; ++i)
				tmp->tmp->prev;
			return tmp->data;
		}
	}
	iterator begin()
	{
		return iterator(head);
	}
	iterator end()
	{
		return iterator(nullptr);
	}
};

}

#endif
