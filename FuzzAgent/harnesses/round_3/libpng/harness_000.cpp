#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <png.h>
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_000 {

// Custom read function structure to work with libpng's C API
struct png_read_data {
    FuzzedDataProvider* fdp;
    const uint8_t* data;
    size_t size;
    size_t offset;
};

static void png_read_callback(png_structp png_ptr, png_bytep outBytes, png_size_t byteCountToRead) {
    png_read_data* read_data = (png_read_data*)png_get_io_ptr(png_ptr);
    
    if (read_data->offset + byteCountToRead > read_data->size) {
        png_error(png_ptr, "Insufficient data");
    }
    
    memcpy(outBytes, read_data->data + read_data->offset, byteCountToRead);
    read_data->offset += byteCountToRead;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 8) {
        return 0;  // PNG signature is 8 bytes
    }

    FuzzedDataProvider fdp(data, size);

    // Create PNG read structure
    png_struct *png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                  nullptr, nullptr, nullptr);
    if (png_ptr == nullptr) {
        return 0;
    }

    // Create PNG info structure
    png_info *info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == nullptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    // Set up custom read data structure
    png_read_data read_data;
    read_data.data = data;
    read_data.size = size;
    read_data.offset = 0;
    
    // Set up the read function
    png_set_read_fn(png_ptr, &read_data, png_read_callback);

    // Set the number of bytes already read (0 since we start from beginning)
    png_set_sig_bytes(png_ptr, 0);

    // Read PNG info
    png_read_info(png_ptr, info_ptr);

    // Get image info before transforms
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);

    // Consume fuzzer data to decide which transforms to apply
    int transforms = fdp.ConsumeIntegral<int>();

    // Apply transforms and read the full image
    png_read_png(png_ptr, info_ptr, transforms, nullptr);

    // Get the row pointers that were allocated by png_read_png
    png_bytep* row_pointers = png_get_rows(png_ptr, info_ptr);
    
    if (row_pointers != nullptr) {
        // Access some pixel data to ensure it's valid
        for (png_uint_32 y = 0; y < height && y < 10; y++) {
            if (row_pointers[y] != nullptr) {
                // Just access first few bytes of each row
                volatile png_byte dummy = row_pointers[y][0];
                (void)dummy;  // Avoid unused variable warning
            }
        }
    }

    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);

    return 0;
}

} // namespace harness_000
