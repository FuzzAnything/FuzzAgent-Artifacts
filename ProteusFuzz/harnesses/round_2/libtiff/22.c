// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFCurrentDirectory at tif_open.c:869:8 in tiffio.h
// TIFFDefaultStripSize at tif_strip.c:218:10 in tiffio.h
// TIFFReadTile at tif_read.c:940:10 in tiffio.h
// TIFFReadTile at tif_read.c:940:10 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFVTileSize64 at tif_tile.c:188:10 in tiffio.h
// TIFFDefaultTileSize at tif_tile.c:267:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <tiffio.h>

static void write_dummy_tiff(const uint8_t *Data, size_t Size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return;
    }
    /* Write a minimal TIFF header (42 bytes) */
    /* Byte order (II = Little Endian) */
    uint16_t byte_order = 0x4949;
    fwrite(&byte_order, sizeof(uint16_t), 1, fp);
    /* TIFF version (42) */
    uint16_t version = 42;
    fwrite(&version, sizeof(uint16_t), 1, fp);
    /* Offset to first IFD (8 bytes from start) */
    uint32_t ifd_offset = 8;
    fwrite(&ifd_offset, sizeof(uint32_t), 1, fp);
    
    /* Write the fuzzing data as the IFD content (even if malformed, libtiff should handle it) */
    if (Size > 0) {
        fwrite(Data, 1, Size, fp);
    }
    
    fclose(fp);
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Step 1: Prepare the environment */
    write_dummy_tiff(Data, Size);

    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        return 0;
    }

    /* Ensure we have a valid directory context if possible */
    /* TIFFReadDirectory returns 0 on failure, but we proceed to test APIs regardless of success */
    TIFFReadDirectory(tif);

    /* Step 2 & 3: Explore program states by invoking target functions */
    
    /* 1. TIFFCurrentDirectory */
    tdir_t dir_idx = TIFFCurrentDirectory(tif);
    (void)dir_idx; /* Suppress unused variable warning */

    /* 2. TIFFDefaultStripSize */
    uint32_t strip_size = TIFFDefaultStripSize(tif, Size > 0 ? Size : 1024);
    (void)strip_size;

    /* 3. TIFFReadTile */
    /* Allocate a small buffer for tile reading */
    uint8_t *tile_buf = (uint8_t *)malloc(4096);
    if (tile_buf) {
        /* Try reading a tile at various coordinates */
        tmsize_t bytes_read = TIFFReadTile(tif, tile_buf, 0, 0, 0, 0);
        (void)bytes_read;
        
        /* Try with coordinates derived from input to explore different states */
        uint32_t x = Size > 0 ? Data[0] : 0;
        uint32_t y = Size > 1 ? Data[1] : 0;
        bytes_read = TIFFReadTile(tif, tile_buf, x, y, 0, 0);
        (void)bytes_read;
        
        free(tile_buf);
    }

    /* 4. TIFFNumberOfTiles */
    uint32_t num_tiles = TIFFNumberOfTiles(tif);
    (void)num_tiles;

    /* 5. TIFFVTileSize64 */
    uint64_t vtile_size = TIFFVTileSize64(tif, Size > 0 ? Size : 1);
    (void)vtile_size;

    /* 6. TIFFDefaultTileSize */
    uint32_t tile_w, tile_h;
    TIFFDefaultTileSize(tif, &tile_w, &tile_h);
    (void)tile_w;
    (void)tile_h;

    /* Step 2: Cleanup */
    TIFFClose(tif);

    return 0;
}