/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "framework/integration_framework/fake_peer/behaviour/behaviour.hpp"

namespace integration_framework {
  namespace fake_peer {

    Behaviour::~Behaviour() {
      absolve();
    }

    void Behaviour::adopt(const std::shared_ptr<FakePeer> &fake_peer) {
      // This code feels like part of constructor, but the use of `this'
      // to call virtual functions from base class constructor seems wrong.
      // Hint: such calls would precede the derived class construction.
      fake_peer_ = fake_peer;
      log_ = logger::log(getName() + " of fake peer at "
                         + fake_peer->getAddress());
      // subscribe for all messages
      subscriptions_.emplace_back(
          getFakePeer().getMstStatesObservable().subscribe(
              [this, alive = fake_peer](const auto &message) {
                this->processMstMessage(message);
              }));
      subscriptions_.emplace_back(
          getFakePeer().getYacStatesObservable().subscribe(
              [this, alive = fake_peer](const auto &message) {
                this->processYacMessage(message);
              }));
      subscriptions_.emplace_back(
          getFakePeer().getOsBatchesObservable().subscribe(
              [this, alive = fake_peer](const auto &batch) {
                this->processOsBatch(batch);
              }));
      subscriptions_.emplace_back(
          getFakePeer().getOgProposalsObservable().subscribe(
              [this, alive = fake_peer](const auto &proposal) {
                this->processOgProposal(proposal);
              }));
      subscriptions_.emplace_back(
          getFakePeer().getBatchesObservable().subscribe(
              [this, alive = fake_peer](const auto &batches_for_round) {
                this->processOrderingBatches(*batches_for_round);
              }));
    }

    void Behaviour::absolve() {
      for (auto &subscription : subscriptions_) {
        subscription.unsubscribe();
      }
      fake_peer_.reset();
    }

    FakePeer &Behaviour::getFakePeer() {
      return *fake_peer_;
    }

    logger::Logger &Behaviour::getLogger() {
      return log_;
    }

  }  // namespace fake_peer
}  // namespace integration_framework
