#pragma once
#ifndef ISTATELINKAGEBUILDER_H
#define ISTATELINKAGEBUILDER_H

#include <functional>

#include "IState.h"

using TransitionCondition = std::function<bool()>;

class IStateLinkageBuilder
{
public:
	virtual ~IStateLinkageBuilder() = default;
    virtual void AddTransition(std::shared_ptr<IState> from, std::shared_ptr<IState> to, TransitionCondition condition) = 0;
	virtual void SetInitialState(std::shared_ptr<IState> state) = 0;
	virtual void SetFinalState(std::shared_ptr<IState> state) = 0;
	virtual bool Validate() const = 0;
};

#endif // ISTATELINKAGEBUILDER_H