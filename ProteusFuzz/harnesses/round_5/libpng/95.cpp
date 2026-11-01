// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_save_uint_32 at pngwutil.c:41:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_get_uint_31 at pngrutil.c:41:1 in png.h
#include <png.h>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <setjmp.h>

// Forward declarations for static helper functions
static void dummy_error(png_structp png_ptr, png_const_charp error_msg);
static void dummy_warning(png_structp png_ptr, png_const_charp warning_msg);

// Global jump buffer to handle longjmp from png_error
static jmp_buf g_jmp_buf;

static void dummy_error(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    // Instead of aborting, longjmp to the fuzzer loop
    longjmp(g_jmp_buf, 1);
}

static void dummy_warning(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
    // Ignore warnings
}

extern "C" int LLVMFuzzerTestOneInput_95(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Allocate buffers for save operations
    png_byte buf4[4];
    png_byte buf2[2];

    // Extract data from input
    const png_byte *input = reinterpret_cast<const png_byte*>(Data);

    // Test png_get_uint_32
    // Ensure we have at least 4 bytes
    png_uint_32 val_u32 = png_get_uint_32(input);
    (void)val_u32; // Suppress unused variable warning

    // Test png_save_uint_32
    png_save_uint_32(buf4, val_u32);

    // Test png_get_int_32
    png_int_32 val_i32 = png_get_int_32(input);
    (void)val_i32;

    // Test png_save_int_32
    png_save_int_32(buf4, val_i32);

    // Test png_save_uint_16
    // Use first 2 bytes as input for 16-bit value if available, or just use a derived value
    unsigned int val_u16 = 0;
    if (Size >= 2) {
        val_u16 = (input[0] << 8) | input[1];
    } else {
        val_u16 = 0;
    }
    png_save_uint_16(buf2, val_u16);

    // Test png_get_uint_31
    // This function requires a valid png_struct pointer.
    // We initialize a static png_struct to avoid allocation overhead per run.
    static png_structp png_ptr = nullptr;
    static png_infop info_ptr = nullptr;
    static bool initialized = false;

    if (!initialized) {
        png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, dummy_error, dummy_warning);
        if (png_ptr) {
            info_ptr = png_create_info_struct(png_ptr);
            // Set up error handling to prevent crashes from png_error calls
            // The dummy_error function handles the jump/abort logic
            initialized = true;
        }
    }

    if (png_ptr && Size >= 4) {
        // Setup the jump buffer before calling png_get_uint_31
        // because it might call png_error if value > PNG_UINT_31_MAX
        if (setjmp(g_jmp_buf) == 0) {
            // png_get_uint_31 checks if value > PNG_UINT_31_MAX (2^31 - 1)
            // If it is, it calls png_error. Our dummy handler performs longjmp.
            // We pass the first 4 bytes.
            png_uint_32 val_u31 = png_get_uint_31(png_ptr, input);
            (void)val_u31;
        }
        // If longjmp happened, we skip the rest of this block and return
    }

    return 0;
}