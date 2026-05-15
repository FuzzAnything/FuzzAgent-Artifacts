// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClientOpen at tif_open.c:289:7 in tiffio.h
// TIFFIsBigTIFF at tif_open.c:907:5 in tiffio.h
// TIFFGetSeekProc at tif_open.c:922:14 in tiffio.h
// TIFFGetReadProc at tif_open.c:912:19 in tiffio.h
// TIFFIsByteSwapped at tif_open.c:884:5 in tiffio.h
// TIFFSwabShort at tif_swab.c:33:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int dummy_close_proc(thandle_t handle) {
    FILE* file = (FILE*)handle;
    if (file) {
        fclose(file);
    }
    return 0;
}

static tmsize_t dummy_read_proc(thandle_t handle, void* buf, tmsize_t size) {
    FILE* file = (FILE*)handle;
    return fread(buf, 1, size, file);
}

static tmsize_t dummy_write_proc(thandle_t handle, void* buf, tmsize_t size) {
    FILE* file = (FILE*)handle;
    return fwrite(buf, 1, size, file);
}

static toff_t dummy_seek_proc(thandle_t handle, toff_t offset, int whence) {
    FILE* file = (FILE*)handle;
    if (fseek(file, offset, whence) != 0) {
        return (toff_t)-1;
    }
    return ftell(file);
}

static int dummy_map_proc(thandle_t handle, void** base, toff_t* size) {
    return 0;
}

static void dummy_unmap_proc(thandle_t handle, void* base, toff_t size) {
    // Do nothing
}

static toff_t dummy_size_proc(thandle_t handle) {
    FILE* file = (FILE*)handle;
    long current = ftell(file);
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, current, SEEK_SET);
    return size;
}

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    // Write fuzzed data to dummy file
    FILE* dummy_file = fopen("./dummy_file", "wb");
    if (!dummy_file) {
        return 0;
    }
    fwrite(Data, 1, Size, dummy_file);
    fclose(dummy_file);
    
    // Open dummy file for reading
    dummy_file = fopen("./dummy_file", "rb");
    if (!dummy_file) {
        unlink("./dummy_file");
        return 0;
    }
    
    // Create TIFF handle with dummy I/O procedures
    TIFF* tif = TIFFClientOpen("dummy", "r", dummy_file,
                               dummy_read_proc, dummy_write_proc,
                               dummy_seek_proc, dummy_close_proc,
                               dummy_size_proc, dummy_map_proc, dummy_unmap_proc);
    
    if (tif) {
        // Call functions in specified order
        int is_bigtiff = TIFFIsBigTIFF(tif);
        TIFFSeekProc seek_proc = TIFFGetSeekProc(tif);
        TIFFReadWriteProc read_proc = TIFFGetReadProc(tif);
        int is_swapped = TIFFIsByteSwapped(tif);
        
        // Prepare data for TIFFSwabShort
        uint16_t short_value = 0;
        if (Size >= sizeof(uint16_t)) {
            memcpy(&short_value, Data, sizeof(uint16_t));
        }
        TIFFSwabShort(&short_value);
        
        // Clean up
        TIFFClose(tif);
    } else {
        fclose(dummy_file);
    }
    
    // Remove dummy file
    unlink("./dummy_file");
    
    return 0;
}