// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFTileSize64 at tif_tile.c:249:10 in tiffio.h
// TIFFGetStrileByteCount at tif_dirread.c:8366:10 in tiffio.h
// TIFFGetStrileByteCount at tif_dirread.c:8366:10 in tiffio.h
// TIFFVTileSize64 at tif_tile.c:188:10 in tiffio.h
// TIFFVTileSize64 at tif_tile.c:188:10 in tiffio.h
// TIFFVTileSize64 at tif_tile.c:188:10 in tiffio.h
// TIFFStripSize64 at tif_strip.c:196:10 in tiffio.h
// TIFFScanlineSize64 at tif_strip.c:257:10 in tiffio.h
// TIFFTileRowSize64 at tif_tile.c:140:10 in tiffio.h
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
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return;
    
    // Write a minimal valid TIFF header (Little Endian)
    // Byte order: II (0x4949)
    uint16_t byte_order = 0x4949;
    fwrite(&byte_order, sizeof(uint16_t), 1, f);
    
    // Magic number: 42
    uint16_t magic = 42;
    fwrite(&magic, sizeof(uint16_t), 1, f);
    
    // Offset to first IFD
    uint32_t ifd_offset = 8;
    fwrite(&ifd_offset, sizeof(uint32_t), 1, f);
    
    // Write IFD
    // Number of directory entries (0 for minimal, but we need at least one for valid state usually)
    // Let's write 0 entries to keep it minimal, or 1 if needed. 
    // For fuzzing, a minimal header might fail directory read, but that's a valid state to test.
    uint16_t num_entries = 0;
    fwrite(&num_entries, sizeof(uint16_t), 1, f);
    
    // Next IFD offset (0 means no more IFDs)
    uint32_t next_ifd = 0;
    fwrite(&next_ifd, sizeof(uint32_t), 1, f);
    
    // If we have data, append it after the header to simulate file content
    if (Data && Size > 0) {
        fwrite(Data, 1, Size, f);
    }
    
    fclose(f);
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    // Ensure we have some data to avoid trivial empty file issues, though libtiff handles empty files
    if (Size == 0) {
        // Create a minimal file even if input is empty
        write_dummy_tiff(NULL, 0);
    } else {
        write_dummy_tiff(Data, Size);
    }

    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        return 0;
    }

    // Attempt to read directory. If this fails, tif is still valid but might not have data.
    // We proceed to call functions regardless to test error handling paths.
    TIFFReadDirectory(tif);

    // 1. TIFFTileSize64
    // Returns tile size or 0 if not tiled or error.
    uint64_t tile_size = TIFFTileSize64(tif);
    (void)tile_size;

    // 2. TIFFGetStrileByteCount
    // Try with index 0. If file has no strips/tiles, it returns 0.
    uint64_t strile_count = TIFFGetStrileByteCount(tif, 0);
    (void)strile_count;
    
    // Try with a large index to test bounds checking logic inside
    strile_count = TIFFGetStrileByteCount(tif, 0xFFFFFFFF);
    (void)strile_count;

    // 3. TIFFVTileSize64
    // Calculate vertical tile size for 1 row and 0 rows
    uint64_t vtile_size = TIFFVTileSize64(tif, 1);
    (void)vtile_size;
    vtile_size = TIFFVTileSize64(tif, 0);
    (void)vtile_size;
    vtile_size = TIFFVTileSize64(tif, 0xFFFFFFFF);
    (void)vtile_size;

    // 4. TIFFStripSize64
    uint64_t strip_size = TIFFStripSize64(tif);
    (void)strip_size;

    // 5. TIFFScanlineSize64
    uint64_t scanline_size = TIFFScanlineSize64(tif);
    (void)scanline_size;

    // 6. TIFFTileRowSize64
    uint64_t tile_row_size = TIFFTileRowSize64(tif);
    (void)tile_row_size;

    TIFFClose(tif);
    
    return 0;
}