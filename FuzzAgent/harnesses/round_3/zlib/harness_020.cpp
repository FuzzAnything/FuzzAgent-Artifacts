#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include "zlib.h"

namespace harness_020 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check: need at least some bytes for fuzzing
    if (size < 4) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Strategy 1: Test zlibVersion() - returns compile-time version string
    const char* version = zlibVersion();
    // Version should not be NULL and should match ZLIB_VERSION
    // We can't assert in fuzzer, but we can use the result
    
    // Strategy 2: Test zlibCompileFlags() - returns compilation flags
    uLong flags = zlibCompileFlags();
    // The flags are a bitmask of compile-time options
    // We can test some properties of the flags
    
    // Check that certain bits are valid (0 or 1)
    // Bits 0-1: sizeof(uInt) (0,1,2,3 for 2,4,8,other bytes)
    uInt sizeof_uInt_bits = flags & 0x3;
    // Bits 2-3: sizeof(uLong) (0,1,2,3 for 2,4,8,other bytes)  
    uInt sizeof_uLong_bits = (flags >> 2) & 0x3;
    // Bits 4-5: sizeof(voidpf) (0,1,2,3 for 2,4,8,other bytes)
    uInt sizeof_voidpf_bits = (flags >> 4) & 0x3;
    // Bits 6-7: sizeof(z_off_t) (0,1,2,3 for 2,4,8,other bytes)
    uInt sizeof_z_off_t_bits = (flags >> 6) & 0x3;
    
    // Other flag bits are compile-time options
    // Bit 8: ZLIB_DEBUG
    // Bit 9: ASMV/ASMINF (commented out)
    // Bit 10: ZLIB_WINAPI
    // Bit 12: BUILDFIXED
    // Bit 13: DYNAMIC_CRC_TABLE
    // Bits 16-17: NO_GZCOMPRESS, NO_GZIP
    // Bit 20: PKZIP_BUG_WORKAROUND
    // Bit 21: FASTEST
    // Bits 24-27: STDC and printf variants
    
    // Strategy 3: Test zError() with various error codes
    // Consume error codes from fuzzer input
    // Valid error codes in zlib: Z_VERSION_ERROR (-6) to Z_NEED_DICT (2)
    // z_errmsg array has 10 entries indexed by 2 - err for err in [-6, 2]
    // For err outside [-6, 2], returns empty string (index 9)
    
    // Test with random error codes covering all possibilities
    int error_code = fdp.ConsumeIntegralInRange<int>(-10, 10);
    const char* error_msg = zError(error_code);
    
    // Test with specific boundary values to cover all z_errmsg array indices
    // Index 0: err = 2 (Z_NEED_DICT)
    const char* msg_need_dict = zError(2);
    // Index 1: err = 1 (Z_STREAM_END)  
    const char* msg_stream_end = zError(1);
    // Index 2: err = 0 (Z_OK)
    const char* msg_ok = zError(0);
    // Index 3: err = -1 (Z_ERRNO)
    const char* msg_errno = zError(-1);
    // Index 4: err = -2 (Z_STREAM_ERROR)
    const char* msg_stream_error = zError(-2);
    // Index 5: err = -3 (Z_DATA_ERROR)
    const char* msg_data_error = zError(-3);
    // Index 6: err = -4 (Z_MEM_ERROR)
    const char* msg_mem_error = zError(-4);
    // Index 7: err = -5 (Z_BUF_ERROR)
    const char* msg_buf_error = zError(-5);
    // Index 8: err = -6 (Z_VERSION_ERROR)
    const char* msg_version_error = zError(-6);
    // Index 9: err < -6 or err > 2 (empty string)
    const char* msg_invalid_low = zError(-7);
    const char* msg_invalid_high = zError(3);
    
    // Strategy 4: Test memory utility functions through zlib APIs
    // The memory functions (zmemcpy, zmemcmp, zmemzero) are internal
    // but zcalloc and zcfree are used as default allocators.
    // We can test these by performing compression/decompression
    // with various data sizes to trigger different allocation paths.
    
    if (fdp.remaining_bytes() > 100) {
        // Create test data for compression
        size_t test_data_size = fdp.ConsumeIntegralInRange<size_t>(10, 100);
        std::vector<uint8_t> test_data = fdp.ConsumeBytes<uint8_t>(test_data_size);
        
        if (test_data.size() > 0) {
            // Calculate compression bound
            uLongf compr_bound = compressBound(test_data.size());
            if (compr_bound > 0 && compr_bound < 10 * 1024 * 1024) {
                // Allocate buffers - these will use system malloc/free
                // but zlib internal allocations will use zcalloc/zcfree
                Bytef* compr = (Bytef*)malloc(compr_bound);
                Bytef* uncompr = (Bytef*)malloc(test_data.size());
                
                if (compr && uncompr) {
                    // Test compress2 with random level and strategy
                    int level = fdp.ConsumeIntegralInRange<int>(0, 9);
                    int strategy = fdp.ConsumeIntegralInRange<int>(0, 4);
                    uLongf compr_len = compr_bound;
                    
                    int compress_ret = compress2(compr, &compr_len, 
                                                test_data.data(), test_data.size(), 
                                                level);
                    
                    if (compress_ret == Z_OK) {
                        // Test uncompress
                        uLongf uncompr_len = test_data.size();
                        int uncompress_ret = uncompress(uncompr, &uncompr_len, 
                                                       compr, compr_len);
                        
                        // If successful, data should match original size
                        if (uncompress_ret == Z_OK) {
                            // Data integrity check (optional)
                        }
                    }
                    
                    // Test with deflate/inflate to use z_stream allocators
                    z_stream strm;
                    strm.zalloc = Z_NULL;  // Use default zcalloc
                    strm.zfree = Z_NULL;   // Use default zcfree
                    strm.opaque = Z_NULL;
                    
                    // Initialize deflate with random parameters
                    int windowBits = fdp.ConsumeIntegralInRange<int>(8, 15);
                    int memLevel = fdp.ConsumeIntegralInRange<int>(1, 9);
                    
                    int init_ret = deflateInit2(&strm, level, Z_DEFLATED, 
                                               windowBits, memLevel, strategy);
                    
                    if (init_ret == Z_OK) {
                        // Setup input/output
                        strm.next_in = test_data.data();
                        strm.avail_in = test_data.size();
                        strm.next_out = compr;
                        strm.avail_out = compr_bound;
                        
                        // Perform compression
                        int deflate_ret = deflate(&strm, Z_FINISH);
                        
                        // Clean up - this will call zcfree for any internal allocations
                        deflateEnd(&strm);
                        
                        // If we got compressed data, try to inflate it
                        if (strm.total_out > 0) {
                            z_stream inflate_strm;
                            inflate_strm.zalloc = Z_NULL;
                            inflate_strm.zfree = Z_NULL;
                            inflate_strm.opaque = Z_NULL;
                            inflate_strm.next_in = compr;
                            inflate_strm.avail_in = strm.total_out;
                            inflate_strm.next_out = uncompr;
                            inflate_strm.avail_out = test_data.size();
                            
                            int inflate_init_ret = inflateInit2(&inflate_strm, windowBits);
                            if (inflate_init_ret == Z_OK) {
                                int inflate_ret = inflate(&inflate_strm, Z_FINISH);
                                inflateEnd(&inflate_strm);
                            }
                        }
                    }
                    
                    free(compr);
                    free(uncompr);
                }
            }
        }
    }
    
    // Strategy 5: Test custom allocators to directly exercise zcalloc/zcfree
    // We can provide our own allocator functions that wrap zcalloc/zcfree
    // or simulate allocation patterns
    
    if (fdp.remaining_bytes() > 20) {
        // Create a simple test with custom allocator that records calls
        // This doesn't directly call zcalloc/zcfree but tests the allocator
        // interface which internally uses them when Z_NULL is specified
        z_stream custom_strm;
        custom_strm.zalloc = Z_NULL;  // Will use zcalloc internally
        custom_strm.zfree = Z_NULL;   // Will use zcfree internally
        custom_strm.opaque = Z_NULL;
        
        // Just initialize and immediately end to test allocator setup/teardown
        int windowBits = 15;  // Default
        int memLevel = 8;     // Default
        int level = Z_DEFAULT_COMPRESSION;
        
        int custom_init_ret = deflateInit2(&custom_strm, level, Z_DEFLATED,
                                          windowBits, memLevel, Z_DEFAULT_STRATEGY);
        
        if (custom_init_ret == Z_OK) {
            // Immediately end to clean up - will call zcfree if any allocations made
            deflateEnd(&custom_strm);
        }
    }
    
    // Strategy 6: Test ERR_RETURN macro usage indirectly
    // ERR_RETURN is used internally in zlib. We can trigger error conditions
    // that cause functions to use ERR_RETURN. For example, passing invalid
    // parameters to deflate/inflate functions.
    
    if (fdp.remaining_bytes() > 10) {
        // Create a z_stream with invalid state to trigger error paths
        z_stream invalid_strm;
        memset(&invalid_strm, 0, sizeof(invalid_strm));
        
        // Try to call deflate with invalid stream (should fail)
        // This may trigger ERR_RETURN internally
        int invalid_ret = deflate(&invalid_strm, Z_NO_FLUSH);
        
        // Try inflate with invalid stream
        int invalid_inflate_ret = inflate(&invalid_strm, Z_NO_FLUSH);
        
        // Initialize then immediately corrupt state
        z_stream corrupt_strm;
        corrupt_strm.zalloc = Z_NULL;
        corrupt_strm.zfree = Z_NULL;
        corrupt_strm.opaque = Z_NULL;
        
        if (deflateInit(&corrupt_strm, Z_DEFAULT_COMPRESSION) == Z_OK) {
            // Corrupt the state pointer after saving it for cleanup
            struct internal_state *saved_state = corrupt_strm.state;
            corrupt_strm.state = NULL;
            // Try to use corrupted stream (should fail with error)
            int corrupt_ret = deflate(&corrupt_strm, Z_NO_FLUSH);
            // Restore state pointer for cleanup
            corrupt_strm.state = saved_state;
            // Clean up - always call deflateEnd after successful init
            deflateEnd(&corrupt_strm);
        }
    }
    
    return 0;
}

} // namespace harness_020
