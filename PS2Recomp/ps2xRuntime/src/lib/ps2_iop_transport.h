#pragma once

#include "ps2_runtime.h"
#include "ps2x/iop/iop_subsystem.h"

#include <utility>

class PS2IopTransport
{
public:
    [[nodiscard]] static ps2x::iop::RpcAbi selectRpcAbi(
        const PS2Runtime *runtime,
        const ps2x::iop::RpcAbiRequest &request)
    {
        return runtime
                   ? runtime->selectIopRpcAbi(request)
                   : ps2x::iop::RpcAbi::RuntimeDefault;
    }

    [[nodiscard]] static ps2x::iop::RpcResult handleRpc(
        PS2Runtime *runtime,
        uint8_t *rdram,
        R5900Context *context,
        ps2x::iop::RpcRequest request)
    {
        return runtime
                   ? runtime->handleIopRpc(rdram, context, std::move(request))
                   : ps2x::iop::RpcResult{};
    }

    // Round N2 (n2snd): the native sound driver's status request called straight, as handleRpc would run it (the
    // IOP's clock caught up, the call scope entered), without SIF / RPC (ps2_runtime.cpp).
    static bool sg2Serve(PS2Runtime *runtime, uint8_t *rdram, R5900Context *context, const uint32_t *command, size_t words, uint8_t *reply);

    [[nodiscard]] static bool canBindRpc(const PS2Runtime *runtime, uint32_t sid)
    {
        return !runtime || runtime->canBindIopRpc(sid);
    }

    static void notifyTransfer(
        PS2Runtime *runtime,
        uint8_t *rdram,
        const ps2x::iop::SifTransfer &transfer)
    {
        if (runtime)
        {
            runtime->notifyIopSifTransfer(rdram, transfer);
        }
    }

    static void reset(PS2Runtime *runtime)
    {
        if (runtime)
        {
            runtime->resetIop();
        }
    }
};
