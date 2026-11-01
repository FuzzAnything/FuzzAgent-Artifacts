// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::ExpandFilePath at tiny_gltf.h:1342:13 in tiny_gltf.h
// tinygltf::WriteWholeFile at tiny_gltf.h:1347:6 in tiny_gltf.h
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

// Helper to create a dummy FsCallbacks struct with minimal implementation
static tinygltf::FsCallbacks CreateDummyFsCallbacks() {
    tinygltf::FsCallbacks fs;
    
    // FileExistsFunction: bool (*)(const std::string&, void*)
    fs.FileExists = [](const std::string &, void *) { return false; };
    
    // ExpandFilePathFunction: std::string (*)(const std::string&, void*)
    fs.ExpandFilePath = [](const std::string &filepath, void *) { return filepath; };
    
    // ReadWholeFileFunction: bool (*)(std::vector<unsigned char>*, std::string*, const std::string&, void*)
    fs.ReadWholeFile = [](std::vector<unsigned char> *, std::string *, const std::string &, void *) { return false; };
    
    // WriteWholeFileFunction: bool (*)(std::string*, const std::string&, const std::vector<unsigned char>&, void*)
    fs.WriteWholeFile = [](std::string *, const std::string &,
                           const std::vector<unsigned char> &, void *) { return false; };
    
    // GetFileSizeInBytesFunction: bool (*)(size_t*, std::string*, const std::string&, void*)
    // Based on the error, the second argument is std::string*, not const std::string&
    fs.GetFileSizeInBytes = [](size_t *, std::string *, const std::string &, void *) { return false; };
    
    fs.user_data = nullptr;
    return fs;
}

// Helper to parse input into strings for path testing
static std::vector<std::string> SplitInputIntoStrings(const uint8_t *Data, size_t Size) {
    std::vector<std::string> parts;
    std::string current;
    for (size_t i = 0; i < Size; ++i) {
        if (Data[i] == 0) {
            parts.push_back(current);
            current.clear();
        } else {
            current.push_back(static_cast<char>(Data[i]));
        }
    }
    parts.push_back(current);
    return parts;
}

// Local versions of static functions to avoid visibility issues
static std::string LocalGetFilePathExtension(const std::string &FileName) {
    size_t pos = FileName.rfind('.');
    if (pos == std::string::npos || pos == 0) {
        return "";
    }
    return FileName.substr(pos + 1);
}

static std::string LocalJoinPath(const std::string &path0, const std::string &path1) {
    if (path0.empty()) {
        return path1;
    }
    if (path1.empty()) {
        return path0;
    }
    if (path0.back() == '/') {
        return path0 + path1;
    }
    return path0 + "/" + path1;
}

static std::string LocalMimeToExt(const std::string &mimeType) {
    if (mimeType == "image/png") return "png";
    if (mimeType == "image/jpeg") return "jpg";
    if (mimeType == "image/bmp") return "bmp";
    if (mimeType == "image/webp") return "webp";
    if (mimeType == "image/tga") return "tga";
    if (mimeType == "image/ktx2") return "ktx2";
    if (mimeType == "image/drm") return "drm";
    return "";
}

// Local mock for LoadExternalFile since it's static and not accessible
static bool LocalLoadExternalFile(std::vector<unsigned char> *out, std::string *err,
 std::string *warn, const std::string &filename,
 const std::string &basedir, bool required,
 size_t reqBytes, bool checkSize,
 size_t maxFileSize, tinygltf::FsCallbacks *fs) {
    (void)out; (void)err; (void)warn; (void)filename; (void)basedir;
    (void)required; (void)reqBytes; (void)checkSize; (void)maxFileSize; (void)fs;
    return false; // Simulate failure
}

extern "C" int LLVMFuzzerTestOneInput_122(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // 1. Fuzz GetFilePathExtension (using local version)
    std::string filename(reinterpret_cast<const char*>(Data), Size);
    std::string ext = LocalGetFilePathExtension(filename);
    (void)ext;

    // 2. Fuzz ExpandFilePath
    std::string expanded = tinygltf::ExpandFilePath(filename, nullptr);
    (void)expanded;

    // 3. Fuzz JoinPath (using local version)
    std::vector<std::string> parts = SplitInputIntoStrings(Data, Size);
    std::string path0 = parts.empty() ? "" : parts[0];
    std::string path1 = parts.size() > 1 ? parts[1] : "";
    std::string joined = LocalJoinPath(path0, path1);
    (void)joined;

    // 4. Fuzz MimeToExt (using local version)
    std::string mime = parts.empty() ? "" : parts[0];
    std::string mimeExt = LocalMimeToExt(mime);
    (void)mimeExt;

    // 5. Fuzz WriteWholeFile
    std::string err;
    std::string filepath = "./dummy_file";
    std::vector<unsigned char> contents(Data, Data + Size);
    bool writeOk = tinygltf::WriteWholeFile(&err, filepath, contents, nullptr);
    (void)writeOk;

    // 6. Fuzz LoadExternalFile (using local mock since it's static)
    tinygltf::FsCallbacks fs = CreateDummyFsCallbacks();
    
    std::string loadErr, loadWarn;
    std::vector<unsigned char> outBuffer;
    std::string loadFilename = parts.empty() ? "test.gltf" : parts[0];
    std::string basedir = parts.size() > 1 ? parts[1] : "";
    
    bool required = (Size % 2) == 0;
    size_t reqBytes = Size;
    bool checkSize = (Size % 3) == 0;
    size_t maxFileSize = Size * 2 + 100;

    bool loadOk = LocalLoadExternalFile(&outBuffer, &loadErr, &loadWarn,
                                             loadFilename, basedir, required,
                                             reqBytes, checkSize, maxFileSize, &fs);
    (void)loadOk;

    return 0;
}