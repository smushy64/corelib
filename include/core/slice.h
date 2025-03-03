#if !defined(CORE_SLICE_H)
#define CORE_SLICE_H
/**
 * @file   slice.h
 * @brief  Buffer types.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   February 02, 2024
*/
#include "core/types.h"

#if defined(CORE_CPLUSPLUS) && defined(CORE_COMPILER_CLANG)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wnested-anon-types"
#endif

/// @brief Slice of bytes.
struct ByteSlice {
    /// @brief Number of bytes in slice.
    usize len;
    /// @brief Anonymous union of slice pointers.
    union {
        /// @brief Pointer to slice as characters.
        const char* cc;
        /// @brief Pointer to slice as mutable characters.
        char* c;
        /// @brief Void pointer to slice.
        void* v;
        /// @brief Pointer to slice bytes.
        u8* bytes;
    };
};
/// @brief Buffer of bytes.
struct ByteBuffer {
    /// @brief Total number of bytes in buffer.
    usize cap;
    /// @brief Anonymous union of slice and buffer components.
    union {
        /// @brief Anonymous struct of buffer components.
        struct {
            /// @brief Number of valid bytes in buffer.
            usize len;
            /// @brief Anonymous union of buffer pointers.
            union {
                /// @brief Pointer to buffer as characters.
                const char* cc;
                /// @brief Pointer to buffer as mutable characters.
                char* c;
                /// @brief Void pointer to buffer.
                void* v;
                /// @brief Pointer to buffer bytes.
                u8* bytes;
            };
        };
        /// @brief Byte buffer as a slice.
        struct ByteSlice slice;
    };
};
/// @brief Slice of generic items.
struct ItemSlice {
    /// @brief Size of each item in slice.
    usize stride;
    /// @brief Number of items in slice.
    usize len;
    /// @brief Void pointer to slice.
    void* buf;
};
/// @brief Buffer of generic items.
struct ItemBuffer {
    /// @brief Total number of items in buffer.
    usize cap;
    /// @brief Anonymous union of slice and buffer components.
    union {
        /// @brief Anonymous struct of buffer components.
        struct {
            /// @brief Size of each item in buffer.
            usize stride;
            /// @brief Number of items in buffer.
            usize len;
            /// @brief Void pointer to buffer.
            void* buf;
        };
        /// @brief Item buffer as a slice.
        struct ItemSlice slice;
    };
};

#if defined(CORE_CPLUSPLUS) && defined(CORE_COMPILER_CLANG) && !defined(CORE_LSP_CLANGD)
    #pragma clang diagnostic pop
    #pragma clang diagnostic pop
#endif

#endif /* header guard */
