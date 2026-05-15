#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include "zlib.h"

namespace harness_018 {

// Custom callback functions for inflateBack with dynamic fuzzer data
// Callback descriptor structure to pass FuzzedDataProvider state
struct CallbackDesc {
    FuzzedDataProvider* fdp;
    size_t max_pull_size;
    size_t total_pulled;
    bool should_fail;
    std::vector<uint8_t> current_chunk;  // Keep chunk alive for callback
};

// Input callback for inflateBack - returns data from fuzzer input
static unsigned fuzz_inflateBack_pull(void *desc, z_const unsigned char **buf) {
    CallbackDesc* cb_desc = static_cast<CallbackDesc*>(desc);
    if (!cb_desc || !cb_desc->fdp || cb_desc->should_fail) {
        return 0;  // Fail intentionally or no data
    }
    
    // Determine how much data to return (1-256 bytes)
    if (cb_desc->fdp->remaining_bytes() == 0) {
        return 0;  // No more data
    }
    
    size_t chunk_size = cb_desc->fdp->ConsumeIntegralInRange<size_t>(1, 256);
    if (chunk_size > cb_desc->fdp->remaining_bytes()) {
        chunk_size = cb_desc->fdp->remaining_bytes();
    }
    
    if (chunk_size == 0) {
        return 0;
    }
    
    // Consume data from fuzzer input and store in descriptor
    cb_desc->current_chunk = cb_desc->fdp->ConsumeBytes<uint8_t>(chunk_size);
    
    *buf = cb_desc->current_chunk.data();
    cb_desc->total_pulled += chunk_size;
    
    return static_cast<unsigned>(chunk_size);
}

// Output callback for inflateBack - accepts decompressed data
static int fuzz_inflateBack_push(void *desc, unsigned char *buf, unsigned len) {
    CallbackDesc* cb_desc = static_cast<CallbackDesc*>(desc);
    if (!cb_desc || cb_desc->should_fail) {
        return -1;  // Force error
    }
    
    // Just accept the data - could validate or store it
    // For fuzzing, we just return success
    (void)buf;
    (void)len;
    
    return 0;  // Success
}

// Alternative pull callback that sometimes returns 0 (no data available)
static unsigned fuzz_inflateBack_pull_intermittent(void *desc, z_const unsigned char **buf) {
    static int call_count = 0;
    call_count++;
    
    CallbackDesc* cb_desc = static_cast<CallbackDesc*>(desc);
    if (!cb_desc || !cb_desc->fdp) {
        return 0;
    }
    
    // Sometimes return 0 to simulate no data available
    if (call_count % 3 == 0) {
        return 0;
    }
    
    return fuzz_inflateBack_pull(desc, buf);
}

// Alternative push callback that sometimes fails
static int fuzz_inflateBack_push_intermittent(void *desc, unsigned char *buf, unsigned len) {
    static int call_count = 0;
    call_count++;
    
    CallbackDesc* cb_desc = static_cast<CallbackDesc*>(desc);
    if (!cb_desc) {
        return -1;
    }
    
    // Sometimes fail to test error handling
    if (call_count % 5 == 0) {
        return -1;
    }
    
    return fuzz_inflateBack_push(desc, buf, len);
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size for meaningful testing
    if (size < 128) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Test 1: Basic inflateBack with dynamic callbacks
    {
        z_stream strm;
        memset(&strm, 0, sizeof(strm));
        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;
        
        // Create window buffer (32KB for windowBits=15)
        unsigned char window[32768];
        
        // Consume windowBits parameter
        int windowBits = fdp.ConsumeIntegralInRange<int>(8, 15);
        
        // Initialize inflateBack
        int ret = inflateBackInit_(&strm, windowBits, window, ZLIB_VERSION, sizeof(z_stream));
        if (ret == Z_OK) {
            CallbackDesc desc;
            desc.fdp = &fdp;
            desc.max_pull_size = 1024;
            desc.total_pulled = 0;
            desc.should_fail = false;
            desc.current_chunk.clear();
            
            // Provide some initial input in strm (optional)
            bool provide_initial_input = fdp.ConsumeBool();
            if (provide_initial_input && fdp.remaining_bytes() > 0) {
                size_t initial_size = fdp.ConsumeIntegralInRange<size_t>(1, 512);
                if (initial_size > fdp.remaining_bytes()) {
                    initial_size = fdp.remaining_bytes();
                }
                std::vector<uint8_t> initial_data = fdp.ConsumeBytes<uint8_t>(initial_size);
                // Store in descriptor to keep data alive
                desc.current_chunk = std::move(initial_data);
                strm.next_in = desc.current_chunk.data();
                strm.avail_in = static_cast<uInt>(initial_size);
            } else {
                strm.next_in = Z_NULL;
                strm.avail_in = 0;
            }
            
            // Call inflateBack with dynamic callbacks
            ret = inflateBack(&strm, fuzz_inflateBack_pull, &desc, 
                             fuzz_inflateBack_push, &desc);
            
            // Record return value for analysis
            (void)ret;
            
            inflateBackEnd(&strm);
        }
    }
    // Test 2: inflateBack with intermittent callbacks (sometimes fail)
    if (fdp.remaining_bytes() > 64) {
        z_stream strm;
        memset(&strm, 0, sizeof(strm));
        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;
        
        unsigned char window[32768];
        int windowBits = fdp.ConsumeIntegralInRange<int>(8, 15);
        
        int ret = inflateBackInit_(&strm, windowBits, window, ZLIB_VERSION, sizeof(z_stream));
        if (ret == Z_OK) {
            CallbackDesc desc;
            desc.fdp = &fdp;
            desc.max_pull_size = 512;
            desc.total_pulled = 0;
            desc.should_fail = false;
            desc.current_chunk.clear();
            
            // No initial input - rely entirely on callbacks
            strm.next_in = Z_NULL;
            strm.avail_in = 0;
            
            ret = inflateBack(&strm, fuzz_inflateBack_pull_intermittent, &desc,
                             fuzz_inflateBack_push_intermittent, &desc);
            
            (void)ret;
            
            inflateBackEnd(&strm);
        }
    }
    
    // Test 3: inflateBack with custom memory allocators
    if (fdp.remaining_bytes() > 32) {
        z_stream strm;
        memset(&strm, 0, sizeof(strm));
        
        // Use default allocators
        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;
        
        unsigned char window[32768];
        int windowBits = fdp.ConsumeIntegralInRange<int>(8, 15);
        
        // Test inflateBackInit (not inflateBackInit_) 
        int ret = inflateBackInit(&strm, windowBits, window);
        if (ret == Z_OK) {
            CallbackDesc desc;
            desc.fdp = &fdp;
            desc.max_pull_size = 256;
            desc.total_pulled = 0;
            desc.should_fail = fdp.ConsumeBool();
            desc.current_chunk.clear();
            
            // Provide initial input
            if (fdp.remaining_bytes() > 0) {
                size_t initial_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
                if (initial_size > fdp.remaining_bytes()) {
                    initial_size = fdp.remaining_bytes();
                }
                std::vector<uint8_t> initial_data = fdp.ConsumeBytes<uint8_t>(initial_size);
                desc.current_chunk = std::move(initial_data);
                strm.next_in = desc.current_chunk.data();
                strm.avail_in = static_cast<uInt>(initial_size);
            } else {
                strm.next_in = Z_NULL;
                strm.avail_in = 0;
            }
            
            ret = inflateBack(&strm, fuzz_inflateBack_pull, &desc,
                             fuzz_inflateBack_push, &desc);
            
            (void)ret;
            
            inflateBackEnd(&strm);
        }
    }
    
    // Test 4: Error cases and edge conditions
    {
        // Test with null stream
        (void)inflateBackInit(Z_NULL, 15, nullptr);
        (void)inflateBack(Z_NULL, nullptr, nullptr, nullptr, nullptr);
        (void)inflateBackEnd(Z_NULL);
        
        // Test with invalid windowBits
        z_stream strm;
        memset(&strm, 0, sizeof(strm));
        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;
        
        unsigned char window[32768];
        int ret = inflateBackInit_(&strm, 7, window, ZLIB_VERSION, sizeof(z_stream));  // Too small
        (void)ret;
        if (strm.state != Z_NULL) {
            inflateBackEnd(&strm);
        }
        
        // Test with too large windowBits
        memset(&strm, 0, sizeof(strm));
        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;
        ret = inflateBackInit_(&strm, 16, window, ZLIB_VERSION, sizeof(z_stream));  // Too large
        (void)ret;
        if (strm.state != Z_NULL) {
            inflateBackEnd(&strm);
        }
    }
    
    // Test 5: Multiple inflateBack cycles on same stream
    if (fdp.remaining_bytes() > 256) {
        z_stream strm;
        memset(&strm, 0, sizeof(strm));
        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;
        
        unsigned char window[32768];
        int windowBits = fdp.ConsumeIntegralInRange<int>(8, 15);
        
        int ret = inflateBackInit_(&strm, windowBits, window, ZLIB_VERSION, sizeof(z_stream));
        if (ret == Z_OK) {
            CallbackDesc desc;
            desc.fdp = &fdp;
            desc.max_pull_size = 128;
            desc.total_pulled = 0;
            desc.should_fail = false;
            desc.current_chunk.clear();
            
            // First call
            strm.next_in = Z_NULL;
            strm.avail_in = 0;
            ret = inflateBack(&strm, fuzz_inflateBack_pull, &desc,
                             fuzz_inflateBack_push, &desc);
            
            // Reset stream for another call (if first didn't complete)
            if (ret != Z_STREAM_END && fdp.remaining_bytes() > 64) {
                // Provide new initial input
                size_t initial_size = fdp.ConsumeIntegralInRange<size_t>(1, 128);
                if (initial_size > fdp.remaining_bytes()) {
                    initial_size = fdp.remaining_bytes();
                }
                std::vector<uint8_t> initial_data = fdp.ConsumeBytes<uint8_t>(initial_size);
                desc.current_chunk = std::move(initial_data);
                strm.next_in = desc.current_chunk.data();
                strm.avail_in = static_cast<uInt>(initial_size);
                
                // Second call with same stream
                ret = inflateBack(&strm, fuzz_inflateBack_pull, &desc,
                                 fuzz_inflateBack_push, &desc);
            }
            
            inflateBackEnd(&strm);
        }
    }
    
    // Test 6: Test with different window sizes
    {
        for (int i = 0; i < 2 && fdp.remaining_bytes() > 64; i++) {
            z_stream strm;
            memset(&strm, 0, sizeof(strm));
            strm.zalloc = Z_NULL;
            strm.zfree = Z_NULL;
            strm.opaque = Z_NULL;
            
            int windowBits = fdp.ConsumeIntegralInRange<int>(8, 15);
            size_t window_size = 1U << windowBits;
            
            // Allocate window dynamically based on windowBits
            std::vector<unsigned char> window(window_size);
            
            int ret = inflateBackInit_(&strm, windowBits, window.data(), 
                                       ZLIB_VERSION, sizeof(z_stream));
            if (ret == Z_OK) {
                CallbackDesc desc;
                desc.fdp = &fdp;
                desc.max_pull_size = 64;
                desc.total_pulled = 0;
                desc.should_fail = false;
                desc.current_chunk.clear();
                
                strm.next_in = Z_NULL;
                strm.avail_in = 0;
                
                ret = inflateBack(&strm, fuzz_inflateBack_pull, &desc,
                                 fuzz_inflateBack_push, &desc);
                
                (void)ret;
                
                inflateBackEnd(&strm);
            }
        }
    }
    
    return 0;
}

} // namespace harness_018
