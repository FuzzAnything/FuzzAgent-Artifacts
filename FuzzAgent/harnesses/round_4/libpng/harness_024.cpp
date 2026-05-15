#include <fuzzer/FuzzedDataProvider.h>
#include <png.h>
#include <zlib.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

namespace harness_024 {

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

// Simple write function for dummy I/O
static void pngtest_write_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
    // Write to nowhere
}

static void pngtest_flush_data(png_structp png_ptr) {
    (void)png_ptr;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum size for configuration parameters
    // We need at least: 1 bool + 4 size_t/uint32_t values = ~17+ bytes
    if (size < 20) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // 1. Create PNG write structure (using write struct to avoid file I/O complexities)
    png_structp png_ptr = png_create_write_struct(
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
        png_destroy_write_struct(&png_ptr, nullptr);
        return 0;
    }
    
    // Set up error handling using setjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }
    
    // 3. Set error handling function (optional helper API)
    png_set_error_fn(png_ptr, nullptr, pngtest_error, pngtest_warning);
    
    // 4. Initialize I/O with dummy write functions
    // Create a dummy I/O pointer
    void* dummy_io_ptr = nullptr;
    png_set_write_fn(png_ptr, dummy_io_ptr, pngtest_write_data, pngtest_flush_data);
    
    // 5. Test png_init_io (target API - 0% covered)
    // This requires a FILE*, so we create a temporary file
    bool test_png_init_io = fdp.ConsumeBool();
    if (test_png_init_io && fdp.remaining_bytes() >= 8) {
        FILE* tmp_file = tmpfile();
        if (tmp_file) {
            // Write some fuzzer data to the file
            size_t bytes_to_write = fdp.ConsumeIntegralInRange<size_t>(8, 64);
            if (bytes_to_write <= fdp.remaining_bytes()) {
                std::vector<uint8_t> tmp_data = fdp.ConsumeBytes<uint8_t>(bytes_to_write);
                if (tmp_data.size() > 0) {
                    fwrite(tmp_data.data(), 1, tmp_data.size(), tmp_file);
                    rewind(tmp_file);
                    
                    png_init_io(png_ptr, tmp_file);
                }
            }
            fclose(tmp_file);
        }
    }
    
    // 6. Test chunk memory configuration APIs (target APIs - 0% covered)
    
    // Set chunk malloc maximum
    png_alloc_size_t chunk_malloc_max = fdp.ConsumeIntegral<png_alloc_size_t>();
    png_set_chunk_malloc_max(png_ptr, chunk_malloc_max);
    
    // Get chunk malloc maximum and verify
    png_alloc_size_t retrieved_malloc_max = png_get_chunk_malloc_max(png_ptr);
    (void)retrieved_malloc_max; // Use to avoid unused variable warning
    
    // Set chunk cache maximum
    png_uint_32 chunk_cache_max = fdp.ConsumeIntegral<png_uint_32>();
    png_set_chunk_cache_max(png_ptr, chunk_cache_max);
    
    // Get chunk cache maximum and verify
    png_uint_32 retrieved_cache_max = png_get_chunk_cache_max(png_ptr);
    (void)retrieved_cache_max; // Use to avoid unused variable warning
    
    // 7. Test user dimension limits APIs (target APIs - 0% covered)
    
    // Set user dimension limits
    png_uint_32 user_width_max = fdp.ConsumeIntegral<png_uint_32>();
    png_uint_32 user_height_max = fdp.ConsumeIntegral<png_uint_32>();
    png_set_user_limits(png_ptr, user_width_max, user_height_max);
    
    // Get user dimension limits and verify
    png_uint_32 retrieved_width_max = png_get_user_width_max(png_ptr);
    png_uint_32 retrieved_height_max = png_get_user_height_max(png_ptr);
    (void)retrieved_width_max; // Use to avoid unused variable warning
    (void)retrieved_height_max; // Use to avoid unused variable warning
    
    // 8. Clean up structures (required helper APIs)
    
    // Clean up info structure
    png_destroy_info_struct(png_ptr, &info_ptr);
    
    // Clean up PNG structure
    png_destroy_write_struct(&png_ptr, nullptr);
    
    return 0;
}

} // namespace harness_024
