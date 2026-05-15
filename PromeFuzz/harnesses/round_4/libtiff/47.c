// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFVTileSize at tif_tile.c:238:10 in tiffio.h
// TIFFDefaultTileSize at tif_tile.c:267:6 in tiffio.h
// TIFFVTileSize64 at tif_tile.c:188:10 in tiffio.h
// TIFFCurrentTile at tif_open.c:879:10 in tiffio.h
// TIFFDefaultStripSize at tif_strip.c:218:10 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static TIFF* create_dummy_tiff(const uint8_t* data, size_t size) {
    FILE* fp = fopen("./dummy_file", "wb");
    if (!fp) return NULL;
    
    // Write a minimal valid TIFF header
    uint8_t header[] = {
        0x49, 0x49, 0x2A, 0x00,  // Little-endian, classic TIFF
        0x08, 0x00, 0x00, 0x00,  // First IFD offset
        0x00, 0x00              // Padding to make size 10
    };
    fwrite(header, 1, sizeof(header), fp);
    
    // Write some fuzzer data as IFD entries
    if (size > 0) {
        size_t to_write = size > 1024 ? 1024 : size;
        fwrite(data, 1, to_write, fp);
    }
    
    fclose(fp);
    
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        unlink("./dummy_file");
    }
    return tif;
}

int LLVMFuzzerTestOneInput_47(const uint8_t* Data, size_t Size) {
    if (Size < 4) return 0;
    
    TIFF* tif = create_dummy_tiff(Data, Size);
    if (!tif) return 0;
    
    // Extract parameters from fuzzer input
    uint32_t nrows = 0;
    uint32_t request = 0;
    
    if (Size >= 4) {
        memcpy(&nrows, Data, 4);
        nrows = nrows % 65536; // Limit to reasonable values
    }
    
    if (Size >= 8) {
        memcpy(&request, Data + 4, 4);
        request = request % 65536;
    }
    
    // Call target functions
    tmsize_t vtsize = TIFFVTileSize(tif, nrows);
    (void)vtsize; // Use result to avoid unused variable warning
    
    uint32_t tile_width = 0, tile_height = 0;
    TIFFDefaultTileSize(tif, &tile_width, &tile_height);
    
    uint64_t vtsize64 = TIFFVTileSize64(tif, nrows);
    (void)vtsize64;
    
    uint32_t current_tile = TIFFCurrentTile(tif);
    (void)current_tile;
    
    uint32_t strip_size = TIFFDefaultStripSize(tif, request);
    (void)strip_size;
    
    uint32_t num_tiles = TIFFNumberOfTiles(tif);
    (void)num_tiles;
    
    // Cleanup
    TIFFClose(tif);
    unlink("./dummy_file");
    
    return 0;
}