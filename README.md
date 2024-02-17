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
    "message": "Hey, musk calling from mars here."   
}
```

## TO DO 
- [ ] Cleanup code, better readability
- [x] Username in arguments so that each client has unique username instead of ip being shown
- [x] Timestamps included for message sorting
- [x] Allow for "enter" to be used as key input

### Optional stuff for Kobe
- [ ] Chat history
- [ ] List of all individual users you have messaged with before
- [ ] *markdown* for some reason





