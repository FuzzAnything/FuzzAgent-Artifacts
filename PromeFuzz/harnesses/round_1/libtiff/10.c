// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSwabShort at tif_swab.c:33:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClientOpen at tif_open.c:289:7 in tiffio.h
// TIFFIsBigTIFF at tif_open.c:907:5 in tiffio.h
// TIFFGetSeekProc at tif_open.c:922:14 in tiffio.h
// TIFFGetReadProc at tif_open.c:912:19 in tiffio.h
// TIFFIsByteSwapped at tif_open.c:884:5 in tiffio.h
// TIFFSwabShort at tif_swab.c:33:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "libtiff/tiffio.h"

static int dummy_close(thandle_t handle) {
    // Close the dummy file
    FILE* file = (FILE*)handle;
    if (file) {
        fclose(file);
    }
    return 0;
}

static tmsize_t dummy_read(thandle_t handle, void* buf, tmsize_t size) {
    // Read from the dummy file
    FILE* file = (FILE*)handle;
    return fread(buf, 1, size, file);
}

static tmsize_t dummy_write(thandle_t handle, void* buf, tmsize_t size) {
    // Write to the dummy file
    FILE* file = (FILE*)handle;
    return fwrite(buf, 1, size, file);
}

static toff_t dummy_seek(thandle_t handle, toff_t offset, int whence) {
    // Seek in the dummy file
    FILE* file = (FILE*)handle;
    if (fseek(file, offset, whence) != 0) {
        return -1;
    }
    return ftell(file);
}

static int dummy_map(thandle_t handle, void** base, toff_t* size) {
    // Not implemented
    return 0;
}

static void dummy_unmap(thandle_t handle, void* base, toff_t size) {
    // Not implemented
}

static toff_t dummy_size(thandle_t handle) {
    // Get size of dummy file
    FILE* file = (FILE*)handle;
    long current = ftell(file);
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, current, SEEK_SET);
    return size;
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    // Write fuzzer data to dummy file
    FILE* dummy_file = fopen("./dummy_file", "wb");
    if (!dummy_file) {
        return 0;
    }
    
    fwrite(Data, 1, Size, dummy_file);
    fclose(dummy_file);
    
    // Open dummy file for reading
    dummy_file = fopen("./dummy_file", "rb");
    if (!dummy_file) {
        return 0;
    }
    
    // Create TIFF handle using dummy I/O functions
    TIFF* tif = TIFFClientOpen("dummy_file", "r", (thandle_t)dummy_file,
                               dummy_read, dummy_write,
                               dummy_seek, dummy_close,
                               dummy_size, dummy_map, dummy_unmap);
    
    if (tif) {
        // Call target functions in specified order
        int is_big_tiff = TIFFIsBigTIFF(tif);
        TIFFSeekProc seek_proc = TIFFGetSeekProc(tif);
        TIFFReadWriteProc read_proc = TIFFGetReadProc(tif);
        int is_byte_swapped = TIFFIsByteSwapped(tif);
        
        // Test TIFFSwabShort with various values
        uint16_t test_values[] = {0x0001, 0x0100, 0x1234, 0xABCD, 0xFFFF};
        for (size_t i = 0; i < sizeof(test_values)/sizeof(test_values[0]); i++) {
            uint16_t value = test_values[i];
            TIFFSwabShort(&value);
        }
        
        // Also test with fuzzer data if we have at least 2 bytes
        if (Size >= 2) {
            uint16_t fuzz_value;
            memcpy(&fuzz_value, Data, sizeof(fuzz_value));
            TIFFSwabShort(&fuzz_value);
        }
        
        // Clean up
        TIFFClose(tif);
    } else {
        fclose(dummy_file);
    }
    
    // Remove dummy file
    remove("./dummy_file");
    
    return 0;
}