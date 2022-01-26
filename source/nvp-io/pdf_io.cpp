/*
 * This file is part of nodevmap open source project.
 *
 * For conditions of distribution and use, see copyright notice in LICENSE file.
 *
 * For more detailed information : https://github.com/moveplaten
 */

#include "pdf_io.h"
#include "draw/nvp_draw.h"

void NvpPdfIO::outputAll(const char* file_name)
{
    return NvpDrawPort::outputPDF(file_name);
}
