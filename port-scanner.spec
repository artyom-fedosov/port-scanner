Name:           port-scanner
Version:        1.0.2
Release:        1%{?dist}
Summary:        Lightweight TCP port scanner for Linux, written in C++.

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
%autosetup

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
* Wed Jul 11 2025 Artyom <artjomsfedosovs2@gmail.com> - 1.0.2

