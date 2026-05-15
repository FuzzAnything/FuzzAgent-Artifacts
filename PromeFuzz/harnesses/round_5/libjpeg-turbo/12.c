// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDecompressHeader2 at turbojpeg.c:1903:15 in turbojpeg.h
// tjDecompressHeader3 at turbojpeg.c:1874:15 in turbojpeg.h
// tjDecompress2 at turbojpeg.c:2059:15 in turbojpeg.h
// tjDecompressToYUV2 at turbojpeg.c:2401:15 in turbojpeg.h
// tjTransform at turbojpeg.c:3041:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int dummy_custom_filter(short *coeffs, tjregion arrayRegion,
                               tjregion planeRegion, int componentID,
                               int transformID, struct tjtransform *transform) {
    return 0;
}

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    tjhandle handle = NULL;
    int width = 0, height = 0, jpegSubsamp = 0, jpegColorspace = 0;
    unsigned char *jpegBuf = NULL;
    unsigned long jpegSize = 0;
    int ret = 0;
    
    if (Size < 1) return 0;
    
    handle = tjInitDecompress();
    if (!handle) return 0;
    
    jpegBuf = (unsigned char *)Data;
    jpegSize = Size;
    
    ret = tjDecompressHeader2(handle, jpegBuf, jpegSize, &width, &height, &jpegSubsamp);
    
    ret = tjDecompressHeader3(handle, jpegBuf, jpegSize, &width, &height, &jpegSubsamp, &jpegColorspace);
    
    if (width > 0 && height > 0 && width <= 4096 && height <= 4096) {
        int pixelFormat = TJPF_RGB;
        int pitch = width * tjPixelSize[pixelFormat];
        unsigned char *dstBuf = NULL;
        
        dstBuf = (unsigned char *)malloc(width * height * tjPixelSize[pixelFormat]);
        if (dstBuf) {
            ret = tjDecompress2(handle, jpegBuf, jpegSize, dstBuf, width, pitch, height, pixelFormat, 0);
            free(dstBuf);
        }
        
        int align = 1;
        unsigned char *yuvBuf = NULL;
        yuvBuf = (unsigned char *)malloc(width * height * 3 / 2);
        if (yuvBuf) {
            ret = tjDecompressToYUV2(handle, jpegBuf, jpegSize, yuvBuf, width, align, height, 0);
            free(yuvBuf);
        }
    }
    
    int n = 1;
    unsigned char **dstBufs = NULL;
    unsigned long *dstSizes = NULL;
    tjtransform *transforms = NULL;
    
    transforms = (tjtransform *)malloc(sizeof(tjtransform) * n);
    if (transforms) {
        dstBufs = (unsigned char **)malloc(sizeof(unsigned char *) * n);
        dstSizes = (unsigned long *)malloc(sizeof(unsigned long) * n);
        
        if (dstBufs && dstSizes) {
            for (int i = 0; i < n; i++) {
                dstBufs[i] = NULL;
                dstSizes[i] = 0;
                
                transforms[i].r.x = 0;
                transforms[i].r.y = 0;
                transforms[i].r.w = width > 0 ? width / 2 : 100;
                transforms[i].r.h = height > 0 ? height / 2 : 100;
                transforms[i].op = TJXOP_NONE;
                transforms[i].options = TJXOPT_PERFECT;
                transforms[i].customFilter = dummy_custom_filter;
            }
            
            ret = tjTransform(handle, jpegBuf, jpegSize, n, dstBufs, dstSizes, transforms, TJXOPT_PERFECT);
            
            for (int i = 0; i < n; i++) {
                if (dstBufs[i]) {
                    free(dstBufs[i]);
                }
            }
        }
        
        free(transforms);
        free(dstBufs);
        free(dstSizes);
    }
    
    ret = tjDestroy(handle);
    
    return 0;
}