#include <fuzzer/FuzzedDataProvider.h>
#include "tiny_gltf.h"
#include <vector>
#include <map>
#include <functional>
#include <sstream>
#include <memory>

namespace harness_003 {
/*
 * Fuzzing harness for tinygltf library writing/export and advanced image I/O operations
 * 
 * This harness targets GLTF writing/export operations and stb_image integration:
 * 1. WriteGltfSceneToStream - GLTF stream writing (140 undiscovered branches)
 * 2. WriteGltfSceneToFile - GLTF file writing (complement to harness_001)
 * 3. stb_image.h functions - Direct image loading (0% coverage)
 * 4. stb_image_write.h functions - Direct image writing (0% coverage)
 * 5. Complete image lifecycle with data URI handling
 * 
 * The harness implements a comprehensive workflow: load model, write to stream/file,
 * process images with stb_image directly, and test the full export pipeline.
 */


// Forward declarations for stb_image functions (already defined in tinygltf library)
extern "C" {
    // stb_image types
    typedef unsigned short stbi_us;
    
    // stb_image functions
    extern int stbi_info_from_memory(const unsigned char* buffer, int len, int* x, int* y, int* comp);
    extern unsigned char* stbi_load_from_memory(const unsigned char* buffer, int len, int* x, int* y, int* comp, int req_comp);
    extern float* stbi_loadf_from_memory(const unsigned char* buffer, int len, int* x, int* y, int* comp, int req_comp);
    extern stbi_us* stbi_load_16_from_memory(const unsigned char* buffer, int len, int* x, int* y, int* comp, int req_comp);
    extern void stbi_image_free(void* retval_from_stbi_load);
    
    // stb_image_write functions  
    extern int stbi_write_png_to_func(void (*func)(void*, void*, int), void* context,
                                     int w, int h, int comp, const void* data, int stride_in_bytes);
    extern int stbi_write_bmp_to_func(void (*func)(void*, void*, int), void* context,
                                     int w, int h, int comp, const void* data);
    extern int stbi_write_hdr_to_func(void (*func)(void*, void*, int), void* context,
                                     int w, int h, int comp, const float* data);
}

// Mock filesystem for fuzzing - based on harness_001 but enhanced for stream testing
class MockStreamFileSystem {
public:
    struct FileInfo {
        std::vector<unsigned char> data;
        bool exists;
    };
    
    std::map<std::string, FileInfo> files;
    std::map<std::string, std::stringstream> streams;
    void* user_data;
    
    MockStreamFileSystem() : user_data(nullptr) {}
    
    // FileExists callback implementation
    static bool MockFileExists(const std::string& abs_filename, void* user_data) {
        MockStreamFileSystem* fs = static_cast<MockStreamFileSystem*>(user_data);
        if (!fs) return false;
        
        auto it = fs->files.find(abs_filename);
        return it != fs->files.end() && it->second.exists;
    }
    
    // ExpandFilePath callback implementation (no-op for mocking)
    static std::string MockExpandFilePath(const std::string& filepath, void* user_data) {
        // Return path as-is for mocking
        return filepath;
    }
    
    // ReadWholeFile callback implementation
    static bool MockReadWholeFile(std::vector<unsigned char>* out, 
                                  std::string* err, 
                                  const std::string& filepath, 
                                  void* user_data) {
        MockStreamFileSystem* fs = static_cast<MockStreamFileSystem*>(user_data);
        if (!fs) {
            if (err) *err = "No filesystem context";
            return false;
        }
        
        auto it = fs->files.find(filepath);
        if (it == fs->files.end() || !it->second.exists) {
            if (err) *err = "File not found: " + filepath;
            return false;
        }
        
        *out = it->second.data;
        return true;
    }
    
    // WriteWholeFile callback implementation  
    static bool MockWriteWholeFile(std::string* err, 
                                   const std::string& filepath,
                                   const std::vector<unsigned char>& contents,
                                   void* user_data) {
        MockStreamFileSystem* fs = static_cast<MockStreamFileSystem*>(user_data);
        if (!fs) {
            if (err) *err = "No filesystem context";
            return false;
        }
        
        FileInfo info;
        info.data = contents;
        info.exists = true;
        fs->files[filepath] = info;
        return true;
    }
    
    // GetFileSizeInBytes callback implementation
    static bool MockGetFileSizeInBytes(size_t* filesize_out,
                                       std::string* err,
                                       const std::string& filepath,
                                       void* user_data) {
        MockStreamFileSystem* fs = static_cast<MockStreamFileSystem*>(user_data);
        if (!fs) {
            if (err) *err = "No filesystem context";
            return false;
        }
        
        auto it = fs->files.find(filepath);
        if (it == fs->files.end() || !it->second.exists) {
            if (err) *err = "File not found: " + filepath;
            return false;
        }
        
        *filesize_out = it->second.data.size();
        return true;
    }
    
    // Setup filesystem callbacks
    tinygltf::FsCallbacks GetFsCallbacks() {
        tinygltf::FsCallbacks callbacks;
        callbacks.FileExists = MockFileExists;
        callbacks.ExpandFilePath = MockExpandFilePath;
        callbacks.ReadWholeFile = MockReadWholeFile;
        callbacks.WriteWholeFile = MockWriteWholeFile;
        callbacks.GetFileSizeInBytes = MockGetFileSizeInBytes;
        callbacks.user_data = this;
        return callbacks;
    }
};

// Mock URI callbacks for fuzzing
class MockURISystem {
public:
    void* user_data;
    
    MockURISystem() : user_data(nullptr) {}
    
    // Decode callback implementation
    static bool MockURIDecode(const std::string& in_uri, std::string* out_uri, 
                              void* user_data) {
        // Simple implementation: pass through
        *out_uri = in_uri;
        return true;
    }
    
    // Encode callback implementation
    static bool MockURIEncode(const std::string& in_uri, const std::string& asset_type,
                              std::string* out_uri, void* user_data) {
        // Simple implementation: pass through
        *out_uri = in_uri;
        return true;
    }
    
    // Get URICallbacks structure
    tinygltf::URICallbacks GetURICallbacks() {
        tinygltf::URICallbacks callbacks;
        callbacks.decode = MockURIDecode;
        callbacks.encode = MockURIEncode;
        callbacks.user_data = this;
        return callbacks;
    }
};

// Custom image loader using stb_image directly
static bool CustomImageLoader(tinygltf::Image* image, const int image_idx, std::string* err,
                             std::string* warn, int req_width, int req_height,
                             const unsigned char* bytes, int size, void* user_data) {
    (void)image_idx;
    (void)err;
    (void)warn;
    (void)req_width;
    (void)req_height;
    (void)user_data;
    
    if (!bytes || size <= 0) return false;
    
    // Use stb_image to load the image
    int width, height, channels;
    unsigned char* data = stbi_load_from_memory(bytes, size, &width, &height, &channels, 0);
    if (!data) {
        // Try other stb_image variants
        float* float_data = stbi_loadf_from_memory(bytes, size, &width, &height, &channels, 0);
        if (!float_data) {
            stbi_us* ushort_data = stbi_load_16_from_memory(bytes, size, &width, &height, &channels, 0);
            if (!ushort_data) {
                return false;
            }
            
            // Convert 16-bit to 8-bit for tinygltf
            image->width = width;
            image->height = height;
            image->component = channels;
            image->bits = 16;
            image->pixel_type = TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT;
            image->image.resize(width * height * channels * 2);
            memcpy(image->image.data(), ushort_data, width * height * channels * 2);
            stbi_image_free(ushort_data);
            return true;
        }
        
        // Handle float data
        image->width = width;
        image->height = height;
        image->component = channels;
        image->bits = 32;
        image->pixel_type = TINYGLTF_COMPONENT_TYPE_FLOAT;
        image->image.resize(width * height * channels * 4);
        memcpy(image->image.data(), float_data, width * height * channels * 4);
        stbi_image_free(float_data);
        return true;
    }
    
    // Regular 8-bit image
    image->width = width;
    image->height = height;
    image->component = channels;
    image->bits = 8;
    image->pixel_type = TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE;
    image->image.resize(width * height * channels);
    memcpy(image->image.data(), data, width * height * channels);
    stbi_image_free(data);
    return true;
}

// Custom image writer using stb_image_write directly
static bool CustomImageWriter(const std::string* basepath, const std::string* filename,
                             const tinygltf::Image* image, bool embedImages,
                             const tinygltf::FsCallbacks* fs,
                             const tinygltf::URICallbacks* uri_cb,
                             std::string* out_uri, void* user_data) {
    (void)basepath;
    (void)embedImages;
    (void)fs;
    (void)uri_cb;
    (void)user_data;
    
    if (!image || image->image.empty() || !filename) return false;
    
    // Only write PNG for now (simplest)
    std::string full_path = *filename;
    
    // Create mock file data using stb_image_write
    std::vector<unsigned char> png_data;
    
    // Use a lambda to capture the data
    auto write_func = [](void* context, void* data, int size) {
        std::vector<unsigned char>* vec = static_cast<std::vector<unsigned char>*>(context);
        vec->insert(vec->end(), static_cast<unsigned char*>(data), 
                   static_cast<unsigned char*>(data) + size);
    };
    
    int success = 0;
    if (image->bits == 8 && image->component >= 1 && image->component <= 4) {
        success = stbi_write_png_to_func(write_func, &png_data, 
                                        image->width, image->height, 
                                        image->component, 
                                        image->image.data(), 
                                        image->width * image->component);
    }
    
    if (success && out_uri) {
        *out_uri = full_path;
    }
    
    // Also test other stb_image_write functions
    if (image->bits == 8 && image->component == 3) {
        // Test BMP writing
        std::vector<unsigned char> bmp_data;
        stbi_write_bmp_to_func(write_func, &bmp_data,
                              image->width, image->height,
                              image->component,
                              image->image.data());
    }
    
    if (image->bits == 32 && image->component == 3) {
        // Test HDR writing (float data)
        std::vector<unsigned char> hdr_data;
        stbi_write_hdr_to_func(write_func, &hdr_data,
                              image->width, image->height,
                              image->component,
                              reinterpret_cast<const float*>(image->image.data()));
    }
    
    return success != 0;
}

// Helper to create a simple glTF model for testing
static void CreateTestModel(tinygltf::Model& model, FuzzedDataProvider& fdp) {
    model = tinygltf::Model();
    
    // Add a simple scene
    tinygltf::Scene scene;
    scene.name = "TestScene";
    model.scenes.push_back(scene);
    model.defaultScene = 0;
    
    // Add a node
    tinygltf::Node node;
    node.name = "TestNode";
    model.nodes.push_back(node);
    
    // Add a mesh
    tinygltf::Mesh mesh;
    mesh.name = "TestMesh";
    
    // Add a primitive with some vertices
    tinygltf::Primitive primitive;
    primitive.mode = TINYGLTF_MODE_TRIANGLES;
    
    // Create a simple buffer with vertex data
    tinygltf::Buffer buffer;
    size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(64, 1024);
    buffer.data.resize(buffer_size);
    for (size_t i = 0; i < buffer_size; i++) {
        buffer.data[i] = fdp.ConsumeIntegral<uint8_t>();
    }
    model.buffers.push_back(buffer);
    
    // Create buffer view
    tinygltf::BufferView buffer_view;
    buffer_view.buffer = 0;
    buffer_view.byteOffset = 0;
    buffer_view.byteLength = buffer.data.size();
    model.bufferViews.push_back(buffer_view);
    
    // Create accessor for positions
    tinygltf::Accessor accessor;
    accessor.bufferView = 0;
    accessor.byteOffset = 0;
    accessor.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
    accessor.count = 3;  // 3 vertices
    accessor.type = TINYGLTF_TYPE_VEC3;
    model.accessors.push_back(accessor);
    
    primitive.attributes["POSITION"] = 0;
    mesh.primitives.push_back(primitive);
    model.meshes.push_back(mesh);
    
    // Add images based on fuzzer input
    int num_images = fdp.ConsumeIntegralInRange<int>(0, 5);
    for (int i = 0; i < num_images; i++) {
        tinygltf::Image image;
        image.name = "Image_" + std::to_string(i);
        image.width = fdp.ConsumeIntegralInRange<int>(1, 64);
        image.height = fdp.ConsumeIntegralInRange<int>(1, 64);
        image.component = fdp.ConsumeIntegralInRange<int>(1, 4);
        image.bits = 8;
        image.pixel_type = TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE;
        
        // Create some image data
        size_t image_size = image.width * image.height * image.component;
        image.image.resize(image_size);
        for (size_t j = 0; j < image_size; j++) {
            image.image[j] = fdp.ConsumeIntegral<uint8_t>();
        }
        
        // Decide between data URI or external file
        bool use_data_uri = fdp.ConsumeBool();
        if (use_data_uri) {
            // Create a simple data URI
            image.uri = "data:image/png;base64," + 
                       std::string(image_size > 0 ? "test" : "");
            image.mimeType = "image/png";
        } else {
            image.uri = "images/texture_" + std::to_string(i) + ".png";
        }
        
        model.images.push_back(image);
    }
    
    // Add a material referencing the first image if available
    if (!model.images.empty()) {
        tinygltf::Material material;
        material.name = "TestMaterial";
        
        tinygltf::Texture texture;
        texture.source = 0;
        model.textures.push_back(texture);
        
        tinygltf::Parameter baseColorTexture;
        baseColorTexture.json_double_value["index"] = 0.0;
        material.values["baseColorTexture"] = baseColorTexture;
        
        model.materials.push_back(material);
    }
}
// Test stb_image functions directly
static void TestStbImageFunctions(FuzzedDataProvider& fdp) {
    // Consume some bytes for image testing
    size_t image_data_size = fdp.ConsumeIntegralInRange<size_t>(0, 1024);
    std::vector<uint8_t> image_data = fdp.ConsumeBytes<uint8_t>(image_data_size);
    
    if (!image_data.empty()) {
        // Test stbi_info_from_memory
        int width, height, channels;
        int has_info = stbi_info_from_memory(image_data.data(), 
                                           static_cast<int>(image_data.size()),
                                           &width, &height, &channels);
        (void)has_info;
        
        // Test stbi_load with different configurations
        if (image_data.size() > 16) {
            unsigned char* data = stbi_load_from_memory(image_data.data(),
                                                       static_cast<int>(image_data.size()),
                                                       &width, &height, &channels, 0);
            if (data) {
                stbi_image_free(data);
            }
            
            // Test float loading
            float* float_data = stbi_loadf_from_memory(image_data.data(),
                                                      static_cast<int>(image_data.size()),
                                                      &width, &height, &channels, 0);
            if (float_data) {
                stbi_image_free(float_data);
            }
            
            // Test 16-bit loading
            stbi_us* ushort_data = stbi_load_16_from_memory(image_data.data(),
                                                           static_cast<int>(image_data.size()),
                                                           &width, &height, &channels, 0);
            if (ushort_data) {
                stbi_image_free(ushort_data);
            }
        }
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum size for meaningful testing
    if (size < 32) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize TinyGLTF with custom callbacks
    tinygltf::TinyGLTF ctx;
    std::string err, warn;
    
    // Set up mock filesystem and URI system
    MockStreamFileSystem mock_fs;
    MockURISystem mock_uri;
    tinygltf::FsCallbacks fs_callbacks = mock_fs.GetFsCallbacks();
    tinygltf::URICallbacks uri_callbacks = mock_uri.GetURICallbacks();
    
    if (!ctx.SetFsCallbacks(fs_callbacks, &err)) {
        return 0; // Failed to set filesystem callbacks
    }
    
    if (!ctx.SetURICallbacks(uri_callbacks, &err)) {
        return 0; // Failed to set URI callbacks
    }
    // Set up custom image loader and writer
    ctx.SetImageLoader(CustomImageLoader, nullptr);
    ctx.SetImageWriter(CustomImageWriter, nullptr);
    
    // Test stb_image functions directly first
    TestStbImageFunctions(fdp);
    
    // Consume operation selector
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>();
    
    // Create a test model
    tinygltf::Model model;
    CreateTestModel(model, fdp);
    
    // Test different writing/export operations
    switch (operation % 6) {
        case 0: {
            // Test WriteGltfSceneToStream with stringstream
            std::stringstream gltf_stream;
            bool pretty_print = fdp.ConsumeBool();
            bool write_binary = fdp.ConsumeBool();
            
            bool stream_result = ctx.WriteGltfSceneToStream(&model, gltf_stream, 
                                                           pretty_print, write_binary);
            (void)stream_result;
            
            // Also test writing to file for comparison
            std::string output_filename = "test_output.gltf";
            bool embed_images = fdp.ConsumeBool();
            bool embed_buffers = fdp.ConsumeBool();
            
            bool file_result = ctx.WriteGltfSceneToFile(&model, output_filename,
                                                       embed_images, embed_buffers,
                                                       pretty_print, write_binary);
            (void)file_result;
            break;
        }
        
        case 1: {
            // Test WriteGltfSceneToFile with various configurations
            std::string output_filename = "test_output";
            
            // Try different extensions
            uint8_t ext_selector = fdp.ConsumeIntegral<uint8_t>() % 3;
            if (ext_selector == 0) output_filename += ".gltf";
            else if (ext_selector == 1) output_filename += ".glb";
            else output_filename += ".json";
            
            bool embed_images = fdp.ConsumeBool();
            bool embed_buffers = fdp.ConsumeBool();
            bool pretty_print = fdp.ConsumeBool();
            bool write_binary = fdp.ConsumeBool();
            
            bool result = ctx.WriteGltfSceneToFile(&model, output_filename,
                                                  embed_images, embed_buffers,
                                                  pretty_print, write_binary);
            (void)result;
            break;
        }
        
        case 2: {
            // Test image extraction and WriteImageData
            if (!model.images.empty()) {
                for (const auto& image : model.images) {
                    // Test IsDataURI
                    bool is_data_uri = tinygltf::IsDataURI(image.uri);
                    (void)is_data_uri;
                    
                    // Test DecodeDataURI if applicable
                    if (is_data_uri) {
                        std::vector<unsigned char> decoded_data;
                        std::string mime_type;
                        bool decode_result = tinygltf::DecodeDataURI(&decoded_data, mime_type,
                                                                    image.uri, 0, false);
                        (void)decode_result;
                    }
                    
                    // Test WriteImageData with custom writer
                    if (!image.image.empty()) {
                        std::string out_uri;
                        std::string basepath = ".";
                        std::string filename = "exported_" + image.name + ".png";
                        
                        bool write_result = tinygltf::WriteImageData(&basepath, &filename, &image,
                                                                     false, &fs_callbacks,
                                                                     &uri_callbacks, &out_uri, nullptr);
                    }
                }
            }
            break;
        }
        
        case 3: {
            // Test complete round-trip: write to stream, then parse it back
            std::stringstream gltf_stream;
            bool write_binary = fdp.ConsumeBool();
            
            bool write_result = ctx.WriteGltfSceneToStream(&model, gltf_stream, 
                                                          true, write_binary);
            if (write_result) {
                std::string gltf_data = gltf_stream.str();
                
                // Try to parse it back
                tinygltf::Model parsed_model;
                std::string parse_err, parse_warn;
                
                if (write_binary) {
                    // For binary GLB
                    bool parse_result = ctx.LoadBinaryFromMemory(&parsed_model, &parse_err, &parse_warn,
                                                                reinterpret_cast<const unsigned char*>(gltf_data.data()),
                                                                static_cast<unsigned int>(gltf_data.size()),
                                                                "", 0);
                    (void)parse_result;
                } else {
                    // For ASCII GLTF
                    bool parse_result = ctx.LoadASCIIFromString(&parsed_model, &parse_err, &parse_warn,
                                                               gltf_data.c_str(),
                                                               static_cast<unsigned int>(gltf_data.size()),
                                                               "", 0);
                    (void)parse_result;
                }
            }
            break;
        }
        
        case 4: {
            // Test with different serialization options
            ctx.SetSerializeDefaultValues(fdp.ConsumeBool());
            ctx.SetStoreOriginalJSONForExtrasAndExtensions(fdp.ConsumeBool());
            ctx.SetPreserveImageChannels(fdp.ConsumeBool());
            
            // Write with current settings
            std::stringstream gltf_stream;
            bool stream_result = ctx.WriteGltfSceneToStream(&model, gltf_stream, 
                                                           true, false);
            (void)stream_result;
            break;
        }
        
        case 5: {
            // Comprehensive test: load existing file (if any), modify, write back
            // First check if there are any files in mock filesystem
            if (!mock_fs.files.empty()) {
                // Try to load the first file
                auto it = mock_fs.files.begin();
                std::string filename = it->first;
                
                tinygltf::Model loaded_model;
                std::string load_err, load_warn;
                
                // Try both ASCII and binary loading
                bool load_result = false;
                if (filename.find(".glb") != std::string::npos || 
                    filename.find(".bin") != std::string::npos) {
                    load_result = ctx.LoadBinaryFromFile(&loaded_model, &load_err, &load_warn,
                                                        filename, 0);
                } else {
                    load_result = ctx.LoadASCIIFromFile(&loaded_model, &load_err, &load_warn,
                                                       filename, 0);
                }
                
                if (load_result) {
                    // Write it back with modifications
                    std::string output_filename = "modified_" + filename;
                    bool write_result = ctx.WriteGltfSceneToFile(&loaded_model, output_filename,
                                                                fdp.ConsumeBool(), fdp.ConsumeBool(),
                                                                true, false);
                    (void)write_result;
                }
            }
            break;
        }
    }
    
    return 0;
}

} // namespace harness_003
