#pragma once

#include <string>

class StateContext;

class State
{
public:
    virtual void processNextChar(StateContext &ctx, char c) = 0;
    virtual void processInputEnd(StateContext &ctx) {}
};

class StateContext
{
protected:
    State *state;
    std::string result;
    StateContext(State *state) : state(state) {}

public:
    void setState(State *newState) { state = newState; }
    std::string& getRes() { return result; }
};

class CommentsRemover : public StateContext
{
protected:
    std::string source;

public:
    CommentsRemover(std::string source);
    std::string process();
};
