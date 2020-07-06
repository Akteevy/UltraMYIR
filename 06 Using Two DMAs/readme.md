Using Two DMAs
==============

Sometimes we will have more than one DMA engine that need to be synchronized. Here I show how to do that with a simple project where one DMA moves data from MM to AXI-Stream, goes to an AXI-Strean FIFO and the output stream data is read by another DMA and stored back in memory.
