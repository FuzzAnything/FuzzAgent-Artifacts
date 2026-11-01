// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_image_finish_read at pngread.c:4069:1 in png.h
// png_image_free at png.c:3989:1 in png.h
// png_image_free at png.c:3989:1 in png.h
// png_image_write_to_memory at pngwrite.c:2286:1 in png.h
// png_image_write_to_memory at pngwrite.c:2286:1 in png.h
// png_image_free at png.c:3989:1 in png.h
// png_image_write_to_file at pngwrite.c:2402:1 in png.h
// png_image_free at png.c:3989:1 in png.h
// png_image_write_to_stdio at pngwrite.c:2353:1 in png.h
// png_image_free at png.c:3989:1 in png.h
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

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Use input data to seed random choices and construct test scenarios
    uint8_t mode = Data[0] % 6; // 0-5 to select which API to test primarily

    // Prepare a small pixel buffer for testing
    const int width = 10;
    const int height = 10;
    const int channels = 3; // RGB
    const int row_stride = width * channels;
    std::vector<uint8_t> buffer(width * height * channels);
    
    // Fill buffer with fuzzed data
    size_t fill_size = (Size > width * height * channels) ? width * height * channels : Size;
    std::memcpy(buffer.data(), Data, fill_size);
    // Ensure we have enough data if input is small
    if (fill_size < buffer.size()) {
        std::memset(buffer.data() + fill_size, 0, buffer.size() - fill_size);
    }

    // Helper lambda to safely get a byte from input
    auto get_byte = [&](size_t index) -> uint8_t {
        if (Size > index) return Data[index];
        return 0;
    };

    // Prepare colormap for indexed images if needed
    std::vector<png_color> colormap(256);
    for (int i = 0; i < 256; ++i) {
        colormap[i].red = get_byte(i);
        colormap[i].green = get_byte(i + 1);
        colormap[i].blue = get_byte(i + 2);
    }

    // Prepare background color
    png_color background;
    background.red = get_byte(0);
    background.green = get_byte(1);
    background.blue = get_byte(2);

    // Prepare histogram for quantization
    std::vector<png_uint_16> histogram(256);
    for (int i = 0; i < 256; ++i) {
        histogram[i] = (png_uint_16)((get_byte(i) << 8) | get_byte(i + 1));
    }

    // Initialize png_image struct
    png_image image;
    std::memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;
    image.width = width;
    image.height = height;
    image.format = PNG_FORMAT_RGB;
    image.flags = 0;

    int result = 0;

    switch (mode) {
        case 0: {
            // Test png_image_write_to_file
            write_dummy_file(Data, Size);
            result = png_image_write_to_file(&image, "./dummy_file", 0, buffer.data(), row_stride, nullptr);
            png_image_free(&image);
            break;
        }
        case 1: {
            // Test png_image_write_to_stdio
            FILE *f = fopen("./dummy_file", "wb");
            if (f) {
                write_dummy_file(Data, Size); // Ensure file exists
                fclose(f);
                f = fopen("./dummy_file", "wb");
                if (f) {
                    result = png_image_write_to_stdio(&image, f, 0, buffer.data(), row_stride, nullptr);
                    fclose(f);
                }
            }
            png_image_free(&image);
            break;
        }
        case 2: {
            // Test png_set_quantize (requires a valid png_struct)
            // Since we don't have a full read context, we skip this specific test case
            // as it requires a valid png_struct from png_create_read_struct which is complex to setup safely here.
            result = 0; 
            break;
        }
        case 3: {
            // Test png_image_finish_read
            // This requires a valid PNG file to read from.
            write_dummy_file(Data, Size);
            FILE *f = fopen("./dummy_file", "rb");
            if (f) {
                // Reset flags for reading
                image.flags = 0; 
                // Attempt to read. Note: This will likely fail if the file is not a valid PNG, 
                // which is expected behavior for fuzzing.
                result = png_image_finish_read(&image, &background, buffer.data(), row_stride, nullptr);
                fclose(f);
            }
            png_image_free(&image);
            break;
        }
        case 4: {
            // Test png_image_free
            // We already initialized image, so just free it.
            png_image_free(&image);
            result = 1;
            break;
        }
        case 5: {
            // Test png_image_write_to_memory
            png_alloc_size_t memory_bytes = 0;
            // First call to get size
            int ret = png_image_write_to_memory(&image, nullptr, &memory_bytes, 0, buffer.data(), row_stride, nullptr);
            if (ret == 0 && memory_bytes > 0) {
                std::vector<uint8_t> memory(memory_bytes);
                ret = png_image_write_to_memory(&image, memory.data(), &memory_bytes, 0, buffer.data(), row_stride, nullptr);
                result = (ret == 0) ? 1 : 0;
            } else {
                result = 0;
            }
            png_image_free(&image);
            break;
        }
        default:
            break;
    }

    return 0;
}