# Câblage

Carte : ESP32-S3 DevKit N16R8, module TTL ↔ RS485 XY-K485. Toutes les masses (GND) sont reliées entre elles.

## Correspondance des broches

| Broche ESP32-S3 | Vers | Rôle |
|---|---|---|
| GPIO17 | XY-K485 TXD | envoi vers la pompe |
| GPIO18 | XY-K485 RXD | réponses de la pompe |
| 3V3 | XY-K485 VCC | alimentation du module |
| GND | XY-K485 GND | masse commune |

Broches à éviter sur la N16R8 : 35, 36, 37 (PSRAM), 19, 20 (USB), 43, 44 (logs), 0, 45, 46 (démarrage).

## Côté pompe

| XY-K485 | Jack 3,5 mm de la pompe |
|---|---|
| A+ | bague |
| B- | pointe |
| terre | manchon |

Brochage repris du projet Hackaday « Eheim Pro 3e Control ». Inverser A et B ne casse rien : si la pompe ne répond pas, les permuter, puis permuter TXD et RXD.

## Alimentation

Chargeur USB-C 5 V, 1 A minimum, sur la prise USB-C de l'ESP32. **Ne pas alimenter l'ESP32 par le jack de la pompe** : ses trois contacts servent aux données et à la masse.

## Notes

- Le module XY-K485 gère seul le sens émission/réception (pas de broche DE/RE à piloter).
- Un seul maître sur le bus : le logiciel EHEIM ControlCenter et l'ESP32 ne peuvent pas être branchés en même temps (la pompe n'a qu'une prise jack).
