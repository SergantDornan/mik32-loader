# Необходимые пакеты
ca-certificates, git, make, python3, openocd, picocom

# Установка
```
g++ installer.cpp -o install # скомпилировать установщик (если надо)
```

```
./install # установить mik32Loader
```

```
./install reinstall # переустановить mik32Loader
```

```
mik32Load uninstall # удалить mik32Loader
```
# mik32Load
mik32Loader можно вызвать из любой директории

Чтобы запустить mik32Loader просто введите "mik32Load", находясь в любой директории, затем эта директория будет передана программе C-CPP-builder

mik32Loader запоминает последние введенные флаги, так что если запустить программу без аргументов, сборка пройдет так же, как в прошлый раз

Напишите "read" в лююбом месте после "mik32Load" чтобы запустить picocom и посмотреть что контроллер выводит в терминал:

```
mik32Load read # запуск picocom
```

# Flags

-b [число] явно указать SERIAL_BOUDRATE для picocom

-p [число] указать номер USB порта для picocom 

Любой флаг не указаный выше (кроме "-o, --CC, --CXX") будет передан как аргумент программе C-CPP-builder: https://github.com/SergantDornan/C-Cpp-builder
