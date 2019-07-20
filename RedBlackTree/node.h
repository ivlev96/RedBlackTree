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
	Node(const T& value, const Color& color, Node* parent = nullptr)
		: value(value)
		, color(color)
		, left(nullptr)
		, right(nullptr)
		, parent(parent)
	{

	}

	rapidjson::Document toJson() const
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


public:
	T value;
	Color color;

	std::unique_ptr<Node> left;
	std::unique_ptr<Node> right;
	Node* parent;
};