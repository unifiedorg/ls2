![Unified Banner](https://raw.githubusercontent.com/unifiedorg/.github/main/img/logo-banner-small.png)

# ls2
An advanced version of the linux command `ls` that displays more information in a table-like manner, with colors!
| Options    | Output                                    |
| ---------- | ----------------------------------------- |
| -h         | Displays the help menu                    |
| -v         | Displays the version of ls2.              |
| -a         | Displays hidden files.                    |
| -n         | Displays permissions in a numerical form. |

![ls2 showcase](./img/showcase.png)

# How to install
## Debian
You can find the latest [debian binary over here](https://github.com/unifiedorg/ls2/releases/latest), and install it by running
```
sudo dpkg -i ls2-1.0-amd64.deb
```

## Manually building
You can manually build by :

**1st) Installing GCC**
```
sudo apt-get update
sudo apt-get install build-essential
```

**2nd) Building the ls2.c file**
```
gcc -o ls2 ./src/ls2.c
```

**3rd) Moving the ls2 executable to bin**
```
sudo mv ./ls2 /usr/local/bin/ls2
```

# Contributing
Want to contribute? Check out [CONTRIBUTING.md](./CONTRIBUTING.md)

# Contributers
Maintainer & Creator - TheUnium
