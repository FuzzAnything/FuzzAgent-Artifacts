// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFWriteEncodedStrip at tif_write.c:215:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFRawStripSize at tif_strip.c:168:10 in tiffio.h
// TIFFReadRawStrip at tif_read.c:717:10 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
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

static void setup_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        } else {
            // Write a minimal valid TIFF header if input is empty
            uint8_t header[8] = {0x49, 0x49, 0x2A, 0x00, 0x08, 0x00, 0x00, 0x00};
            fwrite(header, 1, 8, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    setup_dummy_file(Data, Size);

    // Test reading operations
    TIFF *tif_read = TIFFOpen("./dummy_file", "r");
    if (tif_read) {
        uint32_t strip_count = TIFFNumberOfStrips(tif_read);
        
        // Allocate a buffer for reading, limiting size to prevent OOM
        tmsize_t max_buf_size = 1024 * 1024; // 1MB limit
        tmsize_t buf_size = (Size > max_buf_size) ? max_buf_size : Size;
        if (buf_size == 0) buf_size = 1;
        
        uint8_t *read_buf = (uint8_t *)malloc(buf_size);
        if (read_buf) {
            // Fuzz strip index: use data bytes or 0
            uint32_t strip_idx = 0;
            if (Size >= 4) {
                strip_idx = *(uint32_t *)Data;
            }
            
            // Test TIFFRawStripSize
            TIFFRawStripSize(tif_read, strip_idx);
            
            // Test TIFFReadRawStrip
            TIFFReadRawStrip(tif_read, strip_idx, read_buf, buf_size);
            
            // Test TIFFReadEncodedStrip
            TIFFReadEncodedStrip(tif_read, strip_idx, read_buf, buf_size);
            
            free(read_buf);
        }
        
        TIFFClose(tif_read);
    }

    // Test writing operations
    // Create a new file for writing
    TIFF *tif_write = TIFFOpen("./dummy_file", "w");
    if (tif_write) {
        // Set some basic tags to make the file valid
        TIFFSetField(tif_write, TIFFTAG_IMAGEWIDTH, 10);
        TIFFSetField(tif_write, TIFFTAG_IMAGELENGTH, 10);
        TIFFSetField(tif_write, TIFFTAG_BITSPERSAMPLE, 8);
        TIFFSetField(tif_write, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
        TIFFSetField(tif_write, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
        TIFFSetField(tif_write, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
        
        uint32_t strip_count = TIFFNumberOfStrips(tif_write);
        
        tmsize_t max_buf_size = 1024 * 1024;
        tmsize_t buf_size = (Size > max_buf_size) ? max_buf_size : Size;
        if (buf_size == 0) buf_size = 1;
        
        uint8_t *write_buf = (uint8_t *)malloc(buf_size);
        if (write_buf) {
            // Fill buffer with fuzzing data
            memcpy(write_buf, Data, buf_size);
            
            uint32_t strip_idx = 0;
            if (Size >= 4) {
                strip_idx = *(uint32_t *)Data;
            }
            
            // Ensure strip_idx is within reasonable bounds for the small image
            if (strip_idx >= strip_count && strip_count > 0) {
                strip_idx = 0;
            }
            
            // Test TIFFWriteRawStrip
            TIFFWriteRawStrip(tif_write, strip_idx, write_buf, buf_size);
            
            // Test TIFFWriteEncodedStrip
            TIFFWriteEncodedStrip(tif_write, strip_idx, write_buf, buf_size);
            
            free(write_buf);
        }
        
        TIFFClose(tif_write);
    }

    return 0;
}