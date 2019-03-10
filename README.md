# STLite

只可以用T类的复制构造函数。

不能用T类的`=`和默认构造函数。

## vector

iterator 没有越界检查

iterator.end() 指向最后一个元素的下一个位置

const_iterator 返回的值都应该是const类型

iterator要保存所在的vector地址，用以判断是否为同一个vector

## deque

## map

## priority_queue

CEtest自定类没有复制构造函数，`=` 会调用复制构造函数，一定要使用初始化列表
