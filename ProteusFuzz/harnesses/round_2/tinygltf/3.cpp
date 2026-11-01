// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetImageLoader at tiny_gltf.h:1473:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImageWriter at tiny_gltf.h:1483:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
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

// Custom image loader callback for fuzzing
// Signature must match LoadImageDataFunction exactly:
// bool (tinygltf::Image *image, int index, const std::string *uri, std::string *err, std::string *warn,
//       int width, int height, const unsigned char *data, int data_length, void *user_data)
// Note: The error indicates the expected signature might differ slightly in parameter types or order.
// Based on the error, the expected signature is:
// bool (tinygltf::Image *, const int, std::string *, std::string *, int, int, const unsigned char *, int, void *)
// Wait, the error says:
// 'bool (tinygltf::Image *, int, const std::string *, std::string *, std::string *, int, int, const unsigned char *, int, void *)'
// vs expected:
// 'bool (tinygltf::Image *, const int, std::string *, std::string *, int, int, const unsigned char *, int, void *)'
// The difference is:
// 1. 'const int' vs 'int' for index (usually not the issue)
// 2. 'const std::string *' vs 'std::string *' for uri (my code has const, expected has non-const? No, error says expected has std::string * for uri? Let's re-read)
// Error: expected 'function<bool (tinygltf::Image *, const int, basic_string<char> *, basic_string<char> *, int, int, const unsigned char *, int, void *)>'
// My function: 'bool (tinygltf::Image *, int, const std::string *, std::string *, std::string *, int, int, const unsigned char *, int, void *)'
// Differences:
// - My function has 3 string pointers (uri, err, warn). Expected has 2 string pointers.
// - My function has 'const std::string *' for uri. Expected has 'std::string *' (non-const) for the first string pointer?
// - My function has 'std::string *' for err. Expected has 'std::string *' for the second string pointer.
// - My function has 'std::string *' for warn. Expected has NO third string pointer.
//
// Let's look at the error message again carefully.
// Expected: bool (tinygltf::Image *, const int, basic_string<char> *, basic_string<char> *, int, int, const unsigned char *, int, void *)
// Count: Image*, int, string*, string*, int, int, uchar*, int, void* -> 9 args.
// My function: Image*, int, const string*, string*, string*, int, int, uchar*, int, void* -> 10 args.
//
// It seems the version of tinygltf being used has a different signature for LoadImageDataFunction.
// The error message shows the expected signature has only 2 string pointers, not 3.
// Let's assume the signature is:
// bool LoadImageDataFunction(tinygltf::Image *image, int index, const std::string *uri, std::string *err, std::string *warn, int width, int height, const unsigned char *data, int data_length, void *user_data);
// Wait, the error says the expected type is:
// function<bool (tinygltf::Image *, const int, basic_string<char> *, basic_string<char> *, int, int, const unsigned char *, int, void *)>
// This implies the signature is:
// bool (Image*, const int, string*, string*, int, int, uchar*, int, void*)
// This is 9 arguments.
// My function has 10 arguments.
//
// Let's check the tinygltf.h header provided in the prompt? No, the prompt only gives struct definitions.
// However, the error message is the source of truth for the signature expected by the compiler.
// Expected:
// 1. tinygltf::Image *
// 2. const int
// 3. std::string *
// 4. std::string *
// 5. int
// 6. int
// 7. const unsigned char *
// 8. int
// 9. void *
//
// My function:
// 1. tinygltf::Image *
// 2. int
// 3. const std::string *
// 4. std::string *
// 5. std::string *
// 6. int
// 7. int
// 8. const unsigned char *
// 9. int
// 10. void *
//
// The mismatch is clear. The expected signature has 2 string pointers, mine has 3.
// Also, the first string pointer in expected is non-const, mine is const.
//
// Let's try to match the expected signature exactly as per the error message.
// Expected: bool (tinygltf::Image *, const int, std::string *, std::string *, int, int, const unsigned char *, int, void *)
//
// Let's redefine FuzzImageLoader to match this.
// Note: The 'const int' is usually just 'int' in practice, but we can use 'int' and it should convert.
// The key is the number of arguments and the types of the string pointers.
//
// Hypothesis: The version of tinygltf used here has a signature like:
// bool LoadImageDataFunction(tinygltf::Image *image, int index, const std::string *uri, std::string *err, std::string *warn, int width, int height, const unsigned char *data, int data_length, void *user_data);
// Wait, if the expected signature in the error is missing 'warn', then maybe the signature is:
// bool LoadImageDataFunction(tinygltf::Image *image, int index, const std::string *uri, std::string *err, int width, int height, const unsigned char *data, int data_length, void *user_data);
// But the error says 'basic_string<char> *, basic_string<char> *' which are two string pointers.
// If the first is 'uri' and the second is 'err', where is 'warn'?
// Maybe 'warn' is not part of the signature in this version?
// Or maybe the signature is:
// bool LoadImageDataFunction(tinygltf::Image *image, int index, const std::string *uri, std::string *err, std::string *warn, int width, int height, const unsigned char *data, int data_length, void *user_data);
// And the error message is truncating or misrepresenting?
// No, the error message is explicit about the types.
//
// Let's look at the error again:
// 'no viable conversion from 'bool (tinygltf::Image *, int, const std::string *, std::string *, std::string *, int, int, const unsigned char *, int, void *)' ... to 'function<bool (tinygltf::Image *, const int, basic_string<char> *, basic_string<char> *, int, int, const unsigned char *, int, void *)>'
//
// The expected function type has 9 parameters.
// The provided function type has 10 parameters.
//
// Let's assume the correct signature for this version of tinygltf is:
// bool LoadImageDataFunction(tinygltf::Image *image, int index, const std::string *uri, std::string *err, std::string *warn, int width, int height, const unsigned char *data, int data_length, void *user_data);
// But the error says the expected type has only 2 string pointers.
//
// Alternative: Maybe the signature is:
// bool LoadImageDataFunction(tinygltf::Image *image, int index, const std::string *uri, std::string *err, int width, int height, const unsigned char *data, int data_length, void *user_data);
// This would be 9 parameters: Image*, int, string*, string*, int, int, uchar*, int, void*.
// This matches the count in the error message (9 args).
//
// Let's try this signature.
//
// Also, the error says the first string pointer is 'basic_string<char> *' (non-const) in the expected type, but I have 'const std::string *'.
// Let's make it 'std::string *' to match.

static bool FuzzImageLoader(tinygltf::Image *image, int index, std::string *uri, std::string *err, int width, int height, const unsigned char *data, int data_length, void *user_data) {
  if (image) {
    image->width = width;
    image->height = height;
    if (data && data_length > 0) {
      image->image.assign(data, data + data_length);
    } else {
      image->image.resize(4);
      image->image[0] = 0xFF;
      image->image[1] = 0xFF;
      image->image[2] = 0xFF;
      image->image[3] = 0xFF;
    }
    image->as_is = false;
  }
  return true;
}

// Custom image writer callback for fuzzing
// Signature matches WriteImageDataFunction
static bool FuzzImageWriter(const std::string *base_path, const std::string *image_filename, const tinygltf::Image *image, bool embed, const tinygltf::FsCallbacks *fs, const tinygltf::URICallbacks *uri_cb, std::string *ext, void *user_data) {
  if (base_path && image_filename && image) {
    std::string full_path = *base_path + *image_filename;
    FILE *f = fopen(full_path.c_str(), "wb");
    if (f) {
      fwrite(image->image.data(), 1, image->image.size(), f);
      fclose(f);
      return true;
    }
  }
  return false;
}

extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
  if (Size == 0) return 0;

  tinygltf::TinyGLTF ctx;
  tinygltf::Model model;
  std::string err;
  std::string warn;

  // Step 1: SetImageLoader
  // Using the corrected signature
  ctx.SetImageLoader(FuzzImageLoader, nullptr);

  // Step 2: LoadASCIIFromFile
  const char *dummy_file = "./dummy_file.gltf";
  FILE *f = fopen(dummy_file, "wb");
  if (f) {
    fwrite(Data, 1, Size, f);
    fclose(f);
  } else {
    return 0;
  }

  bool loadSuccess = ctx.LoadASCIIFromFile(&model, &err, &warn, dummy_file);
  (void)loadSuccess;

  // Step 3: SetImageWriter
  ctx.SetImageWriter(FuzzImageWriter, nullptr);

  // Step 4: WriteGltfSceneToFile
  const char *output_file = "./output.gltf";
  bool writeSuccess = ctx.WriteGltfSceneToFile(&model, output_file,
                                               false, // embedImages
                                               false, // embedBuffers
                                               true,  // prettyPrint
                                               false  // writeBinary
  );
  (void)writeSuccess;

  // Cleanup
  std::remove(dummy_file);
  std::remove(output_file);

  return 0;
}