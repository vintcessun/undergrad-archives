add_rules("mode.debug", "mode.release")
set_languages("c++17")
add_requires("sfml 2.6.1")


target("StickBadminton")
    set_kind("binary")
    add_files("src/*.cpp")
    add_packages("sfml")

