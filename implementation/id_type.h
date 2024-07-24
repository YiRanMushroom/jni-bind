/*
 * Copyright 2022 Google LLC
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

#ifndef JNI_BIND_IMPLEMENTATION_ID_TYPE_H_
#define JNI_BIND_IMPLEMENTATION_ID_TYPE_H_

// IWYU pragma: private, include "third_party/jni_wrapper/jni_bind.h"

namespace jni {

enum class IdType {
  CLASS,
  STATIC_OVERLOAD_SET,
  STATIC_OVERLOAD,
  STATIC_OVERLOAD_PARAM,
  OVERLOAD_SET,
  OVERLOAD,
  OVERLOAD_PARAM,
  STATIC_FIELD,
  FIELD,
};

}  // namespace jni

#endif  // JNI_BIND_IMPLEMENTATION_ID_TYPE_H_
