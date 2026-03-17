# pref

macOS has long lacked portable system configuration.  
This project aims to make macOS system preferences more Unix-like and reproducible.

`pref` is a wrapper around the [`defaults`](https://ss64.com/mac/defaults.html) command that provides portable, deterministic system preference configuration.

---

## Configuration

Easily configure macOS preferences from the command line:

```bash
pref docksize 8
```

This avoids the complexity of the `defaults` command and removes the need to use the System Settings UI.

You can also configure preferences using a `.prefrc`:

```bash
prefs=(
  "dock-autohide 1"
  "dock-magnification 0"
  "docksize 8"
  "dock-position right"
  "dock-enable-anim 1"
  "dock-show-indicator 1"
)
```

Then apply the configuration:

```bash
for p in "${prefs[@]}"; do
  ./pref $p
done
```

---

## Contribute

macOS system preferences are constantly evolving.  
Community contributions help maintain support as macOS setting changes and additions over time.
