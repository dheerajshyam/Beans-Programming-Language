// Author: Venkata Subbu Dheeraj Shyam Polavarapu.

#pragma once

#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

struct Token
{
    int count;
    int lineno;

    bool marked;

    string TokenType;
    string value;

    struct Token* link;
};

#endif // TOKEN_HPP
