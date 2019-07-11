#pragma once

#include "comments_remover.h"

class OutsideState : public State
{
private:
    OutsideState() = default;
    static OutsideState uniqueState;

public:
    static OutsideState *getInstance() { return &uniqueState; }

    void processNextChar(StateContext &ctx, char c) override;
};

class StringLiteralState : public State
{
private:
    StringLiteralState() = default;
    static StringLiteralState uniqueState;

public:
    static StringLiteralState *getInstance() { return &uniqueState; }

    void processNextChar(StateContext &ctx, char c) override;
};

class BlockCommentState : public State
{
private:
    BlockCommentState() = default;
    static BlockCommentState uniqueState;

public:
    static BlockCommentState *getInstance() { return &uniqueState; }

    void processNextChar(StateContext &ctx, char c) override;
};

class OneLineCommentState : public State
{
private:
    OneLineCommentState() = default;
    static OneLineCommentState uniqueState;

public:
    static OneLineCommentState *getInstance() { return &uniqueState; }

    void processNextChar(StateContext &ctx, char c) override;
};

class PotentialCommentState : public State
{
private:
    PotentialCommentState() = default;
    static PotentialCommentState uniqueState;

public:
    static PotentialCommentState *getInstance() { return &uniqueState; }

    void processNextChar(StateContext &ctx, char c) override;
};

class PotentialCommentEndState : public State
{
private:
    PotentialCommentEndState() = default;
    static PotentialCommentEndState uniqueState;

public:
    static PotentialCommentEndState *getInstance() { return &uniqueState; }

    void processNextChar(StateContext &ctx, char c) override;
};

// class PotentialRawCommentFirstState : public State {
// private:
//     PotentialRawCommentFirstState() = default;
//     static PotentialRawCommentFirstState uniqueState;
// public:
//     static PotentialRawCommentFirstState*&getInstance() { return uniqueState; }
//     void processNextChar(StateContext &ctx, char c) override;
//     void processInputEnd(std::string &res) override;
// };

// class PotentialRawCommentSecondState : public State {
// private:
//     PotentialRawCommentSecondState() = default;
//     static PotentialRawCommentSecondState uniqueState;
// public:
//     static PotentialRawCommentSecondState*&getInstance() { return uniqueState; }
//     void processNextChar(StateContext &ctx, char c) override;
//     void processInputEnd(std::string &res) override;
// };

// class PotentialRawCommentEndState : public State {
// private:
//     PotentialRawCommentEndState() = default;
//     static PotentialRawCommentEndState uniqueState;
// public:
//     static PotentialRawCommentEndState*&getInstance() { return uniqueState; }
//     void processNextChar(StateContext &ctx, char c) override;
//     void processInputEnd(std::string &res) override;
// };
