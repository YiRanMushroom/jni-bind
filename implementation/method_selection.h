/*
 * Copyright 2021 Google LLC
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

#ifndef JNI_BIND_METHOD_SELECTION_H_
#define JNI_BIND_METHOD_SELECTION_H_

// IWYU pragma: private, include "third_party/jni_wrapper/jni_bind.h"

#include <cstddef>
#include <type_traits>

#include "implementation/id.h"
#include "implementation/id_type.h"
#include "implementation/no_idx.h"
#include "implementation/overload_ref.h"
#include "implementation/proxy_convenience_aliases.h"
#include "metaprogramming/call.h"
#include "metaprogramming/min_max.h"
#include "metaprogramming/reduce.h"
#include "metaprogramming/type_of_nth_element.h"
#include "metaprogramming/unfurl.h"
#include "metaprogramming/vals.h"

namespace jni {

// Viablility helper for an exact parameter.
template <std::size_t I, typename IdT, typename ValkReturnIDType,
          typename... Ts>
struct Viable {
  static constexpr IdType kReturnIDType = ValkReturnIDType::val;
  using IdTmp = typename IdT::template ChangeIdType<kReturnIDType>;
  using IdTParamType = typename IdTmp::template ChangeIdx<2, I>;

  static constexpr bool val =
      Proxy_t<typename IdTParamType::UnstrippedRawVal>::template kViable<
          IdTParamType,
          metaprogramming::TypeOfNthElement_t<I, std::decay_t<Ts>...>>;
};

template <typename OverloadId, IdType kReturnIDType>
struct ArgumentValidate {
  // Helper to prevents instantiating mismatching size unrolls.
  template <typename... Ts>
  static constexpr bool ViableHelper() {
    if constexpr (sizeof...(Ts) == OverloadId::kNumParams) {
      return metaprogramming::UnfurlConjunction_v<
          OverloadId::kNumParams, Viable, OverloadId,
          metaprogramming::Val_t<kReturnIDType>, Ts...>;
    } else {
      return false;
    }
  }

  template <typename... Ts>
  static constexpr bool kValid = ViableHelper<Ts...>();
};

template <typename IdT_, IdType kReturnIDType>
struct OverloadSelection {
  using IdT = IdT_;

  template <typename... Ts>
  static constexpr bool OverloadViable() {
    return ArgumentValidate<IdT, kReturnIDType>::template kValid<Ts...>;
  }

  template <typename... Ts>
  static constexpr size_t OverloadIdxIfViable() {
    return OverloadViable<Ts...>() ? IdT::kSecondaryIdx : kNoIdx;
  }
};

template <typename IdT_, IdType kIDType = IdType::OVERLOAD,
          IdType kReturnIDType = IdType::OVERLOAD_PARAM>
struct MethodSelection {
  using IdT = IdT_;
  using _JniT = typename IdT::_JniT;

  template <std::size_t I, typename... Ts>
  struct Helper {
    using type = metaprogramming::Val_t<OverloadSelection<
        Id<_JniT, kIDType, IdT::kIdx, I, kNoIdx, 0>,
        kReturnIDType>::template OverloadIdxIfViable<Ts...>()>;
  };

  template <typename... Ts>
  static constexpr std::size_t kIdxForTs = metaprogramming::ReduceAsPack_t<
      metaprogramming::Min, metaprogramming::Call_t<metaprogramming::Unfurl_t<
                                IdT::NumParams(), Helper, Ts...>>>::val;

  template <typename... Ts>
  using FindOverloadSelection = OverloadSelection<
      Id<_JniT, kIDType, IdT::kIdx, kIdxForTs<Ts...>, kNoIdx, 0>,
      kReturnIDType>;

  template <typename... Ts>
  static constexpr bool ArgSetViable() {
    return kIdxForTs<Ts...> != kNoIdx;
  }
};

template <typename IdT, IdType kIDType, IdType kReturnIDType, typename... Args>
struct OverloadSelector {
  using OverloadSelectionForArgs = typename MethodSelection<
      IdT, kIDType, kReturnIDType>::template FindOverloadSelection<Args...>;

  using _OverloadRef =
      OverloadRef<Id<typename IdT::_JniT, kIDType, IdT::kIdx,
                     OverloadSelectionForArgs::IdT::kSecondaryIdx, kNoIdx, 0>,
                  kReturnIDType>;

  static constexpr bool kIsValidArgSet =
      MethodSelection<IdT, kIDType,
                      kReturnIDType>::template ArgSetViable<Args...>();
};

}  // namespace jni

#endif  // JNI_BIND_METHOD_SELECTION_H_
