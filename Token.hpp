// Author: Venkata Subbu Dheeraj Shyam Polavarapu.

#pragma once

#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

#include <string.h>

using namespace std;

struct Token
{
    int count;
    int lineno;
    int lexpos;

    bool marked;

    string TokenType;
    string value;

    struct Token* link;
};



#endif