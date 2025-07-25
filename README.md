# ♟️ Шахматы на C++

Полноценная реализация шахмат на C++ с использованием **SFML** и движка **Stockfish**, поддерживающая все правила классических шахмат, режимы игры и визуальный интерфейс.

---

## 🚀 Возможности

* ✅ Игра против **человека** или **компьютера (Stockfish)**
* ♟️ **3 режима игры**: классический, Фишера, до трёх шахов
* 💾 Сохранение и загрузка партии (формат FEN)
* 🔧 Редактор шахматных задач
* 📚 10 встроенных шахматных задач

---

## 🎮 Режимы игры

1. **Классический**

   > Классическая расстановка. Игра продолжается до мата или пата.

2. **Шахматы Фишера**

   > Генерация начальной позиции по правилам шахмат Фишера. Учитываются все особенности рокировки и расстановки.

3. **До трёх шахов**

   > Классическая доска. Игра заканчивается после получения одной стороной трёх шахов.

---

## 🖥️ Минимальные системные требования

* **ОС**: Windows 10 64-bit
* **Оперативная память**: 512 МБ
* **Свободное место на диске**: 200 МБ
* **Минимальное разрешение экрана**: **900×900 пикселей**

---

## 🔧 Зависимости

* **C++17** или выше
* **SFML 2.5+**
* **CMake**
* **GCC / GDB**
* **Git**

---

## 🛠 Установка и сборка проекта

### 1. Установка MSYS2 и зависимостей

1. Скачайте и установите **MSYS2** с официального сайта:
   👉 [https://www.msys2.org](https://www.msys2.org)
   Установите его в папку **без кириллицы**, например: `C:\msys64`.

2. Запустите **MSYS2 MinGW 64-bit** через меню «Пуск».

3. В терминале выполните обновление и установку необходимых пакетов:

   ```bash
   pacman -Syu
   pacman -S mingw-w64-x86_64-gcc -y
   pacman -S mingw-w64-x86_64-gdb -y
   pacman -S mingw-w64-x86_64-cmake -y
   pacman -S mingw-w64-x86_64-sfml -y
   pacman -S git -y
   ```

---

### 2. Клонирование проекта

1. Перейдите в желаемую директорию (например, `C:/`):

   ```bash
   cd /c
   ```

2. Клонируйте репозиторий:

   ```bash
   git clone https://www.github.com/razil131/chess
   ```

---

### 3. Установка Stockfish

1. Скачайте **Stockfish 17.1 для Windows x64** с официального сайта:

   👉 [https://stockfishchess.org/download/](https://stockfishchess.org/download/)

2. Распакуйте архив **в корневую папку проекта \*\*\*\*`chess/`**

   ⚠️ **Не меняйте имя папки или файлов Stockfish!**

---

### 4. Сборка проекта

1. Вернитесь в окно MSYS2 и выполните:

   ```bash
   cd chess
   mkdir -p build
   cd build
   cmake ..
   cmake --build .
   ```

---

### 5. Настройка системных переменных среды

Чтобы приложение могло обнаружить инструменты MinGW, добавьте папку `mingw64/bin` из установки MSYS2 в системный PATH.

1. Откройте **Изменение системных переменных среды**
2. Выберите **Переменные среды...**
3. В разделе **Системные переменные** найдите `Path` → нажмите **Изменить**
4. Нажмите **Создать** и добавьте путь к `mingw64/bin`:

   ```
   <MSYS2_ROOT>\mingw64\bin
   ```

   где `<MSYS2_ROOT>` — это папка, в которую вы устанавливали MSYS2.
5. Нажмите **ОК** и **перезагрузите компьютер**.

---

## ▶️ Запуск приложения

После перезагрузки можно запустить:

```bash
****\chess\build\chess.exe
```

---

## 📄 Лицензия

См. файл [LICENSE](./LICENSE)

---

## 🙋‍♂️ Авторы

* [Илья (shedoesacase)](https://t.me/xxtsmx)
* [Илья (razil)](https://t.me/razil131)
