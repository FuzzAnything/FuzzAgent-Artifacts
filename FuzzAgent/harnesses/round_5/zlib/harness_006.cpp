#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "zlib.h"
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>
#include <string>

namespace harness_006 {
/*
 * Fuzzing harness for zlib advanced inflate control operations
 * Targets: inflateCopy, inflateSync, inflatePrime, inflateMark, inflateSyncPoint,
 *          inflateValidate, inflateGetHeader, inflateUndermine, inflateCodesUsed (9 uncovered APIs)
 * Helper APIs: inflateInit2_, inflate, inflateEnd
 * 
 * This harness exercises the complete advanced inflate control workflow:
 * 1. Initialize with inflateInit2_ (create decompression stream with advanced options)
 * 2. Basic operation with inflate (perform initial decompression to establish state)
 * 3. State duplication with inflateCopy (duplicate decompression state)
 * 4. Synchronization with inflateSync (attempt to find sync points in corrupted data)
 * 5. Bit manipulation with inflatePrime (insert bits into bit buffer)
 * 6. State checkpoint with inflateMark (get/restore checkpoint position)
 * 7. Sync point checking with inflateSyncPoint (check for synchronization points)
 * 8. Validation with inflateValidate (validate stream integrity)
 * 9. Header retrieval with inflateGetHeader (retrieve gzip header information)
 * 10. Undermining with inflateUndermine (test undermine functionality)
 * 11. Code usage tracking with inflateCodesUsed (get code usage statistics)
 * 12. Cleanup with inflateEnd (properly terminate decompression)
 * 
 * This targets 9 currently uncovered public APIs with 0% coverage:
 * - inflateCopy: 16 undiscovered branches
 * - inflateSync: 16 undiscovered branches  
 * - inflatePrime: 10 undiscovered branches
 * - inflateMark: 6 undiscovered branches
 * - inflateSyncPoint: 6 undiscovered branches
 * - inflateValidate: 6 undiscovered branches
 * - inflateGetHeader: 4 undiscovered branches
 * - inflateUndermine: 2 undiscovered branches
 * - inflateCodesUsed: 2 undiscovered branches
 * 
 * Total: 68+ accumulated branches across inflate.c (currently 49.51% branch coverage)
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for meaningful fuzzing
    // Need enough for parameters and compressed data
    if (size < 128) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size parameters first
    int window_bits = fdp.ConsumeIntegralInRange<int>(8, 15);
    // For gzip header support, add 16 to window_bits
    bool use_gzip = fdp.ConsumeBool();
    if (use_gzip) {
        window_bits += 16;
    }
    
    // For raw deflate (no header), use negative window_bits
    bool use_raw = fdp.ConsumeBool();
    if (use_raw) {
        window_bits = -window_bits;
    }
    
    int flush_mode = fdp.ConsumeIntegralInRange<int>(0, 5); // Z_NO_FLUSH to Z_BLOCK
    int bits = fdp.ConsumeIntegralInRange<int>(-16, 16); // inflatePrime bits parameter
    int value = fdp.ConsumeIntegral<int>();
    int check = fdp.ConsumeIntegralInRange<int>(0, 2); // inflateValidate check parameter
    int undermine = fdp.ConsumeIntegralInRange<int>(0, 1); // inflateUndermine parameter
    
    // Consume input data for decompression
    // Use some of the input as "compressed" data (even if not valid)
    size_t compressed_size = fdp.ConsumeIntegralInRange<size_t>(1, 4096);
    if (compressed_size > fdp.remaining_bytes()) {
        compressed_size = fdp.remaining_bytes();
    }
    
    std::vector<uint8_t> compressed_data = fdp.ConsumeBytes<uint8_t>(compressed_size);
    
    // Initialize first zlib stream for decompression
    z_stream strm1;
    memset(&strm1, 0, sizeof(strm1));
    
    // Initialize with inflateInit2_
    int ret = inflateInit2_(&strm1, window_bits, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        return 0; // Initialization failed
    }
    
    // Set up input/output buffers
    strm1.next_in = compressed_data.data();
    strm1.avail_in = static_cast<uInt>(compressed_size);
    
    // Allocate output buffer
    size_t output_size = 8192; // Reasonable buffer size
    std::vector<uint8_t> output_buffer(output_size);
    strm1.next_out = output_buffer.data();
    strm1.avail_out = static_cast<uInt>(output_size);
    
    // Perform initial decompression with inflate to establish state
    ret = inflate(&strm1, Z_NO_FLUSH);
    // Don't check return value - we want to test error paths too
    
    // 1. Test inflateCopy - duplicate decompression state
    z_stream strm2;
    memset(&strm2, 0, sizeof(strm2));
    
    ret = inflateCopy(&strm2, &strm1);
    if (ret == Z_OK) {
        // If copy succeeded, we can perform operations on the copy
        // Set up output buffer for copy
        std::vector<uint8_t> output_buffer2(output_size);
        strm2.next_out = output_buffer2.data();
        strm2.avail_out = static_cast<uInt>(output_size);
        
        // Continue decompression with the copy
        inflate(&strm2, Z_FINISH);
        
        // Clean up the copy
        inflateEnd(&strm2);
    }
    
    // 2. Test inflateSync - attempt to find sync points
    // Reset input for sync testing
    strm1.next_in = compressed_data.data();
    strm1.avail_in = static_cast<uInt>(compressed_size);
    strm1.next_out = output_buffer.data();
    strm1.avail_out = static_cast<uInt>(output_size);
    
    ret = inflateSync(&strm1);
    // Return value doesn't matter for fuzzing
    
    // 3. Test inflatePrime - insert bits into bit buffer
    ret = inflatePrime(&strm1, bits, value);
    // Return value doesn't matter for fuzzing
    
    // 4. Test inflateMark - get checkpoint position
    long mark_result = inflateMark(&strm1);
    // Return value doesn't matter for fuzzing
    
    // 5. Test inflateSyncPoint - check for sync points
    ret = inflateSyncPoint(&strm1);
    // Return value doesn't matter for fuzzing
    
    // 6. Test inflateValidate - validate stream
    ret = inflateValidate(&strm1, check);
    // Return value doesn't matter for fuzzing
    
    // 7. Test inflateGetHeader - retrieve gzip header
    if (window_bits > 15) { // gzip mode (window_bits + 16)
        gz_header header;
        memset(&header, 0, sizeof(header));
        
        // Allocate space for header fields
        char extra_buf[1024];
        char name_buf[1024];
        char comment_buf[1024];
        
        header.extra = (Bytef*)extra_buf;
        header.extra_len = 0;
        header.extra_max = sizeof(extra_buf);
        header.name = (Bytef*)name_buf;
        header.name_max = sizeof(name_buf);
        header.comment = (Bytef*)comment_buf;
        header.comm_max = sizeof(comment_buf);
        header.hcrc = 0;
        header.done = 0;
        
        ret = inflateGetHeader(&strm1, &header);
        // Return value doesn't matter for fuzzing
    }
    
    // 8. Test inflateUndermine - test undermine functionality
    ret = inflateUndermine(&strm1, undermine);
    // Return value doesn't matter for fuzzing
    
    // 9. Test inflateCodesUsed - get code usage statistics
    unsigned long codes_used = inflateCodesUsed(&strm1);
    // Return value doesn't matter for fuzzing
    
    // Continue decompression with any remaining input
    strm1.next_in = compressed_data.data();
    strm1.avail_in = static_cast<uInt>(compressed_size);
    strm1.next_out = output_buffer.data();
    strm1.avail_out = static_cast<uInt>(output_size);
    
    inflate(&strm1, Z_FINISH);
    
    // Clean up original stream
    inflateEnd(&strm1);
    
    return 0;
}

} // namespace harness_006
