.. _MaestroZephyrREADME:
.. Customer's Point of view who wants to try Maestro out without extending it

#################
Maestro on Zephyr
#################
* Based on and tested with Zephyr version, given by tag v4.0.0
* Tested with Zephyr SDK version 16.4
* To see the pre-built documentation, see: `README.html <doc/doc/README.html>`_. Also see the :ref:`documentation section <documentation>`.

.. toctree::
   :glob:
   :maxdepth: 2
   :hidden:

   self
   Record sample <./samples/record_ram/README>
   VIT sample <./samples/record_vit/README>
   Decoder sample <./tests/decoder/README>
   Encoder sample <./tests/encoder/README>
   Mem2mem sample <./tests/memToMem/README>

.. contents::
   :local:
   :depth: 2


**************************
Maestro environment setup
**************************

Follow these steps to set up a Maestro development environment on your machine.

#. If you haven't already, please follow `this guide <https://docs.zephyrproject.org/latest/develop/getting_started/index.html>`_ to set up a Zephyr development environment and its dependencies first:

        * Cmake
        * Python
        * Devicetree compiler
        * West
        * Zephyr SDK bundle

#. Get Maestro. You can pick either of the options listed below. If you need help deciding which option is the best fit for your needs, please see the :ref:`FAQ <faq>`.

    * Freestanding Maestro - This option pulls in only Maestro's necessary dependencies.

         Run::

            1. west init -m <maestro repository url> --mr <revision> --mf west-freestanding.yml <foldername>
            2. cd <foldername>
            3. west update

    * Maestro as a Zephyr module

        To include Maestro into Zephyr, update Zephyr's ``west.yml`` file::

            projects:
            name: maestro
            url: <maestro repository url>
            revision: <revision with Zephyr support>
            path: modules/audio/maestro
            import: west.yml

        Then run ``west update maestro`` command.

*****************************
Build and run Maestro example
*****************************

These steps will guide you through building and running Maestro samples.
You can use either the command line utilizing Zephyr's powerful ``west`` tool or you can use VS Code's GUI.
Detailed steps for both options are listed below.

==================
Using command line
==================

See Zephyr's `Building, Flashing and Debugging <https://docs.zephyrproject.org/latest/develop/west/build-flash-debug.html>`_ guide if you aren't familiar with it yet.

#. To **build** a project, run:

    ::

        west build -b <board> -d <output build directory> <path to example> -p

    For example, this compiles VIT example for rd_rw612_bga board::

        1. cd maestro/zephyr
        2. west build -b rd_rw612_bga -d build samples/vit -p

#. To **run** a project, run:

    ::

        west flash -d <directory>

    e.g.::

        west flash -d build

#. To **debug** a project, run:

    ::

        west debug -d <directory>

    e.g.::

        west debug -d build

============================
Using MCUXpresso for VS Code
============================
For this you have to have NXP's `MCUXpresso for VS Code extension <https://github.com/nxp-mcuxpresso/vscode-for-mcux>`_ installed.

#. Import your topdir as a repository to MCUXPresso for VS Code:

    * Open the MCUXpresso Extension. In the *Quickstart Panel* click *Import Repository*.
    * In the displayed menu click *LOCAL* tab and select the folder location of your *topdir*.
    * Click *Import*.
    * The repository is successfully added to the *Installed Repositories* view once the import is successful.

#. To import any project from the imported repository:

    * In the *Quickstart Panel* click *Import Example from Repository*.
    * For **Repository** select *your imported* repository.
    * For **Zephyr SDK** the installed Zephyr SDK is selected automatically. If not, select one.
    * For **Board** select your board (*make sure* you've selected the *correct* revision).
    * For **Template** select the folder path to your project.
    * Click the *Create* button.

#. Build the project by clicking the *Build Selected* icon (displayed on hover) in the extension's *Projects* view. After the build, the debug console window displays the memory usage (or compiler errors if any).

#. Debug the project by clicking the *Debug* (play) icon (displayed on hover) in the extension's *Projects* view.

#. The execution will pause. To continue execution click *Continue* on the debug options.

#. In the *SERIAL MONITOR* tab of your console panel, the application prints the Zephyr boot banner during startup and then prints the test results.

****************
Folder structure
****************

::

    maestro/
    ├─── ...
    └─── zephyr/                All Zephyr related files
        ├── samples/            Sample examples
        ├── tests/              Tests
        ├── audioTracks/        Audio tracks for testing
        ├── doc/                Documentation configuration for Sphinx
        ├── wrappers/           NXP SDK Wrappers
        ├── scripts/            Helper scripts, mostly for testing
        ├── module.yml          Defines module name, Cmake and Kconfig locations
        ├── CMakeList.txt       Defines module's build process
        ├── Kconfig             Defines module's configuration
        ├── osa/                Deprecated. OSA port for Zephyr
        └── ...

********************************
Supported elements and libraries
********************************

Here is the list of all features currently supported in Maestro on Zephyr.
Our goal is to support all features in Maestro on Zephyr that are already supported in Maestro on NXP's SDK and to extend them further.

**Supported elements:**

    * Memory source
    * Memory sink
    * Audio source
    * Audio sink
    * Process sink
    * Decoder
    * Encoder

**Supported decoders:**

    * WAV
    * MP3
    * FLAC
    * OPUS OGG
    * AAC

**Supported encoders:**

    * OPUS RAW

**Supported libraries:**

    * `VIT <https://www.nxp.com/design/design-center/software/embedded-software/voice-intelligent-technology-wake-word-and-voice-command-engines:VOICE-INTELLIGENT-TECHNOLOGY>`_

****************
Examples support
****************

All included examples use UART as output.
Examples are located in ``zephyr/tests`` and ``zephyr/samples`` directories.

**List of included examples:**

    * :ref:`Maestro_Sample_record_ram`
    * :ref:`Maestro_Sample_record_vit`
    * :ref:`Maestro_Sample_encoder`
    * :ref:`Maestro_Sample_decoder`
    * :ref:`Maestro_Sample_memtomem`

**Examples support for specific boards:**

==========================================  =============================    ====================    ====================    ====================
Example                                     RDRW612BGA                       LPCxpresso55s69         MIMXRT1060EVKB          MIMXRT1170EVKB
==========================================  =============================    ====================    ====================    ====================
:ref:`Record <Maestro_Sample_record_ram>`   YES                              TO BE TESTED            TO BE TESTED            TO BE TESTED
:ref:`VIT <Maestro_Sample_record_vit>`      YES                              TO BE TESTED            TO BE TESTED            TO BE TESTED
:ref:`Encoder <Maestro_Sample_encoder>`     In progress: OPUS RAW            TO BE TESTED            TO BE TESTED            TO BE TESTED
:ref:`Decoder <Maestro_Sample_decoder>`     YES - WAV, FLAC, OPUS OGG        TO BE TESTED            TO BE TESTED            TO BE TESTED
:ref:`Mem2mem <Maestro_Sample_memtomem>`    YES                              TO BE TESTED            TO BE TESTED            TO BE TESTED
==========================================  =============================    ====================    ====================    ====================

*************************
Creating your own example
*************************

There are two ways to create your own example - you can either one of the included examples as a reference or you can create your own example from scratch by hand.

When creating your own example from scratch, set ``CONFIG_MAESTRO_AUDIO_FRAMEWORK=y`` in your ``prj.conf`` file. Then you can start enabling specific elements by setting ``CONFIG_MAESTRO_ELEMENT_<NAME>_ENABLE=y``.

However, the recommended way to edit config options is to open gui-config (or menuconfig) by calling ``west build -t guiconfig``.
Then you can use the graphical interface to interactively turn on/off the features you need.

.. _documentation:

**************
Documentation
**************

Please note, Maestro documentation is under reconstruction. It is currently mixing several tools and formats.

To see the pre-generated Maestro Zephyr documentation, see ``zephyr/doc/doc/README.html``

To generate the Zephyr documentation, go under ``zephyr/doc`` folder and execute ``make html``. Sphinx version ``sphinx-build 8.1.3`` must be installed. Open ``doc/doc/html/README.hml`` afterwards.

To see Maestro core documentation, go to the Maestro top directory and see ``README.md``.

.. _faq:

***
FAQ
***

#. Should I choose the freestanding version of Maestro or should integrate it into my west instead?

    * Freestanding version of Maestro pulls in all the dependencies it needs including Zephyr itself.
    * Integrating it as a module is easier if you already have your Zephyr environment set up.