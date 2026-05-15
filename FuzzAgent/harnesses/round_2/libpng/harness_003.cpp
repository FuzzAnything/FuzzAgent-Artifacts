#include <fuzzer/FuzzedDataProvider.h>
#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <vector>

namespace harness_003 {

// Progressive reading callback structures
struct ProgressiveContext {
    FuzzedDataProvider* fdp;
    png_structp png_ptr;
    png_infop info_ptr;
    std::vector<png_bytep> row_pointers;
    png_uint_32 width;
    png_uint_32 height;
    int bit_depth;
    int color_type;
    bool info_received;
    bool end_received;
    int rows_received;
    jmp_buf jmpbuf;
};

// Error handling functions
void user_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    // Do nothing - just return
}

void user_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
    // Do nothing - just return
}

// Progressive info callback - called when PNG info header is available
void progressive_info_callback(png_structp png_ptr, png_infop info_ptr) {
    ProgressiveContext* ctx = static_cast<ProgressiveContext*>(png_get_progressive_ptr(png_ptr));
    if (!ctx) return;
    
    if (setjmp(ctx->jmpbuf)) {
        return;
    }
    
    // Get image information
    png_get_IHDR(png_ptr, info_ptr, &ctx->width, &ctx->height, &ctx->bit_depth, 
                 &ctx->color_type, NULL, NULL, NULL);
    
    ctx->info_received = true;
    
    // Allocate row pointers for progressive reading
    size_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    ctx->row_pointers.resize(ctx->height);
    for (png_uint_32 i = 0; i < ctx->height; ++i) {
        ctx->row_pointers[i] = static_cast<png_bytep>(png_malloc(png_ptr, rowbytes));
    }
    
    // Update info for progressive reading
    png_read_update_info(png_ptr, info_ptr);
}

// Progressive row callback - called for each row of image data
void progressive_row_callback(png_structp png_ptr, png_bytep new_row, 
                              png_uint_32 row_num, int pass) {
    ProgressiveContext* ctx = static_cast<ProgressiveContext*>(png_get_progressive_ptr(png_ptr));
    if (!ctx) return;
    
    if (setjmp(ctx->jmpbuf)) {
        return;
    }
    
    // Combine the new row with existing row data
    if (new_row && ctx->row_pointers[row_num]) {
        png_progressive_combine_row(png_ptr, ctx->row_pointers[row_num], new_row);
    }
    
    ctx->rows_received++;
}

// Progressive end callback - called when image is complete
void progressive_end_callback(png_structp png_ptr, png_infop info_ptr) {
    ProgressiveContext* ctx = static_cast<ProgressiveContext*>(png_get_progressive_ptr(png_ptr));
    if (!ctx) return;
    
    ctx->end_received = true;
    (void)info_ptr; // Unused
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum data for progressive reading
    if (size < 100) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Create PNG read structure
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                 NULL, 
                                                 user_error_fn, 
                                                 user_warning_fn);
    if (png_ptr == NULL) {
        return 0;
    }
    
    // Create PNG info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }
    
    // Set up progressive context
    ProgressiveContext ctx;
    ctx.fdp = &fdp;
    ctx.png_ptr = png_ptr;
    ctx.info_ptr = info_ptr;
    ctx.info_received = false;
    ctx.end_received = false;
    ctx.rows_received = 0;
    
    // Set up error handling using setjmp/longjmp in main context
    if (setjmp(png_jmpbuf(png_ptr))) {
        // Cleanup on error
        for (auto row : ctx.row_pointers) {
            if (row) png_free(png_ptr, row);
        }
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }
    
    // Set up progressive read callbacks
    png_set_progressive_read_fn(png_ptr, &ctx, 
                                progressive_info_callback,
                                progressive_row_callback,
                                progressive_end_callback);
    
    // Consume parameters for progressive reading strategy
    bool use_pause_resume = fdp.ConsumeBool();
    size_t max_chunk_size = fdp.ConsumeIntegralInRange<size_t>(1, 4096);
    size_t pause_threshold = fdp.ConsumeIntegralInRange<size_t>(1, 1000);
    
    // Get the PNG data to process
    std::vector<uint8_t> png_data = fdp.ConsumeRemainingBytes<uint8_t>();
    if (png_data.empty()) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }
    
    // Process data in chunks to simulate streaming
    size_t processed = 0;
    bool paused = false;
    
    while (processed < png_data.size()) {
        // Determine chunk size
        size_t chunk_size = std::min(max_chunk_size, png_data.size() - processed);
        
        // Process this chunk
        png_process_data(png_ptr, info_ptr, &png_data[processed], chunk_size);
        processed += chunk_size;
        
        // Test pause/resume functionality if enabled
        if (use_pause_resume && processed >= pause_threshold && !paused) {
            // Pause processing
            size_t remaining = png_process_data_pause(png_ptr, 1);
            paused = true;
            
            // Resume after potentially skipping some bytes
            if (fdp.ConsumeBool() && remaining > 0) {
                size_t skip_bytes = fdp.ConsumeIntegralInRange<size_t>(0, remaining);
                processed = png_data.size() - remaining + skip_bytes;
            }
            
            // Resume processing
            png_process_data(png_ptr, info_ptr, &png_data[processed], png_data.size() - processed);
            paused = false;
        }
    }
    
    // Test png_process_data_skip if there's remaining data
    if (fdp.ConsumeBool()) {
        png_uint_32 skip_amount = png_process_data_skip(png_ptr);
        (void)skip_amount; // Result may be used in real scenarios
    }
    
    // Clean up allocated row pointers
    for (auto row : ctx.row_pointers) {
        if (row) png_free(png_ptr, row);
    }
    
    // Clean up PNG structures
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    
    return 0;
}

} // namespace harness_003
