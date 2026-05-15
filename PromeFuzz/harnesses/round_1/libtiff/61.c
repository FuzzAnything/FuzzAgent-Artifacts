// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFWriteBufferSetup at tif_write.c:677:5 in tiffio.h
// TIFFWriteBufferSetup at tif_write.c:677:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadBufferSetup at tif_read.c:1375:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteEncodedTile at tif_write.c:414:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadEncodedTile at tif_read.c:953:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteRawStrip at tif_write.c:328:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteEncodedStrip at tif_write.c:215:10 in tiffio.h
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

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress libtiff error/warning messages during fuzzing
}

static int create_dummy_tiff(const uint8_t* data, size_t size, const char* filename, int write_mode) {
    FILE* f = fopen(filename, write_mode ? "wb" : "wb+");
    if (!f) return 0;
    
    // Write minimal TIFF header for testing
    uint8_t header[] = {
        0x49, 0x49, 0x2A, 0x00, // Little-endian, classic TIFF
        0x08, 0x00, 0x00, 0x00, // First IFD offset
        0x00, 0x00,             // Number of directory entries
        0x00, 0x00, 0x00, 0x00  // Next IFD offset (none)
    };
    fwrite(header, 1, sizeof(header), f);
    
    // Write fuzzer data if provided
    if (size > 0 && data) {
        fwrite(data, 1, size > 1024 ? 1024 : size, f);
    }
    
    fclose(f);
    return 1;
}

int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
    static int initialized = 0;
    if (!initialized) {
        TIFFSetErrorHandler(dummy_handler);
        TIFFSetWarningHandler(dummy_handler);
        initialized = 1;
    }
    
    const char* filename = "./dummy_file";
    
    // Test 1: TIFFWriteRawStrip
    if (Size > 8) {
        if (create_dummy_tiff(Data, Size, filename, 1)) {
            TIFF* tif = TIFFOpen(filename, "w");
            if (tif) {
                // Set required tags for strip-based TIFF
                TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 100);
                TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 100);
                TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
                TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
                TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, 100);
                TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
                TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
                TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
                
                uint32_t strip = 0;
                tmsize_t cc = Size > 1024 ? 1024 : Size;
                void* buf = malloc(cc);
                if (buf) {
                    memcpy(buf, Data, cc);
                    TIFFWriteRawStrip(tif, strip, buf, cc);
                    free(buf);
                }
                TIFFClose(tif);
            }
            unlink(filename);
        }
    }
    
    // Test 2: TIFFWriteEncodedStrip
    if (Size > 12) {
        if (create_dummy_tiff(Data, Size, filename, 1)) {
            TIFF* tif = TIFFOpen(filename, "w");
            if (tif) {
                TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 100);
                TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 100);
                TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
                TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 3);
                TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, 100);
                TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_SEPARATE);
                TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
                TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_LZW);
                
                uint32_t strip = 0;
                tmsize_t cc = Size > 1024 ? 1024 : Size;
                void* buf = malloc(cc);
                if (buf) {
                    memcpy(buf, Data, cc);
                    TIFFWriteEncodedStrip(tif, strip, buf, cc);
                    free(buf);
                }
                TIFFClose(tif);
            }
            unlink(filename);
        }
    }
    
    // Test 3: TIFFWriteBufferSetup
    if (Size > 16) {
        if (create_dummy_tiff(Data, Size, filename, 1)) {
            TIFF* tif = TIFFOpen(filename, "w");
            if (tif) {
                tmsize_t buf_size = Size > 2048 ? 2048 : Size;
                void* buffer = malloc(buf_size);
                if (buffer) {
                    memcpy(buffer, Data, buf_size);
                    TIFFWriteBufferSetup(tif, buffer, buf_size);
                    // Also test with NULL buffer
                    TIFFWriteBufferSetup(tif, NULL, -1);
                    free(buffer);
                }
                TIFFClose(tif);
            }
            unlink(filename);
        }
    }
    
    // Test 4: TIFFReadBufferSetup
    if (Size > 20) {
        if (create_dummy_tiff(Data, Size, filename, 0)) {
            TIFF* tif = TIFFOpen(filename, "r");
            if (tif) {
                tmsize_t buf_size = Size > 2048 ? 2048 : Size;
                void* buffer = malloc(buf_size);
                if (buffer) {
                    memcpy(buffer, Data, buf_size);
                    TIFFReadBufferSetup(tif, buffer, buf_size);
                    free(buffer);
                }
                TIFFClose(tif);
            }
            unlink(filename);
        }
    }
    
    // Test 5: TIFFWriteEncodedTile
    if (Size > 24) {
        if (create_dummy_tiff(Data, Size, filename, 1)) {
            TIFF* tif = TIFFOpen(filename, "w");
            if (tif) {
                TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 256);
                TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 256);
                TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
                TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
                TIFFSetField(tif, TIFFTAG_TILEWIDTH, 64);
                TIFFSetField(tif, TIFFTAG_TILELENGTH, 64);
                TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
                TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
                TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_JPEG);
                
                uint32_t tile = 0;
                tmsize_t cc = Size > 4096 ? 4096 : Size;
                void* buf = malloc(cc);
                if (buf) {
                    memcpy(buf, Data, cc);
                    TIFFWriteEncodedTile(tif, tile, buf, cc);
                    free(buf);
                }
                TIFFClose(tif);
            }
            unlink(filename);
        }
    }
    
    // Test 6: TIFFReadEncodedTile
    if (Size > 28) {
        if (create_dummy_tiff(Data, Size, filename, 0)) {
            TIFF* tif = TIFFOpen(filename, "r");
            if (tif) {
                uint32_t tile = 0;
                tmsize_t buf_size = Size > 4096 ? 4096 : Size;
                void* buffer = malloc(buf_size);
                if (buffer) {
                    TIFFReadEncodedTile(tif, tile, buffer, buf_size);
                    free(buffer);
                }
                TIFFClose(tif);
            }
            unlink(filename);
        }
    }
    
    return 0;
}