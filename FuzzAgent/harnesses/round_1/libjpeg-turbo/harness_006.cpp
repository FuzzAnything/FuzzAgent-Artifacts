#include <fuzzer/FuzzedDataProvider.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <vector>
#include "turbojpeg.h"

namespace harness_006 {
/*
 * libjpeg-turbo fuzzing harness targeting 12-bit and 16-bit precision functions
 * This harness specifically targets high-precision (12-bit and 16-bit) APIs
 * in turbojpeg-mp.c identified as having coverage gaps (500+ undiscovered branches).
 * 
 * Target APIs: tj3Compress12, tj3Compress16, tj3SaveImage12, tj3SaveImage16, 
 *              tj3LoadImage12, tj3LoadImage16 and related high-precision functions.
 * 
 * This harness implements:
 * 1. 12-bit signed short precision image compression/decompression workflow
 * 2. 16-bit unsigned short precision image compression/decompression workflow  
 * 3. Image save/load testing for both precisions with temporary files
 * 4. Comprehensive parameter testing with FuzzedDataProvider
 * 
 * This harness is semantically differentiated from previous harnesses by:
 * 1. Focusing specifically on 12-bit and 16-bit precision APIs (not 8-bit)
 * 2. Testing both signed (12-bit) and unsigned (16-bit) data types
 * 3. Implementing complete save/load workflows with temporary files
 * 4. Targeting turbojpeg-mp.c multi-precision functions for maximum coverage
 */


extern "C" {
}

/* Helper function to create a temporary filename */
static char* create_temp_filename() {
    static char template_str[] = "/tmp/tj_fuzz_XXXXXX";
    int fd = mkstemp(template_str);
    if (fd >= 0) {
        close(fd);
        return strdup(template_str);
    }
    return NULL;
}

/* Helper to generate 12-bit test image data */
static short* generate_12bit_image(int width, int height, int pixel_format, FuzzedDataProvider &fdp) {
    int pixel_size = tjPixelSize[pixel_format];
    size_t buffer_size = (size_t)width * height * pixel_size;
    short* buffer = (short*)tj3Alloc(buffer_size * sizeof(short));
    if (!buffer) return NULL;
    
    /* Fill with fuzzer data - 12-bit values range from -2048 to 2047 */
    for (size_t i = 0; i < buffer_size; i++) {
        buffer[i] = fdp.ConsumeIntegralInRange<short>(-2048, 2047);
    }
    return buffer;
}

/* Helper to generate 16-bit test image data */
static unsigned short* generate_16bit_image(int width, int height, int pixel_format, FuzzedDataProvider &fdp) {
    int pixel_size = tjPixelSize[pixel_format];
    size_t buffer_size = (size_t)width * height * pixel_size;
    unsigned short* buffer = (unsigned short*)tj3Alloc(buffer_size * sizeof(unsigned short));
    if (!buffer) return NULL;
    
    /* Fill with fuzzer data - 16-bit values range from 0 to 65535 */
    for (size_t i = 0; i < buffer_size; i++) {
        buffer[i] = fdp.ConsumeIntegralInRange<unsigned short>(0, 65535);
    }
    return buffer;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    if (size < 128) return 0;  /* Minimum input for meaningful high-precision testing */
    
    FuzzedDataProvider fdp(data, size);
    
    /* Initialize handles */
    tjhandle compress_handle = tj3Init(TJINIT_COMPRESS);
    tjhandle decompress_handle = tj3Init(TJINIT_DECOMPRESS);
    if (!compress_handle || !decompress_handle) {
        if (compress_handle) tj3Destroy(compress_handle);
        if (decompress_handle) tj3Destroy(decompress_handle);
        return 0;
    }
    
    /* Consume fixed-size parameters first */
    int width = fdp.ConsumeIntegralInRange<int>(1, 256);  /* Limit size for performance */
    int height = fdp.ConsumeIntegralInRange<int>(1, 256);
    int pixel_format = fdp.ConsumeIntegralInRange<int>(TJPF_RGB, TJPF_CMYK);
    int subsamp = fdp.ConsumeIntegralInRange<int>(TJSAMP_444, TJSAMP_GRAY);
    int quality = fdp.ConsumeIntegralInRange<int>(1, 100);
    bool lossless = fdp.ConsumeBool();
    bool bottom_up = fdp.ConsumeBool();
    bool fast_dct = fdp.ConsumeBool();
    bool progressive = fdp.ConsumeBool();
    
    /* Set common compression parameters */
    tj3Set(compress_handle, TJPARAM_SUBSAMP, subsamp);
    tj3Set(compress_handle, TJPARAM_QUALITY, quality);
    tj3Set(compress_handle, TJPARAM_LOSSLESS, lossless ? 1 : 0);
    tj3Set(compress_handle, TJPARAM_BOTTOMUP, bottom_up ? 1 : 0);
    tj3Set(compress_handle, TJPARAM_FASTDCT, fast_dct ? 1 : 0);
    tj3Set(compress_handle, TJPARAM_PROGRESSIVE, progressive ? 1 : 0);
    tj3Set(compress_handle, TJPARAM_MAXPIXELS, 65536);  /* 256x256 max */
    
    /* Set decompression parameters to match */
    tj3Set(decompress_handle, TJPARAM_BOTTOMUP, bottom_up ? 1 : 0);
    tj3Set(decompress_handle, TJPARAM_FASTDCT, fast_dct ? 1 : 0);
    
    /* ========== 12-BIT PRECISION TESTING ========== */
    
    /* Generate 12-bit test image */
    short* src_buf_12 = generate_12bit_image(width, height, pixel_format, fdp);
    if (!src_buf_12) {
        tj3Destroy(compress_handle);
        tj3Destroy(decompress_handle);
        return 0;
    }
    
    /* Test 1: tj3Compress12 */
    unsigned char* jpeg_buf_12 = NULL;
    size_t jpeg_size_12 = 0;
    
    /* Set precision for 12-bit compression */
    tj3Set(compress_handle, TJPARAM_PRECISION, 12);
    
    int compress_result = tj3Compress12(compress_handle, src_buf_12, width, 
                                        width * tjPixelSize[pixel_format], height,
                                        pixel_format, &jpeg_buf_12, &jpeg_size_12);
    
    /* Test 2: tj3SaveImage12 - save to temporary file */
    char* temp_filename_12 = create_temp_filename();
    if (temp_filename_12) {
        int save_result = tj3SaveImage12(compress_handle, temp_filename_12, 
                                         src_buf_12, width, 
                                         width * tjPixelSize[pixel_format], 
                                         height, pixel_format);
        /* Test 3: tj3LoadImage12 - load from saved file */
        if (save_result == 0) {
            int loaded_width, loaded_height, loaded_pf;
            loaded_pf = pixel_format;
            short* loaded_buf = tj3LoadImage12(decompress_handle, temp_filename_12,
                                              &loaded_width, 1, &loaded_height, &loaded_pf);
            if (loaded_buf) {
                tj3Free(loaded_buf);
            }
        }
        
        /* Clean up temp file */
        unlink(temp_filename_12);
        free(temp_filename_12);
    }
    
    /* Test 4: Decompress 12-bit JPEG if compression succeeded */
    if (compress_result == 0 && jpeg_buf_12 && jpeg_size_12 > 0) {
        int jpeg_width, jpeg_height, jpeg_precision;
        tj3DecompressHeader(decompress_handle, jpeg_buf_12, jpeg_size_12);
        jpeg_width = tj3Get(decompress_handle, TJPARAM_JPEGWIDTH);
        jpeg_height = tj3Get(decompress_handle, TJPARAM_JPEGHEIGHT);
        jpeg_precision = tj3Get(decompress_handle, TJPARAM_PRECISION);
        
        if (jpeg_width > 0 && jpeg_height > 0 && jpeg_precision == 12) {
            /* Allocate destination buffer for 12-bit decompression */
            size_t dst_size_12 = (size_t)jpeg_width * jpeg_height * 
                                 tjPixelSize[pixel_format] * sizeof(short);
            short* dst_buf_12 = (short*)tj3Alloc(dst_size_12);
            if (dst_buf_12) {
                /* Decompress using appropriate precision API */
                if (lossless) {
                    /* For lossless, we need to use appropriate decompression */
                    tj3Set(decompress_handle, TJPARAM_PRECISION, 12);
                    tj3Decompress12(decompress_handle, jpeg_buf_12, jpeg_size_12,
                                   dst_buf_12, 0, pixel_format);
                } else {
                    /* For lossy, decompress to 8-bit or handle appropriately */
                    /* Note: Lossy 12-bit decompression may need special handling */
                }
                tj3Free(dst_buf_12);
            }
        }
    }
    
    /* Clean up 12-bit resources */
    if (jpeg_buf_12) tj3Free(jpeg_buf_12);
    tj3Free(src_buf_12);
    
    /* ========== 16-BIT PRECISION TESTING ========== */
    
    /* Generate 16-bit test image */
    unsigned short* src_buf_16 = generate_16bit_image(width, height, pixel_format, fdp);
    if (!src_buf_16) {
        tj3Destroy(compress_handle);
        tj3Destroy(decompress_handle);
        return 0;
    }
    
    /* Test 5: tj3Compress16 */
    unsigned char* jpeg_buf_16 = NULL;
    size_t jpeg_size_16 = 0;
    
    /* Set precision for 16-bit compression */
    tj3Set(compress_handle, TJPARAM_PRECISION, 16);
    
    compress_result = tj3Compress16(compress_handle, src_buf_16, width,
                                   width * tjPixelSize[pixel_format], height,
                                   pixel_format, &jpeg_buf_16, &jpeg_size_16);
    
    /* Test 6: tj3SaveImage16 - save to temporary file */
    char* temp_filename_16 = create_temp_filename();
    if (temp_filename_16) {
        int save_result = tj3SaveImage16(compress_handle, temp_filename_16,
                                         src_buf_16, width,
                                         width * tjPixelSize[pixel_format],
                                         height, pixel_format);
        /* Test 7: tj3LoadImage16 - load from saved file */
        if (save_result == 0) {
            int loaded_width, loaded_height, loaded_pf;
            loaded_pf = pixel_format;
            unsigned short* loaded_buf = tj3LoadImage16(decompress_handle, temp_filename_16,
                                                       &loaded_width, 1, &loaded_height, &loaded_pf);
            if (loaded_buf) {
                tj3Free(loaded_buf);
            }
        }
        
        /* Clean up temp file */
        unlink(temp_filename_16);
        free(temp_filename_16);
    }
    
    /* Test 8: Decompress 16-bit JPEG if compression succeeded */
    if (compress_result == 0 && jpeg_buf_16 && jpeg_size_16 > 0) {
        int jpeg_width, jpeg_height, jpeg_precision;
        tj3DecompressHeader(decompress_handle, jpeg_buf_16, jpeg_size_16);
        jpeg_width = tj3Get(decompress_handle, TJPARAM_JPEGWIDTH);
        jpeg_height = tj3Get(decompress_handle, TJPARAM_JPEGHEIGHT);
        jpeg_precision = tj3Get(decompress_handle, TJPARAM_PRECISION);
        
        if (jpeg_width > 0 && jpeg_height > 0 && jpeg_precision == 16) {
            /* Allocate destination buffer for 16-bit decompression */
            size_t dst_size_16 = (size_t)jpeg_width * jpeg_height * 
                                 tjPixelSize[pixel_format] * sizeof(unsigned short);
            unsigned short* dst_buf_16 = (unsigned short*)tj3Alloc(dst_size_16);
            if (dst_buf_16) {
                /* Decompress using appropriate precision API */
                if (lossless) {
                    /* For lossless, we need to use appropriate decompression */
                    tj3Set(decompress_handle, TJPARAM_PRECISION, 16);
                    tj3Decompress16(decompress_handle, jpeg_buf_16, jpeg_size_16,
                                   dst_buf_16, 0, pixel_format);
                } else {
                    /* For lossy, decompress to 8-bit or handle appropriately */
                    /* Note: Lossy 16-bit decompression may need special handling */
                }
                tj3Free(dst_buf_16);
            }
        }
    }
    
    /* Clean up 16-bit resources */
    if (jpeg_buf_16) tj3Free(jpeg_buf_16);
    tj3Free(src_buf_16);
    
    /* ========== CLEANUP ========== */
    
    tj3Destroy(compress_handle);
    tj3Destroy(decompress_handle);
    
    return 0;
}

} // namespace harness_006
