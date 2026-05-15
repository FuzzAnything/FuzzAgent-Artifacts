// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3LoadImage16 at turbojpeg-mp.c:293:21 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Compress16 at turbojpeg-mp.c:71:15 in turbojpeg.h
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
// tjLoadImage at turbojpeg.c:3104:26 in turbojpeg.h
// tjSaveImage at turbojpeg.c:3125:15 in turbojpeg.h
// tjSaveImage at turbojpeg.c:3125:15 in turbojpeg.h
// tjLoadImage at turbojpeg.c:3104:26 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Compress16 at turbojpeg-mp.c:71:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Compress16 at turbojpeg-mp.c:71:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static int get_pixel_format(uint8_t val) {
    const int formats[] = {
        TJPF_RGB, TJPF_BGR, TJPF_RGBX, TJPF_BGRX,
        TJPF_XBGR, TJPF_XRGB, TJPF_GRAY, TJPF_CMYK,
        TJPF_UNKNOWN
    };
    return formats[val % (sizeof(formats)/sizeof(formats[0]))];
}

static int get_subsamp(uint8_t val) {
    const int subsamps[] = {
        TJSAMP_444, TJSAMP_422, TJSAMP_420,
        TJSAMP_GRAY, TJSAMP_440, TJSAMP_411
    };
    return subsamps[val % (sizeof(subsamps)/sizeof(subsamps[0]))];
}

static int get_align(uint8_t val) {
    const int aligns[] = {1, 2, 4, 8, 16, 32, 64, 128};
    return aligns[val % (sizeof(aligns)/sizeof(aligns[0]))];
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;
    
    tjhandle handle = NULL;
    unsigned char *jpegBuf = NULL;
    size_t jpegSize = 0;
    int width = 0, height = 0, pixelFormat = 0;
    unsigned short *srcBuf16 = NULL;
    unsigned char *srcBuf8 = NULL;
    int ret = 0;
    
    // Create dummy file for file-based functions
    write_dummy_file(Data, Size);
    
    // Initialize handle for functions that need it
    handle = tj3Init(TJINIT_COMPRESS);
    if (!handle) goto cleanup;
    
    // Use first byte to determine which function to test
    uint8_t selector = Data[0];
    const uint8_t *func_data = Data + 1;
    size_t func_size = Size - 1;
    
    switch (selector % 6) {
        case 0: { // tj3Compress16
            if (func_size < 1024) break;
            
            width = 16 + (func_data[0] % 64);
            height = 16 + (func_data[1] % 64);
            pixelFormat = get_pixel_format(func_data[2]);
            int pitch = width * tjPixelSize[pixelFormat];
            int align = get_align(func_data[3]);
            if (align > 1) pitch = ((pitch + align - 1) / align) * align;
            
            size_t src_size = pitch * height * sizeof(unsigned short);
            srcBuf16 = (unsigned short*)malloc(src_size);
            if (!srcBuf16) break;
            
            // Fill source buffer with data
            size_t copy_size = func_size - 4;
            if (copy_size > src_size) copy_size = src_size;
            memcpy(srcBuf16, func_data + 4, copy_size);
            
            // Set parameters
            tj3Set(handle, TJPARAM_PRECISION, 13 + (func_data[4] % 4));
            tj3Set(handle, TJPARAM_NOREALLOC, func_data[5] & 1);
            
            // Allocate JPEG buffer
            jpegSize = tj3JPEGBufSize(width, height, TJSAMP_444);
            jpegBuf = tj3Alloc(jpegSize);
            
            ret = tj3Compress16(handle, srcBuf16, width, pitch, height,
                               pixelFormat, &jpegBuf, &jpegSize);
            break;
        }
        
        case 1: { // tj3LoadImage16
            int align = get_align(func_data[0]);
            pixelFormat = get_pixel_format(func_data[1]);
            
            srcBuf16 = tj3LoadImage16(handle, "./dummy_file", 
                                      &width, align, &height, &pixelFormat);
            if (srcBuf16) {
                // Optionally compress the loaded image
                if (width > 0 && height > 0) {
                    jpegSize = tj3JPEGBufSize(width, height, TJSAMP_444);
                    jpegBuf = tj3Alloc(jpegSize);
                    int pitch = width * tjPixelSize[pixelFormat];
                    tj3Compress16(handle, srcBuf16, width, pitch, height,
                                 pixelFormat, &jpegBuf, &jpegSize);
                }
            }
            break;
        }
        
        case 2: { // tjDecodeYUVPlanes
            if (func_size < 128) break;
            
            width = 16 + (func_data[0] % 64);
            height = 16 + (func_data[1] % 64);
            pixelFormat = get_pixel_format(func_data[2]);
            int subsamp = get_subsamp(func_data[3]);
            int flags = func_data[4] & (TJFLAG_BOTTOMUP | TJFLAG_FASTUPSAMPLE);
            int pitch = width * tjPixelSize[pixelFormat];
            
            // Allocate destination buffer
            size_t dst_size = pitch * height;
            unsigned char *dstBuf = (unsigned char*)malloc(dst_size);
            if (!dstBuf) break;
            
            // Set up YUV planes (simplified - real YUV data would be more complex)
            const unsigned char *srcPlanes[3];
            int strides[3];
            
            // Use some data for fake YUV planes
            srcPlanes[0] = func_data + 5;
            strides[0] = width;
            
            if (subsamp != TJSAMP_GRAY) {
                srcPlanes[1] = func_data + 5 + width * height;
                srcPlanes[2] = func_data + 5 + width * height * 2;
                strides[1] = strides[2] = width / 2;
            }
            
            ret = tjDecodeYUVPlanes(handle, srcPlanes, strides, subsamp,
                                   dstBuf, width, pitch, height, 
                                   pixelFormat, flags);
            
            free(dstBuf);
            break;
        }
        
        case 3: { // tjLoadImage
            int align = get_align(func_data[0]);
            pixelFormat = get_pixel_format(func_data[1]);
            int flags = func_data[2] & (TJFLAG_BOTTOMUP | TJFLAG_FASTUPSAMPLE);
            
            srcBuf8 = tjLoadImage("./dummy_file", &width, align, 
                                 &height, &pixelFormat, flags);
            if (srcBuf8) {
                // Optionally save the loaded image
                if (width > 0 && height > 0) {
                    int pitch = width * tjPixelSize[pixelFormat];
                    tjSaveImage("./dummy_file", srcBuf8, width, pitch,
                               height, pixelFormat, flags);
                }
            }
            break;
        }
        
        case 4: { // tjSaveImage
            if (func_size < 256) break;
            
            width = 16 + (func_data[0] % 32);
            height = 16 + (func_data[1] % 32);
            pixelFormat = get_pixel_format(func_data[2]);
            int flags = func_data[3] & (TJFLAG_BOTTOMUP | TJFLAG_FASTUPSAMPLE);
            int pitch = width * tjPixelSize[pixelFormat];
            
            // Allocate and fill source buffer
            size_t src_size = pitch * height;
            srcBuf8 = (unsigned char*)malloc(src_size);
            if (!srcBuf8) break;
            
            size_t copy_size = func_size - 4;
            if (copy_size > src_size) copy_size = src_size;
            memcpy(srcBuf8, func_data + 4, copy_size);
            
            ret = tjSaveImage("./dummy_file", srcBuf8, width, pitch,
                            height, pixelFormat, flags);
            break;
        }
        
        case 5: { // Test combination: load, compress, and free
            // Try to load an image
            pixelFormat = TJPF_UNKNOWN;
            srcBuf8 = tjLoadImage("./dummy_file", &width, 1, 
                                 &height, &pixelFormat, 0);
            if (srcBuf8 && width > 0 && height > 0) {
                // Compress it
                jpegSize = tj3JPEGBufSize(width, height, TJSAMP_444);
                jpegBuf = tj3Alloc(jpegSize);
                int pitch = width * tjPixelSize[pixelFormat];
                
                ret = tj3Compress16(handle, (const unsigned short*)srcBuf8, 
                                   width, pitch, height, pixelFormat,
                                   &jpegBuf, &jpegSize);
                
                // Free the loaded buffer
                tjFree(srcBuf8);
                srcBuf8 = NULL;
            }
            break;
        }
    }

cleanup:
    // Clean up allocated resources
    if (srcBuf16) tj3Free(srcBuf16);
    if (srcBuf8) tjFree(srcBuf8);
    if (jpegBuf) tjFree(jpegBuf);
    if (handle) tj3Destroy(handle);
    
    // Remove dummy file
    remove("./dummy_file");
    
    return 0;
}