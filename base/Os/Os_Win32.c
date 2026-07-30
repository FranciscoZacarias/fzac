fz_internal void
message_box(String title, String content, String file, u32 line)
{
  Scratch scratch = scratch_begin(0,0);
  String body = Sf(scratch.arena, S_FMT" line:%u\n"S_FMT, S_ARG(file), line, S_ARG(content));
  MessageBoxA(NULL, (LPCSTR)body.cstring, (LPCSTR)title.cstring, MB_OK);
  scratch_end(&scratch);
}

fz_internal void
message_box_with_debugger(String title, String content, String file, u32 line)
{
  Scratch scratch = scratch_begin(0, 0);
  String body;

  for(;;)
  {
    if(raddbg_is_attached())
    {
      body = Sf(scratch.arena, S_FMT " line:%u\n" S_FMT "\n\nDebugger attached. Press 'Break' to trap, or 'Close' to dismiss.", S_ARG(file), line, S_ARG(content));
      int result = MessageBoxA(NULL, (LPCSTR)body.cstring, (LPCSTR)title.cstring, MB_OKCANCEL | MB_ICONERROR | MB_TASKMODAL);
      if(result == IDOK)
      {
        scratch_end(&scratch);
        raddbg_break();
        return;
      }
      break;
    }
    else
    {
      body = Sf(scratch.arena, S_FMT " line:%u\n" S_FMT "\n\nNo debugger attached.\n'OK' to check again, 'Cancel' to close.", S_ARG(file), line, S_ARG(content));
      int result = MessageBoxA(NULL, (LPCSTR)body.cstring, (LPCSTR)title.cstring, MB_OKCANCEL | MB_ICONERROR | MB_TASKMODAL);
      if(result == IDCANCEL) 
      { 
        break; 
      }
      if (result == IDOK)
      {
        // IDOK: loop again and re-check
      }
    }
  }

  scratch_end(&scratch);
}