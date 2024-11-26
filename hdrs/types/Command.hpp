#pragma once

class Command;

#include <string>
#include <unistd.h>
#include <map>
#include <algorithm>

typedef std::map<std::string, void(Command::*)(void)>	w_map_Command;

# define ERR_PASS(s) ":" + s + " " + "464" + " * " + ":Password incorrect" + "\r\n"
