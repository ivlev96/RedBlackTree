#include <indexedredblacktreetest.h>

TEST( IndexedRedBlackTreeTest, ConstructorsAndAssignment )
{
    std::ofstream log( "log.txt" );
    EXPECT_TRUE( log );

    constexpr std::size_t N = 1000;
    const IndexedRedBlackTree<int> tree( createRandomTree<int, true>( N, log, Generator<int>( N ) ) );
    log.close();

    EXPECT_TRUE( IndexedRedBlackTreeTest::copyConstructorIsValid( tree ) );
    EXPECT_TRUE( IndexedRedBlackTreeTest::moveConstructorIsValid( tree ) );

    EXPECT_TRUE( IndexedRedBlackTreeTest::copyAssignmentIsValid( tree ) );
    EXPECT_TRUE( IndexedRedBlackTreeTest::moveAssignmentIsValid( tree ) );
}

TEST( IndexedRedBlackTreeTest, Empty )
{
    EXPECT_TRUE( IndexedRedBlackTreeTest::isEmpty( IndexedRedBlackTree<int>{} ) );
    EXPECT_TRUE( IndexedRedBlackTreeTest::isEmpty( IndexedRedBlackTree<std::string>{} ) );
    EXPECT_TRUE( IndexedRedBlackTreeTest::isEmpty( IndexedRedBlackTree<IndexedRedBlackTree<int>>{} ) );
    EXPECT_TRUE( IndexedRedBlackTreeTest::isEmpty( IndexedRedBlackTree<IndexedRedBlackTree<IndexedRedBlackTree<int>>>{} ) );
}

TEST( IndexedRedBlackTreeTest, IndexedRedBlackTree )
{
    std::ofstream log( "log.txt" );
    EXPECT_TRUE( log );

    constexpr std::size_t N = 1000;
    const IndexedRedBlackTree<int> tree( createRandomTree<int, true>( 6 * N, log, Generator<int>( N ) ) );
    log.close();

    EXPECT_EQ( tree.size(), N );
    EXPECT_TRUE( IndexedRedBlackTreeTest::allPointersAreValid( tree ) );
    EXPECT_TRUE( IndexedRedBlackTreeTest::isBinarySearchTree( tree ) );
    EXPECT_TRUE( IndexedRedBlackTreeTest::rootIsBlack( tree ) );
    EXPECT_TRUE( IndexedRedBlackTreeTest::bothChildrenOfRedAreBlack( tree ) );
    EXPECT_TRUE( IndexedRedBlackTreeTest::blackLengthIsCorrectForEveryNode( tree ) );
}


TEST( IndexedRedBlackTreeTest, Iterators )
{
    std::ofstream log( "log.txt" );
    EXPECT_TRUE( log );

    constexpr std::size_t N = 1000;
    const IndexedRedBlackTree<int> tree( createRandomTree<int, true>( N, log, Generator<int>( N ) ) );
    log.close();

    EXPECT_TRUE( IndexedRedBlackTreeTest::iteratorsAreValid( tree ) );
    EXPECT_TRUE( IndexedRedBlackTreeTest::reverseIteratorsAreValid( tree ) );
    EXPECT_TRUE( IndexedRedBlackTreeTest::findIsCorrect( tree ) );
    EXPECT_EQ( tree.find( -1 ), tree.cend() );
}

TEST( IndexedRedBlackTreeTest, Erase )
{
    std::ofstream log( "log.txt" );
    EXPECT_TRUE( log );

    constexpr std::size_t N = 1000;
    const IndexedRedBlackTree<int> tree( createRandomTree<int, true>( N, log, Generator<int>( N ) ) );
    log.close();

    EXPECT_TRUE( IndexedRedBlackTreeTest::eraseIsValid( tree ) );
}

TEST( IndexedRedBlackTreeTest, FindByIndex )
{
    std::ofstream log( "log.txt" );
    EXPECT_TRUE( log );

    constexpr std::size_t N = 10'000;
    const IndexedRedBlackTree<int> tree( createRandomTree<int, true>( N, log, Generator<int>( N ) ) );
    log.close();

    EXPECT_TRUE( IndexedRedBlackTreeTest::findByIndexIsValid( tree ) );

    for ( std::size_t i : { N, N + 1 } )
    {
        try
        {
            tree.findByIndex( i );
            EXPECT_TRUE( false ); // must be exception
        }
        catch ( std::out_of_range& e )
        {
            using namespace std::string_literals;
            EXPECT_EQ( e.what(), "Invalid index"s ); // must be that exception
        }
    }
}

TEST( IndexedRedBlackTreeTest, GetByIndex )
{
    std::ofstream log( "log.txt" );
    EXPECT_TRUE( log );

    constexpr std::size_t N = 10'000;
    const IndexedRedBlackTree<int> tree( createRandomTree<int, true>( N, log, Generator<int>( N ) ) );
    log.close();

    EXPECT_TRUE( IndexedRedBlackTreeTest::getByIndexIsValid( tree ) );

    for ( std::size_t i : { N, N + 1 } )
    {
        try
        {
            tree.getByIndex( i );
            EXPECT_TRUE( false ); // must be exception
        }
        catch ( std::out_of_range& e )
        {
            using namespace std::string_literals;
            EXPECT_EQ( e.what(), "Invalid index"s ); // must be that exception
        }
    }
}

TEST( IndexedRedBlackTreeTest, EraseByIndex0 )
{
    std::ofstream log( "log.txt" );
    EXPECT_TRUE( log );

    constexpr std::size_t N = 1000;
    IndexedRedBlackTree<std::size_t> tree( createRandomTree<std::size_t, true>( N, log, Generator<std::size_t>( N ) ) );
    log.close();

    EXPECT_TRUE( IndexedRedBlackTreeTest::eraseByIndex0IsValid( tree ) );
    EXPECT_TRUE( tree.cbegin() == tree.cend() );
    EXPECT_TRUE( tree.size() == 0 );

    try
    {
        tree.eraseByIndex( 0 );
        EXPECT_TRUE( false ); // must be exception
    }
    catch ( std::out_of_range& e )
    {
        using namespace std::string_literals;
        EXPECT_EQ( e.what(), "Invalid index"s ); // must be that exception
    }
}

TEST( IndexedRedBlackTreeTest, EraseByIndex )
{
    std::ofstream log( "log.txt" );
    EXPECT_TRUE( log );

    constexpr std::size_t N = 1000;
    IndexedRedBlackTree<std::size_t> tree( createRandomTree<std::size_t, true>( N, log, Generator<std::size_t>( N ) ) );
    log.close();

    EXPECT_TRUE( IndexedRedBlackTreeTest::eraseByIndexIsValid( tree, N / 7 * 3 ) );
}

TEST( IndexedRedBlackTreeTest, Serialize )
{
    EXPECT_TRUE( IndexedRedBlackTreeTest::serializeIntIsValid() );
    EXPECT_TRUE( IndexedRedBlackTreeTest::serializeStringIsValid() );
}

TEST( IndexedRedBlackTreeTest, LeftCount )
{
    std::ofstream log( "log.txt" );
    EXPECT_TRUE( log );

    constexpr std::size_t N = 1000;
    const IndexedRedBlackTree<int> tree( createRandomTree<int, true>( N, log, Generator<int>( N ) ) );
    log.close();

    EXPECT_TRUE( IndexedRedBlackTreeTest::leftCountsAreValid( tree ) );
}