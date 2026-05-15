// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFComputeTile at tif_tile.c:35:10 in tiffio.h
// TIFFGetStrileByteCountWithErr at tif_dirread.c:8373:10 in tiffio.h
// TIFFComputeStrip at tif_strip.c:35:10 in tiffio.h
// TIFFCurrentStrip at tif_open.c:874:10 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress libtiff warnings/errors during fuzzing
}

int LLVMFuzzerTestOneInput_84(const uint8_t *Data, size_t Size) {
    // Set error handlers to suppress output
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandler(dummy_handler);
    
    // Write fuzzer input to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Open TIFF file
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        remove("./dummy_file");
        return 0;
    }
    
    // Extract parameters from fuzzer input
    // We'll use the first bytes to generate parameters for the API calls
    size_t pos = 0;
    
    // Parameters for TIFFComputeTile
    uint32_t x = 0, y = 0, z = 0;
    uint16_t s = 0;
    if (pos + sizeof(uint32_t) <= Size) {
        memcpy(&x, Data + pos, sizeof(uint32_t));
        pos += sizeof(uint32_t);
    }
    if (pos + sizeof(uint32_t) <= Size) {
        memcpy(&y, Data + pos, sizeof(uint32_t));
        pos += sizeof(uint32_t);
    }
    if (pos + sizeof(uint32_t) <= Size) {
        memcpy(&z, Data + pos, sizeof(uint32_t));
        pos += sizeof(uint32_t);
    }
    if (pos + sizeof(uint16_t) <= Size) {
        memcpy(&s, Data + pos, sizeof(uint16_t));
        pos += sizeof(uint16_t);
    }
    
    // Call TIFFComputeTile
    (void)TIFFComputeTile(tif, x, y, z, s);
    
    // Parameters for TIFFGetStrileByteCountWithErr
    uint32_t strile = 0;
    int pbErr = 0;
    if (pos + sizeof(uint32_t) <= Size) {
        memcpy(&strile, Data + pos, sizeof(uint32_t));
        pos += sizeof(uint32_t);
    }
    
    // Call TIFFGetStrileByteCountWithErr
    (void)TIFFGetStrileByteCountWithErr(tif, strile, &pbErr);
    
    // Parameters for TIFFComputeStrip
    uint32_t row = 0;
    uint16_t sample = 0;
    if (pos + sizeof(uint32_t) <= Size) {
        memcpy(&row, Data + pos, sizeof(uint32_t));
        pos += sizeof(uint32_t);
    }
    if (pos + sizeof(uint16_t) <= Size) {
        memcpy(&sample, Data + pos, sizeof(uint16_t));
        pos += sizeof(uint16_t);
    }
    
    // Call TIFFComputeStrip
    (void)TIFFComputeStrip(tif, row, sample);
    
    // Call TIFFCurrentStrip
    (void)TIFFCurrentStrip(tif);
    
    // Call TIFFNumberOfTiles
    (void)TIFFNumberOfTiles(tif);
    
    // Parameters for TIFFReadEncodedStrip
    uint32_t strip = 0;
    tmsize_t read_size = 0;
    if (pos + sizeof(uint32_t) <= Size) {
        memcpy(&strip, Data + pos, sizeof(uint32_t));
        pos += sizeof(uint32_t);
    }
    if (pos + sizeof(tmsize_t) <= Size) {
        memcpy(&read_size, Data + pos, sizeof(tmsize_t));
        pos += sizeof(tmsize_t);
    }
    
    // Allocate buffer for reading
    if (read_size > 0 && read_size < 1024 * 1024) { // Limit to 1MB to avoid OOM
        void *buf = malloc(read_size);
        if (buf) {
            // Call TIFFReadEncodedStrip
            (void)TIFFReadEncodedStrip(tif, strip, buf, read_size);
            free(buf);
        }
    }
    
    // Cleanup
    TIFFClose(tif);
    remove("./dummy_file");
    
    return 0;
}