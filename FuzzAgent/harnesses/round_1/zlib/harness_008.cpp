#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include "zlib.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_008 {
/*
 * Fuzzing harness for zlib inflateBack API family - Advanced callback testing
 * Targets inflateBack, inflateBackInit_, inflateBackEnd with semantic diversity
 * Focuses on: different callback implementations, window buffer edge cases,
 * state machine transitions, and comprehensive error handling scenarios
 * 
 * Semantic differences from harness_002:
 * 1. Variable-rate input callbacks (random chunk sizes, delayed failures)
 * 2. Different output buffer strategies (circular buffer, size-limited, rate-limited)
 * 3. Window buffer edge cases (unaligned, overlapping, boundary conditions)
 * 4. State machine testing (multiple init/end sequences, partial decompression)
 * 5. Error injection at different points in the decompression process
 */


// ==============================================
// Advanced callback context structures
// ==============================================

struct VariableRateInputContext {
    const uint8_t* data;
    size_t size;
    size_t pos;
    size_t max_chunk_size;
    size_t min_chunk_size;
    size_t failure_point;  // Position at which to fail (0 = never fail)
    size_t call_count;
};

struct RateLimitedOutputContext {
    std::vector<uint8_t> buffer;
    size_t max_total_size;
    size_t max_chunk_size;
    size_t failure_threshold;  // Call count at which to fail
    size_t call_count;
    bool use_circular_buffer;
    size_t circular_pos;
};

struct StateTrackingContext {
    z_stream* strm;
    int last_mode;
    int transition_count;
    bool track_state_changes;
};

// ==============================================
// Variable-rate input callback implementations
// ==============================================

static unsigned variable_rate_input_callback(void* desc, z_const unsigned char FAR* FAR* buf) {
    VariableRateInputContext* ctx = (VariableRateInputContext*)desc;
    ctx->call_count++;
    
    // Fail if we reached the failure point
    if (ctx->failure_point > 0 && ctx->call_count >= ctx->failure_point) {
        return 0;
    }
    
    if (ctx->pos >= ctx->size) {
        return 0;
    }
    
    *buf = (z_const unsigned char FAR*)&ctx->data[ctx->pos];
    
    // Variable chunk size based on min/max
    size_t available = ctx->size - ctx->pos;
    size_t chunk_size;
    
    if (ctx->max_chunk_size == ctx->min_chunk_size) {
        chunk_size = ctx->max_chunk_size;
    } else {
        // Simple deterministic "random" chunk size
        chunk_size = ctx->min_chunk_size + (ctx->pos % (ctx->max_chunk_size - ctx->min_chunk_size + 1));
    }
    
    if (chunk_size > available) {
        chunk_size = available;
    }
    
    ctx->pos += chunk_size;
    return (unsigned)chunk_size;
}

static unsigned delayed_failure_input_callback(void* desc, z_const unsigned char FAR* FAR* buf) {
    static size_t global_call_counter = 0;
    global_call_counter++;
    
    VariableRateInputContext* ctx = (VariableRateInputContext*)desc;
    
    // Fail after a certain number of global calls
    if (global_call_counter > 100) {
        return 0;
    }
    
    if (ctx->pos >= ctx->size) {
        return 0;
    }
    
    *buf = (z_const unsigned char FAR*)&ctx->data[ctx->pos];
    
    // Always return 1 byte to stress the state machine
    size_t to_return = 1;
    if (ctx->pos + to_return > ctx->size) {
        to_return = ctx->size - ctx->pos;
    }
    
    ctx->pos += to_return;
    return (unsigned)to_return;
}

// ==============================================
// Advanced output callback implementations
// ==============================================

static int rate_limited_output_callback(void* desc, unsigned char FAR* buf, unsigned len) {
    RateLimitedOutputContext* ctx = (RateLimitedOutputContext*)desc;
    ctx->call_count++;
    
    // Fail if we reached the failure threshold
    if (ctx->failure_threshold > 0 && ctx->call_count >= ctx->failure_threshold) {
        return 1;
    }
    
    // Check max total size
    if (ctx->buffer.size() + len > ctx->max_total_size) {
        return 1;
    }
    
    // Check max chunk size
    if (len > ctx->max_chunk_size) {
        return 1;
    }
    
    if (ctx->use_circular_buffer) {
        // Simulate circular buffer behavior
        size_t write_pos = ctx->circular_pos % ctx->max_total_size;
        size_t space_remaining = ctx->max_total_size - write_pos;
        size_t to_write = len > space_remaining ? space_remaining : len;
        
        if (to_write > 0) {
            // In a real circular buffer, we'd wrap around
            // For simplicity, we just track position
            ctx->circular_pos += to_write;
        }
        
        // Always append to linear buffer for verification
        ctx->buffer.insert(ctx->buffer.end(), buf, buf + len);
        return 0;
    } else {
        // Standard linear buffer
        ctx->buffer.insert(ctx->buffer.end(), buf, buf + len);
        return 0;
    }
}

static int boundary_checking_output_callback(void* desc, unsigned char FAR* buf, unsigned len) {
    RateLimitedOutputContext* ctx = (RateLimitedOutputContext*)desc;
    
    // Test boundary conditions
    if (len == 0) {
        return 0;  // Zero-length write is OK
    }
    
    if (ctx->buffer.size() == ctx->max_total_size) {
        return 1;  // Buffer full
    }
    
    // Allow writes that exactly fill the buffer
    if (ctx->buffer.size() + len > ctx->max_total_size) {
        return 1;
    }
    
    ctx->buffer.insert(ctx->buffer.end(), buf, buf + len);
    return 0;
}

// ==============================================
// State tracking and error injection
// ==============================================

static void track_state_transition(StateTrackingContext* ctx, int new_mode) {
    if (ctx->track_state_changes && ctx->last_mode != new_mode) {
        ctx->transition_count++;
        ctx->last_mode = new_mode;
    }
}

// ==============================================
// Main fuzzing entry point
// ==============================================

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume configuration parameters
    int windowBits = fdp.ConsumeIntegralInRange<int>(8, 15);
    int callback_type = fdp.ConsumeIntegralInRange<int>(0, 3);
    bool test_window_edge_cases = fdp.ConsumeBool();
    bool test_state_transitions = fdp.ConsumeBool();
    bool inject_errors = fdp.ConsumeBool();
    bool test_multiple_streams = fdp.ConsumeBool();
    bool use_initial_input = fdp.ConsumeBool();
    
    // Window buffer configuration
    size_t window_size = 1U << windowBits;
    size_t min_required_size = 1U << windowBits;  // Library requires at least this much
    if (test_window_edge_cases) {
        // Test edge cases: unaligned sizes, boundary conditions
        int window_adjust = fdp.ConsumeIntegralInRange<int>(-10, 10);
        size_t adjusted_size = window_size + window_adjust;
        // Ensure adjusted_size is at least min_required_size and within reasonable bounds
        if (adjusted_size >= min_required_size && adjusted_size <= 65536) {
            window_size = adjusted_size;
        }
        // Safety: ensure window_size never goes below min_required_size
        if (window_size < min_required_size) {
            window_size = min_required_size;
        }
    }
    
    unsigned char* window = nullptr;
    void* raw_alloc = nullptr;  // Store the original malloc allocation
    if (window_size > 0) {
        // We need at least min_required_size bytes available after any misalignment
        const size_t max_misalignment = 7;
        size_t alloc_size = window_size + max_misalignment;
        raw_alloc = malloc(alloc_size);
        if (!raw_alloc) {
            return 0;
        }
        
        // Potentially misalign the buffer, but ensure we still have min_required_size bytes
        if (test_window_edge_cases && alloc_size > max_misalignment) {
            // Calculate maximum offset we can use while still leaving min_required_size bytes
            size_t max_valid_offset = alloc_size - min_required_size;
            size_t offset = fdp.ConsumeIntegralInRange<size_t>(0, max_valid_offset > max_misalignment ? max_misalignment : max_valid_offset);
            window = (unsigned char*)((uintptr_t)raw_alloc + offset);
        } else {
            window = (unsigned char*)raw_alloc;
        }
        
        // Verify we have enough space after misalignment
        size_t bytes_available = alloc_size - ((uintptr_t)window - (uintptr_t)raw_alloc);
        if (bytes_available < min_required_size) {
            // This shouldn't happen with our calculations, but be safe
            free(raw_alloc);
            return 0;
        }
    }
    
    // Prepare compressed data
    size_t compressed_data_size = fdp.ConsumeIntegralInRange<size_t>(128, 8192);
    if (fdp.remaining_bytes() < compressed_data_size) {
        if (raw_alloc) free(raw_alloc);
        return 0;
    }
    
    std::vector<uint8_t> compressed_data = fdp.ConsumeBytes<uint8_t>(compressed_data_size);
    
    // Initialize z_stream
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.next_in = Z_NULL;
    strm.avail_in = 0;
    
    // Initialize inflateBack
    int ret = inflateBackInit_(&strm, windowBits, window, ZLIB_VERSION, sizeof(z_stream));
    
    if (ret == Z_OK && window != nullptr) {
        // Set up callback contexts based on callback_type
        VariableRateInputContext in_ctx;
        RateLimitedOutputContext out_ctx;
        StateTrackingContext state_ctx;
        
        // Initialize input context
        in_ctx.data = compressed_data.data();
        in_ctx.size = compressed_data.size();
        in_ctx.pos = 0;
        in_ctx.max_chunk_size = fdp.ConsumeIntegralInRange<size_t>(1, 512);
        in_ctx.min_chunk_size = fdp.ConsumeIntegralInRange<size_t>(1, in_ctx.max_chunk_size);
        in_ctx.failure_point = inject_errors ? fdp.ConsumeIntegralInRange<size_t>(0, 100) : 0;
        in_ctx.call_count = 0;
        
        // Initialize output context
        out_ctx.max_total_size = fdp.ConsumeIntegralInRange<size_t>(1024, 65536);
        out_ctx.max_chunk_size = fdp.ConsumeIntegralInRange<size_t>(64, 4096);
        out_ctx.failure_threshold = inject_errors ? fdp.ConsumeIntegralInRange<size_t>(0, 50) : 0;
        out_ctx.call_count = 0;
        out_ctx.use_circular_buffer = fdp.ConsumeBool();
        out_ctx.circular_pos = 0;
        
        // Initialize state tracking
        state_ctx.strm = &strm;
        state_ctx.last_mode = -1;
        state_ctx.transition_count = 0;
        state_ctx.track_state_changes = test_state_transitions;
        
        // Set initial input if requested
        if (use_initial_input && compressed_data.size() > 0) {
            size_t initial_size = fdp.ConsumeIntegralInRange<size_t>(0, compressed_data.size());
            strm.next_in = (Bytef*)compressed_data.data();
            strm.avail_in = (uInt)initial_size;
            in_ctx.pos = initial_size;
        }
        
        // Select callback functions based on callback_type
        in_func in_callback = nullptr;
        out_func out_callback = nullptr;
        
        switch (callback_type) {
            case 0:
                in_callback = variable_rate_input_callback;
                out_callback = rate_limited_output_callback;
                break;
            case 1:
                in_callback = delayed_failure_input_callback;
                out_callback = boundary_checking_output_callback;
                break;
            case 2:
                // Test with same callback for both (should still work)
                in_callback = variable_rate_input_callback;
                out_callback = rate_limited_output_callback;
                break;
            case 3:
                // Test edge case: minimum chunk sizes
                in_ctx.min_chunk_size = 1;
                in_ctx.max_chunk_size = 1;
                in_callback = variable_rate_input_callback;
                out_callback = boundary_checking_output_callback;
                break;
        }
        
        // Test inflateBack with selected callbacks
        ret = inflateBack(&strm, in_callback, &in_ctx, out_callback, &out_ctx);
        
        // Test error recovery scenarios
        if (ret == Z_BUF_ERROR || ret == Z_DATA_ERROR) {
            // Try to recover by resetting and trying again
            if (test_state_transitions) {
                // Reset input context
                in_ctx.pos = 0;
                in_ctx.call_count = 0;
                out_ctx.buffer.clear();
                out_ctx.call_count = 0;
                
                // Reset stream state
                strm.next_in = Z_NULL;
                strm.avail_in = 0;
                
                // Try inflateBack again
                int ret2 = inflateBack(&strm, in_callback, &in_ctx, out_callback, &out_ctx);
                (void)ret2;  // Ignore result, just exercise the code
            }
        }
        
        // Test multiple streams if requested
        if (test_multiple_streams && ret != Z_STREAM_ERROR) {
            // Create a second stream with different parameters
            z_stream strm2;
            strm2.zalloc = Z_NULL;
            strm2.zfree = Z_NULL;
            strm2.opaque = Z_NULL;
            strm2.next_in = Z_NULL;
            strm2.avail_in = 0;
            
            int windowBits2 = fdp.ConsumeIntegralInRange<int>(8, 15);
            size_t window_size2 = 1U << windowBits2;
            unsigned char* window2 = (unsigned char*)malloc(window_size2);
            
            if (window2) {
                int ret2 = inflateBackInit_(&strm2, windowBits2, window2, ZLIB_VERSION, sizeof(z_stream));
                if (ret2 == Z_OK) {
                    // Reset input context for second stream
                    in_ctx.pos = 0;
                    in_ctx.call_count = 0;
                    out_ctx.buffer.clear();
                    out_ctx.call_count = 0;
                    
                    // Test with different callback configuration
                    int ret3 = inflateBack(&strm2, in_callback, &in_ctx, out_callback, &out_ctx);
                    (void)ret3;
                    
                    inflateBackEnd(&strm2);
                }
                free(window2);
            }
        }
        // Clean up primary stream
        inflateBackEnd(&strm);
        
        // Free allocation in success case
        if (raw_alloc) {
            free(raw_alloc);
            raw_alloc = nullptr;
        }
        
    } else {
        // Test error cleanup paths
        // First do error injection tests before freeing
        if (inject_errors) {
            z_stream bad_strm;
            memset(&bad_strm, 0, sizeof(bad_strm));
            
            // Test with null window
            int bad_ret = inflateBackInit_(&bad_strm, 15, nullptr, ZLIB_VERSION, sizeof(z_stream));
            (void)bad_ret;
            
            // Test with invalid windowBits (window may still be valid here)
            if (window != nullptr) {
                bad_ret = inflateBackInit_(&bad_strm, 7, window, ZLIB_VERSION, sizeof(z_stream));
                (void)bad_ret;
            }
            
            // Test inflateBackEnd with uninitialized stream
            inflateBackEnd(&bad_strm);
        }
        
        // Free allocation after error tests
        if (raw_alloc) {
            free(raw_alloc);
            raw_alloc = nullptr;
        }
    }
    
    // Free window buffer if not already freed (should be nullptr if already freed)
    if (raw_alloc != nullptr) {
        free(raw_alloc);
    }
    
    return 0;
}

} // namespace harness_008
