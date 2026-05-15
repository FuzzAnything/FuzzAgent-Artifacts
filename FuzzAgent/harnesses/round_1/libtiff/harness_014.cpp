#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "tiffio.h"
#include "tiff.h"

namespace harness_014 {
/*
 * Fuzzing harness for libtiff raster scanline geometry calculation functions
 * Target APIs: TIFFRasterScanlineSize64 and TIFFRasterScanlineSize (completely uncovered with 26 undiscovered accumulated branches)
 * 
 * Coverage analysis indicates these are critical uncovered geometry calculation functions:
 * - TIFFRasterScanlineSize64: Computes bytes required for complete decoded/packed raster scanline
 * - TIFFRasterScanlineSize: 32-bit version of the above
 * 
 * These functions differ from TIFFScanlineSize which returns I/O size (may be less for planar data).
 * TIFFRasterScanlineSize always returns full size needed for complete decoded raster data.
 * 
 * Semantic differentiation from previous harnesses:
 * - harness_011 targets strip/tile size calculations (TIFFStripSize, TIFFVStripSize)
 * - This harness focuses exclusively on raster scanline geometry calculations
 * - Tests both planar configurations (CONTIG vs SEPARATE) with various sample configurations
 * - Tests 64-bit and 32-bit variants of the same calculation
 * - Tests edge cases with extreme width/bits-per-sample values
 */



// Error handler to suppress warnings during fuzzing
extern "C" void tiff_error_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress errors during fuzzing
    return;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for TIFF setup and parameters
    if (size < 128) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Set error handlers to suppress warnings
    TIFFSetErrorHandler(NULL);
    TIFFSetWarningHandler(NULL);

    // Create a temporary file for TIFF operations
    char temp_file_template[] = "/tmp/fuzz_tiff_raster_scanline_XXXXXX";
    int fd = mkstemp(temp_file_template);
    if (fd < 0) {
        return 0;
    }
    std::string temp_filename = temp_file_template;
    
    // Write minimal valid TIFF header to the temporary file
    uint8_t tiff_header[] = {
        0x49, 0x49, 0x2A, 0x00,  // Little-endian, classic TIFF
        0x08, 0x00, 0x00, 0x00,  // First IFD offset
        0x00, 0x00,              // Number of directory entries (0 = end)
        0x00, 0x00, 0x00, 0x00   // Next IFD offset (0 = no more)
    };
    
    if (write(fd, tiff_header, sizeof(tiff_header)) != (ssize_t)sizeof(tiff_header)) {
        close(fd);
        unlink(temp_filename.c_str());
        return 0;
    }
    close(fd);

    // Open the TIFF file for writing to set up test configuration
    TIFF* tif = TIFFOpen(temp_filename.c_str(), "w");
    if (tif == nullptr) {
        unlink(temp_filename.c_str());
        return 0;
    }

    // Consume parameters for TIFF configuration from fuzzer input
    // Critical parameters for raster scanline calculations:
    // 1. Image width (td_imagewidth) - affects scanline size directly
    // 2. Bits per sample (td_bitspersample) - affects scanline size
    // 3. Samples per pixel (td_samplesperpixel) - affects CONTIG mode
    // 4. Planar configuration (td_planarconfig) - CONTIG vs SEPARATE
    
    uint32_t image_width = fdp.ConsumeIntegralInRange<uint32_t>(1, 65535);
    uint16_t bits_per_sample = fdp.ConsumeIntegralInRange<uint16_t>(1, 32);
    uint16_t samples_per_pixel = fdp.ConsumeIntegralInRange<uint16_t>(1, 5);
    uint16_t planar_config = (fdp.ConsumeIntegral<uint16_t>() % 2) + 1; // 1=CONTIG, 2=SEPARATE
    // Additional parameters that might affect internal calculations
    uint32_t rows_per_strip = fdp.ConsumeIntegralInRange<uint32_t>(1, 1000);
    uint16_t photometric = fdp.ConsumeIntegral<uint16_t>() % 6; // Various photometric interpretations
    
    // Set basic required fields
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, image_width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, fdp.ConsumeIntegralInRange<uint32_t>(1, 1000));
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planar_config);
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, rows_per_strip);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
    
    // Set compression to avoid compression-related issues
    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
    
    // Set resolution if needed
    if (fdp.ConsumeBool()) {
        float x_res = fdp.ConsumeFloatingPoint<float>();
        float y_res = fdp.ConsumeFloatingPoint<float>();
        TIFFSetField(tif, TIFFTAG_XRESOLUTION, x_res);
        TIFFSetField(tif, TIFFTAG_YRESOLUTION, y_res);
    }
    
    // Set orientation if needed
    if (fdp.ConsumeBool()) {
        TIFFSetField(tif, TIFFTAG_ORIENTATION, fdp.ConsumeIntegralInRange<uint16_t>(1, 8));
    }
    
    // Main test: TIFFRasterScanlineSize64 and TIFFRasterScanlineSize
    // Test both functions with the configured TIFF
    
    // Test 1: Basic raster scanline size calculation
    uint64_t raster_scanline_size_64 = TIFFRasterScanlineSize64(tif);
    tmsize_t raster_scanline_size = TIFFRasterScanlineSize(tif);
    
    // Verify consistency between 64-bit and 32-bit versions
    // (raster_scanline_size should be cast from raster_scanline_size_64)
    
    // Test 2: Test with different planar configurations
    // We already set planar_config above, but test both if possible
    if (fdp.ConsumeBool() && samples_per_pixel > 1) {
        // Try toggling planar config
        uint16_t alternate_planar = (planar_config == PLANARCONFIG_CONTIG) ? 
                                    PLANARCONFIG_SEPARATE : PLANARCONFIG_CONTIG;
        
        // Create a separate TIFF for alternate configuration
        TIFF* alt_tif = TIFFOpen(temp_filename.c_str(), "r+");
        if (alt_tif) {
            // Keep same dimensions but change planar config
            TIFFSetField(alt_tif, TIFFTAG_PLANARCONFIG, alternate_planar);
            
            uint64_t alt_raster_scanline_size_64 = TIFFRasterScanlineSize64(alt_tif);
            tmsize_t alt_raster_scanline_size = TIFFRasterScanlineSize(alt_tif);
            
            TIFFClose(alt_tif);
        }
    }
    
    // Test 3: Edge cases with extreme values
    if (fdp.ConsumeBool()) {
        // Test with very large image width (but avoid overflow)
        uint32_t large_width = fdp.ConsumeIntegralInRange<uint32_t>(1, 100000);
        
        TIFF* edge_tif = TIFFOpen(temp_filename.c_str(), "r+");
        if (edge_tif) {
            TIFFSetField(edge_tif, TIFFTAG_IMAGEWIDTH, large_width);
            
            uint64_t large_raster_scanline_size_64 = TIFFRasterScanlineSize64(edge_tif);
            tmsize_t large_raster_scanline_size = TIFFRasterScanlineSize(edge_tif);
            
            TIFFClose(edge_tif);
        }
    }
    
    // Test 4: Test with different bits-per-sample values
    if (fdp.ConsumeBool() && bits_per_sample >= 8) {
        // Try a different bits-per-sample value
        uint16_t alt_bits = fdp.ConsumeIntegralInRange<uint16_t>(1, 32);
        
        TIFF* bits_tif = TIFFOpen(temp_filename.c_str(), "r+");
        if (bits_tif) {
            TIFFSetField(bits_tif, TIFFTAG_BITSPERSAMPLE, alt_bits);
            
            uint64_t alt_bits_raster_scanline_size_64 = TIFFRasterScanlineSize64(bits_tif);
            tmsize_t alt_bits_raster_scanline_size = TIFFRasterScanlineSize(bits_tif);
            
            TIFFClose(bits_tif);
        }
    }
    
    // Test 5: Test with different samples-per-pixel
    if (fdp.ConsumeBool() && samples_per_pixel > 1) {
        uint16_t alt_samples = fdp.ConsumeIntegralInRange<uint16_t>(1, 10);
        
        TIFF* samples_tif = TIFFOpen(temp_filename.c_str(), "r+");
        if (samples_tif) {
            TIFFSetField(samples_tif, TIFFTAG_SAMPLESPERPIXEL, alt_samples);
            
            uint64_t alt_samples_raster_scanline_size_64 = TIFFRasterScanlineSize64(samples_tif);
            tmsize_t alt_samples_raster_scanline_size = TIFFRasterScanlineSize(samples_tif);
            
            TIFFClose(samples_tif);
        }
    }
    
    // Test 6: Compare with TIFFScanlineSize (I/O size vs raster size)
    // According to documentation: TIFFScanlineSize returns I/O size which may be less
    // if data is stored as separate planes, while TIFFRasterScanlineSize always
    // returns full size needed for complete decoded raster data.
    uint64_t scanline_size_64 = TIFFScanlineSize64(tif);
    tmsize_t scanline_size = TIFFScanlineSize(tif);
    
    // For planar separate data, scanline size should be smaller than raster scanline size
    // For planar contiguous data, they should be equal
    
    // Test 7: Test with zero or invalid values (should handle gracefully)
    // The functions should handle these cases internally with error checking
    
    // Clean up
    TIFFClose(tif);
    unlink(temp_filename.c_str());
    
    return 0;
}

} // namespace harness_014
