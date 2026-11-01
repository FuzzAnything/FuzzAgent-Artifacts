// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFSwabArrayOfTriples at tif_swab.c:99:6 in tiffio.h
// TIFFReadRawStrip at tif_read.c:717:10 in tiffio.h
// TIFFReadRawStrip at tif_read.c:717:10 in tiffio.h
// TIFFReadRawStrip at tif_read.c:717:10 in tiffio.h
// TIFFReverseBits at tif_swab.c:310:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFReadEncodedTile at tif_read.c:953:10 in tiffio.h
// TIFFReadEncodedTile at tif_read.c:953:10 in tiffio.h
// TIFFReadEncodedTile at tif_read.c:953:10 in tiffio.h
// TIFFReadRGBAStrip at tif_getimage.c:3341:5 in tiffio.h
// TIFFReadRGBAStrip at tif_getimage.c:3341:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <tiffio.h>
#include <sys/stat.h>

static void write_dummy_tiff(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return;
    
    // Write a minimal valid TIFF header
    // Byte order (II = little endian)
    uint16_t byte_order = 0x4949;
    fwrite(&byte_order, sizeof(uint16_t), 1, f);
    
    // Magic number for TIFF
    uint16_t magic = 42;
    fwrite(&magic, sizeof(uint16_t), 1, f);
    
    // Offset to first IFD
    uint32_t ifd_offset = 8;
    fwrite(&ifd_offset, sizeof(uint32_t), 1, f);
    
    // Write a minimal IFD (0 entries to keep it simple, but valid structure)
    // Number of directory entries
    uint16_t num_entries = 0;
    fwrite(&num_entries, sizeof(uint16_t), 1, f);
    
    // Offset to next IFD (0 means no more IFDs)
    uint32_t next_ifd = 0;
    fwrite(&next_ifd, sizeof(uint32_t), 1, f);
    
    // Write the fuzzed data as raw image data after the header
    if (size > 0) {
        fwrite(data, 1, size, f);
    }
    
    fclose(f);
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Prepare environment: create a dummy file with the input data
    write_dummy_tiff(Data, Size);

    // Open the file
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        return 0;
    }

    // Allocate buffers for testing
    size_t max_buf_size = 1024 * 1024; // 1MB max buffer
    uint8_t *buf = NULL;
    uint32_t *rgba_buf = NULL;
    
    if (Size > 0) {
        buf = (uint8_t *)malloc(Size < max_buf_size ? Size : max_buf_size);
        rgba_buf = (uint32_t *)malloc(Size < max_buf_size ? Size : max_buf_size);
    } else {
        buf = (uint8_t *)malloc(max_buf_size);
        rgba_buf = (uint32_t *)malloc(max_buf_size);
    }

    if (!buf || !rgba_buf) {
        TIFFClose(tif);
        free(buf);
        free(rgba_buf);
        return 0;
    }

    // Test 1: TIFFReadEncodedTile
    // Try to read a tile (tile index 0)
    // We pass -1 for size to let the library determine the size
    tmsize_t tile_size = TIFFTileSize(tif);
    if (tile_size > 0 && tile_size <= max_buf_size) {
        TIFFReadEncodedTile(tif, 0, buf, tile_size);
        TIFFReadEncodedTile(tif, 0, buf, -1);
        // Try an out-of-range tile
        TIFFReadEncodedTile(tif, 99999, buf, tile_size);
    }

    // Test 2: TIFFReadRGBAStrip
    // Try to read strip 0
    // Calculate approximate size needed for RGBA buffer based on input size
    size_t rgba_count = (Size / 4) + 1;
    if (rgba_count > 0) {
        TIFFReadRGBAStrip(tif, 0, rgba_buf);
        // Try an out-of-range strip
        TIFFReadRGBAStrip(tif, 99999, rgba_buf);
    }

    // Test 3: TIFFReadEncodedStrip
    // Try to read strip 0
    tmsize_t strip_size = TIFFStripSize(tif);
    if (strip_size > 0 && strip_size <= max_buf_size) {
        TIFFReadEncodedStrip(tif, 0, buf, strip_size);
        TIFFReadEncodedStrip(tif, 0, buf, -1);
        // Try an out-of-range strip
        TIFFReadEncodedStrip(tif, 99999, buf, strip_size);
    }

    // Test 4: TIFFSwabArrayOfTriples
    // Prepare a buffer with triples (3 bytes each)
    size_t triples_count = Size / 3;
    if (triples_count > 0) {
        // Ensure buffer has enough data
        uint8_t *triples_buf = buf;
        TIFFSwabArrayOfTriples(triples_buf, triples_count);
    }

    // Test 5: TIFFReadRawStrip
    // Try to read raw strip 0
    if (strip_size > 0 && strip_size <= max_buf_size) {
        TIFFReadRawStrip(tif, 0, buf, strip_size);
        TIFFReadRawStrip(tif, 0, buf, -1);
        // Try an out-of-range strip
        TIFFReadRawStrip(tif, 99999, buf, strip_size);
    }

    // Test 6: TIFFReverseBits
    // Reverse bits in the buffer
    if (Size > 0) {
        TIFFReverseBits(buf, Size);
    }

    // Cleanup
    free(buf);
    free(rgba_buf);
    TIFFClose(tif);
    
    // Remove the dummy file
    remove("./dummy_file");

    return 0;
}