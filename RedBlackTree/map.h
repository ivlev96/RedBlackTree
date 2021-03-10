#pragma once

#include "redblacktree.h"


template<typename T1, typename T2>
rapidjson::Document toJson( const std::pair<T1, T2>& val )
{
    rapidjson::Document doc;
    auto& allocator = doc.GetAllocator();

    doc.SetObject();

    rapidjson::Value jsonValue;

    jsonValue.CopyFrom( toJson( val.first ), allocator );
    doc.AddMember( "first", jsonValue, allocator );

    jsonValue.CopyFrom( toJson( val.second ), allocator );
    doc.AddMember( "second", jsonValue, allocator );

    return doc;
}

template<typename KeyType, typename ValueType, typename Less>
struct PairComparer
{
    bool operator()( const std::pair<const KeyType, ValueType>& left, const std::pair<const KeyType, ValueType>& right ) const
    {
        return less( left.first, right.first );
    }

    Less less;
};

template<typename KeyType, typename ValueType, typename Less = std::less<const KeyType>>
class Map : public RedBlackTree<std::pair<const KeyType, ValueType>, PairComparer<KeyType, ValueType, Less>>
{
    using RedBlackTreeParent = RedBlackTree<std::pair<const KeyType, ValueType>, PairComparer<KeyType, ValueType, Less>>;
public:
    Map();
    Map( const std::initializer_list<std::pair<const KeyType, ValueType>>& values );

    template<typename IterType>
    Map( const IterType& begin, const IterType& end );

    Map( const Map<KeyType, ValueType, Less>& other );
    Map( Map<KeyType, ValueType, Less>&& other );

    Map& operator=( const Map<KeyType, ValueType, Less>& other );
    Map& operator=( Map<KeyType, ValueType, Less>&& other );

    ValueType& operator[]( const KeyType& key );
    const ValueType& operator[]( const KeyType& key ) const;

    const ValueType& at( const KeyType& key ) const;

    bool operator==( const Map& other ) const;
    bool operator!=( const Map& other ) const;

};

template<typename KeyType, typename ValueType, typename Less>
bool Map<KeyType, ValueType, Less>::operator!=( const Map& other ) const
{
    return !( *this == other );
}

template<typename KeyType, typename ValueType, typename Less>
bool Map<KeyType, ValueType, Less>::operator==( const Map& other ) const
{
    return std::equal( begin(), end(), other.begin(), other.end() );
}

template<typename KeyType, typename ValueType, typename Less>
inline Map<KeyType, ValueType, Less>::Map()
    : RedBlackTreeParent()
{
}

template<typename KeyType, typename ValueType, typename Less>
inline Map<KeyType, ValueType, Less>::Map( const std::initializer_list<std::pair<const KeyType, ValueType>>& values )
    : Map( std::cbegin( values ), std::cend( values ) )
{
}

template<typename KeyType, typename ValueType, typename Less>
template<typename IterType>
inline Map<KeyType, ValueType, Less>::Map( const IterType& begin, const IterType& end )
    : RedBlackTreeParent( begin, end )
{
}

template<typename KeyType, typename ValueType, typename Less>
inline Map<KeyType, ValueType, Less>::Map( const Map<KeyType, ValueType, Less>& other )
{
}

template<typename KeyType, typename ValueType, typename Less>
inline Map<KeyType, ValueType, Less>::Map( Map<KeyType, ValueType, Less>&& other )
    : RedBlackTree( other )
{
}

template<typename KeyType, typename ValueType, typename Less>
inline Map<KeyType, ValueType, Less>& Map<KeyType, ValueType, Less>::operator=( const Map<KeyType, ValueType, Less>& other )
{
    return RedBlackTree::operator=( other );
}

template<typename KeyType, typename ValueType, typename Less>
inline Map<KeyType, ValueType, Less>& Map<KeyType, ValueType, Less>::operator=( Map<KeyType, ValueType, Less>&& other )
{
    return RedBlackTree::operator=( other );
}

template<typename KeyType, typename ValueType, typename Less>
inline ValueType& Map<KeyType, ValueType, Less>::operator[]( const KeyType& key )
{
    auto it = find( { key, {} } );
    if ( it == cend() )
    {
        it = insert( { key, {} } );
    }
    return it->second;
}

template<typename KeyType, typename ValueType, typename Less>
inline const ValueType& Map<KeyType, ValueType, Less>::operator[]( const KeyType& key ) const
{
    const auto it = find( { key, {} } );
    if ( it == cend() )
    {
        throw std::out_of_range( "invalid map<K, T> key" );
    }
    return it->second;
}

template<typename KeyType, typename ValueType, typename Less>
inline const ValueType& Map<KeyType, ValueType, Less>::at( const KeyType& key ) const
{
    return operator[]( key );
}
