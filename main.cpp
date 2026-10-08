#include <windows.h>

DWORD shove(DWORD& err, HANDLE wr, const char* b, DWORD k)
{
  DWORD r = 0;
  DWORD h = 0;
  while (r < k) {
    DWORD st = WriteFile(wr, b + r, k - r, &h, NULL);
    if (!st) {
      auto err = GetLastError();
      break;
    }
    r += h;
  }
  return r;
}

DWORD getShoved(DWORD& err, HANDLE rd, char* b, DWORD k)
{
  DWORD r = 0;
  DWORD h = 0;
  while (r < k) {
    DWORD st = ReadFile(rd, b + r, k - r, &h, NULL);
    if (!st) {
      auto err = GetLastError();
      break;
    }
    r += h;
  }
  return r;
}



int main()
{
  return 0;
}
