#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <fuzzer/FuzzedDataProvider.h>
#include "google/protobuf/io/zero_copy_stream.h"
#include "google/protobuf/io/zero_copy_stream_impl.h"
#include "google/protobuf/io/zero_copy_stream_impl_lite.h"
#include "google/protobuf/io/zero_copy_sink.h"
#include "absl/strings/cord.h"
#include "absl/strings/string_view.h"

namespace harness_016 {
// Protocol Buffers fuzzing harness for ZeroCopyStreamByteSink and related streaming APIs
// This harness targets the ZeroCopyStreamByteSink API cluster identified as having
// 16 undiscovered branches and low API coverage (27.99%).
// Focuses on protobuf I/O streaming operations with ZeroCopyStreamByteSink wrapper.
// Differentiates from existing harnesses by targeting the sink abstraction layer
// rather than raw stream operations, exploring error paths, boundary conditions,
// and complex streaming scenarios.



using google::protobuf::io::ZeroCopyOutputStream;
using google::protobuf::io::ArrayOutputStream;
using google::protobuf::io::StringOutputStream;
using google::protobuf::io::CordOutputStream;
using google::protobuf::io::FileOutputStream;
using google::protobuf::io::zc_sink_internal::ZeroCopyStreamByteSink;

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful sink operations
    // We need enough for: stream parameters + test data + operation selection
    if (size < 32) return 0;  // Minimum size for basic operations
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Determine operation mode from fuzzer input
    uint8_t operation_mode = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Step 2: Create appropriate ZeroCopyOutputStream based on mode
    std::unique_ptr<ZeroCopyOutputStream> stream;
    std::string temp_filename;  // For FileOutputStream case
    std::vector<char> buffer;   // For ArrayOutputStream case - declared outside switch to extend lifetime
    
    switch (operation_mode) {
        case 0: {
            // ArrayOutputStream with dynamic buffer size
            size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(64, 4096);
            buffer.resize(buffer_size);  // Resize the existing vector
            stream = std::make_unique<ArrayOutputStream>(buffer.data(), buffer_size);
            break;
        }
        case 1: {
            // StringOutputStream appending to string
            std::string output_string;
            stream = std::make_unique<StringOutputStream>(&output_string);
            break;
        }
        case 2: {
            // CordOutputStream - use default constructor (no parameters)
            stream = std::make_unique<CordOutputStream>();
            break;
        }
        case 3: {
            // FileOutputStream (if we can create temp file)
            char temp_template[] = "/tmp/protobuf_fuzz_sink_XXXXXX";
            int temp_fd = mkstemp(temp_template);
            if (temp_fd != -1) {
                // Use reasonable block size
                int block_size = fdp.ConsumeIntegralInRange<int>(64, 2048);
                stream = std::make_unique<FileOutputStream>(temp_fd, block_size);
                temp_filename = temp_template;  // Store for cleanup
                // FileOutputStream takes ownership of the file descriptor
            } else {
                // Fall back to ArrayOutputStream if file creation fails
                size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(64, 1024);
                buffer.resize(buffer_size);  // Resize the existing vector
                stream = std::make_unique<ArrayOutputStream>(buffer.data(), buffer_size);
            }
            break;
        }
    }
    
    if (!stream) {
        return 0;  // Stream creation failed
    }
    
    // Step 3: Create ZeroCopyStreamByteSink wrapper
    std::unique_ptr<ZeroCopyStreamByteSink> sink = std::make_unique<ZeroCopyStreamByteSink>(stream.get());
    
    // Step 4: Generate test data for Append/Write operations
    size_t total_test_data = fdp.ConsumeIntegralInRange<size_t>(1, 8192);
    std::vector<char> test_data_vec = fdp.ConsumeBytes<char>(total_test_data);
    
    if (test_data_vec.empty()) {
        // Use remaining bytes if ConsumeBytes returned empty
        test_data_vec = fdp.ConsumeRemainingBytes<char>();
        if (test_data_vec.empty()) {
            return 0;  // No test data available
        }
    }
    
    const char* test_data = test_data_vec.data();
    size_t test_data_size = test_data_vec.size();
    
    // Step 5: Test various Append/Write patterns
    
    // Pattern 1: Single large Append
    if (test_data_size > 0) {
        size_t append_size = fdp.ConsumeIntegralInRange<size_t>(1, test_data_size);
        sink->Append(test_data, append_size);
        
        // Check bytes_written after Append
        size_t bytes_written = sink->bytes_written();
        bool failed = sink->failed();
        (void)bytes_written;  // Use to avoid unused variable warning
        (void)failed;
    }
    
    // Pattern 2: Multiple small Appends (simulates streaming)
    if (test_data_size > 16) {
        size_t remaining = test_data_size;
        const char* ptr = test_data;
        size_t max_chunk_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
        
        while (remaining > 0 && !sink->failed()) {
            size_t chunk_size = std::min(remaining, max_chunk_size);
            sink->Append(ptr, chunk_size);
            ptr += chunk_size;
            remaining -= chunk_size;
            
            // Occasionally check bytes_written during streaming
            if (fdp.ConsumeBool()) {
                sink->bytes_written();
            }
        }
    }
    
    // Pattern 3: Write with absl::string_view
    if (test_data_size > 0 && !sink->failed()) {
        // Create string_view from remaining or subset of test data
        size_t write_size = fdp.ConsumeIntegralInRange<size_t>(1, std::min(test_data_size, (size_t)1024));
        absl::string_view str_view(test_data, write_size);
        sink->Write(str_view);
    }
    
    // Pattern 4: Test boundary conditions
    
    // 4.1: Empty Append (zero length)
    if (!sink->failed() && fdp.ConsumeBool()) {
        sink->Append(test_data, 0);
    }
    
    // 4.2: Append with size larger than available buffer
    if (!sink->failed() && test_data_size > 0) {
        // Try appending a very large size (might trigger buffer management)
        size_t large_size = test_data_size * 2;  // Intentional overflow case
        sink->Append(test_data, std::min(large_size, test_data_size));
    }
    
    // 4.3: Interleaved Append and bytes_written checks
    if (!sink->failed() && test_data_size > 8) {
        sink->Append(test_data, 4);
        size_t after_first = sink->bytes_written();
        bool failed_after_first = sink->failed();
        
        sink->Append(test_data + 4, 4);
        size_t after_second = sink->bytes_written();
        
        (void)after_first;
        (void)failed_after_first;
        (void)after_second;
    }
    
    // Pattern 5: Test failure recovery (simulate stream failure)
    // ZeroCopyStreamByteSink doesn't have explicit failure injection,
    // but we can test failed() state after operations
    
    // Final checks before sink destruction
    size_t final_bytes_written = sink->bytes_written();
    bool final_failed_state = sink->failed();
    
    (void)final_bytes_written;
    (void)final_failed_state;
    
    // Step 6: Test sink destruction and stream finalization
    // The sink destructor will call BackUp on remaining buffer
    // This is tested implicitly when sink goes out of scope
    
    // Step 7: Do NOT manipulate stream directly while sink is active
    // The ZeroCopyStreamByteSink manages the stream buffer internally
    // Direct calls to stream->Next() and stream->BackUp() while sink is active
    // violate the API contract and cause CHECK failures in ArrayOutputStream::BackUp()
    // Removed problematic code that was causing buffer management conflicts
    
    // Step 8: Test with different buffer sizes and edge cases
    
    // Create a second sink with different parameters if we have enough data
    if (fdp.remaining_bytes() > 16) {
        size_t second_buffer_size = fdp.ConsumeIntegralInRange<size_t>(16, 512);
        std::vector<char> second_buffer(second_buffer_size);
        ArrayOutputStream second_stream(second_buffer.data(), second_buffer_size);
        
        ZeroCopyStreamByteSink second_sink(&second_stream);
        
        // Test Append with exactly buffer size
        if (second_buffer_size > 0) {
            // Create test data for second sink
            std::vector<char> second_test_data = fdp.ConsumeBytes<char>(second_buffer_size);
            if (!second_test_data.empty()) {
                second_sink.Append(second_test_data.data(), second_test_data.size());
                
                // Test Write method with string_view
                if (!second_sink.failed() && second_test_data.size() > 4) {
                    absl::string_view second_str_view(second_test_data.data(), 4);
                    second_sink.Write(second_str_view);
                }
                
                // Check second sink state
                size_t second_bytes = second_sink.bytes_written();
                bool second_failed = second_sink.failed();
                (void)second_bytes;
                (void)second_failed;
            }
        }
    }
    
    // Step 9: Cleanup for FileOutputStream case
    if (!temp_filename.empty()) {
        // FileOutputStream should have closed the file, but remove temp file
        unlink(temp_filename.c_str());
    }
    
    // Sink and stream will be automatically destroyed here
    // Destructors will clean up resources
    
    return 0;  // Always return 0
}

} // namespace harness_016
