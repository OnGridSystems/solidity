#include <boost/spirit/include/support_utree.hpp>

#include <liblll/llvmCompiler.h>
#include <liblll/Parser.h>
#include <liblll/CodeFragment.h>
#include <liblll/IRBuilder.h>

using namespace std;
using namespace dev;
using namespace dev::lll;


std::string dev::lll::compileLLLToIR(std::string const& _src, std::vector<std::string>* _errors)
{
    try
    {
        sp::utree tree;
        parseTreeLLL(_src, tree);

        if (!tree.empty() and tree.which() == sp::utree_type::list_type) {
            llvm::Module* Mod = buildModule(tree);
            Mod->print(llvm::outs(), nullptr);
        }
        else
            _errors->push_back("Code is empty or top level expression is not list");
        string ret;
        return ret;
    }
    catch (Exception const& _e)
    {
        if (_errors)
        {
            _errors->push_back("Parse error.");
            _errors->push_back(boost::diagnostic_information(_e));
        }
    }
    catch (std::exception const& _e)
    {
        if (_errors)
        {
            _errors->push_back("Parse exception.");
            _errors->push_back(boost::diagnostic_information(_e));
        }
    }
    catch (...)
    {
        if (_errors)
            _errors->push_back("Internal compiler exception.");
    }
    return string();
}
