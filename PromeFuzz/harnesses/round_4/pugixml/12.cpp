// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::previous_sibling at pugixml.cpp:5811:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5754:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5811:34 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5754:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5811:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5754:34 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "pugixml.hpp"
#include <cstdint>
#include <cstring>
#include <string>
#include <string_view>

static void generate_dummy_xml(const uint8_t* Data, size_t Size, const char* filename) {
    FILE* f = fopen(filename, "wb");
    if (!f) return;
    
    // Write XML header and root element
    const char* xml_header = "<?xml version=\"1.0\"?><root>";
    fwrite(xml_header, 1, strlen(xml_header), f);
    
    // Write fuzzer data as text content (ensure valid XML by escaping)
    for (size_t i = 0; i < Size; ++i) {
        if (Data[i] == '<') fwrite("&lt;", 1, 4, f);
        else if (Data[i] == '>') fwrite("&gt;", 1, 4, f);
        else if (Data[i] == '&') fwrite("&amp;", 1, 5, f);
        else if (Data[i] == '\'') fwrite("&apos;", 1, 6, f);
        else if (Data[i] == '"') fwrite("&quot;", 1, 6, f);
        else if (Data[i] >= 32 && Data[i] <= 126) fwrite(&Data[i], 1, 1, f);
        else fwrite("?", 1, 1, f);
    }
    
    // Add some child elements for traversal
    const char* xml_footer = "<child1/><child2/><child3/><child4/><child5/><child6/></root>";
    fwrite(xml_footer, 1, strlen(xml_footer), f);
    
    fclose(f);
}

static std::string create_null_terminated_string(const uint8_t* Data, size_t Size) {
    if (Size == 0) return "";
    size_t len = Size > 100 ? 100 : Size; // Limit length
    std::string result(reinterpret_cast<const char*>(Data), len);
    result.push_back('\0'); // Ensure null termination
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* Data, size_t Size) {
    // Generate XML file from fuzzer input
    generate_dummy_xml(Data, Size, "./dummy_file");
    
    // Parse XML document
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("./dummy_file");
    if (!result) {
        // If parsing fails, create a minimal valid document
        const char* minimal_xml = "<?xml version=\"1.0\"?><root><child1/><child2/></root>";
        result = doc.load_string(minimal_xml);
        if (!result) return 0;
    }
    
    // Get root node and first child to start traversal
    pugi::xml_node root = doc.root();
    pugi::xml_node node = root.first_child();
    if (!node) return 0;
    
    // Create null-terminated string for name parameters
    std::string name_str = create_null_terminated_string(Data, Size);
    std::string_view name_sv(name_str.c_str(), name_str.length() - 1); // Exclude null terminator
    
    // Execute function calls in specified order
    // 1-2: next_sibling
    node = node.next_sibling();
    node = node.next_sibling(name_str.c_str());
    node = node.next_sibling(name_sv);
    node = node.next_sibling();
    
    // 3-4: previous_sibling
    node = node.previous_sibling();
    node = node.previous_sibling(name_str.c_str());
    node = node.previous_sibling(name_sv);
    node = node.previous_sibling();
    
    // 5-10: child (6 times)
    for (int i = 0; i < 6; ++i) {
        node = node.child(name_str.c_str());
    }
    
    // 11-12: next_sibling
    node = node.next_sibling();
    node = node.next_sibling(name_str.c_str());
    node = node.next_sibling(name_sv);
    node = node.next_sibling();
    
    // 13-14: previous_sibling
    node = node.previous_sibling();
    node = node.previous_sibling(name_str.c_str());
    node = node.previous_sibling(name_sv);
    node = node.previous_sibling();
    
    // 15-18: next_sibling (4 times)
    for (int i = 0; i < 4; ++i) {
        node = node.next_sibling();
        node = node.next_sibling(name_str.c_str());
        node = node.next_sibling(name_sv);
    }
    
    // 19-22: previous_sibling (4 times)
    for (int i = 0; i < 4; ++i) {
        node = node.previous_sibling();
        node = node.previous_sibling(name_str.c_str());
        node = node.previous_sibling(name_sv);
    }
    
    // Cleanup
    remove("./dummy_file");
    
    return 0;
}