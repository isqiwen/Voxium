#pragma once
#include "Precompiled.h"

class Class;
class Global;
class Function;
class Enum;

struct SchemaModule
{
    std::string name;

    std::vector<std::shared_ptr<Class>> classes;
};
