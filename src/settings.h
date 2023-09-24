#pragma once

#include <string>
#include <functional>

// Will run the callback function for each line in the file.
bool configure(std::string, std::function<void(std::string)>);