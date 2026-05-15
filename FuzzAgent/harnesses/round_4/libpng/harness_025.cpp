#include <fuzzer/FuzzedDataProvider.h>
#include <png.h>
#include <zlib.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

namespace harness_025 {

// Error handling callbacks for libpng
static void pngtest_error(png_structp png_ptr, png_const_charp msg) {
    (void)png_ptr;
    (void)msg;
    // Do nothing - just return
}

static void pngtest_warning(png_structp png_ptr, png_const_charp msg) {
    (void)png_ptr;
    (void)msg;
    // Do nothing - just return
}

// Memory buffer structure for custom I/O
struct png_memory_buffer {
    const uint8_t* data;
    size_t size;
    size_t offset;
};

static void pngtest_read_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    png_memory_buffer* buffer = (png_memory_buffer*)png_get_io_ptr(png_ptr);
    
    if (buffer->offset + length > buffer->size) {
        png_error(png_ptr, "Read beyond buffer");
        return;
    }
    
    memcpy(data, buffer->data + buffer->offset, length);
    buffer->offset += length;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum size for PNG operations
    if (size < 100) {
        return 0;  // Insufficient input for meaningful testing
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Create memory buffer for PNG operations
    png_memory_buffer buffer;
    buffer.data = data;
    buffer.size = size;
    buffer.offset = 0;
    
    // 1. Initialize PNG reader structure
    png_structp png_ptr = png_create_read_struct(
        PNG_LIBPNG_VER_STRING,
        nullptr,
        pngtest_error,
        pngtest_warning);
    
    if (!png_ptr) {
        return 0;
    }
    
    // 2. Create info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }
    
    // Set up error handling using setjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }
    
    // 3. Set up custom I/O functions
    png_set_read_fn(png_ptr, &buffer, pngtest_read_data);
    
    // 4. Read PNG signature and info
    // First, check if the input looks like a PNG
    png_byte png_sig[8];
    if (buffer.size >= 8) {
        memcpy(png_sig, buffer.data, 8);
        int is_png = png_sig_cmp(png_sig, 0, 8);
        if (is_png != 0) {
            // Not a PNG signature, but we'll continue to test APIs anyway
            // Reset buffer offset for our custom read function
            buffer.offset = 0;
        }
    }
    
    // Try to read PNG info - this may fail if input isn't valid PNG
    // but we still want to test the getter APIs
    png_read_info(png_ptr, info_ptr);
    
    // 5. Retrieve metadata using target get functions
    
    // png_get_eXIf_1 (8 undiscovered branches)
    #ifdef PNG_eXIf_SUPPORTED
    if (fdp.ConsumeBool() && fdp.remaining_bytes() >= 8) {
        png_uint_32 num_exif = 0;
        png_byte* exif_data = nullptr;
        png_get_eXIf_1(png_ptr, info_ptr, &num_exif, &exif_data);
        // Note: exif_data is owned by libpng, don't free it
    }
    #endif
    
    // png_get_filter_type (4 undiscovered branches)
    png_byte filter_type = png_get_filter_type(png_ptr, info_ptr);
    (void)filter_type;
    
    // png_get_channels (4 undiscovered branches)
    png_byte channels = png_get_channels(png_ptr, info_ptr);
    (void)channels;
    
    // png_get_signature (4 undiscovered branches)
    #ifdef PNG_READ_SUPPORTED
    const png_byte* signature = png_get_signature(png_ptr, info_ptr);
    (void)signature;
    #endif
    
    // png_get_palette_max (4 undiscovered branches)
    int palette_max = png_get_palette_max(png_ptr, info_ptr);
    (void)palette_max;
    
    // png_get_interlace_type (4 undiscovered branches)
    png_byte interlace_type = png_get_interlace_type(png_ptr, info_ptr);
    (void)interlace_type;
    
    // png_get_compression_type (4 undiscovered branches)
    png_byte compression_type = png_get_compression_type(png_ptr, info_ptr);
    (void)compression_type;
    
    // png_get_compression_buffer_size (4 undiscovered branches)
    size_t compression_buffer_size = png_get_compression_buffer_size(png_ptr);
    (void)compression_buffer_size;
    
    // 6. Test state manipulation with png_set_invalid
    // Choose random PNG_INFO flags to invalidate
    if (fdp.ConsumeBool() && fdp.remaining_bytes() >= 1) {
        png_uint_32 info_flags = fdp.ConsumeIntegral<png_uint_32>();
        png_set_invalid(png_ptr, info_ptr, info_flags);
    }
    
    // 7. Test memory allocation by ensuring default memory functions are used
    // We don't call png_set_mem_fn, which means png_malloc_default will be used
    // when memory allocation is needed internally
    if (fdp.ConsumeBool() && fdp.remaining_bytes() >= 4) {
        size_t alloc_size = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
        // Trigger memory allocation through png_malloc_default
        // by calling an API that allocates memory internally
        png_voidp mem_ptr = png_malloc(png_ptr, alloc_size);
        if (mem_ptr) {
            png_free(png_ptr, mem_ptr);
        }
    }
    
    // 8. Test png_malloc_default directly (9 undiscovered branches)
    if (fdp.ConsumeBool() && fdp.remaining_bytes() >= 4) {
        size_t default_alloc_size = fdp.ConsumeIntegralInRange<size_t>(1, 512);
        // png_malloc_default is typically called internally, but we can
        // try to trigger it by setting memory functions to NULL first
        png_set_mem_fn(png_ptr, nullptr, nullptr, nullptr);
        png_voidp default_mem = png_malloc(png_ptr, default_alloc_size);
        if (default_mem) {
            png_free(png_ptr, default_mem);
        }
        // Restore default memory functions
        png_set_mem_fn(png_ptr, nullptr, nullptr, nullptr);
    }
    
    // 9. Test additional metadata getters for more coverage
    if (fdp.ConsumeBool()) {
        png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
        png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
        png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
        png_byte color_type = png_get_color_type(png_ptr, info_ptr);
        (void)width; (void)height; (void)bit_depth; (void)color_type;
    }
    
    // 10. Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    return 0;
}

} // namespace harness_025
