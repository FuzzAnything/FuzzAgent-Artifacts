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
//<ID> 163
//<Prompt> []
/*<Combination>: [
*/
//<score> 19.333334, nr_unique_branch: 3
//<Quality> {"density":29,"unique_branches":{"tj3GetICCProfile":[[1984,7,1984,30,0,0,4,1],[1994,7,1994,21,0,0,4,1],[1996,7,1996,15,0,0,4,1]]},"library_calls":["tj3InitVersion","tjInitCompress","tj3Destroy","tjDestroy","tj3LoadImage16","tj3Destroy","tjDestroy","tj3Alloc","tj3Free","tj3Destroy","tjDestroy","tj3YUVBufSize","tj3Alloc","tj3Free","tj3Free","tj3Destroy","tjDestroy","tj3EncodeYUV8","tj3EncodeYUV8","tjCompressFromYUV","tj3YUVPlaneWidth","tj3GetICCProfile","tjFree","tj3Free","tj3Free","tj3Free","tj3Free","tj3Destroy","tjDestroy"],"critical_calls":["tj3InitVersion","tjInitCompress","tj3LoadImage16","tj3Alloc","tj3YUVBufSize","tj3Alloc","tj3EncodeYUV8","tj3EncodeYUV8","tjCompressFromYUV","tj3YUVPlaneWidth","tj3GetICCProfile","tjFree","tj3Free","tj3Free","tj3Free","tj3Free","tj3Destroy","tjDestroy"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* f_data, size_t f_size) {
	if(f_size<72) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_13, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_14, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_15, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_16, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_17, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_18, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Create a temporary file with the input data
    FILE *in_file = fopen("input_file", "wb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    fwrite(data, 1, size, in_file);
    assert_file_closed(&in_file);;
    
    // Initialize handles for both old and new APIs
    tjhandle handle_new = tj3Init(TJINIT_COMPRESS);
    tjhandle handle_old = tjInitCompress();
    
    if (!handle_new || !handle_old) {
        if (handle_new) tj3Destroy(handle_new);
        if (handle_old) tjDestroy(handle_old);
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Try to load the image as 16-bit
    int width = 0, height = 0, pixelFormat = 0;
    unsigned short *image16 = tj3LoadImage16(handle_new, "input_file", &width, fuzz_int32_t_1, &height, &pixelFormat);
    
    if (!image16 || width <= 0 || height <= 0) {
        tj3Destroy(handle_new);
        tjDestroy(handle_old);
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Convert 16-bit image to 8-bit for tj3EncodeYUV8
    size_t pixelSize = 0;
    switch (pixelFormat) {
        case TJPF_GRAY: pixelSize = 1; break;
        case TJPF_RGB: pixelSize = 3; break;
        case TJPF_BGR: pixelSize = 3; break;
        case TJPF_RGBX: pixelSize = 4; break;
        case TJPF_BGRX: pixelSize = 4; break;
        case TJPF_XRGB: pixelSize = 4; break;
        case TJPF_XBGR: pixelSize = 4; break;
        case TJPF_RGBA: pixelSize = 4; break;
        case TJPF_BGRA: pixelSize = 4; break;
        case TJPF_ABGR: pixelSize = 4; break;
        case TJPF_ARGB: pixelSize = 4; break;
        case TJPF_CMYK: pixelSize = 4; break;
        default: pixelSize = 3; break;
    }
    
    unsigned char *image8 = (unsigned char *)tj3Alloc(width * height * pixelSize);
    if (!image8) {
        tj3Free(image16);
        tj3Destroy(handle_new);
        tjDestroy(handle_old);
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Simple conversion: take upper 8 bits of 16-bit samples
    for (size_t i = 0; i < width * height * pixelSize; i++) {
        image8[i] = (unsigned char)(image16[i] >> 8);
    }
    
    // Encode to YUV using tj3EncodeYUV8
    int align = 1;
    size_t yuvBufSize = tj3YUVBufSize(fuzz_int32_t_2, fuzz_int32_t_3, fuzz_int32_t_4, TJSAMP_420);
    unsigned char *yuvBuf = (unsigned char *)tj3Alloc(yuvBufSize);
    
    if (!yuvBuf) {
        tj3Free(image8);
        tj3Free(image16);
        tj3Destroy(handle_new);
        tjDestroy(handle_old);
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }
    
    int result = tj3EncodeYUV8(handle_new, image8, fuzz_int32_t_5, width * pixelSize, fuzz_int32_t_6, 
                               fuzz_int32_t_7, yuvBuf, fuzz_int32_t_8);
    
    if (result != 0) {
        // Try with a different pixel format if the first one fails
        result = tj3EncodeYUV8(handle_new, image8, fuzz_int32_t_9, width * pixelSize, fuzz_int32_t_10, 
                               TJPF_RGB, yuvBuf, fuzz_int32_t_11);
    }
    
    // Compress from YUV using old API
    unsigned char *jpegBuf = NULL;
    unsigned long jpegSize = 0;
    
    if (result == 0) {
        tjCompressFromYUV(handle_old, yuvBuf, fuzz_int32_t_12, fuzz_int32_t_13, fuzz_int32_t_14, TJSAMP_420, 
                         &jpegBuf, &jpegSize, fuzz_int32_t_15, fuzz_int32_t_16);
    }
    
    // Get YUV plane width
    int yPlaneWidth = tj3YUVPlaneWidth(fuzz_int32_t_17, fuzz_int32_t_18, TJSAMP_420);
    
    // Get ICC profile
    unsigned char *iccBuf = NULL;
    size_t iccSize = 0;
    tj3GetICCProfile(handle_new, &iccBuf, &iccSize);
    
    // Cleanup
    if (jpegBuf) tjFree(jpegBuf);
    if (iccBuf) tj3Free(iccBuf);
    tj3Free(yuvBuf);
    tj3Free(image8);
    tj3Free(image16);
    tj3Destroy(handle_new);
    tjDestroy(handle_old);
    remove("input_file");
    
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}