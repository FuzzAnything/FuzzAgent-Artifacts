// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_compression_type at pngget.c:116:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
// png_get_color_type at pngget.c:89:1 in png.h
// png_get_filter_type at pngget.c:98:1 in png.h
// png_get_bit_depth at pngget.c:80:1 in png.h
// png_get_channels at pngget.c:481:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "code/png.h"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

static void dummy_error_fn(png_structp, png_const_charp) {}

extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
  // Write fuzzer input to dummy file
  FILE *fp = fopen("./dummy_file", "wb");
  if (!fp) return 0;
  fwrite(Data, 1, Size, fp);
  fclose(fp);

  // Open dummy file for reading
  fp = fopen("./dummy_file", "rb");
  if (!fp) return 0;

  // Initialize libpng structures
  png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, dummy_error_fn, NULL);
  if (!png_ptr) {
    fclose(fp);
    return 0;
  }

  png_infop info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr) {
    png_destroy_read_struct(&png_ptr, NULL, NULL);
    fclose(fp);
    return 0;
  }

  // Set up error handling
  if (setjmp(png_jmpbuf(png_ptr))) {
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);
    return 0;
  }

  png_init_io(png_ptr, fp);
  
  // Read PNG header
  png_read_info(png_ptr, info_ptr);

  // Call target API functions
  png_get_compression_type(png_ptr, info_ptr);
  
  png_uint_32 width, height;
  int bit_depth, color_type, interlace_method, compression_method, filter_method;
  png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
               &interlace_method, &compression_method, &filter_method);
  
  png_get_color_type(png_ptr, info_ptr);
  png_get_filter_type(png_ptr, info_ptr);
  png_get_bit_depth(png_ptr, info_ptr);
  png_get_channels(png_ptr, info_ptr);

  // Cleanup
  png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
  fclose(fp);
  
  return 0;
}