/* packet-cell_broadcast.h
 *
 * $Id: packet-cell_broadcast.h 47177 2013-01-20 17:14:24Z pascal $
 *
 * Copyright 2011, Mike Morrin <mike.morrin [AT] ipaccess.com>,
 *
 * Wireshark - Network traffic analyzer
 * By Gerald Combs <gerald@wireshark.org>
 * Copyright 1998 Gerald Combs
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef PACKET_CELL_BROADCAST_H
#define PACKET_CELL_BROADCAST_H


/**
 * Dissects the GSM/UMTS/SABP Message Identifier
 * 
 * @param tvb
 * @param tree
 * @param offset
 */
void dissect_cbs_message_identifier(tvbuff_t *tvb, proto_tree *tree, guint16 offset);


/**
 * Decodes the GSM/UMTS/SABP message Serial Number
 * 
 * @param tvb
 * @param tree
 * @param offset
 */
void dissect_cbs_serial_number(tvbuff_t *tvb, proto_tree *tree, guint16 offset);


/**
 * Dissects UMTS/SABP Cell Broadcast Message
 * 
 * @param tvb
 * @param pinfo
 * @param tree
 */
guint16 dissect_umts_cell_broadcast_message(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree);

/**
 * Dissects CB Data
 */
tvbuff_t * dissect_cbs_data(guint8 sms_encoding, tvbuff_t *tvb, proto_tree *tree, packet_info *pinfo, guint16 offset);

#endif /* PACKET_CELL_BROADCAST_H */
