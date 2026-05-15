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

namespace harness_012 {
/* Fuzzing harness for libtiff targeting comprehensive TIFF directory manipulation and strile array writing operations
 * Focus: Directory lifecycle management and strile array writing with 472+ undiscovered branches
 * Strategy: Test complete directory manipulation lifecycle: Create -> Defer/Force Writing -> Checkpoint -> Rewrite -> Custom Write
 * Target APIs: TIFFDeferStrileArrayWriting (Primary - completely uncovered), TIFFForceStrileArrayWriting, 
 *              TIFFCheckpointDirectory, TIFFRewriteDirectory, TIFFWriteCustomDirectory
 * Supporting APIs: TIFFOpenExt/TIFFFdOpenExt, TIFFCreateDirectory, TIFFSetField, TIFFClose
 * Coverage Gap: These directory manipulation APIs are either 0% covered or have minimal coverage with high complexity
 */


/* Error handler to suppress libtiff error messages during fuzzing */
extern "C" void handle_error(const char* unused, const char* unused2, va_list unused3) {
    // Suppress error messages during fuzzing
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for configuration, tags, and operations
    if (size < 128) {
        return 0;
    }

    // Set error handlers to suppress libtiff messages
    TIFFSetErrorHandler(handle_error);
    TIFFSetWarningHandler(handle_error);

    FuzzedDataProvider fdp(data, size);

    // Consume operation mode to determine which combination of APIs to test
    uint8_t operation_mode = fdp.ConsumeIntegral<uint8_t>() % 4;  // 0: basic defer/force, 1: checkpoint/rewrite, 2: custom directory, 3: all operations
    
    // Consume TIFF file configuration parameters
    uint32_t width = fdp.ConsumeIntegralInRange<uint32_t>(1, 2048);
    uint32_t height = fdp.ConsumeIntegralInRange<uint32_t>(1, 2048);
    uint16_t samples_per_pixel = fdp.ConsumeIntegralInRange<uint16_t>(1, 4);
    uint16_t bits_per_sample = fdp.ConsumeIntegralInRange<uint16_t>(1, 32);
    uint16_t compression = fdp.ConsumeIntegral<uint16_t>() % 10; // Various compression schemes
    uint16_t photometric = fdp.ConsumeIntegral<uint16_t>() % 6; // Photometric interpretations
    uint16_t planar_config = fdp.ConsumeIntegral<uint16_t>() % 2; // PLANARCONFIG_CONTIG or SEPARATE
    uint16_t fillorder = fdp.ConsumeIntegral<uint16_t>() % 2; // FILLORDER_MSB2LSB or LSB2MSB
    
    // Consume directory and strile configuration
    bool use_tiles = fdp.ConsumeBool();
    uint32_t tile_width = fdp.ConsumeIntegralInRange<uint32_t>(16, 512);
    uint32_t tile_height = fdp.ConsumeIntegralInRange<uint32_t>(16, 512);
    uint32_t rows_per_strip = fdp.ConsumeIntegralInRange<uint32_t>(1, 256);
    
    // Consume custom directory offset for TIFFWriteCustomDirectory
    uint64_t custom_dir_offset = fdp.ConsumeIntegral<uint64_t>();
    
    // Consume remaining data for potential image data
    std::vector<uint8_t> image_data = fdp.ConsumeRemainingBytes<uint8_t>();
    
    // Create a temporary filename for writing
    char filename[] = "/tmp/tiff_fuzz_directory_XXXXXX.tif";
    int fd = mkstemps(filename, 4);
    if (fd < 0) {
        return 0;
    }
    close(fd); // We'll open via TIFFOpen, not file descriptor

    // Open TIFF file for writing using TIFFOpenExt for more control
    TIFF* tif = TIFFOpen(filename, "w");
    if (!tif) {
        unlink(filename);
        return 0;
    }

    // Create initial directory structure using TIFFCreateDirectory
    TIFFCreateDirectory(tif);
    
    // Set basic image parameters using TIFFSetField (required for directory operations)
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planar_config);
    TIFFSetField(tif, TIFFTAG_FILLORDER, fillorder);
    
    // Set organization-specific parameters
    if (use_tiles) {
        TIFFSetField(tif, TIFFTAG_TILEWIDTH, tile_width);
        TIFFSetField(tif, TIFFTAG_TILELENGTH, tile_height);
    } else {
        TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, rows_per_strip);
    }
    
    // Set additional tags to create more complex directory structure
    TIFFSetField(tif, TIFFTAG_RESOLUTIONUNIT, RESUNIT_INCH);
    TIFFSetField(tif, TIFFTAG_XRESOLUTION, 72.0);
    TIFFSetField(tif, TIFFTAG_YRESOLUTION, 72.0);
    
    // Test 1: TIFFDeferStrileArrayWriting - Defer strile array writing (Primary Target)
    if (operation_mode == 0 || operation_mode == 3) {
        int defer_result = TIFFDeferStrileArrayWriting(tif);
        // Result can be 0 or 1 depending on internal state, both are valid
        
        // Test 2: TIFFForceStrileArrayWriting - Force deferred writes to complete
        // This should be called after TIFFDeferStrileArrayWriting
        int force_result = TIFFForceStrileArrayWriting(tif);
        // Result can be 0 or 1 depending on internal state
    }
    
    // Test 3: TIFFCheckpointDirectory - Create directory checkpoint
    if (operation_mode == 1 || operation_mode == 3) {
        int checkpoint_result = TIFFCheckpointDirectory(tif);
        // Result indicates success (1) or failure (0)
    }
    
    // Test 4: TIFFRewriteDirectory - Rewrite current directory
    if (operation_mode == 1 || operation_mode == 3) {
        int rewrite_result = TIFFRewriteDirectory(tif);
        // Result indicates success (1) or failure (0)
    }
    
    // Test 5: TIFFWriteCustomDirectory - Write custom directory format
    if (operation_mode == 2 || operation_mode == 3) {
        // TIFFWriteCustomDirectory requires a pointer to offset where directory was written
        uint64_t dir_offset = custom_dir_offset;
        int custom_dir_result = TIFFWriteCustomDirectory(tif, &dir_offset);
        // Result indicates success (1) or failure (0), dir_offset gets updated
    }
    
    // Create another directory to test multi-directory scenarios
    TIFFCreateDirectory(tif);
    
    // Set different parameters in second directory
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width / 2);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height / 2);
    TIFFSetField(tif, TIFFTAG_PAGENUMBER, 0, 1); // Current page, total pages
    
    // Test directory operations on second directory
    if (operation_mode == 3) {
        // Test defer/force on second directory
        TIFFDeferStrileArrayWriting(tif);
        TIFFForceStrileArrayWriting(tif);
        
        // Test checkpoint on second directory
        TIFFCheckpointDirectory(tif);
    }
    
    // Write some minimal image data if available to exercise strile writing paths
    if (!image_data.empty() && image_data.size() > 64) {
        if (use_tiles) {
            // Write a single tile if using tiled organization
            uint32_t tile_index = 0;
            size_t tile_size = std::min(image_data.size(), (size_t)(tile_width * tile_height * samples_per_pixel * bits_per_sample / 8));
            if (tile_size > 0) {
                TIFFWriteEncodedTile(tif, tile_index, image_data.data(), tile_size);
            }
        } else {
            // Write a single strip if using strip organization
            uint32_t strip_index = 0;
            size_t strip_size = std::min(image_data.size(), (size_t)(width * rows_per_strip * samples_per_pixel * bits_per_sample / 8));
            if (strip_size > 0) {
                TIFFWriteEncodedStrip(tif, strip_index, image_data.data(), strip_size);
            }
        }
    }
    
    // Test final directory operations after writing data
    if (operation_mode == 3) {
        // Final checkpoint after data writing
        TIFFCheckpointDirectory(tif);
        
        // Final rewrite after data writing
        TIFFRewriteDirectory(tif);
    }
    
    // Clean up - close TIFF handle
    TIFFClose(tif);
    
    // Remove temporary file
    unlink(filename);
    
    return 0;
}

} // namespace harness_012
