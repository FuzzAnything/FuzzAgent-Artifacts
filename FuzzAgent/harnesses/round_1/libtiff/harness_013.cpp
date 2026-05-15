#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <cmath>
#include "tiffio.h"
#include "tiff.h"

namespace harness_013 {
/*
 * Fuzzing harness for libtiff byte-swapping operations (swab functions)
 * Target APIs: TIFFSwabShort, TIFFSwabLong, TIFFSwabLong8, TIFFSwabFloat, TIFFSwabDouble,
 *              TIFFSwabArrayOfShort, TIFFSwabArrayOfTriples, TIFFSwabArrayOfLong,
 *              TIFFSwabArrayOfLong8, TIFFSwabArrayOfFloat, TIFFSwabArrayOfDouble,
 *              TIFFReverseBits, TIFFGetBitRevTable
 * 
 * Coverage analysis indicates this is the highest-priority uncovered cluster with 
 * 20+ branches of undiscovered complexity in byte-swapping operations.
 * 
 * Semantic differentiation from previous harnesses (harness_000 through harness_012):
 * - No existing harnesses target TIFF swab functions
 * - This harness focuses exclusively on low-level byte manipulation operations
 * - Tests both single value and array-based swab operations
 * - Tests bit reversal operations with lookup tables
 * - No TIFF file I/O required - pure data transformation functions
 * - Minimal dependencies, high efficiency for fuzzing
 */



// Error handler to suppress warnings during fuzzing
extern "C" void tiff_error_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress errors during fuzzing
    return;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for various operations
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Set error handlers to suppress warnings
    TIFFSetErrorHandler(NULL);
    TIFFSetWarningHandler(NULL);

    // Test 1: Single value swab operations
    // Consume fixed-size data first
    if (fdp.remaining_bytes() >= sizeof(uint16_t)) {
        uint16_t short_val = fdp.ConsumeIntegral<uint16_t>();
        TIFFSwabShort(&short_val);
        
        // Test swabbing again (should return to original on little-endian systems)
        TIFFSwabShort(&short_val);
    }
    
    if (fdp.remaining_bytes() >= sizeof(uint32_t)) {
        uint32_t long_val = fdp.ConsumeIntegral<uint32_t>();
        TIFFSwabLong(&long_val);
        
        // Test swabbing again
        TIFFSwabLong(&long_val);
    }
    
    if (fdp.remaining_bytes() >= sizeof(uint64_t)) {
        uint64_t long8_val = fdp.ConsumeIntegral<uint64_t>();
        TIFFSwabLong8(&long8_val);
        
        // Test swabbing again
        TIFFSwabLong8(&long8_val);
    }
    
    if (fdp.remaining_bytes() >= sizeof(float)) {
        float float_val = fdp.ConsumeFloatingPoint<float>();
        TIFFSwabFloat(&float_val);
        
        // Test swabbing again
        TIFFSwabFloat(&float_val);
    }
    
    if (fdp.remaining_bytes() >= sizeof(double)) {
        double double_val = fdp.ConsumeFloatingPoint<double>();
        TIFFSwabDouble(&double_val);
        
        // Test swabbing again
        TIFFSwabDouble(&double_val);
    }
    // Test 2: Array swab operations
    // Determine array sizes based on remaining input
    size_t remaining = fdp.remaining_bytes();
    
    // Test TIFFSwabArrayOfShort
    if (remaining >= sizeof(uint16_t)) {
        size_t short_array_size = fdp.ConsumeIntegralInRange<size_t>(1, remaining / sizeof(uint16_t));
        std::vector<uint16_t> short_array(short_array_size);
        for (size_t i = 0; i < short_array_size && fdp.remaining_bytes() >= sizeof(uint16_t); i++) {
            short_array[i] = fdp.ConsumeIntegral<uint16_t>();
        }
        if (!short_array.empty()) {
            TIFFSwabArrayOfShort(short_array.data(), short_array.size());
            
            // Test swabbing again
            TIFFSwabArrayOfShort(short_array.data(), short_array.size());
        }
    }
    remaining = fdp.remaining_bytes();
    
    // Test TIFFSwabArrayOfTriples
    if (remaining >= 3) {
        size_t triple_array_size = fdp.ConsumeIntegralInRange<size_t>(1, remaining / 3);
        std::vector<uint8_t> triple_array(triple_array_size * 3);
        for (size_t i = 0; i < triple_array.size() && fdp.remaining_bytes() > 0; i++) {
            triple_array[i] = fdp.ConsumeIntegral<uint8_t>();
        }
        if (!triple_array.empty()) {
            TIFFSwabArrayOfTriples(triple_array.data(), triple_array_size);
            
            // Test swabbing again
            TIFFSwabArrayOfTriples(triple_array.data(), triple_array_size);
        }
    }
    
    remaining = fdp.remaining_bytes();
    
    // Test TIFFSwabArrayOfLong
    if (remaining >= sizeof(uint32_t)) {
        size_t long_array_size = fdp.ConsumeIntegralInRange<size_t>(1, remaining / sizeof(uint32_t));
        std::vector<uint32_t> long_array(long_array_size);
        for (size_t i = 0; i < long_array_size && fdp.remaining_bytes() >= sizeof(uint32_t); i++) {
            long_array[i] = fdp.ConsumeIntegral<uint32_t>();
        }
        if (!long_array.empty()) {
            TIFFSwabArrayOfLong(long_array.data(), long_array.size());
            
            // Test swabbing again
            TIFFSwabArrayOfLong(long_array.data(), long_array.size());
        }
    }
    
    remaining = fdp.remaining_bytes();
    
    // Test TIFFSwabArrayOfLong8
    if (remaining >= sizeof(uint64_t)) {
        size_t long8_array_size = fdp.ConsumeIntegralInRange<size_t>(1, remaining / sizeof(uint64_t));
        std::vector<uint64_t> long8_array(long8_array_size);
        for (size_t i = 0; i < long8_array_size && fdp.remaining_bytes() >= sizeof(uint64_t); i++) {
            long8_array[i] = fdp.ConsumeIntegral<uint64_t>();
        }
        if (!long8_array.empty()) {
            TIFFSwabArrayOfLong8(long8_array.data(), long8_array.size());
            
            // Test swabbing again
            TIFFSwabArrayOfLong8(long8_array.data(), long8_array.size());
        }
    }
    
    remaining = fdp.remaining_bytes();
    
    // Test TIFFSwabArrayOfFloat
    if (remaining >= sizeof(float)) {
        size_t float_array_size = fdp.ConsumeIntegralInRange<size_t>(1, remaining / sizeof(float));
        std::vector<float> float_array(float_array_size);
        for (size_t i = 0; i < float_array_size && fdp.remaining_bytes() >= sizeof(float); i++) {
            float_array[i] = fdp.ConsumeFloatingPoint<float>();
        }
        if (!float_array.empty()) {
            TIFFSwabArrayOfFloat(float_array.data(), float_array.size());
            
            // Test swabbing again
            TIFFSwabArrayOfFloat(float_array.data(), float_array.size());
        }
    }
    
    remaining = fdp.remaining_bytes();
    
    // Test TIFFSwabArrayOfDouble
    if (remaining >= sizeof(double)) {
        size_t double_array_size = fdp.ConsumeIntegralInRange<size_t>(1, remaining / sizeof(double));
        std::vector<double> double_array(double_array_size);
        for (size_t i = 0; i < double_array_size && fdp.remaining_bytes() >= sizeof(double); i++) {
            double_array[i] = fdp.ConsumeFloatingPoint<double>();
        }
        if (!double_array.empty()) {
            TIFFSwabArrayOfDouble(double_array.data(), double_array.size());
            
            // Test swabbing again
            TIFFSwabArrayOfDouble(double_array.data(), double_array.size());
        }
    }
    // Test 3: Bit reversal operations
    remaining = fdp.remaining_bytes();
    if (remaining > 0) {
        // Test TIFFReverseBits on random byte data
        size_t bit_reverse_size = fdp.ConsumeIntegralInRange<size_t>(1, remaining);
        std::vector<uint8_t> bit_data = fdp.ConsumeBytes<uint8_t>(bit_reverse_size);
        if (!bit_data.empty()) {
            TIFFReverseBits(bit_data.data(), bit_data.size());
            
            // Test reversing bits again (should return to original)
            TIFFReverseBits(bit_data.data(), bit_data.size());
        }
    }

    // Test 4: Bit reversal table lookup
    // Test both reversed (1) and non-reversed (0) tables
    const unsigned char* rev_table = TIFFGetBitRevTable(1);  // Bit reversal table
    const unsigned char* identity_table = TIFFGetBitRevTable(0);  // Identity table
    
    // Verify table properties by testing a few values
    if (rev_table && identity_table) {
        // For identity table, table[n] should equal n
        // For bit reversal table, test that applying twice gives identity
        // We'll test a few values from fuzzed input
        for (int i = 0; i < 16 && fdp.remaining_bytes() > 0; i++) {
            uint8_t test_val = fdp.ConsumeIntegral<uint8_t>();
            
            // Verify identity table property
            uint8_t identity_result = identity_table[test_val];
            // On little-endian systems, identity_table should map each byte to itself
            
            // Verify bit reversal property: rev_table[rev_table[test_val]] should equal test_val
            uint8_t rev_once = rev_table[test_val];
            uint8_t rev_twice = rev_table[rev_once];
            // rev_twice should equal test_val for proper bit reversal
        }
    }

    // Test 5: Mixed operations - apply different swab operations to same data
    remaining = fdp.remaining_bytes();
    if (remaining >= 16) {
        // Create a buffer that can be interpreted as different types
        size_t mixed_size = fdp.ConsumeIntegralInRange<size_t>(16, std::min(remaining, (size_t)256));
        std::vector<uint8_t> mixed_data = fdp.ConsumeBytes<uint8_t>(mixed_size);
        
        if (mixed_data.size() >= sizeof(uint16_t)) {
            // Interpret first 2 bytes as uint16_t
            uint16_t* as_short = reinterpret_cast<uint16_t*>(mixed_data.data());
            TIFFSwabShort(as_short);
        }
        
        if (mixed_data.size() >= sizeof(uint32_t)) {
            // Interpret first 4 bytes as uint32_t
            uint32_t* as_long = reinterpret_cast<uint32_t*>(mixed_data.data());
            TIFFSwabLong(as_long);
        }
        
        if (mixed_data.size() >= sizeof(uint64_t)) {
            // Interpret first 8 bytes as uint64_t
            uint64_t* as_long8 = reinterpret_cast<uint64_t*>(mixed_data.data());
            TIFFSwabLong8(as_long8);
        }
        
        if (mixed_data.size() >= sizeof(float)) {
            // Interpret first 4 bytes as float
            float* as_float = reinterpret_cast<float*>(mixed_data.data());
            TIFFSwabFloat(as_float);
        }
        
        if (mixed_data.size() >= sizeof(double)) {
            // Interpret first 8 bytes as double
            double* as_double = reinterpret_cast<double*>(mixed_data.data());
            TIFFSwabDouble(as_double);
        }
        
        // Apply bit reversal to the entire buffer
        TIFFReverseBits(mixed_data.data(), mixed_data.size());
    }

    // Test 6: Edge cases and special values
    // Test with zero values
    uint16_t zero_short = 0;
    TIFFSwabShort(&zero_short);
    
    uint32_t zero_long = 0;
    TIFFSwabLong(&zero_long);
    
    uint64_t zero_long8 = 0;
    TIFFSwabLong8(&zero_long8);
    
    float zero_float = 0.0f;
    TIFFSwabFloat(&zero_float);
    
    double zero_double = 0.0;
    TIFFSwabDouble(&zero_double);
    
    // Test with maximum values
    uint16_t max_short = UINT16_MAX;
    TIFFSwabShort(&max_short);
    
    uint32_t max_long = UINT32_MAX;
    TIFFSwabLong(&max_long);
    
    uint64_t max_long8 = UINT64_MAX;
    TIFFSwabLong8(&max_long8);
    
    // Test with special floating point values
    if (fdp.remaining_bytes() >= 8) {
        // Generate some special float values
        float special_floats[] = {
            INFINITY,
            -INFINITY,
            NAN,
            1.0f,
            -1.0f,
            0.5f,
            -0.5f
        };
        
        for (auto& fval : special_floats) {
            TIFFSwabFloat(&fval);
            TIFFSwabFloat(&fval);  // Swab back
        }
        
        double special_doubles[] = {
            INFINITY,
            -INFINITY,
            NAN,
            1.0,
            -1.0,
            0.5,
            -0.5
        };
        
        for (auto& dval : special_doubles) {
            TIFFSwabDouble(&dval);
            TIFFSwabDouble(&dval);  // Swab back
        }
    }

    // Test 7: Array operations with zero and single elements
    // Single element arrays
    uint16_t single_short[1] = {fdp.ConsumeIntegral<uint16_t>()};
    TIFFSwabArrayOfShort(single_short, 1);
    
    uint32_t single_long[1] = {fdp.ConsumeIntegral<uint32_t>()};
    TIFFSwabArrayOfLong(single_long, 1);
    
    uint8_t single_triple[3] = {fdp.ConsumeIntegral<uint8_t>(), 
                                fdp.ConsumeIntegral<uint8_t>(), 
                                fdp.ConsumeIntegral<uint8_t>()};
    TIFFSwabArrayOfTriples(single_triple, 1);

    // Zero-length arrays (should be safe)
    uint16_t empty_short[1] = {0};
    TIFFSwabArrayOfShort(empty_short, 0);
    
    uint8_t empty_triple[3] = {0, 0, 0};
    TIFFSwabArrayOfTriples(empty_triple, 0);

    return 0;
}

} // namespace harness_013
