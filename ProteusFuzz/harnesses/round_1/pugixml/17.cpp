// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::get at pugixml.cpp:6966:39 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7055:30 in pugixml.hpp
// pugi::xml_text::data at pugixml.cpp:7212:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7055:30 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
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
#include <string_view>

static bool write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (!f) return false;
    size_t written = fwrite(data, 1, size, f);
    fclose(f);
    return written == size;
}

extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document doc;

    // Try to load from the input data directly as a buffer
    // We pass the exact size to avoid buffer overflows
    if (!doc.load_buffer(Data, Size)) {
        // If parsing fails, try to write to a dummy file and load from there
        if (!write_dummy_file(Data, Size)) {
            return 0;
        }
        if (!doc.load_file("./dummy_file")) {
            // If still fails, try to parse a minimal valid XML
            char buffer[256];
            size_t len = (Size < 250) ? Size : 250;
            
            // Ensure we don't overflow the buffer
            if (len > 0) {
                std::memcpy(buffer, Data, len);
                buffer[len] = '\0';
            }
            
            // Construct a valid XML wrapper
            if (len > 0 && Data[0] != '<') {
                // Escape potential problematic chars if needed, but for fuzzing simple wrapping is often enough
                // We construct: <root><item>[data]</item></root>
                int written = std::snprintf(buffer, sizeof(buffer), "<root><item>%.*s</item></root>", (int)len, Data);
                if (written > 0 && (size_t)written < sizeof(buffer)) {
                    if (!doc.load_string(buffer)) return 0;
                } else {
                    // Fallback to a safe string if snprintf fails or overflows
                    if (!doc.load_string("<root><item>test</item></root>")) return 0;
                }
            } else {
                // If data starts with <, try to load it directly but ensure null termination
                // Since load_buffer failed, the data might be malformed or not null-terminated for C-string functions
                // We create a safe copy with null termination
                char safe_buf[256];
                size_t safe_len = (Size < 255) ? Size : 255;
                std::memcpy(safe_buf, Data, safe_len);
                safe_buf[safe_len] = '\0';
                
                if (!doc.load_string(safe_buf)) {
                    if (!doc.load_string("<root><item>test</item></root>")) return 0;
                }
            }
        }
    }

    // Get the root node
    pugi::xml_node root = doc.child("root");
    if (!root) {
        root = doc;
    }

    // 1. pugi::xml_node::child
    // We need a name to search for. Use a static buffer to avoid stack-use-after-scope
    static char search_name_buf[256];
    const char* search_name = "item";
    
    // Attempt to use input data for name if it looks like a valid identifier
    // Ensure we don't read past the input buffer
    if (Size > 0 && Size < 255) {
        // Copy input to static buffer, ensuring null termination
        std::memcpy(search_name_buf, Data, Size);
        search_name_buf[Size] = '\0';
        search_name = search_name_buf;
    }

    pugi::xml_node child_node = root.child(search_name);
    
    // If child_node is empty, try to create a structure to test the API
    if (!child_node) {
        // Ensure search_name is valid for append_child
        if (search_name && search_name[0]) {
             if (root.append_child(search_name)) {
                child_node = root.child(search_name);
            }
        }
    }

    if (!child_node) {
        if (root.append_child("test_node")) {
            child_node = root.child("test_node");
        }
    }

    if (!child_node) {
        // If we still don't have a child, we can't proceed with text operations safely
        return 0;
    }

    // 2. pugi::xml_node::text
    pugi::xml_text text_proxy = child_node.text();

    // 3. pugi::xml_text::get
    const pugi::char_t* text_content = text_proxy.get();
    (void)text_content;

    // 4. pugi::xml_text::set
    // Prepare a string_view_t from input, ensuring it's null-terminated in a static buffer
    static char set_buf[1025];
    size_t copy_len = (Size < 1024) ? Size : 1024;
    std::memcpy(set_buf, Data, copy_len);
    set_buf[copy_len] = '\0';
    std::string_view set_sv(set_buf, copy_len);

    bool set_result = text_proxy.set(set_sv);
    (void)set_result;

    // 5. pugi::xml_text::data
    pugi::xml_node data_node = text_proxy.data();
    (void)data_node;

    // 6. pugi::xml_text::set (again)
    // Use a modified version in the static buffer
    if (copy_len > 0) {
        set_buf[0] = 'A'; // Simple modification
    }
    std::string_view set_sv2(set_buf, copy_len);
    text_proxy.set(set_sv2);

    // 7. pugi::xml_node::last_child
    pugi::xml_node last_child = child_node.last_child();
    (void)last_child;

    return 0;
}