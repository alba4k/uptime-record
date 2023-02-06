# Uptime Record - alba4k

![uptime-record](images/screenshot.png)

### This useless script is just made to make it possible to have your highest-ever uptime saved somewhere - that way you can flex with your friends how maniac you are

---

## Usage

The uptime is updated every time the program is ran (if higher than the previous record), so all you need to do is run `uptime-record`.

The highest uptime ever recorded is saved in `$XDG_DATA_HOME/uptime-record`, usually `~/.local/share/uptime-record`. This file can be manually edited, but that would make you a cheater. Don't.

## **Warning**: If you've been using an old version of uptime record, your record might still be in ~/.config/uptime-record. Move it to ~/.local/share/uptime-record if you want your old time to be kept

The program accepts some command line arguments, a help page can be found when running with the `--help` flag. 

## How to compile and install this program?

_I might publish an AUR package for this, should I?_

First of all, you need to clone this repository from github:

```shell
$ git clone https://github.com/alba4k/uptime-record
$ cd uptime-record
```

A compiled executable should already be present. You can run it directly or install for an easier access to it (it will be copied to `/usr/bin`):

```shell
# sudo make install
```

You might also want to compile it manually before you install it:

```shell
$ make
```

If you just compiled the source code, you might also want to reinstall the executable, as described earlier

## Tips and tricks

You might want to run this automatically before every shutdown or reboot. You can easily achieve this by having some sort of executable (maybe a shell script or a symlink/copy) that runs the program in `/usr/lib/systemd/system-shutdown/`. Every executable file in this directory will be ran before your system turns off.

Also, you can run the program automatically after you log in every 5 minutes by copying `uptime-record.service` to `~/.config/systemd/user`, followed by running `systemctl --user enable --now uptime-record.service` to enable the service.
You could also copy it to `/etc/systemd/system/` or `/usr/lib/systemd/system` and enable it using `systemctl enable --now uptime-record.service`.
The exact interval can be changed by editing the file itself.

Both methods rely on systemd. If you don't know what your init system is, you are probably using systemd. If you're not, then you probably already know how to do the same things using your init. You can usually find out what init system you are using by simply googling "[Your distro] default init system".

## To-do list:
This is just for me, those are some features I would like to implement but am too lazy to.

- [ ] `--read-only` (`-r`) flag. This would only display the highest recorded uptime. Conflicts with `--silent` and `--background`.

---

## © Aaron Blasko - 2022
###### I spent waaay too much time on this README lmao
