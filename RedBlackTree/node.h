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

template<typename T, bool IsIndexed>
struct NodeBase
{
public:
    static constexpr std::size_t isIndexed = IsIndexed;

    NodeBase( const T& value,
              const Color& color,
              NodeBase* parent = nullptr );

    std::unique_ptr<NodeBase> copy( NodeBase* parent = nullptr ) const;

    bool operator==( const NodeBase& other ) const;

    virtual rapidjson::Document toJson() const;

public:
    T value;
    Color color;
    std::size_t leftCount;

    std::unique_ptr<NodeBase> left;
    std::unique_ptr<NodeBase> right;
    NodeBase* parent;
};

template<typename T, bool IsIndexed>
inline NodeBase<T, IsIndexed>::NodeBase( const T& value,
                                         const Color& color,
                                         NodeBase* parent )
    : value{ value }
    , color{ color }
    , parent{ parent }
    , left{ nullptr }
    , right{ nullptr }
{
    if constexpr ( isIndexed )
    {
        leftCount = 0;
    }
}

template<typename T, bool IsIndexed>
inline std::unique_ptr<NodeBase<T, IsIndexed>> NodeBase<T, IsIndexed>::copy( NodeBase* parentNode ) const
{
    auto copyOfThis = std::make_unique<NodeBase>( value, color, parentNode );
    if constexpr ( isIndexed )
    {
        copyOfThis->leftCount = leftCount;
    }
    copyOfThis->left = left == nullptr ? nullptr : left->copy( copyOfThis.get() );
    copyOfThis->right = right == nullptr ? nullptr : right->copy( copyOfThis.get() );

    return copyOfThis;
}

template<typename T, bool IsIndexed>
inline bool NodeBase<T, IsIndexed>::operator==( const NodeBase& other ) const
{
    if constexpr ( isIndexed )
    {
        if ( leftCount != other.leftCount )
        {
            return false;
        }
    }

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

template<typename T, bool IsIndexed>
inline rapidjson::Document NodeBase<T, IsIndexed>::toJson() const
{
    rapidjson::Document doc;
    auto& allocator = doc.GetAllocator();

    doc.SetObject();

    rapidjson::Value jsonValue;
    jsonValue.CopyFrom( ::toJson( value ), allocator );

    doc.AddMember( "value", jsonValue, allocator );

    jsonValue.SetString( color == Color::Red ? "red" : "black", allocator );
    doc.AddMember( "color", jsonValue, allocator );

    if constexpr ( isIndexed )
    {
        jsonValue.Set( leftCount );
        doc.AddMember( "leftCount", jsonValue, allocator );
    }

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

template<typename T>
using Node = NodeBase<T, false>;

template<typename T>
using IndexedNode = NodeBase<T, true>;