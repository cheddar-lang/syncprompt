#include <nan.h>
#include <v8.h>

#include <string>
#include <iostream>

// Resolve OS dependencies
#ifdef _WIN32

#include <windows.h>

#else

#include <termios.h>
#include <unistd.h>

#endif

// Set echo for prompt
void setStdinEcho(bool res) {
#ifdef _WIN32
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE); 
    DWORD mode;
    GetConsoleMode(hStdin, &mode);

    if( !res )
        mode &= ~ENABLE_ECHO_INPUT;
    else
        mode |= ENABLE_ECHO_INPUT;

    SetConsoleMode(hStdin, mode );
#else
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    if( !res )
        tty.c_lflag &= ~ECHO;
    else
        tty.c_lflag |= ECHO;

    (void) tcsetattr(STDIN_FILENO, TCSANOW, &tty);
#endif
}

// Basic prompt function
std::string syncPrompt() {
    std::string out;
    std::getline(std::cin, out);

    return out;
}

// Bindings
NAN_METHOD(syncPrompt) {
    std::string res = syncPrompt();
    info.GetReturnValue().Set(Nan::New<v8::String>( res ).ToLocalChecked());
}

NAN_METHOD(setEcho) {
    if (info.Length() < 1)
        Nan::ThrowTypeError("Expected echo argument");
    
    // Convert input to boolean
    setStdinEcho( Nan::To<bool>(info[0]).FromMaybe(false) );
}

NAN_MODULE_INIT(InitAll) {
    Nan::Set(target, Nan::New("sync_prompt").ToLocalChecked(),
              Nan::GetFunction(Nan::New<v8::FunctionTemplate>(syncPrompt)).ToLocalChecked());
    Nan::Set(target, Nan::New("set_echo").ToLocalChecked(),
              Nan::GetFunction(Nan::New<v8::FunctionTemplate>(setEcho)).ToLocalChecked());
}

NODE_MODULE(binding, InitAll)
