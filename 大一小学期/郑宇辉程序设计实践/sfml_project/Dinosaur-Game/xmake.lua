add_rules("mode.debug", "mode.release")
set_languages("c++17")
add_requires("sfml 3.0.0", {configs = {shared = false, all = true}})
add_requireconfs("*.sfml", {version = "3.0.0", configs = {all = true}})
add_requires("boost 1.88.0", {configs = {shared = false, all = true}})
add_requireconfs("*.boost", {version = "1.88.0", configs = {all = true}})


target("Dinosaur-Game")
    set_kind("binary")
    add_files("src/*.cpp")

    -- add_links("sfml-graphics", "sfml-window", "sfml-system", "sfml-audio", "sfml-network")
    -- add_linkdirs("SFML-3.0.0/lib")

    add_packages("sfml")
    add_packages("boost")
