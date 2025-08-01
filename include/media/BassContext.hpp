#ifndef BASSCONTEXT_H
#define BASSCONTEXT_H

#include "bass.h"
#include <mutex>
#include <stdexcept>
#include "logging/HasLogger.h"

namespace PiAlarm::media {

    /**
     * @class BassContext
     * @brief RAII wrapper for the BASS audio system initialization and cleanup.
     *
     * Ensures that BASS is initialized on the first instantiation
     * and automatically freed when the last instance is destroyed.
     * Thread-safe.
     */
    class BassContext : public logging::HasLogger{
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
        inline ~BassContext() override;

        BassContext(const BassContext&) = delete; ///< No copy constructor
        BassContext& operator=(const BassContext&) = delete; ///< No copy assignment operator
        BassContext(BassContext&&) = delete; ///< No move constructor
        BassContext& operator=(BassContext&&) = delete; ///< No move assignment operator

    };

    // Methods implementations

    inline BassContext::BassContext(): HasLogger("BassContext") {
        std::lock_guard lock{mutex_};

        if (++instanceCount_ == 1) {
            if (!BASS_Init(-1, 44100, 0, nullptr, nullptr)) {
                --instanceCount_; // roll back on failure

                logger().error("Failed to initialize BASS: code = {}", BASS_ErrorGetCode());
                throw std::runtime_error("Failed to initialize BASS.");
            }
            logger().info("Successfully initialized BASS.");
        }
    }

    inline BassContext::~BassContext() {
        std::lock_guard lock{mutex_};

        if (--instanceCount_ == 0) {
            BASS_Free();
            logger().debug("BASS has been freed.");
        }
    }

} // namespace PiAlarm::media

#endif //BASSCONTEXT_H
