/*
 * Copyright (C) 2020-2025 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "shared/source/utilities/cpuintrinsics.h"

#if defined(_WIN32)
#include <immintrin.h>
#include <intrin.h>
#pragma intrinsic(__rdtsc)
#else
#if defined(__ARM_ARCH)
extern "C" uint64_t __rdtsc();
#elif defined(__riscv) && __riscv_xlen == 64
// nothing
#else
#include <immintrin.h>
#include <x86intrin.h>
#endif
#endif

#if defined(__ARM_ARCH)
#include <sse2neon.h>
#elif defined(__riscv) && __riscv_xlen == 64
// nothing
#else
#include <emmintrin.h>
#endif

namespace NEO {
namespace CpuIntrinsics {

void clFlush(void const *ptr) {
#if !(defined(__riscv) && __riscv_xlen == 64)
    _mm_clflush(ptr);
#else
    asm volatile("fence rw,rw");
#endif
}

void clFlushOpt(void *ptr) {
#ifdef SUPPORTS_CLFLUSHOPT
    _mm_clflushopt(ptr);
#else
#if !(defined(__riscv) && __riscv_xlen == 64)
    _mm_clflush(ptr);
#else
    asm volatile("fence rw,rw");
#endif
#endif
}

void sfence() {
#if !(defined(__riscv) && __riscv_xlen == 64)
    _mm_sfence();
#else
    asm volatile("fence w,w");
#endif
}

void mfence() {
#if !(defined(__riscv) && __riscv_xlen == 64)
    _mm_mfence();
#else
    asm volatile("fence rw,rw");
#endif
}

void pause() {
#if !(defined(__riscv) && __riscv_xlen == 64)
    _mm_pause();
#else
//    need zihintpause
//    asm volatile("pause":::"memory");
    asm volatile(".insn 0x0100000f" ::: "memory");
#endif
}

uint8_t tpause(uint32_t control, uint64_t counter) {
#ifdef SUPPORTS_WAITPKG
    return _tpause(control, counter);
#else
    return 0;
#endif
}

unsigned char umwait(unsigned int ctrl, uint64_t counter) {
#ifdef SUPPORTS_WAITPKG
    return _umwait(ctrl, counter);
#else
    return 0;
#endif
}

void umonitor(void *a) {
#ifdef SUPPORTS_WAITPKG
    _umonitor(a);
#endif
}

uint64_t rdtsc() {
#if defined(__riscv) && __riscv_xlen == 64
    uint64_t val;
    asm volatile("rdtime %0" : "=r"(val));
    return val;
#else
    return __rdtsc();
#endif
}

} // namespace CpuIntrinsics
} // namespace NEO
