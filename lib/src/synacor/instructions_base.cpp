#include "synacor/instructions_base.h"

#include "synacor/assert.h"
#include "synacor/instructions.h"

#include "boost/mp11.hpp"

#include <map>
#include <type_traits>

namespace
{
using InstructionFactory = std::unique_ptr<synacor::Instruction> ( * )( const synacor::IMemoryReader&, synacor::Address );

template <typename I, typename Base>
using InstructionPtrIf = std::enable_if_t<std::is_base_of<Base, I>::value, std::unique_ptr<synacor::Instruction>>;

template <typename I>
InstructionPtrIf<I, synacor::NoOpInstruction> make_instruction( const synacor::IMemoryReader&, const synacor::Address )
{
  return std::make_unique<I>();
}

template <typename I>
InstructionPtrIf<I, synacor::OneOpInstruction> make_instruction( const synacor::IMemoryReader& memory_reader,
                                                                 const synacor::Address instruction_data_address )
{
  const auto a_op = memory_reader.read( instruction_data_address );
  return std::make_unique<I>( a_op );
}

template <typename I>
InstructionPtrIf<I, synacor::TwoOpsInstruction> make_instruction( const synacor::IMemoryReader& memory_reader,
                                                                  synacor::Address instruction_data_address )
{
  const auto a_op = memory_reader.read( instruction_data_address++ );
  const auto b_op = memory_reader.read( instruction_data_address++ );
  return std::make_unique<I>( a_op, b_op );
}

template <typename I>
InstructionPtrIf<I, synacor::ThreeOpsInstruction> make_instruction( const synacor::IMemoryReader& memory_reader,
                                                                    synacor::Address instruction_data_address )
{
  const auto a_op = memory_reader.read( instruction_data_address++ );
  const auto b_op = memory_reader.read( instruction_data_address++ );
  const auto c_op = memory_reader.read( instruction_data_address++ );
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
                                              synacor::instructions::In,
                                              synacor::instructions::Noop>;

template <typename T>
using AddBoostType = boost::type<T>;

template <typename T>
struct get_boost_type;

template <typename T>
struct get_boost_type<boost::type<T>>
{
  using type = T;
};

std::map<synacor::Word, InstructionFactory> make_instructions_map()
{
  std::map<synacor::Word, InstructionFactory> instructions_map;

  using TypedInstructions = boost::mp11::mp_transform<AddBoostType, InstructionsList>;
  boost::mp11::mp_for_each<TypedInstructions>( [&instructions_map]( auto v ) {
    using Instruction                      = typename get_boost_type<decltype( v )>::type;
    instructions_map[ Instruction::index ] = &make_instruction<Instruction>;
  } );

  return instructions_map;
}
}  // namespace

namespace synacor
{

std::unique_ptr<Instruction> read_instruction( const IMemoryReader& memory_reader, const Address instruction_address )
{
  static const std::map<Word, InstructionFactory> instructions_factories = make_instructions_map();

  const auto cmd_id       = memory_reader.read( instruction_address );
  const auto factory_iter = instructions_factories.find( cmd_id );
  SYNACOR_ENSURE( factory_iter != instructions_factories.cend() );

  const auto create_instruction = factory_iter->second;
  return create_instruction( memory_reader, instruction_address + Address( 1 ) );
}

}  // namespace synacor
