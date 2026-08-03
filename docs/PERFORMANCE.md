# Performance checkpoint

## v0.213 results

Tests used a Raspberry Pi 5 running ESXi-Arm 8.0U3c build 24449057 and a wired
external peer on the same LAN.

| Path and traffic profile | Observed result |
|---|---:|
| VM TCP transmit, MSS 1460, 5 seconds | 179 Mbit/s |
| VM TCP receive, MSS 1460, 5 seconds | 263 Mbit/s |
| VM TCP transmit with jumbo frames, 5 seconds | 332 Mbit/s |
| VM TCP receive with jumbo frames, 5 seconds | 721 Mbit/s |

Standard-MTU performance is substantially lower than line rate, while the
jumbo receive path demonstrates that the MAC, PHY and underlying DMA path can
approach Gigabit Ethernet speed. This points toward per-packet processing,
polling cadence, batching and queue overhead as likely optimization areas; it
does not by itself prove one specific bottleneck.

## Reporting comparable results

Include all of the following:

- ESXi and UEFI versions;
- source and destination endpoints;
- direction as seen by `rp1gem`;
- MTU and TCP MSS;
- stream count and duration;
- average and peak throughput;
- packet drops/errors before and after;
- whether management used RP1 or USB;
- relevant `vmkernel.log` lines.

Run single-stream tests first, followed by parallel streams and a 60-second
stability pass. Keep USB management available throughout.
