# Astrologie - FastCGI

Un simple projet FastCGI "Hello, World!".

## Prérequis

### macOS:

\```
brew install fcgi
\```

### Ubuntu:

\```
sudo apt update
sudo apt install libfcgi-dev cmake clang
\```

Pour apache
\```
sudo apt update
sudo apt install libapache2-mod-fcgid
sudo a2enmod fcgid
\```

## Compilation

1. **Créez un répertoire pour les fichiers de build**:
   
\```
mkdir build
cd build
\```

2. **Générez les fichiers de build avec CMake**:

\```
cmake ..
\```

3. **Compilez le projet**:

\```
make
\```

Après la compilation, l'exécutable `astro.fcgi` sera disponible dans le répertoire `build`.
