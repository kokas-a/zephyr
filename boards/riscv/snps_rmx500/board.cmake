# SPDX-License-Identifier: Apache-2.0

set(SUPPORTED_EMU_PLATFORMS nsim)

 board_set_flasher_ifnset(arc-nsim)
board_set_debugger_ifnset(arc-nsim)

set(NSIM_PROPS "${BOARD}.props")
board_runner_args(arc-nsim "--props=${NSIM_PROPS}")

board_finalize_runner_args(arc-nsim)
include(${ZEPHYR_BASE}/boards/common/mdb-nsim.board.cmake)
include(${ZEPHYR_BASE}/boards/common/mdb-hw.board.cmake)
