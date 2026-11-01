// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFScanlineSize at tif_strip.c:343:10 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFReadEncodedTile at tif_read.c:953:10 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFReadEncodedTile at tif_read.c:953:10 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFTileRowSize at tif_tile.c:177:10 in tiffio.h
// TIFFRasterScanlineSize at tif_strip.c:375:10 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <tiffio.h>

static void write_dummy_tiff(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f == NULL) {
        return;
    }
    
    // Write a minimal TIFF header (Basic structure)
    // Byte Order (II or MM)
    uint16_t byte_order = 0x4949; // Little Endian (II)
    fwrite(&byte_order, sizeof(uint16_t), 1, f);
    
    // Magic Number
    uint16_t magic = 42;
    fwrite(&magic, sizeof(uint16_t), 1, f);
    
    // Offset to first IFD
    uint32_t ifd_offset = 8;
    fwrite(&ifd_offset, sizeof(uint32_t), 1, f);
    
    // IFD Entry Count (1 entry)
    uint16_t entry_count = 1;
    fwrite(&entry_count, sizeof(uint16_t), 1, f);
    
    // One dummy IFD entry (Tag 256, Type 3, Count 1, Value 100)
    // Tag: ImageWidth
    uint16_t tag = 256;
    fwrite(&tag, sizeof(uint16_t), 1, f);
    // Type: SHORT
    uint16_t type = 3;
    fwrite(&type, sizeof(uint16_t), 1, f);
    // Count
    uint32_t count = 1;
    fwrite(&count, sizeof(uint32_t), 1, f);
    // Value (100)
    uint32_t value = 100;
    fwrite(&value, sizeof(uint32_t), 1, f);
    
    // Next IFD offset (0)
    uint32_t next_ifd = 0;
    fwrite(&next_ifd, sizeof(uint32_t), 1, f);
    
    // Write fuzzing data as image data
    if (size > 0) {
        fwrite(data, 1, size, f);
    }
    
    fclose(f);
}

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare environment: Create a dummy file with minimal TIFF structure
    write_dummy_tiff(Data, Size);

    // Open the TIFF file
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (tif == NULL) {
        return 0;
    }

    // Ensure we have a valid directory
    if (!TIFFReadDirectory(tif)) {
        TIFFClose(tif);
        return 0;
    }

    // 1. Fuzz _TIFFmalloc with various sizes derived from input
    // We use modulo to keep size reasonable for fuzzing, but cover 0 and small/large cases
    tmsize_t alloc_size = (Size > 0) ? (Size % 10000) : 0;
    if (alloc_size == 0 && Size > 0) alloc_size = 1; // Ensure non-zero if input was large multiple
    
    void *ptr = _TIFFmalloc(alloc_size);
    if (ptr != NULL) {
        // Write some data to ensure memory is touched
        memset(ptr, 0xAA, alloc_size);
        _TIFFfree(ptr);
    }

    // 2. Fuzz TIFFTileRowSize
    // This function returns 0 if not tiled, which is expected for our dummy file
    tmsize_t tile_row_size = TIFFTileRowSize(tif);
    (void)tile_row_size; // Use variable to avoid unused warning

    // 3. Fuzz TIFFRasterScanlineSize
    tmsize_t raster_scanline_size = TIFFRasterScanlineSize(tif);
    (void)raster_scanline_size;

    // 4. Fuzz TIFFScanlineSize
    tmsize_t scanline_size = TIFFScanlineSize(tif);
    (void)scanline_size;

    // 5. Fuzz TIFFTileSize
    tmsize_t tile_size = TIFFTileSize(tif);
    (void)tile_size;

    // 6. Fuzz TIFFReadEncodedTile
    // We need a tile index. Try 0 and a value derived from input.
    // If the file is not tiled, this will likely return -1 or 0, which is safe.
    uint32_t tile_idx = 0;
    if (Size > 0) {
        tile_idx = Data[0]; // Use first byte as tile index
    }
    
    // Allocate buffer if tile_size is non-zero (tiled image)
    void *tile_buf = NULL;
    if (tile_size > 0) {
        tile_buf = _TIFFmalloc(tile_size);
    }
    
    if (tile_buf != NULL) {
        tmsize_t read_result = TIFFReadEncodedTile(tif, tile_idx, tile_buf, tile_size);
        (void)read_result;
        _TIFFfree(tile_buf);
    } else {
        // Even if not tiled, try calling with a small buffer to test error handling paths
        void *dummy_buf = _TIFFmalloc(100);
        if (dummy_buf != NULL) {
            TIFFReadEncodedTile(tif, tile_idx, dummy_buf, 100);
            _TIFFfree(dummy_buf);
        }
    }

    // Cleanup
    TIFFClose(tif);
    
    // Remove dummy file
    unlink("./dummy_file");

    return 0;
}