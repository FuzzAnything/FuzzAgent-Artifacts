#include <fuzzer/FuzzedDataProvider.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include "turbojpeg.h"

namespace harness_001 {
/*
 * libjpeg-turbo fuzzing harness targeting turbojpeg.h API (tj3* functions)
 * This harness tests both compression and decompression paths using the
 * modern turbojpeg API (tj3Init, tj3Compress8, tj3Decompress8, etc.)
 * which is more stable and tested. Memory management follows patterns
 * from existing fuzz harnesses in $SRC/fuzz/.
 */


extern "C" {
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    if (size < 16) return 0;  // Minimum input size
    
    FuzzedDataProvider fdp(data, size);
    
    /* First, try to decompress the input as a JPEG image */
    tjhandle decompress_handle = NULL;
    tjhandle compress_handle = NULL;
    unsigned char *dst_buf = NULL;
    unsigned char *jpeg_buf = NULL;
    size_t jpeg_size = 0;
    
    /* Try decompression first */
    decompress_handle = tj3Init(TJINIT_DECOMPRESS);
    if (!decompress_handle) return 0;
    
    /* Test decompression with various pixel formats */
    enum TJPF pixel_formats[] = {
        TJPF_RGB, TJPF_BGR, TJPF_RGBX, TJPF_BGRX, TJPF_GRAY
    };
    const int num_formats = sizeof(pixel_formats) / sizeof(pixel_formats[0]);
    
    /* Set decompression parameters based on fuzzer input */
    int width = 0, height = 0, precision = 0;
    
    /* We ignore the return value of tj3DecompressHeader() like the existing
     * fuzz harness does, because malformed JPEG images might have header errors
     * but still expose issues in libjpeg-turbo */
    tj3DecompressHeader(decompress_handle, data, size);
    width = tj3Get(decompress_handle, TJPARAM_JPEGWIDTH);
    height = tj3Get(decompress_handle, TJPARAM_JPEGHEIGHT);
    precision = tj3Get(decompress_handle, TJPARAM_PRECISION);
    
    /* Ignore 0-pixel images and images larger than 1 Megapixel */
    if (width < 1 || height < 1 || (uint64_t)width * height > 1048576) {
        tj3Destroy(decompress_handle);
        decompress_handle = NULL;
        
        /* Fall through to compression testing */
    } else {
        /* Test decompression with different pixel formats */
        for (int pfi = 0; pfi < num_formats; pfi++) {
            int pf = pixel_formats[pfi];
            int w = width, h = height;
            
            /* Test non-default decompression options on some iterations */
            tj3Set(decompress_handle, TJPARAM_BOTTOMUP, pfi == 0);
            tj3Set(decompress_handle, TJPARAM_FASTUPSAMPLE, pfi == 0);
            
            if (!tj3Get(decompress_handle, TJPARAM_LOSSLESS)) {
                tj3Set(decompress_handle, TJPARAM_FASTDCT, pfi == 0);
                
                /* Test IDCT scaling on some iterations */
                if (pfi == 1 || pfi == 2) {
                    tjscalingfactor sf = { 1, pfi == 1 ? 2 : 8 };
                    tj3SetScalingFactor(decompress_handle, sf);
                    w = TJSCALED(width, sf);
                    h = TJSCALED(height, sf);
                } else {
                    tj3SetScalingFactor(decompress_handle, TJUNSCALED);
                }
                
                /* Test partial image decompression on some iterations */
                if ((pfi == 1 || pfi == 3) && w >= 97 && h >= 75) {
                    tjregion cr = { 32, 16, 65, 59 };
                    tj3SetCroppingRegion(decompress_handle, cr);
                } else {
                    tj3SetCroppingRegion(decompress_handle, TJUNCROPPED);
                }
                tj3Set(decompress_handle, TJPARAM_SCANLIMIT, 100);
            } else {
                tj3Set(decompress_handle, TJPARAM_SCANLIMIT, 50);
            }
            
            /* Allocate destination buffer */
            int sample_size = (precision > 8) ? 2 : 1;
            size_t dst_buf_size = (size_t)w * h * tjPixelSize[pf] * sample_size;
            dst_buf = (unsigned char *)tj3Alloc(dst_buf_size);
            if (!dst_buf) continue;
            
            /* Perform decompression based on precision */
            if (precision == 8) {
                tj3Decompress8(decompress_handle, data, size, dst_buf, 0, pf);
            } else if (precision == 12) {
                tj3Decompress12(decompress_handle, data, size, (short *)dst_buf, 0, pf);
            } else {
                tj3Decompress16(decompress_handle, data, size, (unsigned short *)dst_buf, 0, pf);
            }
            
            /* Touch output data to catch uninitialized reads with MemorySanitizer */
            volatile int sum = 0;
            for (size_t i = 0; i < dst_buf_size && i < 1024; i++) {
                sum += dst_buf[i];
            }
            
            tj3Free(dst_buf);
            dst_buf = NULL;
        }
        
        tj3Destroy(decompress_handle);
        decompress_handle = NULL;
        
        /* If decompression succeeded, we can exit early */
        return 0;
    }
    
    /* If we reach here, either decompression failed or the input wasn't JPEG.
     * Try compression instead. */
    
    /* Initialize compression handle */
    compress_handle = tj3Init(TJINIT_COMPRESS);
    if (!compress_handle) return 0;
    
    /* Define test configurations similar to compress.cc */
    struct test_config {
        int bottom_up;
        enum TJPF pixel_format;
        int colorspace;
        enum TJSAMP subsampling;
        int fast_dct;
        int quality;
        int optimize;
        int progressive;
        int arithmetic;
        int no_realloc;
        int restart_rows;
    };
    
    struct test_config tests[] = {
        /* BU Pixel      JPEG          Subsampling  Fst Qual Opt Prg Ari No    Rst
             Format     Colorspace    Level        DCT                  Realc Rows */
        { 1, TJPF_RGB,  TJCS_RGB,     TJSAMP_444,  0,  100, 0,  0,  0,  0,    2    },
        { 0, TJPF_BGR,  TJCS_YCbCr,   TJSAMP_422,  0,  90,  0,  1,  0,  0,    0    },
        { 0, TJPF_RGBX, TJCS_DEFAULT, TJSAMP_420,  1,  75,  0,  0,  1,  1,    0    },
        { 0, TJPF_BGRA, TJCS_DEFAULT, TJSAMP_411,  0,  50,  0,  1,  1,  0,    0    },
        { 0, TJPF_GRAY, TJCS_GRAY,    TJSAMP_GRAY, 0,  25,  0,  0,  0,  0,    0    }
    };
    
    const int num_tests = sizeof(tests) / sizeof(tests[0]);
    
    for (int ti = 0; ti < num_tests; ti++) {
        struct test_config *cfg = &tests[ti];
        int width, height;
        unsigned char *src_buf = NULL;
        
        /* Derive image dimensions from input data */
        if (fdp.remaining_bytes() < 2) break;
        
        width = (fdp.ConsumeIntegral<uint8_t>() % 64) + 8;   /* 8-71 */
        height = (fdp.ConsumeIntegral<uint8_t>() % 64) + 8;  /* 8-71 */
        
        /* Check if we have enough data for this image */
        size_t pixel_size = tjPixelSize[cfg->pixel_format];
        size_t required_size = (size_t)width * height * pixel_size;
        
        if (fdp.remaining_bytes() < required_size) {
            /* Not enough data, try smaller dimensions */
            width = 8;
            height = 8;
            required_size = (size_t)width * height * pixel_size;
            if (fdp.remaining_bytes() < required_size) {
                continue;
            }
        }
        
        /* Consume image data */
        std::vector<uint8_t> image_data = fdp.ConsumeBytes<uint8_t>(required_size);
        src_buf = image_data.data();
        
        /* Set compression parameters */
        tj3Set(compress_handle, TJPARAM_BOTTOMUP, cfg->bottom_up);
        tj3Set(compress_handle, TJPARAM_COLORSPACE, cfg->colorspace);
        tj3Set(compress_handle, TJPARAM_SUBSAMP, cfg->subsampling);
        tj3Set(compress_handle, TJPARAM_FASTDCT, cfg->fast_dct);
        tj3Set(compress_handle, TJPARAM_QUALITY, cfg->quality);
        tj3Set(compress_handle, TJPARAM_OPTIMIZE, cfg->optimize);
        tj3Set(compress_handle, TJPARAM_PROGRESSIVE, cfg->progressive);
        tj3Set(compress_handle, TJPARAM_ARITHMETIC, cfg->arithmetic);
        tj3Set(compress_handle, TJPARAM_NOREALLOC, cfg->no_realloc);
        tj3Set(compress_handle, TJPARAM_RESTARTROWS, cfg->restart_rows);
        
        tj3Set(compress_handle, TJPARAM_MAXPIXELS, 1048576);
        
        /* Set ICC profile if we have enough data */
        if (fdp.remaining_bytes() >= 32) {
            std::vector<uint8_t> icc_data = fdp.ConsumeBytes<uint8_t>(32);
            tj3SetICCProfile(compress_handle, icc_data.data(), 32);
        }
        
        /* Calculate JPEG buffer size */
        size_t max_buf_size = tj3JPEGBufSize(width, height, cfg->subsampling);
        
        /* Allocate JPEG buffer */
        if (cfg->no_realloc) {
            jpeg_buf = (unsigned char *)tj3Alloc(max_buf_size);
            jpeg_size = max_buf_size;
        } else {
            jpeg_buf = NULL;
            jpeg_size = 0;
        }
        
        if (!jpeg_buf && cfg->no_realloc) {
            continue;
        }
        
        /* Perform compression */
        if (tj3Compress8(compress_handle, src_buf, width, 0, height, 
                         cfg->pixel_format, &jpeg_buf, &jpeg_size) == 0) {
            /* Touch output data to catch uninitialized reads with MemorySanitizer */
            volatile int sum = 0;
            for (size_t i = 0; i < jpeg_size && i < 1024; i++) {
                sum += jpeg_buf[i];
            }
        }
        
        /* Free JPEG buffer if we allocated it */
        if (jpeg_buf) {
            tj3Free(jpeg_buf);
            jpeg_buf = NULL;
        }
        
        /* Check if we have enough input left for another test */
        if (fdp.remaining_bytes() < 2) break;
    }
    
    /* Cleanup */
    if (compress_handle) {
        tj3Destroy(compress_handle);
    }
    if (decompress_handle) {
        tj3Destroy(decompress_handle);
    }
    if (dst_buf) {
        tj3Free(dst_buf);
    }
    if (jpeg_buf) {
        tj3Free(jpeg_buf);
    }
    
    return 0;
}

} // namespace harness_001
