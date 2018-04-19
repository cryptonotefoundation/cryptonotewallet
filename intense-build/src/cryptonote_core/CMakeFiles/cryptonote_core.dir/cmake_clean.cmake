file(REMOVE_RECURSE
  "libcryptonote_core.pdb"
  "libcryptonote_core.a"
)

# Per-language clean rules from dependency scanning.
foreach(lang CXX)
  include(CMakeFiles/cryptonote_core.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
