// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFRawStripSize at tif_strip.c:168:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFVStripSize at tif_strip.c:142:10 in tiffio.h
// TIFFWriteEncodedStrip at tif_write.c:215:10 in tiffio.h
// TIFFVTileSize at tif_tile.c:238:10 in tiffio.h
// TIFFWriteEncodedTile at tif_write.c:414:10 in tiffio.h
// TIFFReadEncodedTile at tif_read.c:953:10 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "tiffio.h"

int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
    if (Size < 12) {
        return 0;
    }

    // Write fuzzer input to a dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Open the dummy TIFF file
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        // Try opening in write mode if read fails
        tif = TIFFOpen("./dummy_file", "w");
        if (!tif) {
            return 0;
        }
    }

    // Extract parameters from fuzzer input
    uint32_t param1 = *(uint32_t*)Data;
    uint32_t param2 = *(uint32_t*)(Data + 4);
    
    // Adjust parameters to reasonable ranges
    param1 = param1 % 1000 + 1;
    param2 = param2 % 100;

    // 1. Test TIFFVStripSize
    tmsize_t vstrip_size = TIFFVStripSize(tif, param1);
    (void)vstrip_size;

    // 2. Test TIFFWriteEncodedStrip
    size_t buffer_size = 1024;
    if (Size > 8) {
        buffer_size = (Size - 8) % 8192 + 1;
    }
    void *write_buffer = malloc(buffer_size);
    if (write_buffer) {
        // Fill buffer with fuzzer data, ensuring we don't read beyond Data
        size_t copy_size = buffer_size;
        if (Size > 8) {
            size_t available = Size - 8;
            copy_size = buffer_size < available ? buffer_size : available;
            memcpy(write_buffer, Data + 8, copy_size);
        }
        
        tmsize_t written = TIFFWriteEncodedStrip(tif, param2, write_buffer, 
                                                (tmsize_t)buffer_size);
        (void)written;
        free(write_buffer);
    }

    // 3. Test TIFFVTileSize
    tmsize_t vtile_size = TIFFVTileSize(tif, param1);
    (void)vtile_size;

    // 4. Test TIFFWriteEncodedTile
    size_t tile_buffer_size = 512;
    if (Size > 12) {
        tile_buffer_size = (Size - 12) % 4096 + 1;
    }
    void *tile_buffer = malloc(tile_buffer_size);
    if (tile_buffer) {
        // Fill buffer with fuzzer data, ensuring we don't read beyond Data
        size_t copy_size = tile_buffer_size;
        if (Size > 12) {
            size_t available = Size - 12;
            copy_size = tile_buffer_size < available ? tile_buffer_size : available;
            memcpy(tile_buffer, Data + 12, copy_size);
        }
        
        tmsize_t tile_written = TIFFWriteEncodedTile(tif, param2, tile_buffer,
                                                    (tmsize_t)tile_buffer_size);
        (void)tile_written;
        free(tile_buffer);
    }

    // 5. Test TIFFReadEncodedTile
    size_t read_buffer_size = 2048;
    void *read_buffer = malloc(read_buffer_size);
    if (read_buffer) {
        tmsize_t read_bytes = TIFFReadEncodedTile(tif, param2, read_buffer,
                                                 (tmsize_t)read_buffer_size);
        (void)read_bytes;
        free(read_buffer);
    }

    // 6. Test TIFFRawStripSize
    tmsize_t raw_strip_size = TIFFRawStripSize(tif, param2);
    (void)raw_strip_size;

    // Clean up
    TIFFClose(tif);
    
    return 0;
}