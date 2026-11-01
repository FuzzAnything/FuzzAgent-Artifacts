// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFCurrentTile at tif_open.c:879:10 in tiffio.h
// TIFFCurrentRow at tif_open.c:864:10 in tiffio.h
// TIFFCheckTile at tif_tile.c:71:5 in tiffio.h
// TIFFCheckTile at tif_tile.c:71:5 in tiffio.h
// TIFFCheckTile at tif_tile.c:71:5 in tiffio.h
// TIFFCurrentDirectory at tif_open.c:869:8 in tiffio.h
// TIFFCurrentStrip at tif_open.c:874:10 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
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

/*
 * Fuzz driver for libtiff API functions:
 * TIFFCurrentTile, TIFFCurrentRow, TIFFCheckTile,
 * TIFFCurrentDirectory, TIFFCurrentStrip, TIFFNumberOfTiles.
 *
 * This driver creates a dummy TIFF file, opens it, and exercises the
 * target functions with various inputs derived from the fuzz data.
 */

static void setup_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f == NULL) {
        return;
    }
    
    /* Write a minimal valid TIFF header (little-endian) */
    /* Byte order: II (0x4949) */
    uint16_t byte_order = 0x4949;
    fwrite(&byte_order, sizeof(uint16_t), 1, f);
    
    /* Magic number: 42 */
    uint16_t magic = 42;
    fwrite(&magic, sizeof(uint16_t), 1, f);
    
    /* Offset to first IFD: 8 (header size) */
    uint32_t ifd_offset = 8;
    fwrite(&ifd_offset, sizeof(uint32_t), 1, f);
    
    /* Write some dummy IFD data to make it look like a valid TIFF */
    /* Number of directory entries: 1 */
    uint16_t num_entries = 1;
    fwrite(&num_entries, sizeof(uint16_t), 1, f);
    
    /* Dummy tag entry: ImageWidth (256) */
    uint16_t tag = 256;
    uint16_t type = 3; /* SHORT */
    uint32_t count = 1;
    uint32_t value = 64; /* Width */
    fwrite(&tag, sizeof(uint16_t), 1, f);
    fwrite(&type, sizeof(uint16_t), 1, f);
    fwrite(&count, sizeof(uint32_t), 1, f);
    fwrite(&value, sizeof(uint32_t), 1, f);
    
    /* Next IFD offset: 0 (end of file) */
    uint32_t next_ifd = 0;
    fwrite(&next_ifd, sizeof(uint32_t), 1, f);
    
    /* If there is extra data, write it as image data */
    if (size > 0) {
        fwrite(data, 1, size, f);
    }
    
    fclose(f);
}

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Prepare the environment: create a dummy file */
    setup_dummy_file(Data, Size);

    /* Open the file for reading */
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (tif == NULL) {
        return 0;
    }

    /* Read the directory to initialize internal state */
    if (!TIFFReadDirectory(tif)) {
        TIFFClose(tif);
        return 0;
    }

    /* Extract some values from fuzz data for testing */
    uint32_t x = 0, y = 0, z = 0;
    uint16_t s = 0;
    
    if (Size >= 4) {
        x = Data[0];
        y = Data[1];
        z = Data[2];
        s = Data[3];
    } else if (Size >= 1) {
        x = Data[0];
        y = Data[0];
        z = Data[0];
        s = Data[0];
    }

    /* 1. TIFFCurrentTile */
    uint32_t tile_idx = TIFFCurrentTile(tif);
    (void)tile_idx; /* Use return value to avoid unused warning */

    /* 2. TIFFCurrentRow */
    uint32_t row_idx = TIFFCurrentRow(tif);
    (void)row_idx;

    /* 3. TIFFCheckTile */
    /* Test with fuzzed coordinates */
    int check_result = TIFFCheckTile(tif, x, y, z, s);
    (void)check_result;
    
    /* Test with extreme values */
    check_result = TIFFCheckTile(tif, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFF);
    (void)check_result;
    
    /* Test with zero values */
    check_result = TIFFCheckTile(tif, 0, 0, 0, 0);
    (void)check_result;

    /* 4. TIFFCurrentDirectory */
    tdir_t dir_idx = TIFFCurrentDirectory(tif);
    (void)dir_idx;

    /* 5. TIFFCurrentStrip */
    uint32_t strip_idx = TIFFCurrentStrip(tif);
    (void)strip_idx;

    /* 6. TIFFNumberOfTiles */
    uint32_t num_tiles = TIFFNumberOfTiles(tif);
    (void)num_tiles;

    /* Cleanup */
    TIFFClose(tif);

    return 0;
}