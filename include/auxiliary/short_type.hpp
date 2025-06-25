#pragma once

#include <vector>
#include <map>
#include <memory>

template <typename T>
using vec = std::vector<T>;
template <typename T>
using sptr = std::shared_ptr<T>;
template <typename T>
using uptr = std::unique_ptr<T>;
template <typename T>
template <typename L>
using map = std::map<T, L>;
