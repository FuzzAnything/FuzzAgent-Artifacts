// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_x_pixels_per_inch at pngget.c:363:1 in png.h
// png_get_pixels_per_inch at pngget.c:357:1 in png.h
// png_get_x_pixels_per_meter at pngget.c:125:1 in png.h
// png_get_pHYs_dpi at pngget.c:435:1 in png.h
// png_get_y_pixels_per_meter at pngget.c:146:1 in png.h
// png_get_pixels_per_meter at pngget.c:167:1 in png.h
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
#include <cstdlib>
#include <cstring>
#include <fstream>

extern "C" int LLVMFuzzerTestOneInput_85(const uint8_t *Data, size_t Size) {
  static const char* dummy_file = "./dummy_file";
  
  // Write fuzzer input to dummy file
  std::ofstream out(dummy_file, std::ios::binary);
  if (!out.is_open()) {
    return 0;
  }
  out.write(reinterpret_cast<const char*>(Data), Size);
  out.close();
  
  // Open dummy file for reading
  FILE *fp = fopen(dummy_file, "rb");
  if (!fp) {
    return 0;
  }
  
  // Initialize libpng structures
  png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
  if (!png_ptr) {
    fclose(fp);
    return 0;
  }
  
  png_infop info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr) {
    png_destroy_read_struct(&png_ptr, nullptr, nullptr);
    fclose(fp);
    return 0;
  }
  
  // Set up error handling
  if (setjmp(png_jmpbuf(png_ptr))) {
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    fclose(fp);
    return 0;
  }
  
  // Initialize I/O
  png_init_io(png_ptr, fp);
  
  // Read PNG header
  png_read_info(png_ptr, info_ptr);
  
  // Call target API functions
  png_uint_32 res1 = png_get_x_pixels_per_inch(png_ptr, info_ptr);
  png_uint_32 res2 = png_get_pixels_per_inch(png_ptr, info_ptr);
  png_uint_32 res3 = png_get_x_pixels_per_meter(png_ptr, info_ptr);
  
  png_uint_32 res_x, res_y;
  int unit_type;
  png_uint_32 res4 = png_get_pHYs_dpi(png_ptr, info_ptr, &res_x, &res_y, &unit_type);
  
  png_uint_32 res5 = png_get_y_pixels_per_meter(png_ptr, info_ptr);
  png_uint_32 res6 = png_get_pixels_per_meter(png_ptr, info_ptr);
  
  // Clean up
  png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
  fclose(fp);
  
  // Remove dummy file
  remove(dummy_file);
  
  return 0;
}