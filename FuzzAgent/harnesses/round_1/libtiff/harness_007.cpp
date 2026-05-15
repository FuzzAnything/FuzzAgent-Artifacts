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

namespace harness_007 {
/*
 * Fuzzing harness for libtiff compression/decompression codec management and raw buffer reading operations
 * Target APIs: TIFFGetConfiguredCODECs, TIFFRegisterCODEC, TIFFUnRegisterCODEC, 
 *              TIFFReadFromUserBuffer, TIFFReadRawStrip, TIFFReadRawTile
 * Required helper APIs: TIFFOpen/TIFFFdOpen, TIFFSetField, TIFFGetField, TIFFClose
 * 
 * This harness focuses on compression codec lifecycle management (discovery, registration, unregistration)
 * and low-level raw buffer reading operations, distinct from previous harnesses.
 * 
 * Semantic differentiation: Targets compression algorithm support testing and direct buffer manipulation
 * rather than high-level image operations covered in other harnesses.
 */



// Dummy TIFFInitMethod for codec registration
static int dummy_codec_init(TIFF* tif, int scheme) {
    // Return success to indicate codec can be initialized
    // This is a minimal implementation for fuzzing purposes
    (void)tif;
    (void)scheme;
    return 1;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for various parameters and buffer data
    if (size < 128) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Create a temporary file for TIFF operations
    char temp_file_template[] = "/tmp/fuzz_tiff_codec_XXXXXX";
    int fd = mkstemp(temp_file_template);
    if (fd < 0) {
        return 0;
    }
    std::string temp_filename = temp_file_template;
    
    // Close the file descriptor since TIFFOpen will reopen it
    close(fd);

    // Consume parameters for codec operations
    uint8_t operation_type = fdp.ConsumeIntegral<uint8_t>() % 3;
    uint16_t codec_scheme = fdp.ConsumeIntegralInRange<uint16_t>(32768, 65535); // Use custom range for test codec
    std::string codec_name = fdp.ConsumeRandomLengthString(32);
    
    // Consume parameters for compression configuration
    uint16_t compression_type = fdp.ConsumeIntegral<uint16_t>();
    uint32_t strip_index = fdp.ConsumeIntegral<uint32_t>();
    uint32_t tile_index = fdp.ConsumeIntegral<uint32_t>();
    
    // Consume buffer sizes for raw reading operations
    size_t in_buffer_size = fdp.ConsumeIntegralInRange<size_t>(1, 4096);
    size_t out_buffer_size = fdp.ConsumeIntegralInRange<size_t>(1, 8192);
    
    // Open TIFF file for writing (to set up test data)
    TIFF* tif = TIFFOpen(temp_filename.c_str(), "w");
    if (!tif) {
        unlink(temp_filename.c_str());
        return 0;
    }

    // Phase 1: Discover available compression codecs
    TIFFCodec* codec_list = TIFFGetConfiguredCODECs();
    if (codec_list) {
        // Iterate through codec list to explore available codecs
        TIFFCodec* current = codec_list;
        while (current->name != NULL) {
            // Check if this codec is configured
            int is_configured = TIFFIsCODECConfigured(current->scheme);
            (void)is_configured;
            
            // Look up the codec by scheme
            const TIFFCodec* found_codec = TIFFFindCODEC(current->scheme);
            (void)found_codec;
            
            current++;
        }
    }

    // Phase 2: Register a test codec (simulated)
    TIFFCodec* registered_codec = NULL;
    if (!codec_name.empty()) {
        registered_codec = TIFFRegisterCODEC(codec_scheme, codec_name.c_str(), dummy_codec_init);
    }

    // Phase 3: Configure TIFF with compression settings
    if (compression_type != 0) {
        TIFFSetField(tif, TIFFTAG_COMPRESSION, compression_type);
    }
    
    // Set basic image parameters for raw reading operations
    uint32_t width = 64;
    uint32_t height = 64;
    uint16_t samples_per_pixel = 3;
    uint16_t bits_per_sample = 8;
    
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, 16);
    
    // Write dummy data to enable reading operations
    std::vector<uint8_t> dummy_scanline(width * samples_per_pixel);
    for (size_t i = 0; i < dummy_scanline.size(); i++) {
        dummy_scanline[i] = fdp.ConsumeIntegral<uint8_t>();
    }
    
    for (uint32_t row = 0; row < height; row++) {
        TIFFWriteScanline(tif, dummy_scanline.data(), row, 0);
    }
    
    // Close and reopen for reading
    TIFFClose(tif);
    tif = TIFFOpen(temp_filename.c_str(), "r");
    if (!tif) {
        if (registered_codec) {
            TIFFUnRegisterCODEC(registered_codec);
        }
        unlink(temp_filename.c_str());
        return 0;
    }

    // Phase 4: Test raw buffer reading operations based on operation type
    switch (operation_type) {
        case 0: {
            // Test TIFFReadFromUserBuffer
            std::vector<uint8_t> in_buffer(in_buffer_size);
            std::vector<uint8_t> out_buffer(out_buffer_size);
            
            // Fill input buffer with fuzzer data
            for (size_t i = 0; i < in_buffer_size && fdp.remaining_bytes() > 0; i++) {
                in_buffer[i] = fdp.ConsumeIntegral<uint8_t>();
            }
            
            // Try to read from user buffer
            if (in_buffer_size > 0 && out_buffer_size > 0) {
                int result = TIFFReadFromUserBuffer(tif, strip_index, 
                                                   in_buffer.data(), in_buffer_size,
                                                   out_buffer.data(), out_buffer_size);
                (void)result;
            }
            break;
        }
        
        case 1: {
            // Test TIFFReadRawStrip
            size_t strip_read_size = fdp.ConsumeIntegralInRange<size_t>(1, 8192);
            std::vector<uint8_t> strip_buffer(strip_read_size);
            
            if (strip_read_size > 0) {
                tmsize_t bytes_read = TIFFReadRawStrip(tif, strip_index, 
                                                       strip_buffer.data(), strip_read_size);
                (void)bytes_read;
            }
            break;
        }
        
        case 2: {
            // Test TIFFReadRawTile
            size_t tile_read_size = fdp.ConsumeIntegralInRange<size_t>(1, 8192);
            std::vector<uint8_t> tile_buffer(tile_read_size);
            
            if (tile_read_size > 0) {
                tmsize_t bytes_read = TIFFReadRawTile(tif, tile_index, 
                                                      tile_buffer.data(), tile_read_size);
                (void)bytes_read;
            }
            break;
        }
    }

    // Phase 5: Query compression settings
    uint16_t current_compression = 0;
    TIFFGetField(tif, TIFFTAG_COMPRESSION, &current_compression);
    (void)current_compression;

    // Phase 6: Unregister test codec if it was registered
    if (registered_codec) {
        TIFFUnRegisterCODEC(registered_codec);
    }

    // Phase 7: Cleanup
    TIFFClose(tif);
    unlink(temp_filename.c_str());

    return 0;
}

} // namespace harness_007
