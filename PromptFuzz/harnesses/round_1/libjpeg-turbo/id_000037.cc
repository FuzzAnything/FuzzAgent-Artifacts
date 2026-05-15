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
//<ID> 615
//<Prompt> []
/*<Combination>: [
*/
//<score> 171, nr_unique_branch: 100
//<Quality> {"density":36,"unique_branches":{"jpeg_write_icc_profile":[[56,7,56,27,0,0,4,1],[56,31,56,48,0,0,4,1],[58,7,58,44,0,0,4,1],[63,7,63,61,0,0,4,0],[66,10,66,26,0,0,4,0],[66,10,66,26,0,0,4,1],[69,9,69,42,0,0,4,1],[99,12,99,20,0,0,4,0],[99,12,99,20,0,0,4,1]],"prepare_for_pass":[[589,9,589,28,0,0,4,1]],"jpeg_write_m_header":[[267,7,267,32,0,0,4,1],[268,8,268,46,0,0,4,0],[269,8,269,44,0,0,4,1]],"jpeg_finish_compress":[[184,7,184,45,0,0,4,1],[185,7,185,43,0,0,4,0]],"tjBufSize":[[996,7,996,22,0,0,4,0],[1002,10,1002,23,0,0,4,0]],"jinit_compress_master":[[42,7,42,26,0,0,4,1]],"jpeg_write_raw_data":[[158,7,158,47,0,0,4,1],[161,7,161,30,0,0,4,1],[164,7,164,43,0,0,4,1],[166,7,166,50,0,0,4,1],[172,7,172,30,0,0,4,1],[183,7,183,39,0,0,4,0],[183,7,183,39,0,0,4,1],[188,7,188,37,0,0,4,1],[192,7,192,42,0,0,4,1],[194,7,194,52,0,0,4,1]],"tjCompressFromYUV":[[1541,7,1541,18,0,0,4,1],[1541,22,1541,46,0,0,4,1],[1549,7,1549,22,0,0,4,1],[340,7,340,12,1,0,4,1]],"start_pass_main":[[63,7,63,25,0,0,4,0]],"tj3SetCroppingRegion":[[2065,7,2065,37,0,0,4,0]],"write_marker_header":[[447,7,447,36,0,0,4,1]],"jpeg_start_compress":[[63,26,63,44,0,0,4,0]],"tj3EncodeYUVPlanes8":[[1586,7,1586,22,0,0,4,0]],"getPixelFormat":[[350,7,350,21,0,0,4,1],[354,7,354,21,0,0,4,1]],"tjPlaneHeight":[[1178,10,1178,23,0,0,4,1]],"jinit_c_main_controller":[[167,7,167,25,0,0,4,0]],"tj3CompressFromYUV8":[[1493,7,1493,21,0,0,4,1],[1493,25,1493,35,0,0,4,1],[1493,39,1493,48,0,0,4,1],[1493,52,1493,60,0,0,4,1],[1494,7,1494,18,0,0,4,1],[1497,7,1497,38,0,0,4,1],[1504,7,1504,35,0,0,4,1],[1512,9,1513,36,0,0,4,1],[1514,9,1515,36,0,0,4,1],[340,7,340,12,1,0,4,1]],"tj3CompressFromYUVPlanes8":[[1338,7,1338,35,0,0,4,1],[1341,7,1341,17,0,0,4,1],[1341,21,1341,34,0,0,4,1],[1341,38,1341,48,0,0,4,1],[1341,52,1341,63,0,0,4,1],[1342,7,1342,22,0,0,4,1],[1342,26,1342,42,0,0,4,1],[1344,7,1344,35,0,0,4,0],[1344,40,1344,53,0,0,4,1],[1344,57,1344,70,0,0,4,1],[1347,7,1347,26,0,0,4,1],[1349,7,1349,38,0,0,4,1],[1358,7,1358,22,0,0,4,1],[1364,7,1364,27,0,0,4,0],[1364,31,1364,49,0,0,4,0],[1366,15,1366,40,0,0,4,0],[1366,15,1366,40,0,0,4,1],[1376,9,1376,23,0,0,4,0],[1376,9,1376,23,0,0,4,1],[1376,27,1376,38,0,0,4,0],[1376,27,1376,38,0,0,4,1],[1379,9,1379,74,0,0,4,1],[1382,19,1382,30,0,0,4,0],[1382,19,1382,30,0,0,4,1],[1384,15,1384,22,0,0,4,0],[1384,26,1384,41,0,0,4,0],[1387,7,1387,16,0,0,4,0],[1387,7,1387,16,0,0,4,1],[1388,9,1388,76,0,0,4,1],[1391,17,1391,42,0,0,4,0],[1391,17,1391,42,0,0,4,1],[1392,11,1392,77,0,0,4,1],[1394,21,1394,32,0,0,4,0],[1394,21,1394,32,0,0,4,1],[1403,17,1403,47,0,0,4,0],[1403,17,1403,47,0,0,4,1],[1408,17,1408,42,0,0,4,0],[1408,17,1408,42,0,0,4,1],[1412,11,1412,20,0,0,4,0],[1412,11,1412,20,0,0,4,1],[1415,21,1415,28,0,0,4,0],[1415,21,1415,28,0,0,4,1],[1418,27,1418,36,0,0,4,0],[1418,27,1418,36,0,0,4,1],[1422,35,1422,44,0,0,4,0],[1422,35,1422,44,0,0,4,1],[1435,45,1435,57,0,0,4,1],[515,26,515,35,23,0,4,0],[515,26,515,35,23,0,4,1]]},"library_calls":["tj3InitVersion","tj3InitVersion","tj3Destroy","tjDecompressHeader3","tj3LoadImage8","tj3Alloc","tj3Destroy","tj3Destroy","tj3Alloc","tj3SetICCProfile","tj3Free","tj3SetCroppingRegion","tjGetScalingFactors","tjBufSizeYUV","tjAlloc","tjEncodeYUV","tjBufSize","tjCompressFromYUV","tjFree","tjFree","tj3Free","tj3Alloc","tj3SetICCProfile","tj3Free","tj3SetCroppingRegion","tjGetScalingFactors","tj3Alloc","tjDecompress2","tjBufSizeYUV","tjAlloc","tjEncodeYUV","tjBufSize","tjCompressFromYUV","tjFree","tjFree","tj3Free","tj3Destroy","tj3Destroy"],"critical_calls":["tj3InitVersion","tj3InitVersion","tjDecompressHeader3","tj3LoadImage8","tj3Alloc","tj3Alloc","tj3SetICCProfile","tj3Free","tj3SetCroppingRegion","tjGetScalingFactors","tjBufSizeYUV","tjAlloc","tjEncodeYUV","tjBufSize","tjCompressFromYUV","tjFree","tjFree","tj3Free","tj3Destroy","tj3Destroy"],"visited":3}
/**/


extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Create temporary file with input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize decompressor handle
    tjhandle decompHandle = tj3Init(TJINIT_DECOMPRESS);
    if (!decompHandle) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize compressor handle
    tjhandle compHandle = tj3Init(TJINIT_COMPRESS);
    if (!compHandle) {
        tj3Destroy(decompHandle);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Read JPEG header from input data
    int width, height, jpegSubsamp, jpegColorspace;
    unsigned long jpegSize = size;
    
    // Decompress header to get image info
    if (tjDecompressHeader3(compHandle, data, jpegSize, &width, &height, &jpegSubsamp, &jpegColorspace) != 0) {
        // Try alternative approach - load image from file
        rewind(in_file);
        int align = 4;
        int pixelFormat = TJPF_RGB;
        
        unsigned char *imgBuf = tj3LoadImage8(compHandle, "input_file", &width, align, &height, &pixelFormat);
        if (!imgBuf) {
            // If loading fails, create a synthetic image
            width = 64;
            height = 64;
            pixelFormat = TJPF_RGB;
            align = 4;
            
            size_t bufSize = width * height * tjPixelSize[pixelFormat];
            imgBuf = (unsigned char *)tj3Alloc(bufSize);
            if (imgBuf) {
                memset(imgBuf, 128, bufSize);
            } else {
                tj3Destroy(compHandle);
                tj3Destroy(decompHandle);
                assert_file_closed(&in_file);;
                assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
            }
        }
        
        // Set ICC profile (using part of input data as dummy ICC)
        size_t iccSize = size > 100 ? 100 : size;
        unsigned char *iccBuf = (unsigned char *)tj3Alloc(iccSize);
        if (iccBuf && iccSize > 0) {
            memcpy(iccBuf, data, iccSize);
            tj3SetICCProfile(compHandle, iccBuf, iccSize);
            tj3Free(iccBuf);
        }
        
        // Set cropping region (crop center 50% of image)
        tjregion cropRegion;
        cropRegion.x = width / 4;
        cropRegion.y = height / 4;
        cropRegion.w = width / 2;
        cropRegion.h = height / 2;
        tj3SetCroppingRegion(compHandle, cropRegion);
        
        // Get scaling factors
        int numScalingFactors;
        tjscalingfactor *scalingFactors = tjGetScalingFactors(&numScalingFactors);
        if (scalingFactors && numScalingFactors > 0) {
            // Use first scaling factor
            tjscalingfactor scale = scalingFactors[0];
            // Note: tjSetScalingFactor is not in our API list, so we just retrieve the factors
        }
        
        // Calculate buffer sizes
        int subsamp = TJSAMP_420;
        unsigned long yuvBufSize = tjBufSizeYUV(width, height, subsamp);
        unsigned char *yuvBuf = (unsigned char *)tjAlloc(yuvBufSize);
        
        if (yuvBuf) {
            // Encode to YUV
            int flags = 0;
            int pitch = width * tjPixelSize[pixelFormat];
            tjEncodeYUV(compHandle, imgBuf, width, pitch, height, pixelFormat, yuvBuf, subsamp, flags);
            
            // Calculate JPEG buffer size
            unsigned long jpegBufSize = tjBufSize(width, height, jpegSubsamp);
            unsigned char *jpegBuf = NULL;
            unsigned long compressedSize = 0;
            
            // Compress from YUV
            int jpegQual = 85;
            tjCompressFromYUV(compHandle, yuvBuf, width, 4, height, subsamp, &jpegBuf, &compressedSize, jpegQual, flags);
            
            if (jpegBuf) {
                tjFree(jpegBuf);
            }
            
            tjFree(yuvBuf);
        }
        
        tj3Free(imgBuf);
    } else {
        // We successfully read the JPEG header, proceed with other operations
        
        // Set ICC profile (using part of input data as dummy ICC)
        size_t iccSize = size > 100 ? 100 : size;
        unsigned char *iccBuf = (unsigned char *)tj3Alloc(iccSize);
        if (iccBuf && iccSize > 0) {
            memcpy(iccBuf, data, iccSize);
            tj3SetICCProfile(compHandle, iccBuf, iccSize);
            tj3Free(iccBuf);
        }
        
        // Set cropping region
        tjregion cropRegion;
        cropRegion.x = width / 4;
        cropRegion.y = height / 4;
        cropRegion.w = width / 2;
        cropRegion.h = height / 2;
        tj3SetCroppingRegion(compHandle, cropRegion);
        
        // Get scaling factors
        int numScalingFactors;
        tjscalingfactor *scalingFactors = tjGetScalingFactors(&numScalingFactors);
        
        // Decompress to RGB buffer
        int pixelFormat = TJPF_RGB;
        int pitch = width * tjPixelSize[pixelFormat];
        unsigned char *rgbBuf = (unsigned char *)tj3Alloc(pitch * height);
        
        if (rgbBuf) {
            // Decompress the image
            tjDecompress2(decompHandle, data, jpegSize, rgbBuf, width, pitch, height, pixelFormat, 0);
            
            // Calculate buffer sizes
            int subsamp = TJSAMP_420;
            unsigned long yuvBufSize = tjBufSizeYUV(width, height, subsamp);
            unsigned char *yuvBuf = (unsigned char *)tjAlloc(yuvBufSize);
            
            if (yuvBuf) {
                // Encode to YUV
                int flags = 0;
                tjEncodeYUV(compHandle, rgbBuf, width, pitch, height, pixelFormat, yuvBuf, subsamp, flags);
                
                // Calculate JPEG buffer size
                unsigned long jpegBufSize = tjBufSize(width, height, jpegSubsamp);
                unsigned char *jpegBuf = NULL;
                unsigned long compressedSize = 0;
                
                // Compress from YUV
                int jpegQual = 85;
                tjCompressFromYUV(compHandle, yuvBuf, width, 4, height, subsamp, &jpegBuf, &compressedSize, jpegQual, flags);
                
                if (jpegBuf) {
                    tjFree(jpegBuf);
                }
                
                tjFree(yuvBuf);
            }
            
            tj3Free(rgbBuf);
        }
    }
    
    // Cleanup
    tj3Destroy(compHandle);
    tj3Destroy(decompHandle);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}