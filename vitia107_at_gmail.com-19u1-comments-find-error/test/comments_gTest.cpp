// Copyright 2018, GlobalLogic Inc.
// All rights reserved.
//

#pragma warning(disable : 4996)

#include "gtest/gtest.h"

#include "comments.h"
#include "states.h"

TEST(remove_comments, simple_text)
{
    EXPECT_EQ(".", remove_comments("."));
    EXPECT_EQ("abc", remove_comments("abc"));
    EXPECT_EQ("Hello Global World !!!", remove_comments("Hello Global World !!!"));
}

TEST(remove_comments, strings)
{
    EXPECT_EQ("\"", remove_comments("\""));
    EXPECT_EQ("\"\"", remove_comments("\"\""));
    EXPECT_EQ("\"//\"", remove_comments("\"//\""));
    EXPECT_EQ("\"/**/\"", remove_comments("\"/**/\""));
    EXPECT_EQ("\"//*\\\"*/\"", remove_comments("\"//*\\\"*/\""));
}

TEST(remove_comments, special_symbols)
{
    EXPECT_EQ("/", remove_comments("/"));
    EXPECT_EQ("\n", remove_comments("\n"));
    EXPECT_EQ("/\n/\n/", remove_comments("/\n/\n/"));
    EXPECT_EQ(R"(¯\_(ツ)_/¯)", remove_comments(R"(¯\_(ツ)_/¯)"));
}

TEST(remove_comments, one_line_comments)
{
    EXPECT_EQ("", remove_comments("//"));
    EXPECT_EQ("", remove_comments("//asdf"));
    EXPECT_EQ("\njkl;", remove_comments("//asdf\njkl;"));
    EXPECT_EQ("\njkl;\nzxcv", remove_comments("//asdf\njkl;//uiop\nzxcv"));
}

TEST(remove_comments, multi_line_comments)
{
    EXPECT_EQ("", remove_comments("/**/"));
    EXPECT_EQ("", remove_comments("/*asdf*/"));
    EXPECT_EQ("jkl;", remove_comments("jk/*asdf*/l;"));
    EXPECT_EQ("jkl;\nqwerty", remove_comments("jk/*asdf*/l;\nqwer/*******/ty"));
}

TEST(remove_comments, complex_text)
{
    EXPECT_EQ("remove/jkl;\n\nqwer\"//asd/*_*/\"", remove_comments("remove/jk/*asdf*/l;\n\nqw/*12/34*/er\"//asd/*_*/\"//comments"));
}

TEST(remove_comments, comments_in_string)
{
    const char *textWithComments = R"a70370745ec711e9(
        // this is a demo
        int demo() {
            printf("\nhello, w//orld!\n"); // prints hello, world!
            return 0;
        }
    )a70370745ec711e9";
    const char *textWithoutComments = R"a70370745ec711e9(
        
        int demo() {
            printf("\nhello, w//orld!\n"); 
            return 0;
        }
    )a70370745ec711e9";
    EXPECT_EQ(textWithoutComments, remove_comments(textWithComments));
}

TEST(remove_comments, comments_escaped_string)
{
    const char *textWithComments = R"a70370745ec711e9(
        // this is a demo
        int demo() {
            printf("\nhello, \"w//orld\"!\n"); // prints hello, "world"!
            return 0;
        }
    )a70370745ec711e9";
    const char *textWithoutComments = R"a70370745ec711e9(
        
        int demo() {
            printf("\nhello, \"w//orld\"!\n"); 
            return 0;
        }
    )a70370745ec711e9";
    EXPECT_EQ(textWithoutComments, remove_comments(textWithComments));
}

TEST(remove_comments, comments_with_raw_string)
{
    const char * textWithComments = R"a70370745ec711e9(
        // this is a demo
        int demo() {
            char * test = R"oops(
                some inner raw string,
                parenthesis (like this)
                with // single line and
                /*
                    block
                */
                pseudo comments!
                and a double quote "
            )oops";
            printf("\nhello, \"w//orld\"!\n"); // prints hello, "world"!
            return 0;
        }
    )a70370745ec711e9";
    const char * textWithoutComments = "\n        \n        int demo() {\n            char * test = R\"oops(\n                some inner raw string,\n                parenthesis (like this)\n                with // single line and\n                /*\n                    block\n                */\n                pseudo comments!\n                and a double quote \"\n            )oops\";\n            printf(\"\\nhello, \\\"w//orld\\\"!\\n\"); \n            return 0;\n        }\n    ";
    EXPECT_EQ(textWithoutComments, remove_comments(textWithComments));
}

class CommentsRemoverMock : public CommentsRemover
{
public:
    using CommentsRemover::source;
    using StateContext::result;
    using StateContext::state;

    CommentsRemoverMock(std::string source) : CommentsRemover(source) {}
};

TEST(remover_class, generic_test)
{
    std::string src = R"(
        // single line comment with * and " and \n and so on
        int main() {
            /*
                multi line comment 
                with * and " and \n and so on
            */

           char *str = "string // with /* different */ \" types // of /* comments */";
           return 0;
        }
    )";

    std::string res = R"(
        
        int main() {
            

           char *str = "string // with /* different */ \" types // of /* comments */";
           return 0;
        }
    )";

    CommentsRemover remover(src);

    EXPECT_EQ(remover.process(), res);
}

TEST(state_transition, single_line_comment)
{
    CommentsRemoverMock remover("//t\nint main() { return 0; }");

    int i = 0;
    remover.state->processNextChar(remover, remover.source[i++]);
    remover.state->processNextChar(remover, remover.source[i++]);

    EXPECT_EQ(typeid(OneLineCommentState), typeid(*remover.state));

    remover.state->processNextChar(remover, remover.source[i++]);
    remover.state->processNextChar(remover, remover.source[i++]);
    EXPECT_EQ(typeid(OutsideState), typeid(*remover.state));
}

TEST(state_transition, block_comment)
{
    CommentsRemoverMock remover("/*t\n*/");

    int i = 0;
    remover.state->processNextChar(remover, remover.source[i++]);
    remover.state->processNextChar(remover, remover.source[i++]);

    EXPECT_EQ(typeid(BlockCommentState), typeid(*remover.state));

    remover.state->processNextChar(remover, remover.source[i++]);
    EXPECT_EQ(typeid(BlockCommentState), typeid(*remover.state));

    remover.state->processNextChar(remover, remover.source[i++]);
    remover.state->processNextChar(remover, remover.source[i++]);
    remover.state->processNextChar(remover, remover.source[i++]);
    EXPECT_EQ(typeid(OutsideState), typeid(*remover.state));
}

TEST(state_transition, string_literal)
{
    CommentsRemoverMock remover("\"//t\\\"\";");

    int i = 0;
    remover.state->processNextChar(remover, remover.source[i++]);
    EXPECT_EQ(typeid(StringLiteralState), typeid(*remover.state));

    remover.state->processNextChar(remover, remover.source[i++]);
    remover.state->processNextChar(remover, remover.source[i++]);
    EXPECT_EQ(typeid(StringLiteralState), typeid(*remover.state));

    remover.state->processNextChar(remover, remover.source[i++]);
    remover.state->processNextChar(remover, remover.source[i++]);
    remover.state->processNextChar(remover, remover.source[i++]);
    EXPECT_EQ(typeid(StringLiteralState), typeid(*remover.state));

    remover.state->processNextChar(remover, remover.source[i++]);
    EXPECT_EQ(typeid(OutsideState), typeid(*remover.state));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
