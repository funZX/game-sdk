/*
 * Copyright 2017 - 2018 The WizTK Authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "sigcxx/sigcxx.hpp"

namespace sigcxx {

namespace internal {

TrackableBindingNode::~TrackableBindingNode() {
  if (nullptr != token) {
    _ASSERT(token->binding == this);
    token->binding = nullptr;
    delete token;
  }
}

SignalTokenNode::~SignalTokenNode() {
  _ASSERT(nullptr == slot_mark_head.previous());
  Slot::Mark *mark = nullptr;
  while (nullptr != slot_mark_head.next()) {
    mark = static_cast<Slot::Mark *>(slot_mark_head.next());
    ++mark->slot()->it_;
    ++mark->slot()->ref_count_;
    mark->unlink();
  }

  if (nullptr != binding) {
    _ASSERT(binding->token == this);
    binding->token = nullptr;
    delete binding;
  }
}

}  // namespace internal

Trackable::Trackable(const Trackable &)
    : Trackable() {}

Trackable::~Trackable() {
  UnbindAllSignals();
}

void Trackable::UnbindSignal(SLOT slot) {
  using internal::SignalTokenNode;

  if (slot->it_.get()->binding->trackable == this) {
    SignalTokenNode *tmp = slot->it_.get();
    delete tmp;
  }
}

void Trackable::UnbindAllSignals() {
  internal::TrackableBindingNode *tmp = nullptr;

  internal::InterRelatedDeque<internal::TrackableBindingNode>::ReverseIterator it = bindings_.rbegin();
  while (it) {
    tmp = it.get();
    delete tmp;
    it = bindings_.rbegin();
  }
}

size_t Trackable::CountSignalBindings() const {
  size_t count = 0;
  for (auto it = bindings_.cbegin(); it != bindings_.cend(); ++it) {
    count++;
  }
  return count;
}

// ------

} // namespace sigcxx
