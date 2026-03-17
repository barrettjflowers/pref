#ifndef PREFS_H
#define PREFS_H

#include <stdio.h>

#define PREFS_MAX_NAME_LEN 64
#define PREFS_MAX_VALUE_LEN 256

typedef enum {
  PREF_TYPE_BOOL,
  PREF_TYPE_INT,
  PREF_TYPE_FLOAT,
  PREF_TYPE_STRING,
  PREF_TYPE_ARRAY
} PrefType;

typedef struct {
  const char *cmd;           // command-style name: "docksize", "trackpad-speed"
  const char *domain;        // macOS domain
  const char *key;           // macOS key
  PrefType type;
  const char *description;
  
  // value transformation (for scaling)
  float (*to_system)(float user_val);    // user value -> macOS value
  float (*from_system)(float system_val); // macOS value -> user value
  
  float min_user;  // min user value
  float max_user;  // max user value
  float defval;    // default user value
} PrefMapping;

PrefMapping *prefs_find(const char *cmd);
const PrefMapping *prefs_all(void);
int prefs_count(void);

float pref_encode(const PrefMapping *pref, float user_val);
float pref_decode(const PrefMapping *pref, float system_val);
void pref_print(FILE *f, const PrefMapping *pref);

#endif
