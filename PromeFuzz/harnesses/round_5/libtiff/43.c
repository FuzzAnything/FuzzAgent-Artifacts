// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFVTileSize at tif_tile.c:238:10 in tiffio.h
// TIFFReadEncodedTile at tif_read.c:953:10 in tiffio.h
// TIFFReadRawTile at tif_read.c:1176:10 in tiffio.h
// TIFFWriteRawTile at tif_write.c:533:10 in tiffio.h
// TIFFWriteEncodedTile at tif_write.c:414:10 in tiffio.h
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
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0; // Need minimal data for meaningful fuzzing
    
    write_dummy_file(Data, Size);
    
    TIFF* tif = TIFFOpen("./dummy_file", "r+");
    if (!tif) {
        cleanup_dummy_file();
        return 0;
    }
    
    // Extract tile index and nrows from fuzzer data
    uint32_t tile = 0;
    uint32_t nrows = 1;
    if (Size >= 8) {
        memcpy(&tile, Data, 4);
        memcpy(&nrows, Data + 4, 4);
    }
    
    // Test TIFFTileSize
    tmsize_t tile_size = TIFFTileSize(tif);
    
    // Test TIFFVTileSize
    tmsize_t vtile_size = TIFFVTileSize(tif, nrows);
    
    // Allocate buffers for read/write operations
    void* read_buf = NULL;
    void* write_buf = NULL;
    
    if (tile_size > 0 && tile_size < 1024*1024) { // Reasonable size limit
        read_buf = malloc(tile_size);
        write_buf = malloc(tile_size);
        
        if (read_buf && write_buf) {
            // Fill write buffer with fuzzer data
            size_t copy_size = tile_size;
            if (Size - 8 < copy_size) copy_size = Size - 8;
            if (copy_size > 0 && Size > 8) {
                memcpy(write_buf, Data + 8, copy_size);
            }
            
            // Test TIFFReadEncodedTile
            TIFFReadEncodedTile(tif, tile, read_buf, tile_size);
            
            // Test TIFFReadRawTile
            TIFFReadRawTile(tif, tile, read_buf, tile_size);
            
            // Test TIFFWriteRawTile
            TIFFWriteRawTile(tif, tile, write_buf, copy_size);
            
            // Test TIFFWriteEncodedTile
            TIFFWriteEncodedTile(tif, tile, write_buf, copy_size);
        }
        
        free(read_buf);
        free(write_buf);
    }
    
    TIFFClose(tif);
    cleanup_dummy_file();
    
    return 0;
}