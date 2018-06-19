#--------------------------------------------------------
# The CMakeLists.txt for:                       pPrimeFactors
# Author(s):                              Erik Boesen
#--------------------------------------------------------

SET(SRC
  PrimeFactors.cpp
  PrimeFactors_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pPrimeFactors ${SRC})

TARGET_LINK_LIBRARIES(pPrimeFactors
   ${MOOS_LIBRARIES}
   mbutil
   m
   pthread)

