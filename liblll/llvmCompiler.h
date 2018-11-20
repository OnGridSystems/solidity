#pragma once

#include <libdevcore/Common.h>


#include <string>
#include <vector>

namespace dev
{
    namespace lll
    {


        std::string compileLLLToIR(std::string const& _src, std::vector<std::string>* _errors = nullptr);

    }
}
