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
//<ID> 1
//<Prompt> []
/*<Combination>: [
*/
//<score> 45, nr_unique_branch: 0
//<Quality> {"density":45,"unique_branches":{},"library_calls":["tj3InitVersion","tjInitDecompress","tj3Destroy","tjInitCompress","tjDestroy","tj3Destroy","tj3DecompressHeader","tj3Get","tj3Get","tj3Get","tjDecompressHeader3","tjGetErrorStr2","tjDestroy","tjDestroy","tj3Destroy","tjDestroy","tjDestroy","tj3Destroy","TJBUFSIZEYUV","tjAlloc","tjDestroy","tjDestroy","tj3Destroy","tjDecompress2","tjGetErrorStr2","tjFree","tjDestroy","tjDestroy","tj3Destroy","tjAlloc","tjFree","tjDestroy","tjDestroy","tj3Destroy","tjPlaneWidth","tjPlaneHeight","tjPlaneWidth","tjPlaneHeight","tjEncodeYUVPlanes","tjGetErrorStr2","tjFree","tjFree","tjDestroy","tjDestroy","tj3Destroy"],"critical_calls":["tj3InitVersion","tjInitDecompress","tjInitCompress","tj3DecompressHeader","tj3Get","tj3Get","tj3Get","tjDecompressHeader3","TJBUFSIZEYUV","tjAlloc","tjDecompress2","tjAlloc","tjPlaneWidth","tjPlaneHeight","tjPlaneWidth","tjPlaneHeight","tjEncodeYUVPlanes","tjGetErrorStr2","tjFree","tjFree","tjDestroy","tjDestroy","tj3Destroy"],"visited":0}
/**/



extern "C" int LLVMFuzzerTestOneInput_0(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Minimum size for meaningful JPEG data
    
    // Initialize handles for both old and new APIs
    tjhandle new_handle = tj3InitVersion(TJINIT_DECOMPRESS, 0);
    if (!new_handle) {
	return 0;
	}
	
    
    tjhandle old_decomp_handle = tjInitDecompress();
    if (!old_decomp_handle) {
        tj3Destroy(new_handle);
        return 0;
    }
    
    tjhandle old_comp_handle = tjInitCompress();
    if (!old_comp_handle) {
        tjDestroy(old_decomp_handle);
        tj3Destroy(new_handle);
        return 0;
    }
    
    // Step 1: Use tj3DecompressHeader to get image info
    int width = 0, height = 0, jpegSubsamp = 0, jpegColorspace = 0;
    unsigned long jpegSize = static_cast<unsigned long>(size);
    
    // Note: tj3DecompressHeader uses size_t, while older APIs use unsigned long
    // We'll use the new API for header parsing
    int result = tj3DecompressHeader(new_handle, data, size);
    if (result == 0) {
        // Get dimensions and format from new handle
        width = tj3Get(new_handle, TJPARAM_JPEGWIDTH);
        height = tj3Get(new_handle, TJPARAM_JPEGHEIGHT);
        jpegSubsamp = tj3Get(new_handle, TJPARAM_SUBSAMP);
        
        // For old API compatibility, also get info using old API
        tjDecompressHeader3(old_decomp_handle, data, jpegSize, 
                          &width, &height, &jpegSubsamp, &jpegColorspace);
    } else {
        // Get error string using tjGetErrorStr2
        char *error_str = tjGetErrorStr2(old_decomp_handle);
        // Clean up and exit
        tjDestroy(old_comp_handle);
        tjDestroy(old_decomp_handle);
        tj3Destroy(new_handle);
        return 0;
    }
    
    if (width <= 0 || height <= 0) {
        tjDestroy(old_comp_handle);
        tjDestroy(old_decomp_handle);
        tj3Destroy(new_handle);
        return 0;
    }
    
    // Step 2: Use TJBUFSIZEYUV to calculate YUV buffer size
    unsigned long yuvBufSize = TJBUFSIZEYUV(width, height, jpegSubsamp);
    
    // Step 3: Decompress to RGB buffer using old API tjDecompress2
    int pitch = width * tjPixelSize[TJPF_RGB];
    unsigned char *rgbBuf = (unsigned char *)tjAlloc(pitch * height);
    if (!rgbBuf) {
        tjDestroy(old_comp_handle);
        tjDestroy(old_decomp_handle);
        tj3Destroy(new_handle);
        return 0;
    }
    
    result = tjDecompress2(old_decomp_handle, data, jpegSize, rgbBuf,
                          width, pitch, height, TJPF_RGB, 0);
    
    // Check for errors using tjGetErrorStr2
    if (result != 0) {
        char *error_str = tjGetErrorStr2(old_decomp_handle);
        tjFree(rgbBuf);
        tjDestroy(old_comp_handle);
        tjDestroy(old_decomp_handle);
        tj3Destroy(new_handle);
        return 0;
    }
    
    // Step 4: Encode RGB to YUV planes using tjEncodeYUVPlanes
    unsigned char *yuvBuf = (unsigned char *)tjAlloc(yuvBufSize);
    if (!yuvBuf) {
        tjFree(rgbBuf);
        tjDestroy(old_comp_handle);
        tjDestroy(old_decomp_handle);
        tj3Destroy(new_handle);
        return 0;
    }
    
    // Prepare plane pointers for YUV encoding
    unsigned char *yuvPlanes[3];
    int strides[3] = {0};
    
    // Calculate plane dimensions based on subsampling
    int y_width = tjPlaneWidth(0, width, jpegSubsamp);
    int y_height = tjPlaneHeight(0, height, jpegSubsamp);
    int uv_width = tjPlaneWidth(1, width, jpegSubsamp);
    int uv_height = tjPlaneHeight(1, height, jpegSubsamp);
    
    // Set up plane pointers within the allocated buffer
    unsigned long y_size = y_width * y_height;
    unsigned long u_size = uv_width * uv_height;
    unsigned long v_size = uv_width * uv_height;
    
    yuvPlanes[0] = yuvBuf;
    yuvPlanes[1] = yuvBuf + y_size;
    yuvPlanes[2] = yuvBuf + y_size + u_size;
    
    // Encode to YUV planes
    result = tjEncodeYUVPlanes(old_comp_handle, rgbBuf, width, pitch, height,
                              TJPF_RGB, yuvPlanes, strides, jpegSubsamp, 0);
    
    // Check for errors using tjGetErrorStr2
    if (result != 0) {
        char *error_str = tjGetErrorStr2(old_comp_handle);
    }
    
    // Clean up all allocated resources
    tjFree(yuvBuf);
    tjFree(rgbBuf);
    tjDestroy(old_comp_handle);
    tjDestroy(old_decomp_handle);
    tj3Destroy(new_handle);
    
    return 0;
}