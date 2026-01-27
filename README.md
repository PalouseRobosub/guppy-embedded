<p align="center">
    <img src="./guppy.png" width="400">
</p>
<h1 align="center">Guppy Embedded!</h1>
<p align="center">
    <img alt="GitHub last commit" src="https://img.shields.io/github/last-commit/palouserobosub/guppy-embedded">
</p>

Home to the embedded PCB code that runs Guppy: [Palouse RoboSub](https://robosub.eecs.wsu.edu/)'s 2026 AUV for the international RoboSub competition. This code runs on the [custom circuit boards](https://github.com/PalouseRobosub/RP2350PCB) we designed and assembled in-house!

Currently, the CAN base code comes from [KevinOConnor/can2040](https://github.com/KevinOConnor/can2040).

## Setup

1. Install an IDE which supports Dev Containers (VSCode, CLion, etc.)
2. Clone this repo.
3. Build and reopen the project within the Dev Container. (IDE specific)
4. `cd` into `example/` dir.
5. make and enter a build dir: `mkdir build` and `cd build`
6. Generate build files: `cmake ..`
7. Compile: `make`
8. Upload the `.uf2` file onto the microcontroller.

## Organization

Work in progress...

## Contributing

To contribute code:

1. Make a new branch with the format `name/feature_name` such as `adrian/add_status_led` and make all changes in that branch.
2. Make a Pull Request to the `main` branch and request `@guppyadrian` as a reviewer.
3. Make sure all commits have proper messages starting with `feat:`, `chore:`, or `fix:`
4. Document and comment all of your code!
5. Ensure your code works before merging into main!

## Contact and Sponsorship

Need to get in touch? Reach out to `robosub.vcea@wsu.edu`.

We are sponsored by many generous companies and people, including:

- OSH Park PCBs
- Blue Robotics
- Real Digital
- Solidworks
- Vectornav
- JoeScan
- WaterLinked
- LattePanda
- Tektronix
- And many many other personal supporters through their generous donations!

Do you like our work? Consider [sponsoring](https://foundation.wsu.edu/give/?fund=ffdf2195-2497-4361-b697-44e5024bf0b0) our team!
