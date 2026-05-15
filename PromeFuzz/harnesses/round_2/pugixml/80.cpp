// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5754:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::find_child at pugixml.hpp:693:42 in pugixml.hpp
// pugi::xml_node::find_child at pugixml.hpp:693:42 in pugixml.hpp
// pugi::xml_node::find_child at pugixml.hpp:693:42 in pugixml.hpp
// pugi::xml_node::find_child at pugixml.hpp:693:42 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::find_child at pugixml.hpp:693:42 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
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
#include <fstream>
#include <functional>

static void create_dummy_xml(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (!file) return;
    
    // Write XML declaration and root element
    file << "<?xml version=\"1.0\"?>\n";
    file << "<root>\n";
    
    // Create some child nodes with names based on input data
    file << "  <child1>value1</child1>\n";
    file << "  <child2 attr=\"value2\">text</child2>\n";
    file << "  <node>content</node>\n";
    
    // Add a node with data from fuzzer input (ensure valid XML)
    file << "  <data>";
    for (size_t i = 0; i < size && i < 100; ++i) {
        if (data[i] >= 32 && data[i] <= 126 && data[i] != '<' && data[i] != '&') {
            file << static_cast<char>(data[i]);
        } else {
            file << '?';
        }
    }
    file << "</data>\n";
    
    // Add more nodes for traversal
    file << "  <sibling>first</sibling>\n";
    file << "  <sibling>second</sibling>\n";
    file << "  <sibling>third</sibling>\n";
    file << "  <child3>test</child3>\n";
    file << "  <last>end</last>\n";
    
    file << "</root>\n";
    file.close();
}

static std::string extract_string(const uint8_t* data, size_t size, size_t offset = 0) {
    if (offset >= size) return "";
    
    size_t length = std::min<size_t>(size - offset, 100);
    std::string result;
    result.reserve(length + 1);
    
    for (size_t i = 0; i < length; ++i) {
        char c = static_cast<char>(data[offset + i]);
        if (c == 0) break;
        result.push_back(c);
    }
    
    // Ensure null termination for C-string usage
    result.push_back('\0');
    result.pop_back(); // Remove the extra null we added
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_80(const uint8_t* data, size_t size) {
    // Create dummy XML file with fuzzer data embedded
    create_dummy_xml(data, size);
    
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("./dummy_file");
    
    if (!result) {
        // If loading fails, try parsing directly from memory
        std::string xml_content = "<?xml version=\"1.0\"?><root>";
        xml_content += extract_string(data, size);
        xml_content += "</root>";
        result = doc.load_buffer(xml_content.c_str(), xml_content.size());
    }
    
    if (!result) {
        // Create minimal valid XML document
        const char* minimal_xml = "<?xml version=\"1.0\"?><root><a/><b/><c/></root>";
        doc.load_buffer(minimal_xml, strlen(minimal_xml));
    }
    
    // Get root node
    pugi::xml_node root = doc.root().first_child();
    if (!root) return 0;
    
    // Extract string for node name searches
    std::string search_name = extract_string(data, size, 0);
    std::string search_name2 = extract_string(data, size, std::min<size_t>(size/2, size));
    
    // Test 1: previous_sibling()
    {
        // Start from a child node and traverse backwards
        pugi::xml_node node = root.last_child();
        while (node) {
            pugi::xml_node prev = node.previous_sibling();
            if (!prev) break;
            node = prev;
        }
        
        // Test with specific name filter (empty string means any sibling)
        pugi::xml_node child = root.child("sibling");
        if (child) {
            pugi::xml_node prev_sibling = child.previous_sibling();
            if (prev_sibling) {
                // Try getting previous sibling with specific name
                std::string name_str = search_name.empty() ? "" : search_name;
                pugi::xml_node named_prev = child.previous_sibling(name_str.c_str());
            }
        }
    }
    
    // Test 2: first_child()
    {
        pugi::xml_node first = root.first_child();
        if (first) {
            // Recursively get first children
            pugi::xml_node current = first;
            for (int i = 0; i < 10 && current; ++i) {
                current = current.first_child();
            }
        }
    }
    
    // Test 3: child()
    {
        // Search for child with name from fuzzer input
        if (!search_name.empty()) {
            pugi::xml_node found = root.child(search_name.c_str());
        }
        
        // Search for child with second name from fuzzer input
        if (!search_name2.empty()) {
            pugi::xml_node found2 = root.child(search_name2.c_str());
        }
        
        // Search for common node names
        const char* common_names[] = {"child1", "child2", "node", "data", "sibling", nullptr};
        for (int i = 0; common_names[i]; ++i) {
            pugi::xml_node found = root.child(common_names[i]);
        }
    }
    
    // Test 4: last_child()
    {
        pugi::xml_node last = root.last_child();
        if (last) {
            // Recursively get last children
            pugi::xml_node current = last;
            for (int i = 0; i < 10 && current; ++i) {
                current = current.last_child();
            }
        }
    }
    
    // Test 5: find_child()
    {
        // Define various predicates
        auto predicate1 = [](pugi::xml_node node) -> bool {
            return node.name() && std::strlen(node.name()) > 0;
        };
        
        auto predicate2 = [&search_name](pugi::xml_node node) -> bool {
            return node.name() && std::strstr(node.name(), search_name.c_str()) != nullptr;
        };
        
        auto predicate3 = [](pugi::xml_node node) -> bool {
            return node.first_child() || node.first_attribute();
        };
        
        // Use the predicates
        pugi::xml_node found1 = root.find_child(predicate1);
        pugi::xml_node found2 = root.find_child(predicate2);
        pugi::xml_node found3 = root.find_child(predicate3);
        
        // Test with inline lambda
        pugi::xml_node found4 = root.find_child([](pugi::xml_node n) {
            const char* name = n.name();
            return name && name[0] == 'c'; // Nodes starting with 'c'
        });
    }
    
    // Test 6: parent()
    {
        // Get parent of root (should be empty)
        pugi::xml_node root_parent = root.parent();
        
        // Get parents of all children
        for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
            pugi::xml_node parent = child.parent();
            
            // Get parent of grandchildren
            for (pugi::xml_node grandchild = child.first_child(); grandchild; grandchild = grandchild.next_sibling()) {
                pugi::xml_node grandparent = grandchild.parent();
            }
        }
    }
    
    // Additional traversal tests to explore more states
    {
        // Chain multiple operations
        pugi::xml_node node = root.first_child();
        if (node) {
            node = node.next_sibling();
            if (node) {
                pugi::xml_node parent = node.parent();
                pugi::xml_node child = node.child(search_name.c_str());
                pugi::xml_node prev = node.previous_sibling();
                pugi::xml_node last = node.last_child();
            }
        }
        
        // Try to traverse from various starting points
        std::vector<pugi::xml_node> nodes;
        for (pugi::xml_node n = root.first_child(); n; n = n.next_sibling()) {
            nodes.push_back(n);
        }
        
        // Use fuzzer data to select which nodes to operate on
        if (!nodes.empty()) {
            size_t idx = size > 0 ? data[0] % nodes.size() : 0;
            pugi::xml_node selected = nodes[idx];
            
            // Perform all operations on selected node
            pugi::xml_node prev = selected.previous_sibling();
            pugi::xml_node first = selected.first_child();
            pugi::xml_node child = selected.child(search_name.c_str());
            pugi::xml_node last = selected.last_child();
            pugi::xml_node parent = selected.parent();
            
            auto predicate = [](pugi::xml_node n) { return n.name() != nullptr; };
            pugi::xml_node found = selected.find_child(predicate);
        }
    }
    
    return 0;
}