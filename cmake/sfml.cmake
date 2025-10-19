CPMAddPackage(
        NAME SFML
        GITHUB_REPOSITORY SFML/SFML
        GIT_TAG 3.0.2
        GIT_SHALLOW OFF
)


include_directories(${SFML_DIR}/include)
link_directories(${SFML_DIR}/lib)

link_libraries(sfml-graphics sfml-window sfml-system)
