file(REMOVE_RECURSE
  "libp2p.pdb"
  "libp2p.a"
)

# Per-language clean rules from dependency scanning.
foreach(lang CXX)
  include(CMakeFiles/p2p.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
