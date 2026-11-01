// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFWriteEncodedStrip at tif_write.c:215:10 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFIsTiled at tif_open.c:859:5 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFReadEncodedTile at tif_read.c:953:10 in tiffio.h
// TIFFRawStripSize at tif_strip.c:168:10 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFReadRawStrip at tif_read.c:717:10 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <tiffio.h>

static void setup_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        // Write a minimal TIFF header (4 bytes) + fuzzed data
        // Magic number for little-endian TIFF: 0x002A 0x4949
        uint8_t header[4] = {0x49, 0x49, 0x2A, 0x00};
        fwrite(header, 1, 4, f);
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare the dummy file
    setup_dummy_file(Data, Size);

    // Open the file for reading
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        return 0;
    }

    // Ensure we have a valid directory
    if (!TIFFReadDirectory(tif)) {
        TIFFClose(tif);
        return 0;
    }

    uint32_t strip_count = 0;
    uint32_t tile_count = 0;
    tmsize_t strip_size = 0;
    tmsize_t tile_size = 0;

    // Get counts
    if (TIFFIsTiled(tif)) {
        tile_count = TIFFNumberOfTiles(tif);
        tile_size = TIFFTileSize(tif);
    } else {
        strip_count = TIFFNumberOfStrips(tif);
        strip_size = TIFFStripSize(tif);
    }

    // Allocate buffers based on fuzzed data size or calculated sizes
    // Use a safe upper bound to prevent excessive allocation
    size_t max_buf_size = 1024 * 1024; // 1MB limit
    if (Size > max_buf_size) Size = max_buf_size;

    void *buf = _TIFFmalloc(Size);
    if (!buf && Size > 0) {
        TIFFClose(tif);
        return 0;
    }

    // Fuzz TIFFReadEncodedStrip
    if (strip_count > 0) {
        for (uint32_t i = 0; i < strip_count; i++) {
            // Try reading with different sizes
            tmsize_t res = TIFFReadEncodedStrip(tif, i, buf, Size);
            (void)res; // Ignore return for fuzzing stability
        }
    }

    // Fuzz TIFFReadEncodedTile
    if (tile_count > 0) {
        for (uint32_t i = 0; i < tile_count; i++) {
            tmsize_t res = TIFFReadEncodedTile(tif, i, buf, Size);
            (void)res;
        }
    }

    // Fuzz TIFFRawStripSize
    if (strip_count > 0) {
        for (uint32_t i = 0; i < strip_count; i++) {
            tmsize_t raw_size = TIFFRawStripSize(tif, i);
            if (raw_size > 0 && raw_size < max_buf_size) {
                void *raw_buf = _TIFFmalloc(raw_size);
                if (raw_buf) {
                    // Fuzz TIFFReadRawStrip
                    tmsize_t res = TIFFReadRawStrip(tif, i, raw_buf, raw_size);
                    (void)res;
                    _TIFFfree(raw_buf);
                }
            }
        }
    }

    // Cleanup read buffer
    if (buf) {
        _TIFFfree(buf);
    }

    TIFFClose(tif);

    // Prepare for write operations
    setup_dummy_file(Data, Size);
    tif = TIFFOpen("./dummy_file", "w");
    if (!tif) {
        return 0;
    }

    // Set minimal tags to make the file writable
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 16);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 16);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);

    // Allocate write buffer
    size_t write_size = Size > 0 ? Size : 1;
    if (write_size > max_buf_size) write_size = max_buf_size;
    buf = _TIFFmalloc(write_size);
    if (!buf) {
        TIFFClose(tif);
        return 0;
    }
    memset(buf, 0x42, write_size);

    // Fuzz TIFFWriteEncodedStrip
    // We need to ensure strips exist. For a 16x16 image with no compression, 
    // TIFF usually creates strips automatically.
    uint32_t nstrips = TIFFNumberOfStrips(tif);
    if (nstrips == 0) {
        // Force creation by writing a row or setting strip count if possible
        // But simpler: just try to write to strip 0 if it exists or is created
        // If nstrips is 0, we might need to write scanlines first, but for fuzzing
        // we can try writing to strip 0 directly which might trigger internal logic.
        nstrips = 1; 
    }

    for (uint32_t i = 0; i < nstrips; i++) {
        tmsize_t res = TIFFWriteEncodedStrip(tif, i, buf, write_size);
        (void)res;
    }

    if (buf) {
        _TIFFfree(buf);
    }

    TIFFClose(tif);
    return 0;
}