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
#include <aom/aom.h>
}
extern "C" {
#include <aom/aom_codec.h>
}
extern "C" {
#include <aom/aom_decoder.h>
}
extern "C" {
#include <aom/aom_encoder.h>
}
extern "C" {
#include <aom/aom_ext_ratectrl.h>
}
extern "C" {
#include <aom/aom_external_partition.h>
}
extern "C" {
#include <aom/aom_frame_buffer.h>
}
extern "C" {
#include <aom/aom_image.h>
}
extern "C" {
#include <aom/aomcx.h>
}
extern "C" {
#include <aom/aomdx.h>
}
//<ID> 13
//<Prompt> []
/*<Combination>: [
*/
//<score> 14.666667, nr_unique_branch: 4
//<Quality> {"density":11,"unique_branches":{"aom_img_get_metadata":[[433,7,433,12,0,0,4,1]],"get_cdef_row_next_job":[[1063,7,1063,31,0,0,4,1]],"av1_read_mode_info":[[1578,7,1578,30,0,0,4,1],[1584,9,1584,61,0,0,4,1]]},"library_calls":["aom_codec_av1_dx","aom_codec_iface_name","aom_codec_build_config","aom_codec_dec_init_ver","aom_codec_err_to_string","aom_codec_error","aom_codec_destroy","aom_codec_set_frame_buffer_functions","aom_codec_err_to_string","aom_codec_set_cx_data_buf","aom_codec_err_to_string","aom_codec_decode","aom_codec_err_to_string","aom_codec_error","aom_codec_get_frame","aom_img_get_metadata","aom_img_plane_width","aom_img_plane_height","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_iface_name","aom_codec_build_config","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_err_to_string","aom_codec_set_cx_data_buf","aom_codec_err_to_string","aom_codec_decode","aom_codec_get_frame","aom_img_get_metadata","aom_img_plane_width","aom_img_plane_height","aom_codec_destroy"],"visited":2}
/**/


extern "C" {
}

// Custom frame buffer callback functions
static int  id_000009_get_frame_buffer(void *cb_priv, size_t min_size, 
                           aom_codec_frame_buffer_t *fb) {
  // Allocate frame buffer from input data
  static uint8_t frame_buffer[1024 * 1024]; // 1MB buffer
  fb->data = frame_buffer;
  fb->size = sizeof(frame_buffer);
  return 0;
}

static int  id_000009_release_frame_buffer(void *cb_priv, 
                               aom_codec_frame_buffer_t *fb) {
  // Nothing to release for static buffer
  return 0;
}

// Custom metadata structure for testing
static uint8_t  id_000009_test_metadata_payload[] = {0x01, 0x02, 0x03, 0x04};

extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




  // Early return if insufficient data
  if (size < 100) {
    return 0;
  }

  // Create input file from fuzzer data
  FILE *in_file = fmemopen((void *)data, size, "rb");
  if (!in_file) {
    assert_file_closed(&in_file);
	return 0;
  }

  // Create output file for encoded data
  FILE *out_file = fopen("output_file", "wb");
  if (!out_file) {
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
  }

  // Initialize decoder
  aom_codec_ctx_t decoder;
  aom_codec_dec_cfg_t cfg = {0};
  cfg.threads = 2;
  cfg.w = 640;
  cfg.h = 480;
  cfg.allow_lowbitdepth = 1;

  // Get AV1 decoder interface
  aom_codec_iface_t *iface = aom_codec_av1_dx();
  if (!iface) {
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
  }

  // Get interface name (API usage)
  const char *iface_name = aom_codec_iface_name(iface);
  if (iface_name) {
    // Can log if needed: fprintf(stderr, "Using interface: %s\n", iface_name);
  }

  // Get build config (API usage)
  const char *build_config = aom_codec_build_config();
  if (build_config) {
    // Can log if needed: fprintf(stderr, "Build config: %s\n", build_config);
  }

  // Initialize decoder
  aom_codec_err_t init_result = aom_codec_dec_init_ver(&decoder, iface, &cfg, 
                                                      0, AOM_DECODER_ABI_VERSION);
  
  // Check initialization error using error APIs
  if (init_result != AOM_CODEC_OK) {
    const char *error_str = aom_codec_err_to_string(init_result);
    const char *error_detail = aom_codec_error(&decoder);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    aom_codec_destroy(&decoder);
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
  }

  // Set frame buffer functions (API usage)
  aom_codec_err_t buffer_result = aom_codec_set_frame_buffer_functions(
      &decoder,  id_000009_get_frame_buffer,  id_000009_release_frame_buffer, NULL);
  
  if (buffer_result != AOM_CODEC_OK) {
    const char *error_str = aom_codec_err_to_string(buffer_result);
    // Continue anyway
  }

  // Create a fixed buffer for compressed data
  aom_fixed_buf_t fixed_buf;
  size_t buf_size = size > 10000 ? 10000 : size;
  fixed_buf.buf = malloc(buf_size);
  fixed_buf.sz = buf_size;
  
  if (fixed_buf.buf) {
    // Copy part of input data to buffer
    memcpy(fixed_buf.buf, data, buf_size);
    
    // Set compressed data buffer (API usage)
    aom_codec_err_t set_buf_result = aom_codec_set_cx_data_buf(
        &decoder, &fixed_buf, 16, 16); // 16 byte padding
    
    if (set_buf_result != AOM_CODEC_OK) {
      const char *error_str = aom_codec_err_to_string(set_buf_result);
      // Continue anyway
    }
    
    free(fixed_buf.buf);
  }

  // Read and decode data from input file
  fseek(in_file, 0, SEEK_SET);
  uint8_t *read_buffer = (uint8_t *)malloc(4096);
  if (read_buffer) {
    size_t bytes_read = fread(read_buffer, 1, 4096, in_file);
    
    if (bytes_read > 0) {
      // Decode the data
      aom_codec_err_t decode_result = aom_codec_decode(&decoder, read_buffer, 
                                                      bytes_read, NULL);
      
      if (decode_result != AOM_CODEC_OK) {
        const char *error_str = aom_codec_err_to_string(decode_result);
        const char *error_detail = aom_codec_error(&decoder);
      } else {
        // Get decoded frames
        aom_codec_iter_t iter = NULL;
        aom_image_t *img;
        while ((img = aom_codec_get_frame(&decoder, &iter)) != NULL) {
          // Get metadata from image (API usage)
          const aom_metadata_t *metadata = aom_img_get_metadata(img, 0);
          if (metadata) {
            // Metadata exists - can access if needed
            // metadata->type, metadata->payload, metadata->sz
          }
          
          // Write image data to output file
          for (int plane = 0; plane < 3; plane++) {
            int plane_width = aom_img_plane_width(img, plane);
            int plane_height = aom_img_plane_height(img, plane);
            if (plane_width > 0 && plane_height > 0) {
              size_t plane_size = plane_width * plane_height;
              if (plane_size > 0) {
                fwrite(img->planes[plane], 1, 
                      plane_size < 1024 ? plane_size : 1024, out_file);
              }
            }
          }
        }
      }
    }
    
    free(read_buffer);
  }

  // Get file descriptors (API requirement)
  int in_fd = fuzz_fileno(in_file);
  int out_fd = fuzz_fileno(out_file);
  
  // Use file descriptors minimally
  if (in_fd >= 0 && out_fd >= 0) {
    // Can perform file operations if needed
  }

  // Clean up
  aom_codec_destroy(&decoder);
  assert_file_closed(&in_file);;
  assert_file_closed(&out_file);;
  
  // Remove output file
  remove("output_file");

  assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}