// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_IHDR at pngset.c:417:1 in png.h
// png_set_rows at pngset.c:1845:1 in png.h
// png_write_flush at pngwrite.c:978:1 in png.h
// png_write_chunk_end at pngwutil.c:163:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_write_sig at pngwutil.c:68:1 in png.h
// png_write_info_before_PLTE at pngwrite.c:83:1 in png.h
#include <png.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstddef>
#include <vector>
#include <cstdint>

static const char* DUMMY_FILE_PATH = "./dummy_file";

static FILE* open_file_for_write() {
    FILE* f = fopen(DUMMY_FILE_PATH, "wb");
    if (!f) {
        return nullptr;
    }
    return f;
}

static png_structp create_png_struct(FILE* fp, png_infopp info_ptr_out) {
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        return nullptr;
    }
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        return nullptr;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return nullptr;
    }

    png_init_io(png_ptr, fp);
    
    if (info_ptr_out) {
        *info_ptr_out = info_ptr;
    }
    
    return png_ptr;
}

static void destroy_png_struct(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
    }
}

extern "C" int LLVMFuzzerTestOneInput_74(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    FILE* fp = open_file_for_write();
    if (!fp) {
        return 0;
    }

    png_infop info_ptr = nullptr;
    png_structp png_ptr = create_png_struct(fp, &info_ptr);
    
    if (!png_ptr || !info_ptr) {
        fclose(fp);
        return 0;
    }

    // Ensure we have a safe context for error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        destroy_png_struct(png_ptr, info_ptr);
        fclose(fp);
        return 0;
    }

    // 1. png_write_sig
    png_write_sig(png_ptr);

    // 2. png_write_info_before_PLTE
    png_write_info_before_PLTE(png_ptr, info_ptr);

    // 3. png_set_rows
    // Create some dummy row pointers based on input size
    int width = (Size > 100) ? 100 : Size;
    int height = (Size > 100) ? 100 : Size;
    
    // Ensure dimensions are reasonable to avoid memory exhaustion
    if (width > 1000) width = 1000;
    if (height > 1000) height = 1000;

    std::vector<png_byte*> row_pointers(height);
    for (int i = 0; i < height; ++i) {
        row_pointers[i] = new png_byte[width];
        // Fill with some data from the fuzz input if available
        size_t bytes_to_copy = (i * width < Size) ? width : (Size - (i * width));
        if (bytes_to_copy > 0) {
            memcpy(row_pointers[i], Data + (i * width), bytes_to_copy);
            if (bytes_to_copy < width) {
                memset(row_pointers[i] + bytes_to_copy, 0, width - bytes_to_copy);
            }
        } else {
            memset(row_pointers[i], 0, width);
        }
    }
    
    // Set image parameters required before writing rows
    png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_GRAY, 
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    
    png_set_rows(png_ptr, info_ptr, row_pointers.data());

    // 4. png_write_flush
    png_write_flush(png_ptr);

    // 5. png_write_chunk_end
    // Note: This function expects a chunk to be started via png_write_chunk_start.
    // Calling it without starting a chunk may trigger an error or do nothing depending on state.
    // To safely explore this state, we attempt it. If it errors, the setjmp above handles it.
    // However, without a valid chunk start, this is likely to fail. 
    // For fuzzing purposes, we will call it to see if it handles the "no active chunk" state gracefully.
    // If the library is strict, this will jump to the error handler.
    png_write_chunk_end(png_ptr);

    // 6. png_write_end
    png_write_end(png_ptr, info_ptr);

    // Cleanup row pointers
    for (int i = 0; i < height; ++i) {
        delete[] row_pointers[i];
    }

    destroy_png_struct(png_ptr, info_ptr);
    fclose(fp);

    return 0;
}