# Installation, validation and rollback

## Requirements

- Raspberry Pi 5 and compatible UEFI ACPI tables with `RPI0001`.
- ESXi-Arm 8.0U3c build 24449057.
- Secure Boot disabled.
- USB Ethernet management/recovery path kept connected.
- Local console access and backed-up bootbanks.

Before installation, confirm that neither `/bootbank/boot.cfg` nor the
alternate bootbank loads a manually injected `rp1sys.v00`. Also confirm that
no older `rp1gem` VIB is installed.

```sh
vmware -vl
esxcli software acceptance get
grep -o 'rp1sys[^ ]*' /bootbank/boot.cfg /altbootbank/boot.cfg 2>/dev/null
esxcli software vib list | grep -i rp1gem
```

Follow the dry-run and install commands in the main README.

## Post-boot checks

Run these checks through the USB management path:

```sh
esxcli software vib list | grep -i rp1gem
vmkload_mod -s rp1gem_mmio | grep -E 'Version|Status'
esxcli network nic list | grep -E 'vmnic128|vusb0'
ps | grep '[r]p1gem_poll_rx'
df -h
tail -n 250 /var/log/vmkernel.log | grep -Ei 'rp1gem|WARNING|ERROR|PSOD'
```

Validate in this order:

1. Confirm `vmnic128` is Up, 1000/Full and uses the intended MTU.
2. Confirm the USB NIC, USB storage and datastores remain healthy.
3. Test standard ICMP, followed by jumbo payload 8972 only when the entire path
   is configured for MTU 9000.
4. Test ESXi-to-peer traffic in both directions.
5. Test VM-to-peer traffic in both directions.
6. Run sustained TCP load while watching driver counters, drops and logs.
7. Move management to RP1 only after these checks pass. Retain USB recovery.

## Removal

Return management to USB, stop VMs and enter maintenance mode:

```sh
esxcli software vib remove -n rp1gem --maintenance-mode --no-live-install
sync
reboot
```

After reboot, verify the VIB and module are absent. If the host cannot boot or
management does not return, use the local console and alternate bootbank.

Do not register shared RP1 IRQ 261 as a recovery workaround.
