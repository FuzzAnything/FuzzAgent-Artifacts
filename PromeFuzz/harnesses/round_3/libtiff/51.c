// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSwabArrayOfShort at tif_swab.c:81:6 in tiffio.h
// TIFFSwabArrayOfTriples at tif_swab.c:99:6 in tiffio.h
// TIFFSwabDouble at tif_swab.c:201:6 in tiffio.h
// TIFFSwabArrayOfDouble at tif_swab.c:222:6 in tiffio.h
// TIFFSwabArrayOfLong at tif_swab.c:117:6 in tiffio.h
// TIFFSwabArrayOfLong8 at tif_swab.c:138:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <string.h>

int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    // Define a maximum size to avoid excessive memory usage
    const size_t MAX_SIZE = 1024 * 1024; // 1MB
    if (Size > MAX_SIZE) {
        Size = MAX_SIZE;
    }

    // Test TIFFSwabArrayOfShort
    if (Size >= sizeof(uint16_t)) {
        size_t num_shorts = Size / sizeof(uint16_t);
        uint16_t *short_array = (uint16_t *)Data;
        TIFFSwabArrayOfShort(short_array, (tmsize_t)num_shorts);
    }

    // Test TIFFSwabArrayOfTriples
    if (Size >= 3) {
        size_t num_triples = Size / 3;
        uint8_t *triple_array = (uint8_t *)Data;
        TIFFSwabArrayOfTriples(triple_array, (tmsize_t)num_triples);
    }

    // Test TIFFSwabDouble
    if (Size >= sizeof(double)) {
        double d;
        memcpy(&d, Data, sizeof(double));
        TIFFSwabDouble(&d);
    }

    // Test TIFFSwabArrayOfDouble
    if (Size >= sizeof(double)) {
        size_t num_doubles = Size / sizeof(double);
        double *double_array = (double *)Data;
        TIFFSwabArrayOfDouble(double_array, (tmsize_t)num_doubles);
    }

    // Test TIFFSwabArrayOfLong
    if (Size >= sizeof(uint32_t)) {
        size_t num_longs = Size / sizeof(uint32_t);
        uint32_t *long_array = (uint32_t *)Data;
        TIFFSwabArrayOfLong(long_array, (tmsize_t)num_longs);
    }

    // Test TIFFSwabArrayOfLong8
    if (Size >= sizeof(uint64_t)) {
        size_t num_long8s = Size / sizeof(uint64_t);
        uint64_t *long8_array = (uint64_t *)Data;
        TIFFSwabArrayOfLong8(long8_array, (tmsize_t)num_long8s);
    }

    return 0;
}