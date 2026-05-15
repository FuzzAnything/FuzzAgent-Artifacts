// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFVTileSize at tif_tile.c:238:10 in tiffio.h
// TIFFReadEncodedTile at tif_read.c:953:10 in tiffio.h
// TIFFReadRawTile at tif_read.c:1176:10 in tiffio.h
// TIFFWriteEncodedTile at tif_write.c:414:10 in tiffio.h
// TIFFWriteRawTile at tif_write.c:533:10 in tiffio.h
// TIFFReadEncodedTile at tif_read.c:953:10 in tiffio.h
// TIFFReadRawTile at tif_read.c:1176:10 in tiffio.h
// TIFFVTileSize at tif_tile.c:238:10 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "tiffio.h"

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress libtiff warnings/errors during fuzzing
}

static TIFF* open_dummy_tiff(const uint8_t* data, size_t size, int* is_writable) {
    // Write data to dummy file
    FILE* fp = fopen("./dummy_file", "wb");
    if (!fp) return NULL;
    fwrite(data, 1, size, fp);
    fclose(fp);
    
    // Suppress error/warning messages
    TIFFSetWarningHandler(dummy_handler);
    TIFFSetErrorHandler(dummy_handler);
    
    // Open the dummy file
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        // Try opening in write mode if read fails
        tif = TIFFOpen("./dummy_file", "w");
        if (tif && is_writable) {
            *is_writable = 1;
        }
    } else if (is_writable) {
        *is_writable = 0;
    }
    return tif;
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_64(const uint8_t* Data, size_t Size) {
    if (Size < 4) return 0;
    
    int is_writable = 0;
    TIFF* tif = open_dummy_tiff(Data, Size, &is_writable);
    if (!tif) {
        cleanup_dummy_file();
        return 0;
    }
    
    // Use some data for parameters
    uint32_t tile = *(uint32_t*)Data % 1000;  // Limit tile number
    uint32_t nrows = *(uint32_t*)Data % 100;  // Limit rows
    
    // Allocate buffers
    tmsize_t tile_size = TIFFTileSize(tif);
    tmsize_t v_tile_size = TIFFVTileSize(tif, nrows);
    
    // Try different buffer sizes
    tmsize_t buf_sizes[] = {0, 1, 1024, 4096, 65536};
    int num_sizes = sizeof(buf_sizes)/sizeof(buf_sizes[0]);
    
    for (int i = 0; i < num_sizes; i++) {
        tmsize_t buf_size = buf_sizes[i];
        if (buf_size > 0 && buf_size < 1024*1024) {  // Limit to 1MB
            void* buf = malloc(buf_size);
            if (buf) {
                // Test read functions
                TIFFReadEncodedTile(tif, tile, buf, buf_size);
                TIFFReadRawTile(tif, tile, buf, buf_size);
                
                // Test write functions if file is writable
                if (is_writable) {
                    // Use part of fuzzer data as write buffer
                    size_t write_size = Size > 1024 ? 1024 : Size;
                    void* write_data = malloc(write_size);
                    if (write_data) {
                        memcpy(write_data, Data, write_size);
                        TIFFWriteEncodedTile(tif, tile, write_data, write_size);
                        TIFFWriteRawTile(tif, tile, write_data, write_size);
                        free(write_data);
                    }
                }
                free(buf);
            }
        }
    }
    
    // Test with NULL buffer
    TIFFReadEncodedTile(tif, tile, NULL, 0);
    TIFFReadRawTile(tif, tile, NULL, 0);
    
    // Test tile size functions again with different parameters
    for (uint32_t r = 0; r < 10; r++) {
        TIFFVTileSize(tif, r);
    }
    
    TIFFClose(tif);
    cleanup_dummy_file();
    return 0;
}