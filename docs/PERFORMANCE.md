# Performance checkpoint

## v0.214 results

Tests used a Raspberry Pi 5 running ESXi-Arm 8.0U3c build 24449057 and a wired
external peer on the same LAN.

| Path and traffic profile | Observed result |
|---|---:|
| VM TCP transmit, jumbo MTU, 83.9 seconds | 257 Mbit/s, 1 retransmit |
| VM TCP receive, jumbo MTU, 600 seconds | 873 Mbit/s, 61.0 GB |
| VM TCP transmit after `vmnic128` down/up, 10 seconds | 343 Mbit/s |
| VM TCP receive after `vmnic128` down/up, 10 seconds | 852 Mbit/s |
| Bidirectional transmit, jumbo MTU, 600 seconds | 361 Mbit/s |
| Bidirectional receive, jumbo MTU, 600 seconds | 64.6 Mbit/s |

The 600-second receive and bidirectional tests completed without loss of VM or
management connectivity. Driver RX/TX drops and errors remained zero. The
administrative `vmnic128` down/up cycle also restored traffic without a host
reboot. Bidirectional results are listed separately because the current
polling and queue implementation makes the two directions compete for
processing capacity; they are not comparable to the one-direction figures.

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

Run single-stream tests first, followed by bidirectional traffic and at least a
600-second stability pass. Keep USB management available throughout.
