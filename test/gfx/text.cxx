#include "gfx/text.hxx"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Text parsing", "[gfx][unit]") {
    Text text;
    text.set_region({ 5000, 500 });

    SECTION("Works with simple text") {
        text.set_text("Hello World");
        text.update();

        const auto& ops = text.ops();

        REQUIRE(ops.size() == 1);
        REQUIRE(ops[0].text == "Hello World");
    }

    SECTION("Ignores command char at end of string") {
        text.set_text("abc$");
        text.update();

        const auto& ops = text.ops();

        REQUIRE(ops.size() == 1);
        REQUIRE(ops[0].text == "abc");
    }

    SECTION("Outputs escaped dollar at end of string") {
        text.set_text("abc$$");
        text.update();

        const auto& ops = text.ops();

        REQUIRE(ops.size() == 2);
        REQUIRE(ops[0].text == "abc");
        REQUIRE(ops[1].text == "$");
    }

    SECTION("Ignores command char on unknown command specifier") {
        text.set_text("a$b");
        text.update();

        const auto& ops = text.ops();

        REQUIRE(ops.size() == 2);
        REQUIRE(ops[0].text == "a");
        REQUIRE(ops[1].text == "b");
    }

    SECTION("Removes newline command char") {
        text.set_text("a$nb");
        text.update();

        const auto& ops = text.ops();

        REQUIRE(ops.size() == 2);
        REQUIRE(ops[0].text == "a");
        REQUIRE(ops[1].text == "b");
    }

    SECTION("Removes color command string") {
        text.set_text("a$cFF0000b");
        text.update();

        const auto& ops = text.ops();

        REQUIRE(ops.size() == 2);
        REQUIRE(ops[0].text == "a");
        REQUIRE(ops[1].text == "b");
    }

    SECTION("Removes truncated color command at the end of the text") {
        const auto strings = {
            "a$cFF000",
            "a$cFF00",
            "a$cFF0",
            "a$cFF",
            "a$cF",
            "a$c",
        };

        for (auto& str : strings) {
            text.set_text(str);
            text.update();

            const auto& ops = text.ops();

            REQUIRE(ops.size() == 1);
            REQUIRE(ops[0].text == "a");
        }
    }

    SECTION("Removes trailing color command at the end of the text") {
        text.set_text("a$cAABBCC");
        text.update();

        const auto& ops = text.ops();

        REQUIRE(ops.size() == 1);
        REQUIRE(ops[0].text == "a");
    }

    SECTION("Removes color command and restoration substring") {
        text.set_text("a$cFF0000b$rc");
        text.update();

        const auto& ops = text.ops();

        REQUIRE(ops.size() == 3);
        REQUIRE(ops[0].text == "a");
        REQUIRE(ops[1].text == "b");
        REQUIRE(ops[2].text == "c");
    }

    SECTION("Restores previous color") {
        text.set_text("White$cFF0000Red$c00FF00Green$rRed$rWhite");
        text.update();

        const auto& ops = text.ops();

        REQUIRE(ops.size() == 5);
        REQUIRE(ops[0].text == "White");
        REQUIRE(ops[0].color == Color(0xFFFFFF));
        REQUIRE(ops[1].text == "Red");
        REQUIRE(ops[1].color == Color(0xFF0000));
        REQUIRE(ops[2].text == "Green");
        REQUIRE(ops[2].color == Color(0x00FF00));
        REQUIRE(ops[3].text == "Red");
        REQUIRE(ops[3].color == Color(0xFF0000));
        REQUIRE(ops[4].text == "White");
        REQUIRE(ops[4].color == Color(0xFFFFFF));
    }
}
