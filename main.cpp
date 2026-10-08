#include <windows.h>
#include <iostream>

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
  HANDLE read = NULL, write = NULL;
  SECURITY_ATTRIBUTES sa;
  sa.nLength = sizeof(SECURITY_ATTRIBUTES);
  sa.bInheritHandle = TRUE;
  sa.lpSecurityDescriptor = NULL;

  if (!CreatePipe(&read, &write, &sa, 0)) {
    std::cerr << "failed to create a pipe\n";
  }

  SetHandleInformation(write, HANDLE_FLAG_INHERIT, 0);

  STARTUPINFOA si;
  PROCESS_INFORMATION pi;
  ZeroMemory(&si, sizeof(STARTUPINFOA));
  ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

  si.cb = sizeof(STARTUPINFOA);
  si.hStdInput = read;
  si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
  si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
  si.dwFlags |= STARTF_USESTDHANDLES;

  char cmd[] = "printer.exe";
  if (!CreateProcessA(NULL, cmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
    std::cerr << "createproccess failed: " << GetLastError() << "\n";
    CloseHandle(read);
    CloseHandle(write);
    return 1;
  }

  CloseHandle(read);
  std::string dummy;
  std::cin >> dummy;
  DWORD bytesWritten = 0;
  WriteFile(write, dummy.c_str(), static_cast< DWORD >(dummy.size()), &bytesWritten, NULL);

  CloseHandle(write);

  WaitForSingleObject(pi.hProcess, INFINITE);

  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);

  return 0;
  return 0;
}
