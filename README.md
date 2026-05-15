# Power Menu - Flipper Zero Application

A comprehensive power management menu for Flipper Zero that provides quick access to device power controls.

## Features

- **Restart**: Soft reset the device using system reset
- **Restart in DFU**: Boot into bootloader/DFU mode for firmware updates
- **Shutdown**: Power off the device completely
- **Confirmation Dialogs**: Each action requires user confirmation to prevent accidental activation
- **Clean UI**: Intuitive menu interface with visual selection indicators

## Usage

1. **Navigate Menu**: Use UP/DOWN buttons to select an option
2. **Select Option**: Press OK to enter confirmation dialog
3. **Confirm Action**: Press RIGHT or OK to confirm, LEFT or BACK to cancel
4. **Exit App**: Press BACK from the main menu

## Controls

### Main Menu
- **UP/DOWN**: Navigate between options
- **OK**: Select option and show confirmation
- **BACK**: Exit application

### Confirmation Dialog
- **LEFT/BACK**: Cancel and return to menu
- **RIGHT/OK**: Confirm and execute action

## Installation

See [INSTALL.md](INSTALL.md) for detailed installation instructions.

## Technical Details

### Power Functions Used

- `furi_hal_power_reset()` - System reset (restart)
- `furi_hal_bootloader_go_to_dfu_mode()` - Enter DFU mode
- `furi_hal_power_off()` - Power off device

### Application State Machine

```
Menu
 ├─ Restart → Confirm Restart
 ├─ Restart in DFU → Confirm Restart DFU
 └─ Shutdown → Confirm Shutdown
```

## Files

- `power_menu.c` - Main application implementation
- `power_menu.h` - Header file with function declarations
- `application.fam` - Flipper application manifest
- `README.md` - This file
- `INSTALL.md` - Installation instructions
- `CONTRIBUTING.md` - Contribution guidelines

## License

MIT License - See LICENSE file for details

## Building

```bash
# Build the FAP (Flipper Application Package)
fpm build power_menu

# Install to Flipper
fpm install power_menu
```

## Troubleshooting

**App not appearing in menu:**
- Ensure the file is in the correct directory: `applications/main/power_menu/`
- Rebuild the firmware or run `fpm build`

**Buttons not responding:**
- Check that your Flipper's input HAL is working correctly
- Try restarting the device

**DFU mode not working:**
- Ensure your Flipper firmware supports DFU mode
- Check bootloader configuration

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines on how to contribute to this project.
