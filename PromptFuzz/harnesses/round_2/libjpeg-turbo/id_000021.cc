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
//<ID> 326
//<Prompt> []
/*<Combination>: [
*/
//<score> 21, nr_unique_branch: 3
//<Quality> {"density":7,"unique_branches":{"tj3GetErrorCode":[[707,15,707,33,0,0,4,0]],"tj3Decompress8":[[168,61,168,70,0,0,4,0],[284,7,284,41,0,0,4,1]]},"library_calls":["tj3InitVersion","tjInitDecompress","tj3DecompressHeader","tj3Get","tj3Get","tj3Alloc","tj3Decompress8","tjGetErrorCode","tj3TransformBufSize","tj3Alloc","tjPlaneSizeYUV","tj3Alloc","tj3Compress16","tj3Free","tj3TransformBufSize","tj3Transform","tjDecompressHeader2","tjAlloc","tjDecompress2","tjFree","tjGetErrorCode","tj3Destroy","tjDestroy","tj3Free","tj3Free","tj3Free"],"critical_calls":["tj3InitVersion","tjInitDecompress","tj3DecompressHeader","tj3Get","tj3Get","tj3Alloc","tj3Decompress8","tjGetErrorCode","tj3TransformBufSize","tj3Alloc","tjPlaneSizeYUV","tj3Alloc","tj3Compress16","tj3Free","tj3TransformBufSize","tj3Transform","tjDecompressHeader2","tjAlloc","tjDecompress2","tjFree","tjGetErrorCode","tj3Destroy","tjDestroy","tj3Free","tj3Free","tj3Free"],"visited":0}
/*To create a fuzz driver using the specified libjpeg-turbo APIs, I'll implement a program that performs a complete JPEG processing pipeline: decompression, transformation, and recompression. Here's the step-by-step implementation:

*/


extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Validate input
    if (size < 100) {
	return 0;
	}
	
    if (data == nullptr) {
	return 0;
	}
	
    
    tjhandle handle_new = nullptr;
    tjhandle handle_old = nullptr;
    unsigned char *jpegBuf = nullptr;
    unsigned char *dstBuf = nullptr;
    unsigned short *srcBuf16 = nullptr;
    unsigned char *transformedBuf = nullptr;
    tjtransform *transform = nullptr;
    FILE *in_file = nullptr;
    
    // Step 1: Initialize both old and new API handles
    handle_new = tj3Init(TJINIT_DECOMPRESS);
    if (!handle_new) {
	return 0;
	}
	
    
    handle_old = tjInitDecompress();
    if (!handle_old) goto cleanup;
    
    // Step 2: Create input file from fuzzer data
    in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) goto cleanup;
    
    // Step 3: Read JPEG data from memory file
    jpegBuf = (unsigned char *)malloc(size);
    if (!jpegBuf) goto cleanup;
    
    fread(jpegBuf, 1, size, in_file);
    assert_file_closed(&in_file);;
    in_file = nullptr;
    
    // Step 4: Decompress using new API (tj3Decompress8)
    int width, height, pixelFormat;
    if (tj3DecompressHeader(handle_new, jpegBuf, size) == 0) {
        width = tj3Get(handle_new, TJPARAM_JPEGWIDTH);
        height = tj3Get(handle_new, TJPARAM_JPEGHEIGHT);
        pixelFormat = TJPF_RGB;
        
        int pitch = width * tjPixelSize[pixelFormat];
        dstBuf = (unsigned char *)tj3Alloc(pitch * height);
        
        if (dstBuf) {
            int result = tj3Decompress8(handle_new, jpegBuf, size, dstBuf, pitch, pixelFormat);
            
            // Check error code using old API
            int error_code = tjGetErrorCode(handle_old);
            (void)error_code; // Use the error code to avoid unused variable warning
        }
    }
    
    // Step 5: Calculate transformation buffer size
    if (dstBuf) {
        transform = (tjtransform *)malloc(sizeof(tjtransform));
        if (transform) {
            memset(transform, 0, sizeof(tjtransform));
            transform->r.x = 0;
            transform->r.y = 0;
            transform->r.w = width / 2;
            transform->r.h = height / 2;
            transform->op = TJXOP_NONE;
            transform->options = 0;
            
            size_t transformBufSize = tj3TransformBufSize(handle_new, transform);
            if (transformBufSize > 0) {
                transformedBuf = (unsigned char *)tj3Alloc(transformBufSize);
            }
        }
    }
    
    // Step 6: Calculate YUV plane size using old API
    if (width > 0 && height > 0) {
        int componentID = 0; // Y component
        int stride = width;
        int subsamp = TJSAMP_420;
        
        unsigned long planeSize = tjPlaneSizeYUV(componentID, width, stride, height, subsamp);
        (void)planeSize; // Use the result to avoid unused variable warning
    }
    
    // Step 7: Compress using 16-bit API (tj3Compress16)
    if (dstBuf && width > 0 && height > 0) {
        // Convert 8-bit buffer to 16-bit for compression
        int numPixels = width * height;
        srcBuf16 = (unsigned short *)tj3Alloc(numPixels * 3 * sizeof(unsigned short));
        
        if (srcBuf16) {
            // Simple conversion from 8-bit to 16-bit (scale by 256)
            for (int i = 0; i < numPixels * 3; i++) {
                srcBuf16[i] = (unsigned short)(dstBuf[i] << 8);
            }
            
            unsigned char *compressedBuf = nullptr;
            size_t compressedSize = 0;
            int pitch16 = width * 3 * sizeof(unsigned short);
            
            int compressResult = tj3Compress16(handle_new, srcBuf16, width, pitch16, height, 
                                               TJPF_RGB, &compressedBuf, &compressedSize);
            
            if (compressedBuf) {
                tj3Free(compressedBuf);
            }
        }
    }
    
    // Step 8: Perform a simple transform operation if buffers are available
    if (jpegBuf && transformedBuf && transform) {
        unsigned char *dstBufs[1] = {transformedBuf};
        size_t dstSizes[1] = {tj3TransformBufSize(handle_new, transform)};
        
        // Only attempt transform if we have enough data
        if (dstSizes[0] > 0 && dstSizes[0] < 1000000) {
            int transformResult = tj3Transform(handle_new, jpegBuf, size, 1, dstBufs, dstSizes, transform);
        }
    }
    
    // Step 9: Try to decompress using old API to check error code again
    if (jpegBuf && size > 0) {
        int old_width, old_height, old_subsamp;
        if (tjDecompressHeader2(handle_old, jpegBuf, (unsigned long)size, 
                               &old_width, &old_height, &old_subsamp) == 0) {
            unsigned char *old_dstBuf = (unsigned char *)tjAlloc(old_width * old_height * 3);
            if (old_dstBuf) {
                tjDecompress2(handle_old, jpegBuf, (unsigned long)size, old_dstBuf,
                             old_width, 0, old_height, TJPF_RGB, 0);
                tjFree(old_dstBuf);
            }
        }
        
        // Check error code one more time
        int final_error = tjGetErrorCode(handle_old);
        (void)final_error;
    }

cleanup:
    // Clean up all allocated resources
    if (handle_new) tj3Destroy(handle_new);
    if (handle_old) tjDestroy(handle_old);
    if (jpegBuf) free(jpegBuf);
    if (dstBuf) tj3Free(dstBuf);
    if (srcBuf16) tj3Free(srcBuf16);
    if (transformedBuf) tj3Free(transformedBuf);
    if (transform) free(transform);
    if (in_file) assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}