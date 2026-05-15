# Installation Guide

## Prerequisites

- Flipper Zero device
- Flipper Zero firmware development environment (FBT)
- Basic command line knowledge

## Installation Methods

### Method 1: Using FBT (Recommended)

1. **Clone the repository** into your Flipper firmware applications directory:

```bash
cd path/to/flipperzero-firmware/applications/main
git clone https://github.com/flipper-ust1x/Flipper-zero-better-power-menu.git power_menu
```

2. **Build the application:**

```bash
cd path/to/flipperzero-firmware
./fbt fap_power_menu
```

3. **Install to Flipper:**

```bash
./fbt install_faps
```

### Method 2: Using fpm (Flipper Package Manager)

1. **Install the package:**

```bash
fpm install https://github.com/flipper-ust1x/Flipper-zero-better-power-menu
```

2. **Launch the app** from your Flipper's applications menu under System

### Method 3: Manual Installation

1. **Download the repository** as a ZIP file

2. **Extract to** `applications/main/power_menu/` in your Flipper firmware directory

3. **Build the firmware:**

```bash
cd flipperzero-firmware
./fbt build
```

4. **Flash to device:**

```bash
./fbt flash
```

## Verification

After installation:

1. Reboot your Flipper Zero
2. Go to **Main Menu** → **Apps** → **System**
3. Look for **Power Menu** in the list
4. Open it to verify the application is working

## Building from Source

If you want to make modifications:

```bash
# Build just this app
./fbt fap_power_menu

# Build and install
./fbt fap_power_menu install_faps

# Debug with verbose output
./fbt fap_power_menu -v
```

## Troubleshooting

### Application doesn't appear

- Ensure the directory structure is: `applications/main/power_menu/`
- Check that `application.fam` exists in the directory
- Run `./fbt clean` and rebuild

### Build errors

- Verify your Flipper firmware is up to date
- Check that all required headers are available
- Ensure you have the correct FBT version

### Installation with fpm fails

- Update fpm: `pip install --upgrade fpm-cli`
- Check your internet connection
- Try manual installation method instead

## Uninstallation

To remove the application:

1. **Delete the application directory:**

```bash
rm -rf applications/main/power_menu/
```

2. **Rebuild and flash firmware:**

```bash
./fbt flash
```

Or simply delete the Power Menu app from your Flipper's file manager if installed as a standalone FAP.
