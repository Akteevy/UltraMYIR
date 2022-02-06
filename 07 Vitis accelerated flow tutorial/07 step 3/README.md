## Vitis accelerated flow application with the MYIR board
This is the third and last step of this tutorial. So for you have created the Vivado platform and bitstream in step 1 and the Linux elements (FSBL, kernel, image, root filesystem).

In this step we will create the Vitis platform, and build a Vitis application example that will run on the board.
```
Note: Xilinx tools 2021.2 have a bug and will fail if the year is 2022...
I fixed that by setting my VM not to update the date from the host and changing it to 2021
```
### Setting boot and sd card folders
In the `~/workspace/vitis` folder, create two folders, one for the boot files and root filesystem and another for the sd card:
```
cd ~/workspace/vitis
mkdir boot
mkdir sd_card
```
Now the sdk created in step 2 is to be expanded into the boot folder:
```
cd ~/workspace/petalinux/myir/images/linux/
./sdk.sh -y -d ~/workspace/vitis/boot/sdk
```
Copy the following files from `~/workspace/petalinux/myir/images/linux` into `~/workspace/vitis/boot`:
* bl31.elf
* pmufw.elf
* system.dtb
* u-boot-dtb.elf
* zynqmp_fsbl.elf

Rename `u-boot-dtb.elf` to `u-boot.elf`

![missing file](images/03_010.png)

### Launching Vitis
Start Vitis either with the desktop shortcut or via the terminal.
