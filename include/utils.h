/*
 * Copyright (C) 2018 Nikolaos Apostolakos <nikoapos@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* 
 * File:   utils.h
 * Author: Nikolaos Apostolakos <nikoapos@gmail.com>
 *
 * Created on August 17, 2018, 8:53 PM
 */

#ifndef STM8_UTILS_H
#define STM8_UTILS_H


///////////////////////////////////////////////////////////////////////////////
// Aliases for addressing the registers in the memory
///////////////////////////////////////////////////////////////////////////////
#define REGISTER    *(volatile unsigned char *)
#define REGISTER_RO *(volatile const unsigned char *)

///////////////////////////////////////////////////////////////////////////////
// Macros for setting and un-setting bits of a register
///////////////////////////////////////////////////////////////////////////////
#define _registerSet(reg, bits) reg |= bits
#define registerSet(reg, bits) _registerSet(reg, bits)
#define _registerUnset(reg, bits) reg &= !bits
#define registerUnset(reg, bits) _registerUnset(reg, bits)
#define _registerInvert(reg, bits) reg ^= bits
#define registerInvert(reg, bits) _registerInvert(reg, bits)

#endif /* STM8_UTILS_H */

