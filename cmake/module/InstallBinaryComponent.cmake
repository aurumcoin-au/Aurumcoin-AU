# Copyright (c) 2025-present The Bitcoin Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or https://opensource.org/license/mit/.

include_guard(GLOBAL)
include(GNUInstallDirs)

function(install_binary_component component)
  cmake_parse_arguments(PARSE_ARGV 1
    IC                          # prefix
    "HAS_MANPAGE;INTERNAL"      # options
    ""                          # one_value_keywords
    ""                          # multi_value_keywords
  )
  set(target_name ${component})
  # Aurumcoin: executables are named aurumcoin* (target names stay bitcoin*).
  string(REPLACE "bitcoin" "aurumcoin" output_name ${target_name})
  set_target_properties(${target_name} PROPERTIES OUTPUT_NAME ${output_name})
  if(IC_INTERNAL)
    set(runtime_dest ${CMAKE_INSTALL_LIBEXECDIR})
  else()
    set(runtime_dest ${CMAKE_INSTALL_BINDIR})
  endif()
  install(TARGETS ${target_name}
    RUNTIME DESTINATION ${runtime_dest}
    COMPONENT ${component}
  )
  if(INSTALL_MAN AND IC_HAS_MANPAGE)
    install(FILES ${PROJECT_SOURCE_DIR}/doc/man/${target_name}.1
      DESTINATION ${CMAKE_INSTALL_MANDIR}/man1
      RENAME ${output_name}.1
      COMPONENT ${component}
    )
  endif()
endfunction()
