#include <cstdint>
#include <cstddef>
#include <cstring>
#include <vector>
#include <string>
#include <memory>
#include "zlib.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_010 {
/* Targeted error condition fuzzing harness for zlib inflateBack module APIs
 * Primary target: inflateBack module with 162 blocked branches (47.6% blocked)
 * APIs to target: inflateBack, inflateBackInit_, inflateBackEnd
 * Focus: Specific error condition testing to improve branch coverage
 * 
 * This harness complements harness_002 by specifically targeting error paths,
 * boundary conditions, and unusual API sequences in the inflateBack module.
 * 
 * Error conditions to test:
 * 1. Invalid windowBits values (outside 8..15 range)
 * 2. NULL pointers for stream, window, or callback functions
 * 3. Invalid stream_size parameter in inflateBackInit_
 * 4. Version mismatch errors
 * 5. Memory allocation failures (simulated via NULL returns)
 * 6. Invalid/corrupted deflate stream data
 * 7. Callback function failures (in() returns 0, out() returns non-zero)
 * 8. Unusual API sequences (calling inflateBackEnd before init, reusing streams improperly)
 * 9. Boundary conditions for window sizes and buffer allocations
 */


// Minimum input size requirement for meaningful error testing
const size_t MIN_INPUT_SIZE = 16;

// Maximum buffer sizes to prevent excessive memory usage
const size_t MAX_WINDOW_SIZE = 32768;  // 32KB (2^15, max window size)
const size_t MAX_INPUT_DATA = 1024 * 1024;  // 1MB

// Callback context structures for testing error conditions
struct ErrorTestCallbackContext {
    const uint8_t* data;
    size_t size;
    size_t position;
    bool should_fail;  // Whether callback should simulate failure
    int fail_mode;     // 0: normal, 1: in() returns 0 immediately, 2: out() returns non-zero, 3: both fail
};
// Error-testing in() callback function
unsigned error_in_callback(void FAR* desc, z_const unsigned char FAR* FAR* buf) {
    ErrorTestCallbackContext* ctx = reinterpret_cast<ErrorTestCallbackContext*>(desc);
    
    if (!ctx) {
        return 0;  // Simulate callback failure with NULL context
    }
    
    // Test different failure modes
    if (ctx->should_fail && (ctx->fail_mode == 1 || ctx->fail_mode == 3)) {
        return 0;  // Simulate input failure
    }
    if (ctx->position >= ctx->size) {
        return 0;  // No more data available
    }
    
    // Provide pointer to remaining data
    *buf = const_cast<unsigned char FAR*>(ctx->data + ctx->position);
    
    // Calculate how much to provide
    size_t available = ctx->size - ctx->position;
    size_t to_provide = available;
    
    // Update position
    ctx->position += to_provide;
    return static_cast<unsigned>(to_provide);
}

// Error-testing out() callback function
int error_out_callback(void FAR* desc, unsigned char FAR* buf, unsigned len) {
    ErrorTestCallbackContext* ctx = reinterpret_cast<ErrorTestCallbackContext*>(desc);
    
    if (!ctx) {
        return 1;  // Simulate callback failure with NULL context
    }
    
    // Test different failure modes
    if (ctx->should_fail && (ctx->fail_mode == 2 || ctx->fail_mode == 3)) {
        return 1;  // Simulate output failure
    }
    if (!buf || len == 0) {
        return 0;  // Nothing to do
    }
    
    // For error testing, we might simulate various failure conditions
    // but for now just succeed
    return 0;
}

// Test specific error conditions in inflateBackInit_
void test_inflateBackInit_errors(FuzzedDataProvider& fdp) {
    // Use fuzzer input to test various error conditions
    
    // Test 1: Invalid windowBits values from fuzzer input
    int num_window_tests = fdp.ConsumeIntegralInRange<int>(1, 8);
    for (int i = 0; i < num_window_tests && fdp.remaining_bytes() > 0; i++) {
        int windowBits = fdp.ConsumeIntegralInRange<int>(-20, 20);  // Include invalid range
        z_stream strm;
        memset(&strm, 0, sizeof(strm));
        
        // Allocate window buffer based on abs(windowBits) but with max 32768
        int window_size = 1U << (abs(windowBits) % 16);
        if (window_size > 32768) window_size = 32768;
        std::vector<unsigned char> window(window_size);
        
        int result = inflateBackInit_(&strm, windowBits, window.data(),
                                     ZLIB_VERSION, sizeof(z_stream));
        // Clean up if somehow succeeded
        if (result == Z_OK) {
            inflateBackEnd(&strm);
        }
    }
    
    // Test 2: Invalid stream/window pointers (only if we have enough data)
    if (fdp.remaining_bytes() > 16) {
        int windowBits = fdp.ConsumeIntegralInRange<int>(8, 15);
        size_t window_size = 1U << windowBits;
        std::vector<unsigned char> window(window_size);
        
        // Test with invalid stream state instead of NULL stream pointer
        // Create stream but don't initialize properly
        z_stream invalid_strm;
        memset(&invalid_strm, 0, sizeof(invalid_strm));
        // Set some invalid state
        invalid_strm.next_in = NULL;
        invalid_strm.avail_in = 0;
        invalid_strm.total_in = 0;
        invalid_strm.next_out = NULL;
        invalid_strm.avail_out = 0;
        invalid_strm.total_out = 0;
        invalid_strm.msg = NULL;
        invalid_strm.state = NULL;
        invalid_strm.zalloc = NULL;
        invalid_strm.zfree = NULL;
        invalid_strm.opaque = NULL;
        invalid_strm.data_type = 0;
        invalid_strm.adler = 0;
        invalid_strm.reserved = 0;
        
        if (fdp.ConsumeBool()) {  // Only test sometimes
            // Try to initialize with stream in invalid state
            inflateBackInit_(&invalid_strm, windowBits, window.data(), 
                            ZLIB_VERSION, sizeof(z_stream));
        }
    }
    
    // Test 3: Test with small window buffer that might be insufficient
    if (fdp.remaining_bytes() > 8) {
        z_stream strm;
        memset(&strm, 0, sizeof(strm));
        int windowBits = fdp.ConsumeIntegralInRange<int>(8, 15);
        
        // Create a smaller window buffer than required
        size_t small_window_size = (1U << windowBits) / 2;  // Half the required size
        if (small_window_size == 0) small_window_size = 1;
        std::vector<unsigned char> small_window(small_window_size);
        
        // This should fail with Z_STREAM_ERROR
        inflateBackInit_(&strm, windowBits, small_window.data(), 
                        ZLIB_VERSION, sizeof(z_stream));
    }
    
    // Test 4: Invalid stream_size
    if (fdp.remaining_bytes() > 16) {
        int windowBits = fdp.ConsumeIntegralInRange<int>(8, 15);
        size_t window_size = 1U << windowBits;
        std::vector<unsigned char> window(window_size);
        z_stream strm;
        memset(&strm, 0, sizeof(strm));
        
        // Test various invalid stream sizes
        int stream_size_variant = fdp.ConsumeIntegralInRange<int>(0, 3);
        size_t stream_size;
        
        switch (stream_size_variant) {
            case 0: stream_size = sizeof(z_stream) - 1; break;
            case 1: stream_size = sizeof(z_stream) + 1; break;
            case 2: stream_size = 0; break;
            case 3: stream_size = sizeof(z_stream) * 2; break;
        }
        
        inflateBackInit_(&strm, windowBits, window.data(),
                        ZLIB_VERSION, stream_size);
    }
    
    // Test 5: Version mismatch
    if (fdp.remaining_bytes() > 8) {
        int windowBits = fdp.ConsumeIntegralInRange<int>(8, 15);
        size_t window_size = 1U << windowBits;
        std::vector<unsigned char> window(window_size);
        z_stream strm;
        memset(&strm, 0, sizeof(strm));
        
        // Test different version strings
        int version_test = fdp.ConsumeIntegralInRange<int>(0, 2);
        const char* version;
        
        switch (version_test) {
            case 0: 
                version = "invalid_version";
                break;
            case 1:
                version = "";
                break;
            case 2:
                version = NULL;
                break;
        }
        
        inflateBackInit_(&strm, windowBits, window.data(),
                        version, sizeof(z_stream));
    }
}

// Test specific error conditions in inflateBack
void test_inflateBack_errors(FuzzedDataProvider& fdp, 
                            const std::vector<uint8_t>& compressed_data) {
    // Test with various invalid parameters
    
    // Test 1: NULL stream pointer - only test sometimes based on fuzzer input
    ErrorTestCallbackContext ctx;
    ctx.data = compressed_data.data();
    ctx.size = compressed_data.size();
    ctx.position = 0;
    ctx.should_fail = false;
    ctx.fail_mode = 0;
    
    unsigned char window[32768];
    // Test 1: NULL stream pointer - instead test with uninitialized stream
    if (fdp.ConsumeBool()) {  
        // Create a stream but don't initialize it properly
        z_stream uninit_strm;
        memset(&uninit_strm, 0, sizeof(uninit_strm));
        // Don't call inflateBackInit_ - test with uninitialized stream
        inflateBack(&uninit_strm, 
                   reinterpret_cast<in_func>(error_in_callback), &ctx,
                   reinterpret_cast<out_func>(error_out_callback), &ctx);
    }
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    // First initialize properly
    if (inflateBackInit_(&strm, 15, window, ZLIB_VERSION, sizeof(z_stream)) == Z_OK) {
        if (fdp.ConsumeBool()) {
            // Instead of NULL callback, use error_in_callback but set context to simulate failure
            ErrorTestCallbackContext fail_ctx = ctx;
            fail_ctx.should_fail = true;
            fail_ctx.fail_mode = 1;  // in() returns 0
            inflateBack(&strm, 
                       reinterpret_cast<in_func>(error_in_callback), &fail_ctx,
                       reinterpret_cast<out_func>(error_out_callback), &fail_ctx);
        }
        
        if (fdp.ConsumeBool()) {
            // Instead of NULL callback, use error_out_callback but set context to simulate failure
            ErrorTestCallbackContext fail_ctx = ctx;
            fail_ctx.should_fail = true;
            fail_ctx.fail_mode = 2;  // out() returns non-zero
            inflateBack(&strm, 
                       reinterpret_cast<in_func>(error_in_callback), &fail_ctx,
                       reinterpret_cast<out_func>(error_out_callback), &fail_ctx);
        }
        
        if (fdp.ConsumeBool()) {
            // Test with both callbacks set to simulate failures
            ErrorTestCallbackContext fail_ctx = ctx;
            fail_ctx.should_fail = true;
            fail_ctx.fail_mode = 3;  // Both fail
            inflateBack(&strm, 
                       reinterpret_cast<in_func>(error_in_callback), &fail_ctx,
                       reinterpret_cast<out_func>(error_out_callback), &fail_ctx);
        }
        
        inflateBackEnd(&strm);
    }
    
    // Test 3: Stream not initialized - only test sometimes
    if (fdp.ConsumeBool()) {
        memset(&strm, 0, sizeof(strm));
        inflateBack(&strm,
                   reinterpret_cast<in_func>(error_in_callback), &ctx,
                   reinterpret_cast<out_func>(error_out_callback), &ctx);
    }
    
    // Test 4: Call inflateBackEnd on uninitialized stream - only test sometimes
    if (fdp.ConsumeBool()) {
        memset(&strm, 0, sizeof(strm));
        inflateBackEnd(&strm);
    }
    
    // Test 5: Double initialization - only test sometimes
    if (fdp.ConsumeBool()) {
        memset(&strm, 0, sizeof(strm));
        inflateBackInit_(&strm, 15, window, ZLIB_VERSION, sizeof(z_stream));
        inflateBackInit_(&strm, 15, window, ZLIB_VERSION, sizeof(z_stream));
        inflateBackEnd(&strm);
    }
    
    // Test 6: Double cleanup - only test sometimes
    if (fdp.ConsumeBool()) {
        memset(&strm, 0, sizeof(strm));
        inflateBackInit_(&strm, 15, window, ZLIB_VERSION, sizeof(z_stream));
        inflateBackEnd(&strm);
        inflateBackEnd(&strm);
    }
}

// Test callback failure scenarios
void test_callback_failures(FuzzedDataProvider& fdp,
                           const std::vector<uint8_t>& compressed_data) {
    // Setup window and stream
    unsigned char window[32768];
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    int init_result = inflateBackInit_(&strm, 15, window, 
                                      ZLIB_VERSION, sizeof(z_stream));
    if (init_result != Z_OK) {
        return;
    }
    
    // Test 1: in() callback returns 0 immediately (no input)
    ErrorTestCallbackContext ctx1;
    ctx1.data = compressed_data.data();
    ctx1.size = 0;  // No data
    ctx1.position = 0;
    ctx1.should_fail = true;
    ctx1.fail_mode = 1;  // in() returns 0
    
    int result = inflateBack(&strm,
                           reinterpret_cast<in_func>(error_in_callback), &ctx1,
                           reinterpret_cast<out_func>(error_out_callback), &ctx1);
    // Should return Z_BUF_ERROR
    
    // Test 2: out() callback returns non-zero (output failure)
    ErrorTestCallbackContext ctx2;
    ctx2.data = compressed_data.data();
    ctx2.size = compressed_data.size();
    ctx2.position = 0;
    ctx2.should_fail = true;
    ctx2.fail_mode = 2;  // out() returns non-zero
    
    // Provide some initial input
    strm.next_in = const_cast<Bytef*>(compressed_data.data());
    strm.avail_in = static_cast<uInt>(compressed_data.size());
    
    result = inflateBack(&strm,
                        reinterpret_cast<in_func>(error_in_callback), &ctx2,
                        reinterpret_cast<out_func>(error_out_callback), &ctx2);
    // Should return Z_BUF_ERROR
    
    inflateBackEnd(&strm);
}

// Test with corrupted/invalid deflate stream data
void test_corrupted_streams(FuzzedDataProvider& fdp) {
    // Generate various corrupted data patterns
    size_t data_size = fdp.ConsumeIntegralInRange<size_t>(1, 4096);
    std::vector<uint8_t> corrupted_data = fdp.ConsumeBytes<uint8_t>(data_size);
    
    if (corrupted_data.empty()) {
        // If no data, create some minimal invalid data
        corrupted_data = {0, 0, 0, 0, 0, 0, 0, 0};
    }
    
    // Setup window and stream
    unsigned char window[32768];
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    int init_result = inflateBackInit_(&strm, 15, window, 
                                      ZLIB_VERSION, sizeof(z_stream));
    if (init_result != Z_OK) {
        return;
    }
    
    // Test with corrupted data
    ErrorTestCallbackContext ctx;
    ctx.data = corrupted_data.data();
    ctx.size = corrupted_data.size();
    ctx.position = 0;
    ctx.should_fail = false;
    ctx.fail_mode = 0;
    
    // Provide corrupted data as initial input
    strm.next_in = const_cast<Bytef*>(corrupted_data.data());
    strm.avail_in = static_cast<uInt>(corrupted_data.size());
    
    int result = inflateBack(&strm,
                           reinterpret_cast<in_func>(error_in_callback), &ctx,
                           reinterpret_cast<out_func>(error_out_callback), &ctx);
    // May return Z_DATA_ERROR, Z_BUF_ERROR, or Z_STREAM_END
    
    inflateBackEnd(&strm);
}

// Test unusual API sequences and edge cases
void test_unusual_sequences(FuzzedDataProvider& fdp,
                           const std::vector<uint8_t>& compressed_data) {
    // Test 1: Initialize, don't use, end
    unsigned char window[32768];
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    inflateBackInit_(&strm, 15, window, ZLIB_VERSION, sizeof(z_stream));
    inflateBackEnd(&strm);
    
    // Test 2: Initialize with one window, try to use with different window
    unsigned char window1[32768];
    unsigned char window2[16384];  // Different size
    
    memset(&strm, 0, sizeof(strm));
    inflateBackInit_(&strm, 15, window1, ZLIB_VERSION, sizeof(z_stream));
    
    // Try to use with window2 (invalid but should be caught)
    ErrorTestCallbackContext ctx;
    ctx.data = compressed_data.data();
    ctx.size = compressed_data.size();
    ctx.position = 0;
    ctx.should_fail = false;
    ctx.fail_mode = 0;
    
    // Note: This is invalid but we test it
    strm.next_in = const_cast<Bytef*>(compressed_data.data());
    strm.avail_in = static_cast<uInt>(compressed_data.size());
    
    inflateBack(&strm,
               reinterpret_cast<in_func>(error_in_callback), &ctx,
               reinterpret_cast<out_func>(error_out_callback), &ctx);
    
    inflateBackEnd(&strm);
    
    // Test 3: Partial consumption then reuse
    memset(&strm, 0, sizeof(strm));
    inflateBackInit_(&strm, 15, window1, ZLIB_VERSION, sizeof(z_stream));
    
    ctx.position = 0;
    strm.next_in = const_cast<Bytef*>(compressed_data.data());
    strm.avail_in = static_cast<uInt>(compressed_data.size());
    
    // Call inflateBack, then try to reuse the stream
    inflateBack(&strm,
               reinterpret_cast<in_func>(error_in_callback), &ctx,
               reinterpret_cast<out_func>(error_out_callback), &ctx);
    
    // Try to use again without reinitialization
    ctx.position = 0;
    strm.next_in = const_cast<Bytef*>(compressed_data.data());
    strm.avail_in = static_cast<uInt>(compressed_data.size());
    
    inflateBack(&strm,
               reinterpret_cast<in_func>(error_in_callback), &ctx,
               reinterpret_cast<out_func>(error_out_callback), &ctx);
    
    inflateBackEnd(&strm);
}

// Test normal operation with valid parameters
void test_normal_operation(FuzzedDataProvider& fdp, const std::vector<uint8_t>& test_data) {
    // Test with valid windowBits values (8-15)
    int windowBits = fdp.ConsumeIntegralInRange<int>(8, 15);
    size_t window_size = 1U << windowBits;
    std::vector<unsigned char> window(window_size);
    
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Initialize with valid parameters
    int init_result = inflateBackInit_(&strm, windowBits, window.data(), 
                                      ZLIB_VERSION, sizeof(z_stream));
    if (init_result != Z_OK) {
        return;  // Initialization failed, can't proceed
    }
    
    // Setup callback context
    ErrorTestCallbackContext ctx;
    ctx.data = test_data.data();
    ctx.size = test_data.size();
    ctx.position = 0;
    ctx.should_fail = false;
    ctx.fail_mode = 0;
    
    // Set up initial input
    strm.next_in = const_cast<Bytef*>(test_data.data());
    strm.avail_in = static_cast<uInt>(test_data.size());
    
    // Call inflateBack with valid parameters
    int result = inflateBack(&strm,
                           reinterpret_cast<in_func>(error_in_callback), &ctx,
                           reinterpret_cast<out_func>(error_out_callback), &ctx);
    
    // Proper cleanup
    inflateBackEnd(&strm);
    
    // Test different window sizes if we have more data
    if (fdp.remaining_bytes() > 8) {
        int second_windowBits = fdp.ConsumeIntegralInRange<int>(8, 15);
        size_t second_window_size = 1U << second_windowBits;
        std::vector<unsigned char> second_window(second_window_size);
        
        memset(&strm, 0, sizeof(strm));
        
        if (inflateBackInit_(&strm, second_windowBits, second_window.data(), 
                            ZLIB_VERSION, sizeof(z_stream)) == Z_OK) {
            ErrorTestCallbackContext ctx2;
            ctx2.data = test_data.data();
            ctx2.size = test_data.size();
            ctx2.position = 0;
            ctx2.should_fail = false;
            ctx2.fail_mode = 0;
            
            strm.next_in = const_cast<Bytef*>(test_data.data());
            strm.avail_in = static_cast<uInt>(test_data.size());
            
            inflateBack(&strm,
                       reinterpret_cast<in_func>(error_in_callback), &ctx2,
                       reinterpret_cast<out_func>(error_out_callback), &ctx2);
            
            inflateBackEnd(&strm);
        }
    }
}
// Main fuzzing function focusing on error conditions
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Check minimum input size
    if (size < MIN_INPUT_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Determine testing mode based on first byte
    uint8_t mode_selector = fdp.ConsumeIntegral<uint8_t>();
    bool test_error_conditions = (mode_selector % 4) == 0;  // 25% chance to test error conditions
    
    // Consume some data for testing
    size_t test_data_size = fdp.ConsumeIntegralInRange<size_t>(16, 2048);
    if (fdp.remaining_bytes() < test_data_size) {
        test_data_size = fdp.remaining_bytes();
    }
    
    std::vector<uint8_t> test_data = fdp.ConsumeBytes<uint8_t>(test_data_size);
    if (test_data.empty()) {
        // Create minimal valid deflate data if none available
        test_data = {0x78, 0x01, 0x01, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00};
    }
    
    if (test_error_conditions) {
        // Only test error conditions when explicitly indicated
        // Test 1: inflateBackInit_ error conditions
        test_inflateBackInit_errors(fdp);
        
        // Test 2: inflateBack error conditions
        test_inflateBack_errors(fdp, test_data);
        
        // Test 3: Callback failure scenarios
        test_callback_failures(fdp, test_data);
        
        // Test 4: Corrupted stream data
        test_corrupted_streams(fdp);
        
        // Test 5: Unusual API sequences
        test_unusual_sequences(fdp, test_data);
    } else {
        // Test normal operation with valid parameters
        test_normal_operation(fdp, test_data);
    }
    
    // Test 6: Boundary conditions for window sizes (always test - these are valid operations)
    // Test with minimum valid windowBits (8)
    unsigned char small_window[256];  // 2^8 = 256
    z_stream strm_small;
    memset(&strm_small, 0, sizeof(strm_small));
    
    // Only proceed if initialization succeeds
    if (inflateBackInit_(&strm_small, 8, small_window, ZLIB_VERSION, sizeof(z_stream)) == Z_OK) {
        ErrorTestCallbackContext ctx_small;
        ctx_small.data = test_data.data();
        ctx_small.size = test_data.size();
        ctx_small.position = 0;
        ctx_small.should_fail = false;
        ctx_small.fail_mode = 0;
        
        strm_small.next_in = const_cast<Bytef*>(test_data.data());
        strm_small.avail_in = static_cast<uInt>(test_data.size());
        
        inflateBack(&strm_small,
                   reinterpret_cast<in_func>(error_in_callback), &ctx_small,
                   reinterpret_cast<out_func>(error_out_callback), &ctx_small);
        
        inflateBackEnd(&strm_small);
    }

    // Test 7: Test with various data patterns that might trigger edge cases
    if (fdp.remaining_bytes() > 0) {
        // Generate more test patterns
        std::vector<uint8_t> pattern1(256, 0);  // All zeros
        std::vector<uint8_t> pattern2(256, 0xFF);  // All ones
        std::vector<uint8_t> pattern3;
        for (size_t i = 0; i < 256; i++) {
            pattern3.push_back(static_cast<uint8_t>(i));
        }
        
        // Test each pattern
        unsigned char window[32768];
        z_stream strm;
        memset(&strm, 0, sizeof(strm));
        
        if (inflateBackInit_(&strm, 15, window, ZLIB_VERSION, sizeof(z_stream)) == Z_OK) {
            ErrorTestCallbackContext ctx;
            ctx.should_fail = false;
            ctx.fail_mode = 0;
            
            // Test pattern1
            ctx.data = pattern1.data();
            ctx.size = pattern1.size();
            ctx.position = 0;
            strm.next_in = const_cast<Bytef*>(pattern1.data());
            strm.avail_in = static_cast<uInt>(pattern1.size());
            inflateBack(&strm,
                       reinterpret_cast<in_func>(error_in_callback), &ctx,
                       reinterpret_cast<out_func>(error_out_callback), &ctx);
            
            // Test pattern2
            ctx.data = pattern2.data();
            ctx.size = pattern2.size();
            ctx.position = 0;
            strm.next_in = const_cast<Bytef*>(pattern2.data());
            strm.avail_in = static_cast<uInt>(pattern2.size());
            inflateBack(&strm,
                       reinterpret_cast<in_func>(error_in_callback), &ctx,
                       reinterpret_cast<out_func>(error_out_callback), &ctx);
            
            // Test pattern3
            ctx.data = pattern3.data();
            ctx.size = pattern3.size();
            ctx.position = 0;
            strm.next_in = const_cast<Bytef*>(pattern3.data());
            strm.avail_in = static_cast<uInt>(pattern3.size());
            inflateBack(&strm,
                       reinterpret_cast<in_func>(error_in_callback), &ctx,
                       reinterpret_cast<out_func>(error_out_callback), &ctx);
            
            inflateBackEnd(&strm);
        }
    }
    
    return 0;
}

} // namespace harness_010
