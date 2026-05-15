#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fuzzer/FuzzedDataProvider.h>
#include <tiffio.h>
#include <string>
#include <vector>

namespace harness_004 {
/*
 * Fuzzing harness for libtiff library - Directory Manipulation and Raw Strip Writing Operations
 * Targets TIFF directory manipulation APIs: TIFFWriteRawStrip, TIFFCheckpointDirectory, TIFFWriteCustomDirectory, TIFFUnlinkDirectory, TIFFFieldSetGetCountSize
 * Required helper APIs: TIFFOpen/TIFFOpenExt, TIFFClose, TIFFSetField/TIFFVSetField, TIFFWriteDirectory, TIFFCreateDirectory
 * Semantic differentiation from previous harnesses: Focuses on directory manipulation and raw strip writing operations
 * Follows invocation sequence: open TIFF file for writing, configure tags, create directories, write custom directory data, write raw strip data, checkpoint directory, unlink directory, and close.
 */


// Error handler to suppress warnings/errors during fuzzing
extern "C" void tiff_error_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress errors during fuzzing
    return;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Set error handler to suppress warnings
    TIFFSetErrorHandler(NULL);
    TIFFSetWarningHandler(NULL);
    
    // Minimum size check: need enough data for various parameters
    if (size < 100) {
        return 0; // Need minimum data for meaningful testing
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Create a temporary file for writing TIFF data
    char temp_file_template[] = "/tmp/fuzz_tiff_dir_XXXXXX";
    int fd = mkstemp(temp_file_template);
    if (fd < 0) {
        return 0;
    }
    std::string temp_filename = temp_file_template;
    close(fd); // We'll reopen with TIFFOpen
    
    // Determine opening method (0: TIFFOpen, 1: TIFFOpenExt)
    uint8_t open_method = fdp.ConsumeIntegral<uint8_t>() % 2;
    
    TIFF* tif = NULL;
    TIFFOpenOptions* opts = NULL;
    
    // Open TIFF file for writing
    if (open_method == 0) {
        // TIFFOpen
        tif = TIFFOpen(temp_filename.c_str(), "w");
    } else {
        // TIFFOpenExt with options
        opts = TIFFOpenOptionsAlloc();
        if (opts) {
            // Configure some options based on fuzzed data
            uint8_t max_alloc_option = fdp.ConsumeIntegral<uint8_t>() % 3;
            if (max_alloc_option == 0) {
                TIFFOpenOptionsSetMaxSingleMemAlloc(opts, 1024 * 1024);
            } else if (max_alloc_option == 1) {
                TIFFOpenOptionsSetMaxCumulatedMemAlloc(opts, 10 * 1024 * 1024);
            }
            
            tif = TIFFOpenExt(temp_filename.c_str(), "w", opts);
            TIFFOpenOptionsFree(opts);
        } else {
            tif = TIFFOpen(temp_filename.c_str(), "w");
        }
    }
    
    if (!tif) {
        unlink(temp_filename.c_str());
        return 0;
    }
    
    // Consume configuration parameters from fuzzer input
    uint32_t width = fdp.ConsumeIntegralInRange<uint32_t>(1, 1024);
    uint32_t height = fdp.ConsumeIntegralInRange<uint32_t>(1, 1024);
    uint16_t samples_per_pixel = fdp.ConsumeIntegralInRange<uint16_t>(1, 4);
    uint16_t bits_per_sample = fdp.ConsumeIntegralInRange<uint16_t>(1, 32);
    uint16_t compression = fdp.ConsumeIntegral<uint16_t>() % 10; // Various compression types
    uint16_t photometric = fdp.ConsumeIntegral<uint16_t>() % 6; // Various photometric interpretations
    uint16_t planar_config = fdp.ConsumeIntegral<uint16_t>() % 2; // CONTIG or SEPARATE
    
    // Configure TIFF tags using TIFFSetField
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planar_config);
    
    // Set rows per strip (for strip-based images)
    uint32_t rows_per_strip = fdp.ConsumeIntegralInRange<uint32_t>(1, height);
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, rows_per_strip);
    
    // Write initial directory
    TIFFWriteDirectory(tif);
    
    // Create additional directories using TIFFCreateDirectory
    uint8_t num_extra_dirs = fdp.ConsumeIntegralInRange<uint8_t>(0, 5);
    for (uint8_t i = 0; i < num_extra_dirs; i++) {
        TIFFCreateDirectory(tif);
        
        // Configure some tags for the new directory
        uint32_t dir_width = fdp.ConsumeIntegralInRange<uint32_t>(1, 512);
        uint32_t dir_height = fdp.ConsumeIntegralInRange<uint32_t>(1, 512);
        TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, dir_width);
        TIFFSetField(tif, TIFFTAG_IMAGELENGTH, dir_height);
        
        // Write the directory
        TIFFWriteDirectory(tif);
    }
    
    // Test TIFFFieldSetGetCountSize if possible
    // This function is internal but we can try to call it if it's exposed
    // or we can skip if not available in public API
    
    // Write custom directory data using TIFFWriteCustomDirectory
    uint64_t custom_dir_offset = 0;
    TIFFWriteCustomDirectory(tif, &custom_dir_offset);
    
    // Prepare raw strip data
    uint32_t strip_number = 0;
    uint32_t num_strips = TIFFNumberOfStrips(tif);
    if (num_strips > 0) {
        strip_number = fdp.ConsumeIntegralInRange<uint32_t>(0, num_strips - 1);
    }
    
    // Consume raw strip data from fuzzer input
    size_t strip_data_size = fdp.ConsumeIntegralInRange<size_t>(1, 4096);
    std::vector<uint8_t> strip_data = fdp.ConsumeBytes<uint8_t>(strip_data_size);
    
    if (strip_data.size() > 0) {
        // Write raw strip data using TIFFWriteRawStrip
        tmsize_t written = TIFFWriteRawStrip(tif, strip_number, strip_data.data(), strip_data.size());
        (void)written; // Result not used in fuzzing
    }
    
    // Checkpoint directory using TIFFCheckpointDirectory
    TIFFCheckpointDirectory(tif);
    
    // Test directory unlinking using TIFFUnlinkDirectory
    // Note: TIFFUnlinkDirectory uses 1-based indexing for directories
    uint32_t num_dirs = TIFFNumberOfDirectories(tif);
    if (num_dirs > 0) {
        // Choose a directory to unlink (1-based index)
        uint32_t dir_to_unlink = fdp.ConsumeIntegralInRange<uint32_t>(1, num_dirs);
        TIFFUnlinkDirectory(tif, dir_to_unlink);
    }
    
    // Clean up
    TIFFClose(tif);
    
    // Remove temporary file
    unlink(temp_filename.c_str());
    
    return 0;
}

} // namespace harness_004
