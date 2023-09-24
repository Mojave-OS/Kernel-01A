<div style="text-align: center; display: flex; align-items: center;">
    <img src="../documentation/images/mojave-logo-ghub.png">
    <p style="padding: 10px; font-size: 2em;">
        Mojave OS: Bootloader
    </p>
</div>

#

The mojave bootloader is responsible for the initial hardware set-up for the BCM2711 chip. These are the current responsibilities for the Mojave OS Bootloader:

1. Ensure that the kernel will begin execution in the main core of the Cortex-A72.
2. Initialization of the stack.
3. Load the kernel.

Main functionality is found in the [boot.S](./boot.S) file in this directory! 

## Note(s):

* The Mojave OS Bootloader does not take it upon itself to do major hardware setup. This is left to the kernel to be done in "C". This is a decision by the kernel design team because of the close integration with the HAL (Hardware Abstraction Layer) that we are going to provide. 

* Unlike x86 Bootloaders, ARMv8 (and presumably other ARM versions) do not require major hardware set-up like turning on the A20 line, enabling paging, starting in 64bit mode, etc. We can hop straight into the kernel and do the changes from there.

    - I could be wrong, and if so, I will gladly update this section! 

```
If you have any questions or concerns, let us know!
```