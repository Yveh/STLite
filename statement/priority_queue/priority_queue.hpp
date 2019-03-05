#ifndef SJTU_PRIORITY_QUEUE_HPP
#define SJTU_PRIORITY_QUEUE_HPP

#include <cstddef>
#include <functional>
#include "exceptions.hpp"

namespace sjtu {

/**
 * a container like std::priority_queue which is a heap internal.
 * it should be based on the vector written by yourself.
 */
template<typename T, class Compare = std::less<T>>
class priority_queue {
private:
	class node
	{
		public:
			T key;
			size_t degree, mark;
			node *left, *right, *child, *parent;

            node(T _key = 0) : key(_key), degree(0), mark(0), left(this), right(this), child(nullptr), parent(nullptr)
            {
            }
            ~node()
            {
            }
	};

	size_t keyNum, maxDegree;
    node *min;
public:
	/**
	 * TODO constructors
	 */
	priority_queue()
    {
        keyNum = 0;
        maxDegree = 0;
        min = nullptr;
    }
	priority_queue(const priority_queue &other)
    {
        keyNum = other.keyNum;
        maxDegree = other.maxDegree;
        min = other.min;
    }
	/**
	 * TODO deconstructor
	 */
	void del(node *cur)
    {
        if (cur != nullptr)
        {
            node *_left(cur->left),  *_right(cur->right), *_child(cur->child);
            delete cur;
            del(_left);
            del(_right);
            del(_child);
        }
    }
	~priority_queue()
    {
        del(min);
    }
	/**
	 * TODO Assignment operator
	 */
	priority_queue &operator=(const priority_queue &other)
    {
        keyNum = other.keyNum;
        maxDegree = other.maxDegree;
        min = other.min;
        return this;
    }
	/**
	 * get the top of the queue.
	 * @return a reference of the top element.
	 * throw container_is_empty if empty() returns true;
	 */
	const T & top() const
    {
        return min->key;
    }
	/**
	 * TODO
	 * push new element to the priority queue.
	 */
	void push(const T &e)
    {
        ++keyNum;
        node *nd = new node(e);
        if (min.empty())
        {
            min = nd;
        }
        else
        {
            if (Compare()(min->key, e))
            {
                nd->right = min->right;
                min->right->left = nd;
                nd->left = min;
                min->right = nd;
            }
            else
            {
                nd->left = min->left;
                min->left->right = nd;
                nd->right = min;
                min->left = nd;
                min = nd;
            }
        }
    }
	/**
	 * TODO
	 * delete the top element.
	 * throw container_is_empty if empty() returns true;
	 */
	void consolidate()
    {
        
    }
	void pop()
    {
        if (!empty())
        {
            node *i = min->child;
            repeat
            {
                i->right = min->right;
                min->right->left = i;
                i->left = min;
                min->right = i;
                i->parent = nullptr;
            }while(i == min->child);
            --keyNum;
            if (!empty())
            {
                delete min;
            }
            else
            {
                node *tmp = min->right;
                delete min;
                min = tmp;
                consolidate();
            }
        }
    }
	/**
	 * return the number of the elements.
	 */
	size_t size() const
    {
        return keyNum;
    }
	/**
	 * check if the container has at least an element.
	 * @return true if it is empty, false if it has at least an element.
	 */
	bool empty() const
    {
        return keyNum == 0;
    }
	/**
	 * return a merged priority_queue with at least O(logn) complexity.
	 */
	void merge(priority_queue &other)
    {
        if (other.empty()) return;
        if (empty())
        {
            min = other.min;
            keyNum = other.keyNum;
            maxDegree = other.maxDegree;
        }
        else if (Compare()(min->key, other.min->key))
        {
            other.min->left->right = min->right;
            min->right->left = other.min->left;
            other.min->left = min;
            min->right = other.min;
        }
        else
        {
            other.min->left = min->left;
            min->left->right = other.min;
            other.min->right = min;
            min->left = other.min;
            min = other.min;
        }
    }
};

}

#endif
