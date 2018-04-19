file(REMOVE_RECURSE
  "librpc.pdb"
  "librpc.a"
)

# Per-language clean rules from dependency scanning.
foreach(lang CXX)
  include(CMakeFiles/rpc.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
