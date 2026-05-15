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
//<ID> 39
//<Prompt> []
/*<Combination>: [
*/
//<score> 42, nr_unique_branch: 0
//<Quality> {"density":42,"unique_branches":{},"library_calls":["tjLoadImage","tjFree","tj3InitVersion","tj3InitVersion","tjFree","tj3Destroy","tj3Destroy","tj3YUVBufSize","tjFree","tj3Destroy","tj3Destroy","tj3Alloc","tjFree","tj3Destroy","tj3Destroy","tj3EncodeYUV8","tjFree","tj3Free","tj3Destroy","tj3Destroy","tj3Alloc","tjFree","tj3Free","tj3Destroy","tj3Destroy","tj3DecodeYUVPlanes8","tjFree","tj3Free","tj3Free","tj3Destroy","tj3Destroy","tj3SaveImage8","tj3Alloc","tj3SaveImage12","tj3Free","tj3Alloc","tj3SaveImage16","tj3Free","tjFree","tj3Free","tj3Free","tj3Destroy","tj3Destroy"],"critical_calls":["tjLoadImage","tj3InitVersion","tj3InitVersion","tj3YUVBufSize","tj3Alloc","tj3EncodeYUV8","tj3Alloc","tj3DecodeYUVPlanes8","tj3SaveImage8","tj3Alloc","tj3SaveImage12","tj3Free","tj3Alloc","tj3SaveImage16","tj3Free","tjFree","tj3Free","tj3Free","tj3Destroy","tj3Destroy"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* f_data, size_t f_size) {
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
	return 0;
	}
	  // Need enough data for meaningful operations
  
  // Create temporary files
  const char* input_filename = "input_file";
  const char* output8_filename = "output8_file";
  const char* output12_filename = "output12_file";
  const char* output16_filename = "output16_file";
  
  // Write input data to file
  FILE* in_file = fopen(input_filename, "wb");
  if (!in_file) {
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
	}
	
  fwrite(data, 1, size, in_file);
  assert_file_closed(&in_file);;
  
  // Load image using tjLoadImage (8-bit)
  int width, height, pixelFormat;
  unsigned char* loaded_image = tjLoadImage(input_filename, &width, 1, &height, &pixelFormat, 0);
  if (!loaded_image || width <= 0 || height <= 0) {
    if (loaded_image) tjFree(loaded_image);
    remove(input_filename);
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
  }
  
  // Initialize handles
  tjhandle enc_handle = tj3InitVersion(TJINIT_COMPRESS, 0);
  tjhandle dec_handle = tj3InitVersion(TJINIT_DECOMPRESS, 0);
  if (!enc_handle || !dec_handle) {
    if (loaded_image) tjFree(loaded_image);
    if (enc_handle) tj3Destroy(enc_handle);
    if (dec_handle) tj3Destroy(dec_handle);
    remove(input_filename);
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
  }
  
  // Calculate YUV buffer size and allocate
  int align = 1;
  size_t yuv_buf_size = tj3YUVBufSize(width, align, height, TJSAMP_420);
  if (yuv_buf_size == 0) {
    tjFree(loaded_image);
    tj3Destroy(enc_handle);
    tj3Destroy(dec_handle);
    remove(input_filename);
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
  }
  
  unsigned char* yuv_buffer = (unsigned char*)tj3Alloc(yuv_buf_size);
  if (!yuv_buffer) {
    tjFree(loaded_image);
    tj3Destroy(enc_handle);
    tj3Destroy(dec_handle);
    remove(input_filename);
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
  }
  
  // Encode to YUV using tj3EncodeYUV8
  int pitch = width * tjPixelSize[pixelFormat];
  if (tj3EncodeYUV8(enc_handle, loaded_image, width, pitch, height, 
                    pixelFormat, yuv_buffer, align) != 0) {
    // Error occurred during encoding
    tjFree(loaded_image);
    tj3Free(yuv_buffer);
    tj3Destroy(enc_handle);
    tj3Destroy(dec_handle);
    remove(input_filename);
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
  }
  
  // Set up YUV planes for decoding
  int strides[3];
  strides[0] = width;  // Y plane stride
  strides[1] = width / 2;  // U plane stride
  strides[2] = width / 2;  // V plane stride
  
  const unsigned char* src_planes[3];
  src_planes[0] = yuv_buffer;  // Y plane
  src_planes[1] = yuv_buffer + width * height;  // U plane
  src_planes[2] = src_planes[1] + (width / 2) * (height / 2);  // V plane
  
  // Allocate decoded buffer
  unsigned char* decoded_buffer = (unsigned char*)tj3Alloc(width * height * tjPixelSize[pixelFormat]);
  if (!decoded_buffer) {
    tjFree(loaded_image);
    tj3Free(yuv_buffer);
    tj3Destroy(enc_handle);
    tj3Destroy(dec_handle);
    remove(input_filename);
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
  }
  
  // Decode YUV planes back to RGB using tj3DecodeYUVPlanes8
  if (tj3DecodeYUVPlanes8(dec_handle, src_planes, strides, decoded_buffer,
                          width, width * tjPixelSize[pixelFormat], height,
                          pixelFormat) != 0) {
    // Error occurred during decoding
    tjFree(loaded_image);
    tj3Free(yuv_buffer);
    tj3Free(decoded_buffer);
    tj3Destroy(enc_handle);
    tj3Destroy(dec_handle);
    remove(input_filename);
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
  }
  
  // Save decoded image as 8-bit using tj3SaveImage8
  if (tj3SaveImage8(dec_handle, output8_filename, decoded_buffer, 
                    width, width * tjPixelSize[pixelFormat], height,
                    pixelFormat) != 0) {
    // Save failed, but continue with other operations
  }
  
  // Create 12-bit and 16-bit versions by scaling the 8-bit data
  size_t pixel_count = width * height * tjPixelSize[pixelFormat];
  
  // 12-bit buffer (signed short)
  short* buffer12 = (short*)tj3Alloc(pixel_count * sizeof(short));
  if (buffer12) {
    for (size_t i = 0; i < pixel_count; i++) {
      // Scale 8-bit (0-255) to 12-bit (0-4095)
      buffer12[i] = (short)((decoded_buffer[i] * 4095 + 127) / 255);
    }
    
    // Save 12-bit image using tj3SaveImage12
    if (tj3SaveImage12(dec_handle, output12_filename, buffer12,
                       width, width * tjPixelSize[pixelFormat] * sizeof(short),
                       height, pixelFormat) != 0) {
      // Save failed
    }
    
    tj3Free(buffer12);
  }
  
  // 16-bit buffer (unsigned short)
  unsigned short* buffer16 = (unsigned short*)tj3Alloc(pixel_count * sizeof(unsigned short));
  if (buffer16) {
    for (size_t i = 0; i < pixel_count; i++) {
      // Scale 8-bit (0-255) to 16-bit (0-65535)
      buffer16[i] = (unsigned short)((decoded_buffer[i] * 65535 + 127) / 255);
    }
    
    // Save 16-bit image using tj3SaveImage16
    if (tj3SaveImage16(dec_handle, output16_filename, buffer16,
                       width, width * tjPixelSize[pixelFormat] * sizeof(unsigned short),
                       height, pixelFormat) != 0) {
      // Save failed
    }
    
    tj3Free(buffer16);
  }
  
  // Cleanup
  tjFree(loaded_image);
  tj3Free(yuv_buffer);
  tj3Free(decoded_buffer);
  tj3Destroy(enc_handle);
  tj3Destroy(dec_handle);
  
  // Remove temporary files
  remove(input_filename);
  remove(output8_filename);
  remove(output12_filename);
  remove(output16_filename);
  
  assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}