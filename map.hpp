/**
 * implement a container like std::map
 */
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

// only for std::less<T>
#include <functional>
#include <cstddef>
#include "utility.hpp"
#include "exceptions.hpp"

namespace sjtu
{

template<class Key,	class T, class Compare = std::less<Key> >
class map
{
public:
	typedef pair<const Key, T> value_type;
	//
private:
	class node
	{
		friend map;
	private:
		value_type value;
		node *ch[2], *par;
		bool color;
	public:
		node(const value_type &_value) : value(_value), color(0), par(nullptr)
		{
			ch[0] = ch[1] = nullptr;
		}
		node(const Key &other) : value(other, T()), color(0), par(nullptr)
		{
			ch[0] = ch[1] = nullptr;
		}
		~node()
		{
		}
	};
	node *root;
	size_t dataSize;
	Compare cc;

	bool equal(const Key &x, const Key &y) const
	{
		return !cc(x, y) && !cc(y, x);
	}
	bool chlr(node *x) const
	{
		return x->par->ch[1] == x;
	}
	//0-left 1-right
	void rotate(node *x, bool t)
	{
		node *y = x->ch[t ^ 1];
		x->ch[t ^ 1] = y->ch[t];
		if (y->ch[t] != nullptr)
			y->ch[t]->par = x;
		if (x == root)
			root = y;
		else
			x->par->ch[chlr(x)] = y;
		y->par = x->par;
		y->ch[t] = x;
		x->par = y;
	}
	node *prev(node *x) const
	{
		if (x->ch[0] == nullptr)
		{
			while (x != root && chlr(x) == 0)
				x = x->par;
			return x->par;
		}
		else
		{
			x = x->ch[0];
			while (x->ch[1] != nullptr)
				x = x->ch[1];
			return x;
		}
	}
	node *succ(node *x) const
	{
		if (x->ch[1] == nullptr)
		{
			while (x != root && chlr(x) == 1)
				x = x->par;
			return x->par;
		}
		else
		{
			x = x->ch[1];
			while (x->ch[0] != nullptr)
				x = x->ch[0];
			return x;
		}
	}
	void fixInsert(node *z)
	{
		//if z's deep = 2, then z->par(root)->color = 0
		while (z->par != nullptr && z->par->color == 1)
		{
			bool t = chlr(z->par);
			node *y = z->par->par->ch[t ^ 1];
			if (y != nullptr && y->color == 1)
			{
				z->par->color = 0;
				y->color = 0;
				z->par->par->color = 1;
				z = z->par->par;
			}
			else
			{
				if (z == z->par->ch[t ^ 1])
				{
					z = z->par;
					rotate(z, t);
				}
				z->par->color = 0;
				z->par->par->color = 1;
				rotate(z->par->par, t ^ 1);
			}
		}
		root->color = 0;
	}
	void fixDelete(node *x, node *xp)
	{
		while ((x == nullptr || x->color == 0) && x != root)
		{
			bool t = xp->ch[1] == x;
			//x must have a brother
			node *w = xp->ch[t ^ 1];
			if (w->color == 1)
			{
				w->color = 0;
				xp->color = 1;
				rotate(xp, t);
				w = xp->ch[t ^ 1];
			}
			if ((w->ch[t] == nullptr || w->ch[t]->color == 0) && (w->ch[t ^ 1] == nullptr || w->ch[t ^ 1]->color == 0))
			{
				w->color = 1;
				x = xp;
				xp = x->par;
			}
			else
			{
				if (w->ch[t ^ 1] == nullptr || w->ch[t ^ 1]->color == 0)
				{
					w->ch[t]->color = 0;
					w->color = 1;
					rotate(w, t ^ 1);
					w = xp->ch[t ^ 1];
				}
				
				w->color = xp->color;
				xp->color = 0;
				w->ch[t ^ 1]->color = 0;
				rotate(xp, t);
				x = root;
			}
		}
		if (x != nullptr)
			x->color = 0;
	}
	void cra(node **cur, node *other)
    {
    	if (other == nullptr)
    		return;
    	(*cur) = new node(other->value);
    	
	    (*cur)->color = other->color;
	    cra(&((*cur)->ch[0]), other->ch[0]);
	    if ((*cur)->ch[0] != nullptr)
		    (*cur)->ch[0]->par = (*cur);
	    cra(&((*cur)->ch[1]), other->ch[1]);
	   	if ((*cur)->ch[1] != nullptr)
	   		(*cur)->ch[1]->par = (*cur);
    }
    void del(node *cur)
    {
    	if (cur == nullptr)
    		return;
    	del(cur->ch[0]);
    	del(cur->ch[1]);
    	delete cur;
    }
public:
	/**
	 * the internal type of data.
	 * it should have a default constructor, a copy constructor.
	 * You can use sjtu::map as value_type by typedef.
	 */
	/**
	 * see BidirectionalIterator at CppReference for help.
	 *
	 * if there is anything wrong throw invalid_iterator.
	 *     like it = map.begin(); --it;
	 *       or it = map.end(); ++end();
	 */
	class const_iterator;
	class iterator
	{
		friend map;
	private:
		map *mp;
		node *nd;
		iterator(map *_mp, node *_nd) : mp(_mp), nd(_nd) {}
	public:
		iterator() : mp(nullptr), nd(nullptr) {}
		iterator(const iterator &other) : mp(other.mp), nd(other.nd) {}
		/**
		 * return a new iterator which pointer n-next elements
		 *   even if there are not enough elements, just return the answer.
		 * as well as operator-
		 */
		/**
		 * TODO iter++
		 */
		iterator operator++(int)
		{
			if (*this == mp->end())
				throw invalid_iterator();
			iterator ret(*this);
			nd = mp->succ(nd);
			return ret;
		}
		/**
		 * TODO ++iter
		 */
		iterator & operator++()
		{
			if (*this == mp->end())
				throw invalid_iterator();
			nd = mp->succ(nd);
			return *this;
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int)
		{
			if (*this == mp->begin())
				throw invalid_iterator();
			iterator ret(*this);
			if (*this == mp->end())
			{
				nd = mp->root;
				while (nd->ch[1] != nullptr)
					nd = nd->ch[1];
			}
			else
				nd = mp->prev(nd);
			return ret;
		}
		/**
		 * TODO --iter
		 */
		iterator & operator--()
		{
			if (*this == mp->begin())
				throw invalid_iterator();
			if (*this == mp->end())
			{
				nd = mp->root;
				while (nd->ch[1] != nullptr)
					nd = nd->ch[1];
			}
			else
				nd = mp->prev(nd);
			return *this;
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		value_type & operator*() const
		{
			return nd->value;
		}
		bool operator==(const iterator &rhs) const
		{
			return mp == rhs.mp && nd == rhs.nd;
		}
		bool operator==(const const_iterator &rhs) const
		{
			return mp == rhs.mp && nd == rhs.nd;
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const
		{
			return mp != rhs.mp || nd != rhs.nd;
		}
		bool operator!=(const const_iterator &rhs) const
		{
			return mp != rhs.mp || nd != rhs.nd;
		}

		/**
		 * for the support of it->first. 
		 * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
		 */
		value_type* operator->() const noexcept
		{
			return &(nd->value);
		}
	};
	class const_iterator
	{
		friend map;
		// it should has similar member method as iterator.
		//  and it should be able to construct from an iterator.
	private:
		const map *mp;
		node *nd;
		const_iterator(const map *_mp, node *_nd) : mp(_mp), nd(_nd) {}
	public:
		const_iterator() : mp(nullptr), nd(nullptr) {}
		const_iterator(const const_iterator &other) : mp(other.mp), nd(other.nd) {}
		const_iterator(const iterator &other) : mp(other.mp), nd(other.nd) {}
		const_iterator operator++(int)
		{
			if (*this == mp->cend())
				throw invalid_iterator();
			const_iterator ret(*this);
			nd = mp->succ(nd);
			return ret;
		}
		/**
		 * TODO ++iter
		 */
		const_iterator & operator++()
		{
			if (*this == mp->cend())
				throw invalid_iterator();
			nd = mp->succ(nd);
			return *this;
		}
		/**
		 * TODO iter--
		 */
		const_iterator operator--(int)
		{
			if (*this == mp->cbegin())
				throw invalid_iterator();
			const_iterator ret(*this);
			if (*this == mp->cend())
			{
				nd = mp->root;
				while (nd->ch[1] != nullptr)
					nd = nd->ch[1];
			}
			else
				nd = mp->prev(nd);
			return ret;
		}
		/**
		 * TODO --iter
		 */
		const_iterator & operator--()
		{
			if (*this == mp->cbegin())
				throw invalid_iterator();
			if (*this == mp->cend())
			{
				nd = mp->root;
				while (nd->ch[1] != nullptr)
					nd = nd->ch[1];
			}
			else
				nd = mp->prev(nd);
			return *this;
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		const value_type & operator*() const
		{
			return nd->value;
		}
		bool operator==(const iterator &rhs) const
		{
			return mp == rhs.mp && nd == rhs.nd;
		}
		bool operator==(const const_iterator &rhs) const
		{
			return mp == rhs.mp && nd == rhs.nd;
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const
		{
			return mp != rhs.mp || nd != rhs.nd;
		}
		bool operator!=(const const_iterator &rhs) const
		{
			return mp != rhs.mp || nd != rhs.nd;
		}

		/**
		 * for the support of it->first. 
		 * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
		 */
		const value_type* operator->() const noexcept
		{
			return &(nd->value);
		}
	};

	map()
	{
		root = nullptr;
		dataSize = 0;
	}
	map(const map &other)
	{
		root = nullptr;
		cra(&root, other.root);
		dataSize = other.dataSize;
	}
	/**
	 * TODO assignment operator
	 */
	map & operator=(const map &other)
	{
		if (this == &other)
			return *this;
		del(root);
		root = nullptr;
		cra(&root, other.root);
		dataSize = other.dataSize;
	}
	/**
	 * TODO Destructors
	 */
	~map()
	{
		del(root);
	}
	/**
	 * TODO
	 * access specified element with bounds checking
	 * Returns a reference to the mapped value of the element with key equivalent to key.
	 * If no such element exists, an exception of type `index_out_of_bound'
	 */
	T & at(const Key &key)
	{
		node *cur = root;
		while (cur != nullptr)
		{
			if (equal(cur->value.first, key))
				return cur->value.second;
			cur = cur->ch[cc(cur->value.first, key)];
		}
		throw index_out_of_bound();
	}
	const T & at(const Key &key) const
	{
		node *cur = root;
		while (cur != nullptr)
		{
			if (equal(cur->value.first, key))
				return cur->value.second;
			cur = cur->ch[cc(cur->value.first, key)];
		}
		throw index_out_of_bound();
	}
	/**
	 * TODO
	 * access specified element 
	 * Returns a reference to the value that is mapped to a key equivalent to key,
	 *   performing an insertion if such key does not already exist.
	 */
	T & operator[](const Key &key)
	{
		node **cur = &root, *tmp = nullptr;
		while (*cur != nullptr)
		{
			if (equal((*cur)->value.first, key))
				return (*cur)->value.second;
			tmp = *cur;
			cur = &((*cur)->ch[cc((*cur)->value.first, key)]);
		}
		*cur = new node(key);
		(*cur)->par = tmp;
		(*cur)->color = 1;
		++dataSize;
		node *ret = *cur;
		fixInsert(*cur);
		return ret->value.second;
	}
	/**
	 * behave like at() throw index_out_of_bound if such key does not exist.
	 */
	const T & operator[](const Key &key) const
	{
		return at(key);
	}
	/**
	 * return a iterator to the beginning
	 */
	iterator begin()
	{
		node *cur = root;
		while (cur != nullptr && cur->ch[0] != nullptr)
			cur = cur->ch[0];
		return iterator(this, cur);
	}
	const_iterator cbegin() const
	{
		node *cur = root;
		while (cur != nullptr && cur->ch[0] != nullptr)
			cur = cur->ch[0];
		return const_iterator(this, cur);
	}
	/**
	 * return a iterator to the end
	 * in fact, it returns past-the-end.
	 */
	iterator end()
	{
		return iterator(this, nullptr);
	}
	const_iterator cend() const
	{
		return const_iterator(this, nullptr);
	}
	/**
	 * checks whether the container is empty
	 * return true if empty, otherwise false.
	 */
	bool empty() const
	{
		return dataSize == 0;
	}
	/**
	 * returns the number of elements.
	 */
	size_t size() const
	{
		return dataSize;
	}
	/**
	 * clears the contents
	 */
	void clear()
	{
		del(root);
		root = nullptr;
		dataSize = 0;
	}
	/**
	 * insert an element.
	 * return a pair, the first of the pair is
	 *   the iterator to the new element (or the element that prevented the insertion), 
	 *   the second one is true if insert successfully, or false.
	 */
	pair<iterator, bool> insert(const value_type &value)
	{
		node **cur = &root, *tmp = nullptr;
		while (*cur != nullptr)
		{
			if (equal((*cur)->value.first, value.first))
				return pair<iterator, bool>(iterator(this, *cur), 0);
			tmp = *cur;
			cur = &((*cur)->ch[cc((*cur)->value.first, value.first)]);
		}
		*cur = new node(value);
		++dataSize;
		(*cur)->par = tmp;
		(*cur)->color = 1;
		node *ret = *cur;
		fixInsert(*cur);
		return pair<iterator, bool>(iterator(this, ret), 1);
	}
	/**
	 * erase the element at pos.
	 *
	 * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
	 */
	void erase(iterator pos)
	{
		if (pos.mp != this || pos == end())
			throw invalid_iterator();
		--dataSize;
		node *z = pos.nd, *y, *x, *xp;
		bool color;
		if (z->ch[0] != nullptr && z->ch[1] != nullptr)
		{
			y = z->ch[1];
			while (y->ch[0] != nullptr)
				y = y->ch[0];
			
			if (z == root)
				root = y;
			else
				z->par->ch[chlr(z)] = y;
				
			x = y->ch[1];
			xp = y->par;
			color = y->color;
			
			if (y->par == z)
			{
				xp = y;
			}
			else
			{
				if (x != nullptr)
					x->par = xp;
				xp->ch[0] = x;
				
				y->ch[1] = z->ch[1];
				z->ch[1]->par = y;
			}
			y->par = z->par;
			y->color = z->color;
			y->ch[0] = z->ch[0];
			z->ch[0]->par = y;
			
			if (color == 0)
				fixDelete(x, xp);
			delete z;
			return;
		}
		
		if (z->ch[0] == nullptr)
			x = z->ch[1];
		else
			x = z->ch[0];
		xp = z->par;
		color = z->color;
		if (x != nullptr)
			x->par = xp;
			
		if (z == root)
			root = x;
		else
			xp->ch[chlr(z)] = x;
		if (color == 0)
			fixDelete(x, xp);
		delete z;
	}
	/**
	 * Returns the number of elements with key 
	 *   that compares equivalent to the specified argument,
	 *   which is either 1 or 0 
	 *     since this container does not allow duplicates.
	 * The default method of check the equivalence is !(a < b || b > a)
	 */
	size_t count(const Key &key) const
	{
		node *cur = root;
		while (cur != nullptr)
		{
			if (equal(cur->value.first, key))
				return 1;
			cur = cur->ch[cc(cur->value.first, key)];
		}
		return 0;
	}
	/**
	 * Finds an element with key equivalent to key.
	 * key value of the element to search for.
	 * Iterator to an element with key equivalent to key.
	 *   If no such element is found, past-the-end (see end()) iterator is returned.
	 */
	iterator find(const Key &key)
	{
		node *cur = root;
		while (cur != nullptr)
		{
			if (equal(cur->value.first, key))
				return iterator(this, cur);
			cur = cur->ch[cc(cur->value.first, key)];
		}
		return end();
	}
	const_iterator find(const Key &key) const
	{
		node *cur = root;
		while (cur != nullptr)
		{
			if (equal(cur->value.first, key))
				return const_iterator(this, cur);
			cur = cur->ch[cc(cur->value.first, key)];
		}
		return cend();
	}
};

}

#endif
