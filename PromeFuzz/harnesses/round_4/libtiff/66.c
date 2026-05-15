// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFReadEncodedTile at tif_read.c:953:10 in tiffio.h
// TIFFWriteEncodedTile at tif_write.c:414:10 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteRawStrip at tif_write.c:328:10 in tiffio.h
// TIFFWriteEncodedStrip at tif_write.c:215:10 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFReadBufferSetup at tif_read.c:1375:5 in tiffio.h
// TIFFWriteBufferSetup at tif_write.c:677:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
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
#include <fcntl.h>

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress libtiff warnings/errors during fuzzing
}

static TIFF* create_dummy_tiff(const uint8_t* data, size_t size, const char* filename) {
    // Create a dummy file with TIFF header
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return NULL;
    
    // Write minimal TIFF header (little-endian)
    uint8_t header[] = {
        0x49, 0x49,  // Little-endian
        0x2A, 0x00,  // TIFF version 42
        0x08, 0x00, 0x00, 0x00,  // IFD offset
        0x00, 0x00,  // Number of directory entries
        0x00, 0x00, 0x00, 0x00   // Next IFD offset
    };
    write(fd, header, sizeof(header));
    
    // Write some fuzzer data
    if (size > 0) {
        write(fd, data, size > 4096 ? 4096 : size);
    }
    
    close(fd);
    
    // Open the file with libtiff
    TIFFSetWarningHandler(dummy_handler);
    TIFFSetErrorHandler(dummy_handler);
    
    return TIFFOpen(filename, "r+");
}

static void cleanup_tiff(TIFF* tif, const char* filename) {
    if (tif) {
        TIFFClose(tif);
    }
    unlink(filename);
}

int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Use fuzzer data to derive parameters
    uint32_t param1 = *(uint32_t*)Data;
    tmsize_t param2 = (tmsize_t)(Size > 8 ? *(uint32_t*)(Data + 4) : 1024);
    uint32_t tile = param1 % 100;
    uint32_t strip = param1 % 10;
    
    // Allocate buffers
    size_t buf_size = param2 > 0 ? (param2 % 65536) : 1024;
    if (buf_size > Size) buf_size = Size;
    
    void* buffer1 = malloc(buf_size);
    void* buffer2 = malloc(buf_size);
    if (!buffer1 || !buffer2) {
        free(buffer1);
        free(buffer2);
        return 0;
    }
    
    if (buf_size > 0) {
        memcpy(buffer1, Data, buf_size);
        memcpy(buffer2, Data, buf_size > Size ? Size : buf_size);
    }
    
    // Test TIFFReadBufferSetup and TIFFWriteBufferSetup with a basic TIFF
    TIFF* tif1 = create_dummy_tiff(Data, Size, "./dummy_file1");
    if (tif1) {
        TIFFSetField(tif1, TIFFTAG_IMAGEWIDTH, 256);
        TIFFSetField(tif1, TIFFTAG_IMAGELENGTH, 256);
        TIFFSetField(tif1, TIFFTAG_BITSPERSAMPLE, 8);
        TIFFSetField(tif1, TIFFTAG_SAMPLESPERPIXEL, 1);
        TIFFSetField(tif1, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
        TIFFSetField(tif1, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
        TIFFSetField(tif1, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
        
        TIFFReadBufferSetup(tif1, buffer1, buf_size);
        TIFFWriteBufferSetup(tif1, buffer2, buf_size);
        
        cleanup_tiff(tif1, "./dummy_file1");
    }
    
    // Test tile operations with a tiled TIFF
    TIFF* tif2 = create_dummy_tiff(Data, Size, "./dummy_file2");
    if (tif2) {
        TIFFSetField(tif2, TIFFTAG_IMAGEWIDTH, 256);
        TIFFSetField(tif2, TIFFTAG_IMAGELENGTH, 256);
        TIFFSetField(tif2, TIFFTAG_BITSPERSAMPLE, 8);
        TIFFSetField(tif2, TIFFTAG_SAMPLESPERPIXEL, 1);
        TIFFSetField(tif2, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
        TIFFSetField(tif2, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
        TIFFSetField(tif2, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
        TIFFSetField(tif2, TIFFTAG_TILEWIDTH, 64);
        TIFFSetField(tif2, TIFFTAG_TILELENGTH, 64);
        
        void* read_buf = malloc(4096);
        if (read_buf) {
            TIFFReadEncodedTile(tif2, tile, read_buf, 4096);
            free(read_buf);
        }
        
        if (buf_size > 0) {
            TIFFWriteEncodedTile(tif2, tile, buffer1, buf_size);
        }
        
        cleanup_tiff(tif2, "./dummy_file2");
    }
    
    // Test strip operations with a stripped TIFF
    TIFF* tif3 = create_dummy_tiff(Data, Size, "./dummy_file3");
    if (tif3) {
        TIFFSetField(tif3, TIFFTAG_IMAGEWIDTH, 256);
        TIFFSetField(tif3, TIFFTAG_IMAGELENGTH, 256);
        TIFFSetField(tif3, TIFFTAG_BITSPERSAMPLE, 8);
        TIFFSetField(tif3, TIFFTAG_SAMPLESPERPIXEL, 1);
        TIFFSetField(tif3, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
        TIFFSetField(tif3, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
        TIFFSetField(tif3, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
        TIFFSetField(tif3, TIFFTAG_ROWSPERSTRIP, 16);
        
        if (buf_size > 0) {
            TIFFWriteRawStrip(tif3, strip, buffer1, buf_size);
            TIFFWriteEncodedStrip(tif3, strip, buffer2, buf_size);
        }
        
        cleanup_tiff(tif3, "./dummy_file3");
    }
    
    // Cleanup
    free(buffer1);
    free(buffer2);
    
    return 0;
}