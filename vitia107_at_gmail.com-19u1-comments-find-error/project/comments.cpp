
#include "comments.h"
#include "comments_remover.h"

std::string remove_comments(const std::string &str)
{
    CommentsRemover test(str);
    return test.process();
}

