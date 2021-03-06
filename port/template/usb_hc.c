#include "usbh_core.h"
#include <rtthread.h>
#include "hal_data.h"
#include <rtdevice.h>

#ifndef USBH_IRQHandler
#define USBH_IRQHandler OTG_FS_IRQHandler
#endif

struct usb_xxx_priv {
    volatile bool connected; /* Connected to device */
    volatile bool pscwait;   /* True: Thread is waiting for a port event */
    usb_osal_sem_t exclsem;  /* Support mutually exclusive access */
} g_usbhost;

__WEAK void usb_hc_low_level_init(void)
{
}
extern char* e_usb_status_msg[30];

int usb_hc_init(void)
{
    usb_status_t     event, last_event = 0;
    usb_event_info_t event_info;
    int loop_count = 0;
    memset(&g_usbhost, 0, sizeof(struct usb_xxx_priv));

    g_usbhost.exclsem = usb_osal_mutex_create();
    usb_hc_low_level_init();

    //g_usb_on_usb.open(&g_basic0_ctrl, &g_basic0_cfg);

    while(0)
    {

        /* Get USB event data */
        g_usb_on_usb.eventGet(&event_info, &event);
        if (last_event != event) {
            rt_kprintf("%4d UsbEvent %s\n", loop_count++, e_usb_status_msg[event]);
            last_event = event;
        }

        /* Handle the received event (if any) */
        switch (event)
        {
            case USB_STATUS_CONFIGURED:
                /* Configure virtual UART settings */
    //            set_line_coding(&g_basic0_ctrl, event_info.device_address); /* CDC Class request "SetLineCoding" */
    //            device_address = event_info.device_address;
                rt_kprintf("%4d device_address:%d\n", loop_count++, event_info.device_address);
                g_usbhost.connected = true;
                goto exit;
                break;
            default:
                break;
        }
    }
exit:
    return 0;
}

int usbh_reset_port(const uint8_t port)
{
    return 0;
}

uint8_t usbh_get_port_speed(const uint8_t port)
{
    return USB_SPEED_FULL;
}

int usbh_ep0_reconfigure(usbh_epinfo_t ep, uint8_t dev_addr, uint8_t ep_mps, uint8_t speed)
{
    int ret;

    ret = usb_osal_mutex_take(g_usbhost.exclsem);
    if (ret < 0) {
        return ret;
    }

    usb_osal_mutex_give(g_usbhost.exclsem);
    return ret;
}

int usbh_ep_alloc(usbh_epinfo_t *ep, const struct usbh_endpoint_cfg *ep_cfg)
{
    int ret;
    struct usbh_hubport *hport;
    (void) hport;

    rt_kprintf("usbh_ep_alloc %x\n", ep);
    ret = usb_osal_mutex_take(g_usbhost.exclsem);
    if (ret < 0) {
        return ret;
    }

    hport = ep_cfg->hport;

    if (ep_cfg->ep_type == USB_ENDPOINT_TYPE_CONTROL) {
    } else {
    }

    usb_osal_mutex_give(g_usbhost.exclsem);
    return 0;
}

int usbh_ep_free(usbh_epinfo_t ep)
{
    int ret;

    ret = usb_osal_mutex_take(g_usbhost.exclsem);
    if (ret < 0) {
        return ret;
    }

    usb_osal_mutex_give(g_usbhost.exclsem);
    return 0;
}

int usbh_control_transfer(usbh_epinfo_t ep, struct usb_setup_packet *setup, uint8_t *buffer)
{
    int ret;
    rt_kprintf("usbh_control_transfer\n");
    ret = usb_osal_mutex_take(g_usbhost.exclsem);
    if (ret < 0) {
        return ret;
    }

    ret = g_usb_on_usb.hostControlTransfer(&g_basic0_ctrl, (void*)setup, (uint8_t *) buffer, 1);

    rt_kprintf("ret %d\n", ret);
    usb_osal_mutex_give(g_usbhost.exclsem);
    return -ret;
}

int usbh_ep_bulk_transfer(usbh_epinfo_t ep, uint8_t *buffer, uint32_t buflen)
{
    int ret;

    rt_kprintf("usbh_ep_bulk_transfer\n");
    ret = usb_osal_mutex_take(g_usbhost.exclsem);
    if (ret < 0) {
        return ret;
    }
    //R_USB_PipeWrite(p_api_ctrl, buffer, buflen, ep);
    usb_osal_mutex_give(g_usbhost.exclsem);
    return ret;
}

int usbh_ep_intr_transfer(usbh_epinfo_t ep, uint8_t *buffer, uint32_t buflen)
{
    int ret;

    rt_kprintf("usbh_ep_intr_transfer\n");
    ret = usb_osal_mutex_take(g_usbhost.exclsem);
    if (ret < 0) {
        return ret;
    }

    usb_osal_mutex_give(g_usbhost.exclsem);
    return ret;
}

int usbh_ep_bulk_async_transfer(usbh_epinfo_t ep, uint8_t *buffer, uint32_t buflen, usbh_asynch_callback_t callback, void *arg)
{
    int ret;

    ret = usb_osal_mutex_take(g_usbhost.exclsem);
    if (ret < 0) {
        return ret;
    }

    usb_osal_mutex_give(g_usbhost.exclsem);

    return ret;
}

int usbh_ep_intr_async_transfer(usbh_epinfo_t ep, uint8_t *buffer, uint32_t buflen, usbh_asynch_callback_t callback, void *arg)
{
    int ret;

    ret = usb_osal_mutex_take(g_usbhost.exclsem);
    if (ret < 0) {
        return ret;
    }

    usb_osal_mutex_give(g_usbhost.exclsem);

    return ret;
}

int usb_ep_cancel(usbh_epinfo_t ep)
{
    return 0;
}

void USBH_IRQHandler(void)
{
}


