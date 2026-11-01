// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_read_frame_head at pngread.c:195:1 in png.h
// png_get_io_state at pngget.c:1334:1 in png.h
// png_get_io_chunk_type at pngget.c:1340:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_write_chunk_start at pngwutil.c:132:1 in png.h
// png_write_chunk_data at pngwutil.c:144:1 in png.h
// png_write_chunk_end at pngwutil.c:163:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_read_user_chunk_fn at pngset.c:1830:1 in png.h
// png_sig_cmp at png.c:80:1 in png.h
// png_get_io_state at pngget.c:1334:1 in png.h
// png_get_io_chunk_type at pngget.c:1340:1 in png.h
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

// Static helper to write fuzz data to a dummy file
static void write_fuzz_data_to_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

// Custom error handler to prevent termination during fuzzing
static void custom_error_handler(png_structp png_ptr, const char *error_message) {
    (void)png_ptr;
    (void)error_message;
}

// Custom warning handler
static void custom_warning_handler(png_structp png_ptr, const char *warning_message) {
    (void)png_ptr;
    (void)warning_message;
}

// User chunk callback function
// Note: In newer libpng versions (1.6+), the struct member is 'size', not 'length'.
static int user_chunk_callback(png_structp png_ptr, png_unknown_chunkp chunk) {
    (void)png_ptr;
    // Access chunk data safely. 
    // The member name depends on the libpng version. 
    // In 1.6+, it is 'size'. In older versions, it might be 'length'.
    // We use 'size' as it is the standard for modern libpng.
    if (chunk->size > 0 && chunk->data != NULL) {
        // Check first byte to decide action
        if (chunk->data[0] & 0x01) {
            return 1; // Discard
        }
    }
    return 0; // Save
}

extern "C" int LLVMFuzzerTestOneInput_90(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Write input data to a dummy file for reading
    write_fuzz_data_to_file(Data, Size);

    // Initialize png_struct and png_info
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Set custom error handlers to avoid longjmp/exit
    png_set_error_fn(png_ptr, NULL, custom_error_handler, custom_warning_handler);

    // Open the dummy file
    FILE *fp = fopen("./dummy_file", "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Set I/O function
    png_init_io(png_ptr, fp);

    // Fuzzing: Register user chunk callback (Target API 1)
    png_set_read_user_chunk_fn(png_ptr, NULL, user_chunk_callback);

    // Attempt to read the signature
    png_byte sig[8];
    size_t read_bytes = fread(sig, 1, 8, fp);
    if (read_bytes == 8) {
        if (png_sig_cmp(sig, 0, 8) != 0) {
            // Not a valid PNG signature
            fclose(fp);
            
            // Test png_get_io_state (Target API 5)
            png_uint_32 io_state = png_get_io_state(png_ptr);
            (void)io_state;

            // Test png_get_io_chunk_type (Target API 6)
            png_uint_32 chunk_type = png_get_io_chunk_type(png_ptr);
            (void)chunk_type;

            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            return 0;
        }
    } else {
        // Not enough data
        fclose(fp);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Read the header
    png_read_info(png_ptr, info_ptr);

    // Fuzzing: Try to read frame head (Target API 4)
    // This may fail if the image is not APNG, but it's safe to call.
    png_read_frame_head(png_ptr, info_ptr);

    // Fuzzing: Get IO state (Target API 5)
    png_uint_32 io_state = png_get_io_state(png_ptr);
    (void)io_state;

    // Fuzzing: Get IO chunk type (Target API 6)
    png_uint_32 chunk_type = png_get_io_chunk_type(png_ptr);
    (void)chunk_type;

    // Cleanup reading
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);

    // --- Write Mode Tests ---

    // Re-initialize for write tests
    png_structp png_ptr_write = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr_write) {
        return 0;
    }
    
    png_infop info_ptr_write = png_create_info_struct(png_ptr_write);
    if (!info_ptr_write) {
        png_destroy_write_struct(&png_ptr_write, NULL);
        return 0;
    }

    png_set_error_fn(png_ptr_write, NULL, custom_error_handler, custom_warning_handler);

    // Open dummy file for writing
    fp = fopen("./dummy_file", "wb");
    if (!fp) {
        png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
        return 0;
    }

    png_init_io(png_ptr_write, fp);

    // Set minimal image info
    png_set_IHDR(png_ptr_write, info_ptr_write, 1, 1, 8, PNG_COLOR_TYPE_RGB,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    png_write_info(png_ptr_write, info_ptr_write);

    // Fuzzing: Manually write a chunk to test png_write_chunk_end (Target API 3)
    // We start a chunk, write some data, then end it.
    png_byte chunk_name[4] = {'I', 'A', 'C', 'T'}; // Arbitrary chunk type
    png_uint_32 chunk_length = 4;
    png_byte *chunk_data = (png_byte*)malloc(chunk_length);
    if (chunk_data) {
        memset(chunk_data, 0x42, chunk_length);
        
        // Start chunk
        png_write_chunk_start(png_ptr_write, chunk_name, chunk_length);
        
        // Write data using png_write_chunk_data
        png_write_chunk_data(png_ptr_write, chunk_data, chunk_length);
        
        // Target API 3: Finalize the chunk
        png_write_chunk_end(png_ptr_write);
        
        free(chunk_data);
    }

    // Cleanup write
    png_write_end(png_ptr_write, info_ptr_write);
    png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
    fclose(fp);

    return 0;
}