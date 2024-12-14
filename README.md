# toggle-pgm

```
Usage: toggle-pgm proc_name args ...
```
Starts `proc_name` if it does **not** exist, otherwise kill it.

# for what?
This program is written for fuzzel.
With fuzzel in sway, It can be emulate to behave *Start Menu* in Windows 11 when you hit `Win` key.

```:~/.config/sway/config.d/default
# Start your launcher (Win key single-tap)
bindcode 133 exec toggle-pgm fuzzel
```

```ini:~/.config/fuzzel/fuzzel.ini
[key-bindings]
#cancel=Escape Control+g Control+c
cancel=Super_L Escape Control+g Control+c
```

# build

```
git clone https://github.com/hidsh/toggle-pgm
cd toggle-pgm
make
```
