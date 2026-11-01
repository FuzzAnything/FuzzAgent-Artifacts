// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFWriteEncodedStrip at tif_write.c:215:10 in tiffio.h
// TIFFWriteRawStrip at tif_write.c:328:10 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFReadRawStrip at tif_read.c:717:10 in tiffio.h
// TIFFRawStripSize at tif_strip.c:168:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
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
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        } else {
            // Write minimal valid TIFF header if input is empty to prevent immediate crash
            uint16_t header[8] = {42, 0, 0, 0, 8, 0, 0, 0};
            fwrite(header, sizeof(uint16_t), 8, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    setup_dummy_file(Data, Size);

    // Test Read APIs
    TIFF *tif_r = TIFFOpen("./dummy_file", "r");
    if (tif_r) {
        uint32_t num_strips = TIFFNumberOfStrips(tif_r);
        
        // Allocate a buffer for reading
        uint8_t *buf = (uint8_t *)malloc(Size > 0 ? Size : 1024);
        if (buf) {
            // Fuzz strip index
            uint32_t strip_idx = 0;
            if (Size >= 4) {
                strip_idx = *(const uint32_t *)(Data);
            }

            // Test TIFFReadEncodedStrip
            if (strip_idx < num_strips || num_strips == 0) {
                TIFFReadEncodedStrip(tif_r, strip_idx, buf, Size);
            }

            // Test TIFFReadRawStrip
            if (strip_idx < num_strips || num_strips == 0) {
                TIFFReadRawStrip(tif_r, strip_idx, buf, Size);
            }
            
            // Test TIFFRawStripSize
            if (strip_idx < num_strips || num_strips == 0) {
                TIFFRawStripSize(tif_r, strip_idx);
            }

            free(buf);
        }
        TIFFClose(tif_r);
    }

    // Test Write APIs
    // We need to create a valid writable TIFF structure.
    // Since we can't easily construct a valid internal TIFF state from scratch without
    // writing a full header and directory, we rely on the library's ability to
    // handle invalid handles or use a minimal valid file.
    // However, for fuzzing, we can try to open for writing and see if it creates a valid state.
    
    // Remove existing file to ensure clean write
    remove("./dummy_file");
    
    TIFF *tif_w = TIFFOpen("./dummy_file", "w");
    if (tif_w) {
        // Set minimal required tags to make the TIFF valid for writing strips
        TIFFSetField(tif_w, TIFFTAG_IMAGEWIDTH, 1);
        TIFFSetField(tif_w, TIFFTAG_IMAGELENGTH, 1);
        TIFFSetField(tif_w, TIFFTAG_BITSPERSAMPLE, 8);
        TIFFSetField(tif_w, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
        TIFFSetField(tif_w, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
        TIFFSetField(tif_w, TIFFTAG_SAMPLESPERPIXEL, 1);
        TIFFSetField(tif_w, TIFFTAG_ROWSPERSTRIP, 1);
        
        uint8_t *write_buf = (uint8_t *)malloc(Size > 0 ? Size : 1);
        if (write_buf) {
            if (Size > 0) {
                memcpy(write_buf, Data, Size);
            } else {
                write_buf[0] = 0;
            }

            uint32_t strip_idx = 0;
            if (Size >= 4) {
                strip_idx = *(const uint32_t *)(Data);
            }

            // Test TIFFWriteEncodedStrip
            // Note: This might fail if the strip is not pre-allocated or if the file is not ready,
            // which is exactly what we want to test.
            TIFFWriteEncodedStrip(tif_w, strip_idx, write_buf, Size);

            // Test TIFFWriteRawStrip
            TIFFWriteRawStrip(tif_w, strip_idx, write_buf, Size);
            
            // Test TIFFNumberOfStrips on writable file
            TIFFNumberOfStrips(tif_w);

            free(write_buf);
        }
        TIFFClose(tif_w);
    }

    return 0;
}