# Test matrix

The driver is under active development. Test one dimension at a time and preserve a known
bootbank rollback.

## Minimum acceptance test

- Host boots without PSOD.
- `rp1gem_mmio` is loaded from the installed VIB.
- `vmnic128` reports Up, 1000/Full.
- USB management and USB storage remain operational.
- The `rp1gem_poll_rx` world is present.
- Standard ping passes without loss.
- ESXi-to-peer TCP works in both directions.
- VM-to-peer TCP works in both directions.
- Interface counters advance without unexplained drops or watchdog events.

## Extended tests

- MTU 1500 and MTU 9000.
- One and multiple TCP streams.
- 60-second and longer sustained receive/transmit load.
- VM power cycle and host reboot.
- Verify RX immediately after reboot; if it does not advance, record the
  counters before and after one `vmnic128` down/up rearm cycle.
- Link cable removal/reconnection.
- Uplink down/up recovery.
- Management migration from USB to RP1 and back.

## Safety constraints

- Never register shared RP1 interrupt 261.
- Do not combine IRQ, TX, RX and ABI changes in one experiment.
- Keep a USB NIC and local console available.
- Do not claim a result as stable based only on link-up or a short ping.
- Remove host addresses, credentials and unrelated log data before posting an
  issue.
