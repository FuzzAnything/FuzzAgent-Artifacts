#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <tiffio.hxx>
//<ID> 53
//<Prompt> []
/*<Combination>: [
*/
//<score> 13, nr_unique_branch: 0
//<Quality> {"density":13,"unique_branches":{},"library_calls":["TIFFOpenOptionsAlloc","TIFFClientOpenExt","TIFFOpenOptionsFree","TIFFClose","TIFFOpenOptionsFree","TIFFSetFileno","TIFFReadDirectory","TIFFTileSize","TIFFWriteEncodedTile","TIFFCIELabToXYZ","TIFFSwabArrayOfFloat","LogL16fromY","TIFFFlush","TIFFErrorExt","TIFFClose","TIFFOpenOptionsFree"],"critical_calls":["TIFFOpenOptionsAlloc","TIFFClientOpenExt","TIFFSetFileno","TIFFReadDirectory","TIFFTileSize","TIFFWriteEncodedTile","TIFFCIELabToXYZ","TIFFSwabArrayOfFloat","LogL16fromY","TIFFFlush","TIFFErrorExt","TIFFClose","TIFFOpenOptionsFree"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





  // Write input data to file
  FILE *in_file = fopen("input_file", "wb");
  if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
  fwrite(data, 1, size, in_file);
  assert_file_closed(&in_file);;
  
  // Reopen for reading with libtiff
  in_file = fopen("input_file", "rb");
  if (!in_file) {
	assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	return 0;
	}
	
  
  TIFF *tif = NULL;
  TIFFOpenOptions *opts = TIFFOpenOptionsAlloc();
  if (!opts) {
    assert_file_closed(&in_file);;
    assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	return 0;
  }
  
  // Open TIFF from file descriptor
  int fd = fuzz_fileno(in_file);
  tif = TIFFClientOpenExt("input_file", "r", 
                         (thandle_t)(intptr_t)fd,
                         NULL, NULL, NULL, NULL, NULL, NULL, NULL,
                         opts);
  
  if (!tif) {
    TIFFOpenOptionsFree(opts);
    assert_file_closed(&in_file);;
    assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
  }
  
  // Create output file
  FILE *out_file = fopen("output_file", "wb");
  if (!out_file) {
    TIFFClose(tif);
    TIFFOpenOptionsFree(opts);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
  }
  
  int out_fd = fuzz_fileno(out_file);
  
  // Set output file descriptor using TIFFSetFileno
  TIFFSetFileno(tif, out_fd);
  
  // Try to read directory to initialize TIFF structure
  if (TIFFReadDirectory(tif)) {
    // Get tile size
    tmsize_t tile_size = TIFFTileSize(tif);
    
    if (tile_size > 0 && size >= (size_t)tile_size) {
      // Prepare tile data buffer
      std::vector<uint8_t> tile_buf(tile_size);
      size_t copy_size = size < (size_t)tile_size ? size : (size_t)tile_size;
      memcpy(tile_buf.data(), data, copy_size);
      
      // Write encoded tile (use tile 0)
      tmsize_t written = TIFFWriteEncodedTile(tif, 0, tile_buf.data(), tile_size);
      
      if (written > 0) {
        // Prepare color conversion data
        TIFFCIELabToRGB labToRGB;
        uint32_t l_value = 5000; // Example L* value
        int32_t a_value = 0, b_value = 0;
        float X, Y, Z;
        
        // Convert Lab to XYZ
        TIFFCIELabToXYZ(&labToRGB, l_value, a_value, b_value, &X, &Y, &Z);
        
        // Prepare float array for byte swapping
        float float_array[3] = {X, Y, Z};
        TIFFSwabArrayOfFloat(float_array, 3);
        
        // Convert Y to LogL16
        double y_double = (double)Y;
        int logL16_result = LogL16fromY(y_double, 0);
        
        // Flush data
        TIFFFlush(tif);
      }
    }
  }
  
  // Report error if needed (example usage)
  if (size == 0) {
    TIFFErrorExt((thandle_t)tif, "LLVMFuzzerTestOneInput_3", 
                 "Empty input data");
  }
  
  // Cleanup
  TIFFClose(tif);
  TIFFOpenOptionsFree(opts);
  assert_file_closed(&in_file);;
  assert_file_closed(&out_file);;
  
  // Remove temporary files
  remove("input_file");
  remove("output_file");
  
  assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(fd);
	return 0;
}