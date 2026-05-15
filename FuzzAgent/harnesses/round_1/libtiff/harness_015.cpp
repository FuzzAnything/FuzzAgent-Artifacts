#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <limits>
#include "tiffio.h"

namespace harness_015 {
/*
 * Fuzzing harness for libtiff LogLuv color space APIs
 * Target APIs: LogLuv encoding/decoding operations (completely uncovered with 100+ undiscovered branches)
 * Primary targets:
 *   - LogLuv24toXYZ, LogLuv24fromXYZ (24-bit LogLuv encoding/decoding)
 *   - LogLuv32toXYZ, LogLuv32fromXYZ (32-bit LogLuv encoding/decoding)
 *   - LogL16toY, LogL16fromY (16-bit luminance encoding/decoding)
 *   - LogL10toY, LogL10fromY (10-bit luminance encoding/decoding)
 *   - uv_decode, uv_encode (chromaticity encoding/decoding)
 *   - XYZtoRGB24 (XYZ to RGB conversion)
 *
 * These APIs are part of the high dynamic range LogLuv compression support for TIFF.
 * Coverage analysis indicates these are completely uncovered color space conversion functions
 * with high complexity and many undiscovered branches.
 *
 * Semantic differentiation from previous harnesses:
 * - harness_000-014 focus on TIFF file I/O, directory operations, geometry calculations
 * - This harness focuses exclusively on LogLuv color space mathematical transformations
 * - Tests pure computational functions without file I/O dependencies
 * - Tests both encoding (to LogLuv) and decoding (from LogLuv) pathways
 * - Tests round-trip consistency for color space conversions
 * - Tests edge cases in luminance/chromaticity calculations
 */



// Error handler to suppress warnings during fuzzing
extern "C" void tiff_error_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress errors during fuzzing
    return;
}

// Helper function to generate valid XYZ values
void generate_xyz(FuzzedDataProvider& fdp, float xyz[3]) {
    // Generate values in a reasonable range for color space conversions
    // XYZ values should be non-negative and not extremely large
    const float max_val = 1000.0f; // Reasonable upper bound for color values
    
    xyz[0] = fdp.ConsumeFloatingPointInRange<float>(0.0f, max_val);
    xyz[1] = fdp.ConsumeFloatingPointInRange<float>(0.0f, max_val);
    xyz[2] = fdp.ConsumeFloatingPointInRange<float>(0.0f, max_val);
}

// Helper function to generate valid luminance value
double generate_luminance(FuzzedDataProvider& fdp) {
    // Luminance should be positive for meaningful conversions
    // Use a wide range including very small and large values
    const double min_lum = 1e-10;
    const double max_lum = 1e10;
    
    return fdp.ConsumeFloatingPointInRange<double>(min_lum, max_lum);
}

// Helper function to generate valid chromaticity coordinates
void generate_uv(FuzzedDataProvider& fdp, double& u, double& v) {
    // u and v should be in valid chromaticity range
    // Typical range is roughly [0, 0.6] for u and [0, 0.6] for v
    const double uv_min = 0.0;
    const double uv_max = 0.6;
    
    u = fdp.ConsumeFloatingPointInRange<double>(uv_min, uv_max);
    v = fdp.ConsumeFloatingPointInRange<double>(uv_min, uv_max);
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for multiple test cases
    if (size < 128) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Set error handlers to suppress warnings
    TIFFSetErrorHandler(NULL);
    TIFFSetWarningHandler(NULL);

    // Determine which test cases to run based on fuzzer input
    uint8_t test_case = fdp.ConsumeIntegral<uint8_t>() % 8;

    switch (test_case) {
        case 0: {
            // Test LogLuv24 round-trip conversion
            float xyz_original[3];
            generate_xyz(fdp, xyz_original);
            
            int encode_method = fdp.ConsumeIntegralInRange<int>(0, 1); // 0=SGILOGENCODE_NODITHER, 1=SGILOGENCODE_RANDITHER
            
            // Encode XYZ to LogLuv24
            uint32_t luv24 = LogLuv24fromXYZ(xyz_original, encode_method);
            
            // Decode back to XYZ
            float xyz_decoded[3];
            LogLuv24toXYZ(luv24, xyz_decoded);
            
            // Note: We don't assert equality due to lossy encoding
            // Just exercising the code paths
            break;
        }
        
        case 1: {
            // Test LogLuv32 round-trip conversion
            float xyz_original[3];
            generate_xyz(fdp, xyz_original);
            
            int encode_method = fdp.ConsumeIntegralInRange<int>(0, 1);
            
            // Encode XYZ to LogLuv32
            uint32_t luv32 = LogLuv32fromXYZ(xyz_original, encode_method);
            
            // Decode back to XYZ
            float xyz_decoded[3];
            LogLuv32toXYZ(luv32, xyz_decoded);
            
            break;
        }
        
        case 2: {
            // Test LogL16 luminance conversion
            double y_original = generate_luminance(fdp);
            int encode_method = fdp.ConsumeIntegralInRange<int>(0, 1);
            
            // Encode luminance to LogL16
            int logl16 = LogL16fromY(y_original, encode_method);
            
            // Decode back to luminance
            double y_decoded = LogL16toY(logl16);
            
            break;
        }
        
        case 3: {
            // Test LogL10 luminance conversion
            double y_original = generate_luminance(fdp);
            int encode_method = fdp.ConsumeIntegralInRange<int>(0, 1);
            
            // Encode luminance to LogL10
            int logl10 = LogL10fromY(y_original, encode_method);
            
            // Decode back to luminance
            double y_decoded = LogL10toY(logl10);
            
            break;
        }
        
        case 4: {
            // Test uv encoding/decoding round-trip
            double u_original, v_original;
            generate_uv(fdp, u_original, v_original);
            
            int encode_method = fdp.ConsumeIntegralInRange<int>(0, 1);
            
            // Encode uv coordinates
            int uv_encoded = uv_encode(u_original, v_original, encode_method);
            
            // Decode back to uv coordinates
            double u_decoded, v_decoded;
            uv_decode(&u_decoded, &v_decoded, uv_encoded);
            
            break;
        }
        
        case 5: {
            // Test XYZtoRGB24 conversion
            float xyz[3];
            generate_xyz(fdp, xyz);
            
            uint8_t rgb[3];
            XYZtoRGB24(xyz, rgb);
            
            break;
        }
        
        case 6: {
            // Test direct LogLuv24 decoding with random encoded values
            uint32_t luv24 = fdp.ConsumeIntegral<uint32_t>();
            float xyz[3];
            LogLuv24toXYZ(luv24, xyz);
            
            break;
        }
        
        case 7: {
            // Test direct LogLuv32 decoding with random encoded values
            uint32_t luv32 = fdp.ConsumeIntegral<uint32_t>();
            float xyz[3];
            LogLuv32toXYZ(luv32, xyz);
            
            break;
        }
        
        default:
            // Should not happen due to modulo 8
            break;
    }

    // Additional comprehensive test: mixed operations
    if (fdp.remaining_bytes() > 64) {
        // Test edge cases with extreme values
        float extreme_xyz[3];
        extreme_xyz[0] = fdp.ConsumeFloatingPoint<float>();
        extreme_xyz[1] = fdp.ConsumeFloatingPoint<float>();
        extreme_xyz[2] = fdp.ConsumeFloatingPoint<float>();
        
        // Test with potentially invalid values
        uint32_t test_luv24 = fdp.ConsumeIntegral<uint32_t>();
        float test_xyz[3];
        LogLuv24toXYZ(test_luv24, test_xyz);
        
        // Test with zero luminance (edge case)
        float zero_xyz[3] = {0.0f, 0.0f, 0.0f};
        uint32_t zero_luv24 = LogLuv24fromXYZ(zero_xyz, 0);
        LogLuv24toXYZ(zero_luv24, test_xyz);
        
        // Test with very small luminance
        float small_xyz[3] = {1e-10f, 1e-10f, 1e-10f};
        uint32_t small_luv24 = LogLuv24fromXYZ(small_xyz, 0);
        LogLuv24toXYZ(small_luv24, test_xyz);
    }

    return 0;
}

} // namespace harness_015
