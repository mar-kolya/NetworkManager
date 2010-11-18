/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* NetworkManager -- Network link manager
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Copyright (C) 2010 Red Hat, Inc.
 */

#ifndef NM_MANAGER_AUTH_H
#define NM_MANAGER_AUTH_H

#include <polkit/polkit.h>
#include <glib.h>
#include <dbus/dbus-glib.h>

#include <nm-connection.h>
#include "nm-dbus-manager.h"
#include "nm-session-monitor.h"

#define NM_AUTH_PERMISSION_ENABLE_DISABLE_NETWORK     "org.freedesktop.NetworkManager.enable-disable-network"
#define NM_AUTH_PERMISSION_SLEEP_WAKE                 "org.freedesktop.NetworkManager.sleep-wake"
#define NM_AUTH_PERMISSION_ENABLE_DISABLE_WIFI        "org.freedesktop.NetworkManager.enable-disable-wifi"
#define NM_AUTH_PERMISSION_ENABLE_DISABLE_WWAN        "org.freedesktop.NetworkManager.enable-disable-wwan"
#define NM_AUTH_PERMISSION_NETWORK_CONTROL            "org.freedesktop.NetworkManager.network-control"
#define NM_AUTH_PERMISSION_WIFI_SHARE_PROTECTED       "org.freedesktop.NetworkManager.wifi.share.protected"
#define NM_AUTH_PERMISSION_WIFI_SHARE_OPEN            "org.freedesktop.NetworkManager.wifi.share.open"
#define NM_AUTH_PERMISSION_SETTINGS_CONNECTION_MODIFY "org.freedesktop.NetworkManager.settings.modify"
#define NM_AUTH_PERMISSION_SETTINGS_HOSTNAME_MODIFY   "org.freedesktop.NetworkManager.settings.hostname.modify"


typedef struct NMAuthChain NMAuthChain;

typedef enum {
	NM_AUTH_CALL_RESULT_UNKNOWN,
	NM_AUTH_CALL_RESULT_YES,
	NM_AUTH_CALL_RESULT_AUTH,
	NM_AUTH_CALL_RESULT_NO,
} NMAuthCallResult;

typedef void (*NMAuthChainResultFunc) (NMAuthChain *chain,
                                       GError *error,
                                       DBusGMethodInvocation *context,
                                       gpointer user_data);

typedef void (*NMAuthChainCallFunc) (NMAuthChain *chain,
                                     const char *permission,
                                     GError *error,
                                     NMAuthCallResult result,
                                     gpointer user_data);

NMAuthChain *nm_auth_chain_new (PolkitAuthority *authority,
                                DBusGMethodInvocation *context,
                                DBusGProxy *proxy,
                                NMAuthChainResultFunc done_func,
                                gpointer user_data);

NMAuthChain *nm_auth_chain_new_raw_message (PolkitAuthority *authority,
                                            DBusMessage *message,
                                            NMAuthChainResultFunc done_func,
                                            gpointer user_data);

gpointer nm_auth_chain_get_data (NMAuthChain *chain, const char *tag);

void nm_auth_chain_set_data (NMAuthChain *chain,
                             const char *tag,
                             gpointer data,
                             GDestroyNotify data_destroy);

NMAuthCallResult nm_auth_chain_get_result (NMAuthChain *chain,
                                           const char *permission);

gboolean nm_auth_chain_add_call (NMAuthChain *chain,
                                 const char *permission,
                                 gboolean allow_interaction);

void nm_auth_chain_unref (NMAuthChain *chain);

/* Utils */
gboolean nm_auth_get_caller_uid (DBusGMethodInvocation *context,
                                 NMDBusManager *dbus_mgr,
                                 gulong *out_uid,
                                 char **out_error_desc);

/* Caller must free returned error description */
gboolean nm_auth_uid_in_acl (NMConnection *connection,
                             NMSessionMonitor *smon,
                             gulong uid,
                             char **out_error_desc);

#endif /* NM_MANAGER_AUTH_H */

