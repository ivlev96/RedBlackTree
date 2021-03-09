#pragma once
#include "IndexedNode.h"

template<typename T, typename Less = std::less<T>>
class IndexedRedBlackTree
{
private:
    class ConstIterator;

public:
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using iterator = ConstIterator;
    using const_iterator = ConstIterator;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

public:
    IndexedRedBlackTree();
    IndexedRedBlackTree( const std::initializer_list<T>& values );

    template<typename IterType>
    IndexedRedBlackTree( const IterType& begin, const IterType& end );

    IndexedRedBlackTree( const IndexedRedBlackTree<T, Less>& other );
    IndexedRedBlackTree( IndexedRedBlackTree<T, Less>&& other );

    IndexedRedBlackTree& operator=( const IndexedRedBlackTree<T, Less>& other );
    IndexedRedBlackTree& operator=( IndexedRedBlackTree<T, Less>&& other );

    std::size_t size() const;

    const_iterator insert( const T& value );

    void clear();

    bool operator==( const IndexedRedBlackTree<T, Less>& other ) const;

    iterator begin() const;
    iterator end() const;

    const_iterator cbegin() const;
    const_iterator cend() const;

    reverse_iterator rbegin() const;
    reverse_iterator rend() const;

    const_reverse_iterator crbegin() const;
    const_reverse_iterator crend() const;

    const_iterator find( const T& value ) const;
    iterator find( const T& value );

    iterator erase( const T& value );
    iterator erase( const iterator& where );

    std::string serialize( bool compact = false ) const;

private:
    void rotateLeft_( std::unique_ptr<IndexedNode<T>>& node );
    void rotateRight_( std::unique_ptr<IndexedNode<T>>& node );

    IndexedNode<T>* insertAsBST_( const T& value );
    void fixAfterInsert_( IndexedNode<T>* insertedNode );
    void fixAfterErase_( IndexedNode<T>* parent, bool removedNodeIsLeft );

    std::unique_ptr<IndexedNode<T>>& getStorage_( IndexedNode<T>& node );

private:
    Less m_less;
    std::unique_ptr<IndexedNode<T>> m_root;
    std::size_t m_size;

private:
    class ConstIterator
    {
        friend class IndexedRedBlackTree;
    public:
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using iterator_category = std::bidirectional_iterator_tag;
        using const_pointer = const T*;

    public:
        ConstIterator( IndexedNode<T>* const root, IndexedNode<T>* IndexedNode = nullptr );
        ConstIterator( const ConstIterator& other );
        ConstIterator& operator=( const ConstIterator& other );

        value_type operator*() const;
        reference operator*();
        const_pointer operator->() const;
        pointer operator->();

        bool operator==( const ConstIterator& other ) const;
        bool operator!=( const ConstIterator& other ) const;

        ConstIterator& operator++();
        ConstIterator operator++( int );

        ConstIterator& operator--();
        ConstIterator operator--( int );

    private:
        IndexedNode<T>* next_( IndexedNode<T>* node ) const;
        IndexedNode<T>* prev_( IndexedNode<T>* node ) const;

    private:
        IndexedNode<T>* m_node;
        IndexedNode<T>* m_root;
    };
};

template<typename T, typename Less>
inline IndexedRedBlackTree<T, Less>::IndexedRedBlackTree()
    : m_root{ nullptr }
    , m_less{}
    , m_size{ 0 }
{
}

template<typename T, typename Less>
template<typename IterType>
inline IndexedRedBlackTree<T, Less>::IndexedRedBlackTree( const IterType& begin, const IterType& end )
    : m_size{ 0 }
{
    static_assert( std::is_same_v<decltype( *begin ), T&> || std::is_same_v<decltype( *begin ), const T&> );
    for ( auto it = begin; it != end; it = std::next( it ) )
    {
        insert( *it );
    }
}

template<typename T, typename Less>
inline IndexedRedBlackTree<T, Less>::IndexedRedBlackTree( const std::initializer_list<T>& values )
    : IndexedRedBlackTree( std::cbegin( values ), std::cend( values ) )
{
}


template<typename T, typename Less>
inline IndexedRedBlackTree<T, Less>::IndexedRedBlackTree( const IndexedRedBlackTree<T, Less>& other )
    : m_root{ other.m_root->copy() }
    , m_size{ other.m_size }
    , m_less{ other.m_less }
{

}

template<typename T, typename Less>
inline IndexedRedBlackTree<T, Less>::IndexedRedBlackTree( IndexedRedBlackTree<T, Less>&& other )
    : m_root{ std::move( other.m_root ) }
    , m_size{ std::move( other.m_size ) }
    , m_less{ std::move( other.m_less ) }
{
    other.clear();
}

template<typename T, typename Less>
inline IndexedRedBlackTree<T, Less>& IndexedRedBlackTree<T, Less>::operator=( const IndexedRedBlackTree<T, Less>& other )
{
    if ( this == &other )
    {
        return *this;
    }
    clear();
    m_root = other.m_root->copy();
    m_size = other.m_size;
    m_less = other.m_less;

    return *this;
}

template<typename T, typename Less>
inline IndexedRedBlackTree<T, Less>& IndexedRedBlackTree<T, Less>::operator=( IndexedRedBlackTree<T, Less>&& other )
{
    if ( this == &other )
    {
        return *this;
    }
    clear();
    m_root = std::move( other.m_root );
    m_size = std::move( other.m_size );
    m_less = std::move( other.m_less );
    other.clear();

    return *this;
}

template<typename T, typename Less>
inline std::size_t IndexedRedBlackTree<T, Less>::size() const
{
    return m_size;
}

template<typename T, typename Less>
inline typename IndexedRedBlackTree<T, Less>::const_iterator IndexedRedBlackTree<T, Less>::insert( const T& value )
{
    auto insertedNode = insertAsBST_( value );

    if ( insertedNode != nullptr )
    {
        ++m_size;
        fixAfterInsert_( insertedNode );
    }

    return { m_root.get(), insertedNode };
}

template<typename T, typename Less>
inline void IndexedRedBlackTree<T, Less>::clear()
{
    m_root.reset();
    m_size = 0;
}

template<typename T, typename Less>
inline bool IndexedRedBlackTree<T, Less>::operator==( const IndexedRedBlackTree<T, Less>& other ) const
{
    if ( size() != other.size() )
    {
        return false;
    }

    if ( size() == 0 )
    {
        return true;
    }

    return *m_root == *other.m_root;
}

template<typename T, typename Less>
inline typename IndexedRedBlackTree<T, Less>::iterator IndexedRedBlackTree<T, Less>::begin() const
{
    IndexedNode<T>* current = m_root.get();
    if ( current == nullptr )
    {
        return { m_root.get() };
    }

    while ( current->left != nullptr )
    {
        current = current->left.get();
    }

    return { m_root.get(), current };
}

template<typename T, typename Less>
inline typename IndexedRedBlackTree<T, Less>::iterator IndexedRedBlackTree<T, Less>::end() const
{
    return { m_root.get() };
}

template<typename T, typename Less>
inline typename IndexedRedBlackTree<T, Less>::const_iterator IndexedRedBlackTree<T, Less>::cbegin() const
{
    return begin();
}

template<typename T, typename Less>
inline typename IndexedRedBlackTree<T, Less>::const_iterator IndexedRedBlackTree<T, Less>::cend() const
{
    return end();
}

template<typename T, typename Less>
inline typename IndexedRedBlackTree<T, Less>::reverse_iterator IndexedRedBlackTree<T, Less>::rbegin() const
{
    return end();
}

template<typename T, typename Less>
inline typename IndexedRedBlackTree<T, Less>::reverse_iterator IndexedRedBlackTree<T, Less>::rend() const
{
    return begin();
}

template<typename T, typename Less>
inline typename IndexedRedBlackTree<T, Less>::const_reverse_iterator IndexedRedBlackTree<T, Less>::crbegin() const
{
    return IndexedRedBlackTree<T, Less>::const_reverse_iterator{ cend() };
}

template<typename T, typename Less>
inline typename IndexedRedBlackTree<T, Less>::const_reverse_iterator IndexedRedBlackTree<T, Less>::crend() const
{
    return IndexedRedBlackTree<T, Less>::const_reverse_iterator{ cbegin() };
}

template<typename T, typename Less>
inline typename IndexedRedBlackTree<T, Less>::const_iterator IndexedRedBlackTree<T, Less>::find( const T& value ) const
{
    auto current = m_root.get();

    while ( current != nullptr )
    {
        if ( m_less( value, current->value ) )
        {
            current = current->left.get();
        }
        else if ( m_less( current->value, value ) )
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


template<typename T, typename Less /*= std::less<T>*/>
inline typename IndexedRedBlackTree<T, Less>::iterator IndexedRedBlackTree<T, Less>::find( const T& value )
{
    auto current = m_root.get();

    while ( current != nullptr )
    {
        if ( m_less( value, current->value ) )
        {
            current = current->left.get();
        }
        else if ( m_less( current->value, value ) )
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
inline typename IndexedRedBlackTree<T, Less>::iterator IndexedRedBlackTree<T, Less>::erase( const T& value )
{
    return erase( find( value ) );
}

template<typename T, typename Less>
inline typename IndexedRedBlackTree<T, Less>::iterator IndexedRedBlackTree<T, Less>::erase( const iterator& where )
{
    if ( where == end() )
    {
        return end();
    }
    --m_size;
    const auto node = where.m_node;
    iterator next = std::next( where ); //next will be return value

    auto current = node;

    {
        auto parent = node->parent;

        while ( parent )
        {
            if ( current == parent->left.get() )
            {
                --parent->leftCount;
            }
            current = parent;
            parent = current->parent;
        }

        current = node;
    }

    if ( node->left != nullptr )
    {
        --node->leftCount;
        current = node->left.get();
        while ( current->right != nullptr )
        {
            current = current->right.get();
        }
        //current is the rightmost child of left node's child

    }
    else if ( node->right != nullptr )
    {
        current = node->right.get();
        while ( current->left != nullptr )
        {
            --current->leftCount;
            current = current->left.get();
        }
        //current is the leftmost child of right node's child
        next = where;
    }

    node->value = std::move( current->value );
    auto currentsParent = current->parent;
    const bool removedNodeIsLeft = currentsParent == nullptr ? true :
        current == currentsParent->left.get();
    decltype( auto ) currentUnique = getStorage_( *current );

    if ( current->color == Color::Red )
    {
        //Current's color is Red and it has maximum one child and color of this child must be Black.
        //So, both child of current are null (because of equal blackLength for current node).
        ASSERT_NULL( current->left );
        ASSERT_NULL( current->right );
        currentUnique.reset();
        return next;
    }
    //current->color == Color::Black

    std::unique_ptr<IndexedNode<T>> currentsChildUnique =
        current->left == nullptr ?
        std::move( current->right ) :
        std::move( current->left );

    if ( currentsChildUnique != nullptr && currentsChildUnique->color == Color::Red )
    {
        currentsChildUnique->color = Color::Black;
        currentUnique = std::move( currentsChildUnique );
        currentUnique->parent = currentsParent;
        return next;
    }
    //currentsChild == nullptr because of equal blackLength for current node.
    //So, current is a Black leaf
    ASSERT_NULL( current->left );
    ASSERT_NULL( current->right );

    currentUnique.reset();

    fixAfterErase_( currentsParent, removedNodeIsLeft );
    return next;
}

template<typename T, typename Less>
inline std::string IndexedRedBlackTree<T, Less>::serialize( bool compact ) const
{
    if ( !m_root )
    {
        return "Null";
    }

    auto doc = m_root->toJson();
    rapidjson::StringBuffer buffer;

    if ( compact )
    {
        rapidjson::Writer<rapidjson::StringBuffer> writer( buffer );
        doc.Accept( writer );
    }
    else
    {
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer( buffer );
        doc.Accept( writer );
    }

    return buffer.GetString();
}

template<typename T, typename Less>
inline void IndexedRedBlackTree<T, Less>::rotateLeft_( std::unique_ptr<IndexedNode<T>>& node )
{
    ASSERT_NOT_NULL( node->right );
    if ( node->right == nullptr )
    {
        return;
    }

    node->right->parent = node->parent;

    std::unique_ptr<IndexedNode<T>> rightNode = std::move( node->right );

    node->right = std::move( rightNode->left );
    if ( node->right != nullptr )
    {
        node->right->parent = node.get();
    }

    rightNode->leftCount += node->leftCount + 1;

    rightNode->left = std::move( node );
    rightNode->left->parent = rightNode.get();

    node = std::move( rightNode );
}


template<typename T, typename Less>
inline void IndexedRedBlackTree<T, Less>::rotateRight_( std::unique_ptr<IndexedNode<T>>& node )
{
    ASSERT_NOT_NULL( node->left );
    if ( node->left == nullptr )
    {
        return;
    }

    node->left->parent = node->parent;
    std::unique_ptr<IndexedNode<T>> leftNode = std::move( node->left );

    node->left = std::move( leftNode->right );
    if ( node->left != nullptr )
    {
        node->left->parent = node.get();
    }

    node->leftCount -= leftNode->leftCount + 1;

    leftNode->right = std::move( node );
    leftNode->right->parent = leftNode.get();

    node = std::move( leftNode );
}

template<typename T, typename Less>
inline IndexedNode<T>* IndexedRedBlackTree<T, Less>::insertAsBST_( const T& value )
{
    if ( m_root == nullptr )
    {
        m_root = std::make_unique<IndexedNode<T>>( value, Color::Black );
        return m_root.get();
    }

    auto* current = m_root.get();
    while ( true )
    {
        if ( m_less( value, current->value ) )
        {
            ++current->leftCount;
            if ( current->left == nullptr )
            {
                current->left = std::make_unique<IndexedNode<T>>( value, Color::Red, current );
                current = current->left.get();
                break;
            }
            else
            {
                current = current->left.get();
            }
        }
        else if ( m_less( current->value, value ) )
        {
            if ( current->right == nullptr )
            {
                current->right = std::make_unique<IndexedNode<T>>( value, Color::Red, current );
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
inline void IndexedRedBlackTree<T, Less>::fixAfterInsert_( IndexedNode<T>* insertedNode )
{
    if ( insertedNode->parent == nullptr )
    {
        insertedNode->color = Color::Black;
        return;
    }

    if ( insertedNode->parent->color == Color::Black )
    {
        return;
    }

    //parent is red
    auto parent = insertedNode->parent;
    auto grandParent = parent->parent;
    auto uncle = grandParent->left.get() == parent ? grandParent->right.get() : grandParent->left.get();

    if ( uncle != nullptr && uncle->color == Color::Red )
    {
        parent->color = uncle->color = Color::Black;
        grandParent->color = Color::Red;
        fixAfterInsert_( grandParent );
        return;
    }

    //uncle has black color or it is nullptr
    if ( insertedNode == parent->right.get() && parent == grandParent->left.get() )
    {
        rotateLeft_( grandParent->left );
        parent = insertedNode;
        insertedNode = insertedNode->left.get();
    }
    else if ( insertedNode == parent->left.get() && parent == grandParent->right.get() )
    {
        rotateRight_( grandParent->right );
        parent = insertedNode;
        insertedNode = insertedNode->right.get();
    }

    //now parent is LEFT child of grandParent and insertedNode is LEFT child of parent
    //or parent is RIGHT child of grandParent and insertedNode is RIGHT child of parent

    parent->color = Color::Black;
    grandParent->color = Color::Red;

    if ( insertedNode == parent->left.get() && parent == grandParent->left.get() )
    {
        if ( grandParent->parent == nullptr )
        {
            rotateRight_( m_root );
        }
        else if ( grandParent->parent->right.get() == grandParent )
        {
            rotateRight_( grandParent->parent->right );
        }
        else
        {
            rotateRight_( grandParent->parent->left );
        }
    }
    else if ( insertedNode == parent->right.get() && parent == grandParent->right.get() )
    {
        if ( grandParent->parent == nullptr )
        {
            rotateLeft_( m_root );
        }
        else if ( grandParent->parent->right.get() == grandParent )
        {
            rotateLeft_( grandParent->parent->right );
        }
        else
        {
            rotateLeft_( grandParent->parent->left );
        }
    }
    else
    {
        ASSERT( !"Invalid case" );
    }
}

template<typename T, typename Less>
inline void IndexedRedBlackTree<T, Less>::fixAfterErase_( IndexedNode<T>* parent, bool removedNodeIsLeft )
{
    if ( parent == nullptr )
    {
        //node is new root
        return;
    }

    auto sibling = removedNodeIsLeft ?
        parent->right.get() : parent->left.get();
    ASSERT_NOT_NULL( sibling ); //sibling must not be nullptr, because of equal blackLength for parent

    if ( sibling->color == Color::Red )
    {
        parent->color = Color::Red;
        sibling->color = Color::Black;

        if ( removedNodeIsLeft )
        {
            rotateLeft_( getStorage_( *parent ) );
        }
        else
        {
            rotateRight_( getStorage_( *parent ) );
        }
    }
    //now sibling of currentsChild is Black, node is still child of parent,
    //but sibling could changed

    sibling = removedNodeIsLeft ?
        parent->right.get() : parent->left.get();
    ASSERT_NOT_NULL( sibling ); //sibling must not be nullptr, because of equal blackLength for parent

    if ( parent->color == Color::Black &&
        ( sibling->left == nullptr || sibling->left->color == Color::Black ) &&
        ( sibling->right == nullptr || sibling->right->color == Color::Black ) )
    {
        //If all this nodes are black, then we should set sibling's color to Red 
        //to set correct blackLength for parent, but it leads to invalidate
        //blackLength for parent's parents. So, we must fix parent node.
        sibling->color = Color::Red;
        const bool parentIsLeft = parent->parent == nullptr ? true :
            parent == parent->parent->left.get();
        fixAfterErase_( parent->parent, parentIsLeft );
        return;
    }

    if ( parent->color == Color::Red && //in previous case was checked that parent is Black
        ( sibling->left == nullptr || sibling->left->color == Color::Black ) &&
        ( sibling->right == nullptr || sibling->right->color == Color::Black ) )
    {
        //If parent is Red, then turning it to Black increases blackLength to all ways,
        //which are going through node (we want to do it in this function).
        //And it also doesn't changes blackLength of all ways,
        //which are going through sibling. This result satisfies us.
        sibling->color = Color::Red;
        parent->color = Color::Black;
        return;
    }
    //at least one of sibling's children is Red

    //We want to ensure that sibling is right child and it's right child is Red
    //or sibling is left child and it's left child is Red

    if ( sibling == parent->right.get() )
    {
        if ( sibling->right == nullptr || sibling->right->color == Color::Black )
        {
            sibling->color = Color::Red;
            if ( sibling->left )
            {
                sibling->left->color = Color::Black;
            }
            rotateRight_( getStorage_( *sibling ) );
        }
    }
    else
    {
        if ( sibling->left == nullptr || sibling->left->color == Color::Black )
        {
            sibling->color = Color::Red;
            if ( sibling->right )
            {
                sibling->right->color = Color::Black;
            }
            rotateLeft_( getStorage_( *sibling ) );
        }
    }
    //Now sibling is right child and it's right child is Red
    //or sibling is left child and it's left child is Red

    sibling = removedNodeIsLeft ?
        parent->right.get() : parent->left.get();
    ASSERT_NOT_NULL( sibling ); //sibling must not be nullptr, because of equal blackLength for parent

    sibling->color = parent->color;
    parent->color = Color::Black;

    if ( sibling == parent->right.get() )
    {
        sibling->right->color = Color::Black;
        rotateLeft_( getStorage_( *parent ) );
    }
    else
    {
        sibling->left->color = Color::Black;
        rotateRight_( getStorage_( *parent ) );
    }
}

template<typename T, typename Less>
inline std::unique_ptr<IndexedNode<T>>& IndexedRedBlackTree<T, Less>::getStorage_( IndexedNode<T>& node )
{
    if ( node.parent == nullptr )
    {
        return m_root;
    }

    if ( &node == node.parent->left.get() )
    {
        return node.parent->left;
    }

    return node.parent->right;
}

template<typename T, typename Less>
inline IndexedRedBlackTree<T, Less>::ConstIterator::ConstIterator( IndexedNode<T>* const root, IndexedNode<T>* node )
    : m_root( root )
    , m_node( node )
{

}

template<typename T, typename Less>
inline IndexedRedBlackTree<T, Less>::ConstIterator::ConstIterator( const ConstIterator& other )
    : m_root( other.m_root )
    , m_node( other.m_node )
{
}

template<typename T, typename Less>
inline typename IndexedRedBlackTree<T, Less>::ConstIterator& IndexedRedBlackTree<T, Less>::ConstIterator::operator=( const ConstIterator& other )
{
    m_root = other.m_root;
    m_node = other.m_node;

    return *this;
}

template<typename T, typename Less>
inline typename IndexedRedBlackTree<T, Less>::ConstIterator::value_type IndexedRedBlackTree<T, Less>::ConstIterator::operator*() const
{
    if ( m_node == nullptr )
    {
        throw std::out_of_range( "Attempt to dereference end() iterator" );
    }
    return m_node->value;
}

template<typename T, typename Less>
inline typename IndexedRedBlackTree<T, Less>::ConstIterator::reference IndexedRedBlackTree<T, Less>::ConstIterator::operator*()
{
    if ( m_node == nullptr )
    {
        throw std::out_of_range( "Attempt to dereference end() iterator" );
    }
    return m_node->value;
}

template<typename T, typename Less>
inline typename IndexedRedBlackTree<T, Less>::ConstIterator::const_pointer IndexedRedBlackTree<T, Less>::ConstIterator::operator->() const
{
    if ( m_node == nullptr )
    {
        throw std::out_of_range( "Attempt to dereference end() iterator" );
    }
    return &( m_node->value );
}

template<typename T, typename Less>
inline typename IndexedRedBlackTree<T, Less>::ConstIterator::pointer IndexedRedBlackTree<T, Less>::ConstIterator::operator->()
{
    if ( m_node == nullptr )
    {
        throw std::out_of_range( "Attempt to dereference end() iterator" );
    }
    return &( m_node->value );
}

template<typename T, typename Less>
inline bool IndexedRedBlackTree<T, Less>::ConstIterator::operator==( const IndexedRedBlackTree<T, Less>::ConstIterator& other ) const
{
    return m_root == other.m_root && m_node == other.m_node;
}

template<typename T, typename Less>
inline bool IndexedRedBlackTree<T, Less>::ConstIterator::operator!=( const IndexedRedBlackTree<T, Less>::ConstIterator& other ) const
{
    return !( *this == other );
}

template<typename T, typename Less>
inline typename IndexedRedBlackTree<T, Less>::ConstIterator& IndexedRedBlackTree<T, Less>::ConstIterator::operator++()
{
    m_node = next_( m_node );
    return *this;
}

template<typename T, typename Less>
inline typename IndexedRedBlackTree<T, Less>::ConstIterator IndexedRedBlackTree<T, Less>::ConstIterator::operator++( int )
{
    auto copy = *this;
    m_node = next_( m_node );
    return copy;
}

template<typename T, typename Less>
inline typename IndexedRedBlackTree<T, Less>::ConstIterator& IndexedRedBlackTree<T, Less>::ConstIterator::operator--()
{
    m_node = prev_( m_node );
    return *this;
}

template<typename T, typename Less>
inline typename IndexedRedBlackTree<T, Less>::ConstIterator IndexedRedBlackTree<T, Less>::ConstIterator::operator--( int )
{
    auto copy = *this;
    m_node = prev_( m_node );
    return copy;
}

template<typename T, typename Less>
inline IndexedNode<T>* IndexedRedBlackTree<T, Less>::ConstIterator::next_( IndexedNode<T>* node ) const
{
    IndexedNode<T>* nextNode = nullptr;

    if ( node == nullptr )
    {
        throw std::out_of_range( "Can not increment end() iterator" );
        return nullptr;
    }

    if ( node->right != nullptr )
    {
        nextNode = node->right.get();
        while ( nextNode->left != nullptr )
        {
            nextNode = nextNode->left.get();
        }

        return nextNode;
    }

    if ( node->parent == nullptr )
    {
        return nullptr;
    }

    if ( node == node->parent->left.get() )
    {
        return node->parent;
    }

    nextNode = node;
    while ( nextNode->parent->parent != nullptr && nextNode->parent == nextNode->parent->parent->right.get() )
    {
        nextNode = nextNode->parent;
    }

    return nextNode->parent->parent;
}

template<typename T, typename Less>
inline IndexedNode<T>* IndexedRedBlackTree<T, Less>::ConstIterator::prev_( IndexedNode<T>* node ) const
{
    IndexedNode<T>* nextNode = nullptr;

    if ( node == nullptr )
    {
        nextNode = m_root;
        while ( nextNode->right != nullptr )
        {
            nextNode = nextNode->right.get();
        }

        return nextNode;
    }

    if ( node->left != nullptr )
    {
        nextNode = node->left.get();
        while ( nextNode->right != nullptr )
        {
            nextNode = nextNode->right.get();
        }

        return nextNode;
    }

    if ( node->parent == nullptr )
    {
        return nullptr;
    }

    if ( node == node->parent->right.get() )
    {
        return node->parent;
    }

    nextNode = node;
    while ( nextNode->parent->parent != nullptr && nextNode->parent == nextNode->parent->parent->left.get() )
    {
        nextNode = nextNode->parent;
    }

    return nextNode->parent->parent;
}
