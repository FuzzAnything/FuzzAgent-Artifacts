// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.h:1497:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.h:1490:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.h:1445:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.h:1433:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.h:1497:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.h:1490:8 in tiny_gltf.h
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

// Dummy callbacks for FsCallbacks
// Note: The function signatures must match exactly what tinygltf expects.
// Based on common tinygltf definitions:
// ReadWholeFile: bool ReadWholeFile(std::string *out_buf, std::string *err, const std::string &filename, void *user_data)
// GetFileSizeInBytes: int GetFileSizeInBytes(std::string *err, const std::string &filename, void *user_data)
// However, the error message suggests the library expects specific signatures.
// Let's define them to match the likely expected signatures in tiny_gltf.h.
// Typically:
// ReadWholeFile: bool ReadWholeFile(std::vector<unsigned char> *out_buf, std::string *err, const std::string &filename, void *user_data)
// GetFileSizeInBytes: int GetFileSizeInBytes(std::string *err, const std::string &filename, void *user_data) -> returns size_t or int?
// Let's look at the error: "no known conversion from 'bool (std::string *, std::vector<unsigned char> *, const std::string &, void *)' to 'const function<bool (vector<unsigned char> *, basic_string<char> *, const basic_string<char> &, void *)>'"
// This implies the expected signature is: bool (vector<unsigned char> *, std::string *, const std::string &, void *)
// My dummy was: bool (std::string *, std::vector<unsigned char> *, const std::string &, void *) -> Order of first two args was wrong.

static bool DummyFileExists(const std::string &filename, void *user_data) {
    return false;
}

static std::string DummyExpandFilePath(const std::string &filepath, void *user_data) {
    return filepath;
}

// Corrected signature based on error message: vector<unsigned char> *buf, std::string *err
static bool DummyReadWholeFile(std::vector<unsigned char> *buf, std::string *err,
                               const std::string &filename, void *user_data) {
    if (err) err->clear();
    if (buf) buf->clear();
    return false;
}

static bool DummyWriteWholeFile(std::string *err, const std::string &filename,
                                const std::vector<unsigned char> &contents,
                                void *user_data) {
    if (err) err->clear();
    return false;
}

// Corrected signature based on error message: std::string *err, const std::string &filename
// The error message for GetFileSizeInBytes showed: function<bool (unsigned long *, basic_string<char> *, const basic_string<char> &, void *)>
// Wait, the error says: "no known conversion from 'int (const std::string &, void *)' ... to 'function<bool (unsigned long *, basic_string<char> *, const basic_string<char> &, void *)>'"
// This suggests the expected signature is: bool (size_t *out_size, std::string *err, const std::string &filename, void *user_data)
// Or maybe: bool (std::string *err, size_t *out_size, const std::string &filename, void *user_data)?
// Let's re-read carefully: "function<bool (unsigned long *, basic_string<char> *, const basic_string<char> &, void *)>"
// unsigned long * is likely size_t * (output size).
// basic_string<char> * is likely std::string * (error).
// So: bool GetFileSizeInBytes(size_t *size, std::string *err, const std::string &filename, void *user_data)

static bool DummyGetFileSizeInBytes(size_t *size, std::string *err,
                                    const std::string &filename, void *user_data) {
    if (err) err->clear();
    if (size) *size = 0;
    return false;
}

// Dummy callbacks for URICallbacks
// Error message for encode: "function<bool (const basic_string<char> &, const basic_string<char> &, basic_string<char> *, void *)>"
// This looks like: bool encode(const std::string &uri, const std::string &base_dir, std::string *out, void *user_data)
// Or maybe: bool encode(const std::string &uri, std::string *out, void *user_data) but the error shows 4 args.
// Let's assume: bool URIEncodeFunction(const std::string &uri, const std::string &base_dir, std::string *out, void *user_data)
// Error message for decode: "function<bool (const basic_string<char> &, basic_string<char> *, void *)>"
// This looks like: bool decode(const std::string &uri, std::string *out, void *user_data)

static bool DummyURIEncode(const std::string &uri, const std::string &base_dir, std::string *out, void *user_data) {
    if (out) *out = uri;
    return true;
}

static bool DummyURIDecode(const std::string &uri, std::string *out, void *user_data) {
    if (out) *out = uri;
    return true;
}

static void WriteDummyFile(const std::string &filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename.c_str(), "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    tinygltf::TinyGLTF ctx;
    tinygltf::Model model;
    std::string err;
    std::string warn;

    // 1. Test SetFsCallbacks with valid callbacks
    tinygltf::FsCallbacks fs_callbacks;
    fs_callbacks.FileExists = DummyFileExists;
    fs_callbacks.ExpandFilePath = DummyExpandFilePath;
    fs_callbacks.ReadWholeFile = DummyReadWholeFile;
    fs_callbacks.WriteWholeFile = DummyWriteWholeFile;
    fs_callbacks.GetFileSizeInBytes = DummyGetFileSizeInBytes;
    fs_callbacks.user_data = nullptr;
    ctx.SetFsCallbacks(fs_callbacks, &err);

    // 2. Test SetURICallbacks with valid callbacks
    tinygltf::URICallbacks uri_callbacks;
    uri_callbacks.encode = DummyURIEncode;
    uri_callbacks.decode = DummyURIDecode;
    uri_callbacks.user_data = nullptr;
    ctx.SetURICallbacks(uri_callbacks, &err);

    // 3. Test LoadASCIIFromString
    std::string ascii_str(reinterpret_cast<const char*>(Data), Size);
    ctx.LoadASCIIFromString(&model, &err, &warn, ascii_str.c_str(), ascii_str.length(), "", 0);
    model = tinygltf::Model(); // Reset model

    // 4. Test LoadBinaryFromMemory
    ctx.LoadBinaryFromMemory(&model, &err, &warn, Data, Size, "", 0);
    model = tinygltf::Model(); // Reset model

    // 5. Test LoadASCIIFromFile and LoadBinaryFromFile
    const char *dummy_file = "./dummy_file";
    WriteDummyFile(dummy_file, Data, Size);

    ctx.LoadASCIIFromFile(&model, &err, &warn, dummy_file, 0);
    model = tinygltf::Model(); // Reset model

    ctx.LoadBinaryFromFile(&model, &err, &warn, dummy_file, 0);
    model = tinygltf::Model(); // Reset model

    // 6. Test with invalid callbacks to trigger error paths
    tinygltf::FsCallbacks invalid_fs;
    invalid_fs.FileExists = nullptr;
    invalid_fs.ExpandFilePath = nullptr;
    invalid_fs.ReadWholeFile = nullptr;
    invalid_fs.WriteWholeFile = nullptr;
    invalid_fs.GetFileSizeInBytes = nullptr;
    invalid_fs.user_data = nullptr;
    ctx.SetFsCallbacks(invalid_fs, &err);

    tinygltf::URICallbacks invalid_uri;
    invalid_uri.encode = nullptr;
    invalid_uri.decode = nullptr;
    invalid_uri.user_data = nullptr;
    ctx.SetURICallbacks(invalid_uri, &err);

    return 0;
}