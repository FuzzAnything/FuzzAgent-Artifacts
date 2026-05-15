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
//<ID> 114
//<Prompt> []
/*<Combination>: [
*/
//<score> 22, nr_unique_branch: 0
//<Quality> {"density":22,"unique_branches":{},"library_calls":["tjInitCompress","tj3InitVersion","tjDestroy","tjDecompressHeader3","tj3Destroy","tjDestroy","tj3Alloc","tj3Destroy","tjDestroy","tj3Decompress8","tj3Alloc","tj3Decompress16","tj3Free","tj3YUVBufSize","tj3Alloc","tjEncodeYUV2","tj3SetCroppingRegion","tjCompressFromYUV","tjFree","tj3Free","tj3Free","tj3Destroy","tjDestroy"],"critical_calls":["tjInitCompress","tj3InitVersion","tjDecompressHeader3","tj3Alloc","tj3Decompress8","tj3Alloc","tj3Decompress16","tj3Free","tj3YUVBufSize","tj3Alloc","tjEncodeYUV2","tj3SetCroppingRegion","tjCompressFromYUV","tjFree","tj3Free","tj3Free","tj3Destroy","tjDestroy"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified libjpeg-turbo APIs to implement a transformation pipeline:

*/


// Include libjpeg-turbo headers

extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data
    
    // Create compress handle for old API
    tjhandle compress_handle = tjInitCompress();
    if (!compress_handle) {
	return 0;
	}
	
    
    // Create decompress handle for new API
    tjhandle decompress_handle = tj3Init(TJINIT_DECOMPRESS);
    if (!decompress_handle) {
        tjDestroy(compress_handle);
        return 0;
    }
    
    // Try to decompress the input as JPEG using 8-bit API
    int width, height, jpegSubsamp, jpegColorspace;
    if (tjDecompressHeader3(compress_handle, data, size, 
                           &width, &height, &jpegSubsamp, &jpegColorspace) == 0) {
        
        // Ensure reasonable dimensions for fuzzing
        if (width > 4096 || height > 4096 || width <= 0 || height <= 0) {
            tj3Destroy(decompress_handle);
            tjDestroy(compress_handle);
            return 0;
        }
        
        // Calculate pitch (bytes per row)
        int pitch = width * tjPixelSize[TJPF_RGB];
        
        // Allocate destination buffer for 8-bit decompression
        unsigned char *dstBuf8 = (unsigned char *)tj3Alloc(pitch * height);
        if (!dstBuf8) {
            tj3Destroy(decompress_handle);
            tjDestroy(compress_handle);
            return 0;
        }
        
        // Decompress using 8-bit API
        if (tj3Decompress8(decompress_handle, data, size, dstBuf8, 
                          pitch, TJPF_RGB) == 0) {
            
            // Now decompress using 16-bit API (allocate new buffer)
            unsigned short *dstBuf16 = (unsigned short *)tj3Alloc(pitch * height * 2);
            if (dstBuf16) {
                // Note: This will fail for 8-bit JPEGs, but we call it anyway
                tj3Decompress16(decompress_handle, data, size, dstBuf16, 
                               pitch, TJPF_RGB);
                tj3Free(dstBuf16);
            }
            
            // Prepare YUV buffer for encoding
            int align = 4;
            size_t yuvBufSize = tj3YUVBufSize(width, align, height, jpegSubsamp);
            unsigned char *yuvBuf = (unsigned char *)tj3Alloc(yuvBufSize);
            
            if (yuvBuf) {
                // Encode RGB to YUV using old API
                tjEncodeYUV2(compress_handle, dstBuf8, width, pitch, height,
                            TJPF_RGB, yuvBuf, jpegSubsamp, 0);
                
                // Set a cropping region (center 50% of image)
                tjregion cropRegion;
                cropRegion.x = width / 4;
                cropRegion.y = height / 4;
                cropRegion.w = width / 2;
                cropRegion.h = height / 2;
                
                // Apply cropping region to decompress handle
                tj3SetCroppingRegion(decompress_handle, cropRegion);
                
                // Now compress from YUV back to JPEG
                unsigned char *jpegBuf = nullptr;
                unsigned long jpegSize = 0;
                
                if (tjCompressFromYUV(compress_handle, yuvBuf, width, align, height,
                                     jpegSubsamp, &jpegBuf, &jpegSize, 85, 0) == 0) {
                    // Free the compressed JPEG buffer
                    if (jpegBuf) tjFree(jpegBuf);
                }
                
                tj3Free(yuvBuf);
            }
        }
        
        tj3Free(dstBuf8);
    }
    
    // Clean up
    tj3Destroy(decompress_handle);
    tjDestroy(compress_handle);
    
    return 0;
}