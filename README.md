# CorEngine

C++20 and Vulkan-based game and application engine for Linux.

## Requirements

- GNU Make
- A C++20 compiler
- `pkg-config`
- Vulkan development files, including `vulkan/vk_enum_string_helper.h`
- Wayland development files (default), or Xlib development files

Packages are discovered from the system with `pkg-config`; this project does not
contain a local dependency bundle.

## Build

```sh
make
```

The default target is Linux with the Wayland backend. To target Xlib instead:

```sh
make PLATFORM=xlib
```

For packages installed outside the system search path, point `pkg-config` at
their metadata:

```sh
make PACKAGE_PATH=/opt/vulkan/lib/pkgconfig
```

The LunarG Vulkan SDK can be selected directly:

```sh
make VULKAN_SDK="$HOME/VulkanSDK/<version>/x86_64"
```

Standard variables such as `CXX`, `CPPFLAGS`, `CXXFLAGS`, `LDFLAGS`, and
`LDLIBS` may also be overridden. Run `make print-config` to inspect the active
configuration.

The static library is written to `build/libCorEngine.a`. Install it with:

```sh
make install PREFIX=/usr/local
```
