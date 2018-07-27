/*
 * Copyright 2016 Freeman Zhang <zhanggyb@gmail.com>
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

#include "sigcxx/binode.hpp"

namespace sigcxx {

BinodeBase::~BinodeBase() {
  Unlink(this);
}

void BinodeBase::PushFront(BinodeBase *node, BinodeBase *other) {
  if (other == node) return;
  if (node->previous_ == other) return;

  Unlink(other);

  if (nullptr != node->previous_) node->previous_->next_ = other;
  other->previous_ = node->previous_;
  node->previous_ = other;
  other->next_ = node;
}

void BinodeBase::PushBack(BinodeBase *node, BinodeBase *other) {
  if (other == node) return;
  if (node->next_ == other)return;

  Unlink(other);

  if (nullptr != node->next_) node->next_->previous_ = other;
  other->next_ = node->next_;
  node->next_ = other;
  other->previous_ = node;
}

void BinodeBase::Unlink(BinodeBase *node) {
  bool notify = false;

  if (nullptr != node->previous_) {
    notify = true;
    node->previous_->next_ = node->next_;
  }

  if (nullptr != node->next_) {
    notify = true;
    node->next_->previous_ = node->previous_;
  }

  node->previous_ = nullptr;
  node->next_ = nullptr;

  if (notify) node->OnUnlinked();
}

} // namespace sigcxx
