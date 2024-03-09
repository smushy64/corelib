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

/// @brief Slice container.
///
/// Not meant to be used by itself but rather as a typedef'd struct
/// where the appropriate union field is more clear.
/// @see #String
struct CoreSlice {
    /// @brief Union of different pointers.
    union {
        /// @brief Pointer to constant string.
        const char* cc;
        /// @brief Pointer to string.
        char* c;
        /// @brief Pointer to data.
        void* v;
    };
    /// @brief Number of items in slice.
    usize len;
};

/// @brief Generic slice container.
typedef struct GenericSlice {
    /// @brief Pointer to buffer.
    void* buf;
    /// @brief Size of each item in buffer.
    usize stride;
    /// @brief Number of items in slice.
    usize len;
} GenericSlice;

/// @brief Buffer container.
///
/// Not meant to be used by itself but rather as a typedef'd struct
/// where the appropriate union field is more clear.
/// @see #StringBuf
struct CoreBuffer {
    /// @brief Union of fields in common with CoreSlice.
    union {
        /// @brief Buffer and length.
        struct {
            /// @brief Union of different pointers.
            union {
                /// @brief Pointer to constant string.
                const char* cc;
                /// @brief Pointer to string.
                char* c;
                /// @brief Pointer to data.
                void* v;
            };
            /// @brief Number of valid items in buffer.
            usize len;
        };
        /// @brief Buffer as a slice.
        struct CoreSlice slice;
    };
    /// @brief Number of items that buffer is capable of holding.
    usize cap;
};

/// @brief Generic buffer container.
typedef struct GenericBuffer {
    /// @brief Union of fieldds in common with GenericSlice.
    union {
        /// @brief Buffer, stride and length.
        struct {
            /// @brief Pointer to buffer.
            void* buf;
            /// @brief Size of each item in buffer in bytes.
            usize stride;
            /// @brief Number of valid items in buffer.
            usize len;
        };
        /// @brief Buffer as a slice.
        struct GenericSlice slice;
    };
    /// @brief Number of items that buffer is capable of holding.
    ///
    /// Total size of buffer is #len * #cap.
    usize cap;
} GenericBuffer;

#if defined(CORE_CPLUSPLUS) && defined(CORE_COMPILER_CLANG) && !defined(CORE_LSP_CLANGD)
    #pragma clang diagnostic pop
    #pragma clang diagnostic pop
#endif

#endif /* header guard */
