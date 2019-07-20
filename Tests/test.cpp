#include <redblacktree.h>
#include <redblacktreetest.h>

namespace
{
	template<typename T>
	struct Generator
	{
		T operator()()
		{
			static_assert(!"Not implemented");
			return {};
		}
	};

	template<>
	struct Generator<int>
	{
		Generator()
		{
			srand(static_cast<unsigned int>(time(0)));
		}
		int operator()() const
		{
			return rand() % 10000;
		}
	};

	template<typename T, typename Less = std::less<T>, typename GeneratorType = Generator<T>>
	RedBlackTree<T, Less> createRandomTree(std::size_t N, std::ostream& os = std::cout)
	{
		std::vector<T> elements;
		elements.reserve(N);

		GeneratorType generate;
		for (std::size_t i = 0; i < N; ++i)
		{
			elements.push_back(generate());
		}

		std::copy(std::cbegin(elements), std::cend(elements),
			std::ostream_iterator<T>(os, " "));
		os << std::endl << std::endl;

		RedBlackTree<T, Less> tree(std::cbegin(elements), std::cend(elements));
		os << tree.serialize() << std::endl << std::endl << std::endl;
		os.flush();

		return tree;
	}
}


TEST(RedBlackTreeTest, Empty) 
{
	EXPECT_TRUE(RedBlackTreeTest::isEmpty(RedBlackTree<int>{}));
	EXPECT_TRUE(RedBlackTreeTest::isEmpty(RedBlackTree<std::string>{}));
	EXPECT_TRUE(RedBlackTreeTest::isEmpty(RedBlackTree<RedBlackTree<int>>{}));
	EXPECT_TRUE(RedBlackTreeTest::isEmpty(RedBlackTree<RedBlackTree<RedBlackTree<int>>>{}));
}

TEST(RedBlackTreeTest, RedBlackTree) 
{
	std::ofstream log("log.txt");
	EXPECT_TRUE(log.is_open());

	const std::size_t N = 10;
	const RedBlackTree<int> tree(createRandomTree<int>(N, log));
	log.close();
	
	EXPECT_EQ(tree.size(), N);
	EXPECT_TRUE(RedBlackTreeTest::allPointersAreValid(tree));
	EXPECT_TRUE(RedBlackTreeTest::isBinarySearchTree(tree));
	EXPECT_TRUE(RedBlackTreeTest::rootIsBlack(tree));
	EXPECT_TRUE(RedBlackTreeTest::bothChildrenOfRedAreBlack(tree));
	EXPECT_TRUE(RedBlackTreeTest::blackLengthIsCorrectForEveryNode(tree));
	EXPECT_TRUE(RedBlackTreeTest::isRotateLeftCorrect<int>());
}

TEST(RedBlackTreeTest, Rotates)
{
	EXPECT_TRUE(RedBlackTreeTest::isRotateLeftCorrect<int>());
	EXPECT_TRUE(RedBlackTreeTest::isRotateRightCorrect<int>());
}