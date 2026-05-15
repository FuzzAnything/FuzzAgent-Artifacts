#include <fuzzer/FuzzedDataProvider.h>
#include <png.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_012 {
/*
 * Harness for testing PNG pCAL (pixel calibration) chunk operations.
 * Targets high-complexity, currently uncovered APIs:
 * - png_set_pCAL (46+ undiscovered complexity points, 0% coverage in pngget.c)
 * - png_get_pCAL (46+ undiscovered complexity points, 0% coverage in pngget.c)
 *
 * This harness implements the complete pCAL chunk lifecycle for pixel
 * calibration data, including parameter arrays and validation paths.
 * pCAL differs from sCAL: sCAL is for physical scale, pCAL is for pixel
 * value calibration (mapping pixel values to physical measurements).
 */


// Custom write function that does nothing (we're testing API calls, not actual PNG writing)
static void null_write_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Do nothing - we're not actually writing PNG data
    (void)png_ptr;
    (void)data;
    (void)length;
}

// Custom flush function
static void null_flush_data(png_structp png_ptr) {
    (void)png_ptr;
}

// Custom read function for reading path
static void null_read_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Do nothing - we're not actually reading PNG data
    (void)png_ptr;
    (void)data;
    (void)length;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for various parameters
    // We need: purpose string, units string, X0, X1, type, nparams, and params strings
    if (size < 100) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // ==================== WRITING PATH ====================
    
    // Create write structure
    png_structp png_write_ptr = png_create_write_struct(
        PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_write_ptr) return 0;
    
    // Create info structure
    png_infop info_write_ptr = png_create_info_struct(png_write_ptr);
    if (!info_write_ptr) {
        png_destroy_write_struct(&png_write_ptr, nullptr);
        return 0;
    }
    
    // Set error handling
    if (setjmp(png_jmpbuf(png_write_ptr))) {
        png_destroy_write_struct(&png_write_ptr, &info_write_ptr);
        return 0;
    }
    
    // Set write function
    png_set_write_fn(png_write_ptr, nullptr, null_write_data, null_flush_data);
    
    // Set basic image header (IHDR) - required before setting pCAL
    png_uint_32 width = fdp.ConsumeIntegralInRange<png_uint_32>(1, 1000);
    png_uint_32 height = fdp.ConsumeIntegralInRange<png_uint_32>(1, 1000);
    int bit_depth = fdp.PickValueInArray({1, 2, 4, 8, 16});
    int color_type = fdp.PickValueInArray({
        PNG_COLOR_TYPE_GRAY,
        PNG_COLOR_TYPE_PALETTE,
        PNG_COLOR_TYPE_RGB,
        PNG_COLOR_TYPE_RGB_ALPHA,
        PNG_COLOR_TYPE_GRAY_ALPHA
    });
    int interlace_method = fdp.PickValueInArray({PNG_INTERLACE_NONE, PNG_INTERLACE_ADAM7});
    int compression_method = PNG_COMPRESSION_TYPE_BASE;
    int filter_method = PNG_FILTER_TYPE_BASE;
    
    png_set_IHDR(png_write_ptr, info_write_ptr, width, height, bit_depth,
                 color_type, interlace_method, compression_method, filter_method);
    
    // Prepare pCAL parameters from fuzzer input
    // Consume purpose string
    std::string purpose = fdp.ConsumeRandomLengthString(100);
    if (purpose.empty()) purpose = "calibration";
    
    // Consume calibration range values X0 and X1
    png_int_32 X0 = fdp.ConsumeIntegral<png_int_32>();
    png_int_32 X1 = fdp.ConsumeIntegral<png_int_32>();
    
    // Consume equation type (0-3 as per PNG specification)
    int type = fdp.ConsumeIntegralInRange<int>(0, 3);
    
    // Consume number of parameters (0-255 as per pngset.c validation)
    int nparams = fdp.ConsumeIntegralInRange<int>(0, 10); // Limit to 10 for fuzzing
    
    // Consume units string
    std::string units = fdp.ConsumeRandomLengthString(50);
    if (units.empty()) units = "units";
    
    // Prepare params array
    std::vector<char*> params;
    std::vector<std::string> param_strings;
    
    // Reserve capacity to prevent reallocation during push_back
    param_strings.reserve(nparams);
    
    // First, build all parameter strings
    for (int i = 0; i < nparams; ++i) {
        // Each parameter must be a valid floating-point string
        // Generate a random floating-point string format
        std::string param;
        if (fdp.ConsumeBool()) {
            // Integer format
            int int_val = fdp.ConsumeIntegral<int>();
            param = std::to_string(int_val);
        } else {
            // Floating-point format
            double float_val = fdp.ConsumeFloatingPoint<double>();
            param = std::to_string(float_val);
        }
        param_strings.push_back(param);
    }
    
    // Now build params array with stable pointers (no more reallocations)
    params.reserve(nparams + 1); // +1 for null terminator
    for (int i = 0; i < nparams; ++i) {
        params.push_back(const_cast<char*>(param_strings[i].c_str()));
    }
    
    // Add null terminator to params array as required by libpng
    params.push_back(nullptr);
    
    // Test png_set_pCAL with the generated parameters
    png_set_pCAL(png_write_ptr, info_write_ptr, 
                 purpose.c_str(), X0, X1, type,
                 nparams, units.c_str(), params.data());
    
    // ==================== READING PATH ====================
    
    // Create read structure to test get functions
    png_structp png_read_ptr = png_create_read_struct(
        PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_read_ptr) {
        png_destroy_write_struct(&png_write_ptr, &info_write_ptr);
        return 0;
    }
    
    png_infop info_read_ptr = png_create_info_struct(png_read_ptr);
    if (!info_read_ptr) {
        png_destroy_read_struct(&png_read_ptr, nullptr, nullptr);
        png_destroy_write_struct(&png_write_ptr, &info_write_ptr);
        return 0;
    }
    
    // Set error handling for read struct
    if (setjmp(png_jmpbuf(png_read_ptr))) {
        png_destroy_read_struct(&png_read_ptr, &info_read_ptr, nullptr);
        png_destroy_write_struct(&png_write_ptr, &info_write_ptr);
        return 0;
    }
    
    // Set read function
    png_set_read_fn(png_read_ptr, nullptr, null_read_data);
    
    // Test png_get_pCAL - we need to allocate output variables
    char* get_purpose = nullptr;
    png_int_32 get_X0 = 0, get_X1 = 0;
    int get_type = 0, get_nparams = 0;
    char* get_units = nullptr;
    char** get_params = nullptr;
    
    // We don't have actual pCAL data in the read structure, but we can call
    // the function to exercise its error handling paths (should return 0)
    png_uint_32 result = png_get_pCAL(png_read_ptr, info_read_ptr,
                                     &get_purpose, &get_X0, &get_X1,
                                     &get_type, &get_nparams,
                                     &get_units, &get_params);
    
    // The result should be 0 since we didn't set pCAL in the read structure
    // But we've exercised the function and its parameter validation
    
    // ==================== VALIDATION PATH ====================
    
    // Now test getting from the write structure (where we actually set pCAL)
    // First, we need to simulate that the info struct has been written
    // For testing, we can try to get from the write info struct
    char* write_get_purpose = nullptr;
    png_int_32 write_get_X0 = 0, write_get_X1 = 0;
    int write_get_type = 0, write_get_nparams = 0;
    char* write_get_units = nullptr;
    char** write_get_params = nullptr;
    
    // This should succeed since we set pCAL in the write info struct
    png_uint_32 write_result = png_get_pCAL(png_write_ptr, info_write_ptr,
                                           &write_get_purpose, &write_get_X0, &write_get_X1,
                                           &write_get_type, &write_get_nparams,
                                           &write_get_units, &write_get_params);
    
    // ==================== CLEANUP ====================
    
    // Clean up read structures
    png_destroy_read_struct(&png_read_ptr, &info_read_ptr, nullptr);
    
    // Clean up write structures
    png_destroy_write_struct(&png_write_ptr, &info_write_ptr);
    
    return 0;
}

} // namespace harness_012
