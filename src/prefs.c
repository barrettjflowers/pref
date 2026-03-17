#include "prefs.h"
#include <string.h>
#include <stdlib.h>

static float trackpad_to_system(float val)
{
  return val * 0.1f;
}

static float trackpad_from_system(float val)
{
  return val / 0.1f;
}

static float mouse_scaling_to_system(float val)
{
  return val / 4.0f;
}

static float mouse_scaling_from_system(float val)
{
  return val * 4.0f;
}

static float keyrepeat_to_system(float val)
{
  return 120.0f - (val * 20.0f);
}

static float keyrepeat_from_system(float val)
{
  return (120.0f - val) / 20.0f;
}

static float keydelay_to_system(float val)
{
  return 120.0f - (val * 10.0f);
}

static float keydelay_from_system(float val)
{
  return (120.0f - val) / 10.0f;
}

static float dock_tilesize_to_system(float val)
{
  return val * 8.0f + 16.0f;
}

static float dock_tilesize_from_system(float val)
{
  return (val - 16.0f) / 8.0f;
}

static PrefMapping preferences[] = {
  // Finder
  {"finder-show-all-files", "com.apple.finder", "AppleShowAllFiles", PREF_TYPE_BOOL, "Show hidden files in Finder", NULL, NULL, 0, 1, 0},
  {"finder-show-pathbar", "com.apple.finder", "ShowPathbar", PREF_TYPE_BOOL, "Show path bar in Finder", NULL, NULL, 0, 1, 0},
  {"finder-show-tabbar", "com.apple.finder", "ShowTabBar", PREF_TYPE_BOOL, "Show tab bar in Finder", NULL, NULL, 0, 1, 0},
  {"finder-new-window", "com.apple.finder", "NewWindowStyle", PREF_TYPE_INT, "New window style (1=grid, 2=list, 3=column, 4=galley)", NULL, NULL, 1, 4, 1},
  {"finder-default-view", "com.apple.finder", "FXPreferredViewStyle", PREF_TYPE_STRING, "Default view style (icnv, clmv, Flwv, Nlsv)", NULL, NULL, 0, 0, 0},
  
  // Dock
  {"dock-autohide", "com.apple.dock", "autohide", PREF_TYPE_BOOL, "Auto-hide the Dock", NULL, NULL, 0, 1, 0},
  {"dock-magnification", "com.apple.dock", "magnification", PREF_TYPE_BOOL, "Enable Dock magnification", NULL, NULL, 0, 1, 0},
  {"docksize", "com.apple.dock", "tilesize", PREF_TYPE_INT, "Dock icon size (1-16)", dock_tilesize_to_system, dock_tilesize_from_system, 1, 16, 8},
  {"dock-position", "com.apple.dock", "orientation", PREF_TYPE_STRING, "Dock position (left, bottom, right)", NULL, NULL, 0, 0, 0},
  {"dock-enable-anim", "com.apple.dock", "enable-spring-load", PREF_TYPE_BOOL, "Enable spring loading", NULL, NULL, 0, 1, 1},
  {"dock-show-indicator", "com.apple.dock", "show-indicator", PREF_TYPE_BOOL, "Show app indicator lights", NULL, NULL, 0, 1, 1},
  {"dock-minimize-to-app", "com.apple.dock", "minimize-to-application", PREF_TYPE_BOOL, "Minimize windows to app icon", NULL, NULL, 0, 1, 1},
  
  // Trackpad
  {"trackpad-speed", "com.apple.trackpad", "Speed", PREF_TYPE_FLOAT, "Trackpad cursor speed (0-10)", trackpad_to_system, trackpad_from_system, 0, 10, 5},
  {"trackpad-click", "com.apple.trackpad", "Clicking", PREF_TYPE_BOOL, "Tap to click", NULL, NULL, 0, 1, 0},
  {"trackpad-tracking", "com.apple.trackpad", "TrackpadSpeed", PREF_TYPE_FLOAT, "Trackpad tracking speed (0-10)", trackpad_to_system, trackpad_from_system, 0, 10, 5},
  {"trackpad-scroll", "com.apple.preference.trackpad", "NSNaturalScrollDirection", PREF_TYPE_INT, "Natural scrolling (0=off, 1=on)", NULL, NULL, 0, 1, 1},
  
  // Mouse
  {"mouse-speed", "com.apple.mouse", "MouseSpeed", PREF_TYPE_FLOAT, "Mouse cursor speed (0-10)", trackpad_to_system, trackpad_from_system, 0, 10, 5},
  {"mouse-scaling", "-g", "com.apple.mouse.scaling", PREF_TYPE_FLOAT, "Mouse acceleration (1-10)", mouse_scaling_to_system, mouse_scaling_from_system, 1, 10, 3},
  {"mouse-scroll", "com.apple.mouse", "MomentumScroll", PREF_TYPE_BOOL, "Momentum scrolling", NULL, NULL, 0, 1, 1},
  
  // Keyboard
  {"key-repeat", "-g", "KeyRepeat", PREF_TYPE_INT, "Key repeat rate (1-10, 10=fastest)", keyrepeat_to_system, keyrepeat_from_system, 1, 10, 6},
  {"key-delay", "-g", "InitialKeyRepeat", PREF_TYPE_INT, "Key repeat delay (1-10, 10=shortest)", keydelay_to_system, keydelay_from_system, 1, 10, 6},
  {"fn-mode", "-g", "fnState", PREF_TYPE_BOOL, "Use Fn key", NULL, NULL, 0, 1, 0},
  
  // Menu bar
  {"menubar-clock", "com.apple.menuextra.clock", "DateFormat", PREF_TYPE_STRING, "Menu bar date format", NULL, NULL, 0, 0, 0},
  {"menubar-battery", "com.apple.controlcenter", "BatteryShowPercent", PREF_TYPE_BOOL, "Show battery percentage", NULL, NULL, 0, 1, 0},
  
  // Spotlight
  {"spotlight-enabled", "com.apple.spotlight", "enabled", PREF_TYPE_BOOL, "Enable Spotlight", NULL, NULL, 0, 1, 1},
  
  // Safari
  {"safari-homepage", "com.apple.Safari", "HomePage", PREF_TYPE_STRING, "Homepage URL", NULL, NULL, 0, 0, 0},
  {"safari-show-favorites", "com.apple.Safari", "ShowFavoritesBar", PREF_TYPE_BOOL, "Show favorites bar", NULL, NULL, 0, 1, 1},
  
  // Terminal
  {"terminal-shell", "com.apple.Terminal", "Shell", PREF_TYPE_STRING, "Default shell", NULL, NULL, 0, 0, 0},
};

int prefs_count(void)
{
  return sizeof(preferences) / sizeof(preferences[0]);
}

const PrefMapping *prefs_all(void)
{
  return preferences;
}

PrefMapping *prefs_find(const char *cmd)
{
  int count = prefs_count();
  for (int i = 0; i < count; i++) {
    if (strcmp(preferences[i].cmd, cmd) == 0) {
      return &preferences[i];
    }
  }
  return NULL;
}

float pref_encode(const PrefMapping *pref, float user_val)
{
  if (pref->to_system) {
    return pref->to_system(user_val);
  }
  return user_val;
}

float pref_decode(const PrefMapping *pref, float system_val)
{
  if (pref->from_system) {
    return pref->from_system(system_val);
  }
  return system_val;
}

void pref_print(FILE *f, const PrefMapping *pref)
{
  fprintf(f, "  %-25s %s", pref->cmd, pref->description);
  if (pref->min_user != pref->max_user) {
    fprintf(f, " (%.0f-%.0f)", pref->min_user, pref->max_user);
  }
  fprintf(f, "\n");
}
