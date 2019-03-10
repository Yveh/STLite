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
	typedef pair<const Key, T> value_type;
private:
	class node
	{
	private:
		value_type value;
		node *ch[2], *par;
		bool color;
	public:
		node();
		~node() {};
	};
	node *null, *root;
	size_t dataSize;
	Compare cc;

	node::node()
	{
		ch[0] = ch[1] = par = null;
		color = 0;
	}

	bool equal(const T &x, const T &y) const
	{
		return cc(x, y) ^ cc(y, x);
	}
	void setChild(node *x, node *y, bool type)
	{
		x->ch[type] = y;
		y->par = x;
	}
	bool chlr(node *x)
	{
		return x->par->ch[1] == x;
	}
	//0-left 1-right
	void rotate(node *x, bool type)
	{
		/*
		node *y = x->par;
		bool t = x->chlr();
		setChild(y, x->ch[t ^ 1], t);
		if (y == root)
			root = x;
		else
			setChild(y->par, x, y->chlr());
		setChild(x, y, t ^ 1);
		*/
		node *y = x->ch[1 ^ type];
		setChild(x, y->ch[type], 1 ^ type);
		x->ch[1 ^ type] = y->ch[type];
		if (x->par == null)
			root = y;
		else
			setChild(x->par, y, chlr(x));
		setChild(y, x, type);
	}
	node *prev(node *x)
	{
	}
	node *succ(node *x)
	{
		
	}
	void fixInsert(node *z)
	{
		while (z->par->color == 1)
		{
			node *y = z->par->par->ch[chlr(z->par) ^ 1];
			if (y->color == 1)
			{
				z->par->color = 0;
				y->color = 0;
				z->par->par->color = 1;
				z = z->par->par;
			}
			else
			{
				z->par->color = 0;
				z->par->par = 1;
				rotate(z->par->par, z->chlr());
			}
		}
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
	class iterator {
	private:
		map<Key, T>* mp;
		node* nd;
		
	public:
		iterator() {}
		iterator(const iterator &other)
		{
			mp = other.mp;
			nd = other.nd;
		}
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
			iterator ret(*this);
			nd = mp->succ(nd);
			return ret;
		}
		/**
		 * TODO ++iter
		 */
		iterator & operator++()
		{
			nd = mp->succ(nd);
			return *this;
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int)
		{
			iterator ret(*this);
			nd = mp->prev(nd);
			return ret;
		}
		/**
		 * TODO --iter
		 */
		iterator & operator--()
		{
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
			return mp == rhs.np && nd == rhs.nd;
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
	class const_iterator {
		// it should has similar member method as iterator.
		//  and it should be able to construct from an iterator.
	private:
		map<Key, T>* mp;
		node* nd;
	public:
		const_iterator() {}
		const_iterator(const const_iterator &other)
		{
			mp = other.mp;
			nd = other.nd;
		}
		const_iterator(const iterator &other)
		{
			mp = other.mp;
			nd = other.nd;
		}
		const_iterator operator++(int)
		{
			const_iterator ret(*this);
			nd = mp->succ(nd);
			return ret;
		}
		/**
		 * TODO ++iter
		 */
		const_iterator & operator++()
		{
			nd = mp->succ(nd);
			return *this;
		}
		/**
		 * TODO iter--
		 */
		const_iterator operator--(int)
		{
			const_iterator ret(*this);
			nd = mp->prev(nd);
			return ret;
		}
		/**
		 * TODO --iter
		 */
		const_iterator & operator--()
		{
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
			return mp == rhs.np && nd == rhs.nd;
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
		null = new node();
		Root = null;
	}
	map(const map &other) {}
	/**
	 * TODO assignment operator
	 */
	map & operator=(const map &other) {}
	/**
	 * TODO Destructors
	 */
	~map() {}
	/**
	 * TODO
	 * access specified element with bounds checking
	 * Returns a reference to the mapped value of the element with key equivalent to key.
	 * If no such element exists, an exception of type `index_out_of_bound'
	 */
	T & at(const Key &key) {}
	const T & at(const Key &key) const {}
	/**
	 * TODO
	 * access specified element 
	 * Returns a reference to the value that is mapped to a key equivalent to key,
	 *   performing an insertion if such key does not already exist.
	 */
	T & operator[](const Key &key) {}
	/**
	 * behave like at() throw index_out_of_bound if such key does not exist.
	 */
	const T & operator[](const Key &key) const {}
	/**
	 * return a iterator to the beginning
	 */
	iterator begin() {}
	const_iterator cbegin() const {}
	/**
	 * return a iterator to the end
	 * in fact, it returns past-the-end.
	 */
	iterator end() {}
	const_iterator cend() const {}
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
	void clear() {}
	/**
	 * insert an element.
	 * return a pair, the first of the pair is
	 *   the iterator to the new element (or the element that prevented the insertion), 
	 *   the second one is true if insert successfully, or false.
	 */
	pair<iterator, bool> insert(const value_type &value)
	{
		node **cur = &root, *tmp = null;
		if (*cur != null)
		{
			if (equal((*cur)->key), value->first))
				return pair(iterator(*cur), 0);
			tmp = *cur;
			cur = &((*cur)->ch[cc((*cur)->key), value->first]);
		}
		*cur = new node(value);
		(*cur)->par = tmp;
		(*cur)->color = 1;
		fixInsert(*cur);
		return pair(iterator(*cur), 1);
	}
	/**
	 * erase the element at pos.
	 *
	 * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
	 */
	void erase(iterator pos)
	{
		node *z = pos->nd;
		node *y = z;
		bool color = y->color;
		if (z->ch[0] == null)
		{
			x = z->ch[1];
			transPlant(z, z->ch[1]);
		}
		else if (z->ch[1] == null)
		{
			x = z->ch[0];
			transPlant(z, z->ch[0]);
		}
		else
		{
			y = z->ch[1];
			while (y->ch[0] != null)
				y = y->ch[0];
			color = y->color;
			x = y->right;
			if (y)
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
		node **cur = &root;
		if (*cur != null)
		{
			if (equal((*cur)->key), key))
				return 1;
			cur = &((*cur)->ch[cc((*cur)->key), key]);
		}
		return 0;
	}
	/**
	 * Finds an element with key equivalent to key.
	 * key value of the element to search for.
	 * Iterator to an element with key equivalent to key.
	 *   If no such element is found, past-the-end (see end()) iterator is returned.
	 */
	iterator find(const Key &key) {}
	const_iterator find(const Key &key) const {}
};

}

#endif
