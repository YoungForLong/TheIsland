#pragma once

#include<stdlib.h>
#include<stack>
#include<algorithm>
#include<functional>
#include<string>
#include<list>
#include<vector>
#include<cassert>

using namespace std;

template<class __Type__>
struct node
{
	__Type__ data_;
	list<node<__Type__>*> children_;
	node()
	{
	}
};


template<class __Type__>
class SimpleTree//简单的单向树，元素必须为集合（不允许重复元素），用于储存实体类型树，决策树等;Simple directed tree,which is used to maintain Type tree,ID-3 tree etc. 
{
private:
	node<__Type__>* _root;

public:
	typedef std::function<bool(__Type__)> TraversalFunc;//定义遍历时的操作函数，最好使用lambda
public:
	SimpleTree()
	{
		_root = new node<__Type__>();
	}

	~SimpleTree()
	{
		if (this->size() != 0)
		{
			clear();
		}
	}

	void clear(node<__Type__>* leaf)
	{
		if (leaf == NULL)
		{
			return;
		}

		for (auto iter = leaf->children_.begin(); iter != leaf->children_.end(); iter++)
		{
			clear(*iter);
		}

		delete leaf;
		leaf = NULL;
	}

	void addChild(__Type__ parentData, __Type__ childData)
	{
		node<__Type__>* parentNode = findNodeByData(parentData);

		node<__Type__>* new_node = new node<__Type__>();

		new_node->data_ = childData;

		parentNode->children_.push_back(new_node);
	}

	node<__Type__>* findNodeByData(__Type__ purpose)
	{
		node<__Type__>* pOperate = NULL;

		stack<node<__Type__>*> nodeStack;

		nodeStack.push(_root);

		while (!nodeStack.empty())
		{
			pOperate = nodeStack.top();
			nodeStack.pop();

			if (pOperate->data_ == purpose)
				return pOperate;

			for (auto iter = pOperate->children_.begin(); iter != pOperate->children_.end(); iter++)
			{
				nodeStack.push(*iter);
			}
		}

		return pOperate;
	}

	bool traversal(node<__Type__>* leaf, TraversalFunc func)//前序遍历,可以使用lambda对其进行渲染，但是不会改变容器的内部结构
	{
		if (leaf == NULL)
			return false;

		if (func(leaf->data_))
			return true;

		for (auto iter = leaf->children_.begin(); iter != leaf->children_.end(); iter++)
		{
			if (traversal(*iter, func))
				return true;
		}

		return false;
	}

	bool postTraversal(node<__Type__>* leaf, TraversalFunc func)//后序遍历
	{
		if (leaf == NULL)
			return false;

		for (auto iter = leaf->children_.begin(); iter != leaf->children_.end(); iter++)
		{
			if (posterTraversal(*iter, func))
				return true;
		}

		if (func(leaf->data_))
			return true;

		return false;
	}

	node<__Type__>* findFather(__Type__ purpose)
	{

		assert(_root->data_ != purpose&&"cannot deal with root");

		stack<node<__Type__>*> nodeStack;

		node<__Type__>* pOperate = NULL;

		nodeStack.push(_root);

		while (!nodeStack.empty())
		{
			pOperate = nodeStack.top();
			nodeStack.pop();

			for (auto iter = pOperate->children_.begin(); iter != pOperate->children_.end(); iter++)
			{
				if ((*iter)->data_ == purpose)
				{
					return pOperate;
				}

				nodeStack.push(*iter);
			}
		}

		return nullptr;
	}

	vector<__Type__> track(__Type__ purpose)//跟踪，找到一个分类器的包含关系
	{
		vector<__Type__> resultArr;

		if (_root->data_ == purpose)
		{
			resultArr.push_back(purpose);

			return resultArr;
		}

		node<__Type__>* pOperate = NULL;
		pOperate = findNodeByData(purpose);
		resultArr.push_back(pOperate->data_);

		stack<node<__Type__>*> nodeStack;
		nodeStack.push(findFather(purpose));

		while (!nodeStack.empty())
		{
			pOperate = nodeStack.top();

			nodeStack.pop();

			resultArr.push_back(pOperate->data_);

			if (pOperate != _root)
				nodeStack.push(findFather(pOperate->data_));
		}

		return resultArr;
	}

	void combine(SimpleTree<__Type__>* other, __Type__ origin_leaf)//组合两个树
	{
		node<__Type__>* other_root = other->root();

		node<__Type__>* leaf = findNodeByData(origin_leaf);

		leaf->children_.push_back(other_root);
	}

	bool slice(__Type__ data)//剪切,一片叶子或者一个树
	{
		node<__Type__>* child = findNodeByData(data);

		node<__Type__>* father = findFather(data);

		for (auto iter = father->children_.begin(); iter != father->children_.end();)
		{
			if ((*iter)->data_ == data)
			{
				father->children_.erase(iter++);
				return true;
			}
			else
				iter++;
		}

		return false;
	}

	const unsigned int size()
	{
		unsigned int size_ = 0;
		traversal(_root, [&size_](__Type__ data){
			size_++;
			return false;
		});

		return size_;
	}

public://strs&gtrs
	void setRoot(__Type__ rootData)
	{
		_root->data_ = rootData;
	}

	node<__Type__>* root()const { return _root; }
};

