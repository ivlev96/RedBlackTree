#pragma once
#include "rapidjson/document.h"

template<typename T>
rapidjson::Value toJson( const T& val )
{
    rapidjson::Value jsonValue;
    jsonValue.Set( val );
    return jsonValue;
}

template<typename CharT, typename CharTraits = std::char_traits<CharT>>
rapidjson::Value toJson( const std::basic_string<CharT, CharTraits>& val )
{
    rapidjson::Value jsonValue;
    jsonValue.SetString( val.c_str(), static_cast<rapidjson::SizeType>( val.size() ) );
    return jsonValue;
}

enum class Color : bool
{
    Red,
    Black
};

template<typename T>
struct Node
{
public:
    Node( const T& value,
          const Color& color,
          Node* parent = nullptr );

    std::unique_ptr<Node> copy( Node<T>* parent = nullptr ) const;

    bool operator==( const Node<T>& other ) const;

    virtual rapidjson::Document toJson() const;

public:
    T value;
    Color color;

    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
    Node* parent;
};

template<typename T>
inline Node<T>::Node( const T& value,
                      const Color& color,
                      Node* parent )
    : value{ value }
    , color{ color }
    , parent{ parent }
    , left{ nullptr }
    , right{ nullptr }
{
}

template<typename T>
inline std::unique_ptr<Node<T>> Node<T>::copy( Node<T>* parentNode ) const
{
    auto copyOfThis = std::make_unique<Node<T>>( value, color, parentNode );
    copyOfThis->left = left == nullptr ? nullptr : left->copy( copyOfThis.get() );
    copyOfThis->right = right == nullptr ? nullptr : right->copy( copyOfThis.get() );

    return copyOfThis;
}

template<typename T>
inline bool Node<T>::operator==( const Node<T>& other ) const
{
    if ( value != other.value )
    {
        return false;
    }

    bool equal = false;
    if ( left == nullptr )
    {
        if ( other.left != nullptr )
        {
            return false;
        }

        equal = true;
    }
    else
    {
        if ( other.left == nullptr )
        {
            return false;
        }

        equal = *left == *other.left;
    }

    if ( !equal )
    {
        return false;
    }

    if ( right == nullptr )
    {
        if ( other.right != nullptr )
        {
            return false;
        }

        equal = true;
    }
    else
    {
        if ( other.right == nullptr )
        {
            return false;
        }

        equal = *right == *other.right;
    }

    return equal;
}

template<typename T>
inline rapidjson::Document Node<T>::toJson() const
{
    rapidjson::Document doc;
    auto& allocator = doc.GetAllocator();

    doc.SetObject();

    rapidjson::Value jsonValue;
    jsonValue.CopyFrom( ::toJson( value ), allocator );

    doc.AddMember( "value", jsonValue, allocator );

    jsonValue.SetString( color == Color::Red ? "red" : "black", allocator );
    doc.AddMember( "color", jsonValue, allocator );

    if ( left != nullptr )
    {
        jsonValue.CopyFrom( left->toJson(), allocator );
    }
    else
    {
        jsonValue.SetNull();
    }

    doc.AddMember( "left", jsonValue, allocator );

    if ( right != nullptr )
    {
        jsonValue.CopyFrom( right->toJson(), allocator );
    }
    else
    {
        jsonValue.SetNull();
    }

    doc.AddMember( "right", jsonValue, allocator );
    return doc;
}
