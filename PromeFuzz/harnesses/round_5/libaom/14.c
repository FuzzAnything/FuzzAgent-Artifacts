// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_version_extra_str at aom_codec.c:30:13 in aom_codec.h
// aom_codec_version at aom_codec.c:26:5 in aom_codec.h
// aom_codec_build_config at aom_config.c:13:13 in aom_codec.h
// aom_codec_version_str at aom_codec.c:28:13 in aom_codec.h
// aom_codec_iface_name at aom_codec.c:32:13 in aom_codec.h
// aom_obu_type_to_string at aom_codec.c:186:13 in aom_codec.h
// aom_obu_type_to_string at aom_codec.c:186:13 in aom_codec.h
// aom_obu_type_to_string at aom_codec.c:186:13 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_codec.h"

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
  // Function 1: aom_codec_version_extra_str
  const char *extra_str = aom_codec_version_extra_str();
  (void)extra_str; // Avoid unused variable warning
  
  // Function 2: aom_codec_version
  int version = aom_codec_version();
  (void)version; // Avoid unused variable warning
  
  // Function 3: aom_codec_build_config
  const char *build_config = aom_codec_build_config();
  (void)build_config; // Avoid unused variable warning
  
  // Function 4: aom_codec_version_str
  const char *version_str = aom_codec_version_str();
  (void)version_str; // Avoid unused variable warning
  
  // Function 5: aom_codec_iface_name
  // Test with NULL pointer (should handle gracefully)
  const char *null_name = aom_codec_iface_name(NULL);
  (void)null_name;
  
  // Function 6: aom_obu_type_to_string
  // Use fuzzer input to generate various OBU_TYPE values
  if (Size > 0) {
    // Generate OBU_TYPE values from input bytes
    for (size_t i = 0; i < Size && i < 32; i++) {
      OBU_TYPE obu_type = (OBU_TYPE)Data[i];
      const char *obu_str = aom_obu_type_to_string(obu_type);
      (void)obu_str;
    }
    
    // Also test with some boundary values
    const char *min_str = aom_obu_type_to_string((OBU_TYPE)0);
    (void)min_str;
    
    const char *max_str = aom_obu_type_to_string((OBU_TYPE)255);
    (void)max_str;
  }
  
  return 0;
}