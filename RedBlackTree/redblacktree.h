#pragma once
#include "node.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"

template<typename T, typename Less = std::less<T>>
class RedBlackTree
{
private:
	class ConstIterator;

public:
	friend class RedBlackTreeTest;

	using value_type = T;
	using reference = T& ;
	using const_reference = const T&;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	using iterator = ConstIterator;
	using const_iterator = ConstIterator;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

public:
	RedBlackTree();
	RedBlackTree(const std::initializer_list<T>& values);

	template<typename IterType>
	RedBlackTree(const IterType& begin, const IterType& end);

	RedBlackTree(const RedBlackTree<T, Less>& other);
	RedBlackTree(RedBlackTree<T, Less>&& other);

	std::size_t size() const;

	void insert(const T& value);

	void clear();

	bool operator==(const RedBlackTree<T, Less>& other) const;

	iterator begin() const;
	iterator end() const;

	const_iterator cbegin() const;
	const_iterator cend() const;

	reverse_iterator rbegin() const;
	reverse_iterator rend() const;

	const_reverse_iterator crbegin() const;
	const_reverse_iterator crend() const;

	const_iterator find(const T& value) const;

	iterator erase(const T& value);
	iterator erase(const const_iterator& where);

	std::string serialize(bool compact = false) const;

private:
	void rotateLeft(std::unique_ptr<Node<T>>& node);
	void rotateRight(std::unique_ptr<Node<T>>& node);

	Node<T>* insertAsBST(const T& value);
	void fixAfterInsert(Node<T>* insertedNode);

private:
	Less m_less;
	std::unique_ptr<Node<T>> m_root;
	std::size_t m_size;

private:
	class ConstIterator
	{
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = const T&;
		using iterator_category = std::bidirectional_iterator_tag;

	public:
		ConstIterator(const Node<T>* const root, const Node<T>* node = nullptr);
		ConstIterator(const ConstIterator& other);
		ConstIterator& operator=(const ConstIterator& other);

		reference operator*() const;

		bool operator==(const ConstIterator& other) const;
		bool operator!=(const ConstIterator& other) const;

		ConstIterator& operator++();
		ConstIterator operator++(int);

		ConstIterator& operator--();
		ConstIterator operator--(int);

	private:
		const Node<T>* next(const Node<T>* const node) const;
		const Node<T>* prev(const Node<T>* const node) const;

	private:
		const Node<T>* m_node;
		const Node<T>* m_root;
	};
};


template<typename T, typename Less>
inline RedBlackTree<T, Less>::RedBlackTree()
	: m_root{ nullptr }
	, m_less{}
	, m_size{ 0 }
{
}

template<typename T, typename Less>
template<typename IterType>
inline RedBlackTree<T, Less>::RedBlackTree(const IterType& begin, const IterType& end)
	: m_size{ 0 }
{
	static_assert(std::is_same_v<decltype(*begin), T&> || std::is_same_v<decltype(*begin), const T&>);
	for (auto it = begin; it != end; it = std::next(it))
	{
		insert(*it);
	}
}

template<typename T, typename Less>
inline RedBlackTree<T, Less>::RedBlackTree(const std::initializer_list<T>& values)
	: RedBlackTree(std::cbegin(values), std::cend(values))
{
}


template<typename T, typename Less>
inline RedBlackTree<T, Less>::RedBlackTree(const RedBlackTree<T, Less>& other)
	: m_root{ other.m_root->copy() }
	, m_size{ other.m_size }
	, m_less{ other.m_less }
{

}

template<typename T, typename Less>
inline RedBlackTree<T, Less>::RedBlackTree(RedBlackTree<T, Less>&& other)
	: m_less{ std::move(other.m_less) }
	, m_size{ std::move(other.m_size) }
	, m_root{ std::move(other.m_root) }
{
	other.m_size = 0;
}

template<typename T, typename Less>
inline std::size_t RedBlackTree<T, Less>::size() const
{
	return m_size;
}

template<typename T, typename Less>
inline void RedBlackTree<T, Less>::insert(const T& value)
{
	auto insertedNode = insertAsBST(value);

	if (insertedNode != nullptr)
	{
		++m_size;
		fixAfterInsert(insertedNode);
	}
}

template<typename T, typename Less>
inline void RedBlackTree<T, Less>::clear()
{
	m_root.reset();
	m_size = 0;
}

template<typename T, typename Less>
inline bool RedBlackTree<T, Less>::operator==(const RedBlackTree<T, Less>& other) const
{
	if (size() != other.size())
	{
		return false;
	}

	if (size() == 0)
	{
		return true;
	}

	return *m_root == *other.m_root;
}

template<typename T, typename Less>
inline typename RedBlackTree<T, Less>::iterator RedBlackTree<T, Less>::begin() const
{
	Node<T>* current = m_root.get();
	if (current == nullptr)
	{
		return { m_root.get() };
	}

	while (current->left != nullptr)
	{
		current = current->left.get();
	}

	return { m_root.get(), current };
}

template<typename T, typename Less>
inline typename RedBlackTree<T, Less>::iterator RedBlackTree<T, Less>::end() const
{
	return { m_root.get() };
}

template<typename T, typename Less>
inline typename RedBlackTree<T, Less>::const_iterator RedBlackTree<T, Less>::cbegin() const
{
	return begin();
}

template<typename T, typename Less>
inline typename RedBlackTree<T, Less>::const_iterator RedBlackTree<T, Less>::cend() const
{
	return end();
}

template<typename T, typename Less>
inline typename RedBlackTree<T, Less>::reverse_iterator RedBlackTree<T, Less>::rbegin() const
{
	return end();
}

template<typename T, typename Less>
inline typename RedBlackTree<T, Less>::reverse_iterator RedBlackTree<T, Less>::rend() const
{
	return begin();
}

template<typename T, typename Less>
inline typename RedBlackTree<T, Less>::const_reverse_iterator RedBlackTree<T, Less>::crbegin() const
{
	return RedBlackTree<T, Less>::const_reverse_iterator{ cend() };
}

template<typename T, typename Less>
inline typename RedBlackTree<T, Less>::const_reverse_iterator RedBlackTree<T, Less>::crend() const
{
	return RedBlackTree<T, Less>::const_reverse_iterator{ cbegin() };
}

template<typename T, typename Less>
inline typename RedBlackTree<T, Less>::const_iterator RedBlackTree<T, Less>::find(const T& value) const
{
	auto current = m_root.get();

	while (current != nullptr)
	{
		if (m_less(value, current->value))
		{
			current = current->left.get();
		}
		else if (m_less(current->value, value))
		{
			current = current->right.get();
		}
		else
		{
			return { m_root.get(), current };
		}
	}
	return m_root.get();
}

template<typename T, typename Less>
inline typename RedBlackTree<T, Less>::iterator RedBlackTree<T, Less>::erase(const T& value)
{
	return erase(find(value));
}

template<typename T, typename Less>
inline typename RedBlackTree<T, Less>::iterator RedBlackTree<T, Less>::erase(const const_iterator& where)
{
	if (where == end())
	{
		return end();
	}

	return iterator(nullptr);
}

template<typename T, typename Less>
inline std::string RedBlackTree<T, Less>::serialize(bool compact) const
{
	if (!m_root)
	{
		return "Null";
	}

	auto doc = m_root->toJson();
	rapidjson::StringBuffer buffer;

	if (compact)
	{
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		doc.Accept(writer);
	}
	else
	{
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
		doc.Accept(writer);
	}

	return buffer.GetString();
}

template<typename T, typename Less>
inline void RedBlackTree<T, Less>::rotateLeft(std::unique_ptr<Node<T>>& node)
{
	ASSERT_NOT_NULL(node->right);
	if (node->right == nullptr)
	{
		return;
	}

	node->right->parent = node->parent;

	std::unique_ptr<Node<T>> rightNode = std::move(node->right);

	node->right = std::move(rightNode->left);
	if (node->right != nullptr)
	{
		node->right->parent = node.get();
	}

	rightNode->left = std::move(node);
	rightNode->left->parent = rightNode.get();

	node = std::move(rightNode);
}


template<typename T, typename Less>
inline void RedBlackTree<T, Less>::rotateRight(std::unique_ptr<Node<T>>& node)
{
	ASSERT_NOT_NULL(node->left);
	if (node->left == nullptr)
	{
		return;
	}

	node->left->parent = node->parent;
	std::unique_ptr<Node<T>> leftNode = std::move(node->left);

	node->left = std::move(leftNode->right);
	if (node->left != nullptr)
	{
		node->left->parent = node.get();
	}

	leftNode->right = std::move(node);
	leftNode->right->parent = leftNode.get();

	node = std::move(leftNode);
}

template<typename T, typename Less>
inline Node<T>* RedBlackTree<T, Less>::insertAsBST(const T& value)
{
	if (m_root == nullptr)
	{
		m_root = std::make_unique<Node<T>>(value, Color::Black);
		return m_root.get();
	}

	auto* current = m_root.get();
	while (true)
	{
		if (m_less(value, current->value))
		{
			if (current->left == nullptr)
			{
				current->left = std::make_unique<Node<T>>(value, Color::Red, current);
				current = current->left.get();
				break;
			}
			else
			{
				current = current->left.get();
			}
		}
		else if (m_less(current->value, value))
		{
			if (current->right == nullptr)
			{
				current->right = std::make_unique<Node<T>>(value, Color::Red, current);
				current = current->right.get();
				break;
			}
			else
			{
				current = current->right.get();
			}
		}
		else //current->value == value
		{
			return nullptr;
		}
	}

	return current;
}

template<typename T, typename Less>
inline void RedBlackTree<T, Less>::fixAfterInsert(Node<T>* insertedNode)
{
	if (insertedNode->parent == nullptr)
	{
		insertedNode->color = Color::Black;
		return;
	}
	
	if (insertedNode->parent->color == Color::Black)
	{
		return;
	}

	//parent is red
	auto parent = insertedNode->parent;
	auto grandParent = parent->parent;
	auto uncle = grandParent->left.get() == parent ? grandParent->right.get() : grandParent->left.get();

	if (uncle != nullptr && uncle->color == Color::Red)
	{
		parent->color = uncle->color = Color::Black;
		grandParent->color = Color::Red;
		fixAfterInsert(grandParent);
		return;
	}

	//uncle has black color or it is nullptr
	if (insertedNode == parent->right.get() && parent == grandParent->left.get())
	{
		rotateLeft(grandParent->left);
		parent = insertedNode;
		insertedNode = insertedNode->left.get();
	}
	else if (insertedNode == parent->left.get() && parent == grandParent->right.get())
	{
		rotateRight(grandParent->right);
		parent = insertedNode;
		insertedNode = insertedNode->right.get();
	}

	//now parent is LEFT child of grandParent and insertedNode is LEFT child of parent
	//or parent is RIGHT child of grandParent and insertedNode is RIGHT child of parent

	parent->color = Color::Black;
	grandParent->color = Color::Red;

	if (insertedNode == parent->left.get() && parent == grandParent->left.get())
	{
		if (grandParent->parent == nullptr)
		{
			rotateRight(m_root);
		}
		else if (grandParent->parent->right.get() == grandParent)
		{
			rotateRight(grandParent->parent->right);
		}
		else
		{
			rotateRight(grandParent->parent->left);
		}
	}
	else if (insertedNode == parent->right.get() && parent == grandParent->right.get())
	{
		if (grandParent->parent == nullptr)
		{
			rotateLeft(m_root);
		}
		else if (grandParent->parent->right.get() == grandParent)
		{
			rotateLeft(grandParent->parent->right);
		}
		else
		{
			rotateLeft(grandParent->parent->left);
		}
	}
	else
	{
		ASSERT(!"Invalid case");
	}
}

template<typename T, typename Less>
inline RedBlackTree<T, Less>::ConstIterator::ConstIterator(const Node<T>* const root, const Node<T>* node)
	: m_root(root)
	, m_node(node)	
{

}

template<typename T, typename Less>
inline RedBlackTree<T, Less>::ConstIterator::ConstIterator(const ConstIterator& other)
	: m_root(other.m_root)
	, m_node(other.m_node)
{
}

template<typename T, typename Less>
inline typename RedBlackTree<T, Less>::ConstIterator& RedBlackTree<T, Less>::ConstIterator::operator=(const ConstIterator& other)
{
	m_root = other.m_root;
	m_node = other.m_node;

	return *this;
}

template<typename T, typename Less>
inline typename RedBlackTree<T, Less>::ConstIterator::reference RedBlackTree<T, Less>::ConstIterator::operator*() const
{
	if (m_node == nullptr)
	{
		throw std::out_of_range("Attempt to dereference end() iterator");
	}
	return m_node->value;
}

template<typename T, typename Less>
inline bool RedBlackTree<T, Less>::ConstIterator::operator==(const RedBlackTree<T, Less>::ConstIterator& other) const
{
	return m_root == other.m_root && m_node == other.m_node;
}

template<typename T, typename Less>
inline bool RedBlackTree<T, Less>::ConstIterator::operator!=(const RedBlackTree<T, Less>::ConstIterator& other) const
{
	return !(*this == other);
}

template<typename T, typename Less>
inline typename RedBlackTree<T, Less>::ConstIterator& RedBlackTree<T, Less>::ConstIterator::operator++()
{
	m_node = next(m_node);
	return *this;
}

template<typename T, typename Less>
inline typename RedBlackTree<T, Less>::ConstIterator RedBlackTree<T, Less>::ConstIterator::operator++(int)
{
	auto copy = *this;
	m_node = next(m_node);
	return copy;
}

template<typename T, typename Less>
inline typename RedBlackTree<T, Less>::ConstIterator& RedBlackTree<T, Less>::ConstIterator::operator--()
{
	m_node = prev(m_node);
	return *this;
}

template<typename T, typename Less>
inline typename RedBlackTree<T, Less>::ConstIterator RedBlackTree<T, Less>::ConstIterator::operator--(int)
{
	auto copy = *this;
	m_node = prev(m_node);
	return copy;
}

template<typename T, typename Less>
inline const Node<T>* RedBlackTree<T, Less>::ConstIterator::next(const Node<T>* const node) const
{
	const Node<T>* nextNode = nullptr;

	if (node == nullptr)
	{
		throw std::out_of_range("Can not increment end() iterator");
		return nullptr;
	}

	if (node->right != nullptr)
	{
		nextNode = node->right.get();
		while (nextNode->left != nullptr)
		{
			nextNode = nextNode->left.get();
		}

		return nextNode;
	}

	if (node->parent == nullptr)
	{
		return nullptr;
	}

	if (node == node->parent->left.get())
	{
		return node->parent;
	}

	nextNode = node;
	while (nextNode->parent->parent != nullptr && nextNode->parent == nextNode->parent->parent->right.get())
	{
		nextNode = nextNode->parent;
	}

	return nextNode->parent->parent;
}

template<typename T, typename Less>
inline const Node<T>* RedBlackTree<T, Less>::ConstIterator::prev(const Node<T>* const node) const
{
	const Node<T>* nextNode = nullptr;

	if (node == nullptr)
	{
		nextNode = m_root;
		while (nextNode->right != nullptr)
		{
			nextNode = nextNode->right.get();
		}

		return nextNode;
	}

	if (node->left != nullptr)
	{
		nextNode = node->left.get();
		while (nextNode->right != nullptr)
		{
			nextNode = nextNode->right.get();
		}

		return nextNode;
	}

	if (node->parent == nullptr)
	{
		return nullptr;
	}

	if (node == node->parent->right.get())
	{
		return node->parent;
	}

	nextNode = node;
	while (nextNode->parent->parent != nullptr && nextNode->parent == nextNode->parent->parent->left.get())
	{
		nextNode = nextNode->parent;
	}

	return nextNode->parent->parent;
}
