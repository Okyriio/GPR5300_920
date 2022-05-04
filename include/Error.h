#pragma once

#include <string_view>

namespace gpr5300
{
void CheckError(std::string_view file, int line);
#define glCheckError() CheckError(__FILE__, __LINE__)
}
