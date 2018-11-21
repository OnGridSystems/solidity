#include <liblll/IRBuilder.h>

#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/IRBuilder.h"

#include <boost/algorithm/string.hpp>
#include <boost/spirit/include/support_utree.hpp>

using namespace std;
using namespace dev;
using namespace dev::lll;
using namespace llvm;


Value *parseTree(sp::utree const& _t,IRBuilder<> *builder){
    std::vector<Value*> arguments;
    string operation;
    int c = 0;

    for (auto const& i: _t) {
        if(c++){
            switch (i.which()) {
                case sp::utree_type::any_type: {
                    bigint bigintVal = *i.get<bigint*>();
                    int intVal = bigintVal.template convert_to<int>();
                    arguments.push_back(builder->getInt32(intVal));
                    break;
                }
                case sp::utree_type::list_type: {
                    arguments.push_back(parseTree(i, builder));
                }

            }
        }
        else {
            switch (i.which()){
                case sp::utree_type::symbol_type: {
                    auto sr = i.get<sp::basic_string<boost::iterator_range<char const*>, sp::utree_type::symbol_type>>();
                    operation = string(sr.begin(), sr.end());
                    break;
                }
                case sp::utree_type::string_type: {
                    auto sr = i.get<sp::basic_string<boost::iterator_range<char const*>, sp::utree_type::string_type>>();
                    operation = string(sr.begin(), sr.end());
                    break;
                }
            }
        }
    }

    if (operation == "+") {
        return builder->CreateAdd(arguments[0], arguments[1], "return");
    }
    else if (operation == "-") {
        return builder->CreateSub(arguments[0], arguments[1], "return");
    }


    return builder->CreateAdd(arguments[0], arguments[1], "return");
}


Module* dev::lll::buildModule(sp::utree const& _t)
{

    static LLVMContext TheContext;
    Module* mod = new Module("test", TheContext);

    FunctionType *MainFT = FunctionType::get(Type::getInt32Ty(TheContext), false);

    Function *Main = Function::Create(MainFT, Function::ExternalLinkage, "main", mod);

    BasicBlock *BB = BasicBlock::Create(TheContext, "EntryBlock", Main);

    IRBuilder<> builder(BB);

    builder.CreateRet(parseTree(_t, &builder));

    return mod;
}