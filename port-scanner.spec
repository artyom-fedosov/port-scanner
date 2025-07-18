Name:           port-scanner
Version:        1.0.3
Release:        1%{?dist}
Summary:        Lightweight TCP port scanner for Linux, written in C++.

BuildArch:      x86_64
License:        MIT
URL:            https://github.com/artyom-fedosov/port-scanner
Source0:        %{name}-%{version}.tar.gz

BuildRequires:  cmake
BuildRequires:  gcc-c++
BuildRequires:  make

Requires:       glibc

%description
port-scanner — Lightweight TCP port scanner for Linux, written in C++.

%prep
%autosetup -n %{name}-%{version}

%build
%cmake .
%cmake_build

%install
%cmake_install

%files
%license LICENSE
%doc README.md
%{_bindir}/port-scanner
%{_mandir}/man1/port-scanner.1.*

%changelog
* Fri Jul 18 2025 Artyom <artjomsfedosovs2@gmail.com> - 1.0.3-1
- Update

