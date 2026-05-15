// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSwabShort at tif_swab.c:33:6 in tiffio.h
// TIFFSwabLong at tif_swab.c:45:6 in tiffio.h
// TIFFSwabArrayOfShort at tif_swab.c:81:6 in tiffio.h
// TIFFSwabArrayOfTriples at tif_swab.c:99:6 in tiffio.h
// TIFFSwabArrayOfFloat at tif_swab.c:180:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFIsByteSwapped at tif_open.c:884:5 in tiffio.h
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

static void dummy_file_cleanup(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    // Create dummy file for TIFF operations if needed
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Test TIFFSwabShort
    if (Size >= sizeof(uint16_t)) {
        uint16_t short_val;
        memcpy(&short_val, Data, sizeof(uint16_t));
        TIFFSwabShort(&short_val);
    }
    
    // Test TIFFSwabLong
    if (Size >= sizeof(uint32_t)) {
        uint32_t long_val;
        memcpy(&long_val, Data, sizeof(uint32_t));
        TIFFSwabLong(&long_val);
    }
    
    // Test TIFFSwabArrayOfShort
    if (Size >= 2 * sizeof(uint16_t)) {
        size_t n_shorts = Size / sizeof(uint16_t);
        if (n_shorts > 1024) n_shorts = 1024; // Limit allocation
        uint16_t *short_array = (uint16_t *)malloc(n_shorts * sizeof(uint16_t));
        if (short_array) {
            memcpy(short_array, Data, n_shorts * sizeof(uint16_t));
            TIFFSwabArrayOfShort(short_array, n_shorts);
            free(short_array);
        }
    }
    
    // Test TIFFSwabArrayOfTriples
    if (Size >= 3) { // Minimum 1 triple (3 bytes)
        size_t n_triples = Size / 3;
        if (n_triples > 1024) n_triples = 1024; // Limit allocation
        uint8_t *triples = (uint8_t *)malloc(n_triples * 3);
        if (triples) {
            memcpy(triples, Data, n_triples * 3);
            TIFFSwabArrayOfTriples(triples, n_triples);
            free(triples);
        }
    }
    
    // Test TIFFSwabArrayOfFloat
    if (Size >= sizeof(float)) {
        size_t n_floats = Size / sizeof(float);
        if (n_floats > 1024) n_floats = 1024; // Limit allocation
        float *float_array = (float *)malloc(n_floats * sizeof(float));
        if (float_array) {
            memcpy(float_array, Data, n_floats * sizeof(float));
            TIFFSwabArrayOfFloat(float_array, n_floats);
            free(float_array);
        }
    }
    
    // Test TIFFIsByteSwapped - create minimal TIFF structure
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (tif) {
        int swapped = TIFFIsByteSwapped(tif);
        (void)swapped; // Use result to avoid unused variable warning
        TIFFClose(tif);
    }
    
    // Clean up dummy file
    dummy_file_cleanup();
    
    return 0;
}