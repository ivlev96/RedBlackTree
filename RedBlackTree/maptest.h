#pragma once
#include "map.h"

class MapTest
{
public:

#define TEST_DECL(testName) \
	template<typename KeyType, typename ValueType, typename Less = std::less<KeyType>> \
	static bool testName(const Map<KeyType, ValueType, Less>& map)

	TEST_DECL(basicTest);

#undef TEST_DECL
};


#define TEST_DEF(testName) \
	template<typename KeyType, typename ValueType, typename Less> \
	bool MapTest::testName(const Map<KeyType, ValueType, Less>& map)

TEST_DEF(basicTest)
{
	(void)map;
	Map<std::string, int> a;
	a["ololo"] = 5;
	a["abc"] = 14;
	a["ololo"] = 7;

	a["abc"]++;

	for (auto[key, value] : a)
	{
		std::cout << key << " " << value << std::endl;
	}

	return true;
}
