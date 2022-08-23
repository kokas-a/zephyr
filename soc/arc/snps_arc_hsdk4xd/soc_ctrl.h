/*
 * Copyright (c) 2023 Synopsys, Inc. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 */


#ifndef _ARC_HSDK4XD_SOC_CTRL_H_
#define _ARC_HSDK4XD_SOC_CTRL_H_

#ifdef _ASMLANGUAGE
.macro soc_early_asm_init_percpu
	/* case of HS4XD cpu */
	lr	 r0, [_ARC_V2_LPB_BUILD]
	breq r0, 0, skip_lpb_setup
	mov  r0, 1	/* disable LPB for HS4XD */
	sr   r0, [_ARC_V2_LPB_CTRL]
skip_lpb_setup :
.endm
#endif /* _ASMLANGUAGE */

#endif /* _ARC_HSDK4XD_SOC_CTRL_H_ */
