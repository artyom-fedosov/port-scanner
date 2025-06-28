# Port Scanner

A simple TCP port scanner written in C++ to check the accessibility of ports on remote IPv4 hosts.

---

## Features

- Check availability of one or multiple TCP ports on an IPv4 address
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

## Requirements

- Linux operating system
- C++20 compatible compiler
- CMake version 3.16 or higher

---

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
