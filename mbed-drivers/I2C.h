/*
 * Copyright (c) 2006-2016, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef MBED_I2C_H
#define MBED_I2C_H

#include "platform.h"

#if DEVICE_I2C

#include "i2c_api.h"

#if DEVICE_I2C_ASYNCH
#include "CThunk.h"
#include "dma_api.h"
#include "core-util/FunctionPointer.h"
#include "Transaction.h"
#endif

namespace mbed {

/** An I2C Master, used for communicating with I2C slave devices
 *
 * Example:
 * @code
 * // Read from I2C slave at address 0x62
 *
 * #include "mbed.h"
 *
 * I2C i2c(p28, p27);
 *
 * int main() {
 *     int address = 0x62;
 *     char data[2];
 *     i2c.read(address, data, 2);
 * }
 * @endcode
 */
class I2C {

public:
    enum RxStatus {
        NoData,
        MasterGeneralCall,
        MasterWrite,
        MasterRead
    };

    enum RxSlaveStatus {
        ReadAddressed  = 1,
        WriteGeneral   = 2,
        WriteAddressed = 3
    };

    enum Acknowledge {
        NoACK = 0,
        ACK   = 1
    };

    /** Create an I2C Master interface, connected to the specified pins
     *
     *  @param sda I2C data line pin
     *  @param scl I2C clock line pin
     */
    I2C(PinName sda, PinName scl);

    /** Set the frequency of the I2C interface
     *
     *  @param hz The bus frequency in hertz
     */
    void frequency(int hz);

    /** Read from an I2C slave
     *
     * Performs a complete read transaction. The bottom bit of
     * the address is forced to 1 to indicate a read.
     *
     *  @param address 8-bit I2C slave address [ addr | 1 ]
     *  @param data Pointer to the byte-array to read data in to
     *  @param length Number of bytes to read
     *  @param repeated Repeated start, true - don't send stop at end
     *
     *  @returns
     *       0 on success (ack),
     *   non-0 on failure (nack)
     */
    int read(int address, char *data, int length, bool repeated = false);

    /** Read a single byte from the I2C bus
     *
     *  @param ack indicates if the byte is to be acknowledged (1 = acknowledge)
     *
     *  @returns
     *    the byte read
     */
    int read(int ack);

    /** Write to an I2C slave
     *
     * Performs a complete write transaction. The bottom bit of
     * the address is forced to 0 to indicate a write.
     *
     *  @param address 8-bit I2C slave address [ addr | 0 ]
     *  @param data Pointer to the byte-array data to send
     *  @param length Number of bytes to send
     *  @param repeated Repeated start, true - do not send stop at end
     *
     *  @returns
     *       0 on success (ack),
     *   non-0 on failure (nack)
     */
    int write(int address, const char *data, int length, bool repeated = false);

    /** Write single byte out on the I2C bus
     *
     *  @param data data to write out on bus
     *
     *  @returns
     *    '1' if an ACK was received,
     *    '0' otherwise
     */
    int write(int data);

    /** Creates a start condition on the I2C bus
     */

    void start(void);

    /** Creates a stop condition on the I2C bus
     */
    void stop(void);

    /* Slave related functions */

    /** Set the device own address
     */
    void address(int address);

    /** Check if someone hit its own address and
     * check direction of transfer (transmission
     * or reception)
     */
#if DEVICE_I2CSLAVE
    int receive(void);

    /** Slave read in blocking mode
     */
    int slave_read(char *data, int length);

    /** Slave write in blocking mode
     */
    int slave_write(const char *data, int length);

    /** Enable address acknowledge
     */
    void slave_mode(int enable_slave);

    /** Reset the I2C slave back into the known ready receiving state.
     */
    void reset(void);
#endif

#ifdef DEVICE_I2C_DMA
    /* Register I2C transfer callback */
    void attach(event_cb_t fptr1, event_cb_t fptr2, event_cb_t fptr3, event_cb_t fptr4, event_cb_t fptr5, event_cb_t fptr6);
    /** I2C using DMA */
    int master_transmit_DMA(int address, const unsigned char* data, int length, bool repeated);
    int master_receive_DMA(int address, unsigned char* data, int length, bool repeated);
    int slave_transmit_DMA(const unsigned char *data, int length);
    int slave_receive_DMA(unsigned  char *data, int length);
#endif

#if DEVICE_I2C_ASYNCH
    /** I2C transfer callback
     *  @param Buffer the tx buffer
     *  @param Buffer the rx buffer
     *  @param int the event that triggered the calback
     */
    typedef mbed::util::FunctionPointer3<void, Buffer, Buffer, int> event_callback_t;

    /** Start non-blocking I2C transfer.
     *
     * @param address   8/10 bit I2c slave address
     * @param tx_buffer The TX buffer with data to be transfered
     * @param tx_length The length of TX buffer
     * @param rx_buffer The RX buffer which is used for received data
     * @param rx_length The length of RX buffer
     * @param event     The logical OR of events to modify
     * @param callback  The event callback function
     * @param repeated Repeated start, true - do not send stop at end
     * @return Zero if the transfer has started, or -1 if I2C peripheral is busy
     */
    int transfer(int address, char *tx_buffer, int tx_length, char *rx_buffer, int rx_length, const event_callback_t& callback, int event = I2C_EVENT_TRANSFER_COMPLETE, bool repeated = false);

     /** Start non-blocking I2C transfer.
     *
     * @param address   8/10 bit I2c slave address
     * @param tx_buffer The TX buffer with data to be transfered
     * @param rx_buffer The RX buffer which is used for received data
     * @param event     The logical OR of events to modify
     * @param callback  The event callback function
     * @param repeated Repeated start, true - do not send stop at end
     * @return Zero if the transfer has started, or -1 if I2C peripheral is busy
     */
    int transfer(int address, const Buffer& tx_buffer, const Buffer& rx_buffer, const event_callback_t& callback, int event = I2C_EVENT_TRANSFER_COMPLETE, bool repeated = false);

    /** Abort the on-going I2C transfer
     */
    void abort_transfer();
protected:
    typedef TwoWayTransaction<event_callback_t> transaction_data_t;
    typedef Transaction<I2C, transaction_data_t> transaction_t;

    void irq_handler_asynch(void);
    transaction_data_t _current_transaction;
    CThunk<I2C> _irq;
    DMAUsage _usage;
#endif

protected:
    void aquire();

    i2c_t _i2c;
    static I2C  *_owner;
    int         _hz;
};

} // namespace mbed

#endif

#endif
