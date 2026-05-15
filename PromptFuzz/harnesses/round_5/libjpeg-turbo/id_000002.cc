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
//<ID> 4
//<Prompt> []
/*<Combination>: [
*/
//<score> 33, nr_unique_branch: 1
//<Quality> {"density":33,"unique_branches":{"first_marker":[[279,38,279,43,8,0,4,0]]},"library_calls":["tjInitDecompress","tjInitCompress","tjDestroy","tjDestroy","tjDecompressHeader3","tjGetErrorStr2","tjDestroy","tjDestroy","tjDestroy","tjDestroy","tjDestroy","tjDestroy","tjDecompress2","tjGetErrorStr2","tjDestroy","tjDestroy","TJBUFSIZEYUV","tjDestroy","tjDestroy","tjPlaneWidth","tjPlaneHeight","tjAlloc","tjPlaneSizeYUV","tjDestroy","tjDestroy","tjEncodeYUVPlanes","tjGetErrorStr2","tjFree","tjDestroy","tjDestroy","tjFree","tjDestroy","tjDestroy"],"critical_calls":["tjInitDecompress","tjInitCompress","tjDecompressHeader3","tjDecompress2","TJBUFSIZEYUV","tjPlaneWidth","tjPlaneHeight","tjAlloc","tjPlaneSizeYUV","tjEncodeYUVPlanes","tjGetErrorStr2","tjFree","tjDestroy","tjDestroy"],"visited":1}
/**/


// Include necessary libjpeg-turbo headers

extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize handles
    tjhandle decompress_handle = tjInitDecompress();
    tjhandle compress_handle = tjInitCompress();
    if (!decompress_handle || !compress_handle) {
        if (decompress_handle) tjDestroy(decompress_handle);
        if (compress_handle) tjDestroy(compress_handle);
        return 0;
    }

    // Step 1: Decompress header to get image info
    int width = 0, height = 0, jpegSubsamp = 0, jpegColorspace = 0;
    if (tjDecompressHeader3(decompress_handle, data, (unsigned long)size,
                           &width, &height, &jpegSubsamp, &jpegColorspace) != 0) {
        // Use error function
        char *error = tjGetErrorStr2(decompress_handle);
        tjDestroy(decompress_handle);
        tjDestroy(compress_handle);
        return 0;
    }

    // Skip if dimensions are invalid
    if (width <= 0 || height <= 0) {
        tjDestroy(decompress_handle);
        tjDestroy(compress_handle);
        return 0;
    }

    // Step 2: Decompress JPEG to RGB buffer
    int pixelFormat = TJPF_RGB;
    int pitch = width * tjPixelSize[pixelFormat];
    unsigned long rgbBufSize = pitch * height;
    unsigned char *rgbBuf = (unsigned char *)malloc(rgbBufSize);
    if (!rgbBuf) {
        tjDestroy(decompress_handle);
        tjDestroy(compress_handle);
        return 0;
    }

    if (tjDecompress2(decompress_handle, data, (unsigned long)size,
                      rgbBuf, width, pitch, height, pixelFormat, 0) != 0) {
        char *error = tjGetErrorStr2(decompress_handle);
        free(rgbBuf);
        tjDestroy(decompress_handle);
        tjDestroy(compress_handle);
        return 0;
    }

    // Step 3: Calculate YUV buffer size
    unsigned long yuvBufSize = TJBUFSIZEYUV(width, height, jpegSubsamp);
    if (yuvBufSize == 0) {
        free(rgbBuf);
        tjDestroy(decompress_handle);
        tjDestroy(compress_handle);
        return 0;
    }

    // Step 4: Encode to YUV planes
    // Allocate planes
    int strides[3] = {0};
    unsigned char *dstPlanes[3] = {NULL, NULL, NULL};
    
    // Calculate plane dimensions
    int planeWidth = tjPlaneWidth(0, width, jpegSubsamp);
    int planeHeight = tjPlaneHeight(0, height, jpegSubsamp);
    strides[0] = planeWidth;
    
    // Allocate Y plane
    dstPlanes[0] = (unsigned char *)tjAlloc(tjPlaneSizeYUV(0, width, strides[0], height, jpegSubsamp));
    if (!dstPlanes[0]) {
        free(rgbBuf);
        tjDestroy(decompress_handle);
        tjDestroy(compress_handle);
        return 0;
    }

    // For simplicity, we'll only allocate and use Y plane in this example
    // In a full implementation, you'd allocate U and V planes too
    if (tjEncodeYUVPlanes(compress_handle, rgbBuf, width, pitch, height,
                          pixelFormat, dstPlanes, strides, jpegSubsamp, 0) != 0) {
        char *error = tjGetErrorStr2(compress_handle);
        tjFree(dstPlanes[0]);
        free(rgbBuf);
        tjDestroy(decompress_handle);
        tjDestroy(compress_handle);
        return 0;
    }

    // Clean up
    tjFree(dstPlanes[0]);
    free(rgbBuf);
    tjDestroy(decompress_handle);
    tjDestroy(compress_handle);

    return 0;
}