#include "synacor/instructions_base.h"

#include "synacor/assert.h"
#include "synacor/instructions.h"
#include "synacor/memory_storage.h"

#include "boost/mp11.hpp"

#include <type_traits>
#include <map>

namespace
{
using InstructionFactory = std::unique_ptr<synacor::Instruction> ( * )( const synacor::MemoryStorage&, synacor::Address );

template <typename I, typename Base>
using InstructionPtrIf = std::enable_if_t<std::is_base_of<Base, I>::value, std::unique_ptr<synacor::Instruction>>;

template <typename I>
InstructionPtrIf<I, synacor::NoOpInstruction> make_instruction( const synacor::MemoryStorage&, const synacor::Address )
{
  return std::make_unique<I>();
}

template <typename I>
InstructionPtrIf<I, synacor::OneOpInstruction> make_instruction( const synacor::MemoryStorage& memory,
                                                                 const synacor::Address instruction_data_address )
{
  const auto a_op = memory.read( instruction_data_address );
  return std::make_unique<I>( a_op );
}

template <typename I>
InstructionPtrIf<I, synacor::TwoOpsInstruction> make_instruction( const synacor::MemoryStorage& memory,
                                                                  synacor::Address instruction_data_address )
{
  const auto a_op = memory.read( instruction_data_address++ );
  const auto b_op = memory.read( instruction_data_address++ );
  return std::make_unique<I>( a_op, b_op );
}

template <typename I>
InstructionPtrIf<I, synacor::ThreeOpsInstruction> make_instruction( const synacor::MemoryStorage& memory,
                                                                    synacor::Address instruction_data_address )
{
  const auto a_op = memory.read( instruction_data_address++ );
  const auto b_op = memory.read( instruction_data_address++ );
  const auto c_op = memory.read( instruction_data_address++ );
  return std::make_unique<I>( a_op, b_op, c_op );
}

using InstructionsList = boost::mp11::mp_list<synacor::instructions::Halt,
                                              synacor::instructions::Set,
                                              synacor::instructions::Push,
                                              synacor::instructions::Pop,
                                              synacor::instructions::Eq,
                                              synacor::instructions::Gt,
                                              synacor::instructions::Jmp,
                                              synacor::instructions::Jt,
                                              synacor::instructions::Jf,
                                              synacor::instructions::Add,
                                              synacor::instructions::Mult,
                                              synacor::instructions::Mod,
                                              synacor::instructions::And,
                                              synacor::instructions::Or,
                                              synacor::instructions::Not,
                                              synacor::instructions::RMem,
                                              synacor::instructions::WMem,
                                              synacor::instructions::Call,
                                              synacor::instructions::Ret,
                                              synacor::instructions::Out,
                                              synacor::instructions::Noop>;

std::map<synacor::Word, InstructionFactory> make_instructions_map()
{
  std::map<synacor::Word, InstructionFactory> instructions_map;
  // see https://stackoverflow.com/questions/24954220/boostmplfor-each-without-instantiating
  //  boost::mp11::mp_for_each<InstructionsList>( [&instructions_map]( const auto& instruction ) {
  //    using Instruction                      = decltype( instruction );
  //    instructions_map[ Instruction::index ] = &make_instruction<Instruction>;
  //  } );

  return instructions_map;
}
}  // namespace

namespace synacor
{

std::unique_ptr<Instruction> read_instruction( const MemoryStorage& memory, const Address instruction_address )
{
  const auto cmd_id                                               = memory.read( instruction_address );
  const std::map<Word, InstructionFactory> instructions_factories = make_instructions_map();

  const auto factory_iter = instructions_factories.find( cmd_id );
  SYNACOR_ENSURE( factory_iter != instructions_factories.cend() );

  const auto create_instruction = factory_iter->second;
  return create_instruction( memory, instruction_address + Address( 1 ) );
}

}  // namespace synacor
