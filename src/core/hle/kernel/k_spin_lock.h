// SPDX-FileCopyrightText: Copyright 2021 sudachi Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <mutex>

#include "common/common_funcs.h"
#include "core/hle/kernel/k_scoped_lock.h"

namespace Kernel {

class KSpinLock {
public:
    explicit KSpinLock() = default;

    SUDACHI_NON_COPYABLE(KSpinLock);
    SUDACHI_NON_MOVEABLE(KSpinLock);

    void Lock();
    void Unlock();
    bool TryLock();

private:
    std::mutex m_lock;
};

// TODO(bunnei): Alias for now, in case we want to implement these accurately in the future.
using KAlignedSpinLock = KSpinLock;
using KNotAlignedSpinLock = KSpinLock;

using KScopedSpinLock = KScopedLock<KSpinLock>;
using KScopedAlignedSpinLock = KScopedLock<KAlignedSpinLock>;
using KScopedNotAlignedSpinLock = KScopedLock<KNotAlignedSpinLock>;

} // namespace Kernel
