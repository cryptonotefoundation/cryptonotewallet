file(REMOVE_RECURSE
  "libdaemonizer.pdb"
  "libdaemonizer.a"
)

# Per-language clean rules from dependency scanning.
foreach(lang CXX)
  include(CMakeFiles/daemonizer.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
