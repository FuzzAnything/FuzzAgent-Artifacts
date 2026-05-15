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
//<ID> 22
//<Prompt> []
/*<Combination>: [
*/
//<score> 5.6666665, nr_unique_branch: 0
//<Quality> {"density":17,"unique_branches":{},"library_calls":["tj3InitVersion","tj3InitVersion","tj3Destroy","tjDecompressHeader3","tj3DecompressHeader","tjBufSizeYUV","tj3Decompress16","tjPlaneWidth","tjPlaneWidth","tjPlaneWidth","tjPlaneHeight","tjAlloc","tj3EncodeYUVPlanes8","tjEncodeYUV","tjFree","tj3Destroy","tj3Destroy"],"critical_calls":["tj3InitVersion","tj3InitVersion","tjDecompressHeader3","tj3DecompressHeader","tjBufSizeYUV","tj3Decompress16","tjPlaneWidth","tjPlaneWidth","tjPlaneWidth","tjPlaneHeight","tjAlloc","tj3EncodeYUVPlanes8","tjEncodeYUV","tjFree","tj3Destroy","tj3Destroy"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Check minimum size for JPEG header
    if (size < 4) {
	return 0;
	}
	
    
    // Initialize decompressor
    tjhandle decompressor = tj3Init(TJINIT_DECOMPRESS);
    if (!decompressor) {
	return 0;
	}
	
    
    // Initialize compressor
    tjhandle compressor = tj3Init(TJINIT_COMPRESS);
    if (!compressor) {
        tj3Destroy(decompressor);
        return 0;
    }
    
    // Try to decompress header
    int width, height, jpegSubsamp, jpegColorspace;
    int headerResult = tjDecompressHeader3(decompressor, data, size, 
                                          &width, &height, &jpegSubsamp, &jpegColorspace);
    
    if (headerResult == 0 && width > 0 && height > 0) {
        // Also try new API header decompression
        tj3DecompressHeader(decompressor, data, size);
        
        // Calculate buffer sizes
        unsigned long yuvBufSize = tjBufSizeYUV(width, height, jpegSubsamp);
        unsigned long rgbBufSize = width * height * 3; // Simple RGB estimate
        
        // Allocate buffers
        unsigned short *rgb16Buf = (unsigned short*)malloc(rgbBufSize * sizeof(unsigned short));
        unsigned char *yuvBuf = (unsigned char*)malloc(yuvBufSize);
        unsigned char *rgb8Buf = (unsigned char*)malloc(rgbBufSize);
        
        if (rgb16Buf && yuvBuf && rgb8Buf) {
            // Try to decompress to 16-bit RGB
            int decompressResult = tj3Decompress16(decompressor, data, size, rgb16Buf, 
                                                   width * 3, TJPF_RGB);
            
            if (decompressResult == 0) {
                // Convert 16-bit RGB to 8-bit RGB for YUV encoding
                for (unsigned long i = 0; i < rgbBufSize; i++) {
                    rgb8Buf[i] = (unsigned char)(rgb16Buf[i] >> 8);
                }
                
                // Allocate YUV planes for new API
                unsigned char *yuvPlanes[3];
                int strides[3] = {0};
                
                // Calculate plane strides
                strides[0] = tjPlaneWidth(0, width, jpegSubsamp);
                strides[1] = tjPlaneWidth(1, width, jpegSubsamp);
                strides[2] = tjPlaneWidth(2, width, jpegSubsamp);
                
                // Allocate planes
                for (int i = 0; i < 3; i++) {
                    int planeHeight = tjPlaneHeight(i, height, jpegSubsamp);
                    yuvPlanes[i] = (unsigned char*)tjAlloc(strides[i] * planeHeight);
                }
                
                // Try new API YUV plane encoding
                if (yuvPlanes[0] && yuvPlanes[1] && yuvPlanes[2]) {
                    tj3EncodeYUVPlanes8(compressor, rgb8Buf, width, width * 3, height, 
                                       TJPF_RGB, yuvPlanes, strides);
                }
                
                // Try old API YUV encoding
                tjEncodeYUV(compressor, rgb8Buf, width, width * 3, height, 3, 
                           yuvBuf, jpegSubsamp, 0);
                
                // Free YUV planes
                for (int i = 0; i < 3; i++) {
                    if (yuvPlanes[i]) tjFree(yuvPlanes[i]);
                }
            }
        }
        
        // Free buffers
        if (rgb16Buf) free(rgb16Buf);
        if (yuvBuf) free(yuvBuf);
        if (rgb8Buf) free(rgb8Buf);
    }
    
    // Cleanup
    tj3Destroy(decompressor);
    tj3Destroy(compressor);
    
    return 0;
}