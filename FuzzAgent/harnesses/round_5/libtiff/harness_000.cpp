#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <fuzzer/FuzzedDataProvider.h>
#include <tiffio.h>
#include <tiffio.hxx>

namespace harness_000 {
/*
 * libtiff fuzzing harness - based on existing tiff_read_rgba_fuzzer.cc
 * but enhanced with FuzzedDataProvider for parameter variation
 * Targets key TIFF APIs: TIFFOpen (via TIFFStreamOpen), TIFFReadDirectory,
 * TIFFReadScanline, TIFFGetField
 */


/* stolen from tiffiop.h, which is a private header so we can't just include it */
/* safe multiply returns either the multiplied value or 0 if it overflowed */
#define __TIFFSafeMultiply(t, v, m)                                            \
    ((((t)(m) != (t)0) && (((t)(((v) * (m)) / (m))) == (t)(v)))                \
         ? (t)((v) * (m))                                                      \
         : (t)0)

const uint64_t MAX_SIZE = 500000000;

extern "C" void handle_error(const char *unused, const char *unused2,
                             va_list unused3)
{
    return;
}

int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size)
{
#ifndef STANDALONE
    TIFFSetErrorHandler(handle_error);
    TIFFSetWarningHandler(handle_error);
#endif
#if defined(__has_feature)
#if __has_feature(memory_sanitizer)
    // libjpeg-turbo has issues with MSAN and SIMD code
    // See https://bugs.chromium.org/p/oss-fuzz/issues/detail?id=7547
    // and https://github.com/libjpeg-turbo/libjpeg-turbo/pull/365
    setenv("JSIMD_FORCENONE", "1", 1);
#endif
#endif

    // Use FuzzedDataProvider to consume parameters for testing variations
    FuzzedDataProvider fdp(Data, Size);
    
    // Consume a small portion for control parameters
    uint8_t test_variation = fdp.ConsumeIntegral<uint8_t>() % 4;
    size_t remaining_size = fdp.remaining_bytes();
    
    // Use the remaining bytes as TIFF data
    std::vector<uint8_t> tiff_data = fdp.ConsumeRemainingBytes<uint8_t>();
    
    std::istringstream s(std::string(tiff_data.begin(), tiff_data.end()));
    TIFF *tif = TIFFStreamOpen("MemTIFF", &s);
    if (!tif)
    {
        return 0;
    }
    
    uint32_t w = 0, h = 0;
    uint32_t *raster = nullptr;

    // Test TIFFGetField with various tags
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &w);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &h);
    
    // Try to get additional fields based on test variation
    uint16_t samples_per_pixel = 0, bits_per_sample = 0;
    uint32_t rows_per_strip = 0;
    uint16_t planar_config = 0, photometric = 0;
    uint16_t compression = 0;
    
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samples_per_pixel);
    TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bits_per_sample);
    TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rows_per_strip);
    TIFFGetField(tif, TIFFTAG_PLANARCONFIG, &planar_config);
    TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &photometric);
    TIFFGetField(tif, TIFFTAG_COMPRESSION, &compression);

    /* don't continue if file size is ludicrous */
    if (TIFFTileSize64(tif) > MAX_SIZE)
    {
        TIFFClose(tif);
        return 0;
    }
    
    uint64_t bufsize = TIFFTileSize64(tif);
    /* don't continue if the buffer size greater than the max allowed by the
     * fuzzer */
    if (bufsize > MAX_SIZE || bufsize == 0)
    {
        TIFFClose(tif);
        return 0;
    }

    if (TIFFIsTiled(tif))
    {
        /* another hack to work around an OOM in tif_fax3.c */
        uint32_t tilewidth = 0;
        uint32_t imagewidth = 0;
        TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tilewidth);
        TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &imagewidth);
        tilewidth = __TIFFSafeMultiply(uint32_t, tilewidth, 2);
        imagewidth = __TIFFSafeMultiply(uint32_t, imagewidth, 2);
        if (tilewidth * 2 > MAX_SIZE || imagewidth * 2 > MAX_SIZE ||
            tilewidth == 0 || imagewidth == 0)
        {
            TIFFClose(tif);
            return 0;
        }
    }
    else
    {
        // check the size of the non-tiled image
        uint32_t rowsize = 0;
        TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rowsize);
        uint32_t stripsize = TIFFStripSize(tif);
        rowsize = __TIFFSafeMultiply(uint32_t, rowsize, 2);
        stripsize = __TIFFSafeMultiply(uint32_t, stripsize, 2);
        if (rowsize * 2 > MAX_SIZE || stripsize * 2 > MAX_SIZE ||
            rowsize == 0 || stripsize == 0)
        {
            TIFFClose(tif);
            return 0;
        }
    }

    uint32_t size = __TIFFSafeMultiply(uint32_t, w, h);

    if (size > MAX_SIZE || size == 0)
    {
        TIFFClose(tif);
        return 0;
    }

    // Test TIFFReadDirectory - try to read through directories
    int dir_count = 0;
    do {
        dir_count++;
        // Limit to avoid infinite loops with malformed files
        if (dir_count > 100) {
            break;
        }
    } while (TIFFReadDirectory(tif));

    // Go back to first directory
    TIFFSetDirectory(tif, 0);

    raster = (uint32_t *)_TIFFmalloc(size * sizeof(uint32_t));
    int ret = 0;
    if (raster != NULL)
    {
        // Try to read RGBA image
        TIFFReadRGBAImage(tif, w, h, raster, 0);

        if (!TIFFIsTiled(tif))
        {
            // Allocate a buffer to hold one scanline
            tsize_t scanlineSize = TIFFScanlineSize(tif);
            void *buffer = _TIFFmalloc(scanlineSize);
            if (buffer)
            {
                // Read some scanlines (limited number for performance)
                uint32_t rows_to_read = h;
                if (rows_to_read > 100) rows_to_read = 100; // Limit
                
                for (uint32_t row = 0; row < rows_to_read; row++)
                {
                    // Test different sample planes based on test variation
                    uint16_t sample = 0;
                    if (samples_per_pixel > 1 && test_variation % 2 == 0) {
                        sample = (test_variation % samples_per_pixel);
                    }
                    
                    if (TIFFReadScanline(tif, buffer, row, sample) < 0)
                    {
                        // Try with sample 0 if specific sample failed
                        if (sample != 0) {
                            TIFFReadScanline(tif, buffer, row, 0);
                        }
                    }
                }
                _TIFFfree(buffer);
            }
        }
        
        _TIFFfree(raster);
    }
    
    TIFFClose(tif);

    return ret;
}

} // namespace harness_000
