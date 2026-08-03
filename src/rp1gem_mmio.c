/*
 * Read-only MMIO reachability probe for RP1 exposed as ACPI RPI0001.
 *
 * The attach callback maps ACPI memory resource 0, reads a small set of
 * read-only Cadence GEM identification/configuration registers, logs them,
 * and immediately unmaps it.
 * It performs no MMIO writes, DMA allocation, interrupt registration, PHY
 * access, or network-uplink registration.
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>

typedef int32_t vmk_Status;
typedef void *vmk_Driver;
typedef void *vmk_Device;
typedef void *vmk_ACPIDevice;
typedef void *vmk_HeapID;

extern vmk_Status vmk_DriverRegister(const void *props, vmk_Driver *driver);
extern vmk_Status vmk_DriverUnregister(vmk_Driver driver);
extern vmk_Status vmk_NameInitialize(void *name, const char *text);
extern vmk_Status vmk_HeapCreate(const void *props, vmk_HeapID *heap);
extern void vmk_ModuleSetHeapID(uint32_t module_id, vmk_HeapID heap);
extern vmk_HeapID vmk_ModuleGetHeapID(uint32_t module_id);
extern void vmk_HeapDestroy(vmk_HeapID heap);
extern vmk_Status vmk_DeviceGetRegistrationData(vmk_Device device, void **data);
extern vmk_Status vmk_ACPIQueryInfo(vmk_ACPIDevice device, void *info);
extern vmk_Status vmk_ACPIMapIOResource(uint32_t module_id,
                                        vmk_ACPIDevice device,
                                        uint32_t resource_index,
                                        void *resource);
extern vmk_Status vmk_ACPIUnmapIOResource(uint32_t module_id,
                                          vmk_ACPIDevice device,
                                          uint32_t resource_index);
extern vmk_Status vmk_ACPIAllocIntrCookie(uint32_t module_id,
                                          vmk_ACPIDevice device,
                                          uint32_t resource_index,
                                          uint32_t requested_count,
                                          uint32_t *cookies,
                                          uint32_t *allocated_count);
extern vmk_Status vmk_ACPIFreeIntrCookie(uint32_t module_id,
                                         vmk_ACPIDevice device);
extern vmk_Status vmk_IntrRegister(uint32_t module_id,
                                   uint32_t interrupt_cookie,
                                   const void *properties);
extern vmk_Status vmk_IntrUnregister(uint32_t module_id,
                                     uint32_t interrupt_cookie,
                                     void *handler_data);
extern vmk_Status vmk_MappedResourceRead32(const void *resource,
                                            uint64_t offset,
                                            uint32_t *value);
extern vmk_Status vmk_MappedResourceWrite32(void *resource,
                                             uint64_t offset,
                                             uint32_t value);
extern vmk_Status vmk_BusTypeFind(const void *name, uint64_t *bus_type);
extern void vmk_BusTypeRelease(uint64_t bus_type);
extern vmk_Status vmk_LogicalCreateBusAddress(void *bus_address,
                                              vmk_Device parent,
                                              uint32_t index,
                                              uint64_t *address,
                                              uint64_t *cookie);
extern size_t vmk_Strnlen(const char *text, size_t maximum);
extern vmk_Status vmk_DeviceRegister(const void *props,
                                     vmk_Device parent,
                                     vmk_Device *child);
extern vmk_Status vmk_DeviceUnregister(vmk_Device child);
extern vmk_Status vmk_UplinkCapRegister(void *uplink,
                                        uint32_t capability,
                                        void *operations);
extern vmk_Status vmk_UplinkUpdateLinkState(void *uplink,
                                            const void *link_state);
extern vmk_Status vmk_UplinkQueueMkRxQueueID(void *queue_id,
                                             uint32_t group,
                                             uint32_t index);
extern vmk_Status vmk_VA2MA(void *virtual_address,
                            uint64_t length,
                            uint64_t *machine_address);
extern vmk_Status vmk_DMAEngineCreate(const void *properties,
                                      void **dma_engine);
extern vmk_Status vmk_DMAMapElem(void *dma_engine,
                                 uint32_t direction,
                                 const void *input_element,
                                 uint32_t input_count,
                                 void *output_element,
                                 uint64_t *output_count);
extern vmk_Status vmk_DMAUnmapElem(void *dma_engine,
                                   uint32_t direction,
                                   const void *mapped_element);
extern vmk_Status vmk_DMAMapSg(void *dma_engine,
                               uint32_t direction,
                               const void *constraints,
                               const void *sg_array,
                               void **map_handle,
                               void **mapped_sg_array);
extern vmk_Status vmk_DMAUnmapSg(void *dma_engine,
                                 uint32_t direction,
                                 void *sg_ops,
                                 void *map_handle);
extern vmk_Status vmk_SgCreateOpsHandle(vmk_HeapID heap,
                                        void **sg_ops,
                                        uint64_t reserved0,
                                        uint64_t reserved1);
extern vmk_Status vmk_SgDestroyOpsHandle(void *sg_ops);
extern vmk_Status vmk_PktAllocForDMAEngine(uint32_t length,
                                           void *dma_engine,
                                           void **packet);
extern void *vmk_PktSgElemGet(void *packet, uint32_t element);
extern uint32_t vmk_PktSgElemAlignment(void *packet, uint32_t element);
extern void vmk_PktFrameLenSet(void *packet, uint32_t length);
extern uint32_t vmk_PktFrameLenGet(void *packet);
extern uint32_t vmk_PktIsFullyMapped(void *packet);
extern void *vmk_PktFrameMappedPointerGet(void *packet);
extern uint32_t vmk_PktFrameMappedLenGet(void *packet);
extern void vmk_PktRelease(void *packet);
extern uint32_t vmk_PktListGetCount(void *packet_list);
extern void *vmk_PktListPopFirstPkt(void *packet_list);
extern void vmk_PktListPrependPkt(void *packet_list, void *packet);
extern vmk_Status vmk_MemPoolCreate(const void *properties,
                                    void **memory_pool);
extern vmk_Status vmk_MemPoolAllocWithRA(void *memory_pool,
                                         const void *constraints,
                                         void *allocation,
                                         void *return_address);
extern vmk_Status vmk_Map(uint32_t module_id,
                          const void *properties,
                          void **virtual_address);
extern vmk_Status vmk_ServiceGetID(const char *name, uint64_t *service_id);
extern vmk_Status vmk_NetPollCreate(const void *properties,
                                    uint64_t service_id,
                                    uint32_t module_id,
                                    void **netpoll);
extern vmk_Status vmk_NetPollEnable(void *netpoll);
extern vmk_Status vmk_NetPollDisable(void *netpoll);
extern vmk_Status vmk_NetPollActivate(void *netpoll);
extern vmk_Status vmk_NetPollRxPktQueue(void *netpoll, void *packet);
extern vmk_Status vmk_NetPollFlushRx(void *netpoll);
extern vmk_Status vmk_NetPollDestroy(void *netpoll);
extern vmk_Status vmk_WorldCreate(const void *properties,
                                  uint32_t *world_id);
extern vmk_Status vmk_WorldDestroy(uint32_t world_id);
extern vmk_Status vmk_WorldSleep(uint64_t microseconds);
extern void vmk_WorldWaitForDeath(uint32_t world_id);
extern void vmk_DelayUsecs(uint32_t microseconds);
extern void _vmk_WarningMessage(const char *format, ...);
extern uint32_t vmk_ModuleCurrentID;

#define RP1_GEM_OFFSET 0U
#define RP1GEM_ATTACH_RX_PROBES_ENABLED 0U

static vmk_Driver rp1gem_driver;
static vmk_Device rp1gem_parent_device;
static vmk_Device rp1gem_uplink_child;
static void *rp1gem_uplink_handle;
static void *rp1gem_dma_engine;
static void *rp1gem_sg_ops;
static void *rp1gem_memory_pool;
static void *rp1gem_low_ring_page;
static void *rp1gem_jumbo_probe_pages;
static void *rp1gem_jumbo_tx_probe_pages;
static uint64_t rp1gem_jumbo_rx_probe_ma;
static uint64_t rp1gem_jumbo_tx_probe_ma;
static volatile uint32_t rp1gem_raw_rx_active;
static volatile uint32_t rp1gem_raw_rx_events;
static volatile uint32_t rp1gem_raw_rx_next;
static volatile uint32_t rp1gem_rx_stall_restarts;
static volatile uint32_t rp1gem_rx_stall_tx_completions;
static volatile uint32_t rp1gem_rx_stall_last_rx;
static volatile uint32_t rp1gem_rx_stall_last_tx;
static void *rp1gem_rx_packets[32];
static uint64_t rp1gem_rx_addresses[32];
static uint8_t rp1gem_gem_resource[64] __attribute__((aligned(8)));
static vmk_ACPIDevice rp1gem_gem_acpi_device;
static uint32_t rp1gem_gem_resource_mapped;
static void *rp1gem_rx_ring;
static void *rp1gem_tx_ring;
static uint64_t rp1gem_rx_ring_ma;
static uint32_t rp1gem_saved_ncr;
static uint32_t rp1gem_saved_ncfgr;
static uint32_t rp1gem_saved_dmacfg;
static uint32_t rp1gem_saved_jml;
static uint32_t rp1gem_saved_sa1b;
static uint32_t rp1gem_saved_sa1t;
static uint32_t rp1gem_interrupt_cookie;
static uint32_t rp1gem_interrupt_cookie_count;
static uint32_t rp1gem_interrupt_cookie_allocated;
static uint32_t rp1gem_interrupt_registered;

struct rp1gem_dma_desc32 {
    uint32_t addr;
    uint32_t ctrl;
};

struct rp1gem_sg_element {
    uint64_t address;
    uint32_t length;
    uint32_t reserved;
};

struct rp1gem_sg_array {
    uint32_t element_count;
    uint32_t element_capacity;
    uint64_t reserved0;
    uint64_t reserved1;
    struct rp1gem_sg_element element;
};

struct rp1gem_sg_array8 {
    uint32_t element_count;
    uint32_t element_capacity;
    uint64_t reserved0;
    uint64_t reserved1;
    struct rp1gem_sg_element elements[8];
};

_Static_assert(sizeof(struct rp1gem_sg_array) == 40,
               "unexpected SG array ABI size");
_Static_assert(sizeof(struct rp1gem_sg_array8) == 152,
               "unexpected 8-element SG array ABI size");

static struct rp1gem_sg_element rp1gem_rx_mapped_elements[32];

#define RP1GEM_RING_COUNT 32U
#define RP1GEM_RX_USED (1U << 0)
#define RP1GEM_RX_WRAP (1U << 1)
#define RP1GEM_TX_LAST (1U << 15)
#define RP1GEM_TX_USED (1U << 31)
#define RP1GEM_TX_WRAP (1U << 30)
#define RP1GEM_TX_LENGTH_MASK 0x3fffU
#define RP1GEM_JUMBO_PROBE_SIZE 9728U
#define RP1GEM_RAW_RX_COUNT 32U
#define RP1GEM_RAW_RX_PAGES_PER_BUFFER 3U
#define RP1GEM_RAW_RX_TOTAL_PAGES \
    (RP1GEM_RAW_RX_COUNT * RP1GEM_RAW_RX_PAGES_PER_BUFFER)
#define RP1GEM_RAW_RX_STRIDE \
    (RP1GEM_RAW_RX_PAGES_PER_BUFFER * 4096U)
#define RP1GEM_TX_HW_COUNT 8U
#define RP1GEM_TX_PAGES_PER_BUFFER 3U
#define RP1GEM_TX_TOTAL_PAGES \
    (RP1GEM_TX_HW_COUNT * RP1GEM_TX_PAGES_PER_BUFFER)
#define RP1GEM_TX_BUFFER_STRIDE \
    (RP1GEM_TX_PAGES_PER_BUFFER * 4096U)

static void rp1gem_cache_clean(const void *address, uint32_t length);
static void rp1gem_cache_invalidate(const void *address, uint32_t length);

struct rp1gem_netpoll_props {
    uint32_t (*callback)(void *cookie, uint32_t budget);
    uint64_t reserved0;
    void *cookie;
    uint32_t flags;
    uint32_t reserved1;
};

struct rp1gem_intr_props {
    vmk_Device device;
    uint8_t device_name[32];
    void *acknowledge;
    void *handler;
    void *handler_data;
    uint64_t attributes;
};

struct rp1gem_world_props {
    void *name;
    uint32_t module_id;
    uint32_t reserved0;
    vmk_Status (*start)(void *data);
    void *data;
    uint32_t start_world;
    uint32_t reserved1;
    vmk_HeapID heap;
};

_Static_assert(sizeof(struct rp1gem_intr_props) == 72,
               "unexpected interrupt properties ABI size");

_Static_assert(sizeof(struct rp1gem_world_props) == 48,
               "unexpected world properties ABI size");

_Static_assert(sizeof(struct rp1gem_netpoll_props) == 32,
               "unexpected NetPoll properties ABI size");

static volatile uint32_t rp1gem_netpoll_calls;
static volatile uint32_t rp1gem_netpoll_budget;
static volatile uint32_t rp1gem_netpoll_queued;
static volatile uint64_t rp1gem_netpoll_bytes;
static volatile uint64_t rp1gem_netpoll_dropped;
static volatile vmk_Status rp1gem_netpoll_queue_status = (vmk_Status)-1;
static volatile uint32_t rp1gem_netpoll_ring_used;
static volatile uint32_t rp1gem_netpoll_ring_queued;
static volatile uint32_t rp1gem_netpoll_ring_replaced;
static volatile vmk_Status rp1gem_netpoll_ring_status = (vmk_Status)-1;
static volatile uint32_t rp1gem_rx_diag_max_ready;
static volatile uint32_t rp1gem_rx_diag_full_ready;
static volatile uint32_t rp1gem_rx_diag_head_blocked;
static volatile uint32_t rp1gem_rx_diag_head_resyncs;
static volatile uint32_t rp1gem_rx_diag_head_skipped;
static volatile uint32_t rp1gem_rx_diag_head_grace_deferrals;
static volatile uint32_t rp1gem_rx_head_grace_wait;
static volatile uint32_t rp1gem_rx_diag_max_processed;
static volatile uint32_t rp1gem_rx_diag_budget_exhausted;
static volatile uint32_t rp1gem_rx_diag_callbacks_with_rx;
static volatile uint32_t rp1gem_rx_diag_valid_frames;
static volatile uint32_t rp1gem_rx_diag_missing_sof;
static volatile uint32_t rp1gem_rx_diag_missing_eof;
static volatile uint32_t rp1gem_rx_diag_bad_length;
static volatile uint32_t rp1gem_rx_diag_last_ctrl;
static volatile uint32_t rp1gem_rx_diag_rsr;
static volatile uint32_t rp1gem_rx_diag_rsr_or;
static volatile uint32_t rp1gem_rx_diag_isr;
static volatile uint32_t rp1gem_rx_diag_isr_or;
static volatile uint32_t rp1gem_rx_diag_status_samples;
static void *rp1gem_netpoll_handle;
static uint32_t rp1gem_netpoll_enabled;
static uint8_t rp1gem_adapter_context;
#define RP1GEM_PENDING_RX_COUNT 8U
static void *rp1gem_pending_rx_packets[RP1GEM_PENDING_RX_COUNT];
static uint32_t rp1gem_pending_rx_lengths[RP1GEM_PENDING_RX_COUNT];
static volatile uint32_t rp1gem_pending_rx_count;
static volatile uint32_t rp1gem_pending_rx_next;
static volatile uint32_t rp1gem_world_calls;
static volatile vmk_Status rp1gem_world_last_sleep_status =
    (vmk_Status)-1;
static volatile vmk_Status rp1gem_world_rx_status =
    (vmk_Status)-1;
static volatile vmk_Status rp1gem_world_netpoll_status =
    (vmk_Status)-1;
static volatile uint32_t rp1gem_netpoll_activate_enter;
static volatile uint32_t rp1gem_netpoll_activate_return;
static volatile uint32_t rp1gem_netpoll_activate_mismatch_logged;
static volatile uint32_t rp1gem_world_id;
static volatile uint32_t rp1gem_world_running;
static volatile uint32_t rp1gem_world_stop;
static volatile uint32_t rp1gem_link_restart_active;
/* The published mode list defaults to Auto even though cap15 starts at 1000. */
static volatile uint32_t rp1gem_auto_mode_selected = 1U;
static volatile uint32_t rp1gem_auto_restore_pending = 1U;
static volatile uint32_t rp1gem_auto_resolved_speed;
static volatile uint32_t rp1gem_tx_probe_calls;
static volatile uint32_t rp1gem_tx_probe_last_count;
static volatile uint32_t rp1gem_tx_state;
static volatile uint32_t rp1gem_tx_completed;
static volatile uint32_t rp1gem_tx_bytes;
static volatile uint64_t rp1gem_tx_total_bytes;
static volatile uint32_t rp1gem_tx_completion_ctrl;
static volatile uint32_t rp1gem_tx_completion_tsr;
static volatile uint32_t rp1gem_tx_poll_age;
static volatile uint32_t rp1gem_tx_watchdog_fired;
static volatile uint32_t rp1gem_tx_raw_buffer_active;
static volatile uint32_t rp1gem_tx_hw_head;
static volatile uint32_t rp1gem_tx_hw_inflight;
static uint32_t rp1gem_tx_hw_lengths[RP1GEM_TX_HW_COUNT];
static void *rp1gem_tx_packet;
static struct rp1gem_sg_element rp1gem_tx_mapped_element;
static void *rp1gem_tx_bounce_packet;
static void *rp1gem_tx_bounce_pointer;
static uint32_t rp1gem_tx_bounce_length;
#define RP1GEM_TX_SW_QUEUE_COUNT 256U
#define RP1GEM_POLL_INTERVAL_USEC 500U
#define RP1GEM_TX_WATCHDOG_TICKS 1000U
#define RP1GEM_HEARTBEAT_TICKS 30000U
#define RP1GEM_RX_STALL_TX_COMPLETIONS 64U
#define RP1GEM_RX_STALL_RESTART_ENABLED 0U
#define RP1GEM_RX_HEAD_GRACE_POLLS 1U
static uint8_t
    rp1gem_tx_sw_queue[RP1GEM_TX_SW_QUEUE_COUNT]
                       [RP1GEM_JUMBO_PROBE_SIZE]
    __attribute__((aligned(64)));
static uint32_t rp1gem_tx_sw_lengths[RP1GEM_TX_SW_QUEUE_COUNT];
static volatile uint32_t rp1gem_tx_sw_head;
static volatile uint32_t rp1gem_tx_sw_tail;
static volatile uint32_t rp1gem_tx_sw_count;
static volatile uint32_t rp1gem_tx_sw_lock;
static volatile uint32_t rp1gem_tx_sw_dropped;
static volatile uint32_t rp1gem_tx_diag_max_depth;
static volatile uint32_t rp1gem_tx_diag_queue_full;
static volatile uint32_t rp1gem_tx_diag_max_remaining;
static volatile uint32_t rp1gem_tx_diag_kick_busy;
static volatile uint32_t rp1gem_tx_diag_kick_empty;
static volatile uint32_t rp1gem_tx_diag_submitted;
static volatile uint32_t rp1gem_tx_diag_max_poll_age;
static volatile uint32_t rp1gem_tx_diag_state3_rejects;
static uint32_t rp1gem_phy_address = 0xffffffffU;

static void rp1gem_tx_kick(void);
static vmk_Status rp1gem_mdio_read(void *resource, uint32_t phy,
                                    uint32_t reg, uint32_t *value);
static vmk_Status rp1gem_mdio_write(void *resource, uint32_t phy,
                                     uint32_t reg, uint32_t value);

static uint32_t
rp1gem_tx_state_load(void)
{
    uint32_t value;

    __asm__ volatile(
        "ldar %w0, [%1]"
        : "=r"(value)
        : "r"(&rp1gem_tx_state)
        : "memory");
    return value;
}

static void
rp1gem_tx_state_store(uint32_t value)
{
    __asm__ volatile(
        "stlr %w0, [%1]"
        :
        : "r"(value), "r"(&rp1gem_tx_state)
        : "memory");
}

static uint32_t
rp1gem_tx_state_try_claim(void)
{
    uint32_t success;
    uint32_t status;
    uint32_t preparing = 4;

    __asm__ volatile(
        "1:\n"
        "ldaxr %w0, [%2]\n"
        "cbnz %w0, 2f\n"
        "stlxr %w1, %w3, [%2]\n"
        "cbnz %w1, 1b\n"
        "mov %w0, #1\n"
        "b 3f\n"
        "2:\n"
        "clrex\n"
        "mov %w0, #0\n"
        "3:"
        : "=&r"(success), "=&r"(status)
        : "r"(&rp1gem_tx_state), "r"(preparing)
        : "cc", "memory");
    return success;
}

static void
rp1gem_tx_sw_lock_acquire(void)
{
    uint32_t value;
    uint32_t status;
    uint32_t locked = 1;

    __asm__ volatile(
        "1:\n"
        "ldaxr %w0, [%2]\n"
        "cbnz %w0, 1b\n"
        "stlxr %w1, %w3, [%2]\n"
        "cbnz %w1, 1b"
        : "=&r"(value), "=&r"(status)
        : "r"(&rp1gem_tx_sw_lock), "r"(locked)
        : "cc", "memory");
}

static void
rp1gem_tx_sw_lock_release(void)
{
    uint32_t unlocked = 0;

    __asm__ volatile(
        "stlr %w0, [%1]"
        :
        : "r"(unlocked), "r"(&rp1gem_tx_sw_lock)
        : "memory");
}

struct rp1gem_uplink_stats {
    uint64_t rx_packets;
    uint64_t tx_packets;
    uint64_t rx_bytes;
    uint64_t tx_bytes;
    uint64_t rx_dropped;
    uint64_t tx_dropped;
    uint64_t remaining[18];
};

_Static_assert(sizeof(struct rp1gem_uplink_stats) == 192,
               "unexpected uplink statistics ABI size");

static void rp1gem_log_uplink_shared_snapshot(void);

static vmk_Status
rp1gem_poll_world_probe(void *data)
{
    uint32_t ring_index;
    vmk_Status result = 0;
    struct rp1gem_dma_desc32 *rx_ring32 =
        (struct rp1gem_dma_desc32 *)rp1gem_rx_ring;

    (void)data;
    rp1gem_world_last_sleep_status = vmk_WorldSleep(2000000);
    if (rp1gem_world_last_sleep_status != 0) {
        result = rp1gem_world_stop != 0 ?
            0 : rp1gem_world_last_sleep_status;
        goto done;
    }
    if (rp1gem_world_stop != 0)
        goto done;
    if (rx_ring32 == 0 || rp1gem_gem_resource_mapped == 0 ||
        rp1gem_netpoll_handle == 0 || rp1gem_netpoll_enabled == 0) {
        result = 1;
        goto done;
    }

    rp1gem_raw_rx_active =
        rp1gem_jumbo_probe_pages != 0 &&
        rp1gem_jumbo_rx_probe_ma != 0 &&
        (rp1gem_jumbo_rx_probe_ma >> 32) == 0;
    for (ring_index = 0; ring_index < RP1GEM_RING_COUNT;
         ring_index++) {
        rx_ring32[ring_index].ctrl = 0;
        rx_ring32[ring_index].addr =
            rp1gem_raw_rx_active != 0 ? RP1GEM_RX_USED :
            (uint32_t)rp1gem_rx_addresses[ring_index];
    }
    if (rp1gem_raw_rx_active != 0) {
        for (ring_index = 0; ring_index < RP1GEM_RAW_RX_COUNT;
             ring_index++) {
            rx_ring32[ring_index].addr =
                (uint32_t)(rp1gem_jumbo_rx_probe_ma +
                    ring_index * RP1GEM_RAW_RX_STRIDE);
            if (ring_index == RP1GEM_RAW_RX_COUNT - 1U)
                rx_ring32[ring_index].addr |= RP1GEM_RX_WRAP;
        }
        rp1gem_raw_rx_next = 0;
    } else {
        rx_ring32[RP1GEM_RING_COUNT - 1].addr |= RP1GEM_RX_WRAP;
    }
    rp1gem_cache_clean(
        rx_ring32,
        RP1GEM_RING_COUNT * sizeof(struct rp1gem_dma_desc32));

    rp1gem_world_rx_status =
        vmk_MappedResourceWrite32(
            rp1gem_gem_resource, 0x018,
            (uint32_t)rp1gem_rx_ring_ma);
    if (rp1gem_world_rx_status == 0)
        rp1gem_world_rx_status =
            vmk_MappedResourceWrite32(
                rp1gem_gem_resource, 0x04d4,
                (uint32_t)(rp1gem_rx_ring_ma >> 32));
    if (rp1gem_world_rx_status == 0)
        rp1gem_world_rx_status =
        vmk_MappedResourceWrite32(
            rp1gem_gem_resource, 0x088, 0x31505202);
    if (rp1gem_world_rx_status == 0)
        rp1gem_world_rx_status =
            vmk_MappedResourceWrite32(
                rp1gem_gem_resource, 0x08c, 0x00000100);
    if (rp1gem_world_rx_status == 0)
        rp1gem_world_rx_status =
            vmk_MappedResourceWrite32(
                rp1gem_gem_resource, 0x004,
                rp1gem_saved_ncfgr | (1U << 10) |
                    (1U << 4) | (1U << 1) |
                    (rp1gem_raw_rx_active != 0 ? (1U << 3) : 0));
    if (rp1gem_world_rx_status == 0)
        rp1gem_world_rx_status =
            vmk_MappedResourceWrite32(
                rp1gem_gem_resource, 0x010,
                (rp1gem_saved_dmacfg & ~(0xffU << 16)) |
                    ((rp1gem_raw_rx_active != 0 ? 152U : 32U) << 16));
    if (rp1gem_world_rx_status == 0)
        rp1gem_world_rx_status =
            vmk_MappedResourceWrite32(
                rp1gem_gem_resource, 0x048,
                rp1gem_raw_rx_active != 0 ?
                RP1GEM_JUMBO_PROBE_SIZE : 1518U);
    if (rp1gem_world_rx_status == 0)
        rp1gem_world_rx_status =
            vmk_MappedResourceWrite32(
                rp1gem_gem_resource, 0x020, 0x0f);
    if (rp1gem_world_rx_status == 0)
        rp1gem_world_rx_status =
            vmk_MappedResourceWrite32(
                rp1gem_gem_resource, 0x000,
                rp1gem_saved_ncr | (1U << 2));
    if (rp1gem_world_rx_status != 0) {
        result = rp1gem_world_rx_status;
        goto restore;
    }
    _vmk_WarningMessage(
        "rp1gem_mmio: persistent polling RX active "
        "intervalUsec=%u rxStatus=%x raw=%u ma=%lx size=%u count=%u",
        RP1GEM_POLL_INTERVAL_USEC,
        rp1gem_world_rx_status, rp1gem_raw_rx_active,
        rp1gem_jumbo_rx_probe_ma, RP1GEM_JUMBO_PROBE_SIZE,
        RP1GEM_RAW_RX_COUNT);
    rp1gem_rx_stall_last_rx = rp1gem_netpoll_ring_queued;
    rp1gem_rx_stall_last_tx = rp1gem_tx_completed;
    rp1gem_rx_stall_tx_completions = 0;

    while (rp1gem_world_stop == 0) {
        struct rp1gem_dma_desc32 *tx_ring32 =
            (struct rp1gem_dma_desc32 *)rp1gem_tx_ring;

        rp1gem_world_last_sleep_status =
            vmk_WorldSleep(RP1GEM_POLL_INTERVAL_USEC);
        if (rp1gem_world_last_sleep_status != 0) {
            if (rp1gem_world_stop == 0)
                result = rp1gem_world_last_sleep_status;
            break;
        }
        rp1gem_netpoll_activate_enter++;
        rp1gem_world_netpoll_status =
            vmk_NetPollActivate(rp1gem_netpoll_handle);
        rp1gem_netpoll_activate_return++;
        rp1gem_netpoll_activate_mismatch_logged = 0;
        if (rp1gem_world_netpoll_status != 0) {
            result = rp1gem_world_netpoll_status;
            break;
        }
        if ((rp1gem_world_calls % 1000U) == 0) {
            uint32_t rx_rsr = 0;
            uint32_t rx_isr = 0;

            (void)vmk_MappedResourceRead32(
                rp1gem_gem_resource, 0x020, &rx_rsr);
            (void)vmk_MappedResourceRead32(
                rp1gem_gem_resource, 0x024, &rx_isr);
            rp1gem_rx_diag_rsr = rx_rsr;
            rp1gem_rx_diag_rsr_or |= rx_rsr;
            rp1gem_rx_diag_isr = rx_isr;
            rp1gem_rx_diag_isr_or |= rx_isr;
            rp1gem_rx_diag_status_samples++;
        }
        if (rp1gem_tx_state_load() == 2 &&
            tx_ring32 != 0) {
            uint32_t tx_ctrl = 0;
            uint32_t batch_completed = 0;

            while (rp1gem_tx_hw_inflight != 0) {
                uint32_t tx_index = rp1gem_tx_hw_head;

                rp1gem_cache_invalidate(
                    &tx_ring32[tx_index],
                    sizeof(tx_ring32[tx_index]));
                tx_ctrl = tx_ring32[tx_index].ctrl;
                if ((tx_ctrl & RP1GEM_TX_USED) == 0)
                    break;
                rp1gem_tx_completed++;
                rp1gem_tx_total_bytes +=
                    rp1gem_tx_hw_lengths[tx_index];
                rp1gem_tx_bytes = rp1gem_tx_hw_lengths[tx_index];
                rp1gem_tx_hw_lengths[tx_index] = 0;
                rp1gem_tx_hw_head =
                    (tx_index + 1U) % RP1GEM_TX_HW_COUNT;
                rp1gem_tx_hw_inflight--;
                batch_completed++;
            }
            if (rp1gem_tx_hw_inflight == 0 && batch_completed != 0) {
                uint32_t tx_status = 0;
                uint32_t tx_ncr = 0;

                (void)vmk_MappedResourceRead32(
                    rp1gem_gem_resource, 0x014, &tx_status);
                if (vmk_MappedResourceRead32(
                        rp1gem_gem_resource, 0x000,
                        &tx_ncr) == 0) {
                    (void)vmk_MappedResourceWrite32(
                        rp1gem_gem_resource, 0x000,
                        tx_ncr & ~((1U << 9) | (1U << 3)));
                }
                rp1gem_tx_raw_buffer_active = 0;
                rp1gem_tx_completion_ctrl = tx_ctrl;
                rp1gem_tx_completion_tsr = tx_status;
                if (rp1gem_tx_poll_age >
                    rp1gem_tx_diag_max_poll_age)
                    rp1gem_tx_diag_max_poll_age =
                        rp1gem_tx_poll_age;
                rp1gem_tx_poll_age = 0;
                rp1gem_tx_packet = 0;
                /* TXEN restart resets the GEM fetch pointer to TXQBASE. */
                rp1gem_tx_hw_head = 0;
                rp1gem_tx_state_store(0);
                if (rp1gem_tx_completed <= 8U) {
                    _vmk_WarningMessage(
                        "rp1gem_mmio: TX polling completion "
                        "ctrl=%08x TSR=%08x batch=%u "
                        "completed=%u packetBytes=%u totalBytes=%lu state=%u",
                        tx_ctrl, tx_status, batch_completed,
                        rp1gem_tx_completed, rp1gem_tx_bytes,
                        rp1gem_tx_total_bytes, rp1gem_tx_state);
                }
                rp1gem_tx_kick();
            } else {
                rp1gem_tx_poll_age++;
                if (rp1gem_tx_poll_age >=
                    RP1GEM_TX_WATCHDOG_TICKS) {
                    uint32_t watchdog_ncr = 0;
                    uint32_t watchdog_index;

                    if (vmk_MappedResourceRead32(
                            rp1gem_gem_resource, 0x000,
                            &watchdog_ncr) == 0) {
                        (void)vmk_MappedResourceWrite32(
                            rp1gem_gem_resource, 0x000,
                            watchdog_ncr &
                                ~((1U << 9) | (1U << 3)));
                    }
                    rp1gem_tx_raw_buffer_active = 0;
                    for (watchdog_index = 0;
                         watchdog_index < RP1GEM_TX_HW_COUNT;
                         watchdog_index++) {
                        tx_ring32[watchdog_index].ctrl =
                            RP1GEM_TX_USED |
                            (watchdog_index ==
                                 RP1GEM_TX_HW_COUNT - 1U ?
                             RP1GEM_TX_WRAP : 0);
                        rp1gem_tx_hw_lengths[watchdog_index] = 0;
                    }
                    rp1gem_cache_clean(
                        tx_ring32,
                        RP1GEM_TX_HW_COUNT * sizeof(tx_ring32[0]));
                    rp1gem_tx_hw_head = 0;
                    rp1gem_tx_hw_inflight = 0;
                    rp1gem_tx_sw_lock_acquire();
                    rp1gem_tx_sw_dropped +=
                        1 + rp1gem_tx_sw_count;
                    rp1gem_tx_sw_count = 0;
                    rp1gem_tx_sw_head = 0;
                    rp1gem_tx_sw_tail = 0;
                    rp1gem_tx_sw_lock_release();
                    rp1gem_tx_watchdog_fired++;
                    rp1gem_tx_state_store(3);
                    _vmk_WarningMessage(
                        "rp1gem_mmio: TX watchdog fired "
                        "age=%u ctrl=%08x dropped=%u",
                        rp1gem_tx_poll_age, tx_ctrl,
                        rp1gem_tx_sw_dropped);
                }
            }
        }
        if (rp1gem_netpoll_ring_queued != rp1gem_rx_stall_last_rx) {
            rp1gem_rx_stall_last_rx = rp1gem_netpoll_ring_queued;
            rp1gem_rx_stall_last_tx = rp1gem_tx_completed;
            rp1gem_rx_stall_tx_completions = 0;
        } else if (rp1gem_tx_completed != rp1gem_rx_stall_last_tx) {
            rp1gem_rx_stall_tx_completions +=
                rp1gem_tx_completed - rp1gem_rx_stall_last_tx;
            rp1gem_rx_stall_last_tx = rp1gem_tx_completed;
        }
        if (RP1GEM_RX_STALL_RESTART_ENABLED != 0 &&
            rp1gem_raw_rx_active != 0 &&
            rp1gem_rx_stall_tx_completions >=
                RP1GEM_RX_STALL_TX_COMPLETIONS) {
            uint32_t rx_ncr = 0;
            uint32_t rx_rsr = 0;
            uint32_t rx_isr = 0;
            uint32_t rx_qbase = 0;
            uint32_t rx_index;
            vmk_Status restart_status;

            restart_status = vmk_MappedResourceRead32(
                rp1gem_gem_resource, 0x000, &rx_ncr);
            (void)vmk_MappedResourceRead32(
                rp1gem_gem_resource, 0x020, &rx_rsr);
            (void)vmk_MappedResourceRead32(
                rp1gem_gem_resource, 0x024, &rx_isr);
            (void)vmk_MappedResourceRead32(
                rp1gem_gem_resource, 0x018, &rx_qbase);
            rp1gem_cache_invalidate(
                rx_ring32,
                RP1GEM_RAW_RX_COUNT *
                    sizeof(struct rp1gem_dma_desc32));
            _vmk_WarningMessage(
                "rp1gem_mmio: RX stall snapshot count=%u txNoRx=%u "
                "NCR=%08x RSR=%08x ISR=%08x RXQ=%08x next=%u",
                rp1gem_rx_stall_restarts + 1U,
                rp1gem_rx_stall_tx_completions,
                rx_ncr, rx_rsr, rx_isr, rx_qbase,
                rp1gem_raw_rx_next);
            _vmk_WarningMessage(
                "rp1gem_mmio: RX stall descriptors "
                "d0=%08x/%08x d1=%08x/%08x "
                "d2=%08x/%08x d3=%08x/%08x",
                rx_ring32[0].addr, rx_ring32[0].ctrl,
                rx_ring32[1].addr, rx_ring32[1].ctrl,
                rx_ring32[2].addr, rx_ring32[2].ctrl,
                rx_ring32[3].addr, rx_ring32[3].ctrl);
            if (restart_status == 0)
                restart_status = vmk_MappedResourceWrite32(
                    rp1gem_gem_resource, 0x000,
                    rx_ncr & ~(1U << 2));
            __asm__ volatile("dmb ish" ::: "memory");
            for (rx_index = 0;
                 rx_index < RP1GEM_RING_COUNT; rx_index++) {
                rx_ring32[rx_index].ctrl = 0;
                rx_ring32[rx_index].addr = RP1GEM_RX_USED;
            }
            for (rx_index = 0;
                 rx_index < RP1GEM_RAW_RX_COUNT; rx_index++) {
                rx_ring32[rx_index].addr =
                    (uint32_t)(rp1gem_jumbo_rx_probe_ma +
                        rx_index * RP1GEM_RAW_RX_STRIDE);
                if (rx_index == RP1GEM_RAW_RX_COUNT - 1U)
                    rx_ring32[rx_index].addr |= RP1GEM_RX_WRAP;
            }
            rp1gem_cache_clean(
                rx_ring32,
                RP1GEM_RING_COUNT *
                    sizeof(struct rp1gem_dma_desc32));
            if (restart_status == 0)
                restart_status = vmk_MappedResourceWrite32(
                    rp1gem_gem_resource, 0x018,
                    (uint32_t)rp1gem_rx_ring_ma);
            if (restart_status == 0)
                restart_status = vmk_MappedResourceWrite32(
                    rp1gem_gem_resource, 0x04d4,
                    (uint32_t)(rp1gem_rx_ring_ma >> 32));
            if (restart_status == 0)
                restart_status = vmk_MappedResourceWrite32(
                    rp1gem_gem_resource, 0x020, 0x0f);
            __asm__ volatile("dmb ish" ::: "memory");
            if (restart_status == 0)
                restart_status = vmk_MappedResourceWrite32(
                    rp1gem_gem_resource, 0x000,
                    rx_ncr | (1U << 2));
            if (restart_status == 0) {
                rp1gem_rx_stall_restarts++;
                rp1gem_raw_rx_next = 0;
                rp1gem_rx_stall_last_rx =
                    rp1gem_netpoll_ring_queued;
                rp1gem_rx_stall_last_tx = rp1gem_tx_completed;
                rp1gem_rx_stall_tx_completions = 0;
            }
            _vmk_WarningMessage(
                "rp1gem_mmio: RX stall reinit status=%x restarts=%u",
                restart_status, rp1gem_rx_stall_restarts);
            if (restart_status != 0) {
                result = restart_status;
                break;
            }
        }
        rp1gem_world_calls++;
        if ((rp1gem_world_calls % RP1GEM_HEARTBEAT_TICKS) == 0) {
            _vmk_WarningMessage(
                "rp1gem_mmio: polling heartbeat calls=%u "
                "rxPackets=%u rxBytes=%lu rxDropped=%lu "
                "ringQueued=%u ringReplaced=%u ringStatus=%x "
                "stallRestarts=%u txNoRx=%u next=%u",
                rp1gem_world_calls, rp1gem_netpoll_queued,
                rp1gem_netpoll_bytes, rp1gem_netpoll_dropped,
                rp1gem_netpoll_ring_queued,
                rp1gem_netpoll_ring_replaced,
                rp1gem_netpoll_ring_status,
                rp1gem_rx_stall_restarts,
                rp1gem_rx_stall_tx_completions,
                rp1gem_raw_rx_next);
            _vmk_WarningMessage(
                "rp1gem_mmio: RX backlog maxReady=%u fullReady=%u "
                "headBlocked=%u headResyncs=%u headSkipped=%u "
                "headGrace=%u "
                "maxProcessed=%u budgetHit=%u "
                "callbacksWithRx=%u",
                rp1gem_rx_diag_max_ready,
                rp1gem_rx_diag_full_ready,
                rp1gem_rx_diag_head_blocked,
                rp1gem_rx_diag_head_resyncs,
                rp1gem_rx_diag_head_skipped,
                rp1gem_rx_diag_head_grace_deferrals,
                rp1gem_rx_diag_max_processed,
                rp1gem_rx_diag_budget_exhausted,
                rp1gem_rx_diag_callbacks_with_rx);
            _vmk_WarningMessage(
                "rp1gem_mmio: RX descriptor valid=%u missingSOF=%u "
                "missingEOF=%u badLength=%u lastCtrl=%08x "
                "RSR=%08x RSRor=%08x ISR=%08x ISRor=%08x samples=%u",
                rp1gem_rx_diag_valid_frames,
                rp1gem_rx_diag_missing_sof,
                rp1gem_rx_diag_missing_eof,
                rp1gem_rx_diag_bad_length,
                rp1gem_rx_diag_last_ctrl,
                rp1gem_rx_diag_rsr,
                rp1gem_rx_diag_rsr_or,
                rp1gem_rx_diag_isr,
                rp1gem_rx_diag_isr_or,
                rp1gem_rx_diag_status_samples);
            _vmk_WarningMessage(
                "rp1gem_mmio: TX saturation maxDepth=%u full=%u "
                "maxRemaining=%u kickBusy=%u kickEmpty=%u "
                "submitted=%u maxPollAge=%u state3Rejects=%u "
                "dropped=%u watchdog=%u state=%u",
                rp1gem_tx_diag_max_depth,
                rp1gem_tx_diag_queue_full,
                rp1gem_tx_diag_max_remaining,
                rp1gem_tx_diag_kick_busy,
                rp1gem_tx_diag_kick_empty,
                rp1gem_tx_diag_submitted,
                rp1gem_tx_diag_max_poll_age,
                rp1gem_tx_diag_state3_rejects,
                rp1gem_tx_sw_dropped,
                rp1gem_tx_watchdog_fired,
                rp1gem_tx_state);
            rp1gem_log_uplink_shared_snapshot();
        }
    }

restore:
    (void)vmk_MappedResourceWrite32(
        rp1gem_gem_resource, 0x000, rp1gem_saved_ncr);
    (void)vmk_WorldSleep(RP1GEM_POLL_INTERVAL_USEC);
    (void)vmk_NetPollActivate(rp1gem_netpoll_handle);
    (void)vmk_MappedResourceWrite32(
        rp1gem_gem_resource, 0x004, rp1gem_saved_ncfgr);
    (void)vmk_MappedResourceWrite32(
        rp1gem_gem_resource, 0x010, rp1gem_saved_dmacfg);
    (void)vmk_MappedResourceWrite32(
        rp1gem_gem_resource, 0x048, rp1gem_saved_jml);
    (void)vmk_MappedResourceWrite32(
        rp1gem_gem_resource, 0x088, rp1gem_saved_sa1b);
    (void)vmk_MappedResourceWrite32(
        rp1gem_gem_resource, 0x08c, rp1gem_saved_sa1t);

done:
    rp1gem_world_running = 0;
    return result;
}

static void
rp1gem_stop_poll_world(void)
{
    vmk_Status destroy_status = 0;
    uint32_t world_id = rp1gem_world_id;

    if (world_id == 0)
        return;
    rp1gem_world_stop = 1;
    if (rp1gem_world_running != 0)
        destroy_status = vmk_WorldDestroy(world_id);
    vmk_WorldWaitForDeath(world_id);
    rp1gem_world_id = 0;
    rp1gem_world_running = 0;
    _vmk_WarningMessage(
        "rp1gem_mmio: polling RX world stopped destroyStatus=%x "
        "id=%u calls=%u sleepStatus=%x rxStatus=%x "
        "netpollStatus=%x queued=%u ringQueued=%u "
        "ringReplaced=%u ringStatus=%x",
        destroy_status, world_id, rp1gem_world_calls,
        rp1gem_world_last_sleep_status, rp1gem_world_rx_status,
        rp1gem_world_netpoll_status, rp1gem_netpoll_queued,
        rp1gem_netpoll_ring_queued, rp1gem_netpoll_ring_replaced,
        rp1gem_netpoll_ring_status);
}

static uint32_t
rp1gem_empty_netpoll(void *cookie, uint32_t budget)
{
    uint32_t processed = 0;
    uint32_t ring_index;
    struct rp1gem_dma_desc32 *rx_ring32 =
        (struct rp1gem_dma_desc32 *)rp1gem_rx_ring;

    (void)cookie;
    rp1gem_netpoll_budget = budget;
    rp1gem_netpoll_calls++;
    while (rp1gem_pending_rx_next < rp1gem_pending_rx_count &&
           processed < budget) {
        uint32_t pending_index = rp1gem_pending_rx_next++;
        void *packet = rp1gem_pending_rx_packets[pending_index];

        rp1gem_pending_rx_packets[pending_index] = 0;
        vmk_PktFrameLenSet(
            packet, rp1gem_pending_rx_lengths[pending_index]);
        rp1gem_netpoll_queue_status =
            vmk_NetPollRxPktQueue(rp1gem_netpoll_handle, packet);
        if (rp1gem_netpoll_queue_status == 0) {
            rp1gem_netpoll_queued++;
            rp1gem_netpoll_bytes +=
                rp1gem_pending_rx_lengths[pending_index];
        } else {
            rp1gem_netpoll_dropped++;
        }
        processed++;
    }
    if (rp1gem_raw_rx_active != 0 && rx_ring32 != 0 &&
        processed < budget) {
        uint32_t raw_processed_start = processed;
        uint32_t ready_count = 0;
        uint32_t ready_index;

        rp1gem_cache_invalidate(
            rx_ring32,
            RP1GEM_RAW_RX_COUNT *
                sizeof(struct rp1gem_dma_desc32));
        for (ready_index = 0;
             ready_index < RP1GEM_RAW_RX_COUNT; ready_index++) {
            if ((rx_ring32[ready_index].addr &
                 RP1GEM_RX_USED) != 0)
                ready_count++;
        }
        if (ready_count > rp1gem_rx_diag_max_ready)
            rp1gem_rx_diag_max_ready = ready_count;
        if (ready_count == RP1GEM_RAW_RX_COUNT)
            rp1gem_rx_diag_full_ready++;
        if (ready_count != 0 &&
            (rx_ring32[rp1gem_raw_rx_next].addr &
             RP1GEM_RX_USED) == 0)
            rp1gem_rx_diag_head_blocked++;
        while (processed < budget) {
            uint32_t raw_index = rp1gem_raw_rx_next;
            uint32_t scan_offset = 0;
            uint32_t descriptor_control;
            uint32_t received_length;
            uint8_t *raw_buffer;
            uint64_t raw_ma;
            void *packet = 0;
            void *mapped_pointer = 0;
            uint32_t mapped_length = 0;
            vmk_Status status = 1;

            if ((rx_ring32[raw_index].addr & RP1GEM_RX_USED) == 0) {
                for (scan_offset = 1;
                     scan_offset < RP1GEM_RAW_RX_COUNT;
                     scan_offset++) {
                    uint32_t scan_index =
                        (rp1gem_raw_rx_next + scan_offset) %
                        RP1GEM_RAW_RX_COUNT;

                    if ((rx_ring32[scan_index].addr &
                         RP1GEM_RX_USED) != 0) {
                        raw_index = scan_index;
                        break;
                    }
                }
                if (scan_offset == RP1GEM_RAW_RX_COUNT)
                    break;
                if (ready_count != RP1GEM_RAW_RX_COUNT &&
                    rp1gem_rx_head_grace_wait <
                    RP1GEM_RX_HEAD_GRACE_POLLS) {
                    rp1gem_rx_head_grace_wait++;
                    rp1gem_rx_diag_head_grace_deferrals++;
                    break;
                }
                rp1gem_rx_head_grace_wait = 0;
                rp1gem_rx_diag_head_resyncs++;
                rp1gem_rx_diag_head_skipped += scan_offset;
            } else {
                rp1gem_rx_head_grace_wait = 0;
            }
            raw_buffer = (uint8_t *)rp1gem_jumbo_probe_pages +
                raw_index * RP1GEM_RAW_RX_STRIDE;
            raw_ma = rp1gem_jumbo_rx_probe_ma +
                raw_index * RP1GEM_RAW_RX_STRIDE;
            rp1gem_cache_invalidate(&rx_ring32[raw_index],
                                    sizeof(rx_ring32[raw_index]));
            descriptor_control = rx_ring32[raw_index].ctrl;
            received_length = descriptor_control & 0x3fffU;
            if ((rx_ring32[raw_index].addr & RP1GEM_RX_USED) == 0)
                break;
            rp1gem_rx_diag_last_ctrl = descriptor_control;
            if ((descriptor_control & 0x8000U) == 0)
                rp1gem_rx_diag_missing_sof++;
            if ((descriptor_control & 0x4000U) == 0)
                rp1gem_rx_diag_missing_eof++;
            if (received_length == 0 ||
                received_length > RP1GEM_JUMBO_PROBE_SIZE)
                rp1gem_rx_diag_bad_length++;
            if ((descriptor_control & 0xc000U) == 0xc000U &&
                received_length != 0 &&
                received_length <= RP1GEM_JUMBO_PROBE_SIZE)
                rp1gem_rx_diag_valid_frames++;
            rp1gem_netpoll_ring_used = 1;
            if ((descriptor_control & 0xc000U) == 0xc000U &&
                received_length != 0 &&
                received_length <= RP1GEM_JUMBO_PROBE_SIZE) {
                rp1gem_cache_invalidate(raw_buffer, received_length);
                status = vmk_PktAllocForDMAEngine(
                    received_length,
                    rp1gem_dma_engine, &packet);
                if (status == 0 && packet != 0 &&
                    vmk_PktIsFullyMapped(packet) != 0) {
                    mapped_pointer =
                        vmk_PktFrameMappedPointerGet(packet);
                    mapped_length =
                        vmk_PktFrameMappedLenGet(packet);
                }
                if (mapped_pointer != 0 &&
                    mapped_length >= received_length) {
                    memcpy(mapped_pointer, raw_buffer, received_length);
                    vmk_PktFrameLenSet(packet, received_length);
                    status = vmk_NetPollRxPktQueue(
                        rp1gem_netpoll_handle, packet);
                    if (status == 0) {
                        rp1gem_netpoll_ring_queued++;
                        rp1gem_netpoll_queued++;
                        rp1gem_netpoll_bytes += received_length;
                    } else {
                        rp1gem_netpoll_dropped++;
                    }
                } else {
                    if (packet != 0)
                        vmk_PktRelease(packet);
                    rp1gem_netpoll_dropped++;
                    status = status != 0 ? status : 1;
                }
            }
            rp1gem_netpoll_ring_status = status;
            rx_ring32[raw_index].ctrl = 0;
            rx_ring32[raw_index].addr = (uint32_t)raw_ma;
            if (raw_index == RP1GEM_RAW_RX_COUNT - 1U)
                rx_ring32[raw_index].addr |= RP1GEM_RX_WRAP;
            rp1gem_cache_clean(&rx_ring32[raw_index],
                               sizeof(rx_ring32[raw_index]));
            rp1gem_raw_rx_next =
                (raw_index + 1U) % RP1GEM_RAW_RX_COUNT;
            rp1gem_netpoll_ring_replaced++;
            rp1gem_raw_rx_events++;
            if (rp1gem_raw_rx_events <= 8U) {
                _vmk_WarningMessage(
                    "rp1gem_mmio: raw RX event=%u index=%u ctrl=%08x "
                    "length=%u mappedLen=%u status=%x queued=%u "
                    "rearmed=%u",
                    rp1gem_raw_rx_events, raw_index,
                    descriptor_control,
                    received_length, mapped_length, status,
                    status == 0, rp1gem_netpoll_ring_replaced);
            }
            processed++;
        }
        if (processed != raw_processed_start) {
            uint32_t raw_processed =
                processed - raw_processed_start;

            rp1gem_rx_diag_callbacks_with_rx++;
            if (raw_processed > rp1gem_rx_diag_max_processed)
                rp1gem_rx_diag_max_processed = raw_processed;
        }
        if (processed == budget)
            rp1gem_rx_diag_budget_exhausted++;
        return 0;
    }
    if (rx_ring32 != 0) {
        rp1gem_cache_invalidate(
            rx_ring32,
            RP1GEM_RING_COUNT * sizeof(struct rp1gem_dma_desc32));
        rp1gem_netpoll_ring_used = 0;
        for (ring_index = 0; ring_index < RP1GEM_RING_COUNT;
             ring_index++) {
            if ((rx_ring32[ring_index].addr &
                 RP1GEM_RX_USED) != 0)
                rp1gem_netpoll_ring_used++;
        }
        for (ring_index = 0;
             ring_index < RP1GEM_RING_COUNT && processed < budget;
             ring_index++) {
            uint32_t descriptor_control = rx_ring32[ring_index].ctrl;
            uint32_t received_length = descriptor_control & 0xfffU;
            struct rp1gem_sg_element *replacement_sg;
            uint64_t mapped_count = 0;
            void *packet;
            vmk_Status status;

            if ((rx_ring32[ring_index].addr &
                 RP1GEM_RX_USED) == 0 ||
                (descriptor_control & 0xc000U) != 0xc000U ||
                received_length == 0 ||
                rp1gem_rx_packets[ring_index] == 0)
                continue;

            status =
                vmk_DMAUnmapElem(
                    rp1gem_dma_engine, 1,
                    &rp1gem_rx_mapped_elements[ring_index]);
            if (status != 0) {
                rp1gem_netpoll_ring_status = status;
                break;
            }
            packet = rp1gem_rx_packets[ring_index];
            rp1gem_rx_packets[ring_index] = 0;
            vmk_PktFrameLenSet(packet, received_length);
            status =
                vmk_NetPollRxPktQueue(
                    rp1gem_netpoll_handle, packet);
            rp1gem_netpoll_ring_status = status;
            if (status == 0) {
                rp1gem_netpoll_ring_queued++;
                rp1gem_netpoll_queued++;
                rp1gem_netpoll_bytes += received_length;
            } else {
                rp1gem_netpoll_dropped++;
            }
            processed++;

            status =
                vmk_PktAllocForDMAEngine(
                    2048, rp1gem_dma_engine,
                    &rp1gem_rx_packets[ring_index]);
            if (status != 0 ||
                rp1gem_rx_packets[ring_index] == 0) {
                rp1gem_netpoll_ring_status =
                    status != 0 ? status : 1;
                break;
            }
            replacement_sg =
                vmk_PktSgElemGet(
                    rp1gem_rx_packets[ring_index], 0);
            if (replacement_sg == 0 ||
                replacement_sg->length < 2048 ||
                (replacement_sg->address >> 32) != 0) {
                rp1gem_netpoll_ring_status = 1;
                break;
            }
            status =
                vmk_DMAMapElem(
                    rp1gem_dma_engine, 1, replacement_sg, 1,
                    &rp1gem_rx_mapped_elements[ring_index],
                    &mapped_count);
            if (status != 0 ||
                rp1gem_rx_mapped_elements[ring_index].length < 2048 ||
                (rp1gem_rx_mapped_elements[ring_index].address >>
                 32) != 0) {
                rp1gem_netpoll_ring_status =
                    status != 0 ? status : 1;
                break;
            }
            rp1gem_rx_addresses[ring_index] =
                rp1gem_rx_mapped_elements[ring_index].address;
            rx_ring32[ring_index].addr =
                (uint32_t)rp1gem_rx_addresses[ring_index];
            if (ring_index == RP1GEM_RING_COUNT - 1)
                rx_ring32[ring_index].addr |= RP1GEM_RX_WRAP;
            rx_ring32[ring_index].ctrl = 0;
            rp1gem_cache_clean(
                &rx_ring32[ring_index],
                sizeof(struct rp1gem_dma_desc32));
            rp1gem_netpoll_ring_replaced++;
        }
    }
    return rp1gem_pending_rx_next < rp1gem_pending_rx_count;
}

static vmk_Status
rp1gem_create_persistent_netpoll(void)
{
    struct rp1gem_netpoll_props props = {
        .callback = rp1gem_empty_netpoll,
        .reserved0 = 0,
        .cookie = &rp1gem_adapter_context,
        .flags = 0,
        .reserved1 = 0,
    };
    uint64_t service_id = 0;
    void *netpoll = 0;
    vmk_Status service_status;
    vmk_Status create_status;

    service_status = vmk_ServiceGetID("net", &service_id);
    if (service_status == 0) {
        create_status = vmk_NetPollCreate(
            &props, service_id, vmk_ModuleCurrentID, &netpoll);
        if (create_status == 0 && netpoll != 0) {
            rp1gem_netpoll_handle = netpoll;
        }
    } else {
        create_status = (vmk_Status)-1;
    }

    _vmk_WarningMessage(
        "rp1gem_mmio: persistent NetPoll create service=%x id=%lx "
        "status=%x handle=%lx",
        service_status, service_id, create_status, netpoll);
    return create_status;
}

struct rp1gem_memory_pool_props {
    uint8_t name[32];
    uint32_t module_id;
    uint32_t reserved0;
    uint64_t parent;
    uint32_t type;
    uint32_t reserved1;
    uint64_t reserved2[2];
};

_Static_assert(sizeof(struct rp1gem_memory_pool_props) == 72,
               "unexpected memory-pool properties ABI size");

struct rp1gem_memory_range {
    uint64_t first_mpn;
    uint32_t page_count;
    uint32_t reserved;
};

struct rp1gem_memory_allocation {
    uint32_t page_count;
    uint32_t range_count;
    struct rp1gem_memory_range *ranges;
};

struct rp1gem_memory_constraints {
    uint32_t reserved0;
    uint32_t address_class;
    int32_t color;
    uint32_t reserved1;
};

struct rp1gem_map_props {
    uint32_t page_count;
    uint32_t reserved0;
    uint64_t map_count;
    uint32_t range_count;
    uint32_t reserved1;
    struct rp1gem_memory_range *ranges;
    uint64_t reserved2;
};

_Static_assert(sizeof(struct rp1gem_map_props) == 40,
               "unexpected map properties ABI size");

struct rp1gem_dma_constraints {
    uint64_t address_mask;
    uint64_t reserved[4];
};

static struct rp1gem_dma_constraints rp1gem_dma_constraints = {
    0xffffffffULL,
    { 0, 0, 0, 0 },
};

struct rp1gem_dma_engine_props {
    uint8_t name[32];
    uint32_t module_id;
    uint32_t flags;
    vmk_Device device;
    struct rp1gem_dma_constraints *constraints;
};

_Static_assert(sizeof(struct rp1gem_dma_engine_props) == 56,
               "unexpected DMA engine properties ABI size");

/*
 * Reconstructed from the stock ESXi-Arm genet driver. The native uplink
 * registration block starts with ABI version and module ID, followed by
 * eleven callbacks and one reserved callback slot.
 */
struct rp1gem_uplink_registration {
    uint32_t version;
    uint32_t module_id;
    void *ops[12];
    void *driver_data;
    void *adapter_data;
};

struct rp1gem_uplink_device_info {
    uint8_t modes[48];
};

_Static_assert(sizeof(struct rp1gem_uplink_device_info) == 48,
               "unexpected uplink device-info ABI size");

/*
 * Layout reconstructed from nativegenet_startDevice.  The VMkernel uplink
 * layer reads this block directly before invoking the driver callbacks.
 */
struct rp1gem_uplink_shared_data {
    uint32_t update_begin;
    uint32_t update_end;
    uint32_t flags;
    uint32_t state;
    uint32_t link_up;
    uint32_t link_speed;
    uint32_t link_duplex;
    uint32_t mtu;
    uint8_t current_mac[6];
    uint8_t permanent_mac[6];
    uint8_t reserved1[4];
    struct rp1gem_uplink_device_info *device_info;
    uint32_t device_info_revision;
    uint8_t reserved2[0];
    uint8_t driver_name[32];
    uint8_t driver_version[32];
    uint8_t firmware_version[32];
    uint8_t implementation[32];
    void *shared_queue_data;
};

_Static_assert(sizeof(struct rp1gem_uplink_shared_data) == 200,
               "unexpected uplink shared-data ABI size");
_Static_assert(__builtin_offsetof(struct rp1gem_uplink_shared_data, state) == 12,
               "unexpected uplink state ABI offset");
_Static_assert(__builtin_offsetof(struct rp1gem_uplink_shared_data, link_up) == 16,
               "unexpected uplink link-status ABI offset");
_Static_assert(__builtin_offsetof(struct rp1gem_uplink_shared_data, mtu) == 28,
               "unexpected uplink MTU ABI offset");

struct rp1gem_child_device_ops {
    vmk_Status (*remove)(vmk_Device);
};

struct rp1gem_child_props {
    vmk_Driver registering_driver;
    void *bus_address;
    struct rp1gem_child_device_ops *device_ops;
    void *registering_driver_data;
    void *registration_data;
    uint64_t bus_type;
    uint64_t logical_address;
    uint64_t logical_cookie;
    const char *identifier;
    uint32_t identifier_length;
    uint32_t reserved;
};

_Static_assert(sizeof(struct rp1gem_child_props) == 80,
               "unexpected child device properties ABI size");

static struct rp1gem_uplink_registration rp1gem_uplink_registration;
static struct rp1gem_uplink_device_info rp1gem_uplink_device_info;
static struct rp1gem_uplink_shared_data rp1gem_uplink_shared_data;

static void
rp1gem_log_uplink_shared_snapshot(void)
{
    _vmk_WarningMessage(
        "rp1gem_mmio: shared snapshot begin=%u end=%u flags=%x "
        "state=%u linkUp=%u speed=%u duplex=%u mtu=%u",
        rp1gem_uplink_shared_data.update_begin,
        rp1gem_uplink_shared_data.update_end,
        rp1gem_uplink_shared_data.flags,
        rp1gem_uplink_shared_data.state,
        rp1gem_uplink_shared_data.link_up,
        rp1gem_uplink_shared_data.link_speed,
        rp1gem_uplink_shared_data.link_duplex,
        rp1gem_uplink_shared_data.mtu);
}

struct rp1gem_shared_queue_header {
    uint64_t flags;
    uint64_t default_rx_queue_id;
    uint64_t default_tx_queue_id;
    uint32_t rx_queue_count;
    uint32_t tx_queue_count;
    uint8_t reserved1[24];
    void *queue_records;
};

struct rp1gem_shared_queue_record {
    uint32_t flags;
    uint32_t queue_type;
    uint64_t queue_id;
    uint32_t state;
    uint32_t poll_budget;
    uint32_t packet_budget;
    uint32_t reserved0;
    uint64_t reserved1;
    void *netpoll;
    void *adapter_context;
    uint8_t name_valid;
    uint8_t reserved2[3];
    uint8_t name[64];
    uint32_t reserved3;
};

_Static_assert(sizeof(struct rp1gem_shared_queue_header) == 64,
               "unexpected shared-queue header ABI size");
_Static_assert(sizeof(struct rp1gem_shared_queue_record) == 128,
               "unexpected shared-queue record ABI size");

static struct rp1gem_shared_queue_header rp1gem_shared_queue_header;
static struct rp1gem_shared_queue_record rp1gem_shared_queue_record;

static void
rp1gem_init_shared_queue(void)
{
    vmk_Status queue_id_status;

    memset(&rp1gem_shared_queue_header, 0,
           sizeof(rp1gem_shared_queue_header));
    memset(&rp1gem_shared_queue_record, 0,
           sizeof(rp1gem_shared_queue_record));
    rp1gem_shared_queue_header.flags = 3;
    rp1gem_shared_queue_header.default_rx_queue_id =
        UINT64_MAX;
    rp1gem_shared_queue_header.rx_queue_count = 1;
    rp1gem_shared_queue_header.tx_queue_count = 0;
    rp1gem_shared_queue_header.queue_records =
        &rp1gem_shared_queue_record;
    rp1gem_shared_queue_record.flags = 3;
    rp1gem_shared_queue_record.queue_type = 1;
    queue_id_status =
        vmk_UplinkQueueMkRxQueueID(
            &rp1gem_shared_queue_header.default_rx_queue_id,
            0, 0);
    rp1gem_shared_queue_record.queue_id =
        rp1gem_shared_queue_header.default_rx_queue_id;
    rp1gem_shared_queue_record.poll_budget = 1024;
    rp1gem_shared_queue_record.packet_budget = 1024;
    rp1gem_shared_queue_record.netpoll =
        rp1gem_netpoll_handle;
    rp1gem_shared_queue_record.adapter_context =
        &rp1gem_adapter_context;
    rp1gem_uplink_shared_data.shared_queue_data =
        &rp1gem_shared_queue_header;
    _vmk_WarningMessage(
        "rp1gem_mmio: RX queue init status=%x flags=%x type=%u "
        "headerFlags=%lx defaultRx=%lx queueID=%lx "
        "netpoll=%lx context=%lx",
        queue_id_status, rp1gem_shared_queue_record.flags,
        rp1gem_shared_queue_record.queue_type,
        rp1gem_shared_queue_header.flags,
        rp1gem_shared_queue_header.default_rx_queue_id,
        rp1gem_shared_queue_record.queue_id,
        rp1gem_shared_queue_record.netpoll,
        rp1gem_shared_queue_record.adapter_context);
}

static void
rp1gem_cache_clean(const void *address, uint32_t length)
{
    uintptr_t line =
        (uintptr_t)address & ~(uintptr_t)63U;
    uintptr_t end = (uintptr_t)address + length;

    for (; line < end; line += 64)
        __asm__ volatile("dc cvac, %0" : : "r"(line) : "memory");
    __asm__ volatile("dsb ish" ::: "memory");
}

static void
rp1gem_cache_invalidate(const void *address, uint32_t length)
{
    uintptr_t line =
        (uintptr_t)address & ~(uintptr_t)63U;
    uintptr_t end = (uintptr_t)address + length;

    for (; line < end; line += 64)
        __asm__ volatile("dc ivac, %0" : : "r"(line) : "memory");
    __asm__ volatile("dsb ish" ::: "memory");
}

static vmk_Status
rp1gem_uplink_cap_enable(void *adapter, uint32_t capability)
{
    (void)adapter;
    _vmk_WarningMessage(
        "rp1gem_mmio: uplink capability enable id=%u", capability);
    return (vmk_Status)0x0bad0001U;
}

static vmk_Status
rp1gem_uplink_cap_disable(void *adapter, uint32_t capability)
{
    (void)adapter;
    _vmk_WarningMessage(
        "rp1gem_mmio: uplink capability disable id=%u", capability);
    return 0;
}

__attribute__((unused)) static vmk_Status
rp1gem_uplink_tx_probe_legacy(void *adapter, void *packet_list)
{
    uint32_t packet_count;
    uint32_t frame_length = 0;
    uint32_t mapped_length = 0;
    uint32_t fully_mapped = 0;
    uint32_t sg_alignment = 0;
    struct rp1gem_sg_element *sg = 0;
    struct rp1gem_sg_element *bounce_sg = 0;
    struct rp1gem_sg_element bounce_mapped = { 0, 0, 0 };
    uint64_t bounce_mapped_count = 0;
    vmk_Status bounce_map_status = 1;
    vmk_Status bounce_unmap_status = 1;
    vmk_Status tx_start_status = 1;
    uint32_t copied = 0;
    uint32_t restored = 0;
    uint32_t tx_ncr = 0;
    uint32_t claimed = 0;
    uint32_t final_state;
    void *mapped_pointer = 0;
    void *packet = 0;

    (void)adapter;
    packet_count = packet_list != 0 ?
        vmk_PktListGetCount(packet_list) : 0;
    if (packet_count != 0 && rp1gem_dma_engine != 0) {
        claimed = rp1gem_tx_state_try_claim();
    }
    if (claimed != 0) {
        packet = vmk_PktListPopFirstPkt(packet_list);
        if (packet != 0) {
            frame_length = vmk_PktFrameLenGet(packet);
            sg = vmk_PktSgElemGet(packet, 0);
            sg_alignment = vmk_PktSgElemAlignment(packet, 0);
            fully_mapped = vmk_PktIsFullyMapped(packet);
            if (fully_mapped != 0) {
                mapped_pointer =
                    vmk_PktFrameMappedPointerGet(packet);
                mapped_length =
                    vmk_PktFrameMappedLenGet(packet);
            }
            if (mapped_pointer != 0 &&
                frame_length != 0 &&
                frame_length <= mapped_length &&
                frame_length <= rp1gem_tx_bounce_length &&
                rp1gem_tx_bounce_pointer != 0 &&
                rp1gem_tx_bounce_packet != 0) {
                memcpy(rp1gem_tx_bounce_pointer,
                       mapped_pointer, frame_length);
                rp1gem_cache_clean(
                    rp1gem_tx_bounce_pointer, frame_length);
                bounce_sg =
                    vmk_PktSgElemGet(rp1gem_tx_bounce_packet, 0);
                if (bounce_sg != 0) {
                    bounce_map_status =
                        vmk_DMAMapElem(
                            rp1gem_dma_engine, 2, bounce_sg, 1,
                            &bounce_mapped, &bounce_mapped_count);
                    if (bounce_map_status == 0) {
                        struct rp1gem_dma_desc32 *tx_ring32 =
                            (struct rp1gem_dma_desc32 *)
                            rp1gem_tx_ring;

                        if (tx_ring32 != 0 &&
                            bounce_mapped.length >= frame_length &&
                            (bounce_mapped.address >> 32) == 0) {
                            rp1gem_tx_mapped_element =
                                bounce_mapped;
                            tx_ring32[0].addr =
                                (uint32_t)bounce_mapped.address;
                            tx_ring32[0].ctrl =
                                (frame_length &
                                 RP1GEM_TX_LENGTH_MASK) |
                                RP1GEM_TX_LAST;
                            rp1gem_cache_clean(
                                &tx_ring32[0],
                                sizeof(tx_ring32[0]));
                            tx_start_status =
                                vmk_MappedResourceRead32(
                                    rp1gem_gem_resource, 0x000,
                                    &tx_ncr);
                            if (tx_start_status == 0)
                                tx_start_status =
                                    vmk_MappedResourceWrite32(
                                        rp1gem_gem_resource, 0x000,
                                        tx_ncr | (1U << 3) |
                                            (1U << 9));
                            if (tx_start_status == 0) {
                                rp1gem_tx_packet = packet;
                                rp1gem_tx_bytes =
                                    frame_length;
                                rp1gem_tx_state_store(2);
                            } else {
                                tx_ring32[0].ctrl =
                                    RP1GEM_TX_USED;
                                rp1gem_cache_clean(
                                    &tx_ring32[0],
                                    sizeof(tx_ring32[0]));
                            }
                        }
                        if (tx_start_status != 0) {
                            bounce_unmap_status =
                                vmk_DMAUnmapElem(
                                    rp1gem_dma_engine, 2,
                                    &bounce_mapped);
                        }
                    }
                }
                copied = 1;
            }
            if (rp1gem_tx_state_load() != 2) {
                vmk_PktListPrependPkt(packet_list, packet);
                restored = 1;
                rp1gem_tx_state_store(1);
            }
        } else {
            rp1gem_tx_state_store(1);
        }
    }
    final_state = rp1gem_tx_state_load();
    rp1gem_tx_probe_last_count = packet_count;
    rp1gem_tx_probe_calls++;
    if (rp1gem_tx_probe_calls == 1) {
        _vmk_WarningMessage(
            "rp1gem_mmio: TX bounce-copy probe list=%u state=%u "
            "frameLen=%u mappedLen=%u bounceLen=%u copied=%u "
            "restored=%u",
            packet_count, final_state,
            frame_length, mapped_length,
            rp1gem_tx_bounce_length, copied,
            restored);
        _vmk_WarningMessage(
            "rp1gem_mmio: TX bounce-DMA probe fullyMapped=%u "
            "alignment=%u sgLen=%u mapStatus=%x mappedCount=%lu "
            "mappedAddr=%lx mappedLen=%u txStart=%x unmapStatus=%x",
            fully_mapped, sg_alignment,
            sg != 0 ? sg->length : 0,
            bounce_map_status, bounce_mapped_count,
            bounce_mapped.address, bounce_mapped.length,
            tx_start_status, bounce_unmap_status);
    }
    if (packet != 0 && copied == 0) {
        _vmk_WarningMessage(
            "rp1gem_mmio: TX bounce reject list=%u state=%u "
            "frameLen=%u fullyMapped=%u mappedLen=%u sgLen=%u "
            "bounceLen=%u restored=%u",
            packet_count, final_state, frame_length,
            fully_mapped, mapped_length,
            sg != 0 ? sg->length : 0,
            rp1gem_tx_bounce_length, restored);
    }

    /*
     * Success transfers ownership of the single popped packet to the
     * polling completion path. Otherwise VMK_BUSY is returned only after
     * restoring the packet to the caller's list.
     */
    return final_state == 2 && packet_count == 1 ?
        0 : (vmk_Status)0x0bad0004U;
}

static void
rp1gem_tx_kick(void)
{
    struct rp1gem_dma_desc32 *tx_ring32;
    uint32_t frame_length = 0;
    uint32_t submitted = 0;
    uint32_t tx_index;
    uint32_t tx_ncr = 0;
    vmk_Status raw_status = 1;
    vmk_Status start_status = 1;

    if (rp1gem_tx_state_try_claim() == 0) {
        rp1gem_tx_diag_kick_busy++;
        return;
    }
    if (rp1gem_jumbo_tx_probe_pages == 0 ||
        rp1gem_jumbo_tx_probe_ma == 0 ||
        (rp1gem_jumbo_tx_probe_ma >> 32) != 0) {
        rp1gem_tx_state_store(3);
        _vmk_WarningMessage(
            "rp1gem_mmio: TX raw buffer unavailable va=%lx ma=%lx",
            rp1gem_jumbo_tx_probe_pages,
            rp1gem_jumbo_tx_probe_ma);
        return;
    }

    tx_ring32 = (struct rp1gem_dma_desc32 *)rp1gem_tx_ring;
    if (tx_ring32 != 0) {
        rp1gem_tx_sw_lock_acquire();
        while (rp1gem_tx_sw_count != 0 &&
               submitted < RP1GEM_TX_HW_COUNT) {
            void *tx_buffer;
            uint64_t tx_buffer_ma;

            /* Every TXEN/TSTART batch begins at the programmed TXQBASE. */
            tx_index = submitted;
            frame_length =
                rp1gem_tx_sw_lengths[rp1gem_tx_sw_head];
            tx_buffer =
                (uint8_t *)rp1gem_jumbo_tx_probe_pages +
                tx_index * RP1GEM_TX_BUFFER_STRIDE;
            tx_buffer_ma = rp1gem_jumbo_tx_probe_ma +
                tx_index * RP1GEM_TX_BUFFER_STRIDE;
            memcpy(tx_buffer,
                   rp1gem_tx_sw_queue[rp1gem_tx_sw_head],
                   frame_length);
            rp1gem_cache_clean(tx_buffer, frame_length);
            rp1gem_tx_sw_head =
                (rp1gem_tx_sw_head + 1) %
                RP1GEM_TX_SW_QUEUE_COUNT;
            rp1gem_tx_sw_count--;
            rp1gem_tx_hw_lengths[tx_index] = frame_length;
            tx_ring32[tx_index].addr = (uint32_t)tx_buffer_ma;
            tx_ring32[tx_index].ctrl =
                (frame_length & RP1GEM_TX_LENGTH_MASK) |
                RP1GEM_TX_LAST |
                (tx_index == RP1GEM_TX_HW_COUNT - 1U ?
                 RP1GEM_TX_WRAP : 0);
            submitted++;
        }
        rp1gem_tx_sw_lock_release();
    }
    if (submitted == 0) {
        rp1gem_tx_diag_kick_empty++;
        rp1gem_tx_state_store(0);
        return;
    }

    raw_status = 0;
    rp1gem_cache_clean(
        tx_ring32, RP1GEM_TX_HW_COUNT * sizeof(tx_ring32[0]));
    if (raw_status == 0) {
        start_status =
            vmk_MappedResourceRead32(
                rp1gem_gem_resource, 0x000, &tx_ncr);
        if (start_status == 0)
            start_status =
                vmk_MappedResourceWrite32(
                    rp1gem_gem_resource, 0x000,
                    tx_ncr | (1U << 3) | (1U << 9));
        if (start_status == 0) {
            rp1gem_tx_bytes = frame_length;
            rp1gem_tx_poll_age = 0;
            rp1gem_tx_raw_buffer_active = 1;
            rp1gem_tx_hw_inflight = submitted;
            rp1gem_tx_diag_submitted += submitted;
            rp1gem_tx_state_store(2);
            return;
        }
        for (tx_index = 0; tx_index < RP1GEM_TX_HW_COUNT;
             tx_index++) {
            tx_ring32[tx_index].ctrl = RP1GEM_TX_USED |
                (tx_index == RP1GEM_TX_HW_COUNT - 1U ?
                 RP1GEM_TX_WRAP : 0);
            rp1gem_tx_hw_lengths[tx_index] = 0;
        }
        rp1gem_cache_clean(
            tx_ring32, RP1GEM_TX_HW_COUNT * sizeof(tx_ring32[0]));
    }
    rp1gem_tx_sw_lock_acquire();
    rp1gem_tx_sw_dropped += submitted + rp1gem_tx_sw_count;
    rp1gem_tx_sw_count = 0;
    rp1gem_tx_sw_head = 0;
    rp1gem_tx_sw_tail = 0;
    rp1gem_tx_hw_head = 0;
    rp1gem_tx_hw_inflight = 0;
    rp1gem_tx_sw_lock_release();
    rp1gem_tx_state_store(3);
    _vmk_WarningMessage(
        "rp1gem_mmio: TX software queue halted "
        "frameLen=%u batch=%u rawStatus=%x startStatus=%x dropped=%u",
        frame_length, submitted, raw_status, start_status,
        rp1gem_tx_sw_dropped);
}

static vmk_Status
rp1gem_uplink_tx_probe(void *adapter, void *packet_list)
{
    uint32_t initial_count;
    uint32_t accepted = 0;
    uint32_t remaining;

    (void)adapter;
    initial_count = packet_list != 0 ?
        vmk_PktListGetCount(packet_list) : 0;
    if (initial_count == 0 || rp1gem_tx_state_load() == 3) {
        if (initial_count != 0)
            rp1gem_tx_diag_state3_rejects += initial_count;
        return initial_count == 0 ?
            0 : (vmk_Status)0x0bad0004U;
    }

    rp1gem_tx_sw_lock_acquire();
    while (rp1gem_tx_sw_count < RP1GEM_TX_SW_QUEUE_COUNT &&
           vmk_PktListGetCount(packet_list) != 0) {
        void *packet = vmk_PktListPopFirstPkt(packet_list);
        uint32_t frame_length;
        uint32_t fully_mapped;
        uint32_t mapped_length;
        void *mapped_pointer;

        if (packet == 0)
            break;
        frame_length = vmk_PktFrameLenGet(packet);
        fully_mapped = vmk_PktIsFullyMapped(packet);
        mapped_pointer =
            fully_mapped != 0 ?
            vmk_PktFrameMappedPointerGet(packet) : 0;
        mapped_length =
            fully_mapped != 0 ?
            vmk_PktFrameMappedLenGet(packet) : 0;
        if (mapped_pointer == 0 || frame_length == 0 ||
            frame_length > mapped_length ||
            frame_length > RP1GEM_JUMBO_PROBE_SIZE) {
            vmk_PktListPrependPkt(packet_list, packet);
            break;
        }
        memcpy(rp1gem_tx_sw_queue[rp1gem_tx_sw_tail],
               mapped_pointer, frame_length);
        rp1gem_tx_sw_lengths[rp1gem_tx_sw_tail] = frame_length;
        rp1gem_tx_sw_tail =
            (rp1gem_tx_sw_tail + 1) %
            RP1GEM_TX_SW_QUEUE_COUNT;
        rp1gem_tx_sw_count++;
        if (rp1gem_tx_sw_count > rp1gem_tx_diag_max_depth)
            rp1gem_tx_diag_max_depth = rp1gem_tx_sw_count;
        accepted++;
        vmk_PktRelease(packet);
    }
    remaining = vmk_PktListGetCount(packet_list);
    if (remaining != 0) {
        rp1gem_tx_diag_queue_full++;
        if (remaining > rp1gem_tx_diag_max_remaining)
            rp1gem_tx_diag_max_remaining = remaining;
    }
    rp1gem_tx_sw_lock_release();

    rp1gem_tx_probe_last_count = initial_count;
    rp1gem_tx_probe_calls++;
    if (rp1gem_tx_probe_calls == 1) {
        _vmk_WarningMessage(
            "rp1gem_mmio: TX software queue probe "
            "list=%u accepted=%u remaining=%u depth=%u capacity=%u",
            initial_count, accepted, remaining,
            rp1gem_tx_sw_count, RP1GEM_TX_SW_QUEUE_COUNT);
    }
    rp1gem_tx_kick();
    return remaining == 0 ?
        0 : (vmk_Status)0x0bad0004U;
}

static vmk_Status
rp1gem_uplink_mtu_set(void *adapter, uint32_t mtu)
{
    (void)adapter;
    rp1gem_uplink_shared_data.update_begin++;
    rp1gem_uplink_shared_data.mtu = mtu;
    rp1gem_uplink_shared_data.update_end =
        rp1gem_uplink_shared_data.update_begin;
    _vmk_WarningMessage("rp1gem_mmio: uplink MTU set %u", mtu);
    return 0;
}

struct rp1gem_link_status {
    uint32_t up;
    uint32_t speed;
    uint32_t duplex;
};

struct rp1gem_advertised_mode {
    uint32_t speed;
    uint32_t duplex;
    uint32_t flags;
};

struct rp1gem_advertised_mode_ops {
    vmk_Status (*get)(void *adapter, struct rp1gem_advertised_mode *modes,
                      uint32_t *count);
    vmk_Status (*set)(void *adapter,
                      const struct rp1gem_advertised_mode *modes,
                      uint32_t count);
};

static const struct rp1gem_advertised_mode rp1gem_advertised_modes[] = {
    /* Exact copper-mode ABI reconstructed from the stock ntg3 driver. */
    { 1000U, 2U, 2U },
    { 100U, 2U, 2U },
    { 10U, 2U, 2U },
    { 0U, UINT32_MAX, 1U },
};
static volatile uint32_t rp1gem_advertised_modes_get_calls;
static volatile uint32_t rp1gem_advertised_modes_set_calls;

static vmk_Status
rp1gem_advertised_modes_get(void *adapter,
                            struct rp1gem_advertised_mode *modes,
                            uint32_t *count)
{
    uint32_t copy_count;
    uint32_t requested_count;
    uint32_t mode_count =
        sizeof(rp1gem_advertised_modes) /
        sizeof(rp1gem_advertised_modes[0]);

    (void)adapter;
    if (count == 0)
        return 1;
    requested_count = *count;
    copy_count = *count < mode_count ? *count : mode_count;
    if (copy_count != 0 && modes == 0)
        return 1;
    if (copy_count != 0)
        memcpy(modes, rp1gem_advertised_modes,
               copy_count * sizeof(rp1gem_advertised_modes[0]));
    *count = copy_count;
    rp1gem_advertised_modes_get_calls++;
    if (rp1gem_advertised_modes_get_calls <= 8U) {
        _vmk_WarningMessage(
            "rp1gem_mmio: advertised modes get call=%u "
            "requested=%u returned=%u",
            rp1gem_advertised_modes_get_calls,
            requested_count, copy_count);
    }
    return 0;
}

static vmk_Status
rp1gem_advertised_modes_set(void *adapter,
                            const struct rp1gem_advertised_mode *modes,
                            uint32_t count)
{
    (void)adapter;
    rp1gem_advertised_modes_set_calls++;
    if (rp1gem_advertised_modes_set_calls <= 8U) {
        _vmk_WarningMessage(
            "rp1gem_mmio: advertised modes set call=%u count=%u "
            "speed=%u duplex=%u flags=%x",
            rp1gem_advertised_modes_set_calls, count,
            modes != 0 && count != 0 ? modes[0].speed : 0,
            modes != 0 && count != 0 ? modes[0].duplex : 0,
            modes != 0 && count != 0 ? modes[0].flags : 0);
    }
    return 0x0bad0020U;
}

static const struct rp1gem_advertised_mode_ops
rp1gem_advertised_mode_ops = {
    rp1gem_advertised_modes_get,
    rp1gem_advertised_modes_set,
};

static vmk_Status rp1gem_uplink_quiesce_io(void *adapter);
static vmk_Status rp1gem_uplink_start_io(void *adapter);

static vmk_Status
rp1gem_uplink_state_set(void *adapter, uint32_t state)
{
    (void)adapter;
    rp1gem_uplink_shared_data.update_begin++;
    __asm__ volatile("dmb ishst" ::: "memory");
    rp1gem_uplink_shared_data.state = state;
    __asm__ volatile("dmb ishst" ::: "memory");
    rp1gem_uplink_shared_data.update_end =
        rp1gem_uplink_shared_data.update_begin;
    _vmk_WarningMessage("rp1gem_mmio: uplink state set %x", state);
    return 0;
}

static vmk_Status
rp1gem_uplink_stats_get(void *adapter, void *stats)
{
    struct rp1gem_uplink_stats *uplink_stats =
        (struct rp1gem_uplink_stats *)stats;

    (void)adapter;
    if (uplink_stats != 0) {
        memset(uplink_stats, 0, sizeof(*uplink_stats));
        uplink_stats->rx_packets = rp1gem_netpoll_queued;
        uplink_stats->tx_packets = rp1gem_tx_completed;
        uplink_stats->rx_bytes = rp1gem_netpoll_bytes;
        uplink_stats->tx_bytes = rp1gem_tx_total_bytes;
        uplink_stats->rx_dropped = rp1gem_netpoll_dropped;
        uplink_stats->tx_dropped = rp1gem_tx_sw_dropped;
    }
    if (rp1gem_netpoll_activate_enter !=
            rp1gem_netpoll_activate_return &&
        rp1gem_netpoll_activate_mismatch_logged == 0) {
        rp1gem_netpoll_activate_mismatch_logged = 1;
        _vmk_WarningMessage(
            "rp1gem_mmio: NetPoll activate outstanding enter=%u return=%u "
            "worldCalls=%u status=%x",
            rp1gem_netpoll_activate_enter,
            rp1gem_netpoll_activate_return,
            rp1gem_world_calls,
            rp1gem_world_netpoll_status);
    }
    return 0;
}

static vmk_Status
rp1gem_link_status_set(void *adapter,
                       const struct rp1gem_link_status *link)
{
    vmk_Status status = 1;
    vmk_Status restore_status = 0;
    vmk_Status resolve_status = 0;
    vmk_Status confirm_status = 0;
    uint32_t ncr = 0;
    uint32_t ncfgr = 0;
    uint32_t bmcr = 0;
    uint32_t readback = 0;
    uint32_t bmsr = 0;
    uint32_t lpa = 0;
    uint32_t stat1000 = 0;
    uint32_t lpa_confirm = 0;
    uint32_t stat1000_confirm = 0;
    uint32_t resolve_stable = 0;
    uint32_t resolved_speed = 0;
    uint32_t publish_speed;
    uint32_t requested_speed;
    uint32_t management_enabled = 0;
    struct rp1gem_link_status published;

    (void)adapter;
    if (link == 0 || rp1gem_gem_resource_mapped == 0 ||
        rp1gem_phy_address == 0xffffffffU)
        return 1;
    requested_speed = link->speed;
    if (link->up == 0U) {
        vmk_Status quiesce_status;

        if (rp1gem_auto_mode_selected != 0U)
            rp1gem_auto_restore_pending = 1U;
        quiesce_status = rp1gem_uplink_quiesce_io(adapter);
        _vmk_WarningMessage(
            "rp1gem_mmio: administrative down quiesce status=%x",
            quiesce_status);
        return quiesce_status;
    }
    if (rp1gem_auto_restore_pending != 0U &&
        rp1gem_auto_mode_selected != 0U && link->speed == 1000U) {
        requested_speed = 0U;
        rp1gem_auto_restore_pending = 0U;
        _vmk_WarningMessage(
            "rp1gem_mmio: restoring selected Auto mode after "
            "administrative down/up");
    } else {
        rp1gem_auto_restore_pending = 0U;
        rp1gem_auto_mode_selected = (link->speed == 0U) ? 1U : 0U;
    }
    publish_speed = requested_speed;
    status = vmk_MappedResourceRead32(
        rp1gem_gem_resource, 0x000, &ncr);
    if (status == 0) {
        status = vmk_MappedResourceWrite32(
            rp1gem_gem_resource, 0x000, ncr | (1U << 4));
        if (status == 0)
            management_enabled = 1;
    }
    if (status == 0)
        status = vmk_MappedResourceRead32(
            rp1gem_gem_resource, 0x004, &ncfgr);
    if (status == 0 && requested_speed == 0U) {
        status = rp1gem_mdio_write(
            rp1gem_gem_resource, rp1gem_phy_address, 4, 0x0141U);
        if (status == 0)
            status = rp1gem_mdio_write(
                rp1gem_gem_resource, rp1gem_phy_address, 9, 0x0200U);
        bmcr = 0x1200U;
        publish_speed = (rp1gem_auto_resolved_speed != 0U) ?
            rp1gem_auto_resolved_speed : 1000U;
    } else if (status == 0 && requested_speed == 1000U &&
               link->duplex == 2U) {
        status = rp1gem_mdio_write(
            rp1gem_gem_resource, rp1gem_phy_address, 4, 0x0001U);
        if (status == 0)
            status = rp1gem_mdio_write(
                rp1gem_gem_resource, rp1gem_phy_address, 9, 0x0200U);
        bmcr = 0x1200U;
    } else if (status == 0 && requested_speed == 100U &&
               link->duplex == 2U) {
        bmcr = 0x2100U;
    } else if (status == 0 && requested_speed == 10U &&
               link->duplex == 2U) {
        bmcr = 0x0100U;
    } else {
        status = 1;
    }
    if (status == 0)
        status = rp1gem_mdio_write(
            rp1gem_gem_resource, rp1gem_phy_address, 0, bmcr);
    if (status == 0)
        status = rp1gem_mdio_read(
            rp1gem_gem_resource, rp1gem_phy_address, 0, &readback);
    if (status == 0 && requested_speed == 0U && link->speed == 0U) {
        vmk_DelayUsecs(3000000U);
        resolve_status = rp1gem_mdio_read(
            rp1gem_gem_resource, rp1gem_phy_address, 1, &bmsr);
        if (resolve_status == 0)
            resolve_status = rp1gem_mdio_read(
                rp1gem_gem_resource, rp1gem_phy_address, 1, &bmsr);
        if (resolve_status == 0)
            resolve_status = rp1gem_mdio_read(
                rp1gem_gem_resource, rp1gem_phy_address, 5, &lpa);
        if (resolve_status == 0)
            resolve_status = rp1gem_mdio_read(
                rp1gem_gem_resource, rp1gem_phy_address, 10,
                &stat1000);
        if (resolve_status == 0) {
            vmk_DelayUsecs(50000U);
            confirm_status = rp1gem_mdio_read(
                rp1gem_gem_resource, rp1gem_phy_address, 5,
                &lpa_confirm);
            if (confirm_status == 0)
                confirm_status = rp1gem_mdio_read(
                    rp1gem_gem_resource, rp1gem_phy_address, 10,
                    &stat1000_confirm);
            resolve_stable = (confirm_status == 0 &&
                              lpa == lpa_confirm &&
                              stat1000 == stat1000_confirm) ? 1U : 0U;
            if ((stat1000 & 0x0800U) != 0U)
                resolved_speed = 1000U;
            else if ((lpa & 0x0100U) != 0U)
                resolved_speed = 100U;
            else if ((lpa & 0x0040U) != 0U)
                resolved_speed = 10U;
            if (resolved_speed != 0U && resolve_stable != 0U) {
                rp1gem_auto_resolved_speed = resolved_speed;
                publish_speed = resolved_speed;
            }
            _vmk_WarningMessage(
                "rp1gem_mmio: Auto IEEE diagnostic bmsr=%04x "
                "lpa=%04x stat1000=%04x link=%u anComplete=%u "
                "partner1000Full=%u partner100Full=%u partner10Full=%u "
                "resolvedSpeed=%u lpa2=%04x stat1000_2=%04x stable=%u",
                bmsr, lpa, stat1000, (bmsr >> 2) & 1U,
                (bmsr >> 5) & 1U, (stat1000 >> 11) & 1U,
                (lpa >> 8) & 1U, (lpa >> 6) & 1U, resolved_speed,
                lpa_confirm, stat1000_confirm, resolve_stable);
        } else {
            _vmk_WarningMessage(
                "rp1gem_mmio: Auto IEEE resolve skipped status=%x "
                "cachedSpeed=%u fallbackSpeed=%u",
                resolve_status, rp1gem_auto_resolved_speed,
                publish_speed);
        }
    }
    if (status == 0) {
        ncfgr &= ~((1U << 10) | (1U << 0));
        ncfgr |= (1U << 1);
        if (publish_speed == 1000U)
            ncfgr |= (1U << 10);
        else if (publish_speed == 100U)
            ncfgr |= (1U << 0);
        status = vmk_MappedResourceWrite32(
            rp1gem_gem_resource, 0x004, ncfgr);
    }
    if (management_enabled != 0) {
        restore_status = vmk_MappedResourceWrite32(
            rp1gem_gem_resource, 0x000, ncr);
        if (status == 0 && restore_status != 0)
            status = restore_status;
    }
    if (status != 0) {
        _vmk_WarningMessage(
            "rp1gem_mmio: PHY mode request failed speed=%u "
            "duplex=%u status=%x restoreStatus=%x",
            requested_speed, link->duplex, status, restore_status);
        return status;
    }
    if (link->up == 0U) {
        _vmk_WarningMessage(
            "rp1gem_mmio: administrative down keeps physical link "
            "published up speed=%u BMCR=%04x readback=%04x",
            publish_speed, bmcr, readback);
        return 0;
    }
    rp1gem_uplink_shared_data.update_begin++;
    __asm__ volatile("dmb ishst" ::: "memory");
    rp1gem_uplink_shared_data.link_up = link->up;
    rp1gem_uplink_shared_data.link_speed = publish_speed;
    rp1gem_uplink_shared_data.link_duplex = 2U;
    __asm__ volatile("dmb ishst" ::: "memory");
    rp1gem_uplink_shared_data.update_end =
        rp1gem_uplink_shared_data.update_begin;
    published.up = link->up;
    published.speed = publish_speed;
    published.duplex = 2U;
    status = vmk_UplinkUpdateLinkState(
        rp1gem_uplink_handle, &published);
    _vmk_WarningMessage(
        "rp1gem_mmio: PHY mode request up=%u requestedSpeed=%u "
        "publishedSpeed=%u BMCR=%04x readback=%04x "
        "updateStatus=%x",
        link->up, requested_speed, publish_speed, bmcr, readback, status);
    if (status == 0 && rp1gem_world_id == 0) {
        vmk_Status restart_status;

        rp1gem_link_restart_active = 1;
        restart_status = rp1gem_uplink_start_io(adapter);
        rp1gem_link_restart_active = 0;

        _vmk_WarningMessage(
            "rp1gem_mmio: administrative up restart status=%x "
            "worldId=%u worldRunning=%u netpollEnabled=%u",
            restart_status, rp1gem_world_id, rp1gem_world_running,
            rp1gem_netpoll_enabled);
        if (restart_status != 0)
            status = restart_status;
    }
    return status;
}

static vmk_Status
rp1gem_mac_address_set(void *adapter, const uint8_t *mac)
{
    (void)adapter;
    if (mac == 0)
        return 1;
    rp1gem_uplink_shared_data.update_begin++;
    memcpy(rp1gem_uplink_shared_data.current_mac, mac, 6);
    rp1gem_uplink_shared_data.update_end =
        rp1gem_uplink_shared_data.update_begin;
    return 0;
}

static vmk_Status
rp1gem_uplink_associate(void *adapter, void *uplink)
{
    vmk_Status status;

    rp1gem_uplink_handle = uplink;
    _vmk_WarningMessage("rp1gem_mmio: uplink associate uplink=%lx",
                        uplink);

    status = vmk_UplinkCapRegister(
        uplink, 15, (void *)rp1gem_link_status_set);
    _vmk_WarningMessage(
        "rp1gem_mmio: register link-status capability status=%x", status);
    if (status == 0) {
        status = vmk_UplinkCapRegister(
            uplink, 16, (void *)rp1gem_mac_address_set);
        _vmk_WarningMessage(
            "rp1gem_mmio: register MAC capability status=%x", status);
    }
    if (status != 0)
        return status;

    status = vmk_UplinkCapRegister(
        uplink, 39, (void *)&rp1gem_advertised_mode_ops);
    _vmk_WarningMessage(
        "rp1gem_mmio: register advertised-modes capability status=%x",
        status);
    if (status != 0)
        return status;

    (void)adapter;
    return 0;
}

static vmk_Status
rp1gem_uplink_disassociate(void *adapter)
{
    (void)adapter;
    rp1gem_uplink_handle = 0;
    _vmk_WarningMessage("rp1gem_mmio: uplink disassociate");
    return 0;
}

static vmk_Status
rp1gem_uplink_start_io(void *adapter)
{
    struct rp1gem_link_status link = {
        .up = 1,
        .speed = 1000,
        .duplex = 2,
    };
    vmk_Status status;
    vmk_Status world_status;
    uint8_t world_name[32] __attribute__((aligned(8))) = { 0 };
    struct rp1gem_world_props world_props = {
        world_name,
        0,
        0,
        rp1gem_poll_world_probe,
        &rp1gem_adapter_context,
        1,
        0,
        0,
    };
    vmk_Status mmio_status = 1;
    uint32_t mmio_ncr = 0;
    uint32_t ring_index;
    uint32_t startup_wait_loops;

    _vmk_WarningMessage(
        "rp1gem_mmio: lifecycle StartIO adapter=%lx",
        (uint64_t)adapter);
    if (rp1gem_world_id != 0 && rp1gem_world_running != 0) {
        _vmk_WarningMessage(
            "rp1gem_mmio: lifecycle StartIO already active "
            "worldId=%u netpollEnabled=%u",
            rp1gem_world_id, rp1gem_netpoll_enabled);
        return 0;
    }
    struct rp1gem_dma_desc32 *rx_ring32 =
        (struct rp1gem_dma_desc32 *)rp1gem_rx_ring;
    (void)adapter;
    if (rp1gem_gem_resource_mapped != 0)
        mmio_status =
            vmk_MappedResourceRead32(
                rp1gem_gem_resource, RP1_GEM_OFFSET, &mmio_ncr);
    _vmk_WarningMessage(
        "rp1gem_mmio: persistent GEM mapping startIO status=%x "
        "mapped=%u NCR=%08x",
        mmio_status, rp1gem_gem_resource_mapped, mmio_ncr);
    if (mmio_status != 0)
        return mmio_status;

    if (rx_ring32 == 0)
        return 1;
    for (ring_index = 0; ring_index < RP1GEM_RING_COUNT;
         ring_index++) {
        rx_ring32[ring_index].ctrl = 0;
        rx_ring32[ring_index].addr &= ~RP1GEM_RX_USED;
    }
    rp1gem_cache_clean(
        rx_ring32,
        RP1GEM_RING_COUNT * sizeof(struct rp1gem_dma_desc32));
    mmio_status =
        vmk_MappedResourceWrite32(
            rp1gem_gem_resource, 0x088, 0x31505202);
    if (mmio_status == 0)
        mmio_status =
            vmk_MappedResourceWrite32(
                rp1gem_gem_resource, 0x08c, 0x00000100);
    if (mmio_status == 0)
        mmio_status =
            vmk_MappedResourceWrite32(
                rp1gem_gem_resource, 0x004,
                rp1gem_saved_ncfgr | (1U << 10) | (1U << 1));
    if (mmio_status == 0)
        mmio_status =
            vmk_MappedResourceWrite32(
                rp1gem_gem_resource, 0x010,
                (rp1gem_saved_dmacfg & ~(0xffU << 16)) |
                    (32U << 16));
    if (mmio_status == 0)
        mmio_status =
            vmk_MappedResourceWrite32(
                rp1gem_gem_resource, 0x020, 0x0f);
    if (mmio_status == 0)
        mmio_status =
            vmk_MappedResourceWrite32(
                rp1gem_gem_resource, 0x000,
                rp1gem_saved_ncr | (1U << 2));
    if (mmio_status == 0 && rp1gem_link_restart_active == 0)
        vmk_DelayUsecs(1000000);
    (void)vmk_MappedResourceWrite32(
        rp1gem_gem_resource, 0x000, rp1gem_saved_ncr);
    vmk_DelayUsecs(1000);
    (void)vmk_MappedResourceWrite32(
        rp1gem_gem_resource, 0x004, rp1gem_saved_ncfgr);
    (void)vmk_MappedResourceWrite32(
        rp1gem_gem_resource, 0x010, rp1gem_saved_dmacfg);
    (void)vmk_MappedResourceWrite32(
        rp1gem_gem_resource, 0x088, rp1gem_saved_sa1b);
    (void)vmk_MappedResourceWrite32(
        rp1gem_gem_resource, 0x08c, rp1gem_saved_sa1t);
    _vmk_WarningMessage(
        "rp1gem_mmio: startIO bounded RX status=%x durationUsec=1000000",
        mmio_status);
    if (mmio_status != 0)
        return mmio_status;

    rp1gem_init_shared_queue();
    _vmk_WarningMessage(
        "rp1gem_mmio: uplink startIO logical-only sharedQueueData=%lx",
        rp1gem_uplink_shared_data.shared_queue_data);

    if (rp1gem_netpoll_handle != 0 &&
        rp1gem_netpoll_enabled == 0) {
        status = vmk_NetPollEnable(rp1gem_netpoll_handle);
        if (status == 0)
            rp1gem_netpoll_enabled = 1;
        _vmk_WarningMessage(
            "rp1gem_mmio: persistent NetPoll enable status=%x "
            "handle=%lx queueRecord=%lx",
            status, rp1gem_netpoll_handle,
            &rp1gem_shared_queue_record);
        if (status != 0)
            return status;

        status = vmk_NetPollActivate(rp1gem_netpoll_handle);
        vmk_DelayUsecs(100000);
        _vmk_WarningMessage(
            "rp1gem_mmio: associated NetPoll activate status=%x "
            "calls=%u budget=%u queued=%u ringUsed=%u "
            "ringQueued=%u ringReplaced=%u ringStatus=%x",
            status, rp1gem_netpoll_calls, rp1gem_netpoll_budget,
            rp1gem_netpoll_queued, rp1gem_netpoll_ring_used,
            rp1gem_netpoll_ring_queued,
            rp1gem_netpoll_ring_replaced,
            rp1gem_netpoll_ring_status);
        if (status != 0)
            return status;

        for (ring_index = 0; ring_index < RP1GEM_RING_COUNT;
             ring_index++) {
            rx_ring32[ring_index].ctrl = 0;
            rx_ring32[ring_index].addr &= ~RP1GEM_RX_USED;
        }
        rp1gem_cache_clean(
            rx_ring32,
            RP1GEM_RING_COUNT *
                sizeof(struct rp1gem_dma_desc32));
        mmio_status =
            vmk_MappedResourceWrite32(
                rp1gem_gem_resource, 0x088, 0x31505202);
        if (mmio_status == 0)
            mmio_status =
                vmk_MappedResourceWrite32(
                    rp1gem_gem_resource, 0x08c, 0x00000100);
        if (mmio_status == 0)
            mmio_status =
                vmk_MappedResourceWrite32(
                    rp1gem_gem_resource, 0x004,
                    rp1gem_saved_ncfgr |
                        (1U << 10) | (1U << 1));
        if (mmio_status == 0)
            mmio_status =
                vmk_MappedResourceWrite32(
                    rp1gem_gem_resource, 0x010,
                    (rp1gem_saved_dmacfg &
                     ~(0xffU << 16)) |
                        (32U << 16));
        if (mmio_status == 0)
            mmio_status =
                vmk_MappedResourceWrite32(
                    rp1gem_gem_resource, 0x020, 0x0f);
        if (mmio_status == 0)
            mmio_status =
                vmk_MappedResourceWrite32(
                    rp1gem_gem_resource, 0x000,
                    rp1gem_saved_ncr | (1U << 2));
        if (mmio_status == 0 && rp1gem_link_restart_active == 0)
            vmk_DelayUsecs(1000000);
        (void)vmk_MappedResourceWrite32(
            rp1gem_gem_resource, 0x000, rp1gem_saved_ncr);
        vmk_DelayUsecs(1000);
        (void)vmk_MappedResourceWrite32(
            rp1gem_gem_resource, 0x004, rp1gem_saved_ncfgr);
        (void)vmk_MappedResourceWrite32(
            rp1gem_gem_resource, 0x010, rp1gem_saved_dmacfg);
        (void)vmk_MappedResourceWrite32(
            rp1gem_gem_resource, 0x088, rp1gem_saved_sa1b);
        (void)vmk_MappedResourceWrite32(
            rp1gem_gem_resource, 0x08c, rp1gem_saved_sa1t);
        if (mmio_status != 0)
            return mmio_status;
        status = vmk_NetPollActivate(rp1gem_netpoll_handle);
        vmk_DelayUsecs(100000);
        _vmk_WarningMessage(
            "rp1gem_mmio: associated NetPoll second activate "
            "status=%x calls=%u queued=%u ringUsed=%u "
            "ringQueued=%u ringReplaced=%u ringStatus=%x",
            status, rp1gem_netpoll_calls,
            rp1gem_netpoll_queued, rp1gem_netpoll_ring_used,
            rp1gem_netpoll_ring_queued,
            rp1gem_netpoll_ring_replaced,
            rp1gem_netpoll_ring_status);
        if (status != 0)
            return status;
    }

    /*
     * Do not register the ACPI interrupt cookie: ETH0, XHC0 and XHC1 share
     * the same RP1 parent interrupt and registering it from this prototype
     * prevents the USB storage/network path from completing boot.
     */
    _vmk_WarningMessage(
        "rp1gem_mmio: shared interrupt deliberately not registered "
        "cookie=%x device=%lx",
        rp1gem_interrupt_cookie, rp1gem_parent_device);

    rp1gem_world_calls = 0;
    rp1gem_world_last_sleep_status = (vmk_Status)-1;
    rp1gem_world_rx_status = (vmk_Status)-1;
    rp1gem_world_netpoll_status = (vmk_Status)-1;
    rp1gem_world_stop = 0;
    rp1gem_world_running = 1;
    rp1gem_world_id = 0;
    world_props.module_id = vmk_ModuleCurrentID;
    world_props.heap = vmk_ModuleGetHeapID(vmk_ModuleCurrentID);
    world_status = vmk_NameInitialize(world_name, "rp1gem_poll_rx");
    if (world_status == 0)
        world_status =
            vmk_WorldCreate(
                &world_props, (uint32_t *)&rp1gem_world_id);
    if (world_status != 0)
        rp1gem_world_running = 0;
    _vmk_WarningMessage(
        "rp1gem_mmio: persistent polling RX world create status=%x id=%u "
        "calls=%u sleepStatus=%x rxStatus=%x netpollStatus=%x "
        "queued=%u ringQueued=%u ringReplaced=%u ringStatus=%x",
        world_status, rp1gem_world_id, rp1gem_world_calls,
        rp1gem_world_last_sleep_status, rp1gem_world_rx_status,
        rp1gem_world_netpoll_status, rp1gem_netpoll_queued,
        rp1gem_netpoll_ring_queued, rp1gem_netpoll_ring_replaced,
        rp1gem_netpoll_ring_status);
    if (world_status != 0)
        return world_status;

    /*
     * World creation is asynchronous.  Do not publish Link Up until the
     * polling world has programmed the RX ring and enabled RX DMA.  Without
     * this handshake ESXi can start management traffic during the world's
     * initial delay, leaving the boot-time RX path stalled until a manual
     * administrative down/up recreates it.
     */
    for (startup_wait_loops = 0;
         startup_wait_loops < 500U &&
         rp1gem_world_running != 0U &&
         rp1gem_world_rx_status == (vmk_Status)-1;
         startup_wait_loops++)
        vmk_DelayUsecs(10000U);
    _vmk_WarningMessage(
        "rp1gem_mmio: polling RX startup handshake loops=%u "
        "worldRunning=%u rxStatus=%x netpollStatus=%x calls=%u",
        startup_wait_loops, rp1gem_world_running,
        rp1gem_world_rx_status, rp1gem_world_netpoll_status,
        rp1gem_world_calls);
    if (rp1gem_world_running == 0U ||
        rp1gem_world_rx_status != 0) {
        rp1gem_stop_poll_world();
        return rp1gem_world_rx_status == (vmk_Status)-1 ?
            (vmk_Status)1 : rp1gem_world_rx_status;
    }

    status = vmk_UplinkUpdateLinkState(rp1gem_uplink_handle, &link);
    _vmk_WarningMessage(
        "rp1gem_mmio: uplink link-state update up=1 speed=1000 "
        "duplex=2 status=%x", status);
    return 0;
}

static vmk_Status
rp1gem_uplink_quiesce_io(void *adapter)
{
    _vmk_WarningMessage(
        "rp1gem_mmio: lifecycle QuiesceIO adapter=%lx worldRunning=%u "
        "netpollEnabled=%u",
        (uint64_t)adapter, rp1gem_world_running,
        rp1gem_netpoll_enabled);
    rp1gem_stop_poll_world();
    if (rp1gem_netpoll_handle != 0 &&
        rp1gem_netpoll_enabled != 0) {
        (void)vmk_NetPollDisable(rp1gem_netpoll_handle);
        (void)vmk_NetPollFlushRx(rp1gem_netpoll_handle);
        rp1gem_netpoll_enabled = 0;
    }
    return 0;
}

static vmk_Status
rp1gem_uplink_reset(void *adapter)
{
    _vmk_WarningMessage(
        "rp1gem_mmio: lifecycle Reset adapter=%lx",
        (uint64_t)adapter);
    return 0;
}

static vmk_Status
rp1gem_uplink_remove(vmk_Device device)
{
    (void)device;
    rp1gem_uplink_child = 0;
    return 0;
}

static struct rp1gem_child_device_ops rp1gem_uplink_device_ops = {
    rp1gem_uplink_remove,
};

static vmk_Status
rp1gem_register_uplink_skeleton(vmk_Device parent)
{
    uint8_t logical_name[32] __attribute__((aligned(8))) = { 0 };
    struct rp1gem_child_props props = { 0 };
    vmk_Status status;
    static const uint8_t mac[6] = { 0x02, 0x52, 0x50, 0x31, 0x00, 0x01 };

    if (rp1gem_uplink_child != 0)
        return 0;

    status = vmk_NameInitialize(logical_name, "logical");
    if (status != 0)
        return status;
    status = vmk_BusTypeFind(logical_name, &props.bus_type);
    if (status != 0) {
        _vmk_WarningMessage(
            "rp1gem_mmio: logical bus lookup failed status=%x", status);
        return status;
    }

    status = vmk_LogicalCreateBusAddress(
        &props.bus_type, parent, 0, &props.logical_address,
        &props.logical_cookie);
    if (status != 0) {
        _vmk_WarningMessage(
            "rp1gem_mmio: logical address create failed status=%x", status);
        vmk_BusTypeRelease(props.bus_type);
        return status;
    }

    rp1gem_uplink_registration.version = 0x03000000U;
    rp1gem_uplink_registration.module_id = vmk_ModuleCurrentID;
    rp1gem_uplink_registration.ops[0] = (void *)rp1gem_uplink_tx_probe;
    rp1gem_uplink_registration.ops[1] = (void *)rp1gem_uplink_mtu_set;
    rp1gem_uplink_registration.ops[2] = (void *)rp1gem_uplink_state_set;
    rp1gem_uplink_registration.ops[3] = (void *)rp1gem_uplink_stats_get;
    rp1gem_uplink_registration.ops[4] = (void *)rp1gem_uplink_associate;
    rp1gem_uplink_registration.ops[5] = (void *)rp1gem_uplink_disassociate;
    rp1gem_uplink_registration.ops[6] =
        (void *)rp1gem_uplink_cap_enable;
    rp1gem_uplink_registration.ops[7] =
        (void *)rp1gem_uplink_cap_disable;
    rp1gem_uplink_registration.ops[8] = (void *)rp1gem_uplink_start_io;
    rp1gem_uplink_registration.ops[9] = (void *)rp1gem_uplink_quiesce_io;
    rp1gem_uplink_registration.ops[10] = (void *)rp1gem_uplink_reset;
    rp1gem_uplink_registration.ops[11] = 0;

    memset(&rp1gem_uplink_device_info, 0, sizeof(rp1gem_uplink_device_info));
    /*
     * Host Client consumes this legacy table as consecutive 12-byte mode
     * records.  Keep it byte-identical to the capability-39 mode list.
     */
    memcpy(rp1gem_uplink_device_info.modes,
           rp1gem_advertised_modes, sizeof(rp1gem_advertised_modes));

    memset(&rp1gem_uplink_shared_data, 0, sizeof(rp1gem_uplink_shared_data));
    rp1gem_uplink_shared_data.state = 1;
    rp1gem_uplink_shared_data.link_up = 1;
    rp1gem_uplink_shared_data.link_speed = 1000;
    rp1gem_uplink_shared_data.link_duplex = 2;
    rp1gem_uplink_shared_data.mtu = 1500;
    memcpy(rp1gem_uplink_shared_data.current_mac, mac, sizeof(mac));
    memcpy(rp1gem_uplink_shared_data.permanent_mac, mac, sizeof(mac));
    /* Match nativegenet_startDevice's device-info publication exactly. */
    *(uint32_t *)rp1gem_uplink_shared_data.reserved1 = 4;
    rp1gem_uplink_shared_data.device_info = &rp1gem_uplink_device_info;
    rp1gem_uplink_shared_data.device_info_revision = 4;
    (void)vmk_NameInitialize(rp1gem_uplink_shared_data.driver_name,
                             "RP1_GEM");
    (void)vmk_NameInitialize(rp1gem_uplink_shared_data.driver_version,
                             "0.0.213");
    (void)vmk_NameInitialize(rp1gem_uplink_shared_data.firmware_version,
                             "NA");
    (void)vmk_NameInitialize(rp1gem_uplink_shared_data.implementation,
                             "native");

    status = rp1gem_create_persistent_netpoll();
    if (status != 0) {
        vmk_BusTypeRelease(props.bus_type);
        return status;
    }
    rp1gem_init_shared_queue();

    rp1gem_uplink_registration.driver_data = &rp1gem_uplink_shared_data;
    /* nativegenet_startDevice publishes the device context as adapterData. */
    rp1gem_uplink_registration.adapter_data = &rp1gem_adapter_context;

    props.registering_driver = rp1gem_driver;
    props.bus_address = &props.bus_type;
    props.device_ops = &rp1gem_uplink_device_ops;
    props.registering_driver_data = &rp1gem_adapter_context;
    props.registration_data = &rp1gem_uplink_registration;
    props.identifier = "com.vmware.uplink";
    props.identifier_length =
        (uint32_t)vmk_Strnlen(props.identifier, 32);

    status = vmk_DeviceRegister(&props, parent, &rp1gem_uplink_child);
    _vmk_WarningMessage(
        "rp1gem_mmio: uplink skeleton register status=%x child=%lx",
        status, rp1gem_uplink_child);

    /*
     * Keep the temporary logical address allocation alive.  On this ESXi
     * build vmk_LogicalCreateBusAddress allocates it from the devlayer heap,
     * while calling vmk_LogicalFreeBusAddress from this external module tries
     * to release it through the module heap and panics.  The allocation is
     * tiny and scan runs only once; retaining it is safe for this prototype.
     */
    vmk_BusTypeRelease(props.bus_type);
    return status;
}

static vmk_Status
rp1gem_mdio_read(void *resource, uint32_t phy, uint32_t reg, uint32_t *value)
{
    vmk_Status status;
    uint32_t nsr = 0, man = 0, i;

    for (i = 0; i < 10000; i++) {
        status = vmk_MappedResourceRead32(resource, RP1_GEM_OFFSET + 0x008,
                                          &nsr);
        if (status != 0)
            return status;
        if ((nsr & (1U << 2)) != 0)
            break;
    }
    if (i == 10000)
        return 1;

    man = (1U << 30) | (2U << 28) | ((phy & 31U) << 23) |
          ((reg & 31U) << 18) | (2U << 16);
    status = vmk_MappedResourceWrite32(resource, RP1_GEM_OFFSET + 0x034,
                                       man);
    if (status != 0)
        return status;

    for (i = 0; i < 10000; i++) {
        status = vmk_MappedResourceRead32(resource, RP1_GEM_OFFSET + 0x008,
                                          &nsr);
        if (status != 0)
            return status;
        if ((nsr & (1U << 2)) != 0)
            break;
    }
    if (i == 10000)
        return 1;

    status = vmk_MappedResourceRead32(resource, RP1_GEM_OFFSET + 0x034,
                                      &man);
    if (status == 0)
        *value = man & 0xffffU;
    return status;
}

static vmk_Status
rp1gem_mdio_write(void *resource, uint32_t phy, uint32_t reg,
                  uint32_t value)
{
    vmk_Status status;
    uint32_t nsr = 0, man, i;

    for (i = 0; i < 10000; i++) {
        status = vmk_MappedResourceRead32(
            resource, RP1_GEM_OFFSET + 0x008, &nsr);
        if (status != 0)
            return status;
        if ((nsr & (1U << 2)) != 0)
            break;
    }
    if (i == 10000)
        return 1;
    man = (1U << 30) | (1U << 28) | ((phy & 31U) << 23) |
          ((reg & 31U) << 18) | (2U << 16) | (value & 0xffffU);
    status = vmk_MappedResourceWrite32(
        resource, RP1_GEM_OFFSET + 0x034, man);
    if (status != 0)
        return status;
    for (i = 0; i < 10000; i++) {
        status = vmk_MappedResourceRead32(
            resource, RP1_GEM_OFFSET + 0x008, &nsr);
        if (status != 0)
            return status;
        if ((nsr & (1U << 2)) != 0)
            return 0;
    }
    return 1;
}

static vmk_Status
rp1gem_attach(vmk_Device device)
{
    vmk_ACPIDevice acpi_device = 0;
    /*
     * vmk_MappedResource is an inline object, not a pointer. Stock ESXi ARM
     * drivers pass the address of storage embedded in their device context
     * to both vmk_ACPIMapIOResource and vmk_MappedResourceRead32.
     * Reserve more than the observed object footprint to avoid relying on
     * private NDDK layout details.
     */
    uint8_t *gem_resource = rp1gem_gem_resource;
    vmk_Status status;
    uint8_t acpi_info[256] __attribute__((aligned(8))) = { 0 };
    uint32_t ncr = 0, ncfgr = 0, nsr = 0, dmacfg = 0, jml = 0, mid = 0;
    uint32_t dcfg1 = 0, dcfg2 = 0, dcfg3 = 0, dcfg4 = 0, dcfg5 = 0;
    uint32_t dcfg6 = 0;
    uint32_t rxqbase = 0, txqbase = 0, isr = 0, imr = 0;
    uint32_t sa1b = 0, sa1t = 0;
    uint64_t rx_ring_ma = 0, tx_ring_ma = 0;
    vmk_Status rx_ring_status, tx_ring_status;
    vmk_Status ring_program_status = 0;
    uint32_t rxqbase_hi = 0, txqbase_hi = 0;
    uint32_t dmacfg_after = 0;
    uint32_t ring_index;
    struct rp1gem_dma_desc32 *rx_ring32 = 0;
    struct rp1gem_dma_desc32 *tx_ring32 = 0;
    vmk_Status dma_engine_status;
    vmk_Status dma_packet_status = 1;
    vmk_Status dma_map_status = 1;
    vmk_Status dma_unmap_status = 1;
    struct rp1gem_sg_element *dma_test_sg = 0;
    uint64_t dma_mapped_count = 0;
    uint32_t dma_test_alignment = 0;
    uint32_t dma_packet_index;
    uint32_t dma_packet_count = 0;
    void *jumbo_copy_packet = 0;
    void *jumbo_copy_pointer = 0;
    uint32_t jumbo_copy_mapped_length = 0;
    uint32_t jumbo_copy_fully_mapped = 0;
    uint32_t jumbo_copy_metadata_length = 0;
    uint32_t jumbo_copy_verified = 0;
    uint32_t jumbo_copy_index;
    vmk_Status jumbo_copy_alloc_status = 1;
    vmk_Status rx_test_status = 1;
    uint32_t rx_test_used = 0;
    uint32_t rx_test_bitmap = 0;
    uint32_t rx_test_first = 0xffffffffU;
    uint32_t rx_test_ctrl = 0;
    uint32_t rx_test_rsr = 0;
    uint32_t rx_test_isr = 0;
    uint32_t rx_test_iteration = 0;
    uint32_t rx_test_ncfgr = 0;
    uint32_t rx_test_dmacfg = 0;
    vmk_Status jumbo_rx_test_status = 1;
    uint32_t jumbo_rx_test_iteration = 0;
    uint32_t jumbo_rx_test_used = 0;
    uint32_t jumbo_rx_test_ctrl = 0;
    uint32_t jumbo_rx_test_length = 0;
    void *jumbo_rx_test_packet = 0;
    void *jumbo_rx_test_pointer = 0;
    uint32_t jumbo_rx_test_mapped_length = 0;
    uint32_t jumbo_rx_test_metadata_length = 0;
    uint32_t jumbo_rx_test_copied = 0;
    vmk_Status jumbo_tx_test_status = 1;
    uint32_t jumbo_tx_test_completed = 0;
    uint32_t jumbo_tx_test_addr = 0;
    uint32_t jumbo_tx_test_ctrl = 0;
    uint32_t jumbo_tx_test_tsr = 0;
    uint32_t jumbo_tx_test_isr = 0;
    uint32_t jumbo_tx_test_iteration = 0;
    uint32_t jumbo_tx_test_index;
    struct rp1gem_dma_engine_props dma_props = {
        { 0 },
        0,
        4,
        0,
        &rp1gem_dma_constraints,
    };
    vmk_Status memory_pool_status;
    vmk_Status memory_alloc_status = 1;
    struct rp1gem_memory_range memory_test_range = { 0, 0, 0 };
    struct rp1gem_memory_allocation memory_test_allocation = {
        1,
        1,
        &memory_test_range,
    };
    struct rp1gem_memory_constraints memory_constraints = {
        0,
        2,
        -1,
        0,
    };
    vmk_Status memory_map_status = 1;
    vmk_Status memory_map_va2ma_status = 1;
    uint64_t memory_map_ma = 0;
    vmk_Status jumbo_memory_alloc_status = 1;
    vmk_Status jumbo_memory_map_status = 1;
    vmk_Status jumbo_memory_va2ma_status = 1;
    uint64_t jumbo_memory_ma = 0;
    struct rp1gem_memory_range jumbo_memory_range = { 0, 0, 0 };
    struct rp1gem_memory_allocation jumbo_memory_allocation = {
        RP1GEM_RAW_RX_TOTAL_PAGES,
        1,
        &jumbo_memory_range,
    };
    vmk_Status jumbo_tx_alloc_status = 1;
    vmk_Status jumbo_tx_map_status = 1;
    vmk_Status jumbo_tx_va2ma_status = 1;
    uint64_t jumbo_tx_ma = 0;
    struct rp1gem_memory_range jumbo_tx_range = { 0, 0, 0 };
    struct rp1gem_memory_allocation jumbo_tx_allocation = {
        RP1GEM_TX_TOTAL_PAGES,
        1,
        &jumbo_tx_range,
    };
    struct rp1gem_map_props memory_map_props = {
        1,
        0,
        1,
        1,
        0,
        &memory_test_range,
        0,
    };
    struct rp1gem_map_props jumbo_memory_map_props = {
        RP1GEM_RAW_RX_TOTAL_PAGES,
        0,
        1,
        1,
        0,
        &jumbo_memory_range,
        0,
    };
    struct rp1gem_map_props jumbo_tx_map_props = {
        RP1GEM_TX_TOTAL_PAGES,
        0,
        1,
        1,
        0,
        &jumbo_tx_range,
        0,
    };
    struct rp1gem_memory_pool_props memory_pool_props = {
        { 0 },
        0,
        0,
        0,
        1,
        0,
        { 0, 0 },
    };
    uint32_t phy_bmsr = 0xffff, phy_id1 = 0xffff, phy_id2 = 0xffff;
    uint32_t phy_bmcr = 0xffff, phy_anar = 0xffff, phy_anlpar = 0xffff;
    uint32_t phy_gbcr = 0xffff, phy_gbsr = 0xffff, phy_aux = 0xffff;
    uint32_t phy, phy_found = 0;
    vmk_Status mdio_status = 0;

    rp1gem_parent_device = device;
    status = vmk_DeviceGetRegistrationData(device, (void **)&acpi_device);
    if (status != 0 || acpi_device == 0) {
        _vmk_WarningMessage("rp1gem_mmio: registration data failed status=%x",
                            status);
        return status != 0 ? status : 1;
    }

    /*
     * Native ESXi ARM ACPI drivers query the device before mapping its
     * resources.  Besides returning metadata, this establishes the ACPI
     * device state expected by vmk_ACPIMapIOResource.
     */
    status = vmk_ACPIQueryInfo(acpi_device, acpi_info);
    if (status != 0) {
        _vmk_WarningMessage("rp1gem_mmio: ACPI query failed status=%x",
                            status);
        return status;
    }

    memory_pool_props.module_id = vmk_ModuleCurrentID;
    memory_pool_status =
        vmk_NameInitialize(memory_pool_props.name, "rp1gem_dma_pool");
    if (memory_pool_status == 0)
        memory_pool_status =
            vmk_MemPoolCreate(&memory_pool_props, &rp1gem_memory_pool);
    _vmk_WarningMessage(
        "rp1gem_mmio: memory pool create status=%x handle=%lx type=%u",
        memory_pool_status, rp1gem_memory_pool, memory_pool_props.type);
    if (memory_pool_status == 0)
        memory_alloc_status =
            vmk_MemPoolAllocWithRA(rp1gem_memory_pool,
                                   &memory_constraints,
                                   &memory_test_allocation,
                                   __builtin_return_address(0));
    _vmk_WarningMessage(
        "rp1gem_mmio: memory page alloc status=%x class=%u firstMPN=%lx "
        "pages=%u ranges=%u",
        memory_alloc_status, memory_constraints.address_class,
        memory_test_range.first_mpn,
        memory_test_range.page_count,
        memory_test_allocation.range_count);
    if (memory_alloc_status == 0) {
        memory_map_status =
            vmk_Map(vmk_ModuleCurrentID, &memory_map_props,
                    &rp1gem_low_ring_page);
        if (memory_map_status == 0 && rp1gem_low_ring_page != 0) {
            memset(rp1gem_low_ring_page, 0, 4096);
            memory_map_va2ma_status =
                vmk_VA2MA(rp1gem_low_ring_page, 4096, &memory_map_ma);
        }
    }
    _vmk_WarningMessage(
        "rp1gem_mmio: low page map status=%x va=%lx "
        "VA2MAstatus=%x ma=%lx expected=%lx",
        memory_map_status, rp1gem_low_ring_page,
        memory_map_va2ma_status, memory_map_ma,
        memory_test_range.first_mpn << 12);

    if (memory_pool_status == 0)
        jumbo_memory_alloc_status =
            vmk_MemPoolAllocWithRA(
                rp1gem_memory_pool, &memory_constraints,
                &jumbo_memory_allocation,
                __builtin_return_address(0));
    if (jumbo_memory_alloc_status == 0 &&
        jumbo_memory_allocation.range_count == 1 &&
        jumbo_memory_range.page_count >= RP1GEM_RAW_RX_TOTAL_PAGES) {
        jumbo_memory_map_status =
            vmk_Map(vmk_ModuleCurrentID, &jumbo_memory_map_props,
                    &rp1gem_jumbo_probe_pages);
        if (jumbo_memory_map_status == 0 &&
            rp1gem_jumbo_probe_pages != 0) {
            memset(rp1gem_jumbo_probe_pages, 0,
                   RP1GEM_RAW_RX_TOTAL_PAGES * 4096U);
            jumbo_memory_va2ma_status =
                vmk_VA2MA(rp1gem_jumbo_probe_pages,
                          RP1GEM_RAW_RX_TOTAL_PAGES * 4096U,
                          &jumbo_memory_ma);
            if (jumbo_memory_va2ma_status == 0)
                rp1gem_jumbo_rx_probe_ma = jumbo_memory_ma;
        }
    }
    _vmk_WarningMessage(
        "rp1gem_mmio: jumbo contiguous probe allocStatus=%x "
        "requestedPages=%u ranges=%u firstMPN=%lx rangePages=%u "
        "mapStatus=%x va=%lx VA2MAstatus=%x ma=%lx expected=%lx "
        "dma32=%u contiguous=%u",
        jumbo_memory_alloc_status, jumbo_memory_allocation.page_count,
        jumbo_memory_allocation.range_count,
        jumbo_memory_range.first_mpn, jumbo_memory_range.page_count,
        jumbo_memory_map_status, rp1gem_jumbo_probe_pages,
        jumbo_memory_va2ma_status, jumbo_memory_ma,
        jumbo_memory_range.first_mpn << 12,
        (uint32_t)(jumbo_memory_ma >> 32) == 0,
            jumbo_memory_alloc_status == 0 &&
            jumbo_memory_allocation.range_count == 1 &&
            jumbo_memory_range.page_count >=
                RP1GEM_RAW_RX_TOTAL_PAGES &&
            jumbo_memory_map_status == 0 &&
            jumbo_memory_va2ma_status == 0 &&
            jumbo_memory_ma ==
                (jumbo_memory_range.first_mpn << 12));

    if (memory_pool_status == 0)
        jumbo_tx_alloc_status =
            vmk_MemPoolAllocWithRA(
                rp1gem_memory_pool, &memory_constraints,
                &jumbo_tx_allocation,
                __builtin_return_address(0));
    if (jumbo_tx_alloc_status == 0 &&
        jumbo_tx_allocation.range_count == 1 &&
        jumbo_tx_range.page_count >= RP1GEM_TX_TOTAL_PAGES) {
        jumbo_tx_map_status =
            vmk_Map(vmk_ModuleCurrentID, &jumbo_tx_map_props,
                    &rp1gem_jumbo_tx_probe_pages);
        if (jumbo_tx_map_status == 0 &&
            rp1gem_jumbo_tx_probe_pages != 0) {
            memset(rp1gem_jumbo_tx_probe_pages, 0,
                   RP1GEM_TX_TOTAL_PAGES * 4096U);
            jumbo_tx_va2ma_status =
                vmk_VA2MA(rp1gem_jumbo_tx_probe_pages,
                          RP1GEM_TX_TOTAL_PAGES * 4096U,
                          &jumbo_tx_ma);
            if (jumbo_tx_va2ma_status == 0)
                rp1gem_jumbo_tx_probe_ma = jumbo_tx_ma;
        }
    }
    _vmk_WarningMessage(
        "rp1gem_mmio: jumbo dual-buffer probe txAlloc=%x "
        "txRanges=%u txFirstMPN=%lx txPages=%u txMap=%x txVA=%lx "
        "txVA2MA=%x txMA=%lx txExpected=%lx txDMA32=%u "
        "rxReady=%u txReady=%u nonOverlap=%u",
        jumbo_tx_alloc_status, jumbo_tx_allocation.range_count,
        jumbo_tx_range.first_mpn, jumbo_tx_range.page_count,
        jumbo_tx_map_status, rp1gem_jumbo_tx_probe_pages,
        jumbo_tx_va2ma_status, jumbo_tx_ma,
        jumbo_tx_range.first_mpn << 12,
        (uint32_t)(jumbo_tx_ma >> 32) == 0,
        jumbo_memory_alloc_status == 0 &&
            jumbo_memory_map_status == 0 &&
            jumbo_memory_va2ma_status == 0 &&
            jumbo_memory_ma ==
                (jumbo_memory_range.first_mpn << 12),
        jumbo_tx_alloc_status == 0 && jumbo_tx_map_status == 0 &&
            jumbo_tx_va2ma_status == 0 &&
            jumbo_tx_ma == (jumbo_tx_range.first_mpn << 12),
        jumbo_memory_va2ma_status == 0 &&
            jumbo_tx_va2ma_status == 0 &&
            (jumbo_memory_ma +
                 RP1GEM_RAW_RX_TOTAL_PAGES * 4096U <= jumbo_tx_ma ||
             jumbo_tx_ma + RP1GEM_TX_TOTAL_PAGES * 4096U <=
                 jumbo_memory_ma));

    dma_props.module_id = vmk_ModuleCurrentID;
    dma_props.device = device;
    dma_engine_status =
        vmk_NameInitialize(dma_props.name, "rp1gem_dma32");
    if (dma_engine_status == 0)
        dma_engine_status =
            vmk_DMAEngineCreate(&dma_props, &rp1gem_dma_engine);
    if (dma_engine_status == 0 && rp1gem_sg_ops == 0)
        dma_engine_status =
            vmk_SgCreateOpsHandle(
                vmk_ModuleGetHeapID(vmk_ModuleCurrentID),
                &rp1gem_sg_ops, 0, 0);
    _vmk_WarningMessage(
        "rp1gem_mmio: DMA32 engine create status=%x handle=%lx "
        "sgOps=%lx mask=%lx",
        dma_engine_status, rp1gem_dma_engine, rp1gem_sg_ops,
        rp1gem_dma_constraints.address_mask);
    if (dma_engine_status == 0) {
        jumbo_copy_alloc_status =
            vmk_PktAllocForDMAEngine(
                RP1GEM_JUMBO_PROBE_SIZE, rp1gem_dma_engine,
                &jumbo_copy_packet);
        if (jumbo_copy_alloc_status == 0 &&
            jumbo_copy_packet != 0 &&
            rp1gem_jumbo_probe_pages != 0) {
            jumbo_copy_fully_mapped =
                vmk_PktIsFullyMapped(jumbo_copy_packet);
            if (jumbo_copy_fully_mapped != 0) {
                jumbo_copy_pointer =
                    vmk_PktFrameMappedPointerGet(jumbo_copy_packet);
                jumbo_copy_mapped_length =
                    vmk_PktFrameMappedLenGet(jumbo_copy_packet);
            }
            if (jumbo_copy_pointer != 0 &&
                jumbo_copy_mapped_length >= RP1GEM_JUMBO_PROBE_SIZE) {
                for (jumbo_copy_index = 0;
                     jumbo_copy_index < RP1GEM_JUMBO_PROBE_SIZE;
                     jumbo_copy_index++)
                    ((uint8_t *)rp1gem_jumbo_probe_pages)[
                        jumbo_copy_index] =
                        (uint8_t)(jumbo_copy_index ^ 0xa5U);
                rp1gem_cache_clean(
                    rp1gem_jumbo_probe_pages,
                    RP1GEM_JUMBO_PROBE_SIZE);
                rp1gem_cache_invalidate(
                    rp1gem_jumbo_probe_pages,
                    RP1GEM_JUMBO_PROBE_SIZE);
                memcpy(jumbo_copy_pointer,
                       rp1gem_jumbo_probe_pages,
                       RP1GEM_JUMBO_PROBE_SIZE);
                vmk_PktFrameLenSet(
                    jumbo_copy_packet, RP1GEM_JUMBO_PROBE_SIZE);
                jumbo_copy_metadata_length =
                    vmk_PktFrameLenGet(jumbo_copy_packet);
                jumbo_copy_verified =
                    ((uint8_t *)jumbo_copy_pointer)[0] == 0xa5U &&
                    ((uint8_t *)jumbo_copy_pointer)[4095] ==
                        (uint8_t)(4095U ^ 0xa5U) &&
                    ((uint8_t *)jumbo_copy_pointer)[4096] ==
                        (uint8_t)(4096U ^ 0xa5U) &&
                    ((uint8_t *)jumbo_copy_pointer)[
                        RP1GEM_JUMBO_PROBE_SIZE - 1] ==
                        (uint8_t)(
                            (RP1GEM_JUMBO_PROBE_SIZE - 1) ^ 0xa5U) &&
                    jumbo_copy_metadata_length ==
                        RP1GEM_JUMBO_PROBE_SIZE;
            }
        }
        _vmk_WarningMessage(
            "rp1gem_mmio: jumbo CPU-copy probe allocStatus=%x "
            "packet=%lx fullyMapped=%u pointer=%lx mappedLen=%u "
            "metadataLen=%u copied=%u released=%u",
            jumbo_copy_alloc_status, jumbo_copy_packet,
            jumbo_copy_fully_mapped, jumbo_copy_pointer,
            jumbo_copy_mapped_length, jumbo_copy_metadata_length,
            jumbo_copy_verified, jumbo_copy_packet != 0);
        if (jumbo_copy_packet != 0) {
            vmk_PktRelease(jumbo_copy_packet);
            jumbo_copy_packet = 0;
        }
        dma_packet_status = 0;
        for (dma_packet_index = 0;
             dma_packet_index < RP1GEM_RING_COUNT;
             dma_packet_index++) {
            dma_packet_status =
                vmk_PktAllocForDMAEngine(
                    2048, rp1gem_dma_engine,
                    &rp1gem_rx_packets[dma_packet_index]);
            if (dma_packet_status != 0 ||
                rp1gem_rx_packets[dma_packet_index] == 0)
                break;
            dma_test_sg =
                vmk_PktSgElemGet(
                    rp1gem_rx_packets[dma_packet_index], 0);
            if (dma_test_sg == 0 ||
                dma_test_sg->length < 2048 ||
                (dma_test_sg->address >> 32) != 0) {
                dma_packet_status = 1;
                break;
            }
            dma_mapped_count = 0;
            dma_map_status =
                vmk_DMAMapElem(
                    rp1gem_dma_engine, 1, dma_test_sg, 1,
                    &rp1gem_rx_mapped_elements[dma_packet_index],
                    &dma_mapped_count);
            if (dma_map_status != 0 ||
                rp1gem_rx_mapped_elements[dma_packet_index].length < 2048 ||
                (rp1gem_rx_mapped_elements[dma_packet_index].address >>
                 32) != 0) {
                dma_packet_status =
                    dma_map_status != 0 ? dma_map_status : 1;
                break;
            }
            rp1gem_rx_addresses[dma_packet_index] =
                rp1gem_rx_mapped_elements[dma_packet_index].address;
            dma_packet_count++;
        }
        if (dma_packet_count != 0)
            dma_test_alignment =
                vmk_PktSgElemAlignment(rp1gem_rx_packets[0], 0);
        if (dma_packet_count == RP1GEM_RING_COUNT) {
            dma_packet_status =
                vmk_PktAllocForDMAEngine(
                    2048, rp1gem_dma_engine,
                    &rp1gem_tx_bounce_packet);
            if (dma_packet_status == 0 &&
                rp1gem_tx_bounce_packet != 0 &&
                vmk_PktIsFullyMapped(rp1gem_tx_bounce_packet) != 0) {
                rp1gem_tx_bounce_pointer =
                    vmk_PktFrameMappedPointerGet(
                        rp1gem_tx_bounce_packet);
                rp1gem_tx_bounce_length =
                    vmk_PktFrameMappedLenGet(
                        rp1gem_tx_bounce_packet);
            }
            if (rp1gem_tx_bounce_pointer == 0 ||
                rp1gem_tx_bounce_length < 2048)
                dma_packet_status = 1;
        }
    }
    _vmk_WarningMessage(
        "rp1gem_mmio: DMA32 RX buffers status=%x count=%u "
        "mapStatus=%x mappedCount=%lu first=%lx last=%lx alignment=%u",
        dma_packet_status, dma_packet_count, dma_map_status,
        dma_mapped_count,
        dma_packet_count != 0 ? rp1gem_rx_addresses[0] : 0,
        dma_packet_count != 0 ?
            rp1gem_rx_addresses[dma_packet_count - 1] : 0,
        dma_test_alignment);
    _vmk_WarningMessage(
        "rp1gem_mmio: TX bounce allocation status=%x packet=%lx "
        "pointer=%lx length=%u fullyMapped=%u",
        dma_packet_status, rp1gem_tx_bounce_packet,
        rp1gem_tx_bounce_pointer, rp1gem_tx_bounce_length,
        rp1gem_tx_bounce_packet != 0 ?
            vmk_PktIsFullyMapped(rp1gem_tx_bounce_packet) : 0);

    status =
        vmk_ACPIMapIOResource(vmk_ModuleCurrentID, acpi_device, 0,
                              gem_resource);
    if (status != 0) {
        _vmk_WarningMessage("rp1gem_mmio: map GEM resource 0 failed status=%x",
                            status);
        return status != 0 ? status : 1;
    }
    rp1gem_gem_acpi_device = acpi_device;
    rp1gem_gem_resource_mapped = 1;
    status =
        vmk_ACPIAllocIntrCookie(
            vmk_ModuleCurrentID, acpi_device, 1, 1,
            &rp1gem_interrupt_cookie,
            &rp1gem_interrupt_cookie_count);
    if (status == 0)
        rp1gem_interrupt_cookie_allocated = 1;
    _vmk_WarningMessage(
        "rp1gem_mmio: ACPI interrupt cookie probe status=%x "
        "cookie=%x count=%u resourceIndex=1",
        status, rp1gem_interrupt_cookie,
        rp1gem_interrupt_cookie_count);
    if (status != 0)
        return status;

    status = vmk_MappedResourceRead32(gem_resource, RP1_GEM_OFFSET, &ncr);
    if (status == 0)
        status = vmk_MappedResourceRead32(gem_resource,
                                          RP1_GEM_OFFSET + 0x004, &ncfgr);
    if (status == 0)
        status = vmk_MappedResourceRead32(gem_resource,
                                          RP1_GEM_OFFSET + 0x008, &nsr);
    if (status == 0)
        status = vmk_MappedResourceRead32(gem_resource,
                                          RP1_GEM_OFFSET + 0x010, &dmacfg);
    if (status == 0)
        status = vmk_MappedResourceRead32(gem_resource,
                                          RP1_GEM_OFFSET + 0x048, &jml);
    if (status == 0)
        status = vmk_MappedResourceRead32(gem_resource,
                                          RP1_GEM_OFFSET + 0x018, &rxqbase);
    if (status == 0)
        status = vmk_MappedResourceRead32(gem_resource,
                                          RP1_GEM_OFFSET + 0x01c, &txqbase);
    if (status == 0)
        status = vmk_MappedResourceRead32(gem_resource,
                                          RP1_GEM_OFFSET + 0x024, &isr);
    if (status == 0)
        status = vmk_MappedResourceRead32(gem_resource,
                                          RP1_GEM_OFFSET + 0x030, &imr);
    if (status == 0)
        status = vmk_MappedResourceRead32(gem_resource,
                                          RP1_GEM_OFFSET + 0x088, &sa1b);
    if (status == 0)
        status = vmk_MappedResourceRead32(gem_resource,
                                          RP1_GEM_OFFSET + 0x08c, &sa1t);
    if (status == 0)
        status = vmk_MappedResourceRead32(gem_resource,
                                          RP1_GEM_OFFSET + 0x0fc, &mid);
    if (status == 0)
        status = vmk_MappedResourceRead32(gem_resource,
                                          RP1_GEM_OFFSET + 0x280, &dcfg1);
    if (status == 0)
        status = vmk_MappedResourceRead32(gem_resource,
                                          RP1_GEM_OFFSET + 0x284, &dcfg2);
    if (status == 0)
        status = vmk_MappedResourceRead32(gem_resource,
                                          RP1_GEM_OFFSET + 0x288, &dcfg3);
    if (status == 0)
        status = vmk_MappedResourceRead32(gem_resource,
                                          RP1_GEM_OFFSET + 0x28c, &dcfg4);
    if (status == 0)
        status = vmk_MappedResourceRead32(gem_resource,
                                          RP1_GEM_OFFSET + 0x290, &dcfg5);
    if (status == 0)
        status = vmk_MappedResourceRead32(gem_resource,
                                          RP1_GEM_OFFSET + 0x294, &dcfg6);
    if (status == 0) {
        rp1gem_saved_ncr = ncr;
        rp1gem_saved_ncfgr = ncfgr;
        rp1gem_saved_dmacfg = dmacfg;
        rp1gem_saved_jml = jml;
        rp1gem_saved_sa1b = sa1b;
        rp1gem_saved_sa1t = sa1t;
    }

    _vmk_WarningMessage(
        "rp1gem_mmio: GEM probe status=%x NCR=%08x NCFGR=%08x "
        "NSR=%08x DMACFG=%08x MID=%08x",
        status, ncr, ncfgr, nsr, dmacfg, mid);
    _vmk_WarningMessage(
        "rp1gem_mmio: GEM design DCFG1=%08x DCFG2=%08x DCFG3=%08x "
        "DCFG4=%08x DCFG5=%08x DCFG6=%08x",
        dcfg1, dcfg2, dcfg3, dcfg4, dcfg5, dcfg6);
    _vmk_WarningMessage(
        "rp1gem_mmio: GEM pre-DMA RXQBASE=%08x TXQBASE=%08x "
        "ISR=%08x IMR=%08x SA1B=%08x SA1T=%08x",
        rxqbase, txqbase, isr, imr, sa1b, sa1t);

    if (dma_packet_count == RP1GEM_RING_COUNT &&
        memory_map_status == 0 && memory_map_va2ma_status == 0 &&
        rp1gem_low_ring_page != 0 && (memory_map_ma >> 32) == 0) {
        rx_ring32 =
            (struct rp1gem_dma_desc32 *)rp1gem_low_ring_page;
        rp1gem_rx_ring = rx_ring32;
        tx_ring32 =
            (struct rp1gem_dma_desc32 *)
            ((uint8_t *)rp1gem_low_ring_page + 2048);
        rp1gem_tx_ring = tx_ring32;
        for (ring_index = 0; ring_index < RP1GEM_RING_COUNT;
             ring_index++) {
            rx_ring32[ring_index].addr =
                (uint32_t)rp1gem_rx_addresses[ring_index] |
                RP1GEM_RX_USED;
            tx_ring32[ring_index].ctrl = RP1GEM_TX_USED;
        }
        rx_ring32[RP1GEM_RING_COUNT - 1].addr |= RP1GEM_RX_WRAP;
        tx_ring32[RP1GEM_TX_HW_COUNT - 1].ctrl |= RP1GEM_TX_WRAP;
        rx_ring_ma = memory_map_ma;
        rp1gem_rx_ring_ma = rx_ring_ma;
        tx_ring_ma = memory_map_ma + 2048;
        rx_ring_status = 0;
        tx_ring_status = 0;
    } else {
        rx_ring_status = 1;
        tx_ring_status = 1;
    }
    _vmk_WarningMessage(
        "rp1gem_mmio: low rings rxStatus=%x rxMA=%lx "
        "txStatus=%x txMA=%lx aligned=%u/%u",
        rx_ring_status, rx_ring_ma, tx_ring_status, tx_ring_ma,
        (uint32_t)(rx_ring_ma & 0xfffU),
        (uint32_t)(tx_ring_ma & 0xfffU));

    if (tx_ring_status == 0 && tx_ring32 != 0 &&
        jumbo_tx_map_status == 0 && jumbo_tx_va2ma_status == 0 &&
        rp1gem_jumbo_tx_probe_pages != 0 &&
        (jumbo_tx_ma >> 32) == 0) {
        for (jumbo_tx_test_index = 0;
             jumbo_tx_test_index < RP1GEM_JUMBO_PROBE_SIZE;
             jumbo_tx_test_index++)
            ((uint8_t *)rp1gem_jumbo_tx_probe_pages)[
                jumbo_tx_test_index] =
                (uint8_t)(jumbo_tx_test_index ^ 0x5aU);
        memset(rp1gem_jumbo_tx_probe_pages, 0xff, 6);
        ((uint8_t *)rp1gem_jumbo_tx_probe_pages)[6] = 0x02;
        ((uint8_t *)rp1gem_jumbo_tx_probe_pages)[7] = 0x52;
        ((uint8_t *)rp1gem_jumbo_tx_probe_pages)[8] = 0x50;
        ((uint8_t *)rp1gem_jumbo_tx_probe_pages)[9] = 0x31;
        ((uint8_t *)rp1gem_jumbo_tx_probe_pages)[10] = 0x00;
        ((uint8_t *)rp1gem_jumbo_tx_probe_pages)[11] = 0x01;
        ((uint8_t *)rp1gem_jumbo_tx_probe_pages)[12] = 0x88;
        ((uint8_t *)rp1gem_jumbo_tx_probe_pages)[13] = 0xb5;
        rp1gem_cache_clean(
            rp1gem_jumbo_tx_probe_pages,
            RP1GEM_JUMBO_PROBE_SIZE);
        tx_ring32[0].addr = (uint32_t)jumbo_tx_ma;
        tx_ring32[0].ctrl =
            RP1GEM_TX_LAST |
            (RP1GEM_JUMBO_PROBE_SIZE & RP1GEM_TX_LENGTH_MASK);
        rp1gem_cache_clean(&tx_ring32[0], sizeof(tx_ring32[0]));

        jumbo_tx_test_status =
            vmk_MappedResourceWrite32(gem_resource, 0x004,
                                      ncfgr | (1U << 10) |
                                          (1U << 4) | (1U << 3) |
                                          (1U << 1));
        if (jumbo_tx_test_status == 0)
            jumbo_tx_test_status =
                vmk_MappedResourceWrite32(gem_resource, 0x01c,
                                          (uint32_t)tx_ring_ma);
        if (jumbo_tx_test_status == 0)
            jumbo_tx_test_status =
                vmk_MappedResourceWrite32(gem_resource, 0x04c8,
                                          (uint32_t)(tx_ring_ma >> 32));
        if (jumbo_tx_test_status == 0)
            jumbo_tx_test_status =
                vmk_MappedResourceWrite32(gem_resource, 0x000,
                                          ncr | (1U << 3) |
                                              (1U << 9));
        for (jumbo_tx_test_iteration = 0;
             jumbo_tx_test_status == 0 &&
             jumbo_tx_test_iteration < 10;
             jumbo_tx_test_iteration++) {
            vmk_DelayUsecs(100000);
            rp1gem_cache_invalidate(&tx_ring32[0],
                                    sizeof(tx_ring32[0]));
            if ((tx_ring32[0].ctrl & RP1GEM_TX_USED) != 0) {
                jumbo_tx_test_completed = 1;
                break;
            }
        }
        (void)vmk_MappedResourceWrite32(gem_resource, 0x000, ncr);
        vmk_DelayUsecs(1000);
        (void)vmk_MappedResourceRead32(gem_resource, 0x014,
                                       &jumbo_tx_test_tsr);
        (void)vmk_MappedResourceRead32(gem_resource, 0x024,
                                       &jumbo_tx_test_isr);
        (void)vmk_MappedResourceWrite32(gem_resource, 0x004, ncfgr);
        (void)vmk_MappedResourceWrite32(gem_resource, 0x01c, txqbase);
        (void)vmk_MappedResourceWrite32(gem_resource, 0x04c8,
                                        txqbase_hi);
        jumbo_tx_test_addr = tx_ring32[0].addr;
        jumbo_tx_test_ctrl = tx_ring32[0].ctrl;
        tx_ring32[0].addr = 0;
        tx_ring32[0].ctrl = RP1GEM_TX_USED;
        tx_ring32[RP1GEM_TX_HW_COUNT - 1].ctrl |= RP1GEM_TX_WRAP;
        rp1gem_cache_clean(&tx_ring32[0], sizeof(tx_ring32[0]));
    }
    _vmk_WarningMessage(
        "rp1gem_mmio: bounded jumbo TX status=%x completed=%u "
        "loops=%u addr=%08x ctrl=%08x length=%u used=%u last=%u "
        "TSR=%08x ISR=%08x restored=1",
        jumbo_tx_test_status, jumbo_tx_test_completed,
        jumbo_tx_test_iteration, jumbo_tx_test_addr,
        jumbo_tx_test_ctrl,
        jumbo_tx_test_ctrl & RP1GEM_TX_LENGTH_MASK,
        (jumbo_tx_test_ctrl & RP1GEM_TX_USED) != 0,
        (jumbo_tx_test_ctrl & RP1GEM_TX_LAST) != 0,
        jumbo_tx_test_tsr, jumbo_tx_test_isr);

    /*
     * Program only the inert ring base registers. RX/TX and interrupts remain
     * disabled; this validates 64-bit addressing before any DMA can occur.
     */
    if (rx_ring_status == 0 && tx_ring_status == 0 &&
        (((rx_ring_ma | tx_ring_ma) >> 32) == 0 ||
         (dcfg6 & (1U << 30)) != 0)) {
        ring_program_status =
            vmk_MappedResourceWrite32(gem_resource, 0x018,
                                      (uint32_t)rx_ring_ma);
        if (ring_program_status == 0)
            ring_program_status =
                vmk_MappedResourceWrite32(gem_resource, 0x04d4,
                                          (uint32_t)(rx_ring_ma >> 32));
        if (ring_program_status == 0)
            ring_program_status =
                vmk_MappedResourceWrite32(gem_resource, 0x01c,
                                          (uint32_t)tx_ring_ma);
        if (ring_program_status == 0)
            ring_program_status =
                vmk_MappedResourceWrite32(gem_resource, 0x04c8,
                                          (uint32_t)(tx_ring_ma >> 32));
        if (ring_program_status == 0)
            ring_program_status =
                vmk_MappedResourceRead32(gem_resource, 0x018, &rxqbase);
        if (ring_program_status == 0)
            ring_program_status =
                vmk_MappedResourceRead32(gem_resource, 0x04d4,
                                         &rxqbase_hi);
        if (ring_program_status == 0)
            ring_program_status =
                vmk_MappedResourceRead32(gem_resource, 0x01c, &txqbase);
        if (ring_program_status == 0)
            ring_program_status =
                vmk_MappedResourceRead32(gem_resource, 0x04c8,
                                         &txqbase_hi);
    } else if (ring_program_status == 0) {
        ring_program_status = 1;
    }
    _vmk_WarningMessage(
        "rp1gem_mmio: inert ring bases status=%x RX=%08x:%08x "
        "TX=%08x:%08x NCR=%08x DMACFG=%08x",
        ring_program_status, rxqbase_hi, rxqbase, txqbase_hi, txqbase,
        ncr, dmacfg);

    dmacfg_after = dmacfg;
    if (ring_program_status == 0 && (dcfg6 & (1U << 30)) != 0) {
        dmacfg_after |= (1U << 23);
        ring_program_status =
            vmk_MappedResourceWrite32(gem_resource, 0x010, dmacfg_after);
        if (ring_program_status == 0)
            ring_program_status =
                vmk_MappedResourceRead32(gem_resource, 0x010,
                                         &dmacfg_after);
    }
    _vmk_WarningMessage(
        "rp1gem_mmio: empty rings initialized count=%u "
        "DCFG6_ADDR64=%u DMACFG_before=%08x after=%08x status=%x "
        "RXlast=%08x TXlast=%08x",
        RP1GEM_RING_COUNT, (dcfg6 >> 30) & 1U, dmacfg, dmacfg_after,
        ring_program_status,
        rx_ring32 != 0 ?
            rx_ring32[RP1GEM_RING_COUNT - 1].addr : 0,
        tx_ring32 != 0 ?
            tx_ring32[RP1GEM_RING_COUNT - 1].ctrl : 0);

    /*
     * Temporarily enable only the GEM management port and scan all Clause-22
     * PHY addresses. No PHY register is written; NCR is restored exactly.
     */
    if (status == 0)
        mdio_status =
            vmk_MappedResourceWrite32(gem_resource, RP1_GEM_OFFSET,
                                      ncr | (1U << 4));
    for (phy = 0; mdio_status == 0 && phy < 32; phy++) {
        mdio_status = rp1gem_mdio_read(gem_resource, phy, 1, &phy_bmsr);
        if (mdio_status == 0)
            mdio_status = rp1gem_mdio_read(gem_resource, phy, 2, &phy_id1);
        if (mdio_status == 0)
            mdio_status = rp1gem_mdio_read(gem_resource, phy, 3, &phy_id2);
        if (mdio_status == 0 && phy_bmsr != 0xffffU && phy_bmsr != 0U) {
            if (rp1gem_phy_address == 0xffffffffU)
                rp1gem_phy_address = phy;
            phy_found++;
            mdio_status = rp1gem_mdio_read(gem_resource, phy, 0, &phy_bmcr);
            if (mdio_status == 0)
                mdio_status =
                    rp1gem_mdio_read(gem_resource, phy, 4, &phy_anar);
            if (mdio_status == 0)
                mdio_status =
                    rp1gem_mdio_read(gem_resource, phy, 5, &phy_anlpar);
            if (mdio_status == 0)
                mdio_status =
                    rp1gem_mdio_read(gem_resource, phy, 9, &phy_gbcr);
            if (mdio_status == 0)
                mdio_status =
                    rp1gem_mdio_read(gem_resource, phy, 10, &phy_gbsr);
            if (mdio_status == 0)
                mdio_status =
                    rp1gem_mdio_read(gem_resource, phy, 0x19, &phy_aux);
            _vmk_WarningMessage(
                "rp1gem_mmio: PHY%u BMSR=%04x ID1=%04x ID2=%04x",
                phy, phy_bmsr, phy_id1, phy_id2);
            _vmk_WarningMessage(
                "rp1gem_mmio: PHY%u BMCR=%04x ANAR=%04x ANLPAR=%04x "
                "GBCR=%04x GBSR=%04x AUX=%04x",
                phy, phy_bmcr, phy_anar, phy_anlpar, phy_gbcr, phy_gbsr,
                phy_aux);
        }
    }
    _vmk_WarningMessage("rp1gem_mmio: MDIO scan status=%x found=%u",
                        mdio_status, phy_found);

    {
        vmk_Status restore_status;
        restore_status =
            vmk_MappedResourceWrite32(gem_resource, RP1_GEM_OFFSET, ncr);
        if (status == 0 && restore_status != 0)
            status = restore_status;
    }

    /*
     * Bounded one-descriptor jumbo RX probe.  Only the driver-owned
     * contiguous buffer is exposed to GEM.  Registers and the normal ring
     * are restored before the existing persistent RX path can start.
     */
    if (RP1GEM_ATTACH_RX_PROBES_ENABLED != 0 &&
        status == 0 && ring_program_status == 0 && phy_found != 0 &&
        rx_ring32 != 0 && jumbo_memory_map_status == 0 &&
        jumbo_memory_va2ma_status == 0 &&
        (jumbo_memory_ma >> 32) == 0) {
        memset(rp1gem_jumbo_probe_pages, 0, RP1GEM_JUMBO_PROBE_SIZE);
        rp1gem_cache_clean(
            rp1gem_jumbo_probe_pages, RP1GEM_JUMBO_PROBE_SIZE);
        for (ring_index = 0; ring_index < RP1GEM_RING_COUNT;
             ring_index++) {
            rx_ring32[ring_index].addr =
                (uint32_t)rp1gem_rx_addresses[ring_index] |
                RP1GEM_RX_USED;
            rx_ring32[ring_index].ctrl = 0;
        }
        rx_ring32[0].addr = (uint32_t)jumbo_memory_ma;
        rx_ring32[RP1GEM_RING_COUNT - 1].addr |= RP1GEM_RX_WRAP;
        rp1gem_cache_clean(
            rx_ring32,
            RP1GEM_RING_COUNT * sizeof(struct rp1gem_dma_desc32));

        jumbo_rx_test_status =
            vmk_MappedResourceWrite32(
                gem_resource, 0x004,
                ncfgr | (1U << 10) | (1U << 4) |
                    (1U << 3) | (1U << 1));
        if (jumbo_rx_test_status == 0)
            jumbo_rx_test_status =
                vmk_MappedResourceWrite32(
                    gem_resource, 0x048, RP1GEM_JUMBO_PROBE_SIZE);
        if (jumbo_rx_test_status == 0)
            jumbo_rx_test_status =
                vmk_MappedResourceWrite32(
                    gem_resource, 0x010,
                    (dmacfg & ~(0xffU << 16)) | (152U << 16));
        if (jumbo_rx_test_status == 0)
            jumbo_rx_test_status =
                vmk_MappedResourceWrite32(gem_resource, 0x020, 0x0f);
        if (jumbo_rx_test_status == 0)
            jumbo_rx_test_status =
                vmk_MappedResourceWrite32(
                    gem_resource, 0x000, ncr | (1U << 2));
        for (jumbo_rx_test_iteration = 0;
             jumbo_rx_test_status == 0 &&
             jumbo_rx_test_iteration < 10;
             jumbo_rx_test_iteration++) {
            vmk_DelayUsecs(100000);
            rp1gem_cache_invalidate(
                &rx_ring32[0], sizeof(rx_ring32[0]));
            if ((rx_ring32[0].addr & RP1GEM_RX_USED) != 0) {
                jumbo_rx_test_used = 1;
                break;
            }
        }
        (void)vmk_MappedResourceWrite32(gem_resource, 0x000, ncr);
        vmk_DelayUsecs(1000);
        (void)vmk_MappedResourceWrite32(gem_resource, 0x004, ncfgr);
        (void)vmk_MappedResourceWrite32(gem_resource, 0x010, dmacfg);
        (void)vmk_MappedResourceWrite32(gem_resource, 0x048, jml);

        if (jumbo_rx_test_used != 0) {
            rp1gem_cache_invalidate(
                rp1gem_jumbo_probe_pages, RP1GEM_JUMBO_PROBE_SIZE);
            jumbo_rx_test_ctrl = rx_ring32[0].ctrl;
            jumbo_rx_test_length = jumbo_rx_test_ctrl & 0x3fffU;
            if ((jumbo_rx_test_ctrl & 0xc000U) == 0xc000U &&
                jumbo_rx_test_length != 0 &&
                jumbo_rx_test_length <= RP1GEM_JUMBO_PROBE_SIZE &&
                vmk_PktAllocForDMAEngine(
                    RP1GEM_JUMBO_PROBE_SIZE, rp1gem_dma_engine,
                    &jumbo_rx_test_packet) == 0 &&
                jumbo_rx_test_packet != 0 &&
                vmk_PktIsFullyMapped(jumbo_rx_test_packet) != 0) {
                jumbo_rx_test_pointer =
                    vmk_PktFrameMappedPointerGet(jumbo_rx_test_packet);
                jumbo_rx_test_mapped_length =
                    vmk_PktFrameMappedLenGet(jumbo_rx_test_packet);
                if (jumbo_rx_test_pointer != 0 &&
                    jumbo_rx_test_mapped_length >=
                        jumbo_rx_test_length) {
                    memcpy(jumbo_rx_test_pointer,
                           rp1gem_jumbo_probe_pages,
                           jumbo_rx_test_length);
                    vmk_PktFrameLenSet(
                        jumbo_rx_test_packet,
                        jumbo_rx_test_length);
                    jumbo_rx_test_metadata_length =
                        vmk_PktFrameLenGet(jumbo_rx_test_packet);
                    jumbo_rx_test_copied =
                        jumbo_rx_test_metadata_length ==
                            jumbo_rx_test_length;
                }
            }
        }
        if (jumbo_rx_test_packet != 0) {
            vmk_PktRelease(jumbo_rx_test_packet);
            jumbo_rx_test_packet = 0;
        }
        for (ring_index = 0; ring_index < RP1GEM_RING_COUNT;
             ring_index++) {
            rx_ring32[ring_index].addr =
                (uint32_t)rp1gem_rx_addresses[ring_index] |
                RP1GEM_RX_USED;
            rx_ring32[ring_index].ctrl = 0;
        }
        rx_ring32[RP1GEM_RING_COUNT - 1].addr |= RP1GEM_RX_WRAP;
        rp1gem_cache_clean(
            rx_ring32,
            RP1GEM_RING_COUNT * sizeof(struct rp1gem_dma_desc32));
    }
    _vmk_WarningMessage(
        "rp1gem_mmio: bounded jumbo RX probe status=%x used=%u "
        "loops=%u ctrl=%08x length=%u mappedLen=%u metadataLen=%u "
        "copied=%u restored=1",
        jumbo_rx_test_status, jumbo_rx_test_used,
        jumbo_rx_test_iteration, jumbo_rx_test_ctrl,
        jumbo_rx_test_length, jumbo_rx_test_mapped_length,
        jumbo_rx_test_metadata_length, jumbo_rx_test_copied);

    /*
     * Controlled receive-only smoke test. Interrupts remain masked and TX is
     * never enabled. Restore every modified register before unmapping MMIO.
     */
    if (RP1GEM_ATTACH_RX_PROBES_ENABLED != 0 &&
        status == 0 && ring_program_status == 0 &&
        dma_packet_count == RP1GEM_RING_COUNT && phy_found != 0 &&
        rx_ring32 != 0) {
        rx_test_status = 0;
        for (ring_index = 0; ring_index < RP1GEM_RING_COUNT;
             ring_index++) {
            rx_ring32[ring_index].ctrl = 0;
            rx_ring32[ring_index].addr &= ~RP1GEM_RX_USED;
        }
        rp1gem_cache_clean(
            rx_ring32,
            RP1GEM_RING_COUNT * sizeof(struct rp1gem_dma_desc32));

        rx_test_ncfgr = ncfgr | (1U << 10) | (1U << 1);
        rx_test_dmacfg =
            (dmacfg & ~(0xffU << 16)) | (32U << 16);
        rx_test_status =
            vmk_MappedResourceWrite32(gem_resource, 0x088, 0x31505202);
        if (rx_test_status == 0)
            rx_test_status =
                vmk_MappedResourceWrite32(gem_resource, 0x08c, 0x00000100);
        if (rx_test_status == 0)
            rx_test_status =
                vmk_MappedResourceWrite32(gem_resource, 0x004,
                                          rx_test_ncfgr);
        if (rx_test_status == 0)
            rx_test_status =
                vmk_MappedResourceWrite32(gem_resource, 0x010,
                                          rx_test_dmacfg);
        if (rx_test_status == 0)
            rx_test_status =
                vmk_MappedResourceWrite32(gem_resource, 0x020, 0x0f);
        if (rx_test_status == 0)
            rx_test_status =
                vmk_MappedResourceWrite32(gem_resource, 0x000,
                                          ncr | (1U << 2));

        for (rx_test_iteration = 0;
             rx_test_status == 0 && rx_test_iteration < 10;
             rx_test_iteration++) {
            vmk_DelayUsecs(100000);
            rp1gem_cache_invalidate(
                rx_ring32,
                RP1GEM_RING_COUNT *
                    sizeof(struct rp1gem_dma_desc32));
            for (ring_index = 0; ring_index < RP1GEM_RING_COUNT;
                 ring_index++) {
                if ((rx_ring32[ring_index].addr &
                     RP1GEM_RX_USED) != 0) {
                    if ((rx_test_bitmap & (1U << ring_index)) == 0)
                        rx_test_used++;
                    rx_test_bitmap |= 1U << ring_index;
                    if (rx_test_first == 0xffffffffU) {
                        rx_test_first = ring_index;
                        rx_test_ctrl = rx_ring32[ring_index].ctrl;
                    }
                }
            }
        }

        (void)vmk_MappedResourceWrite32(gem_resource, 0x000, ncr);
        vmk_DelayUsecs(1000);
        (void)vmk_MappedResourceRead32(gem_resource, 0x020,
                                       &rx_test_rsr);
        (void)vmk_MappedResourceRead32(gem_resource, 0x024,
                                       &rx_test_isr);
        (void)vmk_MappedResourceWrite32(gem_resource, 0x004, ncfgr);
        (void)vmk_MappedResourceWrite32(gem_resource, 0x010, dmacfg);
        (void)vmk_MappedResourceWrite32(gem_resource, 0x088, sa1b);
        (void)vmk_MappedResourceWrite32(gem_resource, 0x08c, sa1t);
        for (ring_index = 0; ring_index < RP1GEM_RING_COUNT;
             ring_index++)
            rx_ring32[ring_index].addr |= RP1GEM_RX_USED;
        rp1gem_cache_clean(
            rx_ring32,
            RP1GEM_RING_COUNT * sizeof(struct rp1gem_dma_desc32));
    }
    _vmk_WarningMessage(
        "rp1gem_mmio: controlled RX status=%x used=%u first=%u "
        "ctrl=%08x RSR=%08x ISR=%08x loops=%u restoredNCR=%08x",
        rx_test_status, rx_test_used, rx_test_first, rx_test_ctrl,
        rx_test_rsr, rx_test_isr, rx_test_iteration, ncr);

    if (RP1GEM_ATTACH_RX_PROBES_ENABLED != 0 &&
        dma_packet_count == RP1GEM_RING_COUNT) {
        dma_unmap_status = 0;
        for (ring_index = 0; ring_index < dma_packet_count;
             ring_index++) {
            dma_unmap_status =
                vmk_DMAUnmapElem(
                    rp1gem_dma_engine, 1,
                    &rp1gem_rx_mapped_elements[ring_index]);
            if (dma_unmap_status != 0)
                break;
        }
        _vmk_WarningMessage(
            "rp1gem_mmio: DMA RX unmap probe status=%x count=%u "
            "receivedIndex=%u",
            dma_unmap_status, ring_index, rx_test_first);
    }

    if (RP1GEM_ATTACH_RX_PROBES_ENABLED != 0 &&
        rx_test_first < RP1GEM_RING_COUNT) {
        uint32_t received_length = rx_test_ctrl & 0xfffU;
        uint32_t metadata_length;

        vmk_PktFrameLenSet(
            rp1gem_rx_packets[rx_test_first], received_length);
        metadata_length =
            vmk_PktFrameLenGet(rp1gem_rx_packets[rx_test_first]);
        _vmk_WarningMessage(
            "rp1gem_mmio: RX packet metadata probe index=%u "
            "descriptorLength=%u packetLength=%u",
            rx_test_first, received_length, metadata_length);
    }

    if (RP1GEM_ATTACH_RX_PROBES_ENABLED != 0 &&
        dma_unmap_status == 0) {
        uint32_t prepared = 0;
        uint32_t replacement_count = 0;
        uint32_t rearmed_count = 0;
        vmk_Status rearm_status = 0;

        for (ring_index = 0;
             ring_index < RP1GEM_RING_COUNT &&
             prepared < RP1GEM_PENDING_RX_COUNT;
             ring_index++) {
            uint32_t descriptor_control = rx_ring32[ring_index].ctrl;
            uint32_t received_length = descriptor_control & 0xfffU;

            if ((rx_test_bitmap & (1U << ring_index)) == 0 ||
                (descriptor_control & 0xc000U) != 0xc000U ||
                received_length == 0 ||
                rp1gem_rx_packets[ring_index] == 0)
                continue;
            rp1gem_pending_rx_packets[prepared] =
                rp1gem_rx_packets[ring_index];
            rp1gem_pending_rx_lengths[prepared] = received_length;
            rp1gem_rx_packets[ring_index] = 0;
            prepared++;
        }
        rp1gem_pending_rx_next = 0;
        rp1gem_pending_rx_count = prepared;
        _vmk_WarningMessage(
            "rp1gem_mmio: RX multi-packet handoff prepared=%u "
            "observed=%u cap=%u",
            prepared, rx_test_used, RP1GEM_PENDING_RX_COUNT);

        /*
         * Rebuild a complete mapped RX ring after ownership of completed
         * packets has moved to NetPoll.  Received slots get fresh packets;
         * untouched slots reuse their existing packets after the preceding
         * DMA unmap.  RX remains disabled, so this version validates only
         * replacement allocation, remapping, and descriptor rearming.
         */
        for (ring_index = 0;
             rearm_status == 0 && ring_index < RP1GEM_RING_COUNT;
             ring_index++) {
            struct rp1gem_sg_element *rearm_sg;

            if (rp1gem_rx_packets[ring_index] == 0) {
                rearm_status =
                    vmk_PktAllocForDMAEngine(
                        2048, rp1gem_dma_engine,
                        &rp1gem_rx_packets[ring_index]);
                if (rearm_status != 0 ||
                    rp1gem_rx_packets[ring_index] == 0) {
                    if (rearm_status == 0)
                        rearm_status = 1;
                    break;
                }
                replacement_count++;
            }

            rearm_sg =
                vmk_PktSgElemGet(rp1gem_rx_packets[ring_index], 0);
            if (rearm_sg == 0 || rearm_sg->length < 2048 ||
                (rearm_sg->address >> 32) != 0) {
                rearm_status = 1;
                break;
            }
            dma_mapped_count = 0;
            rearm_status =
                vmk_DMAMapElem(
                    rp1gem_dma_engine, 1, rearm_sg, 1,
                    &rp1gem_rx_mapped_elements[ring_index],
                    &dma_mapped_count);
            if (rearm_status != 0 ||
                rp1gem_rx_mapped_elements[ring_index].length < 2048 ||
                (rp1gem_rx_mapped_elements[ring_index].address >> 32) != 0) {
                if (rearm_status == 0)
                    rearm_status = 1;
                break;
            }

            rp1gem_rx_addresses[ring_index] =
                rp1gem_rx_mapped_elements[ring_index].address;
            rx_ring32[ring_index].addr =
                (uint32_t)rp1gem_rx_addresses[ring_index] |
                RP1GEM_RX_USED;
            if (ring_index == RP1GEM_RING_COUNT - 1)
                rx_ring32[ring_index].addr |= RP1GEM_RX_WRAP;
            rx_ring32[ring_index].ctrl = 0;
            rearmed_count++;
        }
        if (rearm_status == 0)
            rp1gem_cache_clean(
                rx_ring32,
                RP1GEM_RING_COUNT *
                    sizeof(struct rp1gem_dma_desc32));
        _vmk_WarningMessage(
            "rp1gem_mmio: RX replacement rearm status=%x "
            "replacements=%u mapped=%u first=%lx last=%lx",
            rearm_status, replacement_count, rearmed_count,
            rearmed_count != 0 ? rp1gem_rx_addresses[0] : 0,
            rearmed_count == RP1GEM_RING_COUNT ?
                rp1gem_rx_addresses[RP1GEM_RING_COUNT - 1] : 0);

        if (rearm_status == 0 &&
            rearmed_count == RP1GEM_RING_COUNT) {
            uint32_t second_bitmap = 0;
            uint32_t second_used = 0;
            uint32_t second_prepared = 0;
            uint32_t second_unmapped = 0;
            uint32_t second_replacements = 0;
            uint32_t second_rearmed = 0;
            uint32_t second_iteration;
            vmk_Status second_status = 0;
            vmk_Status second_unmap_status = 0;
            vmk_Status second_rearm_status = 0;

            for (ring_index = 0; ring_index < RP1GEM_RING_COUNT;
                 ring_index++)
                rx_ring32[ring_index].addr &= ~RP1GEM_RX_USED;
            rp1gem_cache_clean(
                rx_ring32,
                RP1GEM_RING_COUNT *
                    sizeof(struct rp1gem_dma_desc32));

            second_status =
                vmk_MappedResourceWrite32(gem_resource, 0x088,
                                          0x31505202);
            if (second_status == 0)
                second_status =
                    vmk_MappedResourceWrite32(gem_resource, 0x08c,
                                              0x00000100);
            if (second_status == 0)
                second_status =
                    vmk_MappedResourceWrite32(gem_resource, 0x004,
                                              rx_test_ncfgr);
            if (second_status == 0)
                second_status =
                    vmk_MappedResourceWrite32(gem_resource, 0x010,
                                              rx_test_dmacfg);
            if (second_status == 0)
                second_status =
                    vmk_MappedResourceWrite32(gem_resource, 0x020, 0x0f);
            if (second_status == 0)
                second_status =
                    vmk_MappedResourceWrite32(gem_resource, 0x000,
                                              ncr | (1U << 2));

            for (second_iteration = 0;
                 second_status == 0 && second_iteration < 10;
                 second_iteration++) {
                vmk_DelayUsecs(100000);
                rp1gem_cache_invalidate(
                    rx_ring32,
                    RP1GEM_RING_COUNT *
                        sizeof(struct rp1gem_dma_desc32));
                for (ring_index = 0;
                     ring_index < RP1GEM_RING_COUNT;
                     ring_index++) {
                    if ((rx_ring32[ring_index].addr &
                         RP1GEM_RX_USED) != 0) {
                        if ((second_bitmap &
                             (1U << ring_index)) == 0)
                            second_used++;
                        second_bitmap |= 1U << ring_index;
                    }
                }
            }

            (void)vmk_MappedResourceWrite32(gem_resource, 0x000, ncr);
            vmk_DelayUsecs(1000);
            (void)vmk_MappedResourceWrite32(gem_resource, 0x004, ncfgr);
            (void)vmk_MappedResourceWrite32(gem_resource, 0x010, dmacfg);
            (void)vmk_MappedResourceWrite32(gem_resource, 0x088, sa1b);
            (void)vmk_MappedResourceWrite32(gem_resource, 0x08c, sa1t);

            for (ring_index = 0;
                 ring_index < RP1GEM_RING_COUNT;
                 ring_index++) {
                second_unmap_status =
                    vmk_DMAUnmapElem(
                        rp1gem_dma_engine, 1,
                        &rp1gem_rx_mapped_elements[ring_index]);
                if (second_unmap_status != 0)
                    break;
                second_unmapped++;
            }
            if (second_unmap_status == 0) {
                for (ring_index = 0;
                     ring_index < RP1GEM_RING_COUNT &&
                     rp1gem_pending_rx_count <
                         RP1GEM_PENDING_RX_COUNT;
                     ring_index++) {
                    uint32_t descriptor_control =
                        rx_ring32[ring_index].ctrl;
                    uint32_t received_length =
                        descriptor_control & 0xfffU;
                    uint32_t pending_index;

                    if ((second_bitmap & (1U << ring_index)) == 0 ||
                        (descriptor_control & 0xc000U) != 0xc000U ||
                        received_length == 0 ||
                        rp1gem_rx_packets[ring_index] == 0)
                        continue;
                    pending_index = rp1gem_pending_rx_count;
                    rp1gem_pending_rx_packets[pending_index] =
                        rp1gem_rx_packets[ring_index];
                    rp1gem_pending_rx_lengths[pending_index] =
                        received_length;
                    rp1gem_rx_packets[ring_index] = 0;
                    rp1gem_pending_rx_count++;
                    second_prepared++;
                }
            }
            for (ring_index = 0;
                 second_rearm_status == 0 &&
                 ring_index < RP1GEM_RING_COUNT;
                 ring_index++) {
                struct rp1gem_sg_element *second_rearm_sg;

                if (rp1gem_rx_packets[ring_index] == 0) {
                    second_rearm_status =
                        vmk_PktAllocForDMAEngine(
                            2048, rp1gem_dma_engine,
                            &rp1gem_rx_packets[ring_index]);
                    if (second_rearm_status != 0 ||
                        rp1gem_rx_packets[ring_index] == 0) {
                        if (second_rearm_status == 0)
                            second_rearm_status = 1;
                        break;
                    }
                    second_replacements++;
                }
                second_rearm_sg =
                    vmk_PktSgElemGet(
                        rp1gem_rx_packets[ring_index], 0);
                if (second_rearm_sg == 0 ||
                    second_rearm_sg->length < 2048 ||
                    (second_rearm_sg->address >> 32) != 0) {
                    second_rearm_status = 1;
                    break;
                }
                dma_mapped_count = 0;
                second_rearm_status =
                    vmk_DMAMapElem(
                        rp1gem_dma_engine, 1, second_rearm_sg, 1,
                        &rp1gem_rx_mapped_elements[ring_index],
                        &dma_mapped_count);
                if (second_rearm_status != 0 ||
                    rp1gem_rx_mapped_elements[ring_index].length <
                        2048 ||
                    (rp1gem_rx_mapped_elements[ring_index].address >>
                     32) != 0) {
                    if (second_rearm_status == 0)
                        second_rearm_status = 1;
                    break;
                }
                rp1gem_rx_addresses[ring_index] =
                    rp1gem_rx_mapped_elements[ring_index].address;
                rx_ring32[ring_index].addr =
                    (uint32_t)rp1gem_rx_addresses[ring_index] |
                    RP1GEM_RX_USED;
                if (ring_index == RP1GEM_RING_COUNT - 1)
                    rx_ring32[ring_index].addr |= RP1GEM_RX_WRAP;
                rx_ring32[ring_index].ctrl = 0;
                second_rearmed++;
            }
            if (second_rearm_status == 0)
                rp1gem_cache_clean(
                    rx_ring32,
                    RP1GEM_RING_COUNT *
                        sizeof(struct rp1gem_dma_desc32));
            _vmk_WarningMessage(
                "rp1gem_mmio: RX replacement second pass status=%x "
                "used=%u bitmap=%08x loops=%u unmapStatus=%x "
                "unmapped=%u prepared=%u totalPending=%u "
                "rearmStatus=%x replacements=%u mapped=%u",
                second_status, second_used, second_bitmap,
                second_iteration, second_unmap_status, second_unmapped,
                second_prepared, rp1gem_pending_rx_count,
                second_rearm_status, second_replacements,
                second_rearmed);
        }
    }

    _vmk_WarningMessage(
        "rp1gem_mmio: attach RX ownership probes disabled=%u",
        RP1GEM_ATTACH_RX_PROBES_ENABLED == 0);
    _vmk_WarningMessage(
        "rp1gem_mmio: retaining GEM resource mapping for startIO "
        "resource=%lx acpi=%lx",
        rp1gem_gem_resource, rp1gem_gem_acpi_device);

    if (status != 0) {
        _vmk_WarningMessage("rp1gem_mmio: GEM NCR read failed status=%x",
                            status);
        return status;
    }

    return 0;
}

static vmk_Status
rp1gem_noop(vmk_Device device)
{
    (void)device;
    return 0;
}

static vmk_Status
rp1gem_scan(vmk_Device device)
{
    return rp1gem_register_uplink_skeleton(device);
}

struct rp1gem_driver_ops {
    vmk_Status (*attach)(vmk_Device);
    vmk_Status (*scan)(vmk_Device);
    vmk_Status (*detach)(vmk_Device);
    vmk_Status (*quiesce)(vmk_Device);
    vmk_Status (*start)(vmk_Device);
    vmk_Status (*forget)(vmk_Device);
};

struct rp1gem_driver_props {
    uint32_t module_id;
    uint8_t name[32];
    uint32_t reserved0;
    struct rp1gem_driver_ops *ops;
    uint8_t reserved1[8];
};

/*
 * Reconstructed from the ESXi 8 ARM genet/eqos modules.  vmk_Name is
 * 32 bytes; the fixed heap properties occupy 64 bytes in this ABI.
 */
struct rp1gem_heap_props {
    uint32_t type;
    uint8_t name[32];
    uint32_t module_id;
    uint32_t initial_size;
    uint32_t maximum_size;
    int32_t timeout_ms;
    uint32_t reserved0;
    uint64_t reserved1;
};
_Static_assert(sizeof(struct rp1gem_heap_props) == 64,
               "unexpected heap properties ABI size");

static struct rp1gem_driver_ops rp1gem_ops = {
    rp1gem_attach,
    rp1gem_scan,
    rp1gem_noop,
    rp1gem_noop,
    rp1gem_noop,
    rp1gem_noop,
};

static struct rp1gem_driver_props rp1gem_props = {
    0,
    { 0 },
    0,
    &rp1gem_ops,
    { 0 },
};

static vmk_HeapID rp1gem_heap;

__attribute__((section(".vmkmodinfo"), used, aligned(8)))
static const char __vmk_nsRequiredInfo_str[] =
    "nsRequired=com.vmware.vmkapi#v3_0_0_0";

__attribute__((section(".vmkmodinfo"), used, aligned(8)))
static const char __vmk_versionInfo_str[] =
    "version=0.0.213-1rp1gem.803.0.55.24449057";

__attribute__((section(".vmkmodinfo"), used, aligned(8)))
static const char __vmk_buildTypeInfo_str[] = "buildType=release";

__attribute__((section(".vmkmodinfo"), used, aligned(8)))
static const char __vmk_licenseInfo_str[] = "license=BSD";

__attribute__((section(".vmkrequiredns"), used))
static const char rp1gem_required_namespace[] =
    "com.vmware.vmkapi#v3_0_0_0";

int
init_module(void)
{
    vmk_Status status;
    struct rp1gem_heap_props heap_props = {
        1,
        { 0 },
        0,
        64 * 1024,
        1024 * 1024,
        -1,
        0,
        0,
    };

    rp1gem_props.module_id = vmk_ModuleCurrentID;
    heap_props.module_id = vmk_ModuleCurrentID;
    status = vmk_NameInitialize(heap_props.name, "rp1gem_heap");
    if (status != 0) {
        return status;
    }
    status = vmk_HeapCreate(&heap_props, &rp1gem_heap);
    if (status != 0) {
        _vmk_WarningMessage("rp1gem_mmio: heap create failed status=%x",
                            status);
        return status;
    }
    vmk_ModuleSetHeapID(vmk_ModuleCurrentID, rp1gem_heap);
    status = vmk_NameInitialize(rp1gem_props.name, "rp1gem_mmio");
    if (status != 0) {
        vmk_HeapDestroy(rp1gem_heap);
        rp1gem_heap = 0;
        return status;
    }
    status = vmk_DriverRegister(&rp1gem_props, &rp1gem_driver);
    if (status != 0) {
        vmk_HeapDestroy(rp1gem_heap);
        rp1gem_heap = 0;
    }
    return status;
}

void
cleanup_module(void)
{
    rp1gem_stop_poll_world();
    if (rp1gem_driver != 0) {
        (void)vmk_DriverUnregister(rp1gem_driver);
        rp1gem_driver = 0;
    }
    if (rp1gem_netpoll_handle != 0) {
        if (rp1gem_netpoll_enabled != 0) {
            (void)vmk_NetPollDisable(rp1gem_netpoll_handle);
            (void)vmk_NetPollFlushRx(rp1gem_netpoll_handle);
            rp1gem_netpoll_enabled = 0;
        }
        (void)vmk_NetPollDestroy(rp1gem_netpoll_handle);
        rp1gem_netpoll_handle = 0;
    }
    if (rp1gem_interrupt_registered != 0) {
        (void)vmk_IntrUnregister(
            vmk_ModuleCurrentID, rp1gem_interrupt_cookie,
            &rp1gem_adapter_context);
        rp1gem_interrupt_registered = 0;
    }
    if (rp1gem_gem_resource_mapped != 0 &&
        rp1gem_gem_acpi_device != 0) {
        (void)vmk_ACPIUnmapIOResource(
            vmk_ModuleCurrentID, rp1gem_gem_acpi_device, 0);
        rp1gem_gem_resource_mapped = 0;
    }
    if (rp1gem_interrupt_cookie_allocated != 0 &&
        rp1gem_gem_acpi_device != 0) {
        (void)vmk_ACPIFreeIntrCookie(
            vmk_ModuleCurrentID, rp1gem_gem_acpi_device);
        rp1gem_interrupt_cookie_allocated = 0;
    }
    rp1gem_gem_acpi_device = 0;
    rp1gem_parent_device = 0;
    if (rp1gem_tx_bounce_packet != 0) {
        vmk_PktRelease(rp1gem_tx_bounce_packet);
        rp1gem_tx_bounce_packet = 0;
        rp1gem_tx_bounce_pointer = 0;
        rp1gem_tx_bounce_length = 0;
    }
    if (rp1gem_sg_ops != 0) {
        (void)vmk_SgDestroyOpsHandle(rp1gem_sg_ops);
        rp1gem_sg_ops = 0;
    }
    if (rp1gem_heap != 0) {
        vmk_HeapDestroy(vmk_ModuleGetHeapID(vmk_ModuleCurrentID));
        rp1gem_heap = 0;
    }
}
