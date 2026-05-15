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
//<ID> 705
//<Prompt> []
/*<Combination>: [
*/
//<score> 39, nr_unique_branch: 12
//<Quality> {"density":13,"unique_branches":{"tj3GetErrorStr":[[682,15,682,36,0,0,4,1]],"tj3Compress16":[[87,7,87,35,0,0,4,1],[90,7,90,21,0,0,4,1],[90,25,90,35,0,0,4,1],[90,39,90,48,0,0,4,1],[90,52,90,63,0,0,4,1],[91,7,91,22,0,0,4,1],[91,26,91,49,0,0,4,1],[91,53,91,68,0,0,4,1],[92,7,92,23,0,0,4,1],[95,7,95,22,0,0,4,0],[95,26,95,45,0,0,4,0]]},"library_calls":["tj3InitVersion","tjInitCompress","tjInitDecompress","tj3LoadImage16","tj3GetErrorStr","tj3Compress16","tj3Alloc","tj3EncodeYUVPlanes8","tjCompress2","tjDecompressHeader","tj3Alloc","tj3YUVPlaneSize","tjDecompressToYUVPlanes","tj3Destroy","tjDestroy","tjDestroy","tj3Free","tj3Free","tjFree","tj3Free","tj3Free"],"critical_calls":["tj3InitVersion","tjInitCompress","tjInitDecompress","tj3LoadImage16","tj3Compress16","tj3Alloc","tj3EncodeYUVPlanes8","tjCompress2","tjDecompressHeader","tj3Alloc","tj3YUVPlaneSize","tjDecompressToYUVPlanes","tj3Destroy","tjDestroy","tjDestroy","tj3Free","tj3Free","tjFree","tj3Free","tj3Free"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t* f_data, size_t f_size) {
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
        return 0;  // Insufficient data
    }

    tjhandle handle_new = nullptr;
    tjhandle handle_old_compress = nullptr;
    tjhandle handle_old_decompress = nullptr;
    
    FILE *in_file = nullptr;
    FILE *out_file = nullptr;
    unsigned short *img16 = nullptr;
    unsigned char *jpegBuf1 = nullptr;
    unsigned char *jpegBuf2 = nullptr;
    unsigned char *yuvPlanes[3] = {nullptr, nullptr, nullptr};
    int strides[3] = {0, 0, 0};
    unsigned char *src8 = nullptr;
    
    size_t jpegSize1 = 0;
    unsigned long jpegSize2 = 0;
    
    int width = 0, height = 0, pixelFormat = 0;
    int align = 4;
    int pitch = 0;
    int jpegSubsamp = TJSAMP_420;
    int jpegQual = 85;
    int flags = 0;
    
    // Create a temporary input file with the provided data
    in_file = fopen("input_file", "wb");
    if (!in_file) {
        goto cleanup;
    }
    fwrite(data, 1, size, in_file);
    assert_file_closed(&in_file);;
    in_file = nullptr;
    
    // Initialize handles
    handle_new = tj3Init(TJINIT_COMPRESS);
    handle_old_compress = tjInitCompress();
    handle_old_decompress = tjInitDecompress();
    
    if (!handle_new || !handle_old_compress || !handle_old_decompress) {
        goto cleanup;
    }
    
    // Step 1: Load 16-bit image from the temporary file
    img16 = tj3LoadImage16(handle_new, "input_file", &width, align, &height, &pixelFormat);
    if (!img16) {
        const char *error = tj3GetErrorStr(handle_new);
        (void)error; // Could log error if needed
        goto cleanup;
    }
    
    pitch = width * tjPixelSize[pixelFormat];
    
    // Step 2: Compress the 16-bit image to JPEG
    if (tj3Compress16(handle_new, img16, width, pitch, height, pixelFormat, 
                      &jpegBuf1, &jpegSize1) != 0) {
        goto cleanup;
    }
    
    // Step 3: Create an 8-bit source buffer (simple conversion from 16-bit)
    src8 = (unsigned char *)tj3Alloc(width * height * tjPixelSize[pixelFormat]);
    if (!src8) {
        goto cleanup;
    }
    // Simple conversion: take upper 8 bits of 16-bit samples
    for (size_t i = 0; i < (size_t)(width * height * tjPixelSize[pixelFormat]); i++) {
        src8[i] = (unsigned char)(img16[i] >> 8);
    }
    
    // Step 4: Encode the 8-bit image to YUV planes
    if (tj3EncodeYUVPlanes8(handle_new, src8, width, pitch, height, pixelFormat, 
                            yuvPlanes, strides) != 0) {
        goto cleanup;
    }
    
    // Step 5: Compress the 8-bit image using the old API
    if (tjCompress2(handle_old_compress, src8, width, pitch, height, pixelFormat,
                    &jpegBuf2, &jpegSize2, jpegSubsamp, jpegQual, flags) != 0) {
        goto cleanup;
    }
    
    // Step 6: Decompress the first JPEG to YUV planes using the old API
    // We need to set width/height for the destination
    int decompWidth, decompHeight;
    if (tjDecompressHeader(handle_old_decompress, jpegBuf1, jpegSize1, 
                           &decompWidth, &decompHeight) != 0) {
        goto cleanup;
    }
    
    // Allocate YUV plane buffers for decompression
    for (int i = 0; i < 3; i++) {
        yuvPlanes[i] = (unsigned char *)tj3Alloc(tj3YUVPlaneSize(i, decompWidth, 
                                                   strides[i], decompHeight, jpegSubsamp));
        if (!yuvPlanes[i]) {
            goto cleanup;
        }
    }
    
    if (tjDecompressToYUVPlanes(handle_old_decompress, jpegBuf1, jpegSize1, 
                                yuvPlanes, decompWidth, strides, decompHeight, flags) != 0) {
        goto cleanup;
    }
    
    // Create an output file (optional - can be used for debugging)
    out_file = fopen("output_file", "wb");
    if (out_file) {
        fwrite(jpegBuf1, 1, jpegSize1, out_file);
        assert_file_closed(&out_file);;
        out_file = nullptr;
    }
    
cleanup:
    // Free allocated resources
    if (handle_new) {
        tj3Destroy(handle_new);
    }
    if (handle_old_compress) {
        tjDestroy(handle_old_compress);
    }
    if (handle_old_decompress) {
        tjDestroy(handle_old_decompress);
    }
    
    if (img16) {
        tj3Free(img16);
    }
    if (jpegBuf1) {
        tj3Free(jpegBuf1);
    }
    if (jpegBuf2) {
        tjFree(jpegBuf2);
    }
    for (int i = 0; i < 3; i++) {
        if (yuvPlanes[i]) {
            tj3Free(yuvPlanes[i]);
        }
    }
    if (src8) {
        tj3Free(src8);
    }
    if (in_file) {
        assert_file_closed(&in_file);;
    }
    if (out_file) {
        assert_file_closed(&out_file);;
    }
    
    // Remove temporary files
    remove("input_file");
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}