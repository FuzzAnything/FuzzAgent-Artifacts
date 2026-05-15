// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpenOptionsAlloc at tif_open.c:80:18 in tiffio.h
// TIFFOpenOptionsSetMaxSingleMemAlloc at tif_open.c:94:6 in tiffio.h
// TIFFOpenExt at tif_unix.c:237:7 in tiffio.h
// TIFFOpenOptionsFree at tif_open.c:87:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpenExt at tif_unix.c:237:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
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

static int write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return -1;
    ssize_t written = write(fd, data, size);
    close(fd);
    return (written == (ssize_t)size) ? 0 : -1;
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Write fuzzer input to dummy file
    if (write_dummy_file(Data, Size) != 0) {
        return 0;
    }
    
    // Allocate TIFFOpenOptions
    TIFFOpenOptions *opts = TIFFOpenOptionsAlloc();
    if (!opts) {
        unlink("./dummy_file");
        return 0;
    }
    
    // Set max single memory allocation using first byte of input
    tmsize_t max_alloc = (tmsize_t)(Data[0] * 1024);
    TIFFOpenOptionsSetMaxSingleMemAlloc(opts, max_alloc);
    
    // Open TIFF file with options
    TIFF *tif = TIFFOpenExt("./dummy_file", "r", opts);
    
    // Free options regardless of whether open succeeded
    TIFFOpenOptionsFree(opts);
    
    // If open failed, clean up and return
    if (!tif) {
        unlink("./dummy_file");
        return 0;
    }
    
    // Close the TIFF handle (first close)
    TIFFClose(tif);
    
    // Re-open for writing to test TIFFSetField and TIFFWriteDirectory
    tif = TIFFOpenExt("./dummy_file", "w", NULL);
    if (!tif) {
        unlink("./dummy_file");
        return 0;
    }
    
    // Set some fields using fuzzer data
    // Use modulo to get reasonable values
    uint32_t width = (Size > 1) ? (Data[1] % 1000 + 1) : 100;
    uint32_t height = (Size > 2) ? (Data[2] % 1000 + 1) : 100;
    uint16_t samples = (Size > 3) ? ((Data[3] % 4) + 1) : 1;
    uint16_t bits = (Size > 4) ? ((Data[4] % 4) * 8 + 8) : 8;
    
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits);
    TIFFSetField(tif, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
    
    // Write directory
    TIFFWriteDirectory(tif);
    
    // Close TIFF
    TIFFClose(tif);
    
    // Clean up dummy file
    unlink("./dummy_file");
    
    return 0;
}