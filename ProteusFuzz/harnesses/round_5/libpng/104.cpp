// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_shift at pngtrans.c:83:1 in png.h
// png_set_sBIT at pngset.c:804:1 in png.h
// png_get_sBIT at pngget.c:1140:1 in png.h
// png_read_png at pngread.c:957:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_channels at pngget.c:481:1 in png.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <png.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <vector>

static void fuzzer_error_callback(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    // Silently handle errors to prevent fuzzer from crashing on invalid input
}

static void fuzzer_warning_callback(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
    // Silently handle warnings
}

extern "C" int LLVMFuzzerTestOneInput_104(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Write fuzzing data to a temporary file
    const char* dummy_file = "./dummy_file";
    std::ofstream file(dummy_file, std::ios::binary);
    if (!file) {
        return 0;
    }
    file.write(reinterpret_cast<const char*>(Data), Size);
    file.close();

    // Create PNG read structure
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        return 0;
    }

    // Set custom error and warning handlers to prevent crashes
    png_set_error_fn(png_ptr, NULL, fuzzer_error_callback, fuzzer_warning_callback);

    // Create PNG info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Open file for reading
    FILE* fp = fopen(dummy_file, "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Initialize I/O
    png_init_io(png_ptr, fp);

    // Use longjmp to catch errors during reading
    if (setjmp(png_jmpbuf(png_ptr))) {
        fclose(fp);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Read PNG info (headers)
    png_read_info(png_ptr, info_ptr);

    // Fuzz png_get_channels
    png_byte channels = png_get_channels(png_ptr, info_ptr);
    (void)channels; // Use the result to avoid unused variable warning

    // Prepare png_color_8 for shift and sBIT operations
    png_color_8 true_bits;
    png_color_8 sig_bit;
    
    // Initialize with random values from input data
    true_bits.red = Data[0];
    true_bits.green = Data[1 % Size];
    true_bits.blue = Data[2 % Size];
    true_bits.gray = Data[3 % Size];
    true_bits.alpha = Data[4 % Size];

    sig_bit.red = Data[5 % Size];
    sig_bit.green = Data[6 % Size];
    sig_bit.blue = Data[7 % Size];
    sig_bit.gray = Data[8 % Size];
    sig_bit.alpha = Data[9 % Size];

    // Fuzz png_set_shift
    png_set_shift(png_ptr, &true_bits);

    // Fuzz png_set_sBIT
    png_set_sBIT(png_ptr, info_ptr, &sig_bit);

    // Fuzz png_get_sBIT
    png_color_8 *sig_bit_ptr = nullptr;
    png_uint_32 sbit_flag = png_get_sBIT(png_ptr, info_ptr, &sig_bit_ptr);
    (void)sbit_flag;
    (void)sig_bit_ptr;

    // Fuzz png_read_png
    // Use a transform flag derived from input data
    int transforms = (Data[10 % Size] & 0xFF);
    png_read_png(png_ptr, info_ptr, transforms, nullptr);

    // Cleanup
    fclose(fp);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return 0;
}