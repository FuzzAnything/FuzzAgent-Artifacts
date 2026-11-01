// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_image_write_to_stdio at pngwrite.c:2353:1 in png.h
// png_image_free at png.c:3989:1 in png.h
// png_image_write_to_memory at pngwrite.c:2286:1 in png.h
// png_image_write_to_memory at pngwrite.c:2286:1 in png.h
// png_image_free at png.c:3989:1 in png.h
// png_image_begin_read_from_file at pngread.c:1455:1 in png.h
// png_image_finish_read at pngread.c:4069:1 in png.h
// png_image_free at png.c:3989:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_set_quantize at pngrtran.c:472:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_image_write_to_file at pngwrite.c:2402:1 in png.h
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
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

static void* safe_malloc(size_t size) {
    if (size == 0 || size > 100 * 1024 * 1024) return nullptr;
    void* ptr = malloc(size);
    if (ptr) memset(ptr, 0, size);
    return ptr;
}

extern "C" int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    // Create a dummy file for file-based operations
    const char* dummy_file = "./dummy_file";
    FILE* fp = fopen(dummy_file, "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    // Allocate buffer for pixel data
    // Ensure we have enough data to construct a valid image buffer
    // We limit dimensions to prevent huge allocations
    uint32_t width = 10;
    uint32_t height = 10;
    
    if (Size > 2) {
        width = Data[0] % 100 + 1;
        height = Data[1] % 100 + 1;
    }

    // Calculate required buffer size based on format
    // We assume a worst-case 4 bytes per pixel (RGBA) for safety in allocation
    // but we will pass the actual stride to the function.
    size_t bytes_per_pixel = 4; 
    size_t total_buffer_size = width * height * bytes_per_pixel;
    
    // Cap buffer size to prevent OOM
    if (total_buffer_size > 100 * 1024 * 1024) {
        total_buffer_size = 100 * 1024 * 1024;
        // Adjust dimensions if needed to fit
        if (width > 0 && height > 0) {
            height = total_buffer_size / (width * bytes_per_pixel);
        }
    }

    std::vector<uint8_t> buffer(total_buffer_size);
    // Fill buffer with fuzz data if available, otherwise zeros
    size_t fill_size = (Size < total_buffer_size) ? Size : total_buffer_size;
    if (fill_size > 0) {
        memcpy(buffer.data(), Data, fill_size);
    }

    // Test png_image_write_to_file
    png_image image_write_file;
    memset(&image_write_file, 0, sizeof(image_write_file));
    image_write_file.version = PNG_IMAGE_VERSION;
    image_write_file.width = width;
    image_write_file.height = height;
    // Format: 1=RGBA, 2=RGB, 3=Gray+Alpha, 4=Gray, 5=Palette
    image_write_file.format = (Data[2] % 5) + 1; 
    image_write_file.flags = 0; 
    
    // Calculate stride correctly for the chosen format
    // For simplicity in fuzzing, we assume 4 bytes per pixel stride (RGBA)
    // The library handles conversion if the format differs, but we must ensure buffer is large enough
    int stride_file = (int)(width * 4);
    
    // Ensure stride doesn't overflow
    if (stride_file < 0) stride_file = 0;

    png_image_write_to_file(&image_write_file, dummy_file, 0, buffer.data(), stride_file, nullptr);
    png_image_free(&image_write_file);

    // Test png_image_write_to_stdio
    FILE* file_stream = fopen(dummy_file, "wb");
    if (file_stream) {
        png_image image_stdio;
        memset(&image_stdio, 0, sizeof(image_stdio));
        image_stdio.version = PNG_IMAGE_VERSION;
        image_stdio.width = width;
        image_stdio.height = height;
        image_stdio.format = (Data[2] % 5) + 1;
        image_stdio.flags = 0; 
        
        int stride_stdio = (int)(width * 4);
        if (stride_stdio < 0) stride_stdio = 0;
        
        png_image_write_to_stdio(&image_stdio, file_stream, 0, buffer.data(), stride_stdio, nullptr);
        png_image_free(&image_stdio);
        fclose(file_stream);
    }

    // Test png_image_write_to_memory
    png_image image_memory;
    memset(&image_memory, 0, sizeof(image_memory));
    image_memory.version = PNG_IMAGE_VERSION;
    image_memory.width = width;
    image_memory.height = height;
    image_memory.format = (Data[2] % 5) + 1;
    image_memory.flags = 0;

    int stride_memory = (int)(width * 4);
    if (stride_memory < 0) stride_memory = 0;

    png_alloc_size_t memory_bytes = 0;
    
    // First call to get size
    png_image_write_to_memory(&image_memory, nullptr, &memory_bytes, 0, buffer.data(), stride_memory, nullptr);
    
    if (memory_bytes > 0 && memory_bytes < 100 * 1024 * 1024) {
        void* memory = safe_malloc(memory_bytes);
        if (memory) {
            png_image_write_to_memory(&image_memory, memory, &memory_bytes, 0, buffer.data(), stride_memory, nullptr);
            free(memory);
        }
    }
    png_image_free(&image_memory);

    // Test png_image_finish_read
    // Note: This requires a valid PNG file. Our dummy file is just raw fuzz data.
    // We attempt to call it, expecting it to fail gracefully or handle errors.
    png_image image_read;
    memset(&image_read, 0, sizeof(image_read));
    image_read.version = PNG_IMAGE_VERSION;
    image_read.width = width;
    image_read.height = height;
    image_read.format = (Data[2] % 5) + 1;
    image_read.flags = 0; 
    
    FILE* read_fp = fopen(dummy_file, "rb");
    if (read_fp) {
        if (png_image_begin_read_from_file(&image_read, dummy_file)) {
            // If begin_read succeeds, we need a buffer large enough for the image
            // We reuse our buffer size calculation
            size_t row_bytes = width * 4;
            size_t total_read_bytes = row_bytes * height;
            
            if (total_read_bytes > 0 && total_read_bytes < 100 * 1024 * 1024) {
                void* read_buffer = safe_malloc(total_read_bytes);
                if (read_buffer) {
                    png_const_colorp background = nullptr;
                    void* colormap = nullptr;
                    png_int_32 row_stride = (png_int_32)row_bytes;
                    if (row_stride < 0) row_stride = 0;
                    
                    png_image_finish_read(&image_read, background, read_buffer, row_stride, colormap);
                    free(read_buffer);
                }
            }
        }
        png_image_free(&image_read);
        fclose(read_fp);
    }

    // Test png_set_quantize (requires a valid png_struct)
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (png_ptr) {
        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (info_ptr) {
            png_color palette[256];
            for (int i = 0; i < 256; ++i) {
                palette[i].red = i;
                palette[i].green = i;
                palette[i].blue = i;
            }
            png_uint_16 histogram[256];
            for (int i = 0; i < 256; ++i) histogram[i] = i;
            
            int num_palette = 256;
            int maximum_colors = 128;
            int full_quantize = (Size > 5) ? (Data[4] % 2) : 0;
            
            png_set_quantize(png_ptr, palette, num_palette, maximum_colors, histogram, full_quantize);
            
            png_destroy_info_struct(png_ptr, &info_ptr);
        }
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
    }

    return 0;
}