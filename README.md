# PiAlarm

Réveil matin intelligent sur Raspberry Pi

## Auteur

> [Téo Derboven - 60073](mailto:60073@etu.he2b.be)


## Options de configuration de la compilation

Ce projet utilise des options CMake pour sélectionner, au moment de la compilation,
le type d’affichage cible. Une seule option d’affichage peut être activée par build.

### Options disponibles

- `DISPLAY_SSD1322` (par défaut : OFF)
    
  Active le support pour l’écran SSD1322 (driver Adafruit SSD1322).


- `DISPLAY_CONSOLE` (par défaut : OFF)

  Active le support pour l’affichage en console, utile pour les tests ou fallback.

#### Règles

Une seule de ces options doit être activée. Activer les deux ou aucune provoquera une erreur à la compilation.

### Activer une option

Via la ligne de commande CMake, active simplement l’option souhaitée :

```bash
cmake -DDISPLAY_SSD1322=ON
# ou
cmake -DDISPLAY_CONSOLE=ON
```

## Documentation

Une documentation des différentes classes et méthodes est fournie dans les fichiers d'en-tête.
Elle peut être générée au format HTML en utilisant l'outil [Doxygen](https://www.doxygen.nl/).
Entrez la commande suivante à la racine du dépôt pour générer la documentation:

```console
$ doxygen
```

Celle-ci se trouvera dans le dossier `/doc`.