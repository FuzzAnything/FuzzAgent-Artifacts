// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSwabFloat at tif_swab.c:165:6 in tiffio.h
// TIFFSwabArrayOfFloat at tif_swab.c:180:6 in tiffio.h
// TIFFSwabArrayOfShort at tif_swab.c:81:6 in tiffio.h
// TIFFSwabArrayOfDouble at tif_swab.c:222:6 in tiffio.h
// TIFFSwabArrayOfLong at tif_swab.c:117:6 in tiffio.h
// TIFFSwabArrayOfLong8 at tif_swab.c:138:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "libtiff/tiffio.h"

static void fill_buffer(void *buffer, size_t buffer_size, const uint8_t *data, size_t data_size) {
    if (data_size == 0) {
        memset(buffer, 0, buffer_size);
        return;
    }
    uint8_t *buf = (uint8_t*)buffer;
    for (size_t i = 0; i < buffer_size; i++) {
        buf[i] = data[i % data_size];
    }
}

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    // Test TIFFSwabFloat with a single float
    float single_float;
    fill_buffer(&single_float, sizeof(float), Data, Size);
    TIFFSwabFloat(&single_float);

    // Test array functions with arrays of 100 elements
    const size_t array_size = 100;
    
    // TIFFSwabArrayOfFloat
    float float_array[array_size];
    fill_buffer(float_array, sizeof(float) * array_size, Data, Size);
    TIFFSwabArrayOfFloat(float_array, array_size);
    
    // TIFFSwabArrayOfShort
    uint16_t short_array[array_size];
    fill_buffer(short_array, sizeof(uint16_t) * array_size, Data, Size);
    TIFFSwabArrayOfShort(short_array, array_size);
    
    // TIFFSwabArrayOfDouble
    double double_array[array_size];
    fill_buffer(double_array, sizeof(double) * array_size, Data, Size);
    TIFFSwabArrayOfDouble(double_array, array_size);
    
    // TIFFSwabArrayOfLong
    uint32_t long_array[array_size];
    fill_buffer(long_array, sizeof(uint32_t) * array_size, Data, Size);
    TIFFSwabArrayOfLong(long_array, array_size);
    
    // TIFFSwabArrayOfLong8
    uint64_t long8_array[array_size];
    fill_buffer(long8_array, sizeof(uint64_t) * array_size, Data, Size);
    TIFFSwabArrayOfLong8(long8_array, array_size);

    return 0;
}