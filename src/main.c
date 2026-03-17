#include "defaults.h"
#include "prefs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static void print_help(void)
{
  printf("Usage: pref <command> [args]\n");
  printf("\nCommands:\n");
  printf("  pref help                    Show this help message\n");
  printf("  pref list                    List all available preferences\n");
  printf("  pref list -w                 List current global (user) preferences\n");
  printf("  pref <name>                  Get current value\n");
  printf("  pref <name> <value>         Set value\n");
  printf("\nExamples:\n");
  printf("  pref docksize                # get current size\n");
  printf("  pref docksize 8              # set size to 8\n");
  printf("  pref trackpad-speed 5        # set trackpad speed\n");
}

static void list_written(void)
{
  printf("Current global preferences (defaults read -g):\n\n");
  FILE *fp = popen("defaults read -g 2>/dev/null | head -50", "r");
  if (!fp) {
    fprintf(stderr, "Failed to read global preferences\n");
    return;
  }
  
  char buf[256];
  while (fgets(buf, sizeof(buf), fp)) {
    printf("%s", buf);
  }
  pclose(fp);
}

int main(int argc, char *argv[])
{
  if (argc < 2) {
    print_help();
    return 1;
  }

  if (strcmp(argv[1], "help") == 0) {
    print_help();
    return 0;
  }

  if (strcmp(argv[1], "list") == 0) {
    if (argc > 2 && (strcmp(argv[2], "-w") == 0 || strcmp(argv[2], "--written") == 0)) {
      list_written();
      return 0;
    }
    const PrefMapping *p = prefs_all();
    int count = prefs_count();
    printf("Available preferences:\n\n");
    for (int i = 0; i < count; i++) {
      pref_print(stdout, &p[i]);
    }
    return 0;
  }

  PrefMapping *pref = prefs_find(argv[1]);
  if (!pref) {
    fprintf(stderr, "Unknown preference: %s\n", argv[1]);
    fprintf(stderr, "Use 'pref help' to see available commands\n");
    return 1;
  }

  if (argc == 2) {
    if (pref->type == PREF_TYPE_STRING) {
      char *val = defaults_read_string(pref->domain, pref->key);
      printf("%s\n", val ? val : "(not set)");
      free(val);
    } else {
      float val = defaults_read_float(pref->domain, pref->key);
      if (pref->from_system) {
        printf("%.1f (system: %.4f)\n", pref_decode(pref, val), val);
      } else {
        printf("%.1f\n", val);
      }
    }
    return 0;
  }

  if (pref->type == PREF_TYPE_STRING) {
    defaults_write_string(pref->domain, pref->key, argv[2]);
    printf("Set %s to %s\n", pref->cmd, argv[2]);
    return 0;
  }

  float user_val = atof(argv[2]);
  
  if (user_val == 0 && argv[2][0] != '0') {
    fprintf(stderr, "Invalid value: %s\n", argv[2]);
    return 1;
  }
  
  if (pref->min_user != pref->max_user) {
    if (user_val < pref->min_user || user_val > pref->max_user) {
      fprintf(stderr, "Value must be between %.0f and %.0f\n", pref->min_user, pref->max_user);
      return 1;
    }
  }

  float system_val = pref_encode(pref, user_val);
  defaults_write_float(pref->domain, pref->key, system_val);
  
  printf("Set %s to %.1f (system: %.4f)\n", pref->cmd, user_val, system_val);
  return 0;
}
