# PiAlarm

Réveil matin intelligent sur Raspberry Pi

## Auteur

> [Téo Derboven - 60073](mailto:60073@etu.he2b.be)


## Options de configuration de la compilation

Ce projet utilise des options CMake pour sélectionner, au moment de la compilation,
le type d’affichage cible et le niveau de log.

> **Note:** Une seule option d’affichage peut être activée par build.

### Options d'affichage disponibles

- `DISPLAY_SSD1322` (par défaut : OFF)
    
  Active le support pour l’écran SSD1322 (driver Adafruit SSD1322).


- `DISPLAY_CONSOLE` (par défaut : OFF)

  Active le support pour l’affichage en console, utile pour les tests ou fallback.

#### Règles

Une seule de ces options doit être activée. Activer les deux ou aucune provoquera une erreur à la compilation.

### Option de niveau de log

`LOG_LEVEL` (par défaut : `info`)

Définit le niveau de verbosité du logger `spdlog`.
Cette option est transmise au code via une macro de compilation.

Le fichier de log se trouvera dans le dossier `/logs`.

#### Niveaux disponibles :

- `trace`
- `debug`
- `info` (valeur par défaut)
- `warn`
- `error`
- `critical`

### Activer une option

Via la ligne de commande CMake, active simplement l’option souhaitée :

```bash
# exemples
cmake -DDISPLAY_SSD1322=ON -DLOG_LEVEL=info
cmake -DDISPLAY_CONSOLE=ON -DLOG_LEVEL=debug
```

## Documentation

Une documentation des différentes classes et méthodes est fournie dans les fichiers d'en-tête.
Elle peut être générée au format HTML en utilisant l'outil [Doxygen](https://www.doxygen.nl/).
Entrez la commande suivante à la racine du dépôt pour générer la documentation:

```console
$ doxygen
```

Celle-ci se trouvera dans le dossier `/doc`.