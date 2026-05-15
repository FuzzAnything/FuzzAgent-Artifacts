// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFWriteRawStrip at tif_write.c:328:10 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFComputeTile at tif_tile.c:35:10 in tiffio.h
// TIFFComputeStrip at tif_strip.c:35:10 in tiffio.h
// TIFFCurrentStrip at tif_open.c:874:10 in tiffio.h
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
    // Suppress libtiff warnings/errors during fuzzing
    (void)module;
    (void)fmt;
    (void)ap;
}

static TIFF* create_dummy_tiff(const uint8_t* data, size_t size) {
    // Create a dummy TIFF file with minimal valid structure
    int fd = open("./dummy_file", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return NULL;
    
    // Write minimal TIFF header (little-endian)
    uint8_t header[] = {
        0x49, 0x49, 0x2A, 0x00, // Little-endian, TIFF magic
        0x08, 0x00, 0x00, 0x00, // First IFD offset
        0x00, 0x00, // Entry count (will be filled)
        0x00, 0x00, 0x00, 0x00  // Next IFD offset
    };
    write(fd, header, sizeof(header));
    
    // Write some fuzz data
    if (size > 0) {
        write(fd, data, size > 4096 ? 4096 : size);
    }
    
    close(fd);
    
    // Open the file with libtiff
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        tif = TIFFOpen("./dummy_file", "w");
    }
    
    return tif;
}

int LLVMFuzzerTestOneInput_58(const uint8_t *Data, size_t Size) {
    // Set error handlers to suppress output
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandler(dummy_handler);
    
    // Create a dummy TIFF file with fuzz data
    TIFF* tif = create_dummy_tiff(Data, Size);
    if (!tif) {
        unlink("./dummy_file");
        return 0;
    }
    
    // Extract parameters from fuzz data
    uint32_t x = 0, y = 0, z = 0, row = 0, strip_num = 0;
    uint16_t sample = 0;
    tmsize_t cc = 0;
    
    if (Size >= 16) {
        x = *(uint32_t*)Data;
        y = *(uint32_t*)(Data + 4);
        z = *(uint32_t*)(Data + 8);
        sample = *(uint16_t*)(Data + 12);
        row = *(uint32_t*)(Data + 14) % 1000; // Limit to reasonable values
        strip_num = *(uint32_t*)(Data + 18) % 100;
        cc = *(tmsize_t*)(Data + 22) % 4096;
    } else {
        // Use small defaults if not enough data
        x = y = z = row = strip_num = 0;
        sample = 0;
        cc = Size > 0 ? Size : 100;
    }
    
    // Fuzz TIFFComputeTile
    if (tif) {
        uint32_t tile = TIFFComputeTile(tif, x, y, z, sample);
        (void)tile; // Result not used
    }
    
    // Fuzz TIFFComputeStrip
    if (tif) {
        uint32_t strip = TIFFComputeStrip(tif, row, sample);
        (void)strip; // Result not used
    }
    
    // Fuzz TIFFCurrentStrip
    if (tif) {
        uint32_t current_strip = TIFFCurrentStrip(tif);
        (void)current_strip; // Result not used
    }
    
    // Fuzz TIFFWriteRawStrip
    if (tif && Size > 0) {
        void* buf = malloc(cc > 0 ? cc : 1);
        if (buf) {
            size_t copy_size = cc < Size ? cc : Size;
            memcpy(buf, Data, copy_size);
            tmsize_t written = TIFFWriteRawStrip(tif, strip_num, buf, cc);
            (void)written; // Result not used
            free(buf);
        }
    }
    
    // Fuzz TIFFNumberOfTiles
    if (tif) {
        uint32_t num_tiles = TIFFNumberOfTiles(tif);
        (void)num_tiles; // Result not used
    }
    
    // Fuzz TIFFReadEncodedStrip
    if (tif && Size > 0) {
        void* buf = malloc(cc > 0 ? cc : 1);
        if (buf) {
            tmsize_t read = TIFFReadEncodedStrip(tif, strip_num, buf, cc);
            (void)read; // Result not used
            free(buf);
        }
    }
    
    // Cleanup
    if (tif) {
        TIFFClose(tif);
    }
    unlink("./dummy_file");
    
    return 0;
}