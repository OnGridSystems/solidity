#pragma once

#include <llvm/IR/Module.h>

#include <libdevcore/Common.h>

#include <string>

namespace boost { namespace spirit { class utree; } }
namespace sp = boost::spirit;

namespace dev
{
    namespace lll
    {

    llvm::Module* buildModule(sp::utree const& tree);

    }
}
