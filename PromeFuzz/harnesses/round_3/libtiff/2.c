// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
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
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFCheckpointDirectory at tif_dirwrite.c:292:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFCheckpointDirectory at tif_dirwrite.c:292:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
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
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return;
    fwrite(data, 1, size, f);
    fclose(f);
}

static void remove_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFF *tif = TIFFOpen("./dummy_file", "r+");
    if (!tif) {
        remove_dummy_file();
        return 0;
    }
    
    uint32_t width, height;
    uint16_t bitspersample, compression, photometric;
    
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bitspersample);
    TIFFGetField(tif, TIFFTAG_COMPRESSION, &compression);
    TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &photometric);
    
    uint64_t offset = *(uint64_t*)Data;
    TIFFSetSubDirectory(tif, offset);
    
    TIFFReadDirectory(tif);
    
    tdir_t dir_idx = *(tdir_t*)(Data + 8) % 10;
    TIFFSetDirectory(tif, dir_idx);
    
    offset = *(uint64_t*)(Data + 16);
    TIFFSetSubDirectory(tif, offset);
    
    TIFFReadDirectory(tif);
    
    dir_idx = *(tdir_t*)(Data + 24) % 10;
    TIFFSetDirectory(tif, dir_idx);
    
    TIFFReadDirectory(tif);
    
    tdir_t cur_dir = TIFFCurrentDirectory(tif);
    
    offset = *(uint64_t*)(Data + 32);
    TIFFSetSubDirectory(tif, offset);
    
    tdir_t num_dirs = TIFFNumberOfDirectories(tif);
    
    TIFFCreateDirectory(tif);
    
    width = *(uint32_t*)(Data + 40);
    height = *(uint32_t*)(Data + 44);
    bitspersample = *(uint16_t*)(Data + 48);
    compression = *(uint16_t*)(Data + 50);
    photometric = *(uint16_t*)(Data + 52);
    
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bitspersample);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
    
    TIFFWriteDirectory(tif);
    
    width = *(uint32_t*)(Data + 54);
    height = *(uint32_t*)(Data + 58);
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    
    TIFFCheckpointDirectory(tif);
    
    TIFFWriteDirectory(tif);
    
    TIFFCheckpointDirectory(tif);
    
    TIFFWriteDirectory(tif);
    
    TIFFClose(tif);
    
    remove_dummy_file();
    
    return 0;
}