# dropprivs

## Description

This is a very simple tool that can drop privileges of the root user and execute
a command.


## Usage

The user to switch to must be specified on the command line (`-u`). Use a double
dash (`--`) to sepparate the parameters of `droppriv` from the command to be executed,
as in the example below:

```
# dropprivs -u bin -d /tmp -- /bin/bash -c "pwd; id"
/tmp
uid=2(bin) gid=2(bin) groups=2(bin)
```

## Building


```
$ cd source-dir
$ mkdir build && cd build
$ cmake ..
$ make
```
