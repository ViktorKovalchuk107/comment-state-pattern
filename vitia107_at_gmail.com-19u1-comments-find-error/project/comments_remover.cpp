#include "comments_remover.h"
#include "states.h"

CommentsRemover::CommentsRemover(std::string source)
    : source(source), StateContext(OutsideState::getInstance()) {}

std::string CommentsRemover::process()
{
    unsigned pos = 0;
    while (pos < source.length())
    {
        state->processNextChar(*this, source[pos++]);
    }
    state->processInputEnd(*this);
    return result;
}
