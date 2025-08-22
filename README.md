# Port Scanner

A simple TCP port scanner written in C++ to check the accessibility of ports on remote IPv4 and IPv6 hosts.

---

## Features

- Check availability of one or multiple TCP ports on an IPv4/IPv6 address
- Uses POSIX sockets and system calls
- Works **only on Linux** systems
- Built with CMake and C++20 standard

---

## Installation (Fedora / RHEL-based)

This package is available via Fedora Copr:

```bash
sudo dnf copr enable artjom-fedosov/port-scanner
sudo dnf install port-scanner
```

---

## Manual Build

```bash
git clone https://github.com/artyom-fedosov/port-scanner.git
cd port-scanner
./install.sh
```

---

## Requirements

- Linux operating system
- C++20 compatible compiler
- CMake version 3.16 or higher

---

## Documentation

Detailed API documentation is available and can be generated using Doxygen.

To generate the documentation locally, run:

```bash
doxygen Doxyfile
```

The generated HTML documentation will be available in the docs/html directory.

---

## Usage

Run the port scanner by specifying an IP address (IPv4 or IPv6) followed by one or more ports to check:

```bash
$ port-scanner 192.168.8.1 22 80 443
22  is accessible
80  is not accessible
443 is accessible
```

**Note**: In the terminal, accessible ports are displayed in **green**, while inaccessible ports are shown in **red** for better readability.
The order of the port scan results may vary because the checks are performed concurrently using threads.

You can also scan a range of ports using shell brace expansion (bash):

```bash
$ port-scanner 192.168.8.1 {0..65535} | grep "is accessible"
```

---

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
