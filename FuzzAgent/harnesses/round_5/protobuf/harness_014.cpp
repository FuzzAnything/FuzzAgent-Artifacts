#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <algorithm>
#include <limits>
#include <cmath>
#include <fuzzer/FuzzedDataProvider.h>
#include "google/protobuf/arena.h"
#include "google/protobuf/arena_align.h"
#include "google/protobuf/arena_allocation_policy.h"

namespace harness_014 {
// Protocol Buffers fuzzing harness for Arena memory alignment and allocation operations
// This harness targets uncovered Arena alignment APIs identified in coverage guidance:
// - _ZN6google8protobuf8internal12ArenaAlignAsEm (18 undiscovered branches)
// - _ZNK6google8protobuf8internal10ArenaAlign6PaddedEm (6 undiscovered branches)
// - _ZNK6google8protobuf8internal10ArenaAlign4CeilEm (Required Helper)
// - _ZNK6google8protobuf8internal10ArenaAlign5FloorEm (Required Helper)
// - _ZN6google8protobuf8internal16AllocationPolicy9IsDefaultEv (8 undiscovered branches)
// - _ZN6google8protobuf5ArenaC1Ev (Constructor)
// - _ZN6google8protobuf5ArenaD1Ev (Destructor)
// Focus on Arena memory alignment lifecycle with boundary conditions for alignment sizes



using google::protobuf::Arena;
using google::protobuf::internal::ArenaAlign;
using google::protobuf::internal::ArenaAlignAs;
using google::protobuf::internal::ArenaAlignDefault;
using google::protobuf::internal::AllocationPolicy;

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful Arena alignment operations
    // Required: enough bytes for alignment values, size parameters, and flags
    if (size < 32) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Create Arena object using constructor
    // Target API: _ZN6google8protobuf5ArenaC1Ev (Arena constructor)
    Arena arena;
    
    // Step 2: Test AllocationPolicy::IsDefault to understand current policy
    // First, get the allocation policy from arena
    // Note: Arena doesn't expose AllocationPolicy directly, but we can create
    // a default AllocationPolicy and test IsDefault()
    
    AllocationPolicy policy;
    // Target API: _ZN6google8protobuf8internal16AllocationPolicy9IsDefaultEv
    bool is_default_policy = policy.IsDefault();
    
    // Step 3: Test various alignment sizes with ArenaAlignAs
    // Generate alignment values from fuzzer input
    // Alignments must be powers of two >= 8
    
    // Consume alignment values from fuzzer input
    std::vector<size_t> alignments_to_test;
    
    // Test standard alignments (powers of two)
    size_t alignment_candidate = fdp.ConsumeIntegral<size_t>();
    
    // Ensure alignment is a valid power of two >= 8 for testing
    // Create multiple alignment candidates for testing
    
    // Test case 1: Small alignments (8, 16, 32, 64)
    uint8_t alignment_choice = fdp.ConsumeIntegral<uint8_t>() % 4;
    size_t small_alignments[] = {8, 16, 32, 64};
    size_t test_alignment1 = small_alignments[alignment_choice];
    
    // Target API: _ZN6google8protobuf8internal12ArenaAlignAsEm
    ArenaAlign align1 = ArenaAlignAs(test_alignment1);
    
    // Test case 2: Larger alignments (128, 256, 512, 1024)
    uint8_t alignment_choice2 = fdp.ConsumeIntegral<uint8_t>() % 4;
    size_t large_alignments[] = {128, 256, 512, 1024};
    size_t test_alignment2 = large_alignments[alignment_choice2];
    
    ArenaAlign align2 = ArenaAlignAs(test_alignment2);
    
    // Test case 3: Try to generate alignment from fuzzer input
    // Ensure it's a power of two and >= 8
    size_t raw_alignment = fdp.ConsumeIntegralInRange<size_t>(1, 2048);
    
    // Make it a power of two for valid alignment
    size_t valid_alignment = 8; // Default minimum
    if (raw_alignment >= 8) {
        // Find next power of two
        size_t pow2 = 1;
        while (pow2 < raw_alignment && pow2 < 2048) {
            pow2 <<= 1;
        }
        valid_alignment = std::max(pow2, static_cast<size_t>(8));
    }
    
    ArenaAlign align3 = ArenaAlignAs(valid_alignment);
    
    // Step 4: Test ArenaAlign::Padded with various size parameters
    // Consume size values from fuzzer input
    
    // Test Padded with small sizes
    size_t small_size = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
    
    // Target API: _ZNK6google8protobuf8internal10ArenaAlign6PaddedEm
    size_t padded1 = align1.Padded(small_size);
    size_t padded2 = align2.Padded(small_size);
    size_t padded3 = align3.Padded(small_size);
    
    // Test Padded with larger sizes
    size_t large_size = fdp.ConsumeIntegralInRange<size_t>(1025, 10000);
    size_t padded_large1 = align1.Padded(large_size);
    size_t padded_large2 = align2.Padded(large_size);
    
    // Test Padded with edge cases: size 0, size 1, maximum size
    size_t edge_size = fdp.ConsumeIntegralInRange<size_t>(0, 100);
    
    // For alignment testing, need aligned sizes for Padded()
    // Ensure size is aligned before calling Padded
    size_t aligned_edge_size = align1.Ceil(edge_size);
    if (aligned_edge_size > 0) {
        size_t padded_edge = align1.Padded(aligned_edge_size);
    }
    
    // Step 5: Test ArenaAlign::Ceil and ArenaAlign::Floor for boundary calculations
    // Consume various size values for Ceil/Floor operations
    
    // Test Ceil with various sizes
    size_t ceil_test_size1 = fdp.ConsumeIntegralInRange<size_t>(0, 1000);
    size_t ceil_test_size2 = fdp.ConsumeIntegralInRange<size_t>(1001, 10000);
    
    // Target API: _ZNK6google8protobuf8internal10ArenaAlign4CeilEm
    size_t ceil_result1 = align1.Ceil(ceil_test_size1);
    size_t ceil_result2 = align2.Ceil(ceil_test_size2);
    size_t ceil_result3 = align3.Ceil(ceil_test_size1);
    
    // Target API: _ZNK6google8protobuf8internal10ArenaAlign5FloorEm
    size_t floor_result1 = align1.Floor(ceil_test_size1);
    size_t floor_result2 = align2.Floor(ceil_test_size2);
    size_t floor_result3 = align3.Floor(ceil_test_size1);
    
    // Test with already aligned sizes (should return same value)
    size_t aligned_size1 = align1.Ceil(ceil_test_size1);
    size_t floor_aligned1 = align1.Floor(aligned_size1);
    
    // Test with misaligned sizes (boundary cases)
    // Create misaligned size: aligned_size - 1
    if (aligned_size1 > 0) {
        size_t misaligned_size = aligned_size1 - 1;
        size_t ceil_misaligned = align1.Ceil(misaligned_size);
        size_t floor_misaligned = align1.Floor(misaligned_size);
    }
    
    // Step 6: Test IsAligned methods
    // Test size alignment checking
    bool is_aligned_size1 = align1.IsAligned(ceil_result1);
    bool is_aligned_size2 = align2.IsAligned(ceil_result2);
    
    // Test pointer alignment (simulate with integer)
    uintptr_t test_pointer_value = fdp.ConsumeIntegral<uintptr_t>();
    void* test_pointer = reinterpret_cast<void*>(test_pointer_value);
    
    // Note: IsAligned for pointers is a template method, will be tested
    // through compilation but not necessarily executed
    
    // Step 7: Test default alignment
    // Test ArenaAlignDefault operations
    size_t default_ceil = ArenaAlignDefault::Ceil(ceil_test_size1);
    size_t default_floor = ArenaAlignDefault::Floor(ceil_test_size2);
    bool default_is_aligned = ArenaAlignDefault::IsAligned(default_ceil);
    
    // Test Padded on default alignment (should return same size for aligned input)
    size_t default_padded = ArenaAlignDefault::Padded(default_ceil);
    
    // Step 8: Test various boundary conditions
    
    // Test with zero size
    size_t zero_ceil = align1.Ceil(0);
    size_t zero_floor = align1.Floor(0);
    
    // Test with very large size (near size_t max)
    size_t near_max_size = std::numeric_limits<size_t>::max() - 
                          fdp.ConsumeIntegralInRange<size_t>(0, 1000);
    
    // Be careful with overflow in Ceil calculation
    if (near_max_size > align1.align) {
        size_t near_max_ceil = align1.Ceil(near_max_size - align1.align);
        size_t near_max_floor = align1.Floor(near_max_size - align1.align);
    }
    
    // Test power of two detection for alignment
    // Generate various values and check if they're valid for ArenaAlignAs
    
    // Step 9: Test alignment combinations
    // Test different alignments with same sizes
    std::vector<size_t> test_sizes;
    for (int i = 0; i < 5 && fdp.remaining_bytes() > sizeof(size_t); i++) {
        test_sizes.push_back(fdp.ConsumeIntegral<size_t>() % 4096);
    }
    
    for (size_t test_size : test_sizes) {
        // Test all three alignments with this size
        size_t ceil_a1 = align1.Ceil(test_size);
        size_t ceil_a2 = align2.Ceil(test_size);
        size_t ceil_a3 = align3.Ceil(test_size);
        
        size_t floor_a1 = align1.Floor(test_size);
        size_t floor_a2 = align2.Floor(test_size);
        size_t floor_a3 = align3.Floor(test_size);
        
        // Ensure size is aligned before calling Padded
        size_t aligned_test_size = align1.Ceil(test_size);
        if (aligned_test_size > 0) {
            size_t padded_a1 = align1.Padded(aligned_test_size);
        }
    }
    
    // Step 10: Arena will be destroyed automatically when out of scope
    // Target API: _ZN6google8protobuf5ArenaD1Ev (Arena destructor)
    // Destructor is called automatically when 'arena' goes out of scope
    
    // Additional test: Create Arena with custom options
    if (fdp.remaining_bytes() > sizeof(size_t) * 2) {
        size_t initial_block_size = fdp.ConsumeIntegral<size_t>() % 4096;
        size_t max_block_size = fdp.ConsumeIntegral<size_t>() % 8192;
        
        // Create custom policy
        AllocationPolicy custom_policy;
        custom_policy.start_block_size = initial_block_size;
        custom_policy.max_block_size = max_block_size;
        
        // Test IsDefault on custom policy
        bool custom_is_default = custom_policy.IsDefault();
    }
    
    return 0;
}

} // namespace harness_014
