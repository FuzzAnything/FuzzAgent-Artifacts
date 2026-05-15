// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3DecodeYUV8 at turbojpeg.c:2675:15 in turbojpeg.h
// tj3DecodeYUV8 at turbojpeg.c:2675:15 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3DecompressToYUV8 at turbojpeg.c:2338:15 in turbojpeg.h
// tj3DecodeYUV8 at turbojpeg.c:2675:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3SetScalingFactor at turbojpeg.c:1981:15 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3DecompressToYUV8 at turbojpeg.c:2338:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    // Create a dummy file for JPEG data if needed
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Reopen file to read JPEG data
    fp = fopen(filename, "rb");
    if (!fp) return 0;
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    if (file_size <= 0) {
        fclose(fp);
        return 0;
    }
    
    unsigned char *jpegBuf = (unsigned char *)malloc(file_size);
    if (!jpegBuf) {
        fclose(fp);
        return 0;
    }
    
    size_t bytes_read = fread(jpegBuf, 1, file_size, fp);
    fclose(fp);
    
    if (bytes_read != (size_t)file_size) {
        free(jpegBuf);
        return 0;
    }
    
    // Initialize TurboJPEG decompression instance
    tjhandle handle = tj3Init(TJINIT_DECOMPRESS);
    if (!handle) {
        free(jpegBuf);
        return 0;
    }
    
    // Set scaling factor (using 1:1 scaling)
    tjscalingfactor scalingFactor;
    scalingFactor.num = 1;
    scalingFactor.denom = 1;
    tj3SetScalingFactor(handle, scalingFactor);
    
    // First allocation - for YUV buffer (we'll determine size later)
    size_t yuv_buf_size_guess = Size * 2; // Reasonable guess
    if (yuv_buf_size_guess < 1024) yuv_buf_size_guess = 1024;
    unsigned char *yuvBuf = (unsigned char *)tj3Alloc(yuv_buf_size_guess);
    
    // Second allocation - for RGB buffer
    size_t rgb_buf_size_guess = Size * 3; // Reasonable guess for RGB
    if (rgb_buf_size_guess < 1024) rgb_buf_size_guess = 1024;
    unsigned char *rgbBuf = (unsigned char *)tj3Alloc(rgb_buf_size_guess);
    
    if (!yuvBuf || !rgbBuf) {
        if (yuvBuf) tj3Free(yuvBuf);
        if (rgbBuf) tj3Free(rgbBuf);
        tj3Destroy(handle);
        free(jpegBuf);
        return 0;
    }
    
    // Try to get JPEG dimensions first to calculate proper buffer sizes
    int width = 0, height = 0, subsamp = 0;
    if (tj3DecompressHeader(handle, jpegBuf, Size) == 0) {
        width = tj3Get(handle, TJPARAM_JPEGWIDTH);
        height = tj3Get(handle, TJPARAM_JPEGHEIGHT);
        subsamp = tj3Get(handle, TJPARAM_SUBSAMP);
        
        // Calculate actual YUV buffer size needed
        size_t actual_yuv_size = tj3YUVBufSize(width, 1, height, subsamp);
        
        // If our guess was too small, reallocate
        if (actual_yuv_size > 0 && actual_yuv_size > yuv_buf_size_guess) {
            tj3Free(yuvBuf);
            yuvBuf = (unsigned char *)tj3Alloc(actual_yuv_size);
            if (!yuvBuf) {
                tj3Free(rgbBuf);
                tj3Destroy(handle);
                free(jpegBuf);
                return 0;
            }
        }
        
        // Try to decompress to YUV
        if (actual_yuv_size > 0) {
            tj3DecompressToYUV8(handle, jpegBuf, Size, yuvBuf, 1);
            
            // Try to decode YUV to RGB
            int pixelFormat = TJPF_RGB;
            int pitch = width * tjPixelSize[pixelFormat];
            if (pitch == 0) pitch = width * 3; // Fallback for RGB
            
            // Make sure RGB buffer is large enough
            size_t needed_rgb_size = pitch * height;
            if (needed_rgb_size > 0 && needed_rgb_size > rgb_buf_size_guess) {
                tj3Free(rgbBuf);
                rgbBuf = (unsigned char *)tj3Alloc(needed_rgb_size);
                if (rgbBuf) {
                    tj3DecodeYUV8(handle, yuvBuf, 1, rgbBuf, width, pitch, height, pixelFormat);
                }
            } else if (rgbBuf) {
                tj3DecodeYUV8(handle, yuvBuf, 1, rgbBuf, width, pitch, height, pixelFormat);
            }
        }
    } else {
        // If header decompression fails, try with default parameters as fallback
        // Use some safe defaults
        width = 64;
        height = 64;
        subsamp = TJSAMP_420;
        
        size_t fallback_yuv_size = tj3YUVBufSize(width, 1, height, subsamp);
        if (fallback_yuv_size > 0 && fallback_yuv_size <= yuv_buf_size_guess) {
            tj3DecompressToYUV8(handle, jpegBuf, Size, yuvBuf, 1);
            
            int pixelFormat = TJPF_RGB;
            int pitch = width * 3;
            if (pitch * height <= rgb_buf_size_guess) {
                tj3DecodeYUV8(handle, yuvBuf, 1, rgbBuf, width, pitch, height, pixelFormat);
            }
        }
    }
    
    // Cleanup
    if (yuvBuf) tj3Free(yuvBuf);
    if (rgbBuf) tj3Free(rgbBuf);
    tj3Destroy(handle);
    free(jpegBuf);
    
    return 0;
}