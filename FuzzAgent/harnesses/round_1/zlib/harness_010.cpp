#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "zlib.h"
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>
#include <string>

namespace harness_010 {
/*
 * Fuzzing harness for zlib inflation synchronization error recovery targeting inflateSync
 * Target: inflateSync with 16 undiscovered branches - error recovery from corrupted compressed data
 * 
 * This harness specifically targets inflateSync API for synchronization recovery after
 * encountering corrupted or partial compressed data streams. Semantically different from:
 * - harness_001: Has basic inflateSync test but only in Z_DATA_ERROR case
 * - harness_006: Tests inflateSyncPoint but not inflateSync synchronization recovery
 * 
 * API sequence as specified by ManagerAgent:
 * 1. Call inflateInit2_ to initialize inflation stream
 * 2. Call inflate repeatedly with corrupted/partial compressed data to reach error state
 * 3. Call inflateSync to attempt synchronization recovery  
 * 4. Call inflateReset if synchronization fails to reset stream
 * 5. Call inflateEnd to clean up resources
 * 
 * Additional helper APIs: inflateInit2_, inflate, inflateEnd, inflateReset
 * 
 * Strategy: Generate various corrupted data patterns to trigger different error states
 * and test inflateSync's ability to find synchronization points (00 00 FF FF patterns).
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check - need enough for configuration and test data
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // ==============================================
    // Consume fixed-size configuration parameters
    // ==============================================
    
    // Window bits for inflateInit2 (8-15, negative for raw, +16 for gzip)
    int windowBits = fdp.ConsumeIntegralInRange<int>(-15, 31);
    
    // Test scenario selection
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    // Error injection flags
    bool inject_corruption = fdp.ConsumeBool();
    bool test_partial_data = fdp.ConsumeBool();
    bool test_sync_pattern = fdp.ConsumeBool();
    bool test_multiple_sync_calls = fdp.ConsumeBool();
    bool test_reset_after_failure = fdp.ConsumeBool();
    bool test_mixed_operations = fdp.ConsumeBool();
    
    // Data size parameters
    size_t compressed_data_size = fdp.ConsumeIntegralInRange<size_t>(16, 4096);
    if (fdp.remaining_bytes() < compressed_data_size) {
        return 0;
    }
    
    // Consume compressed data (will be corrupted/partial for testing)
    std::vector<uint8_t> compressed_data = fdp.ConsumeBytes<uint8_t>(compressed_data_size);
    
    // ==============================================
    // Corrupt the data if requested
    // ==============================================
    
    if (inject_corruption && compressed_data.size() > 4) {
        // Various corruption patterns to trigger different error states
        uint8_t corruption_type = fdp.ConsumeIntegral<uint8_t>() % 6;
        
        switch (corruption_type) {
            case 0:
                // Random byte flipping
                for (size_t i = 0; i < compressed_data.size() / 4; i++) {
                    size_t pos = fdp.ConsumeIntegralInRange<size_t>(0, compressed_data.size() - 1);
                    compressed_data[pos] ^= 0xFF;
                }
                break;
                
            case 1:
                // Insert invalid bytes in middle
                if (compressed_data.size() > 10) {
                    size_t pos = fdp.ConsumeIntegralInRange<size_t>(5, compressed_data.size() - 5);
                    compressed_data.insert(compressed_data.begin() + pos, 
                                         fdp.ConsumeIntegral<uint8_t>());
                }
                break;
                
            case 2:
                // Truncate data (partial stream)
                if (test_partial_data && compressed_data.size() > 4) {
                    size_t new_size = fdp.ConsumeIntegralInRange<size_t>(1, compressed_data.size() - 1);
                    compressed_data.resize(new_size);
                }
                break;
                
            case 3:
                // Insert sync pattern (00 00 FF FF) to test successful recovery
                if (test_sync_pattern && compressed_data.size() > 8) {
                    size_t pos = fdp.ConsumeIntegralInRange<size_t>(4, compressed_data.size() - 4);
                    compressed_data[pos] = 0x00;
                    compressed_data[pos + 1] = 0x00;
                    compressed_data[pos + 2] = 0xFF;
                    compressed_data[pos + 3] = 0xFF;
                }
                break;
                
            case 4:
                // Create malformed zlib/gzip header
                if (compressed_data.size() > 2) {
                    compressed_data[0] = 0x78;  // Valid zlib header start
                    compressed_data[1] = fdp.ConsumeIntegral<uint8_t>() & 0x1F;  // Potentially invalid
                }
                break;
                
            case 5:
                // Append garbage data
                size_t garbage_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
                for (size_t i = 0; i < garbage_size && fdp.remaining_bytes() > 0; i++) {
                    compressed_data.push_back(fdp.ConsumeIntegral<uint8_t>());
                }
                break;
        }
    }
    
    // ==============================================
    // Initialize inflate stream
    // ==============================================
    
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    
    // Initialize with version string
    const char* version = ZLIB_VERSION;
    int ret = inflateInit2_(&strm, windowBits, version, sizeof(z_stream));
    
    if (ret != Z_OK) {
        // If initialization fails, try with default windowBits
        windowBits = 15;
        ret = inflateInit2_(&strm, windowBits, version, sizeof(z_stream));
        if (ret != Z_OK) {
            return 0;  // Can't proceed without valid stream
        }
    }
    
    // ==============================================
    // Test different scenarios based on fuzzer input
    // ==============================================
    
    int sync_result = Z_OK;
    size_t total_in_before_sync = 0;
    
    switch (test_scenario) {
        case 0:  // Basic inflate with potential error followed by sync
        {
            // Set up input
            strm.next_in = compressed_data.data();
            strm.avail_in = compressed_data.size();
            
            // Allocate output buffer
            size_t out_size = compressed_data.size() * 2;
            std::vector<uint8_t> out_buffer(out_size);
            strm.next_out = out_buffer.data();
            strm.avail_out = out_size;
            
            // Try to inflate (may fail with corrupted data)
            int inflate_ret = inflate(&strm, Z_SYNC_FLUSH);
            
            // Save position before sync
            total_in_before_sync = strm.total_in;
            
            // Attempt synchronization recovery
            sync_result = inflateSync(&strm);
            
            // If sync found something, continue inflation
            if (sync_result == Z_OK) {
                // Continue inflation from sync point
                inflate_ret = inflate(&strm, Z_FINISH);
            }
            break;
        }
        
        case 1:  // Multiple sync attempts with additional input
        {
            // Process data in chunks to simulate streaming
            size_t chunk_size = fdp.ConsumeIntegralInRange<size_t>(1, 64);
            if (chunk_size > compressed_data.size()) {
                chunk_size = compressed_data.size();
            }
            
            std::vector<uint8_t> out_buffer(4096);
            size_t processed = 0;
            
            while (processed < compressed_data.size() && strm.avail_out > 0) {
                // Provide next chunk
                strm.next_in = compressed_data.data() + processed;
                strm.avail_in = (processed + chunk_size <= compressed_data.size()) ? 
                               chunk_size : compressed_data.size() - processed;
                processed += strm.avail_in;
                
                strm.next_out = out_buffer.data();
                strm.avail_out = out_buffer.size();
                
                // Try to inflate
                int inflate_ret = inflate(&strm, Z_SYNC_FLUSH);
                
                // If error, try sync
                if (inflate_ret == Z_DATA_ERROR && test_multiple_sync_calls) {
                    sync_result = inflateSync(&strm);
                    
                    // If sync returns Z_BUF_ERROR, provide more input
                    if (sync_result == Z_BUF_ERROR && processed < compressed_data.size()) {
                        continue;  // Loop will provide more input
                    }
                }
            }
            break;
        }
        
        case 2:  // Test inflateReset after sync failure
        {
            strm.next_in = compressed_data.data();
            strm.avail_in = compressed_data.size();
            
            std::vector<uint8_t> out_buffer(4096);
            strm.next_out = out_buffer.data();
            strm.avail_out = out_buffer.size();
            
            // Try inflation (will likely fail with corrupted data)
            inflate(&strm, Z_SYNC_FLUSH);
            
            // Try sync
            sync_result = inflateSync(&strm);
            
            // If sync fails, try reset
            if (sync_result != Z_OK && test_reset_after_failure) {
                int reset_ret = inflateReset(&strm);
                if (reset_ret == Z_OK) {
                    // Reset successful, try again with same or different data
                    strm.next_in = compressed_data.data();
                    strm.avail_in = compressed_data.size();
                    strm.next_out = out_buffer.data();
                    strm.avail_out = out_buffer.size();
                    
                    inflate(&strm, Z_SYNC_FLUSH);
                }
            }
            break;
        }
        
        case 3:  // Test with Z_FULL_FLUSH points (where sync should work)
        {
            // Create data that might contain sync points
            std::vector<uint8_t> data_with_flush;
            
            // Start with some valid compressed data if we can generate it
            // For now, use the fuzzer input with potential sync patterns inserted
            data_with_flush = compressed_data;
            
            // Insert explicit sync patterns at random positions
            if (test_sync_pattern && data_with_flush.size() > 20) {
                int num_patterns = fdp.ConsumeIntegralInRange<int>(0, 3);
                for (int i = 0; i < num_patterns; i++) {
                    size_t pos = fdp.ConsumeIntegralInRange<size_t>(4, data_with_flush.size() - 4);
                    data_with_flush[pos] = 0x00;
                    data_with_flush[pos + 1] = 0x00;
                    data_with_flush[pos + 2] = 0xFF;
                    data_with_flush[pos + 3] = 0xFF;
                }
            }
            
            strm.next_in = data_with_flush.data();
            strm.avail_in = data_with_flush.size();
            
            std::vector<uint8_t> out_buffer(8192);
            strm.next_out = out_buffer.data();
            strm.avail_out = out_buffer.size();
            
            // Try to inflate
            int inflate_ret = inflate(&strm, Z_SYNC_FLUSH);
            
            // Regardless of result, try sync
            sync_result = inflateSync(&strm);
            
            // Log sync result for coverage
            if (sync_result == Z_OK) {
                // Sync found a point
                size_t sync_point = strm.total_in;
            }
            break;
        }
        
        case 4:  // Test edge cases with minimal data
        {
            if (compressed_data.size() >= 4) {
                // Test with just a sync pattern
                std::vector<uint8_t> just_sync = {0x00, 0x00, 0xFF, 0xFF};
                strm.next_in = just_sync.data();
                strm.avail_in = just_sync.size();
                
                std::vector<uint8_t> out_buffer(4);
                strm.next_out = out_buffer.data();
                strm.avail_out = out_buffer.size();
                
                sync_result = inflateSync(&strm);
            }
            break;
        }
        
        case 5:  // Test with mixed valid and corrupted segments
        {
            // Split data into segments
            size_t segment_count = fdp.ConsumeIntegralInRange<size_t>(2, 8);
            size_t segment_size = compressed_data.size() / segment_count;
            
            std::vector<uint8_t> out_buffer(8192);
            size_t processed = 0;
            
            for (size_t i = 0; i < segment_count && processed < compressed_data.size(); i++) {
                size_t this_segment = (i == segment_count - 1) ? 
                                     compressed_data.size() - processed : segment_size;
                
                strm.next_in = compressed_data.data() + processed;
                strm.avail_in = this_segment;
                processed += this_segment;
                
                strm.next_out = out_buffer.data();
                strm.avail_out = out_buffer.size();
                
                // Inflate this segment
                int inflate_ret = inflate(&strm, Z_SYNC_FLUSH);
                
                // After each segment, optionally try sync
                if (test_mixed_operations && (i % 2 == 0)) {
                    sync_result = inflateSync(&strm);
                }
            }
            break;
        }
        
        case 6:  // Test with different flush modes
        {
            int flush_mode = fdp.ConsumeIntegralInRange<int>(0, 5);
            int flush_value;
            
            switch (flush_mode) {
                case 0: flush_value = Z_NO_FLUSH; break;
                case 1: flush_value = Z_SYNC_FLUSH; break;
                case 2: flush_value = Z_FULL_FLUSH; break;
                case 3: flush_value = Z_FINISH; break;
                case 4: flush_value = Z_BLOCK; break;
                default: flush_value = Z_SYNC_FLUSH; break;
            }
            
            strm.next_in = compressed_data.data();
            strm.avail_in = compressed_data.size();
            
            std::vector<uint8_t> out_buffer(4096);
            strm.next_out = out_buffer.data();
            strm.avail_out = out_buffer.size();
            
            // Inflate with specified flush mode
            int inflate_ret = inflate(&strm, flush_value);
            
            // Try sync
            sync_result = inflateSync(&strm);
            break;
        }
        
        case 7:  // Comprehensive test combining multiple operations
        {
            // Run inflation
            strm.next_in = compressed_data.data();
            strm.avail_in = compressed_data.size();
            
            std::vector<uint8_t> out_buffer(8192);
            strm.next_out = out_buffer.data();
            strm.avail_out = out_buffer.size();
            
            int inflate_ret = inflate(&strm, Z_SYNC_FLUSH);
            
            // Try sync
            sync_result = inflateSync(&strm);
            
            // If sync returns Z_BUF_ERROR, provide more data if available
            if (sync_result == Z_BUF_ERROR && fdp.remaining_bytes() > 0) {
                size_t extra_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
                std::vector<uint8_t> extra_data = fdp.ConsumeBytes<uint8_t>(extra_size);
                
                // Append extra data and try again
                strm.next_in = extra_data.data();
                strm.avail_in = extra_data.size();
                sync_result = inflateSync(&strm);
            }
            
            // Try reset if still failing
            if (sync_result != Z_OK && sync_result != Z_BUF_ERROR) {
                inflateReset(&strm);
            }
            break;
        }
    }
    
    // ==============================================
    // Clean up
    // ==============================================
    
    inflateEnd(&strm);
    
    return 0;
}

} // namespace harness_010
