/**
 * This file is part of the "FnordMetric" project
 *   Copyright (c) 2014 Paul Asmuth, Google Inc.
 *
 * FnordMetric is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License v3.0. You should have received a
 * copy of the GNU General Public License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */
#include "fnord/base/inspect.h"

namespace fnord {
namespace comm {

template <typename ServiceType>
LocalRPCChannel::LocalRPCChannel(ServiceType* service) : service_(service) {
  fnord::reflect::MetaClass<ServiceType>::reflectMethods(this);
}

template <typename MethodType>
void LocalRPCChannel::method(MethodType* method) {
  methods_.emplace(method->name(), [this, method] (AnyRPC* anyrpc) {
    auto rpc = dynamic_cast<
        RPC<
            typename MethodType::ReturnType,
            typename MethodType::ArgPackType>*>(anyrpc);

    if (rpc == nullptr) {
        RAISEF(
            kNoSuchMethodError,
            "invalid argument signature for method: $0",
            anyrpc->method());
    }

    rpc->ready(
        method->call((typename MethodType::ClassType*) service_, rpc->args()));
  });
}

template <class RPCType>
void LocalRPCChannel::call(RPCType* rpc) {
  const auto& method = methods_.find(rpc->method());

  if (method == methods_.end()) {
    RAISEF(kNoSuchMethodError, "no such method: $0", rpc->method());
  }

  method->second(rpc);
}

} // namespace comm
} // namsepace fnord
