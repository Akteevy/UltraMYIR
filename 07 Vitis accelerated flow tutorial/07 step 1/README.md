## Vivado Platform creation for Vitis accelerated flow
After this tutorial step you will have created the XSA file for the MYIR board.

The starting point is a Ubuntu 20.04 machine (real or virtual) with Xilinx tools release 2021.2 installed.

Your folder structure may vary, mine is:

```
~
|-- workspace
        |-- petalinux
        |-- vitis
        |-- vivado
```

And inside of these three folders we will generate project folders for each step of this tutorial. Other people prefer a single project folder with subfolders for each tool. Is just a different arrangement.

Let's start.

1. Start Vivado and create project

    Run vivado either on the terminal (in ~/workspace/vivado) or with the desktop shortcut.

    Click on **Create Project**
    
    Type **myir** as the project name and tick the option **Create project subdirectory**
    
    ![missing image](images/01_010.png)
    
    Click **Next**. Select an RTL project and tick the **Project is an extensible Vitis platform** option
    
    ![missing image](images/01_020.png)
    
    Click **Next**. Now select the board. 
    
    ![missing image](images/01_030.png)
    
    If you don't have the MYIR board installed, just select the part (xczu3eg-sfvc784-1-e)
    
    ![missing image](images/01_035.png)
    
    Click **Finish**. A new blank project should be created.

2. Create and populate the block diagram
    
    In Vivado Flow Navigator, click on **Create Block Design** and name it **bd** 
    
    ![missing image](images/01_040.png)
    
    In the created block diagram, click on the plus sign and add a Zynq Ultrascale+ MPSoC core
    
    ![missing image](images/01_041.png)
    
    Then double click on it to configure. Click on **Presets > Apply Configuration**
    
    ![missing image](images/01_042.png)
    
    Browse for the configuration file (umyir_ps_config.tcl) that is included in this repository.
    
    ![missing image](images/01_043.png)
    
    After the preset is applied, in the **Clock configuration** tab, select **Output clocks** and disable PL1, leave only PL0 at 100 MHz.
    
    ![missing image](images/01_047.png)
    
    In the **PS-PL Configuration** tab, disable all Master and Slave AXI ports
    
    ![missing image](images/01_048.png)
    
    Click **OK**. Then add the following IPs:
    
    - Clocking wizard. Configure it for three outputs at 100, 200 and 400 MHz. Change the reset input to active low. Leave the locked output.
    - Concat. Change it to one input port.
    - Processor System Reset (x 3)

    Connect them as shown below.
    
    ![missing image](images/01_050.png)
    
    Next is to configure the platform. In the **Platform Setup** tab in the block diagram, select the AXI ports as in the picture. Type names for the slave ports.
    
    ![missing image](images/01_060.png)
    
    Now select all three clocks from the wizard to be used by vitis and select the 200 MHz clock as default.
    
    ![missing image](images/01_070.png)
    
    Select interrupts 0 to 7 and enable them to be used by Vitis.
    
    ![missing image](images/01_080.png)
    
    Finally add the platform details.
    
    ![missing image](images/01_090.png)
    
    Now save the platform and click on **Generate Block Design** on the Flow Navigator. Select **Global** as Synthesis option and click **Generate**
    
    ![missing image](images/01_100.png)
    
    Once it's done, right click on the block diagram icon in the Sources panel and select **Create HDL wrapper**. Select the option **Let Vivado manage wrapper and auto-update** and click **OK**.
    
    ![missing image](images/01_105.png)
    
    Now click on **Generate Bitstream** on the Flow Navigator panel.
    
    When the bitstream is created, click on **Export Platform** and select the **Hardware** option.
    
    ![missing image](images/01_110.png)
    
    Next, select the **Pre-Synthesis** platform state and make sure the **Include bitstream** box is ticked.
    
    ![missing image](images/01_120.png)
    
    Next, review the platform details, name, version, etc.
    
    ![missing image](images/01_130.png)
    
    Now choose a name and directory for the XSA file to create. Here I chose **myir_platform** and the ~/workspace/vivado/myir folder
    
    ![missing image](images/01_140.png)

    Click **Finish**. After it's done, the XSA file will appear in the myir project folder.
    
    ![missing image](images/01_150.png)
    
We have now completed the first step. Copy the myir_platform.xsa file to the workspace/petalinux folder to use it in [step 2](../07 step 2/)
