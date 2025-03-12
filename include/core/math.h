#if !defined(CORE_MATH_H)
#define CORE_MATH_H
/**
 * @file   math.h
 * @brief  Include math headers.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   February 09, 2024
*/
// IWYU pragma: begin_exports
#include "core/math/trig.h"
#include "core/math/exponential.h"
#include "core/math/common.h"
#include "core/math/vector2.h"
#include "core/math/vector3.h"
#include "core/math/vector4.h"
#include "core/math/quaternion.h"
#include "core/math/matrix2x2.h"
#include "core/math/matrix3x3.h"
#include "core/math/matrix4x4.h"
#include "core/math/color.h"

#if defined(CORE_CPLUSPLUS)
    #include "core/cpp/math/overload.hpp"
#else
    #include "core/math/overload.h"
#endif

// IWYU pragma: end_exports

#endif /* header guard */
