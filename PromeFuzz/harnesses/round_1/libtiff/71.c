// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFWriteTile at tif_write.c:387:10 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFReadTile at tif_read.c:940:10 in tiffio.h
// TIFFReadRawTile at tif_read.c:1176:10 in tiffio.h
// TIFFIsTiled at tif_open.c:859:5 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFIsTiled at tif_open.c:859:5 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static TIFF* create_test_tiff(const uint8_t* data, size_t size, const char* mode) {
    FILE* fp = fopen("./dummy_file", "wb");
    if (!fp) return NULL;
    fwrite(data, 1, size, fp);
    fclose(fp);
    
    TIFF* tif = TIFFOpen("./dummy_file", mode);
    return tif;
}

static void cleanup_tiff(TIFF* tif) {
    if (tif) {
        TIFFClose(tif);
    }
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_71(const uint8_t* Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create TIFF for reading operations
    TIFF* tif_read = create_test_tiff(Data, Size, "r");
    if (!tif_read) {
        remove("./dummy_file");
        return 0;
    }
    
    // Test TIFFNumberOfTiles
    uint32_t num_tiles = TIFFNumberOfTiles(tif_read);
    
    // Test TIFFTileSize
    tmsize_t tile_size = TIFFTileSize(tif_read);
    
    // Test tile-related functions if we have tiles
    if (num_tiles > 0 && tile_size > 0) {
        // Allocate buffer for tile operations
        void* tile_buf = malloc(tile_size);
        if (tile_buf) {
            // Test TIFFReadTile with various coordinates
            // Use data-derived values for coordinates, modulo reasonable bounds
            uint32_t x = (Data[0] * 256 + Data[1 % Size]) % 1024;
            uint32_t y = (Data[2 % Size] * 256 + Data[3 % Size]) % 1024;
            uint32_t z = Data[4 % Size] % 256;
            uint16_t s = Data[5 % Size] % 16;
            
            TIFFReadTile(tif_read, tile_buf, x, y, z, s);
            
            // Test TIFFReadRawTile with random tile index
            uint32_t tile_idx = (Data[6 % Size] * 256 + Data[7 % Size]) % num_tiles;
            TIFFReadRawTile(tif_read, tile_idx, tile_buf, tile_size);
            
            free(tile_buf);
        }
    }
    
    // Test strip-related functions
    if (TIFFIsTiled(tif_read) == 0) {  // Only for non-tiled (strip-based) images
        uint32_t num_strips = TIFFNumberOfStrips(tif_read);
        if (num_strips > 0) {
            tmsize_t strip_size = TIFFStripSize(tif_read);
            if (strip_size > 0) {
                void* strip_buf = malloc(strip_size);
                if (strip_buf) {
                    uint32_t strip_idx = (Data[8 % Size] * 256 + Data[9 % Size]) % num_strips;
                    // Test with different size values
                    tmsize_t read_size = strip_size;
                    if (Size > 10) {
                        read_size = (Data[10 % Size] * 256 + Data[11 % Size]) % (strip_size * 2);
                    }
                    TIFFReadEncodedStrip(tif_read, strip_idx, strip_buf, read_size);
                    free(strip_buf);
                }
            }
        }
    }
    
    cleanup_tiff(tif_read);
    
    // Create TIFF for writing operations
    TIFF* tif_write = create_test_tiff(Data, Size, "r+");
    if (!tif_write) {
        return 0;
    }
    
    // Test TIFFWriteTile if image is tiled
    if (TIFFIsTiled(tif_write)) {
        tile_size = TIFFTileSize(tif_write);
        if (tile_size > 0) {
            void* write_buf = malloc(tile_size);
            if (write_buf) {
                // Initialize buffer with fuzz data
                size_t copy_size = tile_size < Size ? tile_size : Size;
                memcpy(write_buf, Data, copy_size);
                
                // Use different coordinates for writing
                uint32_t wx = (Data[12 % Size] * 256 + Data[13 % Size]) % 1024;
                uint32_t wy = (Data[14 % Size] * 256 + Data[15 % Size]) % 1024;
                uint32_t wz = Data[16 % Size] % 256;
                uint16_t ws = Data[17 % Size] % 16;
                
                TIFFWriteTile(tif_write, write_buf, wx, wy, wz, ws);
                free(write_buf);
            }
        }
    }
    
    cleanup_tiff(tif_write);
    
    return 0;
}