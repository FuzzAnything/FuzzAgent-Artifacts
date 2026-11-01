// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFWriteRawTile at tif_write.c:533:10 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFVStripSize at tif_strip.c:142:10 in tiffio.h
// TIFFVTileSize at tif_tile.c:238:10 in tiffio.h
// TIFFTileRowSize at tif_tile.c:177:10 in tiffio.h
// TIFFWriteBufferSetup at tif_write.c:677:5 in tiffio.h
// TIFFIsTiled at tif_open.c:859:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

static void cleanup_tiff(TIFF *tif) {
    if (tif != NULL) {
        TIFFClose(tif);
    }
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    const char *filename = "./dummy_file";
    
    // Write input data to the dummy file
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Open the file in read/write mode to allow writing
    TIFF *tif = TIFFOpen(filename, "r+");
    if (!tif) {
        // If opening in r+ fails (e.g., file too small or invalid), try creating a new one
        tif = TIFFOpen(filename, "w");
        if (!tif) {
            return 0;
        }
        
        // Setup minimal valid directory for a new file
        TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 16);
        TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 16);
        TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
        TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
        TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
        TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
        
        // Optionally set tiling if we want to test tile functions specifically
        // But let's keep it flexible based on input
        if (Size > 1 && Data[1] & 0x01) {
            TIFFSetField(tif, TIFFTAG_TILEWIDTH, 16);
            TIFFSetField(tif, TIFFTAG_TILELENGTH, 16);
        }
        
        // Write directory to make it valid
        TIFFWriteDirectory(tif);
    }

    // Ensure we have a valid directory context
    // TIFFIsStrip is not a standard public API, so we skip this check or rely on TIFFIsTiled
    // The logic below handles both tiled and striped cases implicitly via the API calls

    // Allocate a buffer for testing write operations
    size_t alloc_size = Size;
    if (alloc_size == 0) alloc_size = 1;
    // Cap allocation to prevent OOM in fuzzer
    if (alloc_size > 1024 * 1024) alloc_size = 1024 * 1024;
    
    void *buffer = _TIFFmalloc(alloc_size);
    if (!buffer) {
        cleanup_tiff(tif);
        return 0;
    }
    memcpy(buffer, Data, Size);

    // Fuzz TIFFVStripSize
    uint32_t nrows = (Size > 2) ? (Data[0] + Data[1] + Data[2]) % 10000 : 10;
    tmsize_t vstrip_size = TIFFVStripSize(tif, nrows);
    (void)vstrip_size; // Use result to avoid unused warning

    // Fuzz TIFFVTileSize
    tmsize_t vtile_size = TIFFVTileSize(tif, nrows);
    (void)vtile_size;

    // Fuzz TIFFTileRowSize
    tmsize_t tile_row_size = TIFFTileRowSize(tif);
    (void)tile_row_size;

    // Fuzz TIFFWriteBufferSetup
    // Try with NULL buffer (auto-allocate) and specific size
    tmsize_t setup_size = (Size > 4) ? (Data[0] | (Data[1] << 8)) : 8192;
    if (setup_size == 0) setup_size = -1; // Let libtiff decide
    int setup_result = TIFFWriteBufferSetup(tif, NULL, setup_size);
    (void)setup_result;

    // Fuzz TIFFWriteRawTile
    // Only if the image is tiled
    if (TIFFIsTiled(tif)) {
        uint32_t tile_index = (Size > 4) ? (Data[0] | (Data[1] << 8) | (Data[2] << 16) | (Data[3] << 24)) % 1000 : 0;
        tmsize_t cc = (Size > 5) ? (Data[4] | (Data[5] << 8)) : alloc_size;
        if (cc > alloc_size) cc = alloc_size;
        
        tmsize_t written = TIFFWriteRawTile(tif, tile_index, buffer, cc);
        (void)written;
    }

    // Cleanup
    _TIFFfree(buffer);
    cleanup_tiff(tif);
    
    return 0;
}