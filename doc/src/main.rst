Project purpose
================

The aim of the project is to implement an option pricer in C++. More specifically, `c_pricer` is a pricing and GSE library comprising:

- Several models (Black & Scholes, Dupire, Heston)
- Two path simulators (Euler & Brodie Kaya)
- Several pricing possibilities (American & European call/put, etc...)
- A calibration protocol


Install
========

As the development of the project straddles several platforms, the installation method
uses the `CMake` technology. `Cmake` is available on Micorsoft Windows, MacOsX and
Linux. Moreover, a `CMake` project is compatible with `Microsoft Visual Studio`, `Visual
Studio Code` as well as a simple `Terminal` (Best solution !).

.. note::

  The installation process will be detailed for 2 architectures: The first is linked to a Windows computer using the Microsoft Visual Studio platform. The second is on a Linux system using Visual Studio code and a terminal.

  .. image:: /_static/images/vs.png
     :width: 400



1) Install a compiler and cmake (Linux only)
--------------------------------------------

On a `terminal` window, run 

.. code:: bash

  $ sudo apt-get upgrade -y
  $ sudo apt-get install build-essential cmake wget

With `CMake` and a compiler, you are now ready to code ! A quick check to ensure that
everything works :

.. code:: bash

  $ cmake --version
  cmake version 3.28.3

  CMake suite maintained and supported by Kitware (kitware.com/cmake).

  $ gcc --version
  gcc (Ubuntu 13.2.0-23ubuntu4) 13.2.0
  Copyright (C) 2023 Free Software Foundation, Inc.
  This is free software; see the source for copying conditions.  There is NO
  warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

2) Install `Eigen`
------------------

Eigen is a C++ template library for linear algebra: matrices, vectors, numerical solvers, and related algorithms.

.. tab:: On Linux

  **A** - Download the library

  .. code:: bash
      
      $ wget https://gitlab.com/libeigen/eigen/-/archive/3.4.0/eigen-3.4.0.tar.gz

      --2024-05-12 22:50:07--  https://gitlab.com/libeigen/eigen/-/archive/3.4.0/eigen-3.4.0.tar.gz
      Resolving gitlab.com (gitlab.com)... 2606:4700:90:0:f22e:fbec:5bed:a9b9, 172.65.251.78
      Connecting to gitlab.com (gitlab.com)|2606:4700:90:0:f22e:fbec:5bed:a9b9|:443... connected.
      HTTP request sent, awaiting response... 200 OK
      Length: unspecified [application/octet-stream]
      Saving to: ‘eigen-3.4.0.tar.gz’

      2024-05-12 22:50:08 (14.8 MB/s) - ‘eigen-3.4.0.tar.gz’ saved [2705005]


  **B** - Extract the archive

  .. code:: bash

    $ tar -xf eigen-3.4.0.tar.gz
    $ rm eigen-3.4.0.tar.gz
    $ cd eigen-3.4.0


  **C** - Move the library in the right place

  .. code:: bash

    $ sudo cp -r Eigen/ /usr/local/include/.

  .. note::

    In most linux systems, C++ libraries are located in `/usr/local`. `include` folder
    stands for header files (Like the one we have just copied). As for `bin` or `lib`
    folders, they stored binaries/executables of every libraries (For instance...
    `c_pricer.a`)

.. tab:: Windows

  Simple ...


.. note::

  .. image:: /_static/images/eigen.png
    :width: 50

  One can find more information on `Eigen` library `here <https://eigen.tuxfamily.org/index.php?title=Main_Page>`__







Architecture
=============

.. code:: bash

  $ tree
  .
  ├── CMakeLists.txt 
  ├── Makefile
  ├── main.cpp
  ├── MonteCarloEngine
  │   ├── MonteCarlo.cpp
  │   ├── MonteCarlo.h
  │   └── test.hpp
  └── src
      ├── Calibration
      ├── ImpliedVolatilitySurface
      ├── Model
      ├── PathSimulator
      ├── Pricing
      └── ThomasSolver


Dev tool kit
============



Using the Library
=================



