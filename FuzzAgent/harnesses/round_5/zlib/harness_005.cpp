#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "zlib.h"
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>
#include <string>

namespace harness_005 {
/*
 * Fuzzing harness for zlib advanced deflate control operations
 * Targets: deflateBound, deflateBound_z, deflateCopy, deflatePrime, deflatePending,
 *          deflateSetHeader, deflateUsed, deflateTune (8 uncovered APIs)
 * Helper APIs: deflateInit2_, deflate, deflateSetDictionary, deflateParams, deflateEnd
 * 
 * This harness exercises the complete advanced deflate control workflow:
 * 1. Initialize with deflateInit2_ (create compression stream with advanced options)
 * 2. Basic operation with deflate (perform initial compression)
 * 3. Bound checking with deflateBound/deflateBound_z (calculate maximum output size)
 * 4. State management with deflateCopy (duplicate compression state for experimentation)
 * 5. Bit manipulation with deflatePrime (insert bits into bit buffer)
 * 6. Output monitoring with deflatePending (check for pending uncompressed output)
 * 7. Header control with deflateSetHeader (set gzip header for wrapped streams)
 * 8. Input tracking with deflateUsed (check how much input has been consumed)
 * 9. Parameter tuning with deflateTune (adjust internal compression parameters)
 * 10. Dictionary management with deflateSetDictionary (set preset dictionary)
 * 11. Parameter adjustment with deflateParams (change compression parameters mid-stream)
 * 12. Cleanup with deflateEnd (properly terminate compression)
 * 
 * This targets 8 currently uncovered public APIs with 0% coverage:
 * - deflateBound: 50 undiscovered branches
 * - deflateBound_z: 48 undiscovered branches  
 * - deflateCopy: 24 undiscovered branches
 * - deflatePrime: 12 undiscovered branches
 * - deflatePending: 8 undiscovered branches
 * - deflateSetHeader: 4 undiscovered branches
 * - deflateUsed: 4 undiscovered branches
 * - deflateTune: 2 undiscovered branches
 * 
 * Total: 160+ accumulated branches across deflate.c (currently 55.73% branch coverage)
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for meaningful fuzzing
    // Need enough for parameters, data, and dictionary
    if (size < 128) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size parameters first
    uint8_t window_bits = fdp.ConsumeIntegralInRange<int>(8, 15);
    uint8_t mem_level = fdp.ConsumeIntegralInRange<int>(1, 9);
    int strategy = fdp.ConsumeIntegralInRange<int>(0, 4); // Z_DEFAULT_STRATEGY to Z_FIXED
    int flush_mode = fdp.ConsumeIntegralInRange<int>(0, 5); // Z_NO_FLUSH to Z_BLOCK
    uint8_t good_length = fdp.ConsumeIntegral<uint8_t>();
    uint8_t max_lazy = fdp.ConsumeIntegral<uint8_t>();
    uint8_t nice_length = fdp.ConsumeIntegral<uint8_t>();
    uint8_t max_chain = fdp.ConsumeIntegral<uint8_t>();
    int bits = fdp.ConsumeIntegralInRange<int>(1, 16);
    int value = fdp.ConsumeIntegral<int>();
    int level = fdp.ConsumeIntegralInRange<int>(-1, 9); // Z_DEFAULT_COMPRESSION to 9
    unsigned long source_len = fdp.ConsumeIntegralInRange<unsigned long>(1, 65536);
    
    // Consume input data for compression
    size_t input_size = fdp.ConsumeIntegralInRange<size_t>(1, 4096);
    if (input_size > fdp.remaining_bytes()) {
        input_size = fdp.remaining_bytes();
    }
    
    std::vector<uint8_t> input_data = fdp.ConsumeBytes<uint8_t>(input_size);
    
    // Consume dictionary data
    size_t dict_size = fdp.ConsumeIntegralInRange<size_t>(0, 32768);
    if (dict_size > fdp.remaining_bytes()) {
        dict_size = fdp.remaining_bytes();
    }
    
    std::vector<uint8_t> dict_data = fdp.ConsumeBytes<uint8_t>(dict_size);
    
    // Initialize first zlib stream
    z_stream strm1;
    memset(&strm1, 0, sizeof(strm1));
    
    // Initialize with deflateInit2_
    int ret = deflateInit2_(&strm1, level, Z_DEFLATED, window_bits, mem_level, strategy,
                           ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        return 0; // Initialization failed
    }
    
    // Set up input/output buffers
    strm1.next_in = input_data.data();
    strm1.avail_in = static_cast<uInt>(input_size);
    
    // Allocate output buffer based on deflateBound estimate
    unsigned long bound_estimate = deflateBound(&strm1, source_len);
    z_size_t bound_estimate_z = deflateBound_z(&strm1, static_cast<z_size_t>(source_len));
    
    // Use the larger of the two estimates for safety
    size_t output_size = static_cast<size_t>(bound_estimate > bound_estimate_z ? 
                                            bound_estimate : bound_estimate_z);
    if (output_size < 4096) output_size = 4096; // Minimum buffer
    
    std::vector<uint8_t> output_buffer(output_size);
    strm1.next_out = output_buffer.data();
    strm1.avail_out = static_cast<uInt>(output_size);
    
    // Perform initial compression with deflate
    ret = deflate(&strm1, flush_mode);
    // Don't check return value - we want to test error paths too
    
    // Check pending output with deflatePending
    unsigned pending;
    int bits_pending;
    deflatePending(&strm1, &pending, &bits_pending);
    
    // Check input consumption with deflateUsed
    int bits_used;
    deflateUsed(&strm1, &bits_used);
    
    // Tune compression parameters with deflateTune
    deflateTune(&strm1, good_length, max_lazy, nice_length, max_chain);
    
    // Set gzip header if using gzip format (window_bits + 16)
    if (window_bits > 0) {
        gz_header header;
        memset(&header, 0, sizeof(header));
        header.text = fdp.ConsumeBool();
        header.time = fdp.ConsumeIntegral<uInt>();
        header.xflags = fdp.ConsumeIntegral<uint8_t>();
        header.os = fdp.ConsumeIntegral<uint8_t>();
        header.extra = nullptr;
        header.extra_len = 0;
        header.extra_max = 0;
        header.name = nullptr;
        header.name_max = 0;
        header.comment = nullptr;
        header.comm_max = 0;
        header.hcrc = fdp.ConsumeBool();
        header.done = fdp.ConsumeBool();
        
        deflateSetHeader(&strm1, &header);
    }
    
    // Set dictionary if we have dictionary data
    if (dict_size > 0) {
        deflateSetDictionary(&strm1, dict_data.data(), static_cast<uInt>(dict_size));
    }
    
    // Adjust compression parameters with deflateParams
    deflateParams(&strm1, level, strategy);
    
    // Insert bits into bit buffer with deflatePrime
    deflatePrime(&strm1, bits, value);
    
    // Create a copy of the compression state with deflateCopy
    z_stream strm2;
    memset(&strm2, 0, sizeof(strm2));
    
    ret = deflateCopy(&strm2, &strm1);
    if (ret == Z_OK) {
        // If copy succeeded, we can perform operations on the copy
        // Set up output buffer for copy
        std::vector<uint8_t> output_buffer2(output_size);
        strm2.next_out = output_buffer2.data();
        strm2.avail_out = static_cast<uInt>(output_size);
        
        // Continue compression with the copy
        deflate(&strm2, Z_FINISH);
        
        // Clean up the copy
        deflateEnd(&strm2);
    }
    
    // Finish compression with original stream
    deflate(&strm1, Z_FINISH);
    
    // Clean up original stream
    deflateEnd(&strm1);
    
    return 0;
}

} // namespace harness_005
