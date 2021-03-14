#pragma once

#include "gtest/gtest.h"
#include <fstream>
#include <random>
#include <numeric>

#if defined(_DEBUG)

#define ASSERT(X) _ASSERT(X)
#define VERIFY(X) _ASSERT(X)

#else

#define ASSERT(X)
#define VERIFY(X) (X)

#endif

#define ASSERT_NOT_NULL(X) ASSERT((X) != nullptr)
#define ASSERT_NULL(X) ASSERT((X) == nullptr)

#include <redblacktree.h>

template<typename T,
         typename = std::enable_if_t<std::is_integral_v<T>>>
    struct Generator
{
    Generator( std::size_t N )
        : m_numbers( N )
    {
        srand( static_cast<unsigned int>( time( 0 ) ) );

        std::iota( m_numbers.begin(), m_numbers.end(), static_cast<T>( 0 ) );

        std::random_device device;
        std::mt19937 generator( device() );

        std::shuffle( m_numbers.begin(), m_numbers.end(), generator );
    }
    T operator()() const
    {
        static std::size_t i = 0;
        return m_numbers[( i++ ) % m_numbers.size()];
    }

    std::vector<T> m_numbers;
};

template<typename T, bool IsIndexed = false, typename Less = std::less<T>, typename GeneratorType = Generator<T>>
inline RedBlackTreeBase<T, IsIndexed, Less> createRandomTree( std::size_t N, std::ostream& os = std::cout, GeneratorType generate = {} )
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

    RedBlackTreeBase<T, IsIndexed, Less> tree( std::cbegin( elements ), std::cend( elements ) );
    os << tree.serialize() << std::endl << std::endl << std::endl;
    os.flush();

    return tree;
}