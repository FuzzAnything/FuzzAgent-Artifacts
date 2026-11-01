// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_set_progressive_read_fn at pngpread.c:1109:1 in png.h
// png_set_read_user_transform_fn at pngrtran.c:1099:1 in png.h
// png_set_user_transform_info at pngtrans.c:805:1 in png.h
// png_get_user_transform_ptr at pngtrans.c:836:1 in png.h
// png_get_progressive_ptr at pngpread.c:1136:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_start_read_image at pngread.c:291:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_read_row at pngread.c:376:1 in png.h
// png_progressive_combine_row at pngpread.c:1093:1 in png.h
// png_progressive_combine_row at pngpread.c:1093:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_error at pngerror.c:50:1 in png.h
// png_get_progressive_ptr at pngpread.c:1136:1 in png.h
// png_error at pngerror.c:50:1 in png.h
// png_error at pngerror.c:50:1 in png.h
// png_get_user_transform_ptr at pngtrans.c:836:1 in png.h
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
#include <stdint.h>
#include <stddef.h>
#include <cstring>
#include <vector>
#include <cstdio>
#include <setjmp.h>

// Custom read callback for png_set_read_fn
static void
custom_read_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    void *io_ptr = png_get_io_ptr(png_ptr);
    if (io_ptr == nullptr) {
        png_error(png_ptr, "No IO pointer");
        return;
    }

    std::vector<uint8_t>* data_buffer = static_cast<std::vector<uint8_t>*>(io_ptr);
    size_t* read_pos = static_cast<size_t*>(png_get_progressive_ptr(png_ptr));

    if (read_pos == nullptr) {
        png_error(png_ptr, "No read position pointer");
        return;
    }

    if (*read_pos + length > data_buffer->size()) {
        png_error(png_ptr, "Read past end of buffer");
        return;
    }

    std::memcpy(data, data_buffer->data() + *read_pos, length);
    *read_pos += length;
}

// Custom user transform callback for png_set_read_user_transform_fn
static void
custom_user_transform(png_structp png_ptr, png_row_infop row_info, png_bytep row) {
    // Simple transform: do nothing or could apply custom logic
    // Access user transform pointer if needed
    void* user_ptr = png_get_user_transform_ptr(png_ptr);
    (void)user_ptr; // Suppress unused warning
    (void)row_info;
    (void)row;
}

// Helper to create a minimal valid PNG header (8 bytes)
static void
write_minimal_png_header(std::vector<uint8_t>& buffer) {
    // PNG signature
    const unsigned char png_sig[8] = {137, 80, 78, 71, 13, 10, 26, 10};
    buffer.insert(buffer.end(), png_sig, png_sig + 8);

    // IHDR chunk (minimal)
    // Length: 13 bytes
    unsigned char ihdr[13] = {
        8, 0, 0, 0, // Width: 8
        8, 0, 0, 0, // Height: 8
        8,          // Bit depth
        0,          // Color type (grayscale)
        0,          // Compression
        0,          // Filter
        0           // Interlace
    };
    buffer.insert(buffer.end(), ihdr, ihdr + 13);

    // CRC for IHDR (dummy, not calculated)
    unsigned char crc[4] = {0, 0, 0, 0};
    buffer.insert(buffer.end(), crc, crc + 4);

    // IDAT chunk (minimal, empty data)
    unsigned char idat_len[4] = {0, 0, 0, 0};
    buffer.insert(buffer.end(), idat_len, idat_len + 4);
    // No data for IDAT
    // CRC for IDAT (dummy)
    buffer.insert(buffer.end(), crc, crc + 4);

    // IEND chunk
    unsigned char iend_len[4] = {0, 0, 0, 0};
    buffer.insert(buffer.end(), iend_len, iend_len + 4);
    // CRC for IEND (dummy)
    buffer.insert(buffer.end(), crc, crc + 4);
}

extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Create a buffer with minimal PNG header + fuzzed data
    std::vector<uint8_t> buffer;
    write_minimal_png_header(buffer);
    buffer.insert(buffer.end(), Data, Data + Size);

    // Create read structure
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (png_ptr == nullptr) {
        return 0;
    }

    // Create info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == nullptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    // Set read function and IO pointer
    size_t read_pos = 0;
    png_set_read_fn(png_ptr, &buffer, custom_read_data);
    png_set_progressive_read_fn(png_ptr, &read_pos, nullptr, nullptr, nullptr);

    // Set user transform function
    png_set_read_user_transform_fn(png_ptr, custom_user_transform);
    png_set_user_transform_info(png_ptr, nullptr, 0, 0);

    // Test png_get_user_transform_ptr
    void* user_ptr = png_get_user_transform_ptr(png_ptr);
    (void)user_ptr;

    // Test png_get_progressive_ptr
    void* prog_ptr = png_get_progressive_ptr(png_ptr);
    (void)prog_ptr;

    // Try to read info
    png_read_info(png_ptr, info_ptr);

    // Start reading image
    png_start_read_image(png_ptr);

    // Prepare row buffer
    png_bytep row = (png_bytep)png_malloc(png_ptr, png_get_rowbytes(png_ptr, info_ptr));
    if (row == nullptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    // Try to read a row (may fail if data is invalid)
    png_read_row(png_ptr, row, nullptr);

    // Test png_progressive_combine_row
    png_byte old_row[1024];
    std::memset(old_row, 0, sizeof(old_row));
    png_progressive_combine_row(png_ptr, old_row, row);
    png_progressive_combine_row(png_ptr, old_row, nullptr);

    // Cleanup
    png_free(png_ptr, row);
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);

    return 0;
}