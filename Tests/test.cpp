#include <redblacktree.h>
#include <redblacktreetest.h>
#include <maptest.h>

namespace
{
	template<typename T>
	struct Generator
	{
		Generator(std::size_t N)
		{
			static_assert(!"Not implemented");
		}
		T operator()()
		{
			static_assert(!"Not implemented");
			return {};
		}
	};

	template<>
	struct Generator<int>
	{
		Generator(std::size_t N)
			: m_numbers(N)
		{
			srand(static_cast<unsigned int>(time(0)));

			int i = 0;
			for (int& num : m_numbers)
			{
				num = i++;
			}

			std::random_device device;
			std::mt19937 generator(device());

			std::shuffle(m_numbers.begin(), m_numbers.end(), generator);
		}
		int operator()() const
		{
			static std::size_t i = 0;
			return m_numbers[(i++) % m_numbers.size()];
		}

		std::vector<int> m_numbers;
	};

	template<typename T, typename Less = std::less<T>, typename GeneratorType = Generator<T>>
	RedBlackTree<T, Less> createRandomTree(std::size_t N, std::ostream& os = std::cout, GeneratorType generate = {})
	{
		std::vector<T> elements;
		elements.reserve(N);

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

TEST(RedBlackTreeTest, ConstructorsAndAssignment)
{
	std::ofstream log("log.txt");
	EXPECT_TRUE(log.is_open());

	const std::size_t N = 1000;
	const RedBlackTree<int> tree(createRandomTree<int>(N, log, Generator<int>(N)));
	log.close();

	EXPECT_TRUE(RedBlackTreeTest::copyConstructorIsValid(tree));
	EXPECT_TRUE(RedBlackTreeTest::moveConstructorIsValid(tree));

	EXPECT_TRUE(RedBlackTreeTest::copyAssignmentIsValid(tree));
	EXPECT_TRUE(RedBlackTreeTest::moveAssignmentIsValid(tree));
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

	const std::size_t N = 1000;
	const RedBlackTree<int> tree(createRandomTree<int>(6*N, log, Generator<int>(N)));
	log.close();
	
	EXPECT_EQ(tree.size(), N);
	EXPECT_TRUE(RedBlackTreeTest::allPointersAreValid(tree));
	EXPECT_TRUE(RedBlackTreeTest::isBinarySearchTree(tree));
	EXPECT_TRUE(RedBlackTreeTest::rootIsBlack(tree));
	EXPECT_TRUE(RedBlackTreeTest::bothChildrenOfRedAreBlack(tree));
	EXPECT_TRUE(RedBlackTreeTest::blackLengthIsCorrectForEveryNode(tree));
}


TEST(RedBlackTreeTest, Iterators)
{
	std::ofstream log("log.txt");
	EXPECT_TRUE(log.is_open());

	const std::size_t N = 1000;
	const RedBlackTree<int> tree(createRandomTree<int>(N, log, Generator<int>(N)));
	log.close();

	EXPECT_TRUE(RedBlackTreeTest::iteratorsAreValid(tree));
	EXPECT_TRUE(RedBlackTreeTest::reverseIteratorsAreValid(tree));
	EXPECT_TRUE(RedBlackTreeTest::findIsCorrect(tree));
	EXPECT_EQ(tree.find(-1), tree.cend());
}

TEST(RedBlackTreeTest, Erase)
{
	std::ofstream log("log.txt");
	EXPECT_TRUE(log.is_open());

	const std::size_t N = 1000;
	const RedBlackTree<int> tree(createRandomTree<int>(N, log, Generator<int>(N)));
	log.close();
	
	EXPECT_TRUE(RedBlackTreeTest::eraseIsValid(tree));
}


TEST(MapTest, Basic)
{
	EXPECT_TRUE(MapTest::basicTest());
}


TEST(MapTest, Insert)
{
	EXPECT_TRUE(MapTest::insertTest());
}