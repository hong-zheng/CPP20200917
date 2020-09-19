#define _CRT_SECURE_NO_WARNINGS 1
#include <iostream>
using namespace std;

// 定义
template<class T>
struct AVLNode{
	T _val; // 结点值
	AVLNode<T>* _left;
	AVLNode<T>* _right;
	AVLNode<T>* _parent;
	int _bf; // 平衡因子，由右子树高度-左子树高度，缺省为0

	AVLNode(const T& val = T())
		:_val(val)
		, _left(nullptr)
		, _right(nullptr)
		, _parent(nullptr)
		, _bf(0)
	{}
};

template<class T>
class AVLTree{
public:
	typedef AVLNode<T> Node;
	AVLTree(){}
	
	bool insert(const T& val)
	{
		//根节点为空
		if (_root == nullptr)
		{
			_root = new Node(val);
			return true;
		}
		else //根节点非空
		{
			Node* cur = _root;
			Node* parent = nullptr;
			// 需要满足搜索树性质
			while (cur)
			{
				parent = cur;
				if (cur->_val == val)
				{
					// 在平衡树中已经存在，无需再插入,return false
					return false;
				}
				if (cur->_val < val)
				{
					cur = cur->_right;
				}
				else if (cur->_val > val)
				{
					cur = cur->_left;
				}
			}
			// 此时插入parent的子节点上
			cur = new Node(val);
			//将当前结点的父节点保存
			cur->_parent = parent;
			// 插入
			if (parent->_val < val)
			{
				parent->_right = cur;
			}
			else
			{
				parent->_left = cur;
			}
			// 插入完成之后根据值的平衡因子的情况进行调整
			
			while (parent)
			{
				// 1、更新平衡因子
				if (parent->_left == cur)
				{
					--parent->_bf;
				}
				else if (parent->_right == cur)
				{
					++parent->_bf;
				}

				//根据平衡因子做下一步决定
				if (parent->_bf == 0)
				{
					// 已经平衡，直接退出
					break;
				}
				// 当前结点并不平衡，仍然需要向上继续调整
				else if (parent->_bf == 1 || parent->_bf == -1)
				{
					cur = parent;
					parent = parent->_parent;
				}
				// 此时的插入已经极度不平衡，需要做调整，而不是简单的更新
				else if (parent->_bf == 2 || parent->_bf == -2)
				{
					// 右边的右边高：需要左旋
					if (parent->_bf == 2 && cur->_bf == 1)
					{
						RotateL(parent);
						cout << "insert : val = " << val << "-->左旋" << endl;
					}
					// 左边的左边高，需要右旋
					else if (parent->_bf == -2 && cur->_bf == -1)
					{
						RotateR(parent);
						cout << "insert : val = " << val << "-->右旋" << endl;
					}
					// 左边的右边高：左右旋转
					else if (parent->_bf == -2 && cur->_bf == 1)
					{
						Node* subL = cur;
						Node* subLR = cur->_right;
						int bf = subLR->_bf;

						RotateL(cur);
						RotateR(parent);

						if (subLR->_bf == 1)
						{
							parent->_bf = 0;
							subL->_bf = -1;
						}
						else if (subLR->_bf == -1)
						{
							parent->_bf = 1;
							subL->_bf = 0;
						}
						cout << "insert : val = " << val << "-->左右旋" << endl;
					}
					// 右边的左边高:右左旋转
					else if (parent->_bf == 2 && cur->_bf == -1)
					{
						Node* subR = cur;
						Node* subRL = subR->_left;
						int bf = subRL->_bf;

						RotateR(cur);
						RotateL(parent);

						if (bf == 1)
						{
							subR->_bf = 0;
							parent->_bf = -1;
						}
						else if (bf == -1)
						{
							subR->_bf = 1;
							parent->_bf = 0;
						}
						cout << "insert : val = " << val << "-->右左旋" << endl;
					}
					// 调整完之后就直接退出，调整完之后直接就平衡了，不需要再左bf平衡因子的更新
					break;
				}
			}
		}
		return true;
	}


	// 判断是否为平衡树
	bool isBalance()
	{
		return _isBalance(_root);
	}

	void inorder()
	{
		_inorder(_root);
		cout << endl;
	}
private:
	void _inorder(Node* root)
	{
		if (nullptr == root)
			return;
		_inorder(root->_left);
		cout << root->_val << " ";
		_inorder(root->_right);
	}
	bool _isBalance(Node* root)
	{
		if (root == nullptr)
			return true;
		int bf = Height(root->_right) - Height(root->_left);
		if (bf != root->_bf)
		{
			cout << "val :" << root->_val << "->不平衡" << endl;
		}
		return abs(bf) < 2
			&& _isBalance(root->_left)
			&& _isBalance(root->_right);
	}

	int Height(Node* root)
	{
		if (nullptr == root)
			return 0;
		int left = Height(root->_left);
		int right = Height(root->_right);
		return left > right ? left + 1 : right + 1;
	}

	void RotateR(Node* parent)
	{
		Node* subL = parent->_left;
		Node* subLR = subL->_right;

		subL->_right = parent;
		parent->_left = subLR;
		if (subLR)
			subLR->_parent = parent;
		if (parent == _root)
		{
			_root = subL;
			subL->_parent = nullptr;
		}
		else
		{
			Node* g = parent->_parent;
			if (g->_left == parent)
			{
				g->_left = subL;
			}
			else if (g->_right == parent)
			{
				g->_right = subL;
			}
			subL->_parent = g;
		}
		parent->_parent = subL;
		parent->_bf = subL->_bf = 0;
	}
	void RotateL(Node* parent)
	{
		Node* subR = parent->_right;
		Node* subRL = subR->_left;

		subR->_left = parent;
		parent->_right = subRL;
		if (subRL)
			subRL->_parent = parent;

		if (parent == _root)
		{
			_root = subR;
			_root->_parent = nullptr;
		}
		else
		{
			Node* g = parent->_parent;
			if (g->_left == parent)
			{
				g->_left = subR;
			}
			else if (g->_right == parent)
			{
				g->_right = subR;
			}
			subR->_parent = g;
		}
		parent->_parent = subR;
		parent->_bf = subR->_bf = 0;
	}
private:
	Node* _root;
};
void testAVL(){
	//int arr[] = { 16, 3, 7, 11, 9, 26, 18, 14, 15 };
	int arr[] = { 4, 2, 6, 1, 3, 5, 15, 7, 16, 14 };
	AVLTree<int> avl;
	for (const auto& e : arr)
	{
		avl.insert(e); 
	}
	avl.inorder();
	cout << avl.isBalance() << endl;
}
int main(){

	testAVL();
	system("pause");
	return 0;
}