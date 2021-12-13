# include "State.h"

State::State()
{
   std::cout << " State constructor" << std::endl;
}

StateType State::UpdateState()
{
   std::cout << " Base class UpdateState function" << std::endl;
   return stateType;
}

void State::RenderState()
{
   std::cout << " Base class RenderState function" << std::endl;
}