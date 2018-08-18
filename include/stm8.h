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
 * File:   stm8.h
 * Author: Nikolaos Apostolakos <nikoapos@gmail.com>
 *
 * Created on August 17, 2018, 4:11 PM
 */

#ifndef STM8_H
#define STM8_H

#include <stdint.h>


#define TEST
///////////////////////////////////////////////////////////////////////////////
// Check that we have a supported model
///////////////////////////////////////////////////////////////////////////////
#if !defined(STM8S103)
#error "Please select a supported chip model in the Makefile"
#endif



///////////////////////////////////////////////////////////////////////////////
// STM8S useful assembly shortcuts
///////////////////////////////////////////////////////////////////////////////
#define nop() {__asm__("nop\n");}  /* No Operation */


#endif /* STM8_H */

