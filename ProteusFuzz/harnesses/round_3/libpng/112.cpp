// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_save_uint_32 at pngwutil.c:41:1 in png.h
// png_get_uint_31 at pngrutil.c:41:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
// png_save_uint_32 at pngwutil.c:41:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
#include <png.h>
#include <cstdint>
#include <cstddef>
#include <cstring>

// Helper to create a minimal valid png_struct for error handling
static png_structp create_dummy_png_struct() {
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        return NULL;
    }
    // Create a dummy info struct as well, though not strictly used by the target functions,
    // it's good practice for a valid png_struct.
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return NULL;
    }
    return png_ptr;
}

extern "C" int LLVMFuzzerTestOneInput_112(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Prepare buffers
    png_byte buf16[2];
    png_byte buf32[4];

    // Extract values from input data
    unsigned int val16 = 0;
    png_uint_32 val32 = 0;
    png_int_32 sval32 = 0;

    // Safely construct 16-bit value
    if (Size >= 2) {
        val16 = (static_cast<unsigned int>(Data[0]) << 8) | Data[1];
    }

    // Safely construct 32-bit value
    if (Size >= 4) {
        val32 = (static_cast<png_uint_32>(Data[0]) << 24) |
                (static_cast<png_uint_32>(Data[1]) << 16) |
                (static_cast<png_uint_32>(Data[2]) << 8) |
                (static_cast<png_uint_32>(Data[3]));
        sval32 = static_cast<png_int_32>(val32);
    }

    // 1. Test png_save_uint_16
    png_save_uint_16(buf16, val16);

    // 2. Test png_get_uint_32 (requires 4 bytes)
    // We write some data to buf32 first to ensure we read something valid
    buf32[0] = Data[0];
    buf32[1] = Data[1];
    buf32[2] = Data[2];
    buf32[3] = Data[3];
    png_uint_32 read32 = png_get_uint_32(buf32);
    (void)read32;

    // 3. Test png_save_int_32
    png_save_int_32(buf32, sval32);

    // 4. Test png_save_uint_32
    png_save_uint_32(buf32, val32);

    // 5. Test png_get_int_32
    png_int_32 readS32 = png_get_int_32(buf32);
    (void)readS32;

    // 6. Test png_get_uint_31
    // This function may call png_error if the value > PNG_UINT_31_MAX.
    // To prevent the fuzzer from crashing due to unhandled longjmp/abort,
    // we create a valid png_struct and use setjmp/longjmp handling if necessary,
    // OR simply ensure we don't pass nullptr.
    // However, png_get_uint_31 calls png_error which calls longjmp(png_ptr->jmp_buf).
    // If we don't set up the jmp_buf, it will crash.
    // The safest way in a fuzzer for this specific API is to avoid triggering the error
    // or handle the jump. Since we can't easily catch the jump without complex setup,
    // we will clamp the value to be safe or use a try-catch if C++ exceptions were on (they aren't for this).
    // Actually, the standard way to fuzz libpng functions that might error is to initialize
    // the png_struct properly so the longjmp works, but we don't want to crash the fuzzer process.
    // A simpler approach: Only call png_get_uint_31 if the value is known to be safe,
    // OR accept that if it errors, it's a valid "crash" finding if the library is misused.
    // But the error trace shows it's crashing because png_ptr was NULL in the previous attempt.
    // Now we create a valid png_ptr. But png_get_uint_31 will still longjmp if value is bad.
    // To prevent the fuzzer loop from dying, we should avoid the error condition.
    
    // Let's clamp the value to PNG_UINT_31_MAX to prevent the error path for fuzzing stability,
    // unless we specifically want to test the error handling (which requires a full png setup).
    // Given the goal is to test the API logic, let's ensure the input is valid for this specific call.
    // PNG_UINT_31_MAX is 2147483647 (0x7FFFFFFF).
    
    png_uint_32 safe_val = val32;
    if (safe_val > PNG_UINT_31_MAX) {
        // To avoid triggering png_error which requires a valid jmp_buf setup that we haven't done,
        // we skip the call or modify the buffer to be safe.
        // Modifying the buffer to be safe allows us to test the read logic without crashing.
        safe_val = PNG_UINT_31_MAX;
    }
    
    // Re-write the buffer with the safe value to test the read path successfully
    png_save_uint_32(buf32, safe_val);
    
    // Create a dummy png_struct to satisfy the function signature, 
    // though for the safe path it might not even be used, it's required by the API.
    png_structp png_ptr = create_dummy_png_struct();
    if (png_ptr) {
        png_get_uint_31(png_ptr, buf32);
        // Cleanup
        png_destroy_read_struct(&png_ptr, NULL, NULL);
    }

    return 0;
}