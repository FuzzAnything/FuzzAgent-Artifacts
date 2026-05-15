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
//<ID> 453
//<Prompt> []
/*<Combination>: [
*/
//<score> 9, nr_unique_branch: 0
//<Quality> {"density":9,"unique_branches":{},"library_calls":["tjInitDecompress","tjDecompressHeader","tjDestroy","tjGetScalingFactors","tjInitTransform","tjDestroy","tj3JPEGBufSize","tj3TransformBufSize","tjBufSizeYUV","tjDestroy","tjDestroy","tjDecompressToYUV","tjDestroy","tjDestroy","tj3GetScalingFactors","tj3InitVersion","tj3SetCroppingRegion","tj3Destroy","tjDestroy","tjDestroy"],"critical_calls":["tjInitDecompress","tjDecompressHeader","tjGetScalingFactors","tjInitTransform","tj3JPEGBufSize","tj3TransformBufSize","tjBufSizeYUV","tjDecompressToYUV","tj3GetScalingFactors","tj3InitVersion","tj3SetCroppingRegion","tj3Destroy","tjDestroy","tjDestroy"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
        return 0;
    }

    // Initialize variables
    int width = 0, height = 0;
    unsigned long jpegSize = size;
    tjhandle handle = nullptr;
    tjhandle transformHandle = nullptr;
    unsigned char *jpegBuf = nullptr;
    unsigned char *yuvBuf = nullptr;
    unsigned char *transformBuf = nullptr;
    tjregion croppingRegion;
    tjtransform transform;
    int numScalingFactors = 0;
    tjscalingfactor *scalingFactors = nullptr;
    size_t bufSize = 0;
    size_t transformBufSize = 0;
    int result = 0;

    // Create a FILE* for input data using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Read JPEG data from FILE*
    jpegSize = size;
    jpegBuf = (unsigned char *)malloc(jpegSize);
    if (!jpegBuf) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    fread(jpegBuf, 1, jpegSize, in_file);
    assert_file_closed(&in_file);;

    // Initialize decompression handle
    handle = tjInitDecompress();
    if (!handle) {
        free(jpegBuf);
        assert_file_closed(&in_file);
	return 0;
    }

    // Get header information using old API
    result = tjDecompressHeader(handle, jpegBuf, jpegSize, &width, &height);
    if (result != 0) {
        tjDestroy(handle);
        free(jpegBuf);
        assert_file_closed(&in_file);
	return 0;
    }

    // Get scaling factors using old API
    scalingFactors = tjGetScalingFactors(&numScalingFactors);

    // Create transform handle
    transformHandle = tjInitTransform();
    if (!transformHandle) {
        tjDestroy(handle);
        free(jpegBuf);
        assert_file_closed(&in_file);
	return 0;
    }

    // Calculate JPEG buffer size using new API
    bufSize = tj3JPEGBufSize(width, height, TJSAMP_444);

    // Setup cropping region
    croppingRegion.x = 0;
    croppingRegion.y = 0;
    croppingRegion.w = width / 2;
    croppingRegion.h = height / 2;

    // Setup transform structure
    memset(&transform, 0, sizeof(tjtransform));
    transform.r = croppingRegion;
    transform.op = TJXOP_NONE;
    transform.options = TJXOPT_CROP;

    // Calculate transform buffer size using new API
    transformBufSize = tj3TransformBufSize(transformHandle, &transform);

    // Allocate YUV buffer
    yuvBuf = (unsigned char *)malloc(tjBufSizeYUV(width, height, TJSAMP_444));
    if (!yuvBuf) {
        tjDestroy(transformHandle);
        tjDestroy(handle);
        free(jpegBuf);
        assert_file_closed(&in_file);
	return 0;
    }

    // Decompress to YUV using old API
    result = tjDecompressToYUV(handle, jpegBuf, jpegSize, yuvBuf, 0);
    if (result != 0) {
        free(yuvBuf);
        tjDestroy(transformHandle);
        tjDestroy(handle);
        free(jpegBuf);
        assert_file_closed(&in_file);
	return 0;
    }

    // Get scaling factors using new API
    tjscalingfactor *newScalingFactors = tj3GetScalingFactors(&numScalingFactors);

    // Apply cropping region using new API (requires new handle)
    tjhandle newHandle = tj3InitVersion(TJINIT_DECOMPRESS, 0);
    if (newHandle) {
        tj3SetCroppingRegion(newHandle, croppingRegion);
        tj3Destroy(newHandle);
    }

    // Allocate and use transform buffer
    if (transformBufSize > 0) {
        transformBuf = (unsigned char *)malloc(transformBufSize);
        if (transformBuf) {
            // We have the buffer but won't actually transform since
            // we don't have the transform API in the required list
            free(transformBuf);
        }
    }

    // Cleanup
    free(yuvBuf);
    tjDestroy(transformHandle);
    tjDestroy(handle);
    free(jpegBuf);

    assert_file_closed(&in_file);
	return 0;
}