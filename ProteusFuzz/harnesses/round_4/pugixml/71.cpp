// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load at pugixml.cpp:7697:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7704:46 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace_own at pugixml.cpp:7763:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7739:46 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <pugixml.hpp>

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_71(const uint8_t* Data, size_t Size) {
    if (Size == 0) return 0;

    // Initialize random options and encoding based on input data to explore states
    unsigned int options = 0;
    if (Size >= 2) {
        options = static_cast<unsigned int>(Data[0]) | (static_cast<unsigned int>(Data[1]) << 8);
    }
    
    pugi::xml_encoding encoding;
    if (Size >= 3) {
        int enc_idx = Data[2] % 10;
        encoding = static_cast<pugi::xml_encoding>(enc_idx);
    } else {
        encoding = pugi::encoding_auto;
    }

    // 1. Test load_buffer (copies data, safe for raw binary)
    {
        pugi::xml_document doc;
        doc.load_buffer(Data, Size, options, encoding);
    }

    // 2. Test load_buffer_inplace (modifies buffer, requires lifetime management)
    // We must ensure the buffer is freed if the load fails or if we want to avoid leaks on success
    // However, load_buffer_inplace expects the buffer to persist. 
    // To avoid leaks in the fuzzer context, we rely on the document's destructor to free it ONLY if we use load_buffer_inplace_own.
    // For load_buffer_inplace, the caller owns the memory. Since we can't easily track it across the scope without complex logic,
    // and the fuzzer runs many iterations, we should prefer load_buffer_inplace_own for ownership transfer.
    // But to test load_buffer_inplace specifically, we allocate, load, and then manually free the buffer AFTER the document is destroyed.
    // Note: If load succeeds, the document uses the buffer. We must NOT free it until the document is destroyed.
    {
        pugi::xml_document doc;
        void* buffer_copy = malloc(Size);
        if (buffer_copy) {
            memcpy(buffer_copy, Data, Size);
            doc.load_buffer_inplace(buffer_copy, Size, options, encoding);
            // Document uses the buffer. We must wait for doc to be destroyed before freeing.
            // Since doc goes out of scope at the end of this block, we can free here.
            // However, if load failed, buffer_copy is still valid and owned by us.
            // If load succeeded, document owns the data but not the allocation? 
            // Actually, load_buffer_inplace DOES NOT take ownership. The caller must free the buffer.
            // So we free it here.
            free(buffer_copy);
        }
    }

    // 3. Test load_buffer_inplace_own (document owns and frees buffer)
    {
        pugi::xml_document doc;
        void* buffer_copy = malloc(Size);
        if (buffer_copy) {
            memcpy(buffer_copy, Data, Size);
            doc.load_buffer_inplace_own(buffer_copy, Size, options, encoding);
            // Document takes ownership. We do NOT free buffer_copy here.
            // It will be freed when 'doc' is destroyed at the end of this block.
        }
    }

    // 4. Test load_string (requires null-terminated string)
    {
        pugi::xml_document doc;
        // Allocate Size + 1 to ensure space for null terminator
        char* str_buf = new char[Size + 1];
        memcpy(str_buf, Data, Size);
        str_buf[Size] = '\0';
        doc.load_string(str_buf, options);
        delete[] str_buf;
    }

    // 5. Test load_file (requires a file)
    {
        pugi::xml_document doc;
        write_dummy_file(Data, Size);
        doc.load_file(L"./dummy_file", options, encoding);
    }

    // 6. Test load with std::istream (char)
    {
        pugi::xml_document doc;
        std::string str(reinterpret_cast<const char*>(Data), Size);
        std::istringstream stream(str, std::ios::binary);
        doc.load(stream, options, encoding);
    }

    // 7. Test load with std::wistringstream (wchar_t)
    {
        pugi::xml_document doc;
        size_t wide_size = (Size / sizeof(wchar_t)) * sizeof(wchar_t);
        if (wide_size > 0) {
            std::wstring ws(reinterpret_cast<const wchar_t*>(Data), wide_size / sizeof(wchar_t));
            std::wistringstream wstream(ws);
            doc.load(wstream, options);
        }
    }

    return 0;
}