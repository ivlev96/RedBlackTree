#pragma once
#include "redblacktree.h"

class RedBlackTreeTest
{
public:

#define TEST_DECL(testName) \
	template<typename T, typename Less = std::less<T>> \
	static bool testName( const RedBlackTree<T, Less>& tree )

    TEST_DECL( copyConstructorIsValid );
    TEST_DECL( moveConstructorIsValid );

    TEST_DECL( copyAssignmentIsValid );
    TEST_DECL( moveAssignmentIsValid );

    TEST_DECL( isEmpty );

    TEST_DECL( allPointersAreValid );
    TEST_DECL( isBinarySearchTree );
    TEST_DECL( rootIsBlack );
    TEST_DECL( bothChildrenOfRedAreBlack );
    TEST_DECL( blackLengthIsCorrectForEveryNode );

    TEST_DECL( isRedBlackTree );

    TEST_DECL( iteratorsAreValid );
    TEST_DECL( reverseIteratorsAreValid );

    TEST_DECL( findIsCorrect );

    TEST_DECL( eraseIsValid );

    static bool serializeIntIsValid();
    static bool serializeStringIsValid();

#undef TEST_DECL
};

#define TEST_DEF(testName) \
template<typename T, typename Less> \
inline bool RedBlackTreeTest::testName(const RedBlackTree<T, Less>& tree)

TEST_DEF( copyConstructorIsValid )
{
    RedBlackTree<T, Less> copy( tree );
    return isRedBlackTree( copy ) && tree == copy;
}

TEST_DEF( moveConstructorIsValid )
{
    RedBlackTree<T, Less> copyTree( tree );
    RedBlackTree<T, Less> moveTree( std::move( copyTree ) );

    return copyTree.size() == 0 && copyTree.m_root == nullptr &&
        isRedBlackTree( moveTree ) && tree == moveTree;
}

TEST_DEF( copyAssignmentIsValid )
{
    RedBlackTree<T, Less> copy;
    copy = tree;
    return isRedBlackTree( copy ) && tree == copy;
}

TEST_DEF( moveAssignmentIsValid )
{
    RedBlackTree<T, Less> copyTree( tree );
    RedBlackTree<T, Less> moveTree;

    moveTree = std::move( copyTree );

    return copyTree.size() == 0 && copyTree.m_root == nullptr &&
        isRedBlackTree( moveTree ) && tree == moveTree;
}

TEST_DEF( isEmpty )
{
    return tree.size() == 0 && tree.m_root == nullptr;
}

template<typename T, typename Less>
inline bool isBinarySearchTreeImpl( const Node<T>* node, const Less& less )
{
    if ( node == nullptr )
    {
        return true;
    }

    const bool leftIsLess = node->left == nullptr || less( node->left->value, node->value );
    const bool rightIsGreater = node->right == nullptr || less( node->value, node->right->value );

    return
        leftIsLess &&
        rightIsGreater &&
        isBinarySearchTreeImpl( node->left.get(), less ) &&
        isBinarySearchTreeImpl( node->right.get(), less );
}

TEST_DEF( isBinarySearchTree )
{
    return isBinarySearchTreeImpl( tree.m_root.get(), tree.m_less );
}

template<typename T>
inline bool allPointersAreValidImpl( const Node<T>* node )
{
    if ( node == nullptr )
    {
        return true;
    }

    return
        ( node->left == nullptr || node->left->parent == node ) &&
        ( node->right == nullptr || node->right->parent == node ) &&
        allPointersAreValidImpl( node->left.get() ) &&
        allPointersAreValidImpl( node->right.get() );
}

TEST_DEF( allPointersAreValid )
{
    return allPointersAreValidImpl( tree.m_root.get() );
}

TEST_DEF( rootIsBlack )
{
    return tree.m_root == nullptr || tree.m_root->color == Color::Black;
}

template<typename T>
inline bool bothChildrenOfRedAreBlackImpl( const Node<T>* node )
{
    if ( node == nullptr )
    {
        return true;
    }

    return
        ( node->color == Color::Black ||
            ( node->left == nullptr || node->left->color == Color::Black ) &&
            ( node->right == nullptr || node->right->color == Color::Black ) ) &&
        bothChildrenOfRedAreBlackImpl( node->left.get() ) &&
        bothChildrenOfRedAreBlackImpl( node->right.get() );
}

TEST_DEF( bothChildrenOfRedAreBlack )
{
    return bothChildrenOfRedAreBlackImpl( tree.m_root.get() );
}

template<typename T>
inline std::pair<bool, std::size_t> blackLengthIsCorrectForEveryNodeImpl( const Node<T>* node, std::size_t blackLength )
{
    if ( node == nullptr )
    {
        return { true, blackLength + 1 }; // sheets always have black color
    }

    const std::size_t thisNodeLength = node->color == Color::Black ? 1 : 0;

    const auto [leftEqual, leftLength] = blackLengthIsCorrectForEveryNodeImpl( node->left.get(), blackLength + thisNodeLength );
    if ( !leftEqual )
    {
        return { false, leftLength };
    }

    const auto [rightEqual, rightLength] = blackLengthIsCorrectForEveryNodeImpl( node->right.get(), blackLength + thisNodeLength );

    return { leftEqual && rightEqual && leftLength == rightLength, leftLength };
}

TEST_DEF( blackLengthIsCorrectForEveryNode )
{
    return blackLengthIsCorrectForEveryNodeImpl( tree.m_root.get(), 1 ).first;
}


TEST_DEF( isRedBlackTree )
{
    return
        allPointersAreValid( tree ) &&
        isBinarySearchTree( tree ) &&
        rootIsBlack( tree ) &&
        bothChildrenOfRedAreBlack( tree ) &&
        blackLengthIsCorrectForEveryNode( tree );
}


TEST_DEF( iteratorsAreValid )
{
    const std::vector<T> values( tree.cbegin(), tree.cend() );

    return values.size() == tree.size() &&
        std::is_sorted( values.cbegin(), values.cend(), tree.m_less );
}

TEST_DEF( reverseIteratorsAreValid )
{
    const std::vector<T> values( tree.crbegin(), tree.crend() );

    return values.size() == tree.size() &&
        std::is_sorted( values.crbegin(), values.crend(), tree.m_less );
}

TEST_DEF( findIsCorrect )
{
    for ( auto it = tree.cbegin(); it != tree.cend(); it = std::next( it ) )
    {
        auto findRes = tree.find( *it );
        if ( findRes != it || *findRes != *it )
        {
            return false;
        }
    }

    return true;
}

TEST_DEF( eraseIsValid )
{
    std::vector<T> values( tree.cbegin(), tree.cend() );

    std::random_device device;
    std::mt19937 generator( device() );

    std::shuffle( values.begin(), values.end(), generator );

    RedBlackTree<T, Less> copyTree( tree );
    std::size_t size = copyTree.size();

    for ( int value : values )
    {
        copyTree.erase( value );
        --size;

        if ( size != copyTree.size() || !isRedBlackTree( copyTree ) )
        {
            return false;
        }
    }
    return true;
}

inline bool RedBlackTreeTest::serializeIntIsValid()
{
    RedBlackTree<int> rbt;

    for ( int i = 0; i < 10; ++i )
    {
        rbt.insert( i );
    }

    const std::string test = rbt.serialize();

    const std::string ref = 
        "{\n"
        "    \"value\": 3,\n"
        "    \"color\": \"black\",\n"
        "    \"left\": {\n"
        "        \"value\": 1,\n"
        "        \"color\": \"black\",\n"
        "        \"left\": {\n"
        "            \"value\": 0,\n"
        "            \"color\": \"black\",\n"
        "            \"left\": null,\n"
        "            \"right\": null\n"
        "        },\n"
        "        \"right\": {\n"
        "            \"value\": 2,\n"
        "            \"color\": \"black\",\n"
        "            \"left\": null,\n"
        "            \"right\": null\n"
        "        }\n"
        "    },\n"
        "    \"right\": {\n"
        "        \"value\": 5,\n"
        "        \"color\": \"black\",\n"
        "        \"left\": {\n"
        "            \"value\": 4,\n"
        "            \"color\": \"black\",\n"
        "            \"left\": null,\n"
        "            \"right\": null\n"
        "        },\n"
        "        \"right\": {\n"
        "            \"value\": 7,\n"
        "            \"color\": \"red\",\n"
        "            \"left\": {\n"
        "                \"value\": 6,\n"
        "                \"color\": \"black\",\n"
        "                \"left\": null,\n"
        "                \"right\": null\n"
        "            },\n"
        "            \"right\": {\n"
        "                \"value\": 8,\n"
        "                \"color\": \"black\",\n"
        "                \"left\": null,\n"
        "                \"right\": {\n"
        "                    \"value\": 9,\n"
        "                    \"color\": \"red\",\n"
        "                    \"left\": null,\n"
        "                    \"right\": null\n"
        "                }\n"
        "            }\n"
        "        }\n"
        "    }\n"
        "}";

    return test == ref;
}

inline bool RedBlackTreeTest::serializeStringIsValid()
{
    RedBlackTree<std::string> rbt
    {
        "abba",
        "bomba",
        "xyz",
        "moloko",
        "puppy",
        "notice"
    };

    const std::string test = rbt.serialize();
    const std::string ref =
        "{\n"
        "    \"value\": \"bomba\",\n"
        "    \"color\": \"black\",\n"
        "    \"left\": {\n"
        "        \"value\": \"abba\",\n"
        "        \"color\": \"black\",\n"
        "        \"left\": null,\n"
        "        \"right\": null\n"
        "    },\n"
        "    \"right\": {\n"
        "        \"value\": \"puppy\",\n"
        "        \"color\": \"red\",\n"
        "        \"left\": {\n"
        "            \"value\": \"moloko\",\n"
        "            \"color\": \"black\",\n"
        "            \"left\": null,\n"
        "            \"right\": {\n"
        "                \"value\": \"notice\",\n"
        "                \"color\": \"red\",\n"
        "                \"left\": null,\n"
        "                \"right\": null\n"
        "            }\n"
        "        },\n"
        "        \"right\": {\n"
        "            \"value\": \"xyz\",\n"
        "            \"color\": \"black\",\n"
        "            \"left\": null,\n"
        "            \"right\": null\n"
        "        }\n"
        "    }\n"
        "}";

    return test == ref;
}

#undef TEST_DEF