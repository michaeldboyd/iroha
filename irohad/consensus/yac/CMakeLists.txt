#
# Copyright Soramitsu Co., Ltd. 2017 All Rights Reserved.
# http://soramitsu.co.jp
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#        http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

add_library(supermajority_check
    impl/supermajority_checker_impl.cpp
    )
target_link_libraries(supermajority_check
    shared_model_interfaces
    )

add_library(yac
    impl/yac.cpp
    impl/cluster_order.cpp
    impl/timer_impl.cpp
    impl/peer_orderer_impl.cpp
    impl/yac_gate_impl.cpp
    impl/yac_hash_provider_impl.cpp

    storage/impl/yac_common.cpp
    storage/impl/yac_block_storage.cpp
    storage/impl/yac_proposal_storage.cpp
    storage/impl/yac_vote_storage.cpp
    storage/impl/buffered_cleanup_strategy.cpp
    )
target_link_libraries(yac
    supermajority_check
    common
    rxcpp
    logger
    hash
    consensus_round
    gate_object
    )

add_library(yac_transport
    transport/impl/network_impl.cpp
    impl/yac_crypto_provider_impl.cpp
    )
target_link_libraries(yac_transport
    yac
    yac_grpc
    logger
    shared_model_proto_backend
    shared_model_stateless_validation # ProtoCommonObjectsFactory -> FieldValidator
    )
