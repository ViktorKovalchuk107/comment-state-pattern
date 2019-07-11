#include "states.h"

OutsideState OutsideState::uniqueState;

StringLiteralState StringLiteralState::uniqueState;

BlockCommentState BlockCommentState::uniqueState;

OneLineCommentState OneLineCommentState::uniqueState;

PotentialCommentState PotentialCommentState::uniqueState;

PotentialCommentEndState PotentialCommentEndState::uniqueState;

void OutsideState::processNextChar(StateContext &ctx, char c)
{
    ctx.getRes().push_back(c);
    if (c == '/')
    {
        ctx.setState(PotentialCommentState::getInstance());
    }
    else if (c == '\"')
    {
        ctx.setState(StringLiteralState::getInstance());
    }
}

void StringLiteralState::processNextChar(StateContext &ctx, char c)
{
    ctx.getRes().push_back(c);
    if (c == '\"' && !(ctx.getRes().size() >= 2 && ctx.getRes()[ctx.getRes().size() - 2] == '\\'))
    {
        ctx.setState(OutsideState::getInstance());
    }
}

void PotentialCommentState::processNextChar(StateContext &ctx, char c)
{
    if (c == '*')
    {
        ctx.getRes().pop_back();
        ctx.setState(BlockCommentState::getInstance());
    }
    else if (c == '/')
    {
        ctx.getRes().pop_back();
        ctx.setState(OneLineCommentState::getInstance());
    }
    else
    {
        ctx.getRes().push_back(c);
        ctx.setState(OutsideState::getInstance());
    }
}

void BlockCommentState::processNextChar(StateContext &ctx, char c)
{
    if (c == '*')
    {
        ctx.setState(PotentialCommentEndState::getInstance());
    }
}

void OneLineCommentState::processNextChar(StateContext &ctx, char c)
{
    if (c == '\n')
    {
        ctx.getRes().push_back(c);
        ctx.setState(OutsideState::getInstance());
    }
}

void PotentialCommentEndState::processNextChar(StateContext &ctx, char c)
{
    if (c == '/')
    {
        ctx.setState(OutsideState::getInstance());
    }
    else if (c != '*')
    {
        ctx.setState(BlockCommentState::getInstance());
    }
}
