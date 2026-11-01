// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFWriteScanline at tif_write.c:47:5 in tiffio.h
// TIFFWriteScanline at tif_write.c:47:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFCurrentRow at tif_open.c:864:10 in tiffio.h
// TIFFCurrentStrip at tif_open.c:874:10 in tiffio.h
// TIFFComputeStrip at tif_strip.c:35:10 in tiffio.h
// TIFFVStripSize at tif_strip.c:142:10 in tiffio.h
// TIFFReadScanline at tif_read.c:454:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFCurrentRow at tif_open.c:864:10 in tiffio.h
// TIFFCurrentStrip at tif_open.c:874:10 in tiffio.h
// TIFFComputeStrip at tif_strip.c:35:10 in tiffio.h
// TIFFVStripSize at tif_strip.c:142:10 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

static void setup_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        } else {
            // Write minimal valid TIFF header if input is empty
            uint16_t header[8] = {
                0x002A, // Little Endian
                42,     // Magic number
                8,      // Offset to first IFD
                0,      // Number of directory entries (empty IFD)
                0,      // Next IFD offset (0 = none)
                0, 0, 0
            };
            fwrite(header, sizeof(header), 1, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    setup_dummy_file(Data, Size);

    // Test Read Path
    TIFF *tif_r = TIFFOpen("./dummy_file", "r");
    if (tif_r) {
        uint32_t row_r = TIFFCurrentRow(tif_r);
        (void)row_r;

        uint32_t strip_r = TIFFCurrentStrip(tif_r);
        (void)strip_r;

        uint32_t computed_strip_r = TIFFComputeStrip(tif_r, 0, 0);
        (void)computed_strip_r;

        tmsize_t vstrip_size_r = TIFFVStripSize(tif_r, 1);
        if (vstrip_size_r > 0) {
            // Allocate buffer strictly based on the calculated strip size
            // to prevent buffer overflows during read operations
            size_t buf_size = (size_t)vstrip_size_r;
            // Cap allocation to prevent OOM
            if (buf_size > 1024 * 1024) buf_size = 1024 * 1024;
            
            void *buf = malloc(buf_size);
            if (buf) {
                // Try to read a scanline
                int ret = TIFFReadScanline(tif_r, buf, 0, 0);
                (void)ret;
                free(buf);
            }
        }

        TIFFClose(tif_r);
    }

    // Test Write Path
    setup_dummy_file(Data, Size);
    TIFF *tif_w = TIFFOpen("./dummy_file", "w");
    if (tif_w) {
        // Set some basic tags to make the file valid for writing
        uint32_t width = (Size > 0 && Data[0] > 0) ? Data[0] : 1;
        uint32_t height = (Size > 1 && Data[1] > 0) ? Data[1] : 1;
        uint16_t bits = (Size > 2 && Data[2] > 0) ? Data[2] : 8;
        
        TIFFSetField(tif_w, TIFFTAG_IMAGEWIDTH, width);
        TIFFSetField(tif_w, TIFFTAG_IMAGELENGTH, height);
        TIFFSetField(tif_w, TIFFTAG_BITSPERSAMPLE, bits);
        TIFFSetField(tif_w, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
        TIFFSetField(tif_w, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
        TIFFSetField(tif_w, TIFFTAG_SAMPLESPERPIXEL, 1);
        TIFFSetField(tif_w, TIFFTAG_ROWSPERSTRIP, 1);

        uint32_t row_w = TIFFCurrentRow(tif_w);
        (void)row_w;

        uint32_t strip_w = TIFFCurrentStrip(tif_w);
        (void)strip_w;

        uint32_t computed_strip_w = TIFFComputeStrip(tif_w, 0, 0);
        (void)computed_strip_w;

        tmsize_t vstrip_size_w = TIFFVStripSize(tif_w, 1);
        if (vstrip_size_w > 0) {
            // Allocate buffer strictly based on the calculated strip size
            size_t buf_size = (size_t)vstrip_size_w;
            // Cap allocation to prevent OOM
            if (buf_size > 1024 * 1024) buf_size = 1024 * 1024;
            
            void *buf = malloc(buf_size);
            if (buf) {
                memset(buf, 0, buf_size);
                
                // Copy fuzzed data only up to the buffer size to prevent overflow
                size_t copy_size = (Size < buf_size) ? Size : buf_size;
                if (copy_size > 0) {
                    memcpy(buf, Data, copy_size);
                }
                
                int ret = TIFFWriteScanline(tif_w, buf, 0, 0);
                (void)ret;
                
                // Try writing to a different row to trigger state changes
                ret = TIFFWriteScanline(tif_w, buf, 1, 0);
                (void)ret;

                free(buf);
            }
        }

        TIFFClose(tif_w);
    }

    return 0;
}