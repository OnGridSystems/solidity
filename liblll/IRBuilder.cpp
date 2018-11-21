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
    std::vector<int> operands;
    string operation;
    int c = 0;

    for (auto const& i: _t) {

        if(c++){
            bigint bigintVal = *i.get<bigint*>();
            int intVal = bigintVal.template convert_to<int>();
            operands.push_back(intVal);
        }
        else {
            auto sr = i.get<sp::basic_string<boost::iterator_range<char const*>, sp::utree_type::symbol_type>>();
            operation = string(sr.begin(), sr.end());
        }

    }



    Value *Two = builder->getInt32(operands[0]);
    Value *Three = builder->getInt32(operands[1]);

    return builder->CreateAdd(Two, Three, "return");
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