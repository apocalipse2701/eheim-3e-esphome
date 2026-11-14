# eheim-3e-esphome

Supervision d'un filtre **EHEIM professionel 3e** avec un ESP32-S3 sous ESPHome, intégré à Home Assistant. La pompe est lue par sa prise jack (bus RS485), sans passer par le logiciel Windows EHEIM ControlCenter.

## Fonctions

Testé sur un 2076 (professionel 3e 450), firmware 3.80. Devrait fonctionner avec les 2074 (350) et 2078 (700).

- débit mesuré, débit de consigne, débit relatif
- vitesse rotor, palier, mode (flux constant, Bio, Stream, manuel)
- temps de fonctionnement, heures depuis et avant entretien, date du prochain entretien
- alertes : erreurs rotor / air / température, consigne non atteinte, débit faible
- réglage du palier de débit (flux constant, 0 à 14)
- page web locale sur l'ESP32 (`http://eheim-3e.local`)

## Matériel

| Élément | Exemple |
|---|---|
| Microcontrôleur | ESP32-S3 DevKit N16R8 |
| Interface pompe | module TTL ↔ RS485 XY-K485, jack 3,5 mm mâle stéréo (adaptateur à bornier) |
| Alimentation | chargeur USB-C 5 V, 1 A minimum |

Câblage détaillé : [docs/cablage.md](docs/cablage.md)

## Installation

1. Copier `esphome/eheim-3e.yaml` et `esphome/eheim3e.h` dans le dossier ESPHome (`/config/esphome/` sous Home Assistant).
2. Copier `esphome/secrets.yaml.example` en `secrets.yaml` et le remplir.
3. Dans ESPHome Device Builder : **Install** → « Brancher sur cet ordinateur » pour le premier flashage USB, puis « Sur le réseau ».
4. Ajouter l'appareil dans Home Assistant avec la clé API.

## Tester la pompe depuis un PC

Avec le câble USB d'origine (EHEIM Interface 4020740) :

```
pip install pyserial
python tools/eheim3e_test.py COM3
```

Le script ne fait que lire : firmware, type de pompe, puis les données toutes les 2 secondes.

## Protocole

Description complète : [docs/protocole.md](docs/protocole.md)

## Avertissement

Projet personnel, non affilié à EHEIM GmbH & Co. KG. EHEIM et professionel sont des marques de leurs propriétaires. Le protocole a été documenté à des fins d'interopérabilité ; aucun fichier du logiciel EHEIM n'est distribué ici. L'envoi de commandes d'écriture à la pompe se fait à vos risques : commencez toujours en lecture seule.

## Licence

MIT, voir [LICENSE](LICENSE).
