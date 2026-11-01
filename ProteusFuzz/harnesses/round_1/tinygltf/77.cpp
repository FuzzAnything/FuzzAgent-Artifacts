// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::JoinPath at tiny_gltf.h:2206:20 in tiny_gltf.h
// tinygltf::GetBaseFilename at tiny_gltf.h:2263:20 in tiny_gltf.h
// tinygltf::GetFilePathExtension at tiny_gltf.h:2251:20 in tiny_gltf.h
// tinygltf::ExpandFilePath at tiny_gltf.h:1342:13 in tiny_gltf.h
// tinygltf::GetBaseDir at tiny_gltf.h:2257:20 in tiny_gltf.h
// tinygltf::WriteGltfFile at tiny_gltf.h:8460:13 in tiny_gltf.h
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

// The target functions are static and defined within the tiny_gltf.h header file.
// Since they are static, they are not visible in the 'tinygltf' namespace from outside the header.
// To fuzz them, we must either define them here (if not defined) or use the internal implementation
// if the header exposes them. However, standard tinygltf.h defines these as static inline or static
// inside the file. If they are not exported, we cannot call them directly as tinygltf::Func.
//
// Checking the typical tinygltf.h structure:
// These functions are often defined as static inside the header.
// If the header is included, these symbols exist but are not in the namespace 'tinygltf'
// unless the library version specifically exports them or we are compiling against a specific build.
//
// However, the error indicates they are not in the namespace.
// In many versions of tinygltf, these are indeed static functions defined in the header.
// If they are static, they are only visible within the translation unit where they are defined.
// Since we are including tiny_gltf.h, the definitions are present in THIS translation unit.
// But they are likely defined inside an anonymous namespace or just as static functions
// without the 'tinygltf::' prefix in the global scope or inside a specific block.
//
// Let's re-examine the provided signatures:
// static std::string JoinPath(...)
// static bool WriteGltfFile(...)
// static std::string GetBaseFilename(...)
// static std::string GetFilePathExtension(...)
// std::string ExpandFilePath(...) -> This one is NOT static in the prompt description?
// Wait, the prompt says:
// 1. JoinPath: static
// 2. WriteGltfFile: static
// 3. GetBaseFilename: static
// 4. GetFilePathExtension: static
// 5. ExpandFilePath: std::string ExpandFilePath(...) (No 'static' keyword in the prompt description for this one, but usually it is static or part of the class).
// 6. GetBaseDir: static
//
// If they are static in the header, they are global static functions in the translation unit.
// They are NOT inside the 'tinygltf' namespace unless the header defines them as such.
// The error "no member named 'JoinPath' in namespace 'tinygltf'" suggests they are not in that namespace.
//
// Let's assume the standard tinygltf.h structure where these are static functions
// defined in the global scope or inside an anonymous namespace within the header.
// If they are in the global scope (static), we can call them directly without the namespace.
// If they are in an anonymous namespace, we cannot call them from outside.
//
// However, looking at the actual tinygltf source code (which is a single header):
// These functions are typically defined as:
// namespace tinygltf {
//   static std::string JoinPath(...) { ... }
//   ...
// }
//
// If the error says "no member named ... in namespace 'tinygltf'", it implies the compiler
// looked in the namespace and didn't find it. This could happen if:
// 1. The functions are not in the namespace (e.g., defined outside).
// 2. The functions are defined inside an anonymous namespace inside tinygltf.
// 3. The functions are defined inside a class but not static members? No, prompt says static.
//
// Let's check the actual tinygltf.h source logic.
// In tinygltf.h, the functions are usually defined inside the `tinygltf` namespace.
// Example:
// namespace tinygltf {
//   static std::string JoinPath(const std::string &path0, const std::string &path1) { ... }
// }
//
// If the error persists, it might be that the specific version of tinygltf being used
// has these functions defined differently or the include is not picking them up correctly.
//
// Alternative: The prompt says "static std::string JoinPath...".
// If the user provided the signature as `static std::string JoinPath`, and the error is
// "no member named 'JoinPath' in namespace 'tinygltf'", it strongly suggests that
// in the actual header file, these functions are NOT inside the `tinygltf` namespace
// OR they are inside an anonymous namespace.
//
// Let's look at the actual tinygltf.h (v2.8.14 or similar).
// The functions are indeed inside the `tinygltf` namespace.
// Why the error?
// Maybe the header is not included correctly? No, `#include "tiny_gltf.h"` is there.
// Maybe the functions are defined inside a `#ifdef` block that is not active?
// Or maybe the prompt's signature description is slightly off regarding the namespace.
//
// Let's try calling them without the namespace prefix if they are global static.
// But the prompt explicitly says "Target API Functions" and lists them.
// If they are static inside the namespace, they should be `tinygltf::JoinPath`.
//
// Wait, could it be that the functions are defined as `static` inside the namespace,
// but the compiler is treating them as not visible?
// No, static inside a namespace is visible within that namespace.
//
// Let's reconsider the error. "no member named 'JoinPath' in namespace 'tinygltf'".
// This means the compiler found the namespace `tinygltf` but not the member `JoinPath`.
// This implies the definition in the header is either:
// 1. Not inside `namespace tinygltf`.
// 2. Inside an anonymous namespace inside `tinygltf`.
// 3. Defined conditionally and the condition is false.
//
// Let's check the actual source of tinygltf.h.
// In the real tinygltf.h, `JoinPath` is defined as:
// namespace tinygltf {
//   static std::string JoinPath(const std::string &path0, const std::string &path1) {
//     ...
//   }
// }
//
// If the error occurs, it's possible the user's environment has a modified header
// or the functions are not present in the version being used.
// However, I must assume the prompt is correct about the existence of the functions.
//
// Hypothesis: The functions are defined in the header, but maybe the `static` keyword
// makes them have internal linkage, and if they are defined in a header included by
// multiple files, it's fine. But here we are in one file.
//
// Is it possible the functions are defined inside a class? No, the signature is static function.
//
// Let's try a different approach. If the functions are not in the namespace, maybe they are
// just global static functions?
// Let's try calling them as `JoinPath` (global scope) if they are defined outside the namespace.
// But the prompt implies they are part of the library API, usually namespaced.
//
// Wait, I see the issue. In some versions or configurations, these utility functions might be
// defined in an anonymous namespace to prevent symbol collisions.
// If they are in an anonymous namespace inside `tinygltf`, they are not accessible as `tinygltf::JoinPath`.
//
// However, the prompt asks to fuzz these specific functions.
// If they are in an anonymous namespace, we cannot call them from the fuzz driver.
// Unless we copy the implementation or the header is structured differently.
//
// Let's look at the error again. It says "no member named ... in namespace 'tinygltf'".
// This confirms the namespace exists but the member is missing.
//
// Possibility: The functions are defined inside the `tinygltf` namespace but are `static`
// and perhaps the compiler is having trouble? No.
//
// Maybe the functions are defined in a different namespace?
// Or maybe the header file `tiny_gltf.h` is not the one being used?
//
// Let's assume the functions ARE in the namespace but the `static` keyword is the issue?
// No, `static` inside a namespace is fine.
//
// Let's try to define the functions ourselves if they are not visible?
// No, that would be duplicating code and might not match the internal logic.
//
// Wait, I found a potential issue. In some versions of tinygltf, the `static` functions
// are defined inside the `tinygltf` namespace, but if the header is included, they should be there.
//
// Let's check if the functions are actually defined in the header.
// If the user says "Target API Functions" and lists them, they must be callable.
//
// Maybe the functions are defined as `static` in the global scope?
// Let's try calling them without the namespace.
// `JoinPath(path0, path1)`
//
// But the prompt says "Target API Functions" and lists `tinygltf::JoinPath` in the description?
// No, the prompt lists the signature as `static std::string JoinPath(...)`.
// It does NOT explicitly say `tinygltf::JoinPath` in the signature line, but the task says "fuzz the library tinygltf".
//
// Let's look at the actual tinygltf.h source code (v2.8.14):
// ```cpp
// namespace tinygltf {
//   static std::string JoinPath(const std::string &path0, const std::string &path1) { ... }
//   static std::string GetBaseFilename(const std::string &filepath) { ... }
//   static std::string GetFilePathExtension(const std::string &FileName) { ... }
//   static std::string GetBaseDir(const std::string &filepath) { ... }
//   static bool WriteGltfFile(const std::string &output, const std::string &content) { ... }
//   std::string ExpandFilePath(const std::string &filepath, void *); // This one is not static in some versions?
// }
// ```
//
// If the error is "no member named ... in namespace 'tinygltf'", it is very strange if the header is correct.
//
// Could it be that the header file `tiny_gltf.h` is not included properly?
// Or maybe the functions are defined inside a `#ifdef TINYGLTF_IMPLEMENTATION` block?
// Yes! This is the most likely cause.
// In single-header libraries like tinygltf, the implementation is often inside a `#ifdef TINYGLTF_IMPLEMENTATION` block.
// If `TINYGLTF_IMPLEMENTATION` is not defined before including the header, the functions are declared but not defined,
// or they are not defined at all if they are static and only defined in the implementation block.
//
// Wait, if they are static, they are defined inline.
// But in tinygltf.h, the static functions are usually defined in the header regardless of the macro,
// OR they are defined in the implementation section.
//
// Let's check the tinygltf.h structure.
// The header has a section for declarations and a section for definitions.
// If `TINYGLTF_IMPLEMENTATION` is not defined, the definitions might be missing.
//
// To fix this, we need to define `TINYGLTF_IMPLEMENTATION` before including `tiny_gltf.h`.
//
// Let's verify this hypothesis.
// In tinygltf.h:
// ```cpp
// #ifdef TINYGLTF_IMPLEMENTATION
// // definitions of static functions and other implementations
// #endif
// ```
//
// If we don't define `TINYGLTF_IMPLEMENTATION`, the static functions might not be defined,
// leading to "undefined reference" at link time, but here we get "no member named" at compile time.
// This suggests the functions are not even declared in the namespace if the macro is missing?
// Or maybe they are declared but the compiler is confused?
//
// Actually, if the functions are `static` and defined in the header, they should be there.
// But if the header uses the `TINYGLTF_IMPLEMENTATION` macro to guard the definitions,
// and the declarations are also guarded, then they won't be visible.
//
// Let's try defining `TINYGLTF_IMPLEMENTATION` before the include.
// This is a common requirement for single-header libraries.

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>

// Define TINYGLTF_IMPLEMENTATION to ensure the static functions are defined
#define TINYGLTF_IMPLEMENTATION
#include "tiny_gltf.h"

static std::string CreateStringFromInput(const uint8_t *Data, size_t Size) {
    if (Size == 0) return "";
    return std::string(reinterpret_cast<const char*>(Data), Size);
}

static void SplitInput(const uint8_t *Data, size_t Size, std::string &s1, std::string &s2) {
    if (Size == 0) {
        s1 = "";
        s2 = "";
        return;
    }
    if (Size == 1) {
        s1 = CreateStringFromInput(Data, 1);
        s2 = "";
        return;
    }
    size_t splitPoint = Size / 2;
    s1 = CreateStringFromInput(Data, splitPoint);
    s2 = CreateStringFromInput(Data + splitPoint, Size - splitPoint);
}

extern "C" int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    std::string path0, path1, filepath, output;
    
    // Strategy 1: Test JoinPath
    SplitInput(Data, Size, path0, path1);
    
    // Now that TINYGLTF_IMPLEMENTATION is defined, these should be visible in the namespace
    std::string joined = tinygltf::JoinPath(path0, path1);
    (void)joined;

    // Strategy 2: Test GetBaseFilename
    filepath = CreateStringFromInput(Data, Size);
    std::string baseName = tinygltf::GetBaseFilename(filepath);
    (void)baseName;

    // Strategy 3: Test GetFilePathExtension
    std::string ext = tinygltf::GetFilePathExtension(filepath);
    (void)ext;

    // Strategy 4: Test ExpandFilePath
    // Note: ExpandFilePath might not be static, but it should be in the namespace
    std::string expanded = tinygltf::ExpandFilePath(filepath, nullptr);
    (void)expanded;

    // Strategy 5: Test GetBaseDir
    std::string baseDir = tinygltf::GetBaseDir(filepath);
    (void)baseDir;

    // Strategy 6: Test WriteGltfFile
    std::string safeOutput = "./dummy_file";
    if (Size > 0) {
        std::string fname = CreateStringFromInput(Data, Size);
        std::string cleanFname;
        for (char c : fname) {
            if (c != '/' && c != '\\' && c != 0) {
                cleanFname += c;
            }
        }
        if (!cleanFname.empty()) {
            safeOutput = "./fuzz_" + cleanFname + ".gltf";
        }
    }
    
    std::string content = "{\"asset\":{\"version\":\"2.0\"}}";
    if (Size > 0) {
        content += CreateStringFromInput(Data, Size);
    }

    bool writeResult = tinygltf::WriteGltfFile(safeOutput, content);
    (void)writeResult;

    // Cleanup
    if (writeResult) {
        std::remove(safeOutput.c_str());
    }

    return 0;
}