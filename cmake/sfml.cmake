CPMAddPackage(
        NAME SFML
        GITHUB_REPOSITORY SFML/SFML
        GIT_TAG 3.0.2
        GIT_SHALLOW OFF
)

link_libraries(sfml-graphics sfml-window sfml-system)