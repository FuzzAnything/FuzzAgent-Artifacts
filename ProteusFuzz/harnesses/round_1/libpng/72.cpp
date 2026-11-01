// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_crc_action at pngrtran.c:24:1 in png.h
// png_set_benign_errors at pngset.c:1994:1 in png.h
// png_set_chunk_cache_max at pngset.c:1955:1 in png.h
// png_set_check_for_invalid_index at pngset.c:2024:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_set_rgb_to_gray_fixed at pngrtran.c:1013:1 in png.h
// png_set_interlace_handling at pngtrans.c:98:1 in png.h
// png_read_update_info at pngread.c:260:1 in png.h
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
#include <setjmp.h>

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_message) {
    (void)png_ptr;
    (void)error_message;
    // Do not abort, just return. The fuzzer handles the state.
    // We rely on setjmp/longjmp in the main function to catch libpng errors if they trigger longjmp.
    // However, if libpng calls PNG_ABORT() directly (which happens if error_fn is not set correctly or if it's a critical internal error),
    // we can't catch it. But setting the error function prevents the default abort.
    // The default error function calls longjmp(png_jmpbuf(png_ptr), 1).
    // By providing this function, we override the default. We must ensure we don't call longjmp here
    // if we want to continue, but libpng expects the error handler to not return.
    // To safely fuzz, we will let libpng's internal longjmp mechanism work by NOT overriding the error function
    // or by calling longjmp ourselves. 
    // The crash log shows png_default_error calling png_abort. This happens if the error function is not set or if it's set to something that doesn't handle it.
    // Actually, the stack trace shows png_default_error -> png_abort. This implies the error handler called was png_default_error.
    // This means our dummy_error_fn was NOT called, or libpng called png_error directly which calls the error function.
    // Wait, png_error calls the error function. If we set it, it should call ours.
    // The issue might be that we are calling png_set_rgb_to_gray_fixed BEFORE reading the PNG header.
    // The error message in the log says: "invalid before the PNG header has been read".
    // This happens inside png_set_rgb_to_gray_fixed if the PNG header hasn't been read yet?
    // Let's check the libpng source logic.
    // png_set_rgb_to_gray_fixed checks if the image is already read or if it's valid.
    // Actually, the error "invalid before the PNG header has been read" usually comes from trying to read info or do transformations
    // before the IHDR is read.
    // But png_set_rgb_to_gray_fixed is supposed to be called before png_read_start.
    // However, if the file is not a valid PNG (no IHDR), libpng might complain when we try to set transformations that depend on image data?
    // No, png_set_rgb_to_gray_fixed sets flags. It shouldn't read the file.
    // The stack trace shows: png_set_rgb_to_gray_fixed -> png_rtran_ok -> png_app_error.
    // png_rtran_ok checks if the read state is valid.
    // It seems png_set_rgb_to_gray_fixed calls png_rtran_ok which checks if the PNG header has been read.
    // This is unexpected for a "set" function, but if the library implementation requires the IHDR to be read to validate the transformation,
    // then we must read the IHDR first.
    // However, the documentation says "Call after png_create_read_struct but before png_read_start".
    // png_read_start is called after png_read_info.
    // So we must call png_read_info first to read the IHDR.
    // But png_read_info might fail if the file is garbage.
    // We need to wrap the calls in setjmp/longjmp to handle the "invalid before PNG header" error gracefully.
    // The error is triggered because we are calling png_set_rgb_to_gray_fixed on a file that hasn't been parsed (no IHDR read yet).
    // The fix is to call png_read_info first, or catch the error.
    // Since we want to fuzz the API calls, we should try to call them in a valid sequence.
    // Sequence: create -> set_crc -> set_benign -> set_rgb... -> read_info -> set_interlace -> read_update.
    // But if read_info fails (due to bad data), we can't proceed.
    // We will wrap the entire read process in setjmp.
    // If png_read_info fails, we catch it and return.
    // But we want to test the APIs even if the file is bad?
    // The error "invalid before the PNG header has been read" suggests that png_set_rgb_to_gray_fixed checks the state.
    // If the state is not "header read", it errors.
    // So we MUST call png_read_info before png_set_rgb_to_gray_fixed if the file is not valid?
    // No, the documentation says before png_read_start.
    // Let's re-read the error: "invalid before the PNG header has been read".
    // This error is triggered by png_rtran_ok.
    // It seems libpng requires the IHDR to be read before setting some transformations.
    // So the correct order for fuzzing valid sequences is:
    // 1. create
    // 2. set_crc_action
    // 3. set_benign_errors
    // 4. png_read_info (This reads IHDR)
    // 5. set_rgb_to_gray_fixed (Now IHDR is read)
    // 6. set_chunk_cache_max
    // 7. set_check_for_invalid_index
    // 8. set_interlace_handling
    // 9. read_update_info
    // However, if the file is garbage, png_read_info will fail.
    // We will wrap png_read_info in setjmp. If it fails, we skip the rest.
    // But we still want to test the APIs that don't require IHDR?
    // The error log specifically points to png_set_rgb_to_gray_fixed failing because IHDR wasn't read.
    // So we must ensure IHDR is read before calling it.
    // We will move png_read_info before the transformation setters.
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_message) {
    (void)png_ptr;
    (void)warning_message;
}

extern "C" int LLVMFuzzerTestOneInput_72(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    std::vector<uint8_t> buffer(Size);
    if (Size < 8) {
        buffer.assign(Size, 0);
    } else {
        buffer.assign(Data, Data + Size);
    }

    const char* filename = "./dummy_file";
    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(buffer.data(), 1, buffer.size(), fp);
    fclose(fp);

    fp = fopen(filename, "rb");
    if (!fp) {
        return 0;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        fclose(fp);
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        fclose(fp);
        return 0;
    }

    // Set error handlers
    png_set_error_fn(png_ptr, nullptr, dummy_error_fn, dummy_warning_fn);
    png_init_io(png_ptr, fp);

    // We need to handle libpng errors via setjmp/longjmp.
    // The error "invalid before the PNG header has been read" causes a longjmp.
    // We set up the jump buffer.
    if (setjmp(png_jmpbuf(png_ptr)) != 0) {
        // An error occurred (e.g., invalid PNG, or API called too early)
        // Clean up and return
        fclose(fp);
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    // 1. png_set_crc_action (Safe to call early)
    int crit_action = (Size > 0) ? Data[0] % 4 : 0;
    int ancil_action = (Size > 1) ? Data[1] % 4 : 0;
    png_set_crc_action(png_ptr, crit_action, ancil_action);

    // 2. png_set_benign_errors (Safe to call early)
    int allowed = (Size > 2) ? Data[2] % 2 : 0;
    png_set_benign_errors(png_ptr, allowed);

    // 3. png_set_chunk_cache_max (Safe to call early)
    png_uint_32 cache_max = (Size > 3) ? (png_uint_32)Data[3] * 100 : 0;
    png_set_chunk_cache_max(png_ptr, cache_max);

    // 4. png_set_check_for_invalid_index (Safe to call early)
    int check_allowed = (Size > 4) ? Data[4] % 2 : 0;
    png_set_check_for_invalid_index(png_ptr, check_allowed);

    // 5. Try to read info (Reads IHDR). This might fail if file is garbage.
    // If it fails, we jump to the setjmp block and clean up.
    // But we want to test png_set_rgb_to_gray_fixed which requires IHDR.
    // So we must read info first.
    png_read_info(png_ptr, info_ptr);

    // 6. png_set_rgb_to_gray_fixed (Now IHDR is read, so this should be safe)
    int error_action = (Size > 5) ? Data[5] % 3 : 0;
    png_fixed_point red = (Size > 6) ? Data[6] * 100000 : 0;
    png_fixed_point green = (Size > 7) ? Data[7] * 100000 : 0;
    png_set_rgb_to_gray_fixed(png_ptr, error_action, red, green);

    // 7. png_set_interlace_handling (Must be before png_read_update_info)
    int passes = png_set_interlace_handling(png_ptr);
    (void)passes; // Suppress unused warning

    // 8. png_read_update_info
    png_read_update_info(png_ptr, info_ptr);

    // Cleanup
    fclose(fp);
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);

    return 0;
}