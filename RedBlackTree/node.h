#pragma once
#include "rapidjson/document.h"

enum class Color : bool
{
	Red,
	Black
};

template<typename T>
struct Node
{
public:
	Node(const T& value, const Color& color, Node* parent = nullptr);

	bool operator==(const Node<T>& other) const;
	void rotateLeft();
	void rotateRight();

	rapidjson::Document toJson() const;

public:
	T value;
	Color color;

	std::unique_ptr<Node> left;
	std::unique_ptr<Node> right;
	Node* parent;
};

template<typename T>
inline Node<T>::Node(const T& value, const Color& color, Node* parent)
	: value(value)
	, color(color)
	, left(nullptr)
	, right(nullptr)
	, parent(parent)
{

}

template<typename T>
inline bool Node<T>::operator==(const Node<T>& other) const
{
	if (value != other.value)
	{
		return false;
	}

	bool equal = false;
	if (left == nullptr)
	{
		if (other.left != nullptr)
		{
			return false;
		}

		equal = true;
	}
	else
	{
		if (other.left == nullptr)
		{
			return false;
		}

		equal = *left == *other.left;
	}

	if (!equal)
	{
		return false;
	}

	if (right == nullptr)
	{
		if (other.right != nullptr)
		{
			return false;
		}

		equal = true;
	}
	else
	{
		if (other.right == nullptr)
		{
			return false;
		}

		equal = *right == *other.right;
	}

	return equal;
}

template<typename T>
inline void Node<T>::rotateLeft()
{
	ASSERT_NOT_NULL(right);
	if (right == nullptr)
	{
		return;
	}

	std::unique_ptr<Node> leftChildOfRight{ std::move(right->left) };

	Node* rightPtr = right.get();
	right->parent = parent;
	if (parent != nullptr)
	{
		if (parent->left.get() == this)
		{
			right->left = std::move(parent->left);
			parent->left = std::move(right);
		}
		else
		{
			right->left = std::move(parent->right);
			parent->right = std::move(right);
		}
	}
	parent = rightPtr;

	right = std::move(leftChildOfRight);
	if (right != nullptr)
	{
		right->parent = this;
	}
}

template<typename T>
inline void Node<T>::rotateRight()
{
	ASSERT_NOT_NULL(left);
	if (left == nullptr)
	{
		return;
	}

	std::unique_ptr<Node> rightChildOfLeft{ std::move(left->right) };

	Node* leftPtr = left.get();
	left->parent = parent;
	if (parent != nullptr)
	{
		if (parent->left.get() == this)
		{
			left->right = std::move(parent->left);
			parent->left = std::move(left);
		}
		else
		{
			left->right = std::move(parent->right);
			parent->right = std::move(left);
		}
	}
	parent = leftPtr;

	left = std::move(rightChildOfLeft);
	if (left != nullptr)
	{
		left->parent = this;
	}
}

template<typename T>
inline rapidjson::Document Node<T>::toJson() const
{
	rapidjson::Document doc;
	auto& allocator = doc.GetAllocator();

	doc.SetObject();

	rapidjson::Value jsonValue;
	jsonValue.Set(value);

	doc.AddMember("value", jsonValue, allocator);

	jsonValue.SetString(color == Color::Red ? "red" : "black", allocator);
	doc.AddMember("color", jsonValue, allocator);

	if (left != nullptr)
	{
		jsonValue.CopyFrom(left->toJson(), allocator);
	}
	else
	{
		jsonValue.SetNull();
	}

	doc.AddMember("left", jsonValue, allocator);

	if (right != nullptr)
	{
		jsonValue.CopyFrom(right->toJson(), allocator);
	}
	else
	{
		jsonValue.SetNull();
	}

	doc.AddMember("right", jsonValue, allocator);
	return doc;
}
