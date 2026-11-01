// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "tiny_gltf.h"

// Fuzz driver for tinygltf JSON utility functions
// Targets: JsonIsNull, GetNumber, IsObject, ParseJsonAsValue, IsArray, GetInt

extern "C" int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size) {
  if (Size == 0) {
    return 0;
  }

  const char* dummy_file = "./dummy_file";
  
  // Write input to file
  FILE* f = fopen(dummy_file, "wb");
  if (f) {
    fwrite(Data, 1, Size, f);
    fclose(f);
    
    tinygltf::Model model;
    tinygltf::TinyGLTF ctx;
    std::string err;
    std::string warn;

    // Load from file. This internally parses JSON and calls the target functions:
    // JsonIsNull, GetNumber, IsObject, ParseJsonAsValue, IsArray, GetInt
    // We use LoadASCIIFromFile which is public and triggers the JSON parsing logic.
    ctx.LoadASCIIFromFile(&model, &err, &warn, dummy_file);
    
    // Clean up model
    model = tinygltf::Model();
  }

  return 0;
}