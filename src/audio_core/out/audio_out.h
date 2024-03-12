// SPDX-FileCopyrightText: Copyright 2022 sudachi Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <mutex>

#include "audio_core/out/audio_out_system.h"

namespace Core {
class System;
}

namespace Kernel {
class KEvent;
class KReadableEvent;
} // namespace Kernel

namespace AudioCore::AudioOut {
class Manager;

/**
 * Interface between the service and audio out system. Mainly responsible for forwarding service
 * calls to the system.
 */
class Out {
public:
    explicit Out(Core::System& system, Manager& manager, Kernel::KEvent* event, size_t session_id);

    /**
     * Free this audio out from the audio out manager.
     */
    void Free();

    /**
     * Get this audio out's system.
     */
    System& GetSystem();

    /**
     * Get the current state.
     *
     * @return Started or Stopped.
     */
    AudioOut::State GetState();

    /**
     * Start the system
     *
     * @return Result code
     */
    Result StartSystem();

    /**
     * Start the system's device session.
     */
    void StartSession();

    /**
     * Stop the system.
     *
     * @return Result code
     */
    Result StopSystem();

    /**
     * Append a new buffer to the system, the buffer event will be signalled when it is filled.
     *
     * @param buffer - The new buffer to append.
     * @param tag    - Unique tag for this buffer.
     * @return Result code.
     */
    Result AppendBuffer(const AudioOutBuffer& buffer, u64 tag);

    /**
     * Release all completed buffers, and register any appended.
     */
    void ReleaseAndRegisterBuffers();

    /**
     * Flush all buffers.
     */
    bool FlushAudioOutBuffers();

    /**
     * Get all of the currently released buffers.
     *
     * @param tags - Output container for the buffer tags which were released.
     * @return The number of buffers released.
     */
    u32 GetReleasedBuffers(std::span<u64> tags);

    /**
     * Get the buffer event for this audio out, this event will be signalled when a buffer is
     * filled.
     * @return The buffer event.
     */
    Kernel::KReadableEvent& GetBufferEvent();

    /**
     * Get the current system volume.
     *
     * @return The current volume.
     */
    f32 GetVolume() const;

    /**
     * Set the system volume.
     *
     * @param volume - The volume to set.
     */
    void SetVolume(f32 volume);

    /**
     * Check if a buffer is in the system.
     *
     * @param tag - The tag to search for.
     * @return True if the buffer is in the system, otherwise false.
     */
    bool ContainsAudioBuffer(u64 tag) const;

    /**
     * Get the maximum number of buffers.
     *
     * @return The maximum number of buffers.
     */
    u32 GetBufferCount() const;

    /**
     * Get the total played sample count for this audio out.
     *
     * @return The played sample count.
     */
    u64 GetPlayedSampleCount() const;

private:
    /// The AudioOut::Manager this audio out is registered with
    Manager& manager;
    /// Manager's mutex
    std::recursive_mutex& parent_mutex;
    /// Buffer event, signalled when buffers are ready to be released
    Kernel::KEvent* event;
    /// Main audio out system
    System system;
};

} // namespace AudioCore::AudioOut
