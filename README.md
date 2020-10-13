## Building

```
sudo apt install libzmq3-dev
./copy_qemu.sh <path_to_qemu>
make TARGET=whatever
```

## Using

```
./syscall-agent-whatever <zmq server port>
```

for the love of god don't send any memory management syscalls
