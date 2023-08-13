# Astrologie - FastCGI

Cette application établit une liaison avec la librairie [SweBressaniDev](https://github.com/stephaneworkspace/SweBressaniDev), conçue comme un package Swift hybride (Swift + C++). Dans ce projet, j'ai extrait l'intégralité du code C++ pour le transformer en un FastCGI.

> **Licence :** [GNU3](LICENSE)

## Prérequis

### macOS:

```
brew install fcgi
```

### Ubuntu:

```
sudo apt update
sudo apt install libfcgi-dev cmake clang
```

Pour apache
```
sudo apt update
sudo apt install libapache2-mod-fcgid
sudo a2enmod fcgid
```

## Compilation

1. **Créez un répertoire pour les fichiers de build**:
   
```
mkdir build
cd build
```

2. **Générez les fichiers de build avec CMake**:

```
cmake ..
```

3. **Compilez le projet**:

```
make astro.fcgi
```

Après la compilation, l'exécutable `astro.fcgi` sera disponible dans le répertoire `build/bin`.