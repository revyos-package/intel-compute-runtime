/*
 * Copyright (C) 2021-2024 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "shared/source/debug_settings/debug_settings_manager.h"
#include "shared/source/utilities/cpu_info.h"

#ifndef BIT
#define BIT(x) (1ull << (x))
#endif

namespace NEO {
void CpuInfo::detect() const {
}
} // namespace NEO
