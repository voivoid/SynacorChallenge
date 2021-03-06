#pragma once

#include "synacor/instructions_base.h"

namespace synacor
{

namespace instructions
{
#define SYNACOR_DEFINE_INSTRUCTION( name, base, i )                                                                                        \
  struct name : base                                                                                                                       \
  {                                                                                                                                        \
    using base::base;                                                                                                                      \
    static constexpr IndexType index = { i };                                                                                              \
    Address execute( Machine& machine, Address current_address ) override;                                                                 \
  }

SYNACOR_DEFINE_INSTRUCTION( Halt, NoOpInstruction, 0 );
SYNACOR_DEFINE_INSTRUCTION( Set, TwoOpsInstruction, 1 );
SYNACOR_DEFINE_INSTRUCTION( Push, OneOpInstruction, 2 );
SYNACOR_DEFINE_INSTRUCTION( Pop, OneOpInstruction, 3 );
SYNACOR_DEFINE_INSTRUCTION( Eq, ThreeOpsInstruction, 4 );
SYNACOR_DEFINE_INSTRUCTION( Gt, ThreeOpsInstruction, 5 );
SYNACOR_DEFINE_INSTRUCTION( Jmp, OneOpInstruction, 6 );
SYNACOR_DEFINE_INSTRUCTION( Jt, TwoOpsInstruction, 7 );
SYNACOR_DEFINE_INSTRUCTION( Jf, TwoOpsInstruction, 8 );
SYNACOR_DEFINE_INSTRUCTION( Add, ThreeOpsInstruction, 9 );
SYNACOR_DEFINE_INSTRUCTION( Mult, ThreeOpsInstruction, 10 );
SYNACOR_DEFINE_INSTRUCTION( Mod, ThreeOpsInstruction, 11 );
SYNACOR_DEFINE_INSTRUCTION( And, ThreeOpsInstruction, 12 );
SYNACOR_DEFINE_INSTRUCTION( Or, ThreeOpsInstruction, 13 );
SYNACOR_DEFINE_INSTRUCTION( Not, TwoOpsInstruction, 14 );
SYNACOR_DEFINE_INSTRUCTION( RMem, TwoOpsInstruction, 15 );
SYNACOR_DEFINE_INSTRUCTION( WMem, TwoOpsInstruction, 16 );
SYNACOR_DEFINE_INSTRUCTION( Call, OneOpInstruction, 17 );
SYNACOR_DEFINE_INSTRUCTION( Ret, NoOpInstruction, 18 );
SYNACOR_DEFINE_INSTRUCTION( Out, OneOpInstruction, 19 );
SYNACOR_DEFINE_INSTRUCTION( In, OneOpInstruction, 20 );
SYNACOR_DEFINE_INSTRUCTION( Noop, NoOpInstruction, 21 );

}  // namespace instructions

}  // namespace synacor
