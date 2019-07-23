#pragma once
#include <iostream>
#include <sstream>
#include <memory>
#include <tuple>
#include <map>
#include <string>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <any>
#include <set>
#include <functional>
#include <exception>

#if defined(_DEBUG)

#define ASSERT(X) _ASSERT(X)
#define VERIFY(X) _ASSERT(X)

#else

#define ASSERT(X)
#define VERIFY(X) (X)

#endif

#define ASSERT_NOT_NULL(X) ASSERT(X != nullptr)
#define ASSERT_NULL(X) ASSERT(X == nullptr)