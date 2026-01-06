/*
 * Copyright (C) 2014 Jared Boone, ShareBrained Technology, Inc.
 * Copyright (C) 2016 Furrtek
 * Copyright (C) 2026 NotPike
 *
 * This file is part of PortaPack.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#include "proc_4fsk.hpp"
#include "portapack_shared_memory.hpp"
#include "sine_table_int8.hpp"
#include "event_m4.hpp"

#include <cstdint>

void FourFSKProcessor::execute(const buffer_c8_t& buffer) {
    int8_t re, im;

    // This is called at 2.28M/2048 = 1113Hz

    for (size_t i = 0; i < buffer.count; i++) {
        if (configured) {
            if (sample_count >= samples_per_bit) {
                if (bit_pos > length) {
                    // End of data
                    cur_bit = 0;
                    txprogress_message.done = true;
                    shared_memory.application_queue.push(txprogress_message);
                    configured = false;
                } else {

                    // Current asumption is MSB comes first
                    cur_bit = (shared_memory.bb_data.data[bit_pos >> 3] << (bit_pos & 7)) & 0x80;   // First bit
                    bit_pos++;
                    cur_bit << 1;                                                                   // Shift left by 1
                    cur_bit |= (shared_memory.bb_data.data[bit_pos >> 3] << (bit_pos & 7)) & 0x80;  // OR Equal next bit               
                    bit_pos++;

                    if (progress_count >= progress_notice) {
                        progress_count = 0;
                        txprogress_message.progress++;
                        txprogress_message.done = false;
                        shared_memory.application_queue.push(txprogress_message);
                    } else {
                        progress_count++;
                    }
                }
                sample_count = 0;
            } else {
                sample_count++;
            }


            if (cur_bit == 0b11) {
                phase += shift_one_one;
            } else if(cur_bit == 0b10) {
                phase += shift_one_zero;
            } else if(cur_bit == 0b1) {
                phase += shift_zero_one;
            } else {
                phase += shift_zero_zero;
            }

            sphase = phase + (64 << 24);

            re = (sine_table_i8[(sphase & 0xFF000000) >> 24]);
            im = (sine_table_i8[(phase & 0xFF000000) >> 24]);
        } else {
            re = 0;
            im = 0;
        }

        buffer.p[i] = {re, im};
    }
}

void FourFSKProcessor::on_message(const Message* const p) {
    const auto message = *reinterpret_cast<const FourFSKConfigureMessage*>(p);

    if (message.id == Message::ID::FourFSKConfigure) {
        samples_per_bit = message.samples_per_bit;
        length = message.stream_length + 32;  // Why ?!
        
        // 4FSK
        //            cf
        //            |
        //    |    |  |  |    |
        //===========================
        //    11   10    00   01
        // https://k3smt.org/digital-radio/

        shift_zero_one = message.shift * (0xFFFFFFFFULL / 2280000);        // High Frequency
        shift_zero_zero = (message.shift / 3) * (0xFFFFFFFFULL / 2280000);
        shift_one_zero = -shift_zero_zero;
        shift_one_one = -shift_zero_one;                                   // Low Frequency


        progress_notice = message.progress_notice;

        sample_count = samples_per_bit;
        progress_count = 0;
        bit_pos = 0;
        cur_bit = 0;

        txprogress_message.progress = 0;
        txprogress_message.done = false;
        configured = true;
    }
}

int main() {
    EventDispatcher event_dispatcher{std::make_unique<FourFSKProcessor>()};
    event_dispatcher.run();
    return 0;
}
