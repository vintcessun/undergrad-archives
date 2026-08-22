add_rules("mode.debug", "mode.release")
set_languages("c++17")
add_requires("sfml 3.0.0")


target("2048")
    set_kind("binary")
    add_files("src/*.cpp")
    add_packages("sfml")

