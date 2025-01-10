/*
 * Copyright 2023 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "metaprogramming/deep_equal_diminished.h"

#include <tuple>

namespace {

////////////////////////////////////////////////////////////////////////////////
// NOTE: This file is effectively a duplicate of vals_equal_test.cc.
// Unsupported assertions are intentionally left commented for comparison.
////////////////////////////////////////////////////////////////////////////////

using ::jni::metaprogramming::DeepEqualDiminished_v;

struct A {
  constexpr explicit A(int i) : i_(i) {}
  const int i_;
  constexpr bool operator==(const A& rhs) const { return i_ == rhs.i_; }
};

template <typename... Ts>
struct Type {};

template <const auto&... Vs>
struct ConstAutoRef {};

////////////////////////////////////////////////////////////////////////////////
// Types.
////////////////////////////////////////////////////////////////////////////////
static_assert(DeepEqualDiminished_v<int, int>);
static_assert(!DeepEqualDiminished_v<int, float>);

static_assert(!DeepEqualDiminished_v<int, std::tuple<int>>);

static_assert(DeepEqualDiminished_v<std::tuple<int>, std::tuple<int>>);
static_assert(!DeepEqualDiminished_v<std::tuple<int>, std::tuple<float>>);
static_assert(!DeepEqualDiminished_v<std::tuple<int, int>, std::tuple<int>>);

static_assert(
    DeepEqualDiminished_v<std::tuple<Type<int>>, std::tuple<Type<int>>>);
static_assert(
    !DeepEqualDiminished_v<std::tuple<Type<int>>, std::tuple<std::tuple<int>>>);
static_assert(!DeepEqualDiminished_v<std::tuple<Type<int, int>>,
                                     std::tuple<std::tuple<int, int>>>);

////////////////////////////////////////////////////////////////////////////////
// Const Autos Refs.
////////////////////////////////////////////////////////////////////////////////
const auto a = 'a';
const auto b = 'b';
static_assert(
    !DeepEqualDiminished_v<std::tuple<Type<int>>, std::tuple<ConstAutoRef<a>>>);
static_assert(DeepEqualDiminished_v<std::tuple<ConstAutoRef<a>>,
                                    std::tuple<ConstAutoRef<a>>>);
static_assert(!DeepEqualDiminished_v<std::tuple<ConstAutoRef<a>>,
                                     std::tuple<ConstAutoRef<b>>>);
// static_assert(!DeepEqualDiminished_v<std::tuple<ConstAutoRef<a>>,
// std::tuple<ConstAutoRef<a, a>>>);

static_assert(DeepEqualDiminished_v<ConstAutoRef<a>, ConstAutoRef<a>>);
static_assert(!DeepEqualDiminished_v<ConstAutoRef<a>, ConstAutoRef<b>>);
// static_assert(!DeepEqualDiminished_v<ConstAutoRef<a>, ConstAutoRef<a, a>>);

}  // namespace
