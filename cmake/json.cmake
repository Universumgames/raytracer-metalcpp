
CPMAddPackage(
        NAME JSON
        GITHUB_REPOSITORY nlohmann/json
        GIT_TAG v3.12.0
        GIT_SHALLOW OFF
)

link_libraries(nlohmann_json::nlohmann_json)