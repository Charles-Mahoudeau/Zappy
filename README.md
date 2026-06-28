# Zappy

[![Mirror](https://github.com/Charles-Mahoudeau/Zappy/actions/workflows/mirror.yml/badge.svg)](https://github.com/Charles-Mahoudeau/Zappy/actions/workflows/mirror.yml)
[![Linter](https://github.com/Charles-Mahoudeau/Zappy/actions/workflows/linter.yml/badge.svg)](https://github.com/Charles-Mahoudeau/Zappy/actions/workflows/linter.yml)
[![CTest](https://github.com/Charles-Mahoudeau/Zappy/actions/workflows/ctest.yml/badge.svg)](https://github.com/Charles-Mahoudeau/Zappy/actions/workflows/ctest.yml)
[![Makefile](https://github.com/Charles-Mahoudeau/Zappy/actions/workflows/makefile.yml/badge.svg)](https://github.com/Charles-Mahoudeau/Zappy/actions/workflows/makefile.yml)
![CodeRabbit Pull Request Reviews](https://img.shields.io/coderabbit/prs/github/Charles-Mahoudeau/Zappy?utm_source=oss&utm_medium=github&utm_campaign=Charles-Mahoudeau%2FZappy&labelColor=171717&color=FF570A&link=https%3A%2F%2Fcoderabbit.ai&label=CodeRabbit+Reviews)

## Contributors

* Amos Almacin
* Charles Mahoudeau
* Ethan Charpentier
* Ewan Czarny
* Noe Caillaud
* Thomas Boucard

## Installation

### 1. Build the project

```shell
make
```

### 2. Run the project

```shell
./zappy_server -p 4242 -x 10 -y 10 -n team1 team2 -c 5 -f 100
./zappy_gui -h 127.0.0.1 -p 4242   
./zappy_ai -h 127.0.0.1 -p 4242 -n team1 
```
