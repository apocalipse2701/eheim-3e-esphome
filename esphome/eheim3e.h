// Protocole EHEIM professionel 3e (2074 / 2076 / 2078)
// Reconstruit depuis EHEIM ControlCenter 2.3 et validé sur un 2076, firmware 3.80.
//
// Trame : 0x55, adresse (0x60 = pompe, 0xFF = lecture firmware), longueur totale,
//         commande, données. Pas de checksum. 115200 bauds, 8N1, RS485 half-duplex.
#pragma once
#include "esphome.h"
#include <vector>
#include <cstring>

namespace eheim {

static const uint8_t START = 0x55;
static const uint8_t DEV_PUMP = 0x60;
static const uint8_t DEV_BCAST = 0xFF;

static const uint8_t CMD_GET_DEVICEID = 0x01;     // réponse cmd 0xFF : firmware data[0].data[1]
static const uint8_t CMD_GET_ACTUALDATA = 0x13;   // réponse cmd 0x10 : 55 octets
static const uint8_t CMD_GET_PUMPVERSION = 0x16;  // réponse 0x90 0x16 <type-2000>
static const uint8_t CMD_SET_CONSTANTFLOW = 0x28; // data[0] = palier 0..14, réponse 0x90 0x28

// Bits du mot de mode (enum PumpModes du logiciel Eheim)
static const uint16_t PM_ROTOR_ERR = 0x0100;
static const uint16_t PM_AIR_ERR = 0x0200;
static const uint16_t PM_TEMP_ERR = 0x0400;

// Structure FilterData (Pack=1, little-endian), 51 octets après l'en-tête de 4 octets
struct __attribute__((packed)) Data {
  int16_t frequenz, frequenz_soll, current, peak_current, calc_current, dfs_faktor, dfs;
  uint8_t drehzahl, drehrichtung, pulsbreite, pumpe_on, key_state, sensor;
  int16_t led_data, pumpe_mode;
  uint8_t menu_mode, status, soll_step, dfs_calc_diff;
  int16_t regler_dfs_soll, regler_freq_calc, regler_freq_set, dfs_stunde, dfs_tag, winkel;
  int32_t run_time, actual_time;
  int16_t wartung_hours;
  uint8_t wartung_month;
};
static_assert(sizeof(Data) == 51, "taille FilterData incorrecte");

// Débit en l/h par palier, index = palier + 1 (FilterParametersHelper.CalculateFlow)
static const uint16_t FLOW_2074[16] = {585, 400, 440, 480, 515, 550, 585, 620, 650, 680, 710, 740, 770, 800, 830, 860};
static const uint16_t FLOW_2076[16] = {650, 400, 460, 515, 565, 610, 650, 690, 730, 770, 805, 840, 875, 910, 945, 980};
static const uint16_t FLOW_2078[16] = {700, 400, 470, 540, 600, 650, 700, 745, 785, 825, 865, 905, 945, 985, 1025, 1065};

inline int flow_lh(int type, int step) {
  int i = step + 1;
  if (i < 0 || i > 15) return 0;
  if (type == 2074) return FLOW_2074[i];
  if (type == 2078) return FLOW_2078[i];
  return FLOW_2076[i];
}

inline uint8_t last_tx[5] = {0};

// Vide le tampon de réception puis envoie une trame de 5 octets
inline void send(esphome::uart::UARTComponent *u, uint8_t dev, uint8_t cmd, uint8_t data = 0) {
  uint8_t b;
  while (u->available()) u->read_byte(&b);
  uint8_t f[5] = {START, dev, 5, cmd, data};
  memcpy(last_tx, f, 5);
  u->write_array(f, 5);
  u->flush();
}

// Lit tout ce qui est arrivé, découpe en trames et retire l'écho de notre propre envoi
inline std::vector<std::vector<uint8_t>> read_frames(esphome::uart::UARTComponent *u) {
  std::vector<uint8_t> raw;
  uint8_t b;
  while (u->available() && raw.size() < 512) {
    u->read_byte(&b);
    raw.push_back(b);
  }
  std::vector<std::vector<uint8_t>> frames;
  size_t i = 0;
  while (i + 3 <= raw.size()) {
    if (raw[i] == START && (raw[i + 1] == DEV_PUMP || raw[i + 1] == DEV_BCAST) && raw[i + 2] >= 4) {
      size_t n = raw[i + 2];
      if (i + n > raw.size()) break;
      std::vector<uint8_t> f(raw.begin() + i, raw.begin() + i + n);
      bool echo = (n == 5 && memcmp(f.data(), last_tx, 5) == 0);
      if (!echo) frames.push_back(f);
      i += n;
    } else {
      i++;
    }
  }
  return frames;
}

}  // namespace eheim
