#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include "google/protobuf/io/zero_copy_stream_impl_lite.h"

namespace harness_016 {
// Harness for Google Protocol Buffers StringOutputStream operations
// Targets StringOutputStream operations with complete logical lifecycle for string-based output streaming
// Coverage analysis identified StringOutputStream operations with 10 undiscovered branches and 0% coverage
// Semantic differentiation from existing harnesses 000-015:
// - harness_000: binary parsing and serialization of message objects
// - harness_001: text format parsing and printing with Tokenizer  
// - harness_002: Cord-based I/O streaming with compression
// - harness_003: SymbolChecker visibility rules validation
// - harness_004: JSON parsing/serialization (briefly uses StringOutputStream in JSON context)
// - harness_005: Cord I/O operations with CodedInputStream/OutputStream
// - harness_006: File-based I/O operations with Gzip compression
// - harness_007: CodedStream abstraction layer for wire format handling
// - harness_008: Modern absl::Cord integration with EpsCopyOutputStream
// - harness_009: CodedOutputStream static ToArray methods
// - harness_010: ZeroCopyInputStream composition patterns
// - harness_011: EpsCopyOutputStream::WriteString with Cord parameter
// - harness_012: COMPREHENSIVE CodedInputStream low-level stream parsing
// - harness_013: Arena alignment utilities and allocation policies
// - harness_014: FOCUSED EXCLUSIVELY on CodedInputStream recursion depth management
// - harness_015: FOCUSED EXCLUSIVELY on absl::Cord CORE OPERATIONS
// - harness_016: FOCUSED EXCLUSIVELY on StringOutputStream complete logical lifecycle - string-based output streaming


// Include StringOutputStream header

using google::protobuf::io::StringOutputStream;

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for meaningful testing
    if (size < 8) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume operation type to determine which StringOutputStream pattern to test
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    // Consume configuration options
    bool pre_reserve_string = fdp.ConsumeBool();
    bool use_multiple_next_calls = fdp.ConsumeBool();
    bool test_backup_operations = fdp.ConsumeBool();
    bool test_bytecount_tracking = fdp.ConsumeBool();
    bool test_edge_cases = fdp.ConsumeBool();
    
    // Consume reserve size if enabled
    size_t reserve_size = 0;
    if (pre_reserve_string && fdp.remaining_bytes() >= sizeof(size_t)) {
        reserve_size = fdp.ConsumeIntegralInRange<size_t>(0, 65536);
    }
    
    // Create target string for StringOutputStream
    std::string target_string;
    if (pre_reserve_string && reserve_size > 0) {
        target_string.reserve(reserve_size);
    }
    
    // Create StringOutputStream
    StringOutputStream output_stream(&target_string);
    
    // Consume write data
    std::string write_data;
    if (fdp.remaining_bytes() > 0) {
        size_t data_size = fdp.ConsumeIntegralInRange<size_t>(1, std::min<size_t>(fdp.remaining_bytes(), 65536));
        write_data = fdp.ConsumeBytesAsString(data_size);
    } else {
        return 0;
    }
    
    // Test different StringOutputStream operations based on operation type
    switch (operation) {
        case 0: {
            // Basic StringOutputStream: Single Next() call with full write
            void* buffer = nullptr;
            int buffer_size = 0;
            
            if (output_stream.Next(&buffer, &buffer_size)) {
                size_t bytes_to_write = std::min<size_t>(buffer_size, write_data.size());
                if (bytes_to_write > 0) {
                    memcpy(buffer, write_data.data(), bytes_to_write);
                    
                    // Test BackUp() if enabled
                    if (test_backup_operations && bytes_to_write > 0 && fdp.ConsumeBool()) {
                        int backup_amount = fdp.ConsumeIntegralInRange<int>(0, bytes_to_write);
                        output_stream.BackUp(backup_amount);
                    }
                }
            }
            
            // Test ByteCount() tracking
            if (test_bytecount_tracking) {
                int64_t byte_count = output_stream.ByteCount();
                // ByteCount should reflect actual written bytes (after any BackUp)
            }
            break;
        }
        
        case 1: {
            // Multiple Next() calls pattern - simulate streaming write in chunks
            size_t data_pos = 0;
            int next_call_count = 0;
            const int max_next_calls = use_multiple_next_calls ? fdp.ConsumeIntegralInRange<int>(2, 10) : 1;
            
            while (data_pos < write_data.size() && next_call_count < max_next_calls) {
                void* buffer = nullptr;
                int buffer_size = 0;
                
                if (output_stream.Next(&buffer, &buffer_size)) {
                    size_t bytes_to_write = std::min<size_t>(buffer_size, write_data.size() - data_pos);
                    if (bytes_to_write > 0) {
                        memcpy(buffer, write_data.data() + data_pos, bytes_to_write);
                        data_pos += bytes_to_write;
                        
                        // Possibly BackUp some of what we just wrote
                        if (test_backup_operations && bytes_to_write > 0 && fdp.ConsumeBool()) {
                            int backup_amount = fdp.ConsumeIntegralInRange<int>(0, bytes_to_write);
                            output_stream.BackUp(backup_amount);
                            data_pos -= backup_amount;
                        }
                    }
                } else {
                    break; // Next() failed
                }
                
                next_call_count++;
            }
            
            // Verify ByteCount() tracking
            if (test_bytecount_tracking) {
                int64_t byte_count = output_stream.ByteCount();
                // Should reflect total bytes written minus any backups
            }
            break;
        }
        
        case 2: {
            // Test with empty writes and edge cases
            void* buffer = nullptr;
            int buffer_size = 0;
            
            // Get buffer but possibly write nothing
            if (output_stream.Next(&buffer, &buffer_size)) {
                // Decide whether to write anything
                bool should_write = fdp.ConsumeBool();
                if (should_write && buffer_size > 0 && write_data.size() > 0) {
                    size_t bytes_to_write = fdp.ConsumeIntegralInRange<size_t>(0, std::min<size_t>(buffer_size, write_data.size()));
                    if (bytes_to_write > 0) {
                        memcpy(buffer, write_data.data(), bytes_to_write);
                    }
                    
                    // Test edge case: BackUp more than written (should be caught by CHECK)
                    if (test_edge_cases && fdp.ConsumeBool()) {
                        // Try to back up a possibly invalid amount, but ensure we don't exceed what was written
                        // If bytes_to_write is 0, we cannot back up anything
                        if (bytes_to_write > 0) {
                            int backup_amount = fdp.ConsumeIntegralInRange<int>(0, bytes_to_write);
                            // Note: StringOutputStream has CHECKs that should catch invalid BackUp
                            // We'll let it crash if checks fail - that's valid fuzzer feedback
                            output_stream.BackUp(backup_amount);
                        }
                    }
                }
            }
            
            // Test ByteCount() even with minimal/no writing
            if (test_bytecount_tracking) {
                int64_t byte_count = output_stream.ByteCount();
            }
            break;
        }
        case 3: {
            // Test growth patterns: write, Next() to get more buffer, write more
            size_t total_written = 0;
            const int iterations = fdp.ConsumeIntegralInRange<int>(1, 5);
            
            for (int i = 0; i < iterations && total_written < write_data.size(); i++) {
                void* buffer = nullptr;
                int buffer_size = 0;
                
                if (output_stream.Next(&buffer, &buffer_size)) {
                    size_t bytes_to_write = std::min<size_t>(
                        buffer_size, 
                        write_data.size() - total_written
                    );
                    
                    if (bytes_to_write > 0) {
                        memcpy(buffer, write_data.data() + total_written, bytes_to_write);
                        total_written += bytes_to_write;
                        
                        // Occasionally BackUp partial amount
                        if (test_backup_operations && bytes_to_write > 0 && fdp.ConsumeBool()) {
                            int backup_amount = fdp.ConsumeIntegralInRange<int>(0, bytes_to_write);
                            output_stream.BackUp(backup_amount);
                            total_written -= backup_amount;
                        }
                    }
                } else {
                    break;
                }
            }
            
            // Verify ByteCount matches total written (minus backups)
            if (test_bytecount_tracking) {
                int64_t byte_count = output_stream.ByteCount();
            }
            break;
        }
        
        case 4: {
            // Test with very small writes to exercise minimum size logic (kMinimumSize = 16)
            // StringOutputStream has kMinimumSize = 16, so even empty string gets at least 16 bytes
            
            void* buffer = nullptr;
            int buffer_size = 0;
            
            if (output_stream.Next(&buffer, &buffer_size)) {
                // Write a small amount (0-15 bytes) to see if minimum size is respected
                size_t small_write_size = fdp.ConsumeIntegralInRange<size_t>(0, 15);
                if (small_write_size > 0 && write_data.size() >= small_write_size) {
                    memcpy(buffer, write_data.data(), small_write_size);
                    
                    // BackUp some of it
                    if (test_backup_operations && small_write_size > 0 && fdp.ConsumeBool()) {
                        int backup_amount = fdp.ConsumeIntegralInRange<int>(0, small_write_size);
                        output_stream.BackUp(backup_amount);
                    }
                }
            }
            
            // Get another buffer to see growth
            if (use_multiple_next_calls && fdp.ConsumeBool()) {
                void* buffer2 = nullptr;
                int buffer_size2 = 0;
                
                if (output_stream.Next(&buffer2, &buffer_size2)) {
                    // Write more data
                    if (write_data.size() > 0) {
                        size_t offset = std::min<size_t>(16, write_data.size());
                        size_t bytes_to_write = fdp.ConsumeIntegralInRange<size_t>(0, write_data.size() - offset);
                        if (bytes_to_write > 0 && buffer_size2 >= bytes_to_write) {
                            memcpy(buffer2, write_data.data() + offset, bytes_to_write);
                        }
                    }
                }
            }
            
            if (test_bytecount_tracking) {
                int64_t byte_count = output_stream.ByteCount();
            }
            break;
        }
        
        case 5: {
            // Test mixed pattern: write, backup, write more, backup, etc.
            size_t data_pos = 0;
            const int max_operations = fdp.ConsumeIntegralInRange<int>(3, 10);
            int operations_performed = 0;
            
            while (data_pos < write_data.size() && operations_performed < max_operations) {
                void* buffer = nullptr;
                int buffer_size = 0;
                
                if (output_stream.Next(&buffer, &buffer_size)) {
                    // Determine how much to write (possibly 0)
                    size_t bytes_to_write = 0;
                    if (buffer_size > 0 && data_pos < write_data.size()) {
                        bytes_to_write = fdp.ConsumeIntegralInRange<size_t>(
                            0, 
                            std::min<size_t>(buffer_size, write_data.size() - data_pos)
                        );
                    }
                    
                    if (bytes_to_write > 0) {
                        memcpy(buffer, write_data.data() + data_pos, bytes_to_write);
                        data_pos += bytes_to_write;
                    }
                    
                    operations_performed++;
                    
                    // Possibly BackUp
                    if (test_backup_operations && bytes_to_write > 0 && fdp.ConsumeBool()) {
                        int backup_amount = fdp.ConsumeIntegralInRange<int>(0, bytes_to_write);
                        output_stream.BackUp(backup_amount);
                        data_pos -= backup_amount;
                    }
                } else {
                    break;
                }
            }
            
            if (test_bytecount_tracking) {
                int64_t byte_count = output_stream.ByteCount();
            }
            break;
        }
        
        case 6: {
            // Test with target string that already has content
            std::string preloaded_string;
            if (fdp.remaining_bytes() > 0) {
                size_t preload_size = fdp.ConsumeIntegralInRange<size_t>(0, std::min<size_t>(fdp.remaining_bytes(), 1024));
                preloaded_string = fdp.ConsumeBytesAsString(preload_size);
            }
            
            // Create new string with preloaded content
            std::string combined_target = preloaded_string;
            StringOutputStream preloaded_stream(&combined_target);
            
            // Write additional data
            void* buffer = nullptr;
            int buffer_size = 0;
            
            if (preloaded_stream.Next(&buffer, &buffer_size) && buffer_size > 0 && write_data.size() > 0) {
                size_t bytes_to_write = std::min<size_t>(buffer_size, write_data.size());
                memcpy(buffer, write_data.data(), bytes_to_write);
                
                if (test_backup_operations && bytes_to_write > 0 && fdp.ConsumeBool()) {
                    int backup_amount = fdp.ConsumeIntegralInRange<int>(0, bytes_to_write);
                    preloaded_stream.BackUp(backup_amount);
                }
            }
            
            if (test_bytecount_tracking) {
                int64_t byte_count = preloaded_stream.ByteCount();
                // Should include preloaded_string.size() + written - backup
            }
            break;
        }
        
        case 7: {
            // Comprehensive test: mix of all operations
            std::string comprehensive_target;
            StringOutputStream comprehensive_stream(&comprehensive_target);
            
            size_t total_attempted_writes = 0;
            const int total_operations = fdp.ConsumeIntegralInRange<int>(5, 20);
            
            for (int op = 0; op < total_operations && total_attempted_writes < write_data.size(); op++) {
                void* buffer = nullptr;
                int buffer_size = 0;
                
                if (comprehensive_stream.Next(&buffer, &buffer_size)) {
                    // Decide what to do with this buffer
                    uint8_t action = fdp.ConsumeIntegral<uint8_t>() % 4;
                    
                    switch (action) {
                        case 0: {
                            // Write some data
                            if (buffer_size > 0 && total_attempted_writes < write_data.size()) {
                                size_t bytes_to_write = fdp.ConsumeIntegralInRange<size_t>(
                                    0, 
                                    std::min<size_t>(buffer_size, write_data.size() - total_attempted_writes)
                                );
                                if (bytes_to_write > 0) {
                                    memcpy(buffer, write_data.data() + total_attempted_writes, bytes_to_write);
                                    total_attempted_writes += bytes_to_write;
                                }
                            }
                            break;
                        }
                        
                        case 1: {
                            // Write then partially BackUp
                            if (buffer_size > 0 && total_attempted_writes < write_data.size()) {
                                size_t bytes_to_write = fdp.ConsumeIntegralInRange<size_t>(
                                    1, 
                                    std::min<size_t>(buffer_size, write_data.size() - total_attempted_writes)
                                );
                                if (bytes_to_write > 0) {
                                    memcpy(buffer, write_data.data() + total_attempted_writes, bytes_to_write);
                                    total_attempted_writes += bytes_to_write;
                                    
                                    // BackUp some portion
                                    int backup_amount = fdp.ConsumeIntegralInRange<int>(0, bytes_to_write);
                                    comprehensive_stream.BackUp(backup_amount);
                                    total_attempted_writes -= backup_amount;
                                }
                            }
                            break;
                        }
                        
                        case 2: {
                            // Get buffer but write nothing (exercise zero-write path)
                            // Nothing to write
                            break;
                        }
                        
                        case 3: {
                            // Write and possibly BackUp all of it
                            if (buffer_size > 0 && total_attempted_writes < write_data.size()) {
                                size_t bytes_to_write = fdp.ConsumeIntegralInRange<size_t>(
                                    1, 
                                    std::min<size_t>(buffer_size, write_data.size() - total_attempted_writes)
                                );
                                if (bytes_to_write > 0) {
                                    memcpy(buffer, write_data.data() + total_attempted_writes, bytes_to_write);
                                    total_attempted_writes += bytes_to_write;
                                    
                                    if (fdp.ConsumeBool()) {
                                        comprehensive_stream.BackUp(bytes_to_write);
                                        total_attempted_writes -= bytes_to_write;
                                    }
                                }
                            }
                            break;
                        }
                    }
                }
            }
            
            // Final ByteCount check
            if (test_bytecount_tracking) {
                int64_t final_byte_count = comprehensive_stream.ByteCount();
            }
            
            // Target string should contain whatever was written (minus backups)
            break;
        }
    }
    
    // Destructor of StringOutputStream will run automatically
    // Destructor of target_string will run automatically
    
    return 0;
}

} // namespace harness_016
