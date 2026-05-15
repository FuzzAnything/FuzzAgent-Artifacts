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
//<ID> 428
//<Prompt> []
/*<Combination>: [
*/
//<score> 72, nr_unique_branch: 3
//<Quality> {"density":24,"unique_branches":{"tj3TransformBufSize":[[340,7,340,12,1,0,4,0]],"tjBufSize":[[996,7,996,22,0,0,4,1],[1002,10,1002,23,0,0,4,1]]},"library_calls":["tjInitDecompress","tjDestroy","tjDecompressHeader2","tjDestroy","tjInitCompress","tjBufSize","tjDestroy","tjDestroy","tjCompress2","tjFree","tjDestroy","tjInitTransform","tjFree","tjDestroy","tj3TransformBufSize","tj3InitVersion","tjFree","tjDestroy","tjDestroy","tjTransform","tjFree","tjFree","tjDestroy","tjDestroy"],"critical_calls":["tjInitDecompress","tjDecompressHeader2","tjDestroy","tjInitCompress","tjBufSize","tjCompress2","tjInitTransform","tj3TransformBufSize","tj3InitVersion","tjTransform","tjFree","tjFree","tjDestroy","tjDestroy"],"visited":0}
/**/



extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t* f_data, size_t f_size) {
	if(f_size<28) return 0;

	
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
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Step 1: Try to decompress header from input data
    tjhandle decomp_handle = tjInitDecompress();
    if (!decomp_handle) {
        return 0;
    }

    int width = 0, height = 0, jpegSubsamp = 0;
    unsigned long jpegSize = static_cast<unsigned long>(size);
    
    // We need to copy data to non-const buffer for tjDecompressHeader2
    unsigned char* jpegBuf = static_cast<unsigned char*>(malloc(size));
    if (!jpegBuf) {
        tjDestroy(decomp_handle);
        return 0;
    }
    memcpy(jpegBuf, data, size);

    int header_result = tjDecompressHeader2(decomp_handle, jpegBuf, jpegSize, 
                                           &width, &height, &jpegSubsamp);
    free(jpegBuf);
    
    // If header decompression failed, use reasonable defaults
    if (header_result != 0 || width <= 0 || height <= 0) {
        width = 64;
        height = 64;
        jpegSubsamp = TJSAMP_420;
    }

    tjDestroy(decomp_handle);

    // Step 2: Create compressor handle and set parameters
    tjhandle comp_handle = tjInitCompress();
    if (!comp_handle) {
        return 0;
    }

    // Calculate buffer size for JPEG compression
    unsigned long bufSize = tjBufSize(fuzz_int32_t_1, fuzz_int32_t_2, fuzz_int32_t_3);
    if (bufSize == 0) {
        tjDestroy(comp_handle);
        return 0;
    }

    // Step 3: Create source image buffer and compress it
    int pitch = width * 3;  // RGB image
    unsigned char* srcBuf = static_cast<unsigned char*>(malloc(height * pitch));
    if (!srcBuf) {
        tjDestroy(comp_handle);
        return 0;
    }

    // Fill source buffer with pattern using input data
    for (int i = 0; i < height * pitch; i++) {
        srcBuf[i] = static_cast<unsigned char>(data[i % size] ^ (i & 0xFF));
    }

    unsigned char* compressedBuf = nullptr;
    unsigned long compressedSize = 0;
    
    int compress_result = tjCompress2(comp_handle, srcBuf, width, pitch, height,
                                     TJPF_RGB, &compressedBuf, &compressedSize,
                                     fuzz_int32_t_4, fuzz_int32_t_5, fuzz_int32_t_6);
    
    free(srcBuf);
    
    if (compress_result != 0 || !compressedBuf || compressedSize == 0) {
        if (compressedBuf) tjFree(compressedBuf);
        tjDestroy(comp_handle);
        return 0;
    }

    // Step 4: Prepare for transform operation
    tjhandle trans_handle = tjInitTransform();
    if (!trans_handle) {
        tjFree(compressedBuf);
        tjDestroy(comp_handle);
        return 0;
    }

    // Create transform structure
    tjtransform transform;
    transform.r.x = 0;
    transform.r.y = 0;
    transform.r.w = width / 2;
    transform.r.h = height / 2;
    transform.op = TJXOP_NONE;
    transform.options = 0;
    transform.data = nullptr;
    transform.customFilter = nullptr;

    // Calculate transform buffer size
    size_t transformBufSize = tj3TransformBufSize(tj3InitVersion(TJINIT_TRANSFORM, 0), &transform);
    if (transformBufSize == 0) {
        tjFree(compressedBuf);
        tjDestroy(comp_handle);
        tjDestroy(trans_handle);
        return 0;
    }

    // Step 5: Perform the transform
    unsigned char* dstBuf = nullptr;
    unsigned long dstSize = 0;
    
    int transform_result = tjTransform(trans_handle, compressedBuf, compressedSize,
                                       1, &dstBuf, &dstSize, &transform, fuzz_int32_t_7);

    // Step 6: Cleanup
    if (dstBuf) tjFree(dstBuf);
    tjFree(compressedBuf);
    tjDestroy(trans_handle);
    tjDestroy(comp_handle);

    return 0;
}