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
//<ID> 144
//<Prompt> []
/*<Combination>: [
*/
//<score> 52, nr_unique_branch: 11
//<Quality> {"density":26,"unique_branches":{"tj3Compress16":[[87,7,87,35,0,0,4,1],[90,7,90,21,0,0,4,1],[90,25,90,35,0,0,4,1],[90,39,90,48,0,0,4,1],[90,52,90,63,0,0,4,1],[91,7,91,22,0,0,4,1],[91,26,91,49,0,0,4,1],[91,53,91,68,0,0,4,1],[92,7,92,23,0,0,4,1],[95,7,95,22,0,0,4,0],[95,26,95,45,0,0,4,0]]},"library_calls":["tj3InitVersion","tjInitDecompress","tj3LoadImage12","tj3SaveImage12","tj3Alloc","tj3Compress16","TJBUFSIZEYUV","tjAlloc","tjDecompressHeader2","tjDecompressToYUV","tjAlloc","tjDecodeYUV","tjAlloc","tjEncodeYUV2","tjFree","tjSaveImage","tj3Alloc","tjPlaneWidth","tjPlaneHeight","tj3Alloc","tj3Alloc","tj3EncodeYUVPlanes8","tjCompressFromYUVPlanes","tjFree","tj3Free","tj3Free","tj3Free","tj3DecompressHeader","tj3GetICCProfile","tj3Free","tjFree","tjFree","tj3Free","tj3Free","tj3Free","tj3Destroy","tjDestroy"],"critical_calls":["tj3InitVersion","tjInitDecompress","tj3LoadImage12","tj3SaveImage12","tj3Alloc","tj3Compress16","TJBUFSIZEYUV","tjAlloc","tjDecompressHeader2","tjDecompressToYUV","tjAlloc","tjDecodeYUV","tjAlloc","tjEncodeYUV2","tjFree","tjSaveImage","tj3Alloc","tjPlaneWidth","tjPlaneHeight","tj3Alloc","tj3Alloc","tj3EncodeYUVPlanes8","tjCompressFromYUVPlanes","tjFree","tj3Free","tj3Free","tj3Free","tj3DecompressHeader","tj3GetICCProfile","tj3Free","tjFree","tjFree","tj3Free","tj3Free","tj3Free","tj3Destroy","tjDestroy"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Ensure we have enough data for basic operations
    if (size < 1024) {
	return 0;
	}
	
    
    tjhandle handle = NULL;
    tjhandle old_handle = NULL;
    unsigned char *jpegBuf = NULL;
    unsigned char *yuvBuf = NULL;
    unsigned char **planes = NULL;
    unsigned short *src16 = NULL;
    short *src12 = NULL;
    unsigned char *rgbBuf = NULL;
    unsigned char *iccBuf = NULL;
    size_t jpegSize = 0;
    size_t iccSize = 0;
    unsigned long oldJpegSize = 0;
    int width = 0, height = 0;
    int pitch = 0;
    int subsamp = TJSAMP_420;
    int pixelFormat = TJPF_RGB;
    int ret = 0;
    const char *tempFile = "output_file";
    const char *tempInput = "input_file";
    
    // Write input data to a temp file for file-based APIs
    FILE *inputFile = fopen(tempInput, "wb");
    if (!inputFile) goto cleanup;
    fwrite(data, 1, size, inputFile);
    assert_file_closed(&inputFile);;
    
    // Create handles
    handle = tj3Init(TJINIT_COMPRESS);
    old_handle = tjInitDecompress();
    if (!handle || !old_handle) goto cleanup;
    
    // 1. Load a 12-bit image using file API
    // Note: We're using the input file as source
    src12 = tj3LoadImage12(handle, tempInput, &width, 4, &height, &pixelFormat);
    if (!src12) {
        // If loading fails, set reasonable defaults for dummy operations
        width = 64;
        height = 64;
        pitch = width * tjPixelSize[pixelFormat];
    }
    
    // 2. Save 12-bit image (dummy data if loading failed)
    if (src12) {
        tj3SaveImage12(handle, tempFile, src12, width, pitch, height, pixelFormat);
    }
    
    // 3. Create dummy 16-bit source buffer for compression
    src16 = (unsigned short *)tj3Alloc(width * height * 3 * sizeof(unsigned short));
    if (src16) {
        // Initialize with dummy data
        for (int i = 0; i < width * height * 3; i++) {
            src16[i] = (i * 256) % 65536;
        }
        
        // 4. Compress 16-bit image
        ret = tj3Compress16(handle, src16, width, width * 3 * 2, height, 
                           TJPF_RGB, &jpegBuf, &jpegSize);
        if (ret == 0 && jpegBuf && jpegSize > 0) {
            // 5. Decompress to YUV using old API
            unsigned long yuvBufSize = TJBUFSIZEYUV(width, height, subsamp);
            yuvBuf = (unsigned char *)tjAlloc(yuvBufSize);
            if (yuvBuf) {
                // First get header info
                int w, h, s;
                tjDecompressHeader2(old_handle, jpegBuf, jpegSize, &w, &h, &s);
                
                // Decompress to YUV
                ret = tjDecompressToYUV(old_handle, jpegBuf, jpegSize, yuvBuf, 0);
                
                if (ret == 0) {
                    // 6. Decode YUV back to RGB
                    rgbBuf = (unsigned char *)tjAlloc(width * height * 3);
                    if (rgbBuf) {
                        ret = tjDecodeYUV(old_handle, yuvBuf, 4, subsamp, 
                                         rgbBuf, width, width * 3, height, 
                                         pixelFormat, 0);
                        
                        // 7. Encode RGB to YUV using old API
                        if (ret == 0) {
                            unsigned char *yuvBuf2 = (unsigned char *)tjAlloc(yuvBufSize);
                            if (yuvBuf2) {
                                tjEncodeYUV2(old_handle, rgbBuf, width, width * 3, height,
                                            pixelFormat, yuvBuf2, subsamp, 0);
                                tjFree(yuvBuf2);
                            }
                            
                            // 8. Save RGB image using old API
                            tjSaveImage(tempFile, rgbBuf, width, width * 3, height,
                                       pixelFormat, 0);
                        }
                    }
                    
                    // 9. Compress from YUV planes
                    // First create planes from YUV buffer
                    planes = (unsigned char **)tj3Alloc(3 * sizeof(unsigned char *));
                    int strides[3];
                    if (planes) {
                        // For simplicity, use a single buffer approach
                        unsigned char *yuvPlanes[3];
                        int planeWidth = tjPlaneWidth(0, width, subsamp);
                        int planeHeight = tjPlaneHeight(0, height, subsamp);
                        
                        for (int i = 0; i < 3; i++) {
                            planes[i] = (unsigned char *)tj3Alloc(planeWidth * planeHeight);
                            strides[i] = planeWidth;
                        }
                        
                        // Use the new API to encode to YUV planes
                        unsigned char *dummySrc = (unsigned char *)tj3Alloc(width * height * 3);
                        if (dummySrc) {
                            // Initialize dummy source
                            memset(dummySrc, 128, width * height * 3);
                            
                            // 10. Encode to YUV planes using new API
                            tj3EncodeYUVPlanes8(handle, dummySrc, width, width * 3, height,
                                               TJPF_RGB, planes, strides);
                            
                            // 11. Compress from YUV planes
                            unsigned char *jpegBuf2 = NULL;
                            unsigned long jpegSize2 = 0;
                            tjCompressFromYUVPlanes(old_handle, (const unsigned char **)planes,
                                                   width, strides, height, subsamp,
                                                   &jpegBuf2, &jpegSize2, 85, 0);
                            if (jpegBuf2) tjFree(jpegBuf2);
                            
                            tj3Free(dummySrc);
                        }
                        
                        for (int i = 0; i < 3; i++) {
                            if (planes[i]) tj3Free(planes[i]);
                        }
                        tj3Free(planes);
                    }
                }
            }
        }
    }
    
    // 12. Get ICC profile if available
    // We need to decompress the header first
    if (jpegBuf && jpegSize > 0) {
        tj3DecompressHeader(handle, jpegBuf, jpegSize);
        tj3GetICCProfile(handle, &iccBuf, &iccSize);
    }
    
cleanup:
    // Cleanup all allocated resources
    if (iccBuf) tj3Free(iccBuf);
    if (rgbBuf) tjFree(rgbBuf);
    if (yuvBuf) tjFree(yuvBuf);
    if (jpegBuf) tj3Free(jpegBuf);
    if (src16) tj3Free(src16);
    if (src12) tj3Free(src12);
    if (handle) tj3Destroy(handle);
    if (old_handle) tjDestroy(old_handle);
    
    // Remove temporary files
    remove(tempFile);
    remove(tempInput);
    
    assert_file_closed(&inputFile);
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	return 0;
}