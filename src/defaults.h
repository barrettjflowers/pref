#ifndef DEFAULTS_H
#define DEFAULTS_H

float defaults_read_float(const char *domain, const char *key);
char *defaults_read_string(const char *domain, const char *key);
int defaults_write_float(const char *domain, const char *key, float value);
int defaults_write_string(const char *domain, const char *key, const char *value);
int defaults_verify_float(const char *domain, const char *key, float expected);

#endif
