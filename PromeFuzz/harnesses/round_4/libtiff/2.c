// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
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
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "tiffio.h"

static int write_to_file(const char* filename, const uint8_t* data, size_t size) {
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return -1;
    ssize_t written = write(fd, data, size);
    close(fd);
    return (written == (ssize_t)size) ? 0 : -1;
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0;
    
    const char* filename = "./dummy_file";
    if (write_to_file(filename, Data, Size) != 0) return 0;
    
    TIFF* tif = TIFFOpen(filename, "r+");
    if (!tif) {
        tif = TIFFOpen(filename, "w");
        if (!tif) return 0;
    }
    
    uint32_t width = 0, height = 0;
    uint16_t bitspersample = 0, samplesperpixel = 0, photometric = 0;
    uint32_t* raster = NULL;
    
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bitspersample);
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samplesperpixel);
    TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &photometric);
    
    TIFFSetSubDirectory(tif, 0);
    TIFFReadDirectory(tif);
    
    TIFFSetDirectory(tif, 0);
    TIFFSetSubDirectory(tif, 0);
    TIFFReadDirectory(tif);
    
    TIFFSetDirectory(tif, 0);
    TIFFReadDirectory(tif);
    
    tdir_t curdir = TIFFCurrentDirectory(tif);
    TIFFSetSubDirectory(tif, 0);
    
    tdir_t numdirs = TIFFNumberOfDirectories(tif);
    TIFFCreateDirectory(tif);
    
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 256);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 256);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, 32);
    
    TIFFWriteDirectory(tif);
    
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 128);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 128);
    
    TIFFCheckpointDirectory(tif);
    TIFFWriteDirectory(tif);
    TIFFCheckpointDirectory(tif);
    TIFFWriteDirectory(tif);
    
    TIFFClose(tif);
    TIFFClose(NULL);
    
    unlink(filename);
    return 0;
}