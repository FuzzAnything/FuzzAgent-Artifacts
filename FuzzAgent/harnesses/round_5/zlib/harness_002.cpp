#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "zlib.h"
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>
#include <string>

namespace harness_002 {
/*
 * Fuzzing harness for zlib backward inflation functions
 * Targets: inflateBack, inflateBackInit_, inflateBackEnd
 * 
 * This harness exercises the complete backward inflation lifecycle:
 * 1. Initialize z_stream using inflateInit2_
 * 2. Call inflateBackInit_ to set up backward inflation
 * 3. Implement input/output callback functions for data processing
 * 4. Call inflateBack with the callbacks to process data
 * 5. Call inflateBackEnd to clean up backward inflation state
 * 6. Call inflateEnd to clean up the z_stream
 * 
 * This targets 366 undiscovered branches in the inflateBack API family.
 */



// Input provider structure for inflateBack callback
struct InputProvider {
    FuzzedDataProvider* fdp;
    std::vector<uint8_t> buffer;
};

// Input callback function for inflateBack
static unsigned in_callback(void *in_desc, unsigned char **buf) {
    InputProvider* provider = static_cast<InputProvider*>(in_desc);
    FuzzedDataProvider* fdp = provider->fdp;
    
    // Determine how much data to provide
    size_t available = fdp->remaining_bytes();
    if (available == 0) {
        return 0;  // No input available
    }
    
    // Provide between 1 and available bytes
    size_t to_provide = fdp->ConsumeIntegralInRange<size_t>(1, available);
    if (to_provide > available) {
        to_provide = available;
    }
    
    // Consume the data into our persistent buffer
    provider->buffer = fdp->ConsumeBytes<uint8_t>(to_provide);
    
    // Pass the buffer pointer back
    *buf = provider->buffer.data();
    
    return static_cast<unsigned>(provider->buffer.size());
}

// Output callback function for inflateBack
static int out_callback(void *out_desc, unsigned char *buf, unsigned len) {
    // Just consume the output data, we don't need to do anything with it
    // for fuzzing purposes. Return 0 for success.
    return 0;
}

// Simple output callback that tracks output size
struct OutputTracker {
    size_t total_output;
};

static int out_callback_with_tracking(void *out_desc, unsigned char *buf, unsigned len) {
    OutputTracker* tracker = static_cast<OutputTracker*>(out_desc);
    tracker->total_output += len;
    return 0;  // Success
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for meaningful fuzzing
    // Need enough for windowBits, plus some data for inflation
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size parameters first
    int windowBits = fdp.ConsumeIntegralInRange<int>(8, 15);  // Valid range for windowBits
    int flush_mode = fdp.ConsumeIntegralInRange<int>(0, 4);   // Z_NO_FLUSH to Z_FINISH
    
    // Determine how much data to allocate for window
    size_t window_size = 1U << windowBits;  // 2^windowBits
    if (window_size > 32768) {  // Limit window size to 32K
        window_size = 32768;
    }
    
    // Allocate window buffer
    unsigned char* window = static_cast<unsigned char*>(malloc(window_size));
    if (window == nullptr) {
        return 0;
    }
    
    // Initialize z_stream
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Use default memory allocation
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    
    // Initialize for inflation with different methods based on fuzzer input
    int init_method = fdp.ConsumeIntegralInRange<int>(0, 2);
    int init_result = Z_OK;
    
    switch (init_method) {
        case 0:
            // inflateInit2 with windowBits
            init_result = inflateInit2(&strm, windowBits);
            break;
        case 1:
            // inflateInit with default windowBits
            init_result = inflateInit(&strm);
            break;
        case 2:
            // inflateInit2 with negative windowBits for raw deflate
            init_result = inflateInit2(&strm, -windowBits);
            break;
    }
    
    if (init_result != Z_OK) {
        free(window);
        return 0;
    }
    
    // Now initialize for backward inflation using inflateBackInit_
    int back_init_result = inflateBackInit_(&strm, windowBits, window, 
                                           ZLIB_VERSION, sizeof(z_stream));
    
    if (back_init_result != Z_OK) {
        inflateEnd(&strm);
        free(window);
        return 0;
    }
    
    // Prepare callback descriptors
    InputProvider input_provider = {&fdp, {}};
    OutputTracker output_tracker = {0};
    
    // Initialize next_in and avail_in to NULL/0 since inflateBack uses callbacks
    strm.next_in = Z_NULL;
    strm.avail_in = 0;
    
    // Call inflateBack with our callbacks
    // Try different combinations of callbacks
    int callback_choice = fdp.ConsumeIntegralInRange<int>(0, 1);
    int inflate_back_result = Z_OK;
    
    if (callback_choice == 0) {
        // Use simple output callback
        inflate_back_result = inflateBack(&strm, 
                                         in_callback, &input_provider,
                                         out_callback, nullptr);
    } else {
        // Use tracking output callback
        inflate_back_result = inflateBack(&strm,
                                         in_callback, &input_provider,
                                         out_callback_with_tracking, &output_tracker);
    }
    
    // The result could be various values: Z_STREAM_END, Z_BUF_ERROR, Z_DATA_ERROR, etc.
    // All are valid outcomes for fuzzing.
    
    // Clean up backward inflation state
    int back_end_result = inflateBackEnd(&strm);
    
    // Clean up z_stream
    int end_result = inflateEnd(&strm);
    
    // Free window buffer
    free(window);
    
    return 0;
}

} // namespace harness_002
