CPMAddPackage(
        NAME STL_READER
        GITHUB_REPOSITORY sreiter/stl_reader
        GIT_TAG master
        DOWNLOAD_ONLY ON
)

include_directories(${STL_READER_SOURCE_DIR})