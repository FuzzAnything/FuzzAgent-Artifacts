// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_tRNS at pngset.c:1144:1 in png.h
// png_set_tRNS at pngset.c:1144:1 in png.h
// png_get_tRNS at pngget.c:1202:1 in png.h
// png_get_color_type at pngget.c:89:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_process_data at pngpread.c:50:1 in png.h
#include <png.h>
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <setjmp.h>
#include <cstdio>

// Global jump buffer to handle libpng errors gracefully
static jmp_buf fuzzer_jmp_buf;

// Custom error handler that triggers the jump buffer
static void custom_error_handler(png_structp png_ptr, png_const_charp error_message) {
    (void)png_ptr;
    (void)error_message;
    longjmp(fuzzer_jmp_buf, 1);
}

// Custom warning handler (optional, just suppresses warnings)
static void custom_warning_handler(png_structp png_ptr, png_const_charp warning_message) {
    (void)png_ptr;
    (void)warning_message;
}

extern "C" int LLVMFuzzerTestOneInput_71(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;

    // Set up the jump buffer
    if (setjmp(fuzzer_jmp_buf) != 0) {
        // If we jump here, an error occurred in libpng. Clean up and return.
        if (png_ptr != nullptr) {
            if (info_ptr != nullptr) {
                png_destroy_info_struct(png_ptr, &info_ptr);
            }
            png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        }
        return 0;
    }

    // Create the PNG read structure
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (png_ptr == nullptr) {
        return 0;
    }

    // Set custom error and warning handlers
    png_set_error_fn(png_ptr, nullptr, custom_error_handler, custom_warning_handler);

    // Create the info structure
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == nullptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // To use png_process_data, we must be in push mode.
    // We need to initialize the read structure with a dummy file pointer or set the read function.
    // Since we are fuzzing with raw data, we can use png_set_read_fn with a dummy file 
    // or simply initialize the push mode without a file if we feed data directly.
    // However, png_process_data internally calls png_read_data which expects a valid io_ptr or read_data_fn.
    // A safe way to fuzz png_process_data is to set up a dummy file or use a memory-based approach.
    // Here, we create a dummy file to satisfy the read function requirement, even though we feed data directly.
    
    // Create a dummy file to avoid NULL pointer dereference in fread inside libpng
    FILE *dummy_file = fopen("./dummy_file", "wb");
    if (dummy_file) {
        // Write some dummy data to ensure the file exists and is valid
        fwrite(Data, 1, Size, dummy_file);
        fclose(dummy_file);
    }

    // Re-open for reading
    dummy_file = fopen("./dummy_file", "rb");
    if (dummy_file) {
        // Initialize the read function to use this file
        png_set_read_fn(png_ptr, dummy_file, nullptr);
        
        // Now we can safely call png_process_data
        // We cast Data to png_byte* as required
        png_process_data(png_ptr, info_ptr, const_cast<png_byte*>(Data), Size);
        
        fclose(dummy_file);
    } else {
        // If we can't create/open the dummy file, we skip png_process_data to avoid crashes
        // This is a fallback to prevent the SEGV
    }

    // Prepare data for png_set_tRNS
    png_byte trans_alpha[256];
    png_color_16 trans_color;
    memset(trans_alpha, 0, sizeof(trans_alpha));
    memset(&trans_color, 0, sizeof(trans_color));

    // Fuzz png_set_tRNS for paletted images
    int num_trans = (Size > 0) ? (Data[0] % 256) : 0;
    // Ensure num_trans is non-negative and within bounds
    if (num_trans < 0) num_trans = 0;
    if (num_trans > 256) num_trans = 256;
    
    // Fill trans_alpha with fuzz data if available
    if (Size > num_trans) {
        memcpy(trans_alpha, Data, num_trans);
    }
    
    png_set_tRNS(png_ptr, info_ptr, trans_alpha, num_trans, nullptr);

    // Fuzz png_set_tRNS for grayscale/RGB images
    if (Size > 4) {
        trans_color.red = Data[1];
        trans_color.green = Data[2];
        trans_color.blue = Data[3];
        trans_color.index = Data[4];
    }
    png_set_tRNS(png_ptr, info_ptr, nullptr, 0, &trans_color);

    // Fuzz png_get_tRNS
    png_byte *get_trans_alpha = nullptr;
    int get_num_trans = 0;
    png_color_16 *get_trans_color = nullptr;
    png_uint_32 has_trans = png_get_tRNS(png_ptr, info_ptr, &get_trans_alpha, &get_num_trans, &get_trans_color);
    (void)has_trans;

    // Fuzz png_get_color_type
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    (void)color_type;

    // Cleanup
    png_destroy_info_struct(png_ptr, &info_ptr);
    png_destroy_read_struct(&png_ptr, nullptr, nullptr);

    // Remove the dummy file
    remove("./dummy_file");

    return 0;
}