// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFRasterScanlineSize at tif_strip.c:375:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFRawStripSize at tif_strip.c:168:10 in tiffio.h
// TIFFScanlineSize at tif_strip.c:343:10 in tiffio.h
// TIFFTileRowSize at tif_tile.c:177:10 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
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
    // Byte 0-1: Magic number (0x002A for Little Endian)
    uint16_t magic = 0x002A;
    fwrite(&magic, sizeof(magic), 1, fp);

    // Byte 2-3: Offset to first IFD (we will put it at 0x08)
    uint32_t ifd_offset = 8;
    fwrite(&ifd_offset, sizeof(ifd_offset), 1, fp);

    // Padding to reach offset 8
    uint8_t padding[4] = {0, 0, 0, 0};
    fwrite(padding, sizeof(padding), 1, fp);

    // IFD Entry: Number of entries (0 for empty, but let's try 1 to be safe or 0)
    // We'll write 0 entries to keep it simple, or 1 if we have data to construct a tag.
    // For fuzzing robustness, let's write 0 entries to ensure valid structure.
    uint16_t num_entries = 0;
    fwrite(&num_entries, sizeof(num_entries), 1, fp);

    // Next IFD offset (0 means end of list)
    uint32_t next_ifd = 0;
    fwrite(&next_ifd, sizeof(next_ifd), 1, fp);

    // If we have extra data, append it to the file to potentially confuse parsers
    if (Data && Size > 0) {
        fwrite(Data, 1, Size, fp);
    }

    fclose(fp);
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure we have a file to open
    write_dummy_tiff(Data, Size);

    // Open the dummy file
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        return 0;
    }

    // Fuzzing logic for target APIs

    // 1. TIFFTileSize
    // Returns size of a single tile. 0 for non-tiled or error.
    tmsize_t tile_size = TIFFTileSize(tif);
    (void)tile_size; // Suppress unused variable warning

    // 2. TIFFRawStripSize
    // Returns uncompressed size of a specific strip.
    // We try a few strip indices based on input data to explore states.
    uint32_t strip_index = 0;
    if (Size >= 4) {
        strip_index = *(const uint32_t *)Data;
    }
    tmsize_t raw_strip_size = TIFFRawStripSize(tif, strip_index);
    (void)raw_strip_size;

    // 3. TIFFScanlineSize
    // Returns byte size of a single scanline.
    tmsize_t scanline_size = TIFFScanlineSize(tif);
    (void)scanline_size;

    // 4. TIFFTileRowSize
    // Returns byte size of a single tile row.
    tmsize_t tile_row_size = TIFFTileRowSize(tif);
    (void)tile_row_size;

    // 5. _TIFFmalloc
    // Allocate memory based on input data to test allocation logic.
    // Limit size to prevent OOM during fuzzing.
    tmsize_t alloc_size = 0;
    if (Size >= 4) {
        alloc_size = *(const uint32_t *)(Data + (Size > 4 ? 4 : 0));
    }
    // Cap allocation size for safety
    if (alloc_size > 1024 * 1024) { // 1MB limit
        alloc_size = 1024 * 1024;
    }
    void *ptr = _TIFFmalloc(alloc_size);
    if (ptr) {
        // Write some data to the allocated buffer to exercise memory
        memset(ptr, 0xAA, alloc_size);
        _TIFFfree(ptr);
    }

    // 6. TIFFRasterScanlineSize
    // Returns scanline byte size.
    tmsize_t raster_scanline_size = TIFFRasterScanlineSize(tif);
    (void)raster_scanline_size;

    // Cleanup
    TIFFClose(tif);

    return 0;
}