/*
 * This file is part of nodevmap open source project.
 *
 * For conditions of distribution and use, see copyright notice in LICENSE file.
 *
 * For more detailed information : https://github.com/moveplaten
 */

#pragma once

struct NvpPoint;

class NvpMatrix32
{
public:
    NvpMatrix32();

    static NvpMatrix32 calcProduct(const NvpMatrix32& a, const NvpMatrix32& b);

    static NvpMatrix32 calcTranslation(float x, float y);

    static NvpMatrix32 calcScale(float x, float y, const NvpPoint& center);

    float _11, _12;
    float _21, _22;
    float _31, _32;
};
