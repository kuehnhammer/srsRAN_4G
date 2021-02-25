/**
 *
 * \section COPYRIGHT
 *
 * Copyright 2013-2020 Software Radio Systems Limited
 *
 * By using this file, you agree to the terms and conditions set
 * forth in the LICENSE file which can be found at the top level of
 * the distribution.
 *
 */

#ifndef SRSLTE_UE_RLC_INTERFACES_H
#define SRSLTE_UE_RLC_INTERFACES_H

#include "srslte/common/interfaces_common.h"
#include "srslte/interfaces/rlc_interface_types.h"

namespace srsue {

class rlc_interface_rrc
{
public:
  virtual void reset()                                                     = 0;
  virtual void reestablish()                                               = 0;
  virtual void reestablish(uint32_t lcid)                                  = 0;
  virtual void add_bearer(uint32_t lcid, const srslte::rlc_config_t& cnfg) = 0;
  virtual void add_bearer_mrb(uint32_t lcid)                               = 0;
  virtual void del_bearer(uint32_t lcid)                                   = 0;
  virtual void suspend_bearer(uint32_t lcid)                               = 0;
  virtual void resume_bearer(uint32_t lcid)                                = 0;
  virtual void change_lcid(uint32_t old_lcid, uint32_t new_lcid)           = 0;
  virtual bool has_bearer(uint32_t lcid)                                   = 0;
  virtual bool has_data(const uint32_t lcid)                               = 0;
  virtual bool is_suspended(const uint32_t lcid)                           = 0;
  virtual void write_sdu(uint32_t lcid, srslte::unique_byte_buffer_t sdu)  = 0;
};

class rlc_interface_pdcp
{
public:
  ///< PDCP calls RLC to push an RLC SDU. SDU gets placed into the buffer
  ///< MAC pulls RLC PDUs according to TB size
  virtual void write_sdu(uint32_t lcid, srslte::unique_byte_buffer_t sdu) = 0;

  ///< Indicate RLC that a certain SN can be discarded
  virtual void discard_sdu(uint32_t lcid, uint32_t discard_sn) = 0;

  ///< Helper to query RLC mode
  virtual bool rb_is_um(uint32_t lcid) = 0;

  ///< Allow PDCP to query SDU queue status
  virtual bool sdu_queue_is_full(uint32_t lcid) = 0;
};

class rlc_interface_mac : public srslte::read_pdu_interface
{
public:
  /* MAC calls has_data() to query whether a logical channel has data to transmit (without
   * knowing how much. This function should return quickly. */
  virtual bool has_data_locked(const uint32_t lcid) = 0;

  /* MAC calls RLC to get the buffer state for a logical channel. */
  virtual uint32_t get_buffer_state(const uint32_t lcid) = 0;

  const static int MAX_PDU_SEGMENTS = 20;

  /* MAC calls RLC to get RLC segment of nof_bytes length.
   * Segmentation happens in this function. RLC PDU is stored in payload. */
  virtual int read_pdu(uint32_t lcid, uint8_t* payload, uint32_t nof_bytes) = 0;

  /* MAC calls RLC to push an RLC PDU. This function is called from an independent MAC thread.
   * PDU gets placed into the buffer and higher layer thread gets notified. */
  virtual void write_pdu(uint32_t lcid, uint8_t* payload, uint32_t nof_bytes)     = 0;
  virtual void write_pdu_bcch_bch(srslte::unique_byte_buffer_t payload)           = 0;
  virtual void write_pdu_bcch_dlsch(uint8_t* payload, uint32_t nof_bytes)         = 0;
  virtual void write_pdu_pcch(srslte::unique_byte_buffer_t payload)               = 0;
  virtual void write_pdu_mch(uint32_t lcid, uint8_t* payload, uint32_t nof_bytes) = 0;
};

} // namespace srsue

#endif // SRSLTE_UE_RLC_INTERFACES_H
