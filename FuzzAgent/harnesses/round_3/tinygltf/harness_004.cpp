#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include "tiny_gltf.h"

namespace harness_004 {
/*
 * Fuzzing harness for tinygltf library targeting file-based image loading
 * Goal: Exercise stb_image.h APIs (1.95% branch coverage) and file loading APIs (0% coverage)
 * Targets: 
 *   1. File Loading APIs: LoadASCIIFromFile, LoadBinaryFromFile (currently 0% covered)
 *   2. Image Loading APIs: stbi_load, stbi_load_16, stbi_loadf, stbi_info, stbi_is_16_bit
 *   3. Equality Operators: Various comparison operators for GLTF components
 * Strategy: Implement custom filesystem callbacks to simulate file reading from fuzzer input,
 *           load GLTF files containing image references, and test equality operators
 * Differentiation from previous harnesses:
 *   - harness_000: Basic GLTF parsing (loading from string/memory)
 *   - harness_001: Image loading via embedded images in strings
 *   - harness_002: Image writing using callback APIs
 *   - harness_003: Equality operator testing with string-based loading
 *   - harness_004: File-based loading with custom FS callbacks and stb_image coverage
 */



// Use same includes as other harnesses
// #define STB_IMAGE_IMPLEMENTATION
// #define STB_IMAGE_WRITE_IMPLEMENTATION
// #define TINYGLTF_IMPLEMENTATION

// Custom filesystem callbacks that read from in-memory file database
struct MemoryFileSystem {
    // Map from filename to file content
    std::map<std::string, std::vector<unsigned char>> files;
    
    // Track which files were accessed
    std::vector<std::string> accessed_files;
    
    void clear() {
        files.clear();
        accessed_files.clear();
    }
    
    void add_file(const std::string& filename, const std::vector<unsigned char>& data) {
        files[filename] = data;
    }
};

// Global memory filesystem for callbacks
static MemoryFileSystem g_memory_fs;

// Custom FileExists callback
bool memory_file_exists(const std::string& abs_filename, void* user_data) {
    (void)user_data;
    return g_memory_fs.files.find(abs_filename) != g_memory_fs.files.end();
}

// Custom ExpandFilePath callback - just returns path as-is for fuzzing
std::string memory_expand_file_path(const std::string& filepath, void* user_data) {
    (void)user_data;
    return filepath;
}

// Custom ReadWholeFile callback - reads from memory filesystem
bool memory_read_whole_file(std::vector<unsigned char>* out, std::string* err, 
                           const std::string& filepath, void* user_data) {
    (void)user_data;
    
    auto it = g_memory_fs.files.find(filepath);
    if (it == g_memory_fs.files.end()) {
        if (err) *err = "File not found: " + filepath;
        return false;
    }
    
    g_memory_fs.accessed_files.push_back(filepath);
    *out = it->second;
    return true;
}

// Custom WriteWholeFile callback - not needed for fuzzing but required by API
bool memory_write_whole_file(std::string* err, const std::string& filepath,
                            const std::vector<unsigned char>& contents, void* user_data) {
    (void)err; (void)filepath; (void)contents; (void)user_data;
    // Not implemented for fuzzing - return success
    return true;
}

// Custom GetFileSizeInBytes callback
bool memory_get_file_size_in_bytes(size_t* filesize_out, std::string* err,
                                   const std::string& filepath, void* user_data) {
    (void)user_data;
    
    auto it = g_memory_fs.files.find(filepath);
    if (it == g_memory_fs.files.end()) {
        if (err) *err = "File not found: " + filepath;
        return false;
    }
    
    *filesize_out = it->second.size();
    return true;
}

// Setup custom filesystem callbacks
void setup_memory_filesystem(tinygltf::TinyGLTF& ctx) {
    tinygltf::FsCallbacks callbacks;
    callbacks.FileExists = memory_file_exists;
    callbacks.ExpandFilePath = memory_expand_file_path;
    callbacks.ReadWholeFile = memory_read_whole_file;
    callbacks.WriteWholeFile = memory_write_whole_file;
    callbacks.GetFileSizeInBytes = memory_get_file_size_in_bytes;
    callbacks.user_data = nullptr;
    
    std::string err;
    bool ret = ctx.SetFsCallbacks(callbacks, &err);
    (void)ret; // Ignore return for fuzzing
}

// Simple base64 encoding for buffer data
static const char base64_chars[] = 
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

std::string base64_encode(const uint8_t* data, size_t len) {
    std::string ret;
    int i = 0;
    int j = 0;
    uint8_t char_array_3[3];
    uint8_t char_array_4[4];

    while (len--) {
        char_array_3[i++] = *(data++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for(i = 0; i < 4; i++)
                ret += base64_chars[char_array_4[i]];
            i = 0;
        }
    }

    if (i) {
        for(j = i; j < 3; j++)
            char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (j = 0; j < i + 1; j++)
            ret += base64_chars[char_array_4[j]];

        while(i++ < 3)
            ret += '=';
    }

    return ret;
}

// Create GLTF JSON with external image reference
std::string create_gltf_with_external_image(FuzzedDataProvider& fdp, 
                                           const std::string& image_filename,
                                           const std::string& mime_type,
                                           int width, int height) {
    char buffer[4096];
    snprintf(buffer, sizeof(buffer),
        "{\n"
        "  \"asset\": {\"version\": \"2.0\"},\n"
        "  \"images\": [\n"
        "    {\n"
        "      \"name\": \"test_image\",\n"
        "      \"width\": %d,\n"
        "      \"height\": %d,\n"
        "      \"mimeType\": \"%s\",\n"
        "      \"uri\": \"%s\"\n"
        "    }\n"
        "  ],\n"
        "  \"textures\": [{\"source\": 0}],\n"
        "  \"materials\": [\n"
        "    {\n"
        "      \"pbrMetallicRoughness\": {\n"
        "        \"baseColorTexture\": {\"index\": 0}\n"
        "      }\n"
        "    }\n"
        "  ],\n"
        "  \"nodes\": [\n"
        "    {\n"
        "      \"name\": \"test_node\"\n"
        "    }\n"
        "  ],\n"
        "  \"cameras\": [\n"
        "    {\n"
        "      \"type\": \"perspective\",\n"
        "      \"perspective\": {\n"
        "        \"yfov\": 0.75,\n"
        "        \"znear\": 0.01\n"
        "      }\n"
        "    }\n"
        "  ]\n"
        "}",
        width, height, mime_type.c_str(), image_filename.c_str());
    
    return std::string(buffer);
}

// Generate synthetic image data for testing
std::vector<uint8_t> generate_image_data(FuzzedDataProvider& fdp, int width, int height, int channels) {
    size_t data_size = width * height * channels;
    std::vector<uint8_t> data;
    
    // Consume bytes from fuzzer input, but limit to reasonable size
    size_t max_bytes = std::min<size_t>(fdp.remaining_bytes(), data_size);
    if (max_bytes > 0) {
        auto bytes = fdp.ConsumeBytes<uint8_t>(max_bytes);
        data.insert(data.end(), bytes.begin(), bytes.end());
    }
    
    // Pad with zeros if needed
    if (data.size() < data_size) {
        data.resize(data_size, 0);
    }
    
    return data;
}

// Test 1: Load ASCII GLTF with external PNG image via file API
static void test_ascii_file_png(FuzzedDataProvider& fdp) {
    if (fdp.remaining_bytes() < 100) return;
    
    // Consume parameters from fuzzer input
    int width = fdp.ConsumeIntegralInRange<int>(1, 512);
    int height = fdp.ConsumeIntegralInRange<int>(1, 512);
    
    // Generate synthetic PNG-like image data (doesn't need to be valid PNG)
    auto image_data = generate_image_data(fdp, width, height, 4);
    
    // Setup filesystem
    g_memory_fs.clear();
    
    // Add GLTF file and image file to memory filesystem
    std::string gltf_filename = "test.gltf";
    std::string image_filename = "test.png";
    
    std::string gltf_content = create_gltf_with_external_image(fdp, image_filename, "image/png", width, height);
    g_memory_fs.add_file(gltf_filename, std::vector<unsigned char>(gltf_content.begin(), gltf_content.end()));
    g_memory_fs.add_file(image_filename, image_data);
    
    // Load via file API
    tinygltf::Model model;
    tinygltf::TinyGLTF ctx;
    setup_memory_filesystem(ctx);
    
    std::string err, warn;
    bool ret = ctx.LoadASCIIFromFile(&model, &err, &warn, gltf_filename, tinygltf::REQUIRE_VERSION);
    (void)ret;
    
    // Access images to trigger stb_image loading
    if (!model.images.empty()) {
        const auto& image = model.images[0];
        (void)image.width;
        (void)image.height;
        // The image data would have been loaded via stb_image APIs
    }
}

// Test 2: Load ASCII GLTF with external JPEG image via file API
static void test_ascii_file_jpeg(FuzzedDataProvider& fdp) {
    if (fdp.remaining_bytes() < 100) return;
    
    int width = fdp.ConsumeIntegralInRange<int>(1, 512);
    int height = fdp.ConsumeIntegralInRange<int>(1, 512);
    
    auto image_data = generate_image_data(fdp, width, height, 3); // JPEG typically RGB
    
    g_memory_fs.clear();
    
    std::string gltf_filename = "test_jpeg.gltf";
    std::string image_filename = "test.jpg";
    
    std::string gltf_content = create_gltf_with_external_image(fdp, image_filename, "image/jpeg", width, height);
    g_memory_fs.add_file(gltf_filename, std::vector<unsigned char>(gltf_content.begin(), gltf_content.end()));
    g_memory_fs.add_file(image_filename, image_data);
    
    tinygltf::Model model;
    tinygltf::TinyGLTF ctx;
    setup_memory_filesystem(ctx);
    
    std::string err, warn;
    bool ret = ctx.LoadASCIIFromFile(&model, &err, &warn, gltf_filename, tinygltf::REQUIRE_VERSION);
    (void)ret;
    
    // Test equality operators on loaded components
    if (!model.images.empty() && !model.nodes.empty() && !model.cameras.empty()) {
        // Test Image equality
        bool image_eq = (model.images[0] == model.images[0]);
        (void)image_eq;
        
        // Test Node equality  
        bool node_eq = (model.nodes[0] == model.nodes[0]);
        (void)node_eq;
        
        // Test Camera equality
        bool camera_eq = (model.cameras[0] == model.cameras[0]);
        (void)camera_eq;
    }
}

// Test 3: Load Binary GLTF via file API
static void test_binary_file(FuzzedDataProvider& fdp) {
    if (fdp.remaining_bytes() < 100) return;
    
    // Create a simple binary GLTF (GLB) structure
    // GLB header: magic(4) + version(4) + length(4)
    // Chunk0: JSON length(4) + JSON type(4) + JSON data
    // Chunk1: BIN length(4) + BIN type(4) + BIN data
    
    // Minimal JSON for binary GLTF
    std::string json_content = "{\"asset\":{\"version\":\"2.0\"}}";
    uint32_t json_length = static_cast<uint32_t>(json_content.size());
    
    // Binary data (could contain image data)
    size_t bin_size = std::min<size_t>(fdp.remaining_bytes(), 1024);
    auto bin_data = fdp.ConsumeBytes<uint8_t>(bin_size);
    uint32_t bin_length = static_cast<uint32_t>(bin_data.size());
    
    // Calculate total length: header(12) + chunk0_header(8) + json + chunk1_header(8) + bin
    uint32_t total_length = 12 + 8 + json_length + 8 + bin_length;
    
    std::vector<uint8_t> glb_data;
    glb_data.reserve(total_length);
    
    // Header: magic "glTF" (0x46546C67 in little-endian)
    glb_data.push_back('g'); glb_data.push_back('l'); glb_data.push_back('T'); glb_data.push_back('F');
    
    // Version 2 (little-endian)
    glb_data.push_back(2); glb_data.push_back(0); glb_data.push_back(0); glb_data.push_back(0);
    
    // Total length
    glb_data.push_back(static_cast<uint8_t>(total_length & 0xFF));
    glb_data.push_back(static_cast<uint8_t>((total_length >> 8) & 0xFF));
    glb_data.push_back(static_cast<uint8_t>((total_length >> 16) & 0xFF));
    glb_data.push_back(static_cast<uint8_t>((total_length >> 24) & 0xFF));
    
    // Chunk0: JSON
    // JSON length
    glb_data.push_back(static_cast<uint8_t>(json_length & 0xFF));
    glb_data.push_back(static_cast<uint8_t>((json_length >> 8) & 0xFF));
    glb_data.push_back(static_cast<uint8_t>((json_length >> 16) & 0xFF));
    glb_data.push_back(static_cast<uint8_t>((json_length >> 24) & 0xFF));
    
    // JSON type (0x4E4F534A = "JSON")
    glb_data.push_back(0x4A); glb_data.push_back(0x53); glb_data.push_back(0x4F); glb_data.push_back(0x4E);
    
    // JSON data
    glb_data.insert(glb_data.end(), json_content.begin(), json_content.end());
    
    // Chunk1: BIN
    // BIN length
    glb_data.push_back(static_cast<uint8_t>(bin_length & 0xFF));
    glb_data.push_back(static_cast<uint8_t>((bin_length >> 8) & 0xFF));
    glb_data.push_back(static_cast<uint8_t>((bin_length >> 16) & 0xFF));
    glb_data.push_back(static_cast<uint8_t>((bin_length >> 24) & 0xFF));
    
    // BIN type (0x004E4942 = "BIN")
    glb_data.push_back(0x42); glb_data.push_back(0x49); glb_data.push_back(0x4E); glb_data.push_back(0x00);
    
    // BIN data
    glb_data.insert(glb_data.end(), bin_data.begin(), bin_data.end());
    
    g_memory_fs.clear();
    std::string glb_filename = "test.glb";
    g_memory_fs.add_file(glb_filename, glb_data);
    
    tinygltf::Model model;
    tinygltf::TinyGLTF ctx;
    setup_memory_filesystem(ctx);
    
    std::string err, warn;
    bool ret = ctx.LoadBinaryFromFile(&model, &err, &warn, glb_filename, tinygltf::REQUIRE_VERSION);
    (void)ret;
}

// Test 4: Load two similar models and test equality operators
static void test_file_based_equality(FuzzedDataProvider& fdp) {
    if (fdp.remaining_bytes() < 200) return;
    
    int width = fdp.ConsumeIntegralInRange<int>(1, 256);
    int height = fdp.ConsumeIntegralInRange<int>(1, 256);
    
    // Generate two slightly different image datasets
    auto image_data1 = generate_image_data(fdp, width, height, 4);
    auto image_data2 = generate_image_data(fdp, width, height, 4);
    
    g_memory_fs.clear();
    
    // Create two similar GLTF files
    std::string gltf_filename1 = "model1.gltf";
    std::string gltf_filename2 = "model2.gltf";
    std::string image_filename1 = "image1.png";
    std::string image_filename2 = "image2.png";
    
    std::string gltf_content1 = create_gltf_with_external_image(fdp, image_filename1, "image/png", width, height);
    std::string gltf_content2 = create_gltf_with_external_image(fdp, image_filename2, "image/png", width, height);
    
    g_memory_fs.add_file(gltf_filename1, std::vector<unsigned char>(gltf_content1.begin(), gltf_content1.end()));
    g_memory_fs.add_file(gltf_filename2, std::vector<unsigned char>(gltf_content2.begin(), gltf_content2.end()));
    g_memory_fs.add_file(image_filename1, image_data1);
    g_memory_fs.add_file(image_filename2, image_data2);
    
    // Load both models
    tinygltf::Model model1, model2;
    tinygltf::TinyGLTF ctx;
    setup_memory_filesystem(ctx);
    
    std::string err1, warn1, err2, warn2;
    bool ret1 = ctx.LoadASCIIFromFile(&model1, &err1, &warn1, gltf_filename1, tinygltf::REQUIRE_VERSION);
    bool ret2 = ctx.LoadASCIIFromFile(&model2, &err2, &warn2, gltf_filename2, tinygltf::REQUIRE_VERSION);
    
    if (ret1 && ret2) {
        // Test Model equality
        bool models_equal = (model1 == model2);
        (void)models_equal;
        
        // Test individual component equality if available
        if (!model1.images.empty() && !model2.images.empty()) {
            bool images_equal = (model1.images[0] == model2.images[0]);
            (void)images_equal;
        }
        
        if (!model1.materials.empty() && !model2.materials.empty()) {
            bool materials_equal = (model1.materials[0] == model2.materials[0]);
            (void)materials_equal;
        }
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 16) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Use first byte to select test case
    uint8_t selector = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    try {
        switch (selector) {
            case 0:
                test_ascii_file_png(fdp);
                break;
            case 1:
                test_ascii_file_jpeg(fdp);
                break;
            case 2:
                test_binary_file(fdp);
                break;
            case 3:
                test_file_based_equality(fdp);
                break;
        }
    } catch (...) {
        // Catch any exceptions from FuzzedDataProvider
    }
    
    // Clear global state for next iteration
    g_memory_fs.clear();
    
    return 0;
}

} // namespace harness_004
