/*
    PCCC: A portable library for context-cognizant completions.
    Copyright (C) 2017  bcwarner

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

Defines the properties any lexer/parser must have.
*/

#pragma once

#include "macros.h"
#include "types.h"

#include "lp/c.tab.h"

#ifdef DEBUG

void
pccc_lp_analyze(pccc_context *ctxt, pccc_buffer *buf);

void
pccc_lp_tokenize(pccc_context *ctxt, pccc_buffer *buf);

extern pccc_lp pccc_lp_test; // = { &pccc_lp_analyze, &pccc_lp_tokenize };

#endif

void 
pccc_lp_c_analyze(pccc_context *ctxt, pccc_buffer * buf);

void 
pccc_lp_c_tokenize(pccc_context *ctxt, pccc_buffer * buf);

extern pccc_lp pccc_lp_c;// = { &pccc_lp_c_analyze, &pccc_lp_c_tokenize };

void
pccc_lp_default_fn(pccc_context *ctxt, pccc_buffer *buf);

extern pccc_lp pccc_lp_default;

// Select a lexer/parser based upon file name.
pccc_lp * pccc_select_lp(char *fn);