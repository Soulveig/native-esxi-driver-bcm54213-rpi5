# Native ESXi driver for BCM54213 (Raspberry Pi 5)

[English](#english) | [Русский](#русский) | [Releases](https://github.com/Soulveig/native-esxi-driver-bcm54213-rpi5/releases)

## English

Native VMware ESXi-Arm VMkernel network driver for the onboard
Raspberry Pi 5 Ethernet interface.

> Technical clarification: BCM54213PE is the external Gigabit Ethernet PHY.
> The driver itself controls the Cadence GEM MAC integrated into the RP1
> southbridge and communicates with the PHY over MDIO.

## Status

Version **v0.214** is a host-verified development checkpoint for:

- Raspberry Pi 5;
- VMware ESXi-Arm 8.0U3c build 24449057 (`aarch64`);
- UEFI firmware exposing the RP1 Ethernet controller as ACPI `RPI0001`;
- BCM54213PE PHY;
- 1 Gbit/s Ethernet and MTU up to 9000.

![RP1_GEM adapter details in ESXi Host Client](docs/images/esxi-host-client-v0.211.png)

ESXi Host Client correctly reports auto-negotiation and the supported
1000/100/10 Mbit/s full-duplex modes. The screenshot was captured on v0.211;
these capabilities are unchanged in v0.214.

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
- v0.214 uses a 32-entry RX ring and TX batch size 8.
- v0.214 waits for the polling world to finish RX ring/DMA initialization
  before publishing Link Up. Two clean validation reboots brought RP1
  management online without a manual `vmnic128` down/up cycle.
- v0.214 replaces the terminal TX-watchdog state with a recoverable Cadence
  GEM queue reset, so a stuck batch no longer permanently disables TX.
- Wake-on-LAN is not advertised. RP1 contains a magic-packet detector, but the
  Raspberry Pi 5 platform does not provide the complete powered-off wake path
  required to start BCM2712 from Ethernet.
- The VIB is unsigned and has `CommunitySupported` acceptance.
- Secure Boot must be disabled.
- Installation/removal requires maintenance mode and reboot.
- Compatibility is currently restricted to ESXi-Arm build 24449057.
- Reliability on other firmware/build combinations is not yet established.

## Measured performance

The following results were observed on the development Raspberry Pi 5 host
with a wired external peer. They are checkpoints, not guarantees:

| Test | Result |
|---|---:|
| TCP TX, jumbo MTU, 83.9 s | 257 Mbit/s |
| TCP RX, jumbo MTU, 600 s | 873 Mbit/s |
| TCP TX after `vmnic128` down/up | 343 Mbit/s |
| TCP RX after `vmnic128` down/up | 852 Mbit/s |
| Bidirectional TX, 600 s | 361 Mbit/s |
| Bidirectional RX, 600 s | 64.6 Mbit/s |
| Jumbo ICMP payload 8972 | passed |

The bidirectional figures are lower because both directions compete in the
current polling and queue implementation. All 600-second tests completed with
zero driver RX/TX drops and errors. See
[docs/PERFORMANCE.md](docs/PERFORMANCE.md) for scope and interpretation.

## Download

Use the latest GitHub Release:

- `rp1gem-0.0.214-1-offline-bundle.zip` — recommended offline depot;
- `rp1gem-0.0.214-1-community.vib` — standalone VIB;
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
  -d /vmfs/volumes/datastore1/rp1gem-0.0.214-1-offline-bundle.zip \
  --dry-run --no-sig-check --maintenance-mode
```

6. If the dry-run reports BootBankInstaller and no errors, install and reboot:

```sh
esxcli software vib install \
  -d /vmfs/volumes/datastore1/rp1gem-0.0.214-1-offline-bundle.zip \
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
VIB contains the exact host-tested v0.214 binary.

## Testing and contributions

Follow [docs/TESTING.md](docs/TESTING.md) and report the exact Raspberry Pi UEFI
version, ESXi build, MTU, traffic direction, duration, packet drops and relevant
VMkernel log lines. Never experiment with registering shared IRQ 261.

This project is distributed under the [MIT License](LICENSE).

---

## Русский

Нативный сетевой драйвер VMware ESXi-Arm VMkernel для встроенного Ethernet
интерфейса Raspberry Pi 5.

> BCM54213PE — внешний Gigabit Ethernet PHY. Сам драйвер управляет Cadence GEM
> MAC в составе RP1 и взаимодействует с PHY через MDIO.

### Статус и совместимость

Версия **v0.214** проверена на хосте со следующей конфигурацией:

- Raspberry Pi 5;
- VMware ESXi-Arm 8.0U3c build 24449057 (`aarch64`);
- UEFI, предоставляющий Ethernet RP1 как ACPI `RPI0001`;
- PHY BCM54213PE;
- Ethernet 1 Гбит/с и MTU до 9000.

![Параметры адаптера RP1_GEM в ESXi Host Client](docs/images/esxi-host-client-v0.211.png)

ESXi Host Client корректно показывает автосогласование и режимы
1000/100/10 Мбит/с Full Duplex. Снимок сделан на v0.211; в v0.214 эти
возможности не изменялись.

### Обязательный UEFI для Raspberry Pi 5

Для работы встроенного Ethernet необходим изменённый UEFI Raspberry Pi 5,
который предоставляет контроллер RP1 Cadence GEM системе ESXi как ACPI
`RPI0001`. Скачать прошивку и прочитать инструкцию по её установке можно здесь:

**[Soulveig/rpi5-uefi-soulveig-edition](https://github.com/Soulveig/rpi5-uefi-soulveig-edition)**

Штатная конфигурация UEFI может не предоставлять устройство в форме, требуемой
этим драйвером.

Это нативный uplink-драйвер VMkernel, а не Linux-драйвер, USB-эмуляция или PCI
passthrough. ESXi регистрирует интерфейс как физический uplink, обычно
`vmnic128`.

Проект не связан с VMware, Broadcom или Raspberry Pi Foundation и не
поддерживается ими. Не используйте драйвер в производственной среде.

### Возможности

- Нативный модуль ESXi VMkernel для ARM64.
- Привязка через ACPI `RPI0001`.
- Инициализация Cadence GEM MAC.
- Обнаружение BCM54213PE, MDIO, автосогласование и состояние линка.
- Регистрация физического uplink в VMkernel.
- Передача и приём пакетов через сетевой стек VMkernel.
- DMA-кольца дескрипторов и RX replacement buffers.
- Обычный MTU и jumbo frames до MTU 9000.
- Трафик самого ESXi и виртуальных машин.
- Установка через VIB или offline bundle.
- Восстановление после link down/up и диагностические счётчики.

### Архитектура и ограничения

- RX обслуживается polling world с интервалом 500 мкс.
- Общий IRQ RP1 **261 намеренно не регистрируется**, поскольку он также
  используется другими функциями RP1, включая USB-контроллеры.
- В v0.214 используется RX-кольцо на 32 элемента и TX batch 8.
- v0.214 ожидает завершения инициализации RX ring/DMA в polling world до
  публикации Link Up. После двух чистых тестовых перезагрузок RP1 management
  появился без ручного цикла `vmnic128 down/up`.
- v0.214 заменяет терминальное состояние TX watchdog на восстанавливаемый
  сброс очереди Cadence GEM, чтобы зависшая пачка не отключала TX навсегда.
- Wake-on-LAN не заявляется: в RP1 есть детектор magic packet, но платформа
  Raspberry Pi 5 не предоставляет полный путь пробуждения BCM2712 по Ethernet
  из выключенного состояния.
- VIB не подписан и имеет уровень `CommunitySupported`.
- Secure Boot должен быть отключён.
- Установка и удаление требуют maintenance mode и перезагрузки.
- Совместимость пока ограничена ESXi-Arm build 24449057.
- Длительная надёжность и другие комбинации UEFI/ESXi ещё исследуются.

### Измеренная скорость

| Тест | Результат |
|---|---:|
| TCP TX, jumbo MTU, 83,9 с | 257 Мбит/с |
| TCP RX, jumbo MTU, 600 с | 873 Мбит/с |
| TCP TX после `vmnic128` down/up | 343 Мбит/с |
| TCP RX после `vmnic128` down/up | 852 Мбит/с |
| Двунаправленный TX, 600 с | 361 Мбит/с |
| Двунаправленный RX, 600 с | 64,6 Мбит/с |
| ICMP jumbo payload 8972 | успешно |

При двунаправленной нагрузке оба направления конкурируют в текущей реализации
polling и очередей. Все 600-секундные тесты завершились с нулевыми RX/TX drops
и errors драйвера. Подробности приведены в
[docs/PERFORMANCE.md](docs/PERFORMANCE.md).

### Загрузка

Используйте последний раздел [Releases](https://github.com/Soulveig/native-esxi-driver-bcm54213-rpi5/releases):

- `rp1gem-0.0.214-1-offline-bundle.zip` — рекомендуемый offline depot;
- `rp1gem-0.0.214-1-community.vib` — отдельный VIB;
- `SHA256SUMS` — контрольные суммы.

### Установка

До окончания проверок оставьте USB-сетевую карту как management и аварийный
интерфейс. Рекомендуется доступ к локальной консоли. Не устанавливайте VIB
поверх старого вручную добавленного `rp1sys.v00`.

Перед установкой драйвера установите изменённый UEFI из проекта
[Soulveig/rpi5-uefi-soulveig-edition](https://github.com/Soulveig/rpi5-uefi-soulveig-edition).

1. Выключите или перенесите ВМ и включите maintenance mode.
2. Проверьте build ESXi 24449057 и убедитесь, что Secure Boot отключён.
3. Загрузите offline bundle в datastore.
4. Установите уровень acceptance:

```sh
esxcli software acceptance set --level CommunitySupported
```

5. Выполните dry-run:

```sh
esxcli software vib install \
  -d /vmfs/volumes/datastore1/rp1gem-0.0.214-1-offline-bundle.zip \
  --dry-run --no-sig-check --maintenance-mode
```

6. Если dry-run завершился без ошибок, установите пакет и перезагрузите хост:

```sh
esxcli software vib install \
  -d /vmfs/volumes/datastore1/rp1gem-0.0.214-1-offline-bundle.zip \
  --no-sig-check --maintenance-mode --no-live-install
sync
reboot
```

Полная процедура проверки и отката: [docs/INSTALL.md](docs/INSTALL.md).

### Исходный код и сборка

Исходник находится в [src/rp1gem_mmio.c](src/rp1gem_mmio.c). Для сборки нужна
совместимая среда VMware/NDDK, а для упаковки — VMware `esximage` и payload
vmtar. Эти зависимости в репозиторий не входят.

### Тестирование

Следуйте [docs/TESTING.md](docs/TESTING.md). В отчёте указывайте версии UEFI и
ESXi, MTU, направление трафика, длительность, потери пакетов и относящиеся к
драйверу строки VMkernel. Никогда не регистрируйте общий IRQ 261.

Проект распространяется по лицензии [MIT](LICENSE).
