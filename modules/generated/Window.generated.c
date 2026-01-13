function const char*
event_kind_to_string(Event_Kind e)
{
  switch(e)
  {
    case Event_Error: return "Event_Error";
    case Event_Keyboard: return "Event_Keyboard";
    case Event_Text_Input: return "Event_Text_Input";
    case Event_Mouse_Wheel: return "Event_Mouse_Wheel";
    case Event_Window: return "Event_Window";
    case Event_Quit: return "Event_Quit";
    case Event_Drag_And_Drop: return "Event_Drag_And_Drop";
  }
  return "";
}
