// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6449:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6449:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6449:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6449:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6449:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
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
#include "pugixml.hpp"
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

static void populate_document(pugi::xml_document& doc, const uint8_t* data, size_t size) {
    // Use data to create XML content
    std::string xml_content;
    if (size > 0) {
        // Create some XML structure with names/values derived from fuzzer input
        xml_content = "<root>";
        
        // Add some child nodes with names from data
        for (size_t i = 0; i < std::min(size, (size_t)10); ++i) {
            char node_name[32];
            snprintf(node_name, sizeof(node_name), "node%d", data[i] % 10);
            xml_content += "<";
            xml_content += node_name;
            xml_content += ">";
            xml_content += "value";
            xml_content += "</";
            xml_content += node_name;
            xml_content += ">";
        }
        
        xml_content += "</root>";
    } else {
        xml_content = "<root><node1>value1</node1><node2>value2</node2><node3>value3</node3></root>";
    }
    
    doc.load_string(xml_content.c_str());
}

static std::string get_name_from_data(const uint8_t* data, size_t size, size_t offset) {
    if (offset >= size) {
        return "default";
    }
    
    // Extract a null-terminated string from data
    size_t max_len = std::min(size - offset, (size_t)50);
    std::string name;
    for (size_t i = 0; i < max_len; ++i) {
        char c = static_cast<char>(data[offset + i]);
        if (c == '\0') {
            break;
        }
        name.push_back(c);
    }
    
    if (name.empty()) {
        name = "default";
    }
    
    return name;
}

extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* Data, size_t Size) {
    // Initialize pugixml document
    pugi::xml_document doc;
    populate_document(doc, Data, Size);
    
    // Get root node
    pugi::xml_node root = doc.root().first_child();
    if (!root) {
        return 0;
    }
    
    // Create some additional nodes for testing
    pugi::xml_node proto_node = root.append_child("proto_node");
    proto_node.append_attribute("attr1") = "value1";
    proto_node.append_child("child_of_proto").text() = "text";
    
    pugi::xml_node target_node = root.append_child("target_node");
    target_node.append_attribute("attr2") = "value2";
    
    // Prepare names for child() calls
    std::vector<std::string> names;
    for (size_t i = 0; i < 30 && i < Size; i += 5) {
        names.push_back(get_name_from_data(Data, Size, i));
    }
    if (names.empty()) {
        names.push_back("default");
    }
    
    size_t name_index = 0;
    
    try {
        // 1. pugi::xml_node::child
        root.child(names[name_index % names.size()].c_str());
        name_index++;
        
        // 2. pugi::xml_node::child
        root.child(names[name_index % names.size()].c_str());
        name_index++;
        
        // 3. pugi::xml_node::insert_copy_before
        root.insert_copy_before(proto_node, target_node);
        
        // 4. pugi::xml_node::child
        root.child(names[name_index % names.size()].c_str());
        name_index++;
        
        // 5. pugi::xml_node::insert_copy_before
        root.insert_copy_before(proto_node, target_node);
        
        // 6. pugi::xml_node::child
        root.child(names[name_index % names.size()].c_str());
        name_index++;
        
        // 7. pugi::xml_node::child
        root.child(names[name_index % names.size()].c_str());
        name_index++;
        
        // 8. pugi::xml_node::child
        root.child(names[name_index % names.size()].c_str());
        name_index++;
        
        // 9. pugi::xml_node::child
        root.child(names[name_index % names.size()].c_str());
        name_index++;
        
        // 10. pugi::xml_node::insert_copy_before
        root.insert_copy_before(proto_node, target_node);
        
        // 11. pugi::xml_node::child
        root.child(names[name_index % names.size()].c_str());
        name_index++;
        
        // 12. pugi::xml_node::first_child
        root.first_child();
        
        // 13. pugi::xml_node::name
        root.name();
        
        // 14. pugi::xml_node::child
        root.child(names[name_index % names.size()].c_str());
        name_index++;
        
        // 15. pugi::xml_node::insert_copy_before
        root.insert_copy_before(proto_node, target_node);
        
        // 16. pugi::xml_node::child
        root.child(names[name_index % names.size()].c_str());
        name_index++;
        
        // 17. pugi::xml_node::first_child
        root.first_child();
        
        // 18. pugi::xml_node::child
        root.child(names[name_index % names.size()].c_str());
        name_index++;
        
        // 19. pugi::xml_node::last_child
        root.last_child();
        
        // 20. pugi::xml_node::name
        root.name();
        
        // 21. pugi::xml_node::child
        root.child(names[name_index % names.size()].c_str());
        name_index++;
        
        // 22. pugi::xml_node::insert_copy_before
        root.insert_copy_before(proto_node, target_node);
        
        // 23. pugi::xml_node::child
        root.child(names[name_index % names.size()].c_str());
        name_index++;
        
        // 24. pugi::xml_node::first_child
        root.first_child();
        
        // 25. pugi::xml_node::next_sibling
        root.next_sibling();
        
        // 26. pugi::xml_node::child
        root.child(names[name_index % names.size()].c_str());
        name_index++;
        
        // 27. pugi::xml_node::first_child
        root.first_child();
        
        // 28. pugi::xml_node::value
        root.value();
        
    } catch (...) {
        // Catch any exceptions to prevent crashes
    }
    
    return 0;
}