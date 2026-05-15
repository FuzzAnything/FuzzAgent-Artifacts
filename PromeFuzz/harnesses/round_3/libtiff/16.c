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
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFReadEXIFDirectory at tif_dirread.c:5462:5 in tiffio.h
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
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        unlink("./dummy_file");
        return 0;
    }
    
    uint32_t width, height, samplesperpixel, bitspersample, compression;
    uint32_t photometric, rowsperstrip, stripoffsets, stripbytecounts;
    float xresolution, yresolution;
    uint16_t planarconfig, resolutionunit;
    uint16_t *colormap = NULL;
    float *whitepoint = NULL;
    float *primarychromaticities = NULL;
    uint16_t *transferfunction = NULL;
    char *software = NULL;
    char *datetime = NULL;
    char *artist = NULL;
    char *hostcomputer = NULL;
    char *make = NULL;
    char *model = NULL;
    uint16_t orientation;
    uint16_t minsamplevalue, maxsamplevalue;
    float xposition, yposition;
    char *pagename = NULL;
    uint32_t subfiletype;
    
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bitspersample);
    TIFFGetField(tif, TIFFTAG_COMPRESSION, &compression);
    TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &photometric);
    TIFFGetField(tif, TIFFTAG_STRIPOFFSETS, &stripoffsets);
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samplesperpixel);
    TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rowsperstrip);
    TIFFGetField(tif, TIFFTAG_STRIPBYTECOUNTS, &stripbytecounts);
    
    TIFFReadGPSDirectory(tif, 0);
    
    TIFFGetField(tif, TIFFTAG_XRESOLUTION, &xresolution);
    TIFFGetField(tif, TIFFTAG_YRESOLUTION, &yresolution);
    TIFFGetField(tif, TIFFTAG_PLANARCONFIG, &planarconfig);
    TIFFGetField(tif, TIFFTAG_RESOLUTIONUNIT, &resolutionunit);
    TIFFGetField(tif, TIFFTAG_COLORMAP, &colormap);
    TIFFGetField(tif, TIFFTAG_WHITEPOINT, &whitepoint);
    TIFFGetField(tif, TIFFTAG_PRIMARYCHROMATICITIES, &primarychromaticities);
    TIFFGetField(tif, TIFFTAG_TRANSFERFUNCTION, &transferfunction);
    TIFFGetField(tif, TIFFTAG_SOFTWARE, &software);
    TIFFGetField(tif, TIFFTAG_DATETIME, &datetime);
    
    TIFFSetDirectory(tif, 0);
    
    TIFFGetField(tif, TIFFTAG_ARTIST, &artist);
    
    TIFFReadEXIFDirectory(tif, 0);
    
    TIFFGetField(tif, TIFFTAG_HOSTCOMPUTER, &hostcomputer);
    
    const TIFFField *field = TIFFFindField(tif, TIFFTAG_IMAGEWIDTH, TIFF_LONG);
    
    TIFFGetField(tif, TIFFTAG_MAKE, &make);
    TIFFGetField(tif, TIFFTAG_MODEL, &model);
    TIFFGetField(tif, TIFFTAG_ORIENTATION, &orientation);
    TIFFGetField(tif, TIFFTAG_MINSAMPLEVALUE, &minsamplevalue);
    TIFFGetField(tif, TIFFTAG_MAXSAMPLEVALUE, &maxsamplevalue);
    TIFFGetField(tif, TIFFTAG_XPOSITION, &xposition);
    TIFFGetField(tif, TIFFTAG_YPOSITION, &yposition);
    TIFFGetField(tif, TIFFTAG_PAGENAME, &pagename);
    TIFFGetField(tif, TIFFTAG_SUBFILETYPE, &subfiletype);
    
    TIFFClose(tif);
    TIFFClose(tif);
    
    unlink("./dummy_file");
    return 0;
}