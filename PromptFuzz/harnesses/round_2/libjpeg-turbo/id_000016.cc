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
//<ID> 198
//<Prompt> []
/*<Combination>: [
*/
//<score> 60, nr_unique_branch: 1
//<Quality> {"density":30,"unique_branches":{"tj3Destroy":[[641,7,641,12,0,0,4,0]]},"library_calls":["tjLoadImage","tjFree","tjGetScalingFactors","tjInitCompress","tjInitDecompress","tj3InitVersion","tjFree","tjDestroy","tjDestroy","tj3Destroy","tjBufSize","tjPlaneWidth","tjPlaneHeight","tjPlaneSizeYUV","tjAlloc","tj3Compress12","tj3Free","tjCompress2","tjPlaneWidth","tjAlloc","tjPlaneSizeYUV","tj3DecompressToYUVPlanes8","tjFree","tjAlloc","tjDecodeYUV","tjFree","tj3TransformBufSize","tjFree","tjFree","tjFree","tjFree","tjDestroy","tjDestroy","tj3Destroy"],"critical_calls":["tjLoadImage","tjGetScalingFactors","tjInitCompress","tjInitDecompress","tj3InitVersion","tjBufSize","tjPlaneWidth","tjPlaneHeight","tjPlaneSizeYUV","tjAlloc","tj3Compress12","tj3Free","tjCompress2","tjPlaneWidth","tjAlloc","tjPlaneSizeYUV","tj3DecompressToYUVPlanes8","tjFree","tjAlloc","tjDecodeYUV","tjFree","tj3TransformBufSize","tjFree","tjFree","tjFree","tjFree","tjDestroy","tjDestroy","tj3Destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Create a temporary file to store input data for tjLoadImage
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // For output files
    FILE *out_file1 = fopen("output_file1", "wb");
    FILE *out_file2 = fopen("output_file2", "wb");
    if (!out_file1 || !out_file2) {
        assert_file_closed(&in_file);;
        if (out_file1) assert_file_closed(&out_file1);;
        if (out_file2) assert_file_closed(&out_file2);;
        assert_file_closed(&out_file2);
	assert_file_closed(&out_file1);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Write input data to a file for tjLoadImage
    const char *input_filename = "input_file";
    FILE *temp_input = fopen(input_filename, "wb");
    if (temp_input) {
        fwrite(data, 1, size, temp_input);
        assert_file_closed(&temp_input);;
    }
    
    // Event: Load an image, process it through various compression/decompression
    // and transformation APIs, and save the results
    
    // 1. Load image using tjLoadImage
    int width = 0, height = 0, pixelFormat = 0;
    unsigned char *loadedImage = tjLoadImage(input_filename, &width, 4, &height, &pixelFormat, 0);
    if (!loadedImage || width <= 0 || height <= 0) {
        // Cleanup and return
        remove(input_filename);
        remove("output_file1");
        remove("output_file2");
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file1);;
        assert_file_closed(&out_file2);;
        if (loadedImage) tjFree(loadedImage);
        assert_file_closed(&temp_input);
	assert_file_closed(&out_file2);
	assert_file_closed(&out_file1);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Calculate pitch (bytes per row)
    int pitch = width * tjPixelSize[pixelFormat];
    
    // 2. Get scaling factors
    int numScalingFactors = 0;
    tjscalingfactor *scalingFactors = tjGetScalingFactors(&numScalingFactors);
    
    // 3. Initialize handles
    tjhandle compHandleOld = tjInitCompress();
    tjhandle decompHandleOld = tjInitDecompress();
    tjhandle handle3 = tj3InitVersion(TJINIT_COMPRESS, 300);
    
    if (!compHandleOld || !decompHandleOld || !handle3) {
        // Cleanup
        tjFree(loadedImage);
        tjDestroy(compHandleOld);
        tjDestroy(decompHandleOld);
        tj3Destroy(handle3);
        remove(input_filename);
        remove("output_file1");
        remove("output_file2");
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file1);;
        assert_file_closed(&out_file2);;
        assert_file_closed(&temp_input);
	assert_file_closed(&out_file2);
	assert_file_closed(&out_file1);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // 4. Calculate buffer sizes
    unsigned long bufSize = tjBufSize(width, height, TJSAMP_444);
    int planeWidth = tjPlaneWidth(0, width, TJSAMP_444);
    int planeHeight = tjPlaneHeight(0, height, TJSAMP_444);
    unsigned long planeSize = tjPlaneSizeYUV(0, width, pitch, height, TJSAMP_444);
    
    // 5. Allocate buffers
    unsigned char *jpegBuf1 = nullptr;
    unsigned long jpegSize1 = 0;
    unsigned char *jpegBuf2 = nullptr;
    unsigned long jpegSize2 = 0;
    unsigned char *yuvBuf = (unsigned char *)tjAlloc(planeSize);
    
    // 6. Convert 8-bit image to 12-bit for tj3Compress12
    // Allocate 12-bit buffer (12-bit samples stored in 16-bit shorts)
    size_t src12Size = width * height * 3; // RGB
    std::vector<short> src12Buffer(src12Size);
    if (!src12Buffer.empty()) {
        // Scale 8-bit values to 12-bit (0-4095)
        for (size_t i = 0; i < src12Size && i < width * height * 3; i++) {
            src12Buffer[i] = loadedImage[i] << 4; // Scale to 12-bit
        }
        
        // Use tj3Compress12
        unsigned char *jpegBuf12 = nullptr;
        size_t jpegSize12 = 0;
        if (tj3Compress12(handle3, src12Buffer.data(), width, pitch, height, 
                         pixelFormat, &jpegBuf12, &jpegSize12) == 0) {
            // Save compressed 12-bit image
            if (jpegBuf12 && jpegSize12 > 0) {
                fwrite(jpegBuf12, 1, jpegSize12, out_file1);
            }
            if (jpegBuf12) tj3Free(jpegBuf12);
        }
    }
    
    // 7. Use old API tjCompress2
    if (tjCompress2(compHandleOld, loadedImage, width, pitch, height, 
                   pixelFormat, &jpegBuf1, &jpegSize1, TJSAMP_444, 90, 0) == 0) {
        // Save compressed image
        if (jpegBuf1 && jpegSize1 > 0) {
            fwrite(jpegBuf1, 1, jpegSize1, out_file2);
        }
    }
    
    // 8. Decompress to YUV planes using new API
    if (jpegBuf1 && jpegSize1 > 0) {
        unsigned char *yuvPlanes[3] = {nullptr, nullptr, nullptr};
        int strides[3] = {0, 0, 0};
        
        // Allocate plane buffers
        for (int i = 0; i < 3; i++) {
            strides[i] = tjPlaneWidth(i, width, TJSAMP_444);
            yuvPlanes[i] = (unsigned char *)tjAlloc(tjPlaneSizeYUV(i, width, strides[i], height, TJSAMP_444));
        }
        
        // Decompress to YUV planes
        if (tj3DecompressToYUVPlanes8(handle3, jpegBuf1, jpegSize1, yuvPlanes, strides) == 0) {
            // Process YUV planes if needed
        }
        
        // Cleanup plane buffers
        for (int i = 0; i < 3; i++) {
            if (yuvPlanes[i]) tjFree(yuvPlanes[i]);
        }
    }
    
    // 9. Decode YUV using old API
    if (yuvBuf) {
        unsigned char *rgbBuf = (unsigned char *)tjAlloc(width * height * 3);
        if (rgbBuf) {
            // Note: We need actual YUV data here, but we'll use what we have
            if (tjDecodeYUV(decompHandleOld, yuvBuf, 4, TJSAMP_444, 
                           rgbBuf, width, pitch, height, TJPF_RGB, 0) == 0) {
                // Successfully decoded
            }
            tjFree(rgbBuf);
        }
    }
    
    // 10. Calculate transform buffer size
    tjtransform transform;
    memset(&transform, 0, sizeof(transform));
    transform.r.x = 0;
    transform.r.y = 0;
    transform.r.w = width / 2;
    transform.r.h = height / 2;
    transform.op = TJXOP_NONE;
    
    size_t transformBufSize = tj3TransformBufSize(handle3, &transform);
    
    // 11. Cleanup
    tjFree(loadedImage);
    if (jpegBuf1) tjFree(jpegBuf1);
    if (jpegBuf2) tjFree(jpegBuf2);
    if (yuvBuf) tjFree(yuvBuf);
    
    tjDestroy(compHandleOld);
    tjDestroy(decompHandleOld);
    tj3Destroy(handle3);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file1);;
    assert_file_closed(&out_file2);;
    
    // Remove temporary files
    remove(input_filename);
    remove("output_file1");
    remove("output_file2");
    
    assert_file_closed(&temp_input);
	assert_file_closed(&out_file2);
	assert_file_closed(&out_file1);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}