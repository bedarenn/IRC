#pragma once

class Command;

#include <string>
#include <map>
#include <algorithm>

typedef std::map<std::string, void(Command::*)(void)>	w_map_Command;
