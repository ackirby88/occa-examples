add_executable(.CheckVarSelfAssign checkself.cxx)

install(TARGETS .CheckVarSelfAssign
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib/
        ARCHIVE DESTINATION lib/
)

