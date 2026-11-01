// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFWriteEncodedTile at tif_write.c:414:10 in tiffio.h
// TIFFDeferStrileArrayWriting at tif_dirwrite.c:268:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFScanlineSize at tif_strip.c:343:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// TIFFWriteEncodedStrip at tif_write.c:215:10 in tiffio.h
// TIFFWriteRawStrip at tif_write.c:328:10 in tiffio.h
// TIFFWriteBufferSetup at tif_write.c:677:5 in tiffio.h
// TIFFWriteBufferSetup at tif_write.c:677:5 in tiffio.h
// TIFFWriteScanline at tif_write.c:47:5 in tiffio.h
// TIFFWriteScanline at tif_write.c:47:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

static void setup_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (Size > 0 && Data != NULL) {
            fwrite(Data, 1, Size, f);
        } else {
            const char dummy[] = "dummy";
            fwrite(dummy, 1, strlen(dummy), f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    setup_dummy_file(Data, Size);

    TIFF *tif = TIFFOpen("./dummy_file", "w8");
    if (!tif) {
        return 0;
    }

    // Configure image dimensions
    uint32_t width = 64;
    uint32_t height = 64;
    uint16_t bits = 8;
    uint16_t samples = 1;

    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);

    // Calculate the exact size required for a scanline to avoid buffer overflows
    tmsize_t scanline_size = TIFFScanlineSize(tif);
    if (scanline_size <= 0) {
        TIFFClose(tif);
        remove("./dummy_file");
        return 0;
    }

    // Allocate a buffer large enough for a scanline, ensuring we don't under-allocate
    // We use the larger of the input size or the required scanline size
    size_t buf_size = (Size > (size_t)scanline_size) ? Size : (size_t)scanline_size;
    
    // Ensure we have at least 1 byte
    if (buf_size == 0) buf_size = 1;

    uint8_t *test_buf = (uint8_t *)malloc(buf_size);
    if (!test_buf) {
        TIFFClose(tif);
        remove("./dummy_file");
        return 0;
    }

    // Initialize buffer with input data or zeros
    if (Size > 0) {
        memcpy(test_buf, Data, Size);
        // Zero out the rest if the buffer is larger than input
        if (buf_size > Size) {
            memset(test_buf + Size, 0, buf_size - Size);
        }
    } else {
        memset(test_buf, 0, buf_size);
    }

    // 1. Test TIFFWriteEncodedStrip
    // For COMPRESSION_NONE, raw data is effectively encoded data, but we limit size to strip size
    tmsize_t strip_size = TIFFStripSize(tif);
    tmsize_t write_size = (strip_size > 0 && strip_size < buf_size) ? strip_size : buf_size;
    TIFFWriteEncodedStrip(tif, 0, test_buf, write_size);

    // 2. Test TIFFWriteRawStrip
    TIFFWriteRawStrip(tif, 0, test_buf, write_size);

    // 3. Test TIFFWriteBufferSetup
    TIFFWriteBufferSetup(tif, NULL, -1);
    TIFFWriteBufferSetup(tif, test_buf, buf_size);

    // 4. Test TIFFWriteScanline
    // Ensure the buffer passed is at least scanline_size bytes to prevent overflow inside libtiff
    // We allocated buf_size >= scanline_size, so this is safe.
    TIFFWriteScanline(tif, test_buf, 0, 0);
    TIFFWriteScanline(tif, test_buf, 1, 0);

    // 5. Test TIFFWriteEncodedTile
    // Note: The file is in strip mode, so this might fail or behave unexpectedly, 
    // but it's safe to call for fuzzing purposes.
    TIFFWriteEncodedTile(tif, 0, test_buf, write_size);

    // 6. Test TIFFDeferStrileArrayWriting
    TIFFDeferStrileArrayWriting(tif);

    // Cleanup
    free(test_buf);
    TIFFClose(tif);
    remove("./dummy_file");

    return 0;
}