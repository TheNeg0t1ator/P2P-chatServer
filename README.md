# Peer to Peer Chat Server

This git is a chat server for peer to peer configuration

Some Rules regarding development 

Have useful branch naming - (not dev-$NAME for example) 

Have fun

The packets are sent using a JSON, something **Like this** 

```json
{
    "id": "Jeff Bezos",
    "ip": "127.0.0.1",
    "port": "666",
    "message": "Hey, musk calling from mars here.",
    "timestamp":"30/02/2030 18:43"
}
```


# How to build
To build and run (and create a runable exe) You only need to install `Qt Creator` With `MinGw`.

> To be sure, currently using  `Qt Creator` Version `6.6.1`.

When you have installed `Qt Creator` , you clone this git, and open the `Network_P2P.pro` and hit run.
you can create a runnable exe by setting qt to Release build, copying the exe, open the Qt mingw cli and go to the exe location, and run 

`windeployqt EXE_NAME`

this generates all the dll files necessary.




# TO DO 
- [ ] Cleanup code, better readability
- [x] Username in arguments so that each client has unique username instead of ip being shown
- [x] Timestamps included for message sorting
- [x] Allow for "enter" to be used as key input
- [ ] Setters
- [ ] polymorphism
- [ ] const variables and functions
- [ ] delete
- [ ] file io
- [ ] lib

### Optional stuff for Kobe
- [ ] Chat history
- [ ] List of all individual users you have messaged with before
- [ ] *markdown* for some reason

# Grading

### General 10/10

- [x] clean main (i.e. nothing in the main that should be in a class) 
- [x] no globals, but statics if needed 
- [x] correct protections 
- [x] maintainability by clean uniform code style and good function naming and/or comments everywhere 
- [x] separate header files — `chatlog.hpp`,`json.h`,`jsonparser.h`, `mainWindow.hpp`, `tcpclient.h`, `userinterface.h`
- [x] one complete project that compiles and does not crash 
- [x] fully working project 
- [x] sufficient git commits (+/- weekly) 
- [x] correct files on git 
- [x] working build manual as readme on GitHub (project must be possible to build from scratch on a clean PC) 

### OOP 11/15

- [x] at least 2 default constructors — `chatlog.hpp` line 125,247
- [x] at least 2 parameterized constructors —  `chatlog.hpp` line 247, `jsonparser.h` line 8
- [ ] at least 2 copy constructors
- [x] at least 2 destructors — `chatlog.hpp` line 264
- [x] member initialization in constructors (the stuff behind a colon) — `chatlog.hpp` line 20 , `userinterface.cpp` line 11
- [ ] constructor forwarding
- [x] useful proven (dynamic) polymorphism — `chatlog.hpp`, line 245
- [x] useful usage of "this" (if the code does not work without it) — `userinterface.cpp`, line 67, 76
- [x] useful member function — 
- [x] default values in function definition — `chatlog.hpp`, line 247 
- [x] useful member variable — `jsonparser.h` line 17
- [x] useful getters and setters for member variables — `chatlog.hpp`, lines 36,40,279
- [x] correct usage of inline function — `chatlog.hpp` line 36
- [ ] useful template function or class
- [ ] useful friend function or class

### C++ 9/15

- [ ] everything in one or more self-made namespace(s)
- [ ] 2 useful unsigned chars or other better usage of memory efficient type
- [x] at least 4 useful const references for variables — `chatlog.hpp` lines 32,36,44,47
- [x] at least 4 useful const references for functions — `jsonparser.cpp` lines 18-22 
- [x] at least 4 useful bool — `chatlog.hpp` line 23, 32, 44, 48
- [x] dynamic memory allocation (new) — `userinterface.cpp`, line 14
- [x] dynamic memory removing (delete) — `chatlog.hpp`, line 265
- [x] 2 useful (modern) call-by-references — `chatlog.hpp`, line 32 `jsonparser.cpp`, line 5
- [x] useful string class usage — `tcpclient.cpp`, line 116
- [ ] useful container class
- [x] useful usage of nullptr — `chatlog.hpp`, line 260
- [x] useful usage of (modern) file-I/O — `chatlog.hpp`
- [ ] useful exception handling
- [ ] useful usage of lambda function
- [ ] useful usage of threads

### Extensions 6/10

- [x] useful Qt class — `mainWindow.hpp`, line 16
- [x] useful usage of signals/slots — `tcpclient.h`, line 30
- [ ] test-driven development (= written test plan or unit tests)
- [ ] solve bug ticket (with pull request or commit message issue link and issue branch) 
- [x] report a bug ticket on another project — `Seppe Chess`
- [x] usage of a GUI — `userinterface.cpp`, line 118
- [ ] usage of OpenGL or other 3D engine
- [ ] useful usage of an external library (not Qt)
- [x] project that communicates (e.g. UART, BT) with hardware — `tcpclient.cpp`, line 21 (Network packets)
- [x] a nice extra that you think that should deserve grading (stuff you put time in and is not rewarded by an item above) — `userinterface.cpp`, line 70 (Markdown support)



