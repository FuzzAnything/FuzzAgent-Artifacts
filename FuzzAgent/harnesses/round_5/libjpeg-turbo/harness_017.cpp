#include <cstddef>      // for size_t
#include <cstdio>       // for FILE
#include <csetjmp>      // for setjmp/longjmp
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>
#include "jpeglib.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_017 {
/*
 * Fuzzing harness for libjpeg-turbo targeting comprehensive 12-bit JPEG decompression 
 * with scanline manipulation operations.
 * 
 * Target APIs from coverage guidance:
 * - jpeg12_read_raw_data (16 undiscovered branches)
 * - jpeg12_skip_scanlines (80 undiscovered branches)
 * - jpeg12_crop_scanline (42 undiscovered branches)
 * - jpeg12_read_scanlines (16 undiscovered branches)
 * 
 * Required Helper APIs:
 * - jpeg_CreateDecompress (Initialization)
 * - jpeg_mem_src (Input setup)
 * - jpeg_read_header (Header parsing)
 * - jpeg_start_decompress (Decompression start)
 * - jpeg_finish_decompress (Cleanup)
 * - jpeg_destroy_decompress (Memory cleanup)
 * 
 * Invocation sequence based on coverage guidance:
 * 1. Call jpeg_CreateDecompress to initialize decompression object.
 * 2. Call jpeg_mem_src to set up memory-based input source with 12-bit JPEG data.
 * 3. Call jpeg_read_header to parse JPEG headers.
 * 4. Call jpeg_start_decompress to begin decompression.
 * 5. Call jpeg12_read_scanlines for normal scanline reading.
 * 6. Call jpeg12_skip_scanlines to skip scanlines (testing skip functionality).
 * 7. Call jpeg12_crop_scanline to test crop operations.
 * 8. Call jpeg12_read_raw_data for raw data access.
 * 9. Call jpeg_finish_decompress to complete decompression.
 * 10. Call jpeg_destroy_decompress to clean up.
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_001.cpp: 12-bit JPEG handling (TurboJPEG YUV decompression)
 * - This harness: Focuses exclusively on 12-bit scanline manipulation operations
 *                 (read, skip, crop, raw data) in libjpeg decompression
 * 
 * Coverage strategy:
 * 1. Test 12-bit decompression path with memory-based input source
 * 2. Test scanline manipulation operations (read, skip, crop)
 * 3. Test raw data access for 12-bit images
 * 4. Test error handling for invalid scanline operations
 * 5. Test various image dimensions and component counts
 * 
 * Expected Coverage Gain:
 * - Direct coverage of 4 critical 12-bit decompression APIs with 154+ undiscovered branches
 * - Exploration of 12-bit specific code paths in decompression
 * - Testing of scanline manipulation operations not covered by existing harnesses
 * - Memory-based input source testing for 12-bit images
 */

// Include standard headers first as required by jpeglib.h
// Now include jpeglib.h (which depends on size_t and FILE being defined)

// Custom error handler structure for libjpeg
struct fuzzer_error_mgr {
  struct jpeg_error_mgr pub;
  jmp_buf setjmp_buffer;
};

static void fuzzer_error_exit(j_common_ptr cinfo) {
  struct fuzzer_error_mgr *fuzz_err = (struct fuzzer_error_mgr *)cinfo->err;
  longjmp(fuzz_err->setjmp_buffer, 1);
}

static void fuzzer_emit_message(j_common_ptr cinfo, int msg_level) {
  // Suppress warning messages during fuzzing
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  // Minimum input size: need enough for JPEG header + some image data
  // For 12-bit decompression, we need more data than 8-bit
  if (size < 256) return 0;
  
  FuzzedDataProvider fdp(data, size);
  
  // Step 1: Consume JPEG data from fuzzer input
  // We need enough JPEG data for a valid 12-bit JPEG image
  // Ensure we leave at least 64 bytes for other parameters like skip_lines, xoffset, etc.
  size_t max_jpeg_size = size > 64 ? size - 64 : size;
  size_t jpeg_data_size = fdp.ConsumeIntegralInRange<size_t>(128, max_jpeg_size);
  if (jpeg_data_size > fdp.remaining_bytes()) {
    jpeg_data_size = fdp.remaining_bytes();
  }
  std::vector<uint8_t> jpeg_buffer = fdp.ConsumeBytes<uint8_t>(jpeg_data_size);
  
  // Set up error handling
  struct jpeg_decompress_struct cinfo;
  struct fuzzer_error_mgr jerr;
  
  cinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = fuzzer_error_exit;
  jerr.pub.emit_message = fuzzer_emit_message;
  
  // Establish setjmp return context for error handling
  if (setjmp(jerr.setjmp_buffer)) {
    // Error occurred - clean up and return
    // Only destroy if cinfo has been created
    if (cinfo.err != nullptr) {
      jpeg_destroy_decompress(&cinfo);
    }
    return 0;
  }
  
  // Step 1: Initialize decompression object
  jpeg_CreateDecompress(&cinfo, JPEG_LIB_VERSION, sizeof(struct jpeg_decompress_struct));
  
  // Step 2: Set up memory-based input source
  jpeg_mem_src(&cinfo, jpeg_buffer.data(), static_cast<unsigned long>(jpeg_buffer.size()));
  
  // Step 3: Read JPEG header
  if (jpeg_read_header(&cinfo, TRUE) != JPEG_HEADER_OK) {
    jpeg_destroy_decompress(&cinfo);
    return 0;
  }
  
  // Note: After jpeg_read_header(), the library has already initialized
  // cinfo.image_width, cinfo.image_height, cinfo.num_components, 
  // cinfo.jpeg_color_space, and other fields from the JPEG header.
  // Do NOT override these library-initialized fields.
  
  // For 12-bit testing, we need to check if the JPEG has 12-bit precision
  // If not, we can't test 12-bit APIs, so return early
  if (cinfo.data_precision != 12) {
    jpeg_destroy_decompress(&cinfo);
    return 0;
  }
  
  // Step 4: Start decompression
  jpeg_start_decompress(&cinfo);
  
  // Allocate scanline buffer for 12-bit data
  JDIMENSION row_stride = cinfo.output_width * cinfo.output_components;
  J12SAMPARRAY buffer12 = nullptr;
  
  if (cinfo.data_precision == 12) {
    // Use memory manager to allocate 12-bit sample array
    buffer12 = (J12SAMPARRAY)(*cinfo.mem->alloc_sarray)
      ((j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride, 1);
  }
  
  // Step 5: Test jpeg12_read_scanlines
  if (buffer12 && cinfo.data_precision == 12) {
    JDIMENSION lines_read = 0;
    while (cinfo.output_scanline < cinfo.output_height && lines_read < 5) {
      // Read one scanline at a time
      lines_read = jpeg12_read_scanlines(&cinfo, buffer12, 1);
      if (lines_read == 0) {
        break;
      }
    }
  }
  
  // Step 6: Test jpeg12_skip_scanlines
  if (cinfo.data_precision == 12) {
    // Skip some scanlines based on consumed data
    int skip_lines = fdp.ConsumeIntegralInRange<int>(0, 10);
    if (skip_lines > 0 && cinfo.output_scanline + skip_lines < cinfo.output_height) {
      JDIMENSION skipped = jpeg12_skip_scanlines(&cinfo, static_cast<JDIMENSION>(skip_lines));
      // skipped result can be ignored for fuzzing
    }
  }
  
  // Step 7: Test jpeg12_crop_scanline
  if (cinfo.data_precision == 12 && cinfo.output_width > 10) {
    JDIMENSION xoffset = fdp.ConsumeIntegralInRange<JDIMENSION>(0, cinfo.output_width - 10);
    JDIMENSION width = fdp.ConsumeIntegralInRange<JDIMENSION>(10, cinfo.output_width - xoffset);
    
    jpeg12_crop_scanline(&cinfo, &xoffset, &width);
    // After cropping, update row_stride
    row_stride = width * cinfo.output_components;
  }
  
  // Step 8: Test jpeg12_read_raw_data
  if (cinfo.data_precision == 12 && cinfo.raw_data_out) {
    // For raw data, we need JSAMPIMAGE
    // Allocate raw data buffer
    jvirt_barray_ptr *coef_arrays = jpeg_read_coefficients(&cinfo);
    if (coef_arrays) {
      // Raw data requires special setup - skip if not available
      // This is a more advanced operation
    }
  } else if (cinfo.data_precision == 12) {
    // Alternative: Test jpeg12_read_raw_data with downsampled data
    // Need to set up buffers for each component
    int max_v_samp_factor = 0;
    for (int ci = 0; ci < cinfo.num_components; ci++) {
      if (cinfo.comp_info[ci].v_samp_factor > max_v_samp_factor) {
        max_v_samp_factor = cinfo.comp_info[ci].v_samp_factor;
      }
    }
    
    if (max_v_samp_factor > 0) {
      // Allocate raw data buffers
      J12SAMPIMAGE raw_buffer = (J12SAMPIMAGE)(*cinfo.mem->alloc_small)
        ((j_common_ptr)&cinfo, JPOOL_IMAGE,
         cinfo.num_components * sizeof(J12SAMPARRAY));
      
      for (int ci = 0; ci < cinfo.num_components; ci++) {
        int v_samp_factor = cinfo.comp_info[ci].v_samp_factor;
        JDIMENSION comp_height = (cinfo.output_height + v_samp_factor - 1) / v_samp_factor;
        raw_buffer[ci] = (J12SAMPARRAY)(*cinfo.mem->alloc_sarray)
          ((j_common_ptr)&cinfo, JPOOL_IMAGE,
           cinfo.output_width * cinfo.comp_info[ci].h_samp_factor,
           comp_height);
      }
      
      // Try to read raw data
      JDIMENSION lines_read = jpeg12_read_raw_data(&cinfo, raw_buffer, cinfo.output_height);
      // lines_read can be ignored for fuzzing
    }
  }
  
  // Step 9: Finish decompression
  jpeg_finish_decompress(&cinfo);
  
  // Step 10: Clean up
  jpeg_destroy_decompress(&cinfo);
  
  return 0;
}

} // namespace harness_017
