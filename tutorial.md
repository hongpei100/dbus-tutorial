# Purpose: Demo the D-Bus Sensor Service in Linux User Space

## Step 0 — Prepare the Essential Tools

```bash
apt install -y build-essential meson ninja-build pkg-config \
    libsystemd-dev nlohmann-json3-dev \
    python3 python3-pip python3-yaml python3-mako python3-inflection \
    dbus dbus-x11 git

pip3 install jsonschema --break-system-packages
```

## Step 1 — Build sdbusplus

```bash
git clone https://github.com/openbmc/sdbusplus.git
cd sdbusplus
meson setup build --wrap-mode=forcefallback -Dexamples=disabled -Dtests=disabled
ninja -C build
ninja -C build install
ldconfig
```

**Q1: Why do we need to build sdbusplus from source?**

Ubuntu's apt repository does not provide sdbusplus as a package. We have to clone the source and build it using its default build system (meson + ninja).

**Q2: What files does the sdbusplus build generate?**

1. `libsdbusplus.so` — the shared library compiled from `src/bus.cpp`, `src/server/interface.cpp`, and other source files. Your service will link against this at runtime.
2. Header files installed to `/usr/local/include/sdbusplus/` — so that `#include <sdbusplus/bus.hpp>` can be resolved by the compiler.
3. `sdbusplus.pc` — a pkg-config file installed to `/usr/local/lib/pkgconfig/`, so that `dependency('sdbusplus')` in your meson.build can automatically locate the headers and the library.

**Q3: What does `ldconfig` do?**

When `ninja install` copies `libsdbusplus.so` to `/usr/local/lib/`, the Linux dynamic linker does not automatically know a new library has been added. `ldconfig` updates the linker cache so that the system can find the library at runtime. Without it, your service may compile and link successfully but crash at startup with `cannot open shared object file`.

At this point, running `pkg-config --modversion sdbusplus` should output `1.0.0`.

## Step 2 — Create the Project Structure

```bash
mkdir -p yaml/com/example/Sensor
mkdir -p generated/com/example/Sensor/Value
mkdir -p src
```

The YAML directory structure must mirror the D-Bus interface name. For the interface `com.example.Sensor.Value`, the YAML file goes to `yaml/com/example/Sensor/Value.interface.yaml`, where `yaml/` is the root directory passed to sdbus++ via the `-r` flag.

## Step 3 — Write the YAML Interface Definition

Create the file `yaml/com/example/Sensor/Value.interface.yaml` with your interface definition, including properties, methods, and signals.

## Step 4 — Generate C++ Bindings with sdbus++

```bash
SDBUS="/path/to/sdbusplus/tools/sdbus++"
TMPL="/path/to/sdbusplus/tools/sdbusplus/templates"
SCHEMA="/path/to/sdbusplus/tools/sdbusplus/schemas"
IFACE="com.example.Sensor.Value"

$SDBUS -r yaml -t $TMPL -s $SCHEMA interface common-header $IFACE \
    > generated/com/example/Sensor/Value/common.hpp

$SDBUS -r yaml -t $TMPL -s $SCHEMA interface server-header $IFACE \
    > generated/com/example/Sensor/Value/server.hpp

$SDBUS -r yaml -t $TMPL -s $SCHEMA interface server-cpp $IFACE \
    > generated/com/example/Sensor/Value/server.cpp
```

Running `sdbus++ -h` shows the command format: `sdbus++ [-h] [-r ROOTDIR] [-t TEMPLATEDIR] [-s SCHEMADIR] TYPE PROCESS ITEM`. The available TYPE and PROCESS values are defined in `sdbusplus/tools/sdbusplus/main.py`.

**Why these three files?**

- `common.hpp` — defines shared types (e.g. `PropertiesVariant`) used by both server and client. This is always required because `server.hpp` includes it.
- `server.hpp` — the abstract class you inherit from. It provides property getters/setters, signal emit functions, and declares your methods as pure virtual.
- `server.cpp` — the implementation of the D-Bus vtable, property callbacks, and type marshalling. This is the boilerplate you would otherwise have to write by hand.

You can optionally generate `client-header` if you plan to write a separate client program, but it is not needed for running the server.

## Step 5 — Write `src/main.cpp`

**Q: We already defined the interface in YAML and generated the server code. Why do we still need to write `main.cpp`?**

The YAML defines the interface schema, not the behavior. For example, if the YAML declares a method `SetThresholds` that takes two doubles, the codegen produces:

```cpp
virtual void setThresholds(double high, double low) = 0;
```

The codegen knows the method exists, but it cannot know what you want to do when it is called — store the values? Write to a hardware register? Send an alert? That is your business logic, and the YAML does not define it.

The generated class is abstract and cannot be instantiated directly. Your `main.cpp` is where you:

- **Inherit from it and implement the logic** — only you know what `setThresholds` should do.
- **Choose a bus name** — e.g. `com.example.Sensor`.
- **Choose an object path** — e.g. `/com/example/sensor/temperature/cpu0`.
- **Run the event loop** — D-Bus messages are only processed when someone calls `bus.process_discard()` and `bus.wait()`.

## Step 6 — Write `meson.build`

```meson
project(
    'my-dbus-service',
    'cpp',
    default_options: ['cpp_std=c++23', 'warning_level=2'],
    version: '0.1.0',
)

executable(
    'sensor-service',
    'src/main.cpp',
    'generated/com/example/Sensor/Value/server.cpp',
    include_directories: include_directories('generated'),
    dependencies: [
        dependency('sdbusplus'),
        dependency('libsystemd'),
        dependency('threads'),
    ],
    install: false,
)
```

**Q1: Why `install: false`?**

Setting `install: true` would copy the compiled binary to `/usr/local/bin/` when you run `ninja install`. Since this is a demo project, we keep the executable in the `build/` directory and run it directly from there.

**Q2: Why do we need the `dependencies` field?**

`#include` only resolves headers at compile time — it tells the compiler what the functions and classes look like. But the actual machine code lives in shared libraries (e.g. `libsdbusplus.so`). The `dependencies` field uses `pkg-config` to locate both the headers (`-I` flags) and the libraries (`-l` flags), so the linker can connect your code to the actual implementations. Without it, compilation would succeed but linking would fail with `undefined reference` errors.

**Q3: Why `include_directories`?**

The compiler only searches system paths (e.g. `/usr/include`, `/usr/local/include`) by default. The generated files like `com/example/Sensor/Value/server.hpp` live in the `generated/` directory, which is not a system path. Adding `include_directories('generated')` tells the compiler to also search there, equivalent to the `-I generated/` flag.

## Step 7 — Build and Run

```bash
cd my-dbus-service
meson setup build
ninja -C build
```

**Terminal 1** — run the service:

```bash
export LD_LIBRARY_PATH=/usr/local/lib/x86_64-linux-gnu
./build/sensor-service
```

**Terminal 2** — interact with the service using busctl:

```bash
# Introspect the object
busctl --user introspect com.example.Sensor /com/example/sensor/temperature/cpu0

# Read a property
busctl --user get-property com.example.Sensor \
    /com/example/sensor/temperature/cpu0 com.example.Sensor.Value Value

# Write a property
busctl --user set-property com.example.Sensor \
    /com/example/sensor/temperature/cpu0 com.example.Sensor.Value Value d 99.9

# Call a method
busctl --user call com.example.Sensor \
    /com/example/sensor/temperature/cpu0 com.example.Sensor.Value SetThresholds dd 90.0 5.0
```

**Important:** Make sure both terminals share the same `DBUS_SESSION_BUS_ADDRESS` environment variable. On a desktop Linux environment, this is usually inherited automatically. If Terminal 1 has a different value (e.g. from running `dbus-launch`), the service will register on a separate bus that Terminal 2 cannot see.