#include <redblacktreetest.h>
#include <maptest.h>


TEST( RedBlackTreeTest, ConstructorsAndAssignment )
{
    std::ofstream log( "log.txt" );
    EXPECT_TRUE( log );

    constexpr std::size_t N = 1000;
    const RedBlackTree<int> tree( createRandomTree<int>( N, log, Generator<int>( N ) ) );
    log.close();

    EXPECT_TRUE( RedBlackTreeTest::copyConstructorIsValid( tree ) );
    EXPECT_TRUE( RedBlackTreeTest::moveConstructorIsValid( tree ) );

    EXPECT_TRUE( RedBlackTreeTest::copyAssignmentIsValid( tree ) );
    EXPECT_TRUE( RedBlackTreeTest::moveAssignmentIsValid( tree ) );
}

TEST( RedBlackTreeTest, Empty )
{
    EXPECT_TRUE( RedBlackTreeTest::isEmpty( RedBlackTree<int>{} ) );
    EXPECT_TRUE( RedBlackTreeTest::isEmpty( RedBlackTree<std::string>{} ) );
    EXPECT_TRUE( RedBlackTreeTest::isEmpty( RedBlackTree<RedBlackTree<int>>{} ) );
    EXPECT_TRUE( RedBlackTreeTest::isEmpty( RedBlackTree<RedBlackTree<RedBlackTree<int>>>{} ) );
}

TEST( RedBlackTreeTest, RedBlackTree )
{
    std::ofstream log( "log.txt" );
    EXPECT_TRUE( log );

    constexpr std::size_t N = 1000;
    const RedBlackTree<int> tree( createRandomTree<int>( 6 * N, log, Generator<int>( N ) ) );
    log.close();

    EXPECT_EQ( tree.size(), N );
    EXPECT_TRUE( RedBlackTreeTest::allPointersAreValid( tree ) );
    EXPECT_TRUE( RedBlackTreeTest::isBinarySearchTree( tree ) );
    EXPECT_TRUE( RedBlackTreeTest::rootIsBlack( tree ) );
    EXPECT_TRUE( RedBlackTreeTest::bothChildrenOfRedAreBlack( tree ) );
    EXPECT_TRUE( RedBlackTreeTest::blackLengthIsCorrectForEveryNode( tree ) );
}


TEST( RedBlackTreeTest, Iterators )
{
    std::ofstream log( "log.txt" );
    EXPECT_TRUE( log );

    constexpr std::size_t N = 1000;
    const RedBlackTree<int> tree( createRandomTree<int>( N, log, Generator<int>( N ) ) );
    log.close();

    EXPECT_TRUE( RedBlackTreeTest::iteratorsAreValid( tree ) );
    EXPECT_TRUE( RedBlackTreeTest::reverseIteratorsAreValid( tree ) );
    EXPECT_TRUE( RedBlackTreeTest::findIsCorrect( tree ) );
    EXPECT_EQ( tree.find( -1 ), tree.cend() );
}

TEST( RedBlackTreeTest, Erase )
{
    std::ofstream log( "log.txt" );
    EXPECT_TRUE( log );

    constexpr std::size_t N = 1000;
    const RedBlackTree<int> tree( createRandomTree<int>( N, log, Generator<int>( N ) ) );
    log.close();

    EXPECT_TRUE( RedBlackTreeTest::eraseIsValid( tree ) );
}

TEST( RedBlackTreeTest, Serialize )
{
    EXPECT_TRUE( RedBlackTreeTest::serializeIntIsValid() );
    EXPECT_TRUE( RedBlackTreeTest::serializeStringIsValid() );
}

TEST( MapTest, Basic )
{
    EXPECT_TRUE( MapTest::basicTest() );
}


TEST( MapTest, Insert )
{
    EXPECT_TRUE( MapTest::insertTest() );
}

TEST( MapTest, Serialize )
{
    EXPECT_TRUE( MapTest::serializeTest() );
}