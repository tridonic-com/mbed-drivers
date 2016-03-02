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
#include "mbed-drivers/I2C.h"
#include "minar/minar.h"

#if DEVICE_I2C

namespace mbed {

I2C *I2C::_owner = NULL;

I2C::I2C(PinName sda, PinName scl) :
#if DEVICE_I2C_ASYNCH
                                     _irq(this), _usage(DMA_USAGE_NEVER),
#endif
                                      _i2c(), _hz(100000) {
    // The init function also set the frequency to 100000
    i2c_init(&_i2c, sda, scl);

    // Used to avoid unnecessary frequency updates
    _owner = this;
}

void I2C::frequency(int hz) {
    _hz = hz;

    // We want to update the frequency even if we are already the bus owners
    i2c_frequency(&_i2c, _hz);

    // Updating the frequency of the bus we become the owners of it
    _owner = this;
}

void I2C::aquire() {
    if (_owner != this) {
        i2c_frequency(&_i2c, _hz);
        _owner = this;
    }
}

// write - Master Transmitter Mode
int I2C::write(int address, const char* data, int length, bool repeated) {
    aquire();

    int stop = (repeated) ? 0 : 1;
    int written = i2c_write(&_i2c, address, data, length, stop);

    return written;
}

int I2C::write(int data) {
    return i2c_byte_write(&_i2c, data);
}

// read - Master Receiver Mode
int I2C::read(int address, char* data, int length, bool repeated) {
    aquire();

    int stop = (repeated) ? 0 : 1;
    int read = i2c_read(&_i2c, address, data, length, stop);

    return read;
}

int I2C::read(int ack) {
    if (ack) {
        return i2c_byte_read(&_i2c, 0);
    } else {
        return i2c_byte_read(&_i2c, 1);
    }
}

void I2C::start(void) {
    i2c_start(&_i2c);
}

void I2C::stop(void) {
    i2c_stop(&_i2c);
}

void I2C::address(int address) {
    int addr = (address & 0xFF) | 1;
    i2c_set_own_address(&_i2c, addr);
    i2c_enable_i2c_it(&_i2c);
}

#if DEVICE_I2CSLAVE
/* Slave related functions */
void I2C::address(int address) {
    int addr = (address & 0xFF) | 1;
    i2c_slave_address(&_i2c, 0, addr, 0);
    i2c_slave_mode(&_i2c, 1);
}

int I2C::receive(void) {
    return i2c_slave_receive(&_i2c);
}

int I2C::slave_read(char *data, int length) {
	return i2c_slave_read(&_i2c, data, length);
}

int I2C::slave_write(const char *data, int length) {
	return i2c_slave_write(&_i2c, data, length);
}

void I2C::slave_mode(int enable_slave) {
	i2c_slave_mode(&_i2c, enable_slave);
}

void I2C::reset(void) {
    i2c_reset(&_i2c);
}
#endif

#ifdef DEVICE_I2C_DMA
void I2C::attach(event_cb_t fptr1, event_cb_t fptr2, event_cb_t fptr3, event_cb_t fptr4, event_cb_t fptr5, event_cb_t fptr6) {
	i2c_register_event_cb(fptr1, fptr2, fptr3, fptr4, fptr5, fptr6);
}

int I2C::master_transmit_DMA(int address, const unsigned char* data, int length, bool stop) {
	aquire();

	int write = i2c_master_transmit_DMA(&_i2c, address, data, length, stop);

	return write;
}

int I2C::master_receive_DMA(int address, unsigned char* data, int length, bool stop) {
	aquire();

	int read = i2c_master_receive_DMA(&_i2c, address, data, length, stop);

	return read;
}

int I2C::slave_transmit_DMA(const unsigned char *data, int length) {
	aquire();

	return i2c_slave_transmit_DMA(&_i2c, data, length);
}

int I2C::slave_receive_DMA(unsigned char *data, int length) {
	aquire();

	return i2c_slave_receive_DMA(&_i2c, data, length);
}
#endif

#if DEVICE_I2C_ASYNCH

int I2C::transfer(int address, char *tx_buffer, int tx_length, char *rx_buffer, int rx_length, const event_callback_t& callback, int event, bool repeated) {
    return transfer(address, Buffer(tx_buffer, tx_length), Buffer(rx_buffer, rx_length), callback, event, repeated);
}

int I2C::transfer(int address, const Buffer& tx_buffer, const Buffer& rx_buffer, const event_callback_t& callback, int event, bool repeated) {
    if (i2c_active(&_i2c)) {
        return -1; // transaction ongoing
    }
    aquire();

    _current_transaction.tx_buffer = tx_buffer;
    _current_transaction.rx_buffer = rx_buffer;
    _current_transaction.callback = callback;
    int stop = (repeated) ? 0 : 1;
    _irq.callback(&I2C::irq_handler_asynch);
    i2c_transfer_asynch(&_i2c, tx_buffer.buf, tx_buffer.length, rx_buffer.buf, rx_buffer.length, address, stop, _irq.entry(), event, _usage);
    return 0;
}

void I2C::abort_transfer(void)
{
    i2c_abort_asynch(&_i2c);
}

void I2C::irq_handler_asynch(void)
{
    int event = i2c_irq_handler_asynch(&_i2c);
    if (_current_transaction.callback && event) {
        minar::Scheduler::postCallback(_current_transaction.callback.bind(_current_transaction.tx_buffer, _current_transaction.rx_buffer, event));
    }

}


#endif

} // namespace mbed

#endif
