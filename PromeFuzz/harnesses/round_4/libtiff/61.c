// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFReadBufferSetup at tif_read.c:1375:5 in tiffio.h
// TIFFWriteBufferSetup at tif_write.c:677:5 in tiffio.h
// TIFFReadEncodedTile at tif_read.c:953:10 in tiffio.h
// TIFFWriteRawStrip at tif_write.c:328:10 in tiffio.h
// TIFFWriteEncodedTile at tif_write.c:414:10 in tiffio.h
// TIFFWriteEncodedStrip at tif_write.c:215:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
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

static void dummy_error_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress error messages during fuzzing
    (void)module;
    (void)fmt;
    (void)ap;
}

static void dummy_warning_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress warning messages during fuzzing
    (void)module;
    (void)fmt;
    (void)ap;
}

int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
    if (Size < 64) {
        return 0;
    }

    // Set up error handlers to suppress output
    TIFFSetErrorHandler(dummy_error_handler);
    TIFFSetWarningHandler(dummy_warning_handler);

    // Create a dummy TIFF file
    const char* filename = "./dummy_file";
    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    
    // Write minimal TIFF header (BigTIFF compatible)
    uint8_t header[64] = {0};
    // Classic TIFF header
    header[0] = 'I';
    header[1] = 'I';  // Little-endian
    header[2] = 42;   // Classic TIFF magic
    header[3] = 0;
    header[4] = 8;    // First IFD offset
    header[5] = 0;
    header[6] = 0;
    header[7] = 0;
    
    // Write some data to make it look like a TIFF
    fwrite(header, 1, sizeof(header), fp);
    if (Size > sizeof(header)) {
        fwrite(Data, 1, Size - sizeof(header), fp);
    }
    fclose(fp);

    // Open the TIFF file
    TIFF* tif = TIFFOpen(filename, "r+");
    if (!tif) {
        unlink(filename);
        return 0;
    }

    // Parse some parameters from the input data
    uint32_t tile = *(uint32_t*)Data;
    uint32_t strip = *(uint32_t*)(Data + 4);
    tmsize_t buffer_size = *(tmsize_t*)(Data + 8);
    if (buffer_size < 0) buffer_size = -buffer_size;
    if (buffer_size > 1024 * 1024) buffer_size = 1024 * 1024;  // Limit buffer size
    
    // Allocate buffers
    void* read_buffer = malloc(buffer_size > 0 ? buffer_size : 1024);
    void* write_buffer = malloc(buffer_size > 0 ? buffer_size : 1024);
    void* custom_buffer = malloc(buffer_size > 0 ? buffer_size : 1024);
    
    if (!read_buffer || !write_buffer || !custom_buffer) {
        free(read_buffer);
        free(write_buffer);
        free(custom_buffer);
        TIFFClose(tif);
        unlink(filename);
        return 0;
    }
    
    // Initialize buffers with fuzzer data
    size_t data_offset = 16;
    size_t data_remaining = Size > data_offset ? Size - data_offset : 0;
    size_t copy_size = buffer_size > 0 ? (buffer_size < data_remaining ? buffer_size : data_remaining) : 0;
    
    if (copy_size > 0 && data_remaining > 0) {
        memcpy(custom_buffer, Data + data_offset, copy_size);
        memcpy(write_buffer, Data + data_offset, copy_size > 1024 ? 1024 : copy_size);
    }

    // Test TIFFReadBufferSetup
    TIFFReadBufferSetup(tif, custom_buffer, buffer_size);
    
    // Test TIFFWriteBufferSetup
    TIFFWriteBufferSetup(tif, custom_buffer, buffer_size);
    
    // Test TIFFReadEncodedTile
    if (buffer_size > 0) {
        TIFFReadEncodedTile(tif, tile, read_buffer, buffer_size);
    }
    
    // Test TIFFWriteRawStrip
    if (buffer_size > 0) {
        TIFFWriteRawStrip(tif, strip, write_buffer, buffer_size > 1024 ? 1024 : buffer_size);
    }
    
    // Test TIFFWriteEncodedTile
    if (buffer_size > 0) {
        TIFFWriteEncodedTile(tif, tile, write_buffer, buffer_size > 1024 ? 1024 : buffer_size);
    }
    
    // Test TIFFWriteEncodedStrip
    if (buffer_size > 0) {
        TIFFWriteEncodedStrip(tif, strip, write_buffer, buffer_size > 1024 ? 1024 : buffer_size);
    }

    // Clean up
    free(read_buffer);
    free(write_buffer);
    free(custom_buffer);
    TIFFClose(tif);
    unlink(filename);
    
    return 0;
}