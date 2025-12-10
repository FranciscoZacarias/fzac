function b32
update_window_events()
{
  for (;;)
  {
    MSG message;
    BOOL result = PeekMessageW(&message, NULL, 0, 0, PM_REMOVE);
    if (!result) break;
    if (message.message == WM_QUIT) return 0;

    switch (message.message)
    {
      case WM_KEYDOWN:
      {
      }
      break;
    }

    TranslateMessage(&message);
    DispatchMessageW(&message);
  }

  return true;
}