#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include "google/protobuf/io/zero_copy_stream.h"
#include "google/protobuf/io/zero_copy_stream_impl.h"
#include "google/protobuf/io/zero_copy_stream_impl_lite.h"
#include "absl/strings/cord.h"

namespace harness_015 {
// Protocol Buffers fuzzing harness for zero-copy stream operations
// This harness targets uncovered zero-copy stream APIs identified as having 55+ undiscovered branches
// Specifically targets: ConcatenatingInputStream::Skip, IstreamInputStream::Skip, 
// ConcatenatingInputStream::BackUp, CordInputStream::BackUp, LimitingInputStream::ReadCord,
// LimitingInputStream::Skip, LimitingInputStream::Next



using google::protobuf::io::ZeroCopyInputStream;
using google::protobuf::io::ArrayInputStream;
using google::protobuf::io::IstreamInputStream;
using google::protobuf::io::ConcatenatingInputStream;
using google::protobuf::io::LimitingInputStream;
using google::protobuf::io::CordInputStream;

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful stream operations
    if (size < 256) return 0;  // Need enough for multiple streams and parameters
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Create test data for streams
    size_t test_data_size = fdp.ConsumeIntegralInRange<size_t>(128, 4096);
    std::vector<uint8_t> test_data = fdp.ConsumeBytes<uint8_t>(test_data_size);
    
    if (test_data.size() < 64) return 0;  // Need sufficient data
    
    // Convert to string for some stream types
    std::string test_string(reinterpret_cast<const char*>(test_data.data()), 
                           std::min(test_data.size(), (size_t)1024));
    
    // Step 2: Create various input streams to test
    
    // 2.1 ArrayInputStream (base stream for other wrappers)
    ArrayInputStream array_stream1(test_data.data(), test_data.size(), 64);
    ArrayInputStream array_stream2(test_data.data(), test_data.size(), 128);
    
    // 2.2 IstreamInputStream (wrapped around std::istringstream)
    std::istringstream iss(test_string);
    IstreamInputStream istream_stream(&iss);
    
    // 2.3 ConcatenatingInputStream (sequence of multiple streams)
    {
        // Create array of streams for concatenation
        ZeroCopyInputStream* streams[] = {&array_stream1, &array_stream2};
        ConcatenatingInputStream concat_stream(streams, 2);
        
        // Test ConcatenatingInputStream::Skip (primary target)
        const void* buffer;
        int buffer_size;
        if (concat_stream.Next(&buffer, &buffer_size) && buffer_size > 0) {
            // Test Skip on current buffer
            int skip_amount = fdp.ConsumeIntegralInRange<int>(0, buffer_size);
            concat_stream.Skip(skip_amount);  // Target API: ConcatenatingInputStream::Skip
        }
        
        // Test ConcatenatingInputStream::BackUp (primary target)
        // Need to call Next again before BackUp
        if (concat_stream.Next(&buffer, &buffer_size) && buffer_size > 0) {
            int backup_amount = fdp.ConsumeIntegralInRange<int>(0, buffer_size);
            if (backup_amount >= 0) {
                concat_stream.BackUp(backup_amount);  // Target API: ConcatenatingInputStream::BackUp
            }
        }
        
        // Continue reading to test stream switching
        if (concat_stream.Next(&buffer, &buffer_size)) {
            // Additional Skip test
            int skip2 = fdp.ConsumeIntegralInRange<int>(0, buffer_size);
            concat_stream.Skip(skip2);
        }
        
        // Test ByteCount
        int64_t bytes_read = concat_stream.ByteCount();
        (void)bytes_read;  // Use to avoid unused variable warning
    }
    
    // 2.4 CordInputStream (stream from absl::Cord)
    {
        // Create Cord from test data
        absl::Cord test_cord = absl::Cord(test_string);
        CordInputStream cord_stream(&test_cord);
        
        // Test CordInputStream::Next and Skip (primary target)
        const void* cord_buffer;
        int cord_buffer_size;
        if (cord_stream.Next(&cord_buffer, &cord_buffer_size) && cord_buffer_size > 0) {
            // Test CordInputStream::Skip
            int cord_skip = fdp.ConsumeIntegralInRange<int>(0, cord_buffer_size);
            cord_stream.Skip(cord_skip);
        }
        
        // Test CordInputStream::BackUp (primary target) - must be after Next()
        if (cord_stream.Next(&cord_buffer, &cord_buffer_size) && cord_buffer_size > 0) {
            int cord_backup = fdp.ConsumeIntegralInRange<int>(0, cord_buffer_size);
            if (cord_backup >= 0) {
                cord_stream.BackUp(cord_backup);  // Target API: CordInputStream::BackUp
            }
        }
        
        // Test CordInputStream::ReadCord (if more data available)
        if (cord_stream.Next(&cord_buffer, &cord_buffer_size) && cord_buffer_size > 0) {
            absl::Cord read_cord;
            int read_count = fdp.ConsumeIntegralInRange<int>(0, cord_buffer_size);
            cord_stream.ReadCord(&read_cord, read_count);
        }
        
        // Test ByteCount
        int64_t cord_bytes = cord_stream.ByteCount();
        (void)cord_bytes;
    }
    
    // 2.5 LimitingInputStream (stream with byte limit)
    {
        // Create base stream
        ArrayInputStream base_stream(test_data.data(), test_data.size());
        
        // Determine limit from fuzzer input
        int64_t limit = fdp.ConsumeIntegralInRange<int64_t>(0, test_data.size());
        LimitingInputStream limiting_stream(&base_stream, limit);
        
        // Test LimitingInputStream::Next and Skip (primary targets)
        const void* limit_buffer;
        int limit_buffer_size;
        if (limiting_stream.Next(&limit_buffer, &limit_buffer_size) && limit_buffer_size > 0) {
            // Test LimitingInputStream::Skip (primary target)
            int limit_skip = fdp.ConsumeIntegralInRange<int>(0, limit_buffer_size);
            limiting_stream.Skip(limit_skip);  // Target API: LimitingInputStream::Skip
        }
        
        // Test LimitingInputStream::BackUp - must be after Next()
        if (limiting_stream.Next(&limit_buffer, &limit_buffer_size) && limit_buffer_size > 0) {
            int limit_backup = fdp.ConsumeIntegralInRange<int>(0, limit_buffer_size);
            if (limit_backup >= 0) {
                limiting_stream.BackUp(limit_backup);
            }
        }
        
        // Test LimitingInputStream::ReadCord (primary target)
        if (limiting_stream.Next(&limit_buffer, &limit_buffer_size) && limit_buffer_size > 0) {
            absl::Cord limit_cord;
            int read_cord_count = fdp.ConsumeIntegralInRange<int>(0, limit_buffer_size);
            limiting_stream.ReadCord(&limit_cord, read_cord_count);  // Target API: LimitingInputStream::ReadCord
        }
        
        // Additional Skip test after operations
        if (limiting_stream.Next(&limit_buffer, &limit_buffer_size) && limit_buffer_size > 0) {
            int additional_skip = fdp.ConsumeIntegralInRange<int>(0, limit_buffer_size);
            limiting_stream.Skip(additional_skip);
        }
        
        // Test ByteCount
        int64_t limit_bytes = limiting_stream.ByteCount();
        (void)limit_bytes;
    }
    
    // Step 3: Test IstreamInputStream operations specifically
    {
        // Create new istream for fresh testing
        std::istringstream iss2(test_string);
        IstreamInputStream istream_stream2(&iss2);
        
        // Test IstreamInputStream::Skip (primary target)
        const void* istream_buffer;
        int istream_buffer_size;
        if (istream_stream2.Next(&istream_buffer, &istream_buffer_size) && istream_buffer_size > 0) {
            // Test Skip on current buffer
            int istream_skip = fdp.ConsumeIntegralInRange<int>(0, istream_buffer_size);
            istream_stream2.Skip(istream_skip);  // Target API: IstreamInputStream::Skip
        }
        
        // Test IstreamInputStream::BackUp - must be after Next()
        if (istream_stream2.Next(&istream_buffer, &istream_buffer_size) && istream_buffer_size > 0) {
            int istream_backup = fdp.ConsumeIntegralInRange<int>(0, istream_buffer_size);
            if (istream_backup >= 0) {
                istream_stream2.BackUp(istream_backup);
            }
        }
        
        // Continue reading to test more operations
        if (istream_stream2.Next(&istream_buffer, &istream_buffer_size) && istream_buffer_size > 0) {
            // Additional Skip test
            int skip2 = fdp.ConsumeIntegralInRange<int>(0, istream_buffer_size);
            istream_stream2.Skip(skip2);
            
            // Try to read to end with multiple Next calls
            while (istream_stream2.Next(&istream_buffer, &istream_buffer_size) && istream_buffer_size > 0) {
                // Consume small amounts with Skip
                int small_skip = fdp.ConsumeIntegralInRange<int>(0, std::min(istream_buffer_size, 10));
                istream_stream2.Skip(small_skip);
            }
        }
        
        // Test ByteCount
        int64_t istream_bytes = istream_stream2.ByteCount();
        (void)istream_bytes;
    }
    
    // Step 4: Test edge cases and complex scenarios
    
    // 4.1 Test multiple Skip operations in sequence
    {
        ArrayInputStream edge_stream(test_data.data(), test_data.size());
        const void* edge_buffer;
        int edge_buffer_size;
        
        int total_skipped = 0;
        const int max_skips = 10;
        
        for (int i = 0; i < max_skips; i++) {
            if (edge_stream.Next(&edge_buffer, &edge_buffer_size) && edge_buffer_size > 0) {
                int skip_amount = fdp.ConsumeIntegralInRange<int>(0, edge_buffer_size);
                edge_stream.Skip(skip_amount);
                total_skipped += skip_amount;
                
                // For BackUp, we need to call Next() first
                if (fdp.ConsumeBool() && skip_amount > 0) {
                    // Get new buffer before BackUp
                    if (edge_stream.Next(&edge_buffer, &edge_buffer_size) && edge_buffer_size > 0) {
                        int backup_amount = fdp.ConsumeIntegralInRange<int>(0, edge_buffer_size);
                        edge_stream.BackUp(backup_amount);
                        total_skipped -= backup_amount;
                    }
                }
            } else {
                break;  // No more data
            }
        }
    }
    
    // 4.2 Test zero-byte operations
    {
        ArrayInputStream zero_stream(test_data.data(), test_data.size());
        
        // Test zero-byte operations with proper API sequencing
        const void* zero_buffer;
        int zero_buffer_size;
        if (zero_stream.Next(&zero_buffer, &zero_buffer_size) && zero_buffer_size > 0) {
            // Test zero-byte Skip (valid operation)
            zero_stream.Skip(0);
            
            // For BackUp, we need to call Next() again first
            if (zero_stream.Next(&zero_buffer, &zero_buffer_size) && zero_buffer_size > 0) {
                // Test zero-byte BackUp (valid operation)
                zero_stream.BackUp(0);
            }
        }
    }
    
    // 4.3 Test with very small data
    if (test_data.size() >= 16) {
        std::vector<uint8_t> small_data(test_data.begin(), test_data.begin() + 16);
        ArrayInputStream small_stream(small_data.data(), small_data.size());
        
        const void* small_buffer;
        int small_buffer_size;
        if (small_stream.Next(&small_buffer, &small_buffer_size) && small_buffer_size > 0) {
            // Exercise Skip operation on small buffer
            small_stream.Skip(fdp.ConsumeIntegralInRange<int>(0, small_buffer_size));
            
            // Get new buffer before BackUp
            if (small_stream.Next(&small_buffer, &small_buffer_size) && small_buffer_size > 0) {
                small_stream.BackUp(fdp.ConsumeIntegralInRange<int>(0, small_buffer_size));
            }
        }
    }
    
    return 0;
}

} // namespace harness_015
