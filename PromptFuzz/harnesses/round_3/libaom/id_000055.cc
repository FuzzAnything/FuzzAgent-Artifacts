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
//<ID> 160
//<Prompt> []
/*<Combination>: [
*/
//<score> 238, nr_unique_branch: 31
//<Quality> {"density":14,"unique_branches":{"get_lr_job_info":[[780,7,780,27,0,0,4,1]],"check_size_argument_overflow":[[33,7,33,64,0,0,4,0]],"aom_memalign":[[57,7,57,52,0,0,4,0]],"cdef_row_mt_sync_read":[[181,10,181,47,0,0,4,0]],"av1_alloc_cdef_buffers":[[244,7,244,27,0,0,4,0],[244,31,244,42,0,0,4,0],[245,9,245,31,0,0,4,1],[253,16,253,31,0,0,4,0],[256,39,256,47,0,0,4,0],[256,39,256,47,0,0,4,1],[283,9,283,29,0,0,4,1]],"sync_read":[[210,12,210,30,0,0,4,1]],"alloc_cdef_row_sync":[[175,7,175,27,0,0,4,0]],"foreach_rest_unit_in_planes_mt":[[943,7,943,27,0,0,4,1],[943,31,943,58,0,0,4,1],[944,7,944,41,0,0,4,1],[944,45,944,77,0,0,4,1]],"lr_sync_read":[[534,12,534,63,0,0,4,0]],"dec_alloc_cb_buf":[[74,9,74,14,2,0,4,0]],"get_next_job_info":[[3181,11,3181,41,0,0,4,1]],"decode_tiles_row_mt":[[3896,33,3896,64,0,0,4,0],[3897,9,3897,31,0,0,4,0],[3898,23,3898,47,0,0,4,0],[3898,23,3898,47,0,0,4,1]],"lr_sync_write":[[556,7,556,22,0,0,4,0],[558,9,558,18,0,0,4,1]],"av1_decode_tg_tiles_and_wrapup":[[5471,13,5471,33,0,0,4,0]],"enqueue_lr_jobs":[[720,31,720,60,0,0,4,1]],"sync_write":[[235,7,235,22,0,0,4,0],[237,9,237,18,0,0,4,0],[237,9,237,18,0,0,4,1]]},"library_calls":["aom_uleb_decode","aom_uleb_encode","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_peek_stream_info","aom_codec_decode","aom_codec_get_frame","aom_img_alloc","aom_img_alloc_with_border","aom_img_plane_width","aom_img_plane_height","aom_img_flip","aom_img_metadata_alloc","aom_img_add_metadata","aom_img_remove_metadata","aom_img_metadata_free","aom_img_free","aom_img_free","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_enc_config_set","aom_codec_get_global_headers","aom_img_alloc","aom_img_plane_height","aom_img_plane_width","aom_codec_encode","aom_codec_get_preview_frame","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_uleb_decode","aom_uleb_encode","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_peek_stream_info","aom_codec_decode","aom_codec_get_frame","aom_img_alloc","aom_img_alloc_with_border","aom_img_plane_width","aom_img_plane_height","aom_img_flip","aom_img_metadata_alloc","aom_img_add_metadata","aom_img_remove_metadata","aom_img_metadata_free","aom_img_free","aom_img_free","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_enc_config_set","aom_codec_get_global_headers","aom_img_alloc","aom_img_plane_height","aom_img_plane_width","aom_codec_encode","aom_codec_get_preview_frame","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_55(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




  if (size < 100) {
    return 0;
  }

  // Create file for input data
  FILE* in_file = fmemopen((void*)data, size, "rb");
  if (!in_file) {
    assert_file_closed(&in_file);
	return 0;
  }

  // Part 1: ULEB operations
  uint64_t uleb_value;
  size_t uleb_length;
  
  // Decode ULEB from beginning of data
  size_t uleb_data_size = size > 20 ? 20 : size;
  int decode_result = aom_uleb_decode(data, uleb_data_size, &uleb_value, &uleb_length);
  
  // Encode ULEB
  uint8_t uleb_buffer[20];
  size_t encoded_size;
  int encode_result = aom_uleb_encode(uleb_value, 20, uleb_buffer, &encoded_size);
  
  // Part 2: Decoder operations
  aom_codec_iface_t* decoder_iface = aom_codec_av1_dx();
  aom_codec_ctx_t decoder;
  aom_codec_dec_cfg_t dec_cfg = {0};
  dec_cfg.threads = 2;
  dec_cfg.allow_lowbitdepth = 1;
  
  // Initialize decoder
  aom_codec_err_t dec_init_result = aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
  
  if (dec_init_result == AOM_CODEC_OK) {
    // Peek stream info
    aom_codec_stream_info_t stream_info = {0};
    aom_codec_peek_stream_info(decoder_iface, data, size, &stream_info);
    
    // Decode the input data
    aom_codec_decode(&decoder, data, size, nullptr);
    
    // Get decoded frames
    aom_codec_iter_t iter = nullptr;
    aom_image_t* decoded_frame = aom_codec_get_frame(&decoder, &iter);
    
    if (decoded_frame) {
      // Part 3: Image manipulation
      // Allocate images with different methods
      aom_image_t* img1 = aom_img_alloc(nullptr, AOM_IMG_FMT_I420, 64, 64, 16);
      aom_image_t* img2 = aom_img_alloc_with_border(nullptr, AOM_IMG_FMT_I420, 64, 64, 16, 32, 8);
      
      if (img1 && img2) {
        // Get plane dimensions
        int plane_width = aom_img_plane_width(decoded_frame, 0);
        int plane_height = aom_img_plane_height(decoded_frame, 0);
        
        // Flip the decoded image
        aom_img_flip(decoded_frame);
        
        // Part 4: Metadata operations
        // Create metadata
        uint32_t metadata_type = 0x1234;
        uint8_t metadata_data[] = {1, 2, 3, 4, 5};
        size_t metadata_sz = sizeof(metadata_data);
        
        aom_metadata_t* metadata = aom_img_metadata_alloc(
            metadata_type, metadata_data, metadata_sz, AOM_MIF_ANY_FRAME);
        
        if (metadata) {
          // Add metadata to image
          aom_img_add_metadata(img1, metadata_type, metadata_data, metadata_sz, AOM_MIF_ANY_FRAME);
          
          // Remove metadata from image
          aom_img_remove_metadata(img1);
          
          // Free metadata
          aom_img_metadata_free(metadata);
        }
      }
      
      // Free allocated images
      if (img1) aom_img_free(img1);
      if (img2) aom_img_free(img2);
    }
  }
  
  // Part 5: Encoder operations
  aom_codec_iface_t* encoder_iface = aom_codec_av1_cx();
  aom_codec_ctx_t encoder;
  aom_codec_enc_cfg_t enc_cfg;
  
  // Get default encoder config
  aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
  
  // Adjust config based on stream info
  enc_cfg.g_w = 64;
  enc_cfg.g_h = 64;
  enc_cfg.g_timebase.num = 1;
  enc_cfg.g_timebase.den = 30;
  enc_cfg.rc_target_bitrate = 1000;
  enc_cfg.g_threads = 2;
  
  // Initialize encoder
  aom_codec_err_t enc_init_result = aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, AOM_ENCODER_ABI_VERSION);
  
  if (enc_init_result == AOM_CODEC_OK) {
    // Set encoder option
    aom_codec_set_option(&encoder, "cpu-used", "4");
    
    // Re-set config
    aom_codec_enc_config_set(&encoder, &enc_cfg);
    
    // Get global headers
    aom_fixed_buf_t* global_headers = aom_codec_get_global_headers(&encoder);
    if (global_headers) {
      free(global_headers->buf);
      free(global_headers);
    }
    
    // Create a test image for encoding
    aom_image_t* test_img = aom_img_alloc(nullptr, AOM_IMG_FMT_I420, 64, 64, 16);
    if (test_img) {
      // Fill image with some data
      for (int plane = 0; plane < 3; plane++) {
        int stride = test_img->stride[plane];
        int height = aom_img_plane_height(test_img, plane);
        int width = aom_img_plane_width(test_img, plane);
        
        for (int y = 0; y < height; y++) {
          uint8_t* row = test_img->planes[plane] + y * stride;
          for (int x = 0; x < width; x++) {
            row[x] = (x + y) % 256;
          }
        }
      }
      
      // Encode the image
      aom_codec_encode(&encoder, test_img, 0, 1, 0);
      
      // Get preview frame
      const aom_image_t* preview = aom_codec_get_preview_frame(&encoder);
      
      // Get encoded data
      aom_codec_iter_t enc_iter = nullptr;
      const aom_codec_cx_pkt_t* pkt;
      while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
        // Process the packet if needed
      }
      
      aom_img_free(test_img);
    }
    
    // Destroy encoder
    aom_codec_destroy(&encoder);
  }
  
  // Cleanup
  if (dec_init_result == AOM_CODEC_OK) {
    aom_codec_destroy(&decoder);
  }
  
  assert_file_closed(&in_file);;
  
  assert_file_closed(&in_file);
	return 0;
}