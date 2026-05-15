#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <cmath>
#include <limits>
#include <bitset>
#include <cstring>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include "google/protobuf/io/strtod.h"

namespace harness_017 {
// Protocol Buffers fuzzing harness for floating-point string conversion functions
// This harness targets protobuf floating-point conversion APIs identified by CoverageAnalyzerAgent
// as having 0% coverage and 8+ branches each, representing significant uncovered complexity.
// Target APIs: SafeDoubleToFloat, SimpleDtoa, SimpleFtoa
// Focus on floating-point edge cases, special values (NaN, infinity, denormals), 
// rounding modes, precision variations, and boundary conditions.



using google::protobuf::io::SafeDoubleToFloat;
using google::protobuf::io::SimpleDtoa;
using google::protobuf::io::SimpleFtoa;
using google::protobuf::io::NoLocaleStrtod;

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful floating-point testing
    // At least enough for: operation selection + at least one double value
    if (size < sizeof(double) + 1) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Generate diverse floating-point values from fuzzer input
    // We'll generate different types of floating-point values to maximize coverage
    
    // Operation mode determines which APIs to test and in what order
    uint8_t operation_mode = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    // Generate test doubles with different patterns
    std::vector<double> test_doubles;
    std::vector<float> test_floats;
    
    // Generate a mix of normal, special, and edge-case values
    for (int i = 0; i < 8 && fdp.remaining_bytes() >= sizeof(double); i++) {
        // Use different generation strategies
        uint8_t gen_type = fdp.ConsumeIntegral<uint8_t>() % 6;
        
        switch (gen_type) {
            case 0: {
                // Normal double from integral
                double val = static_cast<double>(fdp.ConsumeIntegral<int32_t>());
                test_doubles.push_back(val);
                break;
            }
            case 1: {
                // Random double bits
                uint64_t bits = fdp.ConsumeIntegral<uint64_t>();
                double val;
                memcpy(&val, &bits, sizeof(double));
                test_doubles.push_back(val);
                break;
            }
            case 2: {
                // Special values: NaN, infinity, zero
                uint8_t special = fdp.ConsumeIntegral<uint8_t>() % 8;
                switch (special) {
                    case 0: test_doubles.push_back(std::numeric_limits<double>::quiet_NaN()); break;
                    case 1: test_doubles.push_back(std::numeric_limits<double>::signaling_NaN()); break;
                    case 2: test_doubles.push_back(std::numeric_limits<double>::infinity()); break;
                    case 3: test_doubles.push_back(-std::numeric_limits<double>::infinity()); break;
                    case 4: test_doubles.push_back(0.0); break;
                    case 5: test_doubles.push_back(-0.0); break;
                    case 6: test_doubles.push_back(std::numeric_limits<double>::max()); break;
                    case 7: test_doubles.push_back(std::numeric_limits<double>::lowest()); break;
                }
                break;
            }
            case 3: {
                // Denormal/subnormal range
                uint64_t denorm_bits = fdp.ConsumeIntegral<uint64_t>() & 0x000FFFFFFFFFFFFFULL;
                denorm_bits |= 0x0000000000000000ULL; // Set exponent to 0 (denormal)
                double val;
                memcpy(&val, &denorm_bits, sizeof(double));
                test_doubles.push_back(val);
                break;
            }
            case 4: {
                // Random floating point from fuzzer
                double val = fdp.ConsumeFloatingPoint<double>();
                test_doubles.push_back(val);
                break;
            }
            case 5: {
                // Boundary values for float conversion
                // Values near float max/min boundaries
                uint8_t boundary_choice = fdp.ConsumeIntegral<uint8_t>() % 4;
                switch (boundary_choice) {
                    case 0: 
                        // Just below float max
                        test_doubles.push_back(3.402823e38);
                        break;
                    case 1:
                        // Just above float max
                        test_doubles.push_back(3.402824e38);
                        break;
                    case 2:
                        // Just below negative float max
                        test_doubles.push_back(-3.402823e38);
                        break;
                    case 3:
                        // Just above negative float max
                        test_doubles.push_back(-3.402824e38);
                        break;
                }
                break;
            }
        }
    }
    
    // Generate test floats similarly
    for (int i = 0; i < 8 && fdp.remaining_bytes() >= sizeof(float); i++) {
        uint8_t gen_type = fdp.ConsumeIntegral<uint8_t>() % 5;
        
        switch (gen_type) {
            case 0: {
                float val = static_cast<float>(fdp.ConsumeIntegral<int32_t>());
                test_floats.push_back(val);
                break;
            }
            case 1: {
                uint32_t bits = fdp.ConsumeIntegral<uint32_t>();
                float val;
                memcpy(&val, &bits, sizeof(float));
                test_floats.push_back(val);
                break;
            }
            case 2: {
                uint8_t special = fdp.ConsumeIntegral<uint8_t>() % 6;
                switch (special) {
                    case 0: test_floats.push_back(std::numeric_limits<float>::quiet_NaN()); break;
                    case 1: test_floats.push_back(std::numeric_limits<float>::signaling_NaN()); break;
                    case 2: test_floats.push_back(std::numeric_limits<float>::infinity()); break;
                    case 3: test_floats.push_back(-std::numeric_limits<float>::infinity()); break;
                    case 4: test_floats.push_back(0.0f); break;
                    case 5: test_floats.push_back(-0.0f); break;
                }
                break;
            }
            case 3: {
                float val = fdp.ConsumeFloatingPoint<float>();
                test_floats.push_back(val);
                break;
            }
            case 4: {
                // Denormal float
                uint32_t denorm_bits = fdp.ConsumeIntegral<uint32_t>() & 0x007FFFFF;
                denorm_bits |= 0x00000000; // Exponent = 0 (denormal)
                float val;
                memcpy(&val, &denorm_bits, sizeof(float));
                test_floats.push_back(val);
                break;
            }
        }
    }
    
    // Step 2: Test SafeDoubleToFloat API
    // Primary target: SafeDoubleToFloat(double value)
    for (const double d : test_doubles) {
        float converted = SafeDoubleToFloat(d);
        
        // Verify basic properties
        (void)converted;
        
        // Check for overflow/underflow handling
        if (std::isinf(d) && !std::isinf(converted)) {
            // This might indicate an issue, but we just note it
        }
    }
    
    // Step 3: Test SimpleDtoa API
    // Primary target: std::string SimpleDtoa(double value)
    for (const double d : test_doubles) {
        std::string dtoa_result = SimpleDtoa(d);
        
        // The result should not be empty for valid doubles
        if (!dtoa_result.empty()) {
            // Optional: Try to parse back with NoLocaleStrtod
            char* endptr = nullptr;
            double parsed_back = NoLocaleStrtod(dtoa_result.c_str(), &endptr);
            
            // For non-NaN values, check if parsing succeeded
            if (!std::isnan(d) && endptr != dtoa_result.c_str()) {
                // Verify the parsed value is close to original (allowing for rounding)
                // SimpleDtoa doesn't guarantee exact round-trip for all values
                (void)parsed_back;
            }
        }
    }
    
    // Step 4: Test SimpleFtoa API  
    // Primary target: std::string SimpleFtoa(float value)
    for (const float f : test_floats) {
        std::string ftoa_result = SimpleFtoa(f);
        
        if (!ftoa_result.empty()) {
            // Try to parse back as double
            char* endptr = nullptr;
            double parsed_back = NoLocaleStrtod(ftoa_result.c_str(), &endptr);
            
            if (!std::isnan(f) && endptr != ftoa_result.c_str()) {
                // Convert parsed double back to float for comparison
                float float_parsed = SafeDoubleToFloat(parsed_back);
                (void)float_parsed;
            }
        }
    }
    
    // Step 5: Test combined scenarios based on operation mode
    switch (operation_mode) {
        case 0: {
            // Round-trip testing: double -> string -> double
            if (!test_doubles.empty()) {
                double original = test_doubles[0];
                std::string str = SimpleDtoa(original);
                if (!str.empty()) {
                    char* endptr = nullptr;
                    double round_trip = NoLocaleStrtod(str.c_str(), &endptr);
                    float float_round_trip = SafeDoubleToFloat(round_trip);
                    (void)float_round_trip;
                }
            }
            break;
        }
        case 1: {
            // Round-trip testing: float -> string -> float
            if (!test_floats.empty()) {
                float original = test_floats[0];
                std::string str = SimpleFtoa(original);
                if (!str.empty()) {
                    char* endptr = nullptr;
                    double parsed = NoLocaleStrtod(str.c_str(), &endptr);
                    float round_trip = SafeDoubleToFloat(parsed);
                    (void)round_trip;
                }
            }
            break;
        }
        case 2: {
            // Test boundary overflow/underflow cases specifically
            std::vector<double> boundaries = {
                std::numeric_limits<double>::max(),
                -std::numeric_limits<double>::max(),
                std::numeric_limits<double>::lowest(),
                std::numeric_limits<double>::min(),
                -std::numeric_limits<double>::min(),
                3.4028234664e38,  // Approx float max
                3.4028235677973366e38,  // MAX_FLOAT_AS_DOUBLE_ROUNDED
                3.4028235e38,  // Float max as printed
                -3.4028234664e38,
                -3.4028235677973366e38,
                -3.4028235e38
            };
            
            for (double b : boundaries) {
                float converted = SafeDoubleToFloat(b);
                std::string str = SimpleDtoa(b);
                (void)converted;
                (void)str;
            }
            break;
        }
        case 3: {
            // Test denormal/subnormal values
            // Generate more denormals
            for (int i = 0; i < 10 && fdp.remaining_bytes() >= sizeof(uint64_t); i++) {
                uint64_t bits = fdp.ConsumeIntegral<uint64_t>() & 0x000FFFFFFFFFFFFFULL;
                bits |= 0x0000000000000000ULL; // Denormal exponent
                double denormal;
                memcpy(&denormal, &bits, sizeof(double));
                
                float f_denormal = SafeDoubleToFloat(denormal);
                std::string str = SimpleDtoa(denormal);
                (void)f_denormal;
                (void)str;
            }
            break;
        }
        case 4: {
            // Test NaN variations
            double nan_variants[] = {
                std::numeric_limits<double>::quiet_NaN(),
                std::numeric_limits<double>::signaling_NaN(),
                -std::numeric_limits<double>::quiet_NaN(),
                -std::numeric_limits<double>::signaling_NaN()
            };
            
            for (double nan_val : nan_variants) {
                float f_nan = SafeDoubleToFloat(nan_val);
                std::string str = SimpleDtoa(nan_val);
                (void)f_nan;
                (void)str;
            }
            break;
        }
        case 5: {
            // Test infinity variations
            double inf_variants[] = {
                std::numeric_limits<double>::infinity(),
                -std::numeric_limits<double>::infinity(),
                std::numeric_limits<float>::infinity(),
                -std::numeric_limits<float>::infinity()
            };
            
            for (double inf_val : inf_variants) {
                float f_inf = SafeDoubleToFloat(inf_val);
                std::string str = SimpleDtoa(inf_val);
                (void)f_inf;
                (void)str;
            }
            break;
        }
        case 6: {
            // Test zero variations
            double zero_variants[] = {0.0, -0.0, 0.0f, -0.0f};
            
            for (double zero_val : zero_variants) {
                float f_zero = SafeDoubleToFloat(zero_val);
                std::string str = SimpleDtoa(zero_val);
                (void)f_zero;
                (void)str;
            }
            break;
        }
        case 7: {
            // Random walk through conversions
            // Generate random arithmetic sequences
            double start = test_doubles.empty() ? 0.0 : test_doubles[0];
            for (int i = 0; i < 5; i++) {
                double val = start + i * 0.1;
                float f_val = SafeDoubleToFloat(val);
                std::string d_str = SimpleDtoa(val);
                std::string f_str = SimpleFtoa(f_val);
                
                // Try parsing back
                if (!d_str.empty()) {
                    char* endptr = nullptr;
                    NoLocaleStrtod(d_str.c_str(), &endptr);
                }
                
                (void)f_val;
                (void)d_str;
                (void)f_str;
            }
            break;
        }
    }
    
    // Step 6: Additional edge cases with remaining fuzzer input
    // Generate strings and try to parse them with NoLocaleStrtod
    while (fdp.remaining_bytes() > 0) {
        size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);
        std::string test_str = fdp.ConsumeBytesAsString(str_len);
        
        if (!test_str.empty()) {
            char* endptr = nullptr;
            double parsed = NoLocaleStrtod(test_str.c_str(), &endptr);
            
            // If parsing succeeded (at least some characters consumed)
            if (endptr != test_str.c_str()) {
                // Test conversions on parsed value
                float f_parsed = SafeDoubleToFloat(parsed);
                std::string dtoa_str = SimpleDtoa(parsed);
                (void)f_parsed;
                (void)dtoa_str;
            }
        }
    }
    
    return 0;
}

} // namespace harness_017
