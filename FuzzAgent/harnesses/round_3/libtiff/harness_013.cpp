#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <unistd.h>
#include <algorithm>
#include <tiffio.h>

namespace harness_013 {
/* Fuzzing harness for libtiff targeting comprehensive TIFF directory manipulation functions
 * Focus: Directory lifecycle management with three critical APIs with high undiscovered branch counts
 * Target APIs: 
 *   - TIFFUnlinkDirectory (69 undiscovered branches)
 *   - TIFFUnsetField (25 undiscovered branches) 
 *   - TIFFSetSubDirectory (204 undiscovered branches)
 * Supporting APIs: TIFFCreateDirectory, TIFFSetField, TIFFGetField, TIFFWriteDirectory, 
 *                  TIFFCurrentDirOffset, TIFFClose, TIFFCheckpointDirectory
 * Strategy: Create a multi-directory TIFF file, manipulate fields and directories, test unlinking,
 *           test field unsetting, test subdirectory navigation with actual offsets, and verify cleanup
 */


/* Error handler to suppress libtiff error messages during fuzzing */
extern "C" void handle_error(const char* unused, const char* unused2, va_list unused3) {
    // Suppress error messages during fuzzing
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for configuration, tags, and operations
    if (size < 256) {
        return 0;
    }

    // Set error handlers to suppress libtiff messages
    TIFFSetErrorHandler(handle_error);
    TIFFSetWarningHandler(handle_error);

    FuzzedDataProvider fdp(data, size);

    // Consume operation mode to determine which combination of APIs to test
    uint8_t operation_mode = fdp.ConsumeIntegral<uint8_t>() % 4;  
    // 0: Basic directory creation and unlinking
    // 1: Field setting and unsetting
    // 2: Subdirectory navigation
    // 3: Comprehensive all operations
    
    // Consume TIFF file configuration parameters
    uint32_t width = fdp.ConsumeIntegralInRange<uint32_t>(1, 1024);
    uint32_t height = fdp.ConsumeIntegralInRange<uint32_t>(1, 1024);
    uint16_t samples_per_pixel = fdp.ConsumeIntegralInRange<uint16_t>(1, 4);
    uint16_t bits_per_sample = fdp.ConsumeIntegralInRange<uint16_t>(1, 32);
    uint16_t compression = fdp.ConsumeIntegral<uint16_t>() % 10; // Various compression schemes
    uint16_t photometric = fdp.ConsumeIntegral<uint16_t>() % 6; // Photometric interpretations
    uint16_t planar_config = fdp.ConsumeIntegral<uint16_t>() % 2; // PLANARCONFIG_CONTIG or SEPARATE
    
    // Consume directory count and tag selection
    uint8_t directory_count = fdp.ConsumeIntegralInRange<uint8_t>(2, 5); // Need at least 2 for unlinking tests
    uint8_t tags_to_set = fdp.ConsumeIntegralInRange<uint8_t>(1, 10);
    uint8_t tags_to_unset = fdp.ConsumeIntegralInRange<uint8_t>(1, tags_to_set);
    
    // Consume tag values from fuzzer input
    std::vector<uint32_t> tag_values;
    std::vector<std::string> string_values;
    for (int i = 0; i < tags_to_set * 2; i++) {
        tag_values.push_back(fdp.ConsumeIntegral<uint32_t>());
    }
    for (int i = 0; i < tags_to_set; i++) {
        string_values.push_back(fdp.ConsumeRandomLengthString(50));
    }
    
    // Create a temporary filename for writing
    char filename[] = "/tmp/tiff_fuzz_dirmanip_XXXXXX.tif";
    int fd = mkstemps(filename, 4);
    if (fd < 0) {
        return 0;
    }
    close(fd); // We'll open via TIFFOpen, not file descriptor

    // Open TIFF file for writing
    TIFF* tif = TIFFOpen(filename, "w");
    if (!tif) {
        unlink(filename);
        return 0;
    }

    // Store directory offsets for TIFFSetSubDirectory testing
    std::vector<uint64_t> directory_offsets;
    
    // Create multiple directories and set various fields
    for (tdir_t dir_num = 0; dir_num < directory_count; dir_num++) {
        // Create directory (first one exists by default)
        if (dir_num > 0) {
            TIFFCreateDirectory(tif);
        }
        
        // Set basic image parameters for each directory
        TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width + dir_num); // Vary width per directory
        TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height + dir_num); // Vary height per directory
        TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
        TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
        TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);
        TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
        TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planar_config);
        
        // Set additional custom tags based on fuzzer input
        for (int tag_idx = 0; tag_idx < tags_to_set; tag_idx++) {
            uint32_t tag = 0x8000 + tag_idx * 0x100; // Use private tag space
            if (tag_idx % 3 == 0) {
                // Set integer value
                TIFFSetField(tif, tag, tag_values[tag_idx * 2]);
            } else if (tag_idx % 3 == 1) {
                // Set floating point value
                float float_val = static_cast<float>(tag_values[tag_idx * 2]) / 1000.0f;
                TIFFSetField(tif, tag, float_val);
            } else {
                // Set string value
                TIFFSetField(tif, tag, string_values[tag_idx].c_str());
            }
        }
        
        // Write directory
        TIFFWriteDirectory(tif);
    }
    
    // Close and reopen file to get directory offsets
    TIFFClose(tif);
    
    // Reopen for reading/writing to test directory manipulation
    tif = TIFFOpen(filename, "r+");
    if (!tif) {
        unlink(filename);
        return 0;
    }
    
    // First, collect directory offsets by navigating through directories
    if (operation_mode == 2 || operation_mode == 3) {
        // Navigate through all directories and collect offsets
        for (tdir_t dir_num = 0; dir_num < directory_count; dir_num++) {
            if (TIFFSetDirectory(tif, dir_num)) {
                uint64_t current_offset = TIFFCurrentDirOffset(tif);
                directory_offsets.push_back(current_offset);
            }
        }
    }
    
    // Test TIFFSetSubDirectory if we have directory offsets
    if ((operation_mode == 2 || operation_mode == 3) && !directory_offsets.empty()) {
        // Navigate through directories using TIFFSetSubDirectory with collected offsets
        for (size_t i = 0; i < directory_offsets.size(); i++) {
            // Use TIFFSetSubDirectory to jump to specific offset
            TIFFSetSubDirectory(tif, directory_offsets[i]);
            
            // Verify we're at the expected directory
            tdir_t current_dir = TIFFCurrentDirectory(tif);
            // Note: After TIFFSetSubDirectory, current_dir may be 0 (for SubIFD)
            // Just exercise the API without checking exact directory number
            
            // Try to read some fields to ensure directory is loaded
            uint32_t current_width = 0;
            TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &current_width);
        }
        
        // Test edge cases for TIFFSetSubDirectory
        // 1. Test with offset 0 (should invalidate current directory)
        TIFFSetSubDirectory(tif, 0);
        
        // 2. Test with invalid offset (should fail but not crash)
        TIFFSetSubDirectory(tif, 10); // Small invalid offset
        
        // 3. Test with large invalid offset
        TIFFSetSubDirectory(tif, 0xFFFFFFFFFFFFFFFFULL); // Maximum offset
        
        // Return to a valid directory for further tests
        if (!directory_offsets.empty()) {
            TIFFSetSubDirectory(tif, directory_offsets[0]);
        }
    }
    
    // Test TIFFUnsetField - unset some previously set tags
    if (operation_mode == 1 || operation_mode == 3) {
        // Go to first directory
        TIFFSetDirectory(tif, 0);
        
        // Unset some tags
        for (int tag_idx = 0; tag_idx < tags_to_unset; tag_idx++) {
            uint32_t tag = 0x8000 + tag_idx * 0x100; // Use same tag space as before
            TIFFUnsetField(tif, tag);
        }
        
        // Verify tags were unset by trying to get them
        for (int tag_idx = 0; tag_idx < tags_to_unset; tag_idx++) {
            uint32_t tag = 0x8000 + tag_idx * 0x100;
            uint32_t value = 0;
            // TIFFGetField should return 0 (false) for unset tags
            int got_field = TIFFGetField(tif, tag, &value);
            // Don't check result - just exercise the code path
        }
    }
    
    // Test TIFFUnlinkDirectory - unlink some directories
    if (operation_mode == 0 || operation_mode == 3) {
        // Get current directory count
        tdir_t current_dir_count = TIFFNumberOfDirectories(tif);
        
        // Try to unlink some directories (avoid unlinking directory 0)
        if (current_dir_count > 1) {
            // Unlink middle directories first
            for (tdir_t dir_to_unlink = 1; dir_to_unlink < std::min(current_dir_count, (tdir_t)3); dir_to_unlink++) {
                TIFFUnlinkDirectory(tif, dir_to_unlink);
            }
            
            // Verify directory count changed
            tdir_t new_dir_count = TIFFNumberOfDirectories(tif);
            // Don't assert - just exercise the code
            
            // Try to navigate to remaining directories
            for (tdir_t remaining_dir = 0; remaining_dir < new_dir_count; remaining_dir++) {
                TIFFSetDirectory(tif, remaining_dir);
            }
            
            // Test TIFFSetSubDirectory with unlinked directory offsets
            if ((operation_mode == 2 || operation_mode == 3) && !directory_offsets.empty() && directory_offsets.size() > 1) {
                // Try to access what was directory 1 (now potentially unlinked)
                // This simulates accessing a SubIFD chain as shown in test_directory.c
                TIFFSetSubDirectory(tif, directory_offsets[1]);
            }
        }
    }
    
    // Test error cases for TIFFUnlinkDirectory (directory 0 should fail)
    if (operation_mode == 0 || operation_mode == 3) {
        // This should generate an error but not crash
        TIFFUnlinkDirectory(tif, 0);
    }
    
    // Test comprehensive cleanup with directory unlinking and field unsetting
    if (operation_mode == 3) {
        // Navigate through all remaining directories
        tdir_t final_dir_count = TIFFNumberOfDirectories(tif);
        for (tdir_t dir_num = 0; dir_num < final_dir_count; dir_num++) {
            if (TIFFSetDirectory(tif, dir_num)) {
                // Unset any remaining custom tags
                for (int tag_idx = tags_to_unset; tag_idx < tags_to_set; tag_idx++) {
                    uint32_t tag = 0x8000 + tag_idx * 0x100;
                    TIFFUnsetField(tif, tag);
                }
            }
        }
    }
    
    // Force a checkpoint to flush changes
    TIFFCheckpointDirectory(tif);
    
    // Clean up
    TIFFClose(tif);
    unlink(filename);
    
    return 0;
}

} // namespace harness_013
