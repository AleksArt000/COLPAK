<br/>
<p align="center">
  <img src="https://camo.githubusercontent.com/f51ef40e5c340c16f8697534e2f6e2b57fc87e410e8cbabf03b2f1a99704033c/68747470733a2f2f692e696d6775722e636f6d2f746a56643856792e706e67" alt="Logo" width="80" height="80">

  <h3 align="center">COLPAK</h3> 
  <p align="center">
    CCCP Overseer of (gnu)/Linux Packages
  </p>
</p>

## Table Of Contents

* [About the Project](#about-the-project)
* [Built With](#built-with)
* [Getting Started](#getting-started)
  * [Prerequisites](#prerequisites)
  * [Installation](#installation)
* [License](#license)
* [Acknowledgements](#acknowledgements)

## About The Project

This is an unofficial GUI for CCCP, the package manager of the [Soviet](https://sovietlinux.org/) Linux Distribution

## Built With

The package manager is written in C with the LibSPM library, and the GUI makes use of raylib 5

## Getting Started


### Prerequisites

* LibSPM
* make
* Git
* Raylib
* X11/Wayland

### Installation

1. Build
    ```sh
    make 
    ```
2. Install
    **Note:** Run as root or with sudo
    ```sh
    sudo make install
    ```
3. Uninstall
    **Note:** Run as root or with sudo
    ```sh
    sudo make uninstall
    ```
## License

Distributed under the same GPL-3.0 License as CCCP. See [LICENSE](https://github.com/Soviet-Linux/CCCP/blob/main/LICENSE) for more information.
