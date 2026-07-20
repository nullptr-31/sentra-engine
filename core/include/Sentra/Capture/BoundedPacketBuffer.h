//
// Created by Abdelbaki Boukerche on 19/7/2026.
//

#ifndef SENTRA_ENGINE_BOUNDEDPACKETBUFFER_H
#define SENTRA_ENGINE_BOUNDEDPACKETBUFFER_H

#include <cstddef>
#include <mutex>
#include <optional>
#include <vector>

namespace SCore {
    template<typename T>
    class BoundedPacketBuffer {
    public:
        explicit BoundedPacketBuffer(std::size_t capacity) : m_Buffer(capacity), m_Capacity(capacity) {
            if (capacity == 0) {
                throw std::invalid_argument("BoundedPacketBuffer capacity must be greater than zero");
            }
        }

        BoundedPacketBuffer(const BoundedPacketBuffer &) = delete;
        BoundedPacketBuffer(BoundedPacketBuffer &&) = delete;

        BoundedPacketBuffer &operator=(const BoundedPacketBuffer &) = delete;
        BoundedPacketBuffer &operator=(BoundedPacketBuffer &&) = delete;

        bool TryPush(const T &item) { return TryPushImpl(item); }
        bool TryPush(T &&item) { return TryPushImpl(std::move(item)); }

        bool TryPop(T &item) {
            std::lock_guard lock(m_Mutex);

            if (m_Size == 0) { return false; }

            PopInto(item);
            return true;
        }

        bool WaitPop(T &item) {
            std::unique_lock lock(m_Mutex);

            m_NotEmpty.wait(lock, [this] { return m_Size > 0 || m_Closed; });

            if (m_Size == 0) { return false; }

            PopInto(item);
            return true;
        }

        void Close() {
            {
                std::lock_guard lock(m_Mutex);

                m_Closed = true;
            }
            m_NotEmpty.notify_all();
        }

        void Reset() {
            std::lock_guard lock(m_Mutex);

            for (auto &item: m_Buffer) { item.reset(); }

            m_Head = 0;
            m_Tail = 0;
            m_Size = 0;
            m_Closed = false;
        }

        std::size_t Size() const {
            std::lock_guard lock(m_Mutex);

            return m_Size;
        }

        std::size_t Capacity() const { return m_Capacity; }

        bool IsClosed() const {
            std::lock_guard lock(m_Mutex);

            return m_Closed;
        }

    private:
        template<typename U>
        bool TryPushImpl(U &&item) {
            {
                std::lock_guard lock(m_Mutex);

                if (m_Closed || m_Size == m_Capacity) { return false; }

                m_Buffer[m_Tail].emplace(std::forward<U>(item));

                m_Tail = (m_Tail + 1) % m_Capacity;
                ++m_Size;
            }
            m_NotEmpty.notify_one();
            return true;
        }

        void PopInto(T &item) {
            item = std::move(*m_Buffer[m_Head]);

            m_Buffer[m_Head].reset();

            m_Head = (m_Head + 1) % m_Capacity;
            --m_Size;
        }

    private:
        std::vector<std::optional<T> > m_Buffer;

        std::size_t m_Capacity;
        std::size_t m_Head = 0;
        std::size_t m_Tail = 0;
        std::size_t m_Size = 0;

        bool m_Closed = false;

        mutable std::mutex m_Mutex;
        std::condition_variable m_NotEmpty;
    };
} // SCore

#endif //SENTRA_ENGINE_BOUNDEDPACKETBUFFER_H
