#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fuzzer/FuzzedDataProvider.h>
#include <tiffio.h>
#include <vector>
#include <string>

namespace harness_014 {
/*
 * Fuzzing harness for libtiff library focusing on comprehensive tile writing operations
 * Targets tile writing APIs with 795+ undiscovered branches: TIFFWriteTile, TIFFWriteEncodedTile, TIFFWriteRawTile
 * Implements complete tile writing workflow: TIFFOpen/TIFFClientOpen → TIFFSetField/TIFFSetupStrips → Tile Writing → TIFFClose
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_001: Basic writing operations with tile/strip selection
 * - harness_009: Advanced tile reading and writing with multiple configurations
 * - harness_014: Specifically targets comprehensive tile writing workflow with proper initialization, 
 *   TIFFSetupStrips configuration, and thorough testing of all tile writing APIs
 * 
 * Testing strategy:
 * 1. Create TIFF files with diverse tile configurations (tile dimensions, compression, planar configs)
 * 2. Test TIFFSetupStrips for proper strip/tile setup before writing
 * 3. Exercise all three tile writing APIs with varying coordinate systems
 * 4. Test both TIFFOpen and TIFFClientOpen initialization paths
 * 5. Use FuzzedDataProvider for structured input processing
 */


// Error handler to suppress library error messages during fuzzing
extern "C" void handle_error(const char *module, const char *fmt, va_list ap) {
    (void)module;
    (void)fmt;
    (void)ap;
    return;
}

// Custom I/O handlers for TIFFClientOpen testing
static tmsize_t client_read(thandle_t handle, void* buf, tmsize_t size) {
    FuzzedDataProvider* fdp = (FuzzedDataProvider*)handle;
    if (size <= 0 || fdp->remaining_bytes() == 0) return 0;
    
    tmsize_t bytes_to_read = (tmsize_t)std::min((size_t)size, fdp->remaining_bytes());
    std::vector<uint8_t> data = fdp->ConsumeBytes<uint8_t>(bytes_to_read);
    memcpy(buf, data.data(), data.size());
    return data.size();
}

static tmsize_t client_write(thandle_t handle, void* buf, tmsize_t size) {
    // For writing, we just consume the data but don't actually store it
    // This allows testing TIFFClientOpen without actual file I/O
    (void)handle;
    (void)buf;
    (void)size;
    return size;
}

static toff_t client_seek(thandle_t handle, toff_t offset, int whence) {
    // Simple seek implementation
    (void)handle;
    (void)whence;
    return offset;
}

static int client_close(thandle_t handle) {
    (void)handle;
    return 0;
}

static toff_t client_size(thandle_t handle) {
    FuzzedDataProvider* fdp = (FuzzedDataProvider*)handle;
    return fdp->remaining_bytes();
}

// Helper function to create a tiled TIFF file with diverse configurations
static TIFF* create_tiled_tiff(FuzzedDataProvider& fdp, const char* filename, bool use_client_open) {
    TIFF* tif = nullptr;
    
    if (use_client_open && fdp.remaining_bytes() > 100) {
        // Use TIFFClientOpen for memory-based testing
        tif = TIFFClientOpen(filename, "w", (thandle_t)&fdp,
                             client_read, client_write,
                             client_seek, client_close,
                             client_size, nullptr, nullptr);
    } else {
        // Use regular TIFFOpen
        tif = TIFFOpen(filename, "w");
    }
    
    if (!tif) {
        return nullptr;
    }
    
    // Set error handlers
    TIFFSetErrorHandler(handle_error);
    TIFFSetWarningHandler(handle_error);
    
    // Consume basic image parameters from fuzzed input
    uint32_t width = fdp.ConsumeIntegralInRange<uint32_t>(16, 1024);
    uint32_t height = fdp.ConsumeIntegralInRange<uint32_t>(16, 1024);
    uint16_t samples_per_pixel = fdp.ConsumeIntegralInRange<uint16_t>(1, 4);
    uint16_t bits_per_sample = fdp.ConsumeIntegralInRange<uint16_t>(1, 32);
    
    // Photometric interpretation
    const uint16_t photometric_values[] = {
        0, 1, 2, 3, 4, 5, 6, 8, 9, 10,  // Common values
        PHOTOMETRIC_MINISWHITE, PHOTOMETRIC_MINISBLACK,
        PHOTOMETRIC_RGB, PHOTOMETRIC_PALETTE,
        PHOTOMETRIC_MASK, PHOTOMETRIC_SEPARATED,
        PHOTOMETRIC_YCBCR, PHOTOMETRIC_CIELAB
    };
    uint16_t photometric = photometric_values[
        fdp.ConsumeIntegral<uint16_t>() % (sizeof(photometric_values)/sizeof(photometric_values[0]))
    ];
    
    // Compression methods
    const uint16_t compression_values[] = {
        COMPRESSION_NONE, COMPRESSION_CCITTRLE, COMPRESSION_CCITTFAX3,
        COMPRESSION_CCITTFAX4, COMPRESSION_LZW, COMPRESSION_JPEG,
        COMPRESSION_PACKBITS, COMPRESSION_DEFLATE, COMPRESSION_ADOBE_DEFLATE
    };
    uint16_t compression = compression_values[
        fdp.ConsumeIntegral<uint16_t>() % (sizeof(compression_values)/sizeof(compression_values[0]))
    ];
    
    // Set basic TIFF directory fields
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);
    
    // Planar configuration
    uint16_t planar_config = fdp.ConsumeBool() ? PLANARCONFIG_CONTIG : PLANARCONFIG_SEPARATE;
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planar_config);
    
    // Set tile dimensions
    uint32_t tile_width = fdp.ConsumeIntegralInRange<uint32_t>(16, 256);
    uint32_t tile_length = fdp.ConsumeIntegralInRange<uint32_t>(16, 256);
    
    // Ensure tile dimensions don't exceed image dimensions
    tile_width = (tile_width > width) ? width : tile_width;
    tile_length = (tile_length > height) ? height : tile_length;
    
    TIFFSetField(tif, TIFFTAG_TILEWIDTH, tile_width);
    TIFFSetField(tif, TIFFTAG_TILELENGTH, tile_length);
    
    // Call TIFFSetupStrips to properly set up strip/tile data structures
    // This is critical for tile writing operations
    TIFFSetupStrips(tif);
    
    // Add some optional fields to create more complex directory state
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 4) {
        uint16_t orientation = fdp.ConsumeIntegral<uint16_t>() % 9;
        TIFFSetField(tif, TIFFTAG_ORIENTATION, orientation);
    }
    
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 8) {
        float xres = fdp.ConsumeFloatingPointInRange<float>(10.0f, 600.0f);
        float yres = fdp.ConsumeFloatingPointInRange<float>(10.0f, 600.0f);
        TIFFSetField(tif, TIFFTAG_XRESOLUTION, xres);
        TIFFSetField(tif, TIFFTAG_YRESOLUTION, yres);
        TIFFSetField(tif, TIFFTAG_RESOLUTIONUNIT, fdp.ConsumeIntegral<uint16_t>() % 3);
    }
    
    // Write directory to commit the configuration
    if (!TIFFWriteDirectory(tif)) {
        if (!use_client_open) {
            TIFFClose(tif);
        } else {
            // For client open, we need to handle cleanup differently
            TIFFClose(tif);
        }
        return nullptr;
    }
    
    return tif;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size check - need substantial data for tile writing testing
    if (size < 500) {
        return 0;
    }
    
    // Initialize FuzzedDataProvider for structured input processing
    FuzzedDataProvider fdp(data, size);
    
    // Determine which initialization method to use
    bool use_client_open = fdp.ConsumeBool();
    const char* filename = "/tmp/fuzzed_tile_tiff.tif";
    
    // Create a tiled TIFF file with diverse configurations
    TIFF* tif = create_tiled_tiff(fdp, filename, use_client_open);
    if (!tif) {
        return 0;
    }
    // Calculate tile size and number of tiles
    tmsize_t tile_size = TIFFTileSize(tif);
    uint32_t num_tiles = TIFFNumberOfTiles(tif);
    
    // Get image dimensions from the TIFF file
    uint32_t width = 0, height = 0;
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    
    if (tile_size <= 0 || tile_size > 1048576 || num_tiles == 0 || width == 0 || height == 0) {
        // Invalid tile configuration, clean up and exit
        TIFFClose(tif);
        if (!use_client_open) {
            remove(filename);
        }
        return 0;
    }
    // Test tile writing operations with various approaches
    // We'll try multiple tile writing operations based on available input
    size_t max_tile_ops = fdp.ConsumeIntegralInRange<size_t>(1, 10);
    
    for (size_t op_idx = 0; op_idx < max_tile_ops && fdp.remaining_bytes() > tile_size + 20; op_idx++) {
        // Allocate buffer for tile data
        void* tile_buffer = _TIFFmalloc(tile_size);
        if (!tile_buffer) {
            break;
        }
        
        // Consume tile data from fuzzed input
        std::vector<uint8_t> tile_data = fdp.ConsumeBytes<uint8_t>(tile_size);
        memcpy(tile_buffer, tile_data.data(), tile_size);
        
        // Select which tile writing API to test
        uint8_t write_op = fdp.ConsumeIntegral<uint8_t>() % 3;
        
        switch (write_op) {
            case 0: {
                // Test TIFFWriteTile with coordinate-based tile selection
                // x and y are pixel coordinates within the image
                uint32_t x = fdp.ConsumeIntegralInRange<uint32_t>(0, width - 1);
                uint32_t y = fdp.ConsumeIntegralInRange<uint32_t>(0, height - 1);
                uint32_t z = 0;  // Assume 2D image
                uint16_t sample = fdp.ConsumeIntegral<uint16_t>() % 4;  // Sample index
                
                // Try to write tile at specified coordinates
                TIFFWriteTile(tif, tile_buffer, x, y, z, sample);
                break;
            }
                
            case 1: {
                // Test TIFFWriteEncodedTile with tile index
                uint32_t tile_num = fdp.ConsumeIntegralInRange<uint32_t>(0, num_tiles - 1);
                tmsize_t write_size = fdp.ConsumeIntegralInRange<tmsize_t>(1, tile_size);
                
                // Try to write encoded tile
                TIFFWriteEncodedTile(tif, tile_num, tile_buffer, write_size);
                break;
            }
                
            case 2: {
                // Test TIFFWriteRawTile with tile index
                uint32_t tile_num = fdp.ConsumeIntegralInRange<uint32_t>(0, num_tiles - 1);
                tmsize_t write_size = fdp.ConsumeIntegralInRange<tmsize_t>(1, tile_size);
                
                // Try to write raw tile data
                TIFFWriteRawTile(tif, tile_num, tile_buffer, write_size);
                break;
            }
        }
        
        _TIFFfree(tile_buffer);
        
        // Occasionally test TIFFSetupStrips again after writing
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 10) {
            TIFFSetupStrips(tif);
        }
    }
    
    // Test tile-related query functions
    if (fdp.remaining_bytes() > 20) {
        // Test TIFFCheckTile with various coordinates
        for (int i = 0; i < 3 && fdp.remaining_bytes() > 16; i++) {
            uint32_t x = fdp.ConsumeIntegral<uint32_t>();
            uint32_t y = fdp.ConsumeIntegral<uint32_t>();
            uint32_t z = 0;
            uint16_t sample = fdp.ConsumeIntegral<uint16_t>() % 4;
            
            TIFFCheckTile(tif, x, y, z, sample);
        }
        
        // Test TIFFComputeTile
        if (fdp.remaining_bytes() > 16) {
            uint32_t x = fdp.ConsumeIntegral<uint32_t>();
            uint32_t y = fdp.ConsumeIntegral<uint32_t>();
            uint32_t z = 0;
            uint16_t sample = fdp.ConsumeIntegral<uint16_t>() % 4;
            
            TIFFComputeTile(tif, x, y, z, sample);
        }
    }
    
    // Test flush operations to ensure data is properly written
    if (fdp.ConsumeBool()) {
        TIFFFlush(tif);
    }
    
    // Clean up
    TIFFClose(tif);
    
    // Remove temporary file if we used regular TIFFOpen
    if (!use_client_open) {
        remove(filename);
    }
    
    return 0;
}

} // namespace harness_014
