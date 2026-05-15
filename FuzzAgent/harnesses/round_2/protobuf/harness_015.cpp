#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include "absl/strings/cord.h"
#include "absl/strings/string_view.h"

namespace harness_015 {
// Harness for absl::Cord core operations - memory estimation, equality, swapping, flattening, prefix checking
// Targets the single most critical coverage gap in absl::Cord operations with 40+ undiscovered branches across 7 high-complexity APIs
// APIs: Cord memory estimation (EstimatedMemoryUsage), equality comparisons (operator==), swapping (swap), 
//       flattening (Flatten), prefix checking (StartsWith), and other core Cord operations
// Semantic differentiation from existing harnesses 000-014:
// - harness_000: binary parsing and serialization of message objects (high-level)
// - harness_001: text format parsing and printing with Tokenizer
// - harness_002: Cord-based I/O streaming with compression (CordOutputStream/InputStream) - I/O layer, NOT core Cord ops
// - harness_003: SymbolChecker visibility rules validation
// - harness_004: JSON parsing/serialization for web/REST APIs
// - harness_005: Cord I/O operations with CodedInputStream/OutputStream and stream backup functions - I/O layer
// - harness_006: File-based I/O operations with Gzip compression
// - harness_007: CodedStream abstraction layer for wire format handling with Cord read/write operations - I/O layer
// - harness_008: Modern absl::Cord integration focusing on zero-copy string handling with EpsCopyOutputStream - I/O layer
// - harness_009: CodedOutputStream static ToArray methods with Cord support - I/O layer
// - harness_010: ZeroCopyInputStream composition patterns (ConcatenatingInputStream, LimitingInputStream)
// - harness_011: EpsCopyOutputStream::WriteString with Cord parameter - I/O layer
// - harness_012: COMPREHENSIVE CodedInputStream low-level stream parsing operations
// - harness_013: Arena alignment utilities and allocation policies
// - harness_014: FOCUSED EXCLUSIVELY on CodedInputStream recursion depth management APIs
// - harness_015: FOCUSED EXCLUSIVELY on absl::Cord CORE OPERATIONS - memory estimation, equality, swapping, flattening, prefix checking
//   This harness tests the actual absl::Cord class methods, not protobuf I/O wrappers


// Include absl::Cord header for core operations

// Note: This harness does NOT include protobuf headers as it focuses on absl::Cord itself
// This is a key semantic differentiation from previous harnesses that used Cord through protobuf I/O streams

using absl::Cord;
using absl::CordMemoryAccounting;
using absl::string_view;

// Helper function to create Cords with different fragmentation patterns
void CreateTestCords(FuzzedDataProvider& fdp, std::vector<Cord>& cords) {
    // Clear any existing cords
    cords.clear();
    
    // Determine how many cords to create (1-4)
    size_t num_cords = fdp.ConsumeIntegralInRange<size_t>(1, 4);
    
    for (size_t i = 0; i < num_cords; ++i) {
        if (fdp.remaining_bytes() < 16) break;
        
        // Consume configuration for this cord
        bool make_empty = fdp.ConsumeBool();
        bool make_fragmented = fdp.ConsumeBool();
        bool make_from_string = fdp.ConsumeBool();
        bool make_from_view = fdp.ConsumeBool();
        
        if (make_empty) {
            // Empty cord
            cords.emplace_back();
            continue;
        }
        
        // Consume data for cord content
        size_t content_size = fdp.ConsumeIntegralInRange<size_t>(1, std::min<size_t>(fdp.remaining_bytes(), 4096));
        if (content_size == 0) {
            cords.emplace_back();
            continue;
        }
        
        std::string content = fdp.ConsumeBytesAsString(content_size);
        
        if (make_fragmented) {
            // Create fragmented cord by appending in chunks
            Cord fragmented_cord;
            size_t pos = 0;
            size_t chunks = fdp.ConsumeIntegralInRange<size_t>(2, 8);
            
            for (size_t chunk_idx = 0; chunk_idx < chunks && pos < content.size(); ++chunk_idx) {
                size_t chunk_size;
                if (chunk_idx == chunks - 1) {
                    chunk_size = content.size() - pos;
                } else {
                    chunk_size = fdp.ConsumeIntegralInRange<size_t>(1, content.size() - pos);
                }
                
                if (chunk_size > 0) {
                    fragmented_cord.Append(string_view(content.data() + pos, chunk_size));
                    pos += chunk_size;
                }
            }
            cords.push_back(std::move(fragmented_cord));
        } 
        else if (make_from_string) {
            // Create cord from std::string (explicit constructor)
            cords.emplace_back(content);
        }
        else if (make_from_view) {
            // Create cord from string_view
            cords.emplace_back(string_view(content));
        }
        else {
            // Default: create cord from string
            cords.emplace_back(content);
        }
    }
}

// Test Cord memory estimation with different accounting modes
void TestMemoryEstimation(const std::vector<Cord>& cords, FuzzedDataProvider& fdp) {
    for (const auto& cord : cords) {
        if (cord.empty()) continue;
        
        // Test different memory accounting modes
        uint8_t accounting_mode = fdp.ConsumeIntegral<uint8_t>() % 3;
        CordMemoryAccounting mode;
        
        switch (accounting_mode) {
            case 0: mode = CordMemoryAccounting::kTotal; break;
            case 1: mode = CordMemoryAccounting::kTotalMorePrecise; break;
            case 2: mode = CordMemoryAccounting::kFairShare; break;
            default: mode = CordMemoryAccounting::kTotal; break;
        }
        
        size_t estimated_memory = cord.EstimatedMemoryUsage(mode);
        
        // Also test default parameter (kTotal)
        size_t default_estimated_memory = cord.EstimatedMemoryUsage();
        
        // These are estimates, so we don't validate specific values
        // Just calling the API exercises the coverage paths
    }
}

// Test Cord equality operations
void TestEqualityOperations(std::vector<Cord>& cords, FuzzedDataProvider& fdp) {
    if (cords.size() < 2) return;
    
    // Test cord-to-cord equality
    for (size_t i = 0; i < cords.size() - 1; ++i) {
        for (size_t j = i + 1; j < cords.size(); ++j) {
            bool are_equal = (cords[i] == cords[j]);
            bool are_not_equal = (cords[i] != cords[j]);
            
            // Test symmetry
            bool symmetric_equal = (cords[j] == cords[i]);
            bool symmetric_not_equal = (cords[j] != cords[i]);
            
            // These should be consistent
            // Just calling exercises the coverage
        }
    }
    
    // Test cord-to-string_view equality
    for (const auto& cord : cords) {
        if (cord.empty()) continue;
        
        // Convert cord to string for comparison
        std::string cord_str;
        absl::CopyCordToString(cord, &cord_str);
        
        // Test with string_view of the entire string
        bool equals_full = (cord == string_view(cord_str));
        
        // Test with string_view of prefix
        if (cord_str.size() > 1) {
            size_t prefix_len = fdp.ConsumeIntegralInRange<size_t>(1, cord_str.size() - 1);
            bool equals_prefix = (cord == string_view(cord_str.data(), prefix_len));
        }
        
        // Test with different string
        if (fdp.remaining_bytes() > 16) {
            std::string different_str = fdp.ConsumeRandomLengthString(32);
            bool equals_different = (cord == string_view(different_str));
        }
    }
    
    // Test string_view-to-cord equality (reverse operator)
    for (const auto& cord : cords) {
        if (cord.empty()) continue;
        
        std::string cord_str;
        absl::CopyCordToString(cord, &cord_str);
        
        // Test reverse operator
        bool reverse_equals = (string_view(cord_str) == cord);
    }
}

// Test Cord swapping operations
void TestSwappingOperations(std::vector<Cord>& cords, FuzzedDataProvider& fdp) {
    if (cords.size() < 2) return;
    
    // Test member swap()
    for (size_t i = 0; i < cords.size() - 1; i += 2) {
        Cord cord1 = cords[i];
        Cord cord2 = cords[i + 1];
        
        // Get initial states
        std::string before1, before2;
        absl::CopyCordToString(cord1, &before1);
        absl::CopyCordToString(cord2, &before2);
        
        // Perform swap
        cord1.swap(cord2);
        
        // Get states after swap
        std::string after1, after2;
        absl::CopyCordToString(cord1, &after1);
        absl::CopyCordToString(cord2, &after2);
        
        // Verify swap worked (content should be swapped)
        // Note: We don't assert, just exercise the API
    }
    
    // Test friend swap() function
    if (cords.size() >= 2) {
        Cord cord1 = cords[0];
        Cord cord2 = cords[1];
        
        std::string before1, before2;
        absl::CopyCordToString(cord1, &before1);
        absl::CopyCordToString(cord2, &before2);
        
        // Use friend swap function
        using std::swap;
        swap(cord1, cord2);
        
        std::string after1, after2;
        absl::CopyCordToString(cord1, &after1);
        absl::CopyCordToString(cord2, &after2);
    }
}

// Test Cord flattening operations
void TestFlatteningOperations(std::vector<Cord>& cords, FuzzedDataProvider& fdp) {
    for (auto& cord : cords) {
        if (cord.empty()) continue;
        
        // Test Flatten() method
        string_view flattened = cord.Flatten();
        
        // The flattened view should be valid
        size_t flattened_size = flattened.size();
        
        // Compare with cord size
        size_t cord_size = cord.size();
        
        // They should match
        // Note: Flatten() may or may not actually copy depending on cord structure
        
        // Use the flattened data in some way
        if (flattened_size > 0) {
            // Access first and last characters
            char first_char = flattened[0];
            char last_char = flattened[flattened_size - 1];
            
            // Create string from flattened view
            std::string from_flattened(flattened.data(), flattened_size);
        }
    }
}

// Test Cord prefix checking (StartsWith)
void TestPrefixChecking(std::vector<Cord>& cords, FuzzedDataProvider& fdp) {
    for (const auto& cord : cords) {
        if (cord.empty()) continue;
        
        // Convert cord to string to get prefixes
        std::string cord_str;
        absl::CopyCordToString(cord, &cord_str);
        
        // Test with various prefixes
        if (cord_str.size() > 1) {
            // Test with actual prefix of the cord
            size_t prefix_len = fdp.ConsumeIntegralInRange<size_t>(1, cord_str.size());
            string_view actual_prefix(cord_str.data(), prefix_len);
            bool starts_with_actual = cord.StartsWith(actual_prefix);
            
            // Should be true for actual prefix (unless empty cord)
        }
        
        // Test with random string prefix
        if (fdp.remaining_bytes() > 8) {
            std::string random_prefix = fdp.ConsumeRandomLengthString(16);
            bool starts_with_random = cord.StartsWith(string_view(random_prefix));
        }
        
        // Test with empty prefix (should always return true)
        bool starts_with_empty = cord.StartsWith(string_view());
    }
}

// Test additional Cord operations for comprehensive coverage
void TestAdditionalCordOperations(std::vector<Cord>& cords, FuzzedDataProvider& fdp) {
    for (size_t i = 0; i < cords.size(); ++i) {
        // Test copy constructor and assignment
        if (i < cords.size() - 1) {
            Cord copy_constructed(cords[i]);
            Cord assigned;
            assigned = cords[i + 1];
            
            // Test move operations
            Cord moved_from = std::move(cords[i]);
            Cord move_assigned;
            move_assigned = std::move(cords[i + 1]);
            
            // Put them back for other tests
            cords[i] = std::move(moved_from);
            if (i + 1 < cords.size()) {
                cords[i + 1] = std::move(move_assigned);
            }
        }
        
        // Test size() and empty()
        size_t cord_size = cords[i].size();
        bool is_empty = cords[i].empty();
        
        // Test hash (if we have a hasher)
        // Note: absl::Cord can be used with absl::Hash
        
        // Test comparison operators if available
        if (i < cords.size() - 1) {
            // Test <, >, <=, >= if Cord defines them
            // Note: Cord might not define all comparison operators
        }
    }
}

// Main fuzzer entry point
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for meaningful Cord testing
    if (size < 64) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Create test cords with various fragmentation patterns
    std::vector<Cord> test_cords;
    CreateTestCords(fdp, test_cords);
    
    if (test_cords.empty()) {
        // Still test with at least an empty cord
        test_cords.emplace_back();
    }
    
    // Determine which operations to test based on remaining input
    uint8_t operation_mask = fdp.ConsumeIntegral<uint8_t>();
    
    // Test memory estimation (40+ undiscovered branches)
    if (operation_mask & 0x01) {
        TestMemoryEstimation(test_cords, fdp);
    }
    
    // Test equality operations
    if (operation_mask & 0x02) {
        TestEqualityOperations(test_cords, fdp);
    }
    
    // Test swapping operations
    if (operation_mask & 0x04) {
        TestSwappingOperations(test_cords, fdp);
    }
    
    // Test flattening operations
    if (operation_mask & 0x08) {
        TestFlatteningOperations(test_cords, fdp);
    }
    
    // Test prefix checking operations
    if (operation_mask & 0x10) {
        TestPrefixChecking(test_cords, fdp);
    }
    
    // Test additional Cord operations
    if (operation_mask & 0x20) {
        TestAdditionalCordOperations(test_cords, fdp);
    }
    
    // If no specific operation selected, test a comprehensive sequence
    if (operation_mask == 0) {
        TestMemoryEstimation(test_cords, fdp);
        TestEqualityOperations(test_cords, fdp);
        TestSwappingOperations(test_cords, fdp);
        TestFlatteningOperations(test_cords, fdp);
        TestPrefixChecking(test_cords, fdp);
    }
    
    return 0;
}

} // namespace harness_015
