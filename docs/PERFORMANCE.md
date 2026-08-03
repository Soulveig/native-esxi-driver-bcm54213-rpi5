# Performance checkpoint

## v0.206 results

Tests used a Raspberry Pi 5 running ESXi-Arm 8.0U3c build 24449057 and a wired
external peer on the same LAN.

| Path and traffic profile | Observed result |
|---|---:|
| ESXi/VM TCP transmit, MSS 1460 | 178 Mbit/s |
| ESXi/VM TCP receive, MSS 1460 | 221 Mbit/s |
| Receive with jumbo frames, 60 seconds | 852 Mbit/s average |
| Receive with jumbo frames | 922 Mbit/s peak |

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
