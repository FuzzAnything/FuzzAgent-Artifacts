// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_write_row at pngwrite.c:756:1 in png.h
// png_write_rows at pngwrite.c:637:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_write_image at pngwrite.c:659:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_rows at pngset.c:1845:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_get_rows at pngget.c:50:1 in png.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <vector>

#include "code/png.h"

extern "C" int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
  if (Size < 1) {
    return 0;
  }

  // Initialize dummy file
  FILE *dummy_file = fopen("./dummy_file", "wb");
  if (!dummy_file) {
    return 0;
  }
  fwrite(Data, 1, Size, dummy_file);
  fclose(dummy_file);

  // Create PNG structures
  png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
  if (!png_ptr) {
    return 0;
  }

  png_infop info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr) {
    png_destroy_write_struct(&png_ptr, nullptr);
    return 0;
  }

  // Set up error handling
  if (setjmp(png_jmpbuf(png_ptr))) {
    png_destroy_write_struct(&png_ptr, &info_ptr);
    return 0;
  }

  // Initialize image parameters
  png_uint_32 width = 16;
  png_uint_32 height = 16;
  int bit_depth = 8;
  int color_type = PNG_COLOR_TYPE_RGB;

  png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type,
               PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

  // Create row data - use raw pointers to avoid vector destructor issues
  size_t row_bytes = png_get_rowbytes(png_ptr, info_ptr);
  if (row_bytes == 0) {
    png_destroy_write_struct(&png_ptr, &info_ptr);
    return 0;
  }
  
  png_byte *row_data = (png_byte*)malloc(row_bytes * height);
  if (!row_data) {
    png_destroy_write_struct(&png_ptr, &info_ptr);
    return 0;
  }
  
  // Fill with fuzzer data (cyclic)
  for (size_t i = 0; i < row_bytes * height; ++i) {
    row_data[i] = Data[i % Size];
  }

  // Create row pointers - use raw pointers
  png_byte **row_pointers = (png_byte**)malloc(height * sizeof(png_byte*));
  if (!row_pointers) {
    free(row_data);
    png_destroy_write_struct(&png_ptr, &info_ptr);
    return 0;
  }
  
  for (png_uint_32 y = 0; y < height; ++y) {
    row_pointers[y] = &row_data[y * row_bytes];
  }

  // Test png_set_rows
  png_set_rows(png_ptr, info_ptr, row_pointers);

  // Test png_get_rowbytes
  size_t retrieved_rowbytes = png_get_rowbytes(png_ptr, info_ptr);

  // Test png_get_rows
  png_byte** retrieved_rows = png_get_rows(png_ptr, info_ptr);

  // Open file for writing
  dummy_file = fopen("./dummy_file", "rb");
  if (!dummy_file) {
    free(row_pointers);
    free(row_data);
    png_destroy_write_struct(&png_ptr, &info_ptr);
    return 0;
  }

  // Set up PNG for writing
  png_init_io(png_ptr, dummy_file);
  png_write_info(png_ptr, info_ptr);

  // Test png_write_row
  if (row_bytes > 0 && height > 0) {
    png_write_row(png_ptr, row_pointers[0]);
  }

  // Test png_write_rows
  if (height > 1) {
    png_write_rows(png_ptr, row_pointers + 1, height - 1);
  }

  // Test png_write_image
  // Reset file position
  rewind(dummy_file);
  png_write_info(png_ptr, info_ptr);
  png_write_image(png_ptr, row_pointers);

  // Cleanup
  png_write_end(png_ptr, nullptr);
  fclose(dummy_file);
  
  // Free allocated memory
  free(row_pointers);
  free(row_data);
  
  png_destroy_write_struct(&png_ptr, &info_ptr);

  return 0;
}