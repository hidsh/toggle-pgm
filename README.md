# toggle-pgm

```
Usage: toggle-pgm proc_name args ...
```
Starts `proc_name` if it does **not** exist, otherwise just kill it.

# for what?
This program is written for [fuzzel](https://codeberg.org/dnkl/fuzzel).

With fuzzel in [sway](https://swaywm.org/), It can be emulate to behave *Start Menu* in Windows 11 when you hit `Win` key.

~/.config/sway/config.d/default
```
# Start your launcher (Win key single-tap)
bindcode 133 exec toggle-pgm fuzzel
```

~/.config/fuzzel/fuzzel.ini
```ini
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
