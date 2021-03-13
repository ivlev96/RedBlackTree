#include <redblacktree.h>
#include <indexedredblacktreetest.h>

namespace
{
template<typename T>
struct Generator
{
    Generator( std::size_t N )
    {
        static_assert( !"Not implemented" );
    }
    T operator()()
    {
        static_assert( !"Not implemented" );
        return {};
    }
};

template<>
struct Generator<int>
{
    Generator( std::size_t N )
        : m_numbers( N )
    {
        srand( static_cast<unsigned int>( time( 0 ) ) );

        int i = 0;
        for ( int& num : m_numbers )
        {
            num = i++;
        }

        std::random_device device;
        std::mt19937 generator( device() );

        std::shuffle( m_numbers.begin(), m_numbers.end(), generator );
    }
    int operator()() const
    {
        static std::size_t i = 0;
        return m_numbers[( i++ ) % m_numbers.size()];
    }

    std::vector<int> m_numbers;
};

template<typename T, typename Less = std::less<T>, typename GeneratorType = Generator<T>>
IndexedRedBlackTree<T, Less> createRandomTree( std::size_t N, std::ostream& os = std::cout, GeneratorType generate = {} )
{
    std::vector<T> elements;
    elements.reserve( N );

    for ( std::size_t i = 0; i < N; ++i )
    {
        elements.push_back( generate() );
    }

    std::copy( std::cbegin( elements ), std::cend( elements ),
        std::ostream_iterator<T>( os, " " ) );
    os << std::endl << std::endl;

    IndexedRedBlackTree<T, Less> tree( std::cbegin( elements ), std::cend( elements ) );
    os << tree.serialize() << std::endl << std::endl << std::endl;
    os.flush();

    return tree;
}
}

TEST( IndexedRedBlackTreeTest, ConstructorsAndAssignment )
{
    std::ofstream log( "log.txt" );
    EXPECT_TRUE( log );

    const std::size_t N = 1000;
    const IndexedRedBlackTree<int> tree( createRandomTree<int>( N, log, Generator<int>( N ) ) );
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

    const std::size_t N = 1000;
    const IndexedRedBlackTree<int> tree( createRandomTree<int>( 6 * N, log, Generator<int>( N ) ) );
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

    const std::size_t N = 1000;
    const IndexedRedBlackTree<int> tree( createRandomTree<int>( N, log, Generator<int>( N ) ) );
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

    const std::size_t N = 1000;
    const IndexedRedBlackTree<int> tree( createRandomTree<int>( N, log, Generator<int>( N ) ) );
    log.close();

    EXPECT_TRUE( IndexedRedBlackTreeTest::eraseIsValid( tree ) );
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

    const std::size_t N = 1000;
    const IndexedRedBlackTree<int> tree( createRandomTree<int>( N, log, Generator<int>( N ) ) );
    log.close();

    EXPECT_TRUE( IndexedRedBlackTreeTest::leftCountsAreValid( tree ) );
}