#pragma once
#include "node.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"

template<typename T, typename Less = std::less<T>>
class RedBlackTree
{
public:
	friend class RedBlackTreeTest;

public:
	RedBlackTree();
	RedBlackTree(const std::initializer_list<T>& values);

	template<typename IterType>
	RedBlackTree(const IterType& begin, const IterType& end);

	RedBlackTree(RedBlackTree<T, Less>&& other);
	RedBlackTree(const RedBlackTree<T, Less>& other) = delete;

	std::size_t size() const;

	void insert(const T& value);

	void clear();

	bool operator==(const RedBlackTree<T, Less>& other);

	std::string serialize(bool compact = false) const;

private:
	Less m_less;
	std::unique_ptr<Node<T>> m_root;
	std::size_t m_size;
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
inline RedBlackTree<T, Less>::RedBlackTree(RedBlackTree<T, Less>&& other)
	: m_less(std::move(other.m_less))
	, m_size(std::move(other.m_size))
	, m_root(std::move(other.m_root))
{
}

template<typename T, typename Less>
inline std::size_t RedBlackTree<T, Less>::size() const
{
	return m_size;
}

template<typename T, typename Less>
inline void RedBlackTree<T, Less>::insert(const T& value)
{
	++m_size;
	if (m_root == nullptr)
	{
		m_root = std::make_unique<Node<T>>(value, Color::Black);
		return;
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
		else
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
	}
	//current points to new inserted Node
}

template<typename T, typename Less>
inline void RedBlackTree<T, Less>::clear()
{
	m_root.reset();
	m_size = 0;
}

template<typename T, typename Less>
inline bool RedBlackTree<T, Less>::operator==(const RedBlackTree<T, Less>& other)
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
