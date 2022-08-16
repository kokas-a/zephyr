# SPDX-License-Identifier: Apache-2.0

if(COMPILER STREQUAL gcc)
	zephyr_compile_options(-mcpu=${GCC_M_CPU})


	if(NOT CONFIG_64BIT)
  		zephyr_compile_options(-mno-sdata)
	endif()
else()
	
	zephyr_compile_options( -arcv2hs -core2 -Xatomic
			       -Xunaligned -Xcode_density -Xswap -Xbitscan
			       -Xmpy_option=qmpyh -Xshift_assist -Xbarrel_shifter
			       -Xtimer0 -Xtimer1)

	zephyr_ld_options( -Hlib=hs38_full)
	
endif()

