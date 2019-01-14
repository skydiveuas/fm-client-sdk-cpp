#include "event/input/IInputEvent.hpp"

using namespace fm;
using namespace fm::event::input;

IInputEvent::IInputEvent(InputEventType type) :
    inputEventType(type)
{
}

IInputEvent::~IInputEvent()
{
}

IInputEvent::InputEventType IInputEvent::getInputEventType() const
{
    return inputEventType;
}
