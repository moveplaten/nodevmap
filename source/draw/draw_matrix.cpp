/*
 * This file is part of nodevmap open source project.
 *
 * For conditions of distribution and use, see copyright notice in LICENSE file.
 *
 * For more detailed information : https://github.com/moveplaten
 */

#include "event/nvp_event.h"
#include "draw_matrix.h"

NvpMatrix32::NvpMatrix32() :
    _11(1), _12(0), _21(0), _22(1), _31(0), _32(0)
{

}

NvpMatrix32 NvpMatrix32::calcProduct(const NvpMatrix32& a, const NvpMatrix32& b)
{
    NvpMatrix32 mtx;

    mtx._11 = a._11 * b._11 + a._12 * b._21;
    mtx._12 = a._11 * b._12 + a._12 * b._22;
    mtx._21 = a._21 * b._11 + a._22 * b._21;
    mtx._22 = a._21 * b._12 + a._22 * b._22;
    mtx._31 = a._31 * b._11 + a._32 * b._21 + b._31;
    mtx._32 = a._31 * b._12 + a._32 * b._22 + b._32;

    return mtx;
}

NvpMatrix32 NvpMatrix32::calcTranslation(float x, float y)
{
    NvpMatrix32 mtx;

    mtx._11 = 1.0; mtx._12 = 0.0;
    mtx._21 = 0.0; mtx._22 = 1.0;
    mtx._31 = x;   mtx._32 = y;

    return mtx;
}

NvpMatrix32 NvpMatrix32::calcScale(float x, float y, const NvpPoint& center)
{
    NvpMatrix32 mtx;

    mtx._11 = x;   mtx._12 = 0.0;
    mtx._21 = 0.0; mtx._22 = y;
    mtx._31 = center.x - x * center.x;
    mtx._32 = center.y - y * center.y;

    return mtx;
}
