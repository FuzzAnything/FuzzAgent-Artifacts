#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fuzzer/FuzzedDataProvider.h>
#include <tiffio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <vector>
#include <string>

namespace harness_009 {
/*
 * Fuzzing harness for libtiff library focusing on File Opening and Raw Data Reading APIs
 * Targets low-level file opening and raw data reading APIs: TIFFFdOpen, TIFFClientOpen, 
 * TIFFReadRawTile, TIFFReadRawStrip, TIFFReadFromUserBuffer, TIFFVTileSize
 * Tests file descriptor-based opening, custom client I/O, and raw data reading operations
 * Uses FuzzedDataProvider to process input dynamically
 * Semantically differentiated from:
 * - harness_000 (basic reading with TIFFOpen)
 * - harness_001 (writing operations)
 * - harness_002 (RGBA reading)
 * - harness_003 (directory manipulation)
 * - harness_004 (advanced directory management)
 * - harness_005 (LogLuv compression)
 * - harness_006 (JPEG/compression)
 * - harness_007 (FAX compression and color conversion)
 * - harness_008 (custom directory operations)
 * This harness specifically focuses on low-level file descriptor and custom I/O operations
 * that are not covered by any existing harnesses.
 */


// Error handler to suppress library error messages during fuzzing
extern "C" void handle_error(const char *module, const char *fmt, va_list ap) {
    (void)module;
    (void)fmt;
    (void)ap;
    return;
}

// Custom I/O handlers for TIFFClientOpen
extern "C" tmsize_t custom_read_proc(thandle_t handle, void* buf, tmsize_t size) {
    FILE* file = (FILE*)handle;
    return fread(buf, 1, size, file);
}

extern "C" tmsize_t custom_write_proc(thandle_t handle, void* buf, tmsize_t size) {
    FILE* file = (FILE*)handle;
    return fwrite(buf, 1, size, file);
}

extern "C" toff_t custom_seek_proc(thandle_t handle, toff_t offset, int whence) {
    FILE* file = (FILE*)handle;
    if (fseek(file, offset, whence) == 0) {
        return ftell(file);
    }
    return (toff_t)-1;
}

extern "C" int custom_close_proc(thandle_t handle) {
    FILE* file = (FILE*)handle;
    return fclose(file);
}

extern "C" toff_t custom_size_proc(thandle_t handle) {
    FILE* file = (FILE*)handle;
    long current = ftell(file);
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, current, SEEK_SET);
    return size;
}

extern "C" int custom_map_proc(thandle_t handle, void** base, toff_t* size) {
    (void)handle;
    (void)base;
    (void)size;
    return 0; // Not implemented
}

extern "C" void custom_unmap_proc(thandle_t handle, void* base, toff_t size) {
    (void)handle;
    (void)base;
    (void)size;
    // Not implemented
}

// Helper function to create a temporary TIFF file with specific content
static std::string create_temp_tiff_file(FuzzedDataProvider& fdp) {
    const char* temp_filename = "/tmp/fuzzed_raw_tiff.tif";
    
    // Open TIFF file for writing
    TIFF* tif = TIFFOpen(temp_filename, "w");
    if (!tif) {
        return "";
    }
    
    // Set error handlers
    TIFFSetErrorHandler(handle_error);
    TIFFSetWarningHandler(handle_error);
    
    // Consume image parameters from fuzzed input
    uint32_t width = fdp.ConsumeIntegralInRange<uint32_t>(1, 512);
    uint32_t height = fdp.ConsumeIntegralInRange<uint32_t>(1, 512);
    uint16_t samples_per_pixel = fdp.ConsumeIntegralInRange<uint16_t>(1, 4);
    uint16_t bits_per_sample = fdp.ConsumeIntegralInRange<uint16_t>(1, 32);
    
    // Set basic TIFF directory fields
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    
    // Decide whether to use tiles or strips
    bool use_tiles = fdp.ConsumeBool();
    if (use_tiles) {
        uint32_t tile_width = fdp.ConsumeIntegralInRange<uint32_t>(16, 256);
        uint32_t tile_length = fdp.ConsumeIntegralInRange<uint32_t>(16, 256);
        tile_width = (tile_width > width) ? width : tile_width;
        tile_length = (tile_length > height) ? height : tile_length;
        
        TIFFSetField(tif, TIFFTAG_TILEWIDTH, tile_width);
        TIFFSetField(tif, TIFFTAG_TILELENGTH, tile_length);
        
        // Write tile data if we have enough fuzzed input
        if (fdp.remaining_bytes() > 100) {
            tmsize_t tile_size = TIFFTileSize(tif);
            if (tile_size > 0 && tile_size <= 65536 && fdp.remaining_bytes() >= tile_size) {
                std::vector<uint8_t> tile_data = fdp.ConsumeBytes<uint8_t>(tile_size);
                uint32_t num_tiles_width = (width + tile_width - 1) / tile_width;
                uint32_t num_tiles_height = (height + tile_length - 1) / tile_length;
                
                // Write a few tiles
                uint32_t tiles_to_write = fdp.ConsumeIntegralInRange<uint32_t>(0, 
                    std::min(num_tiles_width * num_tiles_height, 4U));
                for (uint32_t i = 0; i < tiles_to_write; i++) {
                    uint32_t tile_x = fdp.ConsumeIntegralInRange<uint32_t>(0, num_tiles_width - 1);
                    uint32_t tile_y = fdp.ConsumeIntegralInRange<uint32_t>(0, num_tiles_height - 1);
                    TIFFWriteTile(tif, tile_data.data(), tile_x * tile_width, tile_y * tile_length, 0, 0);
                }
            }
        }
    } else {
        // Use strips
        TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, TIFFDefaultStripSize(tif, (uint32_t)-1));
        
        // Write strip data if we have enough fuzzed input
        if (fdp.remaining_bytes() > 100) {
            tmsize_t strip_size = TIFFStripSize(tif);
            if (strip_size > 0 && strip_size <= 65536 && fdp.remaining_bytes() >= strip_size) {
                uint32_t num_strips = TIFFNumberOfStrips(tif);
                std::vector<uint8_t> strip_data = fdp.ConsumeBytes<uint8_t>(strip_size);
                
                // Write a few strips
                uint32_t strips_to_write = fdp.ConsumeIntegralInRange<uint32_t>(0, std::min(num_strips, 4U));
                for (uint32_t i = 0; i < strips_to_write; i++) {
                    uint32_t strip = fdp.ConsumeIntegralInRange<uint32_t>(0, num_strips - 1);
                    TIFFWriteEncodedStrip(tif, strip, strip_data.data(), strip_size);
                }
            }
        }
    }
    
    // Write the directory
    TIFFWriteDirectory(tif);
    TIFFClose(tif);
    
    return temp_filename;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size check - need at least some data for meaningful testing
    if (size < 200) {
        return 0;
    }
    
    // Initialize FuzzedDataProvider for structured input processing
    FuzzedDataProvider fdp(data, size);
    
    // Set error handlers to suppress library messages
    TIFFSetErrorHandler(handle_error);
    TIFFSetWarningHandler(handle_error);
    
    // Create a temporary TIFF file for testing
    std::string filename = create_temp_tiff_file(fdp);
    if (filename.empty()) {
        return 0;
    }
    
    // Determine which opening method to test
    uint8_t opening_method = fdp.ConsumeIntegral<uint8_t>() % 3;
    TIFF* tif = nullptr;
    
    switch (opening_method) {
        case 0: {
            // Test TIFFFdOpen - file descriptor based opening
            int fd = open(filename.c_str(), O_RDONLY);
            if (fd >= 0) {
                const char* mode = fdp.ConsumeBool() ? "r" : "r+";
                tif = TIFFFdOpen(fd, filename.c_str(), mode);
                if (!tif) {
                    close(fd);
                }
            }
            break;
        }
        
        case 1: {
            // Test TIFFClientOpen - custom I/O handlers
            FILE* file = fopen(filename.c_str(), "rb");
            if (file) {
                const char* mode = "r";
                tif = TIFFClientOpen(filename.c_str(), mode,
                                    (thandle_t)file,
                                    custom_read_proc,
                                    custom_write_proc,
                                    custom_seek_proc,
                                    custom_close_proc,
                                    custom_size_proc,
                                    custom_map_proc,
                                    custom_unmap_proc);
                if (!tif) {
                    fclose(file);
                }
            }
            break;
        }
        
        case 2: {
            // Regular TIFFOpen for comparison
            const char* mode = fdp.ConsumeBool() ? "r" : "r+";
            tif = TIFFOpen(filename.c_str(), mode);
            break;
        }
    }
    
    if (!tif) {
        remove(filename.c_str());
        return 0;
    }
    
    // Test raw data reading operations
    uint8_t raw_operation = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    switch (raw_operation) {
        case 0: {
            // Test TIFFReadRawTile
            if (TIFFIsTiled(tif)) {
                uint32_t tile_width = 0, tile_length = 0;
                TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tile_width);
                TIFFGetField(tif, TIFFTAG_TILELENGTH, &tile_length);
                
                if (tile_width > 0 && tile_length > 0) {
                    uint32_t image_width = 0, image_height = 0;
                    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &image_width);
                    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &image_height);
                    
                    uint32_t tiles_across = (image_width + tile_width - 1) / tile_width;
                    uint32_t tiles_down = (image_height + tile_length - 1) / tile_length;
                    uint32_t total_tiles = tiles_across * tiles_down;
                    
                    if (total_tiles > 0) {
                        uint32_t tile_to_read = fdp.ConsumeIntegralInRange<uint32_t>(0, total_tiles - 1);
                        tmsize_t tile_size = TIFFTileSize(tif);
                        
                        if (tile_size > 0 && tile_size <= 65536) {
                            std::vector<uint8_t> buffer(tile_size);
                            TIFFReadRawTile(tif, tile_to_read, buffer.data(), tile_size);
                        }
                    }
                }
            }
            break;
        }
        
        case 1: {
            // Test TIFFReadRawStrip
            if (!TIFFIsTiled(tif)) {
                uint32_t num_strips = TIFFNumberOfStrips(tif);
                if (num_strips > 0) {
                    uint32_t strip_to_read = fdp.ConsumeIntegralInRange<uint32_t>(0, num_strips - 1);
                    tmsize_t strip_size = TIFFRawStripSize(tif, strip_to_read);
                    
                    if (strip_size > 0 && strip_size <= 65536) {
                        std::vector<uint8_t> buffer(strip_size);
                        TIFFReadRawStrip(tif, strip_to_read, buffer.data(), strip_size);
                    }
                }
            }
            break;
        }
        
        case 2: {
            // Test TIFFReadFromUserBuffer
            if (TIFFIsTiled(tif)) {
                uint32_t tile_width = 0, tile_length = 0;
                TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tile_width);
                TIFFGetField(tif, TIFFTAG_TILELENGTH, &tile_length);
                
                if (tile_width > 0 && tile_length > 0) {
                    uint32_t image_width = 0, image_height = 0;
                    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &image_width);
                    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &image_height);
                    
                    uint32_t tiles_across = (image_width + tile_width - 1) / tile_width;
                    uint32_t tiles_down = (image_height + tile_length - 1) / tile_length;
                    uint32_t total_tiles = tiles_across * tiles_down;
                    
                    if (total_tiles > 0) {
                        uint32_t tile_to_read = fdp.ConsumeIntegralInRange<uint32_t>(0, total_tiles - 1);
                        tmsize_t tile_size = TIFFTileSize(tif);
                        
                        if (tile_size > 0 && tile_size <= 65536) {
                            std::vector<uint8_t> in_buffer(tile_size);
                            std::vector<uint8_t> out_buffer(tile_size);
                            
                            // Fill input buffer with fuzzed data if available
                            if (fdp.remaining_bytes() >= tile_size) {
                                auto fuzzed_data = fdp.ConsumeBytes<uint8_t>(tile_size);
                                std::copy(fuzzed_data.begin(), fuzzed_data.end(), in_buffer.begin());
                            }
                            
                            TIFFReadFromUserBuffer(tif, tile_to_read, 
                                                  in_buffer.data(), tile_size,
                                                  out_buffer.data(), tile_size);
                        }
                    }
                }
            } else {
                // Test with strips
                uint32_t num_strips = TIFFNumberOfStrips(tif);
                if (num_strips > 0) {
                    uint32_t strip_to_read = fdp.ConsumeIntegralInRange<uint32_t>(0, num_strips - 1);
                    tmsize_t strip_size = TIFFRawStripSize(tif, strip_to_read);
                    
                    if (strip_size > 0 && strip_size <= 65536) {
                        std::vector<uint8_t> in_buffer(strip_size);
                        std::vector<uint8_t> out_buffer(strip_size);
                        
                        // Fill input buffer with fuzzed data if available
                        if (fdp.remaining_bytes() >= strip_size) {
                            auto fuzzed_data = fdp.ConsumeBytes<uint8_t>(strip_size);
                            std::copy(fuzzed_data.begin(), fuzzed_data.end(), in_buffer.begin());
                        }
                        
                        TIFFReadFromUserBuffer(tif, strip_to_read,
                                              in_buffer.data(), strip_size,
                                              out_buffer.data(), strip_size);
                    }
                }
            }
            break;
        }
        
        case 3: {
            // Test TIFFVTileSize
            if (TIFFIsTiled(tif)) {
                uint32_t tile_length = 0;
                TIFFGetField(tif, TIFFTAG_TILELENGTH, &tile_length);
                
                if (tile_length > 0) {
                    // Test with different numbers of rows
                    uint32_t test_rows = fdp.ConsumeIntegralInRange<uint32_t>(1, tile_length * 2);
                    tmsize_t v_tile_size = TIFFVTileSize(tif, test_rows);
                    uint64_t v_tile_size64 = TIFFVTileSize64(tif, test_rows);
                    
                    // Also test regular tile size for comparison
                    tmsize_t tile_size = TIFFTileSize(tif);
                    uint64_t tile_size64 = TIFFTileSize64(tif);
                }
            }
            break;
        }
    }
    
    // Clean up
    TIFFClose(tif);
    remove(filename.c_str());
    
    return 0;
}

} // namespace harness_009
