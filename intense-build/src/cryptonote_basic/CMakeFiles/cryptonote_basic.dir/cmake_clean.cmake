file(REMOVE_RECURSE
  "libcryptonote_basic.pdb"
  "libcryptonote_basic.a"
)

# Per-language clean rules from dependency scanning.
foreach(lang CXX)
  include(CMakeFiles/cryptonote_basic.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
