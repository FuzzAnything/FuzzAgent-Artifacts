// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Parameter::TextureScale at tiny_gltf.h:465:10 in tiny_gltf.h
// tinygltf::Parameter::TextureTexCoord at tiny_gltf.h:453:7 in tiny_gltf.h
// tinygltf::Parameter::ColorFactor at tiny_gltf.h:494:14 in tiny_gltf.h
// tinygltf::Parameter::TextureStrength at tiny_gltf.h:477:10 in tiny_gltf.h
// tinygltf::Parameter::TextureIndex at tiny_gltf.h:442:7 in tiny_gltf.h
// tinygltf::Parameter::Factor at tiny_gltf.h:489:10 in tiny_gltf.h
// tinygltf::Parameter::ColorFactor at tiny_gltf.h:494:14 in tiny_gltf.h
// tinygltf::Parameter::Factor at tiny_gltf.h:489:10 in tiny_gltf.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <vector>

#include "tiny_gltf.h"

static void sink(const void* data, size_t size) {
  volatile const char* vdata = static_cast<volatile const char*>(data);
  for (size_t i = 0; i < size; ++i) {
    vdata[i];
  }
}

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
  // Create Parameter object
  tinygltf::Parameter param;
  
  // Initialize number_array with at least 3 values as required by ColorFactor
  param.number_array = {1.0, 1.0, 1.0, 1.0};
  
  // Use fuzzer data to modify the parameter's internal state if possible
  // Since Parameter's internal members are not exposed, we can only call the functions
  // on a default-constructed object. However, we can use the fuzzer input to
  // potentially set some values if the structure allows it.
  
  // We'll interpret the fuzzer data to potentially set some values
  if (Size >= sizeof(double)) {
    double d;
    memcpy(&d, Data, sizeof(double));
    // Use the double value to potentially influence the parameter
    // but since we can't directly set internal fields, we'll just
    // use it to create diversity in function calls
  }
  
  // Call all target API functions
  double textureScale = param.TextureScale();
  sink(&textureScale, sizeof(textureScale));
  
  int textureTexCoord = param.TextureTexCoord();
  sink(&textureTexCoord, sizeof(textureTexCoord));
  
  tinygltf::ColorValue colorFactor = param.ColorFactor();
  sink(&colorFactor, sizeof(colorFactor));
  
  double textureStrength = param.TextureStrength();
  sink(&textureStrength, sizeof(textureStrength));
  
  int textureIndex = param.TextureIndex();
  sink(&textureIndex, sizeof(textureIndex));
  
  double factor = param.Factor();
  sink(&factor, sizeof(factor));
  
  // Try to create some diversity by modifying the number_array based on fuzzer input
  if (Size >= sizeof(double) * 4) {
    // Use fuzzer data to set number_array values
    for (size_t i = 0; i < 4 && i * sizeof(double) < Size; ++i) {
      double val;
      memcpy(&val, Data + i * sizeof(double), sizeof(double));
      if (i < param.number_array.size()) {
        param.number_array[i] = val;
      }
    }
    
    // Call functions again with modified number_array
    colorFactor = param.ColorFactor();
    sink(&colorFactor, sizeof(colorFactor));
    
    factor = param.Factor();
    sink(&factor, sizeof(factor));
  }
  
  return 0;
}