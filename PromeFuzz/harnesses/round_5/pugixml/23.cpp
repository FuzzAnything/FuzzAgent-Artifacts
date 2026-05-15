// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
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
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6449:34 in pugixml.hpp
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
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

static void initialize_dummy_document(pugi::xml_document& doc) {
    const char* xml_content = 
        "<root>"
        "  <child1 name=\"attr1\">value1</child1>"
        "  <child2 name=\"attr2\">value2</child2>"
        "  <child3 name=\"attr3\">value3</child3>"
        "  <child4 name=\"attr4\">value4</child4>"
        "  <child5 name=\"attr5\">value5</child5>"
        "  <child6 name=\"attr6\">value6</child6>"
        "</root>";
    doc.load_string(xml_content);
}

static std::string get_random_name(const uint8_t* data, size_t size, size_t offset) {
    if (offset >= size) return "default";
    
    // Create a null-terminated string from fuzzer data
    size_t len = std::min(size - offset, (size_t)20);
    std::string name(reinterpret_cast<const char*>(data + offset), len);
    
    // Ensure null termination for C string compatibility
    if (!name.empty()) {
        name.push_back('\0');
        name.pop_back(); // Remove the added null for std::string, but ensures we have capacity
    }
    
    return name.empty() ? "default" : name;
}

extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t* Data, size_t Size) {
    // Initialize pugixml document
    pugi::xml_document doc;
    
    // Try to parse the fuzzer input as XML
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    // If parsing fails, use a dummy document
    if (!result) {
        initialize_dummy_document(doc);
    }
    
    // Get root node
    pugi::xml_node root = doc.root().first_child();
    if (!root) {
        // Create a minimal document if no root exists
        doc.append_child("root");
        root = doc.root().first_child();
    }
    
    // Create some child nodes if needed
    if (!root.first_child()) {
        root.append_child("child0");
        root.append_child("child1");
        root.append_child("child2");
        root.append_child("child3");
    }
    
    // Generate names from fuzzer data
    std::vector<std::string> names;
    for (size_t i = 0; i < 20 && i * 10 < Size; i++) {
        names.push_back(get_random_name(Data, Size, i * 10));
    }
    
    // Ensure we have enough names
    while (names.size() < 20) {
        names.push_back("child" + std::to_string(names.size()));
    }
    
    // Execute the sequence of API calls
    try {
        // 1. pugi::xml_node::child
        pugi::xml_node node1 = root.child(names[0].c_str());
        
        // 2. pugi::xml_node::child
        pugi::xml_node node2 = root.child(names[1].c_str());
        
        // 3. pugi::xml_node::insert_copy_before
        pugi::xml_node node3 = root.insert_copy_before(node1, node2);
        
        // 4. pugi::xml_node::child
        pugi::xml_node node4 = root.child(names[2].c_str());
        
        // 5. pugi::xml_node::insert_copy_before
        pugi::xml_node node5 = root.insert_copy_before(node3, node4);
        
        // 6. pugi::xml_node::child
        pugi::xml_node node6 = root.child(names[3].c_str());
        
        // 7. pugi::xml_node::child
        pugi::xml_node node7 = root.child(names[4].c_str());
        
        // 8. pugi::xml_node::child
        pugi::xml_node node8 = root.child(names[5].c_str());
        
        // 9. pugi::xml_node::child
        pugi::xml_node node9 = root.child(names[6].c_str());
        
        // 10. pugi::xml_node::insert_copy_before
        pugi::xml_node node10 = root.insert_copy_before(node6, node9);
        
        // 11. pugi::xml_node::child
        pugi::xml_node node11 = root.child(names[7].c_str());
        
        // 12. pugi::xml_node::first_child
        pugi::xml_node node12 = root.first_child();
        
        // 13. pugi::xml_node::name
        const char* name13 = node12.name();
        (void)name13; // Prevent unused variable warning
        
        // 14. pugi::xml_node::child
        pugi::xml_node node14 = root.child(names[8].c_str());
        
        // 15. pugi::xml_node::insert_copy_before
        pugi::xml_node node15 = root.insert_copy_before(node12, node14);
        
        // 16. pugi::xml_node::child
        pugi::xml_node node16 = root.child(names[9].c_str());
        
        // 17. pugi::xml_node::first_child
        pugi::xml_node node17 = root.first_child();
        
        // 18. pugi::xml_node::child
        pugi::xml_node node18 = root.child(names[10].c_str());
        
        // 19. pugi::xml_node::last_child
        pugi::xml_node node19 = root.last_child();
        
        // 20. pugi::xml_node::name
        const char* name20 = node19.name();
        (void)name20; // Prevent unused variable warning
        
        // 21. pugi::xml_node::child
        pugi::xml_node node21 = root.child(names[11].c_str());
        
        // 22. pugi::xml_node::insert_copy_before
        pugi::xml_node node22 = root.insert_copy_before(node19, node21);
        
        // 23. pugi::xml_node::child
        pugi::xml_node node23 = root.child(names[12].c_str());
        
        // 24. pugi::xml_node::first_child
        pugi::xml_node node24 = root.first_child();
        
        // 25. pugi::xml_node::next_sibling
        pugi::xml_node node25 = node24.next_sibling();
        
        // 26. pugi::xml_node::child
        pugi::xml_node node26 = root.child(names[13].c_str());
        
        // 27. pugi::xml_node::first_child
        pugi::xml_node node27 = root.first_child();
        
        // 28. pugi::xml_node::value
        const char* value28 = node27.value();
        (void)value28; // Prevent unused variable warning
        
    } catch (...) {
        // Catch any exceptions to prevent crashes
    }
    
    return 0;
}