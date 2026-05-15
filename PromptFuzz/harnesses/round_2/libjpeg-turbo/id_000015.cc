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
//<ID> 196
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 1
//<Quality> {"density":14,"unique_branches":{"tj3TransformBufSize":[[340,7,340,12,1,0,4,0]]},"library_calls":["tjLoadImage","tjAlloc","tjInitCompress","tj3InitVersion","tjBufSize","tj3JPEGBufSize","tjAlloc","tjCompress2","tj3Compress12","tjFree","tjGetScalingFactors","tjInitDecompress","tj3InitVersion","tjDecompressHeader3","tjPlaneWidth","tjPlaneHeight","tjPlaneSizeYUV","tjAlloc","tjPlaneSizeYUV","tjPlaneWidth","tjPlaneHeight","tj3DecompressToYUVPlanes8","tjAlloc","tjDecodeYUV","tjFree","tjInitTransform","tj3TransformBufSize","tj3Transform","tjFree","tjFree","tjFree","tjFree","tjFree","tjFree","tjDestroy","tjDestroy","tjDestroy","tj3Destroy","tj3Destroy"],"critical_calls":["tjLoadImage","tjAlloc","tjInitCompress","tj3InitVersion","tjBufSize","tj3JPEGBufSize","tjAlloc","tjCompress2","tj3Compress12","tjFree","tjGetScalingFactors","tjInitDecompress","tj3InitVersion","tjDecompressHeader3","tjPlaneWidth","tjPlaneHeight","tjPlaneSizeYUV","tjAlloc","tjPlaneSizeYUV","tjPlaneHeight","tjPlaneWidth","tj3DecompressToYUVPlanes8","tjAlloc","tjDecodeYUV","tjFree","tjInitTransform","tj3TransformBufSize","tj3Transform","tjFree","tjFree","tjFree","tjFree","tjFree","tjFree","tjDestroy","tjDestroy","tjDestroy","tj3Destroy","tj3Destroy"],"visited":0}
/*Here's a fuzz driver that uses all the specified libjpeg-turbo APIs to achieve a complete JPEG processing pipeline:

*/


extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 100) {
	return 0;
	}
	 // Need minimum data
    
    tjhandle jpegCompressor = nullptr;
    tjhandle jpegDecompressor = nullptr;
    tjhandle transformHandle = nullptr;
    unsigned char *srcBuffer = nullptr;
    short *srcBuffer12 = nullptr;
    unsigned char *jpegBuffer = nullptr;
    unsigned char *jpegBuffer2 = nullptr;
    unsigned char *yuvBuffer = nullptr;
    unsigned char **yuvPlanes = nullptr;
    int *strides = nullptr;
    tjscalingfactor *scalingFactors = nullptr;
    int numScalingFactors = 0;
    
    // 1. First, write input data to a temporary file
    FILE *input_file = fopen("input_file", "wb");
    if (!input_file) {
	assert_file_closed(&input_file);
	return 0;
	}
	
    fwrite(data, 1, size, input_file);
    assert_file_closed(&input_file);;
    
    // 2. Load image using tjLoadImage (old API)
    int width = 0, height = 0, pixelFormat = 0;
    srcBuffer = tjLoadImage("input_file", &width, 1, &height, &pixelFormat, 0);
    
    // If image loading failed, try to use input data directly as JPEG
    if (!srcBuffer || width <= 0 || height <= 0) {
        // Use input data as raw image buffer instead
        width = 64;
        height = 64;
        pixelFormat = TJPF_RGB;
        
        // Allocate and fill with test pattern
        srcBuffer = tjAlloc(width * height * 3);
        if (srcBuffer) {
            for (int i = 0; i < width * height * 3; i++) {
                srcBuffer[i] = (i % 256);
            }
        }
    }
    
    if (!srcBuffer) {
        remove("input_file");
        assert_file_closed(&input_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // 3. Initialize compressor handles
    jpegCompressor = tjInitCompress();  // Old API
    tjhandle newCompressor = tj3InitVersion(TJINIT_COMPRESS, 0);  // New API
    
    // 4. Calculate buffer sizes
    unsigned long bufSize = tjBufSize(width, height, TJSAMP_444);
    size_t newBufSize = tj3JPEGBufSize(width, height, TJSAMP_444);
    
    // 5. Compress using old API (tjCompress2)
    unsigned long jpegSize = bufSize;
    if (jpegCompressor && srcBuffer) {
        jpegBuffer = tjAlloc(bufSize);
        if (jpegBuffer) {
            tjCompress2(jpegCompressor, srcBuffer, width, 0, height, pixelFormat, 
                       &jpegBuffer, &jpegSize, TJSAMP_444, 85, 0);
        }
    }
    
    // 6. Create 12-bit source buffer for tj3Compress12
    srcBuffer12 = (short*)malloc(width * height * 3 * sizeof(short));
    if (srcBuffer12 && srcBuffer) {
        for (int i = 0; i < width * height * 3; i++) {
            srcBuffer12[i] = srcBuffer[i] * 16;  // Scale to 12-bit
        }
        
        // 7. Compress using new API (tj3Compress12)
        size_t jpegSize12 = 0;
        unsigned char *jpegBuffer12 = nullptr;
        if (newCompressor) {
            tj3Compress12(newCompressor, srcBuffer12, width, 0, height, 
                         TJPF_RGB, &jpegBuffer12, &jpegSize12);
            if (jpegBuffer12) {
                tjFree(jpegBuffer12);
            }
        }
    }
    
    // 8. Get scaling factors
    scalingFactors = tjGetScalingFactors(&numScalingFactors);
    
    // 9. Initialize decompressor
    jpegDecompressor = tjInitDecompress();
    tjhandle newDecompressor = tj3InitVersion(TJINIT_DECOMPRESS, 0);
    
    // 10. Decompress to YUV planes if we have JPEG data
    if (jpegBuffer && jpegSize > 0 && newDecompressor) {
        // First get header info
        int jpegWidth, jpegHeight, jpegSubsamp, jpegColorspace;
        tjDecompressHeader3(jpegDecompressor, jpegBuffer, jpegSize, 
                           &jpegWidth, &jpegHeight, &jpegSubsamp, &jpegColorspace);
        
        // Allocate YUV planes
        int stridesArray[3] = {0};
        yuvPlanes = (unsigned char**)malloc(3 * sizeof(unsigned char*));
        
        // Calculate plane dimensions
        int planeWidth = tjPlaneWidth(0, jpegWidth, jpegSubsamp);
        int planeHeight = tjPlaneHeight(0, jpegHeight, jpegSubsamp);
        unsigned long planeSize = tjPlaneSizeYUV(0, planeWidth, 0, planeHeight, jpegSubsamp);
        
        // Allocate planes
        for (int i = 0; i < 3; i++) {
            yuvPlanes[i] = tjAlloc(tjPlaneSizeYUV(i, tjPlaneWidth(i, jpegWidth, jpegSubsamp),
                                                  0, tjPlaneHeight(i, jpegHeight, jpegSubsamp),
                                                  jpegSubsamp));
            stridesArray[i] = 0; // Auto-calculate stride
        }
        
        // Decompress to YUV planes
        tj3DecompressToYUVPlanes8(newDecompressor, jpegBuffer, jpegSize, 
                                 yuvPlanes, stridesArray);
        
        // Decode YUV back to RGB
        unsigned char *rgbBuffer = tjAlloc(jpegWidth * jpegHeight * 3);
        if (rgbBuffer && jpegDecompressor) {
            // Use first plane as YUV source
            tjDecodeYUV(jpegDecompressor, yuvPlanes[0], 4, jpegSubsamp,
                       rgbBuffer, jpegWidth, 0, jpegHeight, TJPF_RGB, 0);
            tjFree(rgbBuffer);
        }
    }
    
    // 11. Transform operations
    transformHandle = tjInitTransform();
    if (transformHandle && jpegBuffer && jpegSize > 0) {
        tjtransform transform;
        memset(&transform, 0, sizeof(transform));
        transform.r.x = 0;
        transform.r.y = 0;
        transform.r.w = width / 2;
        transform.r.h = height / 2;
        transform.op = TJXOP_NONE;
        transform.options = 0;
        transform.customFilter = nullptr;
        
        // Calculate transform buffer size
        size_t transformBufSize = tj3TransformBufSize(newDecompressor, &transform);
        
        if (transformBufSize > 0) {
            unsigned char *dstBuf = nullptr;
            size_t dstSize = 0;
            tj3Transform(newDecompressor, jpegBuffer, jpegSize, 1, 
                        &dstBuf, &dstSize, &transform);
            if (dstBuf) {
                tjFree(dstBuf);
            }
        }
    }
    
    // 12. Cleanup
    if (srcBuffer) tjFree(srcBuffer);
    if (srcBuffer12) free(srcBuffer12);
    if (jpegBuffer) tjFree(jpegBuffer);
    if (jpegBuffer2) tjFree(jpegBuffer2);
    if (yuvBuffer) tjFree(yuvBuffer);
    
    if (yuvPlanes) {
        for (int i = 0; i < 3; i++) {
            if (yuvPlanes[i]) tjFree(yuvPlanes[i]);
        }
        free(yuvPlanes);
    }
    
    if (strides) free(strides);
    
    if (jpegCompressor) tjDestroy(jpegCompressor);
    if (jpegDecompressor) tjDestroy(jpegDecompressor);
    if (transformHandle) tjDestroy(transformHandle);
    if (newCompressor) tj3Destroy(newCompressor);
    if (newDecompressor) tj3Destroy(newDecompressor);
    
    remove("input_file");
    
    assert_file_closed(&input_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}