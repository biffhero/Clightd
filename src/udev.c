#include "../inc/udev.h"

static void get_first_matching_device(struct udev_device **dev, const char *subsystem);

/**
 * Set dev to first device in subsystem
 */
static void get_first_matching_device(struct udev_device **dev, const char *subsystem) {
    struct udev_enumerate *enumerate;
    struct udev_list_entry *devices;
    
    enumerate = udev_enumerate_new(udev);
    udev_enumerate_add_match_subsystem(enumerate, subsystem);
    udev_enumerate_scan_devices(enumerate);
    devices = udev_enumerate_get_list_entry(enumerate);
    if (devices) {
        const char *path;
        path = udev_list_entry_get_name(devices);
        *dev = udev_device_new_from_syspath(udev, path);
    }
    /* Free the enumerator object */
    udev_enumerate_unref(enumerate);
}

void get_udev_device(const char *backlight_interface, const char *subsystem,
                            sd_bus_error **ret_error, struct udev_device **dev) {
    // if no backlight_interface is specified, try to get first matching device
    if (!backlight_interface || !strlen(backlight_interface)) {
        get_first_matching_device(dev, subsystem);
    } else {
        *dev = udev_device_new_from_subsystem_sysname(udev, subsystem, backlight_interface);
    }
    if (!(*dev)) {
        sd_bus_error_set_errno(*ret_error, ENODEV);
    }
}
