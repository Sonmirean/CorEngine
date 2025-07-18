#pragma once

#include <vector>
#include <map>
#include <memory>
#include <array>
#include <string>

template <typename T>
using vec = std::vector<T>;

template <typename T, std::size_t L>
using arr = std::array<T, L>;

template <typename T, typename L>
using map = std::map<T, L>;

template <typename T>
using sptr = std::shared_ptr<T>;

template <typename T>
using uptr = std::unique_ptr<T>;

using str = std::string;
