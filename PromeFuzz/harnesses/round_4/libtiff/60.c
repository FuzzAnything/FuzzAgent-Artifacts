// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFCurrentStrip at tif_open.c:874:10 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFWriteScanline at tif_write.c:47:5 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFComputeTile at tif_tile.c:35:10 in tiffio.h
// TIFFGetStrileByteCountWithErr at tif_dirread.c:8373:10 in tiffio.h
// TIFFComputeStrip at tif_strip.c:35:10 in tiffio.h
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
    // Silence libtiff warnings/errors during fuzzing
}

static TIFF* create_test_tiff(const uint8_t* data, size_t size) {
    // Create a dummy file with the fuzzer input
    int fd = open("./dummy_file", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return NULL;
    
    write(fd, data, size);
    close(fd);
    
    // Set error handlers to silent
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandler(dummy_handler);
    
    // Try to open as TIFF
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        // If opening as TIFF fails, try creating a minimal valid TIFF
        tif = TIFFOpen("./dummy_file", "w");
        if (tif) {
            // Set minimal required fields for a valid TIFF
            uint32_t width = 256;
            uint32_t height = 256;
            uint16_t samplesperpixel = 1;
            uint16_t bitspersample = 8;
            uint16_t photometric = PHOTOMETRIC_MINISBLACK;
            uint16_t planarconfig = PLANARCONFIG_CONTIG;
            uint16_t compression = COMPRESSION_NONE;
            
            TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
            TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
            TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samplesperpixel);
            TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bitspersample);
            TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
            TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planarconfig);
            TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);
            TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, 16);
            
            // Write some dummy data
            unsigned char* buf = (unsigned char*)_TIFFmalloc(width);
            if (buf) {
                for (uint32_t row = 0; row < height; row++) {
                    memset(buf, row % 256, width);
                    TIFFWriteScanline(tif, buf, row, 0);
                }
                _TIFFfree(buf);
            }
            
            TIFFClose(tif);
            tif = TIFFOpen("./dummy_file", "r");
        }
    }
    
    return tif;
}

int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    TIFF* tif = create_test_tiff(Data, Size);
    if (!tif) {
        unlink("./dummy_file");
        return 0;
    }
    
    // Extract some parameters from fuzzer input
    // Use modulo to keep values in reasonable ranges
    uint32_t param1 = *(uint32_t*)Data % 1024;
    uint32_t param2 = *(uint32_t*)(Data + 4) % 1024;
    uint32_t param3 = 0;
    uint16_t param4 = *(uint16_t*)(Data + 8) % 256;
    
    // Call target functions
    // 1. TIFFComputeTile
    uint32_t tile = TIFFComputeTile(tif, param1, param2, param3, param4);
    
    // 2. TIFFGetStrileByteCountWithErr
    int err = 0;
    uint32_t strile = param1 % 100;
    uint64_t bytecount = TIFFGetStrileByteCountWithErr(tif, strile, &err);
    
    // 3. TIFFComputeStrip
    uint32_t row = param1 % 1000;
    uint16_t sample = param4 % 4;
    uint32_t strip = TIFFComputeStrip(tif, row, sample);
    
    // 4. TIFFCurrentStrip
    uint32_t current_strip = TIFFCurrentStrip(tif);
    
    // 5. TIFFNumberOfTiles
    uint32_t num_tiles = TIFFNumberOfTiles(tif);
    
    // 6. TIFFReadEncodedStrip
    if (strip < 1000) {  // Avoid huge allocations
        tmsize_t strip_size = TIFFStripSize(tif);
        if (strip_size > 0 && strip_size < 10*1024*1024) {  // Limit to 10MB
            void* buf = malloc(strip_size);
            if (buf) {
                tmsize_t read_size = TIFFReadEncodedStrip(tif, strip, buf, strip_size);
                free(buf);
            }
        }
    }
    
    // Cleanup
    TIFFClose(tif);
    unlink("./dummy_file");
    
    return 0;
}