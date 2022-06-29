#include "beans_parser.hpp"

int main()
{
    baseNode = new GrammarTable();
    
    GrammarTable* function_call = new GrammarTable();
    function_call->key = "function_call";
    function_call->values.push_back(
        vector<string> {"object", "LPAREN", "params", "RPAREN"});

    GrammarTable* object = new GrammarTable();
    object->key = "object";
    object->values.push_back(
        vector<string> {"typeobjects"});
    object->values.push_back(
        vector<string> {"IDENTIFIER"});

    GrammarTable* typeobject = new GrammarTable();
    typeobject->key = "typeobject";
    typeobject->values.push_back(
        vector<string> {"STRING"});
    typeobject->values.push_back(
        vector<string> {"INTEGER"});
    typeobject->values.push_back(
        vector<string>{"FLOAT"});
    typeobject->values.push_back(
        vector<string> {"DOUBLE"});

    GrammarTable* typeobjects = new GrammarTable();
    typeobjects->key = "typeobjects";
    typeobjects->values.push_back(
        vector<string> {"typeobject", "typeobjects"});
    typeobjects->values.push_back(
        vector<string> {"EPSILON"});

    GrammarTable* param = new GrammarTable();
    param->key = "param";
    param->values.push_back(
        vector<string> {"object"});

    GrammarTable* params = new GrammarTable();
    params->key = "params";
    params->values.push_back(
        vector<string> {"param", "comma", "params"});
    params->values.push_back(
        vector<string> {"EPSILON"});
    
    GrammarTable* comma = new GrammarTable();
    comma->key = "comma";
    comma->values.push_back(
        (vector<string>) {"COMMA"});
    comma->values.push_back(
        vector<string> {"EPSILON"});

    GrammarTable* tuple = new GrammarTable();
    tuple->key = "tuple";
    tuple->values.push_back(
        vector<string> {"LPAREN", "RPAREN"});

    baseNode->attachedNodes.push_back(function_call);
    baseNode->attachedNodes.push_back(object);
    baseNode->attachedNodes.push_back(typeobject);
    baseNode->attachedNodes.push_back(typeobjects);
    baseNode->attachedNodes.push_back(param);
    baseNode->attachedNodes.push_back(params);
    baseNode->attachedNodes.push_back(comma);
    baseNode->attachedNodes.push_back(tuple);
    
    FIRST();
    FOLLOW();
    ParsingTable();

    OutputMappingStack* ptr = parseTree();

    while(ptr != NULL)
    {
        for(const auto&
            [key, value]
            : ptr->expansion)
        {   
            std::cout << key << " -> ";
            for(string token : value)
                std::cout << token << " ";    
        }
        std::cout << endl;
        ptr = ptr->top;
    }

    // string code = "Hello";
    // yy_scan_string((const char*)code.c_str());
    // code = "World";
    // yylex();
    // code = "Hello";
    // yy_scan_string((const char*)code.c_str());
    // yylex();

    return 0;
}