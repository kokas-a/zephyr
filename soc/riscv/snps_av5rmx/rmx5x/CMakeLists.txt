# SPDX-License-Identifier: Apache-2.0

zephyr_sources(soc.c)

if(COMPILER STREQUAL gcc)

elseif(COMPILER STREQUAL clang)
  zephyr_sources(div.S)
  
  zephyr_compile_options_ifdef(CONFIG_SOC_SERIES_RMX5X 
			       	--target=riscv32 -march=rv32gc -mno-relax)
else ()
# for CCAC:
#  zephyr_compile_options_ifdef(CONFIG_SOC_SERIES_RMX5X 
#			       	-av5rmx -Zicsr -Zifencei -Zihintpause -Zba -Zbb
#					-Zbs -Zca -Zcb -Zcmp -Zcmt -Za -Zm -Zicbom)

endif()

set(SOC_LINKER_SCRIPT ${ZEPHYR_BASE}/include/zephyr/arch/riscv/common/linker.ld CACHE INTERNAL "")
