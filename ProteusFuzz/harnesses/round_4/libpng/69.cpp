// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_longjmp at pngerror.c:663:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_set_interlace_handling at pngtrans.c:98:1 in png.h
// png_set_rgb_to_gray_fixed at pngrtran.c:1013:1 in png.h
// png_get_rgb_to_gray_status at pngget.c:1264:1 in png.h
// png_start_read_image at pngread.c:291:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_benign_errors at pngset.c:1994:1 in png.h
// png_set_check_for_invalid_index at pngset.c:2024:1 in png.h
// png_set_option at png.c:3740:1 in png.h
#include <png.h>
#include <cstdint>
#include <cstddef>
#include <setjmp.h>
#include <vector>
#include <cstring>

// Dummy callbacks to prevent libpng from aborting on errors/warnings during fuzzing
static void dummy_error_callback(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    png_longjmp(png_ptr, 1);
}

static void dummy_warning_callback(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

// Helper structure for reading from a buffer
struct png_read_state {
    const uint8_t* data;
    size_t offset;
    size_t size;
};

// Static read callback function
static void dummy_read_callback(png_structp png_ptr, png_bytep data, png_size_t length) {
    png_read_state* s = (png_read_state*)png_get_io_ptr(png_ptr);
    if (s->offset + length > s->size) {
        length = s->size - s->offset;
    }
    if (length == 0) {
        return;
    }
    memcpy(data, s->data + s->offset, length);
    s->offset += length;
}

extern "C" int LLVMFuzzerTestOneInput_69(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create read structure
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        return 0;
    }

    // Create info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // Set error callbacks to prevent crashes during fuzzing
    png_set_error_fn(png_ptr, nullptr, dummy_error_callback, dummy_warning_callback);

    // Use setjmp to catch errors triggered by libpng functions
    if (setjmp(png_jmpbuf(png_ptr)) != 0) {
        // An error occurred, clean up and return
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    // 1. png_set_benign_errors
    int benign_flag = (Size > 0 && Data[0] % 2 == 0) ? 1 : 0;
    png_set_benign_errors(png_ptr, benign_flag);

    // 5. png_set_check_for_invalid_index
    int check_index_flag = (Size > 1 && Data[1] % 2 == 0) ? 1 : 0;
    png_set_check_for_invalid_index(png_ptr, check_index_flag);

    // 4. png_set_option
    int option_idx = (Size > 2) ? (Data[2] % 256) : 0;
    if (option_idx % 2 != 0) {
        option_idx += 1; // Make it even
    }
    int onoff = (Size > 3) ? (Data[3] % 2) : 1;
    int prev_state = png_set_option(png_ptr, option_idx, onoff);
    (void)prev_state;

    // Prepare a minimal valid PNG header to allow reading IHDR
    std::vector<uint8_t> png_data;
    // PNG signature
    png_data.push_back(137);
    png_data.push_back(80);
    png_data.push_back(78);
    png_data.push_back(71);
    png_data.push_back(13);
    png_data.push_back(10);
    png_data.push_back(26);
    png_data.push_back(10);
    // IHDR chunk
    // Length: 13
    png_data.push_back(0);
    png_data.push_back(0);
    png_data.push_back(0);
    png_data.push_back(13);
    // Type: "IHDR"
    png_data.push_back('I');
    png_data.push_back('H');
    png_data.push_back('D');
    png_data.push_back('R');
    // Data: width (4 bytes), height (4 bytes), bit depth (1), color type (1), compression (1), filter (1), interlace (1)
    png_data.push_back(0);
    png_data.push_back(0);
    png_data.push_back(0);
    png_data.push_back(1); // width
    png_data.push_back(0);
    png_data.push_back(0);
    png_data.push_back(0);
    png_data.push_back(1); // height
    png_data.push_back(8); // bit depth
    png_data.push_back(2); // color type (RGB)
    png_data.push_back(0); // compression
    png_data.push_back(0); // filter
    png_data.push_back(0); // interlace
    // CRC: dummy value
    png_data.push_back(0);
    png_data.push_back(0);
    png_data.push_back(0);
    png_data.push_back(0);

    // Set up the read function to read from this buffer
    png_read_state state;
    state.data = png_data.data();
    state.offset = 0;
    state.size = png_data.size();

    png_set_read_fn(png_ptr, &state, dummy_read_callback);

    // Read the IHDR chunk
    png_read_info(png_ptr, info_ptr);

    // 6. png_set_interlace_handling
    int passes = png_set_interlace_handling(png_ptr);
    (void)passes;

    // 2. png_set_rgb_to_gray_fixed
    int error_action = (Size > 4) ? (Data[4] % 3) : 0;
    png_fixed_point red_weight = (Size > 5) ? (Data[5] * 1000) : 30000;
    png_fixed_point green_weight = (Size > 6) ? (Data[6] * 1000) : 59000;
    if (red_weight > 100000) red_weight = 100000;
    if (green_weight > 100000) green_weight = 100000;
    png_set_rgb_to_gray_fixed(png_ptr, error_action, red_weight, green_weight);

    // Check status
    png_byte status = png_get_rgb_to_gray_status(png_ptr);
    (void)status;

    // 3. png_start_read_image
    // This might fail if we don't have IDAT chunks, but it's safe to try under setjmp
    png_start_read_image(png_ptr);

    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    return 0;
}