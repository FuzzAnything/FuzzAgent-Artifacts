// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tjLoadImage at turbojpeg.c:3104:26 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tjSaveImage at turbojpeg.c:3125:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Compress16 at turbojpeg-mp.c:71:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3LoadImage16 at turbojpeg-mp.c:293:21 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    /* Initialize handles */
    tjhandle comp_handle = tj3Init(TJINIT_COMPRESS);
    tjhandle decomp_handle = tj3Init(TJINIT_DECOMPRESS);
    if (!comp_handle || !decomp_handle) {
        if (comp_handle) tj3Destroy(comp_handle);
        if (decomp_handle) tj3Destroy(decomp_handle);
        return 0;
    }
    
    /* Use first byte to select function to test */
    uint8_t selector = Data[0] % 6;
    const uint8_t *fuzz_data = Data + 1;
    size_t fuzz_size = Size - 1;
    
    switch (selector) {
        case 0: { /* tj3Compress16 */
            if (fuzz_size < 16) break;
            
            /* Parse parameters from fuzz data */
            int width = *(int*)fuzz_data % 1024 + 1;
            int pitch = *(int*)(fuzz_data + 4) % 2048;
            int height = *(int*)(fuzz_data + 8) % 1024 + 1;
            int pixelFormat = *(int*)(fuzz_data + 12) % 10; /* TJPF values 0-9 */
            
            /* Calculate source buffer size */
            int pixelSize = tjPixelSize[pixelFormat];
            if (pixelSize <= 0) pixelSize = 1;
            size_t src_samples = (pitch ? pitch : width * pixelSize) * height;
            
            /* Allocate and fill source buffer */
            unsigned short *srcBuf = NULL;
            if (src_samples > 0 && src_samples <= 1024 * 1024 * 4) {
                srcBuf = (unsigned short*)malloc(src_samples * sizeof(unsigned short));
                if (srcBuf) {
                    size_t copy_size = fuzz_size - 16;
                    if (copy_size > src_samples * sizeof(unsigned short))
                        copy_size = src_samples * sizeof(unsigned short);
                    memcpy(srcBuf, fuzz_data + 16, copy_size);
                }
            }
            
            if (srcBuf) {
                unsigned char *jpegBuf = NULL;
                size_t jpegSize = 0;
                
                /* Set precision to 16-bit */
                tj3Set(comp_handle, TJPARAM_PRECISION, 16);
                
                /* Call target function */
                tj3Compress16(comp_handle, srcBuf, width, pitch, height,
                             pixelFormat, &jpegBuf, &jpegSize);
                
                /* Cleanup */
                if (jpegBuf) tj3Free(jpegBuf);
                free(srcBuf);
            }
            break;
        }
        
        case 1: { /* tj3LoadImage16 */
            /* Write fuzz data to file */
            FILE *fp = fopen("./dummy_file", "wb");
            if (fp) {
                fwrite(fuzz_data, 1, fuzz_size, fp);
                fclose(fp);
                
                int width, height, pixelFormat = TJPF_UNKNOWN;
                unsigned short *img = tj3LoadImage16(decomp_handle, "./dummy_file",
                                                    &width, 1, &height, &pixelFormat);
                if (img) tj3Free(img);
                
                remove("./dummy_file");
            }
            break;
        }
        
        case 2: { /* tjFree */
            /* Allocate a small buffer and free it */
            if (fuzz_size > 0) {
                size_t alloc_size = fuzz_data[0] % 1024 + 1;
                unsigned char *buf = (unsigned char*)tjAlloc(alloc_size);
                if (buf) tjFree(buf);
            }
            break;
        }
        
        case 3: { /* tjDecodeYUVPlanes */
            if (fuzz_size < 32) break;
            
            int width = *(int*)fuzz_data % 1024 + 1;
            int pitch = *(int*)(fuzz_data + 4) % 2048;
            int height = *(int*)(fuzz_data + 8) % 1024 + 1;
            int pixelFormat = *(int*)(fuzz_data + 12) % 10;
            int subsamp = *(int*)(fuzz_data + 16) % 4; /* TJ_* subsampling values */
            int flags = *(int*)(fuzz_data + 20);
            
            /* Allocate destination buffer */
            int dst_pitch = pitch ? pitch : width * tjPixelSize[pixelFormat];
            unsigned char *dstBuf = NULL;
            if (dst_pitch > 0 && height > 0 && dst_pitch <= 4096 && height <= 4096) {
                dstBuf = (unsigned char*)malloc(dst_pitch * height);
            }
            
            if (dstBuf) {
                /* Setup YUV planes */
                const unsigned char *srcPlanes[3] = {0};
                int strides[3] = {0};
                
                size_t offset = 24;
                for (int i = 0; i < 3 && offset < fuzz_size; i++) {
                    strides[i] = width; /* Simplified stride */
                    srcPlanes[i] = fuzz_data + offset;
                    offset += width * height / (i == 0 ? 1 : 4); /* Approximate plane sizes */
                }
                
                /* Call target function */
                tjDecodeYUVPlanes(decomp_handle, srcPlanes, strides, subsamp,
                                 dstBuf, width, pitch, height, pixelFormat, flags);
                
                free(dstBuf);
            }
            break;
        }
        
        case 4: { /* tjLoadImage */
            /* Write fuzz data to file */
            FILE *fp = fopen("./dummy_file", "wb");
            if (fp) {
                fwrite(fuzz_data, 1, fuzz_size, fp);
                fclose(fp);
                
                int width, height, pixelFormat = TJPF_UNKNOWN;
                unsigned char *img = tjLoadImage("./dummy_file", &width, 1,
                                                 &height, &pixelFormat, 0);
                if (img) free(img);
                
                remove("./dummy_file");
            }
            break;
        }
        
        case 5: { /* tjSaveImage */
            if (fuzz_size < 20) break;
            
            int width = *(int*)fuzz_data % 1024 + 1;
            int pitch = *(int*)(fuzz_data + 4) % 2048;
            int height = *(int*)(fuzz_data + 8) % 1024 + 1;
            int pixelFormat = *(int*)(fuzz_data + 12) % 10;
            int flags = *(int*)(fuzz_data + 16);
            
            /* Allocate and fill buffer */
            int buf_pitch = pitch ? pitch : width * tjPixelSize[pixelFormat];
            unsigned char *buffer = NULL;
            if (buf_pitch > 0 && height > 0 && buf_pitch <= 4096 && height <= 4096) {
                buffer = (unsigned char*)malloc(buf_pitch * height);
                if (buffer && fuzz_size > 20) {
                    size_t copy_size = fuzz_size - 20;
                    if (copy_size > (size_t)(buf_pitch * height))
                        copy_size = buf_pitch * height;
                    memcpy(buffer, fuzz_data + 20, copy_size);
                }
            }
            
            if (buffer) {
                tjSaveImage("./dummy_file", buffer, width, pitch, height,
                           pixelFormat, flags);
                free(buffer);
                remove("./dummy_file");
            }
            break;
        }
    }
    
    /* Cleanup */
    tj3Destroy(comp_handle);
    tj3Destroy(decomp_handle);
    
    return 0;
}