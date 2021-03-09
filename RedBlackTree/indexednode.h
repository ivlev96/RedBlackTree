#pragma once
#include "node.h"

template<typename T>
struct IndexedNode final : public Node<T>
{
public:
    using Node();

    std::unique_ptr<IndexedNode> copy( IndexedNode* parent = nullptr ) const;

    virtual rapidjson::Document toJson() const override;

public:
    size_t leftCount = 0;
};

template<typename T>
rapidjson::Document IndexedNode<T>::toJson() const
{
    auto doc = Node<T>::toJson();
    auto& allocator = doc.GetAllocator();

    rapidjson::Value jsonValue;
    jsonValue.Set( leftCount );

    doc.AddMember( "leftCount", jsonValue, allocator );

    return doc;
}

template<typename T>
std::unique_ptr<IndexedNode<T>> IndexedNode<T>::copy( IndexedNode<T>* parent /*= nullptr */ ) const
{
    auto copyOfThis = std::make_unique<IndexedNode<T>>( value, color, parentNode );
    copyOfThis->left = left == nullptr ? nullptr : left->copy( copyOfThis.get() );
    copyOfThis->right = right == nullptr ? nullptr : right->copy( copyOfThis.get() );

    copyOfThis->leftCount = leftCount;

    return copyOfThis;
}
