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

namespace harness_016 {
/*
 * Fuzzing harness for libtiff directory manipulation functions
 * Target APIs: TIFFWriteCustomDirectory, TIFFRewriteDirectory, and related directory APIs
 * 
 * Coverage analysis indicates these are critical uncovered functions:
 * - TIFFWriteCustomDirectory: 135-276 undiscovered branches (high complexity)
 * - TIFFRewriteDirectory: Similar high complexity in tif_dirwrite.c (30.41% coverage)
 * 
 * Related directory APIs to target:
 * - TIFFWriteDirectory (base function)
 * - TIFFCheckpointDirectory
 * - TIFFCreateCustomDirectory
 * - TIFFCreateEXIFDirectory
 * - TIFFCreateGPSDirectory
 * - TIFFSetDirectory, TIFFSetSubDirectory
 * 
 * These functions are part of the directory write/rewrite logic in tif_dirwrite.c
 * and tif_dir.c (60.64% coverage) which are the most critical coverage gaps.
 * 
 * Semantic differentiation from previous harnesses (harness_000 through harness_015):
 * - No existing harnesses target custom directory creation/manipulation
 * - Focuses on directory write/rewrite operations (not reading)
 * - Tests directory rewriting and checkpointing
 * - Tests offset-based directory operations
 */



// Error handler to suppress warnings during fuzzing
extern "C" void tiff_error_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress errors during fuzzing
    return;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for various configuration parameters
    if (size < 128) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Set error handlers to suppress warnings
    TIFFSetErrorHandler(NULL);
    TIFFSetWarningHandler(NULL);

    // Create a temporary file for TIFF operations
    char temp_file_template[] = "/tmp/fuzz_tiff_custom_dir_XXXXXX";
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

    // Determine which test case to run based on fuzzer input
    uint8_t test_case = fdp.ConsumeIntegral<uint8_t>() % 6;
    
    // Common parameters consumed from fuzzer input
    uint32_t width = fdp.ConsumeIntegralInRange<uint32_t>(1, 4096);
    uint32_t height = fdp.ConsumeIntegralInRange<uint32_t>(1, 4096);
    uint16_t bits_per_sample = fdp.ConsumeIntegralInRange<uint16_t>(1, 32);
    uint16_t samples_per_pixel = fdp.ConsumeIntegralInRange<uint16_t>(1, 4);
    uint16_t photometric = fdp.ConsumeIntegral<uint16_t>() % 6;
    uint32_t rows_per_strip = fdp.ConsumeIntegralInRange<uint32_t>(1, 100);
    
    TIFF* tif = nullptr;
    int result = 0;
    uint64_t dir_offset = 0;
    
    switch (test_case) {
        case 0: {
            // Test 1: Standard directory creation and writing
            tif = TIFFOpen(temp_filename.c_str(), "w");
            if (!tif) {
                unlink(temp_filename.c_str());
                return 0;
            }
            
            // Set basic fields
            TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
            TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
            TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
            TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
            TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
            TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, rows_per_strip);
            
            // Write the directory
            result = TIFFWriteDirectory(tif);
            
            // Try to write another directory with different parameters
            if (result) {
                width = fdp.ConsumeIntegralInRange<uint32_t>(1, 4096);
                height = fdp.ConsumeIntegralInRange<uint32_t>(1, 4096);
                
                TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
                TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
                
                result = TIFFWriteDirectory(tif);
            }
            
            TIFFClose(tif);
            break;
        }
        
        case 1: {
            // Test 2: Directory rewriting
            tif = TIFFOpen(temp_filename.c_str(), "w");
            if (!tif) {
                unlink(temp_filename.c_str());
                return 0;
            }
            
            // Set initial fields and write first directory
            TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
            TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
            TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
            
            result = TIFFWriteDirectory(tif);
            
            if (result) {
                // Change some fields
                width = fdp.ConsumeIntegralInRange<uint32_t>(1, 4096);
                height = fdp.ConsumeIntegralInRange<uint32_t>(1, 4096);
                
                TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
                TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
                
                // Rewrite the directory (relocate to end of file)
                result = TIFFRewriteDirectory(tif);
            }
            
            TIFFClose(tif);
            break;
        }
        
        case 2: {
            // Test 3: Directory checkpointing
            tif = TIFFOpen(temp_filename.c_str(), "w");
            if (!tif) {
                unlink(temp_filename.c_str());
                return 0;
            }
            
            // Write first directory
            TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
            TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
            TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
            
            result = TIFFWriteDirectory(tif);
            
            if (result) {
                // Checkpoint directory (write but keep in memory)
                result = TIFFCheckpointDirectory(tif);
            }
            
            TIFFClose(tif);
            break;
        }
        
        case 3: {
            // Test 4: EXIF directory creation and writing
            tif = TIFFOpen(temp_filename.c_str(), "w");
            if (!tif) {
                unlink(temp_filename.c_str());
                return 0;
            }
            
            // Create EXIF directory
            result = TIFFCreateEXIFDirectory(tif);
            
            if (result) {
                // Write the EXIF directory with offset tracking
                result = TIFFWriteCustomDirectory(tif, &dir_offset);
            }
            
            TIFFClose(tif);
            break;
        }
        
        case 4: {
            // Test 5: GPS directory creation and writing
            tif = TIFFOpen(temp_filename.c_str(), "w");
            if (!tif) {
                unlink(temp_filename.c_str());
                return 0;
            }
            
            // Create GPS directory
            result = TIFFCreateGPSDirectory(tif);
            
            if (result) {
                // Write the GPS directory with offset tracking
                result = TIFFWriteCustomDirectory(tif, &dir_offset);
            }
            
            TIFFClose(tif);
            break;
        }
        
        case 5: {
            // Test 6: Multiple directory operations with offset tracking
            tif = TIFFOpen(temp_filename.c_str(), "w");
            if (!tif) {
                unlink(temp_filename.c_str());
                return 0;
            }
            
            // Write first directory
            TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
            TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
            
            result = TIFFWriteDirectory(tif);
            
            if (result) {
                // Try to create EXIF directory and write with offset
                if (TIFFCreateEXIFDirectory(tif)) {
                    uint64_t exif_offset = 0;
                    TIFFWriteCustomDirectory(tif, &exif_offset);
                    
                    // Try to set subdirectory by offset
                    if (exif_offset != 0) {
                        TIFFSetSubDirectory(tif, exif_offset);
                    }
                }
                
                // Try to create GPS directory and write with offset
                if (TIFFCreateGPSDirectory(tif)) {
                    uint64_t gps_offset = 0;
                    TIFFWriteCustomDirectory(tif, &gps_offset);
                }
            }
            
            TIFFClose(tif);
            break;
        }
    }
    
    // Clean up temp file
    unlink(temp_filename.c_str());
    
    return 0;
}

} // namespace harness_016
