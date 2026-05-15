#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <sstream>
#include <vector>
#include <tiffio.h>
#include <tiffio.hxx>

namespace harness_000 {
/* Fuzzing harness for libtiff targeting basic TIFF file parsing operations */


/* Error handler to suppress libtiff error messages during fuzzing */
extern "C" void handle_error(const char* unused, const char* unused2, va_list unused3) {
    // Suppress error messages during fuzzing
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for basic TIFF header
    if (size < 16) {
        return 0;
    }

    // Set error handlers to suppress libtiff messages
    TIFFSetErrorHandler(handle_error);
    TIFFSetWarningHandler(handle_error);

    FuzzedDataProvider fdp(data, size);

    // Consume operation type to determine which API path to test
    uint8_t operation_type = fdp.ConsumeIntegral<uint8_t>() % 4;

    // Use all remaining data as TIFF input
    std::string tiff_data = fdp.ConsumeRemainingBytesAsString();

    // Create an in-memory TIFF file using stringstream
    std::istringstream s(tiff_data);
    TIFF* tif = TIFFStreamOpen("MemTIFF", &s);
    
    if (!tif) {
        return 0;
    }

    // Test different operations based on operation_type
    switch (operation_type) {
        case 0: {
            // Basic TIFF field reading and scanline reading
            uint32_t width = 0, height = 0;
            uint16_t samples = 0, bits = 0;
            
            TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
            TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
            TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samples);
            TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bits);
            
            // Try to read scanline if image dimensions are available
            if (width > 0 && height > 0 && width < 4096 && height < 4096) {
                tmsize_t scanline_size = TIFFScanlineSize(tif);
                if (scanline_size > 0 && scanline_size < 65536) {
                    uint8_t* scanline = (uint8_t*)_TIFFmalloc(scanline_size);
                    if (scanline) {
                        for (uint32_t row = 0; row < std::min(height, (uint32_t)5); row++) {
                            TIFFReadScanline(tif, scanline, row, 0);
                        }
                        _TIFFfree(scanline);
                    }
                }
            }
            break;
        }
        
        case 1: {
            // Test strip reading
            uint32_t strips_per_image = TIFFNumberOfStrips(tif);
            if (strips_per_image > 0) {
                for (uint32_t strip = 0; strip < std::min(strips_per_image, (uint32_t)3); strip++) {
                    tmsize_t strip_size = TIFFRawStripSize(tif, strip);
                    if (strip_size > 0 && strip_size < 65536) {
                        uint8_t* strip_buffer = (uint8_t*)_TIFFmalloc(strip_size);
                        if (strip_buffer) {
                            TIFFReadRawStrip(tif, strip, strip_buffer, strip_size);
                            _TIFFfree(strip_buffer);
                        }
                    }
                }
            }
            break;
        }
        
        case 2: {
            // Test directory operations
            uint16_t dir_count = 0;
            do {
                dir_count++;
            } while (TIFFReadDirectory(tif));
            
            // Try to go back to first directory if we have multiple
            if (dir_count > 1) {
                TIFFSetDirectory(tif, 0);
            }
            break;
        }
        
        case 3: {
            // Test RGBA image reading for small images
            uint32_t width = 0, height = 0;
            TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
            TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
            
            if (width > 0 && height > 0 && width < 256 && height < 256) {
                uint32_t* raster = (uint32_t*)_TIFFmalloc(width * height * sizeof(uint32_t));
                if (raster) {
                    TIFFReadRGBAImage(tif, width, height, raster, 0);
                    _TIFFfree(raster);
                }
            }
            break;
        }
    }

    // Clean up
    TIFFClose(tif);
    
    return 0;
}

} // namespace harness_000
