#include "beans_parser.hpp"

string start_points[] 
    =  {
        "function_call",
        "tuple"
       };

vector<string> 
    predictive_parsing_table[7][9]; 
    // predictive_parsing_table[non-terminals][terminals].

map<string, vector<string> > FIRST_table;
map<string, vector<string> > FOLLOW_table;

map<string,TERMINAL_ID> terminal_id_mapping;
map<string,NON_TERMINAL_ID> non_terminal_id_mapping;

GrammarTable* baseNode;

bool isAllLower(string value)
{
    bool isAllLower = true;
    for(char ch : value)
        if(isalpha(ch) && !islower(ch)) { isAllLower = false; break; }
    return isAllLower;
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
    terminal_id_mapping["COMMA"] = COMMA;
    terminal_id_mapping["IDENTIFIER"] = IDENTIFIER;
    terminal_id_mapping["STRING"] = STRING;
    terminal_id_mapping["INTEGER"] = INTEGER;
    terminal_id_mapping["FLOAT"] = FLOAT;
    terminal_id_mapping["DOUBLE"] = DOUBLE;
    terminal_id_mapping["EPSILON"] = EPSILON;

    non_terminal_id_mapping["function_call"] = FUNCTION_CALL;
    non_terminal_id_mapping["object"] = OBJECT;
    non_terminal_id_mapping["typeobject"] = TYPE_OBJECT;
    non_terminal_id_mapping["typeobjects"] = TYPE_OBJECTS;
    non_terminal_id_mapping["param"] = PARAM;
    non_terminal_id_mapping["params"] = PARAMS;
    non_terminal_id_mapping["tuple"] = TUPLE;
    
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

vector<vector<string>> getProductionWithKey(string key)
{
    vector<vector<string>> productions;
    for(int i = 0; i < baseNode->attachedNodes.size(); i++)
    {
        if(key == 
            baseNode->attachedNodes[i]
                ->key)
        {
            productions = baseNode->attachedNodes[i]->values;
            break;
        }
    }
    return productions;
}

OutputMappingStack* parseTree()
{
    string filename = "test.beans";

    Token* tokenHead = lex(filename);
    Token* tokenPtr = tokenHead;
    
    if(tokenHead == NULL)
    {
        delete tokenPtr;
        std::cout << "Unable to locate the provided filename \""
                << filename << "\" in the directory" << endl;
        std::exit(0);
    }
    
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

    while(tokenPtr != NULL)
    {
        vector<string> token;
        
        if(tokenPtr->count == 2)
        {
            token.push_back(tokenPtr->value);
            token.push_back(tokenPtr->TokenType);
        }
        else if(tokenPtr->count == 1)
            token.push_back(tokenPtr->TokenType);
        
        isPtr->token = token;
        isPtr->lineno = tokenPtr->lineno;
        isPtr->lexpos = tokenPtr->lexpos;

        tokenPtr
            = tokenPtr->link;
        
        if(tokenPtr != NULL)
        {
            InputStack* top = new InputStack();
            isPtr->top = top;
            isPtr->
                top->
                    bottom = isPtr;
            isPtr = top;
        }
    }

    isPtr = isBottom;

    while(isPtr != NULL)
    {
        int index = 0;
        bool grammarMatched = false;

        while(index <
            sizeof(start_points)
            /
            sizeof(*start_points))
        {
            if(isPtr == NULL)
            {
                break;
            }
            
            string _key
                = start_points[index];

            int i = 0;

            while(i < baseNode->
                    attachedNodes.size())
            {
                if(baseNode->attachedNodes
                    [i]->key == _key)
                {
                    vector<vector<string>>
                        productions = baseNode->
                            attachedNodes[i]->values;
                    
                    for(vector<string>
                        production
                        :
                        productions)
                    {
                        int productionIndex= 0;
                        bool hasRelation = false;

                        while(productionIndex
                                < production.size())
                        {
                            bool skipProductionInc = false;

                            string token;

                            if(isPtr->token.size() == 2)
                                token = isPtr->token[1];
                            else
                                token = isPtr->token[0];
                            
                            string productionValue
                                = production[productionIndex];     

                            if(isAllLower
                                (productionValue))
                            {
                                if(std::find(
                                    FIRST_table[productionValue].begin(),
                                    FIRST_table[productionValue].end(), token
                                    ) == FIRST_table[productionValue].end())
                                {
                                    if(productionValue
                                        != *production.begin())
                                    {
                                        if(std::find(
                                            FOLLOW_table[productionValue].begin(),
                                            FOLLOW_table[productionValue].end(), token
                                            ) == FOLLOW_table[productionValue].end())
                                        {
                                            // Checking further
                                        
                                            if(isPtr->bottom != NULL)
                                            {
                                                string prevToken;
                                                
                                                if(isPtr->
                                                    bottom->token.size() == 2)
                                                    prevToken = isPtr->bottom->token[1];
                                                else
                                                    prevToken = isPtr->bottom->token[0];
                                                
                                                vector<vector<string>>
                                                    productions =
                                                        getProductionWithKey(productionValue);
                                                
                                                bool errorExists = false;
                                                
                                                for(vector<string> 
                                                        production: productions)
                                                {
                                                    int prevI = i;
                                                    i = 0;
                                                    while(i < production.size())
                                                    {
                                                        string productionVal
                                                            = production[i];
                                                        if(!isAllLower(productionVal))
                                                        {
                                                            i++;

                                                            if((i != 
                                                                production.size())
                                                                && (productionVal == prevToken))
                                                            {
                                                                i++;
                                                                string nextProductionVal = production[i];
                                                                if(isAllLower(nextProductionVal))
                                                                {
                                                                    if(std::find(FIRST_table[nextProductionVal].begin(),
                                                                        FIRST_table[nextProductionVal].end(), token)
                                                                        != FIRST_table[nextProductionVal].end())
                                                                    {
                                                                        goto end;
                                                                    }
                                                                }
                                                                else
                                                                {
                                                                    if(token 
                                                                        == nextProductionVal)
                                                                        goto end;
                                                                }
                                                            }
                                                        }
                                                        else
                                                        {
                                                            bool notInFirst 
                                                                = std::find(FIRST_table[productionVal].begin(),
                                                                    FIRST_table[productionVal].end(), prevToken)
                                                                    == FIRST_table[productionVal].end();
                                                            
                                                            bool notInFollow
                                                                = std::find(FOLLOW_table[productionVal].begin(),
                                                                    FOLLOW_table[productionVal].end(), token)
                                                                    == FOLLOW_table[productionVal].end();
                                                            
                                                            if(!notInFirst
                                                                || !notInFollow)
                                                                goto end;
                                                        }
                                                        i++;
                                                    }
                                                    i = prevI;
                                                }

                                                std::cout << "Not matched 1: " 
                                                    << token << "." << endl;
                                                std::exit(0);
                                            }
                                            else
                                            {
                                                
                                                std::cout << "Not matched 1: " 
                                                    << token << "." << endl;
                                                std::exit(0);
                                            }

                                            // End.

                                            end:
                                            {
                                                isPtr = isPtr->top;
                                                goto _production_inc_skip;
                                            };
                                        }
                                    }
                                }

                                if(predictive_parsing_table
                                    [ non_terminal_id_mapping[productionValue] ]
                                    [ terminal_id_mapping[token] ].size() != 0)
                                {
                                    isPtr = isPtr->top;
                                    goto _production_inc_skip;
                                }
                                else
                                {
                                    goto _production_inc;
                                }
                            }
                            else
                            {
                                if(productionValue == token)
                                {
                                    isPtr = isPtr->top;
                                    goto _production_inc;
                                    
                                }
                                else
                                {
                                    if(productionValue
                                        != *production.begin())
                                    {
                                        std::cout << "Not matched 2: " 
                                            << productionValue
                                            << ", "
                                            << token << "." << endl;
                                        std::exit(0);
                                    }
                                    else
                                        goto _index_inc;
                                }
                            }

                            _production_inc:
                            {
                                ssPtr->value = productionValue;

                                StorageStack* top = new StorageStack();
                                StorageStack* prevPtr = ssPtr;

                                ssPtr->top = top;
                                ssPtr = top;
                                ssPtr->bottom = prevPtr;
                                
                                productionIndex++;
                            };

                            _production_inc_skip: {};
                        }
                    }
                    grammarMatched = true;
                    break;
                }
                i++;
            }

            if(!grammarMatched)
                _index_inc: index++;
            else
                index = 0;
        }
    }

    isPtr = isBottom;
    ssPtr = ssBottom;

    while(ssPtr->top != NULL)
    {
        string tokenType;
        bool isPoppedOut = false, isSymbol = false;
        
        if(isPtr->token.size() == 2)
        {
            tokenType = isPtr->token[1];
        }
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
                transform(tokenType.begin(), 
                    tokenType.end(), tokenType.begin(), ::tolower);
                std::cout << "Syntax error: unexpected " << tokenType << " found in line " 
                        << isPtr->lineno << "." << endl;
                std::exit(0);
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
                    << " provided (expecting " << ssPtr->value << ") in line " 
                    << isPtr->lineno << "." << endl;
                std::exit(0);
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

    return omsBottom;
}