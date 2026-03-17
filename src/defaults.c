#include "defaults.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

char *defaults_read_string(const char *domain, const char *key)
{
  char cmd[512];
  if (strcmp(domain, "-g") == 0) {
    snprintf(cmd, sizeof(cmd), "defaults read -g %s 2>/dev/null", key);
  } else {
    snprintf(cmd, sizeof(cmd), "defaults read %s %s 2>/dev/null", domain, key);
  }
  
  FILE *fp = popen(cmd, "r");
  if (!fp) {
    return NULL;
  }

  char *result = NULL;
  char buf[1024];
  if (fgets(buf, sizeof(buf), fp)) {
    // Remove trailing newline and quotes
    size_t len = strlen(buf);
    if (len > 0 && buf[len-1] == '\n') buf[--len] = '\0';
    if (len > 0 && buf[0] == '"' && buf[len-1] == '"') {
      buf[len-1] = '\0';
      result = strdup(buf + 1);
    } else {
      result = strdup(buf);
    }
  }
  pclose(fp);
  return result;
}

float defaults_read_float(const char *domain, const char *key)
{
  char cmd[512];
  if (strcmp(domain, "-g") == 0) {
    snprintf(cmd, sizeof(cmd), "defaults read -g %s 2>/dev/null", key);
  } else {
    snprintf(cmd, sizeof(cmd), "defaults read %s %s 2>/dev/null", domain, key);
  }
  
  FILE *fp = popen(cmd, "r");
  if (!fp) {
    return 0.0f;
  }

  float result = 0.0f;
  char buf[256];
  if (fgets(buf, sizeof(buf), fp)) {
    // Handle "true" and "false"
    if (strncmp(buf, "true", 4) == 0) {
      result = 1.0f;
    } else if (strncmp(buf, "false", 5) == 0) {
      result = 0.0f;
    } else {
      sscanf(buf, "%f", &result);
    }
  }
  pclose(fp);
  return result;
}

int defaults_write_float(const char *domain, const char *key, float value)
{
  char cmd[512];
  if (strcmp(domain, "-g") == 0) {
    snprintf(cmd, sizeof(cmd), "defaults write -g %s -float %f", key, value);
  } else {
    snprintf(cmd, sizeof(cmd), "defaults write %s %s -float %f", domain, key, value);
  }
  
  return system(cmd);
}

int defaults_write_string(const char *domain, const char *key, const char *value)
{
  char cmd[512];
  if (strcmp(domain, "-g") == 0) {
    snprintf(cmd, sizeof(cmd), "defaults write -g %s \"%s\"", key, value);
  } else {
    snprintf(cmd, sizeof(cmd), "defaults write %s %s \"%s\"", domain, key, value);
  }
  
  return system(cmd);
}

int defaults_verify_float(const char *domain, const char *key, float expected)
{
  float current = defaults_read_float(domain, key);
  return (fabs(current - expected) < 0.01f) ? 0 : 1;
}
