README.txt -- part of MBLib

MBLib is a set of utility libraries for writing simple programs in C/C++.

An example of how to link MBLib into the build system is contained in
the "template" folder.

See LICENSE for licensing information.

MBLib is built with the Meson build-system.
For more details, see: <https://mesonbuild.com/index.html>

But for a quick-start guide:
# Setup the build system
meson setup build

# Configure for Release Build
#  ie full optimization, no symbols, no asserts, low warnings
meson configure -Ddevel=false -Ddebug=false build

# Configure for Debug Build
#  ie low optimization, with symbols, with asserts, low warnings
meson configure -Ddevel=false -Ddebug=true build

# Configure for Developer Build:
#  ie low optimization, with symbols, with asserts, high warnings
meson configure -Ddevel=true -Ddebug=true build

# Compile!
meson compile -C build

# Unit Tests
meson test -C build
