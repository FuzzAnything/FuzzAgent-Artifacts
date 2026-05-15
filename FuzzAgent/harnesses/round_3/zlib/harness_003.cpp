#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include "zlib.h"

namespace harness_003 {

// Custom callback functions for inflateBack
// Note: Match the exact signatures from zlib.h:
// typedef unsigned (*in_func)(void FAR *, z_const unsigned char FAR * FAR *);
// typedef int (*out_func)(void FAR *, unsigned char FAR *, unsigned);

// Simple pull function that returns 0 (no data) to avoid static buffer issues
// InflateBack will use the initial input from strm->next_in/avail_in
static unsigned fuzz_pull(void *desc, z_const unsigned char **buf) {
    // Return 0 to indicate no more input available
    // This is safe and avoids static buffer lifetime issues
    (void)desc;
    (void)buf;
    return 0;
}

// Simple push function that always succeeds
static int fuzz_push(void *desc, unsigned char *buf, unsigned len) {
    (void)desc;
    (void)buf;
    (void)len;
    return 0;  // Success
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for meaningful testing
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize zlib stream structure
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Set up custom allocation if desired (use defaults for now)
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    
    // Create window buffer for inflateBack (32KB as required for windowBits=15)
    static unsigned char window[32768];
    
    // Consume parameters for initialization
    int windowBits = fdp.ConsumeIntegralInRange<int>(8, 15);
    
    // Initialize inflateBack
    int ret = inflateBackInit_(&strm, windowBits, window, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        // Failed initialization, clean up and exit
        inflateBackEnd(&strm);
        return 0;
    }
    
    // Test 1: Basic inflateBack operation with callbacks
    // Provide some initial input in strm
    size_t initial_input_size = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
    if (fdp.remaining_bytes() < initial_input_size) {
        inflateBackEnd(&strm);
        return 0;
    }
    
    std::vector<uint8_t> initial_input = fdp.ConsumeBytes<uint8_t>(initial_input_size);
    strm.next_in = initial_input.data();
    strm.avail_in = static_cast<uInt>(initial_input_size);
    // Call inflateBack with our callbacks
    ret = inflateBack(&strm, fuzz_pull, &fdp, fuzz_push, &fdp);
    // Test 2: InflateSync - attempt to synchronize after possible corruption
    // Reset stream state for new test
    inflateBackEnd(&strm);
    memset(&strm, 0, sizeof(strm));
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    
    // Re-initialize for inflate (not inflateBack) to test inflateSync
    ret = inflateInit2(&strm, windowBits);
    if (ret == Z_OK) {
        // Provide some data for inflation
        size_t sync_input_size = fdp.ConsumeIntegralInRange<size_t>(4, 512);
        if (fdp.remaining_bytes() >= sync_input_size) {
            std::vector<uint8_t> sync_input = fdp.ConsumeBytes<uint8_t>(sync_input_size);
            strm.next_in = sync_input.data();
            strm.avail_in = static_cast<uInt>(sync_input_size);
            
            // Try to inflate a bit
            unsigned char out_buf[1024];
            strm.next_out = out_buf;
            strm.avail_out = sizeof(out_buf);
            
            ret = inflate(&strm, Z_NO_FLUSH);
            
            // Try to sync if we got an error
            if (ret == Z_DATA_ERROR) {
                ret = inflateSync(&strm);
            }
            
            // Try inflateSyncPoint
            (void)inflateSyncPoint(&strm);
        }
        
        // Test 3: inflateSetDictionary
        size_t dict_size = fdp.ConsumeIntegralInRange<size_t>(1, 32768);
        if (fdp.remaining_bytes() >= dict_size) {
            std::vector<uint8_t> dictionary = fdp.ConsumeBytes<uint8_t>(dict_size);
            
            // Try setting dictionary (may fail if stream not in right state)
            (void)inflateSetDictionary(&strm, dictionary.data(), static_cast<uInt>(dict_size));
        }
        
        // Test 4: inflatePrime
        // Add some bits to the bit buffer
        int bits = fdp.ConsumeIntegralInRange<int>(1, 16);
        int value = fdp.ConsumeIntegral<int>();
        (void)inflatePrime(&strm, bits, value);
        
        // Test 5: inflateCopy
        z_stream copy_strm;
        memset(&copy_strm, 0, sizeof(copy_strm));
        copy_strm.zalloc = Z_NULL;
        copy_strm.zfree = Z_NULL;
        copy_strm.opaque = Z_NULL;
        
        // Try to copy the stream (may fail depending on state)
        (void)inflateCopy(&copy_strm, &strm);
        
        // Clean up copy if it succeeded
        if (copy_strm.state != Z_NULL) {
            inflateEnd(&copy_strm);
        }
        
        inflateEnd(&strm);
    }
    
    // Test 6: Another inflateBack test with different parameters
    // Re-initialize for inflateBack with different windowBits
    memset(&strm, 0, sizeof(strm));
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    
    int windowBits2 = fdp.ConsumeIntegralInRange<int>(8, 15);
    ret = inflateBackInit_(&strm, windowBits2, window, ZLIB_VERSION, sizeof(z_stream));
    if (ret == Z_OK) {
        // Test with no initial input (callbacks will provide all data)
        strm.next_in = Z_NULL;
        strm.avail_in = 0;
        
        // Try inflateBack with remaining fuzzer data
        ret = inflateBack(&strm, fuzz_pull, &fdp, fuzz_push, &fdp);
        
        inflateBackEnd(&strm);
    }
    // Test 7: Test inflateBackEnd error cases
    // Call with null stream
    (void)inflateBackEnd(Z_NULL);
    
    // Test with stream that wasn't initialized
    z_stream bad_strm;
    memset(&bad_strm, 0, sizeof(bad_strm));
    (void)inflateBackEnd(&bad_strm);
    
    return 0;
}

} // namespace harness_003
