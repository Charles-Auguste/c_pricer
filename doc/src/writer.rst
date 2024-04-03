Output stream
=============

Basic description
-----------------

To ensure a good communication between `C++` language (Efficient but low level) and `Python` (High level for data vizualisation), output data are saved using `.csv` format in a buffer folder.

**For instance** if a user generates Black&Sholes prices for different values of T (maturity). Results are stored in DataFrame-like object (`vector<vector<double>>` for the content and `vector<string>` for the headers). Then using `OutputWriter` class, a lite `.csv` is generated and can be used with `pandas` library in python.


How does it works ?
-------------------

Everything works with `OutputWriter` class. There are two parameters:

- `string` path (The path of the output file, */tmp/test.csv* for instance)

- `char` sep (The separator of the file, [,;|] for example)

In order to save an output file, the `write` method is called with a dataframe like object:

- `vector<string>` header (A vector of header - column names)

- `vector<vector<double>>` content (A matrix of `double` that represents the content of the table)


.. warning::

   The class only works with matrix of `double`. Beware of mixed types when calling `write` method !


**Class specification**

.. literalinclude:: ../../libs/IOlib/include/IOlib/io.h

How to compile and test ?
-------------------------

Like all the other functions and classes, a dedicated subfolder contains the code (`OutputWriter.h` and `OutputWriter.cpp`) and a small toy example to test and understand the library (`main.cpp`). A `CMake` file is also configured to (1) Create a link between this small library and the global pricer project (2) Build a fully functionnal testing code through `main.cpp`.

**Let's try it out !**

*1)* Go to `OutputWriter` subdirectory and run `cmake` command

.. code::

   $ cd OutputWriter
   $ cmake .

.. note::

   cmake is a Makefile generator. When you call cmake [path], you ask it to generate a Makefile in the current directory following instructions given in [path]/CMakeLists.txt


*2)* Generate a executable file and run it

.. code::

   $ make
   $ ./test_output


*3)* A `.csv` file has been created, here is a quick look at its content

.. code::

   $ cat ../io/test.csv
   t|St
   1|10
   2|12.5


A small example
---------------

.. literalinclude:: ../../libs/IOlib/test/main.cpp