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
//<ID> 489
//<Prompt> []
/*<Combination>: [
*/
//<score> 75, nr_unique_branch: 10
//<Quality> {"density":25,"unique_branches":{"tjCompressFromYUVPlanes":[[1461,7,1461,18,0,0,4,1],[1461,22,1461,46,0,0,4,1],[1461,50,1461,66,0,0,4,1],[1462,7,1462,19,0,0,4,1],[1462,23,1462,37,0,0,4,1],[1470,7,1470,22,0,0,4,1],[340,7,340,12,1,0,4,1]],"tj3GetICCProfile":[[1984,7,1984,30,0,0,4,0],[1985,9,1985,28,0,0,4,0],[1986,11,1986,17,0,0,4,0]]},"library_calls":["tj3InitVersion","tjDecompressHeader3","tj3Set","tj3Set","tj3JPEGBufSize","tj3GetICCProfile","tj3Alloc","tj3Alloc","tj3YUVBufSize","tj3Alloc","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tj3YUVPlaneSize","tj3Alloc","tj3Compress12","tj3CompressFromYUVPlanes8","tj3SaveImage16","tjCompressFromYUV","tjCompressFromYUVPlanes","tjFree","tj3Free","tj3Free","tj3Free","tj3Free","tj3Free","tj3Free","tjFree","tj3Free","tj3Destroy"],"critical_calls":["tj3InitVersion","tjDecompressHeader3","tj3Set","tj3Set","tj3JPEGBufSize","tj3GetICCProfile","tj3Alloc","tj3Alloc","tj3YUVBufSize","tj3Alloc","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tj3YUVPlaneSize","tj3Alloc","tj3Compress12","tj3CompressFromYUVPlanes8","tj3SaveImage16","tjCompressFromYUV","tjCompressFromYUVPlanes","tjFree","tj3Free","tj3Free","tj3Free","tj3Free","tj3Free","tj3Free","tjFree","tj3Free","tj3Destroy"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t* f_data, size_t f_size) {
	if(f_size<188) return 0;

	
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
		FDPConsumeIntegral(int32_t, fuzz_int32_t_19, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_20, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_21, fdp);
		FDPConsumeIntegralArray(int32_t, fuzz_int32_t_array_22, fuzz_int32_t_array_size_22, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_23, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_24, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_25, fuzz_str_sz_25, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_26, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_27, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_28, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_29, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_30, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_31, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_32, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_33, fdp);
		FDPConsumeIntegralArray(int32_t, fuzz_int32_t_array_34, fuzz_int32_t_array_size_34, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_35, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_36, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_37, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_38, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_39, fdp);
	
		const int * fuzzer_var_tj3CompressFromYUVPlanes8_15_3 = fuzz_int32_t_array_22;
		const int * fuzzer_var_tjCompressFromYUVPlanes_18_3 = fuzz_int32_t_array_34;//fuzzer shim end}




    // Create a file handle from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize decompressor to read JPEG header
    tjhandle handle = tj3Init(TJINIT_DECOMPRESS);
    if (!handle) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Read the entire input into a buffer for JPEG operations
    unsigned char *jpegBuf = (unsigned char *)data;
    size_t jpegSize = size;

    // Try to decompress header first to get image dimensions
    int width = 0, height = 0, jpegSubsamp = 0, jpegColorspace = 0;
    int headerResult = tjDecompressHeader3(handle, jpegBuf, (unsigned long)jpegSize,
                                           &width, &height, &jpegSubsamp, &jpegColorspace);

    // If header decompression failed, try with simpler function
    if (headerResult != 0) {
        width = 64;  // Default dimensions if we can't determine from header
        height = 64;
        jpegSubsamp = TJSAMP_420;
    }

    // Set some parameters on the handle
    tj3Set(handle, TJPARAM_FASTUPSAMPLE, fuzz_int32_t_1);
    tj3Set(handle, TJPARAM_FASTDCT, fuzz_int32_t_2);

    // Get JPEG buffer size for potential compression
    size_t bufSize = tj3JPEGBufSize(fuzz_int32_t_3, fuzz_int32_t_4, fuzz_int32_t_5);

    // Try to get ICC profile if present
    unsigned char *iccBuf = nullptr;
    size_t iccSize = 0;
    tj3GetICCProfile(handle, &iccBuf, &iccSize);

    // Create source buffers for different operations
    // For 12-bit compression
    short *src12Buf = nullptr;
    size_t src12Size = width * height * 3 * sizeof(short);  // RGB, 12-bit per component
    if (size >= src12Size) {
        src12Buf = (short *)tj3Alloc(src12Size);
        if (src12Buf) {
            // Use input data for 12-bit source
            memcpy(src12Buf, data, std::min(size, src12Size));
        }
    }

    // For 16-bit image saving
    unsigned short *src16Buf = nullptr;
    size_t src16Size = width * height * 3 * sizeof(unsigned short);  // RGB, 16-bit
    if (size >= src16Size) {
        src16Buf = (unsigned short *)tj3Alloc(src16Size);
        if (src16Buf) {
            memcpy(src16Buf, data, std::min(size, src16Size));
        }
    }

    // Create YUV buffers
    int align = 4;
    size_t yuvBufSize = tj3YUVBufSize(fuzz_int32_t_6, fuzz_int32_t_7, fuzz_int32_t_8, fuzz_int32_t_9);
    unsigned char *yuvBuf = nullptr;
    if (yuvBufSize > 0 && size >= yuvBufSize) {
        yuvBuf = (unsigned char *)tj3Alloc(yuvBufSize);
        if (yuvBuf) {
            memcpy(yuvBuf, data, std::min(size, yuvBufSize));
        }
    }

    // Create YUV planes
    const unsigned char *srcPlanes[3] = {nullptr, nullptr, nullptr};
    int strides[3] = {0, 0, 0};
    
    if (yuvBuf) {
        // Calculate plane sizes and strides
        for (int i = 0; i < 3; i++) {
            int planeWidth = tj3YUVPlaneWidth(fuzz_int32_t_10, fuzz_int32_t_11, fuzz_int32_t_12);
            int planeHeight = tj3YUVPlaneHeight(fuzz_int32_t_13, fuzz_int32_t_14, fuzz_int32_t_15);
            strides[i] = planeWidth;
            size_t planeSize = tj3YUVPlaneSize(fuzz_int32_t_16, fuzz_int32_t_17, strides[i], fuzz_int32_t_18, fuzz_int32_t_19);
            
            if (planeSize > 0) {
                srcPlanes[i] = (const unsigned char *)tj3Alloc(planeSize);
                if (srcPlanes[i]) {
                    size_t offset = i * planeSize;
                    if (offset + planeSize <= size) {
                        memcpy((void *)srcPlanes[i], data + offset, planeSize);
                    }
                }
            }
        }
    }

    // Output buffers
    unsigned char *jpegOutBuf = nullptr;
    size_t jpegOutSize = 0;
    unsigned char *jpegOutBuf2 = nullptr;
    unsigned long jpegOutSize2 = 0;
    unsigned char *jpegOutBuf3 = nullptr;
    unsigned long jpegOutSize3 = 0;

    // 1. Try 12-bit compression if we have source data
    if (src12Buf && width > 0 && height > 0) {
        tj3Compress12(handle, src12Buf, fuzz_int32_t_20, width * 3 * sizeof(short),
                      fuzz_int32_t_21, TJPF_RGB, &jpegOutBuf, &jpegOutSize);
    }

    // 2. Try compression from YUV planes (8-bit)
    if (srcPlanes[0] && srcPlanes[1] && srcPlanes[2] && width > 0 && height > 0) {
        tj3CompressFromYUVPlanes8(handle, srcPlanes, fuzz_int32_t_23, fuzzer_var_tj3CompressFromYUVPlanes8_15_3,
                                  fuzz_int32_t_24, &jpegOutBuf2, &jpegOutSize2);
    }

    // 3. Save 16-bit image if we have source data
    if (src16Buf && width > 0 && height > 0) {
        tj3SaveImage16(handle, fuzz_str_25, src16Buf, fuzz_int32_t_26,
                       width * 3 * sizeof(unsigned short), fuzz_int32_t_27, TJPF_RGB);
    }

    // 4. Try compression from YUV using older API
    if (yuvBuf && width > 0 && height > 0) {
        tjCompressFromYUV(handle, yuvBuf, fuzz_int32_t_28, fuzz_int32_t_29, fuzz_int32_t_30,
                          fuzz_int32_t_31, &jpegOutBuf3, &jpegOutSize3,
                          fuzz_int32_t_32, fuzz_int32_t_33);
    }

    // 5. Try compression from YUV planes using older API
    if (srcPlanes[0] && srcPlanes[1] && srcPlanes[2] && width > 0 && height > 0) {
        unsigned char *jpegOutBuf4 = nullptr;
        unsigned long jpegOutSize4 = 0;
        tjCompressFromYUVPlanes(handle, srcPlanes, fuzz_int32_t_35, fuzzer_var_tjCompressFromYUVPlanes_18_3,
                                fuzz_int32_t_36, fuzz_int32_t_37, &jpegOutBuf4, &jpegOutSize4,
                                fuzz_int32_t_38, fuzz_int32_t_39);
        if (jpegOutBuf4) {
            tjFree(jpegOutBuf4);
        }
    }

    // Cleanup
    if (src12Buf) tj3Free(src12Buf);
    if (src16Buf) tj3Free(src16Buf);
    if (yuvBuf) tj3Free(yuvBuf);
    
    for (int i = 0; i < 3; i++) {
        if (srcPlanes[i]) tj3Free((void *)srcPlanes[i]);
    }
    
    if (jpegOutBuf) tj3Free(jpegOutBuf);
    if (jpegOutBuf2) tj3Free(jpegOutBuf2);
    if (jpegOutBuf3) tjFree(jpegOutBuf3);
    
    if (iccBuf) tj3Free(iccBuf);
    
    tj3Destroy(handle);
    assert_file_closed(&in_file);;

    // Remove output file if created
    remove("output_file");

    assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}