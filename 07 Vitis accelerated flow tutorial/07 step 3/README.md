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

Rename `u-boot-dtb.elf` to `u-boot.elf` so it will end up like this.

![missing file](images/03_010.png)

Copy the following files from `~/workspace/petalinux/myir/images/linux` into `~/workspace/vitis/sd_card`:
* boot.scr
* image.ub

![missing file](images/03_020.png)

### Creating the Vitis platform
Start Vitis either with the desktop shortcut or via the terminal.
Click on **File->New->Platform** and enter the name for the platform, I chose myir_plat.

![missing file](images/03_030.png)

Next, browse for the XSA file, navigate to the `~/workspace/vivado/myir` folder and select it.

![missing file](images/03_040.png)

Next change the operating system to Linux, leave the other options and untick the boot file generation as we already have them created with Petalinux

![missing file](images/03_050.png)

In the platform settings the paths for the FSBL and PMU fw need to be entered, use those in the `vitis\boot` folder where we copied them from the Petalinux project.

![missing file](images/03_060.png)

Next the linux domain has to be configured as follows:
- For the **bif file**, click on the arrow and select **Generate bif file**
- For the **boot components directory**, select the `vitis/boot` folder created
- For the **linux rootfs**, browse to the `rootfs.ext4` file in the `images/linux` folder in the petalinux project
- For the **FAT32 partition directory**, select the sd_card folder we created
- For the **Sysroot directory**, select the `~/workspace/vitis/boot/sdk/sysroots/cortexa72-cortexa53-xilinx-linux` folder

The remaining paths below can be left as they are. 

![missing file](images/03_070.png)

Build the platform project.

### Create the Vitis application project

In the Vitis GUI, click on **File->New->Application project**

The previously created platform should be selected for us:

![missing file](images/03_080.png)

Now choose the application name, I go for app_myir.

![missing file](images/03_090.png)

There should be nothing to do next, just checking the domain is linux, and the sysroot, rootfs file and kernel are the right ones

![missing file](images/03_100.png)

Finally, let's choose a template application, the vector addition. 

![missing file](images/03_110.png)

Click **Finish** and wait for the projects to appear. Then change the build mode to **Hardware** in the build icon dropdown menu:

![missing file](images/03_120.png)

Then build all the projects. It takes a while.

### Prepare the boot SD card

After the project is built a series of files appear in the `Hardware/package` folder under the system project:

![missing file](images/03_130.png)

The most important here is the `sd_card.img`, that contains the boot sd card image. Navigate to `app_myir_system/Hardware/package` and open a terminal there

![missing file](images/03_140.png)

Insert an SD card in the PC, make sure it's connected to the Linux VM if you use one and type the following command.

**NOTE**: You should check first that your SD card is sdb, it may be a different letter (sdc, sdd, etc.)

```
sudo dd if=sd_card.img of=/dev/sdb
```

