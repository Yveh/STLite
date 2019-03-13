#ifndef SJTU_DEQUE_HPP
#define SJTU_DEQUE_HPP

#include "exceptions.hpp"

#include <cstddef>

namespace sjtu { 

template<class T>
class deque
{
private:
	const size_t B = 1024;
	class node;
	class block{
	public:
		size_t size;
		block *prev, *succ;
		node *listHead, *listTail;
		block()
		{
			size = 0;
			prev = succ = nullptr;
			listHead = listTail = nullptr;
		}
		block(const size_t &other)
		{
			size = other;
			prev = succ = nullptr;
			listHead = listTail = nullptr;
		}
		~block()
		{
			for (node *i = listHead, *j; i != nullptr; i = j)
			{
				j = i->succ;
				delete i;
			}
		}
	};
	class node{
	public:
		T *data;
		block *par;
		node *prev, *succ;
		node()
		{
			data = nullptr;
			par = nullptr;
			prev = succ = nullptr;
		}
		node(const T &other)
		{
			data = new T(other);
			par = nullptr;
			prev = succ = nullptr;
		}
		~node()
		{
			if (data != nullptr)
				delete data;
		}
	};
	size_t dataSize;
	block *blockHead, *blockTail;
	
	void split(block *blk)
	{
		node *pos = blk->listHead;
		block *tmpBlock = new block(blk->size / 2);
		for (int i = 1; i < blk->size / 2; i++)
		{
			pos->par = tmpBlock;
			pos = pos->succ;
		}
		pos->par = tmpBlock;
		
		tmpBlock->listHead = blk->listHead;
		tmpBlock->listTail = pos;
		tmpBlock->prev = blk->prev;
		tmpBlock->succ = blk;
		
		blk->listHead = pos->succ;
		blk->size -= tmpBlock->size;
		if (blk->prev == nullptr)
			blockHead = tmpBlock;
		else
			blk->prev->succ = tmpBlock;
		blk->prev = tmpBlock;
		
		pos->succ->prev = nullptr;
		pos->succ = nullptr;
	}
	void merge(block *blk)
	{
		if (blk->prev != nullptr)
		{
			block *tmpBlock = blk->prev;
			tmpBlock->succ = blk->succ;
			if (blk->succ != nullptr)
				blk->succ->prev = tmpBlock;
			else
				blockTail = tmpBlock;
			tmpBlock->size += blk->size;
			tmpBlock->listTail->succ = blk->listHead;
			blk->listHead->prev = tmpBlock->listTail;
			tmpBlock->listTail = blk->listTail;
			for (node *i = blk->listHead; i != nullptr; i = i->succ)
				i->par = tmpBlock;
			
			blk->listHead = nullptr;
			blk->listTail = nullptr;
			delete blk;
		}
		else if (blk->succ != nullptr)
		{
			block *tmpBlock = blk->succ;
			tmpBlock->prev = blk->prev;
			if (blk->prev != nullptr)
				blk->prev->succ = tmpBlock;
			else
				blockHead = tmpBlock;
			tmpBlock->size += blk->size;
			tmpBlock->listHead->prev = blk->listTail;
			blk->listTail->succ = tmpBlock->listHead;
			tmpBlock->listHead = blk->listHead;
			for (node *i = blk->listTail; i != nullptr; i = i->prev)
				i->par = tmpBlock;
				
			blk->listHead = nullptr;
			blk->listTail = nullptr;
			delete blk;
		}
	}
	void maintain(block *blk)
	{
		if (blk->size >= 2 * B)
			split(blk);
		if (blk->size <= B / 2)
			merge(blk);
	}
public:
	class const_iterator;
	class iterator {
		friend deque;
	private:
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */
		deque *dq;
		node *nd;
	public:
		iterator(deque *_dq, node *_nd) : dq(_dq), nd(_nd) {}
		iterator() : dq(nullptr), nd(nullptr) {}
		iterator(const iterator &other) : dq(other.dq), nd(other.nd) {}
		iterator & operator=(const iterator &other)
		{
			dq = other.dq;
			nd = other.nd;
		}
		/**
		 * return a new iterator which pointer n-next elements
		 *   even if there are not enough elements, the behaviour is **undefined**.
		 * as well as operator-
		 */
		iterator operator+(const int &n) const
		{
			if (n < 0)
				return (*this - (-n));
			int res = n;
			node *tmpNode = nd;
			while (res > 0 && tmpNode->succ != nullptr)
			{
				--res;
				tmpNode = tmpNode->succ;
			}
			if (res == 0)
				return iterator(dq, tmpNode);
			
			--res;
			block *tmpBlock = tmpNode->par->succ;
			if (tmpBlock == nullptr)
				throw invalid_iterator();
			res -= tmpBlock->size;
			while (res >= 0 && tmpBlock->succ != nullptr)
			{
				tmpBlock = tmpBlock->succ;
				res -= tmpBlock->size;
			}
			res += tmpBlock->size;
			
			tmpNode = tmpBlock->listHead;
			while (res > 0 && tmpNode->succ != nullptr)
			{
				--res;
				tmpNode = tmpNode->succ;
			}
			if (res > 0)
				throw invalid_iterator();
			return iterator(dq, tmpNode);
		}
		iterator operator-(const int &n) const
		{
			if (n < 0)
				return (*this + (-n));
			int res = n;
			node *tmpNode = nd;
			while (res > 0 && tmpNode->prev != nullptr)
			{
				--res;
				tmpNode = tmpNode->prev;
			}
			if (res == 0)
				return iterator(dq, tmpNode);
			
			--res;
			block *tmpBlock = tmpNode->par->prev;
			if (tmpBlock == nullptr)
				throw invalid_iterator();
			res -= tmpBlock->size;
			while (res >= 0 && tmpBlock->prev != nullptr)
			{
				tmpBlock = tmpBlock->prev;
				res -= tmpBlock->size;
			}
			res += tmpBlock->size;
			
			tmpNode = tmpBlock->listTail;
			while (res > 0 && tmpNode->prev != nullptr)
			{
				--res;
				tmpNode = tmpNode->prev;
			}
			if (res > 0)
				throw invalid_iterator();
			return iterator(dq, tmpNode);
		}
		// return th distance between two iterator,
		// if these two iterators points to different vectors, throw invaild_iterator.
		int operator-(const iterator &rhs) const
		{
			if (dq != rhs.dq)
				throw invalid_iterator();
			int ret = 0, cur = 0;
			for (block *i = dq->blockHead; i != nullptr; cur += i->size, i = i->succ)
			{
				if (nd->par == i || rhs.nd->par == i)
				{
					for (node *j = i->listHead; j != nullptr; ++cur, j = j->succ)
					{
						if (nd == j)
							ret += cur;
						if (rhs.nd == j)
							ret -= cur;
					}
					cur -= i->size;
				}
			}
			return ret;
		}
		int operator-(const const_iterator &rhs) const
		{
			if (dq != rhs.dq)
				throw invalid_iterator();
			int ret = 0, cur = 0;
			for (block *i = dq->blockHead; i != nullptr; cur += i->size, i = i->succ)
			{
				if (nd->par == i || rhs.nd->par == i)
				{
					for (node *j = i->listHead; j != nullptr; ++cur, j = j->succ)
					{
						if (nd == j)
							ret += cur;
						if (rhs.nd == j)
							ret -= cur;
					}
					cur -= i->size;
				}
			}
			return ret;
		}
		iterator operator+=(const int &n)
		{
			*this = (*this + n);
			return *this;
		}
		iterator operator-=(const int &n)
		{
			*this = (*this - n);
			return *this;
		}
		/**
		 * TODO iter++
		 */
		iterator operator++(int)
		{
			iterator ret(dq, nd);
			*this += 1;
			return ret;
		}
		/**
		 * TODO ++iter
		 */
		iterator& operator++()
		{
			*this += 1;
			return *this;
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int)
		{
			iterator ret(dq, nd);
			*this -= 1;
			return ret;
		}
		/**
		 * TODO --iter
		 */
		iterator& operator--()
		{
			*this -= 1;
			return *this;
		}
		/**
		 * TODO *it
		 */
		T& operator*() const
		{
			if (*this == dq->end())
				throw invalid_iterator();
			return *(nd->data);
		}
		/**
		 * TODO it->field
		 */
		T* operator->() const noexcept
		{
			if (*this == dq->end())
				throw invalid_iterator();
			return nd->data;
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		bool operator==(const iterator &rhs) const
		{
			return (dq == rhs.dq) && (nd == rhs.nd);
		}
		bool operator==(const const_iterator &rhs) const
		{
			return (dq == rhs.dq) && (nd == rhs.nd);
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const
		{
			return (dq != rhs.dq) || (nd != rhs.nd);
		}
		bool operator!=(const const_iterator &rhs) const
		{
			return (dq != rhs.dq) || (nd != rhs.nd);
		}
	};
	class const_iterator {
		// it should has similar member method as iterator.
		//  and it should be able to construct from an iterator.
		friend deque;
	private:
		const deque *dq;
		node *nd;
		
	public:
		const_iterator(const deque *_dq, node *_nd) : dq(_dq), nd(_nd) {}
		const_iterator() : dq(nullptr), nd(nullptr) {}
		const_iterator(const const_iterator &other) : dq(other.dq), nd(other.nd) {}
		const_iterator(const iterator &other) : dq(other.dq), nd(other.nd) {}
		const_iterator & operator=(const iterator &other)
		{
			dq = other.dq;
			nd = other.nd;
		}
			/**
		 * return a new iterator which pointer n-next elements
		 *   even if there are not enough elements, the behaviour is **undefined**.
		 * as well as operator-
		 */
		const_iterator operator+(const int &n) const
		{
			if (n < 0)
				return (*this - (-n));
			int res = n;
			node *tmpNode = nd;
			while (res > 0 && tmpNode->succ != nullptr)
			{
				--res;
				tmpNode = tmpNode->succ;
			}
			if (res == 0)
				return const_iterator(dq, tmpNode);
			
			--res;
			block *tmpBlock = tmpNode->par->succ;
			if (tmpBlock == nullptr)
				throw invalid_iterator();
			res -= tmpBlock->size;
			while (res >= 0 && tmpBlock->succ != nullptr)
			{
				tmpBlock = tmpBlock->succ;
				res -= tmpBlock->size;
			}
			res += tmpBlock->size;
			
			tmpNode = tmpBlock->listHead;
			while (res > 0 && tmpNode->succ != nullptr)
			{
				--res;
				tmpNode = tmpNode->succ;
			}
			if (res > 0)
				throw invalid_iterator();
			return const_iterator(dq, tmpNode);
		}
		const_iterator operator-(const int &n) const
		{
			if (n < 0)
				return (*this + (-n));
			int res = n;
			node *tmpNode = nd;
			while (res > 0 && tmpNode->prev != nullptr)
			{
				--res;
				tmpNode = tmpNode->prev;
			}
			if (res == 0)
				return const_iterator(dq, tmpNode);
			
			--res;
			block *tmpBlock = tmpNode->par->prev;
			if (tmpBlock == nullptr)
				throw invalid_iterator();
			res -= tmpBlock->size;
			while (res >= 0 && tmpBlock->prev != nullptr)
			{
				tmpBlock = tmpBlock->prev;
				res -= tmpBlock->size;
			}
			res += tmpBlock->size;
			
			tmpNode = tmpBlock->listTail;
			while (res > 0 && tmpNode->prev != nullptr)
			{
				--res;
				tmpNode = tmpNode->prev;
			}
			if (res > 0)
				throw invalid_iterator();
			return const_iterator(dq, tmpNode);
		}
		// return th distance between two iterator,
		// if these two iterators points to different vectors, throw invaild_iterator.
		int operator-(const iterator &rhs) const
		{
			if (dq != rhs.dq)
				throw invalid_iterator();
			int ret = 0, cur = 0;
			for (block *i = dq->blockHead; i != nullptr; cur += i->size, i = i->succ)
			{
				if (nd->par == i || rhs.nd->par == i)
				{
					for (node *j = i->listHead; j != nullptr; ++cur, j = j->succ)
					{
						if (nd == j)
							ret += cur;
						if (rhs.nd == j)
							ret -= cur;
					}
					cur -= i->size;
				}
			}
			return ret;
		}
		int operator-(const const_iterator &rhs) const
		{
			if (dq != rhs.dq)
				throw invalid_iterator();
			int ret = 0, cur = 0;
			for (block *i = dq->blockHead; i != nullptr; cur += i->size, i = i->succ)
			{
				if (nd->par == i || rhs.nd->par == i)
				{
					for (node *j = i->listHead; j != nullptr; ++cur, j = j->succ)
					{
						if (nd == j)
							ret += cur;
						if (rhs.nd == j)
							ret -= cur;
					}
					cur -= i->size;
				}
			}
			return ret;
		}
		const_iterator operator+=(const int &n)
		{
			*this = (*this + n);
			return *this;
		}
		const_iterator operator-=(const int &n)
		{
			*this = (*this - n);
			return *this;
		}
		/**
		 * TODO iter++
		 */
		const_iterator operator++(int)
		{
			const_iterator ret(dq, nd);
			if (nd->succ != nullptr)
				nd = nd->succ;
			else if (nd->par->succ == nullptr)
				*this = dq->cend();
			else
				nd = nd->par->succ->listHead;
			return ret;
		}
		/**
		 * TODO ++iter
		 */
		const_iterator& operator++()
		{
			if (nd->succ != nullptr)
				nd = nd->succ;
			else if (nd->par->succ == nullptr)
				*this = dq->cend();
			else
				nd = nd->par->succ->listHead;
			return *this;
		}
		/**
		 * TODO iter--
		 */
		const_iterator operator--(int)
		{
			const_iterator ret(dq, nd);
			if (nd->prev != nullptr)
				nd = nd->prev;
			else if (nd->par->prev == nullptr)
				*this = dq->cend();
			else
				nd = nd->par->prev->listTail;
			return ret;
		}
		/**
		 * TODO --iter
		 */
		const_iterator& operator--()
		{
			if (nd->prev != nullptr)
				nd = nd->prev;
			else if (nd->par->prev == nullptr)
				*this = dq->cend();
			else
				nd = nd->par->prev->listTail;
			return *this;
		}
		/**
		 * TODO *it
		 */
		const T& operator*() const
		{
			if (*this == dq->cend())
				throw invalid_iterator();
			return *(nd->data);
		}
		/**
		 * TODO it->field
		 */
		const T* operator->() const noexcept
		{
			if (*this == dq->cend())
				throw invalid_iterator();
			return nd->data;
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		bool operator==(const iterator &rhs) const
		{
			return (dq == rhs.dq) && (nd == rhs.nd);
		}
		bool operator==(const const_iterator &rhs) const
		{
			return (dq == rhs.dq) && (nd == rhs.nd);
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const
		{
			return (dq != rhs.dq) || (nd != rhs.nd);
		}
		bool operator!=(const const_iterator &rhs) const
		{
			return (dq != rhs.dq) || (nd != rhs.nd);
		}
	};
	/**
	 * TODO Constructors
	 */
	deque()
	{
		blockHead = new block(1);
		blockTail = blockHead;
		blockHead->listHead = new node();
		blockHead->listTail = blockHead->listHead;
		blockHead->listHead->par = blockHead;
		dataSize = 1;
	}
	deque(const deque &other)
	{
		block *i = other.blockHead, **j = &blockHead, *pj = nullptr;
		while (i != nullptr)
		{
			(*j) = new block(i->size);
			
			node *x = i->listHead, **y = &((*j)->listHead), *py = nullptr;
			while (x != nullptr)
			{
				if (x->data != nullptr)
					(*y) = new node(*(x->data));
				else
					(*y) = new node();
				(*y)->prev = py;
				py = (*y);
				(*y)->par = (*j);
				
				y = &((*y)->succ);
				x = x->succ;
			}
			
			(*j)->prev = pj;
			pj = (*j);
			(*j)->listTail = py;
			
			j = &((*j)->succ);
			i = i->succ;
		}
		blockTail = pj;
		dataSize = other.dataSize;
	}
	/**
	 * TODO Deconstructor
	 */
	~deque()
	{
		for (block *i = blockHead, *j; i != nullptr; i = j)
		{
			j = i->succ;
			delete i;
		}
	}
	/**
	 * TODO assignment operator
	 */
	deque &operator=(const deque &other)
	{
		if (this == &other)
			return *this;
		this->~deque();
		block *i = other.blockHead, **j = &blockHead, *pj = nullptr;
		while (i != nullptr)
		{
			(*j) = new block(i->size);
			
			node *x = i->listHead, **y = &((*j)->listHead), *py = nullptr;
			while (x != nullptr)
			{
				if (x->data != nullptr)
					(*y) = new node(*(x->data));
				else
					(*y) = new node();
				(*y)->prev = py;
				py = (*y);
				(*y)->par = (*j);
				
				y = &((*y)->succ);
				x = x->succ;
			}
			
			(*j)->prev = pj;
			pj = (*j);
			(*j)->listTail = py;
			
			j = &((*j)->succ);
			i = i->succ;
		}
		blockTail = pj;
		dataSize = other.dataSize;
		return *this;
	}
	/**
	 * access specified element with bounds checking
	 * throw index_out_of_bound if out of bound.
	 */
	T & at(const size_t &pos)
	{
		if (pos < 0 || pos >= dataSize - 1)
			throw index_out_of_bound();
		int res = pos;
		block *tmpBlock = blockHead;
		res -= tmpBlock->size;
		while (res >= 0)
		{
			tmpBlock = tmpBlock->succ;
			res -= tmpBlock->size;
		}
		res += tmpBlock->size;
		
		node *tmpNode = tmpBlock->listHead;
		while (res > 0)
		{
			--res;
			tmpNode = tmpNode->succ;
		}
		return *(tmpNode->data);
	}
	const T & at(const size_t &pos) const
	{
		if (pos < 0 || pos >= dataSize - 1)
			throw index_out_of_bound();
		int res = pos;
		block *tmpBlock = blockHead;
		res -= tmpBlock->size;
		while (res >= 0)
		{
			tmpBlock = tmpBlock->succ;
			res -= tmpBlock->size;
		}
		res += tmpBlock->size;
		
		node *tmpNode = tmpBlock->listHead;
		while (res > 0)
		{
			--res;
			tmpNode = tmpNode->succ;
		}
		return *(tmpNode->data);
	}
	T & operator[](const size_t &pos)
	{
		return at(pos);
	}
	const T & operator[](const size_t &pos) const
	{
		return at(pos);
	}
	/**
	 * access the first element
	 * throw container_is_empty when the container is empty.
	 */
	const T & front() const
	{
		if (empty())
			throw container_is_empty();
		return *(blockHead->listHead->data);
	}
	/**
	 * access the last element
	 * throw container_is_empty when the container is empty.
	 */
	const T & back() const
	{
		if (empty())
			throw container_is_empty();
			
		node *ret = blockTail->listTail;
		if (ret->prev != nullptr)
			ret = ret->prev;
		else
			ret = ret->par->prev->listTail;
		return *(ret->data);
	}
	/**
	 * returns an iterator to the beginning.
	 */
	iterator begin()
	{
		return iterator(this, blockHead->listHead);
	}
	const_iterator cbegin() const
	{
		return const_iterator(this, blockHead->listHead);
	}
	/**
	 * returns an iterator to the end.
	 */
	iterator end()
	{
		return iterator(this, blockTail->listTail);
	}
	const_iterator cend() const
	{
		return const_iterator(this, blockTail->listTail);
	}
	/**
	 * checks whether the container is empty.
	 */
	bool empty() const
	{
		return dataSize == 1;
	}
	/**
	 * returns the number of elements
	 */
	size_t size() const
	{
		return dataSize - 1;
	}
	/**
	 * clears the contents
	 */
	void clear()
	{
		this->~deque();
		blockHead = new block(1);
		blockTail = blockHead;
		blockHead->listHead = new node();
		blockHead->listTail = blockHead->listHead;
		blockHead->listHead->par = blockHead;
		dataSize = 1;
	}
	/**
	 * inserts elements at the specified locat on in the container.
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value
	 *     throw if the iterator is invalid or it point to a wrong place.
	 */
	iterator insert(iterator pos, const T &value)
	{
		if (pos.dq != this)
			throw invalid_iterator();
		node *tmpNode = new node(value);
		tmpNode->par = pos.nd->par;
		tmpNode->prev = pos.nd->prev;
		tmpNode->succ = pos.nd;
		if (pos.nd->prev != nullptr)
			pos.nd->prev->succ = tmpNode;
		else
			pos.nd->par->listHead = tmpNode;
		pos.nd->prev = tmpNode;
		
		++dataSize;
		++pos.nd->par->size;
		
		maintain(pos.nd->par);
		
		return pos - 1;
	}
	/**
	 * removes specified element at pos.
	 * removes the element at pos.
	 * returns an iterator pointing to the following element, if pos pointing to the last element, end() will be returned.
	 * throw if the container is empty, the iterator is invalid or it points to a wrong place.
	 */
	iterator erase(iterator pos)
	{
		if (pos == this->end() || pos.dq != this)
			throw invalid_iterator();
		iterator ret = pos + 1;
		if (pos.nd->prev != nullptr)
			pos.nd->prev->succ = pos.nd->succ;
		else
			pos.nd->par->listHead = pos.nd->succ;
		if (pos.nd->succ != nullptr)
			pos.nd->succ->prev = pos.nd->prev;
		else
			pos.nd->par->listTail = pos.nd->prev;
		
		--dataSize;
		--pos.nd->par->size;
		maintain(pos.nd->par);
		
		delete pos.nd;
		
		return ret;
	}
	/**
	 * adds an element to the end
	 */
	void push_back(const T &value)
	{
		insert(this->end(), value);
	}
	/**
	 * removes the last element
	 *     throw when the container is empty.
	 */
	void pop_back()
	{
		erase(this->end() - 1);
	}
	/**
	 * inserts an element to the beginning.
	 */
	void push_front(const T &value)
	{
		insert(this->begin(), value);
	}
	/**
	 * removes the first element.
	 *     throw when the container is empty.
	 */
	void pop_front()
	{
		erase(this->begin());
	}
};

}

#endif
