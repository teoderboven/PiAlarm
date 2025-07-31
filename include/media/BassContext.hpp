#ifndef BASSCONTEXT_H
#define BASSCONTEXT_H

#include "bass.h"
#include <mutex>
#include <stdexcept>

namespace PiAlarm::media {

    /**
     * @class BassContext
     * @brief RAII wrapper for the BASS audio system initialization and cleanup.
     *
     * Ensures that BASS is initialized on the first instantiation
     * and automatically freed when the last instance is destroyed.
     * Thread-safe.
     */
    class BassContext {
        static inline std::mutex mutex_; ///< Mutex for thread safety
        static inline int instanceCount_ {0}; ///< Count of active instances

    public:

        /**
         * @brief Constructor
         * Initializes BASS if this is the first instance.
         * @throws std::runtime_error if BASS initialization fails.
         */
        inline BassContext();

        /**
         * @brief Destructor
         * Frees BASS if this is the last instance.
         */
        inline ~BassContext();

        BassContext(const BassContext&) = delete; ///< No copy constructor
        BassContext& operator=(const BassContext&) = delete; ///< No copy assignment operator
        BassContext(BassContext&&) = delete; ///< No move constructor
        BassContext& operator=(BassContext&&) = delete; ///< No move assignment operator

    };

    // Methods implementations

    inline BassContext::BassContext() {
        std::lock_guard lock{mutex_};

        if (++instanceCount_ == 1) {

            if (!BASS_Init(-1, 44100, 0, nullptr, nullptr)) {
                --instanceCount_; // roll back on failure
                throw std::runtime_error("Failed to initialize BASS.");
            }
        }
    }

    inline BassContext::~BassContext() {
        std::lock_guard lock{mutex_};

        if (--instanceCount_ == 0) {
            BASS_Free();
        }
    }

} // namespace PiAlarm::media

#endif //BASSCONTEXT_H
