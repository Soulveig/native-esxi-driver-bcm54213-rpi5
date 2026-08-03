[English](README.md) | [Русский](README_RU.md) | [Releases](https://github.com/Soulveig/native-esxi-driver-bcm54213-rpi5/releases)

# Native ESXi driver for BCM54213 (Raspberry Pi 5)

Native VMware ESXi-Arm VMkernel network driver for the onboard
Raspberry Pi 5 Ethernet interface.

> Technical clarification: BCM54213PE is the external Gigabit Ethernet PHY.
> The driver itself controls the Cadence GEM MAC integrated into the RP1
> southbridge and communicates with the PHY over MDIO.

## Status

Version **v0.206** is a boot-tested development checkpoint for:

- Raspberry Pi 5;
- VMware ESXi-Arm 8.0U3c build 24449057 (`aarch64`);
- UEFI firmware exposing the RP1 Ethernet controller as ACPI `RPI0001`;
- BCM54213PE PHY;
- 1 Gbit/s Ethernet and MTU up to 9000.

## Required Raspberry Pi 5 UEFI

The onboard Ethernet controller requires a modified Raspberry Pi 5 UEFI that
exposes the RP1 Cadence GEM device to ESXi as ACPI `RPI0001`. Download the
required firmware and follow its installation instructions here:

**[Soulveig/rpi5-uefi-soulveig-edition](https://github.com/Soulveig/rpi5-uefi-soulveig-edition)**

The stock UEFI configuration may not expose the device in the form required by
this driver.

This is a real native VMkernel uplink driver. It is not a Linux driver, USB
emulation, or PCI passthrough device. ESXi exposes the interface as a physical
uplink, normally `vmnic128`.

The project is not affiliated with or supported by VMware, Broadcom, or the
Raspberry Pi Foundation. Do not use it on production systems.

## Features

- Native ESXi VMkernel module for ARM64.
- ACPI device binding through `RPI0001`.
- Cadence GEM MAC initialization.
- BCM54213PE PHY discovery, MDIO access, autonegotiation and link reporting.
- VMkernel physical-uplink registration.
- VMkernel packet TX/RX integration.
- DMA-backed descriptor rings and replacement buffers.
- Standard MTU and jumbo frames up to MTU 9000.
- Traffic from both the ESXi host and virtual machines.
- VIB and offline-depot installation packages.
- Link down/up recovery and driver diagnostics.

## Current architecture and limitations

- RX uses a dedicated polling world at a 500 microsecond interval.
- The shared RP1 interrupt **261 is intentionally never registered** because it
  is shared with other RP1 functions, including USB controllers.
- v0.206 uses a 32-entry RX ring and TX batch size 8.
- The VIB is unsigned and has `CommunitySupported` acceptance.
- Secure Boot must be disabled.
- Installation/removal requires maintenance mode and reboot.
- Compatibility is currently restricted to ESXi-Arm build 24449057.
- Long-duration reliability and behavior on other firmware/build combinations
  are not yet established.

## Measured performance

The following results were observed on the development Raspberry Pi 5 host
with a wired external peer. They are checkpoints, not guarantees:

| Test | Result |
|---|---:|
| TCP TX, MSS 1460 | 178 Mbit/s |
| TCP RX, MSS 1460 | 221 Mbit/s |
| Jumbo RX, 60 seconds | 852 Mbit/s average |
| Jumbo RX peak | 922 Mbit/s |
| Jumbo ICMP payload 8972 | passed |

The slower standard-MTU path remains an area for optimization. See
[docs/PERFORMANCE.md](docs/PERFORMANCE.md) for scope and interpretation.

## Download

Use the latest GitHub Release:

- `rp1gem-0.0.206-1-offline-bundle.zip` — recommended offline depot;
- `rp1gem-0.0.206-1-community.vib` — standalone VIB;
- `SHA256SUMS` — release checksums.

## Installation

Keep a supported USB NIC configured as the management and recovery interface
until all RP1 tests pass. Local console access is strongly recommended.

Install the required modified UEFI from
[Soulveig/rpi5-uefi-soulveig-edition](https://github.com/Soulveig/rpi5-uefi-soulveig-edition)
before installing the driver.

Do not install the VIB over an older manually injected `rp1sys.v00` tardisk.
Use a clean ESXi installation or remove the old tardisk separately with a
verified bootbank rollback.

1. Shut down or migrate VMs and enter maintenance mode.
2. Verify ESXi build 24449057 and confirm Secure Boot is disabled.
3. Upload the offline bundle to a datastore.
4. Set the test host acceptance level:

```sh
esxcli software acceptance set --level CommunitySupported
```

5. Run a dry-run first:

```sh
esxcli software vib install \
  -d /vmfs/volumes/datastore1/rp1gem-0.0.206-1-offline-bundle.zip \
  --dry-run --no-sig-check --maintenance-mode
```

6. If the dry-run reports BootBankInstaller and no errors, install and reboot:

```sh
esxcli software vib install \
  -d /vmfs/volumes/datastore1/rp1gem-0.0.206-1-offline-bundle.zip \
  --no-sig-check --maintenance-mode --no-live-install
sync
reboot
```

See [docs/INSTALL.md](docs/INSTALL.md) for validation and rollback.

## Source and building

The current driver source is [src/rp1gem_mmio.c](src/rp1gem_mmio.c). It uses
VMkernel interfaces available in the ESXi-Arm environment. Building requires a
compatible VMware/NDDK-derived toolchain and packaging requires VMware's
`esximage` Python library plus a native vmtar payload. These dependencies are
not redistributed here.

The checked-in source comment was refreshed for publication, but the release
VIB contains the exact host-tested v0.206 binary.

## Testing and contributions

Follow [docs/TESTING.md](docs/TESTING.md) and report the exact Raspberry Pi UEFI
version, ESXi build, MTU, traffic direction, duration, packet drops and relevant
VMkernel log lines. Never experiment with registering shared IRQ 261.

This project is distributed under the [MIT License](LICENSE).
