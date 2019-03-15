# STLite

只可以用T类的复制构造函数

不能用T类的`=`和默认构造函数

`=`重载一定要调用析构

iterator要保存所在的容器，用以判断是否为同一个容器

## vector

iterator 没有越界检查

iterator.end() 指向最后一个元素的下一个位置

const_iterator 返回的值都应该是const类型

## deque

链表套链表实现

节点存数据保存一个T类型的指针，可以避免没有构造函数(慎用，会导致速度很慢)

需要添加end()节点

保证iterator的有效性：iterator不能存所在块

任何时候iterator都合法

*end()非法

## map

红黑树

没有写哨兵nul，可能会导致fixDelete的时候双黑节点是空节点，因此需要同时保存双黑节点和它的父亲

新建节点时候慎用二级指针，会指向某个节点的child指针，在fixInsert中旋转会导致指针失效

删除后把root重新赋为nullptr

## priority_queue

CEtest自定类没有复制构造函数，初始化的`=` 会调用复制构造函数，一定要使用初始化列表
