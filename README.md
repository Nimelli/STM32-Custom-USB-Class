# STM32 Custom USB Class

This in an example of a custom USB class using ST USB Device library. 
Current implementation is for a NUCLEO-F401RE, but it can be used as a template for implementing USB communication for your STM32 project.

As it is, this simple application implement a USB device with:
- 1 single configuration
- 1 single interface (Vendor specific class)
- 2 endpoints:
  - 1 OUT endpoint: bulk transfert
  - 1 IN endpoint: bulk transfert


## Structure

File structure originate from CubeMx generation. All USB related files have been moved into the *USB* folder.

Most code related to the custom class implementation are in:
- USB/Class/Custom/*/usbd_custom_class.c/h: defining the class descriptor (config, interface, endpoints) and dealing with the mid-level ST USB device library.
- USB/App/usbd_custom_class_it.c/h: Interface for the custom class (interface, callbacks, higher-level functions).

## Interacting with the device
The code is currently echoing what it receives. (see usbd_custom_class_it.c). What you send to the OUT endpoint will be forwarded and can be then received on the IN endpoint.

### On Windows
As it's a custom class, your computer will not know how to interact with it, you need a device driver. You can use https://zadig.akeo.ie/ to load a generic USB driver for the device.

I had no chance in making pyusb with *libusb1-0* as backend and device driver to communicate with my device. The full USB descriptor could be read, but I couldn't make pyusb talk to my device. *(Errno 2: Entity not found.)*. The issue might come from my USB-C hub, but at the time of writing, I can not test without it. (nothing else than usb-c port on my laptop...)

Hopefully it worked with the windows generic *WinUSB* driver (registered with Zadig). I had to use https://github.com/felHR85/WinUsbPy with python to communicate with my device. Some modifications of the winUsbPY library were needed, but then it worked perfectly.

### On other platform
You may have better success with libusb on MAC or linux.





