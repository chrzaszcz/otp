/*
 * %CopyrightBegin%
 *
 * Copyright Ericsson AB 2002-2013. All Rights Reserved.
 *
 * The contents of this file are subject to the Erlang Public License,
 * Version 1.1, (the "License"); you may not use this file except in
 * compliance with the License. You should have received a copy of the
 * Erlang Public License along with this software. If not, it can be
 * retrieved online at http://www.erlang.org/.
 *
 * Software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See
 * the License for the specific language governing rights and limitations
 * under the License.
 *
 * %CopyrightEnd%
 */

#ifndef ERL_MSEG_H_
#define ERL_MSEG_H_

#include "sys.h"
#include "erl_alloc_types.h"

#ifndef HAVE_MMAP
#  define HAVE_MMAP 0
#endif
#ifndef HAVE_MREMAP
#  define HAVE_MREMAP 0
#endif

#if HAVE_MMAP
#  define HAVE_ERTS_MSEG 1
#  define HAVE_SUPER_ALIGNED_MB_CARRIERS 1
#else
#  define HAVE_ERTS_MSEG 0
#  define HAVE_SUPER_ALIGNED_MB_CARRIERS 0
#endif

#if HAVE_SUPER_ALIGNED_MB_CARRIERS
#  define MSEG_ALIGN_BITS (18)
   /* Affects hard limits for sbct and lmbcs documented in erts_alloc.xml */
#else
/* If we don't use super aligned multiblock carriers
 * we will mmap with page size alignment (and thus use corresponding
 * align bits).
 *
 * Current implementation needs this to be a constant and
 * only uses this for user dev testing so setting page size
 * to 4096 (12 bits) is fine.
 */
#  define MSEG_ALIGN_BITS       (12)
#endif

#if HAVE_ERTS_MSEG

#define MSEG_ALIGNED_SIZE     (1 << MSEG_ALIGN_BITS)

#define ERTS_MSEG_FLG_NONE    ((Uint)(0))
#define ERTS_MSEG_FLG_2POW    ((Uint)(1 << 0))


#define ERTS_MSEG_VSN_STR "0.9"

typedef struct {
    Uint amcbf;
    Uint rmcbf;
    Uint mcs;
    Uint nos;
} ErtsMsegInit_t;

#define ERTS_MSEG_INIT_DEFAULT_INITIALIZER				\
{									\
    4*1024*1024,	/* amcbf: Absolute max cache bad fit	*/	\
    20,			/* rmcbf: Relative max cache bad fit	*/	\
    10,			/* mcs:   Max cache size		*/	\
    1000		/* cci:   Cache check interval		*/	\
}

typedef struct {
    int  cache;
    int  preserv;
    UWord abs_shrink_th;
    UWord rel_shrink_th;
    int sched_spec;
#if HALFWORD_HEAP
    int low_mem;
#endif
} ErtsMsegOpt_t;

extern const ErtsMsegOpt_t erts_mseg_default_opt;

void *erts_mseg_alloc(ErtsAlcType_t, Uint *, Uint);
void *erts_mseg_alloc_opt(ErtsAlcType_t, Uint *, Uint, const ErtsMsegOpt_t *);
void  erts_mseg_dealloc(ErtsAlcType_t, void *, Uint, Uint);
void  erts_mseg_dealloc_opt(ErtsAlcType_t, void *, Uint, Uint, const ErtsMsegOpt_t *);
void *erts_mseg_realloc(ErtsAlcType_t, void *, Uint, Uint *, Uint);
void *erts_mseg_realloc_opt(ErtsAlcType_t, void *, Uint, Uint *, Uint, const ErtsMsegOpt_t *);
void  erts_mseg_clear_cache(void);
void  erts_mseg_cache_check(void);
Uint  erts_mseg_no( const ErtsMsegOpt_t *);
Uint  erts_mseg_unit_size(void);
void  erts_mseg_init(ErtsMsegInit_t *init);
void  erts_mseg_late_init(void); /* Have to be called after all allocators,
				   threads and timers have been initialized. */
Eterm erts_mseg_info_options(int, int *, void*, Uint **, Uint *);
Eterm erts_mseg_info(int, int *, void*, int, Uint **, Uint *);

#endif /* #if HAVE_ERTS_MSEG */

UWord erts_mseg_test(UWord, UWord, UWord, UWord);

#endif /* #ifndef ERL_MSEG_H_ */
