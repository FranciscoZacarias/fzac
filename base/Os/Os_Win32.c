function void
message_box(String title, String content, String file, u32 line)
{
  Scratch scratch = scratch_begin(0,0);
  String body = Sf(scratch.arena, S_FMT" line:%u\n"S_FMT, S_ARG(file), line, S_ARG(content));
  MessageBoxA(NULL, (LPCSTR)body.cstring, (LPCSTR)title.cstring, MB_OK);
  scratch_end(&scratch);
}