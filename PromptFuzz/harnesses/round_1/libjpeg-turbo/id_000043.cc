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
//<ID> 963
//<Prompt> []
/*<Combination>: [
*/
//<score> 180, nr_unique_branch: 41
//<Quality> {"density":30,"unique_branches":{"tj3EncodeYUVPlanes8":[[1587,7,1587,20,0,0,4,1],[1589,7,1589,35,0,0,4,0],[1589,40,1589,53,0,0,4,1],[1589,57,1589,70,0,0,4,1],[1592,7,1592,38,0,0,4,1],[1594,7,1594,31,0,0,4,1],[1597,7,1597,17,0,0,4,0],[1611,7,1611,42,0,0,4,1],[1622,7,1622,73,0,0,4,1],[1624,15,1624,25,0,0,4,0],[1624,15,1624,25,0,0,4,1],[1625,9,1625,23,0,0,4,1],[1630,7,1630,19,0,0,4,0],[1630,7,1630,19,0,0,4,1],[1631,22,1631,29,0,0,4,0],[1631,22,1631,29,0,0,4,1],[1633,15,1633,40,0,0,4,0],[1633,15,1633,40,0,0,4,1],[1639,9,1639,20,0,0,4,1],[1643,9,1643,19,0,0,4,1],[1645,19,1645,49,0,0,4,0],[1645,19,1645,49,0,0,4,1],[1656,9,1656,21,0,0,4,1],[1659,9,1659,20,0,0,4,1],[1661,19,1661,47,0,0,4,0],[1661,19,1661,47,0,0,4,1],[1671,9,1671,19,0,0,4,1],[1674,19,1674,30,0,0,4,0],[1674,19,1674,30,0,0,4,1],[1676,15,1676,22,0,0,4,0],[1676,26,1676,41,0,0,4,0],[1682,17,1682,26,0,0,4,0],[1682,17,1682,26,0,0,4,1],[1686,45,1686,70,0,0,4,0],[1686,45,1686,70,0,0,4,1]],"jpeg_quality_scaling":[[139,7,139,19,0,0,4,0],[148,7,148,19,0,0,4,0]],"jpeg_set_defaults":[[196,7,196,31,0,0,4,1]],"jpeg_add_quant_table":[[60,27,60,38,0,0,4,0]],"add_huff_table":[[26,7,26,23,0,0,4,1],[28,12,28,34,0,0,4,1]]},"library_calls":["tjLoadImage","TJBUFSIZE","tjInitCompress","tjFree","tjBufSizeYUV","tjAlloc","tjDestroy","tjFree","tjEncodeYUV2","tjFree","tjDestroy","tjFree","tjCompress2","tjFree","tjDestroy","tjFree","tjDestroy","tj3InitVersion","tjFree","tjFree","tjFree","tj3DecompressHeader","tj3GetErrorStr","tj3Alloc","tj3Decompress16","tj3Alloc","tj3Decompress12","tj3GetICCProfile","tj3Free","tj3Free","tj3Free","tj3Destroy","tjFree","tjFree","tjFree"],"critical_calls":["tjLoadImage","TJBUFSIZE","tjInitCompress","tjBufSizeYUV","tjAlloc","tjEncodeYUV2","tjCompress2","tjDestroy","tj3InitVersion","tj3DecompressHeader","tj3GetErrorStr","tj3Alloc","tj3Decompress16","tj3Alloc","tj3Decompress12","tj3GetICCProfile","tj3Free","tj3Free","tj3Free","tj3Destroy","tjFree","tjFree","tjFree"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Create a temporary file for the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Determine if we should use 8/12/16 bit functions based on input
    int use_16bit = (size % 2 == 0) ? 1 : 0;
    int use_12bit = (size % 3 == 0) ? 1 : 0;
    
    // Variables for image info
    int width = 0, height = 0, pixelFormat = TJPF_RGB;
    int align = 1; // Byte alignment
    int flags = 0;
    
    // Load image from file using legacy API
    unsigned char *srcBuf = tjLoadImage("input_file", &width, align, &height, &pixelFormat, flags);
    if (!srcBuf) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Calculate buffer sizes
    unsigned long jpegBufSize = TJBUFSIZE(width, height);
    unsigned char *jpegBuf = nullptr;
    unsigned long jpegSize = 0;
    
    // Initialize legacy compressor
    tjhandle handleCompress = tjInitCompress();
    if (!handleCompress) {
        tjFree(srcBuf);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Prepare YUV buffer
    int subsamp = TJSAMP_420;
    unsigned long yuvBufSize = tjBufSizeYUV(width, height, subsamp);
    unsigned char *yuvBuf = (unsigned char *)tjAlloc(yuvBufSize);
    if (!yuvBuf) {
        tjDestroy(handleCompress);
        tjFree(srcBuf);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Encode to YUV
    if (tjEncodeYUV2(handleCompress, srcBuf, width, 0, height, pixelFormat, 
                     yuvBuf, subsamp, flags) != 0) {
        tjFree(yuvBuf);
        tjDestroy(handleCompress);
        tjFree(srcBuf);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Compress from YUV
    if (tjCompress2(handleCompress, srcBuf, width, 0, height, pixelFormat,
                    &jpegBuf, &jpegSize, subsamp, 85, flags) != 0) {
        tjFree(yuvBuf);
        tjDestroy(handleCompress);
        tjFree(srcBuf);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Clean up legacy compressor
    tjDestroy(handleCompress);
    
    // Initialize new decompressor (tj3)
    tjhandle handleDecompress = tj3InitVersion(TJINIT_DECOMPRESS, 30020);
    if (!handleDecompress) {
        tjFree(jpegBuf);
        tjFree(yuvBuf);
        tjFree(srcBuf);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Decompress header
    if (tj3DecompressHeader(handleDecompress, jpegBuf, jpegSize) != 0) {
        // Get error string
        const char *errorStr = tj3GetErrorStr(handleDecompress);
        (void)errorStr; // Use error string to avoid unused variable warning
    }
    
    // Prepare buffers for 16-bit and 12-bit decompression
    int pitch = width * tjPixelSize[pixelFormat];
    unsigned short *dstBuf16 = nullptr;
    short *dstBuf12 = nullptr;
    
    if (use_16bit && jpegSize > 0) {
        dstBuf16 = (unsigned short *)tj3Alloc(pitch * height * sizeof(unsigned short));
        if (dstBuf16) {
            // Try 16-bit decompression
            if (tj3Decompress16(handleDecompress, jpegBuf, jpegSize, 
                               dstBuf16, pitch, pixelFormat) != 0) {
                // Error occurred
            }
        }
    }
    
    if (use_12bit && jpegSize > 0) {
        dstBuf12 = (short *)tj3Alloc(pitch * height * sizeof(short));
        if (dstBuf12) {
            // Try 12-bit decompression
            if (tj3Decompress12(handleDecompress, jpegBuf, jpegSize, 
                               dstBuf12, pitch, pixelFormat) != 0) {
                // Error occurred
            }
        }
    }
    
    // Try to get ICC profile
    unsigned char *iccBuf = nullptr;
    size_t iccSize = 0;
    if (tj3GetICCProfile(handleDecompress, &iccBuf, &iccSize) == 0) {
        // ICC profile retrieved successfully
        if (iccBuf) {
            // Can process ICC profile here if needed
        }
    }
    
    // Create output file for writing
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        if (jpegBuf && jpegSize > 0) {
            fwrite(jpegBuf, 1, jpegSize, out_file);
        }
        assert_file_closed(&out_file);;
    }
    
    // Clean up all allocated resources
    if (iccBuf) {
        tj3Free(iccBuf);
    }
    if (dstBuf12) {
        tj3Free(dstBuf12);
    }
    if (dstBuf16) {
        tj3Free(dstBuf16);
    }
    tj3Destroy(handleDecompress);
    
    if (jpegBuf) {
        tjFree(jpegBuf);
    }
    tjFree(yuvBuf);
    tjFree(srcBuf);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}