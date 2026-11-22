#pragma once

#include "ps2_syscalls.h"

namespace ps2_syscalls
{
    void fioOpen(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void fioClose(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void fioRead(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void fioWrite(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    // Round A7: fioWrite's body over its arguments (the syscall and sceWrite's stub call it; typed callers of sceWrite too)
    int32_t fioWriteCore(uint8_t *rdram, PS2Runtime *runtime, int ps2Fd, uint32_t bufAddr, uint32_t size);
    void fioLseek(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void fioMkdir(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void fioChdir(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void fioRmdir(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void fioGetstat(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void fioRemove(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
