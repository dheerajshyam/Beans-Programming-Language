#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <stack>

using namespace std;

struct Token
{
    int count;
    string TokenType;
    string value;

    struct Token* link;
};

struct GrammarTable
{
    string key;
    vector<vector<string>> values;

    vector<GrammarTable*> attachedNodes;
};

void FIRST();
void FOLLOW();
void ParsingTable();
void parseTree();

bool isAllLower(string value)
{
    bool isAllLower = true;
    for(char ch : value)
        if(isalpha(ch) && !islower(ch)) { isAllLower = false; break; }
    return isAllLower;
}

GrammarTable* baseNode;

map<string, vector<string>> FIRST_table;
map<string, vector<string>> FOLLOW_table;

vector<string> predictive_parsing_table[5][8];

enum TERMINAL_ID
{
    LPAREN,
    RPAREN,
    IDENTIFIER,
    STRING,
    INTEGER,
    FLOAT,
    DOUBLE,
    EPSILON
};

enum NON_TERMINAL_ID
{
    FUNCTION_CALL,
    OBJECT,
    TYPE_OBJECT,
    PARAM,
    PARAMS
};

map<string,TERMINAL_ID> terminal_id_mapping;
map<string,NON_TERMINAL_ID> non_terminal_id_mapping;

string start_points[] = {"function_call"};

struct StorageStack
{
    struct StorageStack* bottom;

    string value;

    struct StorageStack* top;
};

struct InputStack
{
    struct InputStack* bottom;

    vector<string> token;

    struct InputStack* top;
};

struct OutputMappingStack
{
    struct OutputMappingStack* bottom;

    map<string, vector<string>> expansion;
    bool isFilled;

    struct OutputMappingStack* top;
};

int main()
{
    baseNode = new GrammarTable();
    
    GrammarTable* function_call = new GrammarTable();
    function_call->key = "function_call";
    function_call->values.push_back(vector<string>{"object", "LPAREN", "params", "RPAREN"});

    GrammarTable* object = new GrammarTable();
    object->key = "object";
    object->values.push_back(vector<string>{"typeobject"});
    object->values.push_back(vector<string>{"IDENTIFIER"});

    GrammarTable* typeobject = new GrammarTable();
    typeobject->key = "typeobject";
    typeobject->values.push_back(vector<string>{"STRING"});
    typeobject->values.push_back(vector<string>{"INTEGER"});
    typeobject->values.push_back(vector<string>{"FLOAT"});
    typeobject->values.push_back(vector<string>{"DOUBLE"});

    GrammarTable* param = new GrammarTable();
    param->key = "param";
    param->values.push_back(vector<string>{"object"});

    GrammarTable* params = new GrammarTable();
    params->key = "params";
    params->values.push_back(vector<string>{"param", "params"});
    params->values.push_back(vector<string>{"EPSILON"});

    baseNode->attachedNodes.push_back(function_call);
    baseNode->attachedNodes.push_back(object);
    baseNode->attachedNodes.push_back(typeobject);
    baseNode->attachedNodes.push_back(param);
    baseNode->attachedNodes.push_back(params);
    
    FIRST();
    FOLLOW();
    ParsingTable();

    parseTree();

    return 0;
}

vector<string> resolveFIRSTOfNonTerminal(string nonTerminal, vector<string> FIRST_productions)
{
    for(int i = 0; i < baseNode->attachedNodes.size(); i++)        
    {
        if(nonTerminal == baseNode->attachedNodes[i]->key)
        {
            vector<vector<string>> productions = baseNode->attachedNodes[i]->values;
            for(int index = 0; index < productions.size(); index++)
            {
                vector<string> production = productions[index];
                if(!isAllLower(production[0]))
                {
                    if(std::find(FIRST_productions.begin(), FIRST_productions.end(), production[0])
                            == FIRST_productions.end())
                        FIRST_productions.push_back(production[0]);
                }
                else
                    FIRST_productions = resolveFIRSTOfNonTerminal(production[0],  FIRST_productions);
            }
        }
    }
    return FIRST_productions;
}
void FIRST()
{
    vector<string> FIRST_productions = {};
    for(int i = 0; i < baseNode->attachedNodes.size(); i++)
    {
        string nonTerminal = baseNode->attachedNodes[i]->key;
        vector<vector<string>> productions = baseNode->attachedNodes[i]->values;
        for(int index = 0; index < productions.size(); index++)
        {
            vector<string> production = productions[index];
            if(!isAllLower(production[0]))
            {
                if(std::find(FIRST_productions.begin(), FIRST_productions.end(), production[0])
                            == FIRST_productions.end())
                    FIRST_productions.push_back(production[0]);
            }
            else
                FIRST_productions = resolveFIRSTOfNonTerminal(production[0],  FIRST_productions);
        }
        FIRST_table[nonTerminal] = FIRST_productions;
        FIRST_productions.clear();
    }
}

vector<vector<string>> findNonTerminalInProduction(string nonTerminal)
{
    vector<vector<string>> productions = {};
    for(int i = 0; i < baseNode->attachedNodes.size(); i++)
    {
        for(int index = 0; index < baseNode->attachedNodes[i]->values.size(); index++)
        {
            vector<string> production = baseNode->attachedNodes[i]->values[index];
            
            if(std::find(production.begin(), production.end(), nonTerminal)
                        != production.end())
                productions.push_back(production);
        }
    }
    return productions;
}

vector<string> getFollowProductions(string nonTerminal)
{
    vector<string> FOLLOW_productions = {};

    if(findNonTerminalInProduction(nonTerminal).size() != 0)
    {
        for(vector<string> production : findNonTerminalInProduction(nonTerminal))
        {
            for(int index = 0; index < production.size(); index++)
            {
                string token = production[index];
                
                if(token == nonTerminal)
                {
                    if(index != 
                            (production.size() - 1))
                    {
                        if(isAllLower(production[index + 1]))
                            for(string production_ : FIRST_table[production[index + 1]])
                                FOLLOW_productions.push_back(production_);
                        else
                            FOLLOW_productions.push_back(production[index + 1]);
                    }
                    else
                    {
                        for(int i = 0; i < baseNode->attachedNodes.size(); i++)
                        {
                            vector<vector<string>> productions = baseNode->attachedNodes[i]->values;
                            
                            if(std::find(productions.begin(), 
                                    productions.end(), production) 
                                    != productions.end())
                            {
                                if(baseNode->attachedNodes[i]->key 
                                            != nonTerminal)
                                {
                                    if(FOLLOW_table.find(baseNode->
                                        attachedNodes[i]->key) != FOLLOW_table.end())
                                    {
                                        for(const auto& key : FOLLOW_table)
                                            if(key.first == baseNode->attachedNodes[i]->key)
                                            {
                                                FOLLOW_productions = key.second; break;
                                                for(string token : key.second)
                                                    if(std::find(FOLLOW_productions.begin(),
                                                            FOLLOW_productions.end(), token) == FOLLOW_productions.end())
                                                        FOLLOW_productions.push_back(token);
                                            }
                                    }
                                    else
                                        for(string token : getFollowProductions(baseNode->attachedNodes[i]->key))
                                            if(std::find(FOLLOW_productions.begin(),
                                                    FOLLOW_productions.end(), token) == FOLLOW_productions.end())
                                                FOLLOW_productions.push_back(token);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return FOLLOW_productions;
}

void FOLLOW()
{
    vector<string> FOLLOW_productions = {};

    for(int i = 0; i < baseNode->attachedNodes.size(); i++)
    {
        string nonTerminal = baseNode->attachedNodes[i]->key;
        
        if(findNonTerminalInProduction(nonTerminal).size() != 0)
        {
            for(vector<string> production : findNonTerminalInProduction(nonTerminal))
            {
                for(int index = 0; index < production.size(); index++)
                {
                    string token = production[index];
                    
                    if(token == nonTerminal)
                    {
                        if(index != (production.size() - 1))
                        {
                            if(isAllLower(production[index + 1]))
                                for(string production_ : FIRST_table[production[index + 1]])
                                    FOLLOW_productions.push_back(production_); 
                            else
                                FOLLOW_productions.push_back(production[index + 1]);
                            FOLLOW_table[nonTerminal] = FOLLOW_productions;
                        }
                        else
                        {
                            for(int i = 0; i < baseNode->attachedNodes.size(); i++)
                            {
                                vector<vector<string>> productions = baseNode->attachedNodes[i]->values;
                                
                                if(std::find(productions.begin(), productions.end(), production) 
                                        != productions.end())
                                {
                                    if(baseNode->attachedNodes[i]->key 
                                                != nonTerminal)
                                    {
                                        for(string token : getFollowProductions(baseNode->attachedNodes[i]->key))
                                            if(std::find(FOLLOW_productions.begin(),
                                                    FOLLOW_productions.end(), token) == FOLLOW_productions.end())
                                                FOLLOW_productions.push_back(token);
                                        FOLLOW_table[nonTerminal] = FOLLOW_productions;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        FOLLOW_productions.clear();
    }
}

void ParsingTable()
{
    terminal_id_mapping["LPAREN"] = LPAREN;
    terminal_id_mapping["RPAREN"] = RPAREN;
    terminal_id_mapping["IDENTIFIER"] = IDENTIFIER;
    terminal_id_mapping["STRING"] = STRING;
    terminal_id_mapping["INTEGER"] = INTEGER;
    terminal_id_mapping["FLOAT"] = FLOAT;
    terminal_id_mapping["DOUBLE"] = DOUBLE;
    terminal_id_mapping["EPSILON"] = EPSILON;

    non_terminal_id_mapping["function_call"] = FUNCTION_CALL;
    non_terminal_id_mapping["object"] = OBJECT;
    non_terminal_id_mapping["typeobject"] = TYPE_OBJECT;
    non_terminal_id_mapping["param"] = PARAM;
    non_terminal_id_mapping["params"] = PARAMS;
    
    for(int i = 0; i < baseNode->attachedNodes.size(); i++)
    {
        for(vector<string> production : baseNode->attachedNodes[i]->values)
        {
            if(isAllLower(production[0]))
                for(int index = 0; index < FIRST_table[production[0]].size(); index++)
                    predictive_parsing_table
                        [ non_terminal_id_mapping[ baseNode->attachedNodes[i]->key ] ]
                        [ terminal_id_mapping[ FIRST_table[production[0]][index] ] ] = production;
            else
                predictive_parsing_table
                        [ non_terminal_id_mapping[ baseNode->attachedNodes[i]->key ] ]
                        [ terminal_id_mapping[ production[0] ] ] = production;
        }
    }
}

void filter() {}

void parseTree()
{
    vector<vector<string>> tokens = 
    {
        vector<string>
            {"disp", "IDENTIFIER"},
        vector<string>
            {"LPAREN"},
        vector<string>
            {"1", "STRING"},
        vector<string>
            {"RPAREN"},

        vector<string>
            {"disp", "IDENTIFIER"},
        vector<string>
            {"LPAREN"},
        vector<string>
            {"1", "INTEGER"},
        vector<string>
            {"RPAREN"}
    };
    
    OutputMappingStack* omsBottom 
        = new OutputMappingStack();
    OutputMappingStack* omsPtr = omsBottom;
    omsPtr->bottom = NULL;

    InputStack* isBottom 
        = new InputStack();
    InputStack* isPtr = isBottom;
    isPtr->bottom = NULL;

    StorageStack* ssBottom 
        = new StorageStack();
    StorageStack* ssPtr = ssBottom;
    ssPtr->bottom = NULL;

    int index = 0;
    while(index < tokens.size())
    {
        vector<string>
            token = tokens[index];
        
        isPtr->token = token;

        index++;

        if(index
            != token.size())
        {
            InputStack* top = new InputStack();
            isPtr->top = top;
            isPtr = top;
        }
    }

    isPtr = isBottom;

    while(isPtr != NULL)
    {
        bool grammarMatched = false;
        string token;
        
        if(isPtr->token.size() == 2)
            token = isPtr->token[1];
        else
            token = isPtr->token[0];

        int index = 0;

        while(index <
            sizeof(start_points)
            /
            sizeof(*start_points))
        {
            string key = start_points[index];
            
            vector<string> first;
            vector<string> follow;

            int i = 0;

            while(i < baseNode->
                attachedNodes.size())
            {
                if(baseNode->attachedNodes
                    [i]->key == key)
                {
                    vector<vector<string>>
                        productions = baseNode->
                            attachedNodes[i]->values;
                    
                    for(vector<string>
                        production: productions)
                    {
                        string first_value = production[0];
                        
                        if(isAllLower(first_value))
                        {
                            if(std::find(FIRST_table[first_value].begin(),
                                FIRST_table[first_value].end(), token) 
                                != FIRST_table[first_value].end())
                            {
                                if(isPtr->
                                    top != NULL)
                                {
                                    string nextToken;
                                
                                    if(isPtr->top != NULL
                                        && isPtr->top->
                                            token.size() == 2)
                                        nextToken = isPtr->top->token[1];
                                    
                                    else if(isPtr->top 
                                            != NULL)
                                        nextToken = isPtr->top->token[0];
                                    
                                    if(std::find(FOLLOW_table[first_value].begin(),
                                        FOLLOW_table[first_value].end(), nextToken)
                                        != FOLLOW_table[first_value].end())
                                    {
                                        int productionIndex = 0;

                                        while(productionIndex <
                                                production.size())
                                        {
                                            string value = production[productionIndex];
                                            ssPtr->value = value;
                                            productionIndex++;
                                            
                                            if(productionIndex != production.size())
                                            {
                                                StorageStack* top = new StorageStack();
                                                ssPtr->top = top;
                                                ssPtr = top;   
                                            }
                                        }
                                        ssPtr = ssBottom;
                                        grammarMatched = true;
                                        break;
                                    }
                                }
                            }
                        }
                        else
                        {
                            if(first_value == token)
                            {
                                if(isPtr->top
                                    != NULL)
                                {
                                    string nextToken;
                                
                                    if(isPtr->top->
                                        token.size() == 2)
                                        nextToken =
                                            isPtr->top->
                                                token[1];
                                    else
                                        nextToken =
                                            isPtr->top->
                                                token[0];
                                    
                                    string secondValue 
                                        = production[1];
                                    
                                    if(isAllLower(secondValue))
                                    {
                                        if(std::find(FIRST_table[secondValue].begin(),
                                            FIRST_table[secondValue].end(), nextToken)
                                            != FIRST_table[secondValue].end())
                                        {

                                            if(isPtr->top->top
                                                != NULL)
                                            {
                                                if(isPtr->top
                                                    ->top != NULL
                                                    && isPtr->top->
                                                        top->token.size() == 2)
                                                    nextToken = 
                                                        isPtr->top->
                                                        top->token[1];
                                                else if(isPtr->top->
                                                    top != NULL)
                                                    nextToken = 
                                                        isPtr->top->
                                                        top->token[0];
                                                
                                                if(std::find(FOLLOW_table[secondValue].begin(),
                                                    FOLLOW_table[secondValue].end(), nextToken)
                                                    != FOLLOW_table[secondValue].end())
                                                {
                                                    int productionIndex = 0;

                                                    while(productionIndex <
                                                            production.size())
                                                    {
                                                        string value = production[productionIndex];
                                                        ssPtr->value = value;
                                                        productionIndex++;
                                                        
                                                        if(productionIndex
                                                            != production.size())
                                                        {
                                                            StorageStack* top = new StorageStack();
                                                            ssPtr->top = top;
                                                            ssPtr = top;   
                                                        }
                                                    }
                                                    ssPtr = ssBottom;
                                                    grammarMatched = true;
                                                    break;
                                                } 
                                            }
                                        }
                                    }
                                    else
                                    {
                                        if(secondValue
                                            == nextToken)
                                        {
                                            int productionIndex = 0;

                                            while(productionIndex <
                                                    production.size())
                                            {
                                                string value = production[productionIndex];
                                                ssPtr->value = value;
                                                productionIndex++;
                                                
                                                if(productionIndex
                                                    != production.size())
                                                {
                                                    StorageStack* top = new StorageStack();
                                                    ssPtr->top = top;
                                                    ssPtr = top;   
                                                }
                                            }
                                            ssPtr = ssBottom;
                                            grammarMatched = true;
                                            break;
                                        }
                                    } 
                                }
                            }
                        }
                    }
                }

                i++;
            }

            index++;
        }

        if(!grammarMatched)
        {
            if(isPtr->top == NULL)
                isPtr = NULL;
            else
                isPtr = isPtr->top;
        }
        else
            break;
    }

    isPtr = isBottom;

    while(ssPtr != NULL)
    {
        string tokenType;
        bool isPoppedOut = false, isSymbol = false;
        
        if(isPtr->token.size() == 2)
            tokenType = isPtr->token[1];
        else
        {
            tokenType = isPtr->token[0];
            isSymbol = true;
        }

        if(isAllLower(ssPtr->value))
        {
            if(!isSymbol && std::find(
                FIRST_table[ssPtr->value].begin(),
                FIRST_table[ssPtr->value].end(), tokenType)
                == FIRST_table[ssPtr->value].end())
            {
                std::cout << "\nSyntax error 1." << endl;
                return;
            }
            
            vector<string> production = 
                predictive_parsing_table
                    [ non_terminal_id_mapping[ ssPtr->value ] ]
                    [ terminal_id_mapping[ tokenType ] ];
            
            if(production.size() != 0)
            {
                StorageStack* prevTop = ssPtr->top;
                StorageStack* ptr = ssPtr;

                string prevValue = ssPtr->value;

                int index = 0;
                while(index < production.size())
                {
                    ptr->value = production[index];
                    
                    omsPtr->expansion[ prevValue ].push_back(production[index]);

                    index++;

                    if(index 
                        != production.size())
                    {
                        StorageStack* top = new StorageStack();
                        ptr->top = top;
                        ptr = top;
                    }
                }
                ptr->top = prevTop;
            }
            else
            {
                omsPtr->expansion[ ssPtr->value ].push_back("EPSILON");
                isPoppedOut = true;
            }

            OutputMappingStack* top = new OutputMappingStack();
            omsPtr->top = top;
            omsPtr->isFilled = true;
            omsPtr = top;
        }
        else
        {
            if(ssPtr->value
                != tokenType)
            {
                transform(tokenType.begin(), tokenType.end(),
                    tokenType.begin(), ::tolower);
                transform(ssPtr->value.begin(), ssPtr->value.end(),
                    ssPtr->value.begin(), ::tolower);
                
                std::cout << "Syntax error: Unexpected " << tokenType 
                    << " provided (expecting " << ssPtr->value << ")." << endl;
                return;
            }
            InputStack* ptr = isPtr;
            isPtr = isPtr->top;
            delete ptr;
            isPoppedOut = true;
        }
        
        if(isPoppedOut)
        {
            StorageStack* ptr = ssPtr;
            ssPtr = ssPtr->top;
            delete ptr;
        }
    }

    omsPtr->top = NULL;

    OutputMappingStack* ptr = omsBottom;

    while(ptr != NULL)
    {
        for(const auto&
            [key, value]
            : ptr->expansion)
        {   
            std::cout << key << " -> ";
            for(string token : value)
                std::cout << token << " ";
            std::cout << endl;    
        }
        
        ptr = ptr->top;
    }
}