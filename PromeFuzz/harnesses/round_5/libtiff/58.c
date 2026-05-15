// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFRasterScanlineSize64 at tif_strip.c:357:10 in tiffio.h
// TIFFTileRowSize64 at tif_tile.c:140:10 in tiffio.h
// TIFFScanlineSize64 at tif_strip.c:257:10 in tiffio.h
// TIFFTileSize64 at tif_tile.c:249:10 in tiffio.h
// TIFFStripSize64 at tif_strip.c:196:10 in tiffio.h
// TIFFVStripSize64 at tif_strip.c:88:10 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFScanlineSize64 at tif_strip.c:257:10 in tiffio.h
// TIFFStripSize64 at tif_strip.c:196:10 in tiffio.h
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

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Silence warnings/errors during fuzzing
}

static int write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return -1;
    
    ssize_t written = write(fd, data, size);
    close(fd);
    
    return (written == (ssize_t)size) ? 0 : -1;
}

int LLVMFuzzerTestOneInput_58(const uint8_t *Data, size_t Size) {
    // Silence libtiff warnings/errors
    TIFFSetWarningHandler(dummy_handler);
    TIFFSetErrorHandler(dummy_handler);
    
    // Write fuzzer input to dummy file
    if (write_dummy_file(Data, Size) != 0) {
        return 0;
    }
    
    // Try to open the file as TIFF
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        unlink("./dummy_file");
        return 0;
    }
    
    // Call all target API functions
    uint64_t result;
    
    // 1. TIFFRasterScanlineSize64
    result = TIFFRasterScanlineSize64(tif);
    (void)result;
    
    // 2. TIFFTileRowSize64
    result = TIFFTileRowSize64(tif);
    (void)result;
    
    // 3. TIFFScanlineSize64
    result = TIFFScanlineSize64(tif);
    (void)result;
    
    // 4. TIFFTileSize64
    result = TIFFTileSize64(tif);
    (void)result;
    
    // 5. TIFFStripSize64
    result = TIFFStripSize64(tif);
    (void)result;
    
    // 6. TIFFVStripSize64 - try with different row counts
    for (uint32_t nrows = 0; nrows < 100; nrows += 10) {
        result = TIFFVStripSize64(tif, nrows);
        (void)result;
    }
    
    // Try to read some directory information to explore more states
    TIFFReadDirectory(tif);
    
    // Try to set different directories
    for (tdir_t dir = 0; dir < 10; ++dir) {
        TIFFSetDirectory(tif, dir);
        
        // Call functions again with different directory
        result = TIFFScanlineSize64(tif);
        (void)result;
        result = TIFFStripSize64(tif);
        (void)result;
    }
    
    // Cleanup
    TIFFClose(tif);
    unlink("./dummy_file");
    
    return 0;
}