// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFGetStrileByteCount at tif_dirread.c:8366:10 in tiffio.h
// TIFFGetStrileByteCount at tif_dirread.c:8366:10 in tiffio.h
// TIFFStripSize64 at tif_strip.c:196:10 in tiffio.h
// TIFFScanlineSize64 at tif_strip.c:257:10 in tiffio.h
// TIFFTileRowSize64 at tif_tile.c:140:10 in tiffio.h
// TIFFTileSize64 at tif_tile.c:249:10 in tiffio.h
// TIFFVStripSize64 at tif_strip.c:88:10 in tiffio.h
// TIFFVStripSize64 at tif_strip.c:88:10 in tiffio.h
// TIFFVStripSize64 at tif_strip.c:88:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "tiffio.h"

static void write_dummy_tiff(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return;
    
    // Write a minimal valid TIFF header (little endian)
    // Magic number: 0x002A (little endian)
    uint16_t magic = 0x002A;
    fwrite(&magic, sizeof(magic), 1, f);
    
    // Offset to first IFD (we'll put it at 8 + size of dummy data if any, or just 8)
    uint32_t ifd_offset = 8;
    fwrite(&ifd_offset, sizeof(ifd_offset), 1, f);
    
    // Write dummy data if provided to make the file larger and potentially trigger edge cases
    if (data && size > 0) {
        fwrite(data, 1, size, f);
    }
    
    // Write a minimal IFD (0 entries, next IFD offset 0)
    uint16_t num_entries = 0;
    fwrite(&num_entries, sizeof(num_entries), 1, f);
    uint32_t next_ifd = 0;
    fwrite(&next_ifd, sizeof(next_ifd), 1, f);
    
    fclose(f);
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    // Ensure we have some data to write a file
    if (Size == 0) {
        return 0;
    }

    // Create a dummy TIFF file
    write_dummy_tiff(Data, Size);

    // Open the file in read mode
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        return 0;
    }

    // Attempt to read the directory. If it fails, the handle is still valid for some operations
    // but size calculations might return 0. This is expected behavior for fuzzing.
    if (TIFFReadDirectory(tif) == 0) {
        // Directory read failed, but we can still try the functions to test error handling paths
    }

    // Fuzz TIFFGetStrileByteCount
    // Try a range of strile indices to explore different states
    for (uint32_t i = 0; i < 10; i++) {
        uint64_t count = TIFFGetStrileByteCount(tif, i);
        (void)count; // Use the result to avoid unused variable warnings
    }
    // Try with a large index
    TIFFGetStrileByteCount(tif, 0xFFFFFFFF);

    // Fuzz TIFFStripSize64
    uint64_t strip_size = TIFFStripSize64(tif);
    (void)strip_size;

    // Fuzz TIFFScanlineSize64
    uint64_t scanline_size = TIFFScanlineSize64(tif);
    (void)scanline_size;

    // Fuzz TIFFTileRowSize64
    uint64_t tile_row_size = TIFFTileRowSize64(tif);
    (void)tile_row_size;

    // Fuzz TIFFTileSize64
    uint64_t tile_size = TIFFTileSize64(tif);
    (void)tile_size;

    // Fuzz TIFFVStripSize64
    // Try with 0 rows
    uint64_t v_strip_size_0 = TIFFVStripSize64(tif, 0);
    (void)v_strip_size_0;
    
    // Try with 1 row
    uint64_t v_strip_size_1 = TIFFVStripSize64(tif, 1);
    (void)v_strip_size_1;
    
    // Try with a large number of rows
    uint64_t v_strip_size_large = TIFFVStripSize64(tif, 0xFFFFFFFF);
    (void)v_strip_size_large;

    // Cleanup
    TIFFClose(tif);
    
    // Remove the dummy file
    remove("./dummy_file");

    return 0;
}