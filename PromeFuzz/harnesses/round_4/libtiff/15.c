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

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        unlink("./dummy_file");
        return 0;
    }
    
    uint32_t val32;
    uint16_t val16;
    float valfloat;
    char* valstr;
    uint32_t* val32p;
    uint16_t* val16p;
    float* valfloatp;
    
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &val32);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &val32);
    TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &val16);
    TIFFGetField(tif, TIFFTAG_COMPRESSION, &val16);
    TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &val16);
    TIFFGetField(tif, TIFFTAG_MAKE, &valstr);
    TIFFGetField(tif, TIFFTAG_MODEL, &valstr);
    TIFFGetField(tif, TIFFTAG_STRIPOFFSETS, &val32p);
    TIFFGetField(tif, TIFFTAG_ORIENTATION, &val16);
    
    toff_t gps_offset = 0;
    if (Size > 200) {
        memcpy(&gps_offset, Data + 100, sizeof(toff_t));
    }
    TIFFReadGPSDirectory(tif, gps_offset);
    
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &val16);
    TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &val32);
    TIFFGetField(tif, TIFFTAG_STRIPBYTECOUNTS, &val32p);
    TIFFGetField(tif, TIFFTAG_XRESOLUTION, &valfloat);
    TIFFGetField(tif, TIFFTAG_YRESOLUTION, &valfloat);
    TIFFGetField(tif, TIFFTAG_PLANARCONFIG, &val16);
    TIFFGetField(tif, TIFFTAG_RESOLUTIONUNIT, &val16);
    TIFFGetField(tif, TIFFTAG_SOFTWARE, &valstr);
    TIFFGetField(tif, TIFFTAG_DATETIME, &valstr);
    TIFFGetField(tif, TIFFTAG_ARTIST, &valstr);
    
    tdir_t dir_num = 0;
    if (Size > 150) {
        memcpy(&dir_num, Data + 120, sizeof(tdir_t));
    }
    TIFFSetDirectory(tif, dir_num);
    
    TIFFGetField(tif, TIFFTAG_SUBIFD, &val32p);
    
    toff_t exif_offset = 0;
    if (Size > 250) {
        memcpy(&exif_offset, Data + 150, sizeof(toff_t));
    }
    TIFFReadEXIFDirectory(tif, exif_offset);
    
    TIFFGetField(tif, TIFFTAG_WHITEPOINT, &valfloatp);
    TIFFGetField(tif, TIFFTAG_PRIMARYCHROMATICITIES, &valfloatp);
    TIFFGetField(tif, TIFFTAG_TRANSFERFUNCTION, &val16p);
    
    const TIFFField* field = TIFFFindField(tif, TIFFTAG_IMAGEWIDTH, TIFF_LONG);
    
    TIFFGetField(tif, TIFFTAG_PREDICTOR, &val16);
    TIFFGetField(tif, TIFFTAG_COLORMAP, &val16p);
    TIFFGetField(tif, TIFFTAG_TILEWIDTH, &val32);
    TIFFGetField(tif, TIFFTAG_TILELENGTH, &val32);
    TIFFGetField(tif, TIFFTAG_EXTRASAMPLES, &val16p);
    TIFFGetField(tif, TIFFTAG_SAMPLEFORMAT, &val16);
    TIFFGetField(tif, TIFFTAG_YCBCRSUBSAMPLING, &val16);
    TIFFGetField(tif, TIFFTAG_REFERENCEBLACKWHITE, &valfloatp);
    TIFFGetField(tif, TIFFTAG_INKNAMES, &valstr);
    
    TIFFClose(tif);
    
    unlink("./dummy_file");
    return 0;
}