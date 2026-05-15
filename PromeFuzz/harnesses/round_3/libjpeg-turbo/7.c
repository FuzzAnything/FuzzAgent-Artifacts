// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tjEncodeYUVPlanes at turbojpeg.c:1663:15 in turbojpeg.h
// tjCompressFromYUVPlanes at turbojpeg.c:1394:15 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tjDecompressToYUVPlanes at turbojpeg.c:2288:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "turbojpeg.h"

static void dummy_cleanup(unsigned char** planes, int count) {
    if (planes) {
        for (int i = 0; i < count; i++) {
            free(planes[i]);
        }
        free(planes);
    }
}

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    if (Size < 20) return 0;

    // Extract parameters from fuzzer input
    int componentID = Data[0] % 3;
    int width = (Data[1] << 8) | Data[2];
    int height = (Data[3] << 8) | Data[4];
    int stride = (Data[5] << 8) | Data[6];
    int subsamp = Data[7] % 6; // TJSAMP values: 0-5
    int pitch = (Data[8] << 8) | Data[9];
    int pixelFormat = Data[10] % 12; // TJPF values: 0-11
    int jpegQual = Data[11] % 101; // 0-100
    int flags = Data[12] & 0xFF;
    
    // Test tj3YUVPlaneWidth
    int plane_width = tj3YUVPlaneWidth(componentID, width, subsamp);
    
    // Test tjPlaneSizeYUV
    unsigned long plane_size = tjPlaneSizeYUV(componentID, width, stride, height, subsamp);
    
    // Test tj3YUVPlaneSize
    size_t plane_size2 = tj3YUVPlaneSize(componentID, width, stride, height, subsamp);
    
    // Initialize handles
    tjhandle comp_handle = tjInitCompress();
    tjhandle decomp_handle = tjInitDecompress();
    
    if (comp_handle && decomp_handle) {
        // Prepare dummy JPEG data for decompression
        unsigned char* jpeg_buf = NULL;
        unsigned long jpeg_size = 0;
        unsigned char* src_buf = malloc(width * height * 3);
        
        if (src_buf && Size > 13) {
            size_t copy_size = Size - 13;
            if (copy_size > width * height * 3) copy_size = width * height * 3;
            memcpy(src_buf, Data + 13, copy_size);
            
            // Test tjEncodeYUVPlanes
            unsigned char** dst_planes = calloc(3, sizeof(unsigned char*));
            int strides[3] = {0, 0, 0};
            
            if (dst_planes) {
                for (int i = 0; i < 3; i++) {
                    size_t psize = tj3YUVPlaneSize(i, width, 0, height, subsamp);
                    if (psize > 0) {
                        dst_planes[i] = malloc(psize);
                    }
                }
                
                tjEncodeYUVPlanes(comp_handle, src_buf, width, pitch, height,
                                  pixelFormat, dst_planes, strides, subsamp, flags);
                
                // Test tjCompressFromYUVPlanes
                const unsigned char* src_planes[3] = {dst_planes[0], dst_planes[1], dst_planes[2]};
                tjCompressFromYUVPlanes(comp_handle, src_planes, width, strides,
                                        height, subsamp, &jpeg_buf, &jpeg_size,
                                        jpegQual, flags);
                
                // Test tjDecompressToYUVPlanes
                if (jpeg_buf && jpeg_size > 0) {
                    unsigned char** out_planes = calloc(3, sizeof(unsigned char*));
                    int out_strides[3] = {0, 0, 0};
                    
                    if (out_planes) {
                        for (int i = 0; i < 3; i++) {
                            size_t psize = tj3YUVPlaneSize(i, width, 0, height, subsamp);
                            if (psize > 0) {
                                out_planes[i] = malloc(psize);
                            }
                        }
                        
                        tjDecompressToYUVPlanes(decomp_handle, jpeg_buf, jpeg_size,
                                                out_planes, width, out_strides,
                                                height, flags);
                        
                        dummy_cleanup(out_planes, 3);
                    }
                }
                
                dummy_cleanup(dst_planes, 3);
            }
            
            free(src_buf);
            if (jpeg_buf) tjFree(jpeg_buf);
        }
        
        tjDestroy(comp_handle);
        tjDestroy(decomp_handle);
    }
    
    return 0;
}