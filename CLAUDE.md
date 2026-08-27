# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Overview

CM-Linux is a collection of independent embedded-Linux / systems-programming learning exercises. Each top-level directory is a self-contained project with its own `Makefile` and build conventions — there is no root build system. Work within the relevant subdirectory.

## Projects

### `Makefile/` — strutils library build exercise
Demonstrates building the same `strutils` C library (`str_reverse`, `str_trim`, `str_to_int`) as both a static (`.a`) and shared (`.so`) library, then linking a `main.c` driver against each.

Layout: sources in `src/` (plus `main.c` at root), headers in `inc/`, objects in `obj/` (`.o` regular, `.os` compiled with `-fPIC` for shared), libraries in `lib/Static_lib/` and `lib/Shared_lib/`, binaries in `bin/`. `CFLAGS` uses `-Wall -Werror`.

Build targets (run from `Makefile/`):
- `make build` — build `bin/main.exe` from all objects directly
- `make static` / `make link_static` — build `libstrutils.a` / link `bin/main_static.exe`
- `make shared` / `make link_shared` — build `libstrutils.so` / link `bin/main_shared.exe`
- `make all` — runs `link_shared` and `link_static`
- `make clean` — removes `obj/`, `bin/`, and both lib dirs
- `make print-<VAR>` — debug: prints the value of any Makefile variable

To run the shared-lib binary, set `LD_LIBRARY_PATH` to the `lib/Shared_lib/` directory first (the absolute path echoed by `link_shared` is stale; use the current repo path).

### `File_system/` — filestat tool
`main.c` is a `stat`-like tool that reads file metadata via `lstat()` (path, file type, size, last-modified). `FileSystem.md` is the full writeup. `example.c` is a separate scratch file.

Build (from `File_system/`): `make all` builds `main.exe`; `make exaple` builds `example.exe` (target name is misspelled in the Makefile — keep it as-is unless asked to fix). `make clean` removes `*.o *.exe`.

### `BBB/MyWorkSpace/` — out-of-tree kernel modules (BeagleBone / Raspberry Pi)
Loadable kernel modules cross-compiled for ARM against a Yocto/Poky SDK.

- `main.c` — minimal platform driver + device (`platform_driver_register` / `platform_device_register_simple`).
- `device_tree/led_act_driver.c` — GPIO LED platform driver bound via device-tree `compatible = "custom,gpio-leds-act"`; parses a `led-act` child node for `gpios`, `label`, and `linux,default-trigger`. Uses the LED class subsystem (`led_classdev`) and `devm_*` managed resources.
- `.dts` files (`am335x-boneblack.dts`, `device_tree/zero-w.dts`, `device_tree/act-led-custom.dts`) are device-tree sources.

Building these requires an ARM cross-toolchain and kernel headers — they will not build on a normal host without the SDK. Two Makefiles with different conventions:
- `MyWorkSpace/Makefile`: `make arm` (uses `$SDKTARGETSYSROOT`, `CROSS_COMPILE=arm-poky-linux-gnueabi-`) or `make host` (builds against the running host kernel via `/lib/modules/$(uname -r)/build`). Corresponding `make clean-arm` / `make clean-host`.
- `device_tree/Makefile`: `make all` with a hardcoded `KDIR` under `/opt/poky/4.0.34/...`. Update `KDIR`/`CROSS_COMPILE` to match the local SDK before building.

## Conventions

- Each project builds and runs independently — always `cd` into the project directory before running `make`.
- Compiled artifacts (`obj/`, `bin/`, `*.exe`, libs, `*.ko`) are build outputs, not committed sources.
- Some comments and echoed messages are in Vietnamese; preserve them when editing surrounding code.
