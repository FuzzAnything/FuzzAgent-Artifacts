// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_warning at pngerror.c:186:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_chunk_warning at pngerror.c:436:1 in png.h
// png_warning at pngerror.c:186:1 in png.h
// png_benign_error at pngerror.c:308:1 in png.h
// png_chunk_benign_error at pngerror.c:453:1 in png.h
// png_error at pngerror.c:50:1 in png.h
// png_chunk_error at pngerror.c:418:1 in png.h
// png_chunk_warning at pngerror.c:436:1 in png.h
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
#include <setjmp.h>

// Global jump buffer to handle libpng longjmps
static jmp_buf fuzz_jmp_buf;

// Custom error handler that triggers longjmp instead of aborting
static void fuzz_error_handler(png_structp png_ptr, const char *error_message) {
    (void)png_ptr;
    (void)error_message;
    // Trigger the longjmp to catch the error in the fuzzing harness
    longjmp(fuzz_jmp_buf, 1);
}

// Custom warning handler that does nothing
static void fuzz_warning_handler(png_structp png_ptr, const char *warning_message) {
    (void)png_ptr;
    (void)warning_message;
    // Silently ignore warnings
    return;
}

static png_structp create_png_struct() {
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        return nullptr;
    }
    // Install custom handlers
    png_set_error_fn(png_ptr, NULL, fuzz_error_handler, fuzz_warning_handler);
    return png_ptr;
}

static void cleanup_png_struct(png_structp png_ptr) {
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
    }
}

extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a valid png_struct for testing
    png_structp png_ptr = create_png_struct();
    if (!png_ptr) {
        return 0;
    }

    // Extract a message from the fuzz input
    size_t msg_len = Size;
    if (msg_len > 256) {
        msg_len = 256;
    }
    char message[257];
    memcpy(message, Data, msg_len);
    message[msg_len] = '\0';

    // Test png_chunk_warning (safe, non-fatal)
    png_chunk_warning(png_ptr, message);

    // Test png_warning (safe, non-fatal)
    png_warning(png_ptr, message);

    // Test png_benign_error (can be fatal or warning depending on flags)
    png_benign_error(png_ptr, message);

    // Test png_chunk_benign_error
    png_chunk_benign_error(png_ptr, message);

    // Setup longjmp to catch fatal errors from png_error and png_chunk_error
    if (setjmp(fuzz_jmp_buf) == 0) {
        // First time through: call the functions that might trigger longjmp
        
        // Test png_error
        png_error(png_ptr, message);
        
        // Test png_chunk_error
        png_chunk_error(png_ptr, message);
    } else {
        // We jumped here due to an error. 
        // The error was handled, so we continue.
        // No need to do anything special, just proceed to cleanup.
    }

    // Test with NULL png_ptr for functions that allow it
    // png_chunk_warning allows NULL
    png_chunk_warning(NULL, message);
    
    // png_warning allows NULL
    png_warning(NULL, message);

    // Cleanup
    cleanup_png_struct(png_ptr);

    return 0;
}