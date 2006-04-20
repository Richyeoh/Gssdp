/* 
 * (C) 2006 OpenedHand Ltd.
 *
 * Author: Jorn Baayen <jorn@openedhand.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/* XXX connect to "notify::available" and handle actual advertisement */

#include <config.h>
#include <uuid/uuid.h>

#include "gssdp-device-private.h"

/* Hack around G_DEFINE_TYPE hardcoding the type function name */
#define gssdp_device_get_type gssdp_device_type

G_DEFINE_TYPE (GSSDPDevice,
               gssdp_device,
               GSSDP_TYPE_DISCOVERABLE);

#undef gssdp_device_get_type

struct _GSSDPDevicePrivate {
        char   uuid[17];

        GList *services;
};

enum {
        PROP_0,
        PROP_UUID,
        PROP_SERVICES
};

static void
gssdp_device_init (GSSDPDevice *device)
{
        device->priv = G_TYPE_INSTANCE_GET_PRIVATE
                                        (device,
                                         GSSDP_TYPE_DEVICE,
                                         GSSDPDevicePrivate);

        /* Generate UUID */
        uuid_generate ((unsigned char *) device->priv->uuid);
}

static void
gssdp_device_get_property (GObject    *object,
                           guint       property_id,
                           GValue     *value,
                           GParamSpec *pspec)
{
        GSSDPDevice *device;

        device = GSSDP_DEVICE (object);

        switch (property_id) {
        case PROP_UUID:
                g_value_set_string
                        (value,
                         gssdp_device_get_uuid (device));
                break;
        case PROP_SERVICES:
                g_value_set_pointer
                        (value,
                         (gpointer) gssdp_device_get_services (device));
                break;
        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
                break;
        }
}

static void
gssdp_device_class_init (GSSDPDeviceClass *klass)
{
        GObjectClass *object_class;

	object_class = G_OBJECT_CLASS (klass);

	object_class->get_property = gssdp_device_get_property;

        g_type_class_add_private (klass, sizeof (GSSDPDevicePrivate));

        g_object_class_install_property
                (object_class,
                 PROP_UUID,
                 g_param_spec_string
                         ("uuid",
                          "UUID",
                          "The device UUID.",
                          NULL,
                          G_PARAM_READABLE |
                          G_PARAM_STATIC_NAME | G_PARAM_STATIC_NICK |
                          G_PARAM_STATIC_BLURB));

        g_object_class_install_property
                (object_class,
                 PROP_SERVICES,
                 g_param_spec_pointer
                         ("services",
                          "Services",
                          "The list of contained services.",
                          G_PARAM_READABLE |
                          G_PARAM_STATIC_NAME | G_PARAM_STATIC_NICK |
                          G_PARAM_STATIC_BLURB));
}

/**
 * gssdp_device_new
 * @parent: A #GSSDPRootDevice
 * @type: A string identifying the type of the device
 * @version: A #gushort identifying the version of the device type
 *
 * Return value: A new #GSSDPDevice object.
 **/
GSSDPDevice *
gssdp_device_new (GSSDPRootDevice *parent,
                  const char      *type,
                  gushort          version)
{
        return g_object_new (GSSDP_TYPE_DEVICE,
                             "parent",  parent,
                             "type",    type,
                             "version", version,
                             NULL);
}

/**
 * gssdp_device_get_uuid
 * @device: A #GSSDPDevice
 *
 * Return value: The device UUID.
 **/
const char *
gssdp_device_get_uuid (GSSDPDevice *device)
{
        g_return_val_if_fail (GSSDP_IS_DEVICE (device), NULL);

        return device->priv->uuid;
}

/**
 * Sets the list of contained services of @device to @services
 **/
void
gssdp_device_set_services (GSSDPDevice *device,
                           GList       *services)
{
        g_return_if_fail (GSSDP_IS_DEVICE (device));

        device->priv->services = services;

        g_object_notify (G_OBJECT (device), "services");
}

/**
 * gssdp_device_get_services
 * @device: A #GSSDPDevice
 *
 * Return value: A #GList of services contained in @device.
 **/
const GList *
gssdp_device_get_services (GSSDPDevice *device)
{
        g_return_val_if_fail (GSSDP_IS_DEVICE (device), NULL);
        
        return device->priv->services;
}
