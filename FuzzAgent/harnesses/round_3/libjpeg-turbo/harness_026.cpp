#include <fuzzer/FuzzedDataProvider.h>
#include "turbojpeg.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <fstream>
#include <cstdio>

namespace harness_026 {
/*
 * Fuzzing harness for libjpeg-turbo PNG and BMP image format loading and saving
 * targeting completely uncovered non-JPEG image format modules (0% coverage).
 * 
 * This harness specifically targets PNG and BMP format support modules that show
 * 0% coverage across all functions (50+ functions, 3000+ branches). The spng
 * library, PNG reader/writer (rdpng.c, wrpng.c), and BMP reader/writer (rdbmp.c,
 * wrbmp.c) code are completely untested by current fuzzers.
 * 
 * Target APIs (per coverage guidance):
 * - tj3Alloc (Required Helper: Memory allocation)
 * - tj3LoadImage8 (Primary Target: 8-bit PNG/BMP loading)
 * - tj3LoadImage12 (Primary Target: 12-bit PNG/BMP loading)
 * - tj3LoadImage16 (Primary Target: 16-bit PNG/BMP loading)
 * - tj3SaveImage8 (Primary Target: 8-bit PNG/BMP saving)
 * - tj3SaveImage12 (Primary Target: 12-bit PNG/BMP saving)
 * - tj3SaveImage16 (Primary Target: 16-bit PNG/BMP saving)
 * - tj3Destroy (Required Helper: Cleanup)
 * 
 * Alternative legacy APIs (if tj3 APIs are problematic):
 * - tjInitDecompress (Initialization)
 * - tjLoadImage (PNG/BMP loading)
 * - tjSaveImage (PNG/BMP saving)
 * - tjDestroy (Cleanup)
 * 
 * Invocation sequence (per coverage guidance):
 * 1. Call tj3Alloc to create a turbojpeg handle.
 * 2. Call tj3LoadImage8/tj3LoadImage12/tj3LoadImage16 with PNG or BMP format test data.
 * 3. Optionally perform simple image processing (scaling, cropping if supported).
 * 4. Call tj3SaveImage8/tj3SaveImage12/tj3SaveImage16 to save as PNG or BMP.
 * 5. Call tj3Destroy to clean up.
 * 
 * Reasoning: The PNG/BMP modules contain 50+ completely uncovered functions with
 * 3000+ undiscovered branches. Current fuzzers only test JPEG format, leaving all
 * non-JPEG format support untested. The image loading/saving APIs (tjLoadImage,
 * tjSaveImage, tj3LoadImage*, tj3SaveImage*) are covered but only execute JPEG
 * code paths. By providing PNG and BMP format test data, we can exercise the spng
 * library, PNG reader/writer (rdpng.c, wrpng.c), and BMP reader/writer (rdbmp.c,
 * wrbmp.c) code. This represents the largest contiguous uncovered code region.
 * 
 * Implementation strategy:
 * 1. Use fuzzer input to generate PNG/BMP test data (valid or corrupt)
 * 2. Write test data to temporary files with .png or .bmp extensions
 * 3. Use tj3Init to create TurboJPEG handle (decompress for loading)
 * 4. Test all three bit depths (8, 12, 16) with appropriate APIs
 * 5. Test both PNG and BMP formats
 * 6. If loading succeeds, test saving with different parameters
 * 7. Include comprehensive error handling and resource cleanup
 * 8. Follow exact invocation sequence from coverage guidance
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for format testing
    // Required: format choice + bit depth + dimensions + image data
    if (size < 128) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // 1. Create TurboJPEG handle for decompression (needed for loading images)
    // Note: The coverage guidance says "Call tj3Alloc to create a turbojpeg handle"
    // but tj3Alloc allocates memory, not a handle. We use tj3Init for handle creation
    // and also demonstrate tj3Alloc usage separately.
    tjhandle handle = tj3Init(TJINIT_DECOMPRESS);
    if (!handle) {
        // Try alternative: use legacy API tjInitDecompress
        handle = tjInitDecompress();
        if (!handle) {
            return 0;
        }
    }
    
    // Demonstrate tj3Alloc usage as specified in coverage guidance
    // Allocate a small buffer for testing tj3Alloc API
    size_t alloc_size = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
    void* test_buffer = tj3Alloc(alloc_size);
    if (test_buffer) {
        // Use the allocated buffer for something simple
        // Just to ensure the allocation is exercised
        unsigned char* buf = reinterpret_cast<unsigned char*>(test_buffer);
        buf[0] = fdp.ConsumeIntegral<uint8_t>();
        // Free the test buffer
        tj3Free(test_buffer);
    }
    // Consume test parameters from fuzzer input
    // Choose image format: 0 = PNG, 1 = BMP
    int format_choice = fdp.ConsumeIntegralInRange<int>(0, 1);
    std::string format_extension = (format_choice == 0) ? ".png" : ".bmp";
    
    // Choose bit depth: 0 = 8-bit, 1 = 12-bit, 2 = 16-bit
    int bit_depth_choice = fdp.ConsumeIntegralInRange<int>(0, 2);
    
    // Create temporary input and output filenames
    std::string input_filename = "/tmp/fuzz_input" + format_extension;
    std::string output_filename = "/tmp/fuzz_output" + format_extension;
    
    // Write fuzzer data to temporary input file
    // This data may be valid or corrupt PNG/BMP data - either way it exercises parsing code
    std::ofstream input_file(input_filename, std::ios::binary);
    if (!input_file) {
        tj3Destroy(handle);
        return 0;
    }
    
    // Use remaining bytes as image data
    std::vector<uint8_t> image_data = fdp.ConsumeRemainingBytes<uint8_t>();
    if (image_data.empty()) {
        input_file.close();
        std::remove(input_filename.c_str());
        tj3Destroy(handle);
        return 0;
    }
    
    input_file.write(reinterpret_cast<const char*>(image_data.data()), image_data.size());
    input_file.close();
    
    // Variables to receive loaded image dimensions and format
    int width = 0, height = 0, pixelFormat = TJPF_UNKNOWN;
    int align = 1;  // No row padding
    
    // 2. Call appropriate tj3LoadImage* based on bit depth choice
    void* loaded_buffer = nullptr;
    bool load_success = false;
    
    switch (bit_depth_choice) {
        case 0:  // 8-bit
        {
            unsigned char* buffer8 = tj3LoadImage8(handle, input_filename.c_str(),
                                                  &width, align, &height, &pixelFormat);
            loaded_buffer = buffer8;
            load_success = (buffer8 != nullptr);
            break;
        }
        case 1:  // 12-bit
        {
            short* buffer12 = tj3LoadImage12(handle, input_filename.c_str(),
                                            &width, align, &height, &pixelFormat);
            loaded_buffer = buffer12;
            load_success = (buffer12 != nullptr);
            break;
        }
        case 2:  // 16-bit
        {
            unsigned short* buffer16 = tj3LoadImage16(handle, input_filename.c_str(),
                                                     &width, align, &height, &pixelFormat);
            loaded_buffer = buffer16;
            load_success = (buffer16 != nullptr);
            break;
        }
    }
    
    // Clean up input file
    std::remove(input_filename.c_str());
    
    // 3. If loading succeeded, optionally perform operations and save
    if (load_success && loaded_buffer && width > 0 && height > 0) {
        // Choose a pixel format if still unknown
        if (pixelFormat == TJPF_UNKNOWN) {
            // Default to RGB for color images
            pixelFormat = TJPF_RGB;
        }
        
        // Calculate pitch (bytes per row)
        int pixelSize = tjPixelSize[pixelFormat];
        int pitch = width * pixelSize;
        
        // 4. Call appropriate tj3SaveImage* based on bit depth
        int save_result = -1;
        
        switch (bit_depth_choice) {
            case 0:  // 8-bit
            {
                save_result = tj3SaveImage8(handle, output_filename.c_str(),
                                           reinterpret_cast<const unsigned char*>(loaded_buffer),
                                           width, pitch, height, pixelFormat);
                break;
            }
            case 1:  // 12-bit
            {
                save_result = tj3SaveImage12(handle, output_filename.c_str(),
                                            reinterpret_cast<const short*>(loaded_buffer),
                                            width, pitch, height, pixelFormat);
                break;
            }
            case 2:  // 16-bit
            {
                save_result = tj3SaveImage16(handle, output_filename.c_str(),
                                            reinterpret_cast<const unsigned short*>(loaded_buffer),
                                            width, pitch, height, pixelFormat);
                break;
            }
        }
        
        // Clean up output file if it was created
        if (save_result == 0) {
            std::remove(output_filename.c_str());
        }
    }
    
    // 5. Clean up loaded buffer if it was allocated
    if (loaded_buffer) {
        tj3Free(loaded_buffer);
    }
    
    // Clean up handle
    tj3Destroy(handle);
    
    return 0;
}

} // namespace harness_026
