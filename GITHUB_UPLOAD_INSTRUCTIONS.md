# Инструкции по загрузке проекта на GitHub

Следуйте этим шагам, чтобы загрузить проект на GitHub по адресу https://github.com/ctpelok73/re4savelocal

## Вариант 1: Использование Git CLI

### 1. Установите Git
Если Git не установлен, скачайте и установите с https://git-scm.com/

### 2. Откройте командную строку в этой папке
Откройте терминал/CMD/PowerShell в текущей папке проекта

### 3. Инициализируйте репозиторий
```bash
git init
git add .
git commit -m "Initial commit: RE4 Save Redirect Plugin"
```

### 4. Создайте репозиторий на GitHub
- Перейдите на https://github.com/new
- Введите имя репозитория: `re4savelocal`
- Выберите Public
- НЕ устанавливайте галочку "Initialize this repository with a README" (так как у нас уже есть README)
- Нажмите "Create repository"

### 5. Свяжите локальный репозиторий с GitHub
```bash
git remote add origin https://github.com/ctpelok73/re4savelocal.git
git branch -M main
git push -u origin main
```

## Вариант 2: Загрузка через веб-интерфейс

### 1. Создайте репозиторий на GitHub
- Перейдите на https://github.com/new
- Введите имя репозитория: `re4savelocal`
- Выберите Public
- НЕ устанавливайте галочку "Initialize this repository with a README"
- Нажмите "Create repository"

### 2. Загрузите файлы
- После создания репозитория нажмите кнопку "Add file" → "Upload files"
- Перетащите все файлы из текущей папки в окно браузера:
  - dllmain.cpp
  - re4_save_redirect.ini
  - Makefile
  - build.bat
  - download_dependencies.bat
  - README.md
  - .gitignore
- Нажмите "Commit changes"

## Вариант 3: Создание ZIP-архива

Если вы хотите создать архив для загрузки:

1. Выберите все файлы в папке
2. Щелкните правой кнопкой мыши → "Send to" → "Compressed (zipped) folder"
3. Затем загрузите этот архив в репозиторий GitHub через веб-интерфейс

## После загрузки

После успешной загрузки на GitHub проект будет доступен по адресу:
https://github.com/ctpelok73/re4savelocal

Убедитесь, что:
- Все файлы загружены правильно
- README.md отображается корректно на главной странице репозитория
- В репозитории есть все необходимые файлы для сборки плагина