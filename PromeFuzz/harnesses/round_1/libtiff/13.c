// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFReadGPSDirectory at tif_dirread.c:5470:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFReadEXIFDirectory at tif_dirread.c:5462:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        unlink("./dummy_file");
        return 0;
    }
    
    uint32_t width, height, rowsperstrip;
    uint16_t samplesperpixel, bitspersample, photometric, compression, planarconfig, orientation;
    float xresolution, yresolution;
    uint16_t* sampleinfo;
    uint32_t* stripbytecounts;
    uint32_t* stripoffsets;
    
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samplesperpixel);
    TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bitspersample);
    TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &photometric);
    TIFFGetField(tif, TIFFTAG_COMPRESSION, &compression);
    TIFFGetField(tif, TIFFTAG_PLANARCONFIG, &planarconfig);
    TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rowsperstrip);
    TIFFGetField(tif, TIFFTAG_ORIENTATION, &orientation);
    
    TIFFReadGPSDirectory(tif, 0);
    
    TIFFGetField(tif, TIFFTAG_XRESOLUTION, &xresolution);
    TIFFGetField(tif, TIFFTAG_YRESOLUTION, &yresolution);
    TIFFGetField(tif, TIFFTAG_EXTRASAMPLES, &sampleinfo);
    TIFFGetField(tif, TIFFTAG_STRIPBYTECOUNTS, &stripbytecounts);
    TIFFGetField(tif, TIFFTAG_STRIPOFFSETS, &stripoffsets);
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samplesperpixel);
    TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bitspersample);
    
    TIFFSetDirectory(tif, 0);
    
    TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &photometric);
    
    TIFFReadEXIFDirectory(tif, 0);
    
    TIFFGetField(tif, TIFFTAG_COMPRESSION, &compression);
    TIFFGetField(tif, TIFFTAG_PLANARCONFIG, &planarconfig);
    
    const TIFFField* field = TIFFFindField(tif, TIFFTAG_IMAGEWIDTH, TIFF_LONG);
    
    TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rowsperstrip);
    TIFFGetField(tif, TIFFTAG_ORIENTATION, &orientation);
    TIFFGetField(tif, TIFFTAG_XRESOLUTION, &xresolution);
    TIFFGetField(tif, TIFFTAG_YRESOLUTION, &yresolution);
    TIFFGetField(tif, TIFFTAG_EXTRASAMPLES, &sampleinfo);
    TIFFGetField(tif, TIFFTAG_STRIPBYTECOUNTS, &stripbytecounts);
    TIFFGetField(tif, TIFFTAG_STRIPOFFSETS, &stripoffsets);
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    
    TIFFClose(tif);
    tif = NULL;
    TIFFClose(tif);
    
    unlink("./dummy_file");
    return 0;
}