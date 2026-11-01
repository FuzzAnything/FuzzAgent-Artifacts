// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_compression_method at pngwrite.c:1253:1 in png.h
// png_set_compression_strategy at pngwrite.c:1207:1 in png.h
// png_write_sig at pngwutil.c:68:1 in png.h
// png_write_chunk_end at pngwutil.c:163:1 in png.h
// png_write_flush at pngwrite.c:978:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
#include <png.h>
#include <cstdint>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>

// Dummy write callback
static void dummy_write(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

// Dummy flush callback
static void dummy_flush(png_structp png_ptr) {
    (void)png_ptr;
}

// Dummy error handler that does not abort
static void dummy_error(png_structp png_ptr, const char *error_message) {
    (void)png_ptr;
    (void)error_message;
    // In a real application, this would log and abort.
    // For fuzzing, we simply return to allow continued execution.
}

// Dummy warning handler
static void dummy_warning(png_structp png_ptr, const char *warning_message) {
    (void)png_ptr;
    (void)warning_message;
}

extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create write struct
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        return 0;
    }

    // Create info struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        return 0;
    }

    // Set custom callbacks to avoid file I/O and aborts
    png_set_write_fn(png_ptr, nullptr, dummy_write, dummy_flush);
    png_set_error_fn(png_ptr, nullptr, dummy_error, dummy_warning);

    // Initialize IO (though we use dummy callbacks)
    png_init_io(png_ptr, nullptr);

    // Fuzz png_set_compression_method
    if (Size >= 1) {
        int method = Data[0];
        png_set_compression_method(png_ptr, method);
    }

    // Fuzz png_set_compression_strategy
    if (Size >= 2) {
        int strategy = Data[1];
        png_set_compression_strategy(png_ptr, strategy);
    }

    // Fuzz png_write_sig
    png_write_sig(png_ptr);

    // Fuzz png_write_chunk_end (without start, to test error handling)
    // This might trigger an error, but our custom handler prevents abort
    png_write_chunk_end(png_ptr);

    // Fuzz png_write_flush
    png_write_flush(png_ptr);

    // We removed the explicit call to png_error because it triggers a longjmp/abort
    // even with a custom handler in some libpng versions if not handled via setjmp.
    // The error paths are covered by the invalid operations above (e.g., chunk_end without start).
    // If we must test png_error specifically, we would need to wrap it in a setjmp/longjmp block,
    // but libpng's internal error handling usually bypasses user code if not set up correctly.
    // Instead, we rely on the invalid API usage to trigger internal error paths safely.

    // Cleanup
    png_destroy_write_struct(&png_ptr, &info_ptr);

    return 0;
}