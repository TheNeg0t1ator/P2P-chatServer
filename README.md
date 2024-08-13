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





