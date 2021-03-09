#pragma once
#include "map.h"

class MapTest
{
public:

#define TEST_DECL(testName) \
	static bool testName()

    TEST_DECL( basicTest );
    TEST_DECL( insertTest );
    TEST_DECL( serializeTest );

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

TEST_DEF( serializeTest )
{
    Map<std::string, int> map;

    map["One"] = map["Two"] = map["Three"] = 1;
    map["Two"]++;
    ++map["Three"];
    ++map["Three"];

    const std::string test = map.serialize();

    const std::string ref =
    "{\n"
    "    \"value\": {\n"
    "        \"first\": \"Three\",\n"
    "        \"second\": 3\n"
    "    },\n"
    "    \"color\": \"black\",\n"
    "    \"left\": {\n"
    "        \"value\": {\n"
    "            \"first\": \"One\",\n"
    "            \"second\": 1\n"
    "        },\n"
    "        \"color\": \"red\",\n"
    "        \"left\": null,\n"
    "        \"right\": null\n"
    "    },\n"
    "    \"right\": {\n"
    "        \"value\": {\n"
    "            \"first\": \"Two\",\n"
    "            \"second\": 2\n"
    "        },\n"
    "        \"color\": \"red\",\n"
    "        \"left\": null,\n"
    "        \"right\": null\n"
    "    }\n"
    "}";

    return test == ref;
}