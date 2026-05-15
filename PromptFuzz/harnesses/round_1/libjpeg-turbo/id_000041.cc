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
//<ID> 722
//<Prompt> []
/*<Combination>: [
*/
//<score> 19, nr_unique_branch: 3
//<Quality> {"density":19,"unique_branches":{"tj3YUVBufSize":[[1035,39,1035,50,0,0,4,0]],"tjDecompressToYUV2":[[2454,43,2454,52,0,0,4,0],[2482,7,2482,41,0,0,4,1]]},"library_calls":["tj3InitVersion","tjInitTransform","tj3LoadImage12","tj3Free","tj3LoadImage16","tj3Free","tj3DecompressHeader","tj3Get","tj3Get","tj3Get","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tj3YUVPlaneWidth","tj3YUVPlaneSize","tj3Alloc","tj3DecompressToYUVPlanes8","tj3YUVBufSize","tj3Alloc","tjDecompressToYUV2","tj3Free","tj3Free","tj3Free","tj3Free","tjDestroy","tj3Destroy"],"critical_calls":["tj3InitVersion","tjInitTransform","tj3LoadImage12","tj3Free","tj3LoadImage16","tj3Free","tj3DecompressHeader","tj3Get","tj3Get","tj3Get","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tj3YUVPlaneWidth","tj3YUVPlaneSize","tj3Alloc","tj3DecompressToYUVPlanes8","tj3YUVBufSize","tj3Alloc","tjDecompressToYUV2","tj3Free","tj3Free","tj3Free","tj3Free","tjDestroy","tj3Destroy"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    tjhandle handle = nullptr;
    tjhandle transform_handle = nullptr;
    FILE *in_file = nullptr;
    FILE *temp_file = nullptr;
    short *image12 = nullptr;
    unsigned short *image16 = nullptr;
    unsigned char *jpegBuf = nullptr;
    unsigned char *yuvBuf = nullptr;
    unsigned char **yuvPlanes = nullptr;
    int *strides = nullptr;
    int width, height, pixelFormat;
    size_t jpegSize = 0;
    unsigned long jpegSizeLong = 0;
    int align = 4;
    int ret = 0;

    // Write input data to a temporary file for loading functions
    in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        goto cleanup;
    }

    // Create a temporary file to simulate JPEG output
    temp_file = fopen("temp_output.jpg", "wb");
    if (!temp_file) {
        goto cleanup;
    }
    fwrite(data, 1, size, temp_file);
    assert_file_closed(&temp_file);;
    temp_file = nullptr;

    // Initialize decompressor handle
    handle = tj3Init(TJINIT_DECOMPRESS);
    if (!handle) {
        goto cleanup;
    }

    // Initialize transform handle (required for tjInitTransform)
    transform_handle = tjInitTransform();
    if (!transform_handle) {
        goto cleanup;
    }

    // Try loading as 12-bit image (will likely fail for JPEG input)
    image12 = tj3LoadImage12(handle, "temp_output.jpg", &width, align, &height, &pixelFormat);
    if (image12) {
        tj3Free(image12);
        image12 = nullptr;
    }

    // Try loading as 16-bit image (will likely fail for JPEG input)
    image16 = tj3LoadImage16(handle, "temp_output.jpg", &width, align, &height, &pixelFormat);
    if (image16) {
        tj3Free(image16);
        image16 = nullptr;
    }

    // Use input data directly for decompression
    jpegBuf = (unsigned char *)data;
    jpegSize = size;
    jpegSizeLong = (unsigned long)size;

    // Get image dimensions for buffer allocation
    if (tj3DecompressHeader(handle, jpegBuf, jpegSize) == 0) {
        width = tj3Get(handle, TJPARAM_JPEGWIDTH);
        height = tj3Get(handle, TJPARAM_JPEGHEIGHT);

        // Allocate YUV plane buffers for decompression
        int subsamp = tj3Get(handle, TJPARAM_SUBSAMP);
        yuvPlanes = new unsigned char*[3];
        strides = new int[3];
        
        for (int i = 0; i < 3; i++) {
            int planeWidth = tj3YUVPlaneWidth(i, width, subsamp);
            int planeHeight = tj3YUVPlaneHeight(i, height, subsamp);
            strides[i] = tj3YUVPlaneWidth(i, width, subsamp);
            size_t planeSize = tj3YUVPlaneSize(i, width, strides[i], height, subsamp);
            yuvPlanes[i] = (unsigned char *)tj3Alloc(planeSize);
            if (!yuvPlanes[i]) {
                goto cleanup;
            }
        }

        // Decompress to YUV planes using new API
        ret = tj3DecompressToYUVPlanes8(handle, jpegBuf, jpegSize, yuvPlanes, strides);
        if (ret != 0) {
            // Error handling - continue to other APIs
        }

        // Allocate buffer for old API decompression
        size_t yuvBufSize = tj3YUVBufSize(width, align, height, subsamp);
        yuvBuf = (unsigned char *)tj3Alloc(yuvBufSize);
        if (!yuvBuf) {
            goto cleanup;
        }

        // Decompress to YUV using old API
        ret = tjDecompressToYUV2(transform_handle, jpegBuf, jpegSizeLong, 
                                 yuvBuf, width, align, height, 0);
        if (ret != 0) {
            // Error handling - continue cleanup
        }
    }

cleanup:
    // Clean up allocated resources
    if (yuvPlanes) {
        for (int i = 0; i < 3; i++) {
            if (yuvPlanes[i]) {
                tj3Free(yuvPlanes[i]);
            }
        }
        delete[] yuvPlanes;
    }
    
    if (strides) {
        delete[] strides;
    }
    
    if (yuvBuf) {
        tj3Free(yuvBuf);
    }
    
    if (image12) {
        tj3Free(image12);
    }
    
    if (image16) {
        tj3Free(image16);
    }
    
    if (transform_handle) {
        tjDestroy(transform_handle);
    }
    
    if (handle) {
        tj3Destroy(handle);
    }
    
    if (in_file) {
        assert_file_closed(&in_file);;
    }
    
    if (temp_file) {
        assert_file_closed(&temp_file);;
    }
    
    // Remove temporary file
    remove("temp_output.jpg");
    
    assert_file_closed(&temp_file);
	assert_file_closed(&in_file);
	return 0;
}