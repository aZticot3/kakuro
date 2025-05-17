# Kakuro Solver

Ce projet est un solveur de jeu Kakuro en C++, développé dans le cadre du cours L2S4P - POO2.

## Description

Le Kakuro est un jeu de type "nombres fléchés" où le but est de remplir une grille avec des chiffres de 1 à 9 tout en respectant les sommes indiquées dans les cases d'indication. Les chiffres ne peuvent apparaître qu'une seule fois dans chaque séquence (ligne ou colonne).

## Installation

Le projet est fourni avec un Makefile pour faciliter la compilation et l'exécution.

```bash
# Pour compiler le projet
make

# Pour nettoyer les fichiers compilés
make clean

# Pour recompiler entièrement le projet
make rebuild
```

## Exécution

Un fichier binaire exécutable est déjà disponible dans le répertoire `bin`. Pour lancer le jeu :

```bash
# Depuis la racine du projet
./bin/kakuro
```

## Fonctionnalités

- Chargement de grilles Kakuro depuis des fichiers de différents formats (format par défaut et JSON)
- Résolution automatique de grilles
- Mode de jeu interactif avec vérification des solutions
- Interface utilisateur en ligne de commande

## Comment jouer

Après avoir lancé le jeu, un menu principal s'affiche avec les options suivantes :

1. **Charger une grille** - Permet de sélectionner une grille parmi celles disponibles
2. **Afficher la grille actuelle** - Montre la grille de jeu actuelle
3. **Jouer manuellement** - Permet de remplir la grille case par case
4. **Résoudre automatiquement** - Le programme résout la grille automatiquement
5. **Vérifier ma solution** - Vérifie si votre solution est correcte
6. **Quitter** - Quitte le jeu

En mode de jeu manuel, utilisez le format suivant pour entrer une valeur :
```
ligne colonne valeur
```
Par exemple, pour entrer le chiffre 5 à la ligne 2, colonne 3 :
```
2 3 5
```

Autres commandes en mode jeu manuel :
- `ligne colonne c` - Ecrase la valeur de la case
- `v` - Vérifie votre solution actuelle
- `h` - Demande un indice : donne 1 chiffre bien placé
- `q` - Quitte le mode jeu manuel

## Ajouter de nouvelles grilles

Pour ajouter de nouvelles grilles au jeu, placez les fichiers de grille dans le répertoire `grilles/`. 

Le programme prend en charge plusieurs formats de fichiers :
- Format par défaut (`.kakuro` ou `.txt`)
- Format JSON (`.json`)

Les nouvelles grilles seront automatiquement détectées au lancement du jeu.

## Format des fichiers de grille

### Format par défaut
```
5 4     // hauteur et largeur de la grille
# # 25/ 2/
# 5/8 _ _
/11 _ 8 5/
/15 2 _ _
# /3 _ _
```

### Format JSON
```json
{
  "height": 5,
  "width": 4,
  "cells": [
    {"row": 0, "col": 0, "type": "black"},
    {"row": 0, "col": 2, "type": "clue", "downSum": 25, "rightSum": 0},
    [...]
  ]
}
```

## Structure du projet

- `bin/` - Contient l'exécutable
- `grilles/` - Contient les fichiers de grilles
- `include/` - Contient les fichiers d'en-tête
- `src/` - Contient les fichiers sources
- `obj/` - Contient les fichiers objets générés lors de la compilation
