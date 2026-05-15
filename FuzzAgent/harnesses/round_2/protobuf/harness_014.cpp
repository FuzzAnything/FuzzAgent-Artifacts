#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <utility>
#include "google/protobuf/io/zero_copy_stream_impl_lite.h"
#include "google/protobuf/io/coded_stream.h"

namespace harness_014 {
// Harness for Google Protocol Buffers CodedInputStream recursion depth management APIs
// Targets CodedInputStream recursion depth management as the most critical coverage gap identified by coverage analysis
// Recursion depth management APIs have 15+ points of undiscovered complexity and are currently 0% covered
// APIs: IncrementRecursionDepthAndPushLimit, DecrementRecursionDepthAndPopLimit, UnsafeDecrementRecursionDepth
// Semantic differentiation from existing harnesses 000-013:
// - harness_000: binary parsing and serialization of message objects (high-level)
// - harness_001: text format parsing and printing with Tokenizer
// - harness_002: Cord-based I/O streaming with compression (CordOutputStream/InputStream)
// - harness_003: SymbolChecker visibility rules validation
// - harness_004: JSON parsing/serialization for web/REST APIs
// - harness_005: Cord I/O operations with CodedInputStream/OutputStream and stream backup functions
// - harness_006: File-based I/O operations with Gzip compression
// - harness_007: CodedStream abstraction layer for wire format handling with Cord read/write operations
// - harness_008: Modern absl::Cord integration focusing on zero-copy string handling with EpsCopyOutputStream
// - harness_009: CodedOutputStream static ToArray methods with Cord support
// - harness_010: ZeroCopyInputStream composition patterns (ConcatenatingInputStream, LimitingInputStream)
// - harness_011: EpsCopyOutputStream::WriteString with Cord parameter
// - harness_012: COMPREHENSIVE CodedInputStream low-level stream parsing operations (includes some recursion testing but not comprehensive)
// - harness_013: Arena alignment utilities and allocation policies
// - harness_014: FOCUSED EXCLUSIVELY on CodedInputStream recursion depth management APIs with proper lifecycle sequencing
//   Tests: IncrementRecursionDepthAndPushLimit, DecrementRecursionDepthAndPopLimit, UnsafeDecrementRecursionDepth,
//          IncrementRecursionDepth, DecrementRecursionDepth, recursion budget tracking, nested limit scenarios


// Include protobuf headers for CodedInputStream

using google::protobuf::io::ArrayInputStream;
using google::protobuf::io::CodedInputStream;
// Limit is a typedef inside CodedInputStream class
void TestBasicRecursionDepthSequencing(CodedInputStream& coded_input, FuzzedDataProvider& fdp) {
    // Set initial recursion limit from fuzzer input
    int recursion_limit = fdp.ConsumeIntegralInRange<int>(1, 10);
    coded_input.SetRecursionLimit(recursion_limit);
    
    // Test basic IncrementRecursionDepth sequence
    int depth = 0;
    while (depth < recursion_limit * 2) {  // Try to exceed limit
        bool success = coded_input.IncrementRecursionDepth();
        if (!success) {
            // Should fail when depth >= recursion_limit
            break;
        }
        depth++;
    }
    
    // Decrement back down
    while (depth > 0) {
        coded_input.DecrementRecursionDepth();
        depth--;
    }
    
    // Test UnsafeDecrementRecursionDepth - only call when we know recursion_budget_ < recursion_limit_
    // After incrementing successfully, we can use UnsafeDecrementRecursionDepth
    if (coded_input.IncrementRecursionDepth()) {
        coded_input.UnsafeDecrementRecursionDepth();
    }
}

void TestIncrementRecursionDepthAndPushLimitScenarios(CodedInputStream& coded_input, FuzzedDataProvider& fdp) {
    int bytes_available = coded_input.BytesUntilLimit();
    if (bytes_available <= 0) return;
    
    // Generate byte_limit within available range
    int byte_limit = fdp.ConsumeIntegralInRange<int>(1, bytes_available);
    
    // Call IncrementRecursionDepthAndPushLimit
    auto result = coded_input.IncrementRecursionDepthAndPushLimit(byte_limit);
    CodedInputStream::Limit limit = result.first;
    int recursion_budget = result.second;
    
    // Check if recursion was successful (budget >= 0)
    if (recursion_budget >= 0) {
        // Recursion succeeded, we can read within the limit
        
        // Read some data within the pushed limit
        if (coded_input.BytesUntilLimit() > 0) {
            size_t read_size = fdp.ConsumeIntegralInRange<size_t>(
                1, std::min<size_t>(coded_input.BytesUntilLimit(), 256));
            std::vector<uint8_t> buffer(read_size);
            bool read_success = coded_input.ReadRaw(buffer.data(), read_size);
            // Result doesn't matter for coverage - both success and failure paths are valuable
        }
        
        // Now test DecrementRecursionDepthAndPopLimit
        bool entire_message_consumed = coded_input.DecrementRecursionDepthAndPopLimit(limit);
        // Result indicates whether entire message was consumed
        
    } else {
        // Recursion failed (budget negative), need to clean up
        // PushLimit was called but recursion failed
        
        // Pop the limit without decrementing recursion (since it didn't increment)
        coded_input.PopLimit(limit);
        
        // Note: recursion_budget is negative, but we shouldn't call UnsafeDecrementRecursionDepth
        // because IncrementRecursionDepthAndPushLimit didn't actually increment the depth
        // when budget went negative
    }
}

void TestNestedRecursionLimits(CodedInputStream& coded_input, FuzzedDataProvider& fdp) {
    // Test nested recursion with multiple limits
    int max_nesting = fdp.ConsumeIntegralInRange<int>(1, 5);
    std::vector<std::pair<CodedInputStream::Limit, int>> nested_limits;
    for (int i = 0; i < max_nesting; i++) {
        int bytes_available = coded_input.BytesUntilLimit();
        if (bytes_available <= 0) break;
        
        int byte_limit = fdp.ConsumeIntegralInRange<int>(1, bytes_available);
        auto result = coded_input.IncrementRecursionDepthAndPushLimit(byte_limit);
        
        if (result.second < 0) {
            // Recursion failed at this level, clean up and stop nesting
            coded_input.PopLimit(result.first);
            break;
        }
        
        nested_limits.push_back(result);
        
        // Read some data at this nesting level
        if (coded_input.BytesUntilLimit() > 0) {
            size_t read_size = fdp.ConsumeIntegralInRange<size_t>(
                1, std::min<size_t>(coded_input.BytesUntilLimit(), 128));
            std::vector<uint8_t> buffer(read_size);
            coded_input.ReadRaw(buffer.data(), read_size);
        }
    }
    
    // Pop limits in reverse order (LIFO)
    for (auto it = nested_limits.rbegin(); it != nested_limits.rend(); ++it) {
        bool entire_message_consumed = coded_input.DecrementRecursionDepthAndPopLimit(it->first);
        // Check result but continue regardless
    }
}

void TestMixedRecursionOperations(CodedInputStream& coded_input, FuzzedDataProvider& fdp) {
    // Test mixing different recursion operations
    
    // First set a reasonable recursion limit
    coded_input.SetRecursionLimit(fdp.ConsumeIntegralInRange<int>(3, 8));
    
    // Mix IncrementRecursionDepth with IncrementRecursionDepthAndPushLimit
    bool use_simple = fdp.ConsumeBool();
    if (use_simple) {
        // Use simple increment
        if (coded_input.IncrementRecursionDepth()) {
            // Do some operations
            if (coded_input.BytesUntilLimit() > 0) {
                size_t read_size = fdp.ConsumeIntegralInRange<size_t>(
                    1, std::min<size_t>(coded_input.BytesUntilLimit(), 64));
                std::vector<uint8_t> buffer(read_size);
                coded_input.ReadRaw(buffer.data(), read_size);
            }
            
            // Use UnsafeDecrementRecursionDepth when we know increment succeeded
            coded_input.UnsafeDecrementRecursionDepth();
        }
    } else {
        // Use combined operation
        int bytes_available = coded_input.BytesUntilLimit();
        if (bytes_available > 0) {
            int byte_limit = fdp.ConsumeIntegralInRange<int>(1, bytes_available);
            auto result = coded_input.IncrementRecursionDepthAndPushLimit(byte_limit);
            
            if (result.second >= 0) {
                // Read within limit
                if (coded_input.BytesUntilLimit() > 0) {
                    size_t read_size = fdp.ConsumeIntegralInRange<size_t>(
                        1, std::min<size_t>(coded_input.BytesUntilLimit(), 64));
                    std::vector<uint8_t> buffer(read_size);
                    coded_input.ReadRaw(buffer.data(), read_size);
                }
                
                // Test CheckEntireMessageConsumedAndPopLimit as alternative
                if (fdp.ConsumeBool()) {
                    bool consumed = coded_input.CheckEntireMessageConsumedAndPopLimit(result.first);
                    // After CheckEntireMessageConsumedAndPopLimit, need to decrement recursion manually
                    coded_input.UnsafeDecrementRecursionDepth();
                } else {
                    bool consumed = coded_input.DecrementRecursionDepthAndPopLimit(result.first);
                }
            } else {
                // Clean up failed recursion
                coded_input.PopLimit(result.first);
            }
        }
    }
}

void TestEdgeCaseScenarios(CodedInputStream& coded_input, FuzzedDataProvider& fdp) {
    // Test edge cases
    
    // 1. Byte limit of 0
    auto result_zero = coded_input.IncrementRecursionDepthAndPushLimit(0);
    if (result_zero.second >= 0) {
        bool consumed = coded_input.DecrementRecursionDepthAndPopLimit(result_zero.first);
    } else {
        coded_input.PopLimit(result_zero.first);
    }
    
    // 2. Byte limit negative (should behave like 0 based on PushLimit implementation)
    auto result_neg = coded_input.IncrementRecursionDepthAndPushLimit(-1);
    if (result_neg.second >= 0) {
        bool consumed = coded_input.DecrementRecursionDepthAndPopLimit(result_neg.first);
    } else {
        coded_input.PopLimit(result_neg.first);
    }
    
    // 3. Byte limit larger than available
    int large_limit = coded_input.BytesUntilLimit() + 1000;
    auto result_large = coded_input.IncrementRecursionDepthAndPushLimit(large_limit);
    if (result_large.second >= 0) {
        bool consumed = coded_input.DecrementRecursionDepthAndPopLimit(result_large.first);
    } else {
        coded_input.PopLimit(result_large.first);
    }
    
    // 4. UnsafeDecrementRecursionDepth call
    // First increment successfully
    if (coded_input.IncrementRecursionDepth()) {
        coded_input.UnsafeDecrementRecursionDepth();
        // Only call UnsafeDecrementRecursionDepth once after successful increment
        // Additional calls would violate recursion_budget_ < recursion_limit_ assertion
    }
}
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for meaningful recursion testing
    if (size < 16) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Create input data for CodedInputStream
    // Use part of the fuzzer input as stream data
    // Fix: Ensure min <= max for ConsumeIntegralInRange by using std::min(32, size) as lower bound
    size_t stream_data_size = fdp.ConsumeIntegralInRange<size_t>(std::min<size_t>(32, size), std::min<size_t>(size, 4096));
    std::vector<uint8_t> stream_data = fdp.ConsumeBytes<uint8_t>(stream_data_size);
    if (stream_data.empty()) {
        // Fallback if not enough data
        stream_data = fdp.ConsumeRemainingBytes<uint8_t>();
        if (stream_data.empty()) return 0;
    }
    
    // Create ArrayInputStream with the data
    ArrayInputStream array_input(stream_data.data(), stream_data.size());
    CodedInputStream coded_input(&array_input);
    
    // Consume test scenario selector
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 5;
    
    switch (scenario) {
        case 0:
            TestBasicRecursionDepthSequencing(coded_input, fdp);
            break;
        case 1:
            TestIncrementRecursionDepthAndPushLimitScenarios(coded_input, fdp);
            break;
        case 2:
            TestNestedRecursionLimits(coded_input, fdp);
            break;
        case 3:
            TestMixedRecursionOperations(coded_input, fdp);
            break;
        case 4:
            TestEdgeCaseScenarios(coded_input, fdp);
            break;
    }
    
    return 0;
}

} // namespace harness_014
