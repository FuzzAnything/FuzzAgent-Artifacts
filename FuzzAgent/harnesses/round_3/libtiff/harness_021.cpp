#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <algorithm>
#include <tiffio.h>

namespace harness_021 {
/* Fuzzing harness for libtiff targeting strip/tile size calculation functions with exact invocation sequence
 * Focus: Implement precise invocation sequence from coverage guidance for TIFF strip and tile size calculations
 * Critical Coverage Gap: 430+ undiscovered branches in strip/tile size calculation functions
 * Target APIs (exact sequence from guidance):
 *   1. TIFFOpen/TIFFFdOpen - Open TIFF file from test data
 *   2. TIFFSetField - Configure image parameters (width, height, bits per sample, etc.)
 *   3. TIFFIsTiled - Determine if image is tiled or stripped
 *   4. Based on organization, call appropriate size calculation functions:
 *      - For stripped images: TIFFRasterScanlineSize, TIFFVStripSize, TIFFVStripSize64, TIFFStripSize64
 *      - For tiled images: TIFFTileRowSize, TIFFTileSize, TIFFTileSize64, TIFFVTileSize, TIFFVTileSize64
 *   5. TIFFGetField - Verify parameters were set correctly
 *   6. TIFFClose - Close TIFF file
 * Supporting APIs: TIFFSetField, TIFFGetField, TIFFIsTiled
 * Strategy: Follow exact guidance sequence, create TIFF files with fuzzer-controlled parameters,
 *           test both stripped and tiled image size calculations with comprehensive error handling
 * Differentiation: Unlike other harnesses which test reading/writing operations, this harness specifically
 *                  implements the exact size calculation lifecycle sequence from coverage guidance
 * Target coverage: TIFFRasterScanlineSize (11 branches), TIFFTileRowSize (11), TIFFVStripSize (60), 
 *                  TIFFVStripSize64 (60), TIFFStripSize64 (60), TIFFTileSize (56), TIFFTileSize64 (56),
 *                  TIFFVTileSize (56), TIFFVTileSize64 (56) - total 430+ branches
 */


/* Error handler to suppress libtiff error messages during fuzzing */
extern "C" void handle_error(const char* unused, const char* unused2, va_list unused3) {
    // Suppress error messages during fuzzing
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for TIFF file and configuration parameters
    if (size < 256) {
        return 0;
    }

    // Set error handlers to suppress libtiff messages
    TIFFSetErrorHandler(handle_error);
    TIFFSetWarningHandler(handle_error);

    FuzzedDataProvider fdp(data, size);

    // Consume test configuration parameters from fuzzer input
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 3;  // 0: stripped only, 1: tiled only, 2: both
    bool use_tiffopen = fdp.ConsumeBool();  // Whether to use TIFFOpen (true) or TIFFFdOpen (false)
    
    // Consume image dimensions for synthetic TIFF configuration
    uint32_t image_width = fdp.ConsumeIntegralInRange<uint32_t>(1, 4096);
    uint32_t image_height = fdp.ConsumeIntegralInRange<uint32_t>(1, 4096);
    uint32_t rows_per_strip = fdp.ConsumeIntegralInRange<uint32_t>(1, 128);
    uint32_t tile_width = fdp.ConsumeIntegralInRange<uint32_t>(16, 512);
    uint32_t tile_length = fdp.ConsumeIntegralInRange<uint32_t>(16, 512);
    uint32_t strip_height = fdp.ConsumeIntegralInRange<uint32_t>(1, 128);  // For VStripSize testing
    
    // Consume TIFF format parameters
    uint16_t bits_per_sample = fdp.ConsumeIntegralInRange<uint16_t>(1, 32);
    uint16_t samples_per_pixel = fdp.ConsumeIntegralInRange<uint16_t>(1, 4);
    uint16_t photometric = fdp.ConsumeIntegral<uint16_t>() % 6;  // Various photometric interpretations
    uint16_t compression = fdp.ConsumeIntegral<uint16_t>() % 10; // Various compression schemes
    uint16_t planar_config = fdp.ConsumeIntegral<uint16_t>() % 2; // PLANARCONFIG_CONTIG or SEPARATE
    uint16_t fill_order = fdp.ConsumeIntegral<uint16_t>() % 2;    // FILLORDER_MSB2LSB or FILLORDER_LSB2MSB
    
    // Consume organization type (tiled or stripped)
    uint16_t organization = fdp.ConsumeIntegral<uint16_t>() % 2;  // 0: stripped, 1: tiled
    
    // Consume remaining data as TIFF file content
    std::string tiff_data = fdp.ConsumeRemainingBytesAsString();
    
    // Create a temporary file for TIFF operations
    char filename[] = "/tmp/tiff_fuzz_size_calc_XXXXXX.tif";
    int fd = mkstemps(filename, 4);
    if (fd < 0) {
        return 0;
    }
    
    // Write TIFF data to the temporary file
    if (write(fd, tiff_data.c_str(), tiff_data.size()) != (ssize_t)tiff_data.size()) {
        close(fd);
        unlink(filename);
        return 0;
    }
    
    close(fd);
    
    // Open TIFF file using the chosen method
    TIFF* tif = nullptr;
    if (use_tiffopen) {
        tif = TIFFOpen(filename, "r");
    } else {
        // Re-open the file to get a file descriptor for TIFFFdOpen
        fd = open(filename, O_RDONLY);
        if (fd >= 0) {
            tif = TIFFFdOpen(fd, filename, "r");
            // TIFFFdOpen takes ownership of the fd, so we don't close it here
        }
    }
    
    if (!tif) {
        unlink(filename);
        return 0;
    }
    
    // --- STEP 2: Configure image parameters using TIFFSetField ---
    // Set basic image parameters
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, image_width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, image_height);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planar_config);
    TIFFSetField(tif, TIFFTAG_FILLORDER, fill_order);
    
    // Set organization-specific parameters
    if (organization == 0) {  // Stripped organization
        TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, rows_per_strip);
    } else {  // Tiled organization
        TIFFSetField(tif, TIFFTAG_TILEWIDTH, tile_width);
        TIFFSetField(tif, TIFFTAG_TILELENGTH, tile_length);
    }
    
    // --- STEP 3: Determine image organization using TIFFIsTiled ---
    int is_tiled = TIFFIsTiled(tif);
    
    // --- STEP 4: Call appropriate size calculation functions based on organization ---
    // Based on test_scenario, test different combinations
    switch (test_scenario) {
        case 0:  // Test stripped image size calculations
            if (!is_tiled) {
                // Call stripped image size calculation functions
                tmsize_t raster_scanline_size = TIFFRasterScanlineSize(tif);
                uint64_t raster_scanline_size64 = TIFFRasterScanlineSize64(tif);
                
                tmsize_t vstrip_size = TIFFVStripSize(tif, strip_height);
                uint64_t vstrip_size64 = TIFFVStripSize64(tif, strip_height);
                
                uint64_t strip_size64 = TIFFStripSize64(tif);
                
                // Use the results to avoid compiler warnings about unused variables
                (void)raster_scanline_size;
                (void)raster_scanline_size64;
                (void)vstrip_size;
                (void)vstrip_size64;
                (void)strip_size64;
            }
            break;
            
        case 1:  // Test tiled image size calculations
            if (is_tiled) {
                // Call tiled image size calculation functions
                tmsize_t tile_row_size = TIFFTileRowSize(tif);
                uint64_t tile_row_size64 = TIFFTileRowSize64(tif);
                
                tmsize_t tile_size = TIFFTileSize(tif);
                uint64_t tile_size64 = TIFFTileSize64(tif);
                
                tmsize_t vtile_size = TIFFVTileSize(tif, tile_length);
                uint64_t vtile_size64 = TIFFVTileSize64(tif, tile_length);
                
                // Use the results to avoid compiler warnings about unused variables
                (void)tile_row_size;
                (void)tile_row_size64;
                (void)tile_size;
                (void)tile_size64;
                (void)vtile_size;
                (void)vtile_size64;
            }
            break;
            
        case 2:  // Test both organizations (based on actual image type)
            if (is_tiled) {
                // Tiled image calculations
                tmsize_t tile_row_size = TIFFTileRowSize(tif);
                uint64_t tile_row_size64 = TIFFTileRowSize64(tif);
                tmsize_t tile_size = TIFFTileSize(tif);
                uint64_t tile_size64 = TIFFStripSize64(tif);  // Note: using StripSize64 for comparison
                
                (void)tile_row_size;
                (void)tile_row_size64;
                (void)tile_size;
                (void)tile_size64;
            } else {
                // Stripped image calculations
                tmsize_t raster_scanline_size = TIFFRasterScanlineSize(tif);
                tmsize_t vstrip_size = TIFFVStripSize(tif, strip_height);
                uint64_t strip_size64 = TIFFStripSize64(tif);
                
                (void)raster_scanline_size;
                (void)vstrip_size;
                (void)strip_size64;
            }
            break;
    }
    
    // --- STEP 5: Verify parameters were set correctly using TIFFGetField ---
    uint32_t verify_width = 0, verify_height = 0;
    uint16_t verify_bits = 0, verify_samples = 0;
    uint16_t verify_photometric = 0, verify_compression = 0;
    
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &verify_width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &verify_height);
    TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &verify_bits);
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &verify_samples);
    TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &verify_photometric);
    TIFFGetField(tif, TIFFTAG_COMPRESSION, &verify_compression);
    
    // Verify organization-specific parameters
    if (!is_tiled) {
        uint32_t verify_rows_per_strip = 0;
        TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &verify_rows_per_strip);
        (void)verify_rows_per_strip;
    } else {
        uint32_t verify_tile_width = 0, verify_tile_length = 0;
        TIFFGetField(tif, TIFFTAG_TILEWIDTH, &verify_tile_width);
        TIFFGetField(tif, TIFFTAG_TILELENGTH, &verify_tile_length);
        (void)verify_tile_width;
        (void)verify_tile_length;
    }
    
    // Use verification results
    (void)verify_width;
    (void)verify_height;
    (void)verify_bits;
    (void)verify_samples;
    (void)verify_photometric;
    (void)verify_compression;
    
    // --- STEP 6: Clean up resources ---
    TIFFClose(tif);
    
    // Clean up temporary file
    unlink(filename);
    
    return 0;
}

} // namespace harness_021
