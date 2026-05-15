#include <cstdint>
#include <cstddef>
#include <cstring>
#include <vector>
#include <string>
#include <memory>
#include "zlib.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_017 {
/* Advanced fuzzing harness targeting inflate function's 478 blocked branches (75.9% uncovered)
 * Primary target: State machine transitions and coverage of specific blocked branches
 * Focus: Systematic testing of inflate state machine transitions with various flush modes,
 *        windowBits configurations, and partial/incomplete data scenarios
 * 
 * Critical coverage gaps identified:
 * - inflate function has 478 blocked branches (75.9% uncovered)
 * - State transitions between different modes: HEAD, FLAGS, TIME, OS, EXLEN, EXTRA, NAME, COMMENT, HCRC
 * - State machine paths: TYPE, TYPEDO, STORED, TABLE, LEN_, DIST, CODELEN, LEN, DIST, MATCH
 * - Interaction between flush modes and state transitions
 * - Partial data handling and intermediate states
 * 
 * This harness specifically targets:
 * 1. State machine transitions through all inflate modes
 * 2. Different flush modes: Z_NO_FLUSH, Z_SYNC_FLUSH, Z_FULL_FLUSH, Z_FINISH, Z_BLOCK
 * 3. WindowBits configurations with different wrap modes (0, 1, 2, 4 bits)
 * 4. Partial/incomplete data to test intermediate state handling
 * 5. State transitions with boundary conditions and edge cases
 * 6. Combinations of valid and invalid data to explore state machine paths
 * 
 * Distinct from existing harnesses:
 * - harness_013: Generates valid gzip data with varied headers for decompression
 * - harness_014: Directly tests inflate() header processing with crafted headers
 * - harness_015: Tests multiple entry points (uncompress* and gz* APIs) to inflate()
 * - harness_016: Focuses on ERROR CONDITIONS and edge cases in state machine
 * - harness_017: Focuses on STATE MACHINE TRANSITIONS and coverage of specific blocked branches (this harness)
 * 
 * Implementation strategy:
 * - Test all inflate modes with systematic state transitions
 * - Use different flush modes to control state machine behavior
 * - Test with partial data to stay in intermediate states
 * - Test windowBits configurations for different wrap modes
 * - Target specific blocked branches through controlled state transitions
 * - Explore combinations of valid/invalid data to maximize branch coverage
 */


const size_t MIN_INPUT_SIZE = 32;
// Maximum buffer sizes to prevent excessive memory usage
const size_t MAX_COMPRESSED_SIZE = 1024 * 1024;  // 1MB
const size_t MAX_UNCOMPRESSED_SIZE = 2 * 1024 * 1024;  // 2MB
const size_t MAX_WINDOW_SIZE = 32768;  // 32KB (2^15, max window size)

// Inflate state modes (from inflate.h)
enum InflateMode {
    MODE_HEAD = 1,          /* i: waiting for magic header */
    MODE_FLAGS,             /* i: waiting for method and flags (gzip) */
    MODE_TIME,              /* i: waiting for modification time (gzip) */
    MODE_OS,                /* i: waiting for extra flags and operating system (gzip) */
    MODE_EXLEN,             /* i: waiting for extra length (gzip) */
    MODE_EXTRA,             /* i: waiting for extra bytes (gzip) */
    MODE_NAME,              /* i: waiting for end of file name (gzip) */
    MODE_COMMENT,           /* i: waiting for end of comment (gzip) */
    MODE_HCRC,              /* i: waiting for header crc (gzip) */
    MODE_DICTID,            /* i: waiting for dictionary check value */
    MODE_DICT,              /* waiting for inflateSetDictionary() call */
    MODE_TYPE,              /* i: waiting for type bits, including last-flag bit */
    MODE_TYPEDO,            /* i: same, but skip check to exit inflate on new block */
    MODE_STORED,            /* i: waiting for stored size (length and complement) */
    MODE_COPY,              /* i/o: waiting for input or output to copy stored block */
    MODE_TABLE,             /* i: waiting for dynamic block table lengths */
    MODE_LENLENS,           /* i: waiting for code length code lengths */
    MODE_CODELENS,          /* i: waiting for length/lit and distance code lengths */
    MODE_LEN_,              /* i: waiting for length/lit code */
    MODE_LENEXT,            /* i: waiting for length extra bits */
    MODE_DIST,              /* i: waiting for distance code */
    MODE_DISTEXT,           /* i: waiting for distance extra bits */
    MODE_MATCH,             /* o: waiting for output space to copy string */
    MODE_LIT,               /* o: waiting for output space to write literal */
    MODE_CHECK,             /* i: waiting for 32-bit check value */
    MODE_LENGTH,            /* i: waiting for 32-bit length (gzip) */
    MODE_DONE,              /* finished check, done -- remain here until reset */
    MODE_BAD,               /* got a data error -- remain here until reset */
    MODE_MEM,               /* got an inflate() memory error -- remain here until reset */
    MODE_SYNC               /* looking for synchronization bytes to restart inflate() */
};

// Helper to generate compressed data for state machine testing
std::vector<uint8_t> generate_compressed_data_for_state_machine(FuzzedDataProvider& fdp, 
                                                               int windowBits,
                                                               bool is_gzip) {
    // Consume some data to compress
    size_t input_size = fdp.ConsumeIntegralInRange<size_t>(1, 4096);
    std::vector<uint8_t> input_data = fdp.ConsumeBytes<uint8_t>(input_size);
    
    if (input_data.empty()) {
        return std::vector<uint8_t>();
    }
    
    // Initialize deflate stream
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Use windowBits + 16 for gzip format if requested
    int actual_windowBits = windowBits;
    if (is_gzip) {
        actual_windowBits = windowBits + 16;
    }
    
    int ret = deflateInit2(&strm, 
                          fdp.ConsumeIntegralInRange<int>(0, 9),  // compression level
                          Z_DEFLATED,
                          actual_windowBits,
                          8,        // memLevel
                          Z_DEFAULT_STRATEGY);
    
    if (ret != Z_OK) {
        return std::vector<uint8_t>();
    }
    
    // Calculate maximum compressed size
    uLongf compressed_bound = compressBound(input_data.size());
    if (compressed_bound > MAX_COMPRESSED_SIZE) {
        deflateEnd(&strm);
        return std::vector<uint8_t>();
    }
    
    // Allocate buffer for compressed data
    std::vector<uint8_t> compressed(compressed_bound);
    
    // Set up input/output
    strm.next_in = const_cast<Bytef*>(input_data.data());
    strm.avail_in = input_data.size();
    strm.next_out = compressed.data();
    strm.avail_out = compressed.size();
    
    // Compress with Z_FINISH to complete the stream
    ret = deflate(&strm, Z_FINISH);
    
    std::vector<uint8_t> result;
    if (ret == Z_STREAM_END) {
        // Success - resize to actual compressed size
        result.resize(compressed.size() - strm.avail_out);
        memcpy(result.data(), compressed.data(), result.size());
    }
    
    deflateEnd(&strm);
    return result;
}

// Test state machine transitions with different flush modes
void test_state_machine_transitions(FuzzedDataProvider& fdp, 
                                   const std::vector<uint8_t>& compressed_data,
                                   int windowBits,
                                   int flush_mode) {
    if (compressed_data.empty()) {
        return;
    }
    
    // Initialize inflate stream
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    int ret = inflateInit2(&strm, windowBits);
    if (ret != Z_OK) {
        return;
    }
    
    // Allocate output buffer
    size_t output_size = compressed_data.size() * 4;
    if (output_size > MAX_UNCOMPRESSED_SIZE) {
        output_size = MAX_UNCOMPRESSED_SIZE;
    }
    
    std::vector<uint8_t> decompressed(output_size);
    
    // Set up input/output
    strm.next_in = const_cast<Bytef*>(compressed_data.data());
    strm.avail_in = compressed_data.size();
    strm.next_out = decompressed.data();
    strm.avail_out = decompressed.size();
    
    // Inflate with specified flush mode
    ret = inflate(&strm, flush_mode);
    
    // Check various return codes that indicate state transitions
    switch (ret) {
        case Z_OK:
            // Normal case - more data needed or more output space needed
            break;
        case Z_STREAM_END:
            // End of stream reached
            break;
        case Z_NEED_DICT:
            // Dictionary needed
            break;
        case Z_BUF_ERROR:
            // No progress possible (buffer error)
            break;
        case Z_DATA_ERROR:
            // Data error (invalid compressed data)
            break;
        case Z_MEM_ERROR:
            // Memory error
            break;
        case Z_STREAM_ERROR:
            // Stream error
            break;
    }
    
    inflateEnd(&strm);
}

// Test partial data handling for intermediate states
void test_partial_data_state_machine(FuzzedDataProvider& fdp,
                                    const std::vector<uint8_t>& compressed_data,
                                    int windowBits) {
    if (compressed_data.size() < 2) {
        return;
    }
    
    // Initialize inflate stream
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    int ret = inflateInit2(&strm, windowBits);
    if (ret != Z_OK) {
        return;
    }
    
    // Allocate output buffer
    size_t output_size = compressed_data.size() * 4;
    if (output_size > MAX_UNCOMPRESSED_SIZE) {
        output_size = MAX_UNCOMPRESSED_SIZE;
    }
    
    std::vector<uint8_t> decompressed(output_size);
    
    // Test with partial data to stay in intermediate states
    size_t partial_size = fdp.ConsumeIntegralInRange<size_t>(1, compressed_data.size());
    
    // Process data in multiple chunks to test state transitions
    size_t processed = 0;
    while (processed < partial_size) {
        size_t chunk_size = fdp.ConsumeIntegralInRange<size_t>(1, partial_size - processed);
        if (chunk_size == 0) break;
        
        strm.next_in = const_cast<Bytef*>(compressed_data.data() + processed);
        strm.avail_in = chunk_size;
        strm.next_out = decompressed.data();
        strm.avail_out = decompressed.size();
        
        // Use different flush modes based on fuzzer input
        int flush_mode = fdp.ConsumeIntegralInRange<int>(0, 4);
        int actual_flush;
        switch (flush_mode) {
            case 0: actual_flush = Z_NO_FLUSH; break;
            case 1: actual_flush = Z_SYNC_FLUSH; break;
            case 2: actual_flush = Z_FULL_FLUSH; break;
            case 3: actual_flush = Z_FINISH; break;
            case 4: actual_flush = Z_BLOCK; break;
            default: actual_flush = Z_NO_FLUSH; break;
        }
        
        ret = inflate(&strm, actual_flush);
        
        processed += chunk_size;
        
        // Break on error or stream end
        if (ret == Z_STREAM_END || ret == Z_STREAM_ERROR || 
            ret == Z_DATA_ERROR || ret == Z_MEM_ERROR) {
            break;
        }
    }
    
    inflateEnd(&strm);
}

// Test windowBits configurations and wrap modes
void test_windowbits_configurations(FuzzedDataProvider& fdp,
                                   const std::vector<uint8_t>& compressed_data) {
    if (compressed_data.empty()) {
        return;
    }
    
    // Test different windowBits configurations
    int windowBits_config = fdp.ConsumeIntegralInRange<int>(0, 4);
    int windowBits_value;
    
    switch (windowBits_config) {
        case 0:  // Raw inflate (no wrapper)
            windowBits_value = -15;  // Raw with 32K window
            break;
        case 1:  // zlib wrapper
            windowBits_value = 15;   // zlib with 32K window
            break;
        case 2:  // gzip wrapper
            windowBits_value = 31;   // gzip auto-detection (15 + 16)
            break;
        case 3:  // gzip with validation
            windowBits_value = 47;   // gzip with validation (15 + 16 + 16)
            break;
        case 4:  // Different window sizes
            windowBits_value = fdp.ConsumeIntegralInRange<int>(8, 15);
            break;
        default:
            windowBits_value = 15;
            break;
    }
    
    // Test state machine with this configuration
    test_state_machine_transitions(fdp, compressed_data, windowBits_value, Z_NO_FLUSH);
}

// Main fuzzer entry point
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < MIN_INPUT_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume configuration parameters
    int test_scenario = fdp.ConsumeIntegralInRange<int>(0, 3);
    int windowBits = fdp.ConsumeIntegralInRange<int>(8, 15);
    bool is_gzip = fdp.ConsumeBool();
    int flush_mode_select = fdp.ConsumeIntegralInRange<int>(0, 4);
    
    int flush_mode;
    switch (flush_mode_select) {
        case 0: flush_mode = Z_NO_FLUSH; break;
        case 1: flush_mode = Z_SYNC_FLUSH; break;
        case 2: flush_mode = Z_FULL_FLUSH; break;
        case 3: flush_mode = Z_FINISH; break;
        case 4: flush_mode = Z_BLOCK; break;
        default: flush_mode = Z_NO_FLUSH; break;
    }
    
    // Generate compressed data for testing
    std::vector<uint8_t> compressed_data = 
        generate_compressed_data_for_state_machine(fdp, windowBits, is_gzip);
    
    if (compressed_data.empty()) {
        // If we can't generate valid compressed data, try testing with raw fuzzer input
        // This helps test error paths and invalid data handling
        std::vector<uint8_t> raw_data = fdp.ConsumeRemainingBytes<uint8_t>();
        
        // Test different windowBits configurations with raw data
        test_windowbits_configurations(fdp, raw_data);
        
        // Test state machine transitions with raw data
        test_state_machine_transitions(fdp, raw_data, windowBits, flush_mode);
        
        // Test partial data handling with raw data
        test_partial_data_state_machine(fdp, raw_data, windowBits);
        
        return 0;
    }
    
    // Execute test based on scenario
    switch (test_scenario) {
        case 0:
            // Test state machine transitions with different flush modes
            test_state_machine_transitions(fdp, compressed_data, 
                                          is_gzip ? windowBits + 16 : windowBits, 
                                          flush_mode);
            break;
            
        case 1:
            // Test partial data handling for intermediate states
            test_partial_data_state_machine(fdp, compressed_data, 
                                           is_gzip ? windowBits + 16 : windowBits);
            break;
            
        case 2:
            // Test windowBits configurations
            test_windowbits_configurations(fdp, compressed_data);
            break;
            
        case 3:
            // Comprehensive test: all scenarios
            test_state_machine_transitions(fdp, compressed_data, 
                                          is_gzip ? windowBits + 16 : windowBits, 
                                          flush_mode);
            test_partial_data_state_machine(fdp, compressed_data, 
                                           is_gzip ? windowBits + 16 : windowBits);
            test_windowbits_configurations(fdp, compressed_data);
            break;
    }
    
    return 0;
}

} // namespace harness_017
