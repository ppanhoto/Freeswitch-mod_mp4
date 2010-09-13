/*
 * SpanDSP - a series of DSP components for telephony
 *
 * generate_sized_pages.c - Create a series of TIFF files in the various page sizes
 *                        and resolutions.
 *
 * Written by Steve Underwood <steveu@coppice.org>
 *
 * Copyright (C) 2006 Steve Underwood
 *
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2, as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/*! \file */

#if defined(HAVE_CONFIG_H)
#include "config.h"
#endif

#include <stdio.h>
#include <inttypes.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include <memory.h>
#include <string.h>
#if defined(HAVE_TGMATH_H)
#include <tgmath.h>
#endif
#if defined(HAVE_MATH_H)
#include <math.h>
#endif

#include "spandsp.h"

struct
{
    const char *name;
    int x_res;
    int y_res;
    int width;
    int length;
} sequence[] =
{
    {
        "R8_385_A4.tif",
        T4_X_RESOLUTION_R8,
        T4_Y_RESOLUTION_STANDARD,
        T4_WIDTH_R8_A4,
        1100
    },
    {
        "R8_385_B4.tif",
        T4_X_RESOLUTION_R8,
        T4_Y_RESOLUTION_STANDARD,
        T4_WIDTH_R8_B4,
        1200
    },
    {
        "R8_385_A3.tif",
        T4_X_RESOLUTION_R8,
        T4_Y_RESOLUTION_STANDARD,
        T4_WIDTH_R8_A3,
        1556
    },
    {
        "R8_77_A4.tif",
        T4_X_RESOLUTION_R8,
        T4_Y_RESOLUTION_FINE,
        T4_WIDTH_R8_A4,
        2200
    },
    {
        "R8_77_B4.tif",
        T4_X_RESOLUTION_R8,
        T4_Y_RESOLUTION_FINE,
        T4_WIDTH_R8_B4,
        2400
    },
    {
        "R8_77_A3.tif",
        T4_X_RESOLUTION_R8,
        T4_Y_RESOLUTION_FINE,
        T4_WIDTH_R8_A3,
        3111
    },
    {
        "R8_154_A4.tif",
        T4_X_RESOLUTION_R8,
        T4_Y_RESOLUTION_SUPERFINE,
        T4_WIDTH_R8_A4,
        4400
    },
    {
        "R8_154_B4.tif",
        T4_X_RESOLUTION_R8,
        T4_Y_RESOLUTION_SUPERFINE,
        T4_WIDTH_R8_B4,
        4800
    },
    {
        "R8_154_A3.tif",
        T4_X_RESOLUTION_R8,
        T4_Y_RESOLUTION_SUPERFINE,
        T4_WIDTH_R8_A3,
        6222
    },
    {
        "R300_300_A4.tif",
        T4_X_RESOLUTION_300,
        T4_Y_RESOLUTION_300,
        T4_WIDTH_300_A4,
        4400
    },
    {
        "R300_300_B4.tif",
        T4_X_RESOLUTION_300,
        T4_Y_RESOLUTION_300,
        T4_WIDTH_300_B4,
        4800
    },
    {
        "R300_300_A3.tif",
        T4_X_RESOLUTION_300,
        T4_Y_RESOLUTION_300,
        T4_WIDTH_300_A3,
        6222
    },
    {
        "R300_600_A4.tif",
        T4_X_RESOLUTION_300,
        T4_Y_RESOLUTION_600,
        T4_WIDTH_300_A4,
        4400
    },
    {
        "R300_600_B4.tif",
        T4_X_RESOLUTION_300,
        T4_Y_RESOLUTION_600,
        T4_WIDTH_300_B4,
        4800
    },
    {
        "R300_600_A3.tif",
        T4_X_RESOLUTION_300,
        T4_Y_RESOLUTION_600,
        T4_WIDTH_300_A3,
        6222
    },
    {
        "R16_154_A4.tif",
        T4_X_RESOLUTION_R16,
        T4_Y_RESOLUTION_SUPERFINE,
        T4_WIDTH_R16_A4,
        4400
    },
    {
        "R16_154_B4.tif",
        T4_X_RESOLUTION_R16,
        T4_Y_RESOLUTION_SUPERFINE,
        T4_WIDTH_R16_B4,
        4800
    },
    {
        "R16_154_A3.tif",
        T4_X_RESOLUTION_R16,
        T4_Y_RESOLUTION_SUPERFINE,
        T4_WIDTH_R16_A3,
        6222
    },
    {
        "R16_800_A4.tif",
        T4_X_RESOLUTION_R16,
        T4_Y_RESOLUTION_800,
        T4_WIDTH_R16_A4,
        4400
    },
    {
        "R16_800_B4.tif",
        T4_X_RESOLUTION_R16,
        T4_Y_RESOLUTION_800,
        T4_WIDTH_R16_B4,
        4800
    },
    {
        "R16_800_A3.tif",
        T4_X_RESOLUTION_R16,
        T4_Y_RESOLUTION_800,
        T4_WIDTH_R16_A3,
        6222
    },
    {
        "R600_600_A4.tif",
        T4_X_RESOLUTION_600,
        T4_Y_RESOLUTION_600,
        T4_WIDTH_600_A4,
        4400
    },
    {
        "R600_600_B4.tif",
        T4_X_RESOLUTION_600,
        T4_Y_RESOLUTION_600,
        T4_WIDTH_600_B4,
        4800
    },
    {
        "R600_600_A3.tif",
        T4_X_RESOLUTION_600,
        T4_Y_RESOLUTION_600,
        T4_WIDTH_600_A3,
        6222
    },
    {
        "R600_1200_A4.tif",
        T4_X_RESOLUTION_600,
        T4_Y_RESOLUTION_1200,
        T4_WIDTH_600_A4,
        4400
    },
    {
        "R600_1200_B4.tif",
        T4_X_RESOLUTION_600,
        T4_Y_RESOLUTION_1200,
        T4_WIDTH_600_B4,
        4800
    },
    {
        "R600_1200_A3.tif",
        T4_X_RESOLUTION_600,
        T4_Y_RESOLUTION_1200,
        T4_WIDTH_600_A3,
        6222
    },
    {
        "R1200_1200_A4.tif",
        T4_X_RESOLUTION_1200,
        T4_Y_RESOLUTION_1200,
        T4_WIDTH_1200_A4,
        4400
    },
    {
        "R1200_1200_B4.tif",
        T4_X_RESOLUTION_1200,
        T4_Y_RESOLUTION_1200,
        T4_WIDTH_1200_B4,
        4800
    },
    {
        "R1200_1200_A3.tif",
        T4_X_RESOLUTION_1200,
        T4_Y_RESOLUTION_1200,
        T4_WIDTH_1200_A3,
        6222
    },
    {
        NULL,
        0,
        0,
        0,
        0
    },
};

int main(int argc, char *argv[])
{
    int row;
    uint8_t image_buffer[8192];
    TIFF *tiff_file;
    struct tm *tm;
    time_t now;
    char buf[133];
    float x_resolution;
    float y_resolution;
    int i;
    int opt;
    int compression;
    int photo_metric;
    int fill_order;
    
    compression = T4_COMPRESSION_ITU_T6;
    photo_metric = PHOTOMETRIC_MINISWHITE;
    fill_order = FILLORDER_LSB2MSB;
    while ((opt = getopt(argc, argv, "126ir")) != -1)
    {
        switch (opt)
        {
        case '1':
            compression = T4_COMPRESSION_ITU_T4_1D;
            break;
        case '2':
            compression = T4_COMPRESSION_ITU_T4_2D;
            break;
        case '6':
            compression = T4_COMPRESSION_ITU_T6;
            break;
        case 'i':
            photo_metric = PHOTOMETRIC_MINISBLACK;
            break;
        case 'r':
            fill_order = FILLORDER_MSB2LSB;
            break;
        default:
            //usage();
            exit(2);
            break;
        }
    }

    for (i = 0;  sequence[i].name;  i++)
    {
        if ((tiff_file = TIFFOpen(sequence[i].name, "w")) == NULL)
            exit(2);

        /* Prepare the directory entry fully before writing the image, or libtiff complains */
        TIFFSetField(tiff_file, TIFFTAG_COMPRESSION, compression);
        TIFFSetField(tiff_file, TIFFTAG_IMAGEWIDTH, sequence[i].width);
        TIFFSetField(tiff_file, TIFFTAG_BITSPERSAMPLE, 1);
        TIFFSetField(tiff_file, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
        TIFFSetField(tiff_file, TIFFTAG_SAMPLESPERPIXEL, 1);
        TIFFSetField(tiff_file, TIFFTAG_ROWSPERSTRIP, -1L);
        TIFFSetField(tiff_file, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
        TIFFSetField(tiff_file, TIFFTAG_PHOTOMETRIC, photo_metric);
        TIFFSetField(tiff_file, TIFFTAG_FILLORDER, fill_order);
    
        x_resolution = sequence[i].x_res/100.0f;
        y_resolution = sequence[i].y_res/100.0f;
        TIFFSetField(tiff_file, TIFFTAG_XRESOLUTION, floorf(x_resolution*2.54f + 0.5f));
        TIFFSetField(tiff_file, TIFFTAG_YRESOLUTION, floorf(y_resolution*2.54f + 0.5f));
        TIFFSetField(tiff_file, TIFFTAG_RESOLUTIONUNIT, RESUNIT_INCH);
    
        TIFFSetField(tiff_file, TIFFTAG_SOFTWARE, "spandsp");
        if (gethostname(buf, sizeof(buf)) == 0)
            TIFFSetField(tiff_file, TIFFTAG_HOSTCOMPUTER, buf);
    
        TIFFSetField(tiff_file, TIFFTAG_IMAGEDESCRIPTION, "Blank test image");
        TIFFSetField(tiff_file, TIFFTAG_MAKE, "soft-switch.org");
        TIFFSetField(tiff_file, TIFFTAG_MODEL, "test data");

        time(&now);
        tm = localtime(&now);
        sprintf(buf,
    	        "%4d/%02d/%02d %02d:%02d:%02d",
                tm->tm_year + 1900,
                tm->tm_mon + 1,
                tm->tm_mday,
                tm->tm_hour,
                tm->tm_min,
                tm->tm_sec);
        TIFFSetField(tiff_file, TIFFTAG_DATETIME, buf);
    
        TIFFSetField(tiff_file, TIFFTAG_IMAGELENGTH, sequence[i].length);
        TIFFSetField(tiff_file, TIFFTAG_PAGENUMBER, 0, 1);
        TIFFSetField(tiff_file, TIFFTAG_CLEANFAXDATA, CLEANFAXDATA_CLEAN);
        TIFFSetField(tiff_file, TIFFTAG_IMAGEWIDTH, sequence[i].width);

        /* Write the image first.... */
        for (row = 0;  row < sequence[i].length;  row++)
        {
            memset(image_buffer, 0, sequence[i].width/8 + 1);
            if (TIFFWriteScanline(tiff_file, image_buffer, row, 0) < 0)
            {
                printf("Write error at row %d.\n", row);
                exit(2);
            }
        }
        /* ....then the directory entry, and libtiff is happy. */
        TIFFWriteDirectory(tiff_file);
        TIFFClose(tiff_file);
    }
    return 0;
}
/*- End of function --------------------------------------------------------*/
/*- End of file ------------------------------------------------------------*/
