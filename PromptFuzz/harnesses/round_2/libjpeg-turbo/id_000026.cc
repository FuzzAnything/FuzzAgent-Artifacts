#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
extern "C" {
#include <turbojpeg.h>
}
//<ID> 377
//<Prompt> []
/*<Combination>: [
*/
//<score> 66.666664, nr_unique_branch: 68
//<Quality> {"density":25,"unique_branches":{"save_marker":[[775,11,775,45,0,0,4,0],[838,3,838,14,0,0,4,0],[841,3,841,15,0,0,4,0],[844,3,844,10,0,0,4,1]],"tjTransform":[[3102,7,3102,37,0,0,4,1],[3105,7,3105,12,0,0,4,1],[3105,16,3105,32,0,0,4,1],[3112,7,3112,22,0,0,4,1],[3118,7,3118,61,0,0,4,1],[3120,15,3120,20,0,0,4,0],[3120,15,3120,20,0,0,4,1],[3122,9,3122,24,0,0,4,1],[3136,15,3136,20,0,0,4,0],[3136,15,3136,20,0,0,4,1],[3140,7,3140,41,0,0,4,1],[3141,7,3141,25,0,0,4,0],[3141,7,3141,25,0,0,4,1],[329,7,329,12,1,0,4,1]],"jpeg_save_markers":[[1345,9,1345,35,0,0,4,0],[1345,39,1345,67,0,0,4,1],[1347,14,1347,41,0,0,4,0],[1347,45,1347,74,0,0,4,1],[1356,7,1356,32,0,0,4,0]],"tjSaveImage":[[3180,7,3180,52,0,0,4,1]],"jcopy_markers_setup":[[2460,7,2460,30,0,0,4,0],[2460,34,2460,56,0,0,4,0],[2464,7,2464,29,0,0,4,0],[2465,17,2465,23,0,0,4,0],[2465,17,2465,23,0,0,4,1],[2467,11,2467,44,0,0,4,1],[2473,7,2473,29,0,0,4,1]],"tj3SetICCProfile":[[1230,7,1230,35,0,0,4,1],[1233,7,1233,29,0,0,4,1],[1239,7,1239,13,0,0,4,0],[1239,17,1239,24,0,0,4,0],[1240,9,1240,66,0,0,4,1],[340,7,340,12,1,0,4,1]],"tj3Transform":[[2921,7,2921,35,0,0,4,1],[2921,39,2921,69,0,0,4,1],[2924,7,2924,22,0,0,4,1],[2924,26,2924,39,0,0,4,1],[2924,43,2924,48,0,0,4,1],[2924,52,2924,67,0,0,4,1],[2925,7,2925,23,0,0,4,1],[2925,27,2925,36,0,0,4,1],[2928,7,2928,22,0,0,4,1],[2938,7,2939,79,0,0,4,1],[2945,7,2945,45,0,0,4,0],[2945,7,2945,45,0,0,4,1],[2948,15,2948,20,0,0,4,0],[2949,9,2949,20,0,0,4,1],[2949,24,2949,44,0,0,4,1],[2952,24,2952,55,0,0,4,1],[2953,21,2953,49,0,0,4,1],[2954,32,2954,60,0,0,4,1],[2955,21,2955,49,0,0,4,1],[2956,9,2956,15,0,0,4,1],[2959,9,2959,22,0,0,4,1],[2973,9,2973,42,0,0,4,0],[2976,30,2976,41,0,0,4,0],[2976,30,2976,41,0,0,4,1],[2978,7,2978,45,0,0,4,0],[2978,7,2978,45,0,0,4,1],[3072,7,3072,41,0,0,4,1],[3076,7,3076,41,0,0,4,0],[3078,7,3078,25,0,0,4,0],[3078,7,3078,25,0,0,4,1],[305,7,305,12,2,0,4,1]]},"library_calls":["tj3InitVersion","tjInitTransform","tj3Destroy","tjDestroy","tj3Destroy","tjDestroy","tj3Destroy","tjDestroy","tj3Destroy","tjDestroy","tj3SetICCProfile","tjDecompressHeader3","tjBufSizeYUV2","tjEncodeYUV3","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tj3Alloc","tj3EncodeYUVPlanes8","tj3Free","tj3TransformBufSize","tj3Alloc","tj3Transform","tj3Free","TJBUFSIZE","tjAlloc","tjTransform","tjFree","tjSaveImage","tj3Destroy","tjDestroy"],"critical_calls":["tj3InitVersion","tjInitTransform","tj3SetICCProfile","tjDecompressHeader3","tjBufSizeYUV2","tjEncodeYUV3","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tj3Alloc","tj3EncodeYUVPlanes8","tj3Free","tj3TransformBufSize","tj3Alloc","tj3Transform","tj3Free","TJBUFSIZE","tjAlloc","tjTransform","tjFree","tjSaveImage","tj3Destroy","tjDestroy"],"visited":2}
/**/



extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
        return 0;  // Too small for meaningful JPEG data
    }

    // Initialize libjpeg-turbo handles
    tjhandle handle3 = tj3Init(TJINIT_TRANSFORM);
    tjhandle handle_old = tjInitTransform();
    
    if (!handle3 || !handle_old) {
        if (handle3) tj3Destroy(handle3);
        if (handle_old) tjDestroy(handle_old);
        return 0;
    }

    // Create a dummy ICC profile
    unsigned char dummy_icc[] = {
        0x00, 0x00, 0x02, 0x30, 0x6C, 0x63, 0x6D, 0x73,
        0x02, 0x10, 0x00, 0x00, 0x6D, 0x6E, 0x74, 0x72,
        0x52, 0x47, 0x42, 0x20, 0x58, 0x59, 0x5A, 0x20,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    size_t icc_size = sizeof(dummy_icc);

    // Create input file for reading
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        tj3Destroy(handle3);
        tjDestroy(handle_old);
        assert_file_closed(&in_file);
	return 0;
    }

    // Read JPEG data from file
    unsigned char *jpegBuf = (unsigned char *)malloc(size);
    if (!jpegBuf) {
        assert_file_closed(&in_file);;
        tj3Destroy(handle3);
        tjDestroy(handle_old);
        assert_file_closed(&in_file);
	return 0;
    }
    
    size_t bytes_read = fread(jpegBuf, 1, size, in_file);
    assert_file_closed(&in_file);;
    
    if (bytes_read != size) {
        free(jpegBuf);
        tj3Destroy(handle3);
        tjDestroy(handle_old);
        assert_file_closed(&in_file);
	return 0;
    }

    // Set ICC profile using new API
    tj3SetICCProfile(handle3, dummy_icc, icc_size);

    // Try to decompress header to get image dimensions (optional, for reference)
    int width, height, jpegSubsamp, jpegColorspace;
    if (tjDecompressHeader3(handle_old, jpegBuf, (unsigned long)size, 
                          &width, &height, &jpegSubsamp, &jpegColorspace) == 0) {
        
        // Allocate source buffer for encoding (dummy data)
        int pixelFormat = TJPF_RGB;
        int src_pitch = width * tjPixelSize[pixelFormat];
        unsigned char *srcBuf = (unsigned char *)malloc(src_pitch * height);
        
        if (srcBuf) {
            // Fill with dummy pattern
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    int idx = (y * src_pitch) + (x * 3);
                    srcBuf[idx] = (x * 255) / width;      // R
                    srcBuf[idx + 1] = (y * 255) / height; // G
                    srcBuf[idx + 2] = 128;                // B
                }
            }

            // Encode to YUV using old API
            int align = 4;
            int subsamp = TJSAMP_420;
            size_t yuv_size = tjBufSizeYUV2(width, align, height, subsamp);
            unsigned char *yuvBuf = (unsigned char *)malloc(yuv_size);
            
            if (yuvBuf) {
                tjEncodeYUV3(handle_old, srcBuf, width, src_pitch, height, 
                           pixelFormat, yuvBuf, align, subsamp, 0);
                
                // Encode to YUV planes using new API
                int strides[3];
                unsigned char *planes[3];
                for (int i = 0; i < 3; i++) {
                    strides[i] = tj3YUVPlaneWidth(i, width, subsamp);
                    int planeHeight = tj3YUVPlaneHeight(i, height, subsamp);
                    planes[i] = (unsigned char *)tj3Alloc(
                        strides[i] * planeHeight);
                }
                
                if (planes[0] && planes[1] && planes[2]) {
                    tj3EncodeYUVPlanes8(handle3, srcBuf, width, src_pitch, 
                                      height, pixelFormat, planes, strides);
                    
                    // Clean up planes
                    for (int i = 0; i < 3; i++) {
                        if (planes[i]) tj3Free(planes[i]);
                    }
                }
                
                free(yuvBuf);
            }
            
            free(srcBuf);
        }
    }

    // Setup transformation
    tjtransform transform;
    transform.r.x = 0;
    transform.r.y = 0;
    transform.r.w = 100;
    transform.r.h = 100;
    transform.op = TJXOP_NONE;
    transform.options = 0;
    transform.data = NULL;
    transform.customFilter = NULL;

    // Transform using new API
    size_t dstSize3 = tj3TransformBufSize(handle3, &transform);
    unsigned char *dstBuf3 = (unsigned char *)tj3Alloc(dstSize3);
    size_t actualDstSize3 = dstSize3;
    
    if (dstBuf3) {
        unsigned char *dstBufs3[1] = {dstBuf3};
        size_t dstSizes3[1] = {actualDstSize3};
        
        tj3Transform(handle3, jpegBuf, size, 1, dstBufs3, dstSizes3, &transform);
        
        tj3Free(dstBuf3);
    }

    // Transform using old API
    unsigned long dstSize_old = TJBUFSIZE(100, 100);
    unsigned char *dstBuf_old = tjAlloc(dstSize_old);
    
    if (dstBuf_old) {
        unsigned char *dstBufs_old[1] = {dstBuf_old};
        unsigned long dstSizes_old[1] = {dstSize_old};
        
        tjTransform(handle_old, jpegBuf, (unsigned long)size, 1, 
                   dstBufs_old, dstSizes_old, &transform, 0);
        
        tjFree(dstBuf_old);
    }

    // Create dummy image for saving
    int save_width = 64;
    int save_height = 64;
    int save_pitch = save_width * 3;
    unsigned char *save_buffer = (unsigned char *)malloc(save_pitch * save_height);
    
    if (save_buffer) {
        // Fill with dummy pattern
        for (int i = 0; i < save_pitch * save_height; i++) {
            save_buffer[i] = i % 256;
        }
        
        // Save image using old API
        tjSaveImage("output_file", save_buffer, save_width, 
                   save_pitch, save_height, TJPF_RGB, 0);
        
        free(save_buffer);
        
        // Clean up output file
        remove("output_file");
    }

    // Cleanup
    free(jpegBuf);
    tj3Destroy(handle3);
    tjDestroy(handle_old);
    
    assert_file_closed(&in_file);
	return 0;
}