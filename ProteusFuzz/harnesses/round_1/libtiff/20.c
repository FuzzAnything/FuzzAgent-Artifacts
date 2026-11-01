// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFReadTile at tif_read.c:940:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFCurrentDirectory at tif_open.c:869:8 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFDefaultStripSize at tif_strip.c:218:10 in tiffio.h
// TIFFDefaultTileSize at tif_tile.c:267:6 in tiffio.h
// TIFFVTileSize64 at tif_tile.c:188:10 in tiffio.h
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
    if (!fp) return;

    // Write a minimal valid TIFF header (Little Endian)
    // Magic number (0x002A)
    uint16_t magic = 0x002A;
    fwrite(&magic, sizeof(uint16_t), 1, fp);

    // Offset to first IFD (8 bytes)
    uint32_t ifd_offset = 8;
    fwrite(&ifd_offset, sizeof(uint32_t), 1, fp);

    // Write IFD
    // Number of directory entries (1)
    uint16_t num_entries = 1;
    fwrite(&num_entries, sizeof(uint16_t), 1, fp);

    // Tag: ImageWidth (256)
    uint16_t tag = 256;
    fwrite(&tag, sizeof(uint16_t), 1, fp);
    // Type: SHORT (3)
    uint16_t type = 3;
    fwrite(&type, sizeof(uint16_t), 1, fp);
    // Count: 1
    uint32_t count = 1;
    fwrite(&count, sizeof(uint32_t), 1, fp);
    // Value: 64 (width)
    uint16_t value = 64;
    fwrite(&value, sizeof(uint16_t), 1, fp);

    // Offset to next IFD (0)
    uint32_t next_ifd = 0;
    fwrite(&next_ifd, sizeof(uint32_t), 1, fp);

    // Pad to align if necessary and write image data if Size allows
    // We just write the input data as raw image data starting after the header
    if (Size > 0) {
        fwrite(Data, 1, Size, fp);
    }

    fclose(fp);
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy TIFF file from input data
    write_dummy_tiff(Data, Size);

    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        return 0;
    }

    // Attempt to read directory
    if (!TIFFReadDirectory(tif)) {
        TIFFClose(tif);
        return 0;
    }

    // Fuzz TIFFCurrentDirectory
    tdir_t dir = TIFFCurrentDirectory(tif);
    (void)dir;

    // Fuzz TIFFNumberOfTiles
    uint32_t num_tiles = TIFFNumberOfTiles(tif);
    (void)num_tiles;

    // Fuzz TIFFDefaultStripSize
    uint32_t strip_size = TIFFDefaultStripSize(tif, Size > 0 ? Size : 1);
    (void)strip_size;

    // Fuzz TIFFDefaultTileSize
    uint32_t tile_w = 0, tile_h = 0;
    TIFFDefaultTileSize(tif, &tile_w, &tile_h);

    // Fuzz TIFFVTileSize64
    uint64_t vtile_size = TIFFVTileSize64(tif, tile_h > 0 ? tile_h : 1);
    (void)vtile_size;

    // Fuzz TIFFReadTile
    // Only attempt if tiling is enabled and we have valid dimensions
    if (tile_w > 0 && tile_h > 0 && vtile_size > 0) {
        void *buf = malloc(vtile_size);
        if (buf) {
            // Try reading a few tiles with varying coordinates
            for (uint32_t x = 0; x < 4; x++) {
                for (uint32_t y = 0; y < 4; y++) {
                    tmsize_t res = TIFFReadTile(tif, buf, x, y, 0, 0);
                    if (res < 0) {
                        break;
                    }
                }
            }
            free(buf);
        }
    }

    TIFFClose(tif);
    return 0;
}