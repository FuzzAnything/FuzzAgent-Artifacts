// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFDeferStrileArrayWriting at tif_dirwrite.c:268:5 in tiffio.h
// TIFFWriteCheck at tif_write.c:605:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFForceStrileArrayWriting at tif_flush.c:76:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFForceStrileArrayWriting at tif_flush.c:76:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFWriteEncodedTile at tif_write.c:414:10 in tiffio.h
// TIFFWriteEncodedStrip at tif_write.c:215:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadEncodedTile at tif_read.c:953:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0;
    
    write_dummy_file(Data, Size);
    
    // First phase: Write operations
    TIFF *tif_w = TIFFOpen("./dummy_file", "w");
    if (!tif_w) {
        cleanup_dummy_file();
        return 0;
    }
    
    // Extract values from fuzzer input for TIFFSetField calls
    uint32_t *data_uint32 = (uint32_t *)Data;
    uint16_t *data_uint16 = (uint16_t *)Data;
    
    // 8 TIFFSetField calls with various tags
    TIFFSetField(tif_w, TIFFTAG_IMAGEWIDTH, data_uint32[0] % 10000);
    TIFFSetField(tif_w, TIFFTAG_IMAGELENGTH, data_uint32[1] % 10000);
    TIFFSetField(tif_w, TIFFTAG_BITSPERSAMPLE, data_uint16[4] % 32);
    TIFFSetField(tif_w, TIFFTAG_COMPRESSION, data_uint16[5] % 10);
    TIFFSetField(tif_w, TIFFTAG_PHOTOMETRIC, data_uint16[6] % 5);
    TIFFSetField(tif_w, TIFFTAG_SAMPLESPERPIXEL, data_uint16[7] % 5);
    TIFFSetField(tif_w, TIFFTAG_ROWSPERSTRIP, data_uint32[4] % 1000);
    TIFFSetField(tif_w, TIFFTAG_PLANARCONFIG, data_uint16[8] % 2);
    
    TIFFDeferStrileArrayWriting(tif_w);
    TIFFWriteCheck(tif_w, 0, "test");
    TIFFWriteDirectory(tif_w);
    TIFFSetDirectory(tif_w, 0);
    TIFFForceStrileArrayWriting(tif_w);
    TIFFSetDirectory(tif_w, 0);
    TIFFForceStrileArrayWriting(tif_w);
    TIFFSetDirectory(tif_w, 0);
    
    // Prepare buffer for write operations
    size_t buffer_size = Size > 1024 ? 1024 : Size;
    void *write_buffer = malloc(buffer_size);
    if (write_buffer) {
        memcpy(write_buffer, Data, buffer_size);
        
        TIFFWriteEncodedTile(tif_w, 0, write_buffer, buffer_size);
        TIFFWriteEncodedStrip(tif_w, 0, write_buffer, buffer_size);
        
        free(write_buffer);
    }
    
    TIFFClose(tif_w);
    
    // Second phase: Read operations
    TIFF *tif_r = TIFFOpen("./dummy_file", "r");
    if (!tif_r) {
        cleanup_dummy_file();
        return 0;
    }
    
    void *read_buffer = malloc(buffer_size);
    if (read_buffer) {
        TIFFReadEncodedTile(tif_r, 0, read_buffer, buffer_size);
        TIFFClose(tif_r);
        
        // Re-open for strip reading
        tif_r = TIFFOpen("./dummy_file", "r");
        if (tif_r) {
            TIFFReadEncodedStrip(tif_r, 0, read_buffer, buffer_size);
            TIFFClose(tif_r);
        }
        
        free(read_buffer);
    } else {
        TIFFClose(tif_r);
    }
    
    cleanup_dummy_file();
    return 0;
}