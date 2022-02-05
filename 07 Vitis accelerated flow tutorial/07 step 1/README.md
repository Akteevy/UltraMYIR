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
    
    In the created block diagram, click on the plus sign and add a 
    
    ![missing image](images/01_041.png)
    
    
