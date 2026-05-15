#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fuzzer/FuzzedDataProvider.h>
#include <tiffio.h>
#include <vector>
#include <string>

namespace harness_012 {
/*
 * Fuzzing harness for libtiff library focusing on EXIF and GPS directory reading operations
 * Targets TIFFReadEXIFDirectory and TIFFReadGPSDirectory with 423+ undiscovered branches as identified by CoverageAnalyzerAgent
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_004: Tests TIFFReadGPSDirectory and TIFFReadEXIFDirectory with random offsets
 *   but doesn't test comprehensive offset variations
 * - harness_010: Focuses on comprehensive directory manipulation lifecycle
 * - harness_012: Specifically targets EXIF and GPS directory reading with various offset strategies
 * 
 * Testing strategy:
 * 1. Create TIFF file with multiple directories (regular, EXIF, GPS)
 * 2. Test TIFFReadEXIFDirectory with valid and invalid offsets
 * 3. Test TIFFReadGPSDirectory with valid and invalid offsets
 * 4. Use FuzzedDataProvider for structured input processing
 */


// Error handler to suppress library error messages during fuzzing
extern "C" void handle_error(const char *module, const char *fmt, va_list ap) {
    (void)module;
    (void)fmt;
    (void)ap;
    return;
}


static TIFF* create_multi_directory_tiff(FuzzedDataProvider& fdp, const char* filename) {
    // Open TIFF file for writing
    TIFF* tif = TIFFOpen(filename, "w+");
    if (!tif) {
        return nullptr;
    }
    
    // Set error handlers
    TIFFSetErrorHandler(handle_error);
    TIFFSetWarningHandler(handle_error);
    
    // Create main directory with basic image
    uint32_t width = fdp.ConsumeIntegralInRange<uint32_t>(1, 512);
    uint32_t height = fdp.ConsumeIntegralInRange<uint32_t>(1, 512);
    
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 3);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, height);
    
    // Write main directory
    if (!TIFFWriteDirectory(tif)) {
        TIFFClose(tif);
        return nullptr;
    }
    
    // Create EXIF directory if we have enough input
    if (fdp.remaining_bytes() > 50) {
        if (TIFFCreateEXIFDirectory(tif) == 0) {
            // Create EXIF directory without setting specific EXIF tags
            uint64_t exif_offset = 0;
            TIFFWriteCustomDirectory(tif, &exif_offset);
            TIFFSetDirectory(tif, 0);
            TIFFSetField(tif, TIFFTAG_EXIFIFD, exif_offset);
        }
    }
    
    // Create GPS directory if we have enough input
    if (fdp.remaining_bytes() > 50) {
        if (TIFFCreateGPSDirectory(tif) == 0) {
            // Create GPS directory without setting specific GPS tags
            // Just create the directory structure
            uint64_t gps_offset = 0;
            TIFFWriteCustomDirectory(tif, &gps_offset);
            TIFFSetDirectory(tif, 0);
            TIFFSetField(tif, TIFFTAG_GPSIFD, gps_offset);
        }
    }
    
    // Rewind to first directory for reading
    TIFFSetDirectory(tif, 0);
    return tif;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size check - need sufficient data for meaningful testing
    if (size < 300) {
        return 0;
    }
    
    // Initialize FuzzedDataProvider for structured input processing
    FuzzedDataProvider fdp(data, size);
    
    // Create a temporary TIFF file with multiple directory types
    const char* filename = "/tmp/fuzzed_dir_read_tiff.tif";
    
    // Create the test TIFF file
    TIFF* tif = create_multi_directory_tiff(fdp, filename);
    if (!tif) {
        return 0;
    }
    
    // Get total number of directories
    tdir_t total_dirs = TIFFNumberOfDirectories(tif);
    if (total_dirs == 0) {
        TIFFClose(tif);
        remove(filename);
        return 0;
    }
    
    // Determine which directory reading operations to test
    uint8_t operation_mask = fdp.ConsumeIntegral<uint8_t>();
    
    /**********************************************************************
     * TEST 1: TIFFReadEXIFDirectory with various offsets
     **********************************************************************/
    if (operation_mask & 0x01) {
        // Test EXIF directory reading
        int num_exif_tests = fdp.ConsumeIntegralInRange<int>(1, 5);
        
        for (int i = 0; i < num_exif_tests && fdp.remaining_bytes() >= 8; i++) {
            uint64_t exif_offset = fdp.ConsumeIntegral<uint64_t>();
            
            // Try to read EXIF directory at the specified offset
            TIFFReadEXIFDirectory(tif, exif_offset);
            
            // Test with known EXIF offset if available
            if (i == 0) {
                uint64_t known_exif_offset = 0;
                if (TIFFGetField(tif, TIFFTAG_EXIFIFD, &known_exif_offset)) {
                    TIFFReadEXIFDirectory(tif, known_exif_offset);
                }
            }
        }
        
        // Test with current directory offset variations
        if (fdp.remaining_bytes() >= 8) {
            uint64_t current_offset = TIFFCurrentDirOffset(tif);
            // Test with offset + small variations
            for (int j = -2; j <= 2; j++) {
                if (j != 0) {
                    TIFFReadEXIFDirectory(tif, current_offset + j);
                }
            }
        }
    }
    
    /**********************************************************************
     * TEST 2: TIFFReadGPSDirectory with various offsets
     **********************************************************************/
    if (operation_mask & 0x02) {
        // Test GPS directory reading
        int num_gps_tests = fdp.ConsumeIntegralInRange<int>(1, 5);
        
        for (int i = 0; i < num_gps_tests && fdp.remaining_bytes() >= 8; i++) {
            uint64_t gps_offset = fdp.ConsumeIntegral<uint64_t>();
            
            // Try to read GPS directory at the specified offset
            TIFFReadGPSDirectory(tif, gps_offset);
            
            // Test with known GPS offset if available
            if (i == 0) {
                uint64_t known_gps_offset = 0;
                if (TIFFGetField(tif, TIFFTAG_GPSIFD, &known_gps_offset)) {
                    TIFFReadGPSDirectory(tif, known_gps_offset);
                }
            }
        }
        
        // Test with boundary offsets
        if (fdp.remaining_bytes() >= 16) {
            uint64_t boundary_offsets[] = {
                0,
                1,
                fdp.ConsumeIntegral<uint64_t>(),
                UINT64_MAX
            };
            
            for (size_t j = 0; j < sizeof(boundary_offsets)/sizeof(boundary_offsets[0]); j++) {
                TIFFReadGPSDirectory(tif, boundary_offsets[j]);
            }
        }
    }
    
    /**********************************************************************
     * TEST 3: Combined operations and error handling
     **********************************************************************/

    if (operation_mask & 0x04 && fdp.remaining_bytes() > 20) {
        // Test navigation between directories and reading operations
        tdir_t target_dir = fdp.ConsumeIntegral<tdir_t>() % (total_dirs + 1);
        
        if (TIFFSetDirectory(tif, target_dir)) {
            // Get current offset and test both EXIF and GPS directory reading
            uint64_t current_offset = TIFFCurrentDirOffset(tif);
            
            // Test EXIF and GPS directory reading at current offset
            TIFFReadEXIFDirectory(tif, current_offset);
            TIFFReadGPSDirectory(tif, current_offset);
            
            // Test with offset variations
            if (fdp.remaining_bytes() >= 8) {
                uint64_t offset_variation = fdp.ConsumeIntegral<uint64_t>();
                TIFFReadEXIFDirectory(tif, current_offset + offset_variation);
                TIFFReadGPSDirectory(tif, current_offset + offset_variation);
            }
        }
    }
    
    // Clean up
    TIFFClose(tif);
    remove(filename);
    
    return 0;
}

} // namespace harness_012
