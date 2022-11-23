.. _arc_stuff:

ARC instrumentation stuff
###########

Overview
********

ARC instrumntation stuff

Building and Running
********************

This application can be built and executed on QEMU as follows:

.. zephyr-app-commands::
   :zephyr-app: samples/hello_world
   :host-os: unix
   :board: nsim_hs
   :goals: run
   :compact:

To build for another board, change "nsim_hs" above to that board's name.

Sample Output
=============

.. code-block:: console

    Hello World! x86

