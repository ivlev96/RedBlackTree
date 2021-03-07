#pragma once
#include "map.h"

class MapTest
{
public:

#define TEST_DECL(testName) \
	static bool testName()

    TEST_DECL( basicTest );
    TEST_DECL( insertTest );

#undef TEST_DECL
};

using namespace std::literals;

#define TEST_DEF(testName) \
	bool MapTest::testName()

TEST_DEF( basicTest )
{
    Map<std::string, int> test;
    test["ololo"s] = 5;
    test["abc"s] = 14;
    test["ololo"s] = 7;
    test["abc"s]++;

    Map<std::string, int> ref
    {
        {"abc"s, 15},
        {"ololo"s, 7}
    };

    return test == ref;
}


TEST_DEF( insertTest )
{
    Map<std::string, int> test;
    test.insert( { "3"s, 3 } );
    test.insert( { "2"s, 2 } );
    test.insert( { "1"s, 1 } );

    Map<std::string, int> ref
    {
        { "1"s, 1 },
        { "2"s, 2 },
        { "3"s, 3 }
    };

    return test == ref;
}
