# README Deprecated! Needs to be updated

# Steps to build and flash/debug examples (tests) on a board:
0. Choose or create a directory on your system and clone the repository (via your preferred method) into it. This directory will become the top level directory of your west workspace - _topdir_

    Your workspace now looks like this:
    ```
    topdir/                        # west topdir
    ├── mcu-streamer/              # .git/ repo
    │   ├── zephyr/
    │   │   ├── osa/               # OSA tests project
    │   │   └── [... other files ...]
    │   └── [... other files ...]
    │
    └── [ ... other projects ...]
    ```

## Command Line Interface

1. Initialize a west workspace in your _topdir_ by running the command

    ```
    west init -l mcu-streamer
    ```

2. Update your workspace by `west update`

    Your workspace now looks like this:
    ```
    topdir/                        # west topdir
    ├── .west/                     # marks the location of the topdir
    │   └── config                 # per-workspace local configuration file
    │
    ├── mcu-streamer/              # .git/ repo
    │   ├── zephyr/
    │   │   ├── osa/               # OSA tests project
    │   │   └── [... other files ...]
    │   └── [... other files ...]
    │
    ├── zephyr/                    # .git/ repo
    │   ├── west.yml               # manifest file
    │   └── [... other files ...]
    │
    ├── modules/
    └── [ ... other projects ...]
    ```

3. Move to the directory containing the test projects

    ```
    cd .\mcu-streamer\zephyr\tests\
    ```

4. To build a project run:

    ```
    west build [-b BOARD] [-d BUILD_DIR] [source_dir]
    ```

    _For example_, to build OSA layer tests for i.MX RT1060 EVKB, run:

    ```
    west build -b mimxrt1060_evkb -d .\osa\build\ .\osa\
    ```

5. To flash the project to your board, run:

    ```
    west flash [-d DIR] [-r RUNNER]
    ```

    e.g.

    ```
    west flash -d .\osa\build\ -r linkserver
    ```

    **OR** to debug the project in command line, run:

    ```
    west debug -d [-d DIR] [-r RUNNER]
    ```

    e.g.

    ```
    west debug -d .\osa\build\ -r linkserver
    ```

6. In the `SERIAL MONITOR` tab of your console panel, the application prints the Zephyr boot banner during startup and then prints the test results.

## MCUXpresso for VS Code

1. Import your topdir as a repository to MCUXPresso for VS Code:
    * Open the MCUXpresso Extension. In the _Quickstart Panel_ click `Import Repository`.
    * In the displayed menu click `LOCAL` tab and select the folder location of your _topdir_.
    * Click `Import`.
    * The repository is successfully added to the `Installed Repositories` view once the import is successful.

2. To import any project from the imported repository:
    * In the _Quickstart Panel_ click `Import Example from Repository`.
    * For **Repository** select _your imported_ repository.
    * For **Zephyr SDK** the installed Zephyr SDK is selected automatically. If not, select one.
    * For **Board** select your board (_make sure_ you've selected the _correct_ revision).
    * For **Template** select the folder path to your project. _**Tests are located in `mcu-streamer/zephyr/tests/` !**_
    * Click the `Create` button.

3. Build the project by clicking the `Build Selected` icon (displayed on hover) in the extension's _Projects_ view.
After the build, the debug console window displays the memory usage (or compiler errors if any).

4. Debug the project by clicking the `Debug` (play) icon (displayed on hover) in the extension's _Projects_ view.

5. The execution will pause. To continue execution click `Continue` on the debug options.

6. In the `SERIAL MONITOR` tab of your console panel, the application prints the Zephyr boot banner during startup and then prints the test results.