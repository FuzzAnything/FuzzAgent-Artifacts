#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <fuzzer/FuzzedDataProvider.h>
#include "png.h"

namespace harness_004 {


// Simple error handling for fuzzing
static void pngtest_error(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    longjmp(png_jmpbuf(png_ptr), 1);
}

static void pngtest_warning(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

// Progressive reading callbacks for fuzzing
typedef struct {
    FuzzedDataProvider* fdp;
    png_bytep* row_pointers;
    png_uint_32 height;
    size_t rows_received;
    size_t info_called;
    size_t end_called;
} progressive_data;

static void info_callback(png_structp png_ptr, png_infop info_ptr) {
    progressive_data* pdata = (progressive_data*)png_get_progressive_ptr(png_ptr);
    if (pdata == NULL) return;
    
    pdata->info_called++;
    
    // Get image dimensions
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    
    // Store height for row allocation
    pdata->height = height;
    
    // Allocate row pointers if we have height
    if (height > 0 && height < 10000) { // Reasonable limit for fuzzing
        pdata->row_pointers = (png_bytep*)png_malloc(png_ptr, height * sizeof(png_bytep));
        if (pdata->row_pointers != NULL) {
            png_size_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);
            for (png_uint_32 i = 0; i < height; i++) {
                pdata->row_pointers[i] = (png_bytep)png_malloc(png_ptr, rowbytes);
                if (pdata->row_pointers[i] != NULL) {
                    memset(pdata->row_pointers[i], 0, rowbytes);
                }
            }
        }
    }
    
    // Apply some transformations based on fuzzer input
    if (pdata->fdp->remaining_bytes() > 0) {
        uint8_t transform_flags = pdata->fdp->ConsumeIntegral<uint8_t>();
        
        if (transform_flags & 0x01) png_set_expand(png_ptr);
        if (transform_flags & 0x02) png_set_gray_to_rgb(png_ptr);
        if (transform_flags & 0x04) png_set_rgb_to_gray(png_ptr, 1, 0.299, 0.587);
        if (transform_flags & 0x08) png_set_strip_16(png_ptr);
        if (transform_flags & 0x10) png_set_strip_alpha(png_ptr);
        if (transform_flags & 0x20) png_set_bgr(png_ptr);
        if (transform_flags & 0x40) png_set_swap(png_ptr);
        if (transform_flags & 0x80) png_set_packing(png_ptr);
    }
    
    // Update info after transformations
    png_read_update_info(png_ptr, info_ptr);
}

static void row_callback(png_structp png_ptr, png_bytep new_row, 
                         png_uint_32 row_num, int pass) {
    progressive_data* pdata = (progressive_data*)png_get_progressive_ptr(png_ptr);
    if (pdata == NULL) return;
    
    pdata->rows_received++;
    
    // Combine row if we have allocated buffer
    if (pdata->row_pointers != NULL && row_num < pdata->height) {
        if (new_row != NULL && pdata->row_pointers[row_num] != NULL) {
            png_progressive_combine_row(png_ptr, pdata->row_pointers[row_num], new_row);
        }
    }
}

static void end_callback(png_structp png_ptr, png_infop info_ptr) {
    progressive_data* pdata = (progressive_data*)png_get_progressive_ptr(png_ptr);
    if (pdata == NULL) return;
    
    pdata->end_called++;
    
    // Clean up row pointers
    if (pdata->row_pointers != NULL && pdata->height > 0) {
        for (png_uint_32 i = 0; i < pdata->height; i++) {
            if (pdata->row_pointers[i] != NULL) {
                png_free(png_ptr, pdata->row_pointers[i]);
            }
        }
        png_free(png_ptr, pdata->row_pointers);
        pdata->row_pointers = NULL;
    }
}

// Custom read function for reading from memory buffer
struct png_read_data {
    const uint8_t* data;
    size_t size;
    size_t offset;
};

static void png_read_from_buffer(png_structp png_ptr, png_bytep out, png_size_t length) {
    png_read_data* rd = (png_read_data*)png_get_io_ptr(png_ptr);
    if (rd->offset + length > rd->size) {
        png_error(png_ptr, "Read beyond buffer");
    }
    memcpy(out, rd->data + rd->offset, length);
    rd->offset += length;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 64) {
        // Need enough data for PNG signature and progressive reading tests
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume configuration from fuzzer input
    uint8_t test_mode = fdp.ConsumeIntegral<uint8_t>() % 4;
    uint8_t chunk_size_mode = fdp.ConsumeIntegral<uint8_t>() % 3;
    
    // Consume remaining bytes for PNG data
    std::vector<uint8_t> png_data = fdp.ConsumeRemainingBytes<uint8_t>();
    if (png_data.size() < 8) {
        // PNG signature is 8 bytes
        return 0;
    }
    
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    png_infop end_info = NULL;
    
    // Create read structure
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, pngtest_error, pngtest_warning);
    if (png_ptr == NULL) {
        return 0;
    }
    
    // Set up error handling with setjmp - MUST be done before any libpng calls that can longjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
        // Error occurred during PNG processing
        if (png_ptr) {
            png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        }
        return 0;
    }
    
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }
    
    end_info = png_create_info_struct(png_ptr);
    if (end_info == NULL) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }
    
    // Set up custom read function
    png_read_data read_data = {png_data.data(), png_data.size(), 0};
    png_set_read_fn(png_ptr, &read_data, png_read_from_buffer);
    png_set_sig_bytes(png_ptr, 0);
    
    // Create progressive data structure
    progressive_data pdata;
    memset(&pdata, 0, sizeof(pdata));
    pdata.fdp = &fdp;
    
    // Set up progressive reading
    png_set_progressive_read_fn(png_ptr, &pdata, info_callback, row_callback, end_callback);
    
    // Test different progressive reading modes
    switch (test_mode) {
        case 0: {
            // Mode 0: Process all data at once (simple progressive reading)
            png_process_data(png_ptr, info_ptr, (png_byte*)png_data.data(), png_data.size());
            break;
        }
        
        case 1: {
            // Mode 1: Process data in chunks (simulating streaming)
            size_t processed = 0;
            while (processed < png_data.size()) {
                size_t chunk_size = 0;
                switch (chunk_size_mode) {
                    case 0: chunk_size = 1024; break;
                    case 1: chunk_size = 256; break;
                    case 2: chunk_size = 4096; break;
                }
                
                if (chunk_size > png_data.size() - processed) {
                    chunk_size = png_data.size() - processed;
                }
                
                png_process_data(png_ptr, info_ptr, (png_byte*)png_data.data() + processed, chunk_size);
                processed += chunk_size;
                
                // Test png_process_data_pause occasionally
                if ((processed % 2048) == 0 && processed > 0) {
                    size_t remaining = png_process_data_pause(png_ptr, 0);
                    (void)remaining; // Use remaining to avoid unused warning
                }
            }
            break;
        }
        
        case 2: {
            // Mode 2: Test png_process_data_pause with save
            size_t processed = 0;
            size_t chunk_size = 512;
            
            while (processed < png_data.size()) {
                if (chunk_size > png_data.size() - processed) {
                    chunk_size = png_data.size() - processed;
                }
                
                png_process_data(png_ptr, info_ptr, (png_byte*)png_data.data() + processed, chunk_size);
                processed += chunk_size;
                
                // Test png_process_data_pause with save
                if (processed >= 1024 && processed < png_data.size()) {
                    size_t remaining = png_process_data_pause(png_ptr, 1);
                    (void)remaining;
                    break; // Simulate pause and resume scenario
                }
            }
            break;
        }
        
        case 3: {
            // Mode 3: Test png_process_data_skip (though it's not implemented in current libpng)
            // Process some data first
            size_t initial_size = png_data.size() > 1024 ? 1024 : png_data.size();
            png_process_data(png_ptr, info_ptr, (png_byte*)png_data.data(), initial_size);
            
            // Call png_process_data_skip (even though it returns 0 in current versions)
            png_uint_32 skip_amount = png_process_data_skip(png_ptr);
            (void)skip_amount;
            
            // Process remaining data if any
            if (initial_size < png_data.size()) {
                png_process_data(png_ptr, info_ptr, (png_byte*)png_data.data() + initial_size, 
                                 png_data.size() - initial_size);
            }
            break;
        }
    }
    
    // Test png_get_progressive_ptr
    void* progressive_ptr = png_get_progressive_ptr(png_ptr);
    (void)progressive_ptr;
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    
    return 0;
}

} // namespace harness_004
