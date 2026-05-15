#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fuzzer/FuzzedDataProvider.h>
#include <tiffio.h>

namespace harness_003 {
/*
 * Fuzzing harness for libtiff library focusing on directory manipulation functions
 * Targets completely uncovered directory APIs: TIFFSetDirectory, TIFFPrintDirectory, 
 * TIFFCurrentDirectory, TIFFLastDirectory, TIFFSetSubDirectory, TIFFNumberOfDirectories,
 * TIFFCurrentDirOffset, and related directory navigation functions
 * Tests multi-directory TIFF files, directory navigation, printing, and subdirectory operations
 * Uses FuzzedDataProvider to process input dynamically
 * Semantically differentiated from:
 * - harness_000 (basic reading)
 * - harness_001 (writing) 
 * - harness_002 (RGBA reading)
 */


// Error handler to suppress library error messages during fuzzing
extern "C" void handle_error(const char *module, const char *fmt, va_list ap) {
    (void)module;
    (void)fmt;
    (void)ap;
    return;
}

// Helper function to create a multi-directory TIFF file with fuzzed content
static TIFF* create_multi_directory_tiff(FuzzedDataProvider& fdp, const char* filename, size_t num_dirs) {
    // Open TIFF file for writing
    TIFF* tif = TIFFOpen(filename, "w");
    if (!tif) {
        return nullptr;
    }
    
    // Set error handlers
    TIFFSetErrorHandler(handle_error);
    TIFFSetWarningHandler(handle_error);
    
    // Create multiple directories with different configurations
    for (size_t dir_idx = 0; dir_idx < num_dirs; dir_idx++) {
        // Basic image parameters
        uint32_t width = fdp.ConsumeIntegralInRange<uint32_t>(1, 1024);
        uint32_t height = fdp.ConsumeIntegralInRange<uint32_t>(1, 1024);
        uint16_t samples_per_pixel = fdp.ConsumeIntegralInRange<uint16_t>(1, 4);
        uint16_t bits_per_sample = fdp.ConsumeIntegralInRange<uint16_t>(1, 32);
        
        // Photometric interpretation from fuzzed input
        const uint16_t photometric_values[] = {0, 1, 2, 3, 4, 5, 6, 8, 9, 10};
        uint16_t photometric_idx = fdp.ConsumeIntegral<uint16_t>() % (sizeof(photometric_values)/sizeof(photometric_values[0]));
        uint16_t photometric = photometric_values[photometric_idx];
        
        // Compression from fuzzed input
        const uint16_t compression_values[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 32773, 32946, 50000, 50001};
        uint16_t compression_idx = fdp.ConsumeIntegral<uint16_t>() % (sizeof(compression_values)/sizeof(compression_values[0]));
        uint16_t compression = compression_values[compression_idx];
        
        // Set basic TIFF directory fields
        TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
        TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
        TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
        TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
        TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
        TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);
        
        // Planar configuration
        uint16_t planar_config = fdp.ConsumeIntegral<uint16_t>() % 2 + 1;
        TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planar_config);
        
        // Write some dummy pixel data if we have enough fuzzed input
        if (fdp.remaining_bytes() > 100) {
            size_t pixel_data_size = width * height * samples_per_pixel * (bits_per_sample / 8);
            if (pixel_data_size > 0 && pixel_data_size < 65536 && fdp.remaining_bytes() >= pixel_data_size) {
                std::vector<uint8_t> pixel_data = fdp.ConsumeBytes<uint8_t>(pixel_data_size);
                
                // Write scanlines for simple image
                if (samples_per_pixel == 1 && bits_per_sample == 8) {
                    for (uint32_t row = 0; row < height; row++) {
                        if (row * width < pixel_data.size()) {
                            uint8_t* row_data = &pixel_data[row * width];
                            TIFFWriteScanline(tif, row_data, row, 0);
                        }
                    }
                }
            }
        }
        
        // Write directory (except for last iteration if we want to test partial writes)
        if (dir_idx < num_dirs - 1 || fdp.ConsumeBool()) {
            if (!TIFFWriteDirectory(tif)) {
                TIFFClose(tif);
                return nullptr;
            }
        }
    }
    
    return tif;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size check - need at least some data for meaningful testing
    if (size < 200) {
        return 0;
    }
    
    // Initialize FuzzedDataProvider for structured input processing
    FuzzedDataProvider fdp(data, size);
    
    // Create a temporary multi-directory TIFF file
    const char* filename = "/tmp/fuzzed_multi_dir_tiff.tif";
    
    // Determine number of directories to create (1-20)
    size_t num_directories = fdp.ConsumeIntegralInRange<size_t>(1, 20);
    
    // Create the multi-directory TIFF file
    TIFF* tif = create_multi_directory_tiff(fdp, filename, num_directories);
    if (!tif) {
        // Clean up any partial file
        remove(filename);
        return 0;
    }
    
    // Close the writing handle
    TIFFClose(tif);
    
    // Now open the file for reading and test directory operations
    tif = TIFFOpen(filename, "r");
    if (!tif) {
        remove(filename);
        return 0;
    }
    
    // Set error handlers for reading operations
    TIFFSetErrorHandler(handle_error);
    TIFFSetWarningHandler(handle_error);
    
    // Get total number of directories
    tdir_t total_dirs = TIFFNumberOfDirectories(tif);
    
    // Test TIFFCurrentDirectory - should be 0 initially
    tdir_t current_dir = TIFFCurrentDirectory(tif);
    
    // Test directory navigation based on fuzzed input
    uint8_t navigation_strategy = fdp.ConsumeIntegral<uint8_t>() % 5;
    
    switch (navigation_strategy) {
        case 0: {
            // Sequential navigation through all directories
            for (tdir_t dir_num = 0; dir_num < total_dirs; dir_num++) {
                if (TIFFSetDirectory(tif, dir_num)) {
                    // Test TIFFCurrentDirectory after setting
                    tdir_t verified_dir = TIFFCurrentDirectory(tif);
                    
                    // Test TIFFLastDirectory
                    int is_last = TIFFLastDirectory(tif);
                    
                    // Test TIFFCurrentDirOffset
                    uint64_t dir_offset = TIFFCurrentDirOffset(tif);
                    (void)dir_offset; // Use to avoid unused variable warning
                    
                    // Test TIFFPrintDirectory with different flags
                    if (fdp.ConsumeBool()) {
                        // Print to null device to avoid console spam
                        FILE* null_output = fopen("/dev/null", "w");
                        if (null_output) {
                            long print_flags = fdp.ConsumeIntegral<long>() & 0xFFFF;
                            TIFFPrintDirectory(tif, null_output, print_flags);
                            fclose(null_output);
                        }
                    }
                }
            }
            break;
        }
        
        case 1: {
            // Random directory navigation
            uint8_t num_random_jumps = fdp.ConsumeIntegralInRange<uint8_t>(1, 20);
            for (uint8_t i = 0; i < num_random_jumps; i++) {
                if (fdp.remaining_bytes() > 0) {
                    tdir_t target_dir = fdp.ConsumeIntegral<tdir_t>() % (total_dirs + 5); // Include out-of-bounds
                    TIFFSetDirectory(tif, target_dir);
                    
                    // Test current directory after attempt
                    tdir_t new_current = TIFFCurrentDirectory(tif);
                    (void)new_current;
                    
                    // Test if we're at last directory
                    if (TIFFLastDirectory(tif)) {
                        // Try to go past last directory
                        TIFFSetDirectory(tif, total_dirs + 1);
                    }
                }
            }
            break;
        }
        
        case 2: {
            // Back-and-forth navigation
            if (total_dirs > 1) {
                // Go to first directory
                TIFFSetDirectory(tif, 0);
                
                // Go to middle directory
                if (total_dirs > 2) {
                    TIFFSetDirectory(tif, total_dirs / 2);
                }
                
                // Go to last directory
                TIFFSetDirectory(tif, total_dirs - 1);
                
                // Go back to first
                TIFFSetDirectory(tif, 0);
                
                // Test TIFFLastDirectory at different positions
                for (tdir_t dir_num = 0; dir_num < total_dirs; dir_num++) {
                    TIFFSetDirectory(tif, dir_num);
                    TIFFLastDirectory(tif);
                }
            }
            break;
        }
        
        case 3: {
            // Test TIFFSetSubDirectory with various offsets
            // First get some directory offsets
            std::vector<uint64_t> dir_offsets;
            for (tdir_t dir_num = 0; dir_num < total_dirs; dir_num++) {
                if (TIFFSetDirectory(tif, dir_num)) {
                    dir_offsets.push_back(TIFFCurrentDirOffset(tif));
                }
            }
            
            // Try to set subdirectories with various offsets
            if (!dir_offsets.empty()) {
                uint8_t num_subdir_tests = fdp.ConsumeIntegralInRange<uint8_t>(1, 10);
                for (uint8_t i = 0; i < num_subdir_tests && fdp.remaining_bytes() > 0; i++) {
                    // Choose an offset - either valid or random
                    uint64_t test_offset;
                    if (fdp.ConsumeBool() && !dir_offsets.empty()) {
                        size_t offset_idx = fdp.ConsumeIntegral<size_t>() % dir_offsets.size();
                        test_offset = dir_offsets[offset_idx];
                    } else {
                        test_offset = fdp.ConsumeIntegral<uint64_t>();
                    }
                    
                    // Try to set subdirectory
                    TIFFSetSubDirectory(tif, test_offset);
                    
                    // Check current directory after attempt
                    TIFFCurrentDirectory(tif);
                }
            }
            break;
        }
        
        case 4: {
            // Comprehensive directory printing test
            // Create a temporary file for directory output
            const char* print_filename = "/tmp/tiff_print_output.txt";
            FILE* print_file = fopen(print_filename, "w");
            if (print_file) {
                // Print directories with various flags
                for (tdir_t dir_num = 0; dir_num < total_dirs; dir_num++) {
                    if (TIFFSetDirectory(tif, dir_num)) {
                        // Different print flags based on fuzzed input
                        long print_flags = 0;
                        if (fdp.ConsumeBool()) print_flags |= TIFFPRINT_NONE;
                        if (fdp.ConsumeBool()) print_flags |= TIFFPRINT_STRIPS;
                        if (fdp.ConsumeBool()) print_flags |= TIFFPRINT_CURVES;
                        if (fdp.ConsumeBool()) print_flags |= TIFFPRINT_COLORMAP;
                        if (fdp.ConsumeBool()) print_flags |= TIFFPRINT_JPEGQTABLES;
                        if (fdp.ConsumeBool()) print_flags |= TIFFPRINT_JPEGACTABLES;
                        if (fdp.ConsumeBool()) print_flags |= TIFFPRINT_JPEGDCTABLES;
                        
                        TIFFPrintDirectory(tif, print_file, print_flags);
                        
                        // Also test with default flags (0)
                        if (fdp.ConsumeBool()) {
                            TIFFPrintDirectory(tif, print_file, 0);
                        }
                    }
                }
                fclose(print_file);
                remove(print_filename);
            }
            break;
        }
    }
    
    // Additional comprehensive directory API tests
    
    // Test edge cases for TIFFSetDirectory
    if (fdp.ConsumeBool()) {
        // Try to set directory beyond total
        TIFFSetDirectory(tif, total_dirs + 1);
        
        // Try to set directory to max possible value
        TIFFSetDirectory(tif, 0xFFFFFFFF);
        
        // Return to a valid directory
        if (total_dirs > 0) {
            TIFFSetDirectory(tif, 0);
        }
    }
    
    // Test TIFFLastDirectory at various positions
    if (total_dirs > 0) {
        for (tdir_t test_dir = 0; test_dir < total_dirs; test_dir++) {
            TIFFSetDirectory(tif, test_dir);
            int is_last = TIFFLastDirectory(tif);
            (void)is_last;
        }
    }
    
    // Test directory offset tracking
    if (fdp.ConsumeBool() && total_dirs > 0) {
        std::vector<uint64_t> offsets;
        for (tdir_t dir_num = 0; dir_num < total_dirs; dir_num++) {
            if (TIFFSetDirectory(tif, dir_num)) {
                offsets.push_back(TIFFCurrentDirOffset(tif));
            }
        }
        
        // Verify offsets are unique (not required but good to check)
        for (size_t i = 0; i < offsets.size(); i++) {
            for (size_t j = i + 1; j < offsets.size(); j++) {
                if (offsets[i] == offsets[j]) {
                    // Duplicate offset found - interesting case for fuzzer
                    break;
                }
            }
        }
    }
    
    // Test combination of directory operations
    if (fdp.ConsumeBool() && total_dirs > 1) {
        // Complex navigation pattern
        TIFFSetDirectory(tif, 0);
        TIFFCurrentDirectory(tif);
        
        TIFFSetDirectory(tif, total_dirs - 1);
        TIFFLastDirectory(tif);
        
        TIFFSetDirectory(tif, total_dirs / 2);
        TIFFCurrentDirOffset(tif);
        
        // Try to print current directory
        FILE* null_out = fopen("/dev/null", "w");
        if (null_out) {
            TIFFPrintDirectory(tif, null_out, fdp.ConsumeIntegral<long>() & 0xFFFF);
            fclose(null_out);
        }
    }
    
    // Clean up
    TIFFClose(tif);
    remove(filename);
    
    return 0;
}

} // namespace harness_003
