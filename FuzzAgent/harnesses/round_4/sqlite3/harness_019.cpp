#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "sqlite3.h"

namespace harness_019 {
/*
 * Fuzzing harness for SQLite library - targeting memory management and heap limit configuration APIs
 * as identified by coverage analysis with 553+ undiscovered branches.
 * 
 * This harness specifically targets SQLite Memory Management APIs with high undiscovered branches:
 * - sqlite3_soft_heap_limit (Target) - 553 undiscovered branches, 0% covered
 * - sqlite3_hard_heap_limit64 (Target) - 537 undiscovered branches, 0% covered  
 * - sqlite3_soft_heap_limit64 (Target) - 551 undiscovered branches, 12.5% covered
 * - sqlite3_memory_highwater (Target) - 439 undiscovered branches, 0% covered
 * - sqlite3_memory_used (Target) - 439 undiscovered branches, 0% covered
 * - sqlite3_status (Target) - 441 undiscovered branches, 0% covered
 * - sqlite3_status64 (Target) - 439 undiscovered branches, 0% covered
 * 
 * Required Helper APIs:
 * - sqlite3_initialize (Required Helper: Init) - already covered
 * - sqlite3_shutdown (Required Helper: Cleanup) - already covered
 * 
 * Invocation Sequence as specified by coverage guidance:
 * 1. Call sqlite3_initialize() to initialize SQLite library
 * 2. Call sqlite3_soft_heap_limit() to set soft heap limit
 * 3. Call sqlite3_hard_heap_limit64() to set hard heap limit
 * 4. Call sqlite3_soft_heap_limit64() to set soft heap limit with 64-bit parameter
 * 5. Call sqlite3_memory_highwater() to check peak memory usage
 * 6. Call sqlite3_memory_used() to check current memory usage
 * 7. Call sqlite3_status() and sqlite3_status64() to query memory status information
 * 8. Call sqlite3_shutdown() to cleanup
 * 
 * Test Scenarios derived from fuzzer input:
 * - Various heap limit values (positive, negative, zero, large values)
 * - Different reset flag values for memory highwater and status functions
 * - Multiple status operation codes (SQLITE_STATUS_* constants)
 * - Edge cases (zero limits, negative limits, extremely large limits)
 * - Interactions between soft and hard heap limits
 * 
 * Error Path Coverage:
 * - Invalid status operation codes
 * - Negative limit values (query mode)
 * - Zero limits (disables limiter)
 * - Memory allocation failures when limits are enforced
 * - Reset flag behavior for highwater marks
 * 
 * Semantic differentiation from existing harnesses (000-018):
 * - Specifically targets memory management APIs (0% coverage currently)
 * - Focuses on heap limit configuration and memory usage monitoring
 * - Tests interactions between soft and hard heap limits
 * - Different from harness_000 which focuses on prepared statements
 * - Different from harness_018 which focuses on SQL completion validation
 * - Provides comprehensive memory subsystem testing
 * 
 * These functions represent 553+ undiscovered branches in the memory management cluster.
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful testing
    // We need enough for: operation type, heap limit values, status codes, reset flags
    if (size < 16) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Initialize SQLite library
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }
    
    // Step 2: Consume operation type from fuzzer input
    // 0 = Test heap limit functions with various values
    // 1 = Test memory usage monitoring functions
    // 2 = Test status functions with various operation codes
    // 3 = Test comprehensive memory management workflow
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Step 3: Consume heap limit values from fuzzer input
    // Use both positive and negative values (negative = query current limit)
    sqlite3_int64 soft_limit_val = fdp.ConsumeIntegral<sqlite3_int64>();
    sqlite3_int64 hard_limit_val = fdp.ConsumeIntegral<sqlite3_int64>();
    
    // Step 4: Consume reset flag for memory highwater and status functions
    int reset_flag = fdp.ConsumeIntegral<int>() & 1; // 0 or 1
    
    // Step 5: Consume status operation codes
    // SQLite defines these status constants:
    // SQLITE_STATUS_MEMORY_USED = 0
    // SQLITE_STATUS_MALLOC_SIZE = 1  
    // SQLITE_STATUS_MALLOC_COUNT = 2
    // SQLITE_STATUS_PAGECACHE_USED = 3
    // SQLITE_STATUS_PAGECACHE_OVERFLOW = 4
    // SQLITE_STATUS_PAGECACHE_SIZE = 5
    // SQLITE_STATUS_PARSER_STACK = 6
    // SQLITE_STATUS_SCRATCH_USED = 7 (deprecated)
    // SQLITE_STATUS_SCRATCH_OVERFLOW = 8 (deprecated)
    // SQLITE_STATUS_SCRATCH_SIZE = 9 (deprecated)
    int status_op = fdp.ConsumeIntegralInRange<int>(0, 9);
    
    switch (operation) {
        case 0: {
            // Test heap limit functions with various values
            
            // First query current limits (negative values for query mode)
            sqlite3_int64 current_soft_limit = sqlite3_soft_heap_limit64(-1);
            sqlite3_int64 current_hard_limit = sqlite3_hard_heap_limit64(-1);
            
            // Set soft heap limit using deprecated interface (32-bit)
            // The parameter is int, not sqlite3_int64
            int soft_limit_32 = fdp.ConsumeIntegral<int>();
            sqlite3_soft_heap_limit(soft_limit_32);
            
            // Set soft heap limit using 64-bit interface
            sqlite3_int64 new_soft_limit = sqlite3_soft_heap_limit64(soft_limit_val);
            
            // Set hard heap limit using 64-bit interface
            sqlite3_int64 new_hard_limit = sqlite3_hard_heap_limit64(hard_limit_val);
            
            // Query limits again to see if they changed
            sqlite3_int64 updated_soft_limit = sqlite3_soft_heap_limit64(-1);
            sqlite3_int64 updated_hard_limit = sqlite3_hard_heap_limit64(-1);
            
            // Test edge cases: zero limits (disables limiter)
            sqlite3_soft_heap_limit64(0);
            sqlite3_hard_heap_limit64(0);
            
            // Test negative values (should not change limits, just query)
            sqlite3_soft_heap_limit64(-100);
            sqlite3_hard_heap_limit64(-100);
            
            // Restore original limits (if we can determine them)
            // Note: We don't have the original values, but we can set reasonable defaults
            sqlite3_soft_heap_limit64(1000000); // 1MB default
            sqlite3_hard_heap_limit64(2000000); // 2MB default
            
            break;
        }
        
        case 1: {
            // Test memory usage monitoring functions
            
            // Get current memory usage
            sqlite3_int64 current_memory = sqlite3_memory_used();
            
            // Get memory highwater mark with optional reset
            sqlite3_int64 highwater = sqlite3_memory_highwater(reset_flag);
            
            // Test with reset flag = 0 (don't reset)
            sqlite3_int64 highwater_no_reset = sqlite3_memory_highwater(0);
            
            // Test with reset flag = 1 (reset after reading)
            sqlite3_int64 highwater_with_reset = sqlite3_memory_highwater(1);
            
            // Test edge case: call multiple times to see if values change
            for (int i = 0; i < 3; i++) {
                sqlite3_memory_used();
                sqlite3_memory_highwater(0);
            }
            
            // Try to allocate some memory to affect memory usage
            // Note: We need to be careful with heap limits set in other operations
            void* test_alloc = sqlite3_malloc(100);
            if (test_alloc != nullptr) {
                sqlite3_int64 memory_after_alloc = sqlite3_memory_used();
                sqlite3_free(test_alloc);
                sqlite3_int64 memory_after_free = sqlite3_memory_used();
            }
            
            break;
        }
        
        case 2: {
            // Test status functions with various operation codes
            
            int current_val = 0;
            int highwater_val = 0;
            sqlite3_int64 current_val64 = 0;
            sqlite3_int64 highwater_val64 = 0;
            
            // Test sqlite3_status() with 32-bit values
            int status_result = sqlite3_status(status_op, &current_val, &highwater_val, reset_flag);
            
            // Test sqlite3_status64() with 64-bit values
            int status_result64 = sqlite3_status64(status_op, &current_val64, &highwater_val64, reset_flag);
            
            // Test all defined status operation codes
            for (int op = 0; op <= 6; op++) {
                int dummy_current = 0;
                int dummy_highwater = 0;
                sqlite3_int64 dummy_current64 = 0;
                sqlite3_int64 dummy_highwater64 = 0;
                
                sqlite3_status(op, &dummy_current, &dummy_highwater, 0);
                sqlite3_status64(op, &dummy_current64, &dummy_highwater64, 0);
            }
            
            // Test edge cases: invalid operation codes
            // SQLite may handle invalid op codes gracefully or return error
            int invalid_op1 = 100;
            int invalid_op2 = -1;
            int dummy1 = 0, dummy2 = 0;
            sqlite3_int64 dummy64_1 = 0, dummy64_2 = 0;
            
            sqlite3_status(invalid_op1, &dummy1, &dummy2, 0);
            sqlite3_status(invalid_op2, &dummy1, &dummy2, 0);
            sqlite3_status64(invalid_op1, &dummy64_1, &dummy64_2, 0);
            sqlite3_status64(invalid_op2, &dummy64_1, &dummy64_2, 0);
            
            break;
        }
        
        case 3: {
            // Test comprehensive memory management workflow
            // This follows the exact invocation sequence from coverage guidance
            
            // 1. sqlite3_initialize() already called at start
            
            // 2. Call sqlite3_soft_heap_limit() to set soft heap limit
            int soft_limit_input = fdp.ConsumeIntegralInRange<int>(-1000000, 1000000);
            sqlite3_soft_heap_limit(soft_limit_input);
            
            // 3. Call sqlite3_hard_heap_limit64() to set hard heap limit
            sqlite3_int64 hard_limit_input = fdp.ConsumeIntegral<sqlite3_int64>();
            sqlite3_int64 hard_limit_result = sqlite3_hard_heap_limit64(hard_limit_input);
            
            // 4. Call sqlite3_soft_heap_limit64() to set soft heap limit with 64-bit parameter
            sqlite3_int64 soft_limit64_input = fdp.ConsumeIntegral<sqlite3_int64>();
            sqlite3_int64 soft_limit64_result = sqlite3_soft_heap_limit64(soft_limit64_input);
            
            // 5. Call sqlite3_memory_highwater() to check peak memory usage
            int reset_flag2 = fdp.ConsumeIntegral<int>() & 1;
            sqlite3_int64 highwater = sqlite3_memory_highwater(reset_flag2);
            
            // 6. Call sqlite3_memory_used() to check current memory usage
            sqlite3_int64 memory_used = sqlite3_memory_used();
            
            // 7. Call sqlite3_status() and sqlite3_status64() to query memory status information
            int status_op1 = fdp.ConsumeIntegralInRange<int>(0, 6);
            int status_op2 = fdp.ConsumeIntegralInRange<int>(0, 6);
            
            int current1 = 0, highwater1 = 0;
            sqlite3_int64 current64_1 = 0, highwater64_1 = 0;
            int current2 = 0, highwater2 = 0;
            sqlite3_int64 current64_2 = 0, highwater64_2 = 0;
            
            sqlite3_status(status_op1, &current1, &highwater1, 0);
            sqlite3_status64(status_op1, &current64_1, &highwater64_1, 1);
            sqlite3_status(status_op2, &current2, &highwater2, 0);
            sqlite3_status64(status_op2, &current64_2, &highwater64_2, 1);
            
            // 8. Call sqlite3_shutdown() to cleanup (will be called at end)
            
            // Test interaction: allocate memory after setting limits
            size_t alloc_size = fdp.ConsumeIntegralInRange<size_t>(1, 10000);
            void* test_mem = sqlite3_malloc(alloc_size);
            if (test_mem != nullptr) {
                // Check memory usage after allocation
                sqlite3_int64 memory_after_alloc = sqlite3_memory_used();
                sqlite3_free(test_mem);
            }
            
            break;
        }
    }
    
    // Final cleanup: shutdown SQLite library
    sqlite3_shutdown();
    
    return 0;
}

} // namespace harness_019
