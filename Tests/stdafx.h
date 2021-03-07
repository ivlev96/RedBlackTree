#pragma once

#include "gtest/gtest.h"
#include <fstream>
#include <random>

#if defined(_DEBUG)

#define ASSERT(X) _ASSERT(X)
#define VERIFY(X) _ASSERT(X)

#else

#define ASSERT(X)
#define VERIFY(X) (X)

#endif

#define ASSERT_NOT_NULL(X) ASSERT((X) != nullptr)
#define ASSERT_NULL(X) ASSERT((X) == nullptr)