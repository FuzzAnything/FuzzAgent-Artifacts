// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFComputeTile at tif_tile.c:35:10 in tiffio.h
// TIFFComputeStrip at tif_strip.c:35:10 in tiffio.h
// TIFFCurrentStrip at tif_open.c:874:10 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFCurrentRow at tif_open.c:864:10 in tiffio.h
// TIFFCheckTile at tif_tile.c:71:5 in tiffio.h
// TIFFComputeTile at tif_tile.c:35:10 in tiffio.h
// TIFFComputeStrip at tif_strip.c:35:10 in tiffio.h
// TIFFCheckTile at tif_tile.c:71:5 in tiffio.h
// TIFFComputeTile at tif_tile.c:35:10 in tiffio.h
// TIFFComputeStrip at tif_strip.c:35:10 in tiffio.h
// TIFFCheckTile at tif_tile.c:71:5 in tiffio.h
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
    if (!fp) return;

    // Write a minimal valid TIFF header (BigTIFF or Classic)
    // Classic TIFF Header:
    // 2 bytes: Byte order (II = Little, MM = Big)
    // 2 bytes: Magic number (42)
    // 4 bytes: Offset to first IFD (8)
    uint8_t header[8];
    header[0] = 'I'; // Little Endian
    header[1] = 'I';
    header[2] = 42;
    header[3] = 0;
    // Offset to first IFD (8)
    header[4] = 8;
    header[5] = 0;
    header[6] = 0;
    header[7] = 0;

    fwrite(header, 1, 8, fp);

    // Write a minimal IFD (Image File Directory)
    // 2 bytes: Number of directory entries (1)
    // 12 bytes per entry (Tag, Type, Count, Value/Offset)
    // 4 bytes: Offset to next IFD (0)
    uint8_t ifd[18];
    uint16_t num_entries = 1;
    ifd[0] = num_entries & 0xFF;
    ifd[1] = (num_entries >> 8) & 0xFF;

    // Entry 1: ImageWidth (Tag 256, Type SHORT, Count 1, Value 64)
    ifd[2] = 256 & 0xFF;
    ifd[3] = (256 >> 8) & 0xFF;
    ifd[4] = 3; // SHORT
    ifd[5] = 0;
    ifd[6] = 1; // Count
    ifd[7] = 0;
    // Value (64)
    ifd[8] = 64 & 0xFF;
    ifd[9] = (64 >> 8) & 0xFF;
    ifd[10] = 0;
    ifd[11] = 0;

    // Next IFD offset (0)
    ifd[12] = 0;
    ifd[13] = 0;
    ifd[14] = 0;
    ifd[15] = 0;

    // Add ImageLength tag to make it more valid for some operations
    // We'll just overwrite the first entry to be ImageLength for simplicity or add a second
    // Let's add a second entry for ImageLength (Tag 257)
    // Actually, let's just write the data we have and hope libtiff handles minimal cases
    // Or better, write a slightly more complete IFD with ImageWidth and ImageLength
    // Re-write IFD with 2 entries
    ifd[0] = 2;
    ifd[1] = 0;
    
    // Entry 1: ImageWidth
    ifd[2] = 256 & 0xFF;
    ifd[3] = (256 >> 8) & 0xFF;
    ifd[4] = 3; // SHORT
    ifd[5] = 0;
    ifd[6] = 1;
    ifd[7] = 0;
    ifd[8] = 64 & 0xFF;
    ifd[9] = (64 >> 8) & 0xFF;
    ifd[10] = 0;
    ifd[11] = 0;

    // Entry 2: ImageLength
    ifd[12] = 257 & 0xFF;
    ifd[13] = (257 >> 8) & 0xFF;
    ifd[14] = 3; // SHORT
    ifd[15] = 0;
    ifd[16] = 1;
    ifd[17] = 0;
    // Value (64)
    // Need 4 more bytes for value
    uint8_t ifd_val[4];
    ifd_val[0] = 64; ifd_val[1] = 0; ifd_val[2] = 0; ifd_val[3] = 0;
    fwrite(ifd_val, 1, 4, fp);

    // Next IFD offset (0)
    fwrite("\0\0\0\0", 1, 4, fp);

    // Write some dummy image data
    size_t data_to_write = (Size > 100) ? 100 : Size;
    if (data_to_write > 0) {
        fwrite(Data, 1, data_to_write, fp);
    }

    fclose(fp);
}

int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a dummy TIFF file
    write_dummy_tiff(Data, Size);

    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        return 0;
    }

    // Read directory to initialize internal structures
    if (!TIFFReadDirectory(tif)) {
        TIFFClose(tif);
        return 0;
    }

    // Fuzz coordinates and sample indices
    uint32_t x = (Size > 0) ? Data[0] : 0;
    uint32_t y = (Size > 1) ? Data[1] : 0;
    uint32_t z = (Size > 2) ? Data[2] : 0;
    uint16_t s = (Size > 3) ? (uint16_t)Data[3] : 0;

    // Ensure some values are within reasonable bounds for testing, but also test out of bounds
    // We'll use the raw data for fuzzing and let the library handle errors

    // 1. TIFFComputeTile
    uint32_t tile_idx = TIFFComputeTile(tif, x, y, z, s);
    (void)tile_idx;

    // 2. TIFFComputeStrip
    uint32_t strip_idx = TIFFComputeStrip(tif, y, s);
    (void)strip_idx;

    // 3. TIFFCurrentStrip
    uint32_t cur_strip = TIFFCurrentStrip(tif);
    (void)cur_strip;

    // 4. TIFFNumberOfTiles
    uint32_t num_tiles = TIFFNumberOfTiles(tif);
    (void)num_tiles;

    // 5. TIFFCurrentRow
    uint32_t cur_row = TIFFCurrentRow(tif);
    (void)cur_row;

    // 6. TIFFCheckTile
    int tile_check = TIFFCheckTile(tif, x, y, z, s);
    (void)tile_check;

    // Try with modified coordinates to explore more states
    uint32_t x2 = (Size > 4) ? Data[4] : 0;
    uint32_t y2 = (Size > 5) ? Data[5] : 0;
    uint32_t z2 = (Size > 6) ? Data[6] : 0;
    uint16_t s2 = (Size > 7) ? (uint16_t)Data[7] : 0;

    (void)TIFFComputeTile(tif, x2, y2, z2, s2);
    (void)TIFFComputeStrip(tif, y2, s2);
    (void)TIFFCheckTile(tif, x2, y2, z2, s2);

    // Try with large values
    (void)TIFFComputeTile(tif, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFF);
    (void)TIFFComputeStrip(tif, 0xFFFFFFFF, 0xFFFF);
    (void)TIFFCheckTile(tif, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFF);

    TIFFClose(tif);

    return 0;
}