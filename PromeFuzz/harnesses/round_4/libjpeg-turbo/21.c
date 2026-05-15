// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjCompressFromYUVPlanes at turbojpeg.c:1394:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tj3CompressFromYUVPlanes8 at turbojpeg.c:1259:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjCompressFromYUV at turbojpeg.c:1476:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "src/turbojpeg.h"

static int get_valid_subsamp(int subsamp) {
    if (subsamp < 0 || subsamp > 5) {
        return 0;
    }
    return subsamp;
}

static int get_valid_quality(int qual) {
    if (qual < 1) return 1;
    if (qual > 100) return 100;
    return qual;
}

static int get_valid_dimension(int dim) {
    if (dim <= 0) return 1;
    if (dim > 4096) return 4096;
    return dim;
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size < 16) {
        return 0;
    }

    // Parse some parameters from the input data
    int width = get_valid_dimension(*(const int*)(Data));
    int height = get_valid_dimension(*(const int*)(Data + 4));
    int subsamp = get_valid_subsamp(*(const int*)(Data + 8) % 6);
    int jpegQual = get_valid_quality(*(const int*)(Data + 12) % 101);
    
    size_t offset = 16;
    
    // Test tjAlloc and tjFree
    int alloc_size = (Size - offset) % 65536;
    if (alloc_size < 0) alloc_size = 1024;
    unsigned char *alloc_buf = tjAlloc(alloc_size);
    if (alloc_buf) {
        tjFree(alloc_buf);
    }
    
    // Test tjInitCompress and tjCompressFromYUV
    tjhandle handle = tjInitCompress();
    if (handle) {
        unsigned char *jpegBuf = NULL;
        unsigned long jpegSize = 0;
        
        // Calculate YUV buffer size needed
        int yuv_width = width;
        int yuv_height = height;
        int align = 1;
        
        // Use remaining data as YUV source
        size_t yuv_needed = yuv_width * yuv_height;
        if (subsamp != TJSAMP_GRAY) {
            yuv_needed += 2 * ((yuv_width + 1) / 2) * ((yuv_height + 1) / 2);
        }
        
        if (offset + yuv_needed <= Size && yuv_needed > 0) {
            const unsigned char *srcBuf = Data + offset;
            
            // Try compression with various flags
            int flags[] = {0, TJFLAG_FASTDCT, TJFLAG_ACCURATEDCT, TJFLAG_PROGRESSIVE};
            for (int i = 0; i < 4; i++) {
                unsigned char *localJpegBuf = NULL;
                unsigned long localJpegSize = 0;
                
                int result = tjCompressFromYUV(
                    handle, srcBuf, 
                    yuv_width, align, yuv_height, subsamp,
                    &localJpegBuf, &localJpegSize, jpegQual, flags[i]
                );
                
                if (localJpegBuf) {
                    tjFree(localJpegBuf);
                }
            }
        }
        
        // Test tjCompressFromYUVPlanes
        if (subsamp != TJSAMP_GRAY && offset + yuv_needed * 3 / 2 <= Size && yuv_needed > 0) {
            const unsigned char *planes[3];
            int strides[3] = {0, 0, 0};
            
            // Split data into planes
            size_t plane_size = yuv_width * yuv_height;
            size_t chroma_size = ((yuv_width + 1) / 2) * ((yuv_height + 1) / 2);
            
            if (offset + plane_size + 2 * chroma_size <= Size) {
                planes[0] = Data + offset;
                planes[1] = Data + offset + plane_size;
                planes[2] = Data + offset + plane_size + chroma_size;
                
                unsigned char *planesJpegBuf = NULL;
                unsigned long planesJpegSize = 0;
                
                int result = tjCompressFromYUVPlanes(
                    handle, planes,
                    yuv_width, strides, yuv_height, subsamp,
                    &planesJpegBuf, &planesJpegSize, jpegQual, 0
                );
                
                if (planesJpegBuf) {
                    tjFree(planesJpegBuf);
                }
            }
        }
        
        // Test tj3CompressFromYUVPlanes8 (TurboJPEG 3.x API)
        // We'll use the same handle but note that this might not work correctly
        // with TurboJPEG 3.x API, but we'll try anyway for fuzzing
        if (offset + yuv_needed <= Size && yuv_needed > 0) {
            const unsigned char *srcPlanes[3];
            int strides[3] = {0, 0, 0};
            
            // For simplicity, use same data for all planes
            srcPlanes[0] = Data + offset;
            srcPlanes[1] = Data + offset;
            srcPlanes[2] = Data + offset;
            
            unsigned char *jpegBuf3 = NULL;
            size_t jpegSize3 = 0;
            
            // Note: This function expects a TurboJPEG 3.x handle, but we're using
            // a handle from tjInitCompress. This may fail, but that's OK for fuzzing.
            int result = tj3CompressFromYUVPlanes8(
                handle, srcPlanes,
                yuv_width, strides, yuv_height,
                &jpegBuf3, &jpegSize3
            );
            
            if (jpegBuf3) {
                tjFree(jpegBuf3);
            }
        }
        
        tjDestroy(handle);
    }
    
    return 0;
}