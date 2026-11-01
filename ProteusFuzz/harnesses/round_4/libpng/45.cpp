// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_process_data at pngpread.c:50:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_process_data at pngpread.c:50:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_process_data at pngpread.c:50:1 in png.h
// png_get_color_type at pngget.c:89:1 in png.h
// png_get_tRNS at pngget.c:1202:1 in png.h
// png_set_tRNS at pngset.c:1144:1 in png.h
// png_set_tRNS at pngset.c:1144:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
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

// Custom error handler to prevent libpng from calling abort/exit during fuzzing
static void
fuzz_png_error(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    // Silently ignore errors to allow fuzzing to continue
}

static void
fuzz_png_warning(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
    // Silently ignore warnings
}

// Helper to create a minimal PNG header for testing push reading
// Requires buffer size of at least 33 bytes (8 signature + 25 IHDR)
static void
create_dummy_png_header(uint8_t *buffer, size_t size) {
    if (size < 33) return;
    
    // PNG Signature (8 bytes)
    buffer[0] = 0x89; buffer[1] = 0x50; buffer[2] = 0x4E; buffer[3] = 0x47;
    buffer[4] = 0x0D; buffer[5] = 0x0A; buffer[6] = 0x1A; buffer[7] = 0x0A;
    
    // IHDR chunk
    // Length: 13 (0x00 0x00 0x00 0x0D)
    buffer[8] = 0x00; buffer[9] = 0x00; buffer[10] = 0x00; buffer[11] = 0x0D; 
    // Type: IHDR
    buffer[12] = 0x49; buffer[13] = 0x48; buffer[14] = 0x44; buffer[15] = 0x52; 
    
    // Width: 1
    buffer[16] = 0x00; buffer[17] = 0x00; buffer[18] = 0x00; buffer[19] = 0x01;
    // Height: 1
    buffer[20] = 0x00; buffer[21] = 0x00; buffer[22] = 0x00; buffer[23] = 0x01;
    // Bit depth: 8
    buffer[24] = 0x08;
    // Color type: 2 (RGB)
    buffer[25] = 0x02;
    // Compression: 0
    buffer[26] = 0x00;
    // Filter: 0
    buffer[27] = 0x00;
    // Interlace: 0
    buffer[28] = 0x00;
    
    // Dummy CRC (4 bytes)
    buffer[29] = 0x00; buffer[30] = 0x00; buffer[31] = 0x00; buffer[32] = 0x00;
}

// Custom read function for push mode that reads from a buffer
static void
fuzz_read_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    // This function is not used in push mode, but we set it to avoid NULL pointer issues
    // In push mode, data is fed via png_process_data
    (void)png_ptr;
    (void)data;
    (void)length;
}

extern "C" int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create png_struct
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, fuzz_png_error, fuzz_png_warning);
    if (!png_ptr) return 0;

    // Create info_struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Setup read function (required even for push mode to avoid NULL pointer dereference)
    // We pass a dummy pointer as io_ptr, but in push mode, data is fed via png_process_data
    png_set_read_fn(png_ptr, NULL, fuzz_read_data); 

    // Prepare buffer for push reading
    uint8_t *buffer = (uint8_t *)malloc(Size);
    if (!buffer) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }
    memcpy(buffer, Data, Size);

    // Ensure we have at least a PNG signature for push mode to work
    if (Size >= 8) {
        // If the input doesn't start with PNG signature, prepend one for better coverage
        if (Data[0] != 0x89 || Data[1] != 0x50 || Data[2] != 0x4E || Data[3] != 0x47) {
            // Create a temporary buffer with valid header
            // We need 33 bytes for header + Size for data
            uint8_t *temp_buf = (uint8_t *)malloc(33 + Size);
            if (temp_buf) {
                create_dummy_png_header(temp_buf, 33 + Size);
                if (Size > 0) {
                    memcpy(temp_buf + 33, Data, Size);
                }
                // Use setjmp/longjmp protection for push reading
                if (setjmp(png_jmpbuf(png_ptr)) == 0) {
                    png_process_data(png_ptr, info_ptr, temp_buf, 33 + Size);
                }
                free(temp_buf);
            }
        } else {
            // Use setjmp/longjmp protection for push reading
            if (setjmp(png_jmpbuf(png_ptr)) == 0) {
                png_process_data(png_ptr, info_ptr, buffer, Size);
            }
        }
    } else {
        // Try to process small chunks even if not valid PNG
        if (setjmp(png_jmpbuf(png_ptr)) == 0) {
            png_process_data(png_ptr, info_ptr, buffer, Size);
        }
    }

    // Test png_get_color_type
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    (void)color_type; 

    // Test png_get_tRNS
    png_byte *trans_alpha = NULL;
    int num_trans = 0;
    png_color_16 trans_color_local = {0};
    png_color_16 *trans_color_ptr = &trans_color_local;
    png_uint_32 has_trans = png_get_tRNS(png_ptr, info_ptr, &trans_alpha, &num_trans, &trans_color_ptr);
    (void)has_trans;

    // Test png_set_tRNS with various parameters
    // Case 1: Palette mode (trans_alpha)
    png_byte trans_alpha_data[256];
    memset(trans_alpha_data, 0, sizeof(trans_alpha_data));
    // Randomize some values based on input
    if (Size > 0) {
        trans_alpha_data[0] = Data[0];
        if (Size > 1) trans_alpha_data[1] = Data[1];
    }
    png_set_tRNS(png_ptr, info_ptr, trans_alpha_data, (Size > 2) ? (int)(Data[2] % 256) : 10, NULL);

    // Case 2: Grayscale/RGB mode (trans_color)
    png_color_16 trans_color_data = {0};
    if (Size > 3) trans_color_data.red = Data[3];
    if (Size > 4) trans_color_data.green = Data[4];
    if (Size > 5) trans_color_data.blue = Data[5];
    png_set_tRNS(png_ptr, info_ptr, NULL, 0, &trans_color_data);

    // Cleanup
    free(buffer);
    png_destroy_info_struct(png_ptr, &info_ptr);
    png_destroy_read_struct(&png_ptr, NULL, NULL);

    return 0;
}