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
//<ID> 190
//<Prompt> []
/*<Combination>: [
*/
//<score> 14.666667, nr_unique_branch: 0
//<Quality> {"density":44,"unique_branches":{},"library_calls":["tjLoadImage","tjLoadImage","tjFree","tjGetScalingFactors","tjBufSize","tjInitCompress","tjFree","tjCompress2","tjDestroy","tjFree","tjFree","tj3InitVersion","tjFree","tjDecompressHeader3","tj3Destroy","tjFree","tjPlaneWidth","tjPlaneHeight","tjPlaneSizeYUV","tjAlloc","tjFree","tj3Destroy","tjFree","tj3DecompressToYUVPlanes8","tjFree","tj3Destroy","tjFree","tjBufSizeYUV2","tjAlloc","tjFree","tj3Destroy","tjFree","tjAlloc","tjFree","tjFree","tj3Destroy","tjFree","tjInitDecompress","tjFree","tjFree","tjFree","tj3Destroy","tjFree","tjDecodeYUV","tjDestroy","tjFree","tjFree","tjFree","tj3Destroy","tjFree"],"critical_calls":["tjLoadImage","tjLoadImage","tjGetScalingFactors","tjBufSize","tjInitCompress","tjCompress2","tjDestroy","tjFree","tj3InitVersion","tjDecompressHeader3","tjPlaneWidth","tjPlaneHeight","tjPlaneSizeYUV","tjAlloc","tj3DecompressToYUVPlanes8","tjBufSizeYUV2","tjAlloc","tjAlloc","tjInitDecompress","tjDecodeYUV","tjDestroy","tjFree","tjFree","tjFree","tj3Destroy","tjFree"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 1) {
        return 0;
    }

    // Step 1: Create input file from data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 2: Load image using tjLoadImage
    int width, height, pixelFormat;
    int align = 1; // Default alignment
    unsigned char *srcBuf = tjLoadImage("input_file", &width, align, &height, &pixelFormat, 0);
    
    // If file loading fails, try loading from the memory file we created
    if (!srcBuf) {
        // Write data to actual file for tjLoadImage
        FILE *tmp_file = fopen("input_file", "wb");
        if (tmp_file) {
            fwrite(data, 1, size, tmp_file);
            assert_file_closed(&tmp_file);;
        }
        srcBuf = tjLoadImage("input_file", &width, align, &height, &pixelFormat, 0);
    }
    
    if (!srcBuf || width <= 0 || height <= 0) {
        assert_file_closed(&in_file);;
        if (srcBuf) tjFree(srcBuf);
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }

    // Step 3: Get scaling factors
    int numScalingFactors = 0;
    tjscalingfactor *scalingFactors = tjGetScalingFactors(&numScalingFactors);
    
    // Step 4: Calculate buffer size for compressed JPEG
    int jpegSubsamp = TJSAMP_420; // Default subsampling
    unsigned long jpegBufSize = tjBufSize(width, height, jpegSubsamp);
    
    // Step 5: Compress the loaded image using tjCompress2
    unsigned char *jpegBuf = nullptr;
    unsigned long jpegSize = 0;
    int jpegQual = 85; // Default quality
    int flags = 0;
    
    tjhandle compressHandle = tjInitCompress();
    if (!compressHandle) {
        tjFree(srcBuf);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    int compressResult = tjCompress2(compressHandle, srcBuf, width, 0, height, 
                                     pixelFormat, &jpegBuf, &jpegSize, 
                                     jpegSubsamp, jpegQual, flags);
    
    tjDestroy(compressHandle);
    tjFree(srcBuf);
    
    if (compressResult != 0 || !jpegBuf || jpegSize == 0) {
        if (jpegBuf) tjFree(jpegBuf);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }

    // Step 6: Decompress to YUV planes using new API
    tjhandle decompressHandle3 = tj3InitVersion(1, 0); // Decompressor
    if (!decompressHandle3) {
        tjFree(jpegBuf);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Get image info first
    int width3, height3, jpegSubsamp3, jpegColorspace3;
    if (tjDecompressHeader3(decompressHandle3, jpegBuf, jpegSize, 
                            &width3, &height3, &jpegSubsamp3, &jpegColorspace3) != 0) {
        tj3Destroy(decompressHandle3);
        tjFree(jpegBuf);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Prepare destination planes
    const int MAX_PLANES = 3;
    unsigned char *dstPlanes[MAX_PLANES] = {nullptr, nullptr, nullptr};
    int strides[MAX_PLANES] = {0, 0, 0};
    
    // Allocate plane buffers
    for (int i = 0; i < MAX_PLANES; i++) {
        strides[i] = tjPlaneWidth(i, width3, jpegSubsamp3);
        if (strides[i] <= 0) {
            strides[i] = 1;
        }
        int planeHeight = tjPlaneHeight(i, height3, jpegSubsamp3);
        if (planeHeight <= 0) {
            planeHeight = 1;
        }
        unsigned long planeSize = tjPlaneSizeYUV(i, width3, strides[i], planeHeight, jpegSubsamp3);
        if (planeSize == 0) {
            planeSize = width3 * height3; // Fallback
        }
        dstPlanes[i] = (unsigned char *)tjAlloc(planeSize);
        if (!dstPlanes[i]) {
            // Cleanup allocated planes
            for (int j = 0; j < i; j++) {
                if (dstPlanes[j]) tjFree(dstPlanes[j]);
            }
            tj3Destroy(decompressHandle3);
            tjFree(jpegBuf);
            assert_file_closed(&in_file);;
            assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
        }
    }
    
    // Decompress to YUV planes
    if (tj3DecompressToYUVPlanes8(decompressHandle3, jpegBuf, jpegSize, 
                                  dstPlanes, strides) != 0) {
        for (int i = 0; i < MAX_PLANES; i++) {
            if (dstPlanes[i]) tjFree(dstPlanes[i]);
        }
        tj3Destroy(decompressHandle3);
        tjFree(jpegBuf);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Step 7: Decode YUV back to RGB using tjDecodeYUV
    // Create a contiguous YUV buffer from planes
    int yuvAlign = 4;
    unsigned long yuvBufSize = tjBufSizeYUV2(width3, yuvAlign, height3, jpegSubsamp3);
    if (yuvBufSize == 0) {
        yuvBufSize = width3 * height3 * 3; // Fallback
    }
    
    unsigned char *yuvBuf = (unsigned char *)tjAlloc(yuvBufSize);
    if (!yuvBuf) {
        for (int i = 0; i < MAX_PLANES; i++) {
            if (dstPlanes[i]) tjFree(dstPlanes[i]);
        }
        tj3Destroy(decompressHandle3);
        tjFree(jpegBuf);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Convert planes to contiguous buffer (simplified - actual conversion depends on subsampling)
    // For simplicity, just copy first plane
    if (dstPlanes[0]) {
        int ySize = width3 * height3;
        if (ySize > (int)yuvBufSize) ySize = yuvBufSize;
        memcpy(yuvBuf, dstPlanes[0], ySize);
    }
    
    // Decode YUV to RGB
    int pitch = width3 * 3; // RGB pitch
    unsigned char *rgbBuf = (unsigned char *)tjAlloc(pitch * height3);
    if (!rgbBuf) {
        tjFree(yuvBuf);
        for (int i = 0; i < MAX_PLANES; i++) {
            if (dstPlanes[i]) tjFree(dstPlanes[i]);
        }
        tj3Destroy(decompressHandle3);
        tjFree(jpegBuf);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    tjhandle decodeHandle = tjInitDecompress();
    if (!decodeHandle) {
        tjFree(rgbBuf);
        tjFree(yuvBuf);
        for (int i = 0; i < MAX_PLANES; i++) {
            if (dstPlanes[i]) tjFree(dstPlanes[i]);
        }
        tj3Destroy(decompressHandle3);
        tjFree(jpegBuf);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    int decodeResult = tjDecodeYUV(decodeHandle, yuvBuf, yuvAlign, jpegSubsamp3,
                                   rgbBuf, width3, pitch, height3, TJPF_RGB, 0);
    
    tjDestroy(decodeHandle);
    
    // Step 8: Cleanup all resources
    tjFree(rgbBuf);
    tjFree(yuvBuf);
    for (int i = 0; i < MAX_PLANES; i++) {
        if (dstPlanes[i]) tjFree(dstPlanes[i]);
    }
    tj3Destroy(decompressHandle3);
    tjFree(jpegBuf);
    assert_file_closed(&in_file);;
    
    // Cleanup temporary files
    remove("input_file");
    
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}