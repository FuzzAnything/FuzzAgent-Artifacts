#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <tiffio.h>
#include <tiffio.hxx>

namespace harness_000 {

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

    // Create a memory stream from the input data
    std::istringstream s(std::string(Data, Data + Size));
    TIFF *tif = TIFFStreamOpen("MemTIFF", &s);
    if (!tif) {
        return 0;
    }

    uint32_t w = 0, h = 0;
    uint32_t *raster;

    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &w);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &h);
    
    /* don't continue if file size is ludicrous */
    if (TIFFTileSize64(tif) > MAX_SIZE) {
        TIFFClose(tif);
        return 0;
    }
    
    uint64_t bufsize = TIFFTileSize64(tif);
    /* don't continue if the buffer size greater than the max allowed by the fuzzer */
    if (bufsize > MAX_SIZE || bufsize == 0) {
        TIFFClose(tif);
        return 0;
    }

    if (TIFFIsTiled(tif)) {
        /* another hack to work around an OOM in tif_fax3.c */
        uint32_t tilewidth = 0;
        uint32_t imagewidth = 0;
        TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tilewidth);
        TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &imagewidth);
        tilewidth = __TIFFSafeMultiply(uint32_t, tilewidth, 2);
        imagewidth = __TIFFSafeMultiply(uint32_t, imagewidth, 2);
        if (tilewidth * 2 > MAX_SIZE || imagewidth * 2 > MAX_SIZE ||
            tilewidth == 0 || imagewidth == 0) {
            TIFFClose(tif);
            return 0;
        }
    } else {
        // check the size of the non-tiled image
        uint32_t rowsize = 0;
        TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rowsize);
        uint32_t stripsize = TIFFStripSize(tif);
        rowsize = __TIFFSafeMultiply(uint32_t, rowsize, 2);
        stripsize = __TIFFSafeMultiply(uint32_t, stripsize, 2);
        if (rowsize * 2 > MAX_SIZE || stripsize * 2 > MAX_SIZE ||
            rowsize == 0 || stripsize == 0) {
            TIFFClose(tif);
            return 0;
        }
    }

    uint32_t size = __TIFFSafeMultiply(uint32_t, w, h);

    if (size > MAX_SIZE || size == 0) {
        TIFFClose(tif);
        return 0;
    }

    raster = (uint32_t *)_TIFFmalloc(size * sizeof(uint32_t));
    if (raster != NULL) {
        TIFFReadRGBAImage(tif, w, h, raster, 0);

        if (!TIFFIsTiled(tif)) {
            // Allocate a buffer to hold one scanline
            tsize_t scanlineSize = TIFFScanlineSize(tif);
            void *buffer = _TIFFmalloc(scanlineSize);
            if (buffer) {
                // Read each scanline
                for (uint32_t row = 0; row < h; row++) {
                    if (TIFFReadScanline(tif, buffer, row, 0) < 0) {
                        _TIFFfree(buffer);
                        _TIFFfree(raster);
                        TIFFClose(tif);
                        return 1;
                    }
                }
                _TIFFfree(buffer);
            }
        }
        _TIFFfree(raster);
    }
    
    // Basic TIFFGetField tests
    uint32_t samplesperpixel = 0;
    uint32_t bitspersample = 0;
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samplesperpixel);
    TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bitspersample);
    
    // Try to read directory info
    tdir_t dir_count = TIFFNumberOfDirectories(tif);
    tdir_t current_dir = TIFFCurrentDirectory(tif);
    
    TIFFClose(tif);

    return 0;
}

} // namespace harness_000
