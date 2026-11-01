// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFWriteRawTile at tif_write.c:533:10 in tiffio.h
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
// _TIFFmemcpy at tif_unix.c:355:6 in tiffio.h
// TIFFReadRawTile at tif_read.c:1176:10 in tiffio.h
// TIFFWriteEncodedTile at tif_write.c:414:10 in tiffio.h
// _TIFFmemcpy at tif_unix.c:355:6 in tiffio.h
// TIFFReadRawStrip at tif_read.c:717:10 in tiffio.h
// TIFFWriteRawStrip at tif_write.c:328:10 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <tiffio.h>

static void cleanup_tiff(TIFF *tif) {
    if (tif != NULL) {
        TIFFClose(tif);
    }
}

int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a temporary file for the fuzzer input
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "w+b");
    if (!fp) {
        return 0;
    }

    // Write input data to the file
    if (fwrite(Data, 1, Size, fp) != Size) {
        fclose(fp);
        return 0;
    }
    fclose(fp);

    // Open the file for reading
    TIFF *tif_read = TIFFOpen(filename, "rb");
    if (!tif_read) {
        return 0;
    }

    // Open the file for writing (to test write functions)
    TIFF *tif_write = TIFFOpen(filename, "wb");
    if (!tif_write) {
        cleanup_tiff(tif_read);
        return 0;
    }

    // Basic setup for write file to allow strip/tile operations
    TIFFSetField(tif_write, TIFFTAG_IMAGEWIDTH, 64);
    TIFFSetField(tif_write, TIFFTAG_IMAGELENGTH, 64);
    TIFFSetField(tif_write, TIFFTAG_BITSPERSAMPLE, 8);
    TIFFSetField(tif_write, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
    TIFFSetField(tif_write, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
    TIFFSetField(tif_write, TIFFTAG_SAMPLESPERPIXEL, 1);
    TIFFSetField(tif_write, TIFFTAG_ROWSPERSTRIP, 1);

    // Allocate a buffer for operations
    size_t buf_size = Size > 1024 ? 1024 : Size;
    uint8_t *buf = (uint8_t *)malloc(buf_size);
    if (!buf) {
        cleanup_tiff(tif_read);
        cleanup_tiff(tif_write);
        return 0;
    }

    // Seed buffer with input data
    if (Size > 0) {
        _TIFFmemcpy(buf, Data, buf_size);
    }

    // Fuzz TIFFReadRawTile
    uint32_t tile_idx = 0;
    if (Size > 0) tile_idx = Data[0] % 100;
    tmsize_t res = TIFFReadRawTile(tif_read, tile_idx, buf, buf_size);
    (void)res;

    // Fuzz TIFFWriteEncodedTile
    // Note: Writing encoded tile usually requires valid encoded data. 
    // We pass the buffer; it may fail, which is expected for fuzzing.
    res = TIFFWriteEncodedTile(tif_write, tile_idx, buf, buf_size);
    (void)res;

    // Fuzz _TIFFmemcpy
    uint8_t *dest = (uint8_t *)malloc(buf_size);
    if (dest) {
        _TIFFmemcpy(dest, buf, buf_size);
        free(dest);
    }

    // Fuzz TIFFReadRawStrip
    uint32_t strip_idx = 0;
    if (Size > 1) strip_idx = Data[1] % 100;
    res = TIFFReadRawStrip(tif_read, strip_idx, buf, buf_size);
    (void)res;

    // Fuzz TIFFWriteRawStrip
    res = TIFFWriteRawStrip(tif_write, strip_idx, buf, buf_size);
    (void)res;

    // Fuzz TIFFWriteRawTile
    res = TIFFWriteRawTile(tif_write, tile_idx, buf, buf_size);
    (void)res;

    // Cleanup
    free(buf);
    cleanup_tiff(tif_read);
    cleanup_tiff(tif_write);

    // Remove temporary file
    remove(filename);

    return 0;
}