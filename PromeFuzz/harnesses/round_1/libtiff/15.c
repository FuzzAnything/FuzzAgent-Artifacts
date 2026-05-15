// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFCurrentDirOffset at tif_dir.c:2223:10 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFCurrentDirOffset at tif_dir.c:2223:10 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

static void dummyErrorHandler(const char* module, const char* fmt, va_list ap) {
    // Do nothing - suppress error output during fuzzing
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    // Create a dummy file for TIFF operations
    const char* filename = "./dummy_file";
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return 0;
    }
    
    // Write fuzzer data to the file
    if (Size > 0) {
        write(fd, Data, Size);
    }
    close(fd);
    
    // Open the TIFF file for writing
    TIFF* tif = TIFFOpen(filename, "w");
    if (!tif) {
        unlink(filename);
        return 0;
    }
    
    // Set some initial fields to make the TIFF valid
    uint32_t width = 1;
    uint32_t height = 1;
    uint16_t samples = 1;
    uint16_t bits = 8;
    uint16_t photometric = PHOTOMETRIC_MINISBLACK;
    uint16_t planar = PLANARCONFIG_CONTIG;
    
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planar);
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, height);
    
    // First TIFFWriteDirectory
    TIFFWriteDirectory(tif);
    
    // TIFFSetSubDirectory - try with various offsets
    uint64_t offset = 0;
    if (Size >= sizeof(uint64_t)) {
        memcpy(&offset, Data, sizeof(uint64_t));
    }
    TIFFSetSubDirectory(tif, offset);
    
    // TIFFSetField - try setting various fields
    uint32_t tag = TIFFTAG_IMAGEWIDTH;
    if (Size >= sizeof(uint32_t)) {
        memcpy(&tag, Data, sizeof(uint32_t));
    }
    TIFFSetField(tif, tag, 100);
    
    // First TIFFSetErrorHandler
    TIFFSetErrorHandler(dummyErrorHandler);
    
    // Second TIFFWriteDirectory
    TIFFWriteDirectory(tif);
    
    // Second TIFFSetErrorHandler
    TIFFSetErrorHandler(NULL);
    
    // TIFFSetDirectory - try with various directory indices
    tdir_t dir = 0;
    if (Size >= sizeof(tdir_t)) {
        memcpy(&dir, Data, sizeof(tdir_t));
    }
    TIFFSetDirectory(tif, dir);
    
    // First TIFFCurrentDirOffset
    TIFFCurrentDirOffset(tif);
    
    // Second TIFFSetDirectory
    tdir_t dir2 = 1;
    if (Size >= sizeof(tdir_t) * 2) {
        memcpy(&dir2, Data + sizeof(tdir_t), sizeof(tdir_t));
    }
    TIFFSetDirectory(tif, dir2);
    
    // Second TIFFCurrentDirOffset
    TIFFCurrentDirOffset(tif);
    
    // Second TIFFSetSubDirectory
    uint64_t offset2 = 100;
    if (Size >= sizeof(uint64_t) * 2) {
        memcpy(&offset2, Data + sizeof(uint64_t), sizeof(uint64_t));
    }
    TIFFSetSubDirectory(tif, offset2);
    
    // First TIFFClose
    TIFFClose(tif);
    
    // Open another TIFF file for the second close
    TIFF* tif2 = TIFFOpen(filename, "r");
    if (tif2) {
        // Second TIFFClose
        TIFFClose(tif2);
    }
    
    // Clean up
    unlink(filename);
    
    return 0;
}