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
//<ID> 704
//<Prompt> []
/*<Combination>: [
*/
//<score> 46, nr_unique_branch: 5
//<Quality> {"density":23,"unique_branches":{"get_dac":[[410,9,410,18,0,0,4,0]],"tj3Set":[[739,3,739,29,0,0,4,0],[725,7,725,16,2,0,4,1],[725,20,725,29,2,0,4,1]],"my_emit_message":[[95,9,95,29,0,0,4,0]]},"library_calls":["tj3InitVersion","tj3Set","tj3DecompressHeader","tj3Get","tj3Get","tj3Get","tj3YUVBufSize","tj3JPEGBufSize","tj3Alloc","TJBUFSIZEYUV","tj3GetICCProfile","tj3DecompressToYUV8","tjInitDecompress","tjAlloc","tjDecompressToYUV","tjFree","tjDestroy","tj3Alloc","tj3Compress16","tj3Alloc","tj3SaveImage12","tj3Alloc","tjSaveImage","tj3Free","tj3Free","tj3Free","tj3Free","tj3Free","tj3Free","tj3Destroy"],"critical_calls":["tj3InitVersion","tj3Set","tj3DecompressHeader","tj3Get","tj3Get","tj3Get","tj3YUVBufSize","tj3JPEGBufSize","tj3Alloc","TJBUFSIZEYUV","tj3GetICCProfile","tj3DecompressToYUV8","tjInitDecompress","tjAlloc","tjDecompressToYUV","tjFree","tjDestroy","tj3Alloc","tj3Compress16","tj3Alloc","tj3SaveImage12","tj3Alloc","tjSaveImage","tj3Free","tj3Free","tj3Free","tj3Free","tj3Free","tj3Free","tj3Destroy"],"visited":1}
/*To create a fuzz driver that utilizes the specified libjpeg-turbo APIs to process JPEG data, I'll design a comprehensive test that performs multiple operations including decompression, compression, ICC profile handling, and image saving. Here's the implementation:

*/


extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for JPEG headers
    
    tjhandle handle = nullptr;
    int width = 0, height = 0, jpegSubsamp = 0, pixelFormat = TJPF_RGB;
    unsigned char *jpegBuf = nullptr;
    unsigned char *yuvBuf = nullptr;
    unsigned short *src16Buf = nullptr;
    short *dst12Buf = nullptr;
    unsigned char *compressedJpeg = nullptr;
    size_t compressedSize = 0;
    unsigned char *iccBuf = nullptr;
    size_t iccSize = 0;
    
    // 1. First try to decompress the input as JPEG to YUV using new API
    handle = tj3Init(TJINIT_DECOMPRESS);
    if (!handle) {
	return 0;
	}
	
    
    // Set some parameters
    tj3Set(handle, TJPARAM_STOPONWARNING, 1);
    
    // Try to decompress header first
    if (tj3DecompressHeader(handle, data, size) == 0) {
        width = tj3Get(handle, TJPARAM_JPEGWIDTH);
        height = tj3Get(handle, TJPARAM_JPEGHEIGHT);
        jpegSubsamp = tj3Get(handle, TJPARAM_SUBSAMP);
        
        if (width > 0 && height > 0 && width <= 4096 && height <= 4096) {
            // Calculate buffer sizes
            size_t yuvBufSize = tj3YUVBufSize(width, 4, height, jpegSubsamp);
            size_t jpegBufSize = tj3JPEGBufSize(width, height, jpegSubsamp);
            
            // Allocate buffers
            yuvBuf = (unsigned char*)tj3Alloc(yuvBufSize);
            unsigned long oldYuvBufSize = TJBUFSIZEYUV(width, height, jpegSubsamp);
            
            // Try to get ICC profile
            tj3GetICCProfile(handle, &iccBuf, &iccSize);
            
            if (yuvBuf) {
                // Decompress to YUV using new API
                tj3DecompressToYUV8(handle, data, size, yuvBuf, 4);
                
                // Also try old API decompression
                tjhandle oldHandle = tjInitDecompress();
                if (oldHandle) {
                    unsigned char *oldYuvBuf = (unsigned char*)tjAlloc(oldYuvBufSize);
                    if (oldYuvBuf) {
                        tjDecompressToYUV(oldHandle, (unsigned char*)data, size, 
                                        oldYuvBuf, 0);
                        tjFree(oldYuvBuf);
                    }
                    tjDestroy(oldHandle);
                }
                
                // 2. Create a 16-bit source buffer for compression
                size_t srcSize = width * height * tjPixelSize[TJPF_RGB];
                src16Buf = (unsigned short*)tj3Alloc(srcSize * sizeof(unsigned short));
                
                if (src16Buf) {
                    // Fill with some data (scaled from YUV or random)
                    for (size_t i = 0; i < srcSize && i < 10000; i++) {
                        src16Buf[i] = (i % 65535);
                    }
                    
                    // Compress 16-bit data
                    tj3Compress16(handle, src16Buf, width, width * 3, height, 
                                TJPF_RGB, &compressedJpeg, &compressedSize);
                    
                    // Save 12-bit image
                    if (width > 10 && height > 10) {
                        dst12Buf = (short*)tj3Alloc(width * height * sizeof(short));
                        if (dst12Buf) {
                            // Fill with some data
                            for (int i = 0; i < width * height && i < 1000; i++) {
                                dst12Buf[i] = (i % 4095);
                            }
                            
                            // Save as 12-bit image
                            tj3SaveImage12(handle, "output_12bit.ppm", dst12Buf, 
                                        width, width * sizeof(short), height, TJPF_GRAY);
                        }
                    }
                }
                
                // 3. Save 8-bit image using old API
                if (width > 10 && height > 10) {
                    // Create an RGB buffer
                    unsigned char *rgbBuf = (unsigned char*)tj3Alloc(width * height * 3);
                    if (rgbBuf) {
                        // Fill with pattern
                        for (int i = 0; i < width * height * 3; i++) {
                            rgbBuf[i] = (i % 256);
                        }
                        
                        // Save using old API
                        tjSaveImage("output_8bit.ppm", rgbBuf, width, width * 3, 
                                  height, TJPF_RGB, 0);
                        
                        tj3Free(rgbBuf);
                    }
                }
            }
        }
    }
    
    // Cleanup
    if (src16Buf) tj3Free(src16Buf);
    if (dst12Buf) tj3Free(dst12Buf);
    if (yuvBuf) tj3Free(yuvBuf);
    if (compressedJpeg) tj3Free(compressedJpeg);
    if (iccBuf) tj3Free(iccBuf);
    if (handle) tj3Destroy(handle);
    
    // Clean up temporary files
    remove("output_8bit.ppm");
    remove("output_12bit.ppm");
    
    return 0;
}