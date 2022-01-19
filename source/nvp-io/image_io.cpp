/*
 * This file is part of nodevmap open source project.
 *
 * For conditions of distribution and use, see copyright notice in LICENSE file.
 *
 * For more detailed information : https://github.com/moveplaten
 */

#include "image_io.h"
#include "draw/draw.h"

void NvpImageIO::outputAll(const char* file_name)
{
    return NvpDrawPort::outputImage(file_name);
}
