// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_file at pugixml.cpp:7739:46 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace_own at pugixml.cpp:7763:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7697:46 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
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
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <sstream>
#include <vector>
#include <random>

static uint32_t get_random_uint(const uint8_t* data, size_t size, size_t& offset) {
    if (offset + 4 > size) {
        offset = 0;
    }
    if (offset + 4 > size) {
        return 0;
    }
    uint32_t val = 0;
    val |= static_cast<uint32_t>(data[offset++]) << 0;
    val |= static_cast<uint32_t>(data[offset++]) << 8;
    val |= static_cast<uint32_t>(data[offset++]) << 16;
    val |= static_cast<uint32_t>(data[offset++]) << 24;
    return val;
}

static pugi::xml_encoding get_random_encoding(const uint8_t* data, size_t size, size_t& offset) {
    uint32_t val = get_random_uint(data, size, offset);
    int enc = val % 10;
    switch (enc) {
        case 0: return pugi::encoding_auto;
        case 1: return pugi::encoding_utf8;
        case 2: return pugi::encoding_utf16_le;
        case 3: return pugi::encoding_utf16_be;
        case 4: return pugi::encoding_utf16;
        case 5: return pugi::encoding_utf32_le;
        case 6: return pugi::encoding_utf32_be;
        case 7: return pugi::encoding_utf32;
        case 8: return pugi::encoding_wchar;
        case 9: return pugi::encoding_latin1;
        default: return pugi::encoding_auto;
    }
}

extern "C" int LLVMFuzzerTestOneInput_80(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    size_t offset = 0;
    
    if (Size < 8) return 0;

    uint32_t choice = get_random_uint(Data, Size, offset);
    uint32_t options = get_random_uint(Data, Size, offset);
    pugi::xml_encoding encoding = get_random_encoding(Data, Size, offset);
    
    size_t content_start = offset;
    size_t content_end = Size;

    if (content_end <= content_start) return 0;

    pugi::xml_document doc;
    pugi::xml_parse_result result;

    switch (choice % 6) {
        case 0: {
            // load_buffer
            std::vector<uint8_t> buffer(Data + content_start, Data + content_end);
            result = doc.load_buffer(buffer.data(), buffer.size(), options, encoding);
            (void)result;
            break;
        }
        case 1: {
            // load (from stream)
            {
                size_t len = content_end - content_start;
                std::string str(reinterpret_cast<const char*>(Data + content_start), len);
                std::istringstream stream(str, std::ios::binary);
                result = doc.load(stream, options, encoding);
                (void)result;
            }
            break;
        }
        case 2: {
            // load_buffer_inplace
            if (content_end > content_start) {
                size_t len = content_end - content_start;
                char* inplace_buf = static_cast<char*>(std::malloc(len));
                if (inplace_buf) {
                    std::memcpy(inplace_buf, Data + content_start, len);
                    result = doc.load_buffer_inplace(inplace_buf, len, options, encoding);
                    (void)result;
                    // load_buffer_inplace does NOT take ownership.
                    // We must free the buffer ourselves after the call.
                    std::free(inplace_buf);
                }
            }
            break;
        }
        case 3: {
            // load_file
            const char* dummy_file = "./dummy_file";
            FILE* f = std::fopen(dummy_file, "wb");
            if (f) {
                size_t written = std::fwrite(Data + content_start, 1, content_end - content_start, f);
                std::fclose(f);
                if (written > 0) {
                    std::wstring wpath(L"./dummy_file");
                    result = doc.load_file(wpath.c_str(), options, encoding);
                    (void)result;
                }
            }
            std::remove(dummy_file);
            break;
        }
        case 4: {
            // load_buffer_inplace_own
            if (content_end > content_start) {
                size_t len = content_end - content_start;
                // load_buffer_inplace_own takes ownership and frees the buffer.
                // We use malloc here. While pugixml typically expects its own allocator,
                // passing malloc'd memory is often accepted in standard builds, 
                // but the key is that we DO NOT free it ourselves.
                void* own_buf = std::malloc(len);
                if (own_buf) {
                    std::memcpy(own_buf, Data + content_start, len);
                    result = doc.load_buffer_inplace_own(own_buf, len, options, encoding);
                    (void)result;
                    // Do NOT free own_buf here. The document will free it on destruction.
                }
            }
            break;
        }
        case 5: {
            // load_string
            if (content_end > content_start) {
                size_t len = content_end - content_start;
                // Ensure null termination for load_string
                std::vector<char> temp_buf(Data + content_start, Data + content_end);
                temp_buf.push_back(0);
                result = doc.load_string(temp_buf.data(), options);
                (void)result;
            }
            break;
        }
    }

    return 0;
}