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
//<ID> 327
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 1
//<Quality> {"density":14,"unique_branches":{"tj3Compress16":[[87,7,87,35,0,0,4,0]]},"library_calls":["tj3InitVersion","tj3DecompressHeader","tj3Get","tj3Get","tj3Alloc","tj3Decompress8","tjGetErrorCode","tj3TransformBufSize","tjPlaneSizeYUV","tj3Alloc","tj3Compress16","tj3Alloc","tj3Alloc","tj3Compress16","tj3Free","tj3Free","tj3Free","tj3Free","tj3Destroy"],"critical_calls":["tj3InitVersion","tj3DecompressHeader","tj3Get","tj3Get","tj3Alloc","tj3Decompress8","tjGetErrorCode","tj3TransformBufSize","tjPlaneSizeYUV","tj3Alloc","tj3Compress16","tj3Alloc","tj3Free","tj3Free","tj3Free","tj3Free","tj3Destroy"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit if input is too small to be meaningful
    if (size < 100) {
	return 0;
	}
	
    
    tjhandle handle = nullptr;
    unsigned char *jpegBuf = nullptr;
    unsigned short *srcBuf16 = nullptr;
    unsigned char *dstBuf8 = nullptr;
    unsigned char *transformedBuf = nullptr;
    size_t jpegSize = 0;
    int width = 0, height = 0;
    int pixelFormat = TJPF_RGB;
    
    // Create a JPEG handle for both compression and decompression
    handle = tj3Init(TJINIT_COMPRESS | TJINIT_DECOMPRESS);
    if (!handle) {
	return 0;
	}
	
    
    // Step 1: First try to decompress the input as JPEG using tj3Decompress8
    // We need to allocate a destination buffer. We'll get dimensions first.
    if (tj3DecompressHeader(handle, data, size) == 0) {
        width = tj3Get(handle, TJPARAM_JPEGWIDTH);
        height = tj3Get(handle, TJPARAM_JPEGHEIGHT);
        
        if (width > 0 && height > 0) {
            int pitch = width * tjPixelSize[pixelFormat];
            dstBuf8 = (unsigned char*)tj3Alloc(pitch * height);
            if (dstBuf8) {
                // Actually decompress the JPEG
                if (tj3Decompress8(handle, data, size, dstBuf8, pitch, pixelFormat) == 0) {
                    // Check error code after decompression
                    int errorCode = tjGetErrorCode(handle);
                    if (errorCode != 0) {
                        // Error occurred, but we continue anyway
                    }
                }
            }
        }
    }
    
    // Step 2: Compute transform buffer size for a dummy transformation
    tjtransform transform;
    memset(&transform, 0, sizeof(transform));
    transform.r.x = 0;
    transform.r.y = 0;
    transform.r.w = (width > 100) ? 100 : (width > 0 ? width : 100);
    transform.r.h = (height > 100) ? 100 : (height > 0 ? height : 100);
    transform.op = TJXOP_NONE;
    transform.options = 0;
    
    size_t transformBufSize = tj3TransformBufSize(handle, &transform);
    
    // Step 3: Calculate YUV plane size for a component
    // Use reasonable defaults if dimensions weren't obtained
    int calcWidth = (width > 0) ? width : 640;
    int calcHeight = (height > 0) ? height : 480;
    unsigned long yuvPlaneSize = tjPlaneSizeYUV(0, calcWidth, 0, calcHeight, TJSAMP_420);
    
    // Step 4: Compress a 16-bit image (convert 8-bit to 16-bit if we have decompressed data)
    if (dstBuf8 && width > 0 && height > 0) {
        // Convert 8-bit to 16-bit by scaling up
        int numPixels = width * height * tjPixelSize[pixelFormat];
        srcBuf16 = (unsigned short*)tj3Alloc(numPixels * sizeof(unsigned short));
        if (srcBuf16) {
            for (int i = 0; i < numPixels; i++) {
                srcBuf16[i] = (unsigned short)dstBuf8[i] * 257; // Scale 0-255 to 0-65535
            }
            
            int pitch = width * tjPixelSize[pixelFormat] * sizeof(unsigned short);
            if (tj3Compress16(handle, srcBuf16, width, pitch, height, pixelFormat, &jpegBuf, &jpegSize) == 0) {
                // Successfully compressed, jpegBuf and jpegSize are now set
                
                // Optionally, we could apply the transformation to the newly compressed JPEG
                if (transformBufSize > 0 && jpegSize > 0) {
                    transformedBuf = (unsigned char*)tj3Alloc(transformBufSize);
                    if (transformedBuf) {
                        // We would use tj3Transform here, but we're only required to use tj3TransformBufSize
                        // So we just allocate the buffer but don't call the transform function
                    }
                }
            }
        }
    } else {
        // If we couldn't decompress, create a simple 16-bit test pattern and compress it
        int testWidth = 64;
        int testHeight = 64;
        int testPitch = testWidth * tjPixelSize[pixelFormat] * sizeof(unsigned short);
        int testNumPixels = testWidth * testHeight * tjPixelSize[pixelFormat];
        
        srcBuf16 = (unsigned short*)tj3Alloc(testNumPixels * sizeof(unsigned short));
        if (srcBuf16) {
            // Create a simple gradient pattern
            for (int i = 0; i < testNumPixels; i++) {
                srcBuf16[i] = (i % 65536);
            }
            
            if (tj3Compress16(handle, srcBuf16, testWidth, testPitch, testHeight, pixelFormat, &jpegBuf, &jpegSize) != 0) {
                // Compression failed, but we continue
            }
        }
    }
    
    // Cleanup
    if (transformedBuf) tj3Free(transformedBuf);
    if (jpegBuf) tj3Free(jpegBuf);
    if (srcBuf16) tj3Free(srcBuf16);
    if (dstBuf8) tj3Free(dstBuf8);
    if (handle) tj3Destroy(handle);
    
    return 0;
}