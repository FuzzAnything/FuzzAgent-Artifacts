// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6030:30 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6030:30 in pugixml.hpp
// pugi::as_utf8 at pugixml.cpp:7849:44 in pugixml.hpp
// pugi::as_utf8 at pugixml.cpp:7842:44 in pugixml.hpp
// pugi::as_wide at pugixml.cpp:7861:59 in pugixml.hpp
// pugi::as_wide at pugixml.cpp:7854:59 in pugixml.hpp
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
#include <cstring>
#include <string>
#include <vector>
#include <stdexcept>

static std::wstring create_wstring_from_data(const uint8_t* data, size_t size) {
    if (size == 0) return L"";
    // Ensure the size is even for wchar_t alignment if needed, though we cast directly
    size_t wsize = (size + 1) / 2;
    std::wstring result;
    result.resize(wsize);
    for (size_t i = 0; i < wsize && i * 2 < size; ++i) {
        result[i] = static_cast<wchar_t>(data[i * 2]);
        if (i * 2 + 1 < size) {
            // Combine two bytes into one wchar_t if possible, or just use the second byte as high part
            // For fuzzing, simple casting is sufficient to generate diverse inputs
            result[i] |= static_cast<wchar_t>(data[i * 2 + 1]) << 8;
        }
    }
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_82(const uint8_t* Data, size_t Size) {
    if (Size == 0) return 0;

    try {
        // --- Test pugi::as_utf8 ---
        
        // Overload 1: as_utf8(const std::basic_string<wchar_t>&)
        std::wstring wide_str = create_wstring_from_data(Data, Size);
        std::string utf8_from_wide_str = pugi::as_utf8(wide_str);
        
        // Overload 2: as_utf8(const wchar_t*)
        // We need a null-terminated wide string. Create a buffer.
        std::vector<wchar_t> wide_buf(wide_str.begin(), wide_str.end());
        wide_buf.push_back(L'\0');
        std::string utf8_from_wide_ptr = pugi::as_utf8(wide_buf.data());

        // --- Test pugi::as_wide ---

        // Overload 1: as_wide(const std::basic_string<char>&)
        std::string narrow_str(reinterpret_cast<const char*>(Data), Size);
        std::wstring wide_from_narrow_str = pugi::as_wide(narrow_str);

        // Overload 2: as_wide(const char*)
        // Ensure null termination for safety, though as_wide handles non-null pointers
        std::vector<char> narrow_buf(narrow_str.begin(), narrow_str.end());
        narrow_buf.push_back('\0');
        std::wstring wide_from_narrow_ptr = pugi::as_wide(narrow_buf.data());

        // --- Test pugi::xml_node::set_value ---
        
        pugi::xml_node node;
        // Default constructor creates an empty node (type null).
        // set_value returns false for elements/attributes, but we test the call.
        // We need to create a valid text node or similar to see true return, 
        // but the task is to fuzz the function invocation.
        
        // Overload 1: set_value(const char_t*)
        // Convert input data to char_t (which is char or wchar_t depending on build)
        // We assume PUGIXML_WCHAR_MODE might be off by default, but we handle both via char_t
        // For fuzzing, we pass the raw data cast to char_t*
        // Note: If PUGIXML_WCHAR_MODE is on, char_t is wchar_t. 
        // We will cast the data appropriately based on the type of char_t.
        
        // Helper to get char_t* from data
        const pugi::char_t* char_t_ptr = nullptr;
        
        // We cannot easily know at compile time if char_t is char or wchar_t without macros,
        // but we can try both paths or rely on the library's definition.
        // Let's assume the standard build (char_t = char) for the pointer cast, 
        // but if it's wchar_t, the cast is still valid for fuzzing (just different interpretation).
        // To be safe, we create a buffer of char_t.
        
        std::vector<pugi::char_t> char_t_buf;
        if (sizeof(pugi::char_t) == 1) {
            char_t_buf.assign(reinterpret_cast<const char*>(Data), reinterpret_cast<const char*>(Data) + Size);
        } else {
            // If wchar_t, use the wide string we created earlier
            char_t_buf.assign(wide_str.begin(), wide_str.end());
        }
        char_t_buf.push_back(pugi::char_t(0)); // Null terminate
        
        char_t_ptr = char_t_buf.data();
        
        // Invoke set_value with const char_t*
        // Note: set_value on a null node (default constructed) usually returns false or does nothing
        // but it exercises the code path.
        bool res1 = node.set_value(char_t_ptr);
        
        // Overload 2: set_value(string_view_t)
        // string_view_t is typedef for std::string_view or similar depending on version
        // In modern pugixml, it's std::string_view for char_t
        // We construct a string_view from our buffer
        pugi::string_view_t sv(char_t_ptr);
        bool res2 = node.set_value(sv);

        // Additional state exploration: Create a document and a text node to test set_value more effectively
        pugi::xml_document doc;
        if (doc.load_string("<root/>")) {
            pugi::xml_node root = doc.child("root");
            if (root) {
                pugi::xml_node text_node = root.append_child(pugi::node_pcdata);
                if (text_node) {
                    text_node.set_value(char_t_ptr);
                    text_node.set_value(sv);
                }
            }
        }

        // Suppress unused variable warnings
        (void)utf8_from_wide_str;
        (void)utf8_from_wide_ptr;
        (void)wide_from_narrow_str;
        (void)wide_from_narrow_ptr;
        (void)res1;
        (void)res2;

    } catch (...) {
        // Catch any unexpected exceptions to prevent fuzzer crash
        return 0;
    }

    return 0;
}