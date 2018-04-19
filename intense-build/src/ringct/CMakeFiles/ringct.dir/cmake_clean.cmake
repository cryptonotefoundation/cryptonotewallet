file(REMOVE_RECURSE
  "libringct.pdb"
  "libringct.a"
)

# Per-language clean rules from dependency scanning.
foreach(lang C CXX)
  include(CMakeFiles/ringct.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
