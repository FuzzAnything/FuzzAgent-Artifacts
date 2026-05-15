// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFVTileSize at tif_tile.c:238:10 in tiffio.h
// TIFFWriteRawTile at tif_write.c:533:10 in tiffio.h
// TIFFWriteEncodedTile at tif_write.c:414:10 in tiffio.h
// TIFFReadRawTile at tif_read.c:1176:10 in tiffio.h
// TIFFReadEncodedTile at tif_read.c:953:10 in tiffio.h
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

static void dummy_cleanup(void) {
    unlink("./dummy_file");
}

static int write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return -1;
    ssize_t written = write(fd, data, size);
    close(fd);
    return (written == (ssize_t)size) ? 0 : -1;
}

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    if (write_dummy_file(Data, Size) != 0) {
        return 0;
    }
    
    TIFF* tif = TIFFOpen("./dummy_file", "r+");
    if (!tif) {
        dummy_cleanup();
        return 0;
    }
    
    uint32_t tile_index = 0;
    if (Size >= 4) {
        tile_index = *(const uint32_t*)Data;
    }
    
    tmsize_t buffer_size = 4096;
    if (Size > 4) {
        buffer_size = (tmsize_t)(Size - 4);
        if (buffer_size > 65536) buffer_size = 65536;
    }
    
    void* buffer = malloc(buffer_size);
    if (!buffer) {
        TIFFClose(tif);
        dummy_cleanup();
        return 0;
    }
    
    if (Size > 4) {
        size_t copy_size = (Size - 4) > (size_t)buffer_size ? (size_t)buffer_size : (Size - 4);
        memcpy(buffer, Data + 4, copy_size);
    }
    
    tmsize_t tile_size = TIFFTileSize(tif);
    tmsize_t v_tile_size = TIFFVTileSize(tif, 1);
    
    tmsize_t raw_write_result = TIFFWriteRawTile(tif, tile_index, buffer, buffer_size);
    tmsize_t encoded_write_result = TIFFWriteEncodedTile(tif, tile_index, buffer, buffer_size);
    
    void* read_buffer = malloc(tile_size > 0 ? tile_size : 1024);
    if (read_buffer) {
        tmsize_t read_size = tile_size > 0 ? tile_size : 1024;
        tmsize_t raw_read_result = TIFFReadRawTile(tif, tile_index, read_buffer, read_size);
        tmsize_t encoded_read_result = TIFFReadEncodedTile(tif, tile_index, read_buffer, read_size);
        free(read_buffer);
    }
    
    free(buffer);
    TIFFClose(tif);
    dummy_cleanup();
    
    return 0;
}