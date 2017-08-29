/*
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