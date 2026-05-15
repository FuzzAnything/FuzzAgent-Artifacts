// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSwabArrayOfTriples at tif_swab.c:99:6 in tiffio.h
// TIFFSwabArrayOfDouble at tif_swab.c:222:6 in tiffio.h
// TIFFSwabArrayOfLong at tif_swab.c:117:6 in tiffio.h
// TIFFSwabArrayOfLong8 at tif_swab.c:138:6 in tiffio.h
// TIFFSwabArrayOfFloat at tif_swab.c:180:6 in tiffio.h
// TIFFSwabDouble at tif_swab.c:201:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fill_buffer_with_data(uint8_t* buffer, size_t buffer_size, const uint8_t* data, size_t data_size) {
    if (data_size == 0) {
        memset(buffer, 0, buffer_size);
        return;
    }
    for (size_t i = 0; i < buffer_size; i++) {
        buffer[i] = data[i % data_size];
    }
}

int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size) {
    // Define buffer sizes for each array type
    const size_t buffer_size = 4096;
    
    // Test TIFFSwabArrayOfTriples
    {
        uint8_t triples[buffer_size];
        size_t triple_count = buffer_size / 3;
        fill_buffer_with_data(triples, buffer_size, Data, Size);
        TIFFSwabArrayOfTriples(triples, triple_count);
    }
    
    // Test TIFFSwabArrayOfDouble
    {
        double doubles[buffer_size / sizeof(double)];
        size_t double_count = buffer_size / sizeof(double);
        fill_buffer_with_data((uint8_t*)doubles, buffer_size, Data, Size);
        TIFFSwabArrayOfDouble(doubles, double_count);
    }
    
    // Test TIFFSwabArrayOfLong
    {
        uint32_t longs[buffer_size / sizeof(uint32_t)];
        size_t long_count = buffer_size / sizeof(uint32_t);
        fill_buffer_with_data((uint8_t*)longs, buffer_size, Data, Size);
        TIFFSwabArrayOfLong(longs, long_count);
    }
    
    // Test TIFFSwabArrayOfLong8
    {
        uint64_t long8s[buffer_size / sizeof(uint64_t)];
        size_t long8_count = buffer_size / sizeof(uint64_t);
        fill_buffer_with_data((uint8_t*)long8s, buffer_size, Data, Size);
        TIFFSwabArrayOfLong8(long8s, long8_count);
    }
    
    // Test TIFFSwabArrayOfFloat
    {
        float floats[buffer_size / sizeof(float)];
        size_t float_count = buffer_size / sizeof(float);
        fill_buffer_with_data((uint8_t*)floats, buffer_size, Data, Size);
        TIFFSwabArrayOfFloat(floats, float_count);
    }
    
    // Test TIFFSwabDouble
    {
        double d;
        fill_buffer_with_data((uint8_t*)&d, sizeof(double), Data, Size);
        TIFFSwabDouble(&d);
    }
    
    return 0;
}