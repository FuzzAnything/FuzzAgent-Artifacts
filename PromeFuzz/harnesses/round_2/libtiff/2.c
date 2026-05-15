// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFCurrentDirectory at tif_open.c:869:8 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFNumberOfDirectories at tif_dir.c:2032:8 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFCheckpointDirectory at tif_dirwrite.c:292:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFCheckpointDirectory at tif_dirwrite.c:292:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
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

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFF* tif = TIFFOpen("./dummy_file", "r+");
    if (!tif) {
        cleanup_dummy_file();
        return 0;
    }
    
    uint32_t width = 0, height = 0;
    uint16_t samples = 0, bits = 0;
    
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samples);
    TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bits);
    
    uint64_t offset = *(uint64_t*)Data;
    TIFFSetSubDirectory(tif, offset);
    
    TIFFReadDirectory(tif);
    
    tdir_t dir = *(tdir_t*)Data;
    TIFFSetDirectory(tif, dir);
    
    offset = *(uint64_t*)(Data + 8);
    TIFFSetSubDirectory(tif, offset);
    
    TIFFReadDirectory(tif);
    
    dir = *(tdir_t*)(Data + 16);
    TIFFSetDirectory(tif, dir);
    
    TIFFReadDirectory(tif);
    
    tdir_t current = TIFFCurrentDirectory(tif);
    
    offset = *(uint64_t*)(Data + 20);
    TIFFSetSubDirectory(tif, offset);
    
    tdir_t num_dirs = TIFFNumberOfDirectories(tif);
    
    TIFFCreateDirectory(tif);
    
    uint32_t value1 = *(uint32_t*)(Data + 28);
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, value1);
    
    TIFFWriteDirectory(tif);
    
    uint32_t value2 = *(uint32_t*)(Data + 32);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, value2);
    
    TIFFCheckpointDirectory(tif);
    
    TIFFWriteDirectory(tif);
    
    TIFFCheckpointDirectory(tif);
    
    TIFFWriteDirectory(tif);
    
    TIFFClose(tif);
    
    tif = TIFFOpen("./dummy_file", "r");
    if (tif) {
        TIFFClose(tif);
    }
    
    cleanup_dummy_file();
    return 0;
}