#pragma once
#include "redblacktree.h"

class RedBlackTreeTest
{
public:

#define TEST_DECL(testName) \
	template<typename T, typename Less = std::less<T>> \
	static bool testName(const RedBlackTree<T, Less>& tree)

	TEST_DECL(isEmpty);
	TEST_DECL(allPointersAreValid);
	TEST_DECL(isBinarySearchTree);
	TEST_DECL(rootIsBlack);
	TEST_DECL(bothChildrenOfRedAreBlack);
	TEST_DECL(blackLengthIsCorrectForEveryNode);

#undef TEST_DECL
};

#define TEST_DEF(testName) \
template<typename T, typename Less> \
inline bool RedBlackTreeTest::testName(const RedBlackTree<T, Less>& tree)

TEST_DEF(isEmpty)
{
	return tree.size() == 0 && tree.m_root == nullptr;
}

template<typename T, typename Less>
inline bool isBinarySearchTreeImpl(const Node<T>* node, const Less& less)
{
	if (node == nullptr)
	{
		return true;
	}

	const bool leftIsLess = node->left == nullptr || !less(node->value, node->left->value);
	const bool rightIsGreater = node->right == nullptr || !less(node->right->value, node->value);

	return
		leftIsLess &&
		rightIsGreater &&
		isBinarySearchTreeImpl(node->left.get(), less) &&
		isBinarySearchTreeImpl(node->right.get(), less);
}

TEST_DEF(isBinarySearchTree)
{
	return isBinarySearchTreeImpl(tree.m_root.get(), tree.m_less);
}

template<typename T>
bool allPointersAreValidImpl(const Node<T>* node)
{
	if (node == nullptr)
	{
		return true;
	}

	return
		(node->left == nullptr || node->left->parent == node) &&
		(node->right == nullptr || node->right->parent == node) &&
		allPointersAreValidImpl(node->left.get()) &&
		allPointersAreValidImpl(node->right.get());
}

TEST_DEF(allPointersAreValid)
{
	return allPointersAreValidImpl(tree.m_root.get());
}

TEST_DEF(rootIsBlack)
{
	return tree.m_root != nullptr && tree.m_root->color == Color::Black;
}

template<typename T>
bool bothChildrenOfRedAreBlackImpl(const Node<T>* node)
{
	if (node == nullptr)
	{
		return true;
	}

	return
		(node->color == Color::Black ||
			(node->left == nullptr || node->left->color == Color::Black) &&
			(node->right == nullptr || node->right->color == Color::Black)) &&
		bothChildrenOfRedAreBlackImpl(node->left.get()) &&
		bothChildrenOfRedAreBlackImpl(node->right.get());
}

TEST_DEF(bothChildrenOfRedAreBlack)
{
	return bothChildrenOfRedAreBlackImpl(tree.m_root.get());
}

template<typename T> 
std::pair<bool, std::size_t> blackLengthIsCorrectForEveryNodeImpl(const Node<T>* node, std::size_t blackLength)
{
	if (node == nullptr)
	{
		return { true, blackLength + 1 }; // sheets always have black color
	}

	const std::size_t thisNodeLength = node->color == Color::Black ? 1 : 0;

	const auto[leftEqual, leftLength] = blackLengthIsCorrectForEveryNodeImpl(node->left.get(), blackLength + thisNodeLength);
	if (!leftEqual)
	{
		return { false, leftLength };
	}

	const auto[rightEqual, rightLength] = blackLengthIsCorrectForEveryNodeImpl(node->right.get(), blackLength + thisNodeLength);

	return { leftEqual && rightEqual && leftLength == rightLength, leftLength };
}

TEST_DEF(blackLengthIsCorrectForEveryNode)
{
	return blackLengthIsCorrectForEveryNodeImpl(tree.m_root.get(), 1).first;
}


#undef TEST_DEF